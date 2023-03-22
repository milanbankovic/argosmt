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

#include "arithmetic_theory_solver.hpp"



bool arithmetic_theory_solver::closest_basic_unknown_selector::
select_basic_unknown(expression & bu)
{
  if(_counter < _limit)
    {
      _counter++;
      _theory_solver->_parallel_time_spent.start();
      bool found = false;
      static value_pair min_distance;
      static value_pair curr_distance;
      unsigned min_length = 0;
      for(unknown_set::const_iterator it = _theory_solver->_basic_unknowns.begin(),
	    it_end = _theory_solver->_basic_unknowns.end(); it != it_end; ++it)
	{
	  arithmetic_theory_solver_data * curr_data = _theory_solver->_data.get_data(*it);
	  bool lv = false, uv = false;
	  if((lv = curr_data->get_valuation() < curr_data->get_lower_bound()) ||
	     (uv = curr_data->get_valuation() > curr_data->get_upper_bound()))
	    {
	      if(!found)
		{
		  bu = *it;
		  min_distance = lv ? curr_data->get_lower_bound() - curr_data->get_valuation() :
		    curr_data->get_valuation() - curr_data->get_upper_bound();
		  min_length = curr_data->row().size();
		  found = true;
		}
	      else
		{
		  unsigned curr_length = curr_data->row().size();
		  curr_distance = lv ? curr_data->get_lower_bound() - curr_data->get_valuation() :
		    curr_data->get_valuation() - curr_data->get_upper_bound();
		  if(curr_distance < min_distance ||
		     (curr_distance == min_distance && curr_length < min_length))
		    {
		      min_distance = curr_distance;
		      min_length = curr_length;
		      bu = *it;
		    }
		}
	    }
	}
      _theory_solver->_parallel_time_spent.acumulate();
      return found;
    }
  else
    {
      return _theory_solver->find_first_basic_unknown(bu);
    }
}

bool arithmetic_theory_solver::least_frequent_non_basic_unknown_selector::
select_non_basic_unknown(const linear_polynomial & lp, bool lb_violated, expression & nbu)
{
  if(_counter < _limit)
    {
      _counter++;

      std::vector< std::pair<expression, unsigned> > counters;
      for(linear_polynomial::const_iterator jt = lp.begin(), jt_end = lp.end(); jt != jt_end; ++jt)
	{
	  if(jt->first == _theory_solver->_one_expression)
	    continue;
	  arithmetic_theory_solver_data * curr_data = _theory_solver->_data.get_data(jt->first);	     
	  if((((lb_violated && jt->second > 0) || (!lb_violated && jt->second < 0)) &&
	      curr_data->get_valuation() < curr_data->get_upper_bound()) ||
	     (((lb_violated && jt->second < 0) || (!lb_violated && jt->second > 0))
	      && curr_data->get_valuation() > curr_data->get_lower_bound()))
	    counters.push_back(std::make_pair(jt->first, 0));
	}
      if(counters.empty())
	return false;
      
      _theory_solver->_parallel_time_spent.start();
      for(unknown_set::const_iterator it = _theory_solver->_basic_unknowns.begin(),
	    it_end = _theory_solver->_basic_unknowns.end(); it != it_end; ++it)
	{
	  const linear_polynomial & bu_lp = _theory_solver->_data.get_data(*it)->row();
	  for(auto jt = counters.begin(), jt_end = counters.end(); jt != jt_end; ++jt)
	    {
	      if(bu_lp.get_coeff(jt->first) != 0)
		jt->second++;		
	    }
	}
      _theory_solver->_parallel_time_spent.acumulate();
      unsigned min = (unsigned)(-1);
      unsigned min_index = (unsigned)(-1);	  
      for(auto jt = counters.begin(), jt_end = counters.end(); jt != jt_end; ++jt)
	{
	  if(jt->second < min || (jt->second == min && _theory_solver->_data.get_data(jt->first)->get_index() < min_index))
	    {
	      min = jt->second;
	      nbu = jt->first;
	      min_index = _theory_solver->_data.get_data(nbu)->get_index();
	    }
	}
      return true;
    }
  else
    {
      return _theory_solver->find_first_non_basic_unknown(lp, lb_violated, nbu);
    }
}


void arithmetic_theory_solver::shuffle_unknowns()
{
  _basic_unknowns.clear();
  _non_basic_unknowns.clear();
  _inactive_unknowns.clear();
  std::random_shuffle(_all_unknowns.begin(),
		      _all_unknowns.end(),
		      [this] (unsigned n)
		      {
			std::uniform_int_distribution<unsigned> int_dist(0, n - 1);
			return int_dist(_rng);
		      }); 
  for(unsigned i = 0; i < _all_unknowns.size(); i++)
    {
      arithmetic_theory_solver_data * u_data = _data.get_data(_all_unknowns[i]);
      u_data->set_index(i);
      if(u_data->is_basic())
	{
	  if(!u_data->get_constraints().empty())
	    _basic_unknowns.insert(_all_unknowns[i]);
	  else
	    _inactive_unknowns.insert(std::make_pair(_all_unknowns[i], false));
	}
      else
	_non_basic_unknowns.insert(_all_unknowns[i]);
    }
}

value_pair arithmetic_theory_solver::calculate_polynomial_value(const linear_polynomial & lp)
{
  value_pair res;

  for(linear_polynomial::const_iterator it = lp.begin(), it_end = lp.end(); it != it_end; ++it)
    {
      if(it->first == _one_expression)
	res += value_pair(it->second, 0);
      else
	res += _data.get_data(it->first)->get_valuation() * it->second;
    }
  return res;
}

void arithmetic_theory_solver::activate_unknown(const expression & unknown)
{
  if(_inactive_unknowns.find(unknown) != _inactive_unknowns.end())
    {
      arithmetic_theory_solver_data * u_data = _data.get_data(unknown);
      assert(u_data->is_basic());
      //if(!u_data->is_basic()) { std::cout << "ERROR: NOT-BASIC-INACTIVE" << std::endl; exit(1); }
      _inactive_unknowns.erase(unknown);
      _basic_unknowns.insert(unknown);
      expand_polynomial(u_data->row());
      u_data->set_valuation(calculate_polynomial_value(u_data->row()));
    }
}

void arithmetic_theory_solver::invalidate_inactive_unknowns()
{
  for(auto & p : _inactive_unknowns)
    p.second = false;
}

const value_pair & arithmetic_theory_solver::recalculate_unknown_value(const expression & unknown)
{
  arithmetic_theory_solver_data * u_data = _data.get_data(unknown);
  std::unordered_map<expression, bool>::iterator it = _inactive_unknowns.find(unknown);
  if(it != _inactive_unknowns.end() && it->second == false)
    {
      linear_polynomial & lp = u_data->row();
      value_pair res;

      for(linear_polynomial::const_iterator it = lp.begin(), it_end = lp.end(); it != it_end; ++it)
	{
	  if(it->first == _one_expression)
	    res += value_pair(it->second, 0);
	  else
	    res += recalculate_unknown_value(it->first) * it->second;
	}
      u_data->set_valuation(std::move(res));
      it->second = true;
    }
  return u_data->get_valuation();
}

void arithmetic_theory_solver::expand_polynomial(linear_polynomial & lp)
{
  linear_polynomial exp_lp;

  for(linear_polynomial::const_iterator it = lp.begin(), it_end = lp.end(); it != it_end; ++it)
    {
      if(it->first == _one_expression)
	{
	  exp_lp.add_to_coeff(it->first, it->second);
	  continue;
	}

      arithmetic_theory_solver_data * curr_data = _data.get_data(it->first);
      if(curr_data->is_basic())
	{
	  if(_inactive_unknowns.find(it->first) != _inactive_unknowns.end())
	    expand_polynomial(curr_data->row());
	  exp_lp.add_polynomial(curr_data->row(), it->second);
	}
      else
	exp_lp.add_to_coeff(it->first, it->second);
    }
  lp = std::move(exp_lp);
}

void arithmetic_theory_solver::init_monomial_unknowns(const linear_polynomial & lp)
{
  for(linear_polynomial::const_iterator it = lp.begin(), it_end = lp.end(); it != it_end; ++it)
    {
      if(it->first == _one_expression)
	continue;

      if(!_data.has_data(it->first))
	init_unknown(it->first);
    }
}

void arithmetic_theory_solver::init_unknown(const expression & unknown)
{
  if(_data.has_data(unknown))
    return;

  bool is_integer = (unknown->get_inferred_sort()->get_symbol() == sort_symbol::INT);
  if(is_integer && is_atom(unknown))
    {
      _integer_unknowns.push_back(unknown);
      //      std::cout << "Integer unknown: " << unknown << std::endl;
    }

  arithmetic_theory_solver_data * u_data = new arithmetic_theory_solver_data(_current_level, _all_unknowns.size(), is_integer);
  linear_polynomial & lp = u_data->row();
  _data.set_data(unknown, u_data);
  get_polynomial_from_expression(unknown, lp);
  _all_unknowns.push_back(unknown);

  if(_solver.get_common_data(unknown)->is_shared())
    {
      add_eq_relevant_unknown(unknown);
    }

  if(is_atom(unknown))
    {
      _non_basic_unknowns.insert(unknown);
      u_data->set_basic(false);      
      search_unknowns_rec(unknown);
    }
  else if(is_coefficient(unknown))
    {
      _inactive_unknowns.insert(std::make_pair(unknown, false));
      u_data->set_basic(true);
    }
  else /* arithmetic */
    {
      init_monomial_unknowns(lp);
      _inactive_unknowns.insert(std::make_pair(unknown, false));
      u_data->set_basic(true);     
    }
  
  if(u_data->is_basic())
    ;
    //u_data->set_valuation(calculate_polynomial_value(lp));
  else if(u_data->is_integer())
    u_data->set_valuation(value_pair(_all_unknowns.size(), 0));
    //u_data->set_valuation(value_pair(0, 0));
  else
    u_data->set_valuation(value_pair(0, _all_unknowns.size()));
    //u_data->set_valuation(value_pair(0, 0));
  //    std::cout << "Init unknown: " << unknown << " (index: " << u_data->get_index() <<  " ) : (is_integer: " << 
  //   is_integer << ") : (initial value: " << u_data->get_valuation() << ")" << std::endl;
  //std::cout << "Basic: " << _basic_unknowns.size() << std::endl;
}

void arithmetic_theory_solver::search_unknowns_rec(const expression & e)
{
  if(is_coefficient(e) || is_arithmetic_operation(e))
    {
      init_unknown(e);
      return;
    }

  const expression_vector & ops = e->get_operands();
  for(unsigned i = 0; i < ops.size(); i++)
    search_unknowns_rec(ops[i]);
}

void arithmetic_theory_solver::add_literal(const expression & l, const expression & l_opp)
{
  if(_data.has_data(l))
    return;

  if(is_arithmetic_relation(l) || 
     (l->is_equality() && is_lra_term(l->get_operands()[0]) && is_lra_term(l->get_operands()[1])))
    {
      _solver.get_literal_data(l)->add_observing_theory_solver(this);
    }
  
  assert(_solver.get_literal_data(l)->is_positive());
  assert(_solver.get_literal_data(l)->get_opposite() == l_opp);

  expression unknown, bound;
  function_symbol fs = function_symbol::UNDEFINED;
  if(is_arithmetic_relation(l))
    {
      unknown = l->get_operands()[0];
      bound = l->get_operands()[1];
      fs = l->get_symbol();
    }
  else if(l->is_equality() && is_lra_term(l->get_operands()[0]) && is_lra_term(l->get_operands()[1]))
    {
      expression can_eq = canonize_relation(l);
      unknown = can_eq->get_operands()[0];
      bound = can_eq->get_operands()[1];
      add_private_expression(unknown);
      fs = can_eq->get_symbol();
    }
  else
    {
      search_unknowns_rec(l);
      return;
    }
  //std::cout << "CONSTRAINT: " << l << std::endl;
  
  unknown_value c = get_value_from_expr(bound);
  if(unknown == _zero_expression)
    {
      found_trivial_propagation(c, fs, l, l_opp);
      _data.set_data(l, new arithmetic_theory_solver_data());
      _data.set_data(l_opp, new arithmetic_theory_solver_data());
      return;
    }

  init_unknown(unknown);
    
  //  if(l->is_equality())
  // {
      //      expression left = l->get_operands()[0];
      //expression right = l->get_operands()[1];
      //init_unknown(left);
      //init_unknown(right);
      //add_eq_relevant_unknown(left);
      //add_eq_relevant_unknown(right);
  // }
  

  bound_type bt;
  if(fs == function_symbol::LE)
    bt = B_UPPER;
  else if(fs == function_symbol::GE)
    bt = B_LOWER;
  else // equality
    bt = B_BOTH;

  value_pair rp(c, 0);
  arithmetic_theory_solver_data * u_data = _data.get_data(unknown);
  if(u_data->is_integer() && (bt == B_UPPER || bt == B_LOWER))
    {
      tighten_integer_bound(rp, bt);
    }
  arithmetic_theory_solver_data * l_data = new arithmetic_theory_solver_data(unknown, std::move(rp), bt);
  _data.set_data(l, l_data);
  u_data->add_constraint(l);
  
  rp.first = std::move(c);
  if(fs == function_symbol::LE)
    {
      bt = B_LOWER;
      rp.second = 1;
    }
  else if(fs == function_symbol::GE)
    {
      bt = B_UPPER;
      rp.second = -1;
    }
  else // disequality
    {
      bt = B_SPLIT;
      rp.second = 0;
    }

  if(u_data->is_integer() && (bt == B_UPPER || bt == B_LOWER))
    {
      tighten_integer_bound(rp, bt);
    }
  arithmetic_theory_solver_data * l_opp_data = new arithmetic_theory_solver_data(unknown, std::move(rp), bt);
  _data.set_data(l_opp, l_opp_data);
  u_data->add_constraint(l_opp);       
  activate_unknown(unknown);
}

void arithmetic_theory_solver::new_level()
{
  _new_level_time_spent.start();
  _current_level++;
  _new_level_time_spent.acumulate();
}

void arithmetic_theory_solver::backjump(unsigned level)
{
  _backjump_time_spent.start();
  _current_level = level;
  for(unsigned i = 0; i < _all_unknowns.size(); i++)
    {
      _data.get_data(_all_unknowns[i])->restore_state();
    }
  _current_assertion_pos = std::min(_current_assertion_pos, _solver.get_trail().size());
  _just_backjumped = true;

  _bound_changed = true;
  _disequality_asserted = true;
  _backjump_time_spent.acumulate();
}

void arithmetic_theory_solver::explain_literal(const expression & l)
{
  _explain_time_spent.start();
  arithmetic_theory_solver_data * l_data = _data.get_data(l);
  if(l_data->has_constraint_data())
    _solver.apply_explain(l, l_data->literal_explanation());
  else
    _solver.apply_explain(l, explanation());
  _explain_time_spent.acumulate();
}


void arithmetic_theory_solver::process_assertions()
{
  _process_assertions_time_spent.start();
  
  while(!_solver.is_conflict() && _current_assertion_pos < _solver.get_trail().size())
    {
      expression l = _solver.get_trail()[_current_assertion_pos++];

      if(!_solver.get_literal_data(l)->is_observing_theory_solver(this))
	continue;
      
      //if(_solver.get_trail().get_source_theory_solver(l) == this)
      //  continue;

      //std::cout << "ASSERTING: " << l << std::endl;
      process_assertion(l);
    }
  _process_assertions_time_spent.acumulate();
}

void arithmetic_theory_solver::process_assertion(const expression & l)
{
  assert(l->is_equality() || l->is_disequality() || is_arithmetic_relation(l));
  assert(_data.has_data(l));
  assert(_solver.has_literal_data(l));
  
  arithmetic_theory_solver_data * l_data = _data.get_data(l);
  if(l_data->has_constraint_data())
    {
      //      std::cout << "PROCESSING ASSERTION: " << l << std::endl;

      switch(l_data->get_constraint().type)
	{
	case B_UPPER:
	  assert_upper(l);
	  break;
	case B_LOWER:
	  assert_lower(l);
	  break;
	case B_BOTH:
	  assert_both(l);
	  break;
	case B_SPLIT:
	  assert_split(l);
	  break;
	}
    }

  // Assertion on integer bounds for integer unknowns
  assert(!l_data->has_constraint_data() || !_data.get_data(l_data->get_unknown())->is_integer() ||
	 (
	  (_data.get_data(l_data->get_unknown())->get_lower_bound().infinity != I_NONE ||
	   (is_integer_value(_data.get_data(l_data->get_unknown())->get_lower_bound().first) &&
	    _data.get_data(l_data->get_unknown())->get_lower_bound().second == 0))
	  &&
	  (_data.get_data(l_data->get_unknown())->get_upper_bound().infinity != I_NONE ||
	   (is_integer_value(_data.get_data(l_data->get_unknown())->get_upper_bound().first) &&
	    _data.get_data(l_data->get_unknown())->get_upper_bound().second == 0))
	  )
	 );
}

void arithmetic_theory_solver::assert_upper(const expression & l)
{
  //std::cout << "ASSERT UPPER: " << l << std::endl;
  arithmetic_theory_solver_data * l_data = _data.get_data(l);
  const expression & unknown = l_data->get_unknown();
  const value_pair & bound = l_data->get_constraint().bound;  
  arithmetic_theory_solver_data * u_data = _data.get_data(unknown);
  
  assert(bound.infinity == I_NONE);
  
  if(bound >= u_data->get_upper_bound()) 
    return;
  
  _bound_changed = true;

  if(bound < u_data->get_lower_bound())
    {
      explanation conflicting;
      conflicting.push_back(u_data->get_lower_bound_relation());
      conflicting.push_back(l);
      _solver.apply_conflict(conflicting, this);
      return;
    }
  
  u_data->set_upper_bound(bound, l);
  
  if(!u_data->is_basic() && u_data->get_valuation() > bound)
    update(unknown, bound);

  if(is_fixed_by_constraints(unknown))
    {
      static constraint c_both;
      c_both.bound = bound;
      c_both.type = B_BOTH;
      propagate_weaker_constraints(unknown, c_both);
    }
  else
    propagate_weaker_constraints(unknown, l_data->get_constraint());
}

void arithmetic_theory_solver::assert_lower(const expression & l)
{
  //  std::cout << "ASSERT LOWER: " << l << std::endl;
  arithmetic_theory_solver_data * l_data = _data.get_data(l);
  const expression & unknown = l_data->get_unknown();
  const value_pair & bound = l_data->get_constraint().bound;  
  arithmetic_theory_solver_data * u_data = _data.get_data(unknown);

  assert(bound.infinity == I_NONE);

  if(bound <= u_data->get_lower_bound())
    return;

  _bound_changed = true;

  if(bound > u_data->get_upper_bound())
    {
      explanation conflicting;
      conflicting.push_back(u_data->get_upper_bound_relation());
      conflicting.push_back(l);
      _solver.apply_conflict(conflicting, this);
      return;
    }

  u_data->set_lower_bound(bound, l);

  if(!u_data->is_basic() && u_data->get_valuation() < bound)
    update(unknown, bound);

  if(is_fixed_by_constraints(unknown))
    {
      static constraint c_both;
      c_both.bound = bound;
      c_both.type = B_BOTH;
      propagate_weaker_constraints(unknown, c_both);
    }
  else
    propagate_weaker_constraints(unknown, l_data->get_constraint());
}

void arithmetic_theory_solver::assert_both(const expression & l)
{
  //  std::cout << "ASSERT BOTH: " << l << std::endl;
  arithmetic_theory_solver_data * l_data = _data.get_data(l);
  const expression & unknown = l_data->get_unknown();
  const value_pair & bound = l_data->get_constraint().bound;  
  arithmetic_theory_solver_data * u_data = _data.get_data(unknown);
  const constraint & cst = l_data->get_constraint();
  assert(bound.infinity == I_NONE);

  if(u_data->is_integer() && !is_integer_value(cst.bound.first))
    {
      explanation conflicting;
      conflicting.push_back(l);
      _solver.apply_conflict(conflicting, this);
    }
  else
    {
      if(bound < u_data->get_lower_bound())
	{
	  explanation conflicting;
	  conflicting.push_back(u_data->get_lower_bound_relation());
	  conflicting.push_back(l);
	  _solver.apply_conflict(conflicting, this);
	  return;
	}

      if(bound > u_data->get_upper_bound())
	{
	  explanation conflicting;
	  conflicting.push_back(u_data->get_upper_bound_relation());
	  conflicting.push_back(l);
	  _solver.apply_conflict(conflicting, this);
	  return;
	}
  
      _bound_changed = true;

      u_data->set_upper_bound(bound, l);
      u_data->set_lower_bound(bound, l);

      if(!u_data->is_basic() && u_data->get_valuation() != bound)
	update(unknown, bound);
      
      static constraint c_both;
      c_both.bound = bound;
      c_both.type = B_BOTH;
      propagate_weaker_constraints(unknown, c_both);
    }
}

void arithmetic_theory_solver::assert_split(const expression & l)
{
  if(_solver.get_trail().get_source_theory_solver(l) != this)
    {      
      _disequality_asserted = true;
      arithmetic_theory_solver_data * l_data = _data.get_data(l);
      const expression & unknown = l_data->get_unknown();
      const constraint & cst = l_data->get_constraint();
      propagate_weaker_constraints(unknown, cst);
    }
}

void arithmetic_theory_solver::split_on_demand(const expression & l)
{
  arithmetic_theory_solver_data * l_data = _data.get_data(l);

  if(l_data->is_split_learned())
    return;
  l_data->set_split_learned(true);  
  _bound_changed = true;

  const expression & unknown = l_data->get_unknown();
  expression c = get_expr_from_value(l_data->get_constraint().bound.first);
  expression lt = _solver.get_factory()->create_expression(function_symbol::LT, unknown, c);
  expression gt = _solver.get_factory()->create_expression(function_symbol::GT, unknown, c);
  _solver.apply_introduce_literal(lt);
  _solver.apply_introduce_literal(gt);
  clause * cl = new clause();  
  cl->push_back(_solver.get_literal_data(l)->get_opposite());
  cl->push_back(std::move(lt));
  cl->push_back(std::move(gt));
  _solver.apply_theory_lemma(cl);
}

void arithmetic_theory_solver::tighten_upper_integer_bound(value_pair & bound)
{
  static unknown_value temp;
  temp = floor_value(bound.first);
  if(bound.first == temp && bound.second < 0)
    bound.first = temp - 1;
  else 
    bound.first = std::move(temp);
  bound.second = 0;  
}

void arithmetic_theory_solver::tighten_lower_integer_bound(value_pair & bound)
{
  static unknown_value temp;
  temp = ceil_value(bound.first);
  if(bound.first == temp && bound.second > 0)
    bound.first = temp + 1;
  else 
    bound.first = std::move(temp);
  bound.second = 0;  
}


void arithmetic_theory_solver::tighten_integer_bound(value_pair & bound, bound_type bt)
{
  if(bound.infinity != I_NONE)
    return;

  if(bt == B_UPPER)
    tighten_upper_integer_bound(bound);
  else if(bt == B_LOWER)
    tighten_lower_integer_bound(bound);
}

bool arithmetic_theory_solver::check_integer_unknowns()
{
  _check_int_time_spent.start();
  bool retval = true;
  for(unsigned i = 0; i < _integer_unknowns.size(); i++)
    {
      const expression & unknown = _integer_unknowns[i];      
      const value_pair & vp = recalculate_unknown_value(unknown);

      static value_pair temp;

      if(vp.second != 0 || !is_integer_value(vp.first))
	{

	  retval = false;
	  // u <= s || u >= t, where s is tightened upper bound, and u is tightened lower bound
	  
	  //std::cout << "OUPS: " << unknown << " value: " << vp << std::endl;
	  //std::cout << _data.get_data(unknown)->is_basic() << std::endl;
	  temp = vp;
	  tighten_upper_integer_bound(temp);
	  //std::cout << "UPPER TIGHTEN: " << temp << std::endl;
	  expression c = get_expr_from_value(temp.first);
	  expression lt = _solver.get_factory()->create_expression(function_symbol::LE, unknown, c);

	  if(_learnt_integer_splits.find(lt) != _learnt_integer_splits.end())
	    continue;
	  else
	    _learnt_integer_splits.insert(lt);

	  temp = vp;
	  tighten_lower_integer_bound(temp);
	  //std::cout << "LOWER TIGHTEN: " << temp << std::endl;
	  c = get_expr_from_value(temp.first);
	  expression gt = _solver.get_factory()->create_expression(function_symbol::GE, unknown, c);
	  _solver.apply_introduce_literal(lt);
	  _solver.apply_introduce_literal(gt);
	  clause * cl = new clause();  
	  cl->push_back(std::move(lt));
	  cl->push_back(std::move(gt));
	  _solver.apply_theory_lemma(cl);	
	  //std::cout << "CLAUSE LEARNT: " << *cl << std::endl;
	}
    }
  _check_int_time_spent.acumulate();
  return retval;
}

void arithmetic_theory_solver::check_and_propagate(unsigned layer)
{
  if(layer >= _num_of_layers)
    return;
  
  _check_and_prop_time_spent.start();

  if(layer > 0)
    _check_count++;

  if(!_shuffled)
    {
      shuffle_unknowns();
      _shuffled = true;
    }

  for(unsigned i = 0; !_solver.is_conflict() && i < _new_trivial_propagations.size(); i++)
    apply_trivial_propagation(_new_trivial_propagations[i]);
  _new_trivial_propagations.clear();

  if(_just_backjumped)
    {
      for(unsigned i = 0; !_solver.is_conflict() && i < _trivial_propagations.size(); i++)
	apply_trivial_propagation(_trivial_propagations[i]);
      _just_backjumped = false;
    }

  process_assertions();
  if(!_solver.is_conflict() && (_solver.all_literals_assigned() || (layer >= 1 && _check_count >= _check_period)))
    {
      do {
	
	expression bu;
	if(!check(bu))
	  {
	    explanation expl;
	    generate_conflict_explanation(bu, expl);
	    _solver.apply_conflict(expl, this);
	  }
	else
	  {
	    assert(check_invariant_nb2());
	    assert(check_invariant_b2());
	    invalidate_inactive_unknowns();
	    if(check_integer_unknowns())
	      {
		if(_eq_intro)
		  introduce_equalities();
		check_equalities();
		process_assertions();
	      }
	    if(++_bound_refinements_count >= _bound_ref_period)
	      {
		propagate_bound_refinements();
		_bound_refinements_count = 0;
	      }	   
	  }
      
	assert(check_invariant_nb3());
	assert(check_invariant_b3());
	assert(check_invariant_nb4());
	assert(check_invariant_b4());
	assert(check_invariant_nb5());
	assert(check_invariant_b5());
      
	_check_count = 0;
	_bound_changed = false;
	_disequality_asserted = false;
	process_assertions();
      } while(_bound_changed || _disequality_asserted);

      /* For tableau statistics */
      //_check_and_prop_time_spent.acumulate();
      //get_tableau_statistics();
      //tableau_report(std::cout);
      //_check_and_prop_time_spent.start();
      /* Comment if it is not needed (expensive) */
    }
  assert(_solver.is_conflict() || _current_assertion_pos == _solver.get_trail().size());
  _check_and_prop_time_spent.acumulate();
}

expression arithmetic_theory_solver::canonize_expression(const expression & e)
{
  return arithmetic_canonizer<unknown_value>::canonize_expression(e);
}

void arithmetic_theory_solver::get_literal_pair(const expression & l, expression & l_pos, expression & l_neg)
{
  arithmetic_canonizer<unknown_value>::get_literal_pair(l, l_pos, l_neg);
}

void arithmetic_theory_solver::get_model(const expression_vector & exps)
{
  calculate_delta(); 
  for(unsigned i = 0; i < exps.size(); i++)
    {
      if(!_data.has_data(exps[i]) || !_data.get_data(exps[i])->has_unknown_data())
	continue;

      //std::cout << "VALUE: " << exps[i] << " : " << get_valuation(exps[i]) << std::endl;

      exps[i]->set_assigned(get_expr_from_value(get_valuation(exps[i])));
    }
}

void arithmetic_theory_solver::found_trivial_propagation(const unknown_value & c, const function_symbol & fs, 
						  expression l_pos, expression l_neg)
{
  
  if((fs == function_symbol::LE && c < 0) ||
     (fs == function_symbol::GE && c > 0) ||
     (fs == function_symbol::EQ && c != 0))
    {
      l_pos.swap(l_neg);
    }
  _trivial_propagations.push_back(l_pos);
  _new_trivial_propagations.push_back(l_pos);
}

void arithmetic_theory_solver::apply_trivial_propagation(const expression & l)
{
  const extended_boolean l_value = _solver.get_trail().get_value(l);
  if(l_value == EB_UNDEFINED)
    _solver.apply_propagate(l, this);
  else if(l_value == EB_FALSE)
    {
      explanation conflicting;
      conflicting.push_back(_solver.get_literal_data(l)->get_opposite());
      _solver.apply_conflict(conflicting, this);
    }  
}

void arithmetic_theory_solver::pivot(const expression & bu, const expression & nbu)
{
  linear_polynomial & bu_lp = _data.get_data(bu)->row();
  linear_polynomial & nbu_lp = _data.get_data(nbu)->row();
  static unknown_value aij;
  aij = bu_lp.get_coeff(nbu);

  //std::cerr << "PIVOTING: " << bu << " and " << nbu << std::endl;

  nbu_lp.swap(bu_lp);
  bu_lp.clear();
  bu_lp.set_coeff(bu, 1);
  nbu_lp.reset_coeff(nbu);
  nbu_lp.set_coeff(bu, -1);
  nbu_lp.scale_polynomial(-1/aij);

  _basic_unknowns.erase(bu);
  _non_basic_unknowns.erase(nbu);

  _parallel_time_spent.start();
  static unknown_value old_akj;
  for(unknown_set::const_iterator it = _basic_unknowns.begin(), it_end = _basic_unknowns.end(); it != it_end; ++it)
    {
      linear_polynomial & c_lp = _data.get_data(*it)->row();
      const unknown_value & akj = c_lp.get_coeff(nbu);
      if(akj != 0)
	{
	  old_akj = akj; // remember old akj
	  c_lp.reset_coeff(nbu);
	  c_lp.add_polynomial(nbu_lp, old_akj);
	}      
    }
  _parallel_time_spent.acumulate();

  _data.get_data(nbu)->set_basic(true);
  if(!_data.get_data(nbu)->get_constraints().empty())
    {
      _basic_unknowns.insert(nbu);
    }
  else
    _inactive_unknowns.insert(std::make_pair(nbu, false));
  _data.get_data(bu)->set_basic(false);
  _non_basic_unknowns.insert(bu);
}

void arithmetic_theory_solver::update(const expression & nbu, const value_pair & v)
{
  arithmetic_theory_solver_data * nbu_data = _data.get_data(nbu);
  const value_pair & nbu_val = nbu_data->get_valuation();


  _parallel_time_spent.start();  
  static value_pair temp; // for calculating aji * (v - nbu_val)
  for(unknown_set::const_iterator it = _basic_unknowns.begin(), it_end = _basic_unknowns.end(); it != it_end; ++it)
    {
      arithmetic_theory_solver_data * bu_data = _data.get_data(*it);
      const unknown_value & aji = bu_data->row().get_coeff(nbu);
      if(aji == 0) continue; 
      temp = v;
      temp -= nbu_val;
      temp *= aji;
      bu_data->add_to_valuation(temp);
    }
  _parallel_time_spent.acumulate();
  nbu_data->set_valuation(v);
  assert(check_invariant_nb3());
  assert(check_invariant_b3());
  assert(check_invariant_nb4());
  assert(check_invariant_b4());
  assert(check_invariant_nb5());
  assert(check_invariant_b5());
}

void arithmetic_theory_solver::pivot_and_update(const expression & bu, const expression & nbu, const value_pair & v)
{
  _pivot_and_update_time_spent.start();
  
  arithmetic_theory_solver_data * bu_data = _data.get_data(bu);
  const value_pair & bu_val = bu_data->get_valuation();
  const unknown_value & aij = bu_data->row().get_coeff(nbu);
  arithmetic_theory_solver_data * nbu_data = _data.get_data(nbu);
  
  
  static value_pair theta;   // theta = (v - bu_val) / aij;
  theta = v;
  theta -= bu_val;
  theta /= aij;
  bu_data->set_valuation(v);
  nbu_data->add_to_valuation(theta);
  
  
  _parallel_time_spent.start();
  static value_pair temp;  // temp = akj * theta;
  for(unknown_set::const_iterator it = _basic_unknowns.begin(), it_end = _basic_unknowns.end(); it != it_end; ++it)
    {
      if(*it == bu) continue;      
      arithmetic_theory_solver_data * c_data = _data.get_data(*it);      
      const unknown_value & akj = c_data->row().get_coeff(nbu);
      if(akj == 0) continue; 
      temp = theta;
      temp *= akj;
      c_data->add_to_valuation(temp);
    }
  _parallel_time_spent.acumulate();
  pivot(bu, nbu);
  
  _pivot_and_update_time_spent.acumulate();
  
  assert(check_invariant_nb4());
  assert(check_invariant_b4());
  assert(check_invariant_nb3());
  assert(check_invariant_b3());
}

bool arithmetic_theory_solver::find_first_non_basic_unknown(const linear_polynomial & lp, bool lb_violated, expression & first)
{
  unsigned first_index = (unsigned)(-1);

  for(linear_polynomial::const_iterator it = lp.begin(), it_end = lp.end(); it != it_end; ++it)
    {
      if(it->first == _one_expression)
	continue;

      arithmetic_theory_solver_data * curr_data = _data.get_data(it->first);
     
      if(curr_data->get_index() < first_index)  
	{
	  if((((lb_violated && it->second > 0) || (!lb_violated && it->second < 0)) && curr_data->get_valuation() < curr_data->get_upper_bound()) ||
	     (((lb_violated && it->second < 0) || (!lb_violated && it->second > 0)) && curr_data->get_valuation() > curr_data->get_lower_bound()))
	    {
	      first = it->first;
	      first_index = curr_data->get_index();
	    }
	}
    }
  return first_index < (unsigned)(-1);
}

bool arithmetic_theory_solver::find_first_basic_unknown(expression & first)
{
  for(unknown_set::const_iterator it = _basic_unknowns.begin(), it_end = _basic_unknowns.end(); it != it_end; ++it)
    {
      arithmetic_theory_solver_data * curr_data = _data.get_data(*it);
      //      std::cout << "Search: " << *it << " : " << curr_data->get_lower_bound() << " < " << curr_data->get_valuation() << " < " << curr_data->get_upper_bound() << std::endl;
      
      if(curr_data->get_valuation() < curr_data->get_lower_bound() || curr_data->get_valuation() > curr_data->get_upper_bound())
	{	  
	  first = *it;
	  return true;
	}
    }
  return false;
}

bool arithmetic_theory_solver::check(expression & bu)
{
  if(!_bound_changed)
    return true;

  _check_time_spent.start();
  bool retval = true;

  _bu_selector->enter_check();
  _nbu_selector->enter_check();
  _check_call_count++;
  expression nbu;
  while(_bu_selector->select_basic_unknown(bu))
    {
      arithmetic_theory_solver_data * bu_data = _data.get_data(bu);
      _pivot_call_count++;
      if(bu_data->get_valuation() < bu_data->get_lower_bound())
	{
	  if(_nbu_selector->select_non_basic_unknown(bu_data->row(), true, nbu))
	    pivot_and_update(bu, nbu, bu_data->get_lower_bound());
	  else
	    {
	      retval = false;
	      break;
	    }
	}
      else if(bu_data->get_valuation() > bu_data->get_upper_bound())
	{
	  if(_nbu_selector->select_non_basic_unknown(bu_data->row(), false, nbu))
	    pivot_and_update(bu, nbu, bu_data->get_upper_bound());
	  else
	    {
	      retval = false;
	      break;
	    }

	}
    }
  _check_time_spent.acumulate();
  return retval;
}

void arithmetic_theory_solver::calculate_bounds(const expression & bu, bounds_map & bmap)
{
  assert(_data.has_data(bu));
  assert(_data.get_data(bu)->has_unknown_data());
  assert(_data.get_data(bu)->is_basic());

  arithmetic_theory_solver_data * bu_data = _data.get_data(bu);
  linear_polynomial & bu_lp = bu_data->row();
  static value_pair lower;
  static value_pair upper;
  expression upp_hole;
  expression low_hole;

  lower.first = 0;
  lower.second = 0;
  lower.infinity = I_NONE;
  upper.first = 0;
  upper.second = 0;
  upper.infinity = I_NONE;
  
  // First, we calculate upper and lower bound for the expression a1x1 + ... + anxn - y. If there
  // is a hole unknown, we remember it. It there are more than one holes, then the bound is infinite.
  for(linear_polynomial::const_iterator it = bu_lp.begin(), it_end = bu_lp.end(); it != it_end; ++it)
    {
      if(it->first == _one_expression)
	{
	  if(upper.infinity == I_NONE || upp_hole.get() != 0)
	    upper.first += it->second;
	  if(lower.infinity == I_NONE || low_hole.get() != 0)
	    lower.first += it->second;
	  continue;
	}

      arithmetic_theory_solver_data * curr_data = _data.get_data(it->first);

      if(it->second > 0)
	{ 
	  if(upper.infinity == I_NONE || upp_hole.get() != 0)
	    {
	      if(curr_data->get_upper_bound().infinity == I_NONE)
		upper.add_product(curr_data->get_upper_bound(), it->second);
	      else if(upp_hole.get() == 0)
		{
		  upper.infinity = I_POSITIVE;
		  upp_hole = it->first;
		}
	      else
		upp_hole = expression();
	    }
	  
	  if(lower.infinity == I_NONE || low_hole.get() != 0)
	    {
	      if(curr_data->get_lower_bound().infinity == I_NONE)
		lower.add_product(curr_data->get_lower_bound(), it->second);
	      else if(low_hole.get() == 0)
		{
		  lower.infinity = I_NEGATIVE;
		  low_hole = it->first;
		}
	      else
		low_hole = expression();
	    }
	}
      else
	{
	  if(upper.infinity == I_NONE || upp_hole.get() != 0)
	    {
	      if(curr_data->get_lower_bound().infinity == I_NONE)
		upper.add_product(curr_data->get_lower_bound(), it->second);
	      else if(upp_hole.get() == 0)
		{
		  upper.infinity = I_POSITIVE;
		  upp_hole = it->first;
		}
	      else
		upp_hole = expression();
	    }

	  if(lower.infinity == I_NONE || low_hole.get() != 0)
	    {
	      if(curr_data->get_upper_bound().infinity == I_NONE)
		lower.add_product(curr_data->get_upper_bound(), it->second);
	      else if(low_hole.get() == 0)
		{
		  lower.infinity = I_NEGATIVE;
		  low_hole = it->first;
		}
	      else
		low_hole = expression();
	    }
	}
    }

  if(upper.infinity == I_NONE || upp_hole.get() != 0)
    {
      if(bu_data->get_lower_bound().infinity == I_NONE)
	upper -= bu_data->get_lower_bound();
      else if(upp_hole.get() == 0)
	{
	  upper.infinity = I_POSITIVE;
	  upp_hole = bu;
	}
      else
	upp_hole = expression();
    }

  if(lower.infinity == I_NONE || low_hole.get() != 0)
    {
      if(bu_data->get_upper_bound().infinity == I_NONE)
	lower -= bu_data->get_upper_bound();
      else if(low_hole.get() == 0)
	{
	  lower.infinity = I_NEGATIVE;
	  low_hole = bu;
	}
      else
	low_hole = expression();
    }

  static value_pair nbu_lower;
  static value_pair nbu_upper;
  static value_pair temp;      
  
  
  // We adjust the bounds for non-basic unknowns
  if(lower.infinity == I_NONE) // all non-basic
    {
      for(linear_polynomial::const_iterator it = bu_lp.begin(), it_end = bu_lp.end(); it != it_end; ++it)
	{
	  const expression & nbu = it->first;
	  const unknown_value & coeff = it->second;
	  arithmetic_theory_solver_data * nbu_data = _data.get_data(nbu);
	  unknown_bounds & nbu_bounds = bmap[nbu];
	  
	  if(coeff > 0)
	    {
	      assert(nbu_data->get_lower_bound().infinity == I_NONE);
	      nbu_upper = nbu_data->get_lower_bound();
	      temp = lower;
	      temp /= coeff;
	      nbu_upper -= temp;
	      if(nbu_data->is_integer())
		tighten_integer_bound(nbu_upper, B_UPPER);
	      if(nbu_upper_comp(this)(nbu_upper, bu,
				      nbu_bounds.upper, nbu_bounds.upper_bu))
		{
		  nbu_bounds.upper = std::move(nbu_upper);
		  nbu_bounds.upper_bu = bu;
		}
	    }	
	  else
	    {
	      assert(nbu_data->get_upper_bound().infinity == I_NONE);
	      nbu_lower = nbu_data->get_upper_bound();
	      temp = lower;
	      temp /= coeff;
	      nbu_lower -= temp;
	      if(nbu_data->is_integer())
		tighten_integer_bound(nbu_lower, B_LOWER);
	      if(nbu_lower_comp(this)(nbu_lower, bu,
				      nbu_bounds.lower, nbu_bounds.lower_bu))
		{		  
		  nbu_bounds.lower = std::move(nbu_lower);
		  nbu_bounds.lower_bu = bu;
		}	      
	    }	  
	}
    }
  else if(low_hole.get() != 0 && low_hole != bu) // only hole
    {
      const expression & nbu = low_hole; 
      const unknown_value & coeff = bu_lp.get_coeff(nbu);
      arithmetic_theory_solver_data * nbu_data = _data.get_data(nbu);
      unknown_bounds & nbu_bounds = bmap[nbu];

      if(coeff > 0)
	{
	  nbu_upper = lower;
	  nbu_upper.infinity = I_NONE;
	  nbu_upper /= coeff;
	  nbu_upper *= -1;
	  if(nbu_data->is_integer())
	    tighten_integer_bound(nbu_upper, B_UPPER);
	  if(nbu_upper_comp(this)(nbu_upper, bu,
				  nbu_bounds.upper, nbu_bounds.upper_bu))
	    {
	      nbu_bounds.upper = std::move(nbu_upper);
	      nbu_bounds.upper_bu = bu;
	    }
	}
      else
	{
	  nbu_lower = lower;
	  nbu_lower.infinity = I_NONE;
	  nbu_lower /= coeff;
	  nbu_lower *= -1;
	  if(nbu_data->is_integer())
	    tighten_integer_bound(nbu_lower, B_LOWER);

	  if(nbu_lower_comp(this)(nbu_lower, bu,
				  nbu_bounds.lower, nbu_bounds.lower_bu))
	    {
	      nbu_bounds.lower = std::move(nbu_lower);
	      nbu_bounds.lower_bu = bu;
	    }	      	  
	}
    }

  if(upper.infinity == I_NONE) // all non-basic
    {
      for(linear_polynomial::const_iterator it = bu_lp.begin(), it_end = bu_lp.end(); it != it_end; ++it)
	{
	  const expression & nbu = it->first;
	  const unknown_value & coeff = it->second;
	  arithmetic_theory_solver_data * nbu_data = _data.get_data(nbu);
	  unknown_bounds & nbu_bounds = bmap[nbu];
	  
	  if(coeff > 0)
	    {
	      assert(nbu_data->get_upper_bound().infinity == I_NONE);
	      nbu_lower = nbu_data->get_upper_bound();
	      temp = upper;
	      temp /= coeff;
	      nbu_lower -= temp;
	      if(nbu_data->is_integer())
		tighten_integer_bound(nbu_lower, B_LOWER);
	      if(nbu_lower_comp(this)(nbu_lower, bu,
				      nbu_bounds.lower, nbu_bounds.lower_bu))
		{
		  nbu_bounds.lower = std::move(nbu_lower);
		  nbu_bounds.lower_bu = bu;
		}	      
	    }	
	  else
	    {
	      assert(nbu_data->get_lower_bound().infinity == I_NONE);
	      nbu_upper = nbu_data->get_lower_bound();
	      temp = upper;
	      temp /= coeff;
	      nbu_upper -= temp;
	      if(nbu_data->is_integer())
		tighten_integer_bound(nbu_upper, B_UPPER);
	      if(nbu_upper_comp(this)(nbu_upper, bu,
				      nbu_bounds.upper, nbu_bounds.upper_bu))
		{
		  nbu_bounds.upper = std::move(nbu_upper);
		  nbu_bounds.upper_bu = bu;
		}
	    }	  
	}      
    }
  else if(upp_hole.get() != 0 && upp_hole != bu)
    {
      const expression & nbu = upp_hole; 
      const unknown_value & coeff = bu_lp.get_coeff(nbu);
      arithmetic_theory_solver_data * nbu_data = _data.get_data(nbu);
      unknown_bounds & nbu_bounds = bmap[nbu];
      
      if(coeff > 0)
	{
	  nbu_lower = upper;
	  nbu_lower.infinity = I_NONE;
	  nbu_lower /= coeff;
	  nbu_lower *= -1;
	  if(nbu_data->is_integer())
	    tighten_integer_bound(nbu_lower, B_LOWER);
	  if(nbu_lower_comp(this)(nbu_lower, bu,
				  nbu_bounds.lower, nbu_bounds.lower_bu))
	    {
	      nbu_bounds.lower = std::move(nbu_lower);
	      nbu_bounds.lower_bu = bu;
	    }	      	  
	}
      else
	{
	  nbu_upper = upper;
	  nbu_upper.infinity = I_NONE;
	  nbu_upper /= coeff;
	  nbu_upper *= -1;
	  if(nbu_data->is_integer())
	    tighten_integer_bound(nbu_upper, B_UPPER);
	  if(nbu_upper_comp(this)(nbu_upper, bu,
				  nbu_bounds.upper, nbu_bounds.upper_bu))
	    {
	      nbu_bounds.upper = std::move(nbu_upper);
	      nbu_bounds.upper_bu = bu;
	    }	  
	}      
    }
  
  // After that, we calculate the bounds for the basic unknown bu
  value_pair & bu_lower = bu_data->calculated_lower_bound();
  bu_lower = std::move(lower);
  if(bu_lower.infinity == I_NONE)
    bu_lower += bu_data->get_upper_bound(); 
  else if(low_hole == bu)
    bu_lower.infinity = I_NONE;

  value_pair & bu_upper = bu_data->calculated_upper_bound();
  bu_upper = std::move(upper);
  if(bu_upper.infinity == I_NONE)
    bu_upper += bu_data->get_lower_bound(); 
  else if(upp_hole == bu)
    bu_upper.infinity = I_NONE;

  if(bu_data->is_integer())
    {
      tighten_integer_bound(bu_upper, B_UPPER);
      tighten_integer_bound(bu_lower, B_LOWER);
    }
}

void arithmetic_theory_solver::propagate_bound_refinements()
{
  if(!_bound_changed)
    return;

  _bound_refinments_time_spent.start();
  
  //  std::cout << "STARTING BOUND REFINEMENTS"<<std::endl;
  static constraint lo_cst;
  static constraint up_cst;
  static bounds_map bmap;
  bmap.clear();

  for(unknown_set::const_iterator it = _basic_unknowns.begin(), it_end = _basic_unknowns.end(); it != it_end; ++it)
    {
      const expression & bu = *it;
      arithmetic_theory_solver_data * bu_data = _data.get_data(bu);
      _parallel_time_spent.start();
      calculate_bounds(bu, bmap);
      _parallel_time_spent.acumulate();
      
      lo_cst.bound = std::move(bu_data->calculated_lower_bound());
      lo_cst.type = B_LOWER;
      
      up_cst.bound = std::move(bu_data->calculated_upper_bound());
      up_cst.type = B_UPPER;
      
      if(lo_cst.bound == up_cst.bound)
	{
	  lo_cst.type = B_BOTH;
	  propagate_weaker_constraints(bu, lo_cst);
	}
      else
	{
	  if(up_cst.bound.infinity == I_NONE)
	    propagate_weaker_constraints(bu, up_cst);
	  if(lo_cst.bound.infinity == I_NONE)
	    propagate_weaker_constraints(bu, lo_cst);
	}
    }  

  for(unknown_set::const_iterator it = _non_basic_unknowns.begin(), it_end = _non_basic_unknowns.end(); it != it_end; ++it)
    {
      const expression & nbu = *it;
      arithmetic_theory_solver_data * nbu_data = _data.get_data(nbu);
      if(bmap.find(nbu) == bmap.end())
	continue;
      
      unknown_bounds & nbu_bounds = bmap[nbu];

      lo_cst.bound = std::move(nbu_bounds.lower);
      lo_cst.type = B_LOWER;
      nbu_data->lower_bu() = nbu_bounds.lower_bu;
      assert(lo_cst.bound.infinity != I_NONE || nbu_data->lower_bu().get() != 0);
      
      up_cst.bound = std::move(nbu_bounds.upper);
      up_cst.type = B_UPPER;
      nbu_data->upper_bu() = nbu_bounds.upper_bu;
      assert(up_cst.bound.infinity != I_NONE || nbu_data->upper_bu().get() != 0);
	         
      if(lo_cst.bound == up_cst.bound)
	{
	  lo_cst.type = B_BOTH;
	  assert(lo_cst.bound.infinity == I_NONE);
	  propagate_weaker_constraints(nbu, lo_cst);
	}
      else
	{
	  if(up_cst.bound.infinity == I_NONE)
	    propagate_weaker_constraints(nbu, up_cst);
	  if(lo_cst.bound.infinity == I_NONE)
	    propagate_weaker_constraints(nbu, lo_cst);
	}      
    }      
  //  std::cout << "ENDING BOUND REFIMENENTS" << std::endl;
  _bound_refinments_time_spent.acumulate();
  
}

void arithmetic_theory_solver::propagate_weaker_constraints(const expression & unknown, const constraint & c)
{
  _weaker_time_spent.start();  
  _data.get_data(unknown)->sort_constraints(this);
  const expression_vector & csts = _data.get_data(unknown)->get_constraints();
  //std::cout << "WEAKER: " << unknown << " : " << csts.size() << std::endl;
  //std::cout << "CONSTRAINT: " << c.bound << " : " << c.type << std::endl;
  int low = 0, up = csts.size() - 1;

  // Optimization: binary search eliminates constraints that are certainly
  // not implied (since the vector of constraints is sorted)
  if(c.type == B_LOWER || c.type == B_SPLIT)
    up = find_constraint_r(csts, c);
  if(c.type == B_UPPER || c.type == B_SPLIT)
    low = find_constraint_l(csts, c);
  
  for(int i = low; i <= up; i++)
    {
      const extended_boolean cstsi_value = _solver.get_trail().get_value(csts[i]);
      if(cstsi_value == EB_TRUE)
	continue;
      
      arithmetic_theory_solver_data * curr_data = _data.get_data(csts[i]);
      assert(curr_data->has_constraint_data());
      const constraint & curr_cst = curr_data->get_constraint();
      
      if(c.implies(curr_cst))
	{
	  explanation expl; 
	  expl.reserve(10);
	  
	  if(cstsi_value == EB_UNDEFINED)	  
	    {
	      //std::cout << "PROP: " << csts[i] << " (" << c.bound << ":" << c.type << ")" << std::endl;	      
	      generate_propagation_explanation(csts[i], c, expl);
	      _solver.apply_propagate(csts[i], this);
	      _data.get_data(csts[i])->literal_explanation() = expl;
	    }
	  else if(cstsi_value == EB_FALSE)
	    {
	      //	      std::cout << "CONS: " << csts[i] << std::endl;
	      generate_propagation_explanation(csts[i], c, expl);
	      expl.push_back(_solver.get_literal_data(csts[i])->get_opposite());
	      _solver.apply_conflict(expl, this);
	      break;
	    }
	}
      //   else
      //	std::cout << "NOT WEAKER: " << csts[i] << std::endl;
    }
  //std::cout << "END WEAKER" << std::endl;
  _weaker_time_spent.acumulate();
}



void arithmetic_theory_solver::introduce_equalities()
{
  if(_eq_relevant_unknowns.size() < 2)
    return;

  //  std::cerr << "ENTER" << std::endl;
  
  //  std::cout << "Eq unknowns: " << _eq_relevant_unknowns.size() << std::endl;
  // std::cout << "All unknowns: " << _all_unknowns.size() << std::endl;
  std::for_each(_eq_relevant_unknowns.begin(), _eq_relevant_unknowns.end(),
		[this] (const expression & unknown)
		{
		  recalculate_unknown_value(unknown);
		});
  std::sort(_eq_relevant_unknowns.begin(), _eq_relevant_unknowns.end(),
	    [this] (const expression & e1, const expression & e2)
	    {
	      arithmetic_theory_solver_data * e1_data = _data.get_data(e1);
	      arithmetic_theory_solver_data * e2_data = _data.get_data(e2);	      
	      return e1_data->get_valuation() < e2_data->get_valuation() ||
		(e1_data->get_valuation() == e2_data->get_valuation() && 
		 e1_data->get_index() < e2_data->get_index());	      
	    });
  //std::cout << "END SORTING" << std::endl;
  for(unsigned i = 0; i < _eq_relevant_unknowns.size() - 1; i++)
    {
      for(unsigned j = i + 1; j < _eq_relevant_unknowns.size(); j++)
	{
	  if(_data.get_data(_eq_relevant_unknowns[i])->get_valuation() == _data.get_data(_eq_relevant_unknowns[j])->get_valuation())
	    {
	      expression eq;	      
	      if(_solver.apply_introduce_shared_equality(_eq_relevant_unknowns[i], _eq_relevant_unknowns[j], eq, 0, 0, true, 1, false))
		{		 
		  /*if(_solver.get_trail().is_undefined(eq))
		    {		      
		      _solver.get_literal_selection_strategy()->branching_priority_hint(eq, 0, 0, true);
		      _solver.get_polarity_selection_strategy()->preffered_polarity_hint(eq, 1, false);
		      }*/		  
		}
	    }
	  else
	    break;
	}
    }
  //  std::cerr << "LEAVE" << std::endl;
}

// binary search (returns the index of the last element in csts that is lower or equal
// than cst, or -1 if no such element exists). 
int arithmetic_theory_solver::find_constraint_r(const expression_vector & csts, const constraint & cst)
{

  int l = 0, r = (int)csts.size();
  int s;
  while(l < r)
    {
      s = (l + r) >> 1;
      const constraint & s_cst = _data.get_data(csts[s])->get_constraint();
      if(cst < s_cst)
	r = s;
      else 
	l = s + 1;
    }
  return l - 1;
}

// binary search (returns the index of the first element in
// csts that is not lower than cst, or csts.size() if no such element exists). 
int arithmetic_theory_solver::find_constraint_l(const expression_vector & csts, const constraint & cst)
{
  int l = 0, r = (int)csts.size();
  int s;
  while(l < r)
    {
      s = (l + r) >> 1;
      const constraint & s_cst = _data.get_data(csts[s])->get_constraint();
      if(s_cst < cst)
	l = s + 1;
      else 
	r = s;
    }
  return l;
}


void arithmetic_theory_solver::check_equalities()
{
  if(!_bound_changed && !_disequality_asserted)
    return;

  _check_eq_time_spent.start();

  static constraint cst;
  
  for(unsigned i = 0; i < _all_unknowns.size(); i++)
    {
      arithmetic_theory_solver_data * u_data = _data.get_data(_all_unknowns[i]);
      u_data->sort_constraints(this);
      const expression_vector & u_csts = u_data->get_constraints();

      
      // Skip unconstrained unknowns
      if(u_csts.empty())
	continue;
      
      cst.bound = u_data->get_valuation();

      if(!is_fixed_by_constraints(_all_unknowns[i]) && !is_poly_fixed_by_constraints(_all_unknowns[i]))
	{
	  cst.type = B_SPLIT;
	  int j = find_constraint_r(u_csts, cst);
	  while(j >= 0 && _data.get_data(u_csts[j])->get_constraint() == cst)
	    {
	      if(_solver.get_trail().get_value(u_csts[j]) == EB_TRUE)
		{
		  expression diseq = u_csts[j];
		  split_on_demand(diseq);
		}
	      j--;
	    }
	}
      else
	{
	  cst.type = B_BOTH;
	  int j = find_constraint_r(u_csts, cst);
	  while(j >= 0 && _data.get_data(u_csts[j])->get_constraint() == cst)
	    {
	      extended_boolean eb = _solver.get_trail().get_value(u_csts[j]);
	      explanation expl;
	      if(eb == EB_UNDEFINED)
		{
		  generate_propagation_explanation(u_csts[j], cst, expl);
		  _solver.apply_propagate(u_csts[j], this);
		  //std::cout << "PROPAGATING: " << u_csts[j] << std::endl;
		  _data.get_data(u_csts[j])->literal_explanation() = expl;
		}
	      else if(eb == EB_FALSE)
		{
		  generate_propagation_explanation(u_csts[j], cst, expl);
		  expl.push_back(_solver.get_literal_data(u_csts[j])->get_opposite());
		  _solver.apply_conflict(expl, this);
		}
	      j--;
	    }	  
	}
      
     
      if(_solver.is_conflict() || !is_poly_fixed_by_constraints(_all_unknowns[i]))
	{
	  continue;
	}

    }

  _check_eq_time_spent.acumulate();
  //  std::cout << "END CHECK EQUALITIES"  << std::endl;
}

struct duplicate_elimination_wrapper {
  explanation * expl;
  expression_set set;

  duplicate_elimination_wrapper()
    :expl(0),
     set(HASH_TABLE_SIZE)
  {}

  void reset(explanation & e)
  {
    set.clear();
    expl = &e;
  }
  
  void push_back(const expression & e)
  {
    if(set.find(e) == set.end())
      {
	expl->push_back(e);
	set.insert(e);
      }
  }
};

void arithmetic_theory_solver::generate_bound_explanation(const expression & unknown, bound_type bt, explanation & expl)
{
  arithmetic_theory_solver_data * u_data = _data.get_data(unknown);
  static duplicate_elimination_wrapper wrapper;
  wrapper.reset(expl);
  assert(bt != B_SPLIT);
  
  if(u_data->is_basic())
    {
      const linear_polynomial & u_lp = u_data->row();
      
      for(linear_polynomial::const_iterator it = u_lp.begin(), it_end = u_lp.end(); it != it_end; ++it)
	{
	  assert(it->first != _one_expression);
	  arithmetic_theory_solver_data * curr_data = _data.get_data(it->first);
	  
	  if(bt == B_UPPER || bt == B_BOTH)
	    {
	      if(it->second > 0)
		{
		  assert(curr_data->get_upper_bound().infinity == I_NONE);
		  wrapper.push_back(curr_data->get_upper_bound_relation());
		}
	      else
		{
		  assert(curr_data->get_lower_bound().infinity == I_NONE);
		  wrapper.push_back(curr_data->get_lower_bound_relation());
		}
	    }
	  
	  if(bt == B_LOWER || bt == B_BOTH)
	    {
	      if(it->second < 0)
		{
		  assert(curr_data->get_upper_bound().infinity == I_NONE);
		  wrapper.push_back(curr_data->get_upper_bound_relation());
		}
	      else
		{
		  assert(curr_data->get_lower_bound().infinity == I_NONE);
		  wrapper.push_back(curr_data->get_lower_bound_relation());
		}
	    }
	}
    }
  else
    {
      if(bt == B_UPPER || bt == B_BOTH)
	{
	  const expression & upper_bu = u_data->upper_bu();
	  arithmetic_theory_solver_data * upper_bu_data = _data.get_data(upper_bu);
	  const linear_polynomial & upper_lp = upper_bu_data->row();
	  const unknown_value & upper_coeff = upper_lp.get_coeff(unknown);
	  
	  if(upper_coeff > 0)
	    {
	      for(linear_polynomial::const_iterator it = upper_lp.begin(), it_end = upper_lp.end(); it != it_end; ++it)
		{
		  assert(it->first != _one_expression);
		  
		  if(it->first == unknown)
		    continue;

		  arithmetic_theory_solver_data * curr_data = _data.get_data(it->first);
		  if(it->second > 0)
		    {
		      assert(curr_data->get_lower_bound().infinity == I_NONE);
		      wrapper.push_back(curr_data->get_lower_bound_relation());
		    }
		  else
		    {
		      assert(curr_data->get_upper_bound().infinity == I_NONE);
		      wrapper.push_back(curr_data->get_upper_bound_relation());
		    }
		}
	      wrapper.push_back(upper_bu_data->get_upper_bound_relation());
	    }
	  else
	    {
	      for(linear_polynomial::const_iterator it = upper_lp.begin(), it_end = upper_lp.end(); it != it_end; ++it)
		{
		  assert(it->first != _one_expression);
		  
		  if(it->first == unknown)
		    continue;
		  
		  arithmetic_theory_solver_data * curr_data = _data.get_data(it->first);
		  if(it->second > 0)
		    {
		      assert(curr_data->get_upper_bound().infinity == I_NONE);
		      wrapper.push_back(curr_data->get_upper_bound_relation());
		    }
		  else
		    {
		      assert(curr_data->get_lower_bound().infinity == I_NONE);
		      wrapper.push_back(curr_data->get_lower_bound_relation());
		    }
		}
	      wrapper.push_back(upper_bu_data->get_lower_bound_relation());
	    }				      
	}

      if(bt == B_LOWER || bt == B_BOTH)
	{
	  const expression & lower_bu = u_data->lower_bu();
	  arithmetic_theory_solver_data * lower_bu_data = _data.get_data(lower_bu);
	  const linear_polynomial & lower_lp = lower_bu_data->row();
	  const unknown_value & lower_coeff = lower_lp.get_coeff(unknown);
	  
	  if(lower_coeff > 0)
	    {
	      for(linear_polynomial::const_iterator it = lower_lp.begin(), it_end = lower_lp.end(); it != it_end; ++it)
		{
		  assert(it->first != _one_expression);
		  
		  if(it->first == unknown)
		    continue;
		  
		  arithmetic_theory_solver_data * curr_data = _data.get_data(it->first);
		  if(it->second > 0)
		    {
		      assert(curr_data->get_upper_bound().infinity == I_NONE);
		      wrapper.push_back(curr_data->get_upper_bound_relation());
		    }
		  else
		    {
		      assert(curr_data->get_lower_bound().infinity == I_NONE);
		      wrapper.push_back(curr_data->get_lower_bound_relation());
		    }
		}
	      assert(lower_bu_data->get_lower_bound().infinity == I_NONE);
	      wrapper.push_back(lower_bu_data->get_lower_bound_relation());
	    }
	  else
	    {
	      for(linear_polynomial::const_iterator it = lower_lp.begin(), it_end = lower_lp.end(); it != it_end; ++it)
		{
		  assert(it->first != _one_expression);
		  
		  if(it->first == unknown)
		    continue;
		  
		  arithmetic_theory_solver_data * curr_data = _data.get_data(it->first);
		  if(it->second > 0)
		    {
		      assert(curr_data->get_lower_bound().infinity == I_NONE);
		      wrapper.push_back(curr_data->get_lower_bound_relation());
		    }
		  else
		    {
		      assert(curr_data->get_upper_bound().infinity == I_NONE);
		      wrapper.push_back(curr_data->get_upper_bound_relation());
		    }
		}
	      assert(lower_bu_data->get_upper_bound().infinity == I_NONE);
	      wrapper.push_back(lower_bu_data->get_upper_bound_relation());
	    }				      	  
	}
    }
}

bool arithmetic_theory_solver::generate_simple_explanation(const expression & e, explanation & expl)
{
  arithmetic_theory_solver_data * e_data = _data.get_data(e);
  const constraint & c = e_data->get_constraint();
  const expression & unknown = e_data->get_unknown();
  arithmetic_theory_solver_data * u_data = _data.get_data(unknown);
  assert(_solver.get_trail().get_value(e) != EB_TRUE);
  
  if(u_data->get_lower_bound().infinity == I_NONE)
    {
      const expression & lower_rel = u_data->get_lower_bound_relation();
      const constraint & c_lower = _data.get_data(lower_rel)->get_constraint();
      if(c.implied_by(c_lower))
	{
	  expl.push_back(lower_rel);
	  //	  std::cout << "SIMPLE EXPLANATION FOR: " << e << " : "<< lower_rel << std::endl;
	  return true;
	}
    }

  if(u_data->get_upper_bound().infinity == I_NONE)
    {
      const expression & upper_rel = u_data->get_upper_bound_relation();
      const constraint & c_upper = _data.get_data(upper_rel)->get_constraint();
      if(c.implied_by(c_upper))
	{
	  expl.push_back(upper_rel);
	  //	  std::cout << "SIMPLE EXPLANATION FOR: " << e << " : "<< upper_rel << std::endl;
	  return true;
	}
    }
  

  // special case x == c is implied by x <= c /\ x >= c 
  if(c.type == B_BOTH && u_data->get_lower_bound() == c.bound && u_data->get_upper_bound() == c.bound)
    {
      const expression & lower_rel = u_data->get_lower_bound_relation();
      const expression & upper_rel = u_data->get_upper_bound_relation();
      
      if((lower_rel->get_symbol() == function_symbol::GE || lower_rel->get_symbol() == function_symbol::GT) &&
	 (upper_rel->get_symbol() == function_symbol::LE || upper_rel->get_symbol() == function_symbol::LT))
	{
	  expl.push_back(lower_rel);
	  expl.push_back(upper_rel);
	  //	  std::cout << "SIMPLE EXPLANATION FOR: " << e << " : "<< expl[0] << ", " << expl[1] << std::endl;
	  return true;  
	}      
    }

  // special case x != c is implied by another (non-canonized) x != c
  if(c.type == B_SPLIT)
    {
      const expression_vector & u_csts = u_data->get_constraints();
      int j = find_constraint_r(u_csts, c);
      while(j >= 0 && _data.get_data(u_csts[j])->get_constraint() == c)
	{
	  if(_solver.get_trail().get_value(u_csts[j]) == EB_TRUE)
	    {
	      //std::cout << "EXPLAINING: " << e << " by " << u_csts[j] << std::endl;
	      expl.push_back(u_csts[j]);
	      return true;
	    }
	  j--;
	}
    }
  
  return false;
}

void arithmetic_theory_solver::generate_propagation_explanation(const expression & e, const constraint & sc, explanation & expl)
{
  assert(_data.has_data(e));
  assert(_solver.has_literal_data(e));
  assert(_data.get_data(e)->has_constraint_data());
  _exp_gen_time_spent.start();
  
  // first, try simple explanation, then try bound explanation
  if(!generate_simple_explanation(e, expl))
    {
      assert(sc.type != B_SPLIT);
      arithmetic_theory_solver_data * e_data = _data.get_data(e);
      const constraint & e_cst = e_data->get_constraint();
      const expression & unknown = e_data->get_unknown();
      
      if(e_cst.type != B_SPLIT)
	generate_bound_explanation(unknown, e_cst.type, expl);
      else if(sc.type != B_BOTH)
	generate_bound_explanation(unknown, sc.type, expl);
      else if(sc.bound > e_cst.bound)
	generate_bound_explanation(unknown, B_LOWER, expl);
      else
	generate_bound_explanation(unknown, B_UPPER, expl);
    }
  _exp_gen_time_spent.acumulate();
}

void arithmetic_theory_solver::generate_conflict_explanation(const expression & bu, explanation & expl)
{
  assert(_data.has_data(bu));
  assert(_data.get_data(bu)->has_unknown_data());
  assert(_data.get_data(bu)->is_basic());

  _exp_gen_time_spent.start();
  
  arithmetic_theory_solver_data * bu_data = _data.get_data(bu);
  bound_type bt;

  if(bu_data->get_valuation() < bu_data->get_lower_bound())
    {
      bt = B_UPPER;
      expl.push_back(bu_data->get_lower_bound_relation());
    }
  else
    {
      bt = B_LOWER;
      expl.push_back(bu_data->get_upper_bound_relation());
    }

  generate_bound_explanation(bu, bt, expl);
  _exp_gen_time_spent.acumulate();
}

void arithmetic_theory_solver::calculate_delta()
{
  if(_delta_calculated)
    return;
  
  _delta = 1;
  invalidate_inactive_unknowns();
  for(unsigned i = 0; i < _all_unknowns.size(); i++)
    {
      arithmetic_theory_solver_data * u_data = _data.get_data(_all_unknowns[i]);
      const value_pair & lower = u_data->get_lower_bound();
      const value_pair & upper = u_data->get_upper_bound();
      const value_pair & val = recalculate_unknown_value(_all_unknowns[i]);
      
      if(lower.infinity == I_NONE && val.first > lower.first && lower.second > val.second)
	{
	  unknown_value q = (val.first - lower.first) / (lower.second - val.second);

	  if(q < _delta)
	    _delta = std::move(q);
	}

      if(upper.infinity == I_NONE && upper.first > val.first && val.second > upper.second)
	{
	  unknown_value q = (upper.first - val.first) / (val.second - upper.second);

	  if(q < _delta)
	    _delta = std::move(q);
	}
    }
  _delta_calculated = true;
}

value_pair arithmetic_theory_solver::get_strongest_lower_bound_on_trail(arithmetic_theory_solver_data * data)
{
  const expression_vector & csts = data->get_constraints();
  value_pair strongest(I_NEGATIVE);
  for(unsigned i = 0; i < csts.size(); i++)
    {
      if(_solver.get_trail().is_true(csts[i]))
	{
	  arithmetic_theory_solver_data * c_data = _data.get_data(csts[i]);
	  if((c_data->get_constraint().type == B_LOWER || c_data->get_constraint().type == B_BOTH) &&
	     c_data->get_constraint().bound > strongest)
	    {
	      strongest = c_data->get_constraint().bound;
	    }
	}
    }
  return strongest;
}


value_pair arithmetic_theory_solver::get_strongest_upper_bound_on_trail(arithmetic_theory_solver_data * data)
{
  const expression_vector & csts = data->get_constraints();
  value_pair strongest(I_POSITIVE);

  for(unsigned i = 0; i < csts.size(); i++)
    {
      if(_solver.get_trail().is_true(csts[i]))
	{
	  arithmetic_theory_solver_data * c_data = _data.get_data(csts[i]);
	  if((c_data->get_constraint().type == B_UPPER || c_data->get_constraint().type == B_BOTH) &&
	     c_data->get_constraint().bound < strongest)
	    {
	      strongest = c_data->get_constraint().bound;
	    }
	}
    }
  return strongest;
}

void arithmetic_theory_solver::expand_to_lra_unknowns(linear_polynomial & lp)
{
  linear_polynomial new_lp;
  for(linear_polynomial::const_iterator it = lp.begin(), it_end = lp.end(); it != it_end; ++it)
    {
      linear_polynomial temp_lp;
      get_polynomial_from_expression(it->first, temp_lp);
      new_lp.add_polynomial(temp_lp, it->second);
    }
  lp = std::move(new_lp);
}

void arithmetic_theory_solver::get_tableau_statistics()
{
  static unknown_integer_value abs_value;
  unsigned coeffs_count = 0;

  _curr_max_num = 0;
  _curr_max_den = 0;
  _stat_calls++;
  
  for(unknown_set::const_iterator it = _basic_unknowns.begin(), it_end = _basic_unknowns.end();
      it != it_end; ++it)
    {
      arithmetic_theory_solver_data * bu_data = _data.get_data(*it);
      const linear_polynomial & lp = bu_data->row();
      coeffs_count += lp.size();
      for(linear_polynomial::const_iterator jt = lp.begin(), jt_end = lp.end(); jt != jt_end; ++jt)
	{
#if defined(_HYBRID_INT_VALUE) || defined(_HYBRID_LONG_VALUE)
	  _is_tableau_gmp = _is_tableau_gmp || jt->second.is_gmp();
#endif
	  abs_value = abs(jt->second.get_num());
	  if(abs_value > _curr_max_num)
	    _curr_max_num = abs_value;
	  abs_value = abs(jt->second.get_den());
	  if(abs_value > _curr_max_den)
	    _curr_max_den = abs_value;
	}
    }
  _curr_density_percent = (double)coeffs_count / (_basic_unknowns.size() * _non_basic_unknowns.size());
  _cum_density_percent += _curr_density_percent;
  _cum_max_num += _curr_max_num;
  _cum_max_den += _curr_max_den;

}

void arithmetic_theory_solver::tableau_report(std::ostream & ostr)
{
  ostr << "------------- TABLEAU --------------" << std::endl;
  ostr << "Unknowns: " << _all_unknowns.size() << std::endl;
  ostr << "Basic unknowns: " << _basic_unknowns.size() << std::endl;
  ostr << "Non-basic unknowns: " << _non_basic_unknowns.size() << std::endl;
  ostr << "Inactive unknowns: " << _inactive_unknowns.size() << std::endl;
  ostr << "Unknowns relevant for other theories:  " << _eq_relevant_unknowns.size() << std::endl;
  ostr << "Current (last) density percent: " << _curr_density_percent << std::endl;
  ostr << "Current (last) max numerator: " << _curr_max_num << std::endl;
  ostr << "Current (last) max denominator: " << _curr_max_den << std::endl;

  ostr << "Average density percent: " << _cum_density_percent / _stat_calls << std::endl;
  ostr << "Average max numerator: " << mpf_class((mpz_class)_cum_max_num) / _stat_calls << std::endl;
  ostr << "Average max denominator: " << mpf_class((mpz_class)_cum_max_den) / _stat_calls << std::endl;
#if defined(_HYBRID_INT_VALUE) || defined(_HYBRID_LONG_VALUE)
  ostr << "GMP values used in tableau: " << (_is_tableau_gmp ? "yes" : "no") << std::endl;
#endif
  ostr << "Average number of pivots per check: " << (double)_pivot_call_count / _check_call_count
       << " (" << _pivot_call_count << "/" << _check_call_count << ")" << std::endl;
  
}

void arithmetic_theory_solver::print_tableau()
{
  for(unknown_set::const_iterator jt = _basic_unknowns.begin(), jt_end = _basic_unknowns.end();
      jt != jt_end; ++jt)
    {
      arithmetic_theory_solver_data * th_data = _data.get_data(*jt);
      //      std::cout << "x_" << th_data->get_index() << " = ";
      //      std::cout << "[ ";
      //      const linear_polynomial & lp = th_data->row();
      /*      for(typename linear_polynomial::const_iterator it = lp.begin(), it_end = lp.end(); it != it_end; ++it)
	{
	  std::cout << "[" << it->second << " * x_" << _data.get_data(it->first)->get_index() << "] ";
	}
      std::cout << " ]" << std::endl;
      */
      
      std::cout << *jt << " = " << th_data->row() << " : " << 
      	th_data->get_lower_bound() << " <= " << th_data->get_valuation() << " <= " << th_data->get_upper_bound() <<
      	" (" << th_data->row().size() << ")" << std::endl;      
    }

  for(unsigned i = 0; i < _all_unknowns.size(); i++)
    {
      arithmetic_theory_solver_data * th_data = _data.get_data(_all_unknowns[i]);
      std::cout << "Unknown: " << _all_unknowns[i] << " : " << th_data->get_valuation() << " : [" << th_data->get_lower_bound() << " - " << th_data->get_upper_bound() << (_inactive_unknowns.find(_all_unknowns[i]) != _inactive_unknowns.end() ? " (INACTIVE)" : "") << std::endl;
    }


}

bool arithmetic_theory_solver::check_invariant_nb1()
{
  unknown_set::const_iterator it = _non_basic_unknowns.begin(), it_end = _non_basic_unknowns.end();

  for(;it != it_end; ++it)
    {
      arithmetic_theory_solver_data * curr_data = _data.get_data(*it);

      if(get_strongest_lower_bound_on_trail(curr_data) != curr_data->get_lower_bound())
	{
	  std::cout << "OUPS: nb1-lower: " << *it << std::endl;
	  std::cout << get_strongest_lower_bound_on_trail(curr_data) << std::endl;
	  std::cout << curr_data->get_lower_bound() << std::endl;
	  std::cout << get_strongest_upper_bound_on_trail(curr_data) << std::endl;
	  std::cout << curr_data->get_upper_bound() << std::endl;
	  std::cout << "VAL: " << curr_data->get_valuation() << std::endl;
	  return false;
	}
      if(get_strongest_upper_bound_on_trail(curr_data) != curr_data->get_upper_bound())
	{
	  std::cout << "OUPS: nb1-upper:" << *it << std::endl;
	  std::cout << get_strongest_lower_bound_on_trail(curr_data) << std::endl;
	  std::cout << curr_data->get_lower_bound() << std::endl;
	  std::cout << get_strongest_upper_bound_on_trail(curr_data) << std::endl;
	  std::cout << curr_data->get_upper_bound() << std::endl;
	  std::cout << "VAL: " << curr_data->get_valuation() << std::endl;
	  return false;
	}
    }
  return true;
}

bool arithmetic_theory_solver::check_invariant_nb2()
{
  unknown_set::const_iterator it = _non_basic_unknowns.begin(), it_end = _non_basic_unknowns.end();
	
  for(;it != it_end; ++it)
    {
      arithmetic_theory_solver_data * curr_data = _data.get_data(*it);

      if(curr_data->get_valuation() < curr_data->get_lower_bound() || curr_data->get_valuation() > curr_data->get_upper_bound())
	{
	  std::cout << "OUPS: nb2: " << *it << std::endl;
	  std::cout << get_strongest_lower_bound_on_trail(curr_data) << std::endl;
	  std::cout << curr_data->get_lower_bound() << std::endl;
	  std::cout << get_strongest_upper_bound_on_trail(curr_data) << std::endl;
	  std::cout << curr_data->get_upper_bound() << std::endl;
	  std::cout << "VAL: " << curr_data->get_valuation() << std::endl;
	  return false;
	}
    }
  return true;
}

bool arithmetic_theory_solver::check_invariant_nb3()
{
  unknown_set::const_iterator it = _non_basic_unknowns.begin(), it_end = _non_basic_unknowns.end();
    
  for(;it != it_end; ++it)
    {
      arithmetic_theory_solver_data * curr_data = _data.get_data(*it);
	
      if(curr_data->get_valuation() != calculate_polynomial_value(curr_data->row()))
	{
	  std::cout << "OUPS: nb3: " << *it << " (" << curr_data->get_valuation() << " != " << calculate_polynomial_value(curr_data->row()) << " )" << std::endl;
	  std::cout << "Polynomial: " << curr_data->row() << std::endl;
	  return false;
	}
    }
  return true;
}
  
bool arithmetic_theory_solver::check_invariant_nb4()
{
  for(unknown_set::const_iterator it = _non_basic_unknowns.begin(), it_end = _non_basic_unknowns.end(); it != it_end; ++it)
    {
      if(_data.get_data(*it)->is_basic())
	{
	  std::cout << "OUPS: nb4-basic: " << *it << std::endl;
	  return false;
	}
	
      const linear_polynomial & lp = _data.get_data(*it)->row();
      if(lp.size() != 1 || lp.begin()->first != *it)
	{
	  std::cout << "OUPS: nb4-poly: " << *it << std::endl;
	  return false;
	}
    }
  return true;
}

bool arithmetic_theory_solver::check_invariant_nb5()
{
  for(unknown_set::const_iterator it = _non_basic_unknowns.begin(), it_end = _non_basic_unknowns.end(); it != it_end; ++it)
    {
      arithmetic_theory_solver_data * curr_data = _data.get_data(*it);
      linear_polynomial lp1, lp2;
      get_polynomial_from_expression(*it, lp1);
      lp2 = curr_data->row();
      expand_to_lra_unknowns(lp2);
	
      if(lp1 != lp2)
	{
	  std::cout << "OUPS: nb5: " << lp1 << " vs " << lp2 << std::endl;
	  return false;
	}
    }
  return true;
}

bool arithmetic_theory_solver::check_invariant_b1()
{
  unknown_set::const_iterator it = _basic_unknowns.begin(), it_end = _basic_unknowns.end();

  for(;it != it_end; ++it)
    {
      arithmetic_theory_solver_data * curr_data = _data.get_data(*it);

      if(get_strongest_lower_bound_on_trail(curr_data) != curr_data->get_lower_bound())
	{
	  std::cout << "OUPS: b1-lower: " << *it << std::endl;
	  std::cout << get_strongest_lower_bound_on_trail(curr_data) << std::endl;
	  std::cout << curr_data->get_lower_bound() << std::endl;
	  std::cout << get_strongest_upper_bound_on_trail(curr_data) << std::endl;
	  std::cout << curr_data->get_upper_bound() << std::endl;
	  std::cout << "VAL: " << curr_data->get_valuation() << std::endl;
	  return false;
	}
      if(get_strongest_upper_bound_on_trail(curr_data) != curr_data->get_upper_bound())
	{
	  std::cout << "OUPS: b1-upper:" << *it << std::endl;
	  std::cout << get_strongest_lower_bound_on_trail(curr_data) << std::endl;
	  std::cout << curr_data->get_lower_bound() << std::endl;
	  std::cout << get_strongest_upper_bound_on_trail(curr_data) << std::endl;
	  std::cout << curr_data->get_upper_bound() << std::endl;
	  std::cout << "VAL: " << curr_data->get_valuation() << std::endl;
	  return false;
	}
    }
  return true;
}

bool arithmetic_theory_solver::check_invariant_b2()
{
  unknown_set::const_iterator it = _basic_unknowns.begin(), it_end = _basic_unknowns.end();
	
  for(;it != it_end; ++it)
    {
      arithmetic_theory_solver_data * curr_data = _data.get_data(*it);

      if(curr_data->get_valuation() < curr_data->get_lower_bound() || curr_data->get_valuation() > curr_data->get_upper_bound())
	{
	  std::cout << "OUPS: b2: " << *it << std::endl;
	  std::cout << get_strongest_lower_bound_on_trail(curr_data) << std::endl;
	  std::cout << curr_data->get_lower_bound() << std::endl;
	  std::cout << get_strongest_upper_bound_on_trail(curr_data) << std::endl;
	  std::cout << curr_data->get_upper_bound() << std::endl;
	  std::cout << "VAL: " << curr_data->get_valuation() << std::endl;
	  return false;
	}
    }
  return true;
}

bool arithmetic_theory_solver::check_invariant_b3()
{
  unknown_set::const_iterator it = _basic_unknowns.begin(), it_end = _basic_unknowns.end();
    
  for(;it != it_end; ++it)
    {
      arithmetic_theory_solver_data * curr_data = _data.get_data(*it);
	
      if(curr_data->get_valuation() != calculate_polynomial_value(curr_data->row()))
	{
	  std::cout << "OUPS: b3: " << *it << " (" << curr_data->get_valuation() << " != " << calculate_polynomial_value(curr_data->row()) << " )" << std::endl;
	  std::cout << "Polynomial: " << curr_data->row() << std::endl;
	  return false;
	}
    }
  return true;
}
  
bool arithmetic_theory_solver::check_invariant_b4()
{
  for(unknown_set::const_iterator it = _basic_unknowns.begin(), it_end = _basic_unknowns.end(); it != it_end; ++it)
    {
      if(!_data.get_data(*it)->is_basic())
	{
	  std::cout << "OUPS: b4-non_basic: " << *it << std::endl;
	  return false;
	}

      const linear_polynomial & lp = _data.get_data(*it)->row();
      for(linear_polynomial::const_iterator jt = lp.begin(), jt_end = lp.end(); jt != jt_end; jt++)
	{
	  if(jt->first == _one_expression)
	    continue;
	  
	  if(_data.get_data(jt->first)->is_basic())
	    {
	      std::cout << "OUPS: b4-poly: " << *it << " with " << jt->first << std::endl;
	      return false;
	    }
	}
    }
  return true;
}

bool arithmetic_theory_solver::check_invariant_b5()
{
  for(unknown_set::const_iterator it = _basic_unknowns.begin(), it_end = _basic_unknowns.end(); it != it_end; ++it)
    {
      arithmetic_theory_solver_data * curr_data = _data.get_data(*it);
      linear_polynomial lp1, lp2;
      get_polynomial_from_expression(*it, lp1);
      lp2 = curr_data->row();
      expand_to_lra_unknowns(lp2);
	
      if(lp1 != lp2)
	{
	  std::cout << "OUPS: b5: " << lp1 << " vs " << lp2 << std::endl;
	  return false;
	}
    }
  return true;
}

bool arithmetic_theory_solver::check_all_invariants()
{
  bool ret = true;
  ret = check_invariant_nb1() && ret;
  ret = check_invariant_nb2() && ret;
  ret = check_invariant_nb3() && ret;
  ret = check_invariant_nb4() && ret;
  ret = check_invariant_nb5() && ret;
  ret = check_invariant_b1() && ret;
  ret = check_invariant_b2() && ret;
  ret = check_invariant_b3() && ret;
  ret = check_invariant_b4() && ret;
  ret = check_invariant_b5() && ret;
  return ret;
}
