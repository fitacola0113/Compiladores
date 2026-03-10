/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 3 "jucompiler.y"

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    typedef struct node {
        char *type;
        char *value;
        struct node *child;
        struct node *next;
    } node;

    node* create_node(char* type, char* value) {
        node* n = (node*)malloc(sizeof(node));
        n->type = type;
        n->value = value ? strdup(value) : NULL;
        n->child = NULL;
        n->next = NULL;
        return n;
    }

    node* append_sibling(node* list, node* new_node) {
        if (!list) return new_node;
        if (!new_node) return list;
        node* tmp = list;
        while (tmp->next) tmp = tmp->next;
        tmp->next = new_node;
        return list;
    }

    void add_child(node* parent, node* new_child) {
        if (!parent || !new_child) return;
        if (!parent->child) parent->child = new_child;
        else {
            node* tmp = parent->child;
            while (tmp->next) tmp = tmp->next;
            tmp->next = new_child;
        }
    }

    void print_tree(node* n, int level) {
    if (!n) return;
    for (int i = 0; i < level; i++) printf("..");
    if (n->value) {
        printf("%s(%s)\n", n->type, n->value);
    } else {
        printf("%s\n", n->type); 
    }
        
        print_tree(n->child, level + 1);
        print_tree(n->next, level);
    }

    int yylex(void);
    void yyerror(char *s);
    
    extern char str_buffer[1024];
    extern int cur_line;
    extern int start_col;
    extern char* yytext;
    int print_tokens = 0;
    int syntax_error = 0;

    node* root = NULL;

#line 137 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    NATURAL = 259,                 /* NATURAL  */
    DECIMAL = 260,                 /* DECIMAL  */
    STRLIT = 261,                  /* STRLIT  */
    BOOLLIT = 262,                 /* BOOLLIT  */
    RESERVED = 263,                /* RESERVED  */
    CLASS = 264,                   /* CLASS  */
    PUBLIC = 265,                  /* PUBLIC  */
    STATIC = 266,                  /* STATIC  */
    VOID = 267,                    /* VOID  */
    STRING = 268,                  /* STRING  */
    BOOL = 269,                    /* BOOL  */
    INT = 270,                     /* INT  */
    DOUBLE = 271,                  /* DOUBLE  */
    IF = 272,                      /* IF  */
    WHILE = 273,                   /* WHILE  */
    RETURN = 274,                  /* RETURN  */
    PRINT = 275,                   /* PRINT  */
    PARSEINT = 276,                /* PARSEINT  */
    DOTLENGTH = 277,               /* DOTLENGTH  */
    SEMICOLON = 278,               /* SEMICOLON  */
    COMMA = 279,                   /* COMMA  */
    LPAR = 280,                    /* LPAR  */
    RPAR = 281,                    /* RPAR  */
    LBRACE = 282,                  /* LBRACE  */
    RBRACE = 283,                  /* RBRACE  */
    LSQ = 284,                     /* LSQ  */
    RSQ = 285,                     /* RSQ  */
    ASSIGN = 286,                  /* ASSIGN  */
    ARROW = 287,                   /* ARROW  */
    OR = 288,                      /* OR  */
    AND = 289,                     /* AND  */
    XOR = 290,                     /* XOR  */
    EQ = 291,                      /* EQ  */
    NE = 292,                      /* NE  */
    LT = 293,                      /* LT  */
    GT = 294,                      /* GT  */
    LE = 295,                      /* LE  */
    GE = 296,                      /* GE  */
    LSHIFT = 297,                  /* LSHIFT  */
    RSHIFT = 298,                  /* RSHIFT  */
    PLUS = 299,                    /* PLUS  */
    MINUS = 300,                   /* MINUS  */
    STAR = 301,                    /* STAR  */
    DIV = 302,                     /* DIV  */
    MOD = 303,                     /* MOD  */
    NOT = 304,                     /* NOT  */
    UNARY_PLUS = 305,              /* UNARY_PLUS  */
    UNARY_MINUS = 306,             /* UNARY_MINUS  */
    LOWER_THAN_ELSE = 307,         /* LOWER_THAN_ELSE  */
    ELSE = 308                     /* ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IDENTIFIER 258
#define NATURAL 259
#define DECIMAL 260
#define STRLIT 261
#define BOOLLIT 262
#define RESERVED 263
#define CLASS 264
#define PUBLIC 265
#define STATIC 266
#define VOID 267
#define STRING 268
#define BOOL 269
#define INT 270
#define DOUBLE 271
#define IF 272
#define WHILE 273
#define RETURN 274
#define PRINT 275
#define PARSEINT 276
#define DOTLENGTH 277
#define SEMICOLON 278
#define COMMA 279
#define LPAR 280
#define RPAR 281
#define LBRACE 282
#define RBRACE 283
#define LSQ 284
#define RSQ 285
#define ASSIGN 286
#define ARROW 287
#define OR 288
#define AND 289
#define XOR 290
#define EQ 291
#define NE 292
#define LT 293
#define GT 294
#define LE 295
#define GE 296
#define LSHIFT 297
#define RSHIFT 298
#define PLUS 299
#define MINUS 300
#define STAR 301
#define DIV 302
#define MOD 303
#define NOT 304
#define UNARY_PLUS 305
#define UNARY_MINUS 306
#define LOWER_THAN_ELSE 307
#define ELSE 308

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 69 "jucompiler.y"

    char* str;
    struct node* node; 

#line 301 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_NATURAL = 4,                    /* NATURAL  */
  YYSYMBOL_DECIMAL = 5,                    /* DECIMAL  */
  YYSYMBOL_STRLIT = 6,                     /* STRLIT  */
  YYSYMBOL_BOOLLIT = 7,                    /* BOOLLIT  */
  YYSYMBOL_RESERVED = 8,                   /* RESERVED  */
  YYSYMBOL_CLASS = 9,                      /* CLASS  */
  YYSYMBOL_PUBLIC = 10,                    /* PUBLIC  */
  YYSYMBOL_STATIC = 11,                    /* STATIC  */
  YYSYMBOL_VOID = 12,                      /* VOID  */
  YYSYMBOL_STRING = 13,                    /* STRING  */
  YYSYMBOL_BOOL = 14,                      /* BOOL  */
  YYSYMBOL_INT = 15,                       /* INT  */
  YYSYMBOL_DOUBLE = 16,                    /* DOUBLE  */
  YYSYMBOL_IF = 17,                        /* IF  */
  YYSYMBOL_WHILE = 18,                     /* WHILE  */
  YYSYMBOL_RETURN = 19,                    /* RETURN  */
  YYSYMBOL_PRINT = 20,                     /* PRINT  */
  YYSYMBOL_PARSEINT = 21,                  /* PARSEINT  */
  YYSYMBOL_DOTLENGTH = 22,                 /* DOTLENGTH  */
  YYSYMBOL_SEMICOLON = 23,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 24,                     /* COMMA  */
  YYSYMBOL_LPAR = 25,                      /* LPAR  */
  YYSYMBOL_RPAR = 26,                      /* RPAR  */
  YYSYMBOL_LBRACE = 27,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 28,                    /* RBRACE  */
  YYSYMBOL_LSQ = 29,                       /* LSQ  */
  YYSYMBOL_RSQ = 30,                       /* RSQ  */
  YYSYMBOL_ASSIGN = 31,                    /* ASSIGN  */
  YYSYMBOL_ARROW = 32,                     /* ARROW  */
  YYSYMBOL_OR = 33,                        /* OR  */
  YYSYMBOL_AND = 34,                       /* AND  */
  YYSYMBOL_XOR = 35,                       /* XOR  */
  YYSYMBOL_EQ = 36,                        /* EQ  */
  YYSYMBOL_NE = 37,                        /* NE  */
  YYSYMBOL_LT = 38,                        /* LT  */
  YYSYMBOL_GT = 39,                        /* GT  */
  YYSYMBOL_LE = 40,                        /* LE  */
  YYSYMBOL_GE = 41,                        /* GE  */
  YYSYMBOL_LSHIFT = 42,                    /* LSHIFT  */
  YYSYMBOL_RSHIFT = 43,                    /* RSHIFT  */
  YYSYMBOL_PLUS = 44,                      /* PLUS  */
  YYSYMBOL_MINUS = 45,                     /* MINUS  */
  YYSYMBOL_STAR = 46,                      /* STAR  */
  YYSYMBOL_DIV = 47,                       /* DIV  */
  YYSYMBOL_MOD = 48,                       /* MOD  */
  YYSYMBOL_NOT = 49,                       /* NOT  */
  YYSYMBOL_UNARY_PLUS = 50,                /* UNARY_PLUS  */
  YYSYMBOL_UNARY_MINUS = 51,               /* UNARY_MINUS  */
  YYSYMBOL_LOWER_THAN_ELSE = 52,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_ELSE = 53,                      /* ELSE  */
  YYSYMBOL_YYACCEPT = 54,                  /* $accept  */
  YYSYMBOL_Program = 55,                   /* Program  */
  YYSYMBOL_ProgramBody = 56,               /* ProgramBody  */
  YYSYMBOL_MethodDecl = 57,                /* MethodDecl  */
  YYSYMBOL_MethodHeader = 58,              /* MethodHeader  */
  YYSYMBOL_FormalParams = 59,              /* FormalParams  */
  YYSYMBOL_FormalParamsList = 60,          /* FormalParamsList  */
  YYSYMBOL_MethodBody = 61,                /* MethodBody  */
  YYSYMBOL_MethodBodyContent = 62,         /* MethodBodyContent  */
  YYSYMBOL_FieldDecl = 63,                 /* FieldDecl  */
  YYSYMBOL_VarDecl = 64,                   /* VarDecl  */
  YYSYMBOL_IdList = 65,                    /* IdList  */
  YYSYMBOL_Type = 66,                      /* Type  */
  YYSYMBOL_Statement = 67,                 /* Statement  */
  YYSYMBOL_StatementList = 68,             /* StatementList  */
  YYSYMBOL_MethodInvocation = 69,          /* MethodInvocation  */
  YYSYMBOL_ExprList = 70,                  /* ExprList  */
  YYSYMBOL_Assignment = 71,                /* Assignment  */
  YYSYMBOL_ParseArgs = 72,                 /* ParseArgs  */
  YYSYMBOL_Expr = 73,                      /* Expr  */
  YYSYMBOL_OpExpr = 74                     /* OpExpr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   305

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  80
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  166

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   308


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    98,    98,   105,   106,   107,   108,   111,   117,   123,
     129,   135,   143,   151,   160,   161,   169,   174,   175,   176,
     179,   197,   200,   219,   220,   223,   224,   225,   228,   239,
     245,   251,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   267,   268,   271,   275,   280,   283,   284,   287,   293,
     298,   301,   302,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "NATURAL", "DECIMAL", "STRLIT", "BOOLLIT", "RESERVED", "CLASS", "PUBLIC",
  "STATIC", "VOID", "STRING", "BOOL", "INT", "DOUBLE", "IF", "WHILE",
  "RETURN", "PRINT", "PARSEINT", "DOTLENGTH", "SEMICOLON", "COMMA", "LPAR",
  "RPAR", "LBRACE", "RBRACE", "LSQ", "RSQ", "ASSIGN", "ARROW", "OR", "AND",
  "XOR", "EQ", "NE", "LT", "GT", "LE", "GE", "LSHIFT", "RSHIFT", "PLUS",
  "MINUS", "STAR", "DIV", "MOD", "NOT", "UNARY_PLUS", "UNARY_MINUS",
  "LOWER_THAN_ELSE", "ELSE", "$accept", "Program", "ProgramBody",
  "MethodDecl", "MethodHeader", "FormalParams", "FormalParamsList",
  "MethodBody", "MethodBodyContent", "FieldDecl", "VarDecl", "IdList",
  "Type", "Statement", "StatementList", "MethodInvocation", "ExprList",
  "Assignment", "ParseArgs", "Expr", "OpExpr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-69)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -6,     9,    26,    19,   -69,   -69,    10,    13,    30,   -69,
     -69,   -69,   -69,   -69,   286,    45,   -69,   -69,   -69,    27,
      49,    32,   -69,   -69,    35,   133,   157,   199,    47,    38,
     -69,    46,    71,    78,   -15,    77,    89,   106,    95,    99,
     -69,   -69,   -69,   -69,   123,   -69,   105,   107,   109,   -69,
     108,   -69,   132,   111,   -69,   -69,   -69,    14,   118,   118,
     118,    25,   -69,   -69,   -69,   -69,    24,   161,   161,   161,
     -69,   -69,   -69,   119,   196,   112,    21,   180,   -69,   -69,
     -69,   -69,   -69,   -69,   141,   121,   126,   -69,     6,   -69,
     -69,   127,   128,   -69,   143,   144,    12,   -69,   -69,   -69,
     -69,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   153,   183,   185,
     187,   -69,   -69,    69,   -69,    50,   -69,   118,   -69,   201,
     201,   -69,   -69,   211,   225,   238,   249,   249,   145,   145,
     145,   145,    51,    51,   148,   148,   -69,   -69,   -69,   194,
     200,   -69,   118,   -69,   223,   -69,   174,   -69,   -69,   -69,
     269,   -69,   201,   277,   -69,   -69
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     3,     0,     0,     0,     6,
       2,     4,     5,    21,     0,     0,    25,    26,    27,     0,
       0,     0,    17,     7,    23,     0,     0,     0,     0,     0,
      11,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,    41,    16,    19,     0,    18,     0,     0,     0,    10,
       0,    20,     0,     0,     9,    14,    40,     0,     0,     0,
       0,    76,    78,    79,    80,    32,     0,     0,     0,     0,
      74,    51,    75,     0,    52,     0,     0,     0,    23,    34,
      35,    36,     8,    24,     0,    12,     0,    43,     0,    46,
      48,     0,     0,    77,     0,     0,    76,    69,    70,    71,
      33,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    42,     0,    13,     0,    45,     0,    44,     0,
       0,    73,    72,    59,    58,    60,    63,    64,    68,    66,
      67,    65,    61,    62,    53,    54,    55,    56,    57,     0,
       0,    50,     0,    22,     0,    47,    29,    31,    39,    38,
       0,    15,     0,     0,    30,    49
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -69,   -69,   -69,   -69,   -69,   278,   -69,   -69,   -69,   -69,
     -69,   226,   -13,   -68,   -69,   -26,   -69,   -24,   -22,   -52,
     -25
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     6,    11,    19,    31,    85,    23,    26,    12,
      43,    28,    32,    45,    77,    70,    88,    71,    72,    73,
      74
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      46,    20,    47,     1,    48,    89,    90,    91,    92,   122,
      57,     7,     3,    44,    95,    86,    58,    61,    62,    63,
       8,    64,   119,   118,   120,    94,     4,    61,    62,    63,
     127,    64,   128,     9,    93,    39,    13,    57,    10,    66,
      87,    14,    97,    98,    99,    39,     5,    93,    21,    66,
      57,    46,    24,    47,    22,    48,    58,    25,    67,    68,
      27,   156,   157,    69,    16,    17,    18,    53,    67,    68,
      51,    52,    54,    69,    55,   155,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   153,    52,   164,   112,   113,   114,   115,   116,
     160,    56,    59,    46,    46,    47,    47,    48,    48,    61,
      62,    63,   154,    64,    60,    61,    62,    63,   117,    64,
      75,    61,    62,    63,    76,    64,    78,    39,    79,    65,
      80,    66,    81,    39,    82,    83,    46,    66,    47,    39,
      48,    84,   100,    66,   124,   125,    29,    16,    17,    18,
      67,    68,   126,   129,   130,    69,    67,    68,    33,    30,
      34,    69,    67,    68,    96,    62,    63,    69,    64,   131,
     132,    16,    17,    18,    35,    36,    37,    38,    39,   149,
      40,    33,    39,    34,    41,    42,    66,   110,   111,   112,
     113,   114,   115,   116,   114,   115,   116,    35,    36,    37,
      38,    39,    33,    40,    34,    67,    68,    41,   121,   150,
      69,   151,    29,    16,    17,    18,   152,   158,    35,    36,
      37,    38,    39,   159,    40,    49,   161,   162,    41,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    15,   163,
      16,    17,    18,   165,   123,    50
};

static const yytype_uint8 yycheck[] =
{
      26,    14,    26,     9,    26,    57,    58,    59,    60,    77,
      25,     1,     3,    26,    66,     1,    31,     3,     4,     5,
      10,     7,     1,    75,     3,     1,     0,     3,     4,     5,
      24,     7,    26,    23,    22,    21,    23,    25,    28,    25,
      26,    11,    67,    68,    69,    21,    27,    22,     3,    25,
      25,    77,     3,    77,    27,    77,    31,    25,    44,    45,
      25,   129,   130,    49,    14,    15,    16,    29,    44,    45,
      23,    24,    26,    49,     3,   127,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    23,    24,   162,    44,    45,    46,    47,    48,
     152,    23,    25,   129,   130,   129,   130,   129,   130,     3,
       4,     5,   125,     7,    25,     3,     4,     5,     6,     7,
      25,     3,     4,     5,    25,     7,     3,    21,    23,    23,
      23,    25,    23,    21,    26,     3,   162,    25,   162,    21,
     162,    30,    23,    25,     3,    24,    13,    14,    15,    16,
      44,    45,    26,    26,    26,    49,    44,    45,     1,    26,
       3,    49,    44,    45,     3,     4,     5,    49,     7,    26,
      26,    14,    15,    16,    17,    18,    19,    20,    21,    26,
      23,     1,    21,     3,    27,    28,    25,    42,    43,    44,
      45,    46,    47,    48,    46,    47,    48,    17,    18,    19,
      20,    21,     1,    23,     3,    44,    45,    27,    28,    26,
      49,    26,    13,    14,    15,    16,    29,    23,    17,    18,
      19,    20,    21,    23,    23,    26,     3,    53,    27,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    12,    30,
      14,    15,    16,    26,    78,    27
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     9,    55,     3,     0,    27,    56,     1,    10,    23,
      28,    57,    63,    23,    11,    12,    14,    15,    16,    58,
      66,     3,    27,    61,     3,    25,    62,    25,    65,    13,
      26,    59,    66,     1,     3,    17,    18,    19,    20,    21,
      23,    27,    28,    64,    66,    67,    69,    71,    72,    26,
      59,    23,    24,    29,    26,     3,    23,    25,    31,    25,
      25,     3,     4,     5,     7,    23,    25,    44,    45,    49,
      69,    71,    72,    73,    74,    25,    25,    68,     3,    23,
      23,    23,    26,     3,    30,    60,     1,    26,    70,    73,
      73,    73,    73,    22,     1,    73,     3,    74,    74,    74,
      23,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,     6,    73,     1,
       3,    28,    67,    65,     3,    24,    26,    24,    26,    26,
      26,    26,    26,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    26,
      26,    26,    29,    23,    66,    73,    67,    67,    23,    23,
      73,     3,    53,    30,    67,    26
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    54,    55,    56,    56,    56,    56,    57,    58,    58,
      58,    58,    59,    59,    60,    60,    61,    62,    62,    62,
      63,    63,    64,    65,    65,    66,    66,    66,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    68,    68,    69,    69,    69,    70,    70,    71,    72,
      72,    73,    73,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     0,     2,     2,     2,     4,     5,     5,
       4,     4,     3,     4,     0,     4,     3,     0,     2,     2,
       6,     2,     4,     0,     3,     1,     1,     1,     3,     5,
       7,     5,     2,     3,     2,     2,     2,     1,     5,     5,
       2,     0,     2,     3,     4,     4,     1,     3,     3,     7,
       4,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     3,     3,     1,     1,     1,     2,     1,     1,
       1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: CLASS IDENTIFIER LBRACE ProgramBody RBRACE  */
#line 98 "jucompiler.y"
                                                    { 
    (yyval.node) = create_node("Program", NULL);
    add_child((yyval.node), create_node("Identifier", (yyvsp[-3].str)));
    if ((yyvsp[-1].node)) add_child((yyval.node), (yyvsp[-1].node));
    root = (yyval.node);
}
#line 1496 "y.tab.c"
    break;

  case 3: /* ProgramBody: %empty  */
#line 105 "jucompiler.y"
                         { (yyval.node) = NULL; }
#line 1502 "y.tab.c"
    break;

  case 4: /* ProgramBody: ProgramBody MethodDecl  */
#line 106 "jucompiler.y"
                                    { (yyval.node) = append_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1508 "y.tab.c"
    break;

  case 5: /* ProgramBody: ProgramBody FieldDecl  */
#line 107 "jucompiler.y"
                                   { (yyval.node) = append_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1514 "y.tab.c"
    break;

  case 6: /* ProgramBody: ProgramBody SEMICOLON  */
#line 108 "jucompiler.y"
                                   { (yyval.node) = (yyvsp[-1].node); }
#line 1520 "y.tab.c"
    break;

  case 7: /* MethodDecl: PUBLIC STATIC MethodHeader MethodBody  */
#line 111 "jucompiler.y"
                                                  { 
    (yyval.node) = create_node("MethodDecl", NULL);
    add_child((yyval.node), (yyvsp[-1].node));
    add_child((yyval.node), (yyvsp[0].node));
}
#line 1530 "y.tab.c"
    break;

  case 8: /* MethodHeader: Type IDENTIFIER LPAR FormalParams RPAR  */
#line 117 "jucompiler.y"
                                                     { 
                (yyval.node) = create_node("MethodHeader", NULL);
                add_child((yyval.node), (yyvsp[-4].node));
                add_child((yyval.node), create_node("Identifier", (yyvsp[-3].str)));
                add_child((yyval.node), (yyvsp[-1].node));
            }
#line 1541 "y.tab.c"
    break;

  case 9: /* MethodHeader: VOID IDENTIFIER LPAR FormalParams RPAR  */
#line 123 "jucompiler.y"
                                                     { 
                (yyval.node) = create_node("MethodHeader", NULL);
                add_child((yyval.node), create_node("Void", NULL));
                add_child((yyval.node), create_node("Identifier", (yyvsp[-3].str)));
                add_child((yyval.node), (yyvsp[-1].node));
            }
#line 1552 "y.tab.c"
    break;

  case 10: /* MethodHeader: Type IDENTIFIER LPAR RPAR  */
#line 129 "jucompiler.y"
                                        { 
                (yyval.node) = create_node("MethodHeader", NULL);
                add_child((yyval.node), (yyvsp[-3].node));
                add_child((yyval.node), create_node("Identifier", (yyvsp[-2].str)));
                add_child((yyval.node), create_node("MethodParams", NULL));
            }
#line 1563 "y.tab.c"
    break;

  case 11: /* MethodHeader: VOID IDENTIFIER LPAR RPAR  */
#line 135 "jucompiler.y"
                                        { 
                (yyval.node) = create_node("MethodHeader", NULL);
                add_child((yyval.node), create_node("Void", NULL));
                add_child((yyval.node), create_node("Identifier", (yyvsp[-2].str)));
                add_child((yyval.node), create_node("MethodParams", NULL));
            }
#line 1574 "y.tab.c"
    break;

  case 12: /* FormalParams: Type IDENTIFIER FormalParamsList  */
#line 143 "jucompiler.y"
                                               { 
                (yyval.node) = create_node("MethodParams", NULL);
                node* param = create_node("ParamDecl", NULL);
                add_child(param, (yyvsp[-2].node));
                add_child(param, create_node("Identifier", (yyvsp[-1].str)));
                add_child((yyval.node), param);
                if((yyvsp[0].node)) add_child((yyval.node), (yyvsp[0].node));
            }
#line 1587 "y.tab.c"
    break;

  case 13: /* FormalParams: STRING LSQ RSQ IDENTIFIER  */
#line 151 "jucompiler.y"
                                        { 
                (yyval.node) = create_node("MethodParams", NULL);
                node* param = create_node("ParamDecl", NULL);
                add_child(param, create_node("StringArray", NULL));
                add_child(param, create_node("Identifier", (yyvsp[0].str)));
                add_child((yyval.node), param);
            }
#line 1599 "y.tab.c"
    break;

  case 14: /* FormalParamsList: %empty  */
#line 160 "jucompiler.y"
                              { (yyval.node) = NULL; }
#line 1605 "y.tab.c"
    break;

  case 15: /* FormalParamsList: FormalParamsList COMMA Type IDENTIFIER  */
#line 161 "jucompiler.y"
                                                         { 
                    node* param = create_node("ParamDecl", NULL);
                    add_child(param, (yyvsp[-1].node));
                    add_child(param, create_node("Identifier", (yyvsp[0].str)));
                    (yyval.node) = append_sibling((yyvsp[-3].node), param);
                }
#line 1616 "y.tab.c"
    break;

  case 16: /* MethodBody: LBRACE MethodBodyContent RBRACE  */
#line 169 "jucompiler.y"
                                            { 
    (yyval.node) = create_node("MethodBody", NULL);
    if ((yyvsp[-1].node)) add_child((yyval.node), (yyvsp[-1].node));
}
#line 1625 "y.tab.c"
    break;

  case 17: /* MethodBodyContent: %empty  */
#line 174 "jucompiler.y"
                               { (yyval.node) = NULL; }
#line 1631 "y.tab.c"
    break;

  case 18: /* MethodBodyContent: MethodBodyContent Statement  */
#line 175 "jucompiler.y"
                                               { (yyval.node) = append_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1637 "y.tab.c"
    break;

  case 19: /* MethodBodyContent: MethodBodyContent VarDecl  */
#line 176 "jucompiler.y"
                                             { (yyval.node) = append_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1643 "y.tab.c"
    break;

  case 20: /* FieldDecl: PUBLIC STATIC Type IDENTIFIER IdList SEMICOLON  */
#line 179 "jucompiler.y"
                                                          { 
             node* first = create_node("FieldDecl", NULL);
             add_child(first, (yyvsp[-3].node));
             add_child(first, create_node("Identifier", (yyvsp[-2].str)));
             (yyval.node) = first;
             node* aux = (yyvsp[-1].node);
             node* curr = first;
             while(aux) {
                 node* next_decl = create_node("FieldDecl", NULL);
                 add_child(next_decl, create_node((yyvsp[-3].node)->type, NULL));
                 node* next_id = aux->next;
                 aux->next = NULL;
                 add_child(next_decl, aux);
                 curr->next = next_decl;
                 curr = next_decl;
                 aux = next_id;
             }
         }
#line 1666 "y.tab.c"
    break;

  case 21: /* FieldDecl: error SEMICOLON  */
#line 197 "jucompiler.y"
                           { (yyval.node) = NULL; }
#line 1672 "y.tab.c"
    break;

  case 22: /* VarDecl: Type IDENTIFIER IdList SEMICOLON  */
#line 200 "jucompiler.y"
                                          { 
            node* first = create_node("VarDecl", NULL);
            add_child(first, (yyvsp[-3].node));
            add_child(first, create_node("Identifier", (yyvsp[-2].str)));
            (yyval.node) = first;
            node* aux = (yyvsp[-1].node);
            node* curr = first;
            while(aux) {
                node* next_decl = create_node("VarDecl", NULL);
                add_child(next_decl, create_node((yyvsp[-3].node)->type, NULL)); 
                node* next_id = aux->next;
                aux->next = NULL;
                add_child(next_decl, aux);
                curr->next = next_decl;
                curr = next_decl;
                aux = next_id;
            }
         }
#line 1695 "y.tab.c"
    break;

  case 23: /* IdList: %empty  */
#line 219 "jucompiler.y"
                    { (yyval.node) = NULL; }
#line 1701 "y.tab.c"
    break;

  case 24: /* IdList: IdList COMMA IDENTIFIER  */
#line 220 "jucompiler.y"
                                { (yyval.node) = append_sibling((yyvsp[-2].node), create_node("Identifier", (yyvsp[0].str))); }
#line 1707 "y.tab.c"
    break;

  case 25: /* Type: BOOL  */
#line 223 "jucompiler.y"
           { (yyval.node) = create_node("Bool", NULL); }
#line 1713 "y.tab.c"
    break;

  case 26: /* Type: INT  */
#line 224 "jucompiler.y"
          { (yyval.node) = create_node("Int", NULL); }
#line 1719 "y.tab.c"
    break;

  case 27: /* Type: DOUBLE  */
#line 225 "jucompiler.y"
             { (yyval.node) = create_node("Double", NULL); }
#line 1725 "y.tab.c"
    break;

  case 28: /* Statement: LBRACE StatementList RBRACE  */
#line 228 "jucompiler.y"
                                       { 
            int count = 0;
            node* aux = (yyvsp[-1].node);
            while(aux) { count++; aux = aux->next; }
            if (count > 1) {
                (yyval.node) = create_node("Block", NULL);
                add_child((yyval.node), (yyvsp[-1].node));
            } else {
                (yyval.node) = (yyvsp[-1].node);
            }
         }
#line 1741 "y.tab.c"
    break;

  case 29: /* Statement: IF LPAR Expr RPAR Statement  */
#line 239 "jucompiler.y"
                                                             { 
            (yyval.node) = create_node("If", NULL);
            add_child((yyval.node), (yyvsp[-2].node));
            add_child((yyval.node), (yyvsp[0].node) ? (yyvsp[0].node) : create_node("Block", NULL));
            add_child((yyval.node), create_node("Block", NULL));
         }
#line 1752 "y.tab.c"
    break;

  case 30: /* Statement: IF LPAR Expr RPAR Statement ELSE Statement  */
#line 245 "jucompiler.y"
                                                      { 
            (yyval.node) = create_node("If", NULL);
            add_child((yyval.node), (yyvsp[-4].node));
            add_child((yyval.node), (yyvsp[-2].node) ? (yyvsp[-2].node) : create_node("Block", NULL));
            add_child((yyval.node), (yyvsp[0].node) ? (yyvsp[0].node) : create_node("Block", NULL));
         }
#line 1763 "y.tab.c"
    break;

  case 31: /* Statement: WHILE LPAR Expr RPAR Statement  */
#line 251 "jucompiler.y"
                                          { 
            (yyval.node) = create_node("While", NULL);
            add_child((yyval.node), (yyvsp[-2].node));
            add_child((yyval.node), (yyvsp[0].node) ? (yyvsp[0].node) : create_node("Block", NULL));
         }
#line 1773 "y.tab.c"
    break;

  case 32: /* Statement: RETURN SEMICOLON  */
#line 256 "jucompiler.y"
                            { (yyval.node) = create_node("Return", NULL); }
#line 1779 "y.tab.c"
    break;

  case 33: /* Statement: RETURN Expr SEMICOLON  */
#line 257 "jucompiler.y"
                                 { (yyval.node) = create_node("Return", NULL); add_child((yyval.node), (yyvsp[-1].node)); }
#line 1785 "y.tab.c"
    break;

  case 34: /* Statement: MethodInvocation SEMICOLON  */
#line 258 "jucompiler.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 1791 "y.tab.c"
    break;

  case 35: /* Statement: Assignment SEMICOLON  */
#line 259 "jucompiler.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 1797 "y.tab.c"
    break;

  case 36: /* Statement: ParseArgs SEMICOLON  */
#line 260 "jucompiler.y"
                               { (yyval.node) = (yyvsp[-1].node); }
#line 1803 "y.tab.c"
    break;

  case 37: /* Statement: SEMICOLON  */
#line 261 "jucompiler.y"
                     { (yyval.node) = NULL; }
#line 1809 "y.tab.c"
    break;

  case 38: /* Statement: PRINT LPAR Expr RPAR SEMICOLON  */
#line 262 "jucompiler.y"
                                          { (yyval.node) = create_node("Print", NULL); add_child((yyval.node), (yyvsp[-2].node)); }
#line 1815 "y.tab.c"
    break;

  case 39: /* Statement: PRINT LPAR STRLIT RPAR SEMICOLON  */
#line 263 "jucompiler.y"
                                            { (yyval.node) = create_node("Print", NULL); add_child((yyval.node), create_node("StrLit", (yyvsp[-2].str))); }
#line 1821 "y.tab.c"
    break;

  case 40: /* Statement: error SEMICOLON  */
#line 264 "jucompiler.y"
                           { (yyval.node) = NULL; }
#line 1827 "y.tab.c"
    break;

  case 41: /* StatementList: %empty  */
#line 267 "jucompiler.y"
                           { (yyval.node) = NULL; }
#line 1833 "y.tab.c"
    break;

  case 42: /* StatementList: StatementList Statement  */
#line 268 "jucompiler.y"
                                       { (yyval.node) = append_sibling((yyvsp[-1].node), (yyvsp[0].node)); }
#line 1839 "y.tab.c"
    break;

  case 43: /* MethodInvocation: IDENTIFIER LPAR RPAR  */
#line 271 "jucompiler.y"
                                       { 
                    (yyval.node) = create_node("Call", NULL);
                    add_child((yyval.node), create_node("Identifier", (yyvsp[-2].str)));
                }
#line 1848 "y.tab.c"
    break;

  case 44: /* MethodInvocation: IDENTIFIER LPAR ExprList RPAR  */
#line 275 "jucompiler.y"
                                                { 
                    (yyval.node) = create_node("Call", NULL);
                    add_child((yyval.node), create_node("Identifier", (yyvsp[-3].str)));
                    add_child((yyval.node), (yyvsp[-1].node));
                }
#line 1858 "y.tab.c"
    break;

  case 45: /* MethodInvocation: IDENTIFIER LPAR error RPAR  */
#line 280 "jucompiler.y"
                                             { (yyval.node) = NULL; }
#line 1864 "y.tab.c"
    break;

  case 46: /* ExprList: Expr  */
#line 283 "jucompiler.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1870 "y.tab.c"
    break;

  case 47: /* ExprList: ExprList COMMA Expr  */
#line 284 "jucompiler.y"
                              { (yyval.node) = append_sibling((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1876 "y.tab.c"
    break;

  case 48: /* Assignment: IDENTIFIER ASSIGN Expr  */
#line 287 "jucompiler.y"
                                   { 
    (yyval.node) = create_node("Assign", NULL);
    add_child((yyval.node), create_node("Identifier", (yyvsp[-2].str)));
    add_child((yyval.node), (yyvsp[0].node));
}
#line 1886 "y.tab.c"
    break;

  case 49: /* ParseArgs: PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR  */
#line 293 "jucompiler.y"
                                                      { 
    (yyval.node) = create_node("ParseArgs", NULL);
    add_child((yyval.node), create_node("Identifier", (yyvsp[-4].str)));
    add_child((yyval.node), (yyvsp[-2].node));
}
#line 1896 "y.tab.c"
    break;

  case 50: /* ParseArgs: PARSEINT LPAR error RPAR  */
#line 298 "jucompiler.y"
                                    { (yyval.node) = NULL; }
#line 1902 "y.tab.c"
    break;

  case 51: /* Expr: Assignment  */
#line 301 "jucompiler.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1908 "y.tab.c"
    break;

  case 52: /* Expr: OpExpr  */
#line 302 "jucompiler.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 1914 "y.tab.c"
    break;

  case 53: /* OpExpr: OpExpr PLUS OpExpr  */
#line 305 "jucompiler.y"
                           { (yyval.node) = create_node("Add", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 1920 "y.tab.c"
    break;

  case 54: /* OpExpr: OpExpr MINUS OpExpr  */
#line 306 "jucompiler.y"
                          { (yyval.node) = create_node("Sub", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 1926 "y.tab.c"
    break;

  case 55: /* OpExpr: OpExpr STAR OpExpr  */
#line 307 "jucompiler.y"
                         { (yyval.node) = create_node("Mul", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 1932 "y.tab.c"
    break;

  case 56: /* OpExpr: OpExpr DIV OpExpr  */
#line 308 "jucompiler.y"
                        { (yyval.node) = create_node("Div", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 1938 "y.tab.c"
    break;

  case 57: /* OpExpr: OpExpr MOD OpExpr  */
#line 309 "jucompiler.y"
                        { (yyval.node) = create_node("Mod", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 1944 "y.tab.c"
    break;

  case 58: /* OpExpr: OpExpr AND OpExpr  */
#line 310 "jucompiler.y"
                        { (yyval.node) = create_node("And", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 1950 "y.tab.c"
    break;

  case 59: /* OpExpr: OpExpr OR OpExpr  */
#line 311 "jucompiler.y"
                       { (yyval.node) = create_node("Or", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 1956 "y.tab.c"
    break;

  case 60: /* OpExpr: OpExpr XOR OpExpr  */
#line 312 "jucompiler.y"
                        { (yyval.node) = create_node("Xor", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 1962 "y.tab.c"
    break;

  case 61: /* OpExpr: OpExpr LSHIFT OpExpr  */
#line 313 "jucompiler.y"
                           { (yyval.node) = create_node("Lshift", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 1968 "y.tab.c"
    break;

  case 62: /* OpExpr: OpExpr RSHIFT OpExpr  */
#line 314 "jucompiler.y"
                           { (yyval.node) = create_node("Rshift", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 1974 "y.tab.c"
    break;

  case 63: /* OpExpr: OpExpr EQ OpExpr  */
#line 315 "jucompiler.y"
                       { (yyval.node) = create_node("Eq", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 1980 "y.tab.c"
    break;

  case 64: /* OpExpr: OpExpr NE OpExpr  */
#line 316 "jucompiler.y"
                       { (yyval.node) = create_node("Ne", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 1986 "y.tab.c"
    break;

  case 65: /* OpExpr: OpExpr GE OpExpr  */
#line 317 "jucompiler.y"
                       { (yyval.node) = create_node("Ge", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 1992 "y.tab.c"
    break;

  case 66: /* OpExpr: OpExpr GT OpExpr  */
#line 318 "jucompiler.y"
                       { (yyval.node) = create_node("Gt", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 1998 "y.tab.c"
    break;

  case 67: /* OpExpr: OpExpr LE OpExpr  */
#line 319 "jucompiler.y"
                       { (yyval.node) = create_node("Le", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2004 "y.tab.c"
    break;

  case 68: /* OpExpr: OpExpr LT OpExpr  */
#line 320 "jucompiler.y"
                       { (yyval.node) = create_node("Lt", NULL); add_child((yyval.node), (yyvsp[-2].node)); add_child((yyval.node), (yyvsp[0].node)); }
#line 2010 "y.tab.c"
    break;

  case 69: /* OpExpr: PLUS OpExpr  */
#line 321 "jucompiler.y"
                                   { (yyval.node) = create_node("Plus", NULL); add_child((yyval.node), (yyvsp[0].node)); }
#line 2016 "y.tab.c"
    break;

  case 70: /* OpExpr: MINUS OpExpr  */
#line 322 "jucompiler.y"
                                     { (yyval.node) = create_node("Minus", NULL); add_child((yyval.node), (yyvsp[0].node)); }
#line 2022 "y.tab.c"
    break;

  case 71: /* OpExpr: NOT OpExpr  */
#line 323 "jucompiler.y"
                 { (yyval.node) = create_node("Not", NULL); add_child((yyval.node), (yyvsp[0].node)); }
#line 2028 "y.tab.c"
    break;

  case 72: /* OpExpr: LPAR Expr RPAR  */
#line 324 "jucompiler.y"
                     { (yyval.node) = (yyvsp[-1].node); }
#line 2034 "y.tab.c"
    break;

  case 73: /* OpExpr: LPAR error RPAR  */
#line 325 "jucompiler.y"
                      { (yyval.node) = NULL; }
#line 2040 "y.tab.c"
    break;

  case 74: /* OpExpr: MethodInvocation  */
#line 326 "jucompiler.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2046 "y.tab.c"
    break;

  case 75: /* OpExpr: ParseArgs  */
#line 327 "jucompiler.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2052 "y.tab.c"
    break;

  case 76: /* OpExpr: IDENTIFIER  */
#line 328 "jucompiler.y"
                 { (yyval.node) = create_node("Identifier", (yyvsp[0].str)); }
#line 2058 "y.tab.c"
    break;

  case 77: /* OpExpr: IDENTIFIER DOTLENGTH  */
#line 329 "jucompiler.y"
                           { (yyval.node) = create_node("Length", NULL); add_child((yyval.node), create_node("Identifier", (yyvsp[-1].str))); }
#line 2064 "y.tab.c"
    break;

  case 78: /* OpExpr: NATURAL  */
#line 330 "jucompiler.y"
              { (yyval.node) = create_node("Natural", (yyvsp[0].str)); }
#line 2070 "y.tab.c"
    break;

  case 79: /* OpExpr: DECIMAL  */
#line 331 "jucompiler.y"
              { (yyval.node) = create_node("Decimal", (yyvsp[0].str)); }
#line 2076 "y.tab.c"
    break;

  case 80: /* OpExpr: BOOLLIT  */
#line 332 "jucompiler.y"
              { (yyval.node) = create_node("BoolLit", (yyvsp[0].str)); }
#line 2082 "y.tab.c"
    break;


#line 2086 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 335 "jucompiler.y"


void yyerror(char *s) {
    syntax_error = 1;
    
    if (yytext && yytext[0] == '"' && yytext[1] == '\0') 
    {
        printf("Line %d, col %d: %s: \"%s\"\n", cur_line, start_col, s, str_buffer);
    } else 
    {
        printf("Line %d, col %d: %s: %s\n", cur_line, start_col, s, yytext);
    }
    
    fflush(stdout); 
}

int main(int argc, char *argv[]) {
    if (argc > 1) {

        if (strcmp(argv[1], "-l") == 0) {
            print_tokens = 1;
            while (yylex()) ;
            return 0;
        } else if (strcmp(argv[1], "-e1") == 0) {
            print_tokens = 0;
            while (yylex()) ; 
            return 0;
        } else if (strcmp(argv[1], "-t") == 0) {
            yyparse(); // Chama o Yacc
            if (root && !syntax_error) print_tree(root, 0);
            return 0;
        } else if (strcmp(argv[1], "-e2") == 0) {
            yyparse();
            return 0;
        }
    }
    
    yyparse();
    return 0;
}
