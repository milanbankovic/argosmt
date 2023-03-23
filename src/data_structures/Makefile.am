AM_CXXFLAGS = -I$(top_srcdir)/src/auxiliary/ \
	      -I$(top_srcdir)/src/expression_library/ \
	      -I$(top_srcdir)/src/theory_solvers/ \
              -I$(top_srcdir)/src/strategies/ \
	      -I$(top_srcdir)/src/quantifiers/ \
              -I$(top_srcdir)/src/solver/ \
              -I$(top_srcdir)/src/

noinst_LIBRARIES = libdatastructures.a

libdatastructures_a_SOURCES =  clause.cpp \
		 	       conflict_set.cpp \
	                       explanation.cpp \
	                       trail.cpp \
		               proofs.cpp

noinst_HEADERS = argosmt_expression_data.hpp \
	         clause.hpp \
	         conflict_set.hpp \
	         explanation.hpp \
	         trail.hpp \
	         heap.hpp \
                 proofs.hpp

AM_CXXFLAGS += -Wall
