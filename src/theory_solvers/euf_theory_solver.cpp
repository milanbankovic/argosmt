/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2017,2021 Milan Bankovic (milan@matf.bg.ac.rs)

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

#include "euf_theory_solver.hpp"
#include "wall_clock.hpp"
#include <iterator>

bool euf_theory_solver::is_owned_expression(const expression & e)
{
  if(e->is_equality() || e->is_disequality())
    {
      return false;
    }
  if(e->is_constant_function())
    {
      sort s = e->get_inferred_sort();
      unsigned s_arity = s->get_operands().size();
      return
	_solver.get_factory()->get_signature()->
	check_sort_symbol(s->get_symbol(), s_arity, S_CONTEXTUAL) &&
	!_solver.get_factory()->get_signature()->
	check_sort_symbol(s->get_symbol(), s_arity, S_STANDALONE) &&
	_solver.get_factory()->get_signature()->
	check_function_symbol(e->get_symbol(), { }, s, S_CONTEXTUAL) &&
	!_solver.get_factory()->get_signature()->
	check_function_symbol(e->get_symbol(), { }, s, S_STANDALONE);
    }
  else if(e->is_function())
    {
      sort_vector sv;
      expression ei = e->infer_sorts();
      const expression_vector & ops = ei->get_operands();
      for(unsigned i = 0; i < ops.size(); i++)
	{
	  sv.push_back(ops[i]->get_sort());
	}
      sort s = ei->get_sort();

      return _solver.get_factory()->
	get_signature()->check_function_symbol(ei->get_symbol(), sv, s, S_CONTEXTUAL) &&
	!_solver.get_factory()->
	get_signature()->check_function_symbol(ei->get_symbol(), sv, s, S_STANDALONE);
    }
  else if(e->is_special_constant())
    {
      sort s = e->get_inferred_sort();

      return _solver.get_factory()->
	get_signature()->check_special_constant(e->get_special_constant(), s, S_CONTEXTUAL) &&
	!_solver.get_factory()->
	get_signature()->check_special_constant(e->get_special_constant(), s, S_STANDALONE);
    }
  else
    return false;
}


void euf_theory_solver::new_expression(const expression & e)
{
  bool relevant = check_term_relevancy(e);

  
  _data.set_data(e, new euf_theory_solver_data(e, relevant, _current_level));

  if(!relevant)
    {
      //std::cout << "NOT RELEVANT: " << e << std::endl;
      return;
    }
  //std::cout << "ADDING TERM: " << e << std::endl;
  _terms.push_back(e);
  //std::cout << "Terms: " << _terms.size() << std::endl;

  if(e->get_operands().empty())
    return;
  
  //if(_solver.get_common_data(e)->get_owner() != this || e->get_operands().empty())
  // return;

  if(_solver.has_literal_data(e))
    _predicate_propagation_causes[e] = e;
  
  lookup_key rft = get_lookup_key(e);

  lookup_table::const_iterator it = lookup().find(rft);

  if(it != lookup().end())
    {
      std::queue<expression_pair> pending_unions;
      pending_unions.push(expression_pair(e, it->second));
      process_pending_unions(pending_unions, true);
    }
  else
    {
      for(unsigned i = 0; i < rft.args().size(); i++)
	_data.get_data(rft.args()[i])->add_to_use_list(e);

      insert_lookup(std::move(rft), e);
    }
}

void euf_theory_solver::add_to_proof_forest(const expression & a, const expression & b, bool explain)
{
  expression_vector stack;
  expression x = a;
  expression p = _data.get_data(x)->get_proof_forest().parent();
  while(p != x)
    {
      stack.push_back(std::move(x));
      x = p;
      p = _data.get_data(x)->get_proof_forest().parent();
    }

  while(x != a)
    {
      bool xexplain = _data.get_data(stack.back())->get_proof_forest().should_explain();
      _data.get_data(x)->set_proof_forest(forest_entry(stack.back(), xexplain));
      x = stack.back();
      stack.pop_back();
    }

  _data.get_data(a)->set_proof_forest(forest_entry(b, explain));
}

void euf_theory_solver::
process_pending_unions(std::queue<expression_pair> & pending_unions, bool explain)
{
  for(; !pending_unions.empty() && !_solver.is_conflict();
      pending_unions.pop(), explain = true)
    {
      const expression_pair & entry = pending_unions.front();

      expression a, b, ra, rb;

      a = entry.first;
      b = entry.second;

      ra = _data.get_data(a)->get_representative();
      rb = _data.get_data(b)->get_representative();

      if(ra == rb)
	continue;

      if(_data.get_data(ra)->get_class_list().size() > _data.get_data(rb)->get_class_list().size())
	{
	  std::swap(ra, rb);
	  std::swap(a, b);
	}

      const expression_vector & ral = _data.get_data(ra)->get_class_list();
      const expression_vector & rbl = _data.get_data(rb)->get_class_list();
      add_to_proof_forest(a, b, explain);
      check_eq_propagations(ral, rbl);
      check_predicate_propagations(ra, rb);
      for(unsigned i = 0; i < ral.size(); i++)
	{
	  _data.get_data(ral[i])->set_representative(rb);
	  _data.get_data(rb)->add_to_class_list(ral[i]);
	}
      _data.get_data(ra)->clear_class_list();

      const expression_vector & raul = _data.get_data(ra)->get_use_list();

      for(unsigned i = 0; i < raul.size(); i++)
	{
	  const expression & use = raul[i];
	  lookup_key rft = get_lookup_key(use);
	  lookup_table::const_iterator it = lookup().find(rft);
	  if(it != lookup().end())
	    {
	      pending_unions.push(expression_pair(use, it->second));
	    }
	  else
	    {
	      insert_lookup(std::move(rft), use);
	      _data.get_data(rb)->add_to_use_list(use);
	    }
	}
      _data.get_data(ra)->clear_use_list();
    }
}

void euf_theory_solver::check_eq_propagations(const expression_vector & cl1,
					      const expression_vector & cl2)
{
  for(unsigned i = 0; i < cl1.size(); i++)
    for(unsigned j = 0; j < cl2.size(); j++)
      {
	const expression & le = cl1[i];
	const expression & re = cl2[j];
	expression eq;

	if((_solver.get_theory_solvers().size() > 2 && _solver.apply_introduce_shared_equality(le, re, eq, 0, 0, false, 1, false)) ||
	   (_solver.get_theory_solvers().size() <= 2 && _solver.is_equality_cached(le, re, eq)))
	  {
	    extended_boolean eq_value = _solver.get_trail().get_value(eq);

	    if(eq_value == EB_UNDEFINED)
	      _solver.apply_propagate(eq, this);
	    else if(eq_value == EB_FALSE)
	      {
		explanation conflicting;
		explain_conflict(le, re, conflicting, _solver.get_literal_data(eq)->get_opposite());
		_solver.apply_conflict(conflicting, this);
		return;
	      }
	  }
      }
}

void euf_theory_solver::check_diseq_propagations(const expression_vector & cl1,
						 const expression_vector & cl2,
						 const expression & cdiseq)
{
  for(unsigned i = 0; i < cl1.size(); i++)
    for(unsigned j = 0; j < cl2.size(); j++)
      {
	const expression & le = cl1[i];
	const expression & re = cl2[j];
	expression eq;

	if((_solver.get_theory_solvers().size() > 2 && _solver.apply_introduce_shared_equality(le, re, eq, 0, 0, false, 0, false)) ||
	   (_solver.get_theory_solvers().size() <= 2 && _solver.is_equality_cached(le, re, eq)))
	  {
	    expression diseq = _solver.get_literal_data(eq)->get_opposite();
	    extended_boolean diseq_value = _solver.get_trail().get_value(diseq);

	    if(diseq_value != EB_TRUE)
	      {
		bool swapped = le != eq->get_operands()[0];
		_disequality_propagation_causes[diseq] = { cdiseq, swapped };
	      }

	    if(diseq_value == EB_UNDEFINED)	      
	      _solver.apply_propagate(diseq, this);
	    else if(diseq_value == EB_FALSE)
	      {
		explanation conflicting;
		explain_disequality(diseq, conflicting);
		conflicting.push_back(eq);
		_solver.apply_conflict(conflicting, this);
		return;
	      }
	  }
      }
}



expression euf_theory_solver::find_common_ancestor(expression_to_expression_map & proof_state,
					    expression a, expression b)
{
  std::unordered_map<expression, bool> path_marker;

  a = proof_state[a];
  path_marker[a] = true;
  expression p =  _data.get_data(a)->get_proof_forest().parent();
  while(a != p)
    {
      a = proof_state[p];
      path_marker[a] = true;
      p = _data.get_data(a)->get_proof_forest().parent();     
    }

  b = proof_state[b];
  while(!path_marker[b])
    {
      b = proof_state[_data.get_data(b)->get_proof_forest().parent()];
    }
  return b;
}

void
euf_theory_solver::explain_along_path(expression_to_expression_map & proof_state,
				      expression_to_vector_map & class_lists,
				      std::vector<expression_pair> & expl_output,
				      std::queue<expression_pair> & pending_proofs,
				      expression a, expression c)
{
  a = proof_state[a];
  expression b;
  while(a != c)
    {
      forest_entry pfa = _data.get_data(a)->get_proof_forest();
      b = pfa.parent();
      if(!pfa.should_explain())
	{
	  expl_output.push_back(expression_pair(a, b));
	}
      else
	{
	  for(unsigned i = 0; i < a->get_operands().size(); i++)
	    pending_proofs.push(expression_pair(a->get_operands()[i],
						b->get_operands()[i]));
	}

      b = proof_state[b];
      expression_vector & cla = class_lists[a];
      expression_vector & clb = class_lists[b];
      for(unsigned i = 0; i < cla.size(); i++)
	{
	  proof_state[cla[i]] = b;
	  clb.push_back(cla[i]);
	}
      cla.clear();
      a = b;
    }
}

void euf_theory_solver::explain(const expression & c1, const expression & c2,
				std::vector<expression_pair> & expl_output)

{
  expression_to_expression_map proof_state;
  expression_to_vector_map class_lists;
  std::queue<expression_pair> pending_proofs;

  for(unsigned i = 0; i < _terms.size(); i++)
    {
      proof_state.insert(std::make_pair(_terms[i], _terms[i]));
      class_lists.insert(std::make_pair(_terms[i], expression_vector(1, _terms[i])));
    }

  pending_proofs.push(expression_pair(c1, c2));
  for(; !pending_proofs.empty() ; pending_proofs.pop())
    {
      const expression_pair & ccp = pending_proofs.front();
      expression c = find_common_ancestor(proof_state, ccp.first, ccp.second);
      explain_along_path(proof_state, class_lists,
			 expl_output, pending_proofs,
			 ccp.first, c);
      explain_along_path(proof_state, class_lists,
			 expl_output, pending_proofs,
			 ccp.second, c);
    }
}

void
euf_theory_solver::
generate_explanation(const std::vector<expression_pair> & expl_output,
		     explanation & expl)
{
  for(unsigned i = 0; i < expl_output.size(); i++)
    {
      const expression & le = expl_output[i].first;
      const expression & re = expl_output[i].second;
      expression eq;
#ifndef NDEBUG
      bool retval;
      retval = _solver.is_equality_cached(le, re, eq);
#else
      _solver.is_equality_cached(le, re, eq);
#endif
      assert(retval);
      assert(!eq->is_undefined());
      assert(_solver.has_literal_data(eq));
      assert(_solver.get_trail().is_true(eq));
      expl.push_back(std::move(eq));
    }
}

euf_theory_solver::lookup_key euf_theory_solver::get_lookup_key(const expression & exp)
{
  const function_symbol & fs = exp->get_symbol();
  const expression_vector & ops = exp->get_operands();
  expression_vector rops;
  rops.reserve(ops.size());
  for(unsigned i = 0; i < ops.size(); i++)
    {
      rops.push_back(_data.get_data(ops[i])->get_representative());
    }
  return lookup_key(fs, std::move(rops));
}

void euf_theory_solver::register_expression(const expression & exp)
{
  if(_data.has_data(exp))
    return;

  if(exp->is_function() && !exp->get_operands().empty())
    {
      const expression_vector & ops = exp->get_operands();
      for(unsigned i = 0; i < ops.size(); i++)
	{
	  register_expression(ops[i]);
	}
    }

  new_expression(exp);
}

expression euf_theory_solver::canonize_eq_diseq(const expression & l)
{
  assert(l->is_equality() || l->is_disequality());

  expression le = l->get_operands()[0];
  expression re = l->get_operands()[1];
  if(re < le)
    return _solver.get_factory()->create_expression(l->get_symbol(), re, le);
  else
    return l;
}

void euf_theory_solver::check_predicate_propagations(const expression & ra, const expression & rb)
{
  if(!_solver.has_literal_data(ra) || !_solver.has_literal_data(rb))
    return;
  
  const extended_boolean ra_value = _solver.get_trail().get_value(ra);
  const extended_boolean rb_value = _solver.get_trail().get_value(rb);
  expression rac = _solver.get_trail().get_source_theory_solver(ra) == this ? _predicate_propagation_causes[ra] : ra;
  expression rbc = _solver.get_trail().get_source_theory_solver(rb) == this ? _predicate_propagation_causes[rb] : rb;
  literal_data * rac_data = _solver.get_literal_data(rac);
  literal_data * rbc_data = _solver.get_literal_data(rbc);
  
  if(ra_value == EB_TRUE)
    {
      if(rb_value == EB_TRUE)
	return;
      else if(rb_value == EB_FALSE)
	{	  
	  explanation expl;
	  explain_conflict(rac, rbc, expl, rac, rbc_data->get_opposite());
	  _solver.apply_conflict(expl, this);
	  return;
	}
      else
	{
	  const expression_vector & clb = _data.get_data(rb)->get_class_list();
	  for(unsigned i = 0; i < clb.size(); i++)
	    if(_solver.has_literal_data(clb[i]))
	      {
		const extended_boolean clbi_value = _solver.get_trail().get_value(clb[i]);
		if(clbi_value == EB_UNDEFINED)
		  {
		    _predicate_propagation_causes[clb[i]] = rac;
		    _solver.apply_propagate(clb[i], this);
		  }
		else if(clbi_value == EB_FALSE)
		  {
		    explanation expl;
		    explain_conflict(rac, clb[i], expl, rac, _solver.get_literal_data(clb[i])->get_opposite());
		    _solver.apply_conflict(expl, this);
		    return;
		  }
	      }
	  return;
	}
    }
  else if(ra_value == EB_FALSE)
    {
      if(rb_value == EB_FALSE)
	return;
      else if(rb_value == EB_TRUE)
	{
	  explanation expl;
	  explain_conflict(rac, rbc, expl, rac_data->get_opposite(), rbc);
	  _solver.apply_conflict(expl, this);
	  return;
	}
      else
	{
	  const expression_vector & clb = _data.get_data(rb)->get_class_list();
	  for(unsigned i = 0; i < clb.size(); i++)
	    if(_solver.has_literal_data(clb[i]))
	      {
		const expression & op = _solver.get_literal_data(clb[i])->get_opposite();
		const extended_boolean op_value = _solver.get_trail().get_value(op);
		if(op_value == EB_UNDEFINED)
		  {
		    _predicate_propagation_causes[clb[i]] = rac;
		    _solver.apply_propagate(op, this);
		  }
		else if(op_value == EB_FALSE)
		  {
		    explanation expl;
		    explain_conflict(rac, clb[i], expl, rac_data->get_opposite(), clb[i]);
		    _solver.apply_conflict(expl, this);
		    return;
		  }
	      }
	  return;
	}
    }
  else // undefined
    {
      if(rb_value == EB_TRUE)
	{
	  const expression_vector & cla = _data.get_data(ra)->get_class_list();
	  for(unsigned i = 0; i < cla.size(); i++)
	    if(_solver.has_literal_data(cla[i]))
	      {
		const extended_boolean clai_value = _solver.get_trail().get_value(cla[i]);
		if(clai_value == EB_UNDEFINED)
		  {
		    _predicate_propagation_causes[cla[i]] = rbc;
		    _solver.apply_propagate(cla[i], this);
		  }
		else if(clai_value == EB_FALSE)
		  {
		    explanation expl;
		    explain_conflict(rbc, cla[i], expl, rbc, _solver.get_literal_data(cla[i])->get_opposite());
		    _solver.apply_conflict(expl, this);
		    return;
		  }
	      }
	  return;
	}
      else if(rb_value == EB_FALSE)
	{
	  const expression_vector & cla = _data.get_data(ra)->get_class_list();
	  for(unsigned i = 0; i < cla.size(); i++)
	    if(_solver.has_literal_data(cla[i]))
	      {
		const expression & op = _solver.get_literal_data(cla[i])->get_opposite();
		const extended_boolean op_value = _solver.get_trail().get_value(op);
		if(op_value == EB_UNDEFINED)
		  {
		    _predicate_propagation_causes[cla[i]] = rbc;
		    _solver.apply_propagate(op, this);
		  }
		else if(op_value == EB_FALSE)
		  {
		    explanation expl;
		    explain_conflict(rbc, cla[i], expl, rbc_data->get_opposite(), cla[i]);
		    _solver.apply_conflict(expl, this);
		    return;
		  }
	      }
	  return;
	}
      else
	return;
    }
}

bool euf_theory_solver::get_simplest_in_class(const expression & e, expression & sim)
{
  if(!_data.has_data(e) || !_data.get_data(e)->is_relevant())
    return false;

  const expression_vector & cl =
    _data.get_data(_data.get_data(e)->get_representative())->get_class_list();

  sim = cl[0];
  unsigned cpl = cl[0]->get_complexity();
  for(unsigned i = 1; i < cl.size(); i++)
    {
      unsigned cpli = cl[i]->get_complexity();
      if(cpli < cpl)
	{
	  cpl = cpli;
	  sim = cl[i];
	}
    }
  return true;
}

bool euf_theory_solver::check_literal_relevancy(const expression & e)
{
  if(check_term_relevancy(e))
    return true;

  if(!e->is_equality())
    return false;

  return check_term_relevancy(e->get_operands()[0]) && check_term_relevancy(e->get_operands()[1]);
}

bool euf_theory_solver::check_term_relevancy(const expression & e)
{
  if(e->is_equality() || e->is_disequality() || e->is_not())
    return false;

  if(_solver.has_literal_data(e) && _solver.get_literal_data(e)->is_negative())
    return false;

  if(e->is_quantifier())
    return false;
  
  common_data * e_data = _solver.get_common_data(e);
  
  if(e->is_constant_function() && e->is_formula() && !e_data->is_shared())
    return false;
  
  return true;   

  //return e_data->get_owner() == this || e_data->is_shared();

}

void euf_theory_solver::apply_trivial_propagation(const expression & l)
{
  const extended_boolean l_value = _solver.get_trail().get_value(l);
  if(l_value == EB_TRUE)
    return;
  else if(l_value == EB_UNDEFINED)
    _solver.apply_propagate(l, this);
  else if(l_value == EB_FALSE)
    {
      explanation conflicting;
      conflicting.push_back(_solver.get_literal_data(l)->get_opposite());
      _solver.apply_conflict(conflicting, this);
    }
}

void euf_theory_solver::process_assertion(const expression & l)
{
  literal_data * ldata = nullptr;
  
  if(l->is_equality())
    {
      const expression & left = l->get_operands()[0];
      const expression & right = l->get_operands()[1];

      merge(left, right);
    }
  else if(l->is_disequality())
    {
      const expression & left = l->get_operands()[0];
      const expression & right = l->get_operands()[1];

      if(are_congruent(left, right))
	{
	  explanation conflicting;
	  explain_conflict(left, right, conflicting, l);
	  _solver.apply_conflict(conflicting, this);
	}
      else
	{
	  const expression_vector & l_list = _data.get_data(_data.get_data(left)->get_representative())->get_class_list();
	  const expression_vector & r_list = _data.get_data(_data.get_data(right)->get_representative())->get_class_list();
	  check_diseq_propagations(l_list, r_list, l);
	}
    }
  else if((ldata = _solver.get_literal_data(l))->is_negative())
    {
      expression lp = ldata->get_opposite();
      _predicate_propagation_causes[lp] = lp;
      const expression_vector & lcl = _data.get_data(_data.get_data(lp)->get_representative())->get_class_list();
      for(unsigned i = 0; i < lcl.size(); i++)
	if(_solver.has_literal_data(lcl[i]))
	  {
	    const expression & op = _solver.get_literal_data(lcl[i])->get_opposite();
	    const extended_boolean op_value = _solver.get_trail().get_value(op);
	    if(op_value == EB_UNDEFINED)
	      {
		_predicate_propagation_causes[lcl[i]] = lp;
		_solver.apply_propagate(op, this);
	      }
	    else if(op_value == EB_FALSE)
	      {
		explanation expl;
		explain_conflict(lp, lcl[i], expl, lcl[i], l);
		_solver.apply_conflict(expl, this);
		return;
	      }
	  }
    }
  else
    {
      _predicate_propagation_causes[l] = l;
      const expression_vector & lcl = _data.get_data(_data.get_data(l)->get_representative())->get_class_list();
      for(unsigned i = 0; i < lcl.size(); i++)
	if(_solver.has_literal_data(lcl[i]))
	  {
	    const extended_boolean lcli_value = _solver.get_trail().get_value(lcl[i]);
	    if(lcli_value == EB_UNDEFINED)
	      {
		_predicate_propagation_causes[lcl[i]] = l;
		_solver.apply_propagate(lcl[i], this);
	      }
	    else if(lcli_value == EB_FALSE)
	      {
		explanation expl;
		explain_conflict(l, lcl[i], expl, _solver.get_literal_data(lcl[i])->get_opposite(), l);
		_solver.apply_conflict(expl, this);
		return;
	      }
	  }
    }
}

void euf_theory_solver::add_literal(const expression & l_pos, const expression & l_neg)
{
  if(check_literal_relevancy(l_pos))
    {
      _solver.get_literal_data(l_pos)->add_observing_theory_solver(this);
    }

  _new_literals.push_back(l_pos);
  _new_literals.push_back(l_neg);
}

void euf_theory_solver::new_level()
{
  _new_level_time_spent.start();
  _current_level++;
  _lookup_history.new_level();
  _new_level_time_spent.acumulate();
}


void euf_theory_solver::backjump(unsigned level)
{
  _backjump_time_spent.start();
  _current_level = level;
  for(unsigned i = 0; i < _terms.size(); i++)
    {
      _data.get_data(_terms[i])->restore_state();
    }
  _lookup_history.backjump(level, backjump_lookup_func(_lookup));
  _current_assertion_pos = std::min(_solver.get_trail().size(), _current_assertion_pos);
  _just_backjumped = true;
  _backjump_time_spent.acumulate();
}

void euf_theory_solver::check_and_propagate(unsigned layer)
{
  if(layer >= _num_of_layers)
    return;

  _check_and_prop_time_spent.start();

  for(unsigned i = 0; i < _new_literals.size(); i += 2)
    {
      expression l_pos = _new_literals[i];
      expression l_neg = _new_literals[i+1];

      register_expression(l_pos);
      register_expression(l_neg);

      if(_data.get_data(l_pos)->is_relevant() && l_pos->is_equality() && l_pos->get_operands()[0] == l_pos->get_operands()[1])
	{
	  _trivial_propagations.push_back(l_pos);
	  apply_trivial_propagation(l_pos);
	}
    }
  _new_literals.clear();

    if(_just_backjumped)
    {
      for(unsigned i = 0; !_solver.is_conflict() && i < _trivial_propagations.size(); i++)
	apply_trivial_propagation(_trivial_propagations[i]);
      _just_backjumped = false;
    }

  while(!_solver.is_conflict() && _current_assertion_pos < _solver.get_trail().size())
    {
      expression l = _solver.get_trail()[_current_assertion_pos++];

      if(!_solver.get_literal_data(l)->is_observing_theory_solver(this))
	continue;
	
      if(_solver.get_trail().get_source_theory_solver(l) == this)
	continue;
      
      process_assertion(l);
    }
  _check_and_prop_time_spent.acumulate();
}

void euf_theory_solver::explain_literal(const expression & l)
{
  _explain_time_spent.start();
  explanation expl;
  if(l->is_equality())
    {
      explain_equality(l, expl);
    }
  else if(l->is_disequality())
    {
      explain_disequality(l, expl);
    }
  else
    {
      explain_predicate(l, expl);
    }
  _solver.apply_explain(l, expl);
  _explain_time_spent.acumulate();
}

void euf_theory_solver::explain_equality(const expression & eq, explanation & expl)
{
  const expression & left = eq->get_operands()[0];
  const expression & right = eq->get_operands()[1];

  std::vector<expression_pair> expl_output;
  explain(left, right, expl_output);
  generate_explanation(expl_output, expl);
}

void euf_theory_solver::explain_disequality(const expression & diseq, explanation & expl)
{
  const dpc_entry & dpc = _disequality_propagation_causes[diseq];
  const expression & cdiseq = dpc.cdiseq;
  expression le = diseq->get_operands()[0];
  expression re = diseq->get_operands()[1];
  expression cle = cdiseq->get_operands()[0];
  expression cre = cdiseq->get_operands()[1];
  
  if(dpc.swapped)
    std::swap(cle, cre);      
  
  assert(_data.get_data(le)->get_representative() == _data.get_data(cle)->get_representative() &&
	 _data.get_data(re)->get_representative() == _data.get_data(cre)->get_representative());
  
  std::vector<expression_pair> expl_output;
  explain(le, cle, expl_output);
  explain(re, cre, expl_output);
  generate_explanation(expl_output, expl);
  expl.push_back(cdiseq);
}

void euf_theory_solver::explain_predicate(const expression & pred, explanation & expl)
{
  literal_data *ldata = _solver.get_literal_data(pred);
  expression e = ldata->is_negative() ? ldata->get_opposite() : pred;

  assert(_solver.has_literal_data(e) &&
	 !_solver.get_trail().is_undefined(e));

  const expression & rle = _predicate_propagation_causes[e]; 
  
  assert(rle.get() != 0 && _solver.has_literal_data(rle) &&
	 !_solver.get_trail().is_undefined(rle));

  std::vector<expression_pair> expl_output;
  explain(e, rle, expl_output);
  generate_explanation(expl_output, expl);
  if(_solver.get_trail().is_true(rle))
    expl.push_back(rle);
  else
    expl.push_back(_solver.get_literal_data(rle)->get_opposite());
}


void euf_theory_solver::explain_conflict(const expression & left, const expression & right, explanation & expl,
					 const expression & cflt1, const expression & cflt2)
{
  std::vector<expression_pair> expl_output;
  explain(left, right, expl_output);
  generate_explanation(expl_output, expl);
  expl.push_back(cflt1);
  if(cflt2.get() != nullptr)
    expl.push_back(cflt2);
}


expression euf_theory_solver::canonize_expression(const expression & e)
{
  expression ep;

  if(e->is_not())
    {
      ep = e->get_operands()[0];

      if(ep->is_equality())
	return _solver.get_factory()->create_expression(function_symbol::DISTINCT, ep->get_operands()[0], ep->get_operands()[1]);
      else if(ep->is_disequality())
	return _solver.get_factory()->create_expression(function_symbol::EQ, ep->get_operands()[0], ep->get_operands()[1]);
      else
	return e;
    }
  else
    {
      if(e->is_equality() || e->is_disequality())
	return canonize_eq_diseq(e);
      else
	return e;
    }
}


void euf_theory_solver::get_literal_pair(const expression & l,
					 expression & l_pos,
					 expression & l_neg)
{
  if(l->is_not())
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


void euf_theory_solver::get_model(const expression_vector & exps)
{
  for(unsigned i = 0; i < exps.size(); i++)
    {

      if(exps[i]->is_formula())
	continue;

      expression sim;
      if(get_simplest_in_class(exps[i], sim))
	exps[i]->set_assigned(sim);
    }
}
