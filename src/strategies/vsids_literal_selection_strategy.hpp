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

#ifndef _VSIDS_LITERAL_SELECTION_STRATEGY_H
#define _VSIDS_LITERAL_SELECTION_STRATEGY_H

#include "literal_selection_strategy.hpp"
#include "solver_observer.hpp"
#include "heap.hpp"
#include "backjump_stack.hpp"
#include "solver.hpp"
#include <limits>
#include <deque>

class vsids_literal_selection_strategy : public literal_selection_strategy, 
					 public solver_observer {
private:

  class vsids_data : public expression_data {
  private:
    double _vsids_activity;
    int _vsids_priority;
  public:
    vsids_data()
      :_vsids_activity(0),
       _vsids_priority(0)
    {}

    double get_vsids_activity() const
    {
      return _vsids_activity;
    }
    
    void set_vsids_activity(double vsids_activity)
    {
      _vsids_activity = vsids_activity;
    }

    int get_vsids_priority() const
    {
      return _vsids_priority;
    }

    void set_vsids_priority(int priority)
    {
      _vsids_priority = priority;
    }

  };

  class activity_comparator {
  private:
    solver * _solver;
    vsids_literal_selection_strategy * _strategy;
  public:
    activity_comparator()
      :_solver(0),
       _strategy(0)
    {}
    
    activity_comparator(solver & sl, vsids_literal_selection_strategy * st)
      :_solver(&sl),
       _strategy(st)
    {}

    bool operator () (const expression & e1, const expression & e2);

  };

  struct on_pop {
    heap<activity_comparator> & _heap;
    on_pop(heap<activity_comparator> & q)
      :_heap(q)
    {}
    void operator () (const expression & l) const
    {
      _heap.add_to_heap(l);
    }
  };

  data_extractor<vsids_data> _vsids_data;
  solver & _solver;
  heap<activity_comparator> _heap;
  backjump_stack<expression, no_op<expression>, on_pop> _trail;
  double _bump_amount;
  double _decay_factor;
  double _activity_limit;
  std::deque<expression> _next_literals;

  void bump_literal(const expression & lit, double value);
  void bump_literals(const explanation & lits);

  template <typename T>
  void rescale_activities(const T & lits, double factor);

  const expression & positive_literal(const expression & l)
  {
    return _solver.get_literal_data(l)->get_positive();
  }

  void adjust_heap_position(const expression & lit, bool up)
  {
    unsigned heap_index = 
      _heap.get_heap_data(lit)->get_heap_index();
    
    if(heap_index == (unsigned)(-1))
      return;
    
    if(up)
      _heap.push_up(heap_index);
    else
      _heap.push_down(heap_index);    
  }

public:
  vsids_literal_selection_strategy(solver & sl,
				   double bump_amount = 1.0, 
				   double decay_factor = 1.0/0.95, 
				   double activity_limit = 1e20);


  vsids_data * get_vsids_data(const expression & e)
  {
    return _vsids_data.get_data(e);
  }

  void set_vsids_data(const expression & e, vsids_data * data)
  {
    _vsids_data.set_data(e, data);
  }

  bool has_vsids_data(const expression & e) const
  {
    return _vsids_data.has_data(e);
  }

  virtual bool select_decision_literal(expression & decision_literal) override;

  virtual void branching_priority_hint(const expression & l, int static_priority, int dynamic_priority, bool force_next) override
  {
    if(force_next)
      {
	const expression & lp = positive_literal(l);
	_next_literals.push_back(lp);
      }
    
    if(static_priority != 0)
      {
	const expression & lp = positive_literal(l);
	vsids_data * l_data = get_vsids_data(lp);
	l_data->set_vsids_priority(l_data->get_vsids_priority() + static_priority);
	adjust_heap_position(lp, static_priority > 0);
      }

    if(dynamic_priority != 0)
      {
	const expression & lp = positive_literal(l);	    
	bump_literal(lp, dynamic_priority * _bump_amount);       
      }
  }

  virtual void literal_introduced(const expression & l,
				  const expression & l_opp)
  {
    vsids_data * l_data = new vsids_data();
    set_vsids_data(l, l_data);
    _heap.introduce_element(l);
    _heap.add_to_heap(l);
  }
    
  virtual void decide_applied(const expression & l)
  {
    if(_trail.empty() || _trail.top() != positive_literal(l))
      _trail.new_level();
  }


  virtual void conflict_applied(const explanation & conflicting, const theory_solver * /* unnamed */)
  {
    _bump_amount *= _decay_factor;
    bump_literals(conflicting);
  }

  virtual void explain_applied(const expression & l, const explanation & expl)
  {
    bump_literals(expl);
    bump_literal(positive_literal(l), _bump_amount);
  }
  
  virtual void backjump_applied(unsigned level, 
			const clause * cl,
			const expression & literal)
  {
    _trail.backjump(level, on_pop(_heap));
  }
  
  virtual void restart_applied()
  {
    _trail.backjump(0, on_pop(_heap));
  }
};


#endif // _VSIDS_LITERAL_SELECTION_STRATEGY_H
