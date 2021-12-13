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
#ifndef _ENUMERATIVE_QUANTIFIERS_PROCESSOR_H
#define _ENUMERATIVE_QUANTIFIERS_PROCESSOR_H

#include "quantifiers_processor.hpp"

class enumerative_quantifiers_processor : public quantifiers_processor {
private:  
  struct partial_inst {
    expression _qlit = nullptr;
    unsigned _term_index = 0;
    expression_vector _gterms;
    bool _skolemized = false;
    sort _sort = nullptr;
  };
  std::vector<partial_inst> _insts;
  unsigned _curr_inst;
public:
  enumerative_quantifiers_processor(solver & sl)
    :quantifiers_processor(sl),
     _curr_inst(0)
  {}

  virtual void add_literal(const expression & l_pos, const expression & l_neg)
  {
    if(!l_pos->is_quantifier())
      return;

    _insts.push_back(partial_inst { l_pos, 0, expression_vector {}, false, l_pos->get_quantified_variables()[0].get_sort() });    
  }

  virtual void new_level() {}
  
  virtual void backjump(unsigned level) {}
  
  virtual void check_quantifiers();

};

#endif // _ENUMERATIVE_QUANTIFIERS_PROCESSOR_H
