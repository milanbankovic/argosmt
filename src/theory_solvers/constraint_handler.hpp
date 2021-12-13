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

#ifndef _CONSTRAINT_HANDLER_H
#define _CONSTRAINT_HANDLER_H

#include "propagation_handler.hpp"

class constraint_handler : public propagation_handler {
protected:
  const expression _constraint;
  const expression _not_constraint;
  std::vector<bool> _is_in_queue;
  bool _is_in_inactive_queue;
  bool _is_active;
  bool _just_activated;
  expression_vector _weaker_from_pos;
  expression_vector _weaker_from_neg;
  bool _reprocessing_flag;

  void propagate_weaker_from_pos_constraints()
  {
    for(unsigned i = 0; i < _weaker_from_pos.size(); i++)
      {
	const expression & l = _weaker_from_pos[i];
	const extended_boolean l_value = _theory_solver->get_solver().get_trail().get_value(l);
	if(l_value == EB_UNDEFINED)
	  {
	    //std::cout << "Propagating: " << l << " from: " << _constraint << std::endl;
	    _theory_solver->get_theory_solver_data(l)->set_explanation_handler(this);
	    _theory_solver->get_solver().apply_propagate(l, _theory_solver);
	  }
	else if(l_value == EB_FALSE)
	  {	    
	    explanation conflicting;
	    const expression & l_opp = _theory_solver->get_solver().get_literal_data(l)->get_opposite();
	    conflicting.push_back(_constraint);
	    conflicting.push_back(l_opp);
	    _theory_solver->get_solver().apply_conflict(conflicting, _theory_solver);
	  }         
      }
  }

  void propagate_weaker_from_neg_constraints()
  {
    for(unsigned i = 0; i < _weaker_from_neg.size(); i++)
      {
	const expression & l = _weaker_from_neg[i];
	const extended_boolean l_value = _theory_solver->get_solver().get_trail().get_value(l);
	if(l_value == EB_UNDEFINED)
	  {
	    //std::cout << "Propagating: " << l << " from: " << _not_constraint << std::endl;
	    _theory_solver->get_theory_solver_data(l)->set_explanation_handler(this);
	    _theory_solver->get_solver().apply_propagate(l, _theory_solver);
	  }
	else if(l_value == EB_FALSE)
	  {	    
	    explanation conflicting;
	    const expression & l_opp = _theory_solver->get_solver().get_literal_data(l)->get_opposite();
	    conflicting.push_back(_not_constraint);
	    conflicting.push_back(l_opp);
	    _theory_solver->get_solver().apply_conflict(conflicting, _theory_solver);
	  }         
      }
  }


public:
  constraint_handler(csp_theory_solver * th, const expression & c, const expression & nc, bool is_active = false)
    :propagation_handler(th),
     _constraint(c),
     _not_constraint(nc),
     _is_in_queue(th->get_num_of_layers(), false),
     _is_in_inactive_queue(false),
     _is_active(is_active),
     _just_activated(is_active),
     _reprocessing_flag(false)
  {}

  const expression & get_constraint() const
  {
    return _constraint;
  }

  const expression & get_not_constraint() const
  {
    return _not_constraint;
  }

  bool is_in_queue(unsigned k) const
  {
    return _is_in_queue[k];
  }
  
  void set_in_queue(unsigned k, bool value)
  {
    _is_in_queue[k] = value;
  }

  void set_all_in_queue(bool value)
  {
    for(unsigned i = 0; i < _is_in_queue.size(); i++)
      _is_in_queue[i] = value;
  }

  bool is_in_inactive_queue() const
  {
    return _is_in_inactive_queue;
  }

  void set_in_inactive_queue(bool value)
  {
    _is_in_inactive_queue = value;
  }

  void set_active(bool value)
  {
    _is_active = value;
    _just_activated = value;
  }

  bool is_active() const
  {
    return _is_active;
  }

  void add_to_weaker_from_pos_constraints(const expression & c)
  {
    _weaker_from_pos.push_back(c);
  }

  void add_to_weaker_from_neg_constraints(const expression & c)
  {
    _weaker_from_neg.push_back(c);
  }

  bool requires_reprocessing() const
  {
    return _reprocessing_flag;
  }

  void reset_reprocessing_flag()
  {
    _reprocessing_flag = false;
  }

  virtual void new_level() = 0;
  virtual void backjump(unsigned level) = 0;
  virtual void assert_literal(const expression & l) = 0;
  virtual void check_and_propagate(unsigned layer) = 0;
  virtual void explain_literal(const expression & l) = 0;

  virtual int get_variable_value_hint(const expression & var) = 0;
  virtual bool is_weaker_from_pos(const expression & c) = 0;
  virtual bool is_weaker_from_neg(const expression & c) = 0;
  virtual void check_implied() = 0;

  virtual ~constraint_handler() {}
};

#endif // _CONSTRAINT_HANDLER_H
