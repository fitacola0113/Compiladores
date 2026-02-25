To compile:
- lex lexer.l
- cc lex.yy.c -o lexer
- ./lexer

To clean the generated files:
- rm lex.yy.c lexer


Login no moooooooooshak

    Login:            uc2023211936_uc2023208585
    Password:        H6dxhy




// --- Estrutura e Meta-Lógica ---
// //Done: Suporte a comentários de bloco /* ... */
// //Done: Contador de Linhas e Colunas (YY_USER_ACTION)
// //Done: Suporte a Unterminated Comments (EOF em comentário)
// //Done: Tratamento de erro para carateres desconhecidos (.)

// --- Literais e Identificadores ---
// //Done: IDENTIFIER (Básico: falta suporte a "_" e "$") -> //ToDo
// //Done: NATURAL 
// //Done: DECIMAL
// //Done: STRLIT (Básico: falta validação rigorosa de escapes e mensagens de erro) -> //ToDo
// //ToDo: BOOLLIT ("true" | "false")

// --- Operadores e Símbolos ---
// //Done: ASSIGN ("=")
// //Done: STAR ("*")
// //Done: COMMA (",")
// //Done: DIV ("/")
// //Done: LPAR ("(")
// //Done: RPAR (")")
// //Done: PLUS ("+")
// //Done: MINUS ("-")
// //Done: AND ("&&")
// //Done: EQ ("==")
// //Done: GE (">=")
// //Done: GT (">")
// //Done: LBRACE ("{")
// //Done: LE ("<=")
// //DOne: LSQ ("[")
// //Done: LT ("<")
// //Done: MOD ("%")
// //Done: NE ("!=")
// //Done: NOT ("!")
// //Done: OR ("||")
// //Done: RBRACE ("}")
// //Done: RSQ ("]")
// //Done: SEMICOLON (";")
// //Done: ARROW ("->")
// //Done: LSHIFT ("<<")
// //Done: RSHIFT (">>")
// //Done: XOR ("^")

// --- Palavras-Chave (Keywords) ---
// //Done: IF ("if")
// //Done: ELSE ("else")
// //Done: DOUBLE ("double")
// //Done: BOOL ("boolean")
// //Done: CLASS ("class")
// //Done: DOTLENGTH (".length")
// //Done: INT ("int")
// //Done: PRINT ("System.out.print")
// //Done: PARSEINT ("Integer.parseInt")
// //Done: PUBLIC ("public")
// //Done: RETURN ("return")
// //Done: STATIC ("static")
// //Done: STRING ("String")
// //Done: VOID ("void")
// //Done: WHILE ("while")

// --- Casos Especiais ---
// //ToDo: RESERVED (Keywords Java, ++, --, null, Integer, System)