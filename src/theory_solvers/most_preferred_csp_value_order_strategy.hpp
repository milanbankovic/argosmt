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

#ifndef _MOST_PREFERRED_CSP_VALUE_ORDER_STRATEGY_H
#define _MOST_PREFERRED_CSP_VALUE_ORDER_STRATEGY_H

#include "csp_value_order_strategy.hpp"
#include "domain_handler.hpp"
#include "constraint_handler.hpp"
#include "sum_constraint_handler.hpp"

class most_preferred_csp_value_order_strategy : public csp_value_order_strategy {
public:
  most_preferred_csp_value_order_strategy(csp_theory_solver * th)
    :csp_value_order_strategy(th)
  {}

  virtual void assert_literal(const expression & l) 
  {}

  virtual void new_level()
  {}

  virtual void backjump(unsigned level)
  {}

  virtual void get_next_value(const expression & var, int & value, bound_type & bt)
  {
    csp_theory_solver::csp_theory_solver_data * v_data = _theory_solver->get_theory_solver_data(var);
    const expression_set & csts = v_data->get_variable_constraints();
    domain_handler * d_handler = v_data->get_variable_domain_handler();

    if(_theory_solver->_sum_common_data._instance_count > 0)// && d_handler->current_domain().size() > 2)
      {
	value = d_handler->get_median(true); // lower median
	bt = B_UPPER;
      }
    else
      {
	value = d_handler->get_min();
	
	unsigned max_counter = 0;
	std::unordered_map<int, unsigned> counters(HASH_TABLE_SIZE);
	for(expression_set::const_iterator i = csts.begin(), i_end = csts.end(); i != i_end; ++i)
	  {
	    csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(*i);
	    constraint_handler * c_handler = c_data->get_constraint_handler();
	    if(!c_handler->is_active())
	      continue;
	    
	    int pval = c_handler->get_variable_value_hint(var);
	    unsigned & c_counter = counters[pval];
	    c_counter++;
	    if(c_counter > max_counter)
	      {
		max_counter = c_counter;
		value = pval;
	      }
	  }
	bt = B_BOTH;
      }
  }

};

#endif // _MOST_PREFERRED_CSP_VALUE_ORDER_STRATEGY_H
