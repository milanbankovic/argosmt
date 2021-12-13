/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2015,2021 Milan Bankovic (milan@matf.bg.ac.rs)

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

#include "domain_handler.hpp"

void domain_handler::assert_literal(const expression & l) 
{
  if(_theory_solver->get_solver().get_trail().get_source_theory_solver(l) != _theory_solver ||
     _theory_solver->get_theory_solver_data(l)->get_explanation_handler() != this)
    {
      _trail.push(l);
    }
}

void domain_handler::check_previous_trail_literals()
{
  for(unsigned i = 0; i < _next_to_assert && !_theory_solver->get_solver().is_conflict(); i++)
    {
      expression li = _trail[i];
      check_finite(li);
    }
}

void domain_handler::check_all_values()
{
  for(std::unordered_map<int, value_info>::const_iterator it = _all_values.begin(),
	it_end = _all_values.end(); it != it_end; ++it)
    {
      if(_domain_values.find(it->first) == _domain_values.end() && it->second._diseq.get() != 0)
	{
	  propagate_literal(it->second._diseq);
	}
    }
}

void domain_handler::init_min_max()
{
  _sorted_values.resize(_domain_values.size());
  std::copy(_domain_values.begin(), _domain_values.end(), _sorted_values.begin());
  std::sort(_sorted_values.begin(), _sorted_values.end());
  for(unsigned i = 0; i < _sorted_values.size(); i++)
    {
      _all_values[_sorted_values[i]]._index = i;
    }
  _min_index = 0;
  _max_index = _sorted_values.size() - 1;

  if(_trail.current_level() == 0)
    {
      _zero_min_index = _min_index;
      _zero_max_index = _max_index;
    }
  
  value_info & info_min = _all_values[_sorted_values[0]];
  if(info_min._ge.get() != 0)
    propagate_literal(info_min._ge);
  value_info & info_max = _all_values[_sorted_values.back()];
  if(info_max._le.get() != 0)
    propagate_literal(info_max._le);
}

void domain_handler::update_min_max()
{
  while(_min_index <= _max_index && _domain_values.find(_sorted_values[_min_index]) == _domain_values.end())
    {
      value_info & info = _all_values[_sorted_values[_min_index]];
      if(!_theory_solver->get_solver().is_conflict() && info._ge.get() != 0)
	propagate_literal(info._ge);
      if(!_theory_solver->get_solver().is_conflict() && info._gt.get() != 0)
	propagate_literal(info._gt);
      
      _min_index++;
    }
  if(_min_index <= _max_index && !_theory_solver->get_solver().is_conflict())
    {
      value_info & info = _all_values[_sorted_values[_min_index]];
      if(info._ge.get() != 0)
	propagate_literal(info._ge);
    }
  
  while(_max_index >= _min_index && _domain_values.find(_sorted_values[_max_index]) == _domain_values.end())
    {
      value_info & info = _all_values[_sorted_values[_max_index]];
      if(!_theory_solver->get_solver().is_conflict() && info._le.get() != 0)
	propagate_literal(info._le);
      if(!_theory_solver->get_solver().is_conflict() && info._lt.get() != 0)
	propagate_literal(info._lt);

     _max_index--;
    }
  if(_max_index >= _min_index && !_theory_solver->get_solver().is_conflict())
    {
      value_info & info = _all_values[_sorted_values[_max_index]];
      if(info._le.get() != 0)
	propagate_literal(info._le);
    }

  if(_trail.current_level() == 0)
    {
      _zero_min_index = _min_index;
      _zero_max_index = _max_index;
    }
}

int domain_handler::get_median(bool lower) const
{
  if(_domain_values.size() == 1)
    return _sorted_values[_min_index];

  unsigned n = (_domain_values.size() >> 1) + (int)(_domain_values.size() % 2 || !lower);
  unsigned ind = _min_index;
  while(n > 0)
    {
      if(_domain_values.find(_sorted_values[ind]) != _domain_values.end())
	n--;
      ind++;
    }
  return _sorted_values[ind - 1];
}

bool domain_handler::check_bounds(expression & l_cause, expression & u_cause, int & lower, int & upper)
{
  lower = 0x80000000;
  upper = 0x7fffffff;

  for(unsigned i = 0; i <= _next_to_assert; i++)
    {
      expression li = _trail[i];
      const function_symbol & fs = li->get_symbol();
      if(fs == function_symbol::LE)
	{
	  int ub = _theory_solver->get_theory_solver_data(li->get_operands()[1])->get_int_value();
	  if(ub < upper)
	    {
	      upper = ub;
	      u_cause = li;
	    }
	}
      else if(fs == function_symbol::LT)
	{
	  int ub = _theory_solver->get_theory_solver_data(li->get_operands()[1])->get_int_value() - 1;
	  if(ub < upper)
	    {
	      upper = ub;
	      u_cause = li;
	    }
	}
      else if(fs == function_symbol::GE)
	{
	  int lb = _theory_solver->get_theory_solver_data(li->get_operands()[1])->get_int_value();
	  if(lb > lower)
	    {
	      lower = lb;
	      l_cause = li;
	    }
	}
      else if(fs == function_symbol::GT)
	{
	  int lb = _theory_solver->get_theory_solver_data(li->get_operands()[1])->get_int_value() + 1;
	  if(lb > lower)
	    {
	      lower = lb;
	      l_cause = li;
	    }
	}
    }
  return lower != (int)0x80000000 && upper != (int)0x7fffffff;
}

void domain_handler::check_not_finite(const expression & l)
{
  if(l->is_disequality())
    {
      return;
    }
  else if(l->is_equality())
    {
      set_finite();
      //      std::cout << "FINITE: " << _var << std::endl;
      _finite_domain_cause.push_back(l);
      _domain_values.insert(_theory_solver->get_theory_solver_data(l->get_operands()[1])->get_int_value());
      check_all_values();
      init_min_max();
      check_previous_trail_literals();
    }
  else if(l->get_symbol() == function_symbol::LE ||
	  l->get_symbol() == function_symbol::LT ||
	  l->get_symbol() == function_symbol::GE ||
	  l->get_symbol() == function_symbol::GT)
    {
      
      expression l_cause, u_cause;
      int lower, upper;
      if(check_bounds(l_cause, u_cause, lower, upper))
	{
	  _finite_domain_cause.push_back(std::move(l_cause));
	  _finite_domain_cause.push_back(std::move(u_cause));
	  set_initial_bounds(lower, upper);
	}
    }
}

void domain_handler::set_initial_bounds(int lower, int upper)
{
  if(is_finite())
    {
      std::cerr << "WARNING: The domain already finite: " << _var << std::endl;
      return;      
    }
  
  set_finite();
  for(int v = lower; v <= upper; v++)
    {
      _theory_solver->add_value(v);
      add_value(v);
      _domain_values.insert(v);
    }
  //introduce_literals();
  check_all_values();
  init_min_max();
  check_singleton_or_empty();
  check_previous_trail_literals();      
}

void domain_handler::init_literal_pair(int v, bool introduce)
{
  value_info & info = _all_values[v];

  if(info._eq.get() != 0)
    return;

  expression val;
  _theory_solver->get_expr_for_int(v, val);
  expression eq;
  bool is_cached = _theory_solver->get_solver().is_equality_cached(_var, val, eq);
  if(is_cached || introduce)
    {
      if(!is_cached)
	{
	  eq = _theory_solver->get_solver().get_factory()->create_expression(function_symbol::EQ, _var, val);
	  _theory_solver->get_solver().apply_introduce_literal(eq);
	  _common_data->_count_equalities++;
	  //_theory_solver->get_solver().get_literal_selection_strategy()->branching_priority_hint(eq, -5);
	}
      info._eq = eq;
      info._diseq = _theory_solver->get_solver().get_literal_data(eq)->get_opposite();
    }
}

void domain_handler::init_upper_bound(int v)
{
  value_info & info = _all_values[v];
  if(info._le.get() != 0)
    return;
  expression val;
  _theory_solver->get_expr_for_int(v, val);

  assert(val.get() != nullptr);

  expression le = _theory_solver->get_solver().get_factory()->create_expression(function_symbol::LE, _var, val);
  if(!_theory_solver->get_solver().has_expression_data(le) || !_theory_solver->get_solver().has_literal_data(le))
    {
      _theory_solver->get_solver().apply_introduce_literal(le);
      _common_data->_count_inequalities++;
      //_theory_solver->get_solver().get_literal_selection_strategy()->branching_priority_hint(le, 5);
    }
  info._le = le;
  info._gt = _theory_solver->get_solver().get_literal_data(le)->get_opposite();
}

void domain_handler::init_lower_bound(int v)
{
  value_info & info = _all_values[v];
  if(info._ge.get() != 0)
    return;

  expression val;
  _theory_solver->get_expr_for_int(v, val);

  assert(val.get() != nullptr);
  
  expression ge = _theory_solver->get_solver().get_factory()->create_expression(function_symbol::GE, _var, val);
  if(!_theory_solver->get_solver().has_expression_data(ge) || !_theory_solver->get_solver().has_literal_data(ge))
    {
      _theory_solver->get_solver().apply_introduce_literal(ge);
      _common_data->_count_inequalities++;
      //_theory_solver->get_solver().get_literal_selection_strategy()->branching_priority_hint(ge, 5);
    }
  info._ge = ge;
  info._lt = _theory_solver->get_solver().get_literal_data(ge)->get_opposite();
}

void domain_handler::init_inequalities(int v)
{
  value_info & info = _all_values[v];
  expression val;
  _theory_solver->get_expr_for_int(v, val);
  expression le = _theory_solver->get_solver().get_factory()->create_expression(function_symbol::LE, _var, val);
  if(_theory_solver->get_solver().has_expression_data(le) && _theory_solver->get_solver().has_literal_data(le))
    {
      info._le = le;
      info._gt = _theory_solver->get_solver().get_literal_data(le)->get_opposite();
    }
  expression ge = _theory_solver->get_solver().get_factory()->create_expression(function_symbol::GE, _var, val);
  if(_theory_solver->get_solver().has_expression_data(ge) && _theory_solver->get_solver().has_literal_data(ge))
    {
      info._ge = ge;
      info._lt = _theory_solver->get_solver().get_literal_data(ge)->get_opposite();
    }
}

void domain_handler::introduce_literals()
{
  for(std::unordered_set<int>::const_iterator it = _domain_values.begin(),
	it_end = _domain_values.end(); it != it_end; ++it)
    {
      init_literal_pair(*it, true);
    }
}

void domain_handler::propagate_literal(const expression & l)
{
  const extended_boolean l_value = _theory_solver->get_solver().get_trail().get_value(l);
  if(l_value == EB_UNDEFINED)
    {
      _theory_solver->get_theory_solver_data(l)->set_explanation_handler(this);
      //      std::cout << "DOMAIN: " << _var << std::endl;
      _theory_solver->get_solver().apply_propagate(l, _theory_solver);
      _common_data->_count_propagate++;
    }
  else if(l_value == EB_FALSE)
    {
      explanation conflicting;
      expression l_opp = _theory_solver->get_solver().get_literal_data(l)->get_opposite();
      generate_explanation(l, conflicting);
      conflicting.push_back(std::move(l_opp));
      _theory_solver->get_solver().apply_conflict(conflicting, _theory_solver);
      _common_data->_count_conflict++;
    } 
}

void domain_handler::check_singleton_or_empty()
{
  if(_domain_values.size() == 0)
    {
      explanation expl;
      generate_conflict_explanation(expl);
      _theory_solver->get_solver().apply_conflict(expl, _theory_solver);
      _common_data->_count_conflict++;
    }
  else if(_domain_values.size() == 1)
    {
      std::unordered_set<int>::const_iterator it = _domain_values.begin();
      value_info & info = _all_values[*it];
      if(info._eq.get() != 0)
      //init_literal_pair(*it, true);
	propagate_literal(info._eq);
    }
}

void domain_handler::prune_value(int value, const expression & l)
{
  std::unordered_set<int>::iterator dv_it;
  if((dv_it = _domain_values.find(value)) != _domain_values.end())
    {
      if(_trail.current_level() != 0)
	_prunings.push(value);
      value_info & vi = _all_values[value];
      vi._expl = l;
      if(vi._diseq.get() != 0)
	propagate_literal(vi._diseq);
      _domain_values.erase(dv_it);
      update_min_max();
      check_singleton_or_empty();
    }
}

template <typename F>
void domain_handler::prune_values_except(const F & f, const expression & l)
{
  static std::unordered_set<int> new_domain;
  new_domain.clear();
  new_domain.rehash(HASH_TABLE_SIZE);
  for(std::unordered_set<int>::const_iterator it = _domain_values.begin(),
	it_end = _domain_values.end(); it != it_end && 
	!_theory_solver->get_solver().is_conflict(); ++it)
    {
      if(!f(*it))
	{
	  if(_trail.current_level() != 0)
	    _prunings.push(*it);
	  value_info & vi = _all_values[*it];
	  vi._expl = l;
	  if(vi._diseq.get() != 0)
	    propagate_literal(vi._diseq);
	}
      else
	{
	  new_domain.insert(*it);
	}
    }	
  
  if(!_theory_solver->get_solver().is_conflict())
    {
      _domain_values.clear();
      _domain_values.swap(new_domain);
      update_min_max();
      check_singleton_or_empty();
    }
}

void domain_handler::prune_all_values_except(int value, const expression & l)
{
  bool found = false;
  for(std::unordered_set<int>::const_iterator it = _domain_values.begin(),
	it_end = _domain_values.end(); it != it_end && 
	!_theory_solver->get_solver().is_conflict(); ++it)
    {
      if(*it == value)
	{
	  found = true;
	  continue;
	}
      if(_trail.current_level() != 0)
	_prunings.push(*it);
      value_info & vi = _all_values[*it];
      vi._expl = l;
      if(vi._diseq.get() != 0)
	propagate_literal(vi._diseq);
    }	
  
  if(!_theory_solver->get_solver().is_conflict())
    {
      _domain_values.clear();
      if(found)
	_domain_values.insert(value);
      update_min_max();
      check_singleton_or_empty();
    }  
}

void domain_handler::prune_greater_values(int value, const expression & l)
{
  int ind = find_smallest_greater_than(value);
  
  if((int)_sorted_values.size() - ind > (int)_domain_values.size())
    {
      prune_values_except([value] (int v) { return v <= value; }, l);
    }
  else
    {
      for(; ind <= _max_index && !_theory_solver->get_solver().is_conflict(); ind++)
	{
	  assert(_sorted_values[ind] > value);
	  std::unordered_set<int>::iterator dv_it = _domain_values.find(_sorted_values[ind]);
	  if(dv_it != _domain_values.end())
	    {
	      if(_trail.current_level() != 0)
		_prunings.push(*dv_it);
	      value_info & vi = _all_values[*dv_it];
	      vi._expl = l;
	      if(vi._diseq.get() != 0)
		propagate_literal(vi._diseq);
	      _domain_values.erase(dv_it);
	    }
	}
      update_min_max();
      check_singleton_or_empty();  
    }
}

void domain_handler::prune_lower_values(int value, const expression & l)
{
  int ind = find_greatest_lower_than(value);

  if(ind + 1 > (int)_domain_values.size())
    {
      prune_values_except([value] (int v) { return v >= value; }, l);
    }
  else
    {
      for(; ind >= _min_index; ind--)
	{
	  assert(_sorted_values[ind] < value);
	  std::unordered_set<int>::iterator dv_it = _domain_values.find(_sorted_values[ind]);
	  if(dv_it != _domain_values.end())
	    {
	      if(_trail.current_level() != 0)
		_prunings.push(*dv_it);
	      value_info & vi = _all_values[*dv_it];
	      vi._expl = l;
	      if(vi._diseq.get() != 0)
		propagate_literal(vi._diseq);
	      _domain_values.erase(dv_it);
	    }
	}
      update_min_max();
      check_singleton_or_empty();  
    }
}

int domain_handler::find_smallest_greater_than(int value) const
{
  int ind = 0;
  std::unordered_map<int, value_info>::const_iterator it = _all_values.find(value);
  if(it != _all_values.end() && it->second._index != (unsigned)(-1))
    ind = it->second._index + 1;
  else
    {
      std::vector<int>::const_iterator vi = std::lower_bound(_sorted_values.begin(), _sorted_values.end(), value);
      ind = vi - _sorted_values.begin();
      if(ind < (int)_sorted_values.size() && _sorted_values[ind] == value)
	ind++;
    }
  return ind;
}

int domain_handler::find_greatest_lower_than(int value) const
{
  int ind = 0;
  std::unordered_map<int, value_info>::const_iterator it = _all_values.find(value);
  if(it != _all_values.end() && it->second._index != (unsigned)(-1))
    ind = it->second._index - 1;
  else
    {
      std::vector<int>::const_iterator vi = std::lower_bound(_sorted_values.begin(), _sorted_values.end(), value);
      ind = vi - _sorted_values.begin() - 1;      
    }
  return ind;
}

int domain_handler::find_smallest_greater_or_equal(int value) const
{
  int ind = 0;
  std::unordered_map<int, value_info>::const_iterator it = _all_values.find(value);
  if(it != _all_values.end() && it->second._index != (unsigned)(-1))
    ind = it->second._index;
  else
    {
      std::vector<int>::const_iterator vi = std::lower_bound(_sorted_values.begin(), _sorted_values.end(), value);
      ind = vi - _sorted_values.begin();
    }
  return ind;  
}

int domain_handler::find_greatest_lower_or_equal(int value) const
{
  int ind = 0;
  std::unordered_map<int, value_info>::const_iterator it = _all_values.find(value);
  if(it != _all_values.end() && it->second._index != (unsigned)(-1))
    ind = it->second._index;
  else
    {
      std::vector<int>::const_iterator vi = std::lower_bound(_sorted_values.begin(), _sorted_values.end(), value);      
      ind = vi - _sorted_values.begin();
      if(ind == (int)_sorted_values.size() || _sorted_values[ind] > value)
	ind--;
    }
  return ind;
}


void domain_handler::check_finite(const expression & l)
{
  //std::cout << "DOMAIN CHECK: " << _var << " : " << l << std::endl;
  if(l->is_disequality())
    {
      const expression & val = l->get_operands()[1];
      int value = _theory_solver->get_theory_solver_data(val)->get_int_value();
      prune_value(value, l);
    } 
  else if(l->is_equality())
    {
      const expression & val = l->get_operands()[1];
      int value = _theory_solver->get_theory_solver_data(val)->get_int_value();
      prune_all_values_except(value, l);
    }
  else if(l->get_symbol() == function_symbol::LE)
    {
      const expression & val = l->get_operands()[1];
      int b = _theory_solver->get_theory_solver_data(val)->get_int_value();
      prune_greater_values(b, l);
    }
  else if(l->get_symbol() == function_symbol::LT)
    {
      const expression & val = l->get_operands()[1];
      int b = _theory_solver->get_theory_solver_data(val)->get_int_value() - 1;
      prune_greater_values(b, l);
    }
  else if(l->get_symbol() == function_symbol::GE)
    {
      const expression & val = l->get_operands()[1];
      int b = _theory_solver->get_theory_solver_data(val)->get_int_value();
      prune_lower_values(b, l);
    }
  else if(l->get_symbol() == function_symbol::GT)
    {
      const expression & val = l->get_operands()[1];
      int b = _theory_solver->get_theory_solver_data(val)->get_int_value() + 1;
      prune_lower_values(b, l);
    } 
}


void domain_handler::check_and_propagate(unsigned /* layer */) 
{
  for( ; !is_finite() && _next_to_assert < _trail.size(); _next_to_assert++)
    {
      expression l = _trail[_next_to_assert];
      check_not_finite(l);
    }

  for( ; _next_to_assert < _trail.size(); _next_to_assert++)
    {
      expression l = _trail[_next_to_assert];
      check_finite(l);
    }
  _theory_solver->process_main_trail();
}

#include "csp_explanation_minimizer.hpp"

void domain_handler::prunings_explanations_singleton(explanation & expl, int val)
{
  expl.reserve(10);
  static explanation_minimizer wrapper;
  wrapper.reset(_theory_solver);
  //explanation_minimizer wrapper(_theory_solver);

  if(_finite_level != 0 && !_finite_domain_cause.empty())
    for(unsigned i = 0; i < _finite_domain_cause.size(); i++)
      wrapper.push_back(_finite_domain_cause[i]);
  
  for(unsigned i = 0; i < _prunings.size(); i++)
    {
      if(_prunings[i] == val)
	continue;

      wrapper.push_back(_all_values[_prunings[i]]._expl);
    }
  wrapper.export_to_expl(expl);
}


void domain_handler::prunings_explanations_lower_vals(explanation & expl, int val)
{
  expl.reserve(10);
  static explanation_minimizer wrapper;
  wrapper.reset(_theory_solver);
  //  explanation_minimizer wrapper(_theory_solver);
  if(_finite_level != 0 && !_finite_domain_cause.empty())
    wrapper.push_back(_finite_domain_cause[0]); // l_cause, equality
  
  for(int i = get_zero_min_index(); i < (int)_sorted_values.size() && _sorted_values[i] < val; i++)
    {
      wrapper.push_back(_all_values[_sorted_values[i]]._expl);
    }
  wrapper.export_to_expl(expl);
}

void domain_handler::prunings_explanations_greater_vals(explanation & expl, int val)
{
  expl.reserve(10);
  static explanation_minimizer wrapper;
  wrapper.reset(_theory_solver);
  //  explanation_minimizer wrapper(_theory_solver);
  if(_finite_level != 0 && !_finite_domain_cause.empty())
    wrapper.push_back(_finite_domain_cause.back()); // u_cause, equality
  
  for(int i = get_zero_max_index(); i >= 0 && _sorted_values[i] > val; i--)
    {
      wrapper.push_back(_all_values[_sorted_values[i]]._expl);
    }
  wrapper.export_to_expl(expl);
}

void domain_handler::generate_conflict_explanation(explanation & expl)
{
  expl.reserve(10);
  static explanation_minimizer wrapper;
  wrapper.reset(_theory_solver);
  //  explanation_minimizer wrapper(_theory_solver);
  if(_finite_level != 0 && !_finite_domain_cause.empty())
    for(unsigned i = 0; i < _finite_domain_cause.size(); i++)
      wrapper.push_back(_finite_domain_cause[i]);
  
  for(unsigned i = 0; i < _prunings.size(); i++)
    {
      wrapper.push_back(_all_values[_prunings[i]]._expl);
    }

  wrapper.export_to_expl(expl);
}

void domain_handler::generate_explanation(const expression & l, explanation & expl)
{  
  int val = _theory_solver->get_theory_solver_data(l->get_operands()[1])->get_int_value();
  const function_symbol & fs = l->get_symbol();
  
  if(fs == function_symbol::DISTINCT)
    {
      expression cause = _all_values[val]._expl;
      if(cause.get() != 0)
	expl.push_back(cause);
      else
	{
	  if(_finite_level != 0 && !_finite_domain_cause.empty())
	    for(unsigned i = 0; i < _finite_domain_cause.size(); i++)
	      expl.push_back(_finite_domain_cause[i]);
	}
    }
  else if(fs == function_symbol::EQ)
    {
      prunings_explanations_singleton(expl, val);
    }
  else if(fs == function_symbol::GE)
    {
      prunings_explanations_lower_vals(expl, val);
    }
  else if(fs == function_symbol::GT)
    {
      prunings_explanations_lower_vals(expl, val + 1);
    }
  else if(fs == function_symbol::LE)
    {
      prunings_explanations_greater_vals(expl, val);
    }
  else if(fs == function_symbol::LT)
    {
      prunings_explanations_greater_vals(expl, val - 1);
    }
}

void domain_handler::explain_minimum_at_trail_size(unsigned size, explanation & expl)
{
  expl.reserve(10);
  static explanation_minimizer wrapper;
  wrapper.reset(_theory_solver);
  //explanation_minimizer wrapper(_theory_solver);

  if(_finite_level != 0 && !_finite_domain_cause.empty())
    wrapper.push_back(_finite_domain_cause[0]); // l_cause, equality

  int i;
  for(i = get_zero_min_index(); i < _min_index; i++)
    {
      assert(_domain_values.find(_sorted_values[i]) == _domain_values.end());
      const value_info & vi = _all_values[_sorted_values[i]];
      if(_theory_solver->get_solver().get_trail().get_trail_index(vi._expl) < size)
	wrapper.push_back(vi._expl);
      else
	break;
    }
  
  if(i < (int)_sorted_values.size())
    {
      const value_info & vi = _all_values[_sorted_values[i]];
      if(vi._eq.get() != 0 && _theory_solver->get_solver().get_trail().get_value(vi._eq) == EB_TRUE
	 && _theory_solver->get_solver().get_trail().get_trail_index(vi._eq) < size)
	{
	  expl.push_back(vi._eq);
	  return;
	}
    }
  
  wrapper.export_to_expl(expl);
}

void domain_handler::explain_maximum_at_trail_size(unsigned size, explanation & expl)
{
  expl.reserve(10);
  static explanation_minimizer wrapper;
  wrapper.reset(_theory_solver);
  //explanation_minimizer wrapper(_theory_solver);

  if(_finite_level != 0 && !_finite_domain_cause.empty())
    wrapper.push_back(_finite_domain_cause.back()); // u_cause, equality

  int i;
  for(i = get_zero_max_index(); i > _max_index; i--)
    {
      assert(_domain_values.find(_sorted_values[i]) == _domain_values.end());
      const value_info & vi = _all_values[_sorted_values[i]];
      if(_theory_solver->get_solver().get_trail().get_trail_index(vi._expl) < size)
	wrapper.push_back(vi._expl);
      else
	break;
    }

  if(i >= 0)
    {
      const value_info & vi = _all_values[_sorted_values[i]];
      if(vi._eq.get() != 0 && _theory_solver->get_solver().get_trail().get_value(vi._eq) == EB_TRUE
	 && _theory_solver->get_solver().get_trail().get_trail_index(vi._eq) < size)
	{
	  expl.push_back(vi._eq);
	  return;
	}
    }
  wrapper.export_to_expl(expl);
}

void domain_handler::explain_minmax_at_trail_size(unsigned size, explanation & expl)
{
  expl.reserve(10);
  static explanation_minimizer wrapper;
  wrapper.reset(_theory_solver);
  //explanation_minimizer wrapper(_theory_solver);

  if(_finite_level != 0 && !_finite_domain_cause.empty())
    for(unsigned i = 0; i < _finite_domain_cause.size(); i++)
      wrapper.push_back(_finite_domain_cause[i]);
  
  int i;
  for(i = get_zero_min_index(); i < _min_index; i++)
    {
      assert(_domain_values.find(_sorted_values[i]) == _domain_values.end());
      const value_info & vi = _all_values[_sorted_values[i]];
      if(_theory_solver->get_solver().get_trail().get_trail_index(vi._expl) < size)
	wrapper.push_back(vi._expl);
      else
	break;
    }
  
  if(i < (int)_sorted_values.size())
    {
      const value_info & vi = _all_values[_sorted_values[i]];
      if(vi._eq.get() != 0 && _theory_solver->get_solver().get_trail().get_value(vi._eq) == EB_TRUE
	 && _theory_solver->get_solver().get_trail().get_trail_index(vi._eq) < size)
	{
	  expl.push_back(vi._eq);
	  return;
	}
    }

  for(i = get_zero_max_index(); i > _max_index; i--)
    {
      assert(_domain_values.find(_sorted_values[i]) == _domain_values.end());
      const value_info & vi = _all_values[_sorted_values[i]];
      if(_theory_solver->get_solver().get_trail().get_trail_index(vi._expl) < size)
	wrapper.push_back(vi._expl);
      else
	break;
    }
  
  wrapper.export_to_expl(expl);  
}


void domain_handler::explain_domain_at_trail_size(unsigned size, explanation & expl)
{
  expl.reserve(10);  
  static explanation_minimizer wrapper;
  wrapper.reset(_theory_solver);
  //explanation_minimizer wrapper(_theory_solver);

  if(_finite_level != 0 && !_finite_domain_cause.empty())
    for(unsigned i = 0; i < _finite_domain_cause.size(); i++)
      wrapper.push_back(_finite_domain_cause[i]);

  int k = -1;
  for(int i = get_zero_min_index(), zero_max_local = get_zero_max_index(); i <= zero_max_local; i++)
    {
      if(_domain_values.find(_sorted_values[i]) != _domain_values.end())
 	{
 	  if(k == -1)
 	    k = i;
 	  else 
 	    k = _sorted_values.size();
	  
 	  continue;
 	}
      const value_info & vi = _all_values[_sorted_values[i]];
      if(_theory_solver->get_solver().get_trail().get_trail_index(vi._expl) < size)
	wrapper.push_back(vi._expl);
      else
	{
	  if(k == -1)
 	    k = i;
 	  else 
 	    k = _sorted_values.size();	  
	}
    }
  
  // signleton domain case
  if(k != -1 && k != (int)_sorted_values.size())
    {
      const value_info & vi = _all_values[_sorted_values[k]];
      if(vi._eq.get() != 0 && _theory_solver->get_solver().get_trail().get_value(vi._eq) == EB_TRUE
 	 && _theory_solver->get_solver().get_trail().get_trail_index(vi._eq) < size)
 	{
 	  expl.push_back(vi._eq);
 	  return;
 	}
    }    
  wrapper.export_to_expl(expl);
}

void domain_handler::explain_literal(const expression & l) 
{
  explanation expl;
  //  std::cout << "Explaining from domain: " << _var << std::endl;
  generate_explanation(l, expl);
  _theory_solver->get_solver().apply_explain(l, expl);
}
  
void domain_handler::backjump(unsigned level)
{
  _trail.backjump(level);
  _prunings.backjump(level, on_pop(this));
  _next_to_assert = _trail.size();
  if(!is_finite())
    {
      _finite_domain_cause.clear();
      _domain_values.clear();
      _sorted_values.clear();
      for(std::unordered_map<int, value_info>::iterator it = _all_values.begin(),
	    it_end = _all_values.end(); it != it_end; ++it)
	it->second._index = (unsigned)(-1);
      _min_index = -1;
      _max_index = -1;
      _zero_min_index = -1;
      _zero_max_index = -1;
      _finite_level = 0xffffffff;
    }
}

const expression & domain_handler::get_assigned_value() const
{
  assert(_domain_values.size() == 1);
  int v = *_domain_values.begin();
  static expression val;
  _theory_solver->get_expr_for_int(v, val);
  return val;
}
