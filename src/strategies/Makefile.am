AM_CXXFLAGS = -I$(top_srcdir)/src/auxiliary/ \
	      -I$(top_srcdir)/src/expression_library/ \
	      -I$(top_srcdir)/src/data_structures/ \
	      -I$(top_srcdir)/src/solver/ \
	      -I$(top_srcdir)/src/theory_solvers/ \
	      -I$(top_srcdir)/src/quantifiers/ \
	      -I$(top_srcdir)/src/

noinst_LIBRARIES = libstrategies.a

libstrategies_a_SOURCES = first_undefined_literal_selection_strategy.cpp \
			  vsids_literal_selection_strategy.cpp \
			  conflict_counting_restart_strategy.cpp

noinst_HEADERS = literal_selection_strategy.hpp \
		 polarity_selection_strategy.hpp \
		 restart_strategy.hpp \
		 forget_strategy.hpp \
		 forget_selection_strategy.hpp \
                 first_undefined_literal_selection_strategy.hpp \
		 random_literal_selection_strategy.hpp \
		 random_decorater_literal_selection_strategy.hpp \
		 vsids_literal_selection_strategy.hpp \
		 interactive_literal_selection_strategy.hpp \
                 always_positive_polarity_selection_strategy.hpp \
                 always_negative_polarity_selection_strategy.hpp \
                 saved_polarity_selection_strategy.hpp \
		 random_polarity_selection_strategy.hpp \
		 interactive_polarity_selection_strategy.hpp \
		 never_forget_strategy.hpp \
                 geometric_forget_strategy.hpp \
		 all_forget_selection_strategy.hpp \
                 percent_to_forget_selection_strategy.hpp \
		 clause_activity_forget_selection_strategy.hpp \
		 clause_length_forget_selection_strategy.hpp \
                 never_restart_strategy.hpp \
		 conflict_counting_restart_strategy.hpp

AM_CXXFLAGS += -Wall
