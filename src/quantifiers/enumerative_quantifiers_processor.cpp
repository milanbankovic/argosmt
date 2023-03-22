/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2021 Milan Bankovic (milan@matf.bg.ac.rs)

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
#include "enumerative_quantifiers_processor.hpp"
#include "solver.hpp"

void enumerative_quantifiers_processor::check_quantifiers()
{
  if(_insts.size() == 0)
    return;
  
  unsigned old_curr_inst = _curr_inst;
  const expression_vector & terms = _solver.get_congruence_closure_theory_solver()->get_all_terms();
  do
    {
      extended_boolean eb = _solver.get_trail().get_value(_insts[_curr_inst]._qlit);
      if(eb == EB_FALSE) 
	{
	  //skolemize
	  if(_insts[_curr_inst]._gterms.empty() && !_insts[_curr_inst]._skolemized)
	    {
	      expression qlit_ex = _solver.get_literal_data(_insts[_curr_inst]._qlit)->get_opposite();
	      _solver.skolemize(qlit_ex);
	      _insts[_curr_inst]._skolemized = true;
	    }
	}
      else if(eb == EB_TRUE)
	{
	  // instantiate
	  while(_insts[_curr_inst]._term_index < terms.size())
	    {
	      expression term = terms[_insts[_curr_inst]._term_index++];
	      sort s = term->get_inferred_sort();
	      if(s != _insts[_curr_inst]._sort)
		continue;
	      
	      expression_vector gterms = _insts[_curr_inst]._gterms;
	      gterms.push_back(term);
	      if(gterms.size() == _insts[_curr_inst]._qlit->get_quantified_variables().size())
		{
		  _solver.instantiate(_insts[_curr_inst]._qlit, gterms);
		  if(_solver.is_state_changed())
		    break;
		}
	      else
		{
		  const sorted_variable & svar = _insts[_curr_inst]._qlit->get_quantified_variables()[gterms.size()];
		  _insts.push_back(partial_inst { _insts[_curr_inst]._qlit, 0, std::move(gterms), false, svar.get_sort() });
		  old_curr_inst = _curr_inst;
		  break;
		}
	    }
	}	 	  
      _curr_inst = (_curr_inst + 1) % _insts.size();     
    } while(!_solver.is_state_changed() && _curr_inst != old_curr_inst);
}
