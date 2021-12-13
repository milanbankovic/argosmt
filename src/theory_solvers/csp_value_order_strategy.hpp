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

#ifndef _CSP_VALUE_ORDER_STRATEGY_H
#define _CSP_VALUE_ORDER_STRATEGY_H

#include "csp_theory_solver.hpp"

enum bound_type { B_LOWER, B_UPPER, B_BOTH, B_SPLIT };

inline
std::ostream & operator << (std::ostream & ostr, bound_type bt)
{
  switch(bt)
    {
    case B_LOWER:
      ostr << " >= ";
      break;
    case B_UPPER:
      ostr << " <= ";
      break;
    case B_BOTH: 
      ostr << " = ";
      break;
    case B_SPLIT:
      ostr << " != ";
      break;
    }
  return ostr;
}

class csp_value_order_strategy {
protected:
  csp_theory_solver * _theory_solver;
public:
  csp_value_order_strategy(csp_theory_solver * th)
    :_theory_solver(th)
  {}

  virtual void assert_literal(const expression & l) = 0;
  virtual void new_level() = 0;
  virtual void backjump(unsigned level) = 0;
  virtual void get_next_value(const expression & var, int & value, bound_type & bt) = 0;

  virtual ~csp_value_order_strategy() {}

};

#endif // _CSP_VALUE_ORDER_STRATEGY_H
