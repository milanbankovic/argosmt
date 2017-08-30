/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
#line 112 "parser.ypp" /* yacc.c:1909  */

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

#line 158 "parser.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
