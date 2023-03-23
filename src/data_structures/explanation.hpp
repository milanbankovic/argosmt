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
  proof _proof;
public:
  explanation()
    :_lits(new expression_vector()),
     _sl(0),
     _cl(0),
     _index((unsigned)(-1)),
     _proof(nullptr)
  {}
  
  explanation(solver * sl, clause * cl)
    :_lits(),
     _sl(sl),
     _cl(cl),
     _index((unsigned)(-1)),
     _proof(cl->get_proof())
  {}
  
  explanation(solver * sl, clause * cl, unsigned index)
    :_lits(),
     _sl(sl),
     _cl(cl),
     _index(index),
     _proof(cl->get_proof())
  {}

  bool is_clause_explanation() const
  {
    return _cl != nullptr;
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

  void set_proof(const proof & pr)
  {
    _proof = pr;
  }

  void reset_proof()
  {
    _proof.reset();
  }

  const proof & get_proof() const
  {
    return _proof;
  }

  bool has_proof() const
  {
    return _proof.get() != nullptr;
  }
  
  void out(std::ostream & ostr) const;
};

inline
std::ostream & operator << (std::ostream & ostr, const explanation & expl)
{
  expl.out(ostr);
  return ostr;
}

#endif // _EXPLANATION_H
