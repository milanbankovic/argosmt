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

#ifndef _BACKJUMP_STACK_H
#define _BACKJUMP_STACK_H

#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include "history_saver.hpp"

template <typename element_type>
struct no_op {
  void operator () (const element_type & elem) const
  {}
};

template < typename element_type, typename on_push_type = no_op<element_type>, typename on_pop_type = no_op<element_type> >
class backjump_stack {
public:
  std::vector<element_type> _elements;
  std::vector<unsigned> _levels;

 public:
  
  backjump_stack(unsigned start_level = 0)
  {
    start_level++;
    while(start_level-- > 0)
      _levels.push_back(0);
  }
    
  void push(const element_type & item, const on_push_type & on_push = on_push_type()) 
  {
    _elements.push_back(item);
    on_push(_elements.back());
  }

  void push(element_type && item, const on_push_type & on_push = on_push_type()) 
  {
    _elements.push_back(std::move(item));
    on_push(_elements.back());
  }

  void pop(const on_pop_type & on_pop = on_pop_type()) 
  {
    assert(!empty());
    on_pop(_elements.back());
    _elements.pop_back();
  }

  unsigned level_start(unsigned i) const
  {
    return _levels[i];
  }

  bool is_level_empty(unsigned i) const
  {
    return i != current_level() ?
      _levels[i] == _levels[i + 1] : _levels.back() == _elements.size();
  }

  unsigned last_level_start() const
  {
    return _levels.back();
  }
  
  bool is_last_level_empty() const 
  {
    return _levels.back() == _elements.size();
  }
  
  const element_type & top() const 
  {
    assert(!empty());
    return _elements.back();
  }

  element_type & top()
  {
    assert(!empty());
    return _elements.back();
  }
  
  void new_level() 
  {
    _levels.push_back(_elements.size());
  }
  
  unsigned current_level() const
  {
    return _levels.size() - 1;
  }
  
  void backjump(unsigned level, const on_pop_type & on_pop = on_pop_type()) 
  { 
    assert(level < current_level());
    
    while(size() > _levels[level + 1])
      {
	pop(on_pop);
      }
    _levels.resize(level + 1);
  }
  
  void clear() 
  {
    _elements.clear();
    _levels.clear();
    _levels.push_back(0);
  }
  
  bool empty() const 
  {
    return _elements.empty();
  }
    
  size_t size() const 
  {
    return _elements.size();
  }

  void reserve(unsigned n)
  {
    _elements.reserve(n);
  }
  
  const element_type & operator [] (unsigned i) const 
  {
    return _elements[i];
  }

  element_type & operator [] (unsigned i) 
  {
    return _elements[i];
  }

  const std::vector<element_type> & elements() const
  {
    return _elements;
  }

  void out(std::ostream & ostr) const
  {
    for(unsigned i = 0, j = 1; i < _elements.size(); i++)
      {
	if(j < _levels.size() && i == _levels[j])
	  {
	  ostr << " |";
	  j++;
	  i--;
	}
	else
	  ostr << " " << _elements[i];
      }
  }
 
};


template <typename element_type>
inline std::ostream & operator << (std::ostream & ostr, 
				   const backjump_stack<element_type> & s)
{
  s.out(ostr);
  return ostr;
}

template < typename element_type, typename on_push_type = no_op<element_type>, typename on_pop_type = no_op<element_type> >
class sparse_backjump_stack {
private:

  struct stack_data {
    element_type _element;
    unsigned _level;
    stack_data(const element_type & element, unsigned level)
      :_element(element),
       _level(level)
    {}
    stack_data(element_type && element, unsigned level)
      :_element(std::move(element)),
       _level(level)
    {}    
  };
  std::vector<stack_data> _elements;
  unsigned _current_level;

public:
  
  sparse_backjump_stack(unsigned start_level = 0)
    :_current_level(start_level)
  {}
    
  void push(const element_type & element, const on_push_type & on_push = on_push_type()) 
  {
    _elements.push_back(stack_data(element, _current_level));
    on_push(_elements.back()._element);   
  }

  void push(element_type && element, const on_push_type & on_push = on_push_type()) 
  {
    _elements.push_back(stack_data(std::move(element), _current_level));
    on_push(_elements.back()._element);   
  }

  void pop(const on_pop_type & on_pop = on_pop_type()) 
  {
    assert(!empty());
    on_pop(_elements.back()._element);
    _elements.pop_back();
  }
    
  const element_type & top() const 
  {
    assert(!empty());
    return _elements.back()._element;
  }

  element_type & top()
  {
    assert(!empty());
    return _elements.back()._element;
  }
  
  void new_level() 
  {
    _current_level++;
  }
  
  unsigned current_level() const
  {
    return _current_level;
  }
  
  void backjump(unsigned level, const on_pop_type & on_pop = on_pop_type()) 
  { 
    assert(level < current_level());
    _current_level = level;    
    while(!_elements.empty() && _elements.back()._level > _current_level)
      pop(on_pop);
  }
    
  bool empty() const 
  {
    return _elements.empty();
  }
    
  size_t size() const 
  {
    return _elements.size();
  }

  void reserve(unsigned n)
  {
    _elements.reserve(n);
  }
  
  const element_type & operator [] (unsigned i) const 
  {
    return _elements[i]._element;
  }

  element_type & operator [] (unsigned i) 
  {
    return _elements[i]._element;
  }

  void out(std::ostream & ostr) const
  {
    unsigned level = 0;
    for(unsigned i = 0; i < _elements.size(); i++)
      {
	while(_elements[i]._level > level)
	  {
	    ostr << " |";
	    level++;
	  }
	ostr << " " << _elements[i]._element;
      }
  }
 
};


template <typename element_type>
inline std::ostream & operator << (std::ostream & ostr, 
				   const sparse_backjump_stack<element_type> & s)
{
  s.out(ostr);
  return ostr;
}

#endif // _BACKJUMP_STACK_H
