%{
    /* place here any C variables and definitions */
    #define YY_USER_ACTION \
        for (int i=0; i<yyleng; i++) { \
            if (yytext[i] == '\n') { \
                cur_line++; \
                cur_column = 0; \
            } else { \
                cur_column++; \
            } \
        }


    int cur_column = 0;
    int cur_line = 1;
%}

/* Define the comment state */
%x COMMENT

digit   [0-9]
letter   [a-zA-Z]

%%
"/*"            { BEGIN(COMMENT); }
<COMMENT>{
    "*/"        { BEGIN(INITIAL); }
    "\n"        { /* Macro handles line++ and col=1 */ }
    .           { /* Macro handles col++ */ }
    <<EOF>>     { BEGIN(INITIAL); } /* Support unterminated comments */
}

"integer"               { printf("INTEGER\n"); }
"double"                { printf("DOUBLE\n"); }
"if"                    { printf("IF\n"); }
"then"                  { printf("THEN\n"); }
"else"                  { printf("ELSE\n"); }

{digit}+                { printf("NATURAL(%s)\n",yytext); }
{digit}*"."{digit}+     { printf("DECIMAL(%s)\n",yytext); }

{letter}({digit}|{letter})*      { printf("IDENTIFIER(%s)\n",yytext); }

[" ""\t""\n"]           {;}

"("                     { printf("(\n"); }
")"                     { printf(")\n"); }
"="                     { printf("=\n"); }
","                     { printf(",\n"); }     /* hell yeah, descobri q é assim q se comenta*/
"*"                     { printf("*\n"); }     /* so n faço ideia como se faz a regra do 3 e*/
"/"                     { printf("/\n"); }     /* ns se eles se referem a prints das op (5) */
"+"                     { printf("+\n"); }
"-"                     { printf("-\n"); }

.                               { printf("ERROR: line %d, collumn %d: unrecognized token \"%s\"\n", cur_line, cur_column, yytext); }

%%

extern int yylex();
int main() {
    yylex();    /* run the lexical analysis automaton */
    return 0;
}
int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}
