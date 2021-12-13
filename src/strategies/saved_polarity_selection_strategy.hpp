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

#ifndef _SAVED_POLARITY_SELECTION_STRATEGY_H
#define _SAVED_POLARITY_SELECTION_STRATEGY_H

#include "polarity_selection_strategy.hpp"
#include "solver.hpp"
#include "solver_observer.hpp"

class saved_polarity_selection_strategy : public polarity_selection_strategy,
					  public solver_observer {
private:

  class saved_polarity_data : public expression_data {
  private:
    int _polarity_hint;
  public:
    saved_polarity_data(int hint = 0)
      :_polarity_hint(hint)
    {}

    int get_polarity_hint() const
    {
      return _polarity_hint;
    }

    void set_polarity_hint(int value) 
    {
      _polarity_hint = value;
    }

    void add_to_polarity_hint(int value)
    {
      _polarity_hint += value;
    }
  };

  
  data_extractor<saved_polarity_data> _saved_polarity_data;

  const expression & positive_literal(const expression & l)
  {
    return _solver.get_literal_data(l)->get_positive();
  }

  void adjust_polarity_hint(const expression & l)
  {
    saved_polarity_data * l_data = get_saved_polarity_data(positive_literal(l));
    bool is_pos = _solver.get_literal_data(l)->is_positive();
    int hint = l_data->get_polarity_hint();
    if(hint >= 0 && !is_pos)
      l_data->add_to_polarity_hint(-1);
    else if(hint < 0 && is_pos)
      l_data->add_to_polarity_hint(1);
  }
   
public:
  saved_polarity_selection_strategy(solver & sl)
    :solver_observer(sl),
     _saved_polarity_data(sl.extractor_counter())
  {
    sl.add_observer(this);
  }

  saved_polarity_data * get_saved_polarity_data(const expression & e)
  {
    return _saved_polarity_data.get_data(e);
  }

  void set_saved_polarity_data(const expression & e, saved_polarity_data * data)
  {
    _saved_polarity_data.set_data(e, data);
  }

  bool has_saved_polarity_data(const expression & e) const
  {
    return _saved_polarity_data.has_data(e);
  }

  virtual expression get_literal(const expression & l)
  {
    const expression & lp = positive_literal(l);
    saved_polarity_data * lp_data = get_saved_polarity_data(lp);

    if(lp_data->get_polarity_hint() >= 0)
      return lp;
    else
      return _solver.get_literal_data(lp)->get_opposite();
  }

  virtual void preffered_polarity_hint(const expression & l, int value, bool add_to) override
  {
    if(add_to)      
      get_saved_polarity_data(positive_literal(l))->add_to_polarity_hint(value);
    else
      get_saved_polarity_data(positive_literal(l))->set_polarity_hint(value);
  }
  

  virtual void literal_introduced(const expression & l, const expression & l_opp)
  {
    literal_data * l_data = _solver.get_literal_data(l);
    literal_data * l_opp_data = _solver.get_literal_data(l_opp);
    
    if(l_data->get_occurrence_counter() > l_opp_data->get_occurrence_counter())
      set_saved_polarity_data(l, new saved_polarity_data(0));
    else 
      set_saved_polarity_data(l, new saved_polarity_data(-1));
  }

  virtual void decide_applied(const expression & l)
  {
    adjust_polarity_hint(l);
  }
  
  virtual void propagate_applied(const expression & l, 
				 const theory_solver * /* source_theory */)
  {
    adjust_polarity_hint(l);
  }
};

#endif // _SAVED_POLARITY_SELECTION_STRATEGY_H
