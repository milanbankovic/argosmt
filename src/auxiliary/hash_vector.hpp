/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2008-2010, 2012-2014  Milan Bankovic (milan@matf.bg.ac.rs)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
***************************************************************************/

#ifndef _HASH_VECTOR_H
#define _HASH_VECTOR_H

#include <utility>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <functional>


template <typename T>
class hash_vector {
public:
  typedef std::unordered_map<T, unsigned> hash_map_type;
  typedef std::vector<T> vector_type;

  typedef typename hash_map_type::value_type hash_map_value_type;
  typedef typename vector_type::value_type vector_value_type;
  
private:
  hash_map_type _hash_map;
  vector_type _vec;
public:
  
  hash_vector()
    :_hash_map(HASH_TABLE_SIZE) 
  {}
  
  bool insert(const T & e)
  {
    return insert(T(e));
  }

  bool insert(T && e)
  {
    if(_hash_map.find(e) != _hash_map.end())
      return false;
    
    _vec.push_back(std::move(e));
    _hash_map.insert(hash_map_value_type(_vec.back(), 
					 _vec.size() - 1));
    
    assert(_vec.size() == _hash_map.size());

    return true;
  }

  
  bool push_back(const T & e)
  {
    return insert(T(e));
  }

  bool push_back(T && e)
  {
    return insert(std::move(e));
  }

  const T & back() const
  {
    return _vec.back();
  }
  
  void pop_back()
  {
    assert(!empty());
    
    _hash_map.erase(_vec.back());
    _vec.pop_back();
    
    assert(_vec.size() == _hash_map.size());
  }

  void erase(const T & e)
  {
    typename hash_map_type::const_iterator it = _hash_map.find(e);
    if(it == _hash_map.end())
      return;
    unsigned index = it->second;
    _hash_map.erase(it);
    _vec[index] = _vec.back();
    _vec.pop_back();
    if(index < _vec.size())
      _hash_map[_vec[index]] = index;
  }

  class const_vector_iterator {
    friend class hash_vector;
  private:
    typename vector_type::const_iterator _it;
    
    const_vector_iterator(const typename vector_type::const_iterator & it)
      :_it(it)
    {}
    
  public:
    const_vector_iterator() 
    {}
    
    const const_vector_iterator & operator ++ () 
    {
      _it++;
      return *this;
    }
    
    const_vector_iterator operator ++ (int)
    {
      const_vector_iterator old(*this);
      _it++;
      return old;
    }
    
    const const_vector_iterator & operator -- () 
    {
      _it--;
      return *this;
    }
    
    const_vector_iterator operator -- (int)
    {
      const_vector_iterator old(*this);
      _it--;
      return old;
    }
    
    bool operator != (const const_vector_iterator & i) const
    {
      return !(*this == i);
    }
    
    bool operator == (const const_vector_iterator & i) const 
    {
      return _it == i._it;
    }
    
    const T & operator * () const
    {
      return *_it;
    }
    
    const T * operator -> () const
    {
      return &(*_it);
    }   
    
  };
  
  typedef const_vector_iterator vector_iterator;
  
  
  class const_hash_map_iterator {
    friend class hash_vector;
  private:
    typename hash_map_type::const_iterator _it;

    const_hash_map_iterator(const typename hash_map_type::const_iterator & it)
      :_it(it)
    {}
    
  public:
    
    const_hash_map_iterator() 
    {}
    
    const const_hash_map_iterator & operator ++ () 
    {
      ++_it;
      return *this;
    }

    const_hash_map_iterator operator ++ (int) 
    {
      const_hash_map_iterator old(*this);
      _it++;
      return old;
    }
    
    bool operator != (const const_hash_map_iterator & i) const
    {
      return !(*this == i);
    }
    
    bool operator == (const const_hash_map_iterator & i) const 
    {
      return _it == i._it;
    }
    
    const hash_map_value_type & operator * () const
    {
      return *_it;
    }
    
    const hash_map_value_type * operator -> () const
    {
      return &(*_it);
    }   
    
  };

  typedef const_hash_map_iterator hash_map_iterator;
  
  
  const_vector_iterator vector_begin() const
  {
    return const_vector_iterator(_vec.begin());
  }
  
  const_vector_iterator vector_end() const
  {
    return const_vector_iterator(_vec.end());
  }
  
  const_hash_map_iterator hash_map_begin() const
  {
    return const_hash_map_iterator(_hash_map.begin());
  }
  
  const_hash_map_iterator hash_map_end() const
  {
    return const_hash_map_iterator(_hash_map.end());
  }
  
  int get_index(const T & e) const
  {
    typename hash_map_type::const_iterator it = _hash_map.find(e);
    assert(it != _hash_map.end());
    return it->second;
  }
  
  const T & operator [] (unsigned i) const
  {
    assert(i < _vec.size());
    return _vec[i];  
  }
  
  
  const_hash_map_iterator convert_to_hash_map_iterator(const const_vector_iterator & it) const
  {
    return const_hash_map_iterator(it != vector_end() ?  
				   _hash_map.find(*it) : 
				   _hash_map.end());
  }
  
  const_vector_iterator convert_to_vector_iterator(const const_hash_map_iterator & it) const
  {
    return const_vector_iterator(it != hash_map_end() ? 
				 _vec.begin() + it->second : 
				 _vec.end());
  }
  
  const_hash_map_iterator hash_map_find(const T & e) const
  {
    return const_hash_map_iterator(_hash_map.find(e)); 
  }
  
  const_vector_iterator vector_find(const T & e) const
  {
    return convert_to_vector_iterator(hash_map_find(e));
  } 
  
  std::size_t size () const
  {
    assert(_vec.size() == _hash_map.size());
    return _vec.size();
  }

  bool empty() const
  {
    assert(_vec.empty() == _hash_map.empty());
    return _vec.empty();
  }
  
  void clear()
  {
    _vec.clear();
    _hash_map.clear();
    assert(_vec.size() == _hash_map.size());
  }
  
};


#endif // _HASH_VECTOR_H
