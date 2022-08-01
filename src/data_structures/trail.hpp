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

#ifndef _TRAIL_H
#define _TRAIL_H

#include "expression_library.hpp"
#include "backjump_stack.hpp"
#include "argosmt_expression_data.hpp"

enum extended_boolean {
  EB_FALSE = -1,
  EB_UNDEFINED = 0,
  EB_TRUE = 1
};

inline
extended_boolean opposite_value(extended_boolean eb)
{
  return (extended_boolean)(-eb);
}

#define UNDEFINED_TRAIL_LEVEL ((unsigned)(-1))
#define UNDEFINED_TRAIL_INDEX ((unsigned)(-1))

class theory_solver;
class solver;

class trail {
private:
  
  struct on_push {
    trail & _tr;
    theory_solver * _source_theory_solver;

    on_push(trail & tr, theory_solver * st)
      :_tr(tr),
       _source_theory_solver(st)
    {}
   
    void operator () (const expression & l) const
    {
      assert(_tr.is_undefined(l));
      trail_data * l_data = _tr._data.get_data(l);  
      l_data->_pair_data->_value = l_data->_positive ? EB_TRUE : EB_FALSE;
      l_data->_pair_data->_source_theory_solver = _source_theory_solver;
      l_data->_pair_data->_trail_index = _tr.size() - 1;
      l_data->_pair_data->_trail_level = _tr.current_level();
    }
  };

  struct on_pop {
    trail & _tr;    
    on_pop(trail & tr)
      :_tr(tr)
    {}

    void operator () (const expression & l) const
    {
      trail_data * l_data = _tr._data.get_data(l);
      l_data->_pair_data->_value = EB_UNDEFINED;
      l_data->_pair_data->_source_theory_solver = 0;
      l_data->_pair_data->_trail_level = UNDEFINED_TRAIL_LEVEL;
      l_data->_pair_data->_trail_index = UNDEFINED_TRAIL_INDEX;
    }
  };
  
  struct trail_data : public expression_data {
    struct pair_data {
      extended_boolean _value;
      theory_solver * _source_theory_solver;
      unsigned _trail_level;
      unsigned _trail_index;       
      pair_data()
	:_value(EB_UNDEFINED), 
	 _source_theory_solver(0),
	 _trail_level(UNDEFINED_TRAIL_LEVEL), 
	 _trail_index(UNDEFINED_TRAIL_INDEX)
      {}
    };
    
    pair_data * _pair_data;
    bool _positive;
    
    trail_data()
      :_pair_data(new pair_data()),
       _positive(true)
    {}
    trail_data(trail_data * data)
      :_pair_data(data->_pair_data),
       _positive(false)
    {}
    
    ~trail_data()
    {
      if(_positive)
	delete _pair_data;
    }    
  };
  
  data_extractor<trail_data> _data;
  backjump_stack<expression, on_push, on_pop> _stack; 

  extended_boolean get_value(trail_data * l_data) const
  {
    return l_data->_positive ? l_data->_pair_data->_value : opposite_value(l_data->_pair_data->_value);
  }

public:
  
  trail(solver & sl, unsigned capacity = 1000);

  void add_literals(const expression & l_pos, const expression & l_neg)
  {
    trail_data * l_pos_data;
    _data.set_data(l_pos, l_pos_data = new trail_data()); 
    _data.set_data(l_neg, new trail_data(l_pos_data));
  }

  extended_boolean get_value(const expression & l) const
  {
    return get_value(_data.get_data(l));
  }
    
  bool is_undefined(const expression & l) const
  {
    return get_value(l) == EB_UNDEFINED;
  }
    
  bool is_true(const expression & l) const
  {
    return get_value(l) == EB_TRUE;
  }
  
  bool is_false(const expression & l) const
  {
    return get_value(l) == EB_FALSE;
  }
  
  theory_solver * get_source_theory_solver(const expression & l) const
  {
    trail_data * l_data = _data.get_data(l);
    return l_data->_pair_data->_source_theory_solver;
  }
  
  unsigned get_trail_level(const expression & l) const
  {
    trail_data * l_data = _data.get_data(l);
    return l_data->_pair_data->_trail_level;
  }
    
  unsigned get_trail_index(const expression & l) const
  {
    trail_data * l_data = _data.get_data(l);
    return l_data->_pair_data->_trail_index;
  }

  unsigned get_trail_index_if_falsified(const expression & l) const
  {
    trail_data * l_data = _data.get_data(l);
    return get_value(l_data) == EB_FALSE ? l_data->_pair_data->_trail_index : UNDEFINED_TRAIL_INDEX;
  }
  
  unsigned size() const
  {
    return _stack.size();
  }

  const expression & operator [] (unsigned i) const 
  {
    return _stack[i];
  }
  
  void push(const expression & l, theory_solver * source_theory_solver)
  {
    _stack.push(l, on_push(*this, source_theory_solver));
  }

  void push(expression && l, theory_solver * source_theory_solver)
  {
    _stack.push(std::move(l), on_push(*this, source_theory_solver));
  }

  
  void pop()
  {
    _stack.pop(on_pop(*this));
  }

  const expression & top() const
  {
    return _stack.top();
  }
  
  void new_level() 
  {
    _stack.new_level();
  }
  
  unsigned current_level() const
  {
    return _stack.current_level();
  }

  unsigned level_start(unsigned i) const
  {
    return _stack.level_start(i);
  }

  bool is_level_empty(unsigned i) const
  {
    return _stack.is_level_empty(i);
  }

  unsigned last_level_start() const
  {
    return _stack.last_level_start();
  }
  
  bool is_last_level_empty() const 
  {
    return _stack.is_last_level_empty();
  }
  
  void backjump(unsigned level)
  {
    _stack.backjump(level, on_pop(*this));
  }
 
  void print_trail(std::ostream & ostr) const
  {
    _stack.out(ostr);
  }
};

inline
std::ostream & operator << (std::ostream & ostr, const trail & tr)
{
  tr.print_trail(ostr);
  return ostr;
}

#endif // _TRAIL_H
