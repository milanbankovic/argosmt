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
#include "bool_theory.hpp"
#include "euf_theory.hpp"
#include "arithmetic_theory.hpp"
#include "csp_theory.hpp"
#include "wall_clock.hpp"
#include "cmd_line_parser.hpp"

#include "simple_csp_variable_order_strategy.hpp"
#include "simple_csp_value_order_strategy.hpp"
#include "most_constrained_csp_variable_order_strategy.hpp"
#include "most_preferred_csp_value_order_strategy.hpp"


class argosmt_solver_interface : public solver_interface {
private:
  
  expression get_unique_constant(const std::string & prefix, const sort & s)
  {
    static int count = 0;
    function_symbol uniq_name(prefix + std::to_string(++count));
    _smt_lib_api->declare_function(uniq_name, s); 
    
    return _smt_lib_api->get_expression_factory()->create_expression(uniq_name);
  }
  
  bool is_chainable(const expression & e)
  {
    const function_symbol & fs = e->get_symbol();
    return 
      fs == function_symbol::EQ ||
      fs == function_symbol::LT ||
      fs == function_symbol::LE ||
      fs == function_symbol::GT ||
      fs == function_symbol::GE;
  }

  bool is_pairwise(const expression & e)
  {
    const function_symbol & fs = e->get_symbol();
    return 
      fs == function_symbol::DISTINCT;
  }

  expression simplification(const expression & expr);

  expression negate_name(const expression & name)
  {
    if(name->is_not())
      return name->get_operands()[0];
    else
      return _smt_lib_api->get_expression_factory()->
	create_expression(function_symbol::NOT, name);
  }

  void cnf_transformation_rec(const expression & expr, 
			      std::vector<clause *> & clauses, 
			      expression & name);
  
  void cnf_transformation(const expression & expr, 
			  std::vector<clause *> & clauses);


  std::vector<solver *> _solvers; /* For parallel portfolio */
  std::vector<statistics_solver_observer *> _statistics_observers;
  solver * _solver;  /* Successful solver pointer */
  wall_clock _cl;
  unsigned _instance_index; /* Succesful solver index */

public:
  argosmt_solver_interface(unsigned numofsolvers = 1)
    :_solvers(numofsolvers, (solver*)0),
     _statistics_observers(numofsolvers, (statistics_solver_observer*)0),
     _solver(0),
     _instance_index(0)
  {}

  void set_solver_instance_index(unsigned k)
  {
    _instance_index = k;
  }
  
  check_sat_response start_solver(const std::vector<clause *> & clauses, unsigned k = 0);

  virtual void init_interface()
  {
    _smt_lib_api->set_option(keyword(":print-success"), false);
  }

  virtual check_sat_response check_sat();  

  virtual expression_vector get_value(const expression_vector & exps);
  
  virtual proof * get_proof()
  {
    throw command_unsupported_exception();
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
    _solver->print_reports(ostr);
  }
};

#endif // _ARGOSMT_SOLVER_INTERFACE_H
