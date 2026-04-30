#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ast.h"
#include "semantics.h"
#include "codegen.h"

// ============================================================================
// GLOBALS
// ============================================================================

static int tmp_counter   = 0;
static int lbl_counter   = 0;
static symbol_table *cur_local = NULL;
static const char  *cur_method = NULL;

static int global_str_idx = 0;
static int emit_str_idx = 0;
static int logical_depth = 0;

// ============================================================================
// HELPERS & MANGLEMENT
// ============================================================================

static const char *llvm_type(const char *juc_type) {
    if (!juc_type)                          return "i32";
    if (strcmp(juc_type, "int")     == 0)  return "i32";
    if (strcmp(juc_type, "double")  == 0)  return "double";
    if (strcmp(juc_type, "boolean") == 0)  return "i1";
    if (strcmp(juc_type, "String")  == 0)  return "i8*";
    if (strcmp(juc_type, "String[]")== 0)  return "i8**"; 
    return "void";
}

static int new_tmp(void) { return tmp_counter++; }
static int new_lbl(void) { return lbl_counter++; }

// Mangle para a declaração de métodos
static void get_mangled_name_decl(char *out, const char *name, node *params_node) {
    strcpy(out, name);
    if (strcmp(name, "main") == 0) return; // O main do C não pode ser mangled!
    
    node *p = params_node ? params_node->child : NULL;
    while (p) {
        node *ty = p->child;
        if (strcmp(ty->type, "Int") == 0) strcat(out, "_i");
        else if (strcmp(ty->type, "Double") == 0) strcat(out, "_d");
        else if (strcmp(ty->type, "Bool") == 0) strcat(out, "_b");
        else if (strcmp(ty->type, "StringArray") == 0) strcat(out, "_sA");
        p = p->next;
    }
}

// Mangle para invocação (Call), usando as anotações perfeitas do Analisador Semântico
static void get_mangled_name_call(char *out, const char *name, const char *annotated_sig) {
    strcpy(out, name);
    if (strcmp(name, "main") == 0) return;
    
    if (annotated_sig && annotated_sig[0] == '(') {
        char sig_copy[512];
        strncpy(sig_copy, annotated_sig + 1, 511);
        char *paren = strchr(sig_copy, ')');
        if(paren) *paren = '\0';
        
        char *tok = strtok(sig_copy, ", ");
        while(tok) {
            if (strcmp(tok, "int") == 0) strcat(out, "_i");
            else if (strcmp(tok, "double") == 0) strcat(out, "_d");
            else if (strcmp(tok, "boolean") == 0) strcat(out, "_b");
            else if (strcmp(tok, "String[]") == 0) strcat(out, "_sA");
            tok = strtok(NULL, ", ");
        }
    }
}

// Resolução de Scopes 100% precisa, mesmo com métodos sobrecarregados
static symbol *find_exact_method_symbol(const char *name, node *params_node) {
    char sig[1024] = "";
    int first = 1;
    node *p = params_node ? params_node->child : NULL;
    while(p) {
        node *ty = p->child;
        const char *tstr = "int";
        if (strcmp(ty->type, "Double") == 0) tstr = "double";
        else if (strcmp(ty->type, "Bool") == 0) tstr = "boolean";
        else if (strcmp(ty->type, "StringArray") == 0) tstr = "String[]";
        
        if(!first) strcat(sig, ",");
        strcat(sig, tstr);
        first = 0;
        p = p->next;
    }
    
    symbol *current = global_table ? global_table->symbols : NULL;
    while(current) {
        if (current->param_types != NULL && strcmp(current->name, name) == 0) {
            const char *sym_sig = current->param_types ? current->param_types : "";
            if (strcmp(sym_sig, sig) == 0) return current;
        }
        current = current->next;
    }
    return NULL;
}

// ============================================================================
// SCOPE LOOKUP
// ============================================================================

static symbol *find_var(const char *name) {
    symbol *s = search_variable_local(cur_local, name);
    if (s) return s;
    return search_variable_local(global_table, name);
}

static int is_global(const char *name) {
    if (!search_variable_local(cur_local, name) &&
         search_variable_local(global_table, name))
        return 1;
    return 0;
}

static int is_param(const char *name) {
    symbol *s = search_variable_local(cur_local, name);
    return (s && s->is_param) ? 1 : 0;
}

// ============================================================================
// FORWARD DECLARATIONS
// ============================================================================

static int         codegen_expr(node *n);
static void        codegen_stmt(node *n, int *returned);
static const char *node_type_node_str(node *tn);

// ============================================================================
// PRE-PASS FOR STRINGS
// ============================================================================

static void collect_and_print_strings(node *n) {
    if (!n) return;
    if (strcmp(n->type, "StrLit") == 0) {
        char *raw = strdup(n->value + 1);
        raw[strlen(raw) - 1] = '\0';
        unsigned char out_bytes[4096];
        int oi = 0;
        for (int i = 0; raw[i]; i++) {
            if (raw[i] == '\\') {
                i++;
                switch (raw[i]) {
                    case 'n':  out_bytes[oi++] = '\n'; break;
                    case 't':  out_bytes[oi++] = '\t'; break;
                    case 'f':  out_bytes[oi++] = '\f'; break;
                    case 'r':  out_bytes[oi++] = '\r'; break;
                    case '\\': out_bytes[oi++] = '\\'; break;
                    case '"':  out_bytes[oi++] = '"'; break;
                    default:   out_bytes[oi++] = raw[i]; break;
                }
            } else {
                out_bytes[oi++] = raw[i];
            }
        }
        out_bytes[oi++] = 0; 
        
        printf("@.global_str_%d = private unnamed_addr constant [%d x i8] c\"", global_str_idx, oi);
        for(int i = 0; i < oi - 1; i++) {
            printf("\\%02X", out_bytes[i]); 
        }
        printf("\\00\"\n");
        
        free(raw);
        global_str_idx++;
    }
    collect_and_print_strings(n->child);
    collect_and_print_strings(n->next);
}

// ============================================================================
// EXPRESSIONS
// ============================================================================

static int codegen_natural(node *n) {
    long long val = 0;
    char clean[128];
    int ci = 0;
    for (int i = 0; n->value[i]; i++)
        if (n->value[i] != '_') clean[ci++] = n->value[i];
    clean[ci] = '\0';
    val = atoll(clean);
    int t = new_tmp();
    printf("  %%%d = add i32 %lld, 0\n", t, val);
    return t;
}

static int codegen_decimal(node *n) {
    char clean[256];
    int ci = 0;
    for (int i = 0; n->value[i]; i++) {
        if (n->value[i] != '_') clean[ci++] = n->value[i];
    }
    clean[ci] = '\0';

    double val = strtod(clean, NULL);
    uint64_t bits;
    memcpy(&bits, &val, sizeof(val));

    int t = new_tmp();
    printf("  %%%d = bitcast i64 %llu to double\n", t, (unsigned long long)bits);
    return t;
}

static int codegen_boollit(node *n) {
    int t = new_tmp();
    int val = (strcmp(n->value, "true") == 0) ? 1 : 0;
    printf("  %%%d = add i1 %d, 0\n", t, val);
    return t;
}

static int codegen_strlit(node *n) {
    char *raw = strdup(n->value + 1);
    raw[strlen(raw) - 1] = '\0';
    int oi = 0;
    for (int i = 0; raw[i]; i++) {
        if (raw[i] == '\\') i++;
        oi++;
    }
    oi++; 
    free(raw);

    int t = new_tmp();
    int idx = emit_str_idx++;
    printf("  %%%d = getelementptr inbounds [%d x i8], [%d x i8]* @.global_str_%d, i32 0, i32 0\n", t, oi, oi, idx);
    return t;
}

static int codegen_identifier(node *n) {
    const char *name = n->value;
    const char *ty   = llvm_type(n->anotated_type);
    int t = new_tmp();

    if (is_global(name)) {
        printf("  %%%d = load %s, %s* @%s\n", t, ty, ty, name);
    } else if (is_param(name)) {
        printf("  %%%d = load %s, %s* %%%s.addr\n", t, ty, ty, name);
    } else {
        printf("  %%%d = load %s, %s* %%%s\n", t, ty, ty, name);
    }
    return t;
}

static int codegen_binop(node *n) {
    node *left  = n->child;
    node *right = left->next;
    int lt = codegen_expr(left);
    int rt = codegen_expr(right);
    const char *lty = left->anotated_type  ? left->anotated_type  : "int";
    const char *rty = right->anotated_type ? right->anotated_type : "int";

    const char *nt = n->type;
    int is_arith = (strcmp(nt,"Add")==0||strcmp(nt,"Sub")==0||
                    strcmp(nt,"Mul")==0||strcmp(nt,"Div")==0||
                    strcmp(nt,"Mod")==0||
                    strcmp(nt,"Eq")==0||strcmp(nt,"Ne")==0||
                    strcmp(nt,"Lt")==0||strcmp(nt,"Gt")==0||
                    strcmp(nt,"Le")==0||strcmp(nt,"Ge")==0);
    int is_double_op = (strcmp(lty,"double")==0 || strcmp(rty,"double")==0);
    
    if (is_arith && is_double_op) {
        if (strcmp(lty,"int")==0) {
            int wt = new_tmp();
            printf("  %%%d = sitofp i32 %%%d to double\n", wt, lt);
            lt = wt; lty = "double";
        }
        if (strcmp(rty,"int")==0) {
            int wt = new_tmp();
            printf("  %%%d = sitofp i32 %%%d to double\n", wt, rt);
            rt = wt; rty = "double";
        }
    }
    int t = new_tmp();
    (void)rty;

    if (strcmp(nt, "Add") == 0) {
        if (strcmp(lty,"double")==0 || (right->anotated_type && strcmp(right->anotated_type,"double")==0))
            printf("  %%%d = fadd double %%%d, %%%d\n", t, lt, rt);
        else printf("  %%%d = add i32 %%%d, %%%d\n", t, lt, rt);
    } else if (strcmp(nt, "Sub") == 0) {
        if (strcmp(lty,"double")==0 || (right->anotated_type && strcmp(right->anotated_type,"double")==0))
            printf("  %%%d = fsub double %%%d, %%%d\n", t, lt, rt);
        else printf("  %%%d = sub i32 %%%d, %%%d\n", t, lt, rt);
    } else if (strcmp(nt, "Mul") == 0) {
        if (strcmp(lty,"double")==0 || (right->anotated_type && strcmp(right->anotated_type,"double")==0))
            printf("  %%%d = fmul double %%%d, %%%d\n", t, lt, rt);
        else printf("  %%%d = mul i32 %%%d, %%%d\n", t, lt, rt);
    } else if (strcmp(nt, "Div") == 0) {
        if (strcmp(lty,"double")==0 || (right->anotated_type && strcmp(right->anotated_type,"double")==0))
            printf("  %%%d = fdiv double %%%d, %%%d\n", t, lt, rt);
        else printf("  %%%d = sdiv i32 %%%d, %%%d\n", t, lt, rt);
    } else if (strcmp(nt, "Mod") == 0) {
        if (strcmp(lty,"double")==0 || (right->anotated_type && strcmp(right->anotated_type,"double")==0))
            printf("  %%%d = frem double %%%d, %%%d\n", t, lt, rt); 
        else 
            printf("  %%%d = srem i32 %%%d, %%%d\n", t, lt, rt);
    } else if (strcmp(nt, "Xor") == 0) {
        if (strcmp(lty,"boolean")==0)
            printf("  %%%d = xor i1 %%%d, %%%d\n", t, lt, rt);
        else
            printf("  %%%d = xor i32 %%%d, %%%d\n", t, lt, rt);
    } else if (strcmp(nt, "Lshift") == 0) {
        printf("  %%%d = shl i32 %%%d, %%%d\n", t, lt, rt);
    } else if (strcmp(nt, "Rshift") == 0) {
        printf("  %%%d = ashr i32 %%%d, %%%d\n", t, lt, rt);
    } else if (strcmp(nt, "Eq") == 0) {
        if (strcmp(lty,"double")==0 || (right->anotated_type && strcmp(right->anotated_type,"double")==0))
            printf("  %%%d = fcmp oeq double %%%d, %%%d\n", t, lt, rt);
        else if (strcmp(lty,"boolean")==0)
            printf("  %%%d = icmp eq i1 %%%d, %%%d\n", t, lt, rt);
        else
            printf("  %%%d = icmp eq i32 %%%d, %%%d\n", t, lt, rt);
    } else if (strcmp(nt, "Ne") == 0) {
        if (strcmp(lty,"double")==0 || (right->anotated_type && strcmp(right->anotated_type,"double")==0))
            printf("  %%%d = fcmp one double %%%d, %%%d\n", t, lt, rt);
        else if (strcmp(lty,"boolean")==0)
            printf("  %%%d = icmp ne i1 %%%d, %%%d\n", t, lt, rt);
        else
            printf("  %%%d = icmp ne i32 %%%d, %%%d\n", t, lt, rt);
    } else if (strcmp(nt, "Lt") == 0) {
        if (strcmp(lty,"double")==0 || (right->anotated_type && strcmp(right->anotated_type,"double")==0))
            printf("  %%%d = fcmp olt double %%%d, %%%d\n", t, lt, rt);
        else printf("  %%%d = icmp slt i32 %%%d, %%%d\n", t, lt, rt);
    } else if (strcmp(nt, "Gt") == 0) {
        if (strcmp(lty,"double")==0 || (right->anotated_type && strcmp(right->anotated_type,"double")==0))
            printf("  %%%d = fcmp ogt double %%%d, %%%d\n", t, lt, rt);
        else printf("  %%%d = icmp sgt i32 %%%d, %%%d\n", t, lt, rt);
    } else if (strcmp(nt, "Le") == 0) {
        if (strcmp(lty,"double")==0 || (right->anotated_type && strcmp(right->anotated_type,"double")==0))
            printf("  %%%d = fcmp ole double %%%d, %%%d\n", t, lt, rt);
        else printf("  %%%d = icmp sle i32 %%%d, %%%d\n", t, lt, rt);
    } else if (strcmp(nt, "Ge") == 0) {
        if (strcmp(lty,"double")==0 || (right->anotated_type && strcmp(right->anotated_type,"double")==0))
            printf("  %%%d = fcmp oge double %%%d, %%%d\n", t, lt, rt);
        else printf("  %%%d = icmp sge i32 %%%d, %%%d\n", t, lt, rt);
    }
    return t;
}

static int codegen_unary(node *n) {
    node *child = n->child;
    int ct = codegen_expr(child);
    const char *ty = child->anotated_type ? child->anotated_type : "int";

    if (strcmp(n->type, "Plus") == 0) {
        return ct;
    }

    int t  = new_tmp();
    if (strcmp(n->type, "Not") == 0) {
        printf("  %%%d = xor i1 %%%d, true\n", t, ct);
    } else if (strcmp(n->type, "Minus") == 0) {
        if (strcmp(ty,"double")==0)
            printf("  %%%d = fsub double -0.000000e+00, %%%d\n", t, ct);
        else
            printf("  %%%d = sub i32 0, %%%d\n", t, ct);
    }
    return t;
}

static int codegen_call_expr(node *n) {
    node *id_node = n->child;
    const char *name = id_node->value;

    char mangled_name[256];
    get_mangled_name_call(mangled_name, name, id_node->anotated_type);

    const char *ret_type = n->anotated_type ? n->anotated_type : "void";
    const char *llret    = llvm_type(ret_type);

    char args[4096];
    args[0] = '\0';
    int first_arg = 1;
    node *arg = id_node->next;
    
    char sig_copy[512] = "";
    if (id_node->anotated_type && id_node->anotated_type[0] == '(') {
        strncpy(sig_copy, id_node->anotated_type + 1, 511);
        char *paren = strchr(sig_copy, ')');
        if(paren) *paren = '\0';
    }

    char *tok = strtok(sig_copy, ", ");
    
    while (arg) {
        int at = codegen_expr(arg);
        char buf[128];
        const char *aty = arg->anotated_type ? arg->anotated_type : "int";

        if (tok && strcmp(tok, "double") == 0 && strcmp(aty, "int") == 0) {
            int wt = new_tmp();
            printf("  %%%d = sitofp i32 %%%d to double\n", wt, at);
            at = wt;
            aty = "double";
        }

        snprintf(buf, sizeof(buf), "%s %%%d", llvm_type(aty), at);
        if (!first_arg) strcat(args, ", ");
        strcat(args, buf);
        first_arg = 0;
        arg = arg->next;
        if (tok) tok = strtok(NULL, ", ");
    }

    int t = new_tmp();
    if (strcmp(ret_type, "void") == 0 || strcmp(ret_type, "undef") == 0) {
        printf("  call void @_%s(%s)\n", mangled_name, args);
        return t; 
    } else {
        printf("  %%%d = call %s @_%s(%s)\n", t, llret, mangled_name, args);
        return t;
    }
}

static int codegen_assign_expr(node *n) {
    node *id_node  = n->child;
    node *rhs_node = id_node->next;
    int rt = codegen_expr(rhs_node);

    const char *name = id_node->value;
    const char *ty   = llvm_type(id_node->anotated_type);
    const char *rty  = rhs_node->anotated_type ? rhs_node->anotated_type : "int";

    if (strcmp(id_node->anotated_type ? id_node->anotated_type : "", "double") == 0
        && strcmp(rty, "int") == 0) {
        int wt = new_tmp();
        printf("  %%%d = sitofp i32 %%%d to double\n", wt, rt);
        rt = wt;
    }

    if (is_global(name)) {
        printf("  store %s %%%d, %s* @%s\n", ty, rt, ty, name);
    } else if (is_param(name)) {
        printf("  store %s %%%d, %s* %%%s.addr\n", ty, rt, ty, name);
    } else {
        printf("  store %s %%%d, %s* %%%s\n", ty, rt, ty, name);
    }

    // Retorna o registo original para ser à prova de bala na atribuição
    return rt;
}

static int codegen_parseargs(node *n) {
    node *arr_id  = n->child;
    node *idx_expr = arr_id->next;

    int arr_ptr = codegen_expr(arr_id); 
    int idx_t  = codegen_expr(idx_expr);
    
    int idx1_t = new_tmp();
    int ptr_t  = new_tmp();
    int str_t  = new_tmp();
    int res_t  = new_tmp();

    printf("  %%%d = add i32 %%%d, 1\n", idx1_t, idx_t);
    printf("  %%%d = getelementptr i8*, i8** %%%d, i32 %%%d\n", ptr_t, arr_ptr, idx1_t);
    printf("  %%%d = load i8*, i8** %%%d\n", str_t, ptr_t);
    printf("  %%%d = call i32 @atoi(i8* %%%d)\n", res_t, str_t);
    return res_t;
}

static int codegen_length(node *n) {
    int t = new_tmp();
    printf("  %%%d = load i32, i32* @global_argc\n", t);
    return t;
}

// ============================================================================
// SHORT-CIRCUIT
// ============================================================================

static int codegen_logical(node *n) {
    int depth = logical_depth++; 
    
    node *left = n->child;
    node *right = left->next;

    int res_ptr = new_tmp();
    printf("  %%%d = getelementptr inbounds [100 x i1], [100 x i1]* %%logical_stack, i32 0, i32 %d\n", res_ptr, depth);

    int lt = codegen_expr(left);
    printf("  store i1 %%%d, i1* %%%d\n", lt, res_ptr);

    int eval_right_lbl = new_lbl();
    int end_lbl = new_lbl();

    if (strcmp(n->type, "And") == 0) {
        printf("  br i1 %%%d, label %%.L%d, label %%.L%d\n", lt, eval_right_lbl, end_lbl);
    } else {
        printf("  br i1 %%%d, label %%.L%d, label %%.L%d\n", lt, end_lbl, eval_right_lbl);
    }

    printf(".L%d:\n", eval_right_lbl);
    int rt = codegen_expr(right);
    printf("  store i1 %%%d, i1* %%%d\n", rt, res_ptr);
    printf("  br label %%.L%d\n", end_lbl);

    printf(".L%d:\n", end_lbl);
    int final_res = new_tmp();
    printf("  %%%d = load i1, i1* %%%d\n", final_res, res_ptr);

    logical_depth--; 
    return final_res;
}

static int codegen_expr(node *n) {
    if (!n) return 0;
    const char *nt = n->type;

    if (strcmp(nt, "Natural")    == 0) return codegen_natural(n);
    if (strcmp(nt, "Decimal")    == 0) return codegen_decimal(n);
    if (strcmp(nt, "BoolLit")    == 0) return codegen_boollit(n);
    if (strcmp(nt, "StrLit")     == 0) return codegen_strlit(n);
    if (strcmp(nt, "Identifier") == 0) return codegen_identifier(n);
    if (strcmp(nt, "Call")       == 0) return codegen_call_expr(n);
    if (strcmp(nt, "Assign")     == 0) return codegen_assign_expr(n);
    if (strcmp(nt, "ParseArgs")  == 0) return codegen_parseargs(n);
    if (strcmp(nt, "Length")     == 0) return codegen_length(n);

    if (strcmp(nt,"Not")==0 || strcmp(nt,"Minus")==0 || strcmp(nt,"Plus")==0)
        return codegen_unary(n);

    if (strcmp(nt, "And") == 0 || strcmp(nt, "Or") == 0)
        return codegen_logical(n);

    return codegen_binop(n);
}

// ============================================================================
// STATEMENTS
// ============================================================================

static void codegen_print_stmt(node *n) {
    node *arg = n->child;
    const char *ty = arg->anotated_type ? arg->anotated_type : "int";
    int at = codegen_expr(arg);

    if (strcmp(ty, "int") == 0) {
        int t = new_tmp();
        printf("  %%%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds "
               "([3 x i8], [3 x i8]* @.fmt_int, i32 0, i32 0), i32 %%%d)\n", t, at);
    } else if (strcmp(ty, "double") == 0) {
        int t = new_tmp();
        printf("  %%%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds "
               "([6 x i8], [6 x i8]* @.fmt_double, i32 0, i32 0), double %%%d)\n", t, at);
    } else if (strcmp(ty, "boolean") == 0) {
        int tl = new_lbl(), fl = new_lbl(), el = new_lbl();
        int dummy1 = new_tmp(), dummy2 = new_tmp();
        printf("  br i1 %%%d, label %%.L%d, label %%.L%d\n", at, tl, fl);
        printf(".L%d:\n", tl);
        printf("  %%%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds "
               "([5 x i8], [5 x i8]* @.fmt_true, i32 0, i32 0))\n", dummy1);
        printf("  br label %%.L%d\n", el);
        printf(".L%d:\n", fl);
        printf("  %%%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds "
               "([6 x i8], [6 x i8]* @.fmt_false, i32 0, i32 0))\n", dummy2);
        printf("  br label %%.L%d\n", el);
        printf(".L%d:\n", el);
    } else if (strcmp(ty, "String") == 0) {
        int dt = new_tmp();
        printf("  %%%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds "
               "([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %%%d)\n", dt, at);
    }
}

static void codegen_return_stmt(node *n) {
    node *expr = n->child;
    if (expr) {
        int et = codegen_expr(expr);
        const char *ty = expr->anotated_type ? expr->anotated_type : "void";

        symbol *rs = search_variable_local(cur_local, "return");
        if (rs && strcmp(rs->type, "double") == 0 && strcmp(ty, "int") == 0) {
            int wt = new_tmp();
            printf("  %%%d = sitofp i32 %%%d to double\n", wt, et);
            et = wt;
            ty = "double";
        }

        printf("  store %s %%%d, %s* %%0\n", llvm_type(ty), et, llvm_type(ty));
    }
    printf("  br label %%return\n");
}

static void codegen_if_stmt(node *n, int *returned) {
    node *cond     = n->child;
    node *then_br  = cond->next;
    node *else_br  = then_br ? then_br->next : NULL;

    int ct = codegen_expr(cond);
    int tl = new_lbl(), fl = new_lbl(), ml = new_lbl();

    printf("  br i1 %%%d, label %%.L%d, label %%.L%d\n", ct, tl, fl);

    printf(".L%d:\n", tl);
    int ret_then = 0;
    codegen_stmt(then_br, &ret_then);
    if (!ret_then) printf("  br label %%.L%d\n", ml);

    printf(".L%d:\n", fl);
    int ret_else = 0;
    if (else_br) codegen_stmt(else_br, &ret_else);
    if (!ret_else) printf("  br label %%.L%d\n", ml);

    if (!ret_then || !ret_else)
        printf(".L%d:\n", ml);

    if (ret_then && ret_else)
        *returned = 1;
}

static void codegen_while_stmt(node *n, int *returned) {
    node *cond = n->child;
    node *body = cond->next;

    int cl = new_lbl(), bl = new_lbl(), el = new_lbl();

    printf("  br label %%.L%d\n", cl);
    printf(".L%d:\n", cl);
    int ct = codegen_expr(cond);
    printf("  br i1 %%%d, label %%.L%d, label %%.L%d\n", ct, bl, el);

    printf(".L%d:\n", bl);
    int ret_body = 0;
    codegen_stmt(body, &ret_body);
    if (!ret_body) printf("  br label %%.L%d\n", cl);

    printf(".L%d:\n", el);
    (void)returned;
}

static void codegen_block_stmt(node *n, int *returned) {
    node *child = n->child;
    while (child) {
        if (*returned) break;
        codegen_stmt(child, returned);
        child = child->next;
    }
}

static void codegen_parseargs_stmt(node *n) {
    node *arr_id   = n->child;
    node *idx_expr = arr_id->next;

    int arr_ptr = codegen_expr(arr_id); 
    int idx_t  = codegen_expr(idx_expr);
    int idx1_t = new_tmp();
    int ptr_t  = new_tmp(), str_t = new_tmp(), res_t = new_tmp();

    printf("  %%%d = add i32 %%%d, 1\n", idx1_t, idx_t);
    printf("  %%%d = getelementptr i8*, i8** %%%d, i32 %%%d\n", ptr_t, arr_ptr, idx1_t);
    printf("  %%%d = load i8*, i8** %%%d\n", str_t, ptr_t);
    printf("  %%%d = call i32 @atoi(i8* %%%d)\n", res_t, str_t);
    (void)res_t;
}

static void codegen_stmt(node *n, int *returned) {
    if (!n || *returned) return;
    const char *nt = n->type;

    if (strcmp(nt, "Block") == 0) {
        codegen_block_stmt(n, returned);
    } else if (strcmp(nt, "If") == 0) {
        codegen_if_stmt(n, returned);
    } else if (strcmp(nt, "While") == 0) {
        codegen_while_stmt(n, returned);
    } else if (strcmp(nt, "Return") == 0) {
        codegen_return_stmt(n);
        *returned = 1;
    } else if (strcmp(nt, "Print") == 0) {
        codegen_print_stmt(n);
    } else if (strcmp(nt, "Assign") == 0 ||
               strcmp(nt, "Call")   == 0 ||
               strcmp(nt, "ParseArgs") == 0) {
        codegen_expr(n);
    }
}

// ============================================================================
// VARDECL INSIDE METHOD BODY
// ============================================================================

static void codegen_vardecl(node *n) {
    node *type_node = n->child;
    node *id_node   = type_node->next;
    const char *ty  = llvm_type(node_type_node_str(type_node));
    const char *name = id_node->value;

    if (strcmp(type_node->type, "Bool") == 0) {
        printf("  %%%s = alloca i1\n", name);
        printf("  store i1 false, i1* %%%s\n", name);
    } else if (strcmp(type_node->type, "Double") == 0) {
        printf("  %%%s = alloca double\n", name);
        printf("  store double 0.0, double* %%%s\n", name);
    } else {
        printf("  %%%s = alloca %s\n", name, ty);
        printf("  store %s 0, %s* %%%s\n", ty, ty, name);
    }
}

static const char *node_type_node_str(node *tn) {
    if (!tn) return "int";
    if (strcmp(tn->type, "Int")    == 0) return "int";
    if (strcmp(tn->type, "Bool")   == 0) return "boolean";
    if (strcmp(tn->type, "Double") == 0) return "double";
    return "int";
}

// ============================================================================
// METHOD / FUNCTION
// ============================================================================

static void codegen_method(node *decl) {
    logical_depth = 0; 
    
    node *header = decl->child;
    node *body   = header->next;

    node *type_node   = header->child;
    node *name_node   = type_node->next;
    node *params_node = name_node->next;

    const char *mname = name_node->value;
    const char *rtype_str;

    if (strcmp(type_node->type, "Void") == 0) rtype_str = "void";
    else if (strcmp(type_node->type, "Int") == 0) rtype_str = "int";
    else if (strcmp(type_node->type, "Double") == 0) rtype_str = "double";
    else rtype_str = "boolean";

    symbol *ms = find_exact_method_symbol(mname, params_node);
    cur_local  = ms ? ms->local_table : NULL;
    cur_method = mname;
    tmp_counter = 1;
    lbl_counter = 0;

    char mangled_name[256];
    get_mangled_name_decl(mangled_name, mname, params_node);
    printf("define %s @_%s(", llvm_type(rtype_str), mangled_name);

    if (params_node) {
        node *param = params_node->child;
        int first = 1;
        while (param) {
            node *ptype  = param->child;
            node *pid    = ptype->next;
            const char *pty;
            if (strcmp(ptype->type, "Int") == 0)    pty = "i32";
            else if (strcmp(ptype->type, "Double") == 0) pty = "double";
            else if (strcmp(ptype->type, "Bool") == 0)   pty = "i1";
            else if (strcmp(ptype->type, "StringArray") == 0) pty = "i8**";
            else pty = "i8*";
            if (!first) printf(", ");
            printf("%s %%%s", pty, pid->value);
            first = 0;
            param = param->next;
        }
    }
    printf(") {\n");

    printf(".L%d:\n", lbl_counter++);

    if (strcmp(rtype_str, "void") != 0) {
        printf("  %%0 = alloca %s\n", llvm_type(rtype_str));
    } else {
        printf("  %%0 = alloca i32\n");
    }
    
    printf("  %%logical_stack = alloca [100 x i1]\n");

    if (params_node) {
        node *param = params_node->child;
        while (param) {
            node *ptype = param->child;
            node *pid   = ptype->next;
            const char *pty;
            if (strcmp(ptype->type, "Int") == 0)    pty = "i32";
            else if (strcmp(ptype->type, "Double") == 0) pty = "double";
            else if (strcmp(ptype->type, "Bool") == 0)   pty = "i1";
            else if (strcmp(ptype->type, "StringArray") == 0) pty = "i8**";
            else pty = "i8*";
            printf("  %%%s.addr = alloca %s\n", pid->value, pty);
            printf("  store %s %%%s, %s* %%%s.addr\n", pty, pid->value, pty, pid->value);
            param = param->next;
        }
    }

    int returned = 0;
    if (body) {
        node *child = body->child;
        while (child) {
            if (returned) break;
            if (strcmp(child->type, "VarDecl") == 0) {
                codegen_vardecl(child);
            } else {
                codegen_stmt(child, &returned);
            }
            child = child->next;
        }
    }

    if (!returned) {
        int fl = lbl_counter++;
        printf("  br label %%.L%d\n", fl);
        printf(".L%d:\n", fl);
        printf("  br label %%return\n");
    }

    printf("return:\n");
    if (strcmp(rtype_str, "void") != 0) {
        int lt = tmp_counter++;
        printf("  %%%d = load %s, %s* %%0\n", lt, llvm_type(rtype_str), llvm_type(rtype_str));
        printf("  ret %s %%%d\n", llvm_type(rtype_str), lt);
    } else {
        printf("  ret void\n");
    }
    printf("}\n\n");
}

// ============================================================================
// MAIN
// ============================================================================

static void codegen_main(node *decl) {
    logical_depth = 0; 
    
    node *header = decl->child;
    node *body   = header->next;

    symbol *ms = find_exact_method_symbol("main", header->child->next->next);
    cur_local  = ms ? ms->local_table : NULL;
    cur_method = "main";
    tmp_counter = 1;
    lbl_counter = 0;

    node *params_node = header->child->next->next;
    const char *args_name = "args";
    if (params_node && params_node->child) {
        node *pid = params_node->child->child->next;
        if (pid) args_name = pid->value;
    }

    printf("define i32 @main(i32 %%argc, i8** %%%s) {\n", args_name);
    printf(".L%d:\n", lbl_counter++);
    printf("  %%0 = alloca i32\n");
    
    printf("  %%logical_stack = alloca [100 x i1]\n");

    int at = tmp_counter++;
    printf("  %%%d = sub i32 %%argc, 1\n", at);
    printf("  store i32 %%%d, i32* @global_argc\n", at);

    printf("  %%%s.addr = alloca i8**\n", args_name);
    printf("  store i8** %%%s, i8*** %%%s.addr\n", args_name, args_name);

    int returned = 0;
    if (body) {
        node *child = body->child;
        while (child) {
            if (returned) break;
            if (strcmp(child->type, "VarDecl") == 0) {
                codegen_vardecl(child);
            } else {
                codegen_stmt(child, &returned);
            }
            child = child->next;
        }
    }

    if (!returned) {
        int fl = lbl_counter++;
        printf("  br label %%.L%d\n", fl);
        printf(".L%d:\n", fl);
        printf("  br label %%return\n");
    }
    printf("return:\n");
    printf("  ret i32 0\n");
    printf("}\n\n");
}

// ============================================================================
// GLOBAL VAR DECL
// ============================================================================

static void codegen_global_var(node *n) {
    node *type_node = n->child;
    node *id_node   = type_node->next;
    const char *name = id_node->value;

    if (strcmp(type_node->type, "Int") == 0)
        printf("@%s = global i32 0\n", name);
    else if (strcmp(type_node->type, "Double") == 0)
        printf("@%s = global double 0.0\n", name);
    else if (strcmp(type_node->type, "Bool") == 0)
        printf("@%s = global i1 false\n", name);
}

// ============================================================================
// TOP-LEVEL
// ============================================================================

void codegen_program(node *program) {
    if (!program) return;

    printf("declare i32 @printf(i8*, ...)\n");
    printf("declare i32 @atoi(i8*)\n");
    printf("@.fmt_int    = private unnamed_addr constant [3 x i8]  c\"%%d\\00\"\n");
    printf("@.fmt_double = private unnamed_addr constant [6 x i8]  c\"%%.16e\\00\"\n");
    printf("@.fmt_true   = private unnamed_addr constant [5 x i8]  c\"true\\00\"\n");
    printf("@.fmt_false  = private unnamed_addr constant [6 x i8]  c\"false\\00\"\n");
    printf("@.fmt_str    = private unnamed_addr constant [3 x i8]  c\"%%s\\00\"\n");
    printf("@global_argc = global i32 0\n");
    printf("\n");

    collect_and_print_strings(program);
    printf("\n");

    node *member = program->child->next;
    while (member) {
        if (strcmp(member->type, "FieldDecl") == 0)
            codegen_global_var(member);
        member = member->next;
    }
    printf("\n");

    member = program->child->next;
    while (member) {
        if (strcmp(member->type, "MethodDecl") == 0) {
            node *hdr      = member->child;
            node *name_nd  = hdr->child->next;
            if (strcmp(name_nd->value, "main") != 0) {
                codegen_method(member);
            }
        }
        member = member->next;
    }

    member = program->child->next;
    while (member) {
        if (strcmp(member->type, "MethodDecl") == 0) {
            node *hdr     = member->child;
            node *name_nd = hdr->child->next;
            if (strcmp(name_nd->value, "main") == 0) {
                codegen_main(member);
                break;
            }
        }
        member = member->next;
    }
}