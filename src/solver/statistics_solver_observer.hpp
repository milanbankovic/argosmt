/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2014 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _STATISTICS_SOLVER_OBSERVER_H
#define _STATISTICS_SOLVER_OBSERVER_H

#include "solver_observer.hpp"

#include <iostream>

class statistics_solver_observer : public solver_observer {
private:
  unsigned _count_propagate;
  unsigned _count_decide;
  unsigned _count_backjump;
  unsigned _count_conflict;
  unsigned _count_explain;
  unsigned _count_forget;
  unsigned _count_restart;
  unsigned _count_learn;
  unsigned _count_literal_introduce;
  unsigned _count_backjump_to_zero;
  unsigned _period;

  unsigned _cum_cflt_expl_size;
  unsigned _cum_prop_expl_size;

  std::ostream & _ostr;

  std::vector<unsigned> _theory_solver_count_conflict;
  std::vector<unsigned> _theory_solver_count_propagate;

  bool _inited;

  unsigned _current_level;
  unsigned _cum_bckjmp_distance;
  unsigned _cum_bckjmp_cl_size;

  void init()
  {
    _theory_solver_count_conflict.resize(_solver.get_theory_solvers().size(), 0);
    _theory_solver_count_propagate.resize(_solver.get_theory_solvers().size(), 0);
    _inited = true;
  }

public:
  statistics_solver_observer(solver & sl, std::ostream & ostr, unsigned period = 1000)
    :solver_observer(sl),
     _count_propagate(0),
     _count_decide(0),
     _count_backjump(0),
     _count_conflict(0),
     _count_explain(0),
     _count_forget(0),
     _count_restart(0),
     _count_learn(0),
     _count_literal_introduce(0),
     _count_backjump_to_zero(0),
     _period(period),
     _cum_cflt_expl_size(0),
     _cum_prop_expl_size(0),
     _ostr(ostr),
     _inited(false),
     _current_level(0),
     _cum_bckjmp_distance(0),
    _cum_bckjmp_cl_size(0)
  {}
  
  virtual void literal_introduced(const expression & l,
				  const expression & l_opp)
  {
    ++_count_literal_introduce;
    if(_count_literal_introduce % _period == 0)
      _ostr << "i" << std::flush;
  }

  virtual void decide_applied(const expression & l)
  {
    ++_count_decide;
    if(_count_decide % _period == 0)
      _ostr << "d" << std::flush;
    ++_current_level;    
  }

  virtual void propagate_applied(const expression & l, 
				const theory_solver * source_ts)
  {
    ++_count_propagate;
    if(_count_propagate % (_period * 100) == 0)
      _ostr << "p" << std::flush;

    if(!_inited)
      init();
    _theory_solver_count_propagate[source_ts->get_index()]++;
  }

  virtual void conflict_applied(const explanation & conflicting,
				const theory_solver * conflict_ts)
  {
    ++_count_conflict;
    _cum_cflt_expl_size += conflicting.size();
    
    if(!_inited)
      init();
    _theory_solver_count_conflict[conflict_ts->get_index()]++;
  }

  virtual void explain_applied(const expression & l, 
			       const explanation & expl)
  {
    ++_count_explain;
    _cum_prop_expl_size += expl.size();
  }

  virtual void learn_applied(const clause * cl)
  {
    ++_count_learn;
  }
    
  virtual void backjump_applied(unsigned level, const clause * cl,
				const expression & literal)
  {
    ++_count_backjump;
    if(level == 0)
      {
	++_count_backjump_to_zero;
	_ostr << "z" << std::flush;
      }

    if(_count_backjump % _period == 0)
      _ostr << "b" << std::flush;
    _cum_bckjmp_distance += _current_level - level;
    _cum_bckjmp_cl_size += cl->size();
    _current_level = level;
  }
  
  virtual void forget_applied(const std::vector<clause *> & learnt_clauses,
			      unsigned number_of_clauses)
  {
    ++_count_forget;
    if(_count_forget % _period == 0)
      _ostr << "f" << std::flush;
  }
  
  virtual void restart_applied()
  {
    ++_count_restart;
    _ostr << "r" << std::endl;
    //report();
  }
  
  void report()
  {
    _ostr << "------------------------------------------------------------------" << std::endl;
    _ostr << "Number of restarts: " << _count_restart << std::endl;
    _ostr << "Number of initial clauses: " << _solver.get_initial_clauses().size() << std::endl;
    _ostr << "Number of learnt clauses: " << _solver.get_learnt_clauses().size() << std::endl;
    _ostr << "Number of literal introductions: " << _count_literal_introduce <<
      std::endl;
    _ostr << "Number of decides: " << _count_decide << std::endl;
    _ostr << "Number of propagations: " 
	      << _count_propagate << std::endl;
    _ostr << "Number of conflicts: " << _count_conflict << std::endl;
    _ostr << "Number of explains: " << _count_explain << std::endl;
    _ostr << "Number of backjumps: " << _count_backjump << std::endl;
    _ostr << "Number of zero-backjumps: " << _count_backjump_to_zero << std::endl;
    _ostr << "Number of learns: " << _count_learn << std::endl;
    _ostr << "Number of forgets: " << _count_forget << std::endl;
    _ostr << "Average number of propagations per cycle: " 
	  << (_count_propagate + 0.0) / 
      (_count_decide + _count_backjump) << std::endl;
    _ostr << "Average number of explains per conflict: " 
	  << (_count_explain + 0.0) / _count_backjump << std::endl;
    _ostr << "Average size of conflict explanation: " 
	  << (_cum_cflt_expl_size + 0.0) / _count_conflict << std::endl;
    _ostr << "Average size of propagation explanation: " 
	  << (_cum_prop_expl_size + 0.0) / _count_explain << std::endl;
    _ostr << "Average backjump distance: " 
	  << (_cum_bckjmp_distance + 0.0) / _count_backjump << std::endl;
    _ostr << "Average backjump clause size: " 
	  << (_cum_bckjmp_cl_size + 0.0) / _count_backjump << std::endl;   
    _ostr << "------------------------------------------------------------------" << std::endl;
  }

  void theory_solver_report(unsigned index)
  {    
    theory_solver * th = _solver.get_theory_solvers()[index];
    _ostr << "------------------------------------------------------------------" << std::endl;
    _ostr << "Theory_Solver: " << th->get_name() << std::endl;
    _ostr << "Propagations: " << _theory_solver_count_propagate[index];
    if(_count_propagate > 0)
      _ostr << " (" << (0.0 + _theory_solver_count_propagate[index]) / _count_propagate << ")" << std::endl;
    else
      _ostr << std::endl;
    _ostr << "Conflicts: " << _theory_solver_count_conflict[index];
    if(_count_conflict > 0)
      _ostr << " (" << (0.0 + _theory_solver_count_conflict[index]) / _count_conflict << ")" << std::endl;
    else
      _ostr << std::endl;
    _ostr << "------------------------------------------------------------------" << std::endl;
  }

  void all_theories_report()
  {
    if(!_inited)
      init();
    for(unsigned i = 0; i < _solver.get_theory_solvers().size(); i++)
      theory_solver_report(i);
  }

  void get_statistics(attribute_set & attr)
  {
    attr.insert(attribute(keyword(":propagate-count"), _count_propagate));
    attr.insert(attribute(keyword(":decide-count"), _count_decide));
    attr.insert(attribute(keyword(":conflict-count"), _count_conflict));
    attr.insert(attribute(keyword(":backjump-count"), _count_backjump));
    attr.insert(attribute(keyword(":zero-backjump-count"), _count_backjump_to_zero));
    attr.insert(attribute(keyword(":learn-count"), _count_learn));
    attr.insert(attribute(keyword(":explain-count"), _count_explain));
    attr.insert(attribute(keyword(":forget-count"), _count_forget));
    attr.insert(attribute(keyword(":restart-count"), _count_restart));
    attr.insert(attribute(keyword(":avg-prop-per-cycle"), 
			  (_count_propagate + 0.0) /
			  (_count_decide + _count_backjump)));
    attr.insert(attribute(keyword(":avg-expl-per-cflt"),
			  (_count_explain + 0.0) / _count_conflict));
    attr.insert(attribute(keyword(":avg-cflt-expl-size"),
			  (_cum_cflt_expl_size + 0.0) / _count_conflict));
    attr.insert(attribute(keyword(":avg-prop-expl-size"),
			  (_cum_prop_expl_size + 0.0) / _count_explain));
    attr.insert(attribute(keyword(":avg-bckjmp-dist"),
			  (_cum_bckjmp_distance + 0.0) / _count_backjump));
    attr.insert(attribute(keyword(":avg-bckjmp-cl-size"),
			  (_cum_bckjmp_cl_size + 0.0) / _count_backjump));
    
  }
};


#endif // _LOGGING_SOLVER_OBSERVER_H
