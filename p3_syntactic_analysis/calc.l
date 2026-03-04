%{
#include "y.tab.h"
int line_num = 1;
int col_num = 0;
int yyparse(void);
%}
digit   [0-9]
%%
"if"                    { col_num += yyleng; return IF; }
"then"                  { col_num += yyleng; return THEN; }
"else"                  { col_num += yyleng; return ELSE; }
{digit}+                { yylval = atoi(yytext); col_num += yyleng; return NATURAL; }
[()=,*/+-]              { col_num += yyleng; return yytext[0]; }
" "                     { col_num++; }
\t                      { col_num += 8; }
            /* [\t ]+                  { col_num += 8; } */   /* comentei esta linha porque considerava 1 espaço e 1 tab a mesma quantidade de colnuas ent otimizei */
\n                      { line_num++; col_num = 1; return 0; }
.                       { printf("Unrecognized character\n"); col_num += yyleng; }
%%
int main() {
    yyparse();
    return 0;
}
int yywrap() {
    return 1;
}
