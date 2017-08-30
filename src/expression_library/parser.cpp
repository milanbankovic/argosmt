/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 20 "parser.ypp" /* yacc.c:339  */


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


#line 91 "parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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
    RW_DECLARE_FUN = 279,
    RW_DEFINE_FUN = 280,
    RW_EXPAND_TERM = 281,
    RW_PUSH = 282,
    RW_POP = 283,
    RW_ASSERT = 284,
    RW_CHECK_SAT = 285,
    RW_GET_SORT = 286,
    RW_GET_ASSERTIONS = 287,
    RW_GET_PROOF = 288,
    RW_GET_UNSAT_CORE = 289,
    RW_GET_VALUE = 290,
    RW_GET_ASSIGNMENT = 291,
    RW_GET_OPTION = 292,
    RW_GET_INFO = 293,
    RW_EXIT = 294,
    SYMBOL = 295,
    KEYWORD = 296
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
#define RW_DECLARE_FUN 279
#define RW_DEFINE_FUN 280
#define RW_EXPAND_TERM 281
#define RW_PUSH 282
#define RW_POP 283
#define RW_ASSERT 284
#define RW_CHECK_SAT 285
#define RW_GET_SORT 286
#define RW_GET_ASSERTIONS 287
#define RW_GET_PROOF 288
#define RW_GET_UNSAT_CORE 289
#define RW_GET_VALUE 290
#define RW_GET_ASSIGNMENT 291
#define RW_GET_OPTION 292
#define RW_GET_INFO 293
#define RW_EXIT 294
#define SYMBOL 295
#define KEYWORD 296

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 112 "parser.ypp" /* yacc.c:355  */

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
      identifier::index_vector * num_v_attr;      
      sorted_variable_vector * sort_var_v_attr;
      sorted_variable * sort_var_attr;
      expression * term_attr;
      variable_binding * vb_attr;
      variable_binding_vector * vb_v_attr;
      expression_vector * term_v_attr;

#line 235 "parser.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 252 "parser.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
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
#define YYLAST   222

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  84
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  198

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      42,    43,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   137,   137,   138,   143,   147,   154,   161,   168,   176,
     185,   190,   200,   213,   224,   229,   236,   243,   250,   258,
     265,   279,   285,   299,   311,   323,   333,   352,   358,   364,
     369,   376,   381,   386,   393,   400,   405,   413,   418,   423,
     428,   433,   441,   447,   453,   461,   466,   473,   478,   486,
     507,   512,   518,   526,   534,   539,   545,   551,   558,   565,
     570,   578,   586,   591,   600,   608,   613,   635,   640,   646,
     652,   658,   665,   671,   678,   686,   694,   702,   711,   719,
     727,   732,   741,   748,   753
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
  "RW_DECLARE_SORT", "RW_DEFINE_SORT", "RW_EXPAND_SORT", "RW_DECLARE_FUN",
  "RW_DEFINE_FUN", "RW_EXPAND_TERM", "RW_PUSH", "RW_POP", "RW_ASSERT",
  "RW_CHECK_SAT", "RW_GET_SORT", "RW_GET_ASSERTIONS", "RW_GET_PROOF",
  "RW_GET_UNSAT_CORE", "RW_GET_VALUE", "RW_GET_ASSIGNMENT",
  "RW_GET_OPTION", "RW_GET_INFO", "RW_EXIT", "SYMBOL", "KEYWORD", "'('",
  "')'", "$accept", "script", "command", "option", "attribute",
  "attribute_value", "attribute_seq", "attribute_ne_seq", "spec_constant",
  "attribute_value_seq", "sort_par_seq", "sort_seq", "sort_ne_seq", "sort",
  "indexed_identifier", "identifier", "sort_symbol", "function_symbol",
  "numeral_seq", "numeral_ne_seq", "sorted_var_seq", "sorted_var_ne_seq",
  "sorted_var", "term", "var_binding", "var_binding_seq",
  "var_binding_ne_seq", "term_seq", "term_ne_seq", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,    40,    41
};
# endif

#define YYPACT_NINF -84

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-84)))

#define YYTABLE_NINF -85

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -84,    16,   -84,   -84,   164,   -84,     8,    28,    28,     4,
       4,    21,    10,     4,    22,    72,    73,    22,    43,    22,
      49,    50,    51,    48,    54,    59,    61,    62,    63,    30,
      64,   -84,    65,   -84,    97,   -84,   -84,   107,    69,   -84,
       7,    70,   -84,    74,   -84,    75,    77,   -84,   -84,   -84,
     -84,   -84,   -84,    26,   -84,   -84,    78,    79,    81,    85,
     -84,    86,   -84,   -84,   -84,   -84,   -84,    87,    88,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,    80,    89,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,    22,    25,    91,    93,
      94,   -84,    71,   -84,   -84,   -84,   -84,   -84,   -84,    22,
      98,   -84,   -84,    17,   -84,   -84,    37,    21,    99,   100,
      41,    11,   -12,   -84,    21,    21,   -84,   -84,   -84,   101,
      47,   102,   103,   105,   -84,   -84,   111,   106,   -84,    21,
     109,   -84,   -84,     4,    21,   -84,   110,    21,   -84,    28,
     113,   114,   116,    95,   120,   112,   123,   124,   -84,    21,
      21,   -84,   -84,   125,   -84,   127,   129,   131,    21,    22,
     132,   -84,   -84,   -84,   136,   134,    22,   135,    22,    22,
     137,   161,   -84,   -84,   -84,   162,   163,    22,   165,   166,
     167,   -84,   -84,    31,   -84,   -84,   168,   -84,   -84,   -84,
     169,   170,    21,   -84,   -84,   -84,   171,   -84
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     1,     3,     0,     2,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,    28,     0,    54,     0,    55,    56,     0,     0,    49,
       0,     0,    50,     0,    57,     0,     0,    37,    38,    40,
      39,    41,    66,     0,    65,    67,     0,     0,     0,     0,
      19,     0,    20,    21,    22,    83,    24,     0,     0,    27,
       5,    32,    43,    30,    31,     6,     7,     0,     0,    45,
      47,    47,    10,    45,    47,    62,     0,     0,     0,     0,
       0,    83,     0,    83,    14,    15,    16,    17,    18,     0,
       0,    25,    26,     0,    59,     8,     0,     0,     0,     0,
       0,     0,     0,    35,     0,     0,    80,    62,    62,     0,
       0,     0,    82,     0,    33,    42,     0,     0,    44,     0,
      46,    51,    52,     0,     0,    46,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    70,     0,
       0,    72,    23,    58,    53,     0,     0,     0,     0,     0,
      34,    77,    68,    69,     0,    79,     0,    61,     0,     0,
       0,     0,     9,    47,    11,     0,     0,     0,     0,     0,
       0,    83,    83,     0,    64,    13,     0,    74,    75,    76,
       0,     0,     0,    78,    71,    73,     0,    12
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -84,   -84,   -84,   -84,    -5,    12,   -84,   -84,   -15,   -84,
      57,   -72,   126,   -11,    -8,    46,   143,    -6,   -84,   -84,
     130,   104,    27,    -4,   -84,   -84,   -84,   -84,   -83
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     5,    30,    31,    73,   139,   140,    54,   103,
     106,   107,   108,   135,    42,    44,    37,    45,   126,   127,
     145,   146,   138,    56,   165,   143,   144,    99,   100
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      41,    35,    35,    32,    35,    35,    55,    46,   119,    55,
     121,    55,   111,    59,    74,    61,     2,     3,    43,    77,
      47,    48,    49,    50,    51,    47,    48,    49,    50,    51,
     136,   137,    81,    47,    48,    49,    50,    51,    77,    86,
      87,    88,    89,    90,    33,    93,    34,    80,    28,    34,
      33,    39,    34,    40,   134,    36,    36,    71,     4,    72,
     124,    39,    52,    40,    53,   114,    91,    34,    92,    29,
      71,    39,    72,    40,   192,    57,    58,   128,    55,   115,
     129,   128,   113,    77,   133,   120,    60,   149,    74,    34,
      65,    55,    62,    63,    64,   122,   130,    66,   190,   191,
      67,   183,    68,   141,   142,    69,    70,    75,    76,    77,
      78,    79,   150,    82,   153,   125,    83,    84,   155,    85,
     104,    94,    95,   157,    96,    35,   159,   156,    97,    98,
     101,   102,   105,   116,   160,   117,   118,   164,   170,   171,
     110,   123,   131,   132,   148,   151,   -84,   175,   152,   154,
     158,    55,   -48,    38,   136,   176,   161,   162,    55,   163,
      55,    55,   178,   166,   179,   180,   168,   169,   -60,    55,
     172,   173,   167,   186,   174,   -36,   177,   -81,   -63,     0,
     181,   196,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,   182,   184,   185,   109,   187,   188,
     189,   193,   194,   195,   197,   112,     0,     0,     0,     0,
       0,     0,   147
};

static const yytype_int16 yycheck[] =
{
      11,     9,    10,     8,    12,    13,    14,    13,    91,    17,
      93,    19,    84,    17,    29,    19,     0,     1,     8,    12,
       3,     4,     5,     6,     7,     3,     4,     5,     6,     7,
      42,    43,    40,     3,     4,     5,     6,     7,    12,    13,
      14,    15,    16,    17,    40,    53,    42,    40,    40,    42,
      40,    40,    42,    42,    43,     9,    10,    40,    42,    42,
      43,    40,    40,    42,    42,    40,    40,    42,    42,    41,
      40,    40,    42,    42,    43,     3,     3,    40,    86,    87,
      43,    40,    86,    12,    43,    14,    43,    40,   103,    42,
      42,    99,    43,    43,    43,    99,   107,    43,   181,   182,
      41,   173,    41,   114,   115,    43,    43,    43,    43,    12,
       3,    42,   120,    43,     3,   103,    42,    42,   129,    42,
      40,    43,    43,   134,    43,   133,   137,   133,    43,    43,
      43,    43,    43,    42,   139,    42,    42,    42,   149,   150,
      83,    43,    43,    43,    43,    43,    43,   158,    43,    43,
      40,   159,    43,    10,    42,   159,    43,    43,   166,    43,
     168,   169,   166,    43,   168,   169,    43,    43,    43,   177,
      43,    42,   145,   177,    43,    43,    40,    43,    43,    -1,
      43,   192,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    43,    43,    43,    81,    43,    43,
      43,    43,    43,    43,    43,    85,    -1,    -1,    -1,    -1,
      -1,    -1,   118
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    45,     0,     1,    42,    46,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      47,    48,    48,    40,    42,    58,    59,    60,    60,    40,
      42,    57,    58,     8,    59,    61,    61,     3,     4,     5,
       6,     7,    40,    42,    52,    58,    67,     3,     3,    67,
      43,    67,    43,    43,    43,    42,    43,    41,    41,    43,
      43,    40,    42,    49,    52,    43,    43,    12,     3,    42,
      40,    58,    43,    42,    42,    42,    13,    14,    15,    16,
      17,    40,    42,    58,    43,    43,    43,    43,    43,    71,
      72,    43,    43,    53,    40,    43,    54,    55,    56,    56,
      54,    55,    64,    67,    40,    58,    42,    42,    42,    72,
      14,    72,    67,    43,    43,    49,    62,    63,    40,    43,
      57,    43,    43,    43,    43,    57,    42,    43,    66,    50,
      51,    57,    57,    69,    70,    64,    65,    65,    43,    40,
      58,    43,    43,     3,    43,    57,    61,    57,    40,    57,
      48,    43,    43,    43,    42,    68,    43,    66,    43,    43,
      57,    57,    43,    42,    43,    57,    67,    40,    67,    67,
      67,    43,    43,    55,    43,    43,    67,    43,    43,    43,
      72,    72,    43,    43,    43,    43,    57,    43
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    44,    45,    45,    45,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    46,    47,    48,
      48,    49,    49,    49,    50,    50,    51,    52,    52,    52,
      52,    52,    53,    53,    54,    54,    55,    55,    56,    57,
      57,    57,    57,    58,    59,    59,    60,    61,    62,    62,
      63,    64,    64,    65,    66,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    68,    69,
      69,    70,    71,    71,    72
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     0,     4,     4,     4,     5,     8,
       4,     8,    12,     9,     4,     4,     4,     4,     4,     3,
       3,     3,     3,     6,     3,     4,     4,     3,     1,     1,
       2,     1,     1,     3,     2,     0,     2,     1,     1,     1,
       1,     1,     2,     0,     2,     0,     2,     0,     2,     1,
       1,     4,     4,     5,     1,     1,     1,     1,     2,     0,
       2,     2,     0,     2,     4,     1,     1,     1,     5,     5,
       4,     8,     4,     8,     7,     7,     7,     5,     4,     2,
       0,     2,     2,     0,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

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
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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
#line 139 "parser.ypp" /* yacc.c:1646  */
    {
       if(!active_error)
       	 throw syntax_error_exception();
       }
#line 1471 "parser.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 148 "parser.ypp" /* yacc.c:1646  */
    {
	  yy_api->set_logic(*(yyvsp[-1].str_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].str_attr);         
	}
#line 1482 "parser.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 155 "parser.ypp" /* yacc.c:1646  */
    {
	  yy_api->set_option(*(yyvsp[-1].attr_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
          delete (yyvsp[-1].attr_attr); 
	}
#line 1493 "parser.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 162 "parser.ypp" /* yacc.c:1646  */
    {
	  yy_api->set_info(*(yyvsp[-1].attr_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].attr_attr); 
	}
#line 1504 "parser.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 169 "parser.ypp" /* yacc.c:1646  */
    {
	  yy_api->declare_sort(*(yyvsp[-2].ss_attr), atoi((yyvsp[-1].str_attr)->c_str()));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-2].ss_attr);
	  delete (yyvsp[-1].str_attr);
	}
#line 1516 "parser.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 177 "parser.ypp" /* yacc.c:1646  */
    {
	  yy_api->define_sort(*(yyvsp[-5].ss_attr), std::move(*(yyvsp[-3].sort_par_v_attr)), *(yyvsp[-1].sort_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-5].ss_attr);
	  delete (yyvsp[-3].sort_par_v_attr);
	  delete (yyvsp[-1].sort_attr);
	}
#line 1529 "parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 186 "parser.ypp" /* yacc.c:1646  */
    {
	  yy_api->cout() << yy_api->expand_sort(*(yyvsp[-1].sort_attr)) << std::endl;
	  delete (yyvsp[-1].sort_attr);
	}
#line 1538 "parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 191 "parser.ypp" /* yacc.c:1646  */
    {
	  (yyvsp[-3].sort_v_attr)->push_back(*(yyvsp[-1].sort_attr));
	  yy_api->declare_function(*(yyvsp[-5].fs_attr), std::move(*(yyvsp[-3].sort_v_attr)));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-5].fs_attr);
	  delete (yyvsp[-3].sort_v_attr);
	  delete (yyvsp[-1].sort_attr);
	}
#line 1552 "parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 201 "parser.ypp" /* yacc.c:1646  */
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
#line 1569 "parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 214 "parser.ypp" /* yacc.c:1646  */
    {
	  yy_api->define_function(*(yyvsp[-6].fs_attr), std::move(*(yyvsp[-4].sort_var_v_attr)), *(yyvsp[-2].sort_attr), *(yyvsp[-1].term_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	 
	  delete (yyvsp[-6].fs_attr);
	  delete (yyvsp[-4].sort_var_v_attr);
	  delete (yyvsp[-2].sort_attr);
	  delete (yyvsp[-1].term_attr);
	}
#line 1584 "parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 225 "parser.ypp" /* yacc.c:1646  */
    {
	  yy_api->cout() << yy_api->expand_expression(*(yyvsp[-1].term_attr)) << std::endl;
	  delete (yyvsp[-1].term_attr);
	}
#line 1593 "parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 230 "parser.ypp" /* yacc.c:1646  */
    {
	  yy_api->push(atoi((yyvsp[-1].str_attr)->c_str()));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].str_attr);
	}
#line 1604 "parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 237 "parser.ypp" /* yacc.c:1646  */
    {
	  yy_api->pop(atoi((yyvsp[-1].str_attr)->c_str()));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].str_attr);
	}
#line 1615 "parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 244 "parser.ypp" /* yacc.c:1646  */
    {
	  yy_api->assert_expression(*(yyvsp[-1].term_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].term_attr);
	}
#line 1626 "parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 251 "parser.ypp" /* yacc.c:1646  */
    {
	  if(yy_api->expand_definitions())
	   yy_api->cout() << yy_api->expand_sort((*(yyvsp[-1].term_attr))->get_sort()) << std::endl;
	  else
	   yy_api->cout() << (*(yyvsp[-1].term_attr))->get_sort() << std::endl;
	  delete (yyvsp[-1].term_attr);
	}
#line 1638 "parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 259 "parser.ypp" /* yacc.c:1646  */
    {
	
	yy_api->check_sat();
	yy_api->get_info(keyword::STATUS)->print_value(yy_api->cout());
	yy_api->cout() << std::endl;	
	}
#line 1649 "parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 266 "parser.ypp" /* yacc.c:1646  */
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
#line 1667 "parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 280 "parser.ypp" /* yacc.c:1646  */
    {
	proof * pr = yy_api->get_proof();
	pr->print_proof(yy_api->cout());
	yy_api->cout() << std::endl;
	}
#line 1677 "parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 286 "parser.ypp" /* yacc.c:1646  */
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
#line 1695 "parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 300 "parser.ypp" /* yacc.c:1646  */
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
#line 1711 "parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 312 "parser.ypp" /* yacc.c:1646  */
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
#line 1727 "parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 324 "parser.ypp" /* yacc.c:1646  */
    {
	attribute_value * value = yy_api->get_option(*(yyvsp[-1].str_attr));
	if(value)
	{  
          value->print_value(yy_api->cout());
        }
        yy_api->cout() << std::endl;
	delete (yyvsp[-1].str_attr);   
	}
#line 1741 "parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 334 "parser.ypp" /* yacc.c:1646  */
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
#line 1764 "parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 353 "parser.ypp" /* yacc.c:1646  */
    {
	  YYACCEPT;
	}
#line 1772 "parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 359 "parser.ypp" /* yacc.c:1646  */
    {
          (yyval.attr_attr) = (yyvsp[0].attr_attr);
       }
#line 1780 "parser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 365 "parser.ypp" /* yacc.c:1646  */
    {
	   (yyval.attr_attr) = new attribute(*(yyvsp[0].str_attr));
	   delete (yyvsp[0].str_attr);	   
	 }
#line 1789 "parser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 370 "parser.ypp" /* yacc.c:1646  */
    {
	   (yyval.attr_attr) = new attribute(*(yyvsp[-1].str_attr), (yyvsp[0].attr_val_attr));
	   delete (yyvsp[-1].str_attr);
	 }
#line 1798 "parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 377 "parser.ypp" /* yacc.c:1646  */
    {
	         (yyval.attr_val_attr) = new standard_attribute_value<special_constant>(*(yyvsp[0].sc_attr));
		 delete (yyvsp[0].sc_attr);
	       }
#line 1807 "parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 382 "parser.ypp" /* yacc.c:1646  */
    {
	         (yyval.attr_val_attr) = new standard_attribute_value<symbol>(*(yyvsp[0].str_attr));
		 delete (yyvsp[0].str_attr);
	       }
#line 1816 "parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 387 "parser.ypp" /* yacc.c:1646  */
    {
	         (yyval.attr_val_attr) = new composite_attribute_value(std::move(*(yyvsp[-1].attr_val_v_attr)));
		 delete (yyvsp[-1].attr_val_v_attr);	         
	       }
#line 1825 "parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 394 "parser.ypp" /* yacc.c:1646  */
    {
	       (yyval.attr_s_attr) = (yyvsp[-1].attr_s_attr);
	       (yyval.attr_s_attr)->insert(std::move(*(yyvsp[0].attr_attr)));
	       delete (yyvsp[0].attr_attr);
	     }
#line 1835 "parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 400 "parser.ypp" /* yacc.c:1646  */
    {
	      (yyval.attr_s_attr) = new attribute_set(HASH_TABLE_SIZE);
	     }
#line 1843 "parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 406 "parser.ypp" /* yacc.c:1646  */
    {
	         (yyval.attr_s_attr) = (yyvsp[-1].attr_s_attr);
	         (yyval.attr_s_attr)->insert(std::move(*(yyvsp[0].attr_attr)));
	         delete (yyvsp[0].attr_attr);
		}
#line 1853 "parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 414 "parser.ypp" /* yacc.c:1646  */
    {
	          (yyval.sc_attr) = new special_constant(special_constant::T_NUMERAL, *(yyvsp[0].str_attr));
		  delete (yyvsp[0].str_attr);
	       }
#line 1862 "parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 419 "parser.ypp" /* yacc.c:1646  */
    {
	          (yyval.sc_attr) = new special_constant(special_constant::T_DECIMAL, *(yyvsp[0].str_attr));
		  delete (yyvsp[0].str_attr);
	     }
#line 1871 "parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 424 "parser.ypp" /* yacc.c:1646  */
    {
	          (yyval.sc_attr) = new special_constant(special_constant::T_BINARY, *(yyvsp[0].str_attr));
		  delete (yyvsp[0].str_attr);
	     }
#line 1880 "parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 429 "parser.ypp" /* yacc.c:1646  */
    {
	          (yyval.sc_attr) = new special_constant(special_constant::T_HEXADECIMAL, *(yyvsp[0].str_attr));
		  delete (yyvsp[0].str_attr);
	     }
#line 1889 "parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 434 "parser.ypp" /* yacc.c:1646  */
    {
	          (yyval.sc_attr) = new special_constant(special_constant::T_STRING_LITERAL, *(yyvsp[0].str_attr));
		  delete (yyvsp[0].str_attr);
	     }
#line 1898 "parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 442 "parser.ypp" /* yacc.c:1646  */
    {
		     (yyval.attr_val_v_attr) = (yyvsp[-1].attr_val_v_attr);
		     (yyval.attr_val_v_attr)->push_back((yyvsp[0].attr_val_attr));
		   }
#line 1907 "parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 447 "parser.ypp" /* yacc.c:1646  */
    {
		     (yyval.attr_val_v_attr) = new attribute_value_vector();
		   }
#line 1915 "parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 454 "parser.ypp" /* yacc.c:1646  */
    {
	      (yyval.sort_par_v_attr) = (yyvsp[-1].sort_par_v_attr);
	      (yyval.sort_par_v_attr)->push_back(sort_parameter(*(yyvsp[0].str_attr)));
	      pars.push_back(sort_parameter(*(yyvsp[0].str_attr)));
	      delete (yyvsp[0].str_attr);
 	    }
#line 1926 "parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 461 "parser.ypp" /* yacc.c:1646  */
    {
	      (yyval.sort_par_v_attr) = new sort_parameter_vector();
	    }
#line 1934 "parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 467 "parser.ypp" /* yacc.c:1646  */
    {
	  (yyval.sort_v_attr) = (yyvsp[-1].sort_v_attr);
	  (yyval.sort_v_attr)->push_back(*(yyvsp[0].sort_attr));
	  delete (yyvsp[0].sort_attr);  
	}
#line 1944 "parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 473 "parser.ypp" /* yacc.c:1646  */
    {
	  (yyval.sort_v_attr) = new sort_vector();
	}
#line 1952 "parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 479 "parser.ypp" /* yacc.c:1646  */
    {
	    (yyval.sort_v_attr) = (yyvsp[-1].sort_v_attr);
	    (yyval.sort_v_attr)->push_back(*(yyvsp[0].sort_attr));
	    delete (yyvsp[0].sort_attr);  
	   }
#line 1962 "parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 487 "parser.ypp" /* yacc.c:1646  */
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
                     create_sort(sort_symbol(*(yyvsp[0].str_attr))));
     }
     delete (yyvsp[0].str_attr);
    }
#line 1987 "parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 508 "parser.ypp" /* yacc.c:1646  */
    {
      (yyval.sort_attr) = new sort(yy_api->get_sort_factory()->create_sort(sort_symbol(*(yyvsp[0].id_attr))));
      delete (yyvsp[0].id_attr);
    }
#line 1996 "parser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 513 "parser.ypp" /* yacc.c:1646  */
    {
      (yyval.sort_attr) = new sort(yy_api->get_sort_factory()->create_sort(sort_symbol(*(yyvsp[-2].str_attr)), std::move(*(yyvsp[-1].sort_v_attr))));
      delete (yyvsp[-2].str_attr);
      delete (yyvsp[-1].sort_v_attr);
    }
#line 2006 "parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 519 "parser.ypp" /* yacc.c:1646  */
    {
      (yyval.sort_attr) = new sort(yy_api->get_sort_factory()->create_sort(sort_symbol(*(yyvsp[-2].id_attr)), std::move(*(yyvsp[-1].sort_v_attr))));
      delete (yyvsp[-2].id_attr);
      delete (yyvsp[-1].sort_v_attr);
    }
#line 2016 "parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 527 "parser.ypp" /* yacc.c:1646  */
    {
	    (yyval.id_attr) = new identifier(*(yyvsp[-2].str_attr), std::move(*(yyvsp[-1].num_v_attr)));
	    delete (yyvsp[-2].str_attr);
	    delete (yyvsp[-1].num_v_attr);
	  }
#line 2026 "parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 535 "parser.ypp" /* yacc.c:1646  */
    {
	     (yyval.id_attr) = new identifier(*(yyvsp[0].str_attr));
	     delete (yyvsp[0].str_attr);
	  }
#line 2035 "parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 540 "parser.ypp" /* yacc.c:1646  */
    {
	     (yyval.id_attr) = (yyvsp[0].id_attr);
	  }
#line 2043 "parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 546 "parser.ypp" /* yacc.c:1646  */
    {
	     (yyval.ss_attr) = new sort_symbol(*(yyvsp[0].id_attr));
	     delete (yyvsp[0].id_attr);
	   }
#line 2052 "parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 552 "parser.ypp" /* yacc.c:1646  */
    {
	        (yyval.fs_attr) = new function_symbol(*(yyvsp[0].id_attr));
		delete (yyvsp[0].id_attr);
	       }
#line 2061 "parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 559 "parser.ypp" /* yacc.c:1646  */
    {
	     (yyval.num_v_attr) = (yyvsp[-1].num_v_attr);
	     (yyval.num_v_attr)->push_back(atoi((yyvsp[0].str_attr)->c_str()));
	     delete (yyvsp[0].str_attr);
           }
#line 2071 "parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 565 "parser.ypp" /* yacc.c:1646  */
    {
	     (yyval.num_v_attr) = new identifier::index_vector();
	   }
#line 2079 "parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 571 "parser.ypp" /* yacc.c:1646  */
    {
	       (yyval.num_v_attr) = (yyvsp[-1].num_v_attr);
	       (yyval.num_v_attr)->push_back(atoi((yyvsp[0].str_attr)->c_str()));
	       delete (yyvsp[0].str_attr);
              }
#line 2089 "parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 579 "parser.ypp" /* yacc.c:1646  */
    {
               (yyval.sort_var_v_attr) = (yyvsp[-1].sort_var_v_attr);
	       (yyval.sort_var_v_attr)->push_back(*(yyvsp[0].sort_var_attr));
	       vars.push_back(*(yyvsp[0].sort_var_attr));
	       delete (yyvsp[0].sort_var_attr);
              }
#line 2100 "parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 586 "parser.ypp" /* yacc.c:1646  */
    {
	       (yyval.sort_var_v_attr) = new sorted_variable_vector(0, dummy_var);
	      }
#line 2108 "parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 592 "parser.ypp" /* yacc.c:1646  */
    {
                  (yyval.sort_var_v_attr) = (yyvsp[-1].sort_var_v_attr);
	          (yyval.sort_var_v_attr)->push_back(*(yyvsp[0].sort_var_attr));
		  vars.push_back(*(yyvsp[0].sort_var_attr));
	          delete (yyvsp[0].sort_var_attr);
                 }
#line 2119 "parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 601 "parser.ypp" /* yacc.c:1646  */
    {
	    (yyval.sort_var_attr) = new sorted_variable(*(yyvsp[-2].str_attr), *(yyvsp[-1].sort_attr));
	    delete (yyvsp[-2].str_attr);
	    delete (yyvsp[-1].sort_attr);
          }
#line 2129 "parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 609 "parser.ypp" /* yacc.c:1646  */
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(*(yyvsp[0].sc_attr)));
      delete (yyvsp[0].sc_attr);
    }
#line 2138 "parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 614 "parser.ypp" /* yacc.c:1646  */
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
                           create_expression(function_symbol(*(yyvsp[0].str_attr))));
     }
     delete (yyvsp[0].str_attr);
    }
#line 2164 "parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 636 "parser.ypp" /* yacc.c:1646  */
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(*(yyvsp[0].id_attr))));
      delete (yyvsp[0].id_attr);      	      
    }
#line 2173 "parser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 641 "parser.ypp" /* yacc.c:1646  */
    {
       (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(*(yyvsp[-2].str_attr)), *(yyvsp[-1].sort_attr)));
       delete (yyvsp[-2].str_attr);
       delete (yyvsp[-1].sort_attr);      
    }
#line 2183 "parser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 647 "parser.ypp" /* yacc.c:1646  */
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(*(yyvsp[-2].id_attr)), *(yyvsp[-1].sort_attr)));      
      delete (yyvsp[-2].id_attr);
      delete (yyvsp[-1].sort_attr);
    }
#line 2193 "parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 653 "parser.ypp" /* yacc.c:1646  */
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(*(yyvsp[-2].str_attr)), std::move(*(yyvsp[-1].term_v_attr))));
      delete (yyvsp[-2].str_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2203 "parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 659 "parser.ypp" /* yacc.c:1646  */
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(*(yyvsp[-4].str_attr)), std::move(*(yyvsp[-1].term_v_attr)), *(yyvsp[-3].sort_attr)));
      delete (yyvsp[-4].str_attr);
      delete (yyvsp[-3].sort_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2214 "parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 666 "parser.ypp" /* yacc.c:1646  */
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(*(yyvsp[-2].id_attr)), std::move(*(yyvsp[-1].term_v_attr))));
      delete (yyvsp[-2].id_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2224 "parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 672 "parser.ypp" /* yacc.c:1646  */
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(*(yyvsp[-4].id_attr)), std::move(*(yyvsp[-1].term_v_attr)), *(yyvsp[-3].sort_attr)));
      delete (yyvsp[-4].id_attr);
      delete (yyvsp[-3].sort_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2235 "parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 679 "parser.ypp" /* yacc.c:1646  */
    {
      std::size_t n = (yyvsp[-3].vb_v_attr)->size();
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(std::move(*(yyvsp[-3].vb_v_attr)), *(yyvsp[-1].term_attr)));
      vars.resize(vars.size() - n, dummy_var);
      delete (yyvsp[-3].vb_v_attr);
      delete (yyvsp[-1].term_attr);
    }
#line 2247 "parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 687 "parser.ypp" /* yacc.c:1646  */
    {
      std::size_t n = (yyvsp[-3].sort_var_v_attr)->size();
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(Q_FORALL, std::move(*(yyvsp[-3].sort_var_v_attr)), *(yyvsp[-1].term_attr)));
      vars.resize(vars.size() - n, dummy_var);
      delete (yyvsp[-3].sort_var_v_attr);
      delete (yyvsp[-1].term_attr);
    }
#line 2259 "parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 695 "parser.ypp" /* yacc.c:1646  */
    {
      std::size_t n = (yyvsp[-3].sort_var_v_attr)->size();
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(Q_EXISTS, std::move(*(yyvsp[-3].sort_var_v_attr)), *(yyvsp[-1].term_attr)));
      vars.resize(vars.size() - n, dummy_var);
      delete (yyvsp[-3].sort_var_v_attr);
      delete (yyvsp[-1].term_attr);      
    }
#line 2271 "parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 703 "parser.ypp" /* yacc.c:1646  */
    {
      (yyval.term_attr) = (yyvsp[-2].term_attr);
      (*(yyval.term_attr))->get_attributes() = std::move(*(yyvsp[-1].attr_s_attr));
      delete (yyvsp[-1].attr_s_attr);
      yy_api->check_label(*(yyval.term_attr));
    }
#line 2282 "parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 712 "parser.ypp" /* yacc.c:1646  */
    {
	     (yyval.vb_attr) = new variable_binding(*(yyvsp[-2].str_attr), *(yyvsp[-1].term_attr));
	     delete (yyvsp[-2].str_attr);
	     delete (yyvsp[-1].term_attr);
	   }
#line 2292 "parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 720 "parser.ypp" /* yacc.c:1646  */
    {
	         (yyval.vb_v_attr) = (yyvsp[-1].vb_v_attr);
		 (yyval.vb_v_attr)->push_back(*(yyvsp[0].vb_attr));
		 vars.push_back(sorted_variable((yyvsp[0].vb_attr)->get_variable(), (yyvsp[0].vb_attr)->get_expression()->get_sort()));
		 delete (yyvsp[0].vb_attr);
	       }
#line 2303 "parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 727 "parser.ypp" /* yacc.c:1646  */
    {
	         (yyval.vb_v_attr) = new variable_binding_vector();
	       }
#line 2311 "parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 733 "parser.ypp" /* yacc.c:1646  */
    {
	          (yyval.vb_v_attr) = (yyvsp[-1].vb_v_attr);
		  (yyval.vb_v_attr)->push_back(*(yyvsp[0].vb_attr));
        	  vars.push_back(sorted_variable((yyvsp[0].vb_attr)->get_variable(), (yyvsp[0].vb_attr)->get_expression()->get_sort()));
		  delete (yyvsp[0].vb_attr);
	         }
#line 2322 "parser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 742 "parser.ypp" /* yacc.c:1646  */
    {
	  (yyval.term_v_attr) = (yyvsp[-1].term_v_attr);
	  (yyval.term_v_attr)->push_back(*(yyvsp[0].term_attr));
	  delete (yyvsp[0].term_attr);
	}
#line 2332 "parser.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 748 "parser.ypp" /* yacc.c:1646  */
    {
	  (yyval.term_v_attr) = new expression_vector();
	}
#line 2340 "parser.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 754 "parser.ypp" /* yacc.c:1646  */
    {
	     (yyval.term_v_attr) = (yyvsp[-1].term_v_attr);
	     (yyval.term_v_attr)->push_back(*(yyvsp[0].term_attr));
	     delete (yyvsp[0].term_attr);
	   }
#line 2350 "parser.cpp" /* yacc.c:1646  */
    break;


#line 2354 "parser.cpp" /* yacc.c:1646  */
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

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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
                  yystos[*yyssp], yyvsp);
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
#line 760 "parser.ypp" /* yacc.c:1906  */
       
