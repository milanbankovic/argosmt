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

#ifndef _EUF_THEORY_SOLVER_H
#define _EUF_THEORY_SOLVER_H

#include "theory_solver.hpp"
#include <queue>
#include "solver.hpp"
#include "wall_clock.hpp"
#include "history_saver.hpp"
#include "vector_hash_code.hpp"
#include <limits>
#include <functional>
#include <utility>

class euf_theory_solver : public congruence_closure_interface_theory_solver {
private:

  class lookup_key {
  private:
    function_symbol _symbol;
    expression_vector _args;
    std::size_t _hash_code;

    void calculate_hash()
    {
      _hash_code = vector_hash_code<expression>(_symbol.hash_code())(_args);      
    }
    
  public:
    lookup_key(const function_symbol & symbol, 
	       expression_vector && args)
      :_symbol(symbol),
       _args(std::move(args))
    {
      calculate_hash();
    }
    
    bool operator == (const lookup_key & ft) const
    {
      return _symbol == ft._symbol && _args == ft._args;
    }
    
    const function_symbol & symbol() const
    {
      return _symbol;
    }

    const expression_vector & args() const
    {
      return _args;
    }

    std::size_t hash_code() const
    {
      return _hash_code;
    }
    
  };

  class lookup_key_hash {
  public:
    std::size_t operator () (const lookup_key & lk) const
    {
      return lk.hash_code();
    }
  };

  typedef std::pair<expression, expression> expression_pair;
    
  class forest_entry {
  private:
    expression _parent;
    bool _explain;
  public:
    forest_entry()
    {}

    forest_entry(const expression & parent, bool explain = false)
      :_parent(parent),
       _explain(explain)
    {}
    
    const expression & parent() const
    {
      return _parent;
    }
    
    bool should_explain() const
    {
      return _explain;
    }
  };


  typedef std::unordered_map<lookup_key, 
			     expression, 
			     lookup_key_hash>  lookup_table;

  typedef std::unordered_map<expression, 
			     expression> expression_to_expression_map;


  typedef std::unordered_map<expression, 
			     expression_vector> expression_to_vector_map;
  

  typedef std::unordered_map<expression,
			     forest_entry> expression_to_forest_map;



  class euf_theory_solver_data : public expression_data {
  private:
    bool _relevant;
    const volatile unsigned & _current_level;
    history_saver<expression> _representative;
    history_saver<unsigned> _class_list_length;
    expression_vector _class_list;
    history_saver<unsigned> _use_list_length;
    expression_vector _use_list;
    history_saver<forest_entry> _proof_forest;
    
  public:
    euf_theory_solver_data(const expression e, bool relevant, const unsigned & level)
      :_relevant(relevant),
       _current_level(level),
       _representative(level, e),
       _class_list_length(level, 1U),
       _class_list{e},
       _use_list_length(level, 0U),
       _use_list(),
       _proof_forest(level, forest_entry(e))
    {
      if(!relevant)
	return;
    }
    
    bool is_relevant() const
    {
      return _relevant;
    }
    
    void restore_state()
    {      
      _representative.restore_state();      
      _class_list_length.restore_state();
      if(_class_list_length.const_ref() != std::numeric_limits<unsigned>::max())
	_class_list.resize(_class_list_length.const_ref());
      _use_list_length.restore_state();
      if(_use_list_length.const_ref() != std::numeric_limits<unsigned>::max())
	_use_list.resize(_use_list_length.const_ref());
      _proof_forest.restore_state();
    }

    unsigned current_level() const
    {
      return _current_level;
    }

    const expression & get_representative() const
    {
      return _representative.const_ref();
    }

    void set_representative(const expression & e)
    {
      _representative.ref() = e;
    }

    const expression_vector & get_class_list() const
    {
      if(_class_list_length.const_ref() != std::numeric_limits<unsigned>::max())	
	return _class_list;
      else
	return EMPTY_EXPRESSION_VECTOR;
    }

    void add_to_class_list(const expression & e)
    {
      if(_class_list_length.const_ref() == std::numeric_limits<unsigned>::max())
	throw "FATAL: class_list empty!";
      
      _class_list_length.ref()++;
      _class_list.push_back(e);
    }

    void clear_class_list()
    {
      _class_list_length.ref() = std::numeric_limits<unsigned>::max();
    }

    const expression_vector & get_use_list() const
    {
      if(_use_list_length.const_ref() != std::numeric_limits<unsigned>::max())
	return _use_list;
      else
	return EMPTY_EXPRESSION_VECTOR;
    }

    void add_to_use_list(const expression & e)
    {
      if(_use_list_length.const_ref() == std::numeric_limits<unsigned>::max())
	throw "FATAL: use list empty!";
      
      _use_list_length.ref()++;
      _use_list.push_back(e);
    }

    void clear_use_list()
    {
      _use_list_length.ref() = std::numeric_limits<unsigned>::max();
    }

    const forest_entry & get_proof_forest() const
    {
      return _proof_forest.const_ref();
    }
    
    void set_proof_forest(const forest_entry & fe)
    {
      _proof_forest.ref() = fe;
    }
  };


  class backjump_lookup_func {
  private:
    lookup_table & _lookup;
  public:
    backjump_lookup_func(lookup_table & lookup)
      :_lookup(lookup)
    {}
    
    void operator () (const std::reference_wrapper<const lookup_key> & elem) const
    {
      _lookup.erase(elem);
    }
    
  };


  data_extractor<euf_theory_solver_data> _data;
  unsigned _current_level;
  expression_vector _terms;
  lookup_table _lookup;
  backjump_stack<std::reference_wrapper<const lookup_key>,
		 no_op<std::reference_wrapper<const lookup_key>>,
		 backjump_lookup_func> _lookup_history;
  expression_vector _trivial_propagations;
  expression_vector _new_literals;

  struct dpc_entry {
    expression cdiseq;
    bool swapped;
  };
  
  std::unordered_map<expression, dpc_entry>  _disequality_propagation_causes;
  expression_to_expression_map _predicate_propagation_causes;
  unsigned _current_assertion_pos;
  bool _just_backjumped;
  wall_clock _check_and_prop_time_spent;
  wall_clock _backjump_time_spent;
  wall_clock _explain_time_spent;
  wall_clock _new_level_time_spent;
  
  const lookup_table & lookup() const
  {
    return _lookup;
  }

  void insert_lookup(lookup_key && key, const expression & value)
  {
    auto ret = _lookup.insert(std::make_pair(std::move(key), value));
    _lookup_history.push(std::cref(ret.first->first));
  }
  
  void new_expression(const expression & e);
  void merge(const expression & a, const expression & b)
  {
    std::queue<expression_pair> pending_unions;
    pending_unions.push(expression_pair(a, b));
    process_pending_unions(pending_unions);	
  }
  void add_to_proof_forest(const expression & a, 
			   const expression & b,
			   bool explain);
  void process_pending_unions(std::queue<expression_pair> & pending_unions,
			      bool explain = false);
  void check_eq_propagations(const expression_vector & cl1, 
			  const expression_vector & cl2);
  void check_diseq_propagations(const expression_vector & cl1,
				const expression_vector & cl2,
				const expression & cdiseq);
  expression find_common_ancestor(expression_to_expression_map & proof_state, 
				  expression a, expression b);
  void explain_along_path(expression_to_expression_map & proof_state, 
			  expression_to_vector_map & class_lists,
			  std::vector<expression_pair> & expl_output,
			  std::queue<expression_pair> & pending_proofs,
			  expression a, expression c);
  void explain(const expression & c1, const expression & c2, 
	       std::vector<expression_pair> & expl_output);
  void generate_explanation(const std::vector<expression_pair> & expl_output, explanation & expl);
  void explain_equality(const expression & eq, explanation & expl);
  void explain_disequality(const expression & diseq, explanation & expl);
  void explain_predicate(const expression & pred, explanation & expl);
  void explain_conflict(const expression & left, const expression & right, explanation & expl,
			const expression & cflt1, const expression & cflt2 = expression());
  lookup_key get_lookup_key(const expression & exp);
  void register_expression(const expression & exp);
  expression canonize_eq_diseq(const expression & l);
  void check_predicate_propagations(const expression & ra, const expression & rb);
  bool get_simplest_in_class(const expression & e, expression & sim);  
  bool check_literal_relevancy(const expression & e);
  bool check_term_relevancy(const expression & e);
  void apply_trivial_propagation(const expression & l);
  void process_assertion(const expression & l);
  
public:
  euf_theory_solver(solver & sl)
    :congruence_closure_interface_theory_solver(sl, "EUF", 1),
     _data(sl.extractor_counter()),
     _current_level(0),
     _lookup(HASH_TABLE_SIZE),
     _lookup_history(0),
     _disequality_propagation_causes(HASH_TABLE_SIZE),
     _current_assertion_pos(0),
     _just_backjumped(false)
  {}

  virtual void add_literal(const expression & l_pos, const expression & l_neg);
  
  virtual void new_level();
  
  virtual void backjump(unsigned level);

  virtual void check_and_propagate(unsigned layer);
  
  virtual void explain_literal(const expression & l);
  
  virtual bool is_owned_expression(const expression & e);

  virtual expression canonize_expression(const expression & e);

  virtual void get_literal_pair(const expression & l, 
				expression & l_pos,
				expression & l_neg);


  virtual bool verify_assignment(const trail & tr)
  {
    return true;
  }

  virtual void get_model(const expression_vector & exps);

  virtual bool are_congruent(const expression & x, const expression & y) const
  {
    return _data.get_data(x)->get_representative() == 
      _data.get_data(y)->get_representative();
  }

  virtual const expression & get_representative(const expression & e) const
  {
    return _data.has_data(e) ? _data.get_data(e)->get_representative() : _solver.get_factory()->UNDEFINED_EXPRESSION();
  }

  virtual const expression_vector & get_congruence_class(const expression & e) const
  {
    return _data.has_data(e) ? _data.get_data(_data.get_data(e)->get_representative())->get_class_list() : EMPTY_EXPRESSION_VECTOR;
  }

  virtual const expression_vector & get_all_terms() const
  {
    return _terms;
  }

  virtual void add_expression(const expression & e)
  {
    register_expression(e);
  }
  
  virtual void print_report(std::ostream & ostr)
  {
    ostr << "-------- EUF THEORY SOLVER ---------" << std::endl;
    ostr << "Time spent in check_and_propagate: " << _check_and_prop_time_spent.cumulative_time() << std::endl;  
    ostr << "Time spent in new_level: " << _new_level_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in backjump: " << _backjump_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in explain: " << _explain_time_spent.cumulative_time() << std::endl;
  }

};


#endif // _EUF_THEORY_SOLVER_H
