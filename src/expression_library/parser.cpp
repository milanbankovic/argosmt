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


#line 96 "parser.cpp"

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
#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
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
    NUMERAL = 258,                 /* NUMERAL  */
    DECIMAL = 259,                 /* DECIMAL  */
    HEXADECIMAL = 260,             /* HEXADECIMAL  */
    BINARY = 261,                  /* BINARY  */
    STRING_LITERAL = 262,          /* STRING_LITERAL  */
    RW_PAR = 263,                  /* RW_PAR  */
    RW_NUMERAL = 264,              /* RW_NUMERAL  */
    RW_DECIMAL = 265,              /* RW_DECIMAL  */
    RW_STRING = 266,               /* RW_STRING  */
    RW_UNDERSCORE = 267,           /* RW_UNDERSCORE  */
    RW_EXCLAMATION = 268,          /* RW_EXCLAMATION  */
    RW_AS = 269,                   /* RW_AS  */
    RW_LET = 270,                  /* RW_LET  */
    RW_FORALL = 271,               /* RW_FORALL  */
    RW_EXISTS = 272,               /* RW_EXISTS  */
    RW_SET_LOGIC = 273,            /* RW_SET_LOGIC  */
    RW_SET_OPTION = 274,           /* RW_SET_OPTION  */
    RW_SET_INFO = 275,             /* RW_SET_INFO  */
    RW_DECLARE_SORT = 276,         /* RW_DECLARE_SORT  */
    RW_DEFINE_SORT = 277,          /* RW_DEFINE_SORT  */
    RW_EXPAND_SORT = 278,          /* RW_EXPAND_SORT  */
    RW_DECLARE_CONST = 279,        /* RW_DECLARE_CONST  */
    RW_DECLARE_FUN = 280,          /* RW_DECLARE_FUN  */
    RW_DEFINE_FUN = 281,           /* RW_DEFINE_FUN  */
    RW_EXPAND_TERM = 282,          /* RW_EXPAND_TERM  */
    RW_PUSH = 283,                 /* RW_PUSH  */
    RW_POP = 284,                  /* RW_POP  */
    RW_ASSERT = 285,               /* RW_ASSERT  */
    RW_CHECK_SAT = 286,            /* RW_CHECK_SAT  */
    RW_GET_SORT = 287,             /* RW_GET_SORT  */
    RW_GET_ASSERTIONS = 288,       /* RW_GET_ASSERTIONS  */
    RW_GET_PROOF = 289,            /* RW_GET_PROOF  */
    RW_GET_UNSAT_CORE = 290,       /* RW_GET_UNSAT_CORE  */
    RW_GET_VALUE = 291,            /* RW_GET_VALUE  */
    RW_GET_ASSIGNMENT = 292,       /* RW_GET_ASSIGNMENT  */
    RW_GET_OPTION = 293,           /* RW_GET_OPTION  */
    RW_GET_INFO = 294,             /* RW_GET_INFO  */
    RW_RESET_ASSERTIONS = 295,     /* RW_RESET_ASSERTIONS  */
    RW_RESET = 296,                /* RW_RESET  */
    RW_ECHO = 297,                 /* RW_ECHO  */
    RW_EXIT = 298,                 /* RW_EXIT  */
    SYMBOL = 299,                  /* SYMBOL  */
    KEYWORD = 300                  /* KEYWORD  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
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

#line 261 "parser.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMERAL = 3,                    /* NUMERAL  */
  YYSYMBOL_DECIMAL = 4,                    /* DECIMAL  */
  YYSYMBOL_HEXADECIMAL = 5,                /* HEXADECIMAL  */
  YYSYMBOL_BINARY = 6,                     /* BINARY  */
  YYSYMBOL_STRING_LITERAL = 7,             /* STRING_LITERAL  */
  YYSYMBOL_RW_PAR = 8,                     /* RW_PAR  */
  YYSYMBOL_RW_NUMERAL = 9,                 /* RW_NUMERAL  */
  YYSYMBOL_RW_DECIMAL = 10,                /* RW_DECIMAL  */
  YYSYMBOL_RW_STRING = 11,                 /* RW_STRING  */
  YYSYMBOL_RW_UNDERSCORE = 12,             /* RW_UNDERSCORE  */
  YYSYMBOL_RW_EXCLAMATION = 13,            /* RW_EXCLAMATION  */
  YYSYMBOL_RW_AS = 14,                     /* RW_AS  */
  YYSYMBOL_RW_LET = 15,                    /* RW_LET  */
  YYSYMBOL_RW_FORALL = 16,                 /* RW_FORALL  */
  YYSYMBOL_RW_EXISTS = 17,                 /* RW_EXISTS  */
  YYSYMBOL_RW_SET_LOGIC = 18,              /* RW_SET_LOGIC  */
  YYSYMBOL_RW_SET_OPTION = 19,             /* RW_SET_OPTION  */
  YYSYMBOL_RW_SET_INFO = 20,               /* RW_SET_INFO  */
  YYSYMBOL_RW_DECLARE_SORT = 21,           /* RW_DECLARE_SORT  */
  YYSYMBOL_RW_DEFINE_SORT = 22,            /* RW_DEFINE_SORT  */
  YYSYMBOL_RW_EXPAND_SORT = 23,            /* RW_EXPAND_SORT  */
  YYSYMBOL_RW_DECLARE_CONST = 24,          /* RW_DECLARE_CONST  */
  YYSYMBOL_RW_DECLARE_FUN = 25,            /* RW_DECLARE_FUN  */
  YYSYMBOL_RW_DEFINE_FUN = 26,             /* RW_DEFINE_FUN  */
  YYSYMBOL_RW_EXPAND_TERM = 27,            /* RW_EXPAND_TERM  */
  YYSYMBOL_RW_PUSH = 28,                   /* RW_PUSH  */
  YYSYMBOL_RW_POP = 29,                    /* RW_POP  */
  YYSYMBOL_RW_ASSERT = 30,                 /* RW_ASSERT  */
  YYSYMBOL_RW_CHECK_SAT = 31,              /* RW_CHECK_SAT  */
  YYSYMBOL_RW_GET_SORT = 32,               /* RW_GET_SORT  */
  YYSYMBOL_RW_GET_ASSERTIONS = 33,         /* RW_GET_ASSERTIONS  */
  YYSYMBOL_RW_GET_PROOF = 34,              /* RW_GET_PROOF  */
  YYSYMBOL_RW_GET_UNSAT_CORE = 35,         /* RW_GET_UNSAT_CORE  */
  YYSYMBOL_RW_GET_VALUE = 36,              /* RW_GET_VALUE  */
  YYSYMBOL_RW_GET_ASSIGNMENT = 37,         /* RW_GET_ASSIGNMENT  */
  YYSYMBOL_RW_GET_OPTION = 38,             /* RW_GET_OPTION  */
  YYSYMBOL_RW_GET_INFO = 39,               /* RW_GET_INFO  */
  YYSYMBOL_RW_RESET_ASSERTIONS = 40,       /* RW_RESET_ASSERTIONS  */
  YYSYMBOL_RW_RESET = 41,                  /* RW_RESET  */
  YYSYMBOL_RW_ECHO = 42,                   /* RW_ECHO  */
  YYSYMBOL_RW_EXIT = 43,                   /* RW_EXIT  */
  YYSYMBOL_SYMBOL = 44,                    /* SYMBOL  */
  YYSYMBOL_KEYWORD = 45,                   /* KEYWORD  */
  YYSYMBOL_46_ = 46,                       /* '('  */
  YYSYMBOL_47_ = 47,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 48,                  /* $accept  */
  YYSYMBOL_script = 49,                    /* script  */
  YYSYMBOL_command = 50,                   /* command  */
  YYSYMBOL_option = 51,                    /* option  */
  YYSYMBOL_attribute = 52,                 /* attribute  */
  YYSYMBOL_attribute_value = 53,           /* attribute_value  */
  YYSYMBOL_attribute_seq = 54,             /* attribute_seq  */
  YYSYMBOL_attribute_ne_seq = 55,          /* attribute_ne_seq  */
  YYSYMBOL_spec_constant = 56,             /* spec_constant  */
  YYSYMBOL_attribute_value_seq = 57,       /* attribute_value_seq  */
  YYSYMBOL_sort_par_seq = 58,              /* sort_par_seq  */
  YYSYMBOL_sort_seq = 59,                  /* sort_seq  */
  YYSYMBOL_sort_ne_seq = 60,               /* sort_ne_seq  */
  YYSYMBOL_sort = 61,                      /* sort  */
  YYSYMBOL_indexed_identifier = 62,        /* indexed_identifier  */
  YYSYMBOL_identifier = 63,                /* identifier  */
  YYSYMBOL_sort_symbol = 64,               /* sort_symbol  */
  YYSYMBOL_function_symbol = 65,           /* function_symbol  */
  YYSYMBOL_index_seq = 66,                 /* index_seq  */
  YYSYMBOL_index_ne_seq = 67,              /* index_ne_seq  */
  YYSYMBOL_sorted_var_seq = 68,            /* sorted_var_seq  */
  YYSYMBOL_sorted_var_ne_seq = 69,         /* sorted_var_ne_seq  */
  YYSYMBOL_sorted_var = 70,                /* sorted_var  */
  YYSYMBOL_term = 71,                      /* term  */
  YYSYMBOL_var_binding = 72,               /* var_binding  */
  YYSYMBOL_var_binding_ne_seq = 73,        /* var_binding_ne_seq  */
  YYSYMBOL_var_binding_ne_seq_par = 74,    /* var_binding_ne_seq_par  */
  YYSYMBOL_term_seq = 75,                  /* term_seq  */
  YYSYMBOL_term_ne_seq = 76                /* term_ne_seq  */
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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   232

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  90
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  211

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   300


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
     189,   194,   199,   208,   218,   231,   246,   251,   258,   265,
     272,   280,   285,   299,   303,   307,   313,   327,   339,   351,
     361,   380,   386,   392,   397,   404,   409,   414,   421,   428,
     433,   441,   446,   451,   456,   461,   469,   475,   481,   489,
     494,   501,   506,   514,   535,   540,   546,   554,   562,   567,
     573,   579,   586,   592,   599,   604,   610,   618,   626,   631,
     640,   648,   653,   675,   680,   686,   692,   698,   705,   711,
     718,   726,   734,   742,   751,   759,   765,   773,   781,   788,
     793
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
  "\"end of file\"", "error", "\"invalid token\"", "NUMERAL", "DECIMAL",
  "HEXADECIMAL", "BINARY", "STRING_LITERAL", "RW_PAR", "RW_NUMERAL",
  "RW_DECIMAL", "RW_STRING", "RW_UNDERSCORE", "RW_EXCLAMATION", "RW_AS",
  "RW_LET", "RW_FORALL", "RW_EXISTS", "RW_SET_LOGIC", "RW_SET_OPTION",
  "RW_SET_INFO", "RW_DECLARE_SORT", "RW_DEFINE_SORT", "RW_EXPAND_SORT",
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
  "var_binding_ne_seq", "var_binding_ne_seq_par", "term_seq",
  "term_ne_seq", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-91)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-91)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -91,    18,   -91,   -91,   178,   -91,   -30,   -22,   -22,    32,
      32,    36,    32,    13,    32,    26,    46,    51,    26,    15,
      26,    22,    54,    55,    58,    60,    28,    63,    62,    64,
      98,    65,    66,    35,    71,   -91,    72,   -91,   109,   -91,
     -91,   107,    76,   -91,    39,    78,   -91,   -91,    36,    80,
      81,    82,   -91,   -91,   -91,   -91,   -91,   -91,    31,   -91,
     -91,    84,    85,    86,    87,   -91,    89,   -91,   -91,   -91,
     -91,   -91,    92,    93,   -91,   -91,    94,   -91,   -91,   -91,
     -91,   -91,   -91,   -91,   -91,    99,    95,   -91,   -91,   -91,
     -91,    97,   -91,   -91,   -91,    26,    40,    83,   100,   101,
     -91,    79,   -91,   -91,   -91,   -91,   -91,   -91,    26,   102,
     -91,   -91,   -91,    21,   -91,   -91,    19,    36,   103,   108,
     -91,    27,     9,   -12,   -91,    36,    36,   110,    26,   -91,
     -91,   111,    48,   112,   113,   114,   -91,   -91,     8,   116,
     -91,    36,   117,   -91,   -91,    32,    36,   -91,   121,    36,
     -91,   -22,   120,   123,   125,   124,   -91,    49,   126,   129,
     131,   134,   -91,    36,    36,   -91,   -91,   135,   136,   -91,
     137,   139,   140,    36,    26,   141,   -91,   -91,   -91,    26,
     -91,   -91,   -91,   142,    26,    26,   143,   144,   -91,   -91,
     -91,   145,   146,   147,   175,   176,   -91,   -91,    14,   -91,
     -91,   -91,   -91,   -91,   177,   179,    36,   -91,   -91,   180,
     -91
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
      12,     0,     0,     0,    39,     0,     0,     0,     0,    68,
      68,     0,     0,     0,    88,     0,    37,    46,     0,     0,
      48,     0,    50,    55,    56,     0,     0,    50,     0,     0,
      67,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,    76,     0,     0,    78,    27,    62,    63,    57,
       0,     0,     0,     0,     0,    38,    83,    74,    75,     0,
      87,    85,    80,    67,     0,     0,     0,     0,     9,    51,
      13,     0,     0,     0,     0,     0,    89,    89,     0,    70,
      15,    84,    81,    82,     0,     0,     0,    77,    79,     0,
      14
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -91,   -91,   -91,   -91,     0,    41,   -91,   -91,   -24,   -91,
      53,   -90,    68,   -11,    -8,    88,   215,     3,   -91,   -91,
     138,    56,    69,    -5,    73,   -91,   -91,   -91,   -80
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     5,    34,    35,    81,   151,   152,    59,   113,
     116,   117,   118,   147,    46,    47,    41,    48,   138,   139,
     159,   160,   150,    61,   156,   157,   128,   108,   109
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      45,    39,    39,   122,    39,    39,    39,    60,    36,    82,
      60,   167,    60,    64,    32,    66,    50,    51,     2,     3,
     131,    49,   133,    33,    52,    53,    54,    55,    56,    52,
      53,    54,    55,    56,   148,   149,    89,    91,    52,    53,
      54,    55,    56,    85,    95,    96,    97,    98,    99,    62,
     102,    85,   168,    43,    63,    44,   146,    37,    43,    38,
      44,   206,    65,   140,     4,    79,   141,    80,   136,    67,
      57,   140,    58,    72,   145,   100,    37,   101,    38,    79,
      43,    80,    44,    88,   125,    38,    38,    60,   126,    82,
     124,    85,   163,   132,    38,   155,   180,    40,    40,   198,
      60,    68,    69,   134,    70,    76,   142,    71,    73,    74,
      86,    75,    77,    78,   153,   154,   204,   205,    83,    84,
      60,    85,    87,   158,   164,    90,    92,    93,    94,   127,
     170,   103,   104,   105,   106,   172,   107,    39,   174,   110,
     111,   112,   115,   114,   120,   121,   129,   130,   171,   135,
     143,   175,   186,   187,   137,   144,   155,   119,   162,   165,
     -90,   166,   191,   169,   -52,   173,    60,   176,   179,   192,
     177,    60,   178,   182,   193,   148,    60,    60,   184,   194,
     195,   185,   -65,   -66,   188,   189,   161,   190,   -40,   -69,
     196,   197,   199,   200,   201,   209,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,   202,   203,   207,    42,   208,   210,   183,     0,
     181,     0,   123
};

static const yytype_int16 yycheck[] =
{
      11,     9,    10,    93,    12,    13,    14,    15,     8,    33,
      18,     3,    20,    18,    44,    20,    13,    14,     0,     1,
     100,     8,   102,    45,     3,     4,     5,     6,     7,     3,
       4,     5,     6,     7,    46,    47,    44,    48,     3,     4,
       5,     6,     7,    12,    13,    14,    15,    16,    17,     3,
      58,    12,    44,    44,     3,    46,    47,    44,    44,    46,
      46,    47,    47,    44,    46,    44,    47,    46,    47,    47,
      44,    44,    46,    45,    47,    44,    44,    46,    46,    44,
      44,    46,    46,    44,    44,    46,    46,    95,    96,   113,
      95,    12,    44,    14,    46,    46,    47,     9,    10,   189,
     108,    47,    47,   108,    46,     7,   117,    47,    45,    47,
       3,    47,    47,    47,   125,   126,   196,   197,    47,    47,
     128,    12,    46,   128,   132,    47,    46,    46,    46,    46,
     141,    47,    47,    47,    47,   146,    47,   145,   149,    47,
      47,    47,    47,    44,    47,    92,    46,    46,   145,    47,
      47,   151,   163,   164,   113,    47,    46,    89,    47,    47,
      47,    47,   173,    47,    47,    44,   174,    47,    44,   174,
      47,   179,    47,    47,   179,    46,   184,   185,    47,   184,
     185,    47,    47,    47,    47,    46,   130,    47,    47,    47,
      47,    47,    47,    47,    47,   206,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    47,    47,    47,    10,    47,    47,   159,    -1,
     157,    -1,    94
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
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
      47,    58,    59,    68,    71,    44,    62,    46,    74,    46,
      46,    76,    14,    76,    71,    47,    47,    53,    66,    67,
      44,    47,    61,    47,    47,    47,    47,    61,    46,    47,
      70,    54,    55,    61,    61,    46,    72,    73,    71,    68,
      69,    69,    47,    44,    62,    47,    47,     3,    44,    47,
      61,    65,    61,    44,    61,    52,    47,    47,    47,    44,
      47,    72,    47,    70,    47,    47,    61,    61,    47,    46,
      47,    61,    71,    71,    71,    71,    47,    47,    59,    47,
      47,    47,    47,    47,    76,    76,    47,    47,    47,    61,
      47
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
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

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
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
       5,     7,     7,     5,     4,     2,     1,     3,     2,     0,
       2
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
  case 3: /* script: script error  */
#line 143 "parser.ypp"
       {
       if(!active_error)
       	 throw syntax_error_exception();
       }
#line 1466 "parser.cpp"
    break;

  case 5: /* command: '(' RW_SET_LOGIC SYMBOL ')'  */
#line 152 "parser.ypp"
        {
	  yy_api->set_logic(*(yyvsp[-1].str_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].str_attr);         
	}
#line 1477 "parser.cpp"
    break;

  case 6: /* command: '(' RW_SET_OPTION option ')'  */
#line 159 "parser.ypp"
        {
	  yy_api->set_option(*(yyvsp[-1].attr_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
          delete (yyvsp[-1].attr_attr); 
	}
#line 1488 "parser.cpp"
    break;

  case 7: /* command: '(' RW_SET_INFO attribute ')'  */
#line 166 "parser.ypp"
        {
	  yy_api->set_info(*(yyvsp[-1].attr_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].attr_attr); 
	}
#line 1499 "parser.cpp"
    break;

  case 8: /* command: '(' RW_DECLARE_SORT sort_symbol NUMERAL ')'  */
#line 173 "parser.ypp"
        {
	  yy_api->declare_sort(*(yyvsp[-2].ss_attr), atoi((yyvsp[-1].str_attr)->c_str()));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-2].ss_attr);
	  delete (yyvsp[-1].str_attr);
	}
#line 1511 "parser.cpp"
    break;

  case 9: /* command: '(' RW_DEFINE_SORT sort_symbol '(' sort_par_seq ')' sort ')'  */
#line 181 "parser.ypp"
        {
	  yy_api->define_sort(*(yyvsp[-5].ss_attr), std::move(*(yyvsp[-3].sort_par_v_attr)), *(yyvsp[-1].sort_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-5].ss_attr);
	  delete (yyvsp[-3].sort_par_v_attr);
	  delete (yyvsp[-1].sort_attr);
	}
#line 1524 "parser.cpp"
    break;

  case 10: /* command: '(' RW_EXPAND_SORT sort ')'  */
#line 190 "parser.ypp"
        {
	  yy_api->cout() << yy_api->expand_sort(*(yyvsp[-1].sort_attr)) << std::endl;
	  delete (yyvsp[-1].sort_attr);
	}
#line 1533 "parser.cpp"
    break;

  case 11: /* command: '(' RW_ECHO STRING_LITERAL ')'  */
#line 195 "parser.ypp"
        {
	   yy_api->echo(*(yyvsp[-1].str_attr));
	   delete (yyvsp[-1].str_attr);
	}
#line 1542 "parser.cpp"
    break;

  case 12: /* command: '(' RW_DECLARE_CONST function_symbol sort ')'  */
#line 200 "parser.ypp"
        {
	  yy_api->declare_const(*(yyvsp[-2].fs_attr), *(yyvsp[-1].sort_attr));
	  if(yy_api->print_success())
	    yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-2].fs_attr);
	  delete (yyvsp[-1].sort_attr);

	}
#line 1555 "parser.cpp"
    break;

  case 13: /* command: '(' RW_DECLARE_FUN function_symbol '(' sort_seq ')' sort ')'  */
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
#line 1569 "parser.cpp"
    break;

  case 14: /* command: '(' RW_DECLARE_FUN RW_PAR '(' sort_par_seq ')' function_symbol '(' sort_seq ')' sort ')'  */
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
#line 1586 "parser.cpp"
    break;

  case 15: /* command: '(' RW_DEFINE_FUN function_symbol '(' sorted_var_seq ')' sort term ')'  */
#line 232 "parser.ypp"
        {
	  std::size_t n = (yyvsp[-4].sort_var_v_attr)->size();
	  yy_api->define_function(*(yyvsp[-6].fs_attr), std::move(*(yyvsp[-4].sort_var_v_attr)), *(yyvsp[-2].sort_attr), *(yyvsp[-1].term_attr));
	  
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;

          vars.resize(vars.size() - n, dummy_var);

	  delete (yyvsp[-6].fs_attr);
	  delete (yyvsp[-4].sort_var_v_attr);
	  delete (yyvsp[-2].sort_attr);
	  delete (yyvsp[-1].term_attr);
	}
#line 1605 "parser.cpp"
    break;

  case 16: /* command: '(' RW_EXPAND_TERM term ')'  */
#line 247 "parser.ypp"
        {
	  yy_api->cout() << yy_api->expand_expression(*(yyvsp[-1].term_attr)) << std::endl;
	  delete (yyvsp[-1].term_attr);
	}
#line 1614 "parser.cpp"
    break;

  case 17: /* command: '(' RW_PUSH NUMERAL ')'  */
#line 252 "parser.ypp"
        {
	  yy_api->push(atoi((yyvsp[-1].str_attr)->c_str()));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].str_attr);
	}
#line 1625 "parser.cpp"
    break;

  case 18: /* command: '(' RW_POP NUMERAL ')'  */
#line 259 "parser.ypp"
        {
	  yy_api->pop(atoi((yyvsp[-1].str_attr)->c_str()));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].str_attr);
	}
#line 1636 "parser.cpp"
    break;

  case 19: /* command: '(' RW_ASSERT term ')'  */
#line 266 "parser.ypp"
        {
	  yy_api->assert_expression(*(yyvsp[-1].term_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].term_attr);
	}
#line 1647 "parser.cpp"
    break;

  case 20: /* command: '(' RW_GET_SORT term ')'  */
#line 273 "parser.ypp"
        {
	  if(yy_api->expand_definitions())
	   yy_api->cout() << yy_api->expand_sort((*(yyvsp[-1].term_attr))->get_sort()) << std::endl;
	  else
	   yy_api->cout() << (*(yyvsp[-1].term_attr))->get_sort() << std::endl;
	  delete (yyvsp[-1].term_attr);
	}
#line 1659 "parser.cpp"
    break;

  case 21: /* command: '(' RW_CHECK_SAT ')'  */
#line 281 "parser.ypp"
        {
	  check_sat_response csr = yy_api->check_sat();
	  yy_api->cout() << csr << std::endl;
	}
#line 1668 "parser.cpp"
    break;

  case 22: /* command: '(' RW_GET_ASSERTIONS ')'  */
#line 286 "parser.ypp"
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
#line 1686 "parser.cpp"
    break;

  case 23: /* command: '(' RW_RESET_ASSERTIONS ')'  */
#line 300 "parser.ypp"
        {
	  yy_api->reset_assertions();
	}
#line 1694 "parser.cpp"
    break;

  case 24: /* command: '(' RW_RESET ')'  */
#line 304 "parser.ypp"
        {
	  yy_api->reset();
	}
#line 1702 "parser.cpp"
    break;

  case 25: /* command: '(' RW_GET_PROOF ')'  */
#line 308 "parser.ypp"
        {
	  proof pr = yy_api->get_proof();
	  pr->print_proof(yy_api->cout());
	  yy_api->cout() << std::endl;
	}
#line 1712 "parser.cpp"
    break;

  case 26: /* command: '(' RW_GET_UNSAT_CORE ')'  */
#line 314 "parser.ypp"
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
#line 1730 "parser.cpp"
    break;

  case 27: /* command: '(' RW_GET_VALUE '(' term_ne_seq ')' ')'  */
#line 328 "parser.ypp"
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
#line 1746 "parser.cpp"
    break;

  case 28: /* command: '(' RW_GET_ASSIGNMENT ')'  */
#line 340 "parser.ypp"
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
#line 1762 "parser.cpp"
    break;

  case 29: /* command: '(' RW_GET_OPTION KEYWORD ')'  */
#line 352 "parser.ypp"
        {
	attribute_value * value = yy_api->get_option(*(yyvsp[-1].str_attr));
	if(value)
	{  
          value->print_value(yy_api->cout());
        }
        yy_api->cout() << std::endl;
	delete (yyvsp[-1].str_attr);   
	}
#line 1776 "parser.cpp"
    break;

  case 30: /* command: '(' RW_GET_INFO KEYWORD ')'  */
#line 362 "parser.ypp"
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
#line 1799 "parser.cpp"
    break;

  case 31: /* command: '(' RW_EXIT ')'  */
#line 381 "parser.ypp"
        {
	  YYACCEPT;
	}
#line 1807 "parser.cpp"
    break;

  case 32: /* option: attribute  */
#line 387 "parser.ypp"
       {
          (yyval.attr_attr) = (yyvsp[0].attr_attr);
       }
#line 1815 "parser.cpp"
    break;

  case 33: /* attribute: KEYWORD  */
#line 393 "parser.ypp"
         {
	   (yyval.attr_attr) = new attribute(*(yyvsp[0].str_attr));
	   delete (yyvsp[0].str_attr);	   
	 }
#line 1824 "parser.cpp"
    break;

  case 34: /* attribute: KEYWORD attribute_value  */
#line 398 "parser.ypp"
         {
	   (yyval.attr_attr) = new attribute(*(yyvsp[-1].str_attr), (yyvsp[0].attr_val_attr));
	   delete (yyvsp[-1].str_attr);
	 }
#line 1833 "parser.cpp"
    break;

  case 35: /* attribute_value: spec_constant  */
#line 405 "parser.ypp"
               {
	         (yyval.attr_val_attr) = new standard_attribute_value<special_constant>(*(yyvsp[0].sc_attr));
		 delete (yyvsp[0].sc_attr);
	       }
#line 1842 "parser.cpp"
    break;

  case 36: /* attribute_value: SYMBOL  */
#line 410 "parser.ypp"
               {
	         (yyval.attr_val_attr) = new standard_attribute_value<symbol>(*(yyvsp[0].str_attr));
		 delete (yyvsp[0].str_attr);
	       }
#line 1851 "parser.cpp"
    break;

  case 37: /* attribute_value: '(' attribute_value_seq ')'  */
#line 415 "parser.ypp"
               {
	         (yyval.attr_val_attr) = new composite_attribute_value(std::move(*(yyvsp[-1].attr_val_v_attr)));
		 delete (yyvsp[-1].attr_val_v_attr);	         
	       }
#line 1860 "parser.cpp"
    break;

  case 38: /* attribute_seq: attribute_seq attribute  */
#line 422 "parser.ypp"
             {
	       (yyval.attr_s_attr) = (yyvsp[-1].attr_s_attr);
	       (yyval.attr_s_attr)->insert(std::move(*(yyvsp[0].attr_attr)));
	       delete (yyvsp[0].attr_attr);
	     }
#line 1870 "parser.cpp"
    break;

  case 39: /* attribute_seq: %empty  */
#line 428 "parser.ypp"
             {
	      (yyval.attr_s_attr) = new attribute_set(HASH_TABLE_SIZE);
	     }
#line 1878 "parser.cpp"
    break;

  case 40: /* attribute_ne_seq: attribute_seq attribute  */
#line 434 "parser.ypp"
                {
	         (yyval.attr_s_attr) = (yyvsp[-1].attr_s_attr);
	         (yyval.attr_s_attr)->insert(std::move(*(yyvsp[0].attr_attr)));
	         delete (yyvsp[0].attr_attr);
		}
#line 1888 "parser.cpp"
    break;

  case 41: /* spec_constant: NUMERAL  */
#line 442 "parser.ypp"
               {
	          (yyval.sc_attr) = new special_constant(special_constant::T_NUMERAL, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	       }
#line 1897 "parser.cpp"
    break;

  case 42: /* spec_constant: DECIMAL  */
#line 447 "parser.ypp"
             {
	          (yyval.sc_attr) = new special_constant(special_constant::T_DECIMAL, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	     }
#line 1906 "parser.cpp"
    break;

  case 43: /* spec_constant: BINARY  */
#line 452 "parser.ypp"
             {
	          (yyval.sc_attr) = new special_constant(special_constant::T_BINARY, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	     }
#line 1915 "parser.cpp"
    break;

  case 44: /* spec_constant: HEXADECIMAL  */
#line 457 "parser.ypp"
             {
	          (yyval.sc_attr) = new special_constant(special_constant::T_HEXADECIMAL, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	     }
#line 1924 "parser.cpp"
    break;

  case 45: /* spec_constant: STRING_LITERAL  */
#line 462 "parser.ypp"
             {
	          (yyval.sc_attr) = new special_constant(special_constant::T_STRING_LITERAL, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	     }
#line 1933 "parser.cpp"
    break;

  case 46: /* attribute_value_seq: attribute_value_seq attribute_value  */
#line 470 "parser.ypp"
                   {
		     (yyval.attr_val_v_attr) = (yyvsp[-1].attr_val_v_attr);
		     (yyval.attr_val_v_attr)->push_back((yyvsp[0].attr_val_attr));
		   }
#line 1942 "parser.cpp"
    break;

  case 47: /* attribute_value_seq: %empty  */
#line 475 "parser.ypp"
                   {
		     (yyval.attr_val_v_attr) = new attribute_value_vector();
		   }
#line 1950 "parser.cpp"
    break;

  case 48: /* sort_par_seq: sort_par_seq SYMBOL  */
#line 482 "parser.ypp"
            {
	      (yyval.sort_par_v_attr) = (yyvsp[-1].sort_par_v_attr);
	      (yyval.sort_par_v_attr)->push_back(sort_parameter(std::move(*(yyvsp[0].str_attr))));
	      pars.push_back(sort_parameter(std::move(*(yyvsp[0].str_attr))));
	      delete (yyvsp[0].str_attr);
 	    }
#line 1961 "parser.cpp"
    break;

  case 49: /* sort_par_seq: %empty  */
#line 489 "parser.ypp"
            {
	      (yyval.sort_par_v_attr) = new sort_parameter_vector();
	    }
#line 1969 "parser.cpp"
    break;

  case 50: /* sort_seq: sort_seq sort  */
#line 495 "parser.ypp"
        {
	  (yyval.sort_v_attr) = (yyvsp[-1].sort_v_attr);
	  (yyval.sort_v_attr)->push_back(*(yyvsp[0].sort_attr));
	  delete (yyvsp[0].sort_attr);  
	}
#line 1979 "parser.cpp"
    break;

  case 51: /* sort_seq: %empty  */
#line 501 "parser.ypp"
        {
	  (yyval.sort_v_attr) = new sort_vector();
	}
#line 1987 "parser.cpp"
    break;

  case 52: /* sort_ne_seq: sort_seq sort  */
#line 507 "parser.ypp"
           {
	    (yyval.sort_v_attr) = (yyvsp[-1].sort_v_attr);
	    (yyval.sort_v_attr)->push_back(*(yyvsp[0].sort_attr));
	    delete (yyvsp[0].sort_attr);  
	   }
#line 1997 "parser.cpp"
    break;

  case 53: /* sort: SYMBOL  */
#line 515 "parser.ypp"
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
#line 2022 "parser.cpp"
    break;

  case 54: /* sort: indexed_identifier  */
#line 536 "parser.ypp"
    {
      (yyval.sort_attr) = new sort(yy_api->get_sort_factory()->create_sort(sort_symbol(std::move(*(yyvsp[0].id_attr)))));
      delete (yyvsp[0].id_attr);
    }
#line 2031 "parser.cpp"
    break;

  case 55: /* sort: '(' SYMBOL sort_ne_seq ')'  */
#line 541 "parser.ypp"
    {
      (yyval.sort_attr) = new sort(yy_api->get_sort_factory()->create_sort(sort_symbol(*(yyvsp[-2].str_attr)), std::move(*(yyvsp[-1].sort_v_attr))));
      delete (yyvsp[-2].str_attr);
      delete (yyvsp[-1].sort_v_attr);
    }
#line 2041 "parser.cpp"
    break;

  case 56: /* sort: '(' indexed_identifier sort_ne_seq ')'  */
#line 547 "parser.ypp"
    {
      (yyval.sort_attr) = new sort(yy_api->get_sort_factory()->create_sort(sort_symbol(*(yyvsp[-2].id_attr)), std::move(*(yyvsp[-1].sort_v_attr))));
      delete (yyvsp[-2].id_attr);
      delete (yyvsp[-1].sort_v_attr);
    }
#line 2051 "parser.cpp"
    break;

  case 57: /* indexed_identifier: '(' RW_UNDERSCORE SYMBOL index_ne_seq ')'  */
#line 555 "parser.ypp"
          {
	    (yyval.id_attr) = new identifier(std::move(*(yyvsp[-2].str_attr)), std::move(*(yyvsp[-1].index_v_attr)));
	    delete (yyvsp[-2].str_attr);
	    delete (yyvsp[-1].index_v_attr);
	  }
#line 2061 "parser.cpp"
    break;

  case 58: /* identifier: SYMBOL  */
#line 563 "parser.ypp"
          {
	     (yyval.id_attr) = new identifier(std::move(*(yyvsp[0].str_attr)));
	     delete (yyvsp[0].str_attr);
	  }
#line 2070 "parser.cpp"
    break;

  case 59: /* identifier: indexed_identifier  */
#line 568 "parser.ypp"
          {
	     (yyval.id_attr) = (yyvsp[0].id_attr);
	  }
#line 2078 "parser.cpp"
    break;

  case 60: /* sort_symbol: identifier  */
#line 574 "parser.ypp"
           {
	     (yyval.ss_attr) = new sort_symbol(std::move(*(yyvsp[0].id_attr)));
	     delete (yyvsp[0].id_attr);
	   }
#line 2087 "parser.cpp"
    break;

  case 61: /* function_symbol: identifier  */
#line 580 "parser.ypp"
               {
	        (yyval.fs_attr) = new function_symbol(std::move(*(yyvsp[0].id_attr)));
		delete (yyvsp[0].id_attr);
	       }
#line 2096 "parser.cpp"
    break;

  case 62: /* index_seq: index_seq NUMERAL  */
#line 587 "parser.ypp"
           {
	     (yyval.index_v_attr) = (yyvsp[-1].index_v_attr);
	     (yyval.index_v_attr)->push_back(atoi((yyvsp[0].str_attr)->c_str()));
	     delete (yyvsp[0].str_attr);
           }
#line 2106 "parser.cpp"
    break;

  case 63: /* index_seq: index_seq SYMBOL  */
#line 593 "parser.ypp"
           {
	     (yyval.index_v_attr) = (yyvsp[-1].index_v_attr);
	     (yyval.index_v_attr)->push_back(symbol(std::move(*(yyvsp[0].str_attr))));
	     delete (yyvsp[0].str_attr);
	   }
#line 2116 "parser.cpp"
    break;

  case 64: /* index_seq: %empty  */
#line 599 "parser.ypp"
           {
	     (yyval.index_v_attr) = new identifier::index_vector();
	   }
#line 2124 "parser.cpp"
    break;

  case 65: /* index_ne_seq: index_seq NUMERAL  */
#line 605 "parser.ypp"
              {
	       (yyval.index_v_attr) = (yyvsp[-1].index_v_attr);
	       (yyval.index_v_attr)->push_back(atoi((yyvsp[0].str_attr)->c_str()));
	       delete (yyvsp[0].str_attr);
              }
#line 2134 "parser.cpp"
    break;

  case 66: /* index_ne_seq: index_seq SYMBOL  */
#line 611 "parser.ypp"
              {
	        (yyval.index_v_attr) = (yyvsp[-1].index_v_attr);
	        (yyval.index_v_attr)->push_back(symbol(std::move(*(yyvsp[0].str_attr))));
	        delete (yyvsp[0].str_attr);
	      }
#line 2144 "parser.cpp"
    break;

  case 67: /* sorted_var_seq: sorted_var_seq sorted_var  */
#line 619 "parser.ypp"
              {
               (yyval.sort_var_v_attr) = (yyvsp[-1].sort_var_v_attr);
	       (yyval.sort_var_v_attr)->push_back(*(yyvsp[0].sort_var_attr));
	       vars.push_back(*(yyvsp[0].sort_var_attr));
	       delete (yyvsp[0].sort_var_attr);
              }
#line 2155 "parser.cpp"
    break;

  case 68: /* sorted_var_seq: %empty  */
#line 626 "parser.ypp"
              {
	       (yyval.sort_var_v_attr) = new sorted_variable_vector();
	      }
#line 2163 "parser.cpp"
    break;

  case 69: /* sorted_var_ne_seq: sorted_var_seq sorted_var  */
#line 632 "parser.ypp"
                 {
                  (yyval.sort_var_v_attr) = (yyvsp[-1].sort_var_v_attr);
	          (yyval.sort_var_v_attr)->push_back(*(yyvsp[0].sort_var_attr));
		  vars.push_back(*(yyvsp[0].sort_var_attr));
	          delete (yyvsp[0].sort_var_attr);
                 }
#line 2174 "parser.cpp"
    break;

  case 70: /* sorted_var: '(' SYMBOL sort ')'  */
#line 641 "parser.ypp"
          {
	    (yyval.sort_var_attr) = new sorted_variable(std::move(*(yyvsp[-2].str_attr)), *(yyvsp[-1].sort_attr));
	    delete (yyvsp[-2].str_attr);
	    delete (yyvsp[-1].sort_attr);
          }
#line 2184 "parser.cpp"
    break;

  case 71: /* term: spec_constant  */
#line 649 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(*(yyvsp[0].sc_attr)));
      delete (yyvsp[0].sc_attr);
    }
#line 2193 "parser.cpp"
    break;

  case 72: /* term: SYMBOL  */
#line 654 "parser.ypp"
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
#line 2219 "parser.cpp"
    break;

  case 73: /* term: indexed_identifier  */
#line 676 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[0].id_attr)))));
      delete (yyvsp[0].id_attr);      	      
    }
#line 2228 "parser.cpp"
    break;

  case 74: /* term: '(' RW_AS SYMBOL sort ')'  */
#line 681 "parser.ypp"
    {
       (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-2].str_attr))), *(yyvsp[-1].sort_attr)));
       delete (yyvsp[-2].str_attr);
       delete (yyvsp[-1].sort_attr);      
    }
#line 2238 "parser.cpp"
    break;

  case 75: /* term: '(' RW_AS indexed_identifier sort ')'  */
#line 687 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-2].id_attr))), *(yyvsp[-1].sort_attr)));      
      delete (yyvsp[-2].id_attr);
      delete (yyvsp[-1].sort_attr);
    }
#line 2248 "parser.cpp"
    break;

  case 76: /* term: '(' SYMBOL term_ne_seq ')'  */
#line 693 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-2].str_attr))), std::move(*(yyvsp[-1].term_v_attr))));
      delete (yyvsp[-2].str_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2258 "parser.cpp"
    break;

  case 77: /* term: '(' '(' RW_AS SYMBOL sort ')' term_ne_seq ')'  */
#line 699 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-4].str_attr))), std::move(*(yyvsp[-1].term_v_attr)), *(yyvsp[-3].sort_attr)));
      delete (yyvsp[-4].str_attr);
      delete (yyvsp[-3].sort_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2269 "parser.cpp"
    break;

  case 78: /* term: '(' indexed_identifier term_ne_seq ')'  */
#line 706 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-2].id_attr))), std::move(*(yyvsp[-1].term_v_attr))));
      delete (yyvsp[-2].id_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2279 "parser.cpp"
    break;

  case 79: /* term: '(' '(' RW_AS indexed_identifier sort ')' term_ne_seq ')'  */
#line 712 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-4].id_attr))), std::move(*(yyvsp[-1].term_v_attr)), *(yyvsp[-3].sort_attr)));
      delete (yyvsp[-4].id_attr);
      delete (yyvsp[-3].sort_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2290 "parser.cpp"
    break;

  case 80: /* term: '(' RW_LET var_binding_ne_seq_par term ')'  */
#line 719 "parser.ypp"
    {
      std::size_t n = (yyvsp[-2].vb_v_attr)->size();
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(std::move(*(yyvsp[-2].vb_v_attr)), *(yyvsp[-1].term_attr)));
      vars.resize(vars.size() - n, dummy_var);
      delete (yyvsp[-2].vb_v_attr);
      delete (yyvsp[-1].term_attr);
    }
#line 2302 "parser.cpp"
    break;

  case 81: /* term: '(' RW_FORALL '(' sorted_var_ne_seq ')' term ')'  */
#line 727 "parser.ypp"
    {
      std::size_t n = (yyvsp[-3].sort_var_v_attr)->size();
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(Q_FORALL, std::move(*(yyvsp[-3].sort_var_v_attr)), *(yyvsp[-1].term_attr)));
      vars.resize(vars.size() - n, dummy_var);
      delete (yyvsp[-3].sort_var_v_attr);
      delete (yyvsp[-1].term_attr);
    }
#line 2314 "parser.cpp"
    break;

  case 82: /* term: '(' RW_EXISTS '(' sorted_var_ne_seq ')' term ')'  */
#line 735 "parser.ypp"
    {
      std::size_t n = (yyvsp[-3].sort_var_v_attr)->size();
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(Q_EXISTS, std::move(*(yyvsp[-3].sort_var_v_attr)), *(yyvsp[-1].term_attr)));
      vars.resize(vars.size() - n, dummy_var);
      delete (yyvsp[-3].sort_var_v_attr);
      delete (yyvsp[-1].term_attr);      
    }
#line 2326 "parser.cpp"
    break;

  case 83: /* term: '(' RW_EXCLAMATION term attribute_ne_seq ')'  */
#line 743 "parser.ypp"
    {
      (yyval.term_attr) = (yyvsp[-2].term_attr);
      (*(yyval.term_attr))->get_attributes() = std::move(*(yyvsp[-1].attr_s_attr));
      delete (yyvsp[-1].attr_s_attr);
      yy_api->check_label(*(yyval.term_attr));
    }
#line 2337 "parser.cpp"
    break;

  case 84: /* var_binding: '(' SYMBOL term ')'  */
#line 752 "parser.ypp"
           {
	     (yyval.vb_attr) = new variable_binding(std::move(*(yyvsp[-2].str_attr)), *(yyvsp[-1].term_attr));
	     delete (yyvsp[-2].str_attr);
	     delete (yyvsp[-1].term_attr);
	   }
#line 2347 "parser.cpp"
    break;

  case 85: /* var_binding_ne_seq: var_binding_ne_seq var_binding  */
#line 760 "parser.ypp"
               {
	         (yyval.vb_v_attr) = (yyvsp[-1].vb_v_attr);
		 (yyval.vb_v_attr)->push_back(*(yyvsp[0].vb_attr));
		 delete (yyvsp[0].vb_attr);
	       }
#line 2357 "parser.cpp"
    break;

  case 86: /* var_binding_ne_seq: var_binding  */
#line 766 "parser.ypp"
               {
	         (yyval.vb_v_attr) = new variable_binding_vector();
		 (yyval.vb_v_attr)->push_back(*(yyvsp[0].vb_attr));
		 delete (yyvsp[0].vb_attr);
	       }
#line 2367 "parser.cpp"
    break;

  case 87: /* var_binding_ne_seq_par: '(' var_binding_ne_seq ')'  */
#line 774 "parser.ypp"
                 {
                     (yyval.vb_v_attr) = (yyvsp[-1].vb_v_attr);
		     const variable_binding_vector & v = *(yyvsp[-1].vb_v_attr);
		     for(unsigned i = 0; i < v.size(); i++)
		       vars.push_back(sorted_variable(v[i].get_variable(), v[i].get_expression()->get_inferred_sort()));		    
                 }
#line 2378 "parser.cpp"
    break;

  case 88: /* term_seq: term_seq term  */
#line 782 "parser.ypp"
        {
	  (yyval.term_v_attr) = (yyvsp[-1].term_v_attr);
	  (yyval.term_v_attr)->push_back(*(yyvsp[0].term_attr));
	  delete (yyvsp[0].term_attr);
	}
#line 2388 "parser.cpp"
    break;

  case 89: /* term_seq: %empty  */
#line 788 "parser.ypp"
        {
	  (yyval.term_v_attr) = new expression_vector();
	}
#line 2396 "parser.cpp"
    break;

  case 90: /* term_ne_seq: term_seq term  */
#line 794 "parser.ypp"
           {
	     (yyval.term_v_attr) = (yyvsp[-1].term_v_attr);
	     (yyval.term_v_attr)->push_back(*(yyvsp[0].term_attr));
	     delete (yyvsp[0].term_attr);
	   }
#line 2406 "parser.cpp"
    break;


#line 2410 "parser.cpp"

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

#line 800 "parser.ypp"
       
