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

#ifndef _CLAUSE_THEORY_SOLVER_H
#define _CLAUSE_THEORY_SOLVER_H

#include "theory_solver.hpp"
#include "solver.hpp"
#include "backjump_stack.hpp"
#include "wall_clock.hpp"

class clause_theory_solver : public clause_interface_theory_solver {
private:

  typedef std::vector<clause *> watch_list;

  class clause_theory_solver_data : public expression_data {
  private:
    watch_list _watch_list;
    clause * _reason_clause;
    unsigned _index;
  public:  
    clause_theory_solver_data()
      :_reason_clause(0),
       _index((unsigned)(-1))
    {}
    
    watch_list & get_watch_list()
    {
      return _watch_list;
    }
    
    clause * get_reason_clause() const
    {
      return _reason_clause;
    }
    
    void set_reason_clause(clause * reason_clause)
    {
      _reason_clause = reason_clause;
    }
    
    bool has_reason_clause() const
    {
      return _reason_clause != 0;
    }
    
    void set_index(unsigned i)
    {
      _index = i;
    }
    
    unsigned get_index() const
    {
      return _index;
    }    
  };

  struct on_push {
    void operator () (clause * cl) const
    {
      cl->set_locked(true);
    }
  };

  struct on_pop {
    void operator () (clause * cl) const
    {
      cl->set_locked(false);
    }
  };

  data_extractor<clause_theory_solver_data> _data;
  std::vector<clause *> _pending_clauses;
  std::vector<clause *> _dead_clauses;
  backjump_stack<clause *, on_push, on_pop> _locked_clauses;
  unsigned _current_trail_pos;
  bool _just_backjumped;
  bool _initial_clauses_checked;
  wall_clock _check_and_prop_time_spent;
  wall_clock _backjump_time_spent;
  wall_clock _explain_time_spent;
  wall_clock _new_level_time_spent;

  unsigned _number_of_prop;
  unsigned _number_of_cflt;
  unsigned _number_of_learn_prop;
  unsigned _number_of_learn_cflt;
  
  void add_locked_clause(clause * cl);
  void set_reason_clause(const expression & l, clause * cl, unsigned index);
  unsigned find_initial_watches(clause * cl, unsigned & w0, unsigned & w1);
  void set_initial_watches(clause * cl);
  void check_clauses(const std::vector<clause *> & clauses);
  void process_watch_list(const expression & l);
  bool find_alternative_watch(clause * cl, unsigned cw);
  bool is_clause_true(const trail & tr, clause * cl);

  void count_propagations(clause * cl)
  {
    _number_of_prop++;
    if(cl->is_learnt())
      _number_of_learn_prop++;
  }

  void count_conflicts(clause * cl)
  {
    _number_of_cflt++;
    if(cl->is_learnt())
      _number_of_learn_cflt++;
  }
  
public:
  clause_theory_solver(solver & sl)
    :clause_interface_theory_solver(sl, "Clause", 1),
     _data(sl.extractor_counter()),
     _current_trail_pos(0),
     _just_backjumped(false),
     _initial_clauses_checked(false),
     _number_of_prop(0),
     _number_of_cflt(0),
     _number_of_learn_prop(0),
     _number_of_learn_cflt(0)

  {
    _dead_clauses.reserve(100);
    _pending_clauses.reserve(100);
    _locked_clauses.reserve(10000);
  }

  virtual void add_clause(clause * cl)
  {
    _pending_clauses.push_back(cl);
  }
  
  virtual void remove_clauses(const std::vector<clause *> & learnt_clauses,
			      unsigned number_of_clauses);

  
  virtual void add_literal(const expression & l_pos, const expression & l_neg)
  {
    _solver.get_literal_data(l_pos)->add_observing_theory_solver(this);
    _data.set_data(l_pos, new clause_theory_solver_data());
    _data.set_data(l_neg, new clause_theory_solver_data());
  }
    
  virtual void new_level();

  virtual void backjump(unsigned level);

  virtual void check_and_propagate(unsigned layer); 

  virtual void explain_literal(const expression & l);

  virtual bool is_owned_expression(const expression & e)
  {
    return e->is_true() || e->is_false() || e->is_sat_literal()
      || (e->is_constant_function() &&  e->is_formula());
  }

  virtual expression canonize_expression(const expression & e);

  virtual void get_literal_pair(const expression & l, expression & l_pos, expression & l_neg);
  
  virtual bool verify_assignment(const trail & tr);

  virtual void get_model(const expression_vector & exps);

  virtual void print_report(std::ostream & ostr)
  {
    ostr << "-------- CLAUSE THEORY SOLVER---------" << std::endl;
    ostr << "Time spent in check_and_propagate: " << _check_and_prop_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in new_level: " << _new_level_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in backjump: " << _backjump_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in explain: " << _explain_time_spent.cumulative_time() << std::endl;
    ostr << "Propagations: all: " << _number_of_prop << ", learnt: " << _number_of_learn_prop << " ("
	 << (_number_of_learn_prop / (double)_number_of_prop) << ")" << std::endl;
    ostr << "Conflicts: all: " << _number_of_cflt << ", learnt: " << _number_of_learn_cflt << " ("
	 << (_number_of_learn_cflt / (double)_number_of_cflt) << ")" << std::endl;
    
  }
  
};

#endif // _CLAUSE_THEORY_SOLVER_H
