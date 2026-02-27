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
    RW_UNDERSCORE = 264,           /* RW_UNDERSCORE  */
    RW_EXCLAMATION = 265,          /* RW_EXCLAMATION  */
    RW_AS = 266,                   /* RW_AS  */
    RW_LET = 267,                  /* RW_LET  */
    RW_FORALL = 268,               /* RW_FORALL  */
    RW_EXISTS = 269,               /* RW_EXISTS  */
    RW_SET_LOGIC = 270,            /* RW_SET_LOGIC  */
    RW_SET_OPTION = 271,           /* RW_SET_OPTION  */
    RW_SET_INFO = 272,             /* RW_SET_INFO  */
    RW_DECLARE_SORT = 273,         /* RW_DECLARE_SORT  */
    RW_DEFINE_SORT = 274,          /* RW_DEFINE_SORT  */
    RW_EXPAND_SORT = 275,          /* RW_EXPAND_SORT  */
    RW_DECLARE_CONST = 276,        /* RW_DECLARE_CONST  */
    RW_DECLARE_FUN = 277,          /* RW_DECLARE_FUN  */
    RW_DEFINE_FUN = 278,           /* RW_DEFINE_FUN  */
    RW_EXPAND_TERM = 279,          /* RW_EXPAND_TERM  */
    RW_PUSH = 280,                 /* RW_PUSH  */
    RW_POP = 281,                  /* RW_POP  */
    RW_ASSERT = 282,               /* RW_ASSERT  */
    RW_CHECK_SAT = 283,            /* RW_CHECK_SAT  */
    RW_GET_SORT = 284,             /* RW_GET_SORT  */
    RW_GET_ASSERTIONS = 285,       /* RW_GET_ASSERTIONS  */
    RW_GET_PROOF = 286,            /* RW_GET_PROOF  */
    RW_GET_UNSAT_CORE = 287,       /* RW_GET_UNSAT_CORE  */
    RW_GET_VALUE = 288,            /* RW_GET_VALUE  */
    RW_GET_ASSIGNMENT = 289,       /* RW_GET_ASSIGNMENT  */
    RW_GET_OPTION = 290,           /* RW_GET_OPTION  */
    RW_GET_INFO = 291,             /* RW_GET_INFO  */
    RW_RESET_ASSERTIONS = 292,     /* RW_RESET_ASSERTIONS  */
    RW_RESET = 293,                /* RW_RESET  */
    RW_ECHO = 294,                 /* RW_ECHO  */
    RW_EXIT = 295,                 /* RW_EXIT  */
    SYMBOL = 296,                  /* SYMBOL  */
    KEYWORD = 297                  /* KEYWORD  */
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
#define RW_UNDERSCORE 264
#define RW_EXCLAMATION 265
#define RW_AS 266
#define RW_LET 267
#define RW_FORALL 268
#define RW_EXISTS 269
#define RW_SET_LOGIC 270
#define RW_SET_OPTION 271
#define RW_SET_INFO 272
#define RW_DECLARE_SORT 273
#define RW_DEFINE_SORT 274
#define RW_EXPAND_SORT 275
#define RW_DECLARE_CONST 276
#define RW_DECLARE_FUN 277
#define RW_DEFINE_FUN 278
#define RW_EXPAND_TERM 279
#define RW_PUSH 280
#define RW_POP 281
#define RW_ASSERT 282
#define RW_CHECK_SAT 283
#define RW_GET_SORT 284
#define RW_GET_ASSERTIONS 285
#define RW_GET_PROOF 286
#define RW_GET_UNSAT_CORE 287
#define RW_GET_VALUE 288
#define RW_GET_ASSIGNMENT 289
#define RW_GET_OPTION 290
#define RW_GET_INFO 291
#define RW_RESET_ASSERTIONS 292
#define RW_RESET 293
#define RW_ECHO 294
#define RW_EXIT 295
#define SYMBOL 296
#define KEYWORD 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 113 "parser.ypp"

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

#line 255 "parser.cpp"

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
  YYSYMBOL_RW_UNDERSCORE = 9,              /* RW_UNDERSCORE  */
  YYSYMBOL_RW_EXCLAMATION = 10,            /* RW_EXCLAMATION  */
  YYSYMBOL_RW_AS = 11,                     /* RW_AS  */
  YYSYMBOL_RW_LET = 12,                    /* RW_LET  */
  YYSYMBOL_RW_FORALL = 13,                 /* RW_FORALL  */
  YYSYMBOL_RW_EXISTS = 14,                 /* RW_EXISTS  */
  YYSYMBOL_RW_SET_LOGIC = 15,              /* RW_SET_LOGIC  */
  YYSYMBOL_RW_SET_OPTION = 16,             /* RW_SET_OPTION  */
  YYSYMBOL_RW_SET_INFO = 17,               /* RW_SET_INFO  */
  YYSYMBOL_RW_DECLARE_SORT = 18,           /* RW_DECLARE_SORT  */
  YYSYMBOL_RW_DEFINE_SORT = 19,            /* RW_DEFINE_SORT  */
  YYSYMBOL_RW_EXPAND_SORT = 20,            /* RW_EXPAND_SORT  */
  YYSYMBOL_RW_DECLARE_CONST = 21,          /* RW_DECLARE_CONST  */
  YYSYMBOL_RW_DECLARE_FUN = 22,            /* RW_DECLARE_FUN  */
  YYSYMBOL_RW_DEFINE_FUN = 23,             /* RW_DEFINE_FUN  */
  YYSYMBOL_RW_EXPAND_TERM = 24,            /* RW_EXPAND_TERM  */
  YYSYMBOL_RW_PUSH = 25,                   /* RW_PUSH  */
  YYSYMBOL_RW_POP = 26,                    /* RW_POP  */
  YYSYMBOL_RW_ASSERT = 27,                 /* RW_ASSERT  */
  YYSYMBOL_RW_CHECK_SAT = 28,              /* RW_CHECK_SAT  */
  YYSYMBOL_RW_GET_SORT = 29,               /* RW_GET_SORT  */
  YYSYMBOL_RW_GET_ASSERTIONS = 30,         /* RW_GET_ASSERTIONS  */
  YYSYMBOL_RW_GET_PROOF = 31,              /* RW_GET_PROOF  */
  YYSYMBOL_RW_GET_UNSAT_CORE = 32,         /* RW_GET_UNSAT_CORE  */
  YYSYMBOL_RW_GET_VALUE = 33,              /* RW_GET_VALUE  */
  YYSYMBOL_RW_GET_ASSIGNMENT = 34,         /* RW_GET_ASSIGNMENT  */
  YYSYMBOL_RW_GET_OPTION = 35,             /* RW_GET_OPTION  */
  YYSYMBOL_RW_GET_INFO = 36,               /* RW_GET_INFO  */
  YYSYMBOL_RW_RESET_ASSERTIONS = 37,       /* RW_RESET_ASSERTIONS  */
  YYSYMBOL_RW_RESET = 38,                  /* RW_RESET  */
  YYSYMBOL_RW_ECHO = 39,                   /* RW_ECHO  */
  YYSYMBOL_RW_EXIT = 40,                   /* RW_EXIT  */
  YYSYMBOL_SYMBOL = 41,                    /* SYMBOL  */
  YYSYMBOL_KEYWORD = 42,                   /* KEYWORD  */
  YYSYMBOL_43_ = 43,                       /* '('  */
  YYSYMBOL_44_ = 44,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 45,                  /* $accept  */
  YYSYMBOL_script = 46,                    /* script  */
  YYSYMBOL_command = 47,                   /* command  */
  YYSYMBOL_option = 48,                    /* option  */
  YYSYMBOL_attribute = 49,                 /* attribute  */
  YYSYMBOL_attribute_value = 50,           /* attribute_value  */
  YYSYMBOL_attribute_seq = 51,             /* attribute_seq  */
  YYSYMBOL_attribute_ne_seq = 52,          /* attribute_ne_seq  */
  YYSYMBOL_spec_constant = 53,             /* spec_constant  */
  YYSYMBOL_attribute_value_seq = 54,       /* attribute_value_seq  */
  YYSYMBOL_sort_par_seq = 55,              /* sort_par_seq  */
  YYSYMBOL_sort_seq = 56,                  /* sort_seq  */
  YYSYMBOL_sort_ne_seq = 57,               /* sort_ne_seq  */
  YYSYMBOL_sort = 58,                      /* sort  */
  YYSYMBOL_indexed_identifier = 59,        /* indexed_identifier  */
  YYSYMBOL_identifier = 60,                /* identifier  */
  YYSYMBOL_sort_symbol = 61,               /* sort_symbol  */
  YYSYMBOL_function_symbol = 62,           /* function_symbol  */
  YYSYMBOL_index_seq = 63,                 /* index_seq  */
  YYSYMBOL_index_ne_seq = 64,              /* index_ne_seq  */
  YYSYMBOL_sorted_var_seq = 65,            /* sorted_var_seq  */
  YYSYMBOL_sorted_var_ne_seq = 66,         /* sorted_var_ne_seq  */
  YYSYMBOL_sorted_var = 67,                /* sorted_var  */
  YYSYMBOL_term = 68,                      /* term  */
  YYSYMBOL_var_binding = 69,               /* var_binding  */
  YYSYMBOL_var_binding_ne_seq = 70,        /* var_binding_ne_seq  */
  YYSYMBOL_var_binding_ne_seq_par = 71,    /* var_binding_ne_seq_par  */
  YYSYMBOL_term_seq = 72,                  /* term_seq  */
  YYSYMBOL_term_ne_seq = 73                /* term_ne_seq  */
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
#define YYLAST   234

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  92
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  213

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   297


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
      43,    44,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   138,   138,   139,   144,   148,   155,   162,   169,   177,
     186,   191,   196,   205,   215,   228,   243,   248,   255,   261,
     268,   275,   282,   290,   295,   309,   313,   317,   323,   337,
     349,   361,   371,   390,   396,   402,   407,   414,   419,   424,
     431,   438,   443,   451,   456,   461,   466,   471,   479,   485,
     491,   499,   504,   511,   516,   524,   545,   550,   556,   564,
     572,   577,   583,   589,   596,   602,   609,   614,   620,   628,
     636,   641,   650,   658,   663,   685,   690,   696,   702,   708,
     715,   721,   728,   736,   744,   752,   761,   769,   775,   783,
     791,   798,   803
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
  "HEXADECIMAL", "BINARY", "STRING_LITERAL", "RW_PAR", "RW_UNDERSCORE",
  "RW_EXCLAMATION", "RW_AS", "RW_LET", "RW_FORALL", "RW_EXISTS",
  "RW_SET_LOGIC", "RW_SET_OPTION", "RW_SET_INFO", "RW_DECLARE_SORT",
  "RW_DEFINE_SORT", "RW_EXPAND_SORT", "RW_DECLARE_CONST", "RW_DECLARE_FUN",
  "RW_DEFINE_FUN", "RW_EXPAND_TERM", "RW_PUSH", "RW_POP", "RW_ASSERT",
  "RW_CHECK_SAT", "RW_GET_SORT", "RW_GET_ASSERTIONS", "RW_GET_PROOF",
  "RW_GET_UNSAT_CORE", "RW_GET_VALUE", "RW_GET_ASSIGNMENT",
  "RW_GET_OPTION", "RW_GET_INFO", "RW_RESET_ASSERTIONS", "RW_RESET",
  "RW_ECHO", "RW_EXIT", "SYMBOL", "KEYWORD", "'('", "')'", "$accept",
  "script", "command", "option", "attribute", "attribute_value",
  "attribute_seq", "attribute_ne_seq", "spec_constant",
  "attribute_value_seq", "sort_par_seq", "sort_seq", "sort_ne_seq", "sort",
  "indexed_identifier", "identifier", "sort_symbol", "function_symbol",
  "index_seq", "index_ne_seq", "sorted_var_seq", "sorted_var_ne_seq",
  "sorted_var", "term", "var_binding", "var_binding_ne_seq",
  "var_binding_ne_seq_par", "term_seq", "term_ne_seq", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-93)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-93)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -93,    21,   -93,   -93,   183,   -93,   -17,     7,     7,    40,
      40,    43,    40,    15,    40,    27,     8,    11,    27,    36,
      27,    59,    60,    62,    17,    63,    46,    67,    66,    68,
     104,    69,    74,    35,    75,   -93,    76,   -93,   112,   -93,
     -93,   120,    81,   -93,    26,    83,   -93,   -93,    43,    85,
      86,    87,   -93,   -93,   -93,   -93,   -93,   -93,    34,   -93,
     -93,    89,    90,   -93,    92,   -93,    94,   -93,    97,   -93,
     -93,   -93,   -93,   -93,    98,    99,   -93,   -93,   100,   -93,
     -93,   -93,   -93,   -93,   -93,   -93,   -93,   105,   101,   -93,
     -93,   -93,   -93,   103,   -93,   -93,   -93,    27,    44,    88,
     106,   108,   -93,    84,   -93,   -93,   -93,   -93,   -93,   -93,
      27,   113,   -93,   -93,   -93,    22,   -93,   -93,    30,    43,
     114,   115,   -93,    38,    10,    29,   -93,    43,    43,   109,
      27,   -93,   -93,   116,    53,   117,   118,   119,   -93,   -93,
      16,   121,   -93,    43,   122,   -93,   -93,    40,    43,   -93,
     107,    43,   -93,     7,   123,   125,   126,   131,   -93,    54,
     130,   132,   133,   136,   -93,    43,    43,   -93,   -93,   139,
     140,   -93,   141,   143,   144,    43,    27,   145,   -93,   -93,
     -93,    27,   -93,   -93,   -93,   146,    27,    27,   147,   148,
     -93,   -93,   -93,   149,   150,   151,   152,   180,   -93,   -93,
      18,   -93,   -93,   -93,   -93,   -93,   181,   182,    43,   -93,
     -93,   184,   -93
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     1,     3,     0,     2,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    35,     0,    34,     0,    60,     0,    61,
      62,     0,     0,    55,     0,     0,    56,    63,     0,     0,
       0,     0,    43,    44,    46,    45,    47,    74,     0,    73,
      75,     0,     0,    18,     0,    20,     0,    23,     0,    24,
      27,    28,    91,    30,     0,     0,    25,    26,     0,    33,
       5,    38,    49,    36,    37,     6,     7,     0,     0,    51,
      53,    53,    10,     0,    51,    53,    70,     0,     0,     0,
       0,     0,    91,     0,    91,    16,    17,    19,    21,    22,
       0,     0,    31,    32,    11,     0,    66,     8,     0,     0,
       0,     0,    12,     0,     0,     0,    41,     0,     0,     0,
       0,    70,    70,     0,     0,     0,    90,     0,    39,    48,
       0,     0,    50,     0,    52,    57,    58,     0,     0,    52,
       0,     0,    69,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,    78,     0,     0,    80,    29,    64,
      65,    59,     0,     0,     0,     0,     0,    40,    85,    76,
      77,     0,    89,    87,    82,    69,     0,     0,     0,     0,
       9,    53,    13,     0,     0,     0,     0,     0,    91,    91,
       0,    72,    15,    86,    83,    84,     0,     0,     0,    79,
      81,     0,    14
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -93,   -93,   -93,   -93,     0,    20,   -93,   -93,   -24,   -93,
      93,   -92,    65,   -11,    -8,    91,   217,     3,   -93,   -93,
     134,   102,    70,    -5,    73,   -93,   -93,   -93,   -84
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     5,    34,    35,    83,   153,   154,    59,   115,
     118,   119,   120,   149,    46,    47,    41,    48,   140,   141,
     161,   162,   152,    61,   158,   159,   130,   110,   111
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      45,    39,    39,   124,    39,    39,    39,    60,    36,    84,
      60,    62,    60,    66,    64,    68,    50,    51,   133,   169,
     135,     2,     3,    49,    32,    52,    53,    54,    55,    56,
      52,    53,    54,    55,    56,    87,    91,    93,    52,    53,
      54,    55,    56,    87,    97,    98,    99,   100,   101,    33,
     104,    43,    63,    44,   148,    65,    37,   170,    38,    43,
      72,    44,   208,    81,     4,    82,   138,    90,    57,    38,
      58,   142,   150,   151,   143,   102,    81,   103,    82,   142,
      67,    37,   147,    38,    43,   127,    44,    38,    74,    60,
     128,    84,   126,    87,   165,   134,    38,   157,   182,   200,
      40,    40,    60,    69,    70,   136,    71,    73,   144,    75,
      76,    78,    77,    79,   206,   207,   155,   156,    80,    85,
      86,    87,    60,    88,    89,   160,   166,    92,    94,    95,
      96,   129,   172,   105,   106,   139,   107,   174,   108,    39,
     176,   109,   112,   113,   114,   117,   116,   122,   175,   131,
     173,   132,   157,   177,   188,   189,   121,   137,   145,   146,
     164,   167,   -92,   168,   193,   171,   -54,   178,    60,   179,
     180,   194,   181,    60,   184,   150,   195,   186,    60,    60,
     187,   196,   197,   -67,   -68,   190,   191,   123,   192,   -42,
     -71,   198,   199,   201,   202,   203,   204,   211,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,   205,   209,   210,    42,   212,     0,
     125,   185,   183,     0,   163
};

static const yytype_int16 yycheck[] =
{
      11,     9,    10,    95,    12,    13,    14,    15,     8,    33,
      18,     3,    20,    18,     3,    20,    13,    14,   102,     3,
     104,     0,     1,     8,    41,     3,     4,     5,     6,     7,
       3,     4,     5,     6,     7,     9,    44,    48,     3,     4,
       5,     6,     7,     9,    10,    11,    12,    13,    14,    42,
      58,    41,    44,    43,    44,    44,    41,    41,    43,    41,
      43,    43,    44,    41,    43,    43,    44,    41,    41,    43,
      43,    41,    43,    44,    44,    41,    41,    43,    43,    41,
      44,    41,    44,    43,    41,    41,    43,    43,    42,    97,
      98,   115,    97,     9,    41,    11,    43,    43,    44,   191,
       9,    10,   110,    44,    44,   110,    44,    44,   119,    42,
      44,     7,    44,    44,   198,   199,   127,   128,    44,    44,
      44,     9,   130,     3,    43,   130,   134,    44,    43,    43,
      43,    43,   143,    44,    44,   115,    44,   148,    44,   147,
     151,    44,    44,    44,    44,    44,    41,    44,    41,    43,
     147,    43,    43,   153,   165,   166,    91,    44,    44,    44,
      44,    44,    44,    44,   175,    44,    44,    44,   176,    44,
      44,   176,    41,   181,    44,    43,   181,    44,   186,   187,
      44,   186,   187,    44,    44,    44,    43,    94,    44,    44,
      44,    44,    44,    44,    44,    44,    44,   208,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    44,    44,    44,    10,    44,    -1,
      96,   161,   159,    -1,   132
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    46,     0,     1,    43,    47,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    48,    49,    49,    41,    43,    59,
      60,    61,    61,    41,    43,    58,    59,    60,    62,     8,
      62,    62,     3,     4,     5,     6,     7,    41,    43,    53,
      59,    68,     3,    44,     3,    44,    68,    44,    68,    44,
      44,    44,    43,    44,    42,    42,    44,    44,     7,    44,
      44,    41,    43,    50,    53,    44,    44,     9,     3,    43,
      41,    59,    44,    58,    43,    43,    43,    10,    11,    12,
      13,    14,    41,    43,    59,    44,    44,    44,    44,    44,
      72,    73,    44,    44,    44,    54,    41,    44,    55,    56,
      57,    57,    44,    55,    56,    65,    68,    41,    59,    43,
      71,    43,    43,    73,    11,    73,    68,    44,    44,    50,
      63,    64,    41,    44,    58,    44,    44,    44,    44,    58,
      43,    44,    67,    51,    52,    58,    58,    43,    69,    70,
      68,    65,    66,    66,    44,    41,    59,    44,    44,     3,
      41,    44,    58,    62,    58,    41,    58,    49,    44,    44,
      44,    41,    44,    69,    44,    67,    44,    44,    58,    58,
      44,    43,    44,    58,    68,    68,    68,    68,    44,    44,
      56,    44,    44,    44,    44,    44,    73,    73,    44,    44,
      44,    58,    44
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    45,    46,    46,    46,    47,    47,    47,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    47,    47,
      47,    47,    47,    47,    48,    49,    49,    50,    50,    50,
      51,    51,    52,    53,    53,    53,    53,    53,    54,    54,
      55,    55,    56,    56,    57,    58,    58,    58,    58,    59,
      60,    60,    61,    62,    63,    63,    63,    64,    64,    65,
      65,    66,    67,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    69,    70,    70,    71,
      72,    72,    73
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     2,     0,     4,     4,     4,     5,     8,
       4,     4,     5,     8,    12,     9,     4,     4,     3,     4,
       3,     4,     4,     3,     3,     3,     3,     3,     3,     6,
       3,     4,     4,     3,     1,     1,     2,     1,     1,     3,
       2,     0,     2,     1,     1,     1,     1,     1,     2,     0,
       2,     0,     2,     0,     2,     1,     1,     4,     4,     5,
       1,     1,     1,     1,     2,     2,     0,     2,     2,     2,
       0,     2,     4,     1,     1,     1,     5,     5,     4,     8,
       4,     8,     5,     7,     7,     5,     4,     2,     1,     3,
       2,     0,     2
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
#line 140 "parser.ypp"
       {
       if(!active_error)
       	 throw syntax_error_exception();
       }
#line 1455 "parser.cpp"
    break;

  case 5: /* command: '(' RW_SET_LOGIC SYMBOL ')'  */
#line 149 "parser.ypp"
        {
	  yy_api->set_logic(*(yyvsp[-1].str_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].str_attr);         
	}
#line 1466 "parser.cpp"
    break;

  case 6: /* command: '(' RW_SET_OPTION option ')'  */
#line 156 "parser.ypp"
        {
	  yy_api->set_option(*(yyvsp[-1].attr_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
          delete (yyvsp[-1].attr_attr); 
	}
#line 1477 "parser.cpp"
    break;

  case 7: /* command: '(' RW_SET_INFO attribute ')'  */
#line 163 "parser.ypp"
        {
	  yy_api->set_info(*(yyvsp[-1].attr_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].attr_attr); 
	}
#line 1488 "parser.cpp"
    break;

  case 8: /* command: '(' RW_DECLARE_SORT sort_symbol NUMERAL ')'  */
#line 170 "parser.ypp"
        {
	  yy_api->declare_sort(*(yyvsp[-2].ss_attr), atoi((yyvsp[-1].str_attr)->c_str()));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-2].ss_attr);
	  delete (yyvsp[-1].str_attr);
	}
#line 1500 "parser.cpp"
    break;

  case 9: /* command: '(' RW_DEFINE_SORT sort_symbol '(' sort_par_seq ')' sort ')'  */
#line 178 "parser.ypp"
        {
	  yy_api->define_sort(*(yyvsp[-5].ss_attr), std::move(*(yyvsp[-3].sort_par_v_attr)), *(yyvsp[-1].sort_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-5].ss_attr);
	  delete (yyvsp[-3].sort_par_v_attr);
	  delete (yyvsp[-1].sort_attr);
	}
#line 1513 "parser.cpp"
    break;

  case 10: /* command: '(' RW_EXPAND_SORT sort ')'  */
#line 187 "parser.ypp"
        {
	  yy_api->cout() << yy_api->expand_sort(*(yyvsp[-1].sort_attr)) << std::endl;
	  delete (yyvsp[-1].sort_attr);
	}
#line 1522 "parser.cpp"
    break;

  case 11: /* command: '(' RW_ECHO STRING_LITERAL ')'  */
#line 192 "parser.ypp"
        {
	   yy_api->echo(*(yyvsp[-1].str_attr));
	   delete (yyvsp[-1].str_attr);
	}
#line 1531 "parser.cpp"
    break;

  case 12: /* command: '(' RW_DECLARE_CONST function_symbol sort ')'  */
#line 197 "parser.ypp"
        {
	  yy_api->declare_const(*(yyvsp[-2].fs_attr), *(yyvsp[-1].sort_attr));
	  if(yy_api->print_success())
	    yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-2].fs_attr);
	  delete (yyvsp[-1].sort_attr);

	}
#line 1544 "parser.cpp"
    break;

  case 13: /* command: '(' RW_DECLARE_FUN function_symbol '(' sort_seq ')' sort ')'  */
#line 206 "parser.ypp"
        {
	  (yyvsp[-3].sort_v_attr)->push_back(*(yyvsp[-1].sort_attr));
	  yy_api->declare_function(*(yyvsp[-5].fs_attr), std::move(*(yyvsp[-3].sort_v_attr)));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-5].fs_attr);
	  delete (yyvsp[-3].sort_v_attr);
	  delete (yyvsp[-1].sort_attr);
	}
#line 1558 "parser.cpp"
    break;

  case 14: /* command: '(' RW_DECLARE_FUN RW_PAR '(' sort_par_seq ')' function_symbol '(' sort_seq ')' sort ')'  */
#line 216 "parser.ypp"
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
#line 1575 "parser.cpp"
    break;

  case 15: /* command: '(' RW_DEFINE_FUN function_symbol '(' sorted_var_seq ')' sort term ')'  */
#line 229 "parser.ypp"
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
#line 1594 "parser.cpp"
    break;

  case 16: /* command: '(' RW_EXPAND_TERM term ')'  */
#line 244 "parser.ypp"
        {
	  yy_api->cout() << yy_api->expand_expression(*(yyvsp[-1].term_attr)) << std::endl;
	  delete (yyvsp[-1].term_attr);
	}
#line 1603 "parser.cpp"
    break;

  case 17: /* command: '(' RW_PUSH NUMERAL ')'  */
#line 249 "parser.ypp"
        {
	  yy_api->push(atoi((yyvsp[-1].str_attr)->c_str()));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].str_attr);
	}
#line 1614 "parser.cpp"
    break;

  case 18: /* command: '(' RW_PUSH ')'  */
#line 256 "parser.ypp"
        {
	  yy_api->push(1);
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	}
#line 1624 "parser.cpp"
    break;

  case 19: /* command: '(' RW_POP NUMERAL ')'  */
#line 262 "parser.ypp"
        {
	  yy_api->pop(atoi((yyvsp[-1].str_attr)->c_str()));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].str_attr);
	}
#line 1635 "parser.cpp"
    break;

  case 20: /* command: '(' RW_POP ')'  */
#line 269 "parser.ypp"
        {
	  yy_api->pop(1);
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	}
#line 1645 "parser.cpp"
    break;

  case 21: /* command: '(' RW_ASSERT term ')'  */
#line 276 "parser.ypp"
        {
	  yy_api->assert_expression(*(yyvsp[-1].term_attr));
	  if(yy_api->print_success())
	   yy_api->cout() << "success" << std::endl;
	  delete (yyvsp[-1].term_attr);
	}
#line 1656 "parser.cpp"
    break;

  case 22: /* command: '(' RW_GET_SORT term ')'  */
#line 283 "parser.ypp"
        {
	  if(yy_api->expand_definitions())
	   yy_api->cout() << yy_api->expand_sort((*(yyvsp[-1].term_attr))->get_sort()) << std::endl;
	  else
	   yy_api->cout() << (*(yyvsp[-1].term_attr))->get_sort() << std::endl;
	  delete (yyvsp[-1].term_attr);
	}
#line 1668 "parser.cpp"
    break;

  case 23: /* command: '(' RW_CHECK_SAT ')'  */
#line 291 "parser.ypp"
        {
	  check_sat_response csr = yy_api->check_sat();
	  yy_api->cout() << csr << std::endl;
	}
#line 1677 "parser.cpp"
    break;

  case 24: /* command: '(' RW_GET_ASSERTIONS ')'  */
#line 296 "parser.ypp"
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
#line 1695 "parser.cpp"
    break;

  case 25: /* command: '(' RW_RESET_ASSERTIONS ')'  */
#line 310 "parser.ypp"
        {
	  yy_api->reset_assertions();
	}
#line 1703 "parser.cpp"
    break;

  case 26: /* command: '(' RW_RESET ')'  */
#line 314 "parser.ypp"
        {
	  yy_api->reset();
	}
#line 1711 "parser.cpp"
    break;

  case 27: /* command: '(' RW_GET_PROOF ')'  */
#line 318 "parser.ypp"
        {
	  proof pr = yy_api->get_proof();
	  pr->print_proof(yy_api->cout());
	  yy_api->cout() << std::endl;
	}
#line 1721 "parser.cpp"
    break;

  case 28: /* command: '(' RW_GET_UNSAT_CORE ')'  */
#line 324 "parser.ypp"
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
#line 1739 "parser.cpp"
    break;

  case 29: /* command: '(' RW_GET_VALUE '(' term_ne_seq ')' ')'  */
#line 338 "parser.ypp"
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
#line 1755 "parser.cpp"
    break;

  case 30: /* command: '(' RW_GET_ASSIGNMENT ')'  */
#line 350 "parser.ypp"
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
#line 1771 "parser.cpp"
    break;

  case 31: /* command: '(' RW_GET_OPTION KEYWORD ')'  */
#line 362 "parser.ypp"
        {
	attribute_value * value = yy_api->get_option(*(yyvsp[-1].str_attr));
	if(value)
	{  
          value->print_value(yy_api->cout());
        }
        yy_api->cout() << std::endl;
	delete (yyvsp[-1].str_attr);   
	}
#line 1785 "parser.cpp"
    break;

  case 32: /* command: '(' RW_GET_INFO KEYWORD ')'  */
#line 372 "parser.ypp"
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
#line 1808 "parser.cpp"
    break;

  case 33: /* command: '(' RW_EXIT ')'  */
#line 391 "parser.ypp"
        {
	  YYACCEPT;
	}
#line 1816 "parser.cpp"
    break;

  case 34: /* option: attribute  */
#line 397 "parser.ypp"
       {
          (yyval.attr_attr) = (yyvsp[0].attr_attr);
       }
#line 1824 "parser.cpp"
    break;

  case 35: /* attribute: KEYWORD  */
#line 403 "parser.ypp"
         {
	   (yyval.attr_attr) = new attribute(*(yyvsp[0].str_attr));
	   delete (yyvsp[0].str_attr);	   
	 }
#line 1833 "parser.cpp"
    break;

  case 36: /* attribute: KEYWORD attribute_value  */
#line 408 "parser.ypp"
         {
	   (yyval.attr_attr) = new attribute(*(yyvsp[-1].str_attr), (yyvsp[0].attr_val_attr));
	   delete (yyvsp[-1].str_attr);
	 }
#line 1842 "parser.cpp"
    break;

  case 37: /* attribute_value: spec_constant  */
#line 415 "parser.ypp"
               {
	         (yyval.attr_val_attr) = new standard_attribute_value<special_constant>(*(yyvsp[0].sc_attr));
		 delete (yyvsp[0].sc_attr);
	       }
#line 1851 "parser.cpp"
    break;

  case 38: /* attribute_value: SYMBOL  */
#line 420 "parser.ypp"
               {
	         (yyval.attr_val_attr) = new standard_attribute_value<symbol>(*(yyvsp[0].str_attr));
		 delete (yyvsp[0].str_attr);
	       }
#line 1860 "parser.cpp"
    break;

  case 39: /* attribute_value: '(' attribute_value_seq ')'  */
#line 425 "parser.ypp"
               {
	         (yyval.attr_val_attr) = new composite_attribute_value(std::move(*(yyvsp[-1].attr_val_v_attr)));
		 delete (yyvsp[-1].attr_val_v_attr);	         
	       }
#line 1869 "parser.cpp"
    break;

  case 40: /* attribute_seq: attribute_seq attribute  */
#line 432 "parser.ypp"
             {
	       (yyval.attr_s_attr) = (yyvsp[-1].attr_s_attr);
	       (yyval.attr_s_attr)->insert(std::move(*(yyvsp[0].attr_attr)));
	       delete (yyvsp[0].attr_attr);
	     }
#line 1879 "parser.cpp"
    break;

  case 41: /* attribute_seq: %empty  */
#line 438 "parser.ypp"
             {
	      (yyval.attr_s_attr) = new attribute_set(HASH_TABLE_SIZE);
	     }
#line 1887 "parser.cpp"
    break;

  case 42: /* attribute_ne_seq: attribute_seq attribute  */
#line 444 "parser.ypp"
                {
	         (yyval.attr_s_attr) = (yyvsp[-1].attr_s_attr);
	         (yyval.attr_s_attr)->insert(std::move(*(yyvsp[0].attr_attr)));
	         delete (yyvsp[0].attr_attr);
		}
#line 1897 "parser.cpp"
    break;

  case 43: /* spec_constant: NUMERAL  */
#line 452 "parser.ypp"
               {
	          (yyval.sc_attr) = new special_constant(special_constant::T_NUMERAL, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	       }
#line 1906 "parser.cpp"
    break;

  case 44: /* spec_constant: DECIMAL  */
#line 457 "parser.ypp"
             {
	          (yyval.sc_attr) = new special_constant(special_constant::T_DECIMAL, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	     }
#line 1915 "parser.cpp"
    break;

  case 45: /* spec_constant: BINARY  */
#line 462 "parser.ypp"
             {
	          (yyval.sc_attr) = new special_constant(special_constant::T_BINARY, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	     }
#line 1924 "parser.cpp"
    break;

  case 46: /* spec_constant: HEXADECIMAL  */
#line 467 "parser.ypp"
             {
	          (yyval.sc_attr) = new special_constant(special_constant::T_HEXADECIMAL, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	     }
#line 1933 "parser.cpp"
    break;

  case 47: /* spec_constant: STRING_LITERAL  */
#line 472 "parser.ypp"
             {
	          (yyval.sc_attr) = new special_constant(special_constant::T_STRING_LITERAL, std::move(*(yyvsp[0].str_attr)));
		  delete (yyvsp[0].str_attr);
	     }
#line 1942 "parser.cpp"
    break;

  case 48: /* attribute_value_seq: attribute_value_seq attribute_value  */
#line 480 "parser.ypp"
                   {
		     (yyval.attr_val_v_attr) = (yyvsp[-1].attr_val_v_attr);
		     (yyval.attr_val_v_attr)->push_back((yyvsp[0].attr_val_attr));
		   }
#line 1951 "parser.cpp"
    break;

  case 49: /* attribute_value_seq: %empty  */
#line 485 "parser.ypp"
                   {
		     (yyval.attr_val_v_attr) = new attribute_value_vector();
		   }
#line 1959 "parser.cpp"
    break;

  case 50: /* sort_par_seq: sort_par_seq SYMBOL  */
#line 492 "parser.ypp"
            {
	      (yyval.sort_par_v_attr) = (yyvsp[-1].sort_par_v_attr);
	      (yyval.sort_par_v_attr)->push_back(sort_parameter(std::move(*(yyvsp[0].str_attr))));
	      pars.push_back(sort_parameter(std::move(*(yyvsp[0].str_attr))));
	      delete (yyvsp[0].str_attr);
 	    }
#line 1970 "parser.cpp"
    break;

  case 51: /* sort_par_seq: %empty  */
#line 499 "parser.ypp"
            {
	      (yyval.sort_par_v_attr) = new sort_parameter_vector();
	    }
#line 1978 "parser.cpp"
    break;

  case 52: /* sort_seq: sort_seq sort  */
#line 505 "parser.ypp"
        {
	  (yyval.sort_v_attr) = (yyvsp[-1].sort_v_attr);
	  (yyval.sort_v_attr)->push_back(*(yyvsp[0].sort_attr));
	  delete (yyvsp[0].sort_attr);  
	}
#line 1988 "parser.cpp"
    break;

  case 53: /* sort_seq: %empty  */
#line 511 "parser.ypp"
        {
	  (yyval.sort_v_attr) = new sort_vector();
	}
#line 1996 "parser.cpp"
    break;

  case 54: /* sort_ne_seq: sort_seq sort  */
#line 517 "parser.ypp"
           {
	    (yyval.sort_v_attr) = (yyvsp[-1].sort_v_attr);
	    (yyval.sort_v_attr)->push_back(*(yyvsp[0].sort_attr));
	    delete (yyvsp[0].sort_attr);  
	   }
#line 2006 "parser.cpp"
    break;

  case 55: /* sort: SYMBOL  */
#line 525 "parser.ypp"
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
#line 2031 "parser.cpp"
    break;

  case 56: /* sort: indexed_identifier  */
#line 546 "parser.ypp"
    {
      (yyval.sort_attr) = new sort(yy_api->get_sort_factory()->create_sort(sort_symbol(std::move(*(yyvsp[0].id_attr)))));
      delete (yyvsp[0].id_attr);
    }
#line 2040 "parser.cpp"
    break;

  case 57: /* sort: '(' SYMBOL sort_ne_seq ')'  */
#line 551 "parser.ypp"
    {
      (yyval.sort_attr) = new sort(yy_api->get_sort_factory()->create_sort(sort_symbol(*(yyvsp[-2].str_attr)), std::move(*(yyvsp[-1].sort_v_attr))));
      delete (yyvsp[-2].str_attr);
      delete (yyvsp[-1].sort_v_attr);
    }
#line 2050 "parser.cpp"
    break;

  case 58: /* sort: '(' indexed_identifier sort_ne_seq ')'  */
#line 557 "parser.ypp"
    {
      (yyval.sort_attr) = new sort(yy_api->get_sort_factory()->create_sort(sort_symbol(*(yyvsp[-2].id_attr)), std::move(*(yyvsp[-1].sort_v_attr))));
      delete (yyvsp[-2].id_attr);
      delete (yyvsp[-1].sort_v_attr);
    }
#line 2060 "parser.cpp"
    break;

  case 59: /* indexed_identifier: '(' RW_UNDERSCORE SYMBOL index_ne_seq ')'  */
#line 565 "parser.ypp"
          {
	    (yyval.id_attr) = new identifier(std::move(*(yyvsp[-2].str_attr)), std::move(*(yyvsp[-1].index_v_attr)));
	    delete (yyvsp[-2].str_attr);
	    delete (yyvsp[-1].index_v_attr);
	  }
#line 2070 "parser.cpp"
    break;

  case 60: /* identifier: SYMBOL  */
#line 573 "parser.ypp"
          {
	     (yyval.id_attr) = new identifier(std::move(*(yyvsp[0].str_attr)));
	     delete (yyvsp[0].str_attr);
	  }
#line 2079 "parser.cpp"
    break;

  case 61: /* identifier: indexed_identifier  */
#line 578 "parser.ypp"
          {
	     (yyval.id_attr) = (yyvsp[0].id_attr);
	  }
#line 2087 "parser.cpp"
    break;

  case 62: /* sort_symbol: identifier  */
#line 584 "parser.ypp"
           {
	     (yyval.ss_attr) = new sort_symbol(std::move(*(yyvsp[0].id_attr)));
	     delete (yyvsp[0].id_attr);
	   }
#line 2096 "parser.cpp"
    break;

  case 63: /* function_symbol: identifier  */
#line 590 "parser.ypp"
               {
	        (yyval.fs_attr) = new function_symbol(std::move(*(yyvsp[0].id_attr)));
		delete (yyvsp[0].id_attr);
	       }
#line 2105 "parser.cpp"
    break;

  case 64: /* index_seq: index_seq NUMERAL  */
#line 597 "parser.ypp"
           {
	     (yyval.index_v_attr) = (yyvsp[-1].index_v_attr);
	     (yyval.index_v_attr)->push_back(atoi((yyvsp[0].str_attr)->c_str()));
	     delete (yyvsp[0].str_attr);
           }
#line 2115 "parser.cpp"
    break;

  case 65: /* index_seq: index_seq SYMBOL  */
#line 603 "parser.ypp"
           {
	     (yyval.index_v_attr) = (yyvsp[-1].index_v_attr);
	     (yyval.index_v_attr)->push_back(symbol(std::move(*(yyvsp[0].str_attr))));
	     delete (yyvsp[0].str_attr);
	   }
#line 2125 "parser.cpp"
    break;

  case 66: /* index_seq: %empty  */
#line 609 "parser.ypp"
           {
	     (yyval.index_v_attr) = new identifier::index_vector();
	   }
#line 2133 "parser.cpp"
    break;

  case 67: /* index_ne_seq: index_seq NUMERAL  */
#line 615 "parser.ypp"
              {
	       (yyval.index_v_attr) = (yyvsp[-1].index_v_attr);
	       (yyval.index_v_attr)->push_back(atoi((yyvsp[0].str_attr)->c_str()));
	       delete (yyvsp[0].str_attr);
              }
#line 2143 "parser.cpp"
    break;

  case 68: /* index_ne_seq: index_seq SYMBOL  */
#line 621 "parser.ypp"
              {
	        (yyval.index_v_attr) = (yyvsp[-1].index_v_attr);
	        (yyval.index_v_attr)->push_back(symbol(std::move(*(yyvsp[0].str_attr))));
	        delete (yyvsp[0].str_attr);
	      }
#line 2153 "parser.cpp"
    break;

  case 69: /* sorted_var_seq: sorted_var_seq sorted_var  */
#line 629 "parser.ypp"
              {
               (yyval.sort_var_v_attr) = (yyvsp[-1].sort_var_v_attr);
	       (yyval.sort_var_v_attr)->push_back(*(yyvsp[0].sort_var_attr));
	       vars.push_back(*(yyvsp[0].sort_var_attr));
	       delete (yyvsp[0].sort_var_attr);
              }
#line 2164 "parser.cpp"
    break;

  case 70: /* sorted_var_seq: %empty  */
#line 636 "parser.ypp"
              {
	       (yyval.sort_var_v_attr) = new sorted_variable_vector();
	      }
#line 2172 "parser.cpp"
    break;

  case 71: /* sorted_var_ne_seq: sorted_var_seq sorted_var  */
#line 642 "parser.ypp"
                 {
                  (yyval.sort_var_v_attr) = (yyvsp[-1].sort_var_v_attr);
	          (yyval.sort_var_v_attr)->push_back(*(yyvsp[0].sort_var_attr));
		  vars.push_back(*(yyvsp[0].sort_var_attr));
	          delete (yyvsp[0].sort_var_attr);
                 }
#line 2183 "parser.cpp"
    break;

  case 72: /* sorted_var: '(' SYMBOL sort ')'  */
#line 651 "parser.ypp"
          {
	    (yyval.sort_var_attr) = new sorted_variable(std::move(*(yyvsp[-2].str_attr)), *(yyvsp[-1].sort_attr));
	    delete (yyvsp[-2].str_attr);
	    delete (yyvsp[-1].sort_attr);
          }
#line 2193 "parser.cpp"
    break;

  case 73: /* term: spec_constant  */
#line 659 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(*(yyvsp[0].sc_attr)));
      delete (yyvsp[0].sc_attr);
    }
#line 2202 "parser.cpp"
    break;

  case 74: /* term: SYMBOL  */
#line 664 "parser.ypp"
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
#line 2228 "parser.cpp"
    break;

  case 75: /* term: indexed_identifier  */
#line 686 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[0].id_attr)))));
      delete (yyvsp[0].id_attr);      	      
    }
#line 2237 "parser.cpp"
    break;

  case 76: /* term: '(' RW_AS SYMBOL sort ')'  */
#line 691 "parser.ypp"
    {
       (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-2].str_attr))), *(yyvsp[-1].sort_attr)));
       delete (yyvsp[-2].str_attr);
       delete (yyvsp[-1].sort_attr);      
    }
#line 2247 "parser.cpp"
    break;

  case 77: /* term: '(' RW_AS indexed_identifier sort ')'  */
#line 697 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-2].id_attr))), *(yyvsp[-1].sort_attr)));      
      delete (yyvsp[-2].id_attr);
      delete (yyvsp[-1].sort_attr);
    }
#line 2257 "parser.cpp"
    break;

  case 78: /* term: '(' SYMBOL term_ne_seq ')'  */
#line 703 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-2].str_attr))), std::move(*(yyvsp[-1].term_v_attr))));
      delete (yyvsp[-2].str_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2267 "parser.cpp"
    break;

  case 79: /* term: '(' '(' RW_AS SYMBOL sort ')' term_ne_seq ')'  */
#line 709 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-4].str_attr))), std::move(*(yyvsp[-1].term_v_attr)), *(yyvsp[-3].sort_attr)));
      delete (yyvsp[-4].str_attr);
      delete (yyvsp[-3].sort_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2278 "parser.cpp"
    break;

  case 80: /* term: '(' indexed_identifier term_ne_seq ')'  */
#line 716 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-2].id_attr))), std::move(*(yyvsp[-1].term_v_attr))));
      delete (yyvsp[-2].id_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2288 "parser.cpp"
    break;

  case 81: /* term: '(' '(' RW_AS indexed_identifier sort ')' term_ne_seq ')'  */
#line 722 "parser.ypp"
    {
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(function_symbol(std::move(*(yyvsp[-4].id_attr))), std::move(*(yyvsp[-1].term_v_attr)), *(yyvsp[-3].sort_attr)));
      delete (yyvsp[-4].id_attr);
      delete (yyvsp[-3].sort_attr);
      delete (yyvsp[-1].term_v_attr);
    }
#line 2299 "parser.cpp"
    break;

  case 82: /* term: '(' RW_LET var_binding_ne_seq_par term ')'  */
#line 729 "parser.ypp"
    {
      std::size_t n = (yyvsp[-2].vb_v_attr)->size();
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(std::move(*(yyvsp[-2].vb_v_attr)), *(yyvsp[-1].term_attr)));
      vars.resize(vars.size() - n, dummy_var);
      delete (yyvsp[-2].vb_v_attr);
      delete (yyvsp[-1].term_attr);
    }
#line 2311 "parser.cpp"
    break;

  case 83: /* term: '(' RW_FORALL '(' sorted_var_ne_seq ')' term ')'  */
#line 737 "parser.ypp"
    {
      std::size_t n = (yyvsp[-3].sort_var_v_attr)->size();
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(Q_FORALL, std::move(*(yyvsp[-3].sort_var_v_attr)), *(yyvsp[-1].term_attr)));
      vars.resize(vars.size() - n, dummy_var);
      delete (yyvsp[-3].sort_var_v_attr);
      delete (yyvsp[-1].term_attr);
    }
#line 2323 "parser.cpp"
    break;

  case 84: /* term: '(' RW_EXISTS '(' sorted_var_ne_seq ')' term ')'  */
#line 745 "parser.ypp"
    {
      std::size_t n = (yyvsp[-3].sort_var_v_attr)->size();
      (yyval.term_attr) = new expression(yy_api->get_expression_factory()->create_expression(Q_EXISTS, std::move(*(yyvsp[-3].sort_var_v_attr)), *(yyvsp[-1].term_attr)));
      vars.resize(vars.size() - n, dummy_var);
      delete (yyvsp[-3].sort_var_v_attr);
      delete (yyvsp[-1].term_attr);      
    }
#line 2335 "parser.cpp"
    break;

  case 85: /* term: '(' RW_EXCLAMATION term attribute_ne_seq ')'  */
#line 753 "parser.ypp"
    {
      (yyval.term_attr) = (yyvsp[-2].term_attr);
      (*(yyval.term_attr))->get_attributes() = std::move(*(yyvsp[-1].attr_s_attr));
      delete (yyvsp[-1].attr_s_attr);
      yy_api->check_label(*(yyval.term_attr));
    }
#line 2346 "parser.cpp"
    break;

  case 86: /* var_binding: '(' SYMBOL term ')'  */
#line 762 "parser.ypp"
           {
	     (yyval.vb_attr) = new variable_binding(std::move(*(yyvsp[-2].str_attr)), *(yyvsp[-1].term_attr));
	     delete (yyvsp[-2].str_attr);
	     delete (yyvsp[-1].term_attr);
	   }
#line 2356 "parser.cpp"
    break;

  case 87: /* var_binding_ne_seq: var_binding_ne_seq var_binding  */
#line 770 "parser.ypp"
               {
	         (yyval.vb_v_attr) = (yyvsp[-1].vb_v_attr);
		 (yyval.vb_v_attr)->push_back(*(yyvsp[0].vb_attr));
		 delete (yyvsp[0].vb_attr);
	       }
#line 2366 "parser.cpp"
    break;

  case 88: /* var_binding_ne_seq: var_binding  */
#line 776 "parser.ypp"
               {
	         (yyval.vb_v_attr) = new variable_binding_vector();
		 (yyval.vb_v_attr)->push_back(*(yyvsp[0].vb_attr));
		 delete (yyvsp[0].vb_attr);
	       }
#line 2376 "parser.cpp"
    break;

  case 89: /* var_binding_ne_seq_par: '(' var_binding_ne_seq ')'  */
#line 784 "parser.ypp"
                 {
                     (yyval.vb_v_attr) = (yyvsp[-1].vb_v_attr);
		     const variable_binding_vector & v = *(yyvsp[-1].vb_v_attr);
		     for(unsigned i = 0; i < v.size(); i++)
		       vars.push_back(sorted_variable(v[i].get_variable(), v[i].get_expression()->get_inferred_sort()));		    
                 }
#line 2387 "parser.cpp"
    break;

  case 90: /* term_seq: term_seq term  */
#line 792 "parser.ypp"
        {
	  (yyval.term_v_attr) = (yyvsp[-1].term_v_attr);
	  (yyval.term_v_attr)->push_back(*(yyvsp[0].term_attr));
	  delete (yyvsp[0].term_attr);
	}
#line 2397 "parser.cpp"
    break;

  case 91: /* term_seq: %empty  */
#line 798 "parser.ypp"
        {
	  (yyval.term_v_attr) = new expression_vector();
	}
#line 2405 "parser.cpp"
    break;

  case 92: /* term_ne_seq: term_seq term  */
#line 804 "parser.ypp"
           {
	     (yyval.term_v_attr) = (yyvsp[-1].term_v_attr);
	     (yyval.term_v_attr)->push_back(*(yyvsp[0].term_attr));
	     delete (yyvsp[0].term_attr);
	   }
#line 2415 "parser.cpp"
    break;


#line 2419 "parser.cpp"

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

#line 810 "parser.ypp"
       
