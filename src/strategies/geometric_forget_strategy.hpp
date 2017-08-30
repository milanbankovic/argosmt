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

#ifndef _GEOMETRIC_FORGET_STRATEGY_H
#define _GEOMETRIC_FORGET_STRATEGY_H

#include "forget_strategy.hpp"
#include "solver.hpp"

class geometric_forget_strategy : public forget_strategy, public solver_observer {
private:
  solver & _solver;
  unsigned _number_of_clauses;
  double _forget_inc;
public:
  geometric_forget_strategy(solver & sl, 
			    double percent_for_first_forget = 1.0/3.0,
			    double forget_inc = 1.1)
    :solver_observer(sl),
     _solver(sl),
     _number_of_clauses((unsigned)(sl.get_initial_clauses().size() * percent_for_first_forget)),
     _forget_inc(forget_inc)
  {
    _solver.add_observer(this);
  }

  geometric_forget_strategy(solver & sl, unsigned number_of_clauses, double forget_inc = 1.1)
    :solver_observer(sl),
     _solver(sl),
     _number_of_clauses(number_of_clauses),
     _forget_inc(forget_inc)
  {
    _solver.add_observer(this);
  }
  
  virtual bool should_forget()
  {
    return _solver.get_learnt_clauses().size() >= _number_of_clauses; 
  }

  virtual void restart_applied()
  {
    _number_of_clauses = (unsigned)(_number_of_clauses * _forget_inc);
  }

};

#endif // _GEOMETRIC_FORGET_STRATEGY_H
