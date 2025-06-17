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
#line 2 "parser.y"

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "common_types.h"
#include "Ast.h" 
#include "symbol_table.h"
#include "type_checker.h"
#include "code_generator.h"

//Variables y funciones definidas fuera del parser (lex.yy.c)
extern int yylex();
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
void yyerror(const char *s);

//Puntero que guardara la raiz del arbol AST una vez completo todo el proceso del parser
Node* globalAstRoot = nullptr;

//Variable para recordar el tipo de variable en una declaracion multiple
Node* current_declaration_type_node = nullptr;


#line 99 "parser.tab.c"

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

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_FLOAT = 4,                      /* FLOAT  */
  YYSYMBOL_STRING_TYPE = 5,                /* STRING_TYPE  */
  YYSYMBOL_VOID = 6,                       /* VOID  */
  YYSYMBOL_EQ = 7,                         /* EQ  */
  YYSYMBOL_NE = 8,                         /* NE  */
  YYSYMBOL_LT = 9,                         /* LT  */
  YYSYMBOL_GT = 10,                        /* GT  */
  YYSYMBOL_LE = 11,                        /* LE  */
  YYSYMBOL_GE = 12,                        /* GE  */
  YYSYMBOL_IF = 13,                        /* IF  */
  YYSYMBOL_UNLESS = 14,                    /* UNLESS  */
  YYSYMBOL_ELSE = 15,                      /* ELSE  */
  YYSYMBOL_WHILE = 16,                     /* WHILE  */
  YYSYMBOL_FOR = 17,                       /* FOR  */
  YYSYMBOL_PRINT = 18,                     /* PRINT  */
  YYSYMBOL_READ = 19,                      /* READ  */
  YYSYMBOL_RETURN = 20,                    /* RETURN  */
  YYSYMBOL_BREAK = 21,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 22,                  /* CONTINUE  */
  YYSYMBOL_AND_OP = 23,                    /* AND_OP  */
  YYSYMBOL_OR_OP = 24,                     /* OR_OP  */
  YYSYMBOL_NOT_OP = 25,                    /* NOT_OP  */
  YYSYMBOL_BOOL_TYPE = 26,                 /* BOOL_TYPE  */
  YYSYMBOL_PIPE = 27,                      /* PIPE  */
  YYSYMBOL_LBRACE = 28,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 29,                    /* RBRACE  */
  YYSYMBOL_LPAREN = 30,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 31,                    /* RPAREN  */
  YYSYMBOL_SEMICOLON = 32,                 /* SEMICOLON  */
  YYSYMBOL_ASSIGN = 33,                    /* ASSIGN  */
  YYSYMBOL_34_ = 34,                       /* ','  */
  YYSYMBOL_PLUS = 35,                      /* PLUS  */
  YYSYMBOL_MINUS = 36,                     /* MINUS  */
  YYSYMBOL_MULT = 37,                      /* MULT  */
  YYSYMBOL_DIV = 38,                       /* DIV  */
  YYSYMBOL_PLUS_ASSIGN = 39,               /* PLUS_ASSIGN  */
  YYSYMBOL_MINUS_ASSIGN = 40,              /* MINUS_ASSIGN  */
  YYSYMBOL_MULT_ASSIGN = 41,               /* MULT_ASSIGN  */
  YYSYMBOL_DIV_ASSIGN = 42,                /* DIV_ASSIGN  */
  YYSYMBOL_NUMBER = 43,                    /* NUMBER  */
  YYSYMBOL_FLOAT_NUMBER = 44,              /* FLOAT_NUMBER  */
  YYSYMBOL_IDENTIFIER = 45,                /* IDENTIFIER  */
  YYSYMBOL_STRING_LITERAL = 46,            /* STRING_LITERAL  */
  YYSYMBOL_TRUE_LITERAL = 47,              /* TRUE_LITERAL  */
  YYSYMBOL_FALSE_LITERAL = 48,             /* FALSE_LITERAL  */
  YYSYMBOL_UMINUS = 49,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 50,                  /* $accept  */
  YYSYMBOL_program = 51,                   /* program  */
  YYSYMBOL_statement_list = 52,            /* statement_list  */
  YYSYMBOL_statement = 53,                 /* statement  */
  YYSYMBOL_compound_statement = 54,        /* compound_statement  */
  YYSYMBOL_type = 55,                      /* type  */
  YYSYMBOL_var_declaration_actual = 56,    /* var_declaration_actual  */
  YYSYMBOL_declarator_list_actual = 57,    /* declarator_list_actual  */
  YYSYMBOL_declarator_actual = 58,         /* declarator_actual  */
  YYSYMBOL_assignment = 59,                /* assignment  */
  YYSYMBOL_io_statement = 60,              /* io_statement  */
  YYSYMBOL_control_statement = 61,         /* control_statement  */
  YYSYMBOL_for_init = 62,                  /* for_init  */
  YYSYMBOL_opt_expression = 63,            /* opt_expression  */
  YYSYMBOL_function = 64,                  /* function  */
  YYSYMBOL_function_signature = 65,        /* function_signature  */
  YYSYMBOL_params_opt = 66,                /* params_opt  */
  YYSYMBOL_param = 67,                     /* param  */
  YYSYMBOL_param_list = 68,                /* param_list  */
  YYSYMBOL_statements_opt = 69,            /* statements_opt  */
  YYSYMBOL_term = 70,                      /* term  */
  YYSYMBOL_expression = 71,                /* expression  */
  YYSYMBOL_factor = 72,                    /* factor  */
  YYSYMBOL_args_opt = 73,                  /* args_opt  */
  YYSYMBOL_arg_list = 74                   /* arg_list  */
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
#define YYFINAL  62
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   624

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  85
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  168

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   303


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
       2,     2,     2,     2,    34,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    89,    89,    95,    97,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   120,   122,
     130,   134,   138,   142,   146,   155,   165,   169,   179,   190,
     207,   224,   234,   242,   250,   260,   261,   282,   284,   289,
     291,   293,   299,   303,   311,   323,   324,   329,   342,   378,
     379,   384,   393,   394,   398,   399,   404,   405,   406,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   450,   451,   452,   453,   454,   468,   469,   470,   471,
     487,   488,   493,   494,   499,   500
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
  "\"end of file\"", "error", "\"invalid token\"", "INT", "FLOAT",
  "STRING_TYPE", "VOID", "EQ", "NE", "LT", "GT", "LE", "GE", "IF",
  "UNLESS", "ELSE", "WHILE", "FOR", "PRINT", "READ", "RETURN", "BREAK",
  "CONTINUE", "AND_OP", "OR_OP", "NOT_OP", "BOOL_TYPE", "PIPE", "LBRACE",
  "RBRACE", "LPAREN", "RPAREN", "SEMICOLON", "ASSIGN", "','", "PLUS",
  "MINUS", "MULT", "DIV", "PLUS_ASSIGN", "MINUS_ASSIGN", "MULT_ASSIGN",
  "DIV_ASSIGN", "NUMBER", "FLOAT_NUMBER", "IDENTIFIER", "STRING_LITERAL",
  "TRUE_LITERAL", "FALSE_LITERAL", "UMINUS", "$accept", "program",
  "statement_list", "statement", "compound_statement", "type",
  "var_declaration_actual", "declarator_list_actual", "declarator_actual",
  "assignment", "io_statement", "control_statement", "for_init",
  "opt_expression", "function", "function_signature", "params_opt",
  "param", "param_list", "statements_opt", "term", "expression", "factor",
  "args_opt", "arg_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-128)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     385,  -128,  -128,  -128,  -128,   -19,   -17,   -11,    -9,    -4,
       1,   552,    -2,     2,   576,  -128,   169,   576,  -128,   576,
    -128,  -128,    39,  -128,  -128,  -128,    41,   385,  -128,  -128,
       7,    22,    24,    25,  -128,  -128,    30,   -13,    80,  -128,
     576,   576,   576,   419,   576,    20,  -128,  -128,   449,  -128,
    -128,  -128,  -128,   205,   111,  -128,   576,   576,   576,   576,
     576,   576,  -128,  -128,   -18,    33,  -128,  -128,  -128,  -128,
     385,   576,   576,   576,   576,   576,   576,   576,   576,   576,
     576,    -8,  -128,   576,   576,   479,   485,   515,    28,    42,
    -128,   551,   521,    46,   576,  -128,  -128,  -128,   551,    53,
      52,   551,   551,   551,   551,   551,    45,   576,    48,   385,
      65,  -128,  -128,   -13,   -13,   -13,   -13,   -13,   -13,    -3,
     557,    63,  -128,   -13,   -13,    68,    69,    71,    67,   576,
    -128,  -128,  -128,   576,    56,    74,  -128,    72,   551,    75,
    -128,  -128,  -128,   385,   385,   385,   576,    70,   551,  -128,
    -128,    45,   241,   277,   313,   551,   576,  -128,    98,  -128,
    -128,    83,   385,    89,  -128,   385,   349,  -128
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    20,    21,    22,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    24,     0,     0,    14,     0,
      71,    72,    75,    76,    73,    74,     0,     2,     3,    16,
       0,     0,    78,     0,     8,     9,     0,    59,     0,    56,
       0,     0,     0,    45,     0,     0,    11,    78,     0,    12,
      13,    80,    18,     0,     0,    81,    82,     0,     0,     0,
       0,     0,     1,     4,    28,    25,    26,     5,     6,     7,
      54,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    15,     0,     0,     0,     0,     0,     0,     0,
      42,    46,     0,     0,     0,    10,    19,    77,    84,     0,
      83,    30,    31,    32,    33,    34,    49,     0,     0,    55,
       0,    57,    58,    62,    63,    64,    65,    66,    67,    68,
      69,     0,    70,    60,    61,     0,     0,     0,    43,    45,
      35,    36,    79,     0,     0,     0,    52,    50,    29,    28,
      27,    47,    17,     0,     0,     0,     0,     0,    85,    51,
      48,     0,     0,     0,     0,    44,    45,    53,    37,    38,
      40,     0,     0,     0,    39,     0,     0,    41
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -128,  -128,   -15,    15,  -128,   -40,  -128,  -128,    17,     0,
    -128,  -128,  -128,  -127,  -128,  -128,  -128,   -25,  -128,  -128,
     363,     3,     4,  -128,  -128
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    26,    27,    28,    29,    30,    31,    65,    66,    47,
      33,    34,    89,    90,    35,    36,   135,   136,   137,   110,
      37,    38,    39,    99,   100
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      32,    53,   147,    88,    73,    74,    75,    76,    77,    78,
     121,    40,   106,    41,    48,   107,    32,    14,    51,    42,
      54,    43,    17,    55,    71,    72,    44,    32,    19,   161,
      49,    45,    83,    84,    50,    20,    21,    22,    23,    24,
      25,    62,    63,    85,    86,    87,    91,    92,     1,     2,
       3,     4,    64,    32,    67,   109,    68,    69,    70,    98,
     101,   102,   103,   104,   105,    93,   134,   108,    63,    56,
      32,    15,    57,   128,   129,   111,   112,   131,    58,    59,
      60,    61,   119,   120,   132,   122,   133,    73,    74,    75,
      76,    77,    78,   139,   141,   142,   143,   144,   122,   145,
     146,   149,   156,    79,    80,   150,   151,    81,   107,    32,
     138,   134,    82,   162,   163,    83,    84,   165,    73,    74,
      75,    76,    77,    78,    63,   140,   157,     0,   152,   153,
     154,     0,    91,     0,    79,    80,   148,     0,    94,     0,
       0,     0,    97,    32,    32,    32,    83,    84,     0,   155,
     166,     0,    32,    32,    32,     0,     0,     0,     0,    91,
       0,     0,    32,     0,     0,    32,    32,    63,    63,    63,
       0,     0,     1,     2,     3,     4,     0,   164,     0,     0,
       0,    63,     5,     6,     0,     7,     8,     9,    10,    11,
      12,    13,     0,     0,    14,    15,     0,    16,    52,    17,
       0,    18,     0,     0,     0,    19,     0,     0,     1,     2,
       3,     4,    20,    21,    22,    23,    24,    25,     5,     6,
       0,     7,     8,     9,    10,    11,    12,    13,     0,     0,
      14,    15,     0,    16,    96,    17,     0,    18,     0,     0,
       0,    19,     0,     0,     1,     2,     3,     4,    20,    21,
      22,    23,    24,    25,     5,     6,     0,     7,     8,     9,
      10,    11,    12,    13,     0,     0,    14,    15,     0,    16,
     158,    17,     0,    18,     0,     0,     0,    19,     0,     0,
       1,     2,     3,     4,    20,    21,    22,    23,    24,    25,
       5,     6,     0,     7,     8,     9,    10,    11,    12,    13,
       0,     0,    14,    15,     0,    16,   159,    17,     0,    18,
       0,     0,     0,    19,     0,     0,     1,     2,     3,     4,
      20,    21,    22,    23,    24,    25,     5,     6,     0,     7,
       8,     9,    10,    11,    12,    13,     0,     0,    14,    15,
       0,    16,   160,    17,     0,    18,     0,     0,     0,    19,
       0,     0,     1,     2,     3,     4,    20,    21,    22,    23,
      24,    25,     5,     6,     0,     7,     8,     9,    10,    11,
      12,    13,     0,     0,    14,    15,     0,    16,   167,    17,
       0,    18,     0,     0,     0,    19,     0,     0,     1,     2,
       3,     4,    20,    21,    22,    23,    24,    25,     5,     6,
       0,     7,     8,     9,    10,    11,    12,    13,     0,     0,
      14,    15,     0,    16,     0,    17,     0,    18,     0,     0,
       0,    19,     1,     2,     3,     4,     0,     0,    20,    21,
      22,    23,    24,    25,     0,     0,   113,   114,   115,   116,
     117,   118,     0,     0,    14,    15,   123,   124,     0,    17,
       0,     0,     0,     0,     0,    19,    73,    74,    75,    76,
      77,    78,    20,    21,    22,    23,    24,    25,     0,     0,
       0,     0,    79,    80,     0,     0,    94,     0,     0,     0,
       0,    95,     0,     0,    83,    84,    73,    74,    75,    76,
      77,    78,    73,    74,    75,    76,    77,    78,     0,     0,
       0,     0,    79,    80,     0,     0,    94,     0,    79,    80,
     125,     0,    94,     0,    83,    84,   126,     0,     0,     0,
      83,    84,    73,    74,    75,    76,    77,    78,    73,    74,
      75,    76,    77,    78,     0,     0,     0,     0,    79,    80,
       0,     0,    94,     0,    79,    80,   127,     0,    94,     0,
      83,    84,   130,     0,     0,     0,    83,    84,    73,    74,
      75,    76,    77,    78,    73,    74,    75,    76,    77,    78,
       0,     0,     0,     0,    79,    80,     0,    14,    94,     0,
      79,     0,    17,     0,    46,     0,    83,    84,    19,     0,
       0,     0,    83,    84,     0,    20,    21,    22,    23,    24,
      25,    14,     0,     0,     0,     0,    17,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,    20,
      21,    22,    23,    24,    25
};

static const yytype_int16 yycheck[] =
{
       0,    16,   129,    43,     7,     8,     9,    10,    11,    12,
      18,    30,    30,    30,    11,    33,    16,    25,    14,    30,
      17,    30,    30,    19,    37,    38,    30,    27,    36,   156,
      32,    30,    35,    36,    32,    43,    44,    45,    46,    47,
      48,     0,    27,    40,    41,    42,    43,    44,     3,     4,
       5,     6,    45,    53,    32,    70,    32,    32,    28,    56,
      57,    58,    59,    60,    61,    45,   106,    34,    53,    30,
      70,    26,    33,    45,    32,    71,    72,    31,    39,    40,
      41,    42,    79,    80,    31,    81,    34,     7,     8,     9,
      10,    11,    12,    45,    29,    32,    28,    28,    94,    28,
      33,    45,    32,    23,    24,    31,    34,    27,    33,   109,
     107,   151,    32,    15,    31,    35,    36,    28,     7,     8,
       9,    10,    11,    12,   109,   108,   151,    -1,   143,   144,
     145,    -1,   129,    -1,    23,    24,   133,    -1,    27,    -1,
      -1,    -1,    31,   143,   144,   145,    35,    36,    -1,   146,
     165,    -1,   152,   153,   154,    -1,    -1,    -1,    -1,   156,
      -1,    -1,   162,    -1,    -1,   165,   166,   152,   153,   154,
      -1,    -1,     3,     4,     5,     6,    -1,   162,    -1,    -1,
      -1,   166,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    25,    26,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    36,    -1,    -1,     3,     4,
       5,     6,    43,    44,    45,    46,    47,    48,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      25,    26,    -1,    28,    29,    30,    -1,    32,    -1,    -1,
      -1,    36,    -1,    -1,     3,     4,     5,     6,    43,    44,
      45,    46,    47,    48,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    25,    26,    -1,    28,
      29,    30,    -1,    32,    -1,    -1,    -1,    36,    -1,    -1,
       3,     4,     5,     6,    43,    44,    45,    46,    47,    48,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    25,    26,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    36,    -1,    -1,     3,     4,     5,     6,
      43,    44,    45,    46,    47,    48,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    25,    26,
      -1,    28,    29,    30,    -1,    32,    -1,    -1,    -1,    36,
      -1,    -1,     3,     4,     5,     6,    43,    44,    45,    46,
      47,    48,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    25,    26,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    36,    -1,    -1,     3,     4,
       5,     6,    43,    44,    45,    46,    47,    48,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      25,    26,    -1,    28,    -1,    30,    -1,    32,    -1,    -1,
      -1,    36,     3,     4,     5,     6,    -1,    -1,    43,    44,
      45,    46,    47,    48,    -1,    -1,    73,    74,    75,    76,
      77,    78,    -1,    -1,    25,    26,    83,    84,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,     7,     8,     9,    10,
      11,    12,    43,    44,    45,    46,    47,    48,    -1,    -1,
      -1,    -1,    23,    24,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    35,    36,     7,     8,     9,    10,
      11,    12,     7,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    -1,    23,    24,    -1,    -1,    27,    -1,    23,    24,
      31,    -1,    27,    -1,    35,    36,    31,    -1,    -1,    -1,
      35,    36,     7,     8,     9,    10,    11,    12,     7,     8,
       9,    10,    11,    12,    -1,    -1,    -1,    -1,    23,    24,
      -1,    -1,    27,    -1,    23,    24,    31,    -1,    27,    -1,
      35,    36,    31,    -1,    -1,    -1,    35,    36,     7,     8,
       9,    10,    11,    12,     7,     8,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    23,    24,    -1,    25,    27,    -1,
      23,    -1,    30,    -1,    32,    -1,    35,    36,    36,    -1,
      -1,    -1,    35,    36,    -1,    43,    44,    45,    46,    47,
      48,    25,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    43,
      44,    45,    46,    47,    48
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,    13,    14,    16,    17,    18,
      19,    20,    21,    22,    25,    26,    28,    30,    32,    36,
      43,    44,    45,    46,    47,    48,    51,    52,    53,    54,
      55,    56,    59,    60,    61,    64,    65,    70,    71,    72,
      30,    30,    30,    30,    30,    30,    32,    59,    71,    32,
      32,    72,    29,    52,    71,    72,    30,    33,    39,    40,
      41,    42,     0,    53,    45,    57,    58,    32,    32,    32,
      28,    37,    38,     7,     8,     9,    10,    11,    12,    23,
      24,    27,    32,    35,    36,    71,    71,    71,    55,    62,
      63,    71,    71,    45,    27,    32,    29,    31,    71,    73,
      74,    71,    71,    71,    71,    71,    30,    33,    34,    52,
      69,    72,    72,    70,    70,    70,    70,    70,    70,    71,
      71,    18,    72,    70,    70,    31,    31,    31,    45,    32,
      31,    31,    31,    34,    55,    66,    67,    68,    71,    45,
      58,    29,    32,    28,    28,    28,    33,    63,    71,    45,
      31,    34,    52,    52,    52,    71,    32,    67,    29,    29,
      29,    63,    15,    31,    53,    28,    52,    29
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    50,    51,    52,    52,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    54,    54,
      55,    55,    55,    55,    55,    56,    57,    57,    58,    58,
      59,    59,    59,    59,    59,    60,    60,    61,    61,    61,
      61,    61,    62,    62,    62,    63,    63,    64,    65,    66,
      66,    67,    68,    68,    69,    69,    70,    70,    70,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    73,    73,    74,    74
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     2,     2,     1,     1,
       3,     2,     2,     2,     1,     2,     1,     4,     2,     3,
       1,     1,     1,     1,     1,     2,     1,     3,     1,     3,
       3,     3,     3,     3,     3,     4,     4,     7,     7,     9,
       7,    11,     1,     2,     4,     0,     1,     4,     5,     0,
       1,     2,     1,     3,     0,     1,     1,     3,     3,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     3,     1,     4,
       2,     2,     0,     1,     1,     3
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
  case 2: /* program: statement_list  */
#line 90 "parser.y"
    { (yyval.nodePtr) = (yyvsp[0].nodePtr); globalAstRoot = (yyval.nodePtr); }
#line 1363 "parser.tab.c"
    break;

  case 3: /* statement_list: statement  */
#line 96 "parser.y"
      { (yyval.nodePtr) = createStatementListNode((yyvsp[0].nodePtr), (yyvsp[0].nodePtr) ? (yyvsp[0].nodePtr)->line : yylineno); }
#line 1369 "parser.tab.c"
    break;

  case 4: /* statement_list: statement_list statement  */
#line 98 "parser.y"
      { (yyval.nodePtr) = addStatementToList((yyvsp[-1].nodePtr), (yyvsp[0].nodePtr)); }
#line 1375 "parser.tab.c"
    break;

  case 5: /* statement: var_declaration_actual SEMICOLON  */
#line 103 "parser.y"
                                       { (yyval.nodePtr) = (yyvsp[-1].nodePtr); }
#line 1381 "parser.tab.c"
    break;

  case 6: /* statement: assignment SEMICOLON  */
#line 104 "parser.y"
                                { (yyval.nodePtr) = (yyvsp[-1].nodePtr); }
#line 1387 "parser.tab.c"
    break;

  case 7: /* statement: io_statement SEMICOLON  */
#line 105 "parser.y"
                                { (yyval.nodePtr) = (yyvsp[-1].nodePtr); }
#line 1393 "parser.tab.c"
    break;

  case 8: /* statement: control_statement  */
#line 106 "parser.y"
                              { (yyval.nodePtr) = (yyvsp[0].nodePtr); }
#line 1399 "parser.tab.c"
    break;

  case 9: /* statement: function  */
#line 107 "parser.y"
                              { (yyval.nodePtr) = (yyvsp[0].nodePtr); }
#line 1405 "parser.tab.c"
    break;

  case 10: /* statement: RETURN expression SEMICOLON  */
#line 108 "parser.y"
                                  { (yyval.nodePtr) = createReturnNode((yyvsp[-1].nodePtr), yylineno); }
#line 1411 "parser.tab.c"
    break;

  case 11: /* statement: RETURN SEMICOLON  */
#line 109 "parser.y"
                                { (yyval.nodePtr) = createReturnNode(nullptr, yylineno); }
#line 1417 "parser.tab.c"
    break;

  case 12: /* statement: BREAK SEMICOLON  */
#line 110 "parser.y"
                         { (yyval.nodePtr) = createBreakNode(yylineno); }
#line 1423 "parser.tab.c"
    break;

  case 13: /* statement: CONTINUE SEMICOLON  */
#line 111 "parser.y"
                         { (yyval.nodePtr) = createContinueNode(yylineno); }
#line 1429 "parser.tab.c"
    break;

  case 14: /* statement: SEMICOLON  */
#line 112 "parser.y"
                                { (yyval.nodePtr) = nullptr; }
#line 1435 "parser.tab.c"
    break;

  case 15: /* statement: expression SEMICOLON  */
#line 113 "parser.y"
                                { (yyval.nodePtr) = (yyvsp[-1].nodePtr); }
#line 1441 "parser.tab.c"
    break;

  case 16: /* statement: compound_statement  */
#line 114 "parser.y"
                              { (yyval.nodePtr) = (yyvsp[0].nodePtr); }
#line 1447 "parser.tab.c"
    break;

  case 17: /* statement: expression PIPE PRINT SEMICOLON  */
#line 115 "parser.y"
                                     { (yyval.nodePtr) = createPrintNode((yyvsp[-3].nodePtr), yylineno); }
#line 1453 "parser.tab.c"
    break;

  case 18: /* compound_statement: LBRACE RBRACE  */
#line 121 "parser.y"
        { (yyval.nodePtr) = createStatementListNode(nullptr, yylineno); }
#line 1459 "parser.tab.c"
    break;

  case 19: /* compound_statement: LBRACE statement_list RBRACE  */
#line 123 "parser.y"
        { (yyval.nodePtr) = (yyvsp[-1].nodePtr); }
#line 1465 "parser.tab.c"
    break;

  case 20: /* type: INT  */
#line 130 "parser.y"
            { (yyval.nodePtr) = createIdentifierNode(strdup("int"), yylineno); 
              ((Node*)(yyval.nodePtr))->type = NODE_TYPE; 
              current_declaration_type_node = (yyval.nodePtr);
            }
#line 1474 "parser.tab.c"
    break;

  case 21: /* type: FLOAT  */
#line 134 "parser.y"
            { (yyval.nodePtr) = createIdentifierNode(strdup("float"), yylineno); 
              ((Node*)(yyval.nodePtr))->type = NODE_TYPE; 
              current_declaration_type_node = (yyval.nodePtr);
            }
#line 1483 "parser.tab.c"
    break;

  case 22: /* type: STRING_TYPE  */
#line 138 "parser.y"
                  { (yyval.nodePtr) = createIdentifierNode(strdup("string"), yylineno); 
                    ((Node*)(yyval.nodePtr))->type = NODE_TYPE;
                    current_declaration_type_node = (yyval.nodePtr); 
                  }
#line 1492 "parser.tab.c"
    break;

  case 23: /* type: VOID  */
#line 142 "parser.y"
            { (yyval.nodePtr) = createIdentifierNode(strdup("void"), yylineno); 
              ((Node*)(yyval.nodePtr))->type = NODE_TYPE; 
              current_declaration_type_node = (yyval.nodePtr);
            }
#line 1501 "parser.tab.c"
    break;

  case 24: /* type: BOOL_TYPE  */
#line 146 "parser.y"
                { (yyval.nodePtr) = createIdentifierNode(strdup("bool"), yylineno); 
                  ((Node*)(yyval.nodePtr))->type = NODE_TYPE; 
                  current_declaration_type_node = (yyval.nodePtr);
                }
#line 1510 "parser.tab.c"
    break;

  case 25: /* var_declaration_actual: type declarator_list_actual  */
#line 157 "parser.y"
    {
        (yyval.nodePtr) = (yyvsp[0].nodePtr); 
        current_declaration_type_node = nullptr;
    }
#line 1519 "parser.tab.c"
    break;

  case 26: /* declarator_list_actual: declarator_actual  */
#line 166 "parser.y"
    {
        (yyval.nodePtr) = createStatementListNode((yyvsp[0].nodePtr), (yyvsp[0].nodePtr) ? (yyvsp[0].nodePtr)->line : yylineno);
    }
#line 1527 "parser.tab.c"
    break;

  case 27: /* declarator_list_actual: declarator_list_actual ',' declarator_actual  */
#line 170 "parser.y"
    {
        (yyval.nodePtr) = addStatementToList((yyvsp[-2].nodePtr), (yyvsp[0].nodePtr));
    }
#line 1535 "parser.tab.c"
    break;

  case 28: /* declarator_actual: IDENTIFIER  */
#line 180 "parser.y"
    {
        Node* id_node_for_ast = createIdentifierNode((yyvsp[0].sval), yylineno);
        Node* type_copy = duplicateTypeNode(current_declaration_type_node);
        (yyval.nodePtr) = createVarDeclarationNode(type_copy, id_node_for_ast, nullptr, yylineno);

        if (!c_insert_variable((yyvsp[0].sval), type_copy, yylineno, (yyval.nodePtr))) 
        {
            yyerror(("Error Semántico: Re-declaración de variable '" + std::string((yyvsp[0].sval)) + "'").c_str());
        }
    }
#line 1550 "parser.tab.c"
    break;

  case 29: /* declarator_actual: IDENTIFIER ASSIGN expression  */
#line 191 "parser.y"
    {
        Node* id_node_for_ast = createIdentifierNode((yyvsp[-2].sval), yylineno);
        Node* type_copy = duplicateTypeNode(current_declaration_type_node);
        (yyval.nodePtr) = createVarDeclarationNode(type_copy, id_node_for_ast, (yyvsp[0].nodePtr), yylineno);

        if (!c_insert_variable((yyvsp[-2].sval), type_copy, yylineno, (yyval.nodePtr))) 
        {
            yyerror(("Error Semántico: Re-declaración de variable '" + std::string((yyvsp[-2].sval)) + "'").c_str());
        }
    }
#line 1565 "parser.tab.c"
    break;

  case 30: /* assignment: IDENTIFIER ASSIGN expression  */
#line 208 "parser.y"
    { 
      C_SymbolInfo info;
      if (!c_lookup_symbol((yyvsp[-2].sval), &info))  
      {
        std::string err_msg = "Error Semántico: Identificador no declarado '" + std::string((yyvsp[-2].sval)) + "'";
        yyerror(err_msg.c_str());
      } 
      else if (info.kind == NODE_FUNCTION_DEF) 
      {
        std::string err_msg = "Error Semántico: '" + std::string((yyvsp[-2].sval)) + "' es una función, no se puede asignar.";
        yyerror(err_msg.c_str());
      }

      Node* id_node_for_ast = createIdentifierNode((yyvsp[-2].sval), yylineno);
      (yyval.nodePtr) = createAssignmentNode(id_node_for_ast, (yyvsp[0].nodePtr), yylineno);     
    }
#line 1586 "parser.tab.c"
    break;

  case 31: /* assignment: IDENTIFIER PLUS_ASSIGN expression  */
#line 225 "parser.y"
    {
        Node* id_node_lhs = createIdentifierNode((yyvsp[-2].sval), yylineno);
        Node* id_node_rhs = createIdentifierNode(strdup((yyvsp[-2].sval)), yylineno);

        Node* binary_op = createBinaryOpNode(PLUS, id_node_rhs, (yyvsp[0].nodePtr), yylineno);

        (yyval.nodePtr) = createAssignmentNode(id_node_lhs, binary_op, yylineno);
    }
#line 1599 "parser.tab.c"
    break;

  case 32: /* assignment: IDENTIFIER MINUS_ASSIGN expression  */
#line 235 "parser.y"
    {
        Node* id_node_lhs = createIdentifierNode((yyvsp[-2].sval), yylineno);
        Node* id_node_rhs = createIdentifierNode(strdup((yyvsp[-2].sval)), yylineno);
        Node* binary_op = createBinaryOpNode(MINUS, id_node_rhs, (yyvsp[0].nodePtr), yylineno);
        (yyval.nodePtr) = createAssignmentNode(id_node_lhs, binary_op, yylineno);
    }
#line 1610 "parser.tab.c"
    break;

  case 33: /* assignment: IDENTIFIER MULT_ASSIGN expression  */
#line 243 "parser.y"
    {
        Node* id_node_lhs = createIdentifierNode((yyvsp[-2].sval), yylineno);
        Node* id_node_rhs = createIdentifierNode(strdup((yyvsp[-2].sval)), yylineno);
        Node* binary_op = createBinaryOpNode(MULT, id_node_rhs, (yyvsp[0].nodePtr), yylineno);
        (yyval.nodePtr) = createAssignmentNode(id_node_lhs, binary_op, yylineno);
    }
#line 1621 "parser.tab.c"
    break;

  case 34: /* assignment: IDENTIFIER DIV_ASSIGN expression  */
#line 251 "parser.y"
    {
        Node* id_node_lhs = createIdentifierNode((yyvsp[-2].sval), yylineno);
        Node* id_node_rhs = createIdentifierNode(strdup((yyvsp[-2].sval)), yylineno);
        Node* binary_op = createBinaryOpNode(DIV, id_node_rhs, (yyvsp[0].nodePtr), yylineno);
        (yyval.nodePtr) = createAssignmentNode(id_node_lhs, binary_op, yylineno);
    }
#line 1632 "parser.tab.c"
    break;

  case 35: /* io_statement: PRINT LPAREN expression RPAREN  */
#line 260 "parser.y"
                                     { (yyval.nodePtr) = createPrintNode((yyvsp[-1].nodePtr), yylineno); }
#line 1638 "parser.tab.c"
    break;

  case 36: /* io_statement: READ LPAREN IDENTIFIER RPAREN  */
#line 262 "parser.y"
    { 
      C_SymbolInfo info;
      if (!c_lookup_symbol((yyvsp[-1].sval), &info)) 
      {
        std::string err_msg = "Error Semántico: Identificador no declarado '" + std::string((yyvsp[-1].sval)) + "' para READ";
        yyerror(err_msg.c_str());
      } 
      else if (info.kind == NODE_FUNCTION_DEF) 
      {
        std::string err_msg = "Error Semántico: No se puede leer en una función '" + std::string((yyvsp[-1].sval)) + "'";
        yyerror(err_msg.c_str());
      }
      
      Node* id_node_for_ast = createIdentifierNode((yyvsp[-1].sval), yylineno);
      (yyval.nodePtr) = createReadNode(id_node_for_ast, yylineno);
    }
#line 1659 "parser.tab.c"
    break;

  case 37: /* control_statement: IF LPAREN expression RPAREN LBRACE statement_list RBRACE  */
#line 283 "parser.y"
      { (yyval.nodePtr) = createIfNode((yyvsp[-4].nodePtr), (yyvsp[-1].nodePtr), nullptr, yylineno); }
#line 1665 "parser.tab.c"
    break;

  case 38: /* control_statement: UNLESS LPAREN expression RPAREN LBRACE statement_list RBRACE  */
#line 285 "parser.y"
      {
        Node* negated_condition = createUnaryOpNode(NOT_OP, (yyvsp[-4].nodePtr), yylineno);
        (yyval.nodePtr) = createIfNode(negated_condition, (yyvsp[-1].nodePtr), nullptr, yylineno);
      }
#line 1674 "parser.tab.c"
    break;

  case 39: /* control_statement: IF LPAREN expression RPAREN LBRACE statement_list RBRACE ELSE statement  */
#line 290 "parser.y"
    { (yyval.nodePtr) = createIfNode((yyvsp[-6].nodePtr), (yyvsp[-3].nodePtr), (yyvsp[0].nodePtr), yylineno); }
#line 1680 "parser.tab.c"
    break;

  case 40: /* control_statement: WHILE LPAREN expression RPAREN LBRACE statement_list RBRACE  */
#line 292 "parser.y"
    { (yyval.nodePtr) = createWhileNode((yyvsp[-4].nodePtr), (yyvsp[-1].nodePtr), yylineno); }
#line 1686 "parser.tab.c"
    break;

  case 41: /* control_statement: FOR LPAREN for_init SEMICOLON opt_expression SEMICOLON opt_expression RPAREN LBRACE statement_list RBRACE  */
#line 294 "parser.y"
    { (yyval.nodePtr) = createForNode((yyvsp[-8].nodePtr), (yyvsp[-6].nodePtr), (yyvsp[-4].nodePtr), (yyvsp[-1].nodePtr), yylineno); }
#line 1692 "parser.tab.c"
    break;

  case 42: /* for_init: opt_expression  */
#line 300 "parser.y"
      {
          (yyval.nodePtr) = (yyvsp[0].nodePtr);
      }
#line 1700 "parser.tab.c"
    break;

  case 43: /* for_init: type IDENTIFIER  */
#line 304 "parser.y"
      {
          Node* id_node = createIdentifierNode((yyvsp[0].sval), yylineno);
          (yyval.nodePtr) = createVarDeclarationNode((yyvsp[-1].nodePtr), id_node, nullptr, yylineno);
          if (!c_insert_variable((yyvsp[0].sval), (yyvsp[-1].nodePtr), yylineno, (yyval.nodePtr))) {
              yyerror(("Error Semántico: Re-declaración de variable '" + std::string((yyvsp[0].sval)) + "' en for.").c_str());
          }
      }
#line 1712 "parser.tab.c"
    break;

  case 44: /* for_init: type IDENTIFIER ASSIGN expression  */
#line 312 "parser.y"
      {
          Node* id_node = createIdentifierNode((yyvsp[-2].sval), yylineno);
          (yyval.nodePtr) = createVarDeclarationNode((yyvsp[-3].nodePtr), id_node, (yyvsp[0].nodePtr), yylineno);
          if (!c_insert_variable((yyvsp[-2].sval), (yyvsp[-3].nodePtr), yylineno, (yyval.nodePtr))) {
              yyerror(("Error Semántico: Re-declaración de variable '" + std::string((yyvsp[-2].sval)) + "' en for.").c_str());
          }
      }
#line 1724 "parser.tab.c"
    break;

  case 45: /* opt_expression: %empty  */
#line 323 "parser.y"
                  { (yyval.nodePtr) = nullptr; }
#line 1730 "parser.tab.c"
    break;

  case 46: /* opt_expression: expression  */
#line 324 "parser.y"
                  { (yyval.nodePtr) = (yyvsp[0].nodePtr); }
#line 1736 "parser.tab.c"
    break;

  case 47: /* function: function_signature LBRACE statements_opt RBRACE  */
#line 330 "parser.y"
    { 
      if ((yyvsp[-3].nodePtr) != nullptr) { 
          (yyval.nodePtr) = createFunctionDefNode((yyvsp[-3].nodePtr), (yyvsp[-1].nodePtr), yylineno);
          c_exit_scope();
      } else {
          (yyval.nodePtr) = nullptr; 
      }
    }
#line 1749 "parser.tab.c"
    break;

  case 48: /* function_signature: type IDENTIFIER LPAREN params_opt RPAREN  */
#line 343 "parser.y"
    { 
      Node* func_name_node_for_ast = createIdentifierNode((yyvsp[-3].sval), yylineno); 
      Node* params_node = (yyvsp[-1].nodePtr);
      Node* sig_node = createFunctionSignatureNode((yyvsp[-4].nodePtr), func_name_node_for_ast, params_node, yylineno);

      if (!c_insert_function((yyvsp[-3].sval), (yyvsp[-4].nodePtr), params_node, yylineno, sig_node)) {
        std::string err_msg = "Error Semántico: Re-declaración de función '" + std::string((yyvsp[-3].sval)) + "'";
        yyerror(err_msg.c_str());
        (yyval.nodePtr) = nullptr; 
      } else {
        Node* current_param_ast_node = params_node;
        while (current_param_ast_node != nullptr) {
            if (current_param_ast_node->type == NODE_VAR_DECLARATION) {
                VarDeclarationNode* p_decl_node = (VarDeclarationNode*)current_param_ast_node;
                IdentifierNode* p_id_node = p_decl_node->identifier;
                Node* p_type_node = p_decl_node->varTypeNode;  

                if (p_id_node && p_id_node->sval && p_type_node) {
                    if (!c_insert_variable(p_id_node->sval, p_type_node, p_decl_node->line, (Node*)p_decl_node)) {
                         std::string err_msg = "Error Semántico: Re-declaración del parámetro '" + 
                                               std::string(p_id_node->sval) + "' en la firma de la función '" + 
                                               std::string((yyvsp[-3].sval)) + "'.";
                         yyerror(err_msg.c_str());
                    }
                }
            }
            current_param_ast_node = current_param_ast_node->next;
        }
        (yyval.nodePtr) = sig_node;
      }
    }
#line 1785 "parser.tab.c"
    break;

  case 49: /* params_opt: %empty  */
#line 378 "parser.y"
             { (yyval.nodePtr) = nullptr; }
#line 1791 "parser.tab.c"
    break;

  case 50: /* params_opt: param_list  */
#line 379 "parser.y"
                 { (yyval.nodePtr) = (yyvsp[0].nodePtr); }
#line 1797 "parser.tab.c"
    break;

  case 51: /* param: type IDENTIFIER  */
#line 385 "parser.y"
    { 
      Node* param_id_node_for_ast = createIdentifierNode((yyvsp[0].sval), yylineno);
      (yyval.nodePtr) = createVarDeclarationNode((yyvsp[-1].nodePtr), param_id_node_for_ast, nullptr, yylineno);
    }
#line 1806 "parser.tab.c"
    break;

  case 52: /* param_list: param  */
#line 393 "parser.y"
            { (yyval.nodePtr) = createParamList((yyvsp[0].nodePtr)); }
#line 1812 "parser.tab.c"
    break;

  case 53: /* param_list: param_list ',' param  */
#line 394 "parser.y"
                           { (yyval.nodePtr) = addParamToList((yyvsp[-2].nodePtr), (yyvsp[0].nodePtr)); }
#line 1818 "parser.tab.c"
    break;

  case 54: /* statements_opt: %empty  */
#line 398 "parser.y"
             { (yyval.nodePtr) = nullptr; }
#line 1824 "parser.tab.c"
    break;

  case 55: /* statements_opt: statement_list  */
#line 399 "parser.y"
                     { (yyval.nodePtr) = (yyvsp[0].nodePtr); }
#line 1830 "parser.tab.c"
    break;

  case 56: /* term: factor  */
#line 404 "parser.y"
                       { (yyval.nodePtr) = (yyvsp[0].nodePtr); }
#line 1836 "parser.tab.c"
    break;

  case 57: /* term: term MULT factor  */
#line 405 "parser.y"
                       { (yyval.nodePtr) = createBinaryOpNode(MULT, (yyvsp[-2].nodePtr), (yyvsp[0].nodePtr), yylineno); }
#line 1842 "parser.tab.c"
    break;

  case 58: /* term: term DIV factor  */
#line 406 "parser.y"
                       { (yyval.nodePtr) = createBinaryOpNode(DIV, (yyvsp[-2].nodePtr), (yyvsp[0].nodePtr), yylineno); }
#line 1848 "parser.tab.c"
    break;

  case 59: /* expression: term  */
#line 411 "parser.y"
                            { (yyval.nodePtr) = (yyvsp[0].nodePtr); }
#line 1854 "parser.tab.c"
    break;

  case 60: /* expression: expression PLUS term  */
#line 412 "parser.y"
                            { (yyval.nodePtr) = createBinaryOpNode(PLUS, (yyvsp[-2].nodePtr), (yyvsp[0].nodePtr), yylineno); }
#line 1860 "parser.tab.c"
    break;

  case 61: /* expression: expression MINUS term  */
#line 413 "parser.y"
                            { (yyval.nodePtr) = createBinaryOpNode(MINUS, (yyvsp[-2].nodePtr), (yyvsp[0].nodePtr), yylineno); }
#line 1866 "parser.tab.c"
    break;

  case 62: /* expression: expression EQ term  */
#line 414 "parser.y"
                            { (yyval.nodePtr) = createBinaryOpNode(EQ, (yyvsp[-2].nodePtr), (yyvsp[0].nodePtr), yylineno); }
#line 1872 "parser.tab.c"
    break;

  case 63: /* expression: expression NE term  */
#line 415 "parser.y"
                            { (yyval.nodePtr) = createBinaryOpNode(NE, (yyvsp[-2].nodePtr), (yyvsp[0].nodePtr), yylineno); }
#line 1878 "parser.tab.c"
    break;

  case 64: /* expression: expression LT term  */
#line 416 "parser.y"
                            { (yyval.nodePtr) = createBinaryOpNode(LT, (yyvsp[-2].nodePtr), (yyvsp[0].nodePtr), yylineno); }
#line 1884 "parser.tab.c"
    break;

  case 65: /* expression: expression GT term  */
#line 417 "parser.y"
                            { (yyval.nodePtr) = createBinaryOpNode(GT, (yyvsp[-2].nodePtr), (yyvsp[0].nodePtr), yylineno); }
#line 1890 "parser.tab.c"
    break;

  case 66: /* expression: expression LE term  */
#line 418 "parser.y"
                            { (yyval.nodePtr) = createBinaryOpNode(LE, (yyvsp[-2].nodePtr), (yyvsp[0].nodePtr), yylineno); }
#line 1896 "parser.tab.c"
    break;

  case 67: /* expression: expression GE term  */
#line 419 "parser.y"
                            { (yyval.nodePtr) = createBinaryOpNode(GE, (yyvsp[-2].nodePtr), (yyvsp[0].nodePtr), yylineno); }
#line 1902 "parser.tab.c"
    break;

  case 68: /* expression: expression AND_OP expression  */
#line 420 "parser.y"
                                   { (yyval.nodePtr) = createBinaryOpNode(AND_OP, (yyvsp[-2].nodePtr), (yyvsp[0].nodePtr), yylineno); }
#line 1908 "parser.tab.c"
    break;

  case 69: /* expression: expression OR_OP expression  */
#line 421 "parser.y"
                                   { (yyval.nodePtr) = createBinaryOpNode(OR_OP, (yyvsp[-2].nodePtr), (yyvsp[0].nodePtr), yylineno); }
#line 1914 "parser.tab.c"
    break;

  case 70: /* expression: expression PIPE factor  */
#line 423 "parser.y"
      {
          Node* left_expr = (yyvsp[-2].nodePtr);
          Node* right_func_factor = (yyvsp[0].nodePtr);
          Node* new_call_node = nullptr;

          if (right_func_factor->type == NODE_FUNCTION_CALL) {
              FunctionCallNode* call_node = (FunctionCallNode*)right_func_factor;
              left_expr->next = call_node->arguments; 
              call_node->arguments = left_expr;

              new_call_node = right_func_factor;

          } else if (right_func_factor->type == NODE_IDENTIFIER) {
              Node* arg_list = createArgList(left_expr);
              new_call_node = createFunctionCallNode(right_func_factor, arg_list, yylineno);

          } else {
              yyerror("Error de Sintaxis: El operador '|>' debe ser seguido por una llamada a función.");
              (yyval.nodePtr) = nullptr; 
          }
          
          (yyval.nodePtr) = new_call_node;
      }
#line 1942 "parser.tab.c"
    break;

  case 71: /* factor: NUMBER  */
#line 450 "parser.y"
                      { (yyval.nodePtr) = createConstantIntNode((yyvsp[0].ival), yylineno); }
#line 1948 "parser.tab.c"
    break;

  case 72: /* factor: FLOAT_NUMBER  */
#line 451 "parser.y"
                      { (yyval.nodePtr) = createConstantFloatNode((yyvsp[0].fval), yylineno); }
#line 1954 "parser.tab.c"
    break;

  case 73: /* factor: TRUE_LITERAL  */
#line 452 "parser.y"
                      { (yyval.nodePtr) = createConstantBoolNode((yyvsp[0].ival), yylineno); }
#line 1960 "parser.tab.c"
    break;

  case 74: /* factor: FALSE_LITERAL  */
#line 453 "parser.y"
                      { (yyval.nodePtr) = createConstantBoolNode((yyvsp[0].ival), yylineno); }
#line 1966 "parser.tab.c"
    break;

  case 75: /* factor: IDENTIFIER  */
#line 455 "parser.y"
    { 
      C_SymbolInfo info;
      if (!c_lookup_symbol((yyvsp[0].sval), &info))
      {
        std::string err_msg = "Error Semántico: Identificador no declarado '" + std::string((yyvsp[0].sval)) + "'";
        yyerror(err_msg.c_str());
        (yyval.nodePtr) = createIdentifierNode((yyvsp[0].sval), yylineno);
      }
      else
      {
        (yyval.nodePtr) = createIdentifierNode((yyvsp[0].sval), yylineno);
      }
    }
#line 1984 "parser.tab.c"
    break;

  case 76: /* factor: STRING_LITERAL  */
#line 468 "parser.y"
                      { (yyval.nodePtr) = createStringLiteralNode((yyvsp[0].sval), yylineno); }
#line 1990 "parser.tab.c"
    break;

  case 77: /* factor: LPAREN expression RPAREN  */
#line 469 "parser.y"
                               { (yyval.nodePtr) = (yyvsp[-1].nodePtr); }
#line 1996 "parser.tab.c"
    break;

  case 78: /* factor: assignment  */
#line 470 "parser.y"
                      { (yyval.nodePtr) = (yyvsp[0].nodePtr); }
#line 2002 "parser.tab.c"
    break;

  case 79: /* factor: IDENTIFIER LPAREN args_opt RPAREN  */
#line 472 "parser.y"
    { 
      C_SymbolInfo func_info;
        if (!c_lookup_symbol((yyvsp[-3].sval), &func_info)) 
        {
          std::string err_msg = "Error Semántico: Función no declarada '" + std::string((yyvsp[-3].sval)) + "'";
          yyerror(err_msg.c_str());
        }
        else if (func_info.kind != NODE_FUNCTION_DEF) 
        { 
          std::string err_msg = "Error Semántico: '" + std::string((yyvsp[-3].sval)) + "' no es una función.";
          yyerror(err_msg.c_str());
        }
        Node* func_call_name_node = createIdentifierNode((yyvsp[-3].sval), yylineno); 
        (yyval.nodePtr) = createFunctionCallNode(func_call_name_node, (yyvsp[-1].nodePtr), yylineno);
    }
#line 2022 "parser.tab.c"
    break;

  case 80: /* factor: NOT_OP factor  */
#line 487 "parser.y"
                      { (yyval.nodePtr) = createUnaryOpNode(NOT_OP, (yyvsp[0].nodePtr), yylineno); }
#line 2028 "parser.tab.c"
    break;

  case 81: /* factor: MINUS factor  */
#line 488 "parser.y"
                                { (yyval.nodePtr) = createUnaryOpNode(MINUS, (yyvsp[0].nodePtr), yylineno); }
#line 2034 "parser.tab.c"
    break;

  case 82: /* args_opt: %empty  */
#line 493 "parser.y"
             { (yyval.nodePtr) = nullptr; }
#line 2040 "parser.tab.c"
    break;

  case 83: /* args_opt: arg_list  */
#line 494 "parser.y"
               { (yyval.nodePtr) = (yyvsp[0].nodePtr); }
#line 2046 "parser.tab.c"
    break;

  case 84: /* arg_list: expression  */
#line 499 "parser.y"
                 { (yyval.nodePtr) = createArgList((yyvsp[0].nodePtr)); }
#line 2052 "parser.tab.c"
    break;

  case 85: /* arg_list: arg_list ',' expression  */
#line 500 "parser.y"
                              { (yyval.nodePtr) = addArgToList((yyvsp[-2].nodePtr), (yyvsp[0].nodePtr)); }
#line 2058 "parser.tab.c"
    break;


#line 2062 "parser.tab.c"

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

#line 503 "parser.y"


//Es la funcion principal para el manejo de errores de syntax
void yyerror(const char *s) {
    std::cerr << "Error de Sintaxis: " << s << " cerca de la línea " << yylineno << std::endl;
    exit(1);//(Si se descomenta esta linea, el proceso de analisis parara al primer error, cosa que puede ser incomodo
    //ya que si hay mas de un error en el codigo, tendria que ir arreglandose uno por uno si no te das cuenta)
}

//El punto de entrada, es el que llama a las demas reglas y funciones
int main(int argc, char *argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return 1;
        }
    } else {
        yyin = stdin;
    }

    c_init_symbol_manager();

    int result = yyparse();

    bool sinErroresDeTipo = true;

    if (result == 0 && globalAstRoot != nullptr) {
        std::cerr << "[INFO] Analisis sintactico completado con exito." << std::endl;
        
        std::cerr << "[INFO] Iniciando verificacion de tipos..." << std::endl;
        sinErroresDeTipo = performTypeChecking(globalAstRoot);
        if (sinErroresDeTipo) {
            std::cerr << "[INFO] Verificacion de tipos completada sin errores." << std::endl;
        } else {
            std::cerr << "[INFO] Verificacion de tipos completada CON ERRORES" << std::endl;
        }

        if (globalAstRoot != nullptr) {
            //std::cerr << "[INFO] Raiz del AST capturada. Imprimiendo AST..." << std::endl;
            //if (!sinErroresDeTipo) {
                //std::cerr << "[INFO] Nota: Se detectaron errores de tipo. El AST podria mostrar nodos marcados como ERROR_TYPE." << std::endl;
            //}else{
                generate_code(globalAstRoot, "salida.cpp");
            //}
            //std::cout << "--- INICIO AST ---" << std::endl;
            //printAst(globalAstRoot);
            //std::cout << "--- FIN AST ---" << std::endl;

        } else {
            std::cerr << "[INFO] Analisis sintactico fallido (codigo " << result << ")." << std::endl;
        }

        //std::cerr << "[INFO] Liberando AST..." << std::endl;
        freeAst(globalAstRoot);
        globalAstRoot = nullptr; 
        //std::cerr << "[INFO] AST Liberado." << std::endl;

    } else if (result == 0 && globalAstRoot == nullptr) {
         std::cerr << "[INFO] Analisis sintactico completado con exito (programa vacio)." << std::endl;
    } else {
        std::cerr << "[INFO] Analisis sintactico fallido (codigo " << result << ")." << std::endl;
    }
    c_destroy_symbol_manager();

    if (yyin != stdin) {
        fclose(yyin);
    }
    return result;
}
