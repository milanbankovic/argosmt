/* A Bison parser, made by GNU Bison 3.5.3.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 20 "parser.ypp"


int yylex();

#define yyerror(X) 

#define YYMAXDEPTH 1000000

/*int yyerror(const char * fmt, ...) {
 return 0;
}*/

#include "smtlib_api.hpp"

bool active_error = false;
std::istream * yy_istream = 0;
smtlib_api * yy_api = 0;

sorted_variable_vector vars;
sort_parameter_vector pars;
sort_parameter dummy_par("");
sorted_variable dummy_var("", sort());


#line 95 "parser.cpp"

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUMERAL = 258,
    DECIMAL = 259,
    HEXADECIMAL = 260,
    BINARY = 261,
    STRING_LITERAL = 262,
    RW_PAR = 263,
    RW_NUMERAL = 264,
    RW_DECIMAL = 265,
    RW_STRING = 266,
    RW_UNDERSCORE = 267,
    RW_EXCLAMATION = 268,
    RW_AS = 269,
    RW_LET = 270,
    RW_FORALL = 271,
    RW_EXISTS = 272,
    RW_SET_LOGIC = 273,
    RW_SET_OPTION = 274,
    RW_SET_INFO = 275,
    RW_DECLARE_SORT = 276,
    RW_DEFINE_SORT = 277,
    RW_EXPAND_SORT = 278,
    RW_DECLARE_CONST = 279,
    RW_DECLARE_FUN = 280,
    RW_DEFINE_FUN = 281,
    RW_EXPAND_TERM = 282,
    RW_PUSH = 283,
    RW_POP = 284,
    RW_ASSERT = 285,
    RW_CHECK_SAT = 286,
    RW_GET_SORT = 287,
    RW_GET_ASSERTIONS = 288,
    RW_GET_PROOF = 289,
    RW_GET_UNSAT_CORE = 290,
    RW_GET_VALUE = 291,
    RW_GET_ASSIGNMENT = 292,
    RW_GET_OPTION = 293,
    RW_GET_INFO = 294,
    RW_RESET_ASSERTIONS = 295,
    RW_RESET = 296,
    RW_ECHO = 297,
    RW_EXIT = 298,
    SYMBOL = 299,
    KEYWORD = 300
  };
#endif
/* Tokens.  */
#define NUMERAL 258
#define DECIMAL 259
#define HEXADECIMAL 260
#define BINARY 261
#define STRING_LITERAL 262
#define RW_PAR 263
#define RW_NUMERAL 264
#define RW_DECIMAL 265
#define RW_STRING 266
#define RW_UNDERSCORE 267
#define RW_EXCLAMATION 268
#define RW_AS 269
#define RW_LET 270
#define RW_FORALL 271
#define RW_EXISTS 272
#define RW_SET_LOGIC 273
#define RW_SET_OPTION 274
#define RW_SET_INFO 275
#define RW_DECLARE_SORT 276
#define RW_DEFINE_SORT 277
#define RW_EXPAND_SORT 278
#define RW_DECLARE_CONST 279
#define RW_DECLARE_FUN 280
#define RW_DEFINE_FUN 281
#define RW_EXPAND_TERM 282
#define RW_PUSH 283
#define RW_POP 284
#define RW_ASSERT 285
#define RW_CHECK_SAT 286
#define RW_GET_SORT 287
#define RW_GET_ASSERTIONS 288
#define RW_GET_PROOF 289
#define RW_GET_UNSAT_CORE 290
#define RW_GET_VALUE 291
#define RW_GET_ASSIGNMENT 292
#define RW_GET_OPTION 293
#define RW_GET_INFO 294
#define RW_RESET_ASSERTIONS 295
#define RW_RESET 296
#define RW_ECHO 297
#define RW_EXIT 298
#define SYMBOL 299
#define KEYWORD 300

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 116 "parser.ypp"

      std::string * str_attr;
      attribute * attr_attr;
      attribute_value * attr_val_attr;
      attribute_value_vector * attr_val_v_attr;
      attribute_set * attr_s_attr;
      special_constant * sc_attr;
      sort_parameter_vector * sort_par_v_attr;
      sort * sort_attr;
      sort_vector * sort_v_attr;
      identifier * id_attr;
      sort_symbol * ss_attr;
      function_symbol * fs_attr;
      identifier::index_vector * index_v_attr;      
      sorted_variable_vector * sort_var_v_attr;
      sorted_variable * sort_var_attr;
      expression * term_attr;
      variable_binding * vb_attr;
      variable_binding_vector * vb_v_attr;
      expression_vector * term_v_attr;

#line 259 "parser.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */



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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   241

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  90
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  210

#define YYUNDEFTOK  2
#define YYMAXUTOK   300


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      46,    47,     2,     2,     2,     2,     2,     2,     2,     2,
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
      45
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   141,   141,   142,   147,   151,   158,   165,   172,   180,
     189,   194,   199,   208,   218,   231,   242,   247,   254,   261,
     268,   276,   281,   295,   299,   303,   309,   323,   335,   347,
     357,   376,   382,   388,   393,   400,   405,   410,   417,   424,
     429,   437,   442,   447,   452,   457,   465,   471,   477,   485,
     490,   497,   502,   510,   531,   536,   542,   550,   558,   563,
     569,   575,   582,   588,   595,   600,   606,   614,   622,   627,
     636,   644,   649,   671,   676,   682,   688,   694,   701,   707,
     714,   722,   730,   738,   747,   755,   763,   768,   777,   784,
     789
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUMERAL", "DECIMAL", "HEXADECIMAL",
  "BINARY", "STRING_LITERAL", "RW_PAR", "RW_NUMERAL", "RW_DECIMAL",
  "RW_STRING", "RW_UNDERSCORE", "RW_EXCLAMATION", "RW_AS", "RW_LET",
  "RW_FORALL", "RW_EXISTS", "RW_SET_LOGIC", "RW_SET_OPTION", "RW_SET_INFO",
  "RW_DECLARE_SORT", "RW_DEFINE_SORT", "RW_EXPAND_SORT",
  "RW_DECLARE_CONST", "RW_DECLARE_FUN", "RW_DEFINE_FUN", "RW_EXPAND_TERM",
  "RW_PUSH", "RW_POP", "RW_ASSERT", "RW_CHECK_SAT", "RW_GET_SORT",
  "RW_GET_ASSERTIONS", "RW_GET_PROOF", "RW_GET_UNSAT_CORE", "RW_GET_VALUE",
  "RW_GET_ASSIGNMENT", "RW_GET_OPTION", "RW_GET_INFO",
  "RW_RESET_ASSERTIONS", "RW_RESET", "RW_ECHO", "RW_EXIT", "SYMBOL",
  "KEYWORD", "'('", "')'", "$accept", "script", "command", "option",
  "attribute", "attribute_value", "attribute_seq", "attribute_ne_seq",
  "spec_constant", "attribute_value_seq", "sort_par_seq", "sort_seq",
  "sort_ne_seq", "sort", "indexed_identifier", "identifier", "sort_symbol",
  "function_symbol", "index_seq", "index_ne_seq", "sorted_var_seq",
  "sorted_var_ne_seq", "sorted_var", "term", "var_binding",
  "var_binding_seq", "var_binding_ne_seq", "term_seq", "term_ne_seq", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,    40,    41
};
# endif

#define YYPACT_NINF (-92)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-91)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -92,    19,   -92,   -92,   176,   -92,   -10,    23,    23,     7,
       7,    30,     7,    13,     7,    25,    75,    86,    25,    47,
      25,    49,    50,    51,    53,    54,    48,    62,    61,    63,
     102,    64,    65,    35,    66,   -92,    69,   -92,   105,   -92,
     -92,   115,    73,   -92,    10,    74,   -92,   -92,    30,    78,
      79,    80,   -92,   -92,   -92,   -92,   -92,   -92,    31,   -92,
     -92,    81,    83,    84,    85,   -92,    88,   -92,   -92,   -92,
     -92,   -92,    91,    92,   -92,   -92,    93,   -92,   -92,   -92,
     -92,   -92,   -92,   -92,   -92,    89,    94,   -92,   -92,   -92,
     -92,    95,   -92,   -92,   -92,    25,    38,    97,    98,   100,
     -92,    21,   -92,   -92,   -92,   -92,   -92,   -92,    25,   104,
     -92,   -92,   -92,    20,   -92,   -92,     8,    30,   106,   107,
     -92,    36,    14,    16,   -92,    30,    30,   -92,   -92,   -92,
     108,    46,   109,   110,   111,   -92,   -92,     5,   113,   -92,
      30,   114,   -92,   -92,     7,    30,   -92,   103,    30,   -92,
      23,   117,   118,   119,   116,   121,   123,   124,   128,   -92,
      30,    30,   -92,   -92,   131,   132,   -92,   133,   136,   137,
      30,    25,   139,   -92,   -92,   -92,   143,   141,    25,   142,
      25,    25,   144,   145,   -92,   -92,   -92,   173,   174,    25,
     175,   177,   178,   -92,   -92,    26,   -92,   -92,   179,   -92,
     -92,   -92,   180,   181,    30,   -92,   -92,   -92,   182,   -92
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     1,     3,     0,     2,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    33,     0,    32,     0,    58,     0,    59,
      60,     0,     0,    53,     0,     0,    54,    61,     0,     0,
       0,     0,    41,    42,    44,    43,    45,    72,     0,    71,
      73,     0,     0,     0,     0,    21,     0,    22,    25,    26,
      89,    28,     0,     0,    23,    24,     0,    31,     5,    36,
      47,    34,    35,     6,     7,     0,     0,    49,    51,    51,
      10,     0,    49,    51,    68,     0,     0,     0,     0,     0,
      89,     0,    89,    16,    17,    18,    19,    20,     0,     0,
      29,    30,    11,     0,    64,     8,     0,     0,     0,     0,
      12,     0,     0,     0,    39,     0,     0,    86,    68,    68,
       0,     0,     0,    88,     0,    37,    46,     0,     0,    48,
       0,    50,    55,    56,     0,     0,    50,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    76,
       0,     0,    78,    27,    62,    63,    57,     0,     0,     0,
       0,     0,    38,    83,    74,    75,     0,    85,     0,    67,
       0,     0,     0,     0,     9,    51,    13,     0,     0,     0,
       0,     0,     0,    89,    89,     0,    70,    15,     0,    80,
      81,    82,     0,     0,     0,    84,    77,    79,     0,    14
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -92,   -92,   -92,   -92,    -5,     9,   -92,   -92,   -18,   -92,
      28,   -80,   101,   -11,    -8,    76,   213,     4,   -92,   -92,
      33,   112,    27,    -4,   -92,   -92,   -92,   -92,   -91
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     5,    34,    35,    81,   150,   151,    59,   113,
     116,   117,   118,   146,    46,    47,    41,    48,   137,   138,
     156,   157,   149,    61,   177,   154,   155,   108,   109
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      45,    39,    39,    36,    39,    39,    39,    60,   164,   130,
      60,   132,    60,   122,    64,    82,    66,    50,    51,     2,
       3,    49,    85,    52,    53,    54,    55,    56,    52,    53,
      54,    55,    56,    85,    32,   131,    89,    91,    52,    53,
      54,    55,    56,    85,    95,    96,    97,    98,    99,   165,
     102,    37,   139,    38,    88,   140,    38,    37,    43,    38,
      44,   145,   147,   148,    79,     4,    80,   135,    33,    57,
      43,    58,    44,   204,    43,   100,    44,   101,    62,    79,
     139,    80,   125,   144,    38,    40,    40,    60,   126,    63,
     160,   124,    38,    72,    65,    82,    67,    68,    69,    70,
      60,    71,   202,   203,   133,   195,   141,    73,    74,    76,
      75,    77,    78,    83,   152,   153,    84,    85,    86,    87,
     121,    90,   136,   161,    92,    93,    94,   123,   103,   167,
     104,   105,   106,   114,   169,   107,    39,   171,   110,   111,
     112,   115,   120,   127,   128,   172,   129,   170,   168,   182,
     183,   134,     0,   142,   143,   159,   162,   -90,   163,   187,
     166,   -52,   176,    60,   173,   174,   175,   188,   178,   147,
      60,   180,    60,    60,   190,   181,   191,   192,   -65,   -66,
     184,    60,   185,   179,   186,   198,   -40,   189,   -87,   -69,
     119,   193,   194,   208,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
     196,   197,   199,    42,   200,   201,   205,   206,   207,   209,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   158
};

static const yytype_int16 yycheck[] =
{
      11,     9,    10,     8,    12,    13,    14,    15,     3,   100,
      18,   102,    20,    93,    18,    33,    20,    13,    14,     0,
       1,     8,    12,     3,     4,     5,     6,     7,     3,     4,
       5,     6,     7,    12,    44,    14,    44,    48,     3,     4,
       5,     6,     7,    12,    13,    14,    15,    16,    17,    44,
      58,    44,    44,    46,    44,    47,    46,    44,    44,    46,
      46,    47,    46,    47,    44,    46,    46,    47,    45,    44,
      44,    46,    46,    47,    44,    44,    46,    46,     3,    44,
      44,    46,    44,    47,    46,     9,    10,    95,    96,     3,
      44,    95,    46,    45,    47,   113,    47,    47,    47,    46,
     108,    47,   193,   194,   108,   185,   117,    45,    47,     7,
      47,    47,    47,    47,   125,   126,    47,    12,     3,    46,
      92,    47,   113,   131,    46,    46,    46,    94,    47,   140,
      47,    47,    47,    44,   145,    47,   144,   148,    47,    47,
      47,    47,    47,    46,    46,   150,    46,    44,   144,   160,
     161,    47,    -1,    47,    47,    47,    47,    47,    47,   170,
      47,    47,    46,   171,    47,    47,    47,   171,    47,    46,
     178,    47,   180,   181,   178,    47,   180,   181,    47,    47,
      47,   189,    46,   156,    47,   189,    47,    44,    47,    47,
      89,    47,    47,   204,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      47,    47,    47,    10,    47,    47,    47,    47,    47,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    49,     0,     1,    46,    50,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    51,    52,    52,    44,    46,    62,
      63,    64,    64,    44,    46,    61,    62,    63,    65,     8,
      65,    65,     3,     4,     5,     6,     7,    44,    46,    56,
      62,    71,     3,     3,    71,    47,    71,    47,    47,    47,
      46,    47,    45,    45,    47,    47,     7,    47,    47,    44,
      46,    53,    56,    47,    47,    12,     3,    46,    44,    62,
      47,    61,    46,    46,    46,    13,    14,    15,    16,    17,
      44,    46,    62,    47,    47,    47,    47,    47,    75,    76,
      47,    47,    47,    57,    44,    47,    58,    59,    60,    60,
      47,    58,    59,    68,    71,    44,    62,    46,    46,    46,
      76,    14,    76,    71,    47,    47,    53,    66,    67,    44,
      47,    61,    47,    47,    47,    47,    61,    46,    47,    70,
      54,    55,    61,    61,    73,    74,    68,    69,    69,    47,
      44,    62,    47,    47,     3,    44,    47,    61,    65,    61,
      44,    61,    52,    47,    47,    47,    46,    72,    47,    70,
      47,    47,    61,    61,    47,    46,    47,    61,    71,    44,
      71,    71,    71,    47,    47,    59,    47,    47,    71,    47,
      47,    47,    76,    76,    47,    47,    47,    47,    61,    47
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    48,    49,    49,    49,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    51,    52,    52,    53,    53,    53,    54,    54,
      55,    56,    56,    56,    56,    56,    57,    57,    58,    58,
      59,    59,    60,    61,    61,    61,    61,    62,    63,    63,
      64,    65,    66,    66,    66,    67,    67,    68,    68,    69,
      70,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    72,    73,    73,    74,    75,    75,
      76
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     2,     0,     4,     4,     4,     5,     8,
       4,     4,     5,     8,    12,     9,     4,     4,     4,     4,
       4,     3,     3,     3,     3,     3,     3,     6,     3,     4,
       4,     3,     1,     1,     2,     1,     1,     3,     2,     0,
       2,     1,     1,     1,     1,     1,     2,     0,     2,     0,
       2,     0,     2,     1,     1,     4,     4,     5,     1,     1,
       1,     1,     2,     2,     0,     2,     2,     2,     0,     2,
       4,     1,     1,     1,     5,     5,     4,     8,     4,     8,
       7,     7,     7,     5,     4,     2,     0,     2,     2,     0,
       2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
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
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
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

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
  case 3:
#line 143 "parser.ypp"
       {
       if(!active_error)
       	 throw syntax_error_exception();
       }
#line 1587 "parser.cpp"
    break;

  case 5:
#line 152 "parser.ypp"
        {
	  yy_api->set_logic(*(yyvsp[-1].str_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].str_attr);         
	}
#line 1598 "parser.cpp"
    break;

  case 6:
#line 159 "parser.ypp"
        {
	  yy_api->set_option(*(yyvsp[-1].attr_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
          delete (yyvsp[-1].attr_attr); 
	}
#line 1609 "parser.cpp"
    break;

  case 7:
#line 166 "parser.ypp"
        {
	  yy_api->set_info(*(yyvsp[-1].attr_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].attr_attr); 
	}
#line 1620 "parser.cpp"
    break;

  case 8:
#line 173 "parser.ypp"
        {
	  yy_api->declare_sort(*(yyvsp[-2].ss_attr), atoi((yyvsp[-1].str_attr)->c_str()));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-2].ss_attr);
	  delete (yyvsp[-1].str_attr);
	}
#line 1632 "parser.cpp"
    break;

  case 9:
#line 181 "parser.ypp"
        {
	  yy_api->define_sort(*(yyvsp[-5].ss_attr), std::move(*(yyvsp[-3].sort_par_v_attr)), *(yyvsp[-1].sort_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-5].ss_attr);
	  delete (yyvsp[-3].sort_par_v_attr);
	  delete (yyvsp[-1].sort_attr);
	}
#line 1645 "parser.cpp"
    break;

  case 10:
#line 190 "parser.ypp"
        {
	  yy_api->cout() << yy_api->expand_sort(*(yyvsp[-1].sort_attr)) << std::endl;
	  delete (yyvsp[-1].sort_attr);
	}
#line 1654 "parser.cpp"
    break;

  case 11:
#line 195 "parser.ypp"
        {
	   yy_api->echo(*(yyvsp[-1].str_attr));
	   delete (yyvsp[-1].str_attr);
	}
#line 1663 "parser.cpp"
    break;

  case 12:
#line 200 "parser.ypp"
        {
	  yy_api->declare_const(*(yyvsp[-2].fs_attr), *(yyvsp[-1].sort_attr));
	  if(yy_api->print_success())
	    yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-2].fs_attr);
	  delete (yyvsp[-1].sort_attr);

	}
#line 1676 "parser.cpp"
    break;

  case 13:
#line 209 "parser.ypp"
        {
	  (yyvsp[-3].sort_v_attr)->push_back(*(yyvsp[-1].sort_attr));
	  yy_api->declare_function(*(yyvsp[-5].fs_attr), std::move(*(yyvsp[-3].sort_v_attr)));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-5].fs_attr);
	  delete (yyvsp[-3].sort_v_attr);
	  delete (yyvsp[-1].sort_attr);
	}
#line 1690 "parser.cpp"
    break;

  case 14:
#line 219 "parser.ypp"
        {
	  (yyvsp[-3].sort_v_attr)->push_back(*(yyvsp[-1].sort_attr));
	  yy_api->declare_function(*(yyvsp[-5].fs_attr), std::move(*(yyvsp[-3].sort_v_attr)));
	  std::size_t n = (yyvsp[-7].sort_par_v_attr)->size();
	  pars.resize(pars.size() - n, dummy_par);
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-7].sort_par_v_attr);
	  delete (yyvsp[-5].fs_attr);
	  delete (yyvsp[-3].sort_v_attr);
	  delete (yyvsp[-1].sort_attr);
	}
#line 1707 "parser.cpp"
    break;

  case 15:
#line 232 "parser.ypp"
        {
	  yy_api->define_function(*(yyvsp[-6].fs_attr), std::move(*(yyvsp[-4].sort_var_v_attr)), *(yyvsp[-2].sort_attr), *(yyvsp[-1].term_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	 
	  delete (yyvsp[-6].fs_attr);
	  delete (yyvsp[-4].sort_var_v_attr);
	  delete (yyvsp[-2].sort_attr);
	  delete (yyvsp[-1].term_attr);
	}
#line 1722 "parser.cpp"
    break;

  case 16:
#line 243 "parser.ypp"
        {
	  yy_api->cout() << yy_api->expand_expression(*(yyvsp[-1].term_attr)) << std::endl;
	  delete (yyvsp[-1].term_attr);
	}
#line 1731 "parser.cpp"
    break;

  case 17:
#line 248 "parser.ypp"
        {
	  yy_api->push(atoi((yyvsp[-1].str_attr)->c_str()));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].str_attr);
	}
#line 1742 "parser.cpp"
    break;

  case 18:
#line 255 "parser.ypp"
        {
	  yy_api->pop(atoi((yyvsp[-1].str_attr)->c_str()));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].str_attr);
	}
#line 1753 "parser.cpp"
    break;

  case 19:
#line 262 "parser.ypp"
        {
	  yy_api->assert_expression(*(yyvsp[-1].term_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].term_attr);
	}
#line 1764 "parser.cpp"
    break;

  case 20:
#line 269 "parser.ypp"
        {
	  if(yy_api->expand_definitions())
	   yy_api->cout() << yy_api->expand_sort((*(yyvsp[-1].term_attr))->get_sort()) << std::endl;
	  else
	   yy_api->cout() << (*(yyvsp[-1].term_attr))->get_sort() << std::endl;
	  delete (yyvsp[-1].term_attr);
	}
#line 1776 "parser.cpp"
    break;

  case 21:
#line 277 "parser.ypp"
        {
	  check_sat_response csr = yy_api->check_sat();
	  yy_api->cout() << csr << std::endl;
	}
#line 1785 "parser.cpp"
    break;

  case 22:
#line 282 "parser.ypp"
        {
	const expression_vector assertions = yy_api->get_assertions();
	yy_api->cout() << "(";
	for(expression_vector::const_iterator it = assertions.begin(),
            it_end = assertions.end(); it != it_end; ++it)
          {
	     if(yy_api->expand_definitions())
               yy_api->cout() << yy_api->expand_expression(*it) << " ";
             else
               yy_api->cout() << *it << " "; 
          }
	yy_api->cout() << ")" << std::endl;
	}
#line 1803 "parser.cpp"
    break;

  case 23:
#line 296 "parser.ypp"
        {
	  yy_api->reset_assertions();
	}
#line 1811 "parser.cpp"
    break;

  case 24:
#line 300 "parser.ypp"
        {
	  yy_api->reset();
	}
#line 1819 "parser.cpp"
    break;

  case 25:
#line 304 "parser.ypp"
        {
	proof * pr = yy_api->get_proof();
	pr->print_proof(yy_api->cout());
	yy_api->cout() << std::endl;
	}
#line 1829 "parser.cpp"
    break;

  case 26:
#line 310 "parser.ypp"
        {
	 expression_vector core = yy_api->get_unsat_core();
	 yy_api->cout() << "(";
         for(expression_vector::const_iterator it = core.begin(),
            it_end = core.end(); it != it_end; ++it)
          {
             if(yy_api->expand_definitions())
               yy_api->cout() << yy_api->expand_expression(*it) << " ";
             else
               yy_api->cout() << *it << " "; 
          }
	  yy_api->cout() << ")" << std::endl;
	}
#line 1847 "parser.cpp"
    break;

  case 27:
#line 324 "parser.ypp"
        {
	 expression_vector assign = yy_api->get_value(*(yyvsp[-2].term_v_attr));
	 yy_api->cout() << "(";
         for(expression_vector::const_iterator it = assign.begin(),
            it_end = assign.end(); it != it_end; ++it)
          {
	    yy_api->cout() << "(" << *it << " " << (*it)->get_assigned() << ") "; 
          }
	  yy_api->cout() << ")" << std::endl;
	  delete (yyvsp[-2].term_v_attr);
	}
#line 1863 "parser.cpp"
    break;

  case 28:
#line 336 "parser.ypp"
        {
	   expression_vector assign = yy_api->get_assignment();
       	 yy_api->cout() << "(";
         for(expression_vector::const_iterator it = assign.begin(),
            it_end = assign.end(); it != it_end; ++it)
          {
	    yy_api->cout() << "(" << *it << " " << (*it)->get_assigned() << ") "; 
          }
	  yy_api->cout() << ")" << std::endl;

	}
#line 1879 "parser.cpp"
    break;

  case 29:
#line 348 "parser.ypp"
        {
	attribute_value * value = yy_api->get_option(*(yyvsp[-1].str_attr));
	if(value)
	{  
          value->print_value(yy_api->cout());
        }
        yy_api->cout() << std::endl;
	delete (yyvsp[-1].str_attr);   
	}
#line 1893 "parser.cpp"
    break;

  case 30:
#line 358 "parser.ypp"
        {
        attribute_value * value = yy_api->get_info(*(yyvsp[-1].str_attr));
	if(*(yyvsp[-1].str_attr) == keyword::ALL_STATISTICS.get_name())
	{
	  yy_api->cout() << "(";
	}
	else
	{
	  yy_api->cout() << "(" << *(yyvsp[-1].str_attr) << " ";	
	}
	
	if(value)
	{
	  value->print_value(yy_api->cout());
	}
	yy_api->cout() << ")" << std::endl; 
	delete (yyvsp[-1].str_attr);   
	}
#line 1916 "parser.cpp"
    break;

  case 31:
#line 377 "parser.ypp"
        {
	  YYACCEPT;
	}
#line 1924 "parser.cpp"
    break;

  case 32:
#line 383 "parser.ypp"
       {
          (yyval.attr_attr) = (yyvsp[0].attr_attr);
       }
#line 1932 "parser.cpp"
    break;

  case 33:
#line 389 "parser.ypp"
         {
	   (yyval.attr_attr) = new attribute(*(yyvsp[0].str_attr));
	   delete (yyvsp[0].str_attr);	   
	 }
#line 1941 "parser.cpp"
    break;

  case 34:
#line 394 "parser.ypp"
         {
	   (yyval.attr_attr) = new attribute(*(yyvsp[-1].str_attr), (yyvsp[0].attr_val_attr));
	   delete (yyvsp[-1].str_attr);
	 }
#line 1950 "parser.cpp"
    break;

  case 35:
#line 401 "parser.ypp"
               {
	         (yyval.attr_val_attr) = new standard_attribute_value<special_constant>(*(yyvsp[0].sc_attr));
		 delete (yyvsp[0].sc_attr);
	       }
#line 1959 "parser.cpp"
    break;

  case 36:
#line 406 "parser.ypp"
               {
	         (yyval.attr_val_attr) = new standard_attribute_value<symbol>(*(yyvsp[0].str_attr));
		 delete (yyvsp[0].str_attr);
	       }
#line 1968 "parser.cpp"
    break;

  case 37:
#line 411 "parser.ypp"
               {
	         (yyval.attr_val_attr) = new composite_attribute_value(std::move(*(yyvsp[-1].attr_val_v_attr)));
		 delete (yyvsp[-1].attr_val_v_attr);	         
	       }
#line 1977 "parser.cpp"
    break;

  case 38:
#line 418 "parser.ypp"
             {
	       (yyval.attr_s_attr) = (yyvsp[-1].attr_s_attr);
	       (yyval.attr_s_attr)->insert(std::move(*(yyvsp[0].attr_attr)));
	       delete (yyvsp[0].attr_attr);
	     }
#line 1987 "parser.cpp"
    break;

  case 39:
#line 424 "parser.ypp"
             {
	      (yyval.attr_s_attr) = new attribute_set(HASH_TABLE_SIZE);
	     }
#line 1995 "parser.cpp"
    break;

  case 40:
#line 430 "parser.ypp"
                {
	         (yyval.attr_s_attr) = (yyvsp[-1].attr_s_attr);
	         (yyval.attr_s_attr)->insert(std::move(*(yyvsp[0].attr_attr)));
	         delete (yyvsp[0].attr_attr);
		}
#line 2005 "parser.cpp"
    break;

  case 41:
#line 438 "parser.ypp"
               {
	          (yyval.sc_attr) = new special_constant(special_constant::T_NUMERAL, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	       }
#line 2014 "parser.cpp"
    break;

  case 42:
#line 443 "parser.ypp"
             {
	          (yyval.sc_attr) = new special_constant(special_constant::T_DECIMAL, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	     }
#line 2023 "parser.cpp"
    break;

  case 43:
#line 448 "parser.ypp"
             {
	          (yyval.sc_attr) = new special_constant(special_constant::T_BINARY, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	     }
#line 2032 "parser.cpp"
    break;

  case 44:
#line 453 "parser.ypp"
             {
	          (yyval.sc_attr) = new special_constant(special_constant::T_HEXADECIMAL, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	     }
#line 2041 "parser.cpp"
    break;

  case 45:
#line 458 "parser.ypp"
             {
	          (yyval.sc_attr) = new special_constant(special_constant::T_STRING_LITERAL, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	     }
#line 2050 "parser.cpp"
    break;

  case 46:
#line 466 "parser.ypp"
                   {
		     (yyval.attr_val_v_attr) = (yyvsp[-1].attr_val_v_attr);
		     (yyval.attr_val_v_attr)->push_back((yyvsp[0].attr_val_attr));
		   }
#line 2059 "parser.cpp"
    break;

  case 47:
#line 471 "parser.ypp"
                   {
		     (yyval.attr_val_v_attr) = new attribute_value_vector();
		   }
#line 2067 "parser.cpp"
    break;

  case 48:
#line 478 "parser.ypp"
            {
	      (yyval.sort_par_v_attr) = (yyvsp[-1].sort_par_v_attr);
	      (yyval.sort_par_v_attr)->push_back(sort_parameter(std::move(*(yyvsp[0].str_attr))));
	      pars.push_back(sort_parameter(std::move(*(yyvsp[0].str_attr))));
	      delete (yyvsp[0].str_attr);
 	    }
#line 2078 "parser.cpp"
    break;

  case 49:
#line 485 "parser.ypp"
            {
	      (yyval.sort_par_v_attr) = new sort_parameter_vector();
	    }
#line 2086 "parser.cpp"
    break;

  case 50:
#line 491 "parser.ypp"
        {
	  (yyval.sort_v_attr) = (yyvsp[-1].sort_v_attr);
	  (yyval.sort_v_attr)->push_back(*(yyvsp[0].sort_attr));
	  delete (yyvsp[0].sort_attr);  
	}
#line 2096 "parser.cpp"
    break;

  case 51:
#line 497 "parser.ypp"
        {
	  (yyval.sort_v_attr) = new sort_vector();
	}
#line 2104 "parser.cpp"
    break;

  case 52:
#line 503 "parser.ypp"
           {
	    (yyval.sort_v_attr) = (yyvsp[-1].sort_v_attr);
	    (yyval.sort_v_attr)->push_back(*(yyvsp[0].sort_attr));
	    delete (yyvsp[0].sort_attr);  
	   }
#line 2114 "parser.cpp"
    break;

  case 53:
#line 511 "parser.ypp"
    {
    sort_parameter_vector::reverse_iterator it, it_end;
    for(it = pars.rbegin(), it_end = pars.rend(); it != it_end; ++it)
    {
      if(it->get_name() == *(yyvsp[0].str_attr))
        break;
    }    

    if(it != pars.rend())
     {
       (yyval.sort_attr) = new sort(yy_api->get_sort_factory()->
                     create_sort(*it));
     }
     else
     {
       (yyval.sort_attr) = new sort(yy_api->get_sort_factory()->
                     create_sort(sort_symbol(std::move(*(yyvsp[0].str_attr)))));
     }
     delete (yyvsp[0].str_attr);
    }
#line 2139 "parser.cpp"
    break;

  case 54:
#line 532 "parser.ypp"
    {
      (yyval.sort_attr) = new sort(yy_api->get_sort_factory()->create_sort(sort_symbol(std::move(*(yyvsp[0].id_attr)))));
      delete (yyvsp[0].id_attr);
    }
#line 2148 "parser.cpp"
    break;

  case 55:
#line 537 "parser.ypp"
    {
      (yyval.sort_attr) = new sort(yy_api->get_sort_factory()->create_sort(sort_symbol(*(yyvsp[-2].str_attr)), std::move(*(yyvsp[-1].sort_v_attr))));
      delete (yyvsp[-2].str_attr);
      delete (yyvsp[-1].sort_v_attr);
    }
#line 2158 "parser.cpp"
    break;

  case 56:
#line 543 "parser.ypp"
    {
      (yyval.sort_attr) = new sort(yy_api->get_sort_factory()->create_sort(sort_symbol(*(yyvsp[-2].id_attr)), std::move(*(yyvsp[-1].sort_v_attr))));
      delete (yyvsp[-2].id_attr);
      delete (yyvsp[-1].sort_v_attr);
    }
#line 2168 "parser.cpp"
    break;

  case 57:
#line 551 "parser.ypp"
          {
	    (yyval.id_attr) = new identifier(std::move(*(yyvsp[-2].str_attr)), std::move(*(yyvsp[-1].index_v_attr)));
	    delete (yyvsp[-2].str_attr);
	    delete (yyvsp[-1].index_v_attr);
	  }
#line 2178 "parser.cpp"
    break;

  case 58:
#line 559 "parser.ypp"
          {
	     (yyval.id_attr) = new identifier(std::move(*(yyvsp[0].str_attr)));
	     delete (yyvsp[0].str_attr);
	  }
#line 2187 "parser.cpp"
    break;

  case 59:
#line 564 "parser.ypp"
          {
	     (yyval.id_attr) = (yyvsp[0].id_attr);
	  }
#line 2195 "parser.cpp"
    break;

  case 60:
#line 570 "parser.ypp"
           {
	     (yyval.ss_attr) = new sort_symbol(std::move(*(yyvsp[0].id_attr)));
	     delete (yyvsp[0].id_attr);
	   }
#line 2204 "parser.cpp"
    break;

  case 61:
#line 576 "parser.ypp"
               {
	        (yyval.fs_attr) = new function_symbol(std::move(*(yyvsp[0].id_attr)));
		delete (yyvsp[0].id_attr);
	       }
#line 2213 "parser.cpp"
    break;

  case 62:
#line 583 "parser.ypp"
           {
	     (yyval.index_v_attr) = (yyvsp[-1].index_v_attr);
	     (yyval.index_v_attr)->push_back(atoi((yyvsp[0].str_attr)->c_str()));
	     delete (yyvsp[0].str_attr);
           }
#line 2223 "parser.cpp"
    break;

  case 63:
#line 589 "parser.ypp"
           {
	     (yyval.index_v_attr) = (yyvsp[-1].index_v_attr);
	     (yyval.index_v_attr)->push_back(symbol(std::move(*(yyvsp[0].str_attr))));
	     delete (yyvsp[0].str_attr);
	   }
#line 2233 "parser.cpp"
    break;

  case 64:
#line 595 "parser.ypp"
           {
	     (yyval.index_v_attr) = new identifier::index_vector();
	   }
#line 2241 "parser.cpp"
    break;

  case 65:
#line 601 "parser.ypp"
              {
	       (yyval.index_v_attr) = (yyvsp[-1].index_v_attr);
	       (yyval.index_v_attr)->push_back(atoi((yyvsp[0].str_attr)->c_str()));
	       delete (yyvsp[0].str_attr);
              }
#line 2251 "parser.cpp"
    break;

  case 66:
#line 607 "parser.ypp"
              {
	        (yyval.index_v_attr) = (yyvsp[-1].index_v_attr);
	        (yyval.index_v_attr)->push_back(symbol(std::move(*(yyvsp[0].str_attr))));
	        delete (yyvsp[0].str_attr);
	      }
#line 2261 "parser.cpp"
    break;

  case 67:
#line 615 "parser.ypp"
              {
               (yyval.sort_var_v_attr) = (yyvsp[-1].sort_var_v_attr);
	       (yyval.sort_var_v_attr)->push_back(*(yyvsp[0].sort_var_attr));
	       vars.push_back(*(yyvsp[0].sort_var_attr));
	       delete (yyvsp[0].sort_var_attr);
              }
#line 2272 "parser.cpp"
    break;

  case 68:
#line 622 "parser.ypp"
              {
	       (yyval.sort_var_v_attr) = new sorted_variable_vector(0, dummy_var);
	      }
#line 2280 "parser.cpp"
    break;

  case 69:
#line 628 "parser.ypp"
                 {
                  (yyval.sort_var_v_attr) = (yyvsp[-1].sort_var_v_attr);
	          (yyval.sort_var_v_attr)->push_back(*(yyvsp[0].sort_var_attr));
		  vars.push_back(*(yyvsp[0].sort_var_attr));
	          delete (yyvsp[0].sort_var_attr);
                 }
#line 2291 "parser.cpp"
    break;

  case 70:
#line 637 "parser.ypp"
          {
	    (yyval.sort_var_attr) = new sorted_variable(std::move(*(yyvsp[-2].str_attr)), *(yyvsp[-1].sort_attr));
	    delete (yyvsp[-2].str_attr);
	    delete (yyvsp[-1].sort_attr);
          }
#line 2301 "parser.cpp"
    break;

  case 71:
#line 645 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(*(yyvsp[0].sc_attr)));
      delete (yyvsp[0].sc_attr);
    }
#line 2310 "parser.cpp"
    break;

  case 72:
#line 650 "parser.ypp"
    {
    sorted_variable_vector::reverse_iterator it, it_end;
    for(it = vars.rbegin(),
        it_end = vars.rend(); it != it_end; ++it)
    {
      if(it->get_variable().get_name() == *(yyvsp[0].str_attr))
        break;
    }    

    if(it != vars.rend())
     {
       (yyval.term_attr) = new expression(yy_api->get_expression_factory()->
                           create_expression(it->get_variable(), it->get_sort()));
     }
     else
     {
       (yyval.term_attr) = new expression(yy_api->get_expression_factory()->
                           create_expression(function_symbol(std::move(*(yyvsp[0].str_attr)))));
     }
     delete (yyvsp[0].str_attr);
    }
#line 2336 "parser.cpp"
    break;

  case 73:
#line 672 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[0].id_attr)))));
      delete (yyvsp[0].id_attr);      	      
    }
#line 2345 "parser.cpp"
    break;

  case 74:
#line 677 "parser.ypp"
    {
       (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-2].str_attr))), *(yyvsp[-1].sort_attr)));
       delete (yyvsp[-2].str_attr);
       delete (yyvsp[-1].sort_attr);      
    }
#line 2355 "parser.cpp"
    break;

  case 75:
#line 683 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-2].id_attr))), *(yyvsp[-1].sort_attr)));      
      delete (yyvsp[-2].id_attr);
      delete (yyvsp[-1].sort_attr);
    }
#line 2365 "parser.cpp"
    break;

  case 76:
#line 689 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-2].str_attr))), std::move(*(yyvsp[-1].term_v_attr))));
      delete (yyvsp[-2].str_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2375 "parser.cpp"
    break;

  case 77:
#line 695 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-4].str_attr))), std::move(*(yyvsp[-1].term_v_attr)), *(yyvsp[-3].sort_attr)));
      delete (yyvsp[-4].str_attr);
      delete (yyvsp[-3].sort_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2386 "parser.cpp"
    break;

  case 78:
#line 702 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-2].id_attr))), std::move(*(yyvsp[-1].term_v_attr))));
      delete (yyvsp[-2].id_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2396 "parser.cpp"
    break;

  case 79:
#line 708 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-4].id_attr))), std::move(*(yyvsp[-1].term_v_attr)), *(yyvsp[-3].sort_attr)));
      delete (yyvsp[-4].id_attr);
      delete (yyvsp[-3].sort_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2407 "parser.cpp"
    break;

  case 80:
#line 715 "parser.ypp"
    {
      std::size_t n = (yyvsp[-3].vb_v_attr)->size();
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(std::move(*(yyvsp[-3].vb_v_attr)), *(yyvsp[-1].term_attr)));
      vars.resize(vars.size() - n, dummy_var);
      delete (yyvsp[-3].vb_v_attr);
      delete (yyvsp[-1].term_attr);
    }
#line 2419 "parser.cpp"
    break;

  case 81:
#line 723 "parser.ypp"
    {
      std::size_t n = (yyvsp[-3].sort_var_v_attr)->size();
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(Q_FORALL, std::move(*(yyvsp[-3].sort_var_v_attr)), *(yyvsp[-1].term_attr)));
      vars.resize(vars.size() - n, dummy_var);
      delete (yyvsp[-3].sort_var_v_attr);
      delete (yyvsp[-1].term_attr);
    }
#line 2431 "parser.cpp"
    break;

  case 82:
#line 731 "parser.ypp"
    {
      std::size_t n = (yyvsp[-3].sort_var_v_attr)->size();
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(Q_EXISTS, std::move(*(yyvsp[-3].sort_var_v_attr)), *(yyvsp[-1].term_attr)));
      vars.resize(vars.size() - n, dummy_var);
      delete (yyvsp[-3].sort_var_v_attr);
      delete (yyvsp[-1].term_attr);      
    }
#line 2443 "parser.cpp"
    break;

  case 83:
#line 739 "parser.ypp"
    {
      (yyval.term_attr) = (yyvsp[-2].term_attr);
      (*(yyval.term_attr))->get_attributes() = std::move(*(yyvsp[-1].attr_s_attr));
      delete (yyvsp[-1].attr_s_attr);
      yy_api->check_label(*(yyval.term_attr));
    }
#line 2454 "parser.cpp"
    break;

  case 84:
#line 748 "parser.ypp"
           {
	     (yyval.vb_attr) = new variable_binding(std::move(*(yyvsp[-2].str_attr)), *(yyvsp[-1].term_attr));
	     delete (yyvsp[-2].str_attr);
	     delete (yyvsp[-1].term_attr);
	   }
#line 2464 "parser.cpp"
    break;

  case 85:
#line 756 "parser.ypp"
               {
	         (yyval.vb_v_attr) = (yyvsp[-1].vb_v_attr);
		 (yyval.vb_v_attr)->push_back(*(yyvsp[0].vb_attr));
		 vars.push_back(sorted_variable((yyvsp[0].vb_attr)->get_variable(), (yyvsp[0].vb_attr)->get_expression()->get_sort()));
		 delete (yyvsp[0].vb_attr);
	       }
#line 2475 "parser.cpp"
    break;

  case 86:
#line 763 "parser.ypp"
               {
	         (yyval.vb_v_attr) = new variable_binding_vector();
	       }
#line 2483 "parser.cpp"
    break;

  case 87:
#line 769 "parser.ypp"
                 {
	          (yyval.vb_v_attr) = (yyvsp[-1].vb_v_attr);
		  (yyval.vb_v_attr)->push_back(*(yyvsp[0].vb_attr));
        	  vars.push_back(sorted_variable((yyvsp[0].vb_attr)->get_variable(), (yyvsp[0].vb_attr)->get_expression()->get_sort()));
		  delete (yyvsp[0].vb_attr);
	         }
#line 2494 "parser.cpp"
    break;

  case 88:
#line 778 "parser.ypp"
        {
	  (yyval.term_v_attr) = (yyvsp[-1].term_v_attr);
	  (yyval.term_v_attr)->push_back(*(yyvsp[0].term_attr));
	  delete (yyvsp[0].term_attr);
	}
#line 2504 "parser.cpp"
    break;

  case 89:
#line 784 "parser.ypp"
        {
	  (yyval.term_v_attr) = new expression_vector();
	}
#line 2512 "parser.cpp"
    break;

  case 90:
#line 790 "parser.ypp"
           {
	     (yyval.term_v_attr) = (yyvsp[-1].term_v_attr);
	     (yyval.term_v_attr)->push_back(*(yyvsp[0].term_attr));
	     delete (yyvsp[0].term_attr);
	   }
#line 2522 "parser.cpp"
    break;


#line 2526 "parser.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
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
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 796 "parser.ypp"
       
