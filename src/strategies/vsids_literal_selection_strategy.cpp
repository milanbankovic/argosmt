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

#include "vsids_literal_selection_strategy.hpp"
#include <cmath>

bool vsids_literal_selection_strategy::activity_comparator::
operator () (const expression & e1, const expression & e2)
{
  double e1act;
  double e2act;
  int e1prio;
  int e2prio;
  vsids_data * e1_vdata = _strategy->get_vsids_data(e1);
  vsids_data * e2_vdata = _strategy->get_vsids_data(e2);

  return (e1prio = e1_vdata->get_vsids_priority()) > (e2prio = e2_vdata->get_vsids_priority()) ||
    (e2prio == e1prio && ((e1act = e1_vdata->get_vsids_activity()) > (e2act = e2_vdata->get_vsids_activity()) ||
			  (!(e2act > e1act) && _solver->get_literal_data(e1)->get_literal_index() < _solver->get_literal_data(e2)->get_literal_index())));
  
  
}



vsids_literal_selection_strategy::
vsids_literal_selection_strategy(solver & sl,
			      double bump_amount,
			      double decay_factor,
			      double activity_limit)
  :solver_observer(sl),
   _vsids_data(sl.extractor_counter()),
   _solver(sl),
   _heap(sl.extractor_counter(), activity_comparator(_solver, this)),
   _bump_amount(bump_amount),
   _decay_factor(decay_factor),
   _activity_limit(activity_limit)
{
  sl.add_observer(this);
}


void vsids_literal_selection_strategy::bump_literal(const expression & lit, double value)
{
  double old_activity = get_vsids_data(lit)->get_vsids_activity();
  double new_activity = old_activity + value;
            
  get_vsids_data(lit)->set_vsids_activity(new_activity);

  if(fabs(new_activity) > _activity_limit)
    {
      rescale_activities(_trail, _activity_limit);
      rescale_activities(_heap.elements(), _activity_limit);
      _bump_amount /= _activity_limit;
    }

  if(value > 0 || value < 0)
    adjust_heap_position(lit, value > 0);
}

void vsids_literal_selection_strategy::bump_literals(const explanation & lits)
{
  unsigned l_size = lits.size();
  for(unsigned i = 0; i < l_size; i++)
    bump_literal(positive_literal(lits[i]), _bump_amount);
}

template <typename T>
void vsids_literal_selection_strategy::rescale_activities(const T & lits, double factor)
{
  for(unsigned i = 0; i < lits.size(); i++)
    {
      double old_activity = get_vsids_data(lits[i])->get_vsids_activity();
      double new_activity = old_activity / factor;
      get_vsids_data(lits[i])->set_vsids_activity(new_activity);
    }
}


bool vsids_literal_selection_strategy::
select_decision_literal(expression & decision_literal)
{
  while(!_next_literals.empty())
    {
      decision_literal = _next_literals.front();
      _next_literals.pop_front();
      if(_solver.get_trail().is_undefined(decision_literal))
	{
	  return true;
	}
    }
  
  while(_heap.get_top_element(decision_literal))
    {
      if(_solver.get_trail().is_undefined(decision_literal))
	{
	  _trail.new_level();
	  _trail.push(decision_literal);
	  return true;
	}
      else
	_trail.push(decision_literal);
    } 
  return false;
}
