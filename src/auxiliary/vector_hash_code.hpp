/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2017 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _VECTOR_HASH_CODE
#define _VECTOR_HASH_CODE

class prime_hash_combiner {
public:
  std::size_t operator() (std::size_t h1, std::size_t h2) const 
  {
    return h1*PRIME_HASH + h2;
  }
};

class boost_hash_combiner {
public:
  std::size_t operator() (std::size_t h1, std::size_t h2) const 
  {
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
  }
};

using default_hash_combiner = prime_hash_combiner;
//using default_hash_combiner = boost_hash_combiner;

template <class T, class HC = default_hash_combiner>
class vector_hash_code {
private:
  std::size_t _initial_value;

  template <class U>
  std::size_t get_hash_code(const U & value) const
  {
    return value.hash_code();
  }

  template <class U>
  std::size_t get_hash_code(const U * value) const
  {
    return value->hash_code();
  }

  template <class U>
  std::size_t get_hash_code(const std::shared_ptr<U> & value) const
  {
    return value->hash_code();
  }

public:

  using value_type = T;
  using hash_combiner = HC;
  
  vector_hash_code(std::size_t init = 0)
    :_initial_value(init)
  {}
  
  std::size_t operator () (const std::vector<T> & vec) const
  {
    std::size_t result = _initial_value;
    
    for(typename std::vector<T>::const_iterator it = vec.begin(),
	  it_end = vec.end(); 
	it != it_end; ++it)
      {
	result = hash_combiner()(result, get_hash_code(*it));
      }    
    return result;
  }
};

#endif // _VECTOR_HASH_CODE
