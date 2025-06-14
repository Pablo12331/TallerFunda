/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 32 "parser.y"

  typedef struct Node Node;

#line 52 "parser.tab.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    FLOAT = 259,
    STRING_TYPE = 260,
    VOID = 261,
    EQ = 262,
    NE = 263,
    LT = 264,
    GT = 265,
    LE = 266,
    GE = 267,
    IF = 268,
    UNLESS = 269,
    ELSE = 270,
    WHILE = 271,
    FOR = 272,
    PRINT = 273,
    READ = 274,
    RETURN = 275,
    BREAK = 276,
    CONTINUE = 277,
    AND_OP = 278,
    OR_OP = 279,
    NOT_OP = 280,
    BOOL_TYPE = 281,
    PIPE = 282,
    LBRACE = 283,
    RBRACE = 284,
    LPAREN = 285,
    RPAREN = 286,
    SEMICOLON = 287,
    ASSIGN = 288,
    PLUS = 289,
    MINUS = 290,
    MULT = 291,
    DIV = 292,
    PLUS_ASSIGN = 293,
    MINUS_ASSIGN = 294,
    MULT_ASSIGN = 295,
    DIV_ASSIGN = 296,
    NUMBER = 297,
    FLOAT_NUMBER = 298,
    IDENTIFIER = 299,
    STRING_LITERAL = 300,
    TRUE_LITERAL = 301,
    FALSE_LITERAL = 302,
    UMINUS = 303
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 38 "parser.y"

  int ival;
  char *sval;
  double fval;
  Node *nodePtr;

#line 119 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
