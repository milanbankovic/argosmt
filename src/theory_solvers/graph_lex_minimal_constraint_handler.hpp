#ifndef _GRAPH_LEX_MINIMAL_CONSTRAINT_HANDLER_H
#define _GRAPH_LEX_MINIMAL_CONSTRAINT_HANDLER_H

#include "constraint_handler.hpp"
#include "domain_handler.hpp"
#include "hash_vector.hpp"
#include "backjump_stack.hpp"
#include "history_saver.hpp"
#include "csp_explanation_minimizer.hpp"



class graph_lex_minimal_constraint_handler : public constraint_handler {
private:
  graph_lex_minimal_common_data * _common_data;
  
  sparse_backjump_stack<expression> _trail;
  unsigned _next_to_assert;
  
  
public:
  graph_lex_minimal_constraint_handler(csp_theory_solver * th, const expression & l_pos, const expression & l_neg);

  
  virtual void new_level();
  
  virtual void backjump(unsigned level);
    
  virtual void assert_literal(const expression & l);
  
  virtual void check_and_propagate(unsigned layer);
    
  virtual void explain_literal(const expression & l);

  virtual int get_variable_value_hint(const expression & var);
  
  virtual bool is_weaker_from_pos(const expression & c);
  
  virtual bool is_weaker_from_neg(const expression & c);
  
  virtual void check_implied();
  
};

#endif // _GRAPH_LEX_MINIMAL_CONSTRAINT_HANDLER_H
