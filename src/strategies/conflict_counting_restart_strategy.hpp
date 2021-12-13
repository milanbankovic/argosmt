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

#ifndef _CONFLICT_COUNTING_RESTART_STRATEGY_H
#define _CONFLICT_COUNTING_RESTART_STRATEGY_H

#include "restart_strategy.hpp"
#include "solver_observer.hpp"

class solver;

class conflict_counting_restart_strategy : public restart_strategy,
					   public solver_observer {
private:
  unsigned _count_conflicts;
  unsigned _should_restart;
  double _inc;

public:
  conflict_counting_restart_strategy(solver & sl, 
				     unsigned first = 100, 
				     double inc = 1.5);

  virtual bool should_restart()
  {
    return _count_conflicts >= _should_restart;
  }

  virtual void conflict_applied(const explanation & conflicting, const theory_solver * /* unnamed */)
  {
    _count_conflicts++;
  }

  virtual void restart_applied()
  {
    _count_conflicts = 0;
    _should_restart = (unsigned)(_should_restart * _inc);
  }

};


#endif // _CONFLICT_COUNTING_RESTART_STRATEGY_H
