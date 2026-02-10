%{
    /* place here any C variables and definitions */
    #include <stdio.h>
%}

digit   [0-9]
letter   [a-z]

%%

"integer"               { printf("INTEGER\n"); }
"double"                { printf("DOUBLE\n"); }
"if"                    { printf("IF\n"); }
"then"                  { printf("THEN\n"); }
"else"                  { printf("ELSE\n"); }

{digit}+                { printf("NATURAL(%s)\n",yytext); }
{digit}*"."{digit}+     { printf("DECIMAL(%s)\n",yytext); }

{letter}({digit}|{letter})*      { printf("IDENTIFIER(%s)\n",yytext); }

%%

extern int yylex();
int main() {
    yylex();    /* run the lexical analysis automaton */
    return 0;
}
int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}
