/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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
    RETURN = 264,                  /* RETURN  */
    PARSEINT = 265,                /* PARSEINT  */
    DOTLENGTH = 266,               /* DOTLENGTH  */
    CLASS = 267,                   /* CLASS  */
    PUBLIC = 268,                  /* PUBLIC  */
    STATIC = 269,                  /* STATIC  */
    VOID = 270,                    /* VOID  */
    STRING = 271,                  /* STRING  */
    BOOL = 272,                    /* BOOL  */
    INT = 273,                     /* INT  */
    DOUBLE = 274,                  /* DOUBLE  */
    IF = 275,                      /* IF  */
    WHILE = 276,                   /* WHILE  */
    PRINT = 277,                   /* PRINT  */
    SEMICOLON = 278,               /* SEMICOLON  */
    COMMA = 279,                   /* COMMA  */
    LPAR = 280,                    /* LPAR  */
    RPAR = 281,                    /* RPAR  */
    LBRACE = 282,                  /* LBRACE  */
    RBRACE = 283,                  /* RBRACE  */
    LSQ = 284,                     /* LSQ  */
    RSQ = 285,                     /* RSQ  */
    ARROW = 286,                   /* ARROW  */
    ASSIGN = 287,                  /* ASSIGN  */
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
#define RETURN 264
#define PARSEINT 265
#define DOTLENGTH 266
#define CLASS 267
#define PUBLIC 268
#define STATIC 269
#define VOID 270
#define STRING 271
#define BOOL 272
#define INT 273
#define DOUBLE 274
#define IF 275
#define WHILE 276
#define PRINT 277
#define SEMICOLON 278
#define COMMA 279
#define LPAR 280
#define RPAR 281
#define LBRACE 282
#define RBRACE 283
#define LSQ 284
#define RSQ 285
#define ARROW 286
#define ASSIGN 287
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
#line 77 "jucompiler.y"
 //alterado para receber a linha e a coluna
    struct {
        char* str;
        int line;
        int col;
    } info;
    struct node* node;

#line 182 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
