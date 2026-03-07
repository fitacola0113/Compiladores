/* Bernardo Mateus 2023211936 Daniel Mendes 2023208585 */

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    typedef struct node {
        char *type;
        char *value;
        struct node *child;
        struct node *next;
    } node;

    node* create_node(char* type, char* value) {
        node* n = (node*)malloc(sizeof(node));
        n->type = type;
        n->value = value ? strdup(value) : NULL;
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
        printf("%s(%s)\n", n->type, n->value); // Sem espaços antes do \n
    } else {
        printf("%s\n", n->type); // Sem espaços antes do \n
    }
        
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

%union {
    char* str;
    struct node* node; 
}

%token <str> IDENTIFIER NATURAL DECIMAL STRLIT BOOLLIT RESERVED
%token CLASS PUBLIC STATIC VOID STRING BOOL INT DOUBLE
%token IF WHILE RETURN PRINT PARSEINT
%token DOTLENGTH SEMICOLON COMMA LPAR RPAR LBRACE RBRACE LSQ RSQ
%token ASSIGN ARROW

%right ASSIGN
%left OR
%left AND
%left XOR
%left EQ NE
%left LT GT LE GE
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD
%right NOT UNARY_PLUS UNARY_MINUS
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%type <node> Program ProgramBody MethodDecl MethodHeader MethodBody Expr Statement StatementList
%type <node> Type MethodInvocation Assignment ParseArgs VarDecl FieldDecl FormalParams FormalParamsList MethodBodyContent IdList ExprList

%%

Program: CLASS IDENTIFIER LBRACE ProgramBody RBRACE { 
    $$ = create_node("Program", NULL);
    add_child($$, create_node("Identifier", $2));
    if ($4) add_child($$, $4);
    root = $$;
} ;

ProgramBody: /* vazio */ { $$ = NULL; }
           | ProgramBody MethodDecl { $$ = append_sibling($1, $2); }
           | ProgramBody FieldDecl { $$ = append_sibling($1, $2); }
           | ProgramBody SEMICOLON { $$ = $1; }
           ;

MethodDecl: PUBLIC STATIC MethodHeader MethodBody { 
    $$ = create_node("MethodDecl", NULL);
    add_child($$, $3);
    add_child($$, $4);
} ;

MethodHeader: Type IDENTIFIER LPAR FormalParams RPAR { 
                $$ = create_node("MethodHeader", NULL);
                add_child($$, $1);
                add_child($$, create_node("Identifier", $2));
                add_child($$, $4);
            }
            | VOID IDENTIFIER LPAR FormalParams RPAR { 
                $$ = create_node("MethodHeader", NULL);
                add_child($$, create_node("Void", NULL));
                add_child($$, create_node("Identifier", $2));
                add_child($$, $4);
            }
            | Type IDENTIFIER LPAR RPAR { 
                $$ = create_node("MethodHeader", NULL);
                add_child($$, $1);
                add_child($$, create_node("Identifier", $2));
                add_child($$, create_node("MethodParams", NULL));
            }
            | VOID IDENTIFIER LPAR RPAR { 
                $$ = create_node("MethodHeader", NULL);
                add_child($$, create_node("Void", NULL));
                add_child($$, create_node("Identifier", $2));
                add_child($$, create_node("MethodParams", NULL));
            }
            ;

FormalParams: Type IDENTIFIER FormalParamsList { 
                $$ = create_node("MethodParams", NULL);
                node* param = create_node("ParamDecl", NULL);
                add_child(param, $1);
                add_child(param, create_node("Identifier", $2));
                add_child($$, param);
                if($3) add_child($$, $3);
            }
            | STRING LSQ RSQ IDENTIFIER { 
                $$ = create_node("MethodParams", NULL);
                node* param = create_node("ParamDecl", NULL);
                add_child(param, create_node("StringArray", NULL));
                add_child(param, create_node("Identifier", $4));
                add_child($$, param);
            }
            ;

FormalParamsList: /* vazio */ { $$ = NULL; }
                | FormalParamsList COMMA Type IDENTIFIER { 
                    node* param = create_node("ParamDecl", NULL);
                    add_child(param, $3);
                    add_child(param, create_node("Identifier", $4));
                    $$ = append_sibling($1, param);
                }
                ;

MethodBody: LBRACE MethodBodyContent RBRACE { 
    $$ = create_node("MethodBody", NULL);
    if ($2) add_child($$, $2);
} ;

MethodBodyContent: /* vazio */ { $$ = NULL; }
                 | MethodBodyContent Statement { $$ = append_sibling($1, $2); }
                 | MethodBodyContent VarDecl { $$ = append_sibling($1, $2); }
                 ;

FieldDecl: PUBLIC STATIC Type IDENTIFIER IdList SEMICOLON { 
             node* first = create_node("FieldDecl", NULL);
             add_child(first, $3);
             add_child(first, create_node("Identifier", $4));
             $$ = first;
             node* aux = $5;
             node* curr = first;
             while(aux) {
                 node* next_decl = create_node("FieldDecl", NULL);
                 add_child(next_decl, create_node($3->type, NULL));
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
            node* first = create_node("VarDecl", NULL);
            add_child(first, $1);
            add_child(first, create_node("Identifier", $2));
            $$ = first;
            node* aux = $3;
            node* curr = first;
            while(aux) {
                node* next_decl = create_node("VarDecl", NULL);
                add_child(next_decl, create_node($1->type, NULL)); 
                node* next_id = aux->next;
                aux->next = NULL;
                add_child(next_decl, aux);
                curr->next = next_decl;
                curr = next_decl;
                aux = next_id;
            }
         } ;

IdList: /* vazio */ { $$ = NULL; }
      | IdList COMMA IDENTIFIER { $$ = append_sibling($1, create_node("Identifier", $3)); }
      ;

Type: BOOL { $$ = create_node("Bool", NULL); } 
    | INT { $$ = create_node("Int", NULL); } 
    | DOUBLE { $$ = create_node("Double", NULL); } 
    ;

Statement: LBRACE StatementList RBRACE { 
            int count = 0;
            node* aux = $2;
            while(aux) { count++; aux = aux->next; }
            if (count > 1) {
                $$ = create_node("Block", NULL);
                add_child($$, $2);
            } else {
                $$ = $2;
            }
         }
         | IF LPAR Expr RPAR Statement %prec LOWER_THAN_ELSE { 
            $$ = create_node("If", NULL);
            add_child($$, $3);
            add_child($$, $5 ? $5 : create_node("Block", NULL));
            add_child($$, create_node("Block", NULL));
         }
         | IF LPAR Expr RPAR Statement ELSE Statement { 
            $$ = create_node("If", NULL);
            add_child($$, $3);
            add_child($$, $5 ? $5 : create_node("Block", NULL));
            add_child($$, $7 ? $7 : create_node("Block", NULL));
         }
         | WHILE LPAR Expr RPAR Statement { 
            $$ = create_node("While", NULL);
            add_child($$, $3);
            add_child($$, $5 ? $5 : create_node("Block", NULL));
         }
         | RETURN SEMICOLON { $$ = create_node("Return", NULL); }
         | RETURN Expr SEMICOLON { $$ = create_node("Return", NULL); add_child($$, $2); }
         | MethodInvocation SEMICOLON { $$ = $1; }
         | Assignment SEMICOLON { $$ = $1; }
         | ParseArgs SEMICOLON { $$ = $1; }
         | SEMICOLON { $$ = NULL; }
         | PRINT LPAR Expr RPAR SEMICOLON { $$ = create_node("Print", NULL); add_child($$, $3); }
         | PRINT LPAR STRLIT RPAR SEMICOLON { $$ = create_node("Print", NULL); add_child($$, create_node("StrLit", $3)); }
         | error SEMICOLON { $$ = NULL; }
         ;

StatementList: /* vazio */ { $$ = NULL; }
             | StatementList Statement { $$ = append_sibling($1, $2); }
             ;

MethodInvocation: IDENTIFIER LPAR ExprList RPAR { 
                    $$ = create_node("Call", NULL);
                    add_child($$, create_node("Identifier", $1));
                    if($3) add_child($$, $3);
                }
                | IDENTIFIER LPAR error RPAR { $$ = NULL; }
                ;

ExprList: { $$ = NULL; }
        | Expr { $$ = $1; }
        | ExprList COMMA Expr { $$ = append_sibling($1, $3); }
        ;

Assignment: IDENTIFIER ASSIGN Expr { 
    $$ = create_node("Assign", NULL);
    add_child($$, create_node("Identifier", $1));
    add_child($$, $3);
} ;

ParseArgs: PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR { 
    $$ = create_node("ParseArgs", NULL);
    add_child($$, create_node("Identifier", $3));
    add_child($$, $5);
}
         | PARSEINT LPAR error RPAR { $$ = NULL; }
         ;

Expr: Expr PLUS Expr { $$ = create_node("Add", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr MINUS Expr { $$ = create_node("Sub", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr STAR Expr { $$ = create_node("Mul", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr DIV Expr { $$ = create_node("Div", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr MOD Expr { $$ = create_node("Mod", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr AND Expr { $$ = create_node("And", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr OR Expr { $$ = create_node("Or", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr XOR Expr { $$ = create_node("Xor", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr LSHIFT Expr { $$ = create_node("Lshift", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr RSHIFT Expr { $$ = create_node("Rshift", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr EQ Expr { $$ = create_node("Eq", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr NE Expr { $$ = create_node("Ne", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr GE Expr { $$ = create_node("Ge", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr GT Expr { $$ = create_node("Gt", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr LE Expr { $$ = create_node("Le", NULL); add_child($$, $1); add_child($$, $3); }
    | Expr LT Expr { $$ = create_node("Lt", NULL); add_child($$, $1); add_child($$, $3); }
    | PLUS Expr %prec UNARY_PLUS { $$ = create_node("Plus", NULL); add_child($$, $2); }
    | MINUS Expr %prec UNARY_MINUS { $$ = create_node("Minus", NULL); add_child($$, $2); }
    | NOT Expr { $$ = create_node("Not", NULL); add_child($$, $2); }
    | LPAR Expr RPAR { $$ = $2; }
    | LPAR error RPAR { $$ = NULL; }
    | MethodInvocation { $$ = $1; }
    | Assignment { $$ = $1; }
    | ParseArgs { $$ = $1; }
    | IDENTIFIER { $$ = create_node("Identifier", $1); }
    | IDENTIFIER DOTLENGTH { $$ = create_node("Length", NULL); add_child($$, create_node("Identifier", $1)); }
    | NATURAL { $$ = create_node("Natural", $1); }
    | DECIMAL { $$ = create_node("Decimal", $1); }
    | BOOLLIT { $$ = create_node("BoolLit", $1); }
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
            while (yylex()) ; // Consome todos os tokens e imprime
            return 0;
        } else if (strcmp(argv[1], "-e1") == 0) {
            print_tokens = 0;
            while (yylex()) ; // Consome tokens sem imprimir (só imprime erros)
            return 0;
        } else if (strcmp(argv[1], "-t") == 0) {
            yyparse(); // Chama o Yacc
            if (root && !syntax_error) print_tree(root, 0);
            return 0;
        } else if (strcmp(argv[1], "-e2") == 0) {
            yyparse(); // Chama o Yacc, mas não imprime a árvore
            return 0;
        }
    }
    
    // Comportamento por defeito (sem argumentos)
    yyparse();
    return 0;
}