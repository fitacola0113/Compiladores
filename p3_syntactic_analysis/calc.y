%{
#include <stdio.h>
extern int yylex(void);
void yyerror(char *);
extern char *yytext;
%}

%token NATURAL
%left '+' '-'
%left '*' '/'
%left '(' ')'

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
          ;

%%

void yyerror(char *error) {
    printf("%s '%s'\n", error, yytext);
}
