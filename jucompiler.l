/* Bernardo Mateus 2023211936 Daniel Mendes 2023208585 */

%{
    #include <stdio.h>
    #include <string.h>
    #include "y.tab.h"

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
        yylval.info.line = cur_line; \
        yylval.info.col = cur_column; \
        start_col = cur_column; \
        cur_column += yyleng;
%}

%x COMMENT STR_STATE LINE_COMMENT

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
    BEGIN(STR_STATE);
}

<STR_STATE>{
    \" {
        if (!has_error) {
            str_buffer[str_index] = '\0';
            if (print_tokens) printf("STRLIT(\"%s\")\n", str_buffer);
            BEGIN(INITIAL);
            
            start_col = str_start_col;
            
            char temp[1026];
            sprintf(temp, "\"%s\"", str_buffer);
            yylval.info.str = strdup(temp);
            
            return STRLIT;
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

"boolean"           { if(print_tokens) printf("BOOL\n"); return BOOL; }
"class"             { if(print_tokens) printf("CLASS\n"); return CLASS; }
".length"           { if(print_tokens) printf("DOTLENGTH\n"); return DOTLENGTH; }
"double"            { if(print_tokens) printf("DOUBLE\n"); return DOUBLE; }
"else"              { if(print_tokens) printf("ELSE\n"); return ELSE; }
"if"                { if(print_tokens) printf("IF\n"); return IF; }
"int"               { if(print_tokens) printf("INT\n"); return INT; }
"System.out.print"  { if(print_tokens) printf("PRINT\n"); return PRINT; }
"Integer.parseInt"  { if(print_tokens) printf("PARSEINT\n"); return PARSEINT; }
"public"            { if(print_tokens) printf("PUBLIC\n"); return PUBLIC; }
"return"            { if(print_tokens) printf("RETURN\n"); return RETURN; }
"static"            { if(print_tokens) printf("STATIC\n"); return STATIC; }
"String"            { if(print_tokens) printf("STRING\n"); return STRING; }
"void"              { if(print_tokens) printf("VOID\n"); return VOID; }
"while"             { if(print_tokens) printf("WHILE\n"); return WHILE; }

    /* --- REGRA RESERVED */
"abstract"|"assert"|"break"|"byte"|"case"|"catch"|"char"|"const"|"continue"|"default"|"do"|"enum"|"extends"|"final"|"finally"|"float"|"for"|"goto"|"implements"|"import"|"instanceof"|"interface"|"long"|"native"|"new"|"package"|"private"|"protected"|"short"|"strictfp"|"super"|"switch"|"synchronized"|"this"|"throw"|"throws"|"transient"|"try"|"volatile"|"++"|"--"|"null"|"Integer"|"System" {
    if(print_tokens) printf("RESERVED(%s)\n", yytext); yylval.info.str = strdup(yytext); 
    return RESERVED;
}

"true"|"false"      { if(print_tokens) printf("BOOLLIT(%s)\n", yytext); yylval.info.str = strdup(yytext);
    return BOOLLIT; }

0|[1-9]({digit}|_)*{digit}|[1-9] { if(print_tokens) printf("NATURAL(%s)\n", yytext); yylval.info.str = strdup(yytext); 
    return NATURAL; }

({digit_seq}"."{digit_seq}?([eE][+-]?{digit_seq})?)|("."{digit_seq}([eE][+-]?{digit_seq})?)|({digit_seq}[eE][+-]?{digit_seq}) {
    if(print_tokens) printf("DECIMAL(%s)\n", yytext); yylval.info.str = strdup(yytext);
    return DECIMAL;
}

{letter}({digit}|{letter})* { if(print_tokens) printf("IDENTIFIER(%s)\n", yytext); yylval.info.str = strdup(yytext); return IDENTIFIER; }

"("                 { if(print_tokens) printf("LPAR\n"); return LPAR;}
")"                 { if(print_tokens) printf("RPAR\n"); return RPAR;}
"{"                 { if(print_tokens) printf("LBRACE\n"); return LBRACE;}
"}"                 { if(print_tokens) printf("RBRACE\n"); return RBRACE;}
"["                 { if(print_tokens) printf("LSQ\n"); return LSQ;}
"]"                 { if(print_tokens) printf("RSQ\n"); return RSQ;}
"=="                { if(print_tokens) printf("EQ\n"); return EQ;}
"!="                { if(print_tokens) printf("NE\n"); return NE;}
"<="                { if(print_tokens) printf("LE\n"); return LE;}
">="                { if(print_tokens) printf("GE\n"); return GE;}
"&&"                { if(print_tokens) printf("AND\n"); return AND;}
"||"                { if(print_tokens) printf("OR\n"); return OR;}
"->"                { if(print_tokens) printf("ARROW\n"); return ARROW;}
"<<"                { if(print_tokens) printf("LSHIFT\n"); return LSHIFT;}
">>"                { if(print_tokens) printf("RSHIFT\n"); return RSHIFT;}
"="                 { if(print_tokens) printf("ASSIGN\n"); return ASSIGN;}
";"                 { if(print_tokens) printf("SEMICOLON\n"); return SEMICOLON;}
","                 { if(print_tokens) printf("COMMA\n"); return COMMA;}
"+"                 { if(print_tokens) printf("PLUS\n"); return PLUS;}
"-"                 { if(print_tokens) printf("MINUS\n"); return MINUS;}
"*"                 { if(print_tokens) printf("STAR\n"); return STAR;}
"/"                 { if(print_tokens) printf("DIV\n"); return DIV;}
"%"                 { if(print_tokens) printf("MOD\n"); return MOD;}
"<"                 { if(print_tokens) printf("LT\n"); return LT;}
">"                 { if(print_tokens) printf("GT\n"); return GT;}
"!"                 { if(print_tokens) printf("NOT\n"); return NOT;}
"^"                 { if(print_tokens) printf("XOR\n"); return XOR;}

<INITIAL><<EOF>> {
    start_col = cur_column;
    return 0;
}

. { printf("Line %d, col %d: illegal character (%s)\n", cur_line, start_col, yytext); }

%%

int yywrap() { return 1; }