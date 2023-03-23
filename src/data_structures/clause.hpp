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

#ifndef _CLAUSE_H
#define _CLAUSE_H

#include "expression_library.hpp"

class clause {
private:
  expression_vector _lits;
  unsigned _watches[2];
  double _activity;
  bool _is_learnt;
  bool _is_locked;
  unsigned _index;
  proof _proof;
public:
  
  typedef expression_vector::iterator iterator;
  typedef expression_vector::const_iterator const_iterator;
  
  clause()
    :_activity(0.0),
     _is_learnt(false),
     _is_locked(false),
     _index(0),
     _proof(nullptr)
  {
    _watches[0] = (unsigned)(-1);
    _watches[1] = (unsigned)(-1);
  }
  
  const expression & operator [] (unsigned i) const
  {
    return _lits[i];
  }
  
  expression & operator [] (unsigned i)
  {
    return _lits[i];
  }

  bool operator == (const clause & cl) const
  {
    return _lits == cl._lits;
  }

  bool operator != (const clause & cl) const
  {
    return _lits != cl._lits;
  }

  void push_back(const expression & l)
  {
    _lits.push_back(l);
  }

  void push_back(expression && l)
  {
    _lits.push_back(std::move(l));
  }
  
  void pop_back()
  {
    _lits.pop_back();
  }

  const expression & back() const
  {
    return _lits.back();
  }

  expression & back()
  {
    return _lits.back();
  }

  unsigned size() const
  {
    return _lits.size();
  }

  void reserve(unsigned n) 
  {
    _lits.reserve(n);
  }

  void resize(unsigned n)
  {
    _lits.resize(n);
  }
  
  unsigned capacity() const
  {
    return _lits.capacity();
  }

  iterator begin()
  {
    return _lits.begin();
  }
  
  iterator end()
  {
    return _lits.end();
  }


  const_iterator begin() const
  {
    return _lits.begin();
  }

  const_iterator end() const
  {
    return _lits.end();
  }

  iterator find_literal(const expression & lit)
  {
    return std::find(_lits.begin(), _lits.end(), lit);
  }

  const_iterator find_literal(const expression & lit) const
  {
    return std::find(_lits.begin(), _lits.end(), lit);
  }

  bool contains_literal(const expression & lit) const
  {
    return find_literal(lit) != end();
  }

  void set_watch_pos(unsigned w, unsigned pos)
  {
    _watches[w] = pos;
  }

  void set_watch_literal(unsigned w, const expression & lit)
  {
    expression_vector::iterator it = 
      std::find(_lits.begin(), _lits.end(), lit);
    assert(it != _lits.end());
    _watches[w] =  it - _lits.begin();
  }
  
  const expression & get_watch_literal(unsigned w) const
  {
    assert(_lits.size() > 0);
    return _lits[_watches[w]];
  }

  unsigned get_watch_pos(unsigned w) const
  {
    return _watches[w];
  }

  bool is_watch_set(unsigned w) const
  {
    return _watches[w] != (unsigned)(-1);
  }

  double get_activity() const
  {
    return _activity;
  }

  void set_activity(double activity)
  {
    _activity = activity;
  }

  bool is_learnt() const
  {
    return _is_learnt;
  }

  void set_learnt(bool is_learnt)
  {
    _is_learnt = is_learnt;
  }

  bool is_locked() const
  {
    return _is_locked;
  }

  void set_locked(bool value)
  {
    _is_locked = value;
  }
  
  unsigned get_index() const
  {
    return _index;
  }

  void set_index(unsigned index)
  {
    _index = index;
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
  
  void print_clause(std::ostream & ostr) const;
};

inline 
std::ostream & operator << (std::ostream & ostr, const clause & c)
{
  c.print_clause(ostr);
  return ostr;
}

#endif // _CLAUSE_H
