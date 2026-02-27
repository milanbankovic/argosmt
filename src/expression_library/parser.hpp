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

#line 173 "parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
