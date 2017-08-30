	#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([argosmt], [current], [milan@matf.bg.ac.rs])
AC_CONFIG_SRCDIR([src/argosmt.cpp])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

AC_DEFINE_UNQUOTED(BUILDDATE, "`date -u`", [Date as reported by shell])

# Checking for programs
AC_PROG_LEX
AC_PROG_YACC
AC_PROG_RANLIB

# Clang++ use switch
use_clang=no
AC_ARG_ENABLE([clang++], [AC_HELP_STRING([--enable-clang++],
    [build using clang++ instead of g++ (default NO)])],
    [use_clang=$enableval])

if test "x$use_clang" = "xyes" ; then
      echo "---------------------------------"
      echo "-   Compiling using clang++     -"
      echo "---------------------------------"
      AC_PROG_CXX([clang++])
else
      AC_PROG_CXX([g++])
fi




# Checking for libraries
AC_CHECK_LIB([gmp],[main],[],[AC_MSG_ERROR([GMP library not found!], [1])]) 

# General C++ options
CXXFLAGS="-pedantic -Wall -std=c++14"
#CXXFLAGS+=" -msse -msse2 -msse3 -mfpmath=sse"

# Choosing build mode (optimize vs debug vs profile)
build_mode="optimize"
AC_ARG_ENABLE([optimize], [AC_HELP_STRING([--enable-optimize],
    [build with optimizations enabled (default YES);
        same as --enable-mode=optimize])],
    [test "x$enableval" = "xyes" && build_mode=optimize])

AC_ARG_ENABLE([debug], [AC_HELP_STRING([--enable-debug],
    [build with debugging information (default NO);
        same as --enable-mode=debug])],
    [test "$enableval" = "yes" && build_mode=debug])

AC_ARG_ENABLE([profile], [AC_HELP_STRING([--enable-profile],
    [build with profiling enabled (default NO);
        same as --enable-mode=profile])],
    [test "$enableval" = "yes" && build_mode=profile])


AC_ARG_ENABLE([mode], [AC_HELP_STRING([--enable-mode],
    [set build mode])],
    [if test "x$enableval" = "xdebug" ; then
       build_mode=debug
     elif test "x$enableval" = "xoptimize" ; then 
       build_mode=optimize
     elif test "x$enableval" = "xprofile" ; then
       build_mode=profile
     else
       AC_MSG_ERROR([--enable-mode must be optimize|debug|profile])
     fi])

OPTIMIZEFLAGS="-DNDEBUG -O3"
if test "x$build_mode" = "xoptimize" ; then
       echo "---------------------"
       echo "- optimize mode set -"
       echo "---------------------"
       CXXFLAGS="$CXXFLAGS $OPTIMIZEFLAGS"
fi

DEBUGFLAGS="-g"
if test "x$build_mode" = "xdebug" ; then
       echo "---------------------"
       echo "-   debug mode set  -"
       echo "---------------------"
       CXXFLAGS="$CXXFLAGS $DEBUGFLAGS"
fi

PROFILEFLAGS="-g -DNDEBUG -fno-inline-small-functions"
if test "x$build_mode" = "xprofile" ; then
       echo "---------------------"
       echo "- profile mode set  -"
       echo "---------------------"
       CXXFLAGS="$CXXFLAGS $PROFILEFLAGS"
fi


# General libraries and linking options
LIBS="-lgmp -lgmpxx -lrt"


# Choosing value type for unknowns in arithmetics
unknown_value_type=gmp
AC_ARG_ENABLE([gmp-values], [AC_HELP_STRING([--enable-gmp-values],
    [use GMP values in arithmetic theory (default YES)])],
    [test "x$enableval" = "xyes" && unknown_value_type=gmp])

AC_ARG_ENABLE([long-double-values], [AC_HELP_STRING([--enable-long-double-values],
    [use long double values in arithmetic theory (default NO)])],
    [test "x$enableval" = "xyes" && unknown_value_type=long_double])

AC_ARG_ENABLE([long-int-values], [AC_HELP_STRING([--enable-long-int-values],
    [use long int values in arithmetic theory (default NO)])],
    [test "x$enableval" = "xyes" && unknown_value_type=long_int])

AC_ARG_ENABLE([hybrid-int-values], [AC_HELP_STRING([--enable-hybrid-int-values],
    [use hybrid int values in arithmetic theory (default NO)])],
    [test "x$enableval" = "xyes" && unknown_value_type=hybrid_int])

AC_ARG_ENABLE([hybrid-long-values], [AC_HELP_STRING([--enable-hybrid-long-values],
    [use hybrid long values in arithmetic theory (default NO)])],
    [test "x$enableval" = "xyes" && unknown_value_type=hybrid_long])



if test "x$unknown_value_type" = "xgmp" ; then
      echo "-----------------------"
      echo "-   using GMP values  -"
      echo "-----------------------"
      CXXFLAGS+=" -D_MPQ_VALUE"
fi

if test "x$unknown_value_type" = "xlong_double" ; then
      echo "-------------------------------"
      echo "-   using long double values  -"
      echo "-------------------------------"
      CXXFLAGS+=" -D_LONG_DOUBLE_VALUE"
fi

if test "x$unknown_value_type" = "xlong_int" ; then
      echo "-------------------------------"
      echo "-   using long int values     -"
      echo "-------------------------------"
      CXXFLAGS+=" -D_LONG_VALUE"
fi

if test "x$unknown_value_type" = "xhybrid_int" ; then
      echo "-------------------------------"
      echo "-   using hybrid-int values   -"
      echo "-------------------------------"
      CXXFLAGS+=" -D_HYBRID_INT_VALUE"
fi

if test "x$unknown_value_type" = "xhybrid_long" ; then
      echo "-------------------------------"
      echo "-   using hybrid-long values  -"
      echo "-------------------------------"
      CXXFLAGS+=" -D_HYBRID_LONG_VALUE"
fi

use_kat=no
AC_ARG_ENABLE([katsirelos-alldiff-expl], [AC_HELP_STRING([--enable-katsirelos-alldiff-expl],
    [build with Katsirelos' alldiff explanation enabled (default NO)])],
    [use_kat=$enableval])

if test "x$use_kat" = "xyes" ; then
      echo "-------------------------------------"
      echo "-   using Katsirelos' explanations  -"
      echo "-------------------------------------"
      CXXFLAGS+=" -D_USE_KATSIRELOS_EXPLANATIONS"
fi

# TBB use switch
use_tbb=no

# Simplex paralelization switch
parallel_simplex=no
AC_ARG_ENABLE([parallel-simplex], [AC_HELP_STRING([--enable-parallel-simplex],
    [build with parallel simplex enabled (default NO)])],
    [parallel_simplex=$enableval])

if test "x$parallel_simplex" = "xyes" ; then
      echo "---------------------------------"
      echo "-   Parallel simplex enabled    -"
      echo "---------------------------------"

      CXXFLAGS+=" -D_PARALLEL_SIMPLEX"
      use_tbb=yes
fi

# Portfolio paralelization switch
parallel_portfolio=no
AC_ARG_ENABLE([parallel-portfolio], [AC_HELP_STRING([--enable-parallel-portfolio],
    [build with parallel portfolio enabled (default NO)])],
    [parallel_portfolio=$enableval])

if test "x$parallel_portfolio" = "xyes" ; then
      echo "---------------------------------"
      echo "-   Parallel portfolio enabled  -"
      echo "---------------------------------"

      CXXFLAGS+=" -D_PARALLEL_PORTFOLIO"
      use_tbb=yes
fi

# Specifying path to tbb library (if not installed on system)
tbb_path=''
AC_ARG_ENABLE([tbb-path], [AC_HELP_STRING([--enable-tbb-path=PATH],
    [path to TBB installation])],
    [test x$enableval != x && tbb_path=$enableval])

if test "x$tbb_path" = "x" ; then
   TBBPATHFLAGS=""
else
   TBBPATHFLAGS="-L$tbb_path"
fi

# Specifying path to tbb headers (if not installed on system)
tbb_include_path=''
AC_ARG_ENABLE([tbb-include-path], [AC_HELP_STRING([--enable-tbb-include-path=PATH],
    [path to TBB headers])],
    [test x$enableval != x && tbb_include_path=$enableval])

if test "x$tbb_include_path" = "x" ; then
   TBINCLUDEBPATHFLAGS=""
else
   TBBINCLUDEPATHFLAGS="-I$tbb_include_path"
fi

# Including TBB if needed
if test "x$use_tbb" = "xyes" ; then
      echo "------------------"
      echo "-   using TBB    -"
      echo "------------------"

      # If tbb library path is not specified, check for system installation
      if test "x$tbb_path" = "x" ; then
         AC_CHECK_LIB([tbb], [main], [], [AC_MSG_ERROR([TBB library not found!], [1])])
      fi

      CXXFLAGS+=" -D_USE_TBB $TBBINCLUDEPATHFLAGS"
      LDFLAGS="-fno-builtin-malloc -fno-builtin-free -fno-builtin-realloc -fno-builtin-calloc"
      LIBS+=" $TBBPATHFLAGS -ltbbmalloc_proxy -ltbbmalloc -ltbb"
fi



AC_CONFIG_FILES([Makefile
                 src/Makefile
		 src/auxiliary/Makefile
                 src/expression_library/Makefile
	 	 src/data_structures/Makefile
		 src/theories/Makefile
                 src/strategies/Makefile
		 src/solver/Makefile])
AC_OUTPUT
