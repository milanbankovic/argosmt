AM_CXXFLAGS = -I$(top_srcdir)/src/auxiliary/ \
	      -I$(top_srcdir)/src/expression_library/ \
	      -I$(top_srcdir)/src/data_structures/ \
	      -I$(top_srcdir)/src/solver/ \
	      -I$(top_srcdir)/src/strategies/ \
	      -I$(top_srcdir)/src/quantifiers/ \
	      -I$(top_srcdir)/src/

noinst_LIBRARIES = libtheorysolvers.a

libtheorysolvers_a_SOURCES = clause_theory_solver.cpp \
		        euf_theory_solver.cpp \
	                arithmetic_theory_solver.cpp \
	                csp_theory_solver.cpp \
	                domain_handler.cpp \
		        alldiff_constraint_handler.cpp \
	                bipartite_graph.cpp \
	                sum_constraint_handler.cpp

noinst_HEADERS = theory_solver.hpp \
		 clause_theory_solver.hpp \
	         euf_theory_solver.hpp \
	         arithmetic_theory_solver.hpp \
	         csp_theory_solver.hpp \
		 constraint_handler.hpp \
	         domain_handler.hpp \
	         alldiff_constraint_handler.hpp \
		 bipartite_graph.hpp \
	         sum_constraint_handler.hpp \
	         linear_polynomial.hpp \
		 arithmetic_canonizer.hpp \
	         propagation_handler.hpp \
		 csp_variable_order_strategy.hpp \
	         csp_value_order_strategy.hpp \
		 simple_csp_value_order_strategy.hpp \
	         simple_csp_variable_order_strategy.hpp \
	         most_constrained_csp_variable_order_strategy.hpp \
	         most_preferred_csp_value_order_strategy.hpp \
	         csp_explanation_minimizer.hpp \
		 csp_common_data.hpp \
		 hybrid_types.hpp

AM_CXXFLAGS += -Wall
