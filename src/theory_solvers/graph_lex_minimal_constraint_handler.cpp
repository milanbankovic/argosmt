#include "graph_lex_minimal_constraint_handler.hpp"

graph_lex_minimal_constraint_handler::graph_lex_minimal_constraint_handler(csp_theory_solver * th, const expression & l_pos, const expression & l_neg)
  :constraint_handler(th, l_pos, l_neg),
   _common_data(&th->_graph_lex_minimal_common_data),
   _next_to_assert(0)
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


void graph_lex_minimal_constraint_handler::new_level()
  {
    _trail.new_level();
    // TODO
  }
  
void graph_lex_minimal_constraint_handler::backjump(unsigned level)
  {
    assert(level < _trail.current_level());
    _trail.backjump(level);
    _next_to_assert = _trail.size();
    // TODO  
  }
    
void graph_lex_minimal_constraint_handler::assert_literal(const expression & l)
  {   
    if(_theory_solver->get_solver().is_conflict())
      return;
    
    if(_theory_solver->get_solver().get_trail().get_source_theory_solver(l) != _theory_solver ||
       _theory_solver->get_theory_solver_data(l)->get_explanation_handler() != this)
      {
	_trail.push(l);
      }
  }
  
void graph_lex_minimal_constraint_handler::check_and_propagate(unsigned layer)
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
	return;
      }
    
    
    // TODO
    
    
  }
    
void graph_lex_minimal_constraint_handler::explain_literal(const expression & l)
  {
    // TODO
  }

int graph_lex_minimal_constraint_handler::get_variable_value_hint(const expression & var)
  {
    // DO NOTHING
    return 0; 
  }
  
bool graph_lex_minimal_constraint_handler::is_weaker_from_pos(const expression & c)
  {
    // DO NOTHING
    return false; 
  }
  
bool graph_lex_minimal_constraint_handler::is_weaker_from_neg(const expression & c)
  {
    // DO NOTHING
    return false; 
  }
  
void graph_lex_minimal_constraint_handler::check_implied()
  {
    // DO NOTHING
  }

