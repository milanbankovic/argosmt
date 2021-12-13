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

#include "csp_theory_solver.hpp"
#include "domain_handler.hpp"
#include "alldiff_constraint_handler.hpp"
#include "sum_constraint_handler.hpp"
#include "csp_variable_order_strategy.hpp"
#include "csp_value_order_strategy.hpp"


csp_theory_solver::csp_theory_solver_data::
csp_theory_solver_data(csp_expression_type t, unsigned index,
		       csp_theory_solver_data * data)
{
  if(t == CSP_VARIABLE)
    {
      _variable_data = new variable_data();
      _variable_data->_index = index;
      _variable_data->_handler = 0;
      _constraint_data = 0;
      _value_data = 0;
      _domain_restriction_data = 0;
    }
  else if(t == CSP_CONSTRAINT)
    {
      _constraint_data = new constraint_data();
      if(data == 0)
	{
	  _constraint_data->_positive = true;
	  _constraint_data->_pair_data = new constraint_data::pair_data();
	  _constraint_data->_pair_data->_handler = 0;		
	  _constraint_data->_pair_data->_explain_handler = 0;
	  _constraint_data->_pair_data->_index = index;
	}
      else
	{
	  _constraint_data->_positive = false;
	  _constraint_data->_pair_data = data->_constraint_data->_pair_data;
	}
      _variable_data = 0;
      _value_data = 0;
      _domain_restriction_data = 0;
    }
  else if(t == CSP_VALUE)
    {
      _value_data = new value_data();
      _value_data->_index = index;
      _constraint_data = 0;
      _variable_data = 0;
      _domain_restriction_data = 0;
    }
  else // t == CSP_DOMAIN_RESTRICTION
    {
      _value_data = 0;
      _constraint_data = 0;
      _variable_data = 0;
      _domain_restriction_data = new domain_restriction_data();
      if(data == 0)
	{
	  _domain_restriction_data->_positive = true;
	  _domain_restriction_data->_pair_data = new domain_restriction_data::pair_data();
	  _domain_restriction_data->_pair_data->_handler = 0;
	  _domain_restriction_data->_pair_data->_index = index;
	}
      else
	{
	  _domain_restriction_data->_positive = false;
	  _domain_restriction_data->_pair_data = data->_domain_restriction_data->_pair_data;
	}
    }
}

csp_theory_solver::csp_theory_solver_data::~csp_theory_solver_data()
{
  if(_constraint_data)
    {
      if(_constraint_data->_positive)
	{
	  delete _constraint_data->_pair_data->_handler;
	  delete _constraint_data->_pair_data;
	}
      delete _constraint_data;
    }
      
  if(_variable_data)
    {
      delete _variable_data->_handler;
      delete _variable_data;
    }
      
  if(_value_data)
    delete _value_data;
      
  if(_domain_restriction_data)
    {
      if(_domain_restriction_data->_positive)
	delete _domain_restriction_data->_pair_data;
      delete _domain_restriction_data;
    }
}

void csp_theory_solver::on_pop::operator () (const expression & e) const
{
  csp_theory_solver_data * e_data = _theory->_data.get_data(e);
  if(e_data != 0 && e_data->has_constraint_data())
    {
      //std::cout << "Deactivating: " << e << std::endl;
      e_data->get_constraint_handler()->set_active(false);
    }
}


csp_theory_solver::csp_theory_solver_data * csp_theory_solver::add_variable(const expression & var)
{
  assert(var->is_function());
  csp_theory_solver_data * var_data = _data.get_data(var);
  if(var_data == 0)
    {
      //std::cout << "VAR: " << var << std::endl;
      var_data = new csp_theory_solver_data(CSP_VARIABLE, _variables.size());
      _data.set_data(var, var_data);
      _variables.push_back(var);
      var_data->set_variable_domain_handler(new domain_handler(this, var));
    }
  return var_data;
}

csp_theory_solver::csp_theory_solver_data * csp_theory_solver::add_value(const expression & val)
{
  csp_theory_solver_data * val_data = _data.get_data(val);
  if(val_data == 0)
    {
      val_data = new csp_theory_solver_data(CSP_VALUE, _values.size());
      _data.set_data(val, val_data);
      _values.push_back(val);
      int v = get_value_from_expr(val);
      val_data->set_int_value(v);
      _value_cache[v] = val;
    }
  return val_data;
}

csp_theory_solver::csp_theory_solver_data * csp_theory_solver::add_value(int v)
{
  expression val;
  if(!get_expr_for_int(v, val))
    val = get_expr_from_value(v);

  _solver.init_expression_data(val);
  csp_theory_solver_data * val_data = _data.get_data(val);
  if(val_data == 0)
    {
      val_data = new csp_theory_solver_data(CSP_VALUE, _values.size());
      _data.set_data(val, val_data);
      _values.push_back(val);
      val_data->set_int_value(v);
      _value_cache[v] = val;
    }
  return val_data;
}

csp_theory_solver::csp_theory_solver_data * csp_theory_solver::add_constraint_pair(const expression & l_pos, const expression & l_neg)
{
  csp_theory_solver_data * l_pos_data = _data.get_data(l_pos);
  if(l_pos_data == 0)
    {
      l_pos_data = new csp_theory_solver_data(CSP_CONSTRAINT, _constraints.size());	  
      csp_theory_solver_data * l_neg_data = new csp_theory_solver_data(CSP_CONSTRAINT, _constraints.size(), l_pos_data);
      _data.set_data(l_pos, l_pos_data);
      _data.set_data(l_neg, l_neg_data);
      _constraints.push_back(l_pos);
    }
  return l_pos_data;
}

csp_theory_solver::csp_theory_solver_data * csp_theory_solver::add_domain_restriction_pair(const expression & l_pos, const expression & l_neg)
{
  csp_theory_solver_data * l_pos_data = _data.get_data(l_pos);
  if(l_pos_data == 0)
    {
      l_pos_data = new csp_theory_solver_data(CSP_DOMAIN_RESTRICTION, _domain_restrictions.size());
      csp_theory_solver_data * l_neg_data = new csp_theory_solver_data(CSP_DOMAIN_RESTRICTION, _domain_restrictions.size(), l_pos_data);
      _data.set_data(l_pos, l_pos_data);
      _data.set_data(l_neg, l_neg_data);
      _domain_restrictions.push_back(l_pos);
    }
  return l_pos_data;
}

void csp_theory_solver::check_constraint_implications()
{
  const expression & last = _constraints.back();
  const expression & last_neg = _solver.get_literal_data(last)->get_opposite();
  csp_theory_solver_data * last_data = _data.get_data(last);
  constraint_handler * last_handler = last_data->get_constraint_handler();

  for(unsigned i = 0; i < _constraints.size() - 1; i++)
    {
      const expression & curr = _constraints[i];
      const expression & curr_neg = _solver.get_literal_data(curr)->get_opposite();
      csp_theory_solver_data * curr_data = _data.get_data(curr);
      constraint_handler * curr_handler = curr_data->get_constraint_handler();

      if(last_handler->is_weaker_from_pos(curr))
	{
	  last_handler->add_to_weaker_from_pos_constraints(curr);
	  curr_handler->add_to_weaker_from_neg_constraints(last_neg);
	}
      else if(last_handler->is_weaker_from_pos(curr_neg))
	{
	  last_handler->add_to_weaker_from_pos_constraints(curr_neg);
	  curr_handler->add_to_weaker_from_pos_constraints(last_neg);
	}
      else if(last_handler->is_weaker_from_neg(curr))
	{	 
	  last_handler->add_to_weaker_from_neg_constraints(curr);
	  curr_handler->add_to_weaker_from_neg_constraints(last);
	}
      else if(last_handler->is_weaker_from_neg(curr_neg))
	{
	  last_handler->add_to_weaker_from_neg_constraints(curr_neg);
	  curr_handler->add_to_weaker_from_pos_constraints(last);
	}
    }
}

void csp_theory_solver::add_literal(const expression & l_pos, const expression & l_neg)
{
  if(l_pos->is_equality() ||
     l_pos->get_symbol() == function_symbol::LE ||
     l_pos->get_symbol() == function_symbol::GE)
    {
      expression left = l_pos->get_operands()[0];
      expression right = l_pos->get_operands()[1];
      
      if(!is_csp_term(left) || !is_csp_term(right))
	return;

      if(is_atom(left) && is_coefficient(right))
	{
	  //	  if(l_pos->is_equality())
	  //  _solver.get_literal_selection_strategy()->branching_priority_hint(l_pos, 0x80000000);
	  //else
	  // _solver.get_literal_selection_strategy()->branching_priority_hint(l_pos, 0x7fffffff);
	  _solver.get_polarity_selection_strategy()->preffered_polarity_hint(l_pos, 1, false);
	  _solver.get_literal_data(l_pos)->add_observing_theory_solver(this);
	  csp_theory_solver_data * var_data = add_variable(left);
	  csp_theory_solver_data * val_data = add_value(right);
	  var_data->get_variable_domain_handler()->add_value(val_data->get_int_value());
	  csp_theory_solver_data * l_pos_data = add_domain_restriction_pair(l_pos, l_neg);
	  var_data->add_variable_domain_restriction(l_pos);
	  l_pos_data->set_domain_restriction_variable(left);
	  l_pos_data->add_domain_restriction_value(val_data->get_int_value());
	}
      else
	{
	  //_solver.get_literal_selection_strategy()->branching_priority_hint(l_pos, 4);
	  _solver.get_polarity_selection_strategy()->preffered_polarity_hint(l_pos, 1, false);
	  _solver.get_literal_data(l_pos)->add_observing_theory_solver(this);
	  csp_theory_solver_data * l_pos_data = add_constraint_pair(l_pos, l_neg);
	  if(!l_pos_data->has_constraint_handler())
	    {
	      l_pos_data->set_constraint_handler(new sum_constraint_handler(this, l_pos, l_neg));
	      check_constraint_implications();
	      add_to_inactive_queue(l_pos_data->get_constraint_handler());
	    }
	}
    }
  else if(l_pos->get_symbol() == function_symbol::ALLDIFF)
    {
      //_solver.get_literal_selection_strategy()->branching_priority_hint(l_pos, 4);
      _solver.get_polarity_selection_strategy()->preffered_polarity_hint(l_pos, 1, false);
      _solver.get_literal_data(l_pos)->add_observing_theory_solver(this);
      csp_theory_solver_data * l_pos_data = add_constraint_pair(l_pos, l_neg);
      if(!l_pos_data->has_constraint_handler())
	{
	  l_pos_data->set_constraint_handler(new alldiff_constraint_handler(this, l_pos, l_neg));
	  check_constraint_implications();
	  add_to_inactive_queue(l_pos_data->get_constraint_handler());
	}
    }
}

void csp_theory_solver::new_level() 
{
  _new_level_time_spent.start();
  _current_level++;
  for(unsigned i = 0; i < _constraints.size(); i++)
    _data.get_data(_constraints[i])->get_constraint_handler()->new_level();
  for(unsigned i = 0; i < _variables.size(); i++)
    _data.get_data(_variables[i])->get_variable_domain_handler()->new_level();
  _varorder->new_level();
  _valorder->new_level();
  clear_all_queues();
  _new_level_time_spent.acumulate();
}

void csp_theory_solver::backjump(unsigned level) 
{
  _backjump_time_spent.start();
  _current_trail_pos = std::min(_solver.get_trail().size(), _current_trail_pos);
  _current_level = level;
  for(unsigned i = 0; i < _constraints.size(); i++)
    _data.get_data(_constraints[i])->get_constraint_handler()->backjump(level);
  for(unsigned i = 0; i < _variables.size(); i++)
    _data.get_data(_variables[i])->get_variable_domain_handler()->backjump(level);
  _varorder->backjump(level);
  _valorder->backjump(level);
  clear_all_queues();
  _backjump_time_spent.acumulate();
}

void csp_theory_solver::add_to_all_queues(constraint_handler * handler)
{
  for(unsigned i = 0; i < _handler_queues.size(); i++)
    if(!handler->is_in_queue(i))
      {
	_handler_queues[i].push_back(handler);
	handler->set_in_queue(i, true);
      }
}

void csp_theory_solver::add_to_domain_handler_queue(domain_handler * handler)
{
  if(!handler->is_in_queue())
    {
      _domain_handler_queue.push_back(handler);
      handler->set_in_queue(true);
    }
}

void csp_theory_solver::add_to_inactive_queue(constraint_handler * handler)
{
  if(!handler->is_in_inactive_queue())
    {
      _inactive_queue.push_back(handler);
      handler->set_in_inactive_queue(true);
    }
}

void csp_theory_solver::clear_all_queues()
{
  for(unsigned i = 0; i < _handler_queues.size(); i++)
    clear_queue(i);
  clear_domain_queue();
  clear_inactive_queue();
}

void csp_theory_solver::clear_domain_queue()
{
  while(!_domain_handler_queue.empty())
    {
      _domain_handler_queue.front()->set_in_queue(false);
      _domain_handler_queue.pop_front();
    }
}

void csp_theory_solver::clear_inactive_queue()
{
  while(!_inactive_queue.empty())
    {
      _inactive_queue.front()->set_in_inactive_queue(false);
      _inactive_queue.pop_front();
    }
}


void csp_theory_solver::clear_queue(unsigned i)
{
  while(!_handler_queues[i].empty())
    {
      _handler_queues[i].front()->set_in_queue(i, false);
      _handler_queues[i].pop_front();
    }
}  

void csp_theory_solver::process_main_trail()
{
  while(_current_trail_pos < _solver.get_trail().size())
    {
      expression l = _solver.get_trail()[_current_trail_pos++];
      if(!_solver.get_literal_data(l)->is_observing_theory_solver(this))
	continue;

      process_literal(l);
    }
}

void csp_theory_solver::process_literal(const expression & l) 
{
  csp_theory_solver_data * l_data = _data.get_data(l);
  assert(l_data != 0);
  if(l_data->has_domain_restriction_data())
    {
      _varorder->assert_literal(l);
      _valorder->assert_literal(l);
      const expression & var = l_data->get_domain_restriction_variable();
      csp_theory_solver_data * var_data = _data.get_data(var);
      assert(var_data != 0 && var_data->has_variable_data());
      domain_handler * d_handler = var_data->get_variable_domain_handler();
      add_to_domain_handler_queue(d_handler);
      d_handler->assert_literal(l);
      const expression_set & var_cons = var_data->get_variable_constraints();
      for(expression_set::const_iterator it = var_cons.begin(), it_end = var_cons.end();
	  it != it_end; ++it)
	{
	  csp_theory_solver_data * cons_data = _data.get_data(*it);
	  assert(cons_data != 0 && cons_data->has_constraint_data());
	  constraint_handler * c_handler = cons_data->get_constraint_handler();
	  if(c_handler->is_active())
	    add_to_all_queues(c_handler);
	  else
	    add_to_inactive_queue(c_handler);

	  c_handler->assert_literal(l);
	}
    }
  else if(l_data->has_constraint_data())
    {
      constraint_handler * c_handler = l_data->get_constraint_handler();
      //      std::cout << "Activating: " << l << std::endl;
      c_handler->set_active(true);
      add_to_all_queues(c_handler);
    }
}

void csp_theory_solver::init_predefined_variable_bounds()
{
  for(unsigned i = 0; i < _variables.size(); i++)
    {
      csp_theory_solver_data * v_data = _data.get_data(_variables[i]);
      v_data->get_variable_domain_handler()->
	set_initial_bounds(_lower_predefined_bound, _upper_predefined_bound);
    }
  _predefined_bounds_set = true;
}

void csp_theory_solver::check_and_propagate(unsigned layer) 
{
  if(layer >= _num_of_layers)
    return;
  
  _check_and_prop_time_spent.start();

  process_main_trail();
  
  if(_predefined_bounds && !_predefined_bounds_set)
    init_predefined_variable_bounds();
  
  layer = std::min(layer, get_num_of_layers() - 1);

  while(!_solver.is_conflict() && (!_domain_handler_queue.empty() || !_handler_queues[layer].empty()))
    {
      while(!_solver.is_conflict() && !_domain_handler_queue.empty())
	{
	  domain_handler * d_handler = _domain_handler_queue.front();
	  d_handler->check_and_propagate(layer);
	  _domain_handler_queue.pop_front();
	  d_handler->set_in_queue(false);
	}
      
      if(!_solver.is_conflict() && !_handler_queues[layer].empty())
	{
	  constraint_handler * handler = _handler_queues[layer].front();
	  handler->check_and_propagate(layer);
	  if(handler->requires_reprocessing())
	    {
	      //  std::cout << "Reprocessing: " << handler->get_constraint() << std::endl;
	      handler->reset_reprocessing_flag();
	    }
	  else
	    {
	      _handler_queues[layer].pop_front();
	      handler->set_in_queue(layer, false);
	    }
	}
    }

  if(_solver.is_conflict())
    {
      clear_domain_queue();
      clear_queue(layer);
      _check_and_prop_time_spent.acumulate();
      return;
    }

  while(!_solver.is_conflict() && !_inactive_queue.empty())
    {
      constraint_handler * handler = _inactive_queue.front();
      _inactive_queue.pop_front();
      handler->set_in_inactive_queue(false);
      if(!handler->is_active())
	{
	  handler->check_implied();
	}
    }
  
  expression var;
  if(_solver.all_literals_assigned() && layer == get_num_of_layers() - 1 && _varorder->get_next_variable(var))
    {
      int val;
      bound_type bt;
      _valorder->get_next_value(var, val, bt);
      //std::cout << "Introducing varval: " << var << bt << val << std::endl;
      domain_handler * d_handler = _data.get_data(var)->get_variable_domain_handler();
      //std::cout << d_handler->get_min() << " : " << d_handler->get_max() << std::endl;
      switch(bt)
	{
	case B_BOTH:
	  d_handler->init_literal_pair(val, true);
	  break;
	case B_UPPER:
	  d_handler->init_upper_bound(val);
	  break;
	case B_LOWER:
	  d_handler->init_lower_bound(val);
	  break;
	default:
	  break;
	}
    }

  _check_and_prop_time_spent.acumulate();
}

void csp_theory_solver::explain_literal(const expression & l) 
{
  _explain_time_spent.start();
  _data.get_data(l)->get_explanation_handler()->explain_literal(l);
  _explain_time_spent.acumulate();
}


expression csp_theory_solver::canonize_expression(const expression & e)
{
  if(e->get_symbol() == function_symbol::ALLDIFF || e->get_symbol() == function_symbol::NOT_ALLDIFF)
    {
      expression_vector operands = e->get_operands();
      std::sort(operands.begin(), operands.end());
      return _solver.get_factory()->create_expression(e->get_symbol(), std::move(operands));
    }
  else if(e->is_not())
    {
      expression ep = e->get_operands()[0];
      const function_symbol & ep_fs = ep->get_symbol();
      if(ep_fs == function_symbol::ALLDIFF || ep_fs == function_symbol::NOT_ALLDIFF)
	return _solver.get_factory()->create_expression(get_opposite_symbol(ep_fs), ep->get_operands());
      else
	return arithmetic_canonizer<int>::canonize_expression(e);
    }
  else
    return arithmetic_canonizer<int>::canonize_expression(e);
}

void csp_theory_solver::get_literal_pair(const expression & l, expression & l_pos, expression & l_neg)
{
  const function_symbol & fs = l->get_symbol();
  if(fs == function_symbol::ALLDIFF)
    {
      l_pos = l;
      l_neg = _solver.get_factory()->create_expression(get_opposite_symbol(fs), l->get_operands());
    }
  else if(fs == function_symbol::NOT_ALLDIFF)
    {
      l_neg = l;
      l_pos = _solver.get_factory()->create_expression(get_opposite_symbol(fs), l->get_operands());
    }
  else
    arithmetic_canonizer<int>::get_literal_pair(l, l_pos, l_neg);
}

void csp_theory_solver::get_model(const expression_vector & exps)
{
  for(unsigned i = 0; i < exps.size(); i++)
    {
      if(!_data.has_data(exps[i]) || !_data.get_data(exps[i])->has_variable_data())
	continue;
      csp_theory_solver_data * var_data = _data.get_data(exps[i]);
      exps[i]->set_assigned(var_data->get_variable_domain_handler()->get_assigned_value());
    }
}

void csp_theory_solver::print_report(std::ostream & ostr)
{
  ostr << "-------- CSP THEORY ---------" << std::endl;
  ostr << "Time spent in check_and_propagate: " << _check_and_prop_time_spent.cumulative_time() << std::endl;
  ostr << "Time spent in new_level: " << _new_level_time_spent.cumulative_time() << std::endl;
  ostr << "Time spent in backjump: " << _backjump_time_spent.cumulative_time() << std::endl;
  ostr << "Time spent in explain: " << _explain_time_spent.cumulative_time() << std::endl;
  if(_as_algorithm != AS_NONE && _sum_common_data._instance_count > 0)    
    ostr << "Time spent in alldiff-sum algorithm: " << _sum_common_data._as_algorithm_time_spent.cumulative_time() << std::endl;
  
  ostr << "EQUALITIES INTRODUCED: " << _domain_common_data._count_equalities << std::endl;
  ostr << "INEQUALITIES INTRODUCED: " << _domain_common_data._count_inequalities << std::endl;
  ostr << "----------------------------" << std::endl;
  ostr << "DOMAIN HANDLER: " << _domain_common_data._count_propagate << " propagations and " << 
    _domain_common_data._count_conflict << " conflicts" << std::endl;
  ostr << "----------------------------" << std::endl;
  ostr << "ALLDIFF HANDLER: " << _alldiff_common_data._count_propagate << " propagations and " << 
    _alldiff_common_data._count_conflict << " conflicts" << std::endl;
  ostr << "Tarjan calls: " << _bipartite_graph_common_data._count_tarjan << std::endl;
  ostr << "Tarjan productive calls: " << _bipartite_graph_common_data._count_tarjan_productive << std::endl;
  ostr << "Trivial prunings: " << _bipartite_graph_common_data._count_trivial_prunings << std::endl;
  ostr << "Hall prunings: " << _bipartite_graph_common_data._count_hall_prunings << std::endl;
  ostr << "Trivial explanations: " << _alldiff_common_data._count_trivial_explain << std::endl;
  ostr << "Hall explanations: " << _alldiff_common_data._count_hall_explain << std::endl;
  ostr << "Average explanation depth: " << (_alldiff_common_data._cumulative_expl_depth / _alldiff_common_data._expl_depth_count) << std::endl;
  ostr << "----------------------------" << std::endl;

  ostr << "SUM CONSTRAINT HANDLER: " << _sum_common_data._count_propagate << " propagations and " <<
    _sum_common_data._count_conflict << " conflicts" << std::endl;
  if(_as_algorithm != AS_NONE && _sum_common_data._instance_count > 0)
    {
      ostr << "PERCENT OF IMPROVED BOUNDS: " << _sum_common_data._count_improved_bound_change / (double)_sum_common_data._count_bound_change << std::endl;
      ostr << "AVERAGE BOUND IMPROVEMENT: " << _sum_common_data._cumulative_improvement / (double)_sum_common_data._count_improved_bound_change << std::endl;
    }
}

csp_theory_solver::~csp_theory_solver()
{
  delete _varorder;
  delete _valorder;
}
