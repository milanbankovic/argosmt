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

#ifndef _LINEAR_POLYNOMIAL_H
#define _LINEAR_POLYNOMIAL_H

#include <iostream>

template <typename T>
class linear_polynomial_template {
public:
  
  typedef T map_type;  
  typedef typename map_type::key_type unknown_type;
  typedef typename map_type::mapped_type coeff_type;

private:
  map_type _map;

public:

  linear_polynomial_template()
  {}

  const coeff_type & get_coeff(const unknown_type & e) const
  {
    static const coeff_type zero = 0;
    typename map_type::const_iterator it = _map.find(e);
    if(it != _map.end())
      return it->second;
    else
      return zero;
  }

  void set_coeff(const unknown_type & e, const coeff_type & c)
  {
    set_coeff(e, coeff_type(c));
  }

  void set_coeff(const unknown_type & e, coeff_type && c)
  {
    if(c != 0)
      _map[e] = c;
    else
      _map.erase(e);
  }
  
  void reset_coeff(const unknown_type & e)
  {
    _map.erase(e);
  }

  void add_to_coeff(const unknown_type & e, const coeff_type & k)
  {    
    coeff_type & c = _map[e];
    
    c += k;

    if(c == 0)
      _map.erase(e);
  }

  void add_product_to_coeff(const unknown_type & e, const coeff_type & k1, const coeff_type & k2)
  {
    coeff_type & c = _map[e];
    static coeff_type temp;
    temp = k1;
    temp *= k2;
    c += temp;

    if(c == 0)
      _map.erase(e);
  }


  void scale_coeff(const unknown_type & e, const coeff_type & k)
  {
    coeff_type & c = _map[e];

    c *= k;
    
    if(c == 0)
      _map.erase(e);
  }

  typedef typename map_type::iterator iterator;
  typedef typename map_type::const_iterator const_iterator;
  
  iterator begin()
  {
    return _map.begin();
  }

  iterator end()
  {
    return _map.end();
  }


  const_iterator begin() const
  {
    return _map.begin();
  }

  const_iterator end() const
  {
    return _map.end();
  }
   
  bool operator == (const linear_polynomial_template & lp) const
  {
    for(const_iterator it = begin(), it_end = end(); it != it_end; ++it)
      if(lp.get_coeff(it->first) != it->second)
	return false;
    
    for(const_iterator it = lp.begin(), it_end = lp.end(); it != it_end; ++it)
      if(get_coeff(it->first) != it->second)
	return false;

    return true;
  }

  bool operator != (const linear_polynomial_template & lp) const
  {
    return !(*this == lp);
  }

  unsigned size() const
  {
    return _map.size();
  }

  bool empty() const
  {
    return _map.empty();
  }

  void clear()
  {
    _map.clear();
  }

  void swap(linear_polynomial_template & lp)
  {
    _map.swap(lp._map);
  }

  template <typename U>
  void add_polynomial(const linear_polynomial_template<U> & lp, const coeff_type & k = 1)
  {
    for(typename linear_polynomial_template<U>::const_iterator it = lp.begin(), it_end = lp.end(); it != it_end; ++it)
      add_product_to_coeff(it->first, it->second, k);
  }
  
  void scale_polynomial(const coeff_type & k)
  {
    if(k == 0)
      {
	clear();
	return;
      }
    
    for(typename linear_polynomial_template<T>::iterator it = begin(), it_end = end(); it != it_end; ++it)
      it->second *= k;
  }
};


template <typename T>
inline 
std::ostream & operator << (std::ostream & ostr, const linear_polynomial_template<T> & lp)
{
  ostr << "[ ";
  for(typename linear_polynomial_template<T>::const_iterator it = lp.begin(), it_end = lp.end(); it != it_end; ++it)
    {
      ostr << "[" << it->second << " * " << it->first << "] ";
    }
  ostr << " ]";
  return ostr;
}

#endif // _LINEAR_POLYNOMIAL_H
