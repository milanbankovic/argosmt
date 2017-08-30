/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2013 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _COW_WRAPPER_H
#define _COW_WRAPPER_H

#include <memory>

template <typename T>
class cow_wrapper {
private:
  std::shared_ptr<T> _ptr;
public:
  cow_wrapper()
    :_ptr((T*)0)
  {}

  cow_wrapper(T * t)
    :_ptr(t)
  {}

  cow_wrapper(const T & t)
    :_ptr(new T(t))
  {}

  cow_wrapper(T && t)
    :_ptr(new T(std::move(t)))
  {}
  
  bool is_valid() const
  {
    return _ptr.get() != 0;
  }

  const T & const_ref() const
  {
    return *_ptr;
  }

  const T & ref() const
  {
    return *_ptr;
  }

  T & ref() 
  {
    if(!_ptr.unique())
      {
	_ptr.reset(new T(*_ptr));
      }
    return *_ptr;
  }

  bool operator == (const cow_wrapper<T> & cow) const
  {
    return *_ptr == *cow._ptr;
  }

  bool operator != (const cow_wrapper<T> & cow) const
  {
    return *_ptr != *cow._ptr;
  }

};

#endif // _COW_WRAPPER_H
