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

#ifndef _CLAUSE_LENGTH_FORGET_SELECTION_STRATEGY_H
#define _CLAUSE_LENGTH_FORGET_SELECTION_STRATEGY_H

#include "percent_to_forget_selection_strategy.hpp"

class clause_length_forget_selection_strategy : 
  public percent_to_forget_selection_strategy {  
public:
  clause_length_forget_selection_strategy(double percent_to_forget = 0.5)
    :percent_to_forget_selection_strategy(percent_to_forget)
  {}
  
  virtual unsigned 
  number_of_clauses_to_forget(std::vector<clause *> & clauses,
			      unsigned start_index)
  {
    std::sort(clauses.begin() + start_index, clauses.end(),
	      [] (const clause * c1, const clause *c2) 
	      {
		return c1->size() < c2->size();
	      });
    return percent_to_forget_selection_strategy::number_of_clauses_to_forget(clauses, start_index);
  }
};

#endif // _CLAUSE_LENGTH_FORGET_SELECTION_STRATEGY_H
