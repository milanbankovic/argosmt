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

#include "alldiff_constraint_handler.hpp"
#include "domain_handler.hpp"

alldiff_constraint_handler::alldiff_constraint_handler(csp_theory_solver * th, const expression & l_pos, const expression & l_neg, unsigned check_period)
  :constraint_handler(th, l_pos, l_neg),
   _inited_at_level((unsigned)(-1)),
   _next_to_assert(0),
   _start_graph(0,0,0),
   _graph_states(th->_current_level, bipartite_graph(0,0,0)),
   _dirty_trail(false),
   _propagation_positions(HASH_TABLE_SIZE),
   _edges_to_remove(HASH_TABLE_SIZE),
   _check_count(0),
   _check_period(check_period),
   _common_data(&th->_alldiff_common_data)
{
  _common_data->_instance_count++;
  const expression_vector & ops = _constraint->get_operands();
  csp_theory_solver::csp_theory_solver_data * cons_data = _theory_solver->get_theory_solver_data(_constraint);
  for(unsigned i = 0; i < ops.size(); i++)
    {
      csp_theory_solver::csp_theory_solver_data * var_data = _theory_solver->add_variable(ops[i]);
      var_data->add_variable_constraint(_constraint);
      cons_data->add_constraint_variable(ops[i]);
    }
  for(unsigned i = 0; i < th->_current_level; i++)
    _trail.new_level();
}

bool alldiff_constraint_handler::convert_atom_to_edge(const expression & atom, edge & e) const
{
  int left, right;
  const expression & var = atom->get_operands()[0];
  int val = _theory_solver->get_theory_solver_data(atom->get_operands()[1])->get_int_value();
  expression_hash_vector::const_hash_map_iterator it = _variables.hash_map_find(var);
  if(it == _variables.hash_map_end())
    return false;
  left = it->second;

  int_hash_vector::const_hash_map_iterator jt = _values.hash_map_find(val);
  if(jt == _values.hash_map_end())
    return false;
  right = jt->second;

  e = edge(left, right);
  return true;
}

void alldiff_constraint_handler::convert_edge_to_equality(const edge & e, expression & atom) const
{
  unsigned left, right;
  
  left = e.left_vertex().vertex_number();
  right = e.right_vertex().vertex_number();
  assert(left < _variables.size());
  assert(right < _values.size());

  csp_theory_solver::csp_theory_solver_data * data = _theory_solver->get_theory_solver_data(_variables[left]);
  domain_handler * d_handler = data->get_variable_domain_handler();
  d_handler->init_literal_pair(_values[right], true);
  atom = d_handler->get_equality(_values[right]);
}

void alldiff_constraint_handler::convert_edge_to_disequality(const edge & e, expression & atom) const
{
  unsigned left, right;
  
  left = e.left_vertex().vertex_number();
  right = e.right_vertex().vertex_number();
  assert(left < _variables.size());
  assert(right < _values.size());
  
  csp_theory_solver::csp_theory_solver_data * data = _theory_solver->get_theory_solver_data(_variables[left]);
  domain_handler * d_handler = data->get_variable_domain_handler();
  d_handler->init_literal_pair(_values[right], true);
  atom = d_handler->get_disequality(_values[right]);
}

bool alldiff_constraint_handler::convert_variable_to_vertex(const expression & var, vertex & v) const
{
  expression_hash_vector::const_hash_map_iterator it = 
    _variables.hash_map_find(var);
  
  if(it == _variables.hash_map_end())
    return false;
  v = vertex(it->second, bipartite_graph::LEFT);
  return true;
}

bool alldiff_constraint_handler::convert_value_to_vertex(int val, vertex & v) const
{
  int_hash_vector::const_hash_map_iterator it = 
    _values.hash_map_find(val);

  if(it == _values.hash_map_end())
    return false;
  v = vertex(it->second, bipartite_graph::RIGHT);
  return true;
}

void alldiff_constraint_handler::convert_vertex_to_term(const vertex & v, expression & term) const
{
  switch(v.vertex_side())
    {
    case bipartite_graph::LEFT:
      assert(v.vertex_number() < _variables.size());
      term = _variables[v.vertex_number()];
      break;
    case bipartite_graph::RIGHT:
      assert(v.vertex_number() < _values.size());
      _theory_solver->get_expr_for_int(_values[v.vertex_number()], term);
      break;
    default:
      break;
    }
}

void alldiff_constraint_handler::propagate_literal(const expression & l)
{
  const extended_boolean l_value = _theory_solver->get_solver().get_trail().get_value(l);
  if(l_value == EB_UNDEFINED)
    {
      _theory_solver->get_theory_solver_data(l)->set_explanation_handler(this);
      _propagation_positions[l] = _trail.size();
      _theory_solver->get_solver().apply_propagate(l, _theory_solver);
      //      std::cout << "ALLDIFF: " << _constraint << " propagated: " << l << std::endl;
      _common_data->_count_propagate++;
    }
  else if(l_value == EB_FALSE)
    {
      std::cout << "FATAL: " << l << std::endl;
      exit(1);
      explanation conflicting;
      expression l_opp = _theory_solver->get_solver().get_literal_data(l)->get_opposite();
      generate_explanation(l, conflicting);
      conflicting.push_back(l_opp);
      _theory_solver->get_solver().apply_conflict(conflicting, _theory_solver);
      _common_data->_count_conflict++;
    }  
}

void alldiff_constraint_handler::propagate_literals(const std::vector <edge> & removed_edges, 
						    const std::vector <edge> & new_vital_edges)
{
  const bipartite_graph & graph = get_const_current_graph();  
  for(unsigned i = 0; i <  new_vital_edges.size(); i++)
    {
      const edge & e = new_vital_edges[i];
      expression l; 
      convert_edge_to_equality(e, l);
      propagate_literal(l);
    }

  for(unsigned i = 0; i <  removed_edges.size(); i++)
    {
      const edge & e = removed_edges[i];

      // Don't introduce disequalities if equality holds
      if(graph.is_matched_vital(e.left_vertex()))
	continue;
      
      expression l;
      convert_edge_to_disequality(e, l);      
      propagate_literal(l);
    }
}

struct duplicate_elimination_wrapper {
  explanation & expl;
  expression_set set;
  
  duplicate_elimination_wrapper(explanation & e)
    :expl(e),
     set(HASH_TABLE_SIZE)
  {}

  void push_back(const expression & e)
  {
    if(set.find(e) == set.end())
      {
	expl.push_back(e);
	set.insert(e);
      }
  }
};


const std::vector<alldiff_constraint_handler::edge> &
alldiff_constraint_handler::get_edges_to_remove(const expression & l)
{
  std::unordered_map<expression, std::vector<edge> >::iterator it = _edges_to_remove.find(l);
  if(it != _edges_to_remove.end())
    return it->second;
  else
    {
      const bipartite_graph & start_graph = get_const_start_graph();
      std::vector<edge> & v_edges = _edges_to_remove[l]; // creates empty vector
      const function_symbol & fs = l->get_symbol();
      if(l->is_disequality())
	{
	  edge ed;
	  if(convert_atom_to_edge(l, ed) && start_graph.edge_exists(ed))
	    v_edges.push_back(ed);	  
	}
      else if(l->is_equality())
	{	  
	  edge ed;
	  if(!convert_atom_to_edge(l, ed))
	    return v_edges;
	  
 	  for(bipartite_graph::edge_iterator 
		it = start_graph.begin_edge(ed.left_vertex()),
		it_end = start_graph.end_edge(ed.left_vertex());
	      it != it_end; ++it)
	    {
	      if(*it == ed)
		{
		  continue;
		}
	      v_edges.push_back(*it);
	    }	  
 	}
      else if(fs == function_symbol::LE || fs == function_symbol::LT ||
	      fs == function_symbol::GE || fs == function_symbol::GT)
	{
	  const expression & var = l->get_operands()[0];
	  int val = _theory_solver->get_theory_solver_data(l->get_operands()[1])->get_int_value();
	  vertex v(_variables.get_index(var), bipartite_graph::LEFT);
	  for(bipartite_graph::edge_iterator 
		it = start_graph.begin_edge(v),
		it_end = start_graph.end_edge(v);
	      it != it_end; ++it)
	    {
	      int c_val = _values[it->right_vertex().vertex_number()];
	      if((fs == function_symbol::LE && c_val > val) ||
		 (fs == function_symbol::LT && c_val >= val) ||
		 (fs == function_symbol::GE && c_val < val) ||
		 (fs == function_symbol::GT && c_val <= val))
		{
		  v_edges.push_back(*it);
		}	  
	    }
	}
      return v_edges;
    }
}

void alldiff_constraint_handler::
generate_explanation(const expression & l, explanation & expl)
{
  edge e;
  static std::vector<edge> reason;
  reason.clear();
  bipartite_graph & start_graph = get_start_graph();
  const bipartite_graph & graph = get_const_current_graph();
  static bipartite_graph::edge_marking trail_marking;
  trail_marking.reset_marking(&start_graph);
  static bipartite_graph::edge_marking reason_marking;
  reason_marking.reset_marking(&start_graph);
  static bipartite_graph::edge_attribute<expression> edge_cause;
  edge_cause.reset_attributes(&start_graph);
  //multivar_explanation_minimizer & wrapper = _expl_wrapper;
  //wrapper.reset(_theory_solver);
  multivar_explanation_minimizer wrapper(_theory_solver);
  
  convert_atom_to_edge(l, e);

  std::unordered_map<expression, unsigned>::const_iterator it = _propagation_positions.find(l);
  assert(it != _propagation_positions.end());
  unsigned trail_index = it->second;
  for(unsigned i = 0; i < trail_index; i++)
    {      
      if(l->is_disequality() && _trail[i]->is_equality()
	 && (
	     (l->get_operands()[0] == _trail[i]->get_operands()[0] && 
	      l->get_operands()[1] != _trail[i]->get_operands()[1])
	     ||
	     (l->get_operands()[0] != _trail[i]->get_operands()[0] && 
	      l->get_operands()[1] == _trail[i]->get_operands()[1])
	     )
	 )
	{
	  _common_data->_count_trivial_explain++;
	  expl.push_back(_trail[i]);
	  return;
	}
      
      const function_symbol & fs = _trail[i]->get_symbol();
      const std::vector<edge> & v_edges = get_edges_to_remove(_trail[i]);
      for(unsigned j = 0; j < v_edges.size(); j++)
	{
	  const expression & ej_attr = edge_cause.get_attribute(v_edges[j]);
	  trail_marking.mark_edge(v_edges[j]);
	  if(ej_attr.get() == 0 || fs == function_symbol::EQ || (fs != function_symbol::DISTINCT && ej_attr->is_disequality()))
	    edge_cause.set_attribute(v_edges[j], _trail[i]);
	}	
    }

  if(graph.is_covered_left())
    {
      for(vertex_iterator it = graph.begin_left(); 
	  it != graph.end_left(); 
	  ++it)
	{
	  edge matched_edge(*it, graph.matched_with(*it));
	  start_graph.add_to_matching(matched_edge);
	  assert(!trail_marking.is_marked(matched_edge));
	}
    } 
  else
    {
      for(std::size_t i = 0; i < _last_covering_matching.size(); i++)
	{
	  start_graph.add_to_matching(_last_covering_matching[i]);
	  assert(!trail_marking.is_marked(_last_covering_matching[i]));
	}
    }
    

  start_graph.reduce_prop_expl_to_mos(e, trail_marking, reason_marking);
  _common_data->_count_hall_explain++;

  //  reason_marking = trail_marking;
  
  reason_marking.marked_edges_vector(reason);

  for(unsigned i = 0; i < reason.size(); i++)
    {
      wrapper.push_back(edge_cause.get_attribute(reason[i]));
    }

  wrapper.export_to_expl(expl);
  _common_data->_cumulative_expl_depth += calculate_explanation_depth(expl);
  _common_data->_expl_depth_count++;
  generate_finite_alldiff_explanation(expl);
}
      
void alldiff_constraint_handler::generate_conflict_explanation(explanation & expl)
{
  static std::vector<edge> reason;
  reason.clear();
  bipartite_graph & start_graph = get_start_graph();
  const bipartite_graph & graph = get_const_current_graph();
  static bipartite_graph::edge_marking trail_marking;
  trail_marking.reset_marking(&start_graph);
  static bipartite_graph::edge_marking reason_marking;
  reason_marking.reset_marking(&start_graph);
  static bipartite_graph::edge_attribute<expression> edge_cause;
  edge_cause.reset_attributes(&start_graph);
  //multivar_explanation_minimizer & wrapper = _expl_wrapper;
  //wrapper.reset(_theory_solver);
  multivar_explanation_minimizer wrapper(_theory_solver);
  
  
  for(unsigned i = 0; i < _trail.size(); i++)
    {      
      const function_symbol & fs = _trail[i]->get_symbol();
      const std::vector<edge> & v_edges = get_edges_to_remove(_trail[i]);
      for(unsigned j = 0; j < v_edges.size(); j++)
	{	  
	  const expression & ej_attr = edge_cause.get_attribute(v_edges[j]);
	  trail_marking.mark_edge(v_edges[j]);
	  if(ej_attr.get() == 0 || fs == function_symbol::EQ || (fs != function_symbol::DISTINCT && ej_attr->is_disequality()))
	    edge_cause.set_attribute(v_edges[j], _trail[i]);
	}	
    }

  for(vertex_iterator it = graph.begin_left(); 
      it != graph.end_left(); 
      ++it)
    {
      if(graph.is_matched(*it))
	{
	  start_graph.add_to_matching(edge(*it, graph.matched_with(*it)));
	}
      else
	start_graph.unmatch_vertex(*it);
    }

#ifndef _USE_KATSIRELOS_EXPLANATIONS
  start_graph.reduce_cflt_expl_to_mos(trail_marking, reason_marking);
#else 
  start_graph.reduce_cflt_expl_to_kat(trail_marking, reason_marking);
#endif
  
  assert(!start_graph.is_covered_left());
  // reason_marking = trail_marking;
  
  
  reason_marking.marked_edges_vector(reason);
  
  for(unsigned i = 0; i < reason.size(); i++)
    {
      wrapper.push_back(edge_cause.get_attribute(reason[i]));
    }
  
  wrapper.export_to_expl(expl);
  _common_data->_cumulative_expl_depth += calculate_explanation_depth(expl);
  _common_data->_expl_depth_count++;
  generate_finite_alldiff_explanation(expl);
}

void alldiff_constraint_handler::generate_explanation_kat(const expression & l, explanation & expl)
{    
  edge e;  
  static std::vector<edge> reason;
  reason.clear();
  bipartite_graph & start_graph = get_start_graph();
  const bipartite_graph & graph = get_const_current_graph();
  static bipartite_graph::edge_marking trail_marking;
  trail_marking.reset_marking(&start_graph);
  static bipartite_graph::edge_marking reason_marking;
  reason_marking.reset_marking(&start_graph);
  static bipartite_graph::edge_attribute<expression> edge_cause;
  edge_cause.reset_attributes(&start_graph);
  //multivar_explanation_minimizer & wrapper = _expl_wrapper;
  //wrapper.reset(_theory_solver);
  multivar_explanation_minimizer wrapper(_theory_solver);
  
  convert_atom_to_edge(l, e);

  std::unordered_map<expression, unsigned>::const_iterator it = _propagation_positions.find(l);
  assert(it != _propagation_positions.end());
  unsigned trail_index = it->second;
  for(unsigned i = 0; i < trail_index; i++)
    {
      if(l->is_disequality() && _trail[i]->is_equality()
	 && (
	     (l->get_operands()[0] == _trail[i]->get_operands()[0] && 
	      l->get_operands()[1] != _trail[i]->get_operands()[1])
	     ||
	     (l->get_operands()[0] != _trail[i]->get_operands()[0] && 
	      l->get_operands()[1] == _trail[i]->get_operands()[1])
	     )
	 )
	{
	  _common_data->_count_trivial_explain++;
	  expl.push_back(_trail[i]);
	  return;
	}

      const function_symbol & fs = _trail[i]->get_symbol();
      const std::vector<edge> & v_edges = get_edges_to_remove(_trail[i]);
      for(unsigned j = 0; j < v_edges.size(); j++)
	{
	  const expression & ej_attr = edge_cause.get_attribute(v_edges[j]);
	  trail_marking.mark_edge(v_edges[j]);
	  if(ej_attr.get() == 0 || fs == function_symbol::EQ || (fs != function_symbol::DISTINCT && ej_attr->is_disequality()))
	    edge_cause.set_attribute(v_edges[j], _trail[i]);
	}	
    }
  
  if(graph.is_covered_left())
    {
      for(vertex_iterator it = graph.begin_left(); 
	  it != graph.end_left(); 
	  ++it)
	{
	  edge matched_edge(*it, graph.matched_with(*it));
	  start_graph.add_to_matching(matched_edge);
	  assert(!trail_marking.is_marked(matched_edge));
	}
    } 
  else
    {
      for(std::size_t i = 0; i < _last_covering_matching.size(); i++)
	{
	  start_graph.add_to_matching(_last_covering_matching[i]);
	  assert(!trail_marking.is_marked(_last_covering_matching[i]));
	}
    }
    

  start_graph.reduce_prop_expl_to_kat(e, trail_marking, reason_marking);
  _common_data->_count_hall_explain++;

  reason_marking.marked_edges_vector(reason);
      
  for(unsigned i = 0; i < reason.size(); i++)
    {
      if(trail_marking.is_marked(reason[i]))
	{
	  wrapper.push_back(edge_cause.get_attribute(reason[i]));
	}
      else
	{	    
	  edge me(reason[i].left_vertex(), start_graph.matched_with(reason[i].left_vertex()));
	  expression var = _variables[reason[i].left_vertex().vertex_number()];
	  domain_handler * d_handler = _theory_solver->get_theory_solver_data(var)->get_variable_domain_handler();
	  int eq_val = _values[me.right_vertex().vertex_number()];
	  int diseq_val = _values[reason[i].right_vertex().vertex_number()];
	  expression equality = d_handler->get_equality(eq_val);
	  
	  if(equality.get() != 0 && _theory_solver->get_solver().get_trail().get_value(equality) == EB_TRUE)
	    {
	      wrapper.push_back(equality);
	    }
	  else
	    {
	      expression disequality = d_handler->get_disequality(diseq_val);
	      assert(disequality.get() != 0);
	      assert(_theory_solver->get_solver().get_trail().get_value(disequality) == EB_TRUE);
	      wrapper.push_back(disequality);
	    }	      
	}
    }

  wrapper.export_to_expl(expl);
  _common_data->_cumulative_expl_depth += calculate_explanation_depth(expl);
  _common_data->_expl_depth_count++;
  generate_finite_alldiff_explanation(expl);
}


void alldiff_constraint_handler::generate_finite_alldiff_explanation(explanation & expl)
{
  if(_inited_at_level == 0)
    return;

  if(_theory_solver->get_solver().get_trail().get_trail_level(_constraint) > 0)
    expl.push_back(_constraint);

  for(unsigned i = 0; i < _variables.size(); i++)
    {
      csp_theory_solver::csp_theory_solver_data * v_data = _theory_solver->get_theory_solver_data(_variables[i]);
      domain_handler * v_handler = v_data->get_variable_domain_handler();
      if(v_handler->get_finite_level() > 0)
	{
	  const expression_vector & fcause = v_handler->explain_finite_domain();
	  for(unsigned j = 0; j < fcause.size(); j++)
	    expl.push_back(fcause[j]);
	}
    }
}

void alldiff_constraint_handler::init_handler()
{
  const expression_vector & vars = _constraint->get_operands();
  for(unsigned i = 0; i < vars.size(); i++)
    {            
      csp_theory_solver::csp_theory_solver_data * vi_data = _theory_solver->get_theory_solver_data(vars[i]);
      assert(vi_data->has_variable_data());
      domain_handler * vi_handler = vi_data->get_variable_domain_handler();
      if(!vi_handler->is_finite())
	{
	  _variables.clear();
	  _values.clear();
	  return;
	}

      _variables.push_back(vars[i]);
      
      const std::vector<int> & domain = vi_handler->initial_finite_domain();
      for(unsigned j = vi_handler->get_zero_min_index(), j_end = vi_handler->get_zero_max_index(); j <= j_end; j++)
	{
	  _values.push_back(domain[j]);
	}
    }

  _inited_at_level = _theory_solver->_current_level;
  init_start_graph();
  bipartite_graph & start_graph = get_start_graph();

  for(unsigned i = 0; i < _variables.size(); i++)
    {
      domain_handler * vi_handler = _theory_solver->get_theory_solver_data(_variables[i])->get_variable_domain_handler();
      const std::vector<int> & domain = vi_handler->initial_finite_domain();

      for(unsigned j = vi_handler->get_zero_min_index(), j_end = vi_handler->get_zero_max_index(); j <= j_end; j++)
	{
	  start_graph.add_edge(edge(i, _values.get_index(domain[j])));
	}
    }

  std::vector<edge> dummy;
  std::vector<edge> removed_edges;
  std::vector<edge> new_vital_edges;

  start_graph.greedy_matching();
  
  bool unsat = !start_graph.do_propagation(dummy, dummy, removed_edges, new_vital_edges, true);
  init_current_graph();
  if(unsat)
    {
      explanation conflicting;
      generate_conflict_explanation(conflicting);
      _theory_solver->get_solver().apply_conflict(conflicting, _theory_solver);
      _common_data->_count_conflict++;
    }
  else
    {
      propagate_literals(removed_edges, new_vital_edges);
    }
}

void alldiff_constraint_handler::assert_literal(const expression & l) 
{
  if(_theory_solver->get_solver().is_conflict())
    return;

  if(_theory_solver->get_solver().get_trail().get_source_theory_solver(l) != _theory_solver ||
     _theory_solver->get_theory_solver_data(l)->get_explanation_handler() != this)
    {
      _trail.push(l);
    }
}

void alldiff_constraint_handler::check_and_propagate(unsigned layer) 
{
  if(_theory_solver->get_solver().is_conflict())
    {
      return;
    }

  extended_boolean eb = _theory_solver->get_solver().get_trail().get_value(_constraint);
  if(eb == EB_UNDEFINED)
    return; // never happens

  if(_just_activated)
    {
      if(eb == EB_TRUE)
	propagate_weaker_from_pos_constraints();
      else
	propagate_weaker_from_neg_constraints();
      _just_activated = false;
    }
  
  if(eb == EB_FALSE)
    {
      simple_check_not_alldiff();
      _theory_solver->process_main_trail();
      return;
    }
  
  if(_inited_at_level == (unsigned)(-1))
    init_handler();
  
  if(_inited_at_level == (unsigned)(-1))
    {
      simple_check_alldiff();
      _theory_solver->process_main_trail();
      return;
    }
  
  bipartite_graph & graph = get_current_graph(); 
  std::vector<edge> remove;
  std::vector<edge> vital;
  std::vector<edge> removed_edges;
  std::vector<edge> new_vital_edges;
  
  if(_next_to_assert < _trail.size())
    _dirty_trail = true;

  for(unsigned i = _next_to_assert; i < _trail.size(); i++)
    {      
      if(_trail[i]->is_equality())
	{
	  edge e;
	  if(!convert_atom_to_edge(_trail[i], e))
	    continue;
	  vital.push_back(e);
	}
      else
	{
	  const std::vector<edge> & v_edges = get_edges_to_remove(_trail[i]);
	  unsigned rsize = remove.size();
	  remove.resize(rsize + v_edges.size());
	  std::copy(v_edges.begin(), v_edges.end(), remove.begin() + rsize);
	}
    }
    
  if(_dirty_trail)
    {
      //      std::cout << "ALLDIFF CHECK: " << _constraint << " (layer: " << layer << ")" << std::endl;
      bool unsat = !graph.do_propagation(remove, vital,
					 removed_edges, 
					 new_vital_edges, 
					 strong_deduce(layer));
      if(strong_deduce(layer))
	{
	  _dirty_trail = false;
	  _check_count = 0;
	}
      else if(layer > 0) 
	{
	  _check_count++;
	}

      if(graph.is_covered_left())
	{
	  _last_covering_matching.clear();
	  for(vertex_iterator iter = graph.begin_left(),
		iter_end = graph.end_left();
	      iter != iter_end; ++iter)
	    {
	      _last_covering_matching.push_back
		(edge(*iter, graph.matched_with(*iter)));
	    }
	}
      
      if(unsat)
	{
	  explanation conflicting;
	  generate_conflict_explanation(conflicting);
	  _theory_solver->get_solver().apply_conflict(conflicting, _theory_solver);
	  _common_data->_count_conflict++;
	}
      else
	{
	  propagate_literals(removed_edges, new_vital_edges);
	}
    }
  _next_to_assert = _trail.size();
  _theory_solver->process_main_trail();
}

void alldiff_constraint_handler::explain_literal(const expression & l) 
{
  assert(_theory_solver->has_theory_solver_data(l));
  explanation expl;
  if(_theory_solver->get_theory_solver_data(l)->has_constraint_data())
    {
      if(l == _constraint || l == _not_constraint)
	{
	  generate_self_explanation(expl);
	}
      else
	{
	  expl.push_back(_theory_solver->get_solver().get_trail().get_value(_constraint) == EB_TRUE ?
			 _constraint : _not_constraint);
	}
    }
  else
    {
#ifndef _USE_KATSIRELOS_EXPLANATIONS
      generate_explanation(l, expl);
#else
      generate_explanation_kat(l, expl);
#endif
    }
  _theory_solver->get_solver().apply_explain(l, expl);
}

void alldiff_constraint_handler::generate_self_explanation(explanation & expl)
{
  for(unsigned i = 0; i < _variables.size(); i++)
    {
      const expression & var = _variables[i];
      csp_theory_solver::csp_theory_solver_data * v_data = _theory_solver->get_theory_solver_data(var);
      assert(v_data != 0);
      domain_handler * d_handler = v_data->get_variable_domain_handler();
      d_handler->explain_domain_at_trail_size(_theory_solver->get_solver().get_trail().get_trail_index(_constraint), expl);
    }
}

void alldiff_constraint_handler::new_level() 
{
  _trail.new_level();
}

void alldiff_constraint_handler::backjump(unsigned level) 
{
  assert(level < _trail.current_level());
  _trail.backjump(level);
  assert(_next_to_assert >= _trail.size());
  _next_to_assert = _trail.size();
  _graph_states.restore_state();
  if(!_graph_states.const_ref().is_valid())
    {
      _start_graph.assign(0,0,0);
      _inited_at_level = (unsigned)(-1);
    }
  _dirty_trail = false;
}

bool alldiff_constraint_handler::is_weaker_from_pos(const expression & c)
{
  csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(c);
  if(c_data == 0 || !c_data->has_constraint_data())
    return false;
  
  alldiff_constraint_handler * c_handler = dynamic_cast<alldiff_constraint_handler*>(c_data->get_constraint_handler());
  if(c_handler == 0)
    return false;

  if(c_handler->_constraint == c)
    {
      for(unsigned i = 0; i < c_handler->_variables.size(); i++)
	if(_variables.hash_map_find(c_handler->_variables[i]) == _variables.hash_map_end())
	  return false;
      return true;
    }
  else
    return false;
}

bool alldiff_constraint_handler::is_weaker_from_neg(const expression & c)
{
  csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(c);
  if(c_data == 0 || !c_data->has_constraint_data())
    return false;
  
  alldiff_constraint_handler * c_handler = dynamic_cast<alldiff_constraint_handler*>(c_data->get_constraint_handler());
  if(c_handler == 0)
    return false;

  if(c_handler->_not_constraint == c)
    {
      for(unsigned i = 0; i < _variables.size(); i++)
	if(c_handler->_variables.hash_map_find(_variables[i]) == c_handler->_variables.hash_map_end())
	  return false;
      return true;
    }
  else
    return false;
}

void alldiff_constraint_handler::check_implied()
{
  assert(_theory_solver->get_solver().get_trail().get_value(_constraint) == EB_UNDEFINED);

  std::unordered_set<int> values(HASH_TABLE_SIZE);
  for(unsigned i = 0; i < _variables.size(); i++)
    {
      const expression & var = _variables[i];
      csp_theory_solver::csp_theory_solver_data * v_data = _theory_solver->get_theory_solver_data(var);
      assert(v_data != 0);
      domain_handler * d_handler = v_data->get_variable_domain_handler();
      if(d_handler->current_domain().size() == 1)	
	values.insert(d_handler->get_min());
      else
	return;
    }

  if(values.size() == _variables.size())
    {
      _theory_solver->get_theory_solver_data(_constraint)->set_explanation_handler(this);      
      _theory_solver->get_solver().apply_propagate(_constraint, _theory_solver);
    }
  else 
    {
      _theory_solver->get_theory_solver_data(_not_constraint)->set_explanation_handler(this);      
      _theory_solver->get_solver().apply_propagate(_not_constraint, _theory_solver);
    }  
}
