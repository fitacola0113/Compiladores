#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.h"

// Variáveis globais
symbol_table *global_table = NULL;
int semantic_errors = 0;
symbol_table *last_table = NULL;

// ============================================================================
// GESTÃO DAS TABELAS DE SÍMBOLOS
// ============================================================================

symbol_table *create_symbol_table(char *table_name) {
    symbol_table *new_table = (symbol_table *)malloc(sizeof(symbol_table));
    new_table->table_name = strdup(table_name);
    new_table->symbols = NULL;
    new_table->next = NULL;

    if (global_table == NULL) {
        global_table = new_table;
        last_table = new_table;
    } else {
        last_table->next = new_table;
        last_table = new_table;
    }
    return new_table;
}

symbol *insert_symbol(symbol_table *table, const char *name, const char *type, const char *param_types, bool is_param) {
    if (table == NULL || name == NULL || type == NULL) return NULL;

    symbol *new_symbol = (symbol *)malloc(sizeof(symbol));
    new_symbol->name = strdup(name);
    new_symbol->type = strdup(type);
    new_symbol->param_types = param_types ? strdup(param_types) : NULL;
    new_symbol->local_table = NULL;
    new_symbol->is_param = is_param;
    new_symbol->next = NULL;

    if (table->symbols == NULL) {
        table->symbols = new_symbol;
    } else {
        symbol *current = table->symbols;
        while (current->next != NULL) current = current->next;
        current->next = new_symbol;
    }
    return new_symbol;
}

// ============================================================================
// PESQUISAS INTELIGENTES
// ============================================================================

symbol *search_variable_local(symbol_table *table, const char *name) {
    if (!table) return NULL;
    symbol *current = table->symbols;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && current->param_types == NULL) return current;
        current = current->next;
    }
    return NULL;
}

symbol *search_method_local(symbol_table *table, const char *name, const char *param_types) {
    if (!table) return NULL;
    symbol *current = table->symbols;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && current->param_types != NULL) {
            if (strcmp(current->param_types, param_types) == 0) return current;
        }
        current = current->next;
    }
    return NULL;
}

symbol *search_variable_global(symbol_table *local_table, const char *name) {
    symbol *sym = search_variable_local(local_table, name);
    if (sym != NULL) return sym;
    return search_variable_local(global_table, name);
}

symbol *search_method_global(const char *name) {
    if (!global_table) return NULL;
    symbol *current = global_table->symbols;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && current->param_types != NULL) return current;
        current = current->next;
    }
    return NULL;
}

// ============================================================================
// IMPRESSÃO E LIBERTAÇÃO
// ============================================================================

void print_symbol_tables() {
    symbol_table *current_table = global_table;
    while (current_table != NULL) {
        printf("===== %s =====\n", current_table->table_name);
        symbol *current_symbol = current_table->symbols;
        while (current_symbol != NULL) {
            
            if (current_symbol->param_types != NULL) {
                printf("%s\t(%s)\t%s", current_symbol->name, current_symbol->param_types, current_symbol->type);
            } 
            
            else {
                printf("%s\t\t%s", current_symbol->name, current_symbol->type);
            }
            
            // PARAM: 1 tab antes
            if (current_symbol->is_param) {
                printf("\tparam");
            }
            
            printf("\n");
            current_symbol = current_symbol->next;
        }
        if (current_table->next != NULL) printf("\n");
        current_table = current_table->next;
    }
}
void free_symbol_tables() {
    symbol_table *table = global_table;
    while (table != NULL) {
        symbol_table *next_table = table->next;
        symbol *sym = table->symbols;
        while (sym != NULL) {
            symbol *next_sym = sym->next;
            free(sym->name);
            free(sym->type);
            if (sym->param_types) free(sym->param_types);
            free(sym);
            sym = next_sym;
        }
        free(table->table_name);
        free(table);
        table = next_table;
    }
    global_table = NULL;
    last_table = NULL;
}

// ============================================================================
// CONSTRUÇÃO DA TABELA
// ============================================================================

static const char *node_type_to_str(node *type_node) {
    if (type_node == NULL) return "undef";
    if (strcmp(type_node->type, "Int") == 0)         return "int";
    if (strcmp(type_node->type, "Bool") == 0)        return "boolean";
    if (strcmp(type_node->type, "Double") == 0)      return "double";
    if (strcmp(type_node->type, "Void") == 0)        return "void";
    if (strcmp(type_node->type, "StringArray") == 0) return "String[]";
    return "undef";
}

static char *build_param_types_string(node *method_params_node) {
    if (method_params_node == NULL) return strdup("");
    char buf[16000];
    buf[0] = '\0';
    int first = 1;

    node *param = method_params_node->child;
    while (param != NULL) {
        node *type_node = param->child;
        const char *type_str = node_type_to_str(type_node);
        if (!first) strncat(buf, ",", sizeof(buf) - strlen(buf) - 1);
        strncat(buf, type_str, sizeof(buf) - strlen(buf) - 1);
        first = 0;
        param = param->next;
    }
    return strdup(buf);
}

static void process_method_decl(node *method_decl, symbol_table *class_table) {
    if (method_decl == NULL) return;

    node *method_header = method_decl->child;
    if (method_header == NULL) return;

    node *return_type_node = method_header->child;
    node *name_node = return_type_node ? return_type_node->next : NULL;
    if (name_node == NULL) return;
    node *method_params_node = name_node->next;

    const char *return_type = node_type_to_str(return_type_node);
    const char *method_name = name_node->value;
    char *param_types_str = build_param_types_string(method_params_node);

    symbol *existing = search_method_local(class_table, method_name, param_types_str);
    if (existing != NULL) {
        printf("Line %d, col %d: Symbol %s already defined\n", name_node->line, name_node->col, method_name);
        semantic_errors++;
        free(param_types_str);
        return;
    }

    symbol *method_sym = insert_symbol(class_table, method_name, return_type, param_types_str, false);

    char table_name_buf[16000];
    snprintf(table_name_buf, sizeof(table_name_buf), "Method %s(%s) Symbol Table", method_name, param_types_str);
    free(param_types_str);

    symbol_table *local_table = create_symbol_table(table_name_buf);
    method_sym->local_table = local_table;

    insert_symbol(local_table, "return", return_type, NULL, false);

    if (method_params_node != NULL) {
        node *param = method_params_node->child;
        while (param != NULL) {
            node *param_type_node = param->child;
            node *param_name_node = param_type_node ? param_type_node->next : NULL;
            if (param_type_node != NULL && param_name_node != NULL) {
                const char *param_type = node_type_to_str(param_type_node);
                const char *param_name = param_name_node->value;
                if (search_variable_local(local_table, param_name) != NULL) {
                    printf("Line %d, col %d: Symbol %s already defined\n", param_name_node->line, param_name_node->col, param_name);
                    semantic_errors++;
                } else {
                    insert_symbol(local_table, param_name, param_type, NULL, true);
                }
            }
            param = param->next;
        }
    }
    // NOTA IMPORTANTE: Já não adicionamos as variáveis locais aqui! 
    // Vão ser adicionadas na annotate_tree para respeitar a ordem do código.
}

static void process_field_decl(node *field_decl, symbol_table *class_table) {
    if (field_decl == NULL) return;

    node *type_node = field_decl->child;
    node *name_node = type_node ? type_node->next : NULL;
    if (type_node == NULL || name_node == NULL) return;

    const char *field_type = node_type_to_str(type_node);
    const char *field_name = name_node->value;

    if (search_variable_local(class_table, field_name) != NULL) {
        printf("Line %d, col %d: Symbol %s already defined\n", name_node->line, name_node->col, field_name);
        semantic_errors++;
        return;
    }
    insert_symbol(class_table, field_name, field_type, NULL, false);
}

// ============================================================================
// FUNÇÕES AUXILIARES PARA ERROS E RESOLUÇÃO
// ============================================================================

bool is_out_of_bounds(const char *val) {
    if (val == NULL) return false;
    char digits[32];
    int di = 0;
    for (int i = 0; val[i] && di < 31; i++) {
        if (val[i] != '_') digits[di++] = val[i];
    }
    digits[di] = '\0';
    if (di > 10) return true;
    if (di == 10 && strcmp(digits, "2147483648") >= 0) return true;
    return false;
}

const char* get_op_symbol(const char* node_type) {
    if (strcmp(node_type, "Add") == 0 || strcmp(node_type, "Plus") == 0) return "+";
    if (strcmp(node_type, "Sub") == 0 || strcmp(node_type, "Minus") == 0) return "-";
    if (strcmp(node_type, "Mul") == 0) return "*";
    if (strcmp(node_type, "Div") == 0) return "/";
    if (strcmp(node_type, "Mod") == 0) return "%";
    if (strcmp(node_type, "And") == 0) return "&&";
    if (strcmp(node_type, "Or") == 0) return "||";
    if (strcmp(node_type, "Not") == 0) return "!";
    if (strcmp(node_type, "Eq") == 0) return "==";
    if (strcmp(node_type, "Ne") == 0) return "!=";
    if (strcmp(node_type, "Lt") == 0) return "<";
    if (strcmp(node_type, "Gt") == 0) return ">";
    if (strcmp(node_type, "Le") == 0) return "<=";
    if (strcmp(node_type, "Ge") == 0) return ">=";
    if (strcmp(node_type, "Xor") == 0) return "^";
    if (strcmp(node_type, "Lshift") == 0) return "<<";
    if (strcmp(node_type, "Rshift") == 0) return ">>";
    if (strcmp(node_type, "Assign") == 0) return "=";
    return node_type;
}

#define SAFE_TYPE(node_ptr) ((node_ptr) && (node_ptr)->anotated_type ? (node_ptr)->anotated_type : "undef")

static symbol_table *find_local_table_for_method(node *method_decl) {
    if (!method_decl) return NULL;
    node *header = method_decl->child;
    if (!header) return NULL;
    node *name_node = header->child->next;
    if (!name_node) return NULL;
    node *params_node = name_node->next;

    char *param_types = build_param_types_string(params_node);
    char table_name[16000];
    snprintf(table_name, sizeof(table_name), "Method %s(%s) Symbol Table", name_node->value, param_types);
    free(param_types);

    symbol_table *temp = global_table;
    while (temp) {
        if (strcmp(temp->table_name, table_name) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

static void resolve_call(node *n, symbol_table *local_table) {
    if (!n || !n->child) { n->anotated_type = strdup("undef"); return; }

    node *id_node = n->child;
    const char *method_name = id_node->value;

    int nargs = 0;
    node *arg = id_node->next;
    while (arg) { nargs++; arg = arg->next; }

    char arg_types[64][64];
    int ai = 0;
    arg = id_node->next;
    while (arg && ai < 64) {
        const char *t = SAFE_TYPE(arg);
        strncpy(arg_types[ai], t, 63);
        arg_types[ai][63] = '\0';
        ai++;
        arg = arg->next;
    }

    symbol *exact_match = NULL;
    symbol *compat_match = NULL;
    int compat_count = 0;

    symbol *sym = global_table ? global_table->symbols : NULL;
    while (sym) {
        if (sym->param_types == NULL || strcmp(sym->name, method_name) != 0) {
            sym = sym->next;
            continue;
        }

        int nformal = 0;
        char formal_types[64][64];
        
        char copy[16000];
        strncpy(copy, sym->param_types, 15999);
        copy[15999] = '\0';
        char *tok = strtok(copy, ",");
        while (tok && nformal < 64) {
            strncpy(formal_types[nformal], tok, 63);
            formal_types[nformal][63] = '\0';
            nformal++;
            tok = strtok(NULL, ",");
        }

        if (nformal != nargs) { sym = sym->next; continue; }

        bool is_exact = true;
        for (int i = 0; i < nargs; i++) {
            if (strcmp(arg_types[i], formal_types[i]) != 0) { is_exact = false; break; }
        }
        if (is_exact) { exact_match = sym; break; }

        bool is_compat = true;
        for (int i = 0; i < nargs; i++) {
            if (strcmp(arg_types[i], formal_types[i]) == 0) continue;
            if (strcmp(formal_types[i], "double") == 0 && strcmp(arg_types[i], "int") == 0) continue;
            if (strcmp(arg_types[i], "undef") == 0) continue; 
            is_compat = false;
            break;
        }
        if (is_compat) { compat_match = sym; compat_count++; }

        sym = sym->next;
    }

    symbol *chosen = NULL;
    if (exact_match) {
        chosen = exact_match;
    } else if (compat_count == 1) {
        chosen = compat_match;
    } else if (compat_count > 1) {
        printf("Line %d, col %d: Reference to method %s is ambiguous\n", id_node->line, id_node->col, method_name);
        semantic_errors++;
    } else {
        printf("Line %d, col %d: Cannot find symbol %s\n", id_node->line, id_node->col, method_name);
        semantic_errors++;
    }

    if (chosen) {
        char buf[16000];
        snprintf(buf, sizeof(buf), "(%s)", chosen->param_types ? chosen->param_types : "");
        id_node->anotated_type = strdup(buf);
        n->anotated_type = strdup(chosen->type);
    } else {
        id_node->anotated_type = strdup("undef");
        n->anotated_type = strdup("undef");
    }
}

// ============================================================================
// ANOTAÇÃO E PERCURSO DA ÁRVORE (ONDE ACONTECE A MAGIA)
// ============================================================================

void annotate_tree(node *n, symbol_table *local_table) {
    if (!n) return;

    // NÓS A IGNORAR TOTALMENTE
    if (strcmp(n->type, "FieldDecl") == 0 ||
        strcmp(n->type, "MethodHeader") == 0 || strcmp(n->type, "ParamDecl") == 0 ||
        strcmp(n->type, "MethodParams") == 0) {
        annotate_tree(n->next, local_table);
        return;
    }

    // NÓ PROGRAMA
    if (strcmp(n->type, "Program") == 0) {
        if (n->child) {
            annotate_tree(n->child->next, local_table);
        }
        annotate_tree(n->next, local_table);
        return;
    }

    // CONTEXTO DO MÉTODO
    symbol_table *next_table = local_table;
    if (strcmp(n->type, "MethodDecl") == 0) {
        symbol_table *found = find_local_table_for_method(n);
        if (found) next_table = found;
    }

    // --- NOVA LÓGICA DO VARDECL AQUI! ---
    // Adiciona as variáveis localmente à medida que passa por elas.
    if (strcmp(n->type, "VarDecl") == 0) {
        node *var_type_node = n->child;
        node *var_name_node = var_type_node ? var_type_node->next : NULL;
        if (var_type_node != NULL && var_name_node != NULL) {
            const char *var_type = node_type_to_str(var_type_node);
            const char *var_name = var_name_node->value;
            
            if (strcmp(var_name, "_") == 0) {
                printf("Line %d, col %d: Symbol _ is reserved\n", var_name_node->line, var_name_node->col);
                semantic_errors++;
            } else if (search_variable_local(next_table, var_name) != NULL) {
                printf("Line %d, col %d: Symbol %s already defined\n", var_name_node->line, var_name_node->col, var_name);
                semantic_errors++;
            } else {
                insert_symbol(next_table, var_name, var_type, NULL, false);
            }
        }
        annotate_tree(n->next, local_table);
        return;
    }

    // CALL
    if (strcmp(n->type, "Call") == 0) {
        if (n->child && n->child->next) {
            annotate_tree(n->child->next, next_table);
        }
        resolve_call(n, next_table);
        annotate_tree(n->next, local_table);
        return;
    }

    // DESCIDA NORMAL NA ÁRVORE
    annotate_tree(n->child, next_table);

    // -----------------------------------------------------------------------
    // LITERAIS
    // -----------------------------------------------------------------------
    if (strcmp(n->type, "Natural") == 0) {
        if (is_out_of_bounds(n->value)) {
            printf("Line %d, col %d: Number %s out of bounds\n", n->line, n->col, n->value);
            semantic_errors++;
        }
        n->anotated_type = strdup("int");
    }
    else if (strcmp(n->type, "Decimal") == 0) { n->anotated_type = strdup("double"); }
    else if (strcmp(n->type, "BoolLit") == 0) { n->anotated_type = strdup("boolean"); }
    else if (strcmp(n->type, "StrLit") == 0) { n->anotated_type = strdup("String"); }

    // -----------------------------------------------------------------------
    // IDENTIFICADORES
    // -----------------------------------------------------------------------
    else if (strcmp(n->type, "Identifier") == 0) {
        if (!n->value) {
            n->anotated_type = strdup("undef");
        } else if (strcmp(n->value, "_") == 0) {
            printf("Line %d, col %d: Symbol _ is reserved\n", n->line, n->col);
            semantic_errors++;
            n->anotated_type = strdup("undef");
        } else {
            symbol *sym = search_variable_global(local_table, n->value);
            if (sym != NULL) {
                n->anotated_type = strdup(sym->type);
            } else {
                printf("Line %d, col %d: Cannot find symbol %s\n", n->line, n->col, n->value);
                semantic_errors++;
                n->anotated_type = strdup("undef");
            }
        }
    }

    // -----------------------------------------------------------------------
    // LENGTH e PARSEARGS
    // -----------------------------------------------------------------------
    else if (strcmp(n->type, "Length") == 0) {
        if (n->child) {
            const char *t1 = SAFE_TYPE(n->child);
            if (strcmp(t1, "undef") != 0 && strcmp(t1, "String[]") != 0) {
                printf("Line %d, col %d: Operator .length cannot be applied to type %s\n", n->line, n->col, t1);
                semantic_errors++;
            }
        }
        n->anotated_type = strdup("int");
    }
    else if (strcmp(n->type, "ParseArgs") == 0) {
        if (n->child && n->child->next) {
            const char *t1 = SAFE_TYPE(n->child);
            const char *t2 = SAFE_TYPE(n->child->next);
            if (strcmp(t1, "undef") != 0 && strcmp(t2, "undef") != 0) {
                if (strcmp(t1, "String[]") != 0 || strcmp(t2, "int") != 0) {
                    printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n", n->line, n->col, t1, t2);
                    semantic_errors++;
                }
            }
        }
        n->anotated_type = strdup("int");
    }

    // -----------------------------------------------------------------------
    // OPERADORES ARITMÉTICOS
    // -----------------------------------------------------------------------
    else if (strcmp(n->type, "Add") == 0 || strcmp(n->type, "Sub") == 0 ||
             strcmp(n->type, "Mul") == 0 || strcmp(n->type, "Div") == 0 ||
             strcmp(n->type, "Mod") == 0) {
        if (!n->child || !n->child->next) { n->anotated_type = strdup("undef"); }
        else {
            const char *t1 = SAFE_TYPE(n->child);
            const char *t2 = SAFE_TYPE(n->child->next);
            if (strcmp(t1, "undef") == 0 || strcmp(t2, "undef") == 0) {
                n->anotated_type = strdup("undef");
            } else if (strcmp(t1, "int") == 0 && strcmp(t2, "int") == 0) {
                n->anotated_type = strdup("int");
            } else if ((strcmp(t1, "int") == 0 || strcmp(t1, "double") == 0) &&
                       (strcmp(t2, "int") == 0 || strcmp(t2, "double") == 0)) {
                n->anotated_type = strdup("double");
            } else {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", n->line, n->col, get_op_symbol(n->type), t1, t2);
                semantic_errors++;
                n->anotated_type = strdup("undef");
            }
        }
    }

    // -----------------------------------------------------------------------
    // RELACIONAIS Eq, Ne
    // -----------------------------------------------------------------------
    else if (strcmp(n->type, "Eq") == 0 || strcmp(n->type, "Ne") == 0) {
        if (!n->child || !n->child->next) { n->anotated_type = strdup("undef"); }
        else {
            const char *t1 = SAFE_TYPE(n->child);
            const char *t2 = SAFE_TYPE(n->child->next);
            if (strcmp(t1, "undef") == 0 || strcmp(t2, "undef") == 0) {
                n->anotated_type = strdup("undef");
            } else {
                bool valid = false;
                if ((strcmp(t1, "int") == 0 || strcmp(t1, "double") == 0) &&
                    (strcmp(t2, "int") == 0 || strcmp(t2, "double") == 0)) {
                    valid = true;
                } else if (strcmp(t1, "boolean") == 0 && strcmp(t2, "boolean") == 0) {
                    valid = true;
                }
                
                if (!valid) {
                    printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", n->line, n->col, get_op_symbol(n->type), t1, t2);
                    semantic_errors++;
                    n->anotated_type = strdup("undef");
                } else {
                    n->anotated_type = strdup("boolean");
                }
            }
        }
    }

    // -----------------------------------------------------------------------
    // RELACIONAIS Lt, Gt, Le, Ge
    // -----------------------------------------------------------------------
    else if (strcmp(n->type, "Lt") == 0 || strcmp(n->type, "Gt") == 0 ||
             strcmp(n->type, "Le") == 0 || strcmp(n->type, "Ge") == 0) {
        if (!n->child || !n->child->next) { n->anotated_type = strdup("undef"); }
        else {
            const char *t1 = SAFE_TYPE(n->child);
            const char *t2 = SAFE_TYPE(n->child->next);
            if (strcmp(t1, "undef") == 0 || strcmp(t2, "undef") == 0) {
                n->anotated_type = strdup("undef");
            } else if ((strcmp(t1, "int") == 0 || strcmp(t1, "double") == 0) &&
                       (strcmp(t2, "int") == 0 || strcmp(t2, "double") == 0)) {
                n->anotated_type = strdup("boolean");
            } else {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", n->line, n->col, get_op_symbol(n->type), t1, t2);
                semantic_errors++;
                n->anotated_type = strdup("undef");
            }
        }
    }

    // -----------------------------------------------------------------------
    // LÓGICOS And, Or
    // -----------------------------------------------------------------------
    else if (strcmp(n->type, "And") == 0 || strcmp(n->type, "Or") == 0) {
        if (!n->child || !n->child->next) { n->anotated_type = strdup("undef"); }
        else {
            const char *t1 = SAFE_TYPE(n->child);
            const char *t2 = SAFE_TYPE(n->child->next);
            if (strcmp(t1, "undef") == 0 || strcmp(t2, "undef") == 0) {
                n->anotated_type = strdup("undef");
            } else if (strcmp(t1, "boolean") == 0 && strcmp(t2, "boolean") == 0) {
                n->anotated_type = strdup("boolean");
            } else {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", n->line, n->col, get_op_symbol(n->type), t1, t2);
                semantic_errors++;
                n->anotated_type = strdup("undef");
            }
        }
    }

    // -----------------------------------------------------------------------
    // BITWISE Lshift, Rshift
    // -----------------------------------------------------------------------
    else if (strcmp(n->type, "Lshift") == 0 || strcmp(n->type, "Rshift") == 0) {
        if (!n->child || !n->child->next) { n->anotated_type = strdup("undef"); }
        else {
            const char *t1 = SAFE_TYPE(n->child);
            const char *t2 = SAFE_TYPE(n->child->next);
            if (strcmp(t1, "undef") == 0 || strcmp(t2, "undef") == 0) {
                n->anotated_type = strdup("undef");
            } else if (strcmp(t1, "int") == 0 && strcmp(t2, "int") == 0) {
                n->anotated_type = strdup("int");
            } else {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", n->line, n->col, get_op_symbol(n->type), t1, t2);
                semantic_errors++;
                n->anotated_type = strdup("undef");
            }
        }
    }
    
    // -----------------------------------------------------------------------
    // BITWISE / LOGICAL Xor
    // -----------------------------------------------------------------------
    else if (strcmp(n->type, "Xor") == 0) {
        if (!n->child || !n->child->next) { n->anotated_type = strdup("undef"); }
        else {
            const char *t1 = SAFE_TYPE(n->child);
            const char *t2 = SAFE_TYPE(n->child->next);
            if (strcmp(t1, "undef") == 0 || strcmp(t2, "undef") == 0) {
                n->anotated_type = strdup("undef");
            } else if (strcmp(t1, "int") == 0 && strcmp(t2, "int") == 0) {
                n->anotated_type = strdup("int");
            } else if (strcmp(t1, "boolean") == 0 && strcmp(t2, "boolean") == 0) {
                n->anotated_type = strdup("boolean"); 
            } else {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", n->line, n->col, get_op_symbol(n->type), t1, t2);
                semantic_errors++;
                n->anotated_type = strdup("undef");
            }
        }
    }

    // -----------------------------------------------------------------------
    // UNÁRIO Not
    // -----------------------------------------------------------------------
    else if (strcmp(n->type, "Not") == 0) {
        if (!n->child) { n->anotated_type = strdup("undef"); }
        else {
            const char *t1 = SAFE_TYPE(n->child);
            if (strcmp(t1, "undef") == 0) {
                n->anotated_type = strdup("undef");
            } else if (strcmp(t1, "boolean") == 0) {
                n->anotated_type = strdup("boolean");
            } else {
                printf("Line %d, col %d: Operator %s cannot be applied to type %s\n", n->line, n->col, get_op_symbol(n->type), t1);
                semantic_errors++;
                n->anotated_type = strdup("undef");
            }
        }
    }

    // -----------------------------------------------------------------------
    // UNÁRIOS Plus, Minus
    // -----------------------------------------------------------------------
    else if (strcmp(n->type, "Plus") == 0 || strcmp(n->type, "Minus") == 0) {
        if (!n->child) { n->anotated_type = strdup("undef"); }
        else {
            const char *t1 = SAFE_TYPE(n->child);
            if (strcmp(t1, "undef") == 0) {
                n->anotated_type = strdup("undef");
            } else if (strcmp(t1, "int") == 0 || strcmp(t1, "double") == 0) {
                n->anotated_type = strdup(t1);
            } else {
                printf("Line %d, col %d: Operator %s cannot be applied to type %s\n", n->line, n->col, get_op_symbol(n->type), t1);
                semantic_errors++;
                n->anotated_type = strdup("undef");
            }
        }
    }

    // -----------------------------------------------------------------------
    // ASSIGN
    // -----------------------------------------------------------------------
    else if (strcmp(n->type, "Assign") == 0) {
        if (!n->child || !n->child->next) { n->anotated_type = strdup("undef"); }
        else {
            const char *t1 = SAFE_TYPE(n->child);
            const char *t2 = SAFE_TYPE(n->child->next);
            if (strcmp(t1, "undef") == 0 || strcmp(t2, "undef") == 0) {
                n->anotated_type = strdup(strcmp(t1, "undef") != 0 ? t1 : "undef");
            } else {
                bool valid = false;
                if (strcmp(t1, "int") == 0 || strcmp(t1, "double") == 0 || strcmp(t1, "boolean") == 0) {
                    if (strcmp(t1, t2) == 0) valid = true;
                    else if (strcmp(t1, "double") == 0 && strcmp(t2, "int") == 0) valid = true;
                }
                
                if (!valid) {
                    printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", n->line, n->col, get_op_symbol(n->type), t1, t2);
                    semantic_errors++;
                }
                n->anotated_type = strdup(t1);
            }
        }
    }

    // -----------------------------------------------------------------------
    // ESTRUTURAS DE CONTROLO If, While
    // -----------------------------------------------------------------------
    else if (strcmp(n->type, "If") == 0 || strcmp(n->type, "While") == 0) {
        if (n->child && n->child->anotated_type) {
            const char *cond_type = n->child->anotated_type;
            if (strcmp(cond_type, "undef") != 0 && strcmp(cond_type, "boolean") != 0) {
                printf("Line %d, col %d: Incompatible type %s in %s statement\n",
                       n->child->line, n->child->col, cond_type,
                       strcmp(n->type, "If") == 0 ? "if" : "while");
                semantic_errors++;
            }
        }
    }

    // -----------------------------------------------------------------------
    // RETURN
    // -----------------------------------------------------------------------
    else if (strcmp(n->type, "Return") == 0) {
        symbol *ret_sym = search_variable_local(local_table, "return");
        const char *expected = ret_sym ? ret_sym->type : "void";
        node *expr = n->child;
        const char *actual = (expr && expr->anotated_type) ? expr->anotated_type : "void";

        if (strcmp(actual, "undef") != 0) {
            bool ok = (strcmp(expected, actual) == 0) ||
                      (strcmp(expected, "double") == 0 && strcmp(actual, "int") == 0);
            if (!ok) {
                printf("Line %d, col %d: Incompatible type %s in return statement\n",
                       n->line, n->col, actual);
                semantic_errors++;
            }
        }
    }

    // -----------------------------------------------------------------------
    // PRINT
    // -----------------------------------------------------------------------
    else if (strcmp(n->type, "Print") == 0) {
        if (n->child && n->child->anotated_type) {
            const char *ptype = n->child->anotated_type;
            if (strcmp(ptype, "undef") != 0 &&
                strcmp(ptype, "int") != 0 &&
                strcmp(ptype, "double") != 0 &&
                strcmp(ptype, "boolean") != 0 &&
                strcmp(ptype, "String") != 0) {
                printf("Line %d, col %d: Incompatible type %s in System.out.print statement\n",
                       n->child->line, n->child->col, ptype);
                semantic_errors++;
            }
        }
    }

    // Continua para o próximo irmão
    annotate_tree(n->next, local_table);
}

// ============================================================================
// PONTO DE ENTRADA DA ANÁLISE SEMÂNTICA
// ============================================================================

void check_program(node *program_node) {
    if (program_node == NULL) return;

    node *class_name_node = program_node->child;
    if (class_name_node == NULL) return;

    const char *class_name = class_name_node->value;

    char table_name_buf[16000];
    snprintf(table_name_buf, sizeof(table_name_buf), "Class %s Symbol Table", class_name);

    symbol_table *class_table = create_symbol_table(table_name_buf);

    node *member = class_name_node->next;
    while (member != NULL) {
        if (strcmp(member->type, "FieldDecl") == 0) {
            process_field_decl(member, class_table);
        } else if (strcmp(member->type, "MethodDecl") == 0) {
            process_method_decl(member, class_table);
        }
        member = member->next;
    }

    annotate_tree(program_node, class_table);
}