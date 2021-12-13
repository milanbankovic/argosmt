AM_CXXFLAGS = -I$(top_srcdir)/src/auxiliary/ \
	      -I$(top_srcdir)/src/expression_library/ \
	      -I$(top_srcdir)/src/data_structures/ \
	      -I$(top_srcdir)/src/solver/ \
	      -I$(top_srcdir)/src/strategies/ \
	      -I$(top_srcdir)/src/theory_solvers/ \
	      -I$(top_srcdir)/src/

noinst_LIBRARIES = libquantifiers.a

libquantifiers_a_SOURCES = enumerative_quantifiers_processor.cpp \
	                   e_matching_quantifiers_processor.cpp

noinst_HEADERS = quantifiers_processor.hpp \
		 enumerative_quantifiers_processor.hpp \
	         e_matching_quantifiers_processor.hpp

AM_CXXFLAGS += -Wall
