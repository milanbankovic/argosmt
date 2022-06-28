/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2015 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _EXPLANATION_H
#define _EXPLANATION_H

#include "clause.hpp"

class solver;

class explanation {
private:
  std::shared_ptr<expression_vector> _lits;
  solver * _sl;
  clause * _cl;
  unsigned _index;
public:
  explanation()
    :_lits(new expression_vector()),
     _sl(0),
     _cl(0),
     _index((unsigned)(-1))
  {}
    
  explanation(solver * sl, clause * cl, unsigned index = (unsigned)(-1))
    :_lits(),
     _sl(sl),
     _cl(cl),
     _index(index)
  {
#ifdef _TRAIL_SAVING
    if(cl != nullptr)
      cl->increment_expl_obj_count();
#endif
  }

#ifdef _TRAIL_SAVING
  explanation(const explanation & expl)
    :_lits(expl._lits),
     _sl(expl._sl),
     _cl(expl._cl),
     _index(expl._index)
  {
    if(_cl != nullptr)
      _cl->increment_expl_obj_count();
  }
  explanation(explanation && expl)
    :_lits(std::move(expl._lits)),
     _sl(expl._sl),
     _cl(expl._cl),
     _index(expl._index)
  {
    expl._cl = nullptr;
  }

  explanation & operator = (const explanation & expl)
  {
    if(&expl == this)
      return *this;

    _lits = expl._lits;
    _sl = expl._sl;
    if(_cl != nullptr)
      _cl->decrement_expl_obj_count();
    _cl = expl._cl;
    if(_cl != nullptr)
      _cl->increment_expl_obj_count();
    _index = expl._index;
    
    return *this;    
  }
  
  explanation & operator = (explanation && expl)
  {
    if(&expl == this)
      return *this;

    _lits = std::move(expl._lits);
    _sl = expl._sl;
    if(_cl != nullptr)
      _cl->decrement_expl_obj_count();
    _cl = expl._cl;
    expl._cl = nullptr;
    _index = expl._index;

    return *this;    
  }

#endif
  
  bool is_clause_explanation() const
  {
    return _cl != 0;
  }
  
  clause * get_clause() const
  {
    return _cl;
  }

  const expression & operator [] (unsigned i) const;
  unsigned size() const;
  void push_back(const expression & l);
  void push_back(expression && l);
  void reserve(unsigned n);
  const expression & back() const
  {
    return operator[](size() - 1);
  }
  
  void out(std::ostream & ostr) const;

#ifdef _TRAIL_SAVING
  ~explanation()
  {
    if(_cl != nullptr)
      _cl->decrement_expl_obj_count();
  }
#endif 
};

inline
std::ostream & operator << (std::ostream & ostr, const explanation & expl)
{
  expl.out(ostr);
  return ostr;
}

#endif // _EXPLANATION_H
