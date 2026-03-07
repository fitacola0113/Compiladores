/* Bernardo Mateus 2023211936 Daniel Mendes 2023208585 */

%{
    #include <stdio.h>
    #include <string.h>

    char str_buffer[1024];
    int str_index = 0;
    int has_error = 0; 

    int cur_column = 1;
    int cur_line = 1;
    int start_col = 1; 

    int str_start_line = 1;
    int str_start_col = 1;

    int comment_line = 1;
    int comment_col = 1;

    extern int print_tokens;

    #define YY_USER_ACTION \
        start_col = cur_column; \
        cur_column += yyleng;
%}

%x COMMENT STRING LINE_COMMENT

digit      [0-9]
digit_seq  {digit}({digit}|_)*{digit}|{digit}
letter     [a-zA-Z_$]

%%

"//" { BEGIN(LINE_COMMENT); }
<LINE_COMMENT>{
    \n|\r|\r\n { 
        cur_line++; 
        cur_column = 1; 
        BEGIN(INITIAL); 
    }
    . { ; }
}

"/*" { 
    comment_line = cur_line; 
    comment_col = start_col; 
    BEGIN(COMMENT); 
}
<COMMENT>{
    "*/"       { BEGIN(INITIAL); }
    \n|\r|\r\n { cur_line++; cur_column = 1; }
    .          { }
    <<EOF>>    { 
        printf("Line %d, col %d: unterminated comment\n", comment_line, comment_col);
        BEGIN(INITIAL); 
    }
}

\" {
    str_index = 0;
    has_error = 0;
    str_start_line = cur_line;
    str_start_col = start_col;
    BEGIN(STRING);
}

<STRING>{
    \" {
        if (!has_error) {
            str_buffer[str_index] = '\0';
            if (print_tokens) printf("STRLIT(\"%s\")\n", str_buffer);
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
        printf("Line %d, col %d: invalid escape sequence (%s)\n", cur_line, start_col, yytext);
        has_error = 1;
    }
    
    \\ {
        printf("Line %d, col %d: invalid escape sequence (%s)\n", cur_line, start_col, yytext);
        has_error = 1;
    }
    
    [^\n\r\"\\]+ {
        for(int i = 0; i < yyleng && str_index < 1023; i++)
            str_buffer[str_index++] = yytext[i];
    }
    
    \n|\r|\r\n {
        printf("Line %d, col %d: unterminated string literal\n", str_start_line, str_start_col);
        cur_line++; 
        cur_column = 1;
        BEGIN(INITIAL);
    }
    
    <<EOF>> {
        printf("Line %d, col %d: unterminated string literal\n", str_start_line, str_start_col);
        BEGIN(INITIAL);
    }
}

\n|\r|\r\n { cur_line++; cur_column = 1; }

[ \t\f]+ { ; }

"boolean"           { if(print_tokens) printf("BOOL\n"); }
"class"             { if(print_tokens) printf("CLASS\n"); }
".length"           { if(print_tokens) printf("DOTLENGTH\n"); }
"double"            { if(print_tokens) printf("DOUBLE\n"); }
"else"              { if(print_tokens) printf("ELSE\n"); }
"if"                { if(print_tokens) printf("IF\n"); }
"int"               { if(print_tokens) printf("INT\n"); }
"System.out.print"  { if(print_tokens) printf("PRINT\n"); }
"Integer.parseInt"  { if(print_tokens) printf("PARSEINT\n"); }
"public"            { if(print_tokens) printf("PUBLIC\n"); }
"return"            { if(print_tokens) printf("RETURN\n"); }
"static"            { if(print_tokens) printf("STATIC\n"); }
"String"            { if(print_tokens) printf("STRING\n"); }
"void"              { if(print_tokens) printf("VOID\n"); }
"while"             { if(print_tokens) printf("WHILE\n"); }

    /* --- REGRA RESERVED */
"abstract"|"assert"|"break"|"byte"|"case"|"catch"|"char"|"const"|"continue"|"default"|"do"|"enum"|"extends"|"final"|"finally"|"float"|"for"|"goto"|"implements"|"import"|"instanceof"|"interface"|"long"|"native"|"new"|"package"|"private"|"protected"|"short"|"strictfp"|"super"|"switch"|"synchronized"|"this"|"throw"|"throws"|"transient"|"try"|"volatile"|"++"|"--"|"null"|"Integer"|"System" {
    if(print_tokens) printf("RESERVED(%s)\n", yytext);
}

"true"|"false"      { if(print_tokens) printf("BOOLLIT(%s)\n", yytext); }

0|[1-9]({digit}|_)*{digit}|[1-9] { if(print_tokens) printf("NATURAL(%s)\n", yytext); }

({digit_seq}"."{digit_seq}?([eE][+-]?{digit_seq})?)|("."{digit_seq}([eE][+-]?{digit_seq})?)|({digit_seq}[eE][+-]?{digit_seq}) {
    if(print_tokens) printf("DECIMAL(%s)\n", yytext);
}

{letter}({digit}|{letter})* { if(print_tokens) printf("IDENTIFIER(%s)\n", yytext); }

"("                 { if(print_tokens) printf("LPAR\n"); }
")"                 { if(print_tokens) printf("RPAR\n"); }
"{"                 { if(print_tokens) printf("LBRACE\n"); }
"}"                 { if(print_tokens) printf("RBRACE\n"); }
"["                 { if(print_tokens) printf("LSQ\n"); }
"]"                 { if(print_tokens) printf("RSQ\n"); }
"=="                { if(print_tokens) printf("EQ\n"); }
"!="                { if(print_tokens) printf("NE\n"); }
"<="                { if(print_tokens) printf("LE\n"); }
">="                { if(print_tokens) printf("GE\n"); }
"&&"                { if(print_tokens) printf("AND\n"); }
"||"                { if(print_tokens) printf("OR\n"); }
"->"                { if(print_tokens) printf("ARROW\n"); }
"<<"                { if(print_tokens) printf("LSHIFT\n"); }
">>"                { if(print_tokens) printf("RSHIFT\n"); }
"="                 { if(print_tokens) printf("ASSIGN\n"); }
";"                 { if(print_tokens) printf("SEMICOLON\n"); }
","                 { if(print_tokens) printf("COMMA\n"); }
"+"                 { if(print_tokens) printf("PLUS\n"); }
"-"                 { if(print_tokens) printf("MINUS\n"); }
"*"                 { if(print_tokens) printf("STAR\n"); }
"/"                 { if(print_tokens) printf("DIV\n"); }
"%"                 { if(print_tokens) printf("MOD\n"); }
"<"                 { if(print_tokens) printf("LT\n"); }
">"                 { if(print_tokens) printf("GT\n"); }
"!"                 { if(print_tokens) printf("NOT\n"); }
"^"                 { if(print_tokens) printf("XOR\n"); }

. { printf("Line %d, col %d: illegal character (%s)\n", cur_line, start_col, yytext); }

%%

int yywrap() { return 1; }