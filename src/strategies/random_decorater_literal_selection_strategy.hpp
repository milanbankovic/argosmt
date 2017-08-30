/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2015 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _RANDOM_DECORATER_LITERAL_SELECTION_STRATEGY_H
#define _RANDOM_DECORATER_LITERAL_SELECTION_STRATEGY_H

#include "random_literal_selection_strategy.hpp"
#include "solver_observer.hpp"

template <typename RNG = std::mt19937>
class random_decorater_literal_selection_strategy : public literal_selection_strategy, public solver_observer {
private:
  random_literal_selection_strategy<RNG> * _random_strategy;
  literal_selection_strategy * _main_strategy;
  RNG & _rng;
  const double _p_rand;
  const unsigned _r_rand;
  unsigned _decide_count;

public:
  random_decorater_literal_selection_strategy(solver & sl, 
					      literal_selection_strategy * ms, 
					      RNG & rng,
					      double p = 0.05,
					      unsigned r = 0)
    :solver_observer(sl),
     _random_strategy(new random_literal_selection_strategy<RNG>(sl, rng)),
     _main_strategy(ms),
     _rng(rng),
     _p_rand(p),
     _r_rand(r),
     _decide_count(0)
  {
    sl.add_observer(this);
  }
  
  virtual bool select_decision_literal(expression & decision_literal)
  {
    if(_decide_count < _r_rand)
      {
	_decide_count++;
	return _random_strategy->select_decision_literal(decision_literal);
      }
    


    double val = _rng() / (double)_rng.max();

    if(val < _p_rand)
      return _random_strategy->select_decision_literal(decision_literal);
    else
      return _main_strategy->select_decision_literal(decision_literal);      
  }

  virtual void branching_priority_hint(const expression & l, int static_priority, int dynamic_priority, bool force_next) override
  {
    _main_strategy->branching_priority_hint(l, static_priority, dynamic_priority, force_next);
  }

  virtual void restart_applied() 
  {
    _decide_count = 0;
  }

};

#endif // _RANDOM_DECORATER_LITERAL_SELECTION_STRATEGY_H
