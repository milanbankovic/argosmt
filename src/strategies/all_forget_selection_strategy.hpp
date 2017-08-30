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

#ifndef _ALL_FORGET_SELECTION_STRATEGY_H
#define _ALL_FORGET_SELECTION_STRATEGY_H

#include "forget_selection_strategy.hpp"

class all_forget_selection_strategy : public forget_selection_strategy {
public:
  virtual unsigned 
  number_of_clauses_to_forget(std::vector<clause *> & clauses,
			      unsigned start_index)
  {
    return clauses.size() - start_index;
  }
};

#endif // _ALL_FORGET_SELECTION_STRATEGY_H
