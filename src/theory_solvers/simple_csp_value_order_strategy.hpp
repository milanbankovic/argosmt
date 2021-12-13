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

#ifndef _SIMPLE_CSP_VALUE_ORDER_STRATEGY_H
#define _SIMPLE_CSP_VALUE_ORDER_STRATEGY_H

#include "csp_value_order_strategy.hpp"
#include "domain_handler.hpp"

class simple_csp_value_order_strategy : public csp_value_order_strategy {
public:
  simple_csp_value_order_strategy(csp_theory_solver * th)
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
    domain_handler * d_handler = _theory_solver->get_theory_solver_data(var)->get_variable_domain_handler();
    value = d_handler->get_min();
    bt = B_BOTH;
  }

};

#endif // _SIMPLE_CSP_VALUE_ORDER_STRATEGY_H
