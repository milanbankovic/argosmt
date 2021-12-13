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

#ifndef _SIMPLE_CSP_VARIABLE_ORDER_STRATEGY_H
#define _SIMPLE_CSP_VARIABLE_ORDER_STRATEGY_H

#include "csp_variable_order_strategy.hpp"
#include "domain_handler.hpp"

class simple_csp_variable_order_strategy : public csp_variable_order_strategy {
public:
  simple_csp_variable_order_strategy(csp_theory_solver * th)
    :csp_variable_order_strategy(th)
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
    for(unsigned i = 0; i < vars.size(); i++)
      {
	if(_theory_solver->get_theory_solver_data(vars[i])->get_variable_domain_handler()->current_domain().size() > 1)
	{
	  var = vars[i];
	  return true;
	}
      }
    return false;
  }
};

#endif // _SIMPLE_CSP_VARIABLE_ORDER_STRATEGY_H
