/* Bernardo Mateus 2023211936 Daniel Mendes 2023208585 */

%{
#include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "ast.h"
    #include "semantics.h"

    node* create_node(char* type, char* value, int line, int col) {
        node* n = (node*)malloc(sizeof(node));
        n->type = type;
        n->value = value ? strdup(value) : NULL;
        
        n->anotated_type = NULL; 
        n->line = line;          
        n->col = col;            
        
        n->child = NULL;
        n->next = NULL;
        return n;
    }

    node* append_sibling(node* list, node* new_node) {
        if (!list) return new_node;
        if (!new_node) return list;
        node* tmp = list;
        while (tmp->next) tmp = tmp->next;
        tmp->next = new_node;
        return list;
    }

    void add_child(node* parent, node* new_child) {
        if (!parent || !new_child) return;
        if (!parent->child) parent->child = new_child;
        else {
            node* tmp = parent->child;
            while (tmp->next) tmp = tmp->next;
            tmp->next = new_child;
        }
    }

    void print_tree(node* n, int level) {
        if (!n) return;
        for (int i = 0; i < level; i++) printf("..");

        if (n->value) {
            printf("%s(%s)", n->type, n->value);
        } else {
            printf("%s", n->type);
        }

        // Imprime o tipo anotado se existir (apenas para nós de expressão)
        if (n->anotated_type) {
            printf(" - %s", n->anotated_type);
        }

        printf("\n");

        print_tree(n->child, level + 1);
        print_tree(n->next, level);
    }

    int yylex(void);
    void yyerror(char *s);
    
    extern char str_buffer[1024];
    extern int cur_line;
    extern int start_col;
    extern char* yytext;
    int print_tokens = 0;
    int syntax_error = 0;

    node* root = NULL;
%}

%union { //alterado para receber a linha e a coluna
    struct {
        char* str;
        int line;
        int col;
    } info;
    struct node* node;
}

// TODOS os tokens passam a receber o <info>
%token <info> IDENTIFIER NATURAL DECIMAL STRLIT BOOLLIT RESERVED
%token <info> RETURN PARSEINT DOTLENGTH
%token <info> CLASS PUBLIC STATIC VOID STRING BOOL INT DOUBLE
%token <info> IF WHILE PRINT
%token <info> SEMICOLON COMMA LPAR RPAR LBRACE RBRACE LSQ RSQ ARROW

%right <info> ASSIGN
%left <info> OR
%left <info> AND
%left <info> XOR
%left <info> EQ NE
%left <info> LT GT LE GE
%left <info> LSHIFT RSHIFT
%left <info> PLUS MINUS
%left <info> STAR DIV MOD
%right <info> NOT UNARY_PLUS UNARY_MINUS
%nonassoc LOWER_THAN_ELSE
%nonassoc <info> ELSE

%type <node> Program ProgramBody MethodDecl MethodHeader MethodBody Expr Statement StatementList
%type <node> Type MethodInvocation Assignment ParseArgs VarDecl FieldDecl FormalParams FormalParamsList MethodBodyContent IdList ExprList OpExpr

%%

Program: CLASS IDENTIFIER LBRACE ProgramBody RBRACE { 
    $$ = create_node("Program", NULL, $1.line, $1.col);
    add_child($$, create_node("Identifier", $2.str, $2.line, $2.col));
    if ($4) add_child($$, $4);
    root = $$;
} ;

ProgramBody: /* vazio */ { $$ = NULL; }
           | ProgramBody MethodDecl { $$ = append_sibling($1, $2); }
           | ProgramBody FieldDecl { $$ = append_sibling($1, $2); }
           | ProgramBody SEMICOLON { $$ = $1; }
           ;

MethodDecl: PUBLIC STATIC MethodHeader MethodBody { 
    $$ = create_node("MethodDecl", NULL, $1.line, $1.col);
    add_child($$, $3);
    add_child($$, $4);
} ;

MethodHeader: Type IDENTIFIER LPAR FormalParams RPAR { 
                $$ = create_node("MethodHeader", NULL, $1->line, $1->col);
                add_child($$, $1);
                add_child($$, create_node("Identifier", $2.str, $2.line, $2.col));
                add_child($$, $4);
            }
            | VOID IDENTIFIER LPAR FormalParams RPAR { 
                $$ = create_node("MethodHeader", NULL, $1.line, $1.col);
                add_child($$, create_node("Void", NULL, $1.line, $1.col));
                add_child($$, create_node("Identifier", $2.str, $2.line, $2.col));
                add_child($$, $4);
            }
            | Type IDENTIFIER LPAR RPAR { 
                $$ = create_node("MethodHeader", NULL, $1->line, $1->col);
                add_child($$, $1);
                add_child($$, create_node("Identifier", $2.str, $2.line, $2.col));
                add_child($$, create_node("MethodParams", NULL, $3.line, $3.col));
            }
            | VOID IDENTIFIER LPAR RPAR { 
                $$ = create_node("MethodHeader", NULL, $1.line, $1.col);
                add_child($$, create_node("Void", NULL, $1.line, $1.col));
                add_child($$, create_node("Identifier", $2.str, $2.line, $2.col));
                add_child($$, create_node("MethodParams", NULL, $3.line, $3.col));
            }
            ;

FormalParams: Type IDENTIFIER FormalParamsList { 
                $$ = create_node("MethodParams", NULL, $1->line, $1->col);
                node* param = create_node("ParamDecl", NULL, $1->line, $1->col);
                add_child(param, $1);
                add_child(param, create_node("Identifier", $2.str, $2.line, $2.col));
                add_child($$, param);
                if($3) add_child($$, $3);
            }
            | STRING LSQ RSQ IDENTIFIER { 
                $$ = create_node("MethodParams", NULL, $1.line, $1.col);
                node* param = create_node("ParamDecl", NULL, $1.line, $1.col);
                add_child(param, create_node("StringArray", NULL, $1.line, $1.col));
                add_child(param, create_node("Identifier", $4.str, $4.line, $4.col));
                add_child($$, param);
            }
            ;

FormalParamsList: /* vazio */ { $$ = NULL; }
                | FormalParamsList COMMA Type IDENTIFIER { 
                    node* param = create_node("ParamDecl", NULL, $3->line, $3->col);
                    add_child(param, $3);
                    add_child(param, create_node("Identifier", $4.str, $4.line, $4.col));
                    $$ = append_sibling($1, param);
                }
                ;

MethodBody: LBRACE MethodBodyContent RBRACE { 
    $$ = create_node("MethodBody", NULL, $1.line, $1.col);
    if ($2) add_child($$, $2);
} ;

MethodBodyContent: /* vazio */ { $$ = NULL; }
                 | MethodBodyContent Statement { $$ = append_sibling($1, $2); }
                 | MethodBodyContent VarDecl { $$ = append_sibling($1, $2); }
                 ;

FieldDecl: PUBLIC STATIC Type IDENTIFIER IdList SEMICOLON { 
             node* first = create_node("FieldDecl", NULL, $3->line, $3->col);
             add_child(first, $3);
             add_child(first, create_node("Identifier", $4.str, $4.line, $4.col));
             $$ = first;
             node* aux = $5;
             node* curr = first;
             while(aux) {
                 node* next_decl = create_node("FieldDecl", NULL, $3->line, $3->col);
                 add_child(next_decl, create_node($3->type, NULL, $3->line, $3->col));
                 node* next_id = aux->next;
                 aux->next = NULL;
                 add_child(next_decl, aux);
                 curr->next = next_decl;
                 curr = next_decl;
                 aux = next_id;
             }
         }
         | error SEMICOLON { $$ = NULL; }
         ;

VarDecl: Type IDENTIFIER IdList SEMICOLON { 
            node* first = create_node("VarDecl", NULL, $1->line, $1->col);
            add_child(first, $1);
            add_child(first, create_node("Identifier", $2.str, $2.line, $2.col));
            $$ = first;
            node* aux = $3;
            node* curr = first;
            while(aux) {
                node* next_decl = create_node("VarDecl", NULL, $1->line, $1->col);
                add_child(next_decl, create_node($1->type, NULL, $1->line, $1->col)); 
                node* next_id = aux->next;
                aux->next = NULL;
                add_child(next_decl, aux);
                curr->next = next_decl;
                curr = next_decl;
                aux = next_id;
            }
         } ;

IdList: /* vazio */ { $$ = NULL; }
      | IdList COMMA IDENTIFIER { $$ = append_sibling($1, create_node("Identifier", $3.str, $3.line, $3.col)); }
      ;

Type: BOOL { $$ = create_node("Bool", NULL, $1.line, $1.col); } 
    | INT { $$ = create_node("Int", NULL, $1.line, $1.col); } 
    | DOUBLE { $$ = create_node("Double", NULL, $1.line, $1.col); } 
    ;

Statement: LBRACE StatementList RBRACE { 
            int count = 0;
            node* aux = $2;
            while(aux) { count++; aux = aux->next; }
            if (count > 1) {
                $$ = create_node("Block", NULL, $1.line, $1.col);
                add_child($$, $2);
            } else {
                $$ = $2;
            }
         }
         | IF LPAR Expr RPAR Statement %prec LOWER_THAN_ELSE { 
            $$ = create_node("If", NULL, $1.line, $1.col);
            add_child($$, $3);
            add_child($$, $5 ? $5 : create_node("Block", NULL, $4.line, $4.col));
            add_child($$, create_node("Block", NULL, $4.line, $4.col));
         }
         | IF LPAR Expr RPAR Statement ELSE Statement { 
            $$ = create_node("If", NULL, $1.line, $1.col);
            add_child($$, $3);
            add_child($$, $5 ? $5 : create_node("Block", NULL, $4.line, $4.col));
            add_child($$, $7 ? $7 : create_node("Block", NULL, $6.line, $6.col));
         }
         | WHILE LPAR Expr RPAR Statement { 
            $$ = create_node("While", NULL, $1.line, $1.col);
            add_child($$, $3);
            add_child($$, $5 ? $5 : create_node("Block", NULL, $4.line, $4.col));
         }
         | RETURN SEMICOLON { $$ = create_node("Return", NULL, $1.line, $1.col); }
         | RETURN Expr SEMICOLON { $$ = create_node("Return", NULL, $1.line, $1.col); add_child($$, $2); }
         | MethodInvocation SEMICOLON { $$ = $1; }
         | Assignment SEMICOLON { $$ = $1; }
         | ParseArgs SEMICOLON { $$ = $1; }
         | SEMICOLON { $$ = NULL; }
         | PRINT LPAR Expr RPAR SEMICOLON { $$ = create_node("Print", NULL, $1.line, $1.col); add_child($$, $3); }
         | PRINT LPAR STRLIT RPAR SEMICOLON { $$ = create_node("Print", NULL, $1.line, $1.col); add_child($$, create_node("StrLit", $3.str, $3.line, $3.col)); }
         | error SEMICOLON { $$ = NULL; }
         ;

StatementList: /* vazio */ { $$ = NULL; }
             | StatementList Statement { $$ = append_sibling($1, $2); }
             ;

MethodInvocation: IDENTIFIER LPAR RPAR { 
                    $$ = create_node("Call", NULL, $1.line, $1.col);
                    add_child($$, create_node("Identifier", $1.str, $1.line, $1.col));
                }
                | IDENTIFIER LPAR ExprList RPAR { 
                    $$ = create_node("Call", NULL, $1.line, $1.col);
                    add_child($$, create_node("Identifier", $1.str, $1.line, $1.col));
                    add_child($$, $3);
                }
                | IDENTIFIER LPAR error RPAR { $$ = NULL; }
                ;

ExprList: Expr { $$ = $1; }
        | ExprList COMMA Expr { $$ = append_sibling($1, $3); }
        ;

Assignment: IDENTIFIER ASSIGN Expr { 
    $$ = create_node("Assign", NULL, $2.line, $2.col);
    add_child($$, create_node("Identifier", $1.str, $1.line, $1.col));
    add_child($$, $3);
} ;

ParseArgs: PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR { 
    $$ = create_node("ParseArgs", NULL, $1.line, $1.col);
    add_child($$, create_node("Identifier", $3.str, $3.line, $3.col));
    add_child($$, $5);
}
         | PARSEINT LPAR error RPAR { $$ = NULL; }
         ;

Expr: Assignment { $$ = $1; }
    | OpExpr { $$ = $1; }
    ;

OpExpr: OpExpr PLUS OpExpr { $$ = create_node("Add", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr MINUS OpExpr { $$ = create_node("Sub", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr STAR OpExpr { $$ = create_node("Mul", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr DIV OpExpr { $$ = create_node("Div", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr MOD OpExpr { $$ = create_node("Mod", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr AND OpExpr { $$ = create_node("And", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr OR OpExpr { $$ = create_node("Or", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr XOR OpExpr { $$ = create_node("Xor", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr LSHIFT OpExpr { $$ = create_node("Lshift", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr RSHIFT OpExpr { $$ = create_node("Rshift", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr EQ OpExpr { $$ = create_node("Eq", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr NE OpExpr { $$ = create_node("Ne", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr GE OpExpr { $$ = create_node("Ge", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr GT OpExpr { $$ = create_node("Gt", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr LE OpExpr { $$ = create_node("Le", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | OpExpr LT OpExpr { $$ = create_node("Lt", NULL, $2.line, $2.col); add_child($$, $1); add_child($$, $3); }
    | PLUS OpExpr %prec UNARY_PLUS { $$ = create_node("Plus", NULL, $1.line, $1.col); add_child($$, $2); }
    | MINUS OpExpr %prec UNARY_MINUS { $$ = create_node("Minus", NULL, $1.line, $1.col); add_child($$, $2); }
    | NOT OpExpr { $$ = create_node("Not", NULL, $1.line, $1.col); add_child($$, $2); }
    | LPAR Expr RPAR { $$ = $2; }
    | LPAR error RPAR { $$ = NULL; }
    | MethodInvocation { $$ = $1; }
    | ParseArgs { $$ = $1; }
    | IDENTIFIER { $$ = create_node("Identifier", $1.str, $1.line, $1.col); }
    | IDENTIFIER DOTLENGTH { $$ = create_node("Length", NULL, $2.line, $2.col); add_child($$, create_node("Identifier", $1.str, $1.line, $1.col)); }
    | NATURAL { $$ = create_node("Natural", $1.str, $1.line, $1.col); }
    | DECIMAL { $$ = create_node("Decimal", $1.str, $1.line, $1.col); }
    | BOOLLIT { $$ = create_node("BoolLit", $1.str, $1.line, $1.col); }
    ;

%%

void yyerror(char *s) {
    syntax_error = 1;
    
    if (yytext && yytext[0] == '"' && yytext[1] == '\0') 
    {
        printf("Line %d, col %d: %s: \"%s\"\n", cur_line, start_col, s, str_buffer);
    } else 
    {
        printf("Line %d, col %d: %s: %s\n", cur_line, start_col, s, yytext);
    }
    
    fflush(stdout); 
}

int main(int argc, char *argv[]) {
    if (argc > 1) {

        if (strcmp(argv[1], "-l") == 0) {
            print_tokens = 1;
            while (yylex()) ;
            return 0;
        } else if (strcmp(argv[1], "-e1") == 0) {
            print_tokens = 0;
            while (yylex()) ;
            return 0;
        } else if (strcmp(argv[1], "-t") == 0) {
            yyparse();
            if (root && !syntax_error) print_tree(root, 0);
            return 0;
        } else if (strcmp(argv[1], "-e2") == 0) {
            yyparse();
            return 0;
        } else if (strcmp(argv[1], "-s") == 0) {
            yyparse();
            if (!syntax_error && root) {
                check_program(root);
                print_symbol_tables();
                printf("\n");
                print_tree(root, 0);
            }
            free_symbol_tables();
            return 0;
        } else if (strcmp(argv[1], "-e3") == 0) {
            yyparse();
            if (!syntax_error && root) {
                check_program(root);
            }
            free_symbol_tables();
            return 0;
        }
    }

    // Sem opções: análise semântica mas sem imprimir tabelas nem AST
    yyparse();
    if (!syntax_error && root) {
        check_program(root);
    }
    free_symbol_tables();
    return 0;
}