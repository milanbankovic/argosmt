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
#ifndef _QUANTIFIERS_PROCESSOR_H
#define _QUANTIFIERS_PROCESSOR_H

#include "clause.hpp"

class solver;

class quantifiers_processor {
protected:
  solver & _solver;
public:
  quantifiers_processor(solver & sl)
    :_solver(sl)
  {}
  
  solver & get_solver()
  {
    return _solver;
  }

  /* Informs the q-processor that a new literal pair is introduced.
     IMPORTANT: This method should not affect the solver's state
     (i.e. invoke _solver.apply_* methods or skolemize() and
     instantiate() methods). This method should be used only to
     initialize needed internal data structures of the q-processor. */
  virtual void add_literal(const expression & l_pos, const expression & l_neg) = 0;

  /* Informs the q-processor that a new decision level is
     established.  */
  virtual void new_level() = 0;
  
  /* Informs the q-processor that the state has backjumped to the given
     level.  */
  virtual void backjump(unsigned level) = 0;
  
  /* Ask the q-processor to perform quantifier skolemizations and instantiations.
     If there are such, the q-processor should invoke solver.skolemize() and 
     solver.instantiate() for appropriate quantifier literals (and terms for 
     instantiation) */
  virtual void check_quantifiers() = 0;
  
  virtual ~quantifiers_processor() {}
};

#endif // _QUANTIFIER_PROCESSOR_H
