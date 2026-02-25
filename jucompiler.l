/*Bernardo Mateus 2023211936 Daniel Mendes 2023208585 */

%{
    #include <stdio.h>
    #include <string.h>

    char str_buffer[1024];
    int str_index = 0;
    int has_error = 0; 

    int cur_column = 1;
    int cur_line = 1;
    int start_col = 1; // Guarda onde o token começou

    // Guardamos a coluna inicial antes de atualizar a posição
    #define YY_USER_ACTION \
        start_col = cur_column; \
        for (int i=0; i<yyleng; i++) { \
            if (yytext[i] == '\n') { \
                cur_line++; \
                cur_column = 1; \
            } else { \
                cur_column++; \
            } \
        }
%}

%x COMMENT STRING

digit      [0-9]
digit_seq  {digit}(_?{digit})*
letter     [a-zA-Z_$]

%%
"/*"            { BEGIN(COMMENT); }
<COMMENT>{
    "*/"        { BEGIN(INITIAL); }
    "\n"        { }
    .           { }
    <<EOF>>     { BEGIN(INITIAL); }
}

\" {
    str_index = 0;
    has_error = 0;
    BEGIN(STRING);
}

<STRING>{
    \" {
        if (!has_error) {
            str_buffer[str_index] = '\0';
            printf("STRLIT(\"%s\")\n", str_buffer);
        }
        BEGIN(INITIAL);
    }
    \\[fnrt\\\"] {
        if (str_index + 2 < 1024) {
            str_buffer[str_index++] = yytext[0];
            str_buffer[str_index++] = yytext[1];
        }
    }
    \\[^fnrt\\\"\n\r] {
        printf("Line %d, column %d: invalid escape sequence (%s)\n", cur_line, start_col, yytext);
        has_error = 1;
    }
    \\ {
        printf("Line %d, column %d: invalid escape sequence (%s)\n", cur_line, start_col, yytext);
        has_error = 1;
    }
    [^\n\r\"\\]+ {
        for(int i = 0; i < yyleng && str_index < 1023; i++)
            str_buffer[str_index++] = yytext[i];
    }
    \n|\r {
        // Usamos start_col para evitar o -1
        printf("Line %d, column %d: unterminated string literal\n", cur_line - 1, start_col);
        BEGIN(INITIAL);
    }
    <<EOF>> {
        printf("Line %d, column %d: unterminated string literal\n", cur_line, start_col);
        BEGIN(INITIAL);
    }
}

"boolean"           { printf("BOOL\n"); }
"class"             { printf("CLASS\n"); }
".length"           { printf("DOTLENGTH\n"); }
"double"            { printf("DOUBLE\n"); }
"else"              { printf("ELSE\n"); }
"if"                { printf("IF\n"); }
"int"               { printf("INT\n"); }
"System.out.print"  { printf("PRINT\n"); }
"Integer.parseInt"  { printf("PARSEINT\n"); }
"public"            { printf("PUBLIC\n"); }
"return"            { printf("RETURN\n"); }
"static"            { printf("STATIC\n"); }
"String"            { printf("STRING\n"); }
"void"              { printf("VOID\n"); }
"while"             { printf("WHILE\n"); }

    /* --- REGRA RESERVED --- */
"abstract"|"assert"|"break"|"byte"|"case"|"catch"|"char"|"const"|"continue"|"default"|"do"|"enum"|"extends"|"final"|"finally"|"float"|"for"|"goto"|"implements"|"import"|"instanceof"|"interface"|"long"|"native"|"new"|"package"|"private"|"protected"|"short"|"strictfp"|"super"|"switch"|"synchronized"|"this"|"throw"|"throws"|"transient"|"try"|"volatile"|"++"|"--"|"null"|"Integer"|"System" {
    printf("RESERVED(%s)\n", yytext);
}

"true"|"false"      { printf("BOOLLIT(%s)\n", yytext); }

0|([1-9](_?{digit})*) { printf("NATURAL(%s)\n", yytext); }

({digit_seq}"."{digit_seq}?([eE][+-]?{digit_seq})?)|("."{digit_seq}([eE][+-]?{digit_seq})?)|({digit_seq}[eE][+-]?{digit_seq}) {
    printf("DECIMAL(%s)\n", yytext);
}

{letter}({digit}|{letter})* { printf("IDENTIFIER(%s)\n", yytext); }

[ \t\r\n]+          { ; }

"("                 { printf("LPAR\n"); }
")"                 { printf("RPAR\n"); }
"{"                 { printf("LBRACE\n"); }
"}"                 { printf("RBRACE\n"); }
"["                 { printf("LSQ\n"); }
"]"                 { printf("RSQ\n"); }
"=="                { printf("EQ\n"); }
"!="                { printf("NE\n"); }
"<="                { printf("LE\n"); }
">="                { printf("GE\n"); }
"&&"                { printf("AND\n"); }
"||"                { printf("OR\n"); }
"->"                { printf("ARROW\n"); }
"<<"                { printf("LSHIFT\n"); }
">>"                { printf("RSHIFT\n"); }
"="                 { printf("ASSIGN\n"); }
";"                 { printf("SEMICOLON\n"); }
","                 { printf("COMMA\n"); }
"+"                 { printf("PLUS\n"); }
"-"                 { printf("MINUS\n"); }
"*"                 { printf("STAR\n"); }
"/"                 { printf("DIV\n"); }
"%"                 { printf("MOD\n"); }
"<"                 { printf("LT\n"); }
">"                 { printf("GT\n"); }
"!"                 { printf("NOT\n"); }
"^"                 { printf("XOR\n"); }

. { printf("Line %d, column %d: illegal character (%s)\n", cur_line, start_col, yytext); }

%%

int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}