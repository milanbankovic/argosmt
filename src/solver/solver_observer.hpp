/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2013 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _SOLVER_OBSERVER_H
#define _SOLVER_OBSERVER_H

#include "clause.hpp"
#include "theory_solver.hpp"

class explanation;

class solver_observer {
protected:
  solver & _solver;

  solver_observer(solver & sl)
    :_solver(sl)
  {}

public:

  virtual void literal_introduced(const expression & l,
				  const expression & l_opp)
  {}

  virtual void decide_applied(const expression & l) 
  {}

  virtual void propagate_applied(const expression & l, 
				 const theory_solver * source_theory)
  {}

  virtual void conflict_applied(const explanation & conflicting,
				const theory_solver * conflict_theory)
  {}

  virtual void explain_applied(const expression & l, 
			       const explanation & expl)
  {}

  virtual void learn_applied(const clause * cl) 
  {}

  virtual void backjump_applied(unsigned level, 
				const clause * cl,
				const expression & literal) 
  {}

  virtual void forget_applied(const std::vector<clause *> & learnt_clauses, 
			      unsigned number_of_clauses) 
  {}

  virtual void restart_applied() 
  {}

  virtual ~solver_observer() {}
};


#endif // _SOLVER_OBSERVER_H
