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

#ifndef _CLAUSE_ACTIVITY_FORGET_SELECTION_STRATEGY_H
#define _CLAUSE_ACTIVITY_FORGET_SELECTION_STRATEGY_H

#include "percent_to_forget_selection_strategy.hpp"

class clause_activity_forget_selection_strategy : 
  public percent_to_forget_selection_strategy,  public solver_observer {
private:

  double _bump_amount;
  double _decay_factor;
  double _activity_limit;

  void decay_clause_activities()
  {
    _bump_amount *= _decay_factor;
  }
  
  void bump_clause(clause * cl)
  {
    if(cl->is_learnt())
      {
	double old_activity = cl->get_activity();
	double new_activity = old_activity + _bump_amount;
	cl->set_activity(new_activity);
	
	if(new_activity >= _activity_limit)
	  rescale_clause_activities(); 
      }
  }

  void rescale_clause_activities()
  {  
    const std::vector<clause *> & learnt_clauses = _solver.get_learnt_clauses();
    for(unsigned i = 0; i < learnt_clauses.size(); i++)
      {
	double old_activity = learnt_clauses[i]->get_activity();
	double new_activity = old_activity / _activity_limit;
	learnt_clauses[i]->set_activity(new_activity);
      }
    _bump_amount /= _activity_limit;
  }
  

public:
  clause_activity_forget_selection_strategy(solver & sl, 
					    double percent_to_forget = 0.5,
					    double bump_amount = 1.0,
					    double decay_factor = 1.0/0.999,
					    double activity_limit = 1e20)
    :percent_to_forget_selection_strategy(percent_to_forget),
     solver_observer(sl),
     _bump_amount(bump_amount),
     _decay_factor(decay_factor),
     _activity_limit(activity_limit)
  {
    sl.add_observer(this);
  }
  
  virtual void conflict_applied(const explanation & conflicting, 				
				const theory_solver * conflict_ts)
  {
    if(conflicting.is_clause_explanation())
      bump_clause(conflicting.get_clause());
  }


  virtual void backjump_applied(unsigned level, 
				const clause * cl,
				const expression & literal) 
  {
    decay_clause_activities();
  }

  virtual unsigned 
  number_of_clauses_to_forget(std::vector<clause *> & clauses,
			      unsigned start_index)
  {
    std::sort(clauses.begin() + start_index, clauses.end(),
	      [](const clause* c1, const clause* c2) 
	      {
		return c1->get_activity() > c2->get_activity() ||
		  (!(c1->get_activity() < c2->get_activity()) && 
		   c1->get_index() > c2->get_index());
	      });
    return percent_to_forget_selection_strategy::number_of_clauses_to_forget(clauses, start_index);
  }

};

#endif // _CLAUSE_ACTIVITY_FORGET_SELECTION_STRATEGY_H
