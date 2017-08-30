AM_CXXFLAGS = -I$(top_srcdir)/src/auxiliary

AM_YFLAGS = -d
AM_CXXFLAGS += -Wall

noinst_LIBRARIES = libexpression.a

libexpression_a_SOURCES =  basic_types.cpp \
			   signature.cpp \
	                   sort.cpp \
			   expression.cpp \
	                   theory_declaration.cpp \
                           logic_declaration.cpp \
		           smtlib_api.cpp \
	                   parser.ypp \
	                   lexer.lpp

noinst_HEADERS =  basic_types.hpp \
	          search_mode.hpp \
	          signature.hpp \
                  sort.hpp \
	          expression.hpp \
	          theory_declaration.hpp \
	          logic_declaration.hpp \
		  smtlib_api.hpp \
	          expression_library.hpp
