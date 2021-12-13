/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2014,2021 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _MOST_CONSTRAINED_CSP_VARIABLE_ORDER_STRATEGY_H
#define _MOST_CONSTRAINED_CSP_VARIABLE_ORDER_STRATEGY_H


#include "csp_variable_order_strategy.hpp"
#include "domain_handler.hpp"
#include "constraint_handler.hpp"

class most_constrained_csp_variable_order_strategy : public csp_variable_order_strategy {
private:
  std::unordered_map<expression, bool> _csts_at_least_two;

  void calculate(const expression & v, unsigned & dom_size, unsigned & csts_count)
  {
    csp_theory_solver::csp_theory_solver_data * v_data =
      _theory_solver->get_theory_solver_data(v);
    domain_handler * v_handler = v_data->get_variable_domain_handler();
    dom_size = v_handler->current_domain().size();
    if(dom_size < 2)
      return;
    
    const expression_set & csts = v_data->get_variable_constraints();
    csts_count = 0;
    for(expression_set::const_iterator i = csts.begin(), i_end = csts.end(); i != i_end; ++i)
      {
	if(!_theory_solver->get_theory_solver_data(*i)->get_constraint_handler()->is_active())
	  continue;

	std::unordered_map<expression, bool>::const_iterator it = _csts_at_least_two.find(*i);
	if(it != _csts_at_least_two.end())
	  {
	    if(it->second)
	      csts_count++;
	  }
	else
	  {
	    csp_theory_solver::csp_theory_solver_data * c_data =
	      _theory_solver->get_theory_solver_data(*i);
	    const expression_set & vars = c_data->get_constraint_variables();
	    bool val = false;
	    for(expression_set::const_iterator j = vars.begin(), j_end = vars.end(); j != j_end; ++j)
	      if(*j != v && _theory_solver->get_theory_solver_data(*j)->get_variable_domain_handler()->current_domain().size() > 1)
		{
		  val = true;
		  csts_count++;
		  break;
		}
	    _csts_at_least_two.insert(std::make_pair(*i, val));	    
	  }
      }
  }

public:
  most_constrained_csp_variable_order_strategy(csp_theory_solver * th)
    :csp_variable_order_strategy(th),
     _csts_at_least_two(HASH_TABLE_SIZE)
  {}

  virtual void assert_literal(const expression & l)
  {}

  virtual void new_level()
  {}

  virtual void backjump(unsigned level)
  {}

  virtual bool get_next_variable(expression & var)
  {
    const expression_vector & vars = _theory_solver->get_variables();
    var = expression();
    unsigned dom_size = 0;
    unsigned csts_count = 0;
    _csts_at_least_two.clear();
    for(unsigned i = 0; i < vars.size(); i++)
      {
	unsigned c_dom_size = 0, c_csts_count = 0;
	calculate(vars[i], c_dom_size, c_csts_count);
	if(c_dom_size > 1 && (var.get() == 0 || 
			      c_dom_size < dom_size || 
			      (c_dom_size == dom_size && 
			       c_csts_count > csts_count)))
	  {
	    var = vars[i];
	    dom_size = c_dom_size;
	    csts_count = c_csts_count;
	  }
      }
    return var.get() != 0;
  }
};

#endif // _MOST_CONSTRAINED_CSP_VARIABLE_ORDER_STRATEGY_H
