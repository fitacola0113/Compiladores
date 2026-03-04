%{
#include <stdio.h>
extern int yylex(void);
extern int line_num;
extern int col_num;
void yyerror(char *);
extern char *yytext;
%}

%token NATURAL
%nonassoc LOWEST
%left '+' '-'
%left '*' '/'
%left '(' ')'
%nonassoc IF THEN ELSE

%%

calculator: expression                  { printf("%d\n", $1); }
          | calculator ',' expression   { printf("%d\n", $3); }
          ;

expression: NATURAL                     { $$ = $1; }
          | expression '+' expression   { $$ = $1 + $3; }
          | expression '-' expression   { $$ = $1 - $3; }
          | expression '*' expression   { $$ = $1 * $3; }
          | expression '/' expression   { $$ = $1 / $3; }
          |    '(' expression ')'       { $$ = $2; }
          | IF expression THEN expression ELSE expression %prec LOWEST { $$ = $2 ? $4 : $6; }
          ;

%%

void yyerror(char *error) {
    fprintf(stderr, "Syntax Error na linha %d, coluna %d: inesperado '%s'\n", 
            line_num, col_num, yytext);
}
