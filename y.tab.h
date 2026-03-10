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

#line 178 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
