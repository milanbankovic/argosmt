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

#include "bool_theory.hpp"

void bool_theory::add_locked_clause(clause * cl)
{
  if(cl->is_learnt() && !cl->is_locked())
    {
      _locked_clauses.push(cl);
    }
}

void bool_theory::set_reason_clause(const expression & l, clause * cl, unsigned index)
{
  bool_theory_data * l_data = get_theory_data(l);
  l_data->set_reason_clause(cl);
  l_data->set_index(index);
  add_locked_clause(cl);
}

unsigned bool_theory::find_initial_watches(clause * cl, unsigned & w0, unsigned & w1)
{
  unsigned w = 0;
  w0 = 0;
  w1 = 1;

  unsigned w0_index = _solver.get_trail().get_trail_index_if_falsified((*cl)[0]);
  unsigned w1_index = _solver.get_trail().get_trail_index_if_falsified((*cl)[1]);

  if(w1_index > w0_index)
    {
      std::swap(w1_index, w0_index);
      std::swap(w1, w0);
    }

  if(w1_index == UNDEFINED_TRAIL_INDEX)
    w = 2;
  else if(w0_index == UNDEFINED_TRAIL_INDEX)
    w = 1;


  for(unsigned i = 2; w < 2 && i < cl->size(); i++)
    {
      unsigned i_index = _solver.get_trail().get_trail_index_if_falsified((*cl)[i]);

      if(i_index > w0_index)
	{
	  w1 = w0;
	  w0 = i;
	  w1_index = w0_index;
	  w0_index = i_index;
	}
      else if(i_index > w1_index)
	{
	  w1_index = i_index;
	  w1 = i;
	}

      if(i_index == UNDEFINED_TRAIL_INDEX)
	w++;
    }

  return w;
}


void bool_theory::set_initial_watches(clause * cl)
{
  // special case -- empty clause!!
  if(cl->size() == 0)
    {
      _solver.apply_conflict(explanation(&_solver, cl), this);
      if(_solver.get_trail().current_level() != 0)
	_dead_clauses.push_back(cl);
      count_conflicts(cl);
    }
  // special case -- unit clause!!
  else if(cl->size() == 1)
    {
      extended_boolean value = _solver.get_trail().get_value((*cl)[0]);
      if(value == EB_UNDEFINED)
	{
	  _solver.apply_propagate((*cl)[0], this);
	  set_reason_clause((*cl)[0], cl, 0);
	  count_propagations(cl);
	}
      else if(value == EB_FALSE)
	{
	  _solver.apply_conflict(explanation(&_solver, cl), this);
	  count_conflicts(cl);
	}

      if(_solver.get_trail().current_level() != 0)
	_dead_clauses.push_back(cl);
    }
  else
    {
      unsigned w0, w1;
      unsigned w = find_initial_watches(cl, w0, w1);

      if(w == 0)
	{
	  _solver.apply_conflict(explanation(&_solver, cl), this);
	  count_conflicts(cl);
	}
      else if(w == 1)
	{
	  expression wl = (*cl)[w0];
	  extended_boolean value = _solver.get_trail().get_value(wl);
	  assert(value != EB_FALSE);

	  if(value == EB_UNDEFINED)
	    {
	      _solver.apply_propagate(wl, this);
	      set_reason_clause(wl, cl, w0);
	      count_propagations(cl);
	    }
	  else if(value == EB_FALSE) // NEVER HAPPENS!!
	    {
	      _solver.apply_conflict(explanation(&_solver, cl), this);
	      count_conflicts(cl);
	    }
	}

      unsigned w1_level = _solver.get_trail().get_trail_level((*cl)[w1]);
      unsigned curr_level =  _solver.get_trail().current_level();

      if(w < 2 && w1_level < curr_level)
	{
	  _dead_clauses.push_back(cl);
	}
      else
	{
	  cl->set_watch_pos(0, w0);
	  watch_list & w0_list = get_theory_data((*cl)[w0])->get_watch_list();
	  w0_list.push_back(cl);

	  cl->set_watch_pos(1, w1);
	  watch_list & w1_list = get_theory_data((*cl)[w1])->get_watch_list();
	  w1_list.push_back(cl);
	}
    }
}

void bool_theory::check_clauses(const std::vector<clause *> & clauses)
{
  for(unsigned i = 0; i < clauses.size(); i++)
    {
      //std::cout << "Clause: " << *clauses[i] << std::endl;
      set_initial_watches(clauses[i]);
    }
}

void bool_theory::process_watch_list(const expression & lp)
{
  watch_list & watch_list = get_theory_data(lp)->get_watch_list();
  unsigned k = 0;

  for(unsigned j = 0; j < watch_list.size(); j++)
    {
      clause * cl = watch_list[j];
      unsigned w = cl->get_watch_literal(0) == lp ? 0 : 1;
      expression owl = cl->get_watch_literal(1 - w);
      unsigned owl_pos = cl->get_watch_pos(1 - w);
      extended_boolean owl_value = _solver.get_trail().get_value(owl);

      if(owl_value == EB_TRUE)
	{
	  watch_list[k++] = cl;
	  continue;
	}

      if(find_alternative_watch(cl, w))
	continue;

      if(owl_value == EB_UNDEFINED)
	{
	  _solver.apply_propagate(owl, this);
	  set_reason_clause(owl, cl, owl_pos);
	  watch_list[k++] = cl;
	  count_propagations(cl);
	}
      else // owl_value == EB_FALSE
	{
	  _solver.apply_conflict(explanation(&_solver, cl), this);
	  count_conflicts(cl);
	  while(j < watch_list.size())
	    {
	      watch_list[k++] = watch_list[j++];
	    }
	  break;
	}
    }
  watch_list.resize(k);
}

bool bool_theory::find_alternative_watch(clause * cl, unsigned cw)
{
  unsigned pos0 = cl->get_watch_pos(0);
  unsigned pos1 = cl->get_watch_pos(1);

  for(unsigned i = 0; i < cl->size(); i++)
    {
      if(i == pos0 || i == pos1)
	continue;

      if(!_solver.get_trail().is_false((*cl)[i]))
	{
	  cl->set_watch_pos(cw, i);
	  get_theory_data((*cl)[i])->get_watch_list().push_back(cl);
	  return true;
	}
    }
  return false;
}

bool bool_theory::is_clause_true(const trail & tr, clause * cl)
{
  for(unsigned i = 0; i < cl->size(); i++)
    if(_solver.get_trail().is_true((*cl)[i]))
      return true;
  return false;
}

void bool_theory::remove_clauses(const std::vector<clause *> & learnt_clauses,
				 unsigned number_of_clauses)
{
  for(unsigned i = learnt_clauses.size() - number_of_clauses;
      i < learnt_clauses.size(); i++)
    {
      clause * cl = learnt_clauses[i];

      std::vector<clause *>::iterator it =
	std::find(_pending_clauses.begin(), _pending_clauses.end(), cl);

      if(it != _pending_clauses.end())
	{
	  *it = _pending_clauses.back();
	  _pending_clauses.pop_back();
	  continue;
	}

      it = std::find(_dead_clauses.begin(), _dead_clauses.end(), cl);

      if(it != _dead_clauses.end())
	{
	  *it = _dead_clauses.back();
	  _dead_clauses.pop_back();
	  continue;
	}

      if(cl->size() < 2)
	continue;

      for(unsigned j = 0; j < 2; j++)
	{
	  watch_list & wl = get_theory_data(cl->get_watch_literal(j))->get_watch_list();
	  it = std::find(wl.begin(), wl.end(), cl);
	  if(it != wl.end())
	    {
	      *it = wl.back();
	      wl.pop_back();
	    }
	  else
	    {
	      std::cout << "ERROR FATAL" << std::endl;
	      exit(1);
	    }
	}
    }
}

void bool_theory::new_level()
{
  _new_level_time_spent.start();
  _locked_clauses.new_level();
  _new_level_time_spent.acumulate();
}


void bool_theory::backjump(unsigned level)
{
  _backjump_time_spent.start();
  _locked_clauses.backjump(level);
  _current_trail_pos = _solver.get_trail().size();
  _just_backjumped = true;
  _backjump_time_spent.acumulate();
}

void bool_theory::check_and_propagate(unsigned layer)
{
  _check_and_prop_time_spent.start();

  if(!_initial_clauses_checked && !_solver.is_conflict())
    {
      check_clauses(_solver.get_initial_clauses());
      _initial_clauses_checked = true;
    }
  if(!_solver.is_conflict())
    {
      check_clauses(_pending_clauses);
      _pending_clauses.clear();
    }
  if(_just_backjumped && !_solver.is_conflict())
    {
      if(!_dead_clauses.empty())
	{
	  std::vector<clause *> temp;
	  temp.swap(_dead_clauses);
	  check_clauses(temp);
	}
      _just_backjumped = false;
    }
  CHECK_CANCELED;
  //  std::cout << "BOOL THEORY: " << layer << std::endl;
  while(!_solver.is_conflict() && _current_trail_pos < _solver.get_trail().size())
    {
      expression l = _solver.get_trail()[_current_trail_pos++];
      expression lp = _solver.get_literal_data(l)->get_opposite();
      process_watch_list(lp);
      CHECK_CANCELED;
    }
  _check_and_prop_time_spent.acumulate();
}

void bool_theory::explain_literal(const expression & l)
{
  _explain_time_spent.start();
  bool_theory_data * l_data = get_theory_data(l);
  clause * reason_clause = l_data->get_reason_clause();
  unsigned index = l_data->get_index();
  _solver.apply_explain(l, explanation(&_solver, reason_clause, index));
  _explain_time_spent.acumulate();
}

expression bool_theory::canonize_expression(const expression & e)
{
  expression ep;
  bool negative;

  if(e->is_not())
    {
      ep = e->get_operands()[0];
      negative = true;
    }
  else
    {
      ep = e;
      negative = false;
    }

  if(ep->is_positive_sat_literal())
    return negative ?
      _solver.get_factory()->create_expression(ep->get_sat_variable(),
					       SLP_NEGATIVE) : ep;


  if(ep->is_negative_sat_literal())
    return negative ?
      _solver.get_factory()->create_expression(ep->get_sat_variable(),
					       SLP_POSITIVE) : ep;

  if(ep->is_true())
    return negative ?
      _solver.get_factory()->FALSE_EXPRESSION() : ep;

  if(ep->is_false())
    return negative ?
      _solver.get_factory()->TRUE_EXPRESSION() : ep;

  return e;
}

void bool_theory::get_literal_pair(const expression & l, expression & l_pos, expression & l_neg)
{
  if(l->is_positive_sat_literal())
    {
      l_pos = l;
      l_neg = _solver.get_factory()->create_expression(l->get_sat_variable(),
						       SLP_NEGATIVE);
    }
  else if(l->is_negative_sat_literal())
    {
      l_neg = l;
      l_pos = _solver.get_factory()->create_expression(l->get_sat_variable(),
						       SLP_POSITIVE);
    }
  else if(l->is_true())
    {
      l_pos = l;
      l_neg = _solver.get_factory()->FALSE_EXPRESSION();
    }
  else if(l->is_false())
    {
      l_pos = _solver.get_factory()->TRUE_EXPRESSION();
      l_neg = l;
    }
  else if(l->is_not())
    {
      l_pos = l->get_operands()[0];
      l_neg = l;
    }
  else
    {
      l_pos = l;
      l_neg = _solver.get_factory()->create_expression(function_symbol::NOT, l);
    }
}

bool bool_theory::verify_assignment(const trail & tr)
{
  const std::vector<clause *> & initial_clauses =
    _solver.get_initial_clauses();
  for(unsigned i = 0; i < initial_clauses.size(); i++)
    if(!is_clause_true(tr, initial_clauses[i]))
      return false;

  return true;
}

void bool_theory::get_model(const expression_vector & exps)
{
  for(unsigned i = 0; i < exps.size(); i++)
    {
      if(exps[i]->has_expression_data() && _solver.has_literal_data(exps[i]))
	{
	  switch(_solver.get_trail().get_value(exps[i]))
	    {
	    case EB_UNDEFINED:
	      exps[i]->set_assigned(_solver.get_factory()->UNDEFINED_EXPRESSION());
	      break;
	    case EB_TRUE:
	      exps[i]->set_assigned(_solver.get_factory()->TRUE_EXPRESSION());
	      break;
	    case EB_FALSE:
	      exps[i]->set_assigned(_solver.get_factory()->FALSE_EXPRESSION());
	      break;
	    }
	}
    }
}
