/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2016 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _DOMAIN_HANDLER_H
#define _DOMAIN_HANDLER_H

#include "propagation_handler.hpp"

class domain_handler : public propagation_handler {
private:

  struct value_info {
    expression _eq;     // x = v
    expression _diseq;  // x != v
    expression _le;     // x <= v    
    expression _lt;     // x < v
    expression _ge;     // x >= v
    expression _gt;     // x > v
    expression _expl; 
    unsigned _index;
    value_info(const expression & eq = expression(), 
	       const expression & diseq = expression())
      :_eq(eq),
       _diseq(diseq),
       _le(),
       _lt(),
       _ge(),
       _gt(),
       _expl(),
       _index((unsigned)(-1))
    {}
  };

  expression _var;
  std::unordered_map<int, value_info> _all_values;
  std::unordered_set<int> _domain_values;  
  std::vector<int> _sorted_values;
  int _min_index;
  int _max_index;
  int _zero_min_index;
  int _zero_max_index;
  
  struct on_pop {
    domain_handler * _dh;
    on_pop(domain_handler * dh)
      :_dh(dh) {}
    
    void operator () (int value) const
    {
      if(_dh->_domain_values.empty() || value > _dh->_sorted_values[_dh->_max_index])
	_dh->_max_index = _dh->_all_values[value]._index;
      if(_dh->_domain_values.empty() || value < _dh->_sorted_values[_dh->_min_index])
	_dh->_min_index = _dh->_all_values[value]._index;
      _dh->_domain_values.insert(value);
    }
  };

  sparse_backjump_stack<int, no_op<int>, on_pop> _prunings;
  unsigned _finite_level;
  sparse_backjump_stack<expression> _trail;
  unsigned _next_to_assert;
  expression_vector _finite_domain_cause;
  bool _is_in_queue;
  domain_common_data * _common_data;


  void set_finite()
  {
    if(_finite_level == (unsigned)(-1))
      _finite_level = _trail.current_level();
  }

  void check_not_finite(const expression & l);
  void check_finite(const expression & l);
  void check_previous_trail_literals();
  void check_all_values();
  void introduce_literals();
  void init_min_max();
  void update_min_max();
  void prunings_explanations_singleton(explanation & expl, int val);
  void prunings_explanations_lower_vals(explanation & expl, int val);
  void prunings_explanations_greater_vals(explanation & expl, int val);
  void generate_explanation(const expression & l, explanation & expl);
  void generate_conflict_explanation(explanation & expl);
  void propagate_literal(const expression & l);
  void check_singleton_or_empty();
  void prune_value(int value, const expression & l);
  template <typename F>
  void prune_values_except(const F & f, const expression & l);
  void prune_all_values_except(int value, const expression & l);
  void prune_greater_values(int value, const expression & l);
  void prune_lower_values(int value, const expression & l);
  bool check_bounds(expression & l_cause, expression & u_cause, int & lower, int & upper);
  int find_smallest_greater_than(int value) const;
  int find_greatest_lower_than(int value) const;
  int find_smallest_greater_or_equal(int value) const;
  int find_greatest_lower_or_equal(int value) const;
    
  
public:
  domain_handler(csp_theory_solver * th, const expression & var)
    :propagation_handler(th),
     _var(var),
     _all_values(HASH_TABLE_SIZE),
     _domain_values(HASH_TABLE_SIZE),
     _min_index(-1),
     _max_index(-1),
     _zero_min_index(-1),
     _zero_max_index(-1),
     _finite_level(0xffffffff),
     _next_to_assert(0),
     _is_in_queue(false),
     _common_data(&th->_domain_common_data)
  {
    for(unsigned i = 0; i < th->_current_level; i++)
      new_level();
  }
  
  bool is_in_queue() const
  {
    return _is_in_queue;
  }
  
  void set_in_queue(bool value)
  {
    _is_in_queue = value;
  }

  const expression & get_variable() const
  {
    return _var;
  }

  void add_value(int val)
  {
    if(_all_values.find(val) == _all_values.end())
      {
	init_literal_pair(val, false);
	init_inequalities(val);
      }
  }

  void init_literal_pair(int v, bool introduce);  
  void init_upper_bound(int v);
  void init_lower_bound(int v);
  void init_inequalities(int v);
  void set_initial_bounds(int lower, int upper);
  
  int get_min() const
  {
    return _sorted_values[_min_index];
  }
  
  int get_max() const
  {
    return _sorted_values[_max_index];
  }

  int get_zero_min_index() const
  {
    return _zero_min_index != -1 ? _zero_min_index : 0;
  }
  int get_zero_max_index() const
  {
    return _zero_max_index != -1 ? _zero_max_index : (int)_sorted_values.size() - 1;
  }
  
  int get_median(bool lower = false) const;

  const std::unordered_set<int> & current_domain() const
  {
    return _domain_values;
  }

  const std::vector<int> & initial_finite_domain() const
  {
    return _sorted_values;
  }

  const expression_vector & explain_finite_domain() const
  {
    return _finite_domain_cause;
  }

  bool in_domain(int value) const
  {
    return _domain_values.find(value) != _domain_values.end();
  }
  
  bool is_finite() const
  {
    return _finite_level <= _trail.current_level();
  }

  unsigned get_finite_level() const
  {
    return _finite_level;
  }

  const expression & get_equality(int value)
  {
    assert(_all_values.find(value) != _all_values.end());
    return _all_values[value]._eq;
  }

  const expression & get_disequality(int value)
  {
    assert(_all_values.find(value) != _all_values.end());
    return _all_values[value]._diseq;
  }
  
  const expression & get_lower_or_equal(int value)
  {
    assert(_all_values.find(value) != _all_values.end());
    return _all_values[value]._le;
  }

  const expression & get_lower_than(int value)
  {
    assert(_all_values.find(value) != _all_values.end());
    return _all_values[value]._lt;
  }

  const expression & get_greater_or_equal(int value)
  {
    assert(_all_values.find(value) != _all_values.end());
    return _all_values[value]._ge;
  }

  const expression & get_greater_than(int value)
  {
    assert(_all_values.find(value) != _all_values.end());
    return _all_values[value]._gt;
  }

  virtual void new_level()
  {  
    _trail.new_level();
    _prunings.new_level();
  }

  virtual void backjump(unsigned level);
  const expression & get_assigned_value() const;
  virtual void assert_literal(const expression & l);
  virtual void check_and_propagate(unsigned layer);
  virtual void explain_literal(const expression & l);

  void explain_minimum_at_trail_size(unsigned size, explanation & expl);
  void explain_maximum_at_trail_size(unsigned size, explanation & expl);
  void explain_minmax_at_trail_size(unsigned size, explanation & expl);
  void explain_domain_at_trail_size(unsigned size, explanation & expl);


  class const_iterator {
  private:
    std::vector<int>::const_iterator _it;
    const domain_handler * _dh;
  public:
    const_iterator():_it(), _dh(0) {}
    const_iterator(const std::vector<int>::const_iterator it, const domain_handler * dh)
      :_it(it),
       _dh(dh)
    {}

    const const_iterator & operator ++ ()
    {
      while(++_it != _dh->_sorted_values.end() && _dh->_domain_values.find(*_it) == _dh->_domain_values.end());
      return *this;
    }

    const_iterator operator ++ (int)
    {
      const_iterator temp(*this);
      operator++();
      return temp;
    }

    const const_iterator & operator -- ()
    {
      while(--_it >= _dh->_sorted_values.begin() && _dh->_domain_values.find(*_it) == _dh->_domain_values.end());
      return *this;
    }

    const_iterator operator -- (int)
    {
      const_iterator temp(*this);
      operator--();
      return temp;
    }

    int operator * ()
    {
      return *_it;
    }

    bool operator == (const const_iterator & it) const
    {
      return _it == it._it;
    }

    bool operator != (const const_iterator & it) const
    {
      return _it != it._it;
    }
  };

  typedef const_iterator iterator;


  class const_reverse_iterator {
  private:
    std::vector<int>::const_reverse_iterator _it;
    const domain_handler * _dh;
  public:
    const_reverse_iterator():_it(), _dh(0) {}
    const_reverse_iterator(const std::vector<int>::const_reverse_iterator it, const domain_handler * dh)
      :_it(it),
       _dh(dh)
    {}

    const const_reverse_iterator & operator ++ ()
    {
      while(++_it != _dh->_sorted_values.rend() && _dh->_domain_values.find(*_it) == _dh->_domain_values.end());
      return *this;
    }

    const_reverse_iterator operator ++ (int)
    {
      const_reverse_iterator temp(*this);
      operator++();
      return temp;
    }

    const const_reverse_iterator & operator -- ()
    {
      while(--_it >= _dh->_sorted_values.rbegin() && _dh->_domain_values.find(*_it) == _dh->_domain_values.end());
      return *this;
    }

    const_reverse_iterator operator -- (int)
    {
      const_reverse_iterator temp(*this);
      operator--();
      return temp;
    }

    int operator * ()
    {
      return *_it;
    }

    bool operator == (const const_reverse_iterator & it) const
    {
      return _it == it._it;
    }

    bool operator != (const const_reverse_iterator & it) const
    {
      return _it != it._it;
    }
  };

  typedef const_reverse_iterator reverse_iterator;

  const_iterator begin() const
  {
    return const_iterator(_sorted_values.begin() + _min_index, this);
  }

  const_iterator end() const
  {
    return const_iterator(_sorted_values.end(), this);
  }

  const_iterator begin_greater_than(int value) const
  {
    int ind = find_smallest_greater_than(value);
    while(ind <= _max_index && _domain_values.find(_sorted_values[ind]) == _domain_values.end())
      ind++;
    if(ind > _max_index)
      ind = (int)_sorted_values.size();
    return const_iterator(_sorted_values.begin() + ind, this);
  }

  const_iterator begin_greater_or_equal(int value) const
  {
    int ind = find_smallest_greater_or_equal(value);
    while(ind <= _max_index && _domain_values.find(_sorted_values[ind]) == _domain_values.end())
      ind++;
    if(ind > _max_index)
      ind = (int)_sorted_values.size();
    return const_iterator(_sorted_values.begin() + ind, this);
  }


  const_reverse_iterator rbegin() const
  {
    return const_reverse_iterator(_sorted_values.rbegin() + (_sorted_values.size() - 1 - _max_index), this);
  }

  const_reverse_iterator rend() const
  {
    return const_reverse_iterator(_sorted_values.rend(), this);
  }

  const_reverse_iterator rbegin_lower_than(int value) const
  {
    int ind = find_greatest_lower_than(value);
    while(ind >= _min_index && _domain_values.find(_sorted_values[ind]) == _domain_values.end())
      ind--;
    if(ind < _min_index)
      ind = -1;
    return const_reverse_iterator(_sorted_values.rbegin() + (_sorted_values.size() - 1 - ind), this);
  }

  const_reverse_iterator rbegin_lower_or_equal(int value) const
  {
    int ind = find_greatest_lower_or_equal(value);
    while(ind >= _min_index && _domain_values.find(_sorted_values[ind]) == _domain_values.end())
      ind--;
    if(ind < _min_index)
      ind = -1;
    return const_reverse_iterator(_sorted_values.rbegin() + (_sorted_values.size() - 1 - ind), this);
  }
  
};

#endif // _DOMAIN_HANDLER_H
