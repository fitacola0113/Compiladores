%{
    /* place here any C variables and definitions */
    #include <stdio.h>
%}

digit   [0-9]

%%

{digit}+                { printf("NATURAL(%s)\n",yytext); }
{digit}*"."{digit}+     { printf("DECIMAL(%s)\n",yytext); }

%%

extern int yylex();
int main() {
    yylex();    /* run the lexical analysis automaton */
    return 0;
}
int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}
