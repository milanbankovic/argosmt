/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2014 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _ARGOSMT_EXPRESSION_DATA_H
#define _ARGOSMT_EXPRESSION_DATA_H

#include "expression_library.hpp"

class data_extractor_counter {
private:
  unsigned _count;
public:
  data_extractor_counter()
    :_count(0)
  {}

  unsigned get_count() const
  {
    return _count;
  }
  
  unsigned next_index() 
  {
    return _count++;
  }
};

class argosmt_expression_data : public expression_data {
private:
  std::vector<expression_data *> _data;
public:
  argosmt_expression_data(const data_extractor_counter & counter)
    :_data(counter.get_count(), 0)
  {}


  expression_data * get_data(unsigned i) const
  {
    return _data[i];
  }

  void set_data(unsigned i, expression_data * data)
  {
    _data[i] = data;
  }

  bool has_data(unsigned i) const
  {
    return _data[i] != 0;

  }
    
  virtual ~argosmt_expression_data()
  {
    for(unsigned i = 0; i < _data.size(); i++)
      delete _data[i]; 
  }
};

template <typename T>
class data_extractor {
private:
  unsigned _index;
public:
  data_extractor(data_extractor_counter & counter)
    :_index(counter.next_index())
  {}

  unsigned get_index() const
  {
    return _index;
  }

  T * get_data(const expression & e) const
  {
    return static_cast<T*>(static_cast<argosmt_expression_data *>(e->get_expression_data())->get_data(_index));
  }

  void set_data(const expression & e, T * data)
  {
    static_cast<argosmt_expression_data*>(e->get_expression_data())->set_data(_index, data);
  }

  bool has_data(const expression & e) const
  {
    return static_cast<argosmt_expression_data*>(e->get_expression_data())->has_data(_index);
  }

};

#endif // _ARGOSMT_EXPRESSION_DATA_H
