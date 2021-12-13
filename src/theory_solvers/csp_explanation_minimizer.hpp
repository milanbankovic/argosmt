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

#ifndef _CSP_EXPLANATION_MINIMIZER_H
#define _CSP_EXPLANATION_MINIMIZER_H

#include "csp_theory_solver.hpp"

struct explanation_minimizer {
  expression_set set;
  int lower;
  int upper;
  expression l_cause;
  expression u_cause;
  csp_theory_solver * th;

  explanation_minimizer(csp_theory_solver * t = 0)
    :set(HASH_TABLE_SIZE),
     lower((int)0x80000000),
     upper((int)0x7fffffff),
     l_cause(expression()),
     u_cause(expression()),
     th(t)
  {}
  
  void reset(csp_theory_solver * t)
  {
    set.clear();
    set.rehash(HASH_TABLE_SIZE);
    lower = (int)0x80000000;
    upper = (int)0x7fffffff;
    l_cause = expression();
    u_cause = expression();
    th = t;
  }
  
  void push_back(const expression & e)
  {
    const function_symbol & fs = e->get_symbol();

    if(fs == function_symbol::DISTINCT)
      {
	set.insert(e);
      }
    else if(fs == function_symbol::EQ)
      {
	int b = th->get_theory_solver_data(e->get_operands()[1])->get_int_value();
	if(b <= upper)
	  {
	    upper = b;
	    u_cause = e;
	  }
	if(b >= lower)
	  {
	    lower = b;
	    l_cause = e;
	  }
      }
    else if(fs == function_symbol::LE)
      {
	int ub = th->get_theory_solver_data(e->get_operands()[1])->get_int_value();
	if(ub < upper)
	  {
	    upper = ub;
	    u_cause = e;
	  }
      }
    else if(fs == function_symbol::LT)
      {
	int ub = th->get_theory_solver_data(e->get_operands()[1])->get_int_value() - 1;
	if(ub < upper)
	  {
	    upper = ub;
	    u_cause = e;
	  }
      }
    else if(fs == function_symbol::GE)
      {
	int lb = th->get_theory_solver_data(e->get_operands()[1])->get_int_value();
	if(lb > lower)
	  {
	    lower = lb;
	    l_cause = e;
	  }
      }
    else if(fs == function_symbol::GT)
      {
	int lb = th->get_theory_solver_data(e->get_operands()[1])->get_int_value() + 1;
	if(lb > lower)
	  {
	    lower = lb;
	    l_cause = e;
	  }
      }
  }


  void export_to_expl(explanation & expl)
  {
    if(u_cause.get() != 0)
      expl.push_back(u_cause);
    if(l_cause.get() != 0 && l_cause != u_cause)
      expl.push_back(l_cause);

    for(expression_set::const_iterator it = set.begin(), it_end = set.end();
	it != it_end; ++it)
      {
	if((*it)->is_disequality())
	  {
	    int v = th->get_theory_solver_data((*it)->get_operands()[1])->get_int_value();
	    if(v > upper || v < lower)
	      continue;
	  }
	  
	expl.push_back(*it);
      }
  }

};



struct multivar_explanation_minimizer {
  typedef std::shared_ptr<explanation_minimizer> em_ptr;
  typedef std::unordered_map<expression, em_ptr> map_type;
  map_type _map;
  csp_theory_solver * _th;
  multivar_explanation_minimizer(csp_theory_solver * th = 0)
    :_map(HASH_TABLE_SIZE),
     _th(th)
  {}

  void reset(csp_theory_solver * th)
  {
    for(map_type::iterator it = _map.begin(), it_end = _map.end();
    	it != it_end; ++it)
      {
    	it->second->reset(th);
      }
    _th = th;
  }
  
  void push_back(const expression & l)
  {
    const expression & var = l->get_operands()[0];
    map_type::iterator it;
    if((it = _map.find(var)) == _map.end())
      {
	it = _map.insert(std::make_pair(var, em_ptr(new explanation_minimizer(_th)))).first;
      }
    it->second->push_back(l);
  }

  void export_to_expl(explanation & expl)
  {
    for(map_type::iterator it = _map.begin(), it_end = _map.end();
	it != it_end; ++it)
      {
	it->second->export_to_expl(expl);
      }
  }
};

#endif // _CSP_EXPLANATION_MINIMIZER_H
