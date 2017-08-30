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

#ifndef _HEAP_H
#define _HEAP_H

#include "argosmt_expression_data.hpp"

template <typename comparator>
class base_heap {
protected:
  class heap_data : public expression_data {
  private:
    unsigned _heap_index;
  public:
    heap_data()
      :_heap_index((unsigned)(-1))
    {}
    
    unsigned get_heap_index() const
    {
      return _heap_index;
    }
    
    void set_heap_index(unsigned heap_index)
    {
      _heap_index = heap_index;
    }
  };

  comparator _comp;
  expression_vector _elements;
  
  static unsigned parent(unsigned index)
  {
    return (index - 1) >> 1;
  }
    
  static unsigned left_child(unsigned index)
  {
    return (index << 1) + 1;
  }
  
  static unsigned right_child(unsigned index)
  {
    return (index << 1) + 2;
  }
  
  bool is_greater(unsigned i1, unsigned i2)
  {
    return _comp(_elements[i1], _elements[i2]);
  }
    
  unsigned greater_child(unsigned index)
  {
    unsigned left = left_child(index);

    if(left >= _elements.size())
      return (unsigned)(-1);

    unsigned right = right_child(index);
    
    if(right >= _elements.size())
      return left;
    
    return is_greater(left, right) ? left : right;
  }
  
  bool should_push_up(unsigned index)
  {
    return index != 0 && is_greater(index, parent(index));    
  }

  bool should_push_down(unsigned index, unsigned & gc)
  {
    return (gc = greater_child(index)) != (unsigned)(-1) && is_greater(gc, index); 
  }
  
public:
  base_heap(const comparator & comp = comparator())
    :_comp(comp)
  {}

  void set_comparator(const comparator & comp)
  {
    _comp = comp;
  }
  
  virtual heap_data * get_heap_data(const expression & e) = 0;
  
  virtual void set_heap_data(const expression & e, heap_data * data) = 0;

  virtual bool has_heap_data(const expression & e) const = 0;

  void introduce_element(const expression & l)
  {
    if(!has_heap_data(l))
      set_heap_data(l, new heap_data());
  }
  
  const expression_vector & elements() const
  {
    return _elements;
  }

  bool empty() const
  {
    return _elements.empty();
  }

  const expression & top_element() const
  {
    return _elements[0];
  }
  
  void push_up(unsigned index)
  {
    heap_data * ind_data = get_heap_data(_elements[index]);

    while(should_push_up(index))
      {
	unsigned p = parent(index);
	expression & elem = _elements[index];
	ind_data->set_heap_index(p);
	expression & par = _elements[p];
	get_heap_data(par)->set_heap_index(index);
	elem.swap(par);
	index = p;
      }
  }

  void push_down(unsigned index)
  {
    unsigned gc;
    heap_data * ind_data = get_heap_data(_elements[index]);
    
    while(should_push_down(index, gc))
      {
	expression & elem = _elements[index];
	ind_data->set_heap_index(gc);
	expression & chld = _elements[gc];
	get_heap_data(chld)->set_heap_index(index);
	elem.swap(chld);
	index = gc;
      }
  }

  void add_to_heap(const expression & elem)
  {
    _elements.push_back(elem);
    get_heap_data(elem)->set_heap_index(_elements.size() - 1);
    push_up(_elements.size() - 1);  
  }

  void add_to_heap(expression && elem)
  {
    _elements.push_back(std::move(elem));    
    get_heap_data(_elements.back())->set_heap_index(_elements.size() - 1);
    push_up(_elements.size() - 1);  
  }

  bool get_top_element(expression & top_element)
  {
    if(_elements.empty())
      return false;
  
    top_element = std::move(_elements[0]);
    get_heap_data(top_element)->set_heap_index((unsigned)(-1));


    _elements[0].swap(_elements.back());
    _elements.pop_back();

    if(!_elements.empty())
      {
	get_heap_data(_elements[0])->set_heap_index(0);
	push_down(0);
      }
  
    return true;
  }

  void clear()
  {
    for(auto & l : _elements)
      get_heap_data(l)->set_heap_index((unsigned)(-1));
    _elements.clear();
  }
  
};

template <typename comparator>
class heap : public base_heap<comparator> {
private:
  using typename base_heap<comparator>::heap_data;
  data_extractor<heap_data> _heap_data;
public:
  heap(data_extractor_counter & counter, const comparator & comp = comparator())
    :base_heap<comparator>(comp),
     _heap_data(counter)     
  {}

  virtual heap_data * get_heap_data(const expression & e)
  {
    return _heap_data.get_data(e);
  }
  
  virtual void set_heap_data(const expression & e, heap_data * data)
  {
    _heap_data.set_data(e, data);
  }

  virtual bool has_heap_data(const expression & e) const
  {
    return _heap_data.has_data(e);
  }

  
};

template <typename comparator>
class hash_heap : public base_heap<comparator> {
private:
  using typename base_heap<comparator>::heap_data;
  std::unordered_map<expression, heap_data *> _heap_data;
public:

  hash_heap(const comparator & comp = comparator())
    :base_heap<comparator>(comp),
     _heap_data(HASH_TABLE_SIZE)     
  {}

  virtual heap_data * get_heap_data(const expression & e)
  {
    return _heap_data.find(e)->second;
  }
  
  virtual void set_heap_data(const expression & e, heap_data * data)
  {
    _heap_data[e] = data;
  }

  virtual bool has_heap_data(const expression & e) const
  {
    return _heap_data.find(e) != _heap_data.end();
  }
  
  virtual ~hash_heap()
  {
    for(auto & p : _heap_data)
      delete p.second;
  }
};

template <typename comparator>
class simple_heap : public base_heap<comparator> {
private:
  using typename base_heap<comparator>::heap_data;
  heap_data * _dummy;
public:

  simple_heap(const comparator & comp = comparator())
    :base_heap<comparator>(comp),
    _dummy(new heap_data())
  {}

  virtual heap_data * get_heap_data(const expression & e)
  {
    return _dummy;
  }
  
  virtual void set_heap_data(const expression & e, heap_data * data)
  {
    throw "ERROR";
  }

  virtual bool has_heap_data(const expression & e) const
  {
    return true;
  }
  
  virtual ~simple_heap()
  {
    delete _dummy;
  }
};


#endif // _HEAP_H
