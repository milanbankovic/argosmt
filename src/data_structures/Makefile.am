AM_CXXFLAGS = -I$(top_srcdir)/src/auxiliary/ \
	      -I$(top_srcdir)/src/expression_library/ \
	      -I$(top_srcdir)/src/theories/ \
              -I$(top_srcdir)/src/strategies/ \
              -I$(top_srcdir)/src/solver/

noinst_LIBRARIES = libdatastructures.a

libdatastructures_a_SOURCES =  clause.cpp \
		 	       conflict_set.cpp \
	                       explanation.cpp \
	                       trail.cpp

noinst_HEADERS = argosmt_expression_data.hpp \
	         clause.hpp \
	         conflict_set.hpp \
	         explanation.hpp \
	         trail.hpp \
	         heap.hpp

AM_CXXFLAGS += -Wall
