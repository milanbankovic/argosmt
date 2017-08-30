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

#ifndef _CONFLICT_SET_H
#define _CONFLICT_SET_H

#include "expression_library.hpp"
#include "argosmt_expression_data.hpp"

class clause;
class explanation;
class solver;

class conflict_set {
private:

  struct conflict_data : public expression_data {
    struct pair_data {
      bool _explained;
      bool _conflicting;
      bool _subsumed;
      pair_data()
	:_explained(false),
	 _conflicting(false),
	 _subsumed(false)
      {}
    };

    pair_data * _pair_data;
    bool _positive;
    
    conflict_data()
      :_pair_data(new pair_data()),
       _positive(true)
    {}

    conflict_data(conflict_data * data)
      :_pair_data(data->_pair_data),
       _positive(false)
    {}

    ~conflict_data()
    {
      if(_positive)
	delete _pair_data;
    }
  };


  solver & _solver;
  data_extractor<conflict_data> _data;
  expression_vector _conflicting;
  expression_vector _explained;
  unsigned _next_to_explain;
  unsigned _backjump_level;
  unsigned _last_level;
  expression _uip_literal;
  bool _is_conflict;

public:
  conflict_set(solver & sl);

  void add_literals(const expression & l_pos, const expression & l_neg)
  {
    conflict_data * l_data = new conflict_data();
    _data.set_data(l_pos, l_data);
    _data.set_data(l_neg, new conflict_data(l_data));
  }

  void set_conflict()
  {
    _is_conflict = true;
  }

  void reset_conflict();

  bool is_conflict() const
  {
    return _is_conflict;
  }

  void add_new_literals(const explanation & new_lits);

  expression next_to_explain()
  {
    return _explained[_next_to_explain++];
  }

  const expression_vector & get_conflicting() const
  {
    return _conflicting;
  }

  bool check_subsumed(const expression & l, const explanation & expl);

  void subsume(const expression & l)
  {
    _data.get_data(l)->_pair_data->_subsumed = true;
  }

  clause * get_clause() const;

  bool all_explained() const
  {
    return _next_to_explain == _explained.size();
  }

  const expression & uip_literal() const
  {
    return _uip_literal;
  }
  
  unsigned backjump_level() const
  {
    return _backjump_level;
  }

  unsigned last_level() const
  {
    return _last_level;
  }

  bool is_uip() const
  {
    return last_level() != 0 && all_explained();
  }
  
  void calculate_last_level(const explanation & conflicting);
};

#endif // _CONFLICT_SET_H
