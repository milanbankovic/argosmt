AM_CXXFLAGS = -I$(top_srcdir)/src/expression_library/ \
	      -I$(top_srcdir)/src/data_structures/ \
	      -I$(top_srcdir)/src/theories/ \
	      -I$(top_srcdir)/src/strategies/ \
	      -I$(top_srcdir)/src/auxiliary/

noinst_LIBRARIES = libsolver.a

libsolver_a_SOURCES = solver.cpp

libsolver_a_LIBADD = 

noinst_HEADERS = solver.hpp \
	         solver_observer.hpp \
	         logging_solver_observer.hpp \
	         statistics_solver_observer.hpp

AM_CXXFLAGS += -Wall
