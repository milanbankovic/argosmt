/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2017 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _SOLVER_H
#define _SOLVER_H

#include "clause.hpp"
#include "argosmt_expression_data.hpp"
#include "explanation.hpp"
#include "conflict_set.hpp"
#include "trail.hpp"
#include "theory.hpp"
#include "forget_strategy.hpp"
#include "forget_selection_strategy.hpp"
#include "restart_strategy.hpp"
#include "literal_selection_strategy.hpp"
#include "polarity_selection_strategy.hpp"
#include "solver_observer.hpp"
#include "wall_clock.hpp"


#ifdef _PARALLEL_PORTFOLIO
struct solving_cancelled_exception { };
#define CHECK_CANCELED { extern bool solving_cancelled; if(solving_cancelled) throw solving_cancelled_exception(); }
#else
#define CHECK_CANCELED 
#endif

class common_data : public expression_data {
private:
    
  /* Interested theories */
  theory * _owner = nullptr;
  bool _shared = false;
public:
  
  theory * get_owner() const
  {
    return _owner;
  }

  void set_owner(theory * th)
  {
    _owner = th;
  }

  bool is_shared() const
  {
    return _shared;
  }

  void set_shared(bool sh)
  {
    _shared = sh;
  }
  
};
  
class literal_data : public expression_data {
private:
    
  struct pair_data {
      
    // Pair of literals
    expression _l_pos;
    expression _l_neg;
      
    // Observing theories
    std::vector<theory *> _theories;
            
    pair_data(const expression & l_pos,
	      const expression & l_neg)
      :_l_pos(l_pos),
       _l_neg(l_neg)
    {}
  };
  
  // Index in literal list
  unsigned _index;
    
  // Number of occurrences in initial clauses
  unsigned _occurrence_counter;
    
  // Common literal pair data
  pair_data * _pair_data;
    
public:
  literal_data(const expression & l_pos,
	       const expression & l_neg,
	       unsigned index)
    :_index(index),
     _occurrence_counter(0),
     _pair_data(new pair_data(l_pos, l_neg))
  {}
    
  literal_data(literal_data * l_pos_data)
    :_index(l_pos_data->_index + 1),
     _occurrence_counter(0),
     _pair_data(l_pos_data->_pair_data)
  {}
    
  const expression & get_literal() const
  {
    return is_positive() ? _pair_data->_l_pos : _pair_data->_l_neg;
  }
    
  const expression & get_opposite() const
  {
    return is_positive() ? _pair_data->_l_neg : _pair_data->_l_pos;
  }
    
  unsigned get_literal_index() const
  {
    return _index;
  }
        
  void increment_occurrence_counter()
  {
    _occurrence_counter++;
  }
    
  unsigned get_occurrence_counter() const
  {
    return _occurrence_counter;
  }
    
  bool is_positive() const
  {
    return !(_index & 1);
  }
    
  bool is_negative() const
  {
    return (_index & 1);
  }
    
  const expression & get_positive() const
  {
    return _pair_data->_l_pos;
  }
    
  const expression & get_negative() const
  {
    return _pair_data->_l_neg;
  }
  
  const std::vector<theory *> & get_observing_theories() const
  {
    return _pair_data->_theories;
  }
    
  void add_observing_theory(theory * th) 
  {
    if(!is_observing_theory(th))
      _pair_data->_theories.push_back(th);
  }
    
  bool is_observing_theory(theory * th) const
  {
    return std::find(_pair_data->_theories.begin(), _pair_data->_theories.end(), th) != _pair_data->_theories.end();
  }
    
  ~literal_data()
  {
    if(is_positive())
      delete _pair_data;
  }
};

class solver {

private:

  typedef std::pair<expression, expression> expression_pair;
  
  struct expression_pair_hash {
    unsigned operator () (const expression_pair & ep) const
    {
      return default_hash_combiner()(ep.second->hash_code(), ep.first->hash_code());
    }
  };
  
  typedef std::unordered_map<expression_pair, 
				  expression, 
				  expression_pair_hash> 
  expression_pair_to_expression_map;

  /* Expression factory */
  expression_factory * _factory;

  /* Data extractor counter */
  data_extractor_counter _extractor_counter;
    
  /* Term data extractor */
  data_extractor<common_data> _common_data;
 
  /* Literal data extractor */
  data_extractor<literal_data> _literal_data;
 
  /* All registered literals */
  expression_vector _literals;

  /* Clause set (initial and learnt clauses) */
  std::vector<clause *> _initial_clauses;
  std::vector<clause *> _learnt_clauses;

  /* Counter for learnt clauses */
  unsigned _learnt_clauses_counter; 

  /* Partial assignment -- assertion trail */
  trail _trail;
  
  /* Conflict set */
  conflict_set _conflict_set;
  
  /* State change indicator */
  bool _state_changed;

  /* Cached equalities */
  expression_pair_to_expression_map _mapped_equalities;

  std::vector<solver_observer *> _observers;
  std::vector<theory *> _theories;
  literal_selection_strategy * _literal_selection_strategy;
  polarity_selection_strategy * _polarity_selection_strategy;
  forget_strategy * _forget_strategy;
  forget_selection_strategy * _forget_selection_strategy;
  restart_strategy * _restart_strategy;

  unsigned _num_of_vars;
  bool _dimacs;

#ifdef _PARALLEL_PORTFOLIO
  unsigned _solver_index;
  std::vector<clause *> _shared_clauses;
  std::vector<unsigned> _shared_indices;
  std::vector<solver *> & _solvers;
  unsigned _share_size_limit;
  unsigned _count_imports;
#endif

  wall_clock _check_and_prop_time_spent;
  wall_clock _explain_time_spent;
  wall_clock _subsume_time_spent;
  wall_clock _backjump_time_spent;
  wall_clock _heuristic_time_spent;
  wall_clock _decide_time_spent;
  
  void cache_equality(const expression & le, const expression & re, const expression & eq)
  { 
    _mapped_equalities.insert(std::make_pair(expression_pair(le, re), eq));
    _mapped_equalities.insert(std::make_pair(expression_pair(re, le), eq));
  }


  // Adds a new literal pair to the solver.
  void add_literals(const expression & l_pos, const expression & l_neg);

  // Clears the literals and their data
  void clear_literals();

  // Introduces a new literal pair that corresponds to the literal l without sending
  // it to the theories.
  void introduce_literal_without_sending(expression & l);

  // Sends literal pair to the theories.
  void send_literals(const expression & l_pos, const expression & l_neg);

  void add_initial_clause(clause * cl);
  void clear_initial_clauses();  
  void add_learnt_clause(clause * cl);
  void clear_learnt_clauses();

#ifdef _PARALLEL_PORTFOLIO
  void check_and_export_shared_clause(clause * cl);
  void import_shared_clauses();
#endif

  unsigned get_first_unlocked_index();
  
public:
#ifndef _PARALLEL_PORTFOLIO
  solver(expression_factory * factory, bool dimacs = false)
    :_factory(factory),
     _common_data(_extractor_counter),
     _literal_data(_extractor_counter),
     _learnt_clauses_counter(0),
     _trail(*this),
     _conflict_set(*this),
     _state_changed(false),
     _mapped_equalities(HASH_TABLE_SIZE),
     _literal_selection_strategy(nullptr),
     _polarity_selection_strategy(nullptr),
     _forget_strategy(nullptr),
     _forget_selection_strategy(nullptr),
     _restart_strategy(nullptr),
     _num_of_vars(0),
     _dimacs(dimacs)
  {
    _literals.reserve(2000);
  }
#else
  solver(expression_factory * factory,  std::vector<solver *> & solvers,
	 unsigned solver_index, unsigned share_size_limit,
	 bool dimacs = false)
    :_factory(factory),
     _common_data(_extractor_counter),
     _literal_data(_extractor_counter),
     _learnt_clauses_counter(0),
     _trail(*this),
     _conflict_set(*this),
     _state_changed(false),
     _mapped_equalities(HASH_TABLE_SIZE),
     _literal_selection_strategy(nullptr),
     _polarity_selection_strategy(nullptr),
     _forget_strategy(nullptr),
     _forget_selection_strategy(nullptr),
     _restart_strategy(nullptr),
     _num_of_vars(0),
     _dimacs(dimacs),    
     _solver_index(solver_index),
     _shared_indices(solvers.size(), 0),
     _solvers(solvers),
     _share_size_limit(share_size_limit),
     _count_imports(0)
  {
    _literals.reserve(2000);
    _shared_clauses.reserve(2000);
  }

  unsigned count_imports() const
  {
    return _count_imports;
  }
#endif  
  
  void clear_state_change()
  {
    _state_changed = false;
  }

  data_extractor_counter & extractor_counter()
  {
    return _extractor_counter;
  }

  literal_data * get_literal_data(const expression & e) const
  {
    return _literal_data.get_data(e);
  }
  void set_literal_data(const expression & e, literal_data * data)
  {
    _literal_data.set_data(e, data);
  } 
  bool has_literal_data(const expression & e) const
  {
    return _literal_data.has_data(e);
  }

  common_data * get_common_data(const expression & e) const
  {
    return _common_data.get_data(e);
  }
  
  void set_common_data(const expression & e, common_data * data)
  {
    _common_data.set_data(e, data);
  }
  
  bool has_common_data(const expression & e) const
  {
    return _common_data.has_data(e);
  }

  bool is_state_changed() const
  {
    return _state_changed;
  }

  bool all_literals_assigned() const
  {
    return _trail.size() == (_literals.size() >> 1);
  }
  
  trail & get_trail()
  {
    return _trail;
  }

  const expression_vector & literals() const
  {
    return _literals;
  }

  const std::vector<clause *> & get_initial_clauses() const
  {
    return _initial_clauses;
  }
  const std::vector<clause *> & get_learnt_clauses() const
  {
    return _learnt_clauses;
  }

  expression_factory * get_factory() const
  {
    return _factory;
  }

  void add_theory(theory * th)
  {
    th->set_index(_theories.size());
    _theories.push_back(th);
  }

  const std::vector<theory *> & get_theories() const
  {
    return _theories;
  }

  void set_num_of_vars(unsigned num_of_vars)
  {
    _num_of_vars = num_of_vars;
  }

  unsigned get_num_of_vars() const
  {
    return _num_of_vars;
  }

  void add_observer(solver_observer * observer)
  {
    _observers.push_back(observer);
  }

  void add_clause(clause * cl)
  {
    add_initial_clause(cl);
  }

  void clear_clauses()
  {
    clear_initial_clauses();
    clear_learnt_clauses();
  }

  void set_literal_selection_strategy(literal_selection_strategy * lss)
  {
    _literal_selection_strategy = lss;
  }

  literal_selection_strategy * get_literal_selection_strategy() 
  {
    return _literal_selection_strategy;
  }

  void set_polarity_selection_strategy(polarity_selection_strategy * pss)
  {
    _polarity_selection_strategy = pss;
  }

  polarity_selection_strategy * get_polarity_selection_strategy()
  {
    return _polarity_selection_strategy;
  }

  void set_restart_strategy(restart_strategy * rs)
  {
    _restart_strategy = rs;
  }

  restart_strategy * get_restart_strategy()
  {
    return _restart_strategy;
  }

  void set_forget_selection_strategy(forget_selection_strategy * fss)
  {
    _forget_selection_strategy = fss;
  }

  forget_selection_strategy * get_forget_selection_strategy()
  {
    return _forget_selection_strategy;
  }
    
  void set_forget_strategy(forget_strategy * fs)
  {
    _forget_strategy = fs;
  }

  forget_strategy * get_forget_strategy()
  {
    return _forget_strategy;
  }



  // Initializes expression data for the literal e and its subexpressions 
  void init_expression_data(const expression & e);

  // Clears expression data for the literal and its subexpressions
  void clear_expression_data(const expression & e);

  // Checks if the expression data has been inited
  bool has_expression_data(const expression & e) const
  {
    return e->has_expression_data();
  }

  // Canonizes the literal
  expression canonize_literal(const expression & e);

  // Gets the literal pair that corresponds to the literal l
  void get_literal_pair(const expression & l, 
			expression & l_pos,
			expression & l_neg);

  // Returns true if the equality of (le, re) is cached, and returns the cached
  // equality by reference eq
  bool is_equality_cached(const expression & le, const expression & re, expression & eq);



  void initialize_solver();
  void initialize_solver_dimacs();

  void apply_introduce_literal(expression & l);  
  bool apply_introduce_shared_equality(const expression & le, 
				       const expression & re,
				       expression & eq,
				       int static_branching_hint,
				       int dynamic_branching_hint,
				       bool next_time_branch,
				       int polarity_hint,
				       bool add_to_polarity_hint);  
  void apply_decide(const expression & l);
  void apply_propagate(const expression & l, theory * source_theory);
  void apply_conflict(const explanation & conflicting, theory * conflict_theory);
  void apply_explain(const expression & l, const explanation & expl);
  void apply_theory_lemma(clause * cl); 
  void apply_backjump(); 
  void apply_forget(unsigned number_of_clauses);
  void apply_restart();

  check_sat_response solve();

  bool is_conflict()
  {
    return _conflict_set.is_conflict();
  }

  bool is_sat()
  {
    return !_conflict_set.is_conflict();
  }

  bool verify_assignment();

  void get_model(const expression_vector & exps);

  void print_reports(std::ostream & ostr);
  
  ~solver();
};


#endif // _SOLVER_H
