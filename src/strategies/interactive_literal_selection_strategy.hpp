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

#ifndef _INTERACTIVE_LITERAL_SELECTION_STRATEGY_H
#define _INTERACTIVE_LITERAL_SELECTION_STRATEGY_H

#include "literal_selection_strategy.hpp"
#include "solver.hpp"
#include "solver_observer.hpp"


class interactive_literal_selection_strategy : public literal_selection_strategy, public solver_observer {
private:
  
  struct on_pop {
    expression_vector & _vec;
    on_pop(expression_vector & v)
      :_vec(v)
    {}
    void operator () (const expression & l) const
    {
      _vec.push_back(l);
    }
  };

  expression_vector  _undefined_atoms;
  backjump_stack<expression, no_op<expression>, on_pop> _trail;
  
  const expression & positive_literal(const expression & l)
  {
    return _solver.get_literal_data(l)->get_positive();
  }

public:
  interactive_literal_selection_strategy(solver & sl)
    :solver_observer(sl)
  {
    sl.add_observer(this);
  }
  
  virtual void literal_introduced(const expression & l,
				  const expression & l_opp)
  {
    _undefined_atoms.push_back(l);
  }

  virtual void decide_applied(const expression & l) 
  {
    if(_trail.empty() || _trail.top() != positive_literal(l))
      _trail.new_level();
  }

  virtual void backjump_applied(unsigned level, 
			       const clause * cl,
			       const expression & literal) 
  {
    _trail.backjump(level, on_pop(_undefined_atoms));
  }

  virtual void restart_applied() 
  {
    _trail.backjump(0, on_pop(_undefined_atoms));
  }

  unsigned choose_undefined_atom()
  {
    unsigned index;
    expression lit;
    do{
      if(std::cin.fail())
	{
	  std::string temp;
	  std::cin.clear();
	  std::cin >> temp;
	}

      std::cerr << "Enter literal number: ";
      std::cin >> index;
      index -= 1;
      index <<= 1;
      std::cerr << "Read: " << index << std::endl;
      lit = _solver.literals()[index];
      std::cerr << "Chosen literal: " << lit << std::endl;
    } while(std::cin.fail() || !_solver.get_trail().is_undefined(lit));
    
    expression_vector::const_iterator it = std::find(_undefined_atoms.begin(), _undefined_atoms.end(), lit);
    assert(it != _undefined_atoms.end());
    return it - _undefined_atoms.begin();
  }

  virtual bool select_decision_literal(expression & decision_literal)
  {
    while(!_undefined_atoms.empty())
      {
	unsigned var = choose_undefined_atom();	
	decision_literal = _undefined_atoms[var];
	_undefined_atoms[var] = _undefined_atoms.back();
	_undefined_atoms.pop_back();
	_trail.push(decision_literal);
	
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
};



#endif // _INTERACTIVE_LITERAL_SELECTION_STRATEGY_H
