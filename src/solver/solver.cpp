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

#include "solver.hpp"
#include "solver_observer.hpp"
#include <algorithm>
#include "logging_solver_observer.hpp"
#include "cmd_line_parser.hpp"

expression strip_double_negations(const expression & l)
{
  if(l->is_not() && l->get_operands()[0]->is_not())
    return strip_double_negations(l->get_operands()[0]->get_operands()[0]);
  else
    return l;
}

void solver::add_literals(const expression & l_pos, const expression & l_neg)
{
  if(!has_literal_data(l_pos))
    {
      
      literal_data * l_pos_data = new literal_data(l_pos, l_neg, _literals.size());
      literal_data * l_neg_data = new literal_data(l_pos_data);      
      set_literal_data(l_pos, l_pos_data);
      set_literal_data(l_neg, l_neg_data);      
      _literals.push_back(l_pos);
      _literals.push_back(l_neg);    
      _trail.add_literals(l_pos, l_neg);
      _conflict_set.add_literals(l_pos, l_neg);

      _state_changed = true;
    } 
}

void solver::clear_literals()
{
  for(unsigned i = 0; i < _literals.size(); i++)
    {
      clear_expression_data(_literals[i]);
    }
  _literals.clear();
}

void solver::init_expression_data(const expression & e)
{    
  if(e->has_expression_data())
    return;

  if(e->is_function() && !e->get_operands().empty())
    {
      for(expression_vector::const_iterator it = e->get_operands().begin(),
	    it_end = e->get_operands().end(); it != it_end; ++it)
	{
	  init_expression_data(*it);
	}
    }

  e->set_expression_data(new argosmt_expression_data(_extractor_counter));
  set_common_data(e, new common_data());
  
  common_data * e_common = get_common_data(e);

  //std::cout << "INITING: " << e << std::endl;
    
  // Equalities and disequalities have owner only if not mixed
  if(e->is_equality() || e->is_disequality())
    {
      const expression & le = e->get_operands()[0];
      const expression & re = e->get_operands()[1];
      common_data * le_common = get_common_data(le);
      common_data * re_common = get_common_data(re);
      if(le_common->get_owner() != re_common->get_owner())
	{
	  if(le_common->get_owner()->get_index() >
	     re_common->get_owner()->get_index())
	    le_common->set_shared(true);
	  else
	    re_common->set_shared(true);	  
	    
	  //le_common->set_shared(true);
	  //re_common->set_shared(true);
	  e_common->set_owner(nullptr);
	}
      else
	{
	  e_common->set_owner(le_common->get_owner());
	}	
    }
  // Negations ~p, ~p(e1,...,en)
  else if(e->is_not())
    {
      const expression & op = e->get_operands()[0];
      common_data * op_common = get_common_data(op);
      e_common->set_owner(op_common->get_owner());
    }
  // Quantifiers do not have owner
  else if(e->is_quantifier())
    {
      e_common->set_owner(nullptr);
    }
  else
    {    
      // Other terms should always have owner
      for(unsigned i = 0; i < _theory_solvers.size(); i++)
	{
	  if(_theory_solvers[i]->is_owned_expression(e))
	    {
	      e_common->set_owner(_theory_solvers[i]);
	      const expression_vector & ops = e->get_operands();
	      for(unsigned j = 0; j < ops.size(); j++)
		{
		  common_data * ops_common = get_common_data(ops[j]);
		  if(ops_common->get_owner() != _theory_solvers[i])
		    ops_common->set_shared(true);
		}
	      break;
	    }
	}
    }
}

void solver::clear_expression_data(const expression & e)
{
  delete e->get_expression_data();
  e->set_expression_data(nullptr);

  if(e->is_function() && !e->get_operands().empty())
    {
      for(expression_vector::const_iterator it = e->get_operands().begin(),
	    it_end = e->get_operands().end(); it != it_end; ++it)
	{
	  clear_expression_data(*it);
	}
    }
}

void solver::introduce_literal_without_sending(expression & l)
{
  expression l_pos;
  expression l_neg;

  l = canonize_literal(l);

  init_expression_data(l);

  if(!has_literal_data(l))
    {
      get_literal_pair(l, l_pos, l_neg);                 
      init_expression_data(l_pos);      
      init_expression_data(l_neg);
      add_literals(l_pos, l_neg);      
      if(l_pos->is_equality())
	cache_equality(l_pos->get_operands()[0], 
		       l_pos->get_operands()[1], 
		       l_pos);
      if(l_pos->is_quantifier())
	{
	  const sorted_variable_vector & svars = l_pos->get_quantified_variables();
	  for(const sorted_variable & svar : svars)
	    {
	      expression sc = _formula_transformer->get_sort_constant(svar.get_sort());
	      if(!sc->has_expression_data())
		{
		  init_expression_data(sc);
		  get_congruence_closure_theory_solver()->add_expression(sc);
		}
	    }	  
	}
    }
}


void solver::send_literals(const expression & l_pos, 
			   const expression & l_neg)
{
  if(get_literal_data(l_pos)->is_observed())
    return;
  
  for(unsigned i = 0; i < _observers.size(); i++)
    _observers[i]->literal_introduced(l_pos, l_neg);
  
  for(unsigned i = 0; i < _theory_solvers.size(); i++)
    _theory_solvers[i]->add_literal(l_pos, l_neg);

  for(unsigned i = 0; i < _q_processors.size(); i++)
    _q_processors[i]->add_literal(l_pos, l_neg);
}

void solver::add_initial_clause(clause * cl)
{
  _initial_clauses.push_back(cl);
  cl->set_learnt(false);
}

void solver::clear_initial_clauses()
{
  for(std::vector<clause *>::iterator it = _initial_clauses.begin(),
	it_end = _initial_clauses.end(); it != it_end; ++it)
    delete *it;
  
  _initial_clauses.clear();
}

void solver::add_learnt_clause(clause * cl)
{
  _learnt_clauses.push_back(cl);
  cl->set_learnt(true);
  cl->set_index(_learnt_clauses_counter++);
}

void solver::clear_learnt_clauses()
{
  for(std::vector<clause *>::iterator it = _learnt_clauses.begin(),
	it_end = _learnt_clauses.end(); it != it_end; ++it)
    delete *it;
  
  _learnt_clauses.clear();
}

unsigned solver::get_first_unlocked_index()
{
  unsigned first_unlocked = 0;
  for(unsigned i = 0; i < _learnt_clauses.size(); i++)
    {
#ifndef _TRAIL_SAVING
      if(_learnt_clauses[i]->is_locked())
#else
	if(_learnt_clauses[i]->is_locked() || _learnt_clauses[i]->get_expl_obj_count() > 0)
#endif
	{
	  clause * temp = _learnt_clauses[first_unlocked];
	  _learnt_clauses[first_unlocked] = _learnt_clauses[i];
	  _learnt_clauses[i] = temp;
	  first_unlocked++;
	}
    }
  return first_unlocked;
}

expression solver::canonize_literal_rec(const expression & e)
{
  expression ep;
  if(e->is_function() && !e->get_operands().empty())
    {
      expression_vector c_ops;
      c_ops.reserve(e->get_operands().size());
      for(expression_vector::const_iterator it = e->get_operands().begin(),
	    it_end = e->get_operands().end(); it != it_end; ++it)
	{
	  c_ops.push_back(canonize_literal_rec(*it));
	}
      ep = _factory->create_expression(e->get_symbol(), std::move(c_ops));
    }
  else
    ep = e;
  
  if(ep->is_quantifier())
    {
      sorted_variable_vector svars;
      quantifier quant = ep->get_quantifier();
      expression op = ep;
      do {
	const sorted_variable_vector & op_svars = op->get_quantified_variables();
	std::copy(op_svars.begin(), op_svars.end(), std::back_inserter(svars));
	op = op->get_subexpression();
      } while(op->is_quantifier() && op->get_quantifier() == quant);
      std::sort(svars.begin(), svars.end());
      return _factory->create_expression(quant, svars, strip_double_negations(op));
    }

  // ~forall x.P(x) <=> exists x.~P(x)
  // ~exists x.P(x) <=> forall x.~P(x)
  if(ep->is_not() && ep->get_operands()[0]->is_quantifier())
    {
      auto opposite_quant = [] (quantifier quant)
			    {
			      switch(quant)
				{
				case Q_FORALL:
				  return Q_EXISTS;
				case Q_EXISTS:
				  return Q_FORALL;
				default:
				  return Q_UNDEFINED;
				}
			    };
      return _factory->create_expression(opposite_quant(ep->get_operands()[0]->get_quantifier()),
					 ep->get_operands()[0]->get_quantified_variables(),
					 strip_double_negations(_factory->
								create_expression(function_symbol::NOT,
										  ep->get_operands()[0]->
										  get_subexpression())));      
    }

  
  for(unsigned i = 0; i < _theory_solvers.size(); i++)
    ep = _theory_solvers[i]->canonize_expression(ep);
  return ep;
}

expression solver::canonize_literal(const expression & e)
{
  if(e->has_expression_data())
    return e;

  return canonize_literal_rec(e);
}


void solver::get_literal_pair(const expression & l, 
			      expression & l_pos,
			      expression & l_neg)
{

  if(l->is_equality())
    {
      l_pos = l;
      l_neg = _factory->create_expression(function_symbol::DISTINCT,
					  l->get_operands()[0], 
					  l->get_operands()[1]);
    }
  else if(l->is_disequality())
    {
      l_neg = l;
      l_pos = _factory->create_expression(function_symbol::EQ,
					  l->get_operands()[0], 
					  l->get_operands()[1]);
    }
  else if(l->is_quantifier())
    {
      if(l->get_quantifier() == Q_FORALL)
	{
	  l_pos = l;
	  l_neg = _factory->create_expression(Q_EXISTS,
					      l->get_quantified_variables(),
					      strip_double_negations(_factory->
								     create_expression(function_symbol::NOT,
										       l->get_subexpression())));
	}
      else // l->get_quantifier() == Q_EXISTS
	{
	  l_neg = l;
	  l_pos = _factory->create_expression(Q_FORALL,
					      l->get_quantified_variables(),
					      strip_double_negations(_factory->
								     create_expression(function_symbol::NOT,
										       l->get_subexpression())));
	}
    }
  else
    {
      common_data * l_common = get_common_data(l);
      assert(l_common != nullptr);
      assert(l_common->get_owner() != nullptr);
      l_common->get_owner()->get_literal_pair(l, l_pos, l_neg);
    }
}

bool solver::is_equality_cached(const expression & le, 
				const expression & re,
				expression & eq)
{
  expression_pair_to_expression_map::iterator it = 
    _mapped_equalities.find(expression_pair(le, re));
  
  if(it != _mapped_equalities.end())
    {
      eq = it->second;
      return true;
    }
  else
    return false;
}


void solver::apply_introduce_literal(expression & l)
{
  introduce_literal_without_sending(l);
  expression l_pos, l_neg;
  literal_data * l_data = get_literal_data(l);
  l_pos = l_data->get_positive();
  l_neg = l_data->get_negative();
  send_literals(l_pos, l_neg);
}

bool solver::apply_introduce_shared_equality(const expression & le, 
					     const expression & re,
					     expression & eq,
					     int static_branching_hint,
					     int dynamic_branching_hint,
					     bool next_time_branch,
					     int polarity_hint,
					     bool add_to_polarity_hint)
{
  if(is_equality_cached(le, re, eq))
    {
      return !eq->is_undefined();
    }
  else
    {
      if(get_common_data(le)->is_shared() && get_common_data(re)->is_shared())
	{
	  eq = _factory->create_expression(function_symbol::EQ, le, re);
	  introduce_literal_without_sending(eq);
	  literal_data * eq_data = get_literal_data(eq);
	  const expression & neq = eq_data->get_opposite();
	  send_literals(eq, neq);
	  _literal_selection_strategy->branching_priority_hint(eq, static_branching_hint, dynamic_branching_hint, next_time_branch);
	  _polarity_selection_strategy->preffered_polarity_hint(eq, polarity_hint, add_to_polarity_hint);
	  return true;
	}
    
      eq = _factory->UNDEFINED_EXPRESSION();
      cache_equality(le, re, eq);
      return false;
    }
}

void solver::initialize_solver()
{
  // Traversing initial clauses, canonizing and introducing literals
  // Counting literal occurrences in initial clauses
  for(unsigned i = 0; i < _initial_clauses.size(); i++)
    {
      unsigned k = 0;
      std::unordered_set<expression> cache;
      for(unsigned j = 0; j < _initial_clauses[i]->size(); j++)
	{
	  expression & l = (*_initial_clauses[i])[j];
	  l = strip_double_negations(l);
	  introduce_literal_without_sending(l);
	  if(cache.find(l) != cache.end())
	    continue;
	  cache.insert(l);
	  literal_data * l_data = get_literal_data(l);
	  l_data->increment_occurrence_counter();
	  (*_initial_clauses[i])[k++] = l;
	}
      _initial_clauses[i]->resize(k);      
    }

  // Notifying observers and theory_solvers about the introduced literals
  for(unsigned i = 0; i < _literals.size(); i+=2)
    {      
      send_literals(_literals[i], _literals[i + 1]);
    }
  //  std::cout << "SOLVER INITED" << std::endl;
}

void solver::initialize_solver_dimacs()
{

  // Initialize DIMACS literals
  for(unsigned i = 1; i <= _num_of_vars; i++)
    {
      expression l_pos = _factory->create_expression(i, SLP_POSITIVE);
      expression l_neg = _factory->create_expression(i, SLP_NEGATIVE);
      init_expression_data(l_pos);
      init_expression_data(l_neg);
      add_literals(l_pos, l_neg);
    }


  // Counting occurrences of literals in initial clauses
  for(unsigned i = 0; i < _initial_clauses.size(); i++)
    {
      for(unsigned j = 0; j < _initial_clauses[i]->size(); j++)
	{	  
	  expression & l = (*_initial_clauses[i])[j];
	  literal_data * l_data = get_literal_data(l);
	  l_data->increment_occurrence_counter();
	}
    }


  // Notifying observers and Bool theory about the introduced literals
  for(unsigned i = 0; i < _literals.size(); i+=2)
    {
      send_literals(_literals[i], _literals[i + 1]);
    }
}


void solver::apply_decide(const expression & l)
{
  _trail.new_level();
   
  for(unsigned i = 0; i < _theory_solvers.size(); i++)
    _theory_solvers[i]->new_level();

  for(unsigned i = 0; i < _q_processors.size(); i++)
    _q_processors[i]->new_level();
  
  _trail.push(l, 0);
  _state_changed = true;
  
  for(unsigned i = 0; i < _observers.size(); i++)
    _observers[i]->decide_applied(l);

#ifdef _TRAIL_SAVING
  _ts_all_decides_count++;
#endif
}

void solver::apply_propagate(const expression & l, theory_solver * source_ts)
{
  if(_conflict_set.is_conflict())
    return;

  assert(_trail.is_undefined(l));

  _trail.push(l, source_ts);
  _state_changed = true; 
  
  for(unsigned i = 0; i < _observers.size(); i++)
    _observers[i]->propagate_applied(l, source_ts);

#ifdef _TRAIL_SAVING
  _ts_all_props_count++;
#endif
}

void solver::apply_conflict(const explanation & conflicting, theory_solver * conflict_ts)
{
  if(_conflict_set.is_conflict())
    return;

  _conflict_set.set_conflict();
  _state_changed = true;
  _conflict_set.calculate_last_level(conflicting);
  _conflict_set.add_new_literals(conflicting);
  
  for(unsigned i = 0; i < _observers.size(); i++)
    _observers[i]->conflict_applied(conflicting, conflict_ts);
#ifdef _TRAIL_SAVING
  _ts_all_cflts_count++;
#endif
}

void solver::apply_explain(const expression & l, const explanation & expl)
{
  assert(_trail.get_trail_level(l) <= _conflict_set.last_level());
  
#ifndef NDEBUG
  for(unsigned i = 0; i < expl.size(); i++)
    {
      assert(_trail.get_trail_index(expl[i]) < _trail.get_trail_index(l));
    }
#endif

  if(_trail.get_trail_level(l) < _conflict_set.last_level())
    {
      if(_conflict_set.check_subsumed(l, expl))
	{
	  //std::cout << "SUBSUME: " << l << std::endl;
	  _conflict_set.subsume(l);
	  for(unsigned i = 0; i < _observers.size(); i++)
	    _observers[i]->explain_applied(l, expl);
	}
    }
  else 
    {	
      _conflict_set.add_new_literals(expl);
      for(unsigned i = 0; i < _observers.size(); i++)
	_observers[i]->explain_applied(l, expl);
    }
}

void solver::apply_theory_lemma(clause * cl)
{
  add_learnt_clause(cl);
  cl->set_locked(true); // don't forget theory-learned clauses
  _state_changed = true;

  for(unsigned i = 0; i < _observers.size(); i++)
    _observers[i]->learn_applied(cl);

  get_clause_theory_solver()->add_clause(cl);
}

void solver::apply_backjump()
{
  assert(_conflict_set.last_level() != 0);

  unsigned level = 
    _conflict_set.backjump_level();
  expression uip_literal = _conflict_set.uip_literal();
  clause * learnt_clause = _conflict_set.get_clause();

  assert(level < _trail.current_level());
  
#ifdef _TRAIL_SAVING
  save_trail(level);
#endif
  
  _trail.backjump(level);
  _conflict_set.reset_conflict();
  add_learnt_clause(learnt_clause); 
  _state_changed = true;
   
  for(unsigned i = 0; i < _observers.size(); i++)
    _observers[i]->backjump_applied(_trail.current_level(), 
				    learnt_clause,    
				    get_literal_data(uip_literal)->
				    get_opposite());
  
  for(unsigned i = 0; i < _theory_solvers.size(); i++)
    {
      _theory_solvers[i]->backjump(_trail.current_level());
    }

  for(unsigned i = 0; i < _q_processors.size(); i++)
    _q_processors[i]->backjump(_trail.current_level());
  
  get_clause_theory_solver()->add_clause(learnt_clause);

#ifdef _TRAIL_SAVING
  _ts_all_backtracks_count++;

  if(_saved_trail.size() > _literals.size())
    {
      filter_saved_trail();
    }
#endif  
}

void solver::apply_forget(unsigned number_of_clauses)
{
  if(number_of_clauses == 0)
    return;
  
  _state_changed = true;
  
  for(unsigned i = 0; i < _observers.size(); i++)
    _observers[i]->forget_applied(_learnt_clauses, number_of_clauses);

  get_clause_theory_solver()->remove_clauses(_learnt_clauses, number_of_clauses);
  
  for(unsigned i = _learnt_clauses.size() - number_of_clauses; 
      i < _learnt_clauses.size(); i++)
    {
      delete _learnt_clauses[i];
    }  
  _learnt_clauses.resize(_learnt_clauses.size() - number_of_clauses);
}

void solver::apply_restart()
{
  assert(_trail.current_level() != 0);

#ifdef _TRAIL_SAVING
  save_trail(0);
#endif

  
  _trail.backjump(0);
  _state_changed = true; 
  
  for(unsigned i = 0; i < _observers.size(); i++)
    _observers[i]->restart_applied();
  
  for(unsigned i = 0; i < _theory_solvers.size(); i++)
    _theory_solvers[i]->backjump(0);

  for(unsigned i = 0; i < _q_processors.size(); i++)
    _q_processors[i]->backjump(0);


#ifdef _TRAIL_SAVING
  if(_saved_trail.size() > _literals.size())
    {
      filter_saved_trail();
    }
#endif  
}


void solver::skolemize(const expression & l)
{
  static const std::string sk("sk");
  // exists x1...xn. F(x1,....,xn)
  assert(l->is_quantifier() && l->get_quantifier() == Q_EXISTS);

  literal_data * ldata = get_literal_data(l);
  if(ldata->is_skolemized())
    return;
  
  // std::cout << "SKOLEMIZING: " << l << std::endl;
  
  // create unique constants c1,...,cn
  const sorted_variable_vector & svars = l->get_quantified_variables();  
  substitution sub;
  for(const sorted_variable & svar : svars)
    sub.insert(std::make_pair(svar.get_variable(), _formula_transformer->get_unique_constant(sk, svar.get_sort())));
   
  expression f = l->get_subexpression()->get_instance(sub);

  //  std::cout << "OBTAINED: " << f << std::endl;

  ldata->set_skolemized(true);
  
  // ~exists x1...xn.F(x1,...,xn) \/ F(c1,...,cn)
  // forall x1...xn.~F(x1,...,xn) \/ F(c1,...,cn)
  
  // cnf_transformation of F(c1,...,cn) -> name, def_clauses
  expression name;
  std::vector<clause *> clauses;
  _formula_transformer->cnf_transformation(f, clauses, name);

  // forall x1...xn.~F(x1,...,xn) \/ name /\ (def_clauses)

  apply_introduce_literal(name);
  clause * cl = new clause();  
  cl->push_back(get_literal_data(l)->get_opposite());
  cl->push_back(name);

  apply_theory_lemma(cl);
  // ... Introducing new literals in clauses ...
  for(unsigned i = 0; i < clauses.size(); i++)
    {
      std::unordered_set<expression> cache;
      unsigned k = 0;
      for(unsigned j = 0; j < clauses[i]->size(); j++)
	{
	  expression & l = (*clauses[i])[j];
	  l = strip_double_negations(l);
	  apply_introduce_literal(l);
	  if(cache.find(l) != cache.end())
	    continue;
	  cache.insert(l);
	  (*clauses[i])[k++] = l;
	}
      clauses[i]->resize(k);
      apply_theory_lemma(clauses[i]);
    }
}

void solver::instantiate(const expression & l, const expression_vector & gterms)
{
  // forall x1...xn. F(x1,...,xn)
  assert(l->is_quantifier() && l->get_quantifier() == Q_FORALL);
    
  const sorted_variable_vector & svars = l->get_quantified_variables();
  assert(svars.size() == gterms.size());
  substitution sub;
  for(unsigned i = 0; i < svars.size(); i++)
    sub.insert(std::make_pair(svars[i].get_variable(), gterms[i]));

  expression f = l->get_subexpression()->get_instance(sub);

  literal_data * ldata = get_literal_data(l);
  if(ldata->is_instantiated(f))
    return;
  
  /*  std::cout << "INSTANTIATING: " << l << " with: ";
  for(const auto & t : gterms)
    std::cout << t << " , ";
  std::cout << std::endl;
  std::cout << "OBTAINED: " << f << std::endl;
  */

  
  ldata->add_instantiation(f);
  
  
  // exists x1...xn. ~F(x1,...,xn) \/ F(t1,...,tn)

  // cnf_transformation of F(t1,...,tn) -> name, def_clauses
  expression name;
  std::vector<clause *> clauses;
  _formula_transformer->cnf_transformation(f, clauses, name);
  
  // exists x1...xn. ~F(x1,...,xn) \/ name /\ (def_clauses)
  apply_introduce_literal(name);
  clause * cl = new clause();
  cl->push_back(get_literal_data(l)->get_opposite());
  cl->push_back(name);
  apply_theory_lemma(cl);
  // ... Introducing new literals in clauses ...
  for(unsigned i = 0; i < clauses.size(); i++)
    {
      std::unordered_set<expression> cache;
      unsigned k = 0;
      for(unsigned j = 0; j < clauses[i]->size(); j++)
	{
	  expression & l = (*clauses[i])[j];
	  l = strip_double_negations(l);
	  apply_introduce_literal(l);
	  if(cache.find(l) != cache.end())
	    continue;
	  cache.insert(l);
	  (*clauses[i])[k++] = l;
	}
      clauses[i]->resize(k);
      apply_theory_lemma(clauses[i]);
    }
}


check_sat_response solver::solve()
{
  if(!_dimacs)
    initialize_solver();
  else
    initialize_solver_dimacs();
  
  unsigned decide_count = 0;
  unsigned num_of_layers = 0;
  unsigned quantifier_instantiation_count = 0;
  for(unsigned i = 0; i < _theory_solvers.size(); i++)
    num_of_layers = std::max(num_of_layers, _theory_solvers[i]->get_num_of_layers());
  
  unsigned layer = 0;
  while(true)
    {
      unsigned i = 0;
      
      _check_and_prop_time_spent.start();
      do
	{	  
	  _state_changed = false;

	  _theory_solvers[i]->check_and_propagate(layer);

	  if(_state_changed && (i != 0 || layer != 0))
	    i = layer = 0;
	  else
	    i++;
	}
      while(!_conflict_set.is_conflict() && i < _theory_solvers.size());
      _check_and_prop_time_spent.acumulate();

      if(!_conflict_set.is_conflict())
	{
	  if(++layer < num_of_layers)
	    continue;
	  else
	    layer = 0;       
	}
      else
	{
	  // Resolving literal from current level
	  _explain_time_spent.start();
	  while(!_conflict_set.all_explained())
	    {
	      expression l = _conflict_set.next_to_explain();	     
	      _trail.get_source_theory_solver(l)->explain_literal(l);
	    }
	  _explain_time_spent.acumulate();

	  //Removing subsumed literals
	  _subsume_time_spent.start();
	  const expression_vector & conflicting = _conflict_set.get_conflicting();
	  for(unsigned j = 0; j < conflicting.size(); j++)
	    {
	      theory_solver * source_ts = _trail.get_source_theory_solver(conflicting[j]);	      
	      // Subsumtion applied only to BCP-infered literals. 
	      if(source_ts == _theory_solvers[0])
	  	source_ts->explain_literal(conflicting[j]);	      
	    }
	  _subsume_time_spent.acumulate();
	  assert(_conflict_set.last_level() != 0 || conflicting.empty());
	  // Backjump or report UNSAT
	  if(_conflict_set.last_level() != 0)
	    {
	      _backjump_time_spent.start();
	      apply_backjump();
	      _backjump_time_spent.acumulate();
	      layer = 0;
	      continue;
	    }
	  else
	    break;
	}
      
      if(_trail.current_level() > 0 && _restart_strategy->should_restart())
	{ 
	  apply_restart();
	  continue;
	}
      
      if(_forget_strategy->should_forget())
	{
	  unsigned start_index = get_first_unlocked_index();
	  unsigned number_of_clauses = _forget_selection_strategy->
	    number_of_clauses_to_forget(_learnt_clauses, start_index);      
	  apply_forget(number_of_clauses);
	}

      expression decision_literal;
      _heuristic_time_spent.start();
#ifdef _TRAIL_SAVING  
      confirm_saved_propagations();
      expression dliteral;
      if(_ts_use_lookahead)
	{
	  if(_should_check_forced_decides)
	    saved_conflict_lookahead();
      
	  while(!_ts_force_decides.empty() && _trail.get_value(_ts_force_decides[0]) == EB_TRUE)
	    _ts_force_decides.pop_front();
      	  
	  if(!_ts_force_decides.empty() && _trail.get_value(_ts_force_decides[0]) == EB_UNDEFINED)
	    {
	      dliteral = _ts_force_decides[0];
	      _ts_force_decides.pop_front();
	      _ts_lookahead_decides_count++;
	    }
	  //	  else
	  // _ts_force_decides.clear();
	}

      if(dliteral.get() == nullptr)
	{
	  if(_literal_selection_strategy->select_decision_literal(decision_literal))
	    dliteral = _polarity_selection_strategy->get_literal(decision_literal);
	}
      
      if(dliteral.get() != nullptr)
	{
#else
      if(_literal_selection_strategy->select_decision_literal(decision_literal))
	{
	  expression dliteral = _polarity_selection_strategy->get_literal(decision_literal);
#endif
	  _heuristic_time_spent.acumulate();
	  _decide_time_spent.start();
	  apply_decide(dliteral);
	  _decide_time_spent.acumulate();
	  if(++decide_count >= cmd_line_parser::decide_limit() && cmd_line_parser::decide_limit() != 0)
	    return CSR_UNKNOWN;
	}
      else
	{	  
	  _heuristic_time_spent.acumulate();
	  _state_changed = false;
	  for(unsigned k = 0; k < _q_processors.size(); k++)
	    {
	      _q_processors[k]->check_quantifiers();
	      if(_state_changed)
		break;
	    }
	  if(!_state_changed)	  
	    break;

	  if(++quantifier_instantiation_count >= _quantifier_instantiation_count_limit)
	    return CSR_UNKNOWN;
	}
    }
  
  return !_conflict_set.is_conflict() ? CSR_SAT : CSR_UNSAT;
}

#ifdef _TRAIL_SAVING

void print_deque(const std::deque<expression> & d)
{
  for(unsigned i = 0; i < d.size(); i++)
    std::cout << d[i] << ", ";
  std::cout << std::endl;
}

unsigned calculate_explanation_lbd(const explanation & expl, const trail & tr)
{
  static std::unordered_set<unsigned> levels;
  levels.clear();
  for(unsigned i = 0; i < expl.size(); i++)
    levels.insert(tr.get_trail_level(expl[i]));
  //std::cout << "LBD: " << levels.size() << ", SIZE: " << expl.size() << std::endl;
  return levels.size();
}

bool solver::use_saved_trail()
{
  while(_ts_pivot < _saved_trail.size())
    {
      const expression & l = _saved_trail[_ts_pivot];
      literal_data * l_data = get_literal_data(l);
      if(!l_data->has_saved_explanation())
	{
	  if(_trail.get_value(l) == EB_TRUE)
	    _ts_pivot++;
	  else
	    return true;
	}
      else
	{
	  extended_boolean lv = _trail.get_value(l);
	  if(lv == EB_TRUE)
	    _ts_pivot++;
	  else if(lv == EB_FALSE)
	    {
	      const explanation & l_expl = l_data->get_saved_explanation();
	      explanation cflt_expl(nullptr, nullptr);
	      if(l_expl.is_clause_explanation())
		{
		  clause * cl = l_expl.get_clause();
		  cflt_expl = explanation(this, cl);
		}
	      else
		{
		  cflt_expl = explanation();
		  for(unsigned i = 0; i < l_expl.size(); i++)
		    cflt_expl.push_back(l_expl[i]);
		  cflt_expl.push_back(l_data->get_opposite());
		}
	      apply_conflict(cflt_expl, &_ts_explainer);
	      _ts_saved_cflts_count++;
	      return false;
	    }
	  else 
	    {
	      const explanation & l_expl = l_data->get_saved_explanation();
	      if(!l_expl.is_clause_explanation() ||
		 ((!_ts_use_lbd || calculate_explanation_lbd(l_expl, _trail) <= _ts_lbd_limit)
		  && (!_ts_use_size || l_expl.size() <= _ts_size_limit)))
		{
		  apply_propagate(l, &_ts_explainer);
		  _ts_pivot++;
		  
		  _ts_saved_props_count++;
		}
	      else
		return true;
	    }
	}     
    }
  
  return true;
}

void solver::save_trail(unsigned level)
{
  if(_ts_use_lookahead)
    {
      _ts_force_decides.clear();
      _should_check_forced_decides = true;
    }
      
  if(_trail.current_level() == _critical_level)
    {
      _ts_crit_backtracks_count++;
      reset_saved_trail();
    }
  
  if(level < _trail.current_level() - 1)
    {
      _ts_save_count++;
      _ts_saved_levels_count += _trail.current_level() - level - 1;
      if(!_saved_trail.empty())
	_ts_append_count++;
    }
  else
    _ts_shallow_backtracks_count++;

  
  if(_critical_level == (unsigned)(-1) || _trail.current_level() - level > 1)
    _critical_level = level;

  _ts_saved_lits_count += _trail.last_level_start() - _trail.level_start(level + 1);
  
  for(int i = _trail.last_level_start() - 1; i >= (int)_trail.level_start(level + 1); i--)
    {
      const expression & l = _trail[i];
      literal_data * l_data = get_literal_data(l);
      
      theory_solver * ts = _trail.get_source_theory_solver(l);
      if(ts != nullptr)
	{
	  explanation expl = ts->get_literal_explanation(l);	  
	  l_data->set_saved_explanation(std::move(expl));
	}
      else
	l_data->reset_saved_explanation();
      _saved_trail.push_front(l);      
    }
  _ts_pivot = 0;
}

void solver::reset_saved_trail()
{ 
  if(!_saved_trail.empty())
    _ts_reset_count++;

  for(unsigned i = 0; i < _saved_trail.size(); i++)
    {
      literal_data * l_data = get_literal_data(_saved_trail[i]);
      if(l_data->has_saved_explanation() &&
	 (_trail.get_value(_saved_trail[i]) != EB_TRUE ||
	  _trail.get_source_theory_solver(_saved_trail[i]) != &_ts_explainer))	
	l_data->reset_saved_explanation();      
    }
  _saved_trail.clear();
  _ts_pivot = 0;
  _critical_level = (unsigned)(-1);  
}

void solver::confirm_saved_propagations()
{
  if(_ts_pivot > 0)
    {
      _critical_level = _trail.current_level();


      if(_ts_use_lookahead && _ts_force_decides.empty())
	_should_check_forced_decides = true;
      
      for(unsigned i = 0; i < _ts_pivot; i++)
	{
	  literal_data * l_data = get_literal_data(_saved_trail[i]);
	  if(l_data->has_saved_explanation() && _trail.get_source_theory_solver(_saved_trail[i]) != &_ts_explainer)	
	    l_data->reset_saved_explanation();
	}
      _saved_trail.erase(_saved_trail.begin(), _saved_trail.begin() + _ts_pivot);
      _ts_pivot = 0;
    }
  assert(_saved_trail.size() == 0 || !get_literal_data(_saved_trail[0])->has_saved_explanation());
}

void solver::filter_saved_trail()
{
  _ts_filter_count++;
  
  std::unordered_map<expression, extended_boolean> _saved_values;

  unsigned i,k;
  for(i = 0, k = 0; i < _saved_trail.size(); i++)
    {
      const expression & lit = _saved_trail[i];
      literal_data * l_data = get_literal_data(lit);
      const expression & var = l_data->get_positive();
      extended_boolean saved_var_eb = EB_UNDEFINED;
      auto it = _saved_values.find(var);
      if(it != _saved_values.end())
	saved_var_eb = it->second;
      extended_boolean saved_lit_eb = l_data->is_positive() ? saved_var_eb : opposite_value(saved_var_eb);
      if(saved_lit_eb == EB_TRUE)
	continue;

      _saved_trail[k++] = lit;
      
      if(saved_lit_eb == EB_FALSE)
	break;
      
      _saved_values[var] = l_data->is_positive() ? EB_TRUE : EB_FALSE;

    }
  
  for(unsigned j = i + 1; j < _saved_trail.size(); j++)
    {
      literal_data * l_data = get_literal_data(_saved_trail[j]);
      extended_boolean saved_lit_eb = EB_UNDEFINED;
      auto it = _saved_values.find(l_data->get_positive());
      if(it != _saved_values.end())
	saved_lit_eb = l_data->is_positive() ? it->second : opposite_value(it->second);
      
      if(l_data->has_saved_explanation() &&
	 saved_lit_eb != EB_TRUE && _saved_trail[j] != _saved_trail[k - 1] && 
	 (_trail.get_value(_saved_trail[j]) != EB_TRUE ||
	  _trail.get_source_theory_solver(_saved_trail[j]) != &_ts_explainer))	
	l_data->reset_saved_explanation();      
    }
  _saved_trail.resize(k);
}

void solver::saved_conflict_lookahead()
{
  unsigned dec_count = 0;
  _should_check_forced_decides = false;
  for(unsigned i = 0; i < _saved_trail.size(); i++)
    {
      extended_boolean eb = _trail.get_value(_saved_trail[i]);
      if(!get_literal_data(_saved_trail[i])->has_saved_explanation())
	{
	  if(eb == EB_FALSE)
	    {
	      _ts_force_decides.clear();
	      return;
	    }

	  if(eb == EB_UNDEFINED)
	    {
	      if(++dec_count > _ts_lookahead_levels)
		{
		  _ts_force_decides.clear();
		  return;
		}
	      _ts_force_decides.push_back(_saved_trail[i]);
	    }	  
	  
	  continue;
	}

      if(_trail.get_value(_saved_trail[i]) == EB_FALSE)
	return;
    }
}
#endif

bool solver::verify_assignment()
{
  for(unsigned i = 0; i < _theory_solvers.size(); i++)
    if(!_theory_solvers[i]->verify_assignment(_trail))
      {
	std::cout << "THEORY: " << _theory_solvers[i]->get_name() << " DID NOT AKNOWLEDGE THE MODEL" << std::endl;
	return false;
      }
  return true;
  }

void solver::get_model(const expression_vector & exps)
{
  for(unsigned i = 0; i < _theory_solvers.size(); i++)
    _theory_solvers[i]->get_model(exps);
}

void solver::print_reports(std::ostream & ostr)
{
    ostr << "-------- SOLVER OVERALL ---------" << std::endl;
    ostr << "Time spent in check_and_propagate: " << _check_and_prop_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in heuristics: " << _heuristic_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in decide: " << _decide_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in backjump: " << _backjump_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in explain: " << _explain_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in subsume: " << _subsume_time_spent.cumulative_time() << std::endl;
    ostr << "--------------------------------" << std::endl;
  for(unsigned i = 0; i < _theory_solvers.size(); i++)
      _theory_solvers[i]->print_report(ostr);   
}

solver::~solver()
{
#ifdef _TRAIL_SAVING  
  std::cout << "================== TRAIL SAVING STATISTICS =======================" << std::endl;
  std::cout << "All propagations count: " << _ts_all_props_count << std::endl;
  std::cout << "Saved propagations count: " << _ts_saved_props_count << std::endl;
  std::cout << "Percent of saved propagations: " << ((double)_ts_saved_props_count / _ts_all_props_count * 100) << std::endl;  
  std::cout << "All conflicts count: " <<  _ts_all_cflts_count << std::endl;
  std::cout << "Saved conflicts count: " << _ts_saved_cflts_count << std::endl;
  std::cout << "Percent of saved conflicts: " << ((double)_ts_saved_cflts_count / _ts_all_cflts_count * 100) << std::endl;
  std::cout << "Save trail count: " <<  _ts_save_count << std::endl;
  std::cout << "Saved levels cumulative: " <<  _ts_saved_levels_count << std::endl;
  std::cout << "Average saved levels: " << ((double) _ts_saved_levels_count / _ts_save_count) << std::endl;
  std::cout << "Saved literals cumulative: " << _ts_saved_lits_count << std::endl;
  std::cout << "Average saved literals: " << ((double) _ts_saved_lits_count / _ts_save_count) << std::endl;
  std::cout << "Saved trail append count: " <<  _ts_append_count << std::endl;
  std::cout << "Saved trail reset count: " <<  _ts_reset_count << std::endl;
  std::cout << "Saved trail filter count: " <<  _ts_filter_count << std::endl;
  std::cout << "Critical backjumps count: " <<  _ts_crit_backtracks_count << std::endl;
  std::cout << "All backjumps count: " <<  _ts_all_backtracks_count << std::endl;
  std::cout << "Shallow backjumps count: " <<  _ts_shallow_backtracks_count << std::endl;
  std::cout << "Percent of shallow backjumps: " << ((double)_ts_shallow_backtracks_count / _ts_all_backtracks_count * 100) << std::endl;
  std::cout << "Percent of critical backjumps: " << ((double)_ts_crit_backtracks_count / _ts_all_backtracks_count *100) << std::endl;
  std::cout << "All decides count: " << _ts_all_decides_count << std::endl;
  std::cout << "Lookahead decides count: " << _ts_lookahead_decides_count << std::endl;
  std::cout << "Percent of lookahead decides: " << ((double)_ts_lookahead_decides_count / _ts_all_decides_count * 100) << std::endl;
  std::cout << "=================================================================" << std::endl;
#endif
  
  if(dynamic_cast<solver_observer *>(_literal_selection_strategy) == 0)
    delete _literal_selection_strategy;
 
  if(dynamic_cast<solver_observer *>(_polarity_selection_strategy) == 0)
    delete _polarity_selection_strategy;
  
  if(dynamic_cast<solver_observer *>(_forget_strategy) == 0)
    delete _forget_strategy;
  
  if(dynamic_cast<solver_observer *>(_forget_selection_strategy) == 0)
    delete _forget_selection_strategy;
  
  if(dynamic_cast<solver_observer *>(_restart_strategy) == 0)
    delete _restart_strategy;

  for(unsigned i = 0; i < _theory_solvers.size(); i++)
    delete _theory_solvers[i];
  
  for(unsigned i = 0; i < _observers.size(); i++)
    delete _observers[i];

  delete _formula_transformer;
  
  clear_initial_clauses();
  clear_learnt_clauses();
  clear_literals();

}
