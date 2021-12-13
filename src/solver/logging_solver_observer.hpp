/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2013 Milan Bankovic (milan@matf.bg.ac.rs)

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
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02010-1301  USA
***************************************************************************/

#ifndef _LOGGING_SOLVER_OBSERVER_H
#define _LOGGING_SOLVER_OBSERVER_H

#include "solver_observer.hpp"

#include <iostream>

class logging_solver_observer : public solver_observer {
private:
  std::ostream & _ostr;

public:
  logging_solver_observer(solver & sl, std::ostream & ostr)
    :solver_observer(sl),
     _ostr(ostr)
  {}

  virtual void literal_introduced(const expression & l,
				  const expression & l_opp)
  {
    _ostr << "Literal Introduced: " << l << " : " << l_opp << " (" << 
      _solver.get_literal_data(l)->get_literal_index() << ")" << std::endl;
  }

  virtual void decide_applied(const expression & l)
  {
  
    _ostr << "Decide applied: " << l << " (" << _solver.get_trail().get_trail_level(l) << ")" << std::endl;
  }

  virtual void propagate_applied(const expression & l, 
				 const theory_solver * source_ts)
  {
    _ostr << "Propagation applied: " << l << " (theory_solver " << source_ts->get_name() << ")" << std::endl;
  }

  virtual void conflict_applied(const explanation & conflicting,
				const theory_solver * conflict_ts)
  {
     _ostr << "Conflict applied (theory_solver " << conflict_ts->get_name() << "): ";
    for(unsigned i = 0; i < conflicting.size(); i++)
      _ostr << conflicting[i] << ",";
    _ostr << std::endl; 
  }

  virtual void explain_applied(const expression & l, const explanation & expl)
  {
    _ostr << "Explain applied: " << l 
    	  << " (" << _solver.get_trail().get_source_theory_solver(l)->get_name() 
    	  << ") (level: " << _solver.get_trail().get_trail_level(l) 
    	  << ") : (size: " << expl.size() << ") : ";
    for(unsigned i = 0; i < expl.size(); i++)
      _ostr << expl[i] << ",";
    _ostr << std::endl; 
  }

  virtual void learn_applied(const clause * cl)
  {
    _ostr << "Learned applied: " << *cl << std::endl;
  }
    
  virtual void backjump_applied(unsigned level, const clause * cl,
				const expression & literal)
  {
    _ostr << "Backjump to: " << level << " " << *cl << std::endl;
  }
  
  virtual void forget_applied(const std::vector<clause *> & learnt_clauses,
			      unsigned number_of_clauses)
  {
    _ostr << "Forget applied: " << number_of_clauses << "(of: " << learnt_clauses.size() << ")" << std::endl;
  }

  virtual void restart_applied()
  {
    _ostr << "Restarted" << std::endl;
  }
};


#endif // _LOGGING_SOLVER_OBSERVER_H
