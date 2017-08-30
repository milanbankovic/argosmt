AM_CXXFLAGS = -I$(top_srcdir)/src/auxiliary \
	      -I$(top_srcdir)/src/expression_library \
	      -I$(top_srcdir)/src/data_structures \
	      -I$(top_srcdir)/src/theories \
	      -I$(top_srcdir)/src/strategies \
	      -I$(top_srcdir)/src/solver

SUBDIRS = auxiliary expression_library data_structures theories strategies solver 

bin_PROGRAMS = argosmt_dimacs argosmt

noinst_HEADERS = argosmt_solver_interface.hpp dimacs_solver_interface.hpp

argosmt_dimacs_SOURCES = dimacs_solver_interface.cpp argosmt_dimacs.cpp 

argosmt_SOURCES = argosmt_solver_interface.cpp argosmt.cpp 

LDADD = auxiliary/libauxiliary.a \
	solver/libsolver.a \
	strategies/libstrategies.a \
	theories/libtheories.a \
	data_structures/libdatastructures.a \
	expression_library/libexpression.a 

AM_CXXFLAGS += -Wall
