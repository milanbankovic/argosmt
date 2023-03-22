/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2016,2021 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _CSP_THEORY_SOLVER_H
#define _CSP_THEORY_SOLVER_H

#include "theory_solver.hpp"
#include "solver.hpp"
#include "backjump_stack.hpp"
#include <list>
#include <deque>
#include "arithmetic_canonizer.hpp"
#include "csp_common_data.hpp"
#include "wall_clock.hpp"
#include "history_saver.hpp"

class propagation_handler;
class constraint_handler;
class domain_handler;
class csp_variable_order_strategy;
class csp_value_order_strategy;

enum alldiff_sum_algorithm {
  AS_NONE, AS_PARTITION, AS_BELDICEANU, AS_FAST
};

class csp_theory_solver : public theory_solver, public arithmetic_canonizer<int> {
public:

  enum csp_expression_type { CSP_CONSTRAINT, CSP_VARIABLE, CSP_VALUE, CSP_DOMAIN_RESTRICTION };

  class csp_theory_solver_data : public expression_data {
  private:
    struct constraint_data {
      struct pair_data {
	expression_set _variables;
	constraint_handler * _handler;		
	propagation_handler * _explain_handler;
	unsigned _index;
      };
      bool _positive;
      pair_data * _pair_data;
    };
    struct variable_data {
      expression_set _constraints;
      expression_set _domain_restrictions;
      domain_handler * _handler;
      unsigned _index;
    };

    struct value_data {
      int _int_value;
      unsigned _index;
    };

    struct domain_restriction_data {
      struct pair_data {
	expression _variable;
	std::unordered_set<int> _values;
	propagation_handler * _handler;
	unsigned _index;
	pair_data():_values(HASH_TABLE_SIZE){}
      };
      bool _positive;
      pair_data * _pair_data;
    };

    constraint_data * _constraint_data;
    variable_data * _variable_data;
    value_data * _value_data;
    domain_restriction_data * _domain_restriction_data;

  public:
    csp_theory_solver_data(csp_expression_type t, unsigned index, 
		    csp_theory_solver_data * data = 0);

    bool has_variable_data() const
    {
      return _variable_data != 0;
    }

    bool has_constraint_data() const
    {
      return _constraint_data != 0;
    }

    bool has_value_data() const
    {
      return _value_data != 0;
    }

    bool has_domain_restriction_data() const
    {
      return _domain_restriction_data != 0;
    }

    unsigned get_variable_index() const
    {
      return _variable_data->_index;
    }

    const expression_set & get_variable_constraints() const
    {
      return _variable_data->_constraints;
    }

    void add_variable_constraint(const expression & cons)
    {
      _variable_data->_constraints.insert(cons);
    }

    bool is_variable_constrained_by(const expression & cons) const
    {
      return _variable_data->_constraints.find(cons) != 
    	_variable_data->_constraints.end();
    }

    const expression_set & get_variable_domain_restrictions() const
    {
      return _variable_data->_domain_restrictions;
    }

    void add_variable_domain_restriction(const expression & rs)
    {
      _variable_data->_domain_restrictions.insert(rs);
    }

    bool is_variable_domain_restricted_by(const expression & rs) const
    {
      return _variable_data->_domain_restrictions.find(rs) !=
	_variable_data->_domain_restrictions.end();
    }

    void set_variable_domain_handler(domain_handler * handler)
    {
      _variable_data->_handler = handler;
    }

    domain_handler * get_variable_domain_handler() 
    {
      return _variable_data->_handler;
    }

    bool has_variable_domain_handler() const
    {
      return _variable_data->_handler != 0;
    }

    unsigned get_constraint_index() const
    {
      return _constraint_data->_pair_data->_index;
    }

    constraint_handler * get_constraint_handler()
    {
      return _constraint_data->_pair_data->_handler;
    }

    void set_constraint_handler(constraint_handler * handler)
    {
      _constraint_data->_pair_data->_handler = handler;
    }

    bool has_constraint_handler() const
    {
      return _constraint_data->_pair_data->_handler != 0;
    }

    const expression_set & get_constraint_variables() const
    {
      return _constraint_data->_pair_data->_variables;
    }

    void add_constraint_variable(const expression & var)
    {
      _constraint_data->_pair_data->_variables.insert(var);
    }

    bool is_constraint_interested(const expression & var) const
    {
      return _constraint_data->_pair_data->_variables.find(var) != 
	_constraint_data->_pair_data->_variables.end();
    }

    unsigned get_value_index() const
    {
      return _value_data->_index;
    }

    int get_int_value() const
    {
      return _value_data->_int_value;
    }

    void set_int_value(int v)
    {
      _value_data->_int_value = v;
    }

    unsigned get_domain_restriction_index() const
    {
      return _domain_restriction_data->_pair_data->_index;
    }

    void set_domain_restriction_variable(const expression & var) 
    {
      _domain_restriction_data->_pair_data->_variable = var;
    }

    const expression & get_domain_restriction_variable() const
    {
      return _domain_restriction_data->_pair_data->_variable;
    }

    propagation_handler * get_explanation_handler() 
    {
      if(_domain_restriction_data != 0)
	return _domain_restriction_data->_pair_data->_handler;
      else
	return _constraint_data->_pair_data->_explain_handler;
    }

    void set_explanation_handler(propagation_handler * handler)
    {
      if(_domain_restriction_data != 0)
	_domain_restriction_data->_pair_data->_handler = handler;
      else
	_constraint_data->_pair_data->_explain_handler = handler;
    }

    bool has_explanation_handler() const
    {
      return _domain_restriction_data->_pair_data->_handler != 0;
    }

    void add_domain_restriction_value(int value)
    {
      _domain_restriction_data->_pair_data->_values.insert(value);
    }

    const std::unordered_set<int> & get_domain_restriction_values() const
    {
      return _domain_restriction_data->_pair_data->_values;
    }
    
    ~csp_theory_solver_data();
  };

private:

  struct on_pop {
    csp_theory_solver * _theory;
    on_pop(csp_theory_solver * th)
      :_theory(th) {}

    void operator () (const expression & e) const;
  };

  data_extractor<csp_theory_solver_data> _data;
  expression_vector _constraints;
  expression_vector _domain_restrictions;
  expression_vector _variables;
  expression_vector _values;
  unsigned _current_trail_pos;
public:
  unsigned _current_level;
private:
  std::unordered_map<int, expression> _value_cache;

  typedef std::deque<constraint_handler *> constraint_handler_queue;
  typedef std::deque<domain_handler *> domain_handler_queue;
  std::vector<constraint_handler_queue> _handler_queues;
  domain_handler_queue _domain_handler_queue;
  constraint_handler_queue _inactive_queue;
  alldiff_sum_algorithm _as_algorithm;
  csp_variable_order_strategy * _varorder;
  csp_value_order_strategy * _valorder;
  bool _predefined_bounds;
  bool _predefined_bounds_set;
  int _lower_predefined_bound;
  int _upper_predefined_bound;

  wall_clock _check_and_prop_time_spent;
  wall_clock _new_level_time_spent;
  wall_clock _backjump_time_spent;
  wall_clock _explain_time_spent;  

  
  bool is_csp_variable(const expression & e)
  {
    return !is_coefficient(e);
  }

  const function_symbol & get_opposite_symbol(const function_symbol & fs)
  {
    if(fs == function_symbol::ALLDIFF)
      return function_symbol::NOT_ALLDIFF;
    if(fs == function_symbol::NOT_ALLDIFF)
      return function_symbol::ALLDIFF;

    return arithmetic_canonizer<int>::get_opposite_symbol(fs);
  }

  void add_to_all_queues(constraint_handler * handler);
  void add_to_domain_handler_queue(domain_handler * handler);
  void add_to_inactive_queue(constraint_handler * handler);
  void clear_all_queues();
  void clear_queue(unsigned i);
  void clear_domain_queue();
  void clear_inactive_queue();

  bool is_csp_term(const expression & e)
  {
    common_data * e_data = _solver.get_common_data(e);
    return e_data->get_owner() == this || e_data->is_shared();
  }

  void check_constraint_implications();

  const expression & positive_literal(const expression & l)
  {
    return _solver.get_literal_data(l)->get_positive();
  }

  void init_predefined_variable_bounds();

  void process_literal(const expression & l); 

public:
  
  /* Constraints common data */
  domain_common_data _domain_common_data;
  alldiff_common_data _alldiff_common_data;
  sum_common_data _sum_common_data;
  bipartite_graph_common_data _bipartite_graph_common_data;

public:
  csp_theory_solver(solver & sl, alldiff_sum_algorithm as_alg = AS_NONE,
	     bool predefined_bounds = false, int lower = 0, int upper = -1)
    :theory_solver(sl, "CSP", 2),
     arithmetic_canonizer<int>(sl.get_factory()),
     _data(sl.extractor_counter()),
     _current_trail_pos(0),
     _current_level(0),
     _value_cache(HASH_TABLE_SIZE),
     _handler_queues(get_num_of_layers()),
     _as_algorithm(as_alg),
     _varorder(0),
     _valorder(0),
     _predefined_bounds(predefined_bounds),
     _predefined_bounds_set(false),
     _lower_predefined_bound(lower),
     _upper_predefined_bound(upper)
  {}
  
  csp_theory_solver_data * get_theory_solver_data(const expression & e)
  {
    return _data.get_data(e);
  }
  
  void set_theory_solver_data(const expression & e, csp_theory_solver_data * data)
  {
    _data.set_data(e, data);
  }
  
  bool has_theory_solver_data(const expression & e) const
  {
    return _data.has_data(e);
  }  
  
  void set_variable_order_strategy(csp_variable_order_strategy * varorder)
  {
    _varorder = varorder;
  }

  void set_value_order_strategy(csp_value_order_strategy * valorder)
  {
    _valorder = valorder;
  }
  
  csp_theory_solver_data * add_variable(const expression & var);
  csp_theory_solver_data * add_value(const expression & val);
  csp_theory_solver_data * add_value(int v);
  csp_theory_solver_data * add_constraint_pair(const expression & l_pos, const expression & l_neg);
  csp_theory_solver_data * add_domain_restriction_pair(const expression & l_pos, const expression & l_neg);

  const expression_vector & get_variables() const
  {
    return _variables;
  }
    
  const expression_vector & get_constraints() const
  {
    return _constraints;
  }
  
  bool get_expr_for_int(int v, expression & expr) const
  {
    std::unordered_map<int, expression>::const_iterator it = _value_cache.find(v);
    if(it != _value_cache.end())
      {
	expr = it->second;
	return true;
      }
    else
      return false;
  }

  void process_main_trail();
  
  virtual void add_literal(const expression & l_pos, const expression & l_neg);
  virtual void new_level();
  virtual void backjump(unsigned level); 
  virtual void check_and_propagate(unsigned layer);   
  virtual void explain_literal(const expression & l);
  
  virtual bool is_owned_expression(const expression & e)
  {
    return e->get_symbol() == function_symbol::ALLDIFF || 
      e->get_symbol() == function_symbol::NOT_ALLDIFF ||
      (e->is_constant_function() && e->get_inferred_sort()->get_symbol() == sort_symbol::REAL) ||
      (e->is_constant_function() && e->get_inferred_sort()->get_symbol() == sort_symbol::INT) ||
      (e->is_numeral_constant() || e->is_decimal_constant() || is_arithmetic(e));
  }  

  alldiff_sum_algorithm get_alldiff_sum_algorithm() const
  {
    return _as_algorithm;
  }
  
  virtual expression canonize_expression(const expression & e);
  virtual void get_literal_pair(const expression & l, expression & l_pos, expression & l_neg);  

  virtual bool verify_assignment(const trail & tr)
  {
    return true;
  }  

  virtual void get_model(const expression_vector & exps);  
  virtual void print_report(std::ostream & ostr);  
  virtual ~csp_theory_solver();

};


#endif // _CSP_THEORY_SOLVER_H
