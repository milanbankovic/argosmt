/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2014 Milan Bankovic (milan@matf.bg.ac.rs)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public Lcense
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
***************************************************************************/

#ifndef _ARGOSMT_SOLVER_INTERFACE_H
#define _ARGOSMT_SOLVER_INTERFACE_H

#include <iostream>
#include "solver.hpp"
#include "logging_solver_observer.hpp"
#include "statistics_solver_observer.hpp"
#include "first_undefined_literal_selection_strategy.hpp"
#include "random_literal_selection_strategy.hpp"
#include "vsids_literal_selection_strategy.hpp"
#include "interactive_literal_selection_strategy.hpp"
#include "random_decorater_literal_selection_strategy.hpp"
#include "always_positive_polarity_selection_strategy.hpp"
#include "always_negative_polarity_selection_strategy.hpp"
#include "saved_polarity_selection_strategy.hpp"
#include "random_polarity_selection_strategy.hpp"
#include "interactive_polarity_selection_strategy.hpp"
#include "never_restart_strategy.hpp"
#include "conflict_counting_restart_strategy.hpp"
#include "never_forget_strategy.hpp"
#include "geometric_forget_strategy.hpp"
#include "all_forget_selection_strategy.hpp"
#include "clause_activity_forget_selection_strategy.hpp"
#include "clause_length_forget_selection_strategy.hpp"
#include "clause_theory_solver.hpp"
#include "euf_theory_solver.hpp"
#include "arithmetic_theory_solver.hpp"
#include "csp_theory_solver.hpp"
#include "wall_clock.hpp"
#include "cmd_line_parser.hpp"
#include "formula_transformer.hpp"
#include "enumerative_quantifiers_processor.hpp"
#include "e_matching_quantifiers_processor.hpp"

#include "simple_csp_variable_order_strategy.hpp"
#include "simple_csp_value_order_strategy.hpp"
#include "most_constrained_csp_variable_order_strategy.hpp"
#include "most_preferred_csp_value_order_strategy.hpp"


class argosmt_solver_interface : public solver_interface {
private:
  solver * _solver;
  statistics_solver_observer * _stat;
  wall_clock _cl;

public:
  argosmt_solver_interface()
    :_solver(nullptr),
     _stat(nullptr)
  {}
  
  check_sat_response start_solver(const std::vector<clause *> & clauses, formula_transformer * ft);

  virtual void init_interface()
  {
    _smt_lib_api->set_option(keyword(":print-success"), false);
  }

  virtual void reset_interface()
  {
    _stat = nullptr;
    delete _solver;
    _solver = nullptr;
  }
  
  virtual check_sat_response check_sat();  

  virtual expression_vector get_value(const expression_vector & exps);
  
  virtual proof get_proof()
  {
    return _solver->get_proof();
  }
  
  virtual expression_vector get_unsat_core() 
  {
    throw command_unsupported_exception();
  }
    
  virtual expression_vector get_assignment()
  {
    throw command_unsupported_exception();
  }

  void print_reports(std::ostream & ostr)
  {
    if(_solver != nullptr)
      _solver->print_reports(ostr);
  }

  virtual ~argosmt_solver_interface()
  {
    reset_interface();
  }
};

#endif // _ARGOSMT_SOLVER_INTERFACE_H
