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

#ifndef _PROPAGATION_HANDLER_H
#define _PROPAGATION_HANDLER_H

#include "csp_theory_solver.hpp"

class propagation_handler {
protected:
  csp_theory_solver * _theory_solver;
public:
  propagation_handler(csp_theory_solver * th)
    :_theory_solver(th)
  {}

  csp_theory_solver * get_theory_solver()
  {
    return _theory_solver;
  }
  
  virtual void new_level() = 0;
  virtual void backjump(unsigned level) = 0;
  virtual void assert_literal(const expression & l) = 0;
  virtual void check_and_propagate(unsigned layer) = 0;
  virtual void explain_literal(const expression & l) = 0;

  virtual ~propagation_handler() {}
};


#endif // _PROPAGATION_HANDLER_H
