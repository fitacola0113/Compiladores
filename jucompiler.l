/* Bernardo Mateus 2023211936 Daniel Mendes 2023208585 */

%{
    #include <stdio.h>
    #include <string.h>

    char str_buffer[1024];
    int str_index = 0;        /* usado para armazenar e validar strings */
    int has_error = 0; 

    int cur_column = 1;
    int cur_line = 1;        /* usado para dar track dos tokens do inicio ao fim (e tmb para reports de erros) */
    int start_col = 1; 

    int str_start_line = 1;
    int str_start_col = 1;

    int comment_line = 1;           /* variaveis para o track de comentarios */
    int comment_col = 1;

    int print_tokens = 0;      /* o boolean para selecionar entre print de erro ou output normal */

    #define YY_USER_ACTION \
        start_col = cur_column; \    /* isto é ganda flex do lex (ganda trocadilho han) */
        cur_column += yyleng;        /* isto poupa trabalho de atualizar em cada parte do codigo, aqui atualiza automaticamente as colunas  */
%}

%x COMMENT STRING LINE_COMMENT

digit      [0-9]
digit_seq  {digit}({digit}|_)*{digit}|{digit}   /* esta adição ao código anterior é para permitir '_' aos numeros (mas so entre eles) */
letter     [a-zA-Z_$]

%%

"//" { BEGIN(LINE_COMMENT); }            /* isto é para comentarios de linha btw, o de blocos é asseguir ^^ */
<LINE_COMMENT>{
    \n|\r|\r\n { 
        cur_line++;                      /* esta parte faz (mal inicie o '//') com que ignore tudo até encontrar um dos carateres especiais (\n,\t,...) */
        cur_column = 1; 
        BEGIN(INITIAL);                  /* quando encontra, incrementa uma linha, da resetzao na coluna e começa o default (INITIAL) */
    }                     
    . { ; }                      /* a junção q faz ignorar tudo até o descrito */
}

"/*" {                                    /* esta é a parte que ignora bloco de codigo */
    comment_line = cur_line; 
    comment_col = start_col;              /* da save onde começou */
    BEGIN(COMMENT); 
}
<COMMENT>{
    "*/"       { BEGIN(INITIAL); }
    \n|\r|\r\n { cur_line++; cur_column = 1; }           /* se encontrar "*/" termina e volta ao INITIAL, se encontrar aqueles carateres especiais, incrementa linha e reset na coluna */
    .          { }                                       /* ignora tudo enquanto ta no processo */
    <<EOF>>    { 
        printf("Line %d, col %d: unterminated comment\n", comment_line, comment_col);         /* se chegar a EOF sem encontrar o "*/" da erro */
        BEGIN(INITIAL); 
    }
}

\" {
    str_index = 0;
    has_error = 0;
    str_start_line = cur_line;     /* quando encontra aspas, da inicializaçao dos parametros e no STRING  */
    str_start_col = start_col;
    BEGIN(STRING);
}

<STRING>{
    \" {
        if (!has_error) {                                /* se voltar a encontrar aspas, e n tiver erros, da um fim ao buffer e STRLIT da frase  */
            str_buffer[str_index] = '\0';
            if (print_tokens) printf("STRLIT(\"%s\")\n", str_buffer);
        }
        BEGIN(INITIAL);
    }
    
    \\[fnrt\\\"] {
        if (str_index + 2 < 1024) {                   /*  parte q faz a STRING aceitar os caracteres especiais metido (\n, \f, \r e \t mas mesmo escritos) e adiciona los ao buffer  */
            str_buffer[str_index++] = yytext[0];
            str_buffer[str_index++] = yytext[1];
        }
    }
    
    \\[^fnrt\\\"\n\r] {
        printf("Line %d, col %d: invalid escape sequence (%s)\n", cur_line, start_col, yytext);        /*  gera mensagem de erro caso alguem meta um carater diferente dos mencionados  */
        has_error = 1;
    }
    
    \\ {
        printf("Line %d, col %d: invalid escape sequence (%s)\n", cur_line, start_col, yytext);     /* deteçao para casos especiais caso o user de enter com barra solta no final por exemplo  */
        has_error = 1;
    }
    
    [^\n\r\"\\]+ {
        for(int i = 0; i < yyleng && str_index < 1023; i++)         /*  chars basicos (texto) adicionado ao buffer   */
            str_buffer[str_index++] = yytext[i];
    }
    
    \n|\r|\r\n {
        printf("Line %d, col %d: unterminated string literal\n", str_start_line, str_start_col);      /*  mesmo carateres especiais executados que é para incrementar, resetar e ir para o INITIAL  */
        cur_line++; 
        cur_column = 1;
        BEGIN(INITIAL);
    }
    
    <<EOF>> {
        printf("Line %d, col %d: unterminated string literal\n", str_start_line, str_start_col);  /* se chegar ao EOF sem terminar a string, gera um errozito e vai po INITIAL  */
        BEGIN(INITIAL);
    }
}

\n|\r|\r\n { cur_line++; cur_column = 1; }             /*  mudanças de linha (ja fora da STRING) caso detetadas, fazem a gestao q ja comentei 30 vezes acima */

[ \t\f]+ { ; }                                   /* ignora whitespaces e stuff like that  */

"boolean"           { if(print_tokens) printf("BOOL\n"); }
"class"             { if(print_tokens) printf("CLASS\n"); }
".length"           { if(print_tokens) printf("DOTLENGTH\n"); }
"double"            { if(print_tokens) printf("DOUBLE\n"); }
"else"              { if(print_tokens) printf("ELSE\n"); }
"if"                { if(print_tokens) printf("IF\n"); }
"int"               { if(print_tokens) printf("INT\n"); }
"System.out.print"  { if(print_tokens) printf("PRINT\n"); }                   /* acho q isto é self explanatory, da print ao correspondido se n houver erros */
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

0|[1-9]({digit}|_)*{digit}|[1-9] { if(print_tokens) printf("NATURAL(%s)\n", yytext); }                     /* esta secçao é prints de booleans, numeros naturais e decimais  */

({digit_seq}"."{digit_seq}?([eE][+-]?{digit_seq})?)|("."{digit_seq}([eE][+-]?{digit_seq})?)|({digit_seq}[eE][+-]?{digit_seq}) {  
    if(print_tokens) printf("DECIMAL(%s)\n", yytext);
}

{letter}({digit}|{letter})* { if(print_tokens) printf("IDENTIFIER(%s)\n", yytext); }                      /* print de casos que comecem com uma letra/'_'/$ e em seguida letras/numeros  */

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
"="                 { if(print_tokens) printf("ASSIGN\n"); }                     /* self explanatory outra vez, da print as operações e constituintes  */
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

. { printf("Line %d, col %d: illegal character (%s)\n", cur_line, start_col, yytext); }       /* para chegar aqui é mesmo lixado, nd acima deu match do carater ent ya, char ilegal  */

%%

int main(int argc, char *argv[]) {
    // Verifica os argumentos passados na linha de comandos 
    if (argc > 1) { 
        if (strcmp(argv[1], "-l") == 0 ) { 
            print_tokens = 1;
        } else if (strcmp(argv[1], "-e1") == 0) {                             /* verifica os argumentos no terminal, se passar pr_tk = 1, se não pr_tk = 0 */
            print_tokens = 0;
        }
    }
    
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}