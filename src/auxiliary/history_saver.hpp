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

#ifndef _HISTORY_SAVER_H
#define _HISTORY_SAVER_H

#include <vector>
#include <type_traits>

template <typename T>
class history_saver {
public:
  typedef T saved_data_type;
private:
  struct data_level {
    T _data;
    unsigned _level;
    data_level(const T & t, unsigned l)
      :_data(t),
       _level(l)
    {}
    data_level(T && t, unsigned l)
      :_data(std::move(t)),
       _level(l)
    {}

  };
  std::vector<data_level> _history;
  const volatile unsigned & _current_level;
public:
  template <typename U>
  history_saver(const unsigned & level, U && u)
    :_current_level(level)
  {
    _history.emplace_back(std::forward<U>(u), 0);
  }
  
  T & ref()
  {
    const data_level & top = _history.back();
    if(top._level < _current_level)
      {
	_history.emplace_back(top._data, _current_level);
      }
    return _history.back()._data;
  }

  const T & ref() const
  {
    return _history.back()._data;
  }

  const T & const_ref() const
  {
    return _history.back()._data;
  }

  void restore_state()
  {
    while(_history.back()._level > _current_level)
      _history.pop_back();
  }
  
};

#endif // _HISTORY_SAVER_H
