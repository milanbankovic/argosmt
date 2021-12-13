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

#ifndef _ALLDIFF_CONSTRAINT_HANDLER_H
#define _ALLDIFF_CONSTRAINT_HANDLER_H

#include "constraint_handler.hpp"
#include "hash_vector.hpp"
#include "backjump_stack.hpp"
#include "bipartite_graph.hpp"
#include "history_saver.hpp"
#include "csp_explanation_minimizer.hpp"

class alldiff_constraint_handler : public constraint_handler {
private:
  typedef hash_vector<expression> expression_hash_vector;
  typedef hash_vector<int> int_hash_vector;
  typedef std::unordered_map<expression, int> expression_to_int_hash_map;

  typedef bipartite_graph::vertex vertex;
  typedef bipartite_graph::edge edge;
  typedef bipartite_graph::vertex_iterator vertex_iterator;
  typedef bipartite_graph::edge_marking edge_marking;
  typedef bipartite_graph::vertex_marking vertex_marking;

  unsigned _inited_at_level;  
  expression_hash_vector _variables;
  int_hash_vector _values;
  unsigned _next_to_assert;
  bipartite_graph _start_graph;
  sparse_backjump_stack<expression> _trail;
  history_saver<bipartite_graph> _graph_states;
  bool _dirty_trail;
  std::vector<edge> _last_covering_matching;
  std::unordered_map<expression, unsigned> _propagation_positions;
  std::unordered_map< expression, std::vector<edge> > _edges_to_remove;
  multivar_explanation_minimizer _expl_wrapper;
  
  unsigned _check_count;
  const unsigned _check_period;

  alldiff_common_data * _common_data;

  void init_start_graph()
  {
    _start_graph.assign(_variables.size(),_values.size(), &_theory_solver->_bipartite_graph_common_data);
  }
  
  void init_current_graph()
  {
    _graph_states.ref() = _start_graph;
  }

  bipartite_graph & get_current_graph()
  {
    return _graph_states.ref();
  }

  const bipartite_graph & get_const_current_graph() const
  {
    return _graph_states.const_ref();
  }

  bipartite_graph & get_start_graph()
  {
    return _start_graph;
  }

  const bipartite_graph & get_const_start_graph() const
  {
    return _start_graph;
  }

  bool strong_deduce(unsigned level) const
  {
    return (level > 0 && _check_count >= _check_period) || _theory_solver->get_solver().all_literals_assigned();
  }

  double calculate_explanation_depth(const explanation & expl) const
  {
    int s = 0;
    for(unsigned i = 0; i < expl.size(); i++)
      {
	s += _theory_solver->get_solver().get_trail().current_level() - 
	  _theory_solver->get_solver().get_trail().get_trail_level(expl[i]);
      }
    return (double)s / expl.size();
  }
  
  bool convert_atom_to_edge(const expression & atom, edge & e) const;
  void convert_edge_to_equality(const edge & e, expression & atom) const;
  void convert_edge_to_disequality(const edge & e, expression & atom) const;
  bool convert_variable_to_vertex(const expression & var, vertex & vertex) const;
  bool convert_value_to_vertex(int val, vertex & vertex) const;
  void convert_vertex_to_term(const vertex & vertex, expression & term) const;
  void propagate_literal(const expression & l);
  void propagate_literals(const std::vector <edge> & removed_edges, 
			  const std::vector <edge> & new_vital_edges);
  void generate_explanation(const expression & l, explanation & expl);
  void generate_conflict_explanation(explanation & expl);
  void generate_finite_alldiff_explanation(explanation & expl);
  void generate_explanation_kat(const expression & l, explanation & expl);
  void generate_self_explanation(explanation & expl);

  const std::vector<edge> & get_edges_to_remove(const expression & l);
  
  void init_handler();

  void simple_check_alldiff() {} // TODO
  void simple_check_not_alldiff() {} // TODO
public:

  alldiff_constraint_handler(csp_theory_solver * th, const expression & l_pos, const expression & l_neg, unsigned check_period = 0);
  virtual void new_level();
  virtual void backjump(unsigned level);
  virtual void assert_literal(const expression & l);
  virtual void check_and_propagate(unsigned layer);
  virtual void explain_literal(const expression & l);

  virtual int get_variable_value_hint(const expression & var)
  {
    bipartite_graph::vertex var_vertex;
    if(!convert_variable_to_vertex(var, var_vertex))
      throw "FATAL";
    bipartite_graph::vertex val_vertex = get_const_current_graph().matched_with(var_vertex);
    if(!get_const_current_graph().vertex_exists(val_vertex))
      {
	bipartite_graph::edge_iterator ei = get_const_current_graph().begin_edge(var_vertex);
	assert(ei != get_const_current_graph().end_edge(var_vertex));
	val_vertex = ei->right_vertex();	
      }
    return _values[val_vertex.vertex_number()];
  }
  virtual bool is_weaker_from_pos(const expression & c);
  virtual bool is_weaker_from_neg(const expression & c);
  virtual void check_implied();

};

#endif // _ALLDIFF_CONSTRAINT_HANDLER_H
