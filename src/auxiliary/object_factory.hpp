/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2014 Milan Bankovic (milan@matf.bg.ac.rs)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
***************************************************************************/

#ifndef _OBJECT_FACTORY_H
#define _OBJECT_FACTORY_H

#include <unordered_set>
#include <memory>

#include "common.hpp"

template <typename T>
class object_factory {
private:
  struct object_hash {
    std::size_t operator () (const T & p) const
    {
      return p._hash_code;
    }
  };

  struct object_deleter {
    object_factory<T> * _factory;
    object_deleter(object_factory<T> * factory)
      :_factory(factory)
    {}
    
    void operator () (const T * obj) const
    {
     _factory->_objects.erase(*obj);
    }
  };

  std::unordered_set<T, object_hash> _objects;
public:
  
  object_factory(std::size_t size = HASH_TABLE_SIZE)
    :_objects(size)
  {}

  std::shared_ptr<const T> add_object(const T & obj)
  {
    return add_object(T(obj));
  }

  std::shared_ptr<const T> add_object(T && obj)
  {
    typename std::unordered_set<T, object_hash>::const_iterator it = _objects.find(obj);
    if(it != _objects.end())
      return it->shared_from_this();				     
    else
      {
	it = _objects.insert(std::move(obj)).first;
	return std::shared_ptr<const T>(&*it, object_deleter(this));
      }
  }

};


#endif // _OBJECT_FACTORY_H
