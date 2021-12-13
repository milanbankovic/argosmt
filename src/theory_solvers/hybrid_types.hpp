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

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
***************************************************************************/

#ifndef _HYBRID_TYPES_H
#define _HYBRID_TYPES_H

#include <utility>
#include <string>
#include <cctype>
#include <type_traits>
#include <cstddef>
#include <gmpxx.h>

template <typename IntType>
const mpz_class & MPZ(IntType x)
{
  static mpz_class mpz;
  mpz = x;
  return mpz;
}

template <typename IntType>
const mpq_class & MPQ(IntType x, IntType y)
{
  static mpq_class mpq;
  mpq.get_num() = x;
  mpq.get_den() = y;
  return mpq;
}

template <typename IntType>
const mpq_class & MPQ1(IntType x, IntType y)
{
  static mpq_class mpq;
  mpq.get_num() = x;
  mpq.get_den() = y;
  return mpq;
}

template <typename IntType>
const mpq_class & MPQ2(IntType x, IntType y)
{
  static mpq_class mpq;
  mpq.get_num() = x;
  mpq.get_den() = y;
  return mpq;
}

// Returns true iff the number is 01xxx..xxx or 10xxx..xxx
template <typename IntType>
inline
bool is_large_absolute(IntType x)
{
  return ((x ^ (x << 1)) & ((IntType)1 << ((sizeof(IntType) << 3) - 1))) != 0;
}

// Returns true iff x and y are both positive or both negative
template <typename IntType>
inline
bool have_same_sign(IntType x, IntType y)
{
  return ((x ^ y) & ((IntType)1 << ((sizeof(IntType) << 3) - 1))) == 0;
}

// Returns false iff x + y never overflows
template <typename IntType>
inline
bool is_add_overflow(IntType x, IntType y)
{
  static_assert(std::is_integral<IntType>::value &&
		std::is_signed<IntType>::value,
		"IntType must be signed integral type");
  return have_same_sign(x, y) &&
    (is_large_absolute(x) || is_large_absolute(y)); 
}

// Returns false iff x - y never overflows
template <typename IntType>
inline
bool is_sub_overflow(IntType x, IntType y)
{
  static_assert(std::is_integral<IntType>::value &&
		std::is_signed<IntType>::value,
		"IntType must be signed integral type");  
  return !have_same_sign(x, y) &&
    (is_large_absolute(x) || is_large_absolute(y)); 
}


// Returns false iff change-sign operation never overflows
template <typename IntType>
inline
bool is_chs_overflow(IntType x)
{
  static_assert(std::is_integral<IntType>::value &&
		std::is_signed<IntType>::value,
		"IntType must be signed integral type");
  return x == (IntType)((IntType)1 << ((sizeof(IntType) << 3) - 1));
}

// Returns the size of the number (i.e. the number of used bits)
/*template <typename IntType>
inline
unsigned number_size(IntType x)
{
  unsigned mask_length = sizeof(IntType) << 2;
  typename std::make_unsigned<IntType>::type mask =
    ~((typename std::make_unsigned<IntType>::type)0) << (sizeof(IntType) << 2);
  unsigned size = (sizeof(IntType) << 3);

  if(x == 0) return 0; // no '1' bits in x
  
  while(mask_length)
    {      
      if(x & mask) // upper half
	{
	  mask_length >>= 1;
	  mask &= (mask << mask_length);      
	}
      else  // lower half
	{
	  size -= mask_length;
	  mask >>= mask_length;
	  mask_length >>= 1;
	  mask &= (mask << mask_length);
	}
    }
  return size;
  }*/


template <typename IntType>
inline
typename std::enable_if<sizeof(IntType) == 4, unsigned>::type
number_size(IntType x)
{
  if((x & 0xffffff80) == 0)
    return 7;
  if((x & 0xffff8000) == 0)
    return 15;
  if((x & 0xff800000) == 0)
    return 23;
  return 31;
}

template<typename IntType>
inline
typename std::enable_if<sizeof(IntType) == 8, unsigned>::type
number_size(IntType x)
{
  if((x & 0xffffffffffff8000) == 0)
    return 15;
  if((x & 0xffffffff80000000) == 0)
    return 31;
  if((x & 0xffff800000000000) == 0)
    return 47;
  return 63;
}

// Returns the number of significant bits in 2's complement (counting the sign bith)
template <typename IntType>
inline
unsigned num_of_significant_bits(IntType x)
{
  static_assert(std::is_integral<IntType>::value &&
		std::is_signed<IntType>::value,
		"IntType must be signed integral type");
  return x >= 0 ? number_size(x) + 1 : number_size(~x) + 1;
}

// Returns false iff x * y never overflows
template <typename IntType>
inline
bool is_mul_overflow(IntType x, IntType y)
{
  static_assert(std::is_integral<IntType>::value &&
		std::is_signed<IntType>::value,
		"IntType must be signed integral type");
  return num_of_significant_bits(x) + num_of_significant_bits(y) > (sizeof(IntType) << 3) + (int)(x >= 0 || y >= 0);
}

// Returns false iff x / y never overflows
template <typename IntType>
inline
bool is_div_overflow(IntType x, IntType y)
{
  static_assert(std::is_integral<IntType>::value &&
		std::is_signed<IntType>::value,
		"IntType must be signed integral type");
  return is_chs_overflow(x) && y == -1;
}

// The class represents the hybrid implementation of integer numbers that
// can switch between the hardware and gmp types when needed.
template <typename IntType>
class hybrid_integer {
  static_assert(std::is_integral<IntType>::value &&
		std::is_signed<IntType>::value,
		"IntType must be signed integral type");
public:
  typedef IntType hardware_type;
private:
  hardware_type _hd_val = 0;
  mpz_class * _mpz_val = 0;
  
  void create_gmp()
  {
    _mpz_val = new mpz_class(_hd_val);
    _hd_val = 0;
  }
     
public:
  hybrid_integer(hardware_type n = 0)
    :_hd_val(n)
  {}
  hybrid_integer(const mpz_class & z)
    :_mpz_val(new mpz_class(z))
  {}
  hybrid_integer(mpz_class && z)
    :_mpz_val(new mpz_class(std::move(z)))
  {}
  explicit hybrid_integer(const std::string & str)
  {
    unsigned i = 0;
    bool chs = false;
    *this = (hardware_type)0;
    while(i < str.size() && isspace(str[i])) i++;
    if(i < str.size() && str[i] == '-')
      {
	chs = true;
	i++;
      }
    for(; i < str.size(); i++)
      {
	*this *= 10;
	*this += (str[i] - '0');
      }
    if(chs)
      change_sign();
  }  
  hybrid_integer(const hybrid_integer & ci)
    :_hd_val(ci._hd_val),
     _mpz_val(ci._mpz_val != 0 ? new mpz_class(*ci._mpz_val) : 0)
  {}
  hybrid_integer(hybrid_integer && ci)
    :_hd_val(ci._hd_val),
     _mpz_val(ci._mpz_val)
  {
    ci._hd_val = 0;
    ci._mpz_val = 0;
  }
  
  hybrid_integer & operator = (const hybrid_integer & ci)
  {
    if(&ci == this)
      return *this;

    if(!is_gmp() && !ci.is_gmp())
      _hd_val = ci._hd_val;
    else if(is_gmp() && !ci.is_gmp())
      {
	delete _mpz_val;
	_mpz_val = 0;
	_hd_val = ci._hd_val;
      }
    else if(!is_gmp() && ci.is_gmp())
      {
	create_gmp();
	*_mpz_val = *ci._mpz_val;
      }
    else
      *_mpz_val = *ci._mpz_val;
    return *this;
  }

  hybrid_integer & operator = (hybrid_integer && ci)
  {
    swap(ci);        
    return *this;
  }

  hybrid_integer & operator = (hardware_type n)
  {
    delete _mpz_val;

    _hd_val = n;
    _mpz_val = 0;

    return *this;
  }

  hybrid_integer & operator = (const mpz_class & z)
  {
    if(!is_gmp())
      create_gmp();
    *_mpz_val = z;

    return *this;
  }

  hybrid_integer & operator = (mpz_class && z)
  {
    if(!is_gmp())
      create_gmp();
    *_mpz_val = std::move(z);
    
    return *this;
  }
  
  ~hybrid_integer()
  {
    delete _mpz_val;
  }

  bool is_gmp() const
  {
    return _mpz_val != 0;
  }

  const mpz_class & get_mpz_value() const 
  {
    if(!is_gmp()) throw "Not in gmp mode";
    return *_mpz_val;
  }

  const hardware_type & get_hardware_value() const
  {
    if(is_gmp()) throw "In gmp mode";
    return _hd_val;
  }

  void swap(hybrid_integer & ci)
  {
    std::swap(_hd_val, ci._hd_val);
    std::swap(_mpz_val, ci._mpz_val);
  }

  std::string get_str() const
  {
    if(!is_gmp())
      return std::to_string(_hd_val);
    else
      return _mpz_val->get_str();
  }
  
  hybrid_integer & operator += (const hybrid_integer & ci)
  {
    if(!is_gmp() && !ci.is_gmp())
      {
	if(!is_add_overflow(_hd_val, ci._hd_val))
	  _hd_val += ci._hd_val;
	else
	  {
	    create_gmp();
	    *_mpz_val += ci._hd_val;
	  }
      }
    else if(is_gmp() && !ci.is_gmp())
      *_mpz_val += ci._hd_val;
    else if(!is_gmp() && ci.is_gmp())
      {
	create_gmp();
	*_mpz_val += *ci._mpz_val;
      }
    else
      *_mpz_val += *ci._mpz_val;

    return *this;
  }

  template <typename T>
  hybrid_integer & operator += (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    IntType it = (IntType)t;
    if(!is_gmp())
      {
	if(!is_add_overflow(_hd_val, it))
	  _hd_val += it;
	else
	  {
	    create_gmp();
	    *_mpz_val += it;
	  }
      }
    else
      *_mpz_val += it;

    return *this;    
  }

  hybrid_integer & operator -= (const hybrid_integer & ci)
  {
    if(!is_gmp() && !ci.is_gmp())
      {
	if(!is_sub_overflow(_hd_val, ci._hd_val))
	  _hd_val -= ci._hd_val;
	else
	  {
	    create_gmp();
	    *_mpz_val -= ci._hd_val;
	  }
      }
    else if(is_gmp() && !ci.is_gmp())
      *_mpz_val -= ci._hd_val;
    else if(!is_gmp() && ci.is_gmp())
      {
	create_gmp();
	*_mpz_val -= *ci._mpz_val;
      }
    else
      *_mpz_val -= *ci._mpz_val;

    return *this;
  }

  template <typename T>
  hybrid_integer & operator -= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    IntType it = (IntType)t;
    if(!is_gmp())
      {
	if(!is_sub_overflow(_hd_val, it))
	  _hd_val -= it;
	else
	  {
	    create_gmp();
	    *_mpz_val -= it;
	  }
      }
    else
      *_mpz_val -= it;

    return *this;    
  }

  hybrid_integer & operator *= (const hybrid_integer & ci)
  {
    if(!is_gmp() && !ci.is_gmp())
      {
	if(!is_mul_overflow(_hd_val, ci._hd_val))
	  _hd_val *= ci._hd_val;
	else
	  {
	    create_gmp();
	    *_mpz_val *= ci._hd_val;
	  }
      }
    else if(is_gmp() && !ci.is_gmp())
      *_mpz_val *= ci._hd_val;
    else if(!is_gmp() && ci.is_gmp())
      {
	create_gmp();
	*_mpz_val *= *ci._mpz_val;
      }
    else
      *_mpz_val *= *ci._mpz_val;

    return *this;
  }

  template <typename T>
  hybrid_integer & operator *= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    IntType it = (IntType)t;
    if(!is_gmp())
      {
	if(!is_mul_overflow(_hd_val, it))
	  _hd_val *= it;
	else
	  {
	    create_gmp();
	    *_mpz_val *= it;
	  }
      }
    else
      *_mpz_val *= it;

    return *this;    
  }


  hybrid_integer & operator /= (const hybrid_integer & ci)
  {
    if(!is_gmp() && !ci.is_gmp())
      {
	if(!is_div_overflow(_hd_val, ci._hd_val))
	  _hd_val /= ci._hd_val;
	else
	  {
	    create_gmp();
	    *_mpz_val /= ci._hd_val;
	  }
      }
    else if(is_gmp() && !ci.is_gmp())
      *_mpz_val /= ci._hd_val;
    else if(!is_gmp() && ci.is_gmp())
      {
	create_gmp();
	*_mpz_val /= *ci._mpz_val;
      }
    else
      *_mpz_val /= *ci._mpz_val;    

    return *this;
  }

  template <typename T>
  hybrid_integer & operator /= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    IntType it = (IntType)t;
    if(!is_gmp())
      {
	if(!is_div_overflow(_hd_val, it))
	  _hd_val /= it;
	else
	  {
	    create_gmp();
	    *_mpz_val /= it;
	  }
      }
    else 
      *_mpz_val /= it;

    return *this;    
  }

  hybrid_integer & operator %= (const hybrid_integer & ci)
  {
    if(!is_gmp() && !ci.is_gmp())
      {
	if(!is_div_overflow(_hd_val, ci._hd_val))
	  _hd_val %= ci._hd_val;
	else
	  {
	    create_gmp();
	    *_mpz_val %= ci._hd_val;
	  }
      }
    else if(is_gmp() && !ci.is_gmp())
      *_mpz_val %= ci._hd_val;
    else if(!is_gmp() && ci.is_gmp())
      {
	create_gmp();
	*_mpz_val %= *ci._mpz_val;
      }
    else
      *_mpz_val %= *ci._mpz_val;    

    return *this;
  }

  template <typename T>
  hybrid_integer & operator %= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    IntType it = (IntType)t;
    if(!is_gmp())
      {
	if(!is_div_overflow(_hd_val, it))
	  _hd_val %= it;
	else
	  {
	    create_gmp();
	    *_mpz_val %= it;
	  }
      }
    else 
      *_mpz_val %= it;

    return *this;    
  }

  hybrid_integer & change_sign()
  {
    if(!is_gmp())
      {
	if(!is_chs_overflow(_hd_val))
	  _hd_val = -_hd_val;
	else
	  {
	    create_gmp();
	    *_mpz_val *= -1;
	  }
      }
    else
      *_mpz_val *= -1;

    return *this;
  }
  
  explicit operator mpz_class () const
  {
    if(!is_gmp())
      return mpz_class(_hd_val);
    else
      return *_mpz_val;
  }  
};

template <typename IntType>
inline
hybrid_integer<IntType> operator + (const hybrid_integer<IntType> & ci)
{
  return ci;
}

template <typename IntType>
inline
hybrid_integer<IntType> operator - (const hybrid_integer<IntType> & ci)
{
  if(!ci.is_gmp())
    {
      if(!is_chs_overflow(ci.get_hardware_value()))
	return hybrid_integer<IntType>(-ci.get_hardware_value());
      else
	return hybrid_integer<IntType>(-MPZ(ci.get_hardware_value()));
    }
  else
    return hybrid_integer<IntType>(-ci.get_mpz_value());
}

template <typename IntType>
inline
hybrid_integer<IntType> operator + (const hybrid_integer<IntType> & ci1,
				    const hybrid_integer<IntType> & ci2)
{
  if(!ci1.is_gmp() && !ci2.is_gmp())
    {
      if(!is_add_overflow(ci1.get_hardware_value(), ci2.get_hardware_value()))
	return hybrid_integer<IntType>(ci1.get_hardware_value() + ci2.get_hardware_value());
      else
	return hybrid_integer<IntType>(MPZ(ci1.get_hardware_value()) + ci2.get_hardware_value());
    }
  else if(ci1.is_gmp() && !ci2.is_gmp())
    return hybrid_integer<IntType>(ci1.get_mpz_value() + ci2.get_hardware_value());
  else if(!ci1.is_gmp() && ci2.is_gmp())
    return hybrid_integer<IntType>(ci1.get_hardware_value() + ci2.get_mpz_value());
  else
    return hybrid_integer<IntType>(ci1.get_mpz_value() + ci2.get_mpz_value());
}

template <typename IntType, typename T>
inline
hybrid_integer<IntType> operator + (const hybrid_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    {
      if(!is_add_overflow(ci.get_hardware_value(), it))
	return hybrid_integer<IntType>(ci.get_hardware_value() + it);
      else
	return hybrid_integer<IntType>(MPZ(ci.get_hardware_value()) + it);
    }
  else 
    return hybrid_integer<IntType>(ci.get_mpz_value() + it);
}

template <typename IntType, typename T>
inline
hybrid_integer<IntType> operator + (T t, const hybrid_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    {
      if(!is_add_overflow(it, ci.get_hardware_value()))
	return hybrid_integer<IntType>(it + ci.get_hardware_value());
      else
	return hybrid_integer<IntType>(it + MPZ(ci.get_hardware_value()));
    }
  else 
    return hybrid_integer<IntType>(it + ci.get_mpz_value());
}



template <typename IntType>
inline
hybrid_integer<IntType> operator - (const hybrid_integer<IntType> & ci1,
				    const hybrid_integer<IntType> & ci2)
{
  if(!ci1.is_gmp() && !ci2.is_gmp())
    {
      if(!is_sub_overflow(ci1.get_hardware_value(), ci2.get_hardware_value()))
	return hybrid_integer<IntType>(ci1.get_hardware_value() - ci2.get_hardware_value());
      else
	return hybrid_integer<IntType>(MPZ(ci1.get_hardware_value()) - ci2.get_hardware_value());
    }
  else if(ci1.is_gmp() && !ci2.is_gmp())
    return hybrid_integer<IntType>(ci1.get_mpz_value() - ci2.get_hardware_value());
  else if(!ci1.is_gmp() && ci2.is_gmp())
    return hybrid_integer<IntType>(ci1.get_hardware_value() - ci2.get_mpz_value());
  else
    return hybrid_integer<IntType>(ci1.get_mpz_value() - ci2.get_mpz_value());
}

template <typename IntType, typename T>
inline
hybrid_integer<IntType> operator - (const hybrid_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    {
      if(!is_sub_overflow(ci.get_hardware_value(), it))
	return hybrid_integer<IntType>(ci.get_hardware_value() - it);
      else
	return hybrid_integer<IntType>(MPZ(ci.get_hardware_value()) - it);
    }
  else 
    return hybrid_integer<IntType>(ci.get_mpz_value() - it);
}

template <typename IntType, typename T>
inline
hybrid_integer<IntType> operator - (T t, const hybrid_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    {
      if(!is_sub_overflow(it, ci.get_hardware_value()))
	return hybrid_integer<IntType>(it - ci.get_hardware_value());
      else
	return hybrid_integer<IntType>(it - MPZ(ci.get_hardware_value()));
    }
  else 
    return hybrid_integer<IntType>(it - ci.get_mpz_value());
}

template <typename IntType>
inline
hybrid_integer<IntType> operator * (const hybrid_integer<IntType> & ci1,
				    const hybrid_integer<IntType> & ci2)
{
  if(!ci1.is_gmp() && !ci2.is_gmp())
    {
      if(!is_mul_overflow(ci1.get_hardware_value(), ci2.get_hardware_value()))
	return hybrid_integer<IntType>(ci1.get_hardware_value() * ci2.get_hardware_value());
      else
	return hybrid_integer<IntType>(MPZ(ci1.get_hardware_value()) * ci2.get_hardware_value());
    }
  else if(ci1.is_gmp() && !ci2.is_gmp())
    return hybrid_integer<IntType>(ci1.get_mpz_value() * ci2.get_hardware_value());
  else if(!ci1.is_gmp() && ci2.is_gmp())
    return hybrid_integer<IntType>(ci1.get_hardware_value() * ci2.get_mpz_value());
  else
    return hybrid_integer<IntType>(ci1.get_mpz_value() * ci2.get_mpz_value());  
}

template <typename IntType, typename T>
inline
hybrid_integer<IntType> operator * (const hybrid_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    {
      if(!is_mul_overflow(ci.get_hardware_value(), it))
	return hybrid_integer<IntType>(ci.get_hardware_value() * it);
      else
	return hybrid_integer<IntType>(MPZ(ci.get_hardware_value()) * it);
    }
  else 
    return hybrid_integer<IntType>(ci.get_mpz_value() * it);
}

template <typename IntType, typename T>
inline
hybrid_integer<IntType> operator * (T t, const hybrid_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    {
      if(!is_mul_overflow(it, ci.get_hardware_value()))
	return hybrid_integer<IntType>(it * ci.get_hardware_value());
      else
	return hybrid_integer<IntType>(it * MPZ(ci.get_hardware_value()));
    }
  else 
    return hybrid_integer<IntType>(it * ci.get_mpz_value());
}

template <typename IntType>
inline
hybrid_integer<IntType> operator / (const hybrid_integer<IntType> & ci1,
				    const hybrid_integer<IntType> & ci2)
{
  if(!ci1.is_gmp() && !ci2.is_gmp())
    {
      if(!is_div_overflow(ci1.get_hardware_value(), ci2.get_hardware_value()))
	return hybrid_integer<IntType>(ci1.get_hardware_value() / ci2.get_hardware_value());
      else
	return hybrid_integer<IntType>(MPZ(ci1.get_hardware_value()) / ci2.get_hardware_value());
    }
  else if(ci1.is_gmp() && !ci2.is_gmp())
    return hybrid_integer<IntType>(ci1.get_mpz_value() / ci2.get_hardware_value());
  else if(!ci1.is_gmp() && ci2.is_gmp())
    return hybrid_integer<IntType>(ci1.get_hardware_value() / ci2.get_mpz_value());
  else
    return hybrid_integer<IntType>(ci1.get_mpz_value() / ci2.get_mpz_value());  
}

template <typename IntType, typename T>
inline
hybrid_integer<IntType> operator / (const hybrid_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    {
      if(!is_div_overflow(ci.get_hardware_value(), it))
	return hybrid_integer<IntType>(ci.get_hardware_value() / it);
      else
	return hybrid_integer<IntType>(MPZ(ci.get_hardware_value()) / it);
    }
  else 
    return hybrid_integer<IntType>(ci.get_mpz_value() / it);
}

template <typename IntType, typename T>
inline
hybrid_integer<IntType> operator / (T t, const hybrid_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    {
      if(!is_div_overflow(it, ci.get_hardware_value()))
	return hybrid_integer<IntType>(it / ci.get_hardware_value());
      else
	return hybrid_integer<IntType>(it / MPZ(ci.get_hardware_value()));
    }
  else 
    return hybrid_integer<IntType>(it / ci.get_mpz_value());
}


template <typename IntType>
inline
hybrid_integer<IntType> operator % (const hybrid_integer<IntType> & ci1,
				    const hybrid_integer<IntType> & ci2)
{
  if(!ci1.is_gmp() && !ci2.is_gmp())
    {
      if(!is_div_overflow(ci1.get_hardware_value(), ci2.get_hardware_value()))
	return hybrid_integer<IntType>(ci1.get_hardware_value() % ci2.get_hardware_value());
      else
	return hybrid_integer<IntType>(MPZ(ci1.get_hardware_value()) % ci2.get_hardware_value());
    }
  else if(ci1.is_gmp() && !ci2.is_gmp())
    return hybrid_integer<IntType>(ci1.get_mpz_value() % ci2.get_hardware_value());
  else if(!ci1.is_gmp() && ci2.is_gmp())
    return hybrid_integer<IntType>(ci1.get_hardware_value() % ci2.get_mpz_value());
  else
    return hybrid_integer<IntType>(ci1.get_mpz_value() % ci2.get_mpz_value());  
}

template <typename IntType, typename T>
inline
hybrid_integer<IntType> operator % (const hybrid_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    {
      if(!is_div_overflow(ci.get_hardware_value(), it))
	return hybrid_integer<IntType>(ci.get_hardware_value() % it);
      else
	return hybrid_integer<IntType>(MPZ(ci.get_hardware_value()) % it);
    }
  else 
    return hybrid_integer<IntType>(ci.get_mpz_value() % it);
}

template <typename IntType, typename T>
inline
hybrid_integer<IntType> operator % (T t, const hybrid_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    {
      if(!is_div_overflow(it, ci.get_hardware_value()))
	return hybrid_integer<IntType>(it % ci.get_hardware_value());
      else
	return hybrid_integer<IntType>(it % MPZ(ci.get_hardware_value()));
    }
  else 
    return hybrid_integer<IntType>(it % ci.get_mpz_value());
}


template <typename IntType>
inline
bool operator == (const hybrid_integer<IntType> & ci1,
		  const hybrid_integer<IntType> & ci2)
{
  if(!ci1.is_gmp() && !ci2.is_gmp())
    return ci1.get_hardware_value() == ci2.get_hardware_value();
  else if(ci1.is_gmp() && !ci2.is_gmp())
    return ci1.get_mpz_value() == ci2.get_hardware_value();
  else if(!ci1.is_gmp() && ci2.is_gmp())
    return ci1.get_hardware_value() == ci2.get_mpz_value();
  else
    return ci1.get_mpz_value() == ci2.get_mpz_value();    
}

template <typename IntType, typename T>
inline
bool operator == (const hybrid_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    return ci.get_hardware_value() == it;
  else 
    return ci.get_mpz_value() == it;
}

template <typename IntType, typename T>
inline
bool operator == (T t, const hybrid_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    return it == ci.get_hardware_value();
  else 
    return it == ci.get_mpz_value();
}

template <typename IntType>
inline
bool operator != (const hybrid_integer<IntType> & ci1,
		  const hybrid_integer<IntType> & ci2)
{
  return !(ci1 == ci2);
}

template <typename IntType, typename T>
inline
bool operator != (const hybrid_integer<IntType> & ci, T t)
{
  return !(ci == t);
}

template <typename IntType, typename T>
inline
bool operator != (T t, const hybrid_integer<IntType> & ci)
{
  return !(t == ci);
}

template <typename IntType>
inline
bool operator < (const hybrid_integer<IntType> & ci1,
		 const hybrid_integer<IntType> & ci2)
{
  if(!ci1.is_gmp() && !ci2.is_gmp())
    return ci1.get_hardware_value() < ci2.get_hardware_value();
  else if(ci1.is_gmp() && !ci2.is_gmp())
    return ci1.get_mpz_value() < ci2.get_hardware_value();
  else if(!ci1.is_gmp() && ci2.is_gmp())
    return ci1.get_hardware_value() < ci2.get_mpz_value();
  else
    return ci1.get_mpz_value() < ci2.get_mpz_value();    
}

template <typename IntType, typename T>
inline
bool operator < (const hybrid_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    return ci.get_hardware_value() < it;
  else 
    return ci.get_mpz_value() < it;
}

template <typename IntType, typename T>
inline
bool operator < (T t, const hybrid_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    return it < ci.get_hardware_value();
  else 
    return it < ci.get_mpz_value();
}

template <typename IntType>
inline
bool operator <= (const hybrid_integer<IntType> & ci1,
		  const hybrid_integer<IntType> & ci2)
{
  if(!ci1.is_gmp() && !ci2.is_gmp())
    return ci1.get_hardware_value() <= ci2.get_hardware_value();
  else if(ci1.is_gmp() && !ci2.is_gmp())
    return ci1.get_mpz_value() <= ci2.get_hardware_value();
  else if(!ci1.is_gmp() && ci2.is_gmp())
    return ci1.get_hardware_value() <= ci2.get_mpz_value();
  else
    return ci1.get_mpz_value() <= ci2.get_mpz_value();    
}

template <typename IntType, typename T>
inline
bool operator <= (const hybrid_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    return ci.get_hardware_value() <= it;
  else 
    return ci.get_mpz_value() <= it;
}

template <typename IntType, typename T>
inline
bool operator <= (T t, const hybrid_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  if(!ci.is_gmp())
    return it <= ci.get_hardware_value();
  else 
    return it <= ci.get_mpz_value();
}

template <typename IntType>
inline
bool operator > (const hybrid_integer<IntType> & ci1,
		 const hybrid_integer<IntType> & ci2)
{
  return ci2 < ci1;
}

template <typename IntType, typename T>
inline
bool operator > (const hybrid_integer<IntType> & ci, T t)
{
  return t < ci;
}

template <typename IntType, typename T>
inline
bool operator > (T t, const hybrid_integer<IntType> & ci)
{
  return ci < t;
}

template <typename IntType>
inline
bool operator >= (const hybrid_integer<IntType> & ci1,
		  const hybrid_integer<IntType> & ci2)
{
  return ci2 <= ci1;
}

template <typename IntType, typename T>
inline
bool operator >= (const hybrid_integer<IntType> & ci, T t)
{
  return t <= ci;
}

template <typename IntType, typename T>
inline
bool operator >= (T t, const hybrid_integer<IntType> & ci)
{
  return ci <= t;
}

template <typename IntType>
inline
hybrid_integer<IntType> abs(const hybrid_integer<IntType> & ci)
{
  return 0 <= ci ? ci : -ci;
}

template <typename IntType>
inline
void swap(hybrid_integer<IntType> & ci1, hybrid_integer<IntType> & ci2)
{
  ci1.swap(ci2);
}

template <typename IntType>
inline 
std::ostream & operator << (std::ostream & ostr, const hybrid_integer<IntType> & ci)
{
  if(!ci.is_gmp())
    ostr << (long long) ci.get_hardware_value();
  else
    ostr << ci.get_mpz_value();
  return ostr;
}

// Returns the greatest common divisor for m and n
template <typename T>
inline
T gcd(T m, T n)
{
  m = m >= 0 ? m : -m;
  n = n >= 0 ? n : -n;
  while(n)
    {
      T r = m % n;
      m = n;
      n = r;
    }
  return m;
}

// The class reprsents rational numbers by pairs (num, den), where
// both num and den are of some hardware signed integer type.
template <typename IntType>
class hardware_rational {
  static_assert(std::is_integral<IntType>::value &&
		std::is_signed<IntType>::value,
		"IntType must be signed integral type");
public:
  typedef IntType hardware_type;
private:
  hardware_type _num;
  hardware_type _den;
public:
  hardware_rational(hardware_type n = 0, hardware_type d = 1)
    :_num(n),
     _den(d)
  {}

  hardware_rational & canonicalize()
  {
    if(_den < 0)
      {
	_num = -_num;
	_den = -_den;
      }
    hardware_type d = gcd(_num, _den);
    _num /= d;
    _den /= d;
    return *this;
  }

  hardware_type get_num() const
  {
    return _num;
  }

  hardware_type get_den() const
  {
    return _den;
  }

  void swap(hardware_rational & sr)
  {
    std::swap(_num, sr._num);
    std::swap(_den, sr._den);
  }

  std::string get_str() const
  {
    std::string ret = std::to_string(_num);
    if(_den != 1)
      ret += std::string("/") + std::to_string(_den);
    return ret;
  }
  
  hardware_rational & operator += (const hardware_rational & sr)
  {
    _num = _num * sr._den + sr._num * _den;
    _den = _den * sr._den;
    canonicalize();
    return *this;
  }

  template <typename T>
  hardware_rational & operator += (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    _num = _num + (IntType)t * _den;
    return *this;
  }

  hardware_rational & operator -= (const hardware_rational & sr)
  {
    _num = _num * sr._den - sr._num * _den;
    _den = _den * sr._den;
    canonicalize();
    return *this;
  }

  template <typename T>
  hardware_rational & operator -= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    _num = _num - (IntType)t * _den;
    return *this;
  }
  
  hardware_rational & operator *= (const hardware_rational & sr)
  {
    _num = _num * sr._num;
    _den = _den * sr._den;
    canonicalize();
    return *this;
  }

  template <typename T>
  hardware_rational & operator *= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    _num = _num * (IntType)t;
    canonicalize();
    return *this;
  }

  hardware_rational & operator /= (const hardware_rational & sr)
  {
    _num = _num * sr._den;
    _den = _den * sr._num;
    canonicalize();
    return *this;
  }

  template <typename T>
  hardware_rational & operator /= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    _den = _den * (IntType)t;
    canonicalize();
    return *this;
  }

  explicit operator hardware_type () const
  {
    return _num / _den;
  }
};

template <typename IntType>
inline
hardware_rational<IntType> operator + (const hardware_rational<IntType> & sr)
{
  return sr;
}

template <typename IntType>
inline
hardware_rational<IntType> operator - (const hardware_rational<IntType> & sr)
{
  return hardware_rational<IntType>(-sr.get_num(), sr.get_den());
}

template <typename IntType>
inline
hardware_rational<IntType> operator + (const hardware_rational<IntType> & sr1,
				       const hardware_rational<IntType> & sr2)
{   
  return hardware_rational<IntType>(sr1.get_num() * sr2.get_den() +
				    sr2.get_num() * sr1.get_den(),
				    sr1.get_den() * sr2.get_den()).canonicalize();
}

template <typename IntType, typename T>
inline
hardware_rational<IntType> operator + (const hardware_rational<IntType> & sr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  return hardware_rational<IntType>(sr.get_num() + (IntType)t * sr.get_den(), sr.get_den());
}

template <typename IntType, typename T>
inline
hardware_rational<IntType> operator + (T t, const hardware_rational<IntType> & sr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  return hardware_rational<IntType>((IntType)t * sr.get_den() + sr.get_num(), sr.get_den());
}

template <typename IntType>
inline
hardware_rational<IntType> operator - (const hardware_rational<IntType> & sr1,
				       const hardware_rational<IntType> & sr2)
{
  return hardware_rational<IntType>(sr1.get_num() * sr2.get_den() -
				    sr2.get_num() * sr1.get_den(),
				    sr1.get_den() * sr2.get_den()).canonicalize();
}

template <typename IntType, typename T>
inline
hardware_rational<IntType> operator - (const hardware_rational<IntType> & sr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  return hardware_rational<IntType>(sr.get_num() - (IntType)t * sr.get_den(), sr.get_den());
}

template <typename IntType, typename T>
inline
hardware_rational<IntType> operator - (T t, const hardware_rational<IntType> & sr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  return hardware_rational<IntType>((IntType)t * sr.get_den() - sr.get_num(), sr.get_den());
}

template <typename IntType>
inline
hardware_rational<IntType> operator * (const hardware_rational<IntType> & sr1,
				       const hardware_rational<IntType> & sr2)
{
  return hardware_rational<IntType>(sr1.get_num() * sr2.get_num(),
				    sr1.get_den() * sr2.get_den()).canonicalize();
}

template <typename IntType, typename T>
inline
hardware_rational<IntType> operator * (const hardware_rational<IntType> & sr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  return hardware_rational<IntType>(sr.get_num() * (IntType)t, sr.get_den()).canonicalize();
}

template <typename IntType, typename T>
inline
hardware_rational<IntType> operator * (T t, const hardware_rational<IntType> & sr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  return hardware_rational<IntType>((IntType)t * sr.get_num(), sr.get_den()).canonicalize();
}


template <typename IntType>
inline
hardware_rational<IntType> operator / (const hardware_rational<IntType> & sr1,
				       const hardware_rational<IntType> & sr2)
{
  return hardware_rational<IntType>(sr1.get_num() * sr2.get_den(),
				    sr1.get_den() * sr2.get_num()).canonicalize();
}

template <typename IntType, typename T>
inline
hardware_rational<IntType> operator / (const hardware_rational<IntType> & sr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  return hardware_rational<IntType>(sr.get_num(), sr.get_den() * (IntType)t).canonicalize();
}

template <typename IntType, typename T>
inline
hardware_rational<IntType> operator / (T t, const hardware_rational<IntType> & sr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  return hardware_rational<IntType>((IntType)t * sr.get_den(), sr.get_num()).canonicalize();
}

template <typename IntType>
inline
bool operator == (const hardware_rational<IntType> & sr1,
		  const hardware_rational<IntType> & sr2)
{
  return sr1.get_num() == sr2.get_num() && sr1.get_den() == sr2.get_den();
}

template <typename IntType, typename T>
inline
bool operator == (const hardware_rational<IntType> & sr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  return sr.get_num() == (IntType)t && sr.get_den() == 1;
}

template <typename IntType, typename T>
inline
bool operator == (T t, const hardware_rational<IntType> & sr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  return sr.get_num() == (IntType)t && sr.get_den() == 1;
}

template <typename IntType>
inline
bool operator != (const hardware_rational<IntType> & sr1,
		  const hardware_rational<IntType> & sr2)
{
  return !(sr1 == sr2);
}

template <typename IntType, typename T>
inline
bool operator != (const hardware_rational<IntType> & sr, T t)
{
  return !(sr == t);
}

template <typename IntType, typename T>
inline
bool operator != (T t, const hardware_rational<IntType> & sr)
{
  return !(t == sr);
}

template <typename IntType>
inline
bool operator < (const hardware_rational<IntType> & sr1,
		 const hardware_rational<IntType> & sr2)
{
  return sr1.get_num() * sr2.get_den() < sr2.get_num() * sr1.get_den();
}

template <typename IntType, typename T>
inline
bool operator < (const hardware_rational<IntType> & sr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  return  sr.get_num() < (IntType)t * sr.get_den();
}

template <typename IntType, typename T>
inline
bool operator < (T t, const hardware_rational<IntType> & sr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  return (IntType)t * sr.get_den() < sr.get_num();
}

template <typename IntType>
inline
bool operator <= (const hardware_rational<IntType> & sr1,
		  const hardware_rational<IntType> & sr2)
{
  return sr1.get_num() * sr2.get_den() <= sr2.get_num() * sr1.get_den();
}

template <typename IntType, typename T>
inline
bool operator <= (const hardware_rational<IntType> & sr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  return sr.get_num() <= (IntType)t * sr.get_den();
}

template <typename IntType, typename T>
inline
bool operator <= (T t, const hardware_rational<IntType> & sr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  return  (IntType)t * sr.get_den() <= sr.get_num();
}

template <typename IntType>
inline
bool operator > (const hardware_rational<IntType> & sr1,
		 const hardware_rational<IntType> & sr2)
{
  return sr2 < sr1;
}

template <typename IntType, typename T>
inline
bool operator > (const hardware_rational<IntType> & sr, T t)
{
  return t < sr;
}

template <typename IntType, typename T>
inline
bool operator > (T t, const hardware_rational<IntType> & sr)
{
  return sr < t;
}

template <typename IntType>
inline
bool operator >= (const hardware_rational<IntType> & sr1,
		  const hardware_rational<IntType> & sr2)
{
  return sr2 <= sr1;
}

template <typename IntType, typename T>
inline
bool operator >= (const hardware_rational<IntType> & sr, T t)
{
  return t <= sr;
}

template <typename IntType, typename T>
inline
bool operator >= (T t, const hardware_rational<IntType> & sr)
{
  return sr <= t;
}

template <typename IntType>
inline
hardware_rational<IntType> abs(const hardware_rational<IntType> & sr)
{
  return hardware_rational<IntType>(abs(sr.get_num()), sr.get_den());
}

template <typename IntType>
inline
void swap(hardware_rational<IntType> & sr1, hardware_rational<IntType> & sr2)
{
  sr1.swap(sr2);
}

template <typename IntType>
inline 
std::ostream & operator << (std::ostream & ostr, const hardware_rational<IntType> & sr)
{
  ostr << (long long)sr.get_num();
  if(sr.get_den() != 1)
    ostr << "/" << (long long)sr.get_den();
  return ostr;
}


template <typename IntType>
inline 
bool is_add_overflow(const hardware_rational<IntType> & sr1, 
		     const hardware_rational<IntType> & sr2,
		     hardware_rational<IntType> & res)
{
  IntType a = sr1.get_num(),
    b = sr1.get_den(),
    c = sr2.get_num(),
    d = sr2.get_den();
  if(is_mul_overflow(b, d) ||
     is_mul_overflow(a, d) ||
     is_mul_overflow(c, b))
    return true;

  IntType bd = b * d;  
  IntType ad = a * d;
  IntType cb = c * b;
  if(is_add_overflow(ad, cb))
    return true;

  IntType adcb = ad + cb;
  if(is_chs_overflow(adcb))
    return true;
  
  res = hardware_rational<IntType>(adcb, bd);
  res.canonicalize();
  return false;
}

template <typename IntType, typename T>
inline 
bool is_add_overflow(const hardware_rational<IntType> & sr, T t, hardware_rational<IntType> & res)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  IntType a = sr.get_num(), b = sr.get_den();
  if(is_mul_overflow(it, b))
    return true;
  IntType bt = b * it;
  if(is_add_overflow(a, bt))
    return true;
  res = hardware_rational<IntType>(a + bt, b);
  return false;
}

template <typename IntType, typename T>
inline 
bool is_add_overflow(T t, const hardware_rational<IntType> & sr, hardware_rational<IntType> & res)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  IntType a = sr.get_num(), b = sr.get_den();
  if(is_mul_overflow(it, b))
    return true;
  IntType bt = b * it;
  if(is_add_overflow(bt, a))
    return true;
  res = hardware_rational<IntType>(bt + a, b);
  return false;
}

template <typename IntType>
inline 
bool is_sub_overflow(const hardware_rational<IntType> & sr1, 
		     const hardware_rational<IntType> & sr2,
		     hardware_rational<IntType> & res)
{
  IntType a = sr1.get_num(),
    b = sr1.get_den(),
    c = sr2.get_num(),
    d = sr2.get_den();
  if(is_mul_overflow(b, d) ||
     is_mul_overflow(a, d) ||
     is_mul_overflow(c, b))
    return true;

  IntType bd = b * d;
  IntType ad = a * d;
  IntType cb = c * b;
  if(is_sub_overflow(ad, cb))
    return true;

  IntType adcb = ad - cb;
  if(is_chs_overflow(adcb))
    return true;
  
  res = hardware_rational<IntType>(adcb, bd);
  res.canonicalize();
  return false;
}

template <typename IntType, typename T>
inline 
bool is_sub_overflow(const hardware_rational<IntType> & sr, T t, hardware_rational<IntType> & res)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  IntType a = sr.get_num(), b = sr.get_den();
  if(is_mul_overflow(it, b))
    return true;
  IntType bt = b * it;
  if(is_sub_overflow(a, bt))
    return true;
  res = hardware_rational<IntType>(a - bt, b);
  return false;

}

template <typename IntType, typename T>
inline 
bool is_sub_overflow(T t, const hardware_rational<IntType> & sr, hardware_rational<IntType> & res)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  IntType a = sr.get_num(), b = sr.get_den();
  if(is_mul_overflow(it, b))
    return true;
  IntType bt = b * it;
  if(is_sub_overflow(bt, a))
    return true;
  res = hardware_rational<IntType>(bt - a, b);
  return false;
}

template <typename IntType>
inline 
bool is_mul_overflow(const hardware_rational<IntType> & sr1, 
		     const hardware_rational<IntType> & sr2,
		     hardware_rational<IntType> & res)
{
  IntType a = sr1.get_num(),
    b = sr1.get_den(),
    c = sr2.get_num(),
    d = sr2.get_den();
  if(is_mul_overflow(a, c) ||
     is_mul_overflow(b, d))
    return true;

  IntType ac = a * c;
  IntType bd = b * d;
  if(is_chs_overflow(ac) ||
     is_chs_overflow(bd))
    return true;

  res = hardware_rational<IntType>(ac, bd);
  res.canonicalize();
  return false;
}


template <typename IntType, typename T>
inline 
bool is_mul_overflow(const hardware_rational<IntType> & sr, T t, hardware_rational<IntType> & res)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  IntType a = sr.get_num(),
    b = sr.get_den();
  if(is_mul_overflow(a, it))
    return true;
  
  IntType at = a * it;
  if(is_chs_overflow(at))
    return true;

  res = hardware_rational<IntType>(at, b);
  res.canonicalize();
  return false;
}


template <typename IntType, typename T>
inline 
bool is_mul_overflow(T t, const hardware_rational<IntType> & sr, hardware_rational<IntType> & res)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  IntType a = sr.get_num(),
    b = sr.get_den();
  if(is_mul_overflow(it, a))
    return true;
  
  IntType at = it * a;
  if(is_chs_overflow(at))
    return true;

  res = hardware_rational<IntType>(at, b);
  res.canonicalize();
  return false;
}


template <typename IntType>
inline 
bool is_div_overflow(const hardware_rational<IntType> & sr1, 
		     const hardware_rational<IntType> & sr2,
		     hardware_rational<IntType> & res)
{
  IntType a = sr1.get_num(),
    b = sr1.get_den(),
    c = sr2.get_num(),
    d = sr2.get_den();
  if(is_mul_overflow(a, d) ||
     is_mul_overflow(b, c))
    return true;

  IntType ad = a * d;
  IntType bc = b * c;
  if(is_chs_overflow(ad) ||
     is_chs_overflow(bc))
    return true;

  res = hardware_rational<IntType>(ad, bc);
  res.canonicalize();
  return false;
}


template <typename IntType, typename T>
inline 
bool is_div_overflow(const hardware_rational<IntType> & sr, T t, hardware_rational<IntType> & res)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  IntType a = sr.get_num(),
    b = sr.get_den();
  
  if(is_chs_overflow(a))
    return true;
  
  if(is_mul_overflow(b, it))
    return true;

  IntType bt = b * it;
  if(is_chs_overflow(bt))
    return true;

  res = hardware_rational<IntType>(a, bt);
  res.canonicalize();
  return false;
}

template <typename IntType, typename T>
inline 
bool is_div_overflow(T t, const hardware_rational<IntType> & sr, hardware_rational<IntType> & res)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  IntType a = sr.get_num(),
    b = sr.get_den();

  if(is_chs_overflow(a))
    return true;
  
  if(is_mul_overflow(b, it))
    return true;
  
  IntType bt = b * it;
  if(is_chs_overflow(bt))
    return true;

  res = hardware_rational<IntType>(bt, a);
  res.canonicalize();
  return false;

}

template <typename IntType>
inline 
bool is_chs_overflow(const hardware_rational<IntType> & sr, hardware_rational<IntType> & res)
{
  IntType a = sr.get_num(), b = sr.get_den();
  if(is_chs_overflow(a))
    return true;
  res = hardware_rational<IntType>(-a, b);
  return false;
}

template <typename IntType>
inline
bool is_lt_overflow(const hardware_rational<IntType> & sr1,
		    const hardware_rational<IntType> & sr2,
		    bool & res)
{
  IntType a = sr1.get_num(),
    b = sr1.get_den(),
    c = sr2.get_num(),
    d = sr2.get_den();

  if(is_mul_overflow(a, d) || is_mul_overflow(b, c))
    return true;
  
  res = a * d < b * c;
  return false;
}

template <typename IntType, typename T>
inline
bool is_lt_overflow(const hardware_rational<IntType> & sr, T t, bool & res)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType a = sr.get_num(), b = sr.get_den();
  IntType it = (IntType)t;
  if(is_mul_overflow(b, it))
    return true;
  
  res = a < b * it;
  return false;
}

template <typename IntType, typename T>
inline
bool is_lt_overflow(T t, const hardware_rational<IntType> & sr, bool & res)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType a = sr.get_num(), b = sr.get_den();
  IntType it = (IntType)t;
  if(is_mul_overflow(b, it))
    return true;
  
  res = b * it < a;
  return false;
}

template <typename IntType>
inline
bool is_le_overflow(const hardware_rational<IntType> & sr1,
		    const hardware_rational<IntType> & sr2,
		    bool & res)
{
  IntType a = sr1.get_num(),
    b = sr1.get_den(),
    c = sr2.get_num(),
    d = sr2.get_den();

  if(is_mul_overflow(a, d) || is_mul_overflow(b, c))
    return true;
  
  res = a * d <= b * c;
  return false;
}


template <typename IntType, typename T>
inline
bool is_le_overflow(const hardware_rational<IntType> & sr, T t, bool & res)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType a = sr.get_num(), b = sr.get_den();
  IntType it = (IntType)t;
  if(is_mul_overflow(b, it))
    return true;
  
  res = a <= b * it;
  return false;
}

template <typename IntType, typename T>
inline
bool is_le_overflow(T t, const hardware_rational<IntType> & sr, bool & res)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType a = sr.get_num(), b = sr.get_den();
  IntType it = (IntType)t;
  if(is_mul_overflow(b, it))
    return true;
  
  res = b * it <= a;
  return false;
}

template <typename IntType>
class hybrid_rational {
  static_assert(std::is_integral<IntType>::value &&
		std::is_signed<IntType>::value,
		"IntType must be signed integral type");

public:
  typedef IntType hardware_type;
private:
  hardware_rational<IntType> _hd_val = 0;
  mpq_class * _mpq_val = 0;

  void create_gmp()
  {
    _mpq_val = new mpq_class(_hd_val.get_num(), _hd_val.get_den());
    _hd_val = 0;
  }
  
public:
  hybrid_rational(hardware_type n = 0, hardware_type d = 1)
    :_hd_val(n, d)
  {}

  hybrid_rational(const hardware_rational<hardware_type> & sr)
    :_hd_val(sr)
  {}
  
  hybrid_rational(const mpq_class & q)
    :_mpq_val(new mpq_class(q))
  {}

  hybrid_rational(mpq_class && q)
    :_mpq_val(new mpq_class(std::move(q)))
  {}

  hybrid_rational(const hybrid_integer<hardware_type> & n, const hybrid_integer<hardware_type> & d = 1)
  {
    if(!n.is_gmp() && !d.is_gmp())
      _hd_val = hardware_rational<hardware_type>(n.get_hardware_value(), d.get_hardware_value());
    else if(n.is_gmp() && !d.is_gmp())
      _mpq_val = new mpq_class(n.get_mpz_value(), d.get_hardware_value());
    else if(!n.is_gmp() && d.is_gmp())
      _mpq_val = new mpq_class(n.get_hardware_value(), d.get_mpz_value());
    else
      _mpq_val = new mpq_class(n.get_mpz_value(), d.get_mpz_value());
  }

  hybrid_rational(const mpz_class & n, const mpz_class & d)
    :_mpq_val(new mpq_class(n, d))
  {}
  
  explicit hybrid_rational(const std::string & num, const std::string & den = std::string("1"))
    : hybrid_rational(hybrid_integer<hardware_type>(num),
		      hybrid_integer<hardware_type>(den))
  {}
  
  hybrid_rational(const hybrid_rational & cr)
    :_hd_val(cr._hd_val),
     _mpq_val(cr._mpq_val != 0 ? new mpq_class(*cr._mpq_val) : 0)
  {}

  hybrid_rational(hybrid_rational && cr)
    :_hd_val(cr._hd_val),
     _mpq_val(cr._mpq_val)
  {
    cr._hd_val = 0;
    cr._mpq_val = 0;
  }

  ~hybrid_rational()
  {
    delete _mpq_val;
  }

  hybrid_rational & operator = (const hybrid_rational & cr)
  {
    if(&cr == this)
      return *this;

    if(!is_gmp() && !cr.is_gmp())
      _hd_val = cr._hd_val;
    else if(is_gmp() && !cr.is_gmp())
      {
	delete _mpq_val;
	_mpq_val = 0;
	_hd_val = cr._hd_val;
      }
    else if(!is_gmp() && cr.is_gmp())
      {
	create_gmp();
	*_mpq_val = *cr._mpq_val;
      }
    else
      *_mpq_val = *cr._mpq_val;
	
    return *this;
  }

  hybrid_rational & operator = (hybrid_rational && cr)
  {
    swap(cr);    
    return *this;
  }

  hybrid_rational & operator = (hardware_type t)
  {
    delete _mpq_val;
    _mpq_val = 0;
    _hd_val = t;

    return *this;
  }

  hybrid_rational & operator = (const hardware_rational<hardware_type> & sr)
  {
    delete _mpq_val;
    _mpq_val = 0;
    _hd_val = sr;
    return *this;
  }

  hybrid_rational & operator = (const mpq_class & q)
  {
    if(!is_gmp())
      create_gmp();	
    *_mpq_val = q;
    return *this;
  }

  hybrid_rational & operator = (mpq_class && q)
  {
    if(!is_gmp())
      create_gmp();	
    *_mpq_val = std::move(q);
    return *this;
  }

  hybrid_rational & operator = (const hybrid_integer<hardware_type> & ci)
  {
    if(!ci.is_gmp())
      {
	delete _mpq_val;
	_mpq_val = 0;
	_hd_val = ci.get_hardware_value();
      }
    else
      {
	if(!is_gmp())
	  create_gmp();
	*_mpq_val = ci.get_mpz_value();	
      }
      
    return *this;
  }

  hybrid_rational & operator = (const mpz_class & z)
  {
    if(!is_gmp())
      create_gmp();
    *_mpq_val = z;
    return *this;
  }
  
  bool is_gmp() const
  {
    return _mpq_val != 0;
  }

  hybrid_rational & canonicalize()
  {
    if(!is_gmp())
      {
	if(!is_chs_overflow(_hd_val.get_num()) && !is_chs_overflow(_hd_val.get_den()))
	  _hd_val.canonicalize();
	else
	  {
	    create_gmp();
	    _mpq_val->canonicalize();
	  }
      }
    else
      _mpq_val->canonicalize();

    return *this;
  }

  const hardware_rational<hardware_type> & get_hardware_value() const
  {
    if(is_gmp())
      throw "In gmp mode!";
    return _hd_val;
  }

  const mpq_class & get_mpq_value() const
  {
    if(!is_gmp())
      throw "Not in gmp mode!";
    return *_mpq_val;
  }
  
  const hybrid_integer<hardware_type> get_num() const
  {
    if(!is_gmp())
      return hybrid_integer<hardware_type>(_hd_val.get_num());
    else
      return hybrid_integer<hardware_type>(_mpq_val->get_num());
  }

  const hybrid_integer<hardware_type> get_den() const
  {
    if(!is_gmp())
      return hybrid_integer<hardware_type>(_hd_val.get_den());
    else
      return hybrid_integer<hardware_type>(_mpq_val->get_den());
  }

  std::string get_str() const
  {
    if(!is_gmp())
      return _hd_val.get_str();
    else
      return _mpq_val->get_str();
  }
  
  void swap(hybrid_rational & cr)
  {
    ::swap(_hd_val, cr._hd_val);
    std::swap(_mpq_val, cr._mpq_val);
  }

  hybrid_rational & operator += (const hybrid_rational & cr)
  {
    if(!is_gmp() && !cr.is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_add_overflow(_hd_val, cr._hd_val, res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val += MPQ(cr._hd_val.get_num(), cr._hd_val.get_den());
	  }
      }
    else if(is_gmp() && !cr.is_gmp())
      *_mpq_val += MPQ(cr._hd_val.get_num(), cr._hd_val.get_den());    
    else if(!is_gmp() && cr.is_gmp())
      {
	create_gmp();
	*_mpq_val += *cr._mpq_val;
      }
    else
      *_mpq_val += *cr._mpq_val;

    return *this;
  }

  template <typename T>
  hybrid_rational & operator += (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    hardware_type it = (hardware_type)t;

    if(!is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_add_overflow(_hd_val, it, res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val += it;
	  }
      }
    else
      *_mpq_val += it;

    return *this;
  }

  hybrid_rational & operator += (const hybrid_integer<IntType> & ci)
  {    
    if(!is_gmp() && !ci.is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_add_overflow(_hd_val, ci.get_hardware_value(), res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val += ci.get_hardware_value();
	  }
      }
    else if(is_gmp() && !ci.is_gmp())
      *_mpq_val += ci.get_hardware_value();
    else if(!is_gmp() && ci.is_gmp())
      {
	create_gmp();
	*_mpq_val += ci.get_mpz_value();
      }
    else
      *_mpq_val += ci.get_mpz_value();
    
    return *this;
  }

  hybrid_rational & operator += (const hardware_rational<IntType> & sr)
  {
    if(!is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_add_overflow(_hd_val, sr, res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val += MPQ(sr.get_num(), sr.get_den());
	  }
      }
    else
      *_mpq_val += MPQ(sr.get_num(), sr.get_den());    

    return *this;
  }


  hybrid_rational & operator -= (const hybrid_rational & cr)
  {
    if(!is_gmp() && !cr.is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_sub_overflow(_hd_val, cr._hd_val, res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val -= MPQ(cr._hd_val.get_num(), cr._hd_val.get_den());
	  }
      }
    else if(is_gmp() && !cr.is_gmp())
      *_mpq_val -= MPQ(cr._hd_val.get_num(), cr._hd_val.get_den());    
    else if(!is_gmp() && cr.is_gmp())
      {
	create_gmp();
	*_mpq_val -= *cr._mpq_val;
      }
    else
      *_mpq_val -= *cr._mpq_val;

    return *this;
  }

  template <typename T>
  hybrid_rational & operator -= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    hardware_type it = (hardware_type)t;

    if(!is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_sub_overflow(_hd_val, it, res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val -= it;
	  }
      }
    else
      *_mpq_val -= it;

    return *this;
  }

  hybrid_rational & operator -= (const hybrid_integer<IntType> & ci)
  {    
    if(!is_gmp() && !ci.is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_sub_overflow(_hd_val, ci.get_hardware_value(), res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val -= ci.get_hardware_value();
	  }
      }
    else if(is_gmp() && !ci.is_gmp())
      *_mpq_val -= ci.get_hardware_value();
    else if(!is_gmp() && ci.is_gmp())
      {
	create_gmp();
	*_mpq_val -= ci.get_mpz_value();
      }
    else
      *_mpq_val -= ci.get_mpz_value();
    
    return *this;
  }

  hybrid_rational & operator -= (const hardware_rational<IntType> & sr)
  {
    if(!is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_sub_overflow(_hd_val, sr, res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val -= MPQ(sr.get_num(), sr.get_den());
	  }
      }
    else
      *_mpq_val -= MPQ(sr.get_num(), sr.get_den());    

    return *this;
  }

  hybrid_rational & operator *= (const hybrid_rational & cr)
  {
    if(!is_gmp() && !cr.is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_mul_overflow(_hd_val, cr._hd_val, res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val *= MPQ(cr._hd_val.get_num(), cr._hd_val.get_den());
	  }
      }
    else if(is_gmp() && !cr.is_gmp())
      *_mpq_val *= MPQ(cr._hd_val.get_num(), cr._hd_val.get_den());    
    else if(!is_gmp() && cr.is_gmp())
      {
	create_gmp();
	*_mpq_val *= *cr._mpq_val;
      }
    else
      *_mpq_val *= *cr._mpq_val;

    return *this;
  }

  template <typename T>
  hybrid_rational & operator *= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    hardware_type it = (hardware_type)t;

    if(!is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_mul_overflow(_hd_val, it, res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val *= it;
	  }
      }
    else
      *_mpq_val *= it;

    return *this;
  }

  hybrid_rational & operator *= (const hybrid_integer<IntType> & ci)
  {    
    if(!is_gmp() && !ci.is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_mul_overflow(_hd_val, ci.get_hardware_value(), res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val *= ci.get_hardware_value();
	  }
      }
    else if(is_gmp() && !ci.is_gmp())
      *_mpq_val *= ci.get_hardware_value();
    else if(!is_gmp() && ci.is_gmp())
      {
	create_gmp();
	*_mpq_val *= ci.get_mpz_value();
      }
    else
      *_mpq_val *= ci.get_mpz_value();
    
    return *this;
  }

  hybrid_rational & operator *= (const hardware_rational<IntType> & sr)
  {
    if(!is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_mul_overflow(_hd_val, sr, res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val *= MPQ(sr.get_num(), sr.get_den());
	  }
      }
    else
      *_mpq_val *= MPQ(sr.get_num(), sr.get_den());    

    return *this;
  }

  hybrid_rational & operator /= (const hybrid_rational & cr)
  {
    if(!is_gmp() && !cr.is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_div_overflow(_hd_val, cr._hd_val, res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val /= MPQ(cr._hd_val.get_num(), cr._hd_val.get_den());
	  }
      }
    else if(is_gmp() && !cr.is_gmp())
      *_mpq_val /= MPQ(cr._hd_val.get_num(), cr._hd_val.get_den());    
    else if(!is_gmp() && cr.is_gmp())
      {
	create_gmp();
	*_mpq_val /= *cr._mpq_val;
      }
    else
      *_mpq_val /= *cr._mpq_val;

    return *this;
  }

  template <typename T>
  hybrid_rational & operator /= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    hardware_type it = (hardware_type)t;

    if(!is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_div_overflow(_hd_val, it, res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val /= it;
	  }
      }
    else
      *_mpq_val /= it;

    return *this;
  }

  hybrid_rational & operator /= (const hybrid_integer<IntType> & ci)
  {    
    if(!is_gmp() && !ci.is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_div_overflow(_hd_val, ci.get_hardware_value(), res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val /= ci.get_hardware_value();
	  }
      }
    else if(is_gmp() && !ci.is_gmp())
      *_mpq_val /= ci.get_hardware_value();
    else if(!is_gmp() && ci.is_gmp())
      {
	create_gmp();
	*_mpq_val /= ci.get_mpz_value();
      }
    else
      *_mpq_val /= ci.get_mpz_value();
    
    return *this;
  }

  hybrid_rational & operator /= (const hardware_rational<IntType> & sr)
  {
    if(!is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_div_overflow(_hd_val, sr, res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val /= MPQ(sr.get_num(), sr.get_den());
	  }
      }
    else
      *_mpq_val /= MPQ(sr.get_num(), sr.get_den());    

    return *this;
  }

  hybrid_rational & change_sign()
  {
    if(!is_gmp())
      {
	hardware_rational<IntType> res;
	if(!is_chs_overflow(_hd_val, res))
	  _hd_val = res;
	else
	  {
	    create_gmp();
	    *_mpq_val *= -1;
	  }
      }
    else
      *_mpq_val *= -1;
    
    return *this;
  }

  
  explicit operator hybrid_integer<IntType> () const
  {
    if(!is_gmp())
      return hybrid_integer<IntType>((IntType)_hd_val);
    else
      return hybrid_integer<IntType>((mpz_class)*_mpq_val);
  }

  explicit operator mpq_class () const
  {
    if(!is_gmp())
      return mpq_class(_hd_val.get_num(), _hd_val.get_den());
    else
      return *_mpq_val;
      
  }  
};

template <typename IntType>
inline
hybrid_rational<IntType> operator + (const hybrid_rational<IntType> & cr)
{
  return cr;
}

template <typename IntType>
inline
hybrid_rational<IntType> operator - (const hybrid_rational<IntType> & cr)
{
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_chs_overflow(cr.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(-MPQ(cr.get_hardware_value().get_num(),
						   cr.get_hardware_value().get_den()));
    }
  else
    return hybrid_rational<IntType>(-cr.get_mpq_value());
}


template <typename IntType>
inline
hybrid_rational<IntType> operator + (const hybrid_rational<IntType> & cr1,
				     const hybrid_rational<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_add_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ1(cr1.get_hardware_value().get_num(),
						  cr1.get_hardware_value().get_den()) +
					MPQ2(cr2.get_hardware_value().get_num(),
						  cr2.get_hardware_value().get_den()));      	
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_mpq_value() +
				    MPQ(cr2.get_hardware_value().get_num(),
					      cr2.get_hardware_value().get_den()));
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return hybrid_rational<IntType>(MPQ(cr1.get_hardware_value().get_num(),
					      cr1.get_hardware_value().get_den()) +
				    cr2.get_mpq_value());
  else
    return hybrid_rational<IntType>(cr1.get_mpq_value() + cr2.get_mpq_value());    
}

template <typename IntType>
inline
hybrid_rational<IntType> operator + (const hybrid_rational<IntType> & cr1,
				     const hybrid_integer<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_add_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ(cr1.get_hardware_value().get_num(),
						  cr1.get_hardware_value().get_den()) +
					cr2.get_hardware_value());      	
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_mpq_value() + cr2.get_hardware_value());
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return hybrid_rational<IntType>(MPQ(cr1.get_hardware_value().get_num(),
					      cr1.get_hardware_value().get_den()) +
				    cr2.get_mpz_value());
  else
    return hybrid_rational<IntType>(cr1.get_mpq_value() + cr2.get_mpz_value());    
}

template <typename IntType>
inline
hybrid_rational<IntType> operator + (const hybrid_integer<IntType> & cr1,
				     const hybrid_rational<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_add_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(cr1.get_hardware_value() +
					MPQ(cr2.get_hardware_value().get_num(),
						  cr2.get_hardware_value().get_den()));      	
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_mpz_value() + MPQ(cr2.get_hardware_value().get_num(),
								    cr2.get_hardware_value().get_den()));
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_hardware_value() + cr2.get_mpq_value()); 
  else
    return hybrid_rational<IntType>(cr1.get_mpz_value() + cr2.get_mpq_value());    
}

template <typename IntType, typename T>
inline
hybrid_rational<IntType> operator + (const hybrid_rational<IntType> & cr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_add_overflow(cr.get_hardware_value(), it, res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ(cr.get_hardware_value().get_num(),
						  cr.get_hardware_value().get_den()) + it);
    }
  else
    return hybrid_rational<IntType>(cr.get_mpq_value() + it);
}

template <typename IntType, typename T>
inline
hybrid_rational<IntType> operator + (T t, const hybrid_rational<IntType> & cr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_add_overflow(it, cr.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(it + MPQ(cr.get_hardware_value().get_num(),
						       cr.get_hardware_value().get_den()));
    }
  else
    return hybrid_rational<IntType>(it + cr.get_mpq_value());
}

template <typename IntType>
inline
hybrid_rational<IntType> operator + (const hybrid_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_add_overflow(cr.get_hardware_value(), t, res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ1(cr.get_hardware_value().get_num(),
						  cr.get_hardware_value().get_den()) +
					MPQ2(t.get_num(), t.get_den()));
    }
  else
    return hybrid_rational<IntType>(cr.get_mpq_value() + MPQ(t.get_num(), t.get_den()));
}

template <typename IntType>
inline
hybrid_rational<IntType> operator + (const hardware_rational<IntType> & t, const hybrid_rational<IntType> & cr)
{  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_add_overflow(t, cr.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ1(t.get_num(), t.get_den()) +
					MPQ2(cr.get_hardware_value().get_num(),
						  cr.get_hardware_value().get_den()));
    }
  else
    return hybrid_rational<IntType>(MPQ(t.get_num(), t.get_den()) + cr.get_mpq_value());
}

template <typename IntType>
inline
hybrid_rational<IntType> operator - (const hybrid_rational<IntType> & cr1,
				     const hybrid_rational<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_sub_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ1(cr1.get_hardware_value().get_num(),
						  cr1.get_hardware_value().get_den()) -
					MPQ2(cr2.get_hardware_value().get_num(),
						  cr2.get_hardware_value().get_den()));      	
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_mpq_value() -
				    MPQ(cr2.get_hardware_value().get_num(),
					      cr2.get_hardware_value().get_den()));
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return hybrid_rational<IntType>(MPQ(cr1.get_hardware_value().get_num(),
					      cr1.get_hardware_value().get_den()) -
				    cr2.get_mpq_value());
  else
    return hybrid_rational<IntType>(cr1.get_mpq_value() - cr2.get_mpq_value());    
}

template <typename IntType>
inline
hybrid_rational<IntType> operator - (const hybrid_rational<IntType> & cr1,
				     const hybrid_integer<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_sub_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ(cr1.get_hardware_value().get_num(),
						  cr1.get_hardware_value().get_den()) -
					cr2.get_hardware_value());      	
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_mpq_value() - cr2.get_hardware_value());
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return hybrid_rational<IntType>(MPQ(cr1.get_hardware_value().get_num(),
					      cr1.get_hardware_value().get_den()) -
				    cr2.get_mpz_value());
  else
    return hybrid_rational<IntType>(cr1.get_mpq_value() - cr2.get_mpz_value());    
}

template <typename IntType>
inline
hybrid_rational<IntType> operator - (const hybrid_integer<IntType> & cr1,
				     const hybrid_rational<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_sub_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(cr1.get_hardware_value() -
					MPQ(cr2.get_hardware_value().get_num(),
						  cr2.get_hardware_value().get_den()));      	
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_mpz_value() - MPQ(cr2.get_hardware_value().get_num(),
								    cr2.get_hardware_value().get_den()));
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_hardware_value() - cr2.get_mpq_value()); 
  else
    return hybrid_rational<IntType>(cr1.get_mpz_value() - cr2.get_mpq_value());    
}


template <typename IntType, typename T>
inline
hybrid_rational<IntType> operator - (const hybrid_rational<IntType> & cr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_sub_overflow(cr.get_hardware_value(), it, res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ(cr.get_hardware_value().get_num(),
						  cr.get_hardware_value().get_den()) - it);
    }
  else
    return hybrid_rational<IntType>(cr.get_mpq_value() - it);
}

template <typename IntType, typename T>
inline
hybrid_rational<IntType> operator - (T t, const hybrid_rational<IntType> & cr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_sub_overflow(it, cr.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(it - MPQ(cr.get_hardware_value().get_num(),
						       cr.get_hardware_value().get_den()));
    }
  else
    return hybrid_rational<IntType>(it - cr.get_mpq_value());
}

template <typename IntType>
inline
hybrid_rational<IntType> operator - (const hybrid_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_sub_overflow(cr.get_hardware_value(), t, res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ1(cr.get_hardware_value().get_num(),
						  cr.get_hardware_value().get_den()) -
					MPQ2(t.get_num(), t.get_den()));
    }
  else
    return hybrid_rational<IntType>(cr.get_mpq_value() - MPQ(t.get_num(), t.get_den()));
}

template <typename IntType>
inline
hybrid_rational<IntType> operator - (const hardware_rational<IntType> & t, const hybrid_rational<IntType> & cr)
{  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_sub_overflow(t, cr.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ1(t.get_num(), t.get_den()) -
					MPQ2(cr.get_hardware_value().get_num(),
						  cr.get_hardware_value().get_den()));
    }
  else
    return hybrid_rational<IntType>(MPQ(t.get_num(), t.get_den()) - cr.get_mpq_value());
}

template <typename IntType>
inline
hybrid_rational<IntType> operator * (const hybrid_rational<IntType> & cr1,
				     const hybrid_rational<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_mul_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ1(cr1.get_hardware_value().get_num(),
						  cr1.get_hardware_value().get_den()) *
					MPQ2(cr2.get_hardware_value().get_num(),
						  cr2.get_hardware_value().get_den()));      	
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_mpq_value() *
				    MPQ(cr2.get_hardware_value().get_num(),
					      cr2.get_hardware_value().get_den()));
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return hybrid_rational<IntType>(MPQ(cr1.get_hardware_value().get_num(),
					      cr1.get_hardware_value().get_den()) *
				    cr2.get_mpq_value());
  else
    return hybrid_rational<IntType>(cr1.get_mpq_value() * cr2.get_mpq_value());    
}

template <typename IntType>
inline
hybrid_rational<IntType> operator * (const hybrid_rational<IntType> & cr1,
				     const hybrid_integer<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_mul_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ(cr1.get_hardware_value().get_num(),
						  cr1.get_hardware_value().get_den()) *
					cr2.get_hardware_value());      	
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_mpq_value() * cr2.get_hardware_value());
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return hybrid_rational<IntType>(MPQ(cr1.get_hardware_value().get_num(),
					      cr1.get_hardware_value().get_den()) *
				    cr2.get_mpz_value());
  else
    return hybrid_rational<IntType>(cr1.get_mpq_value() * cr2.get_mpz_value());    
}

template <typename IntType>
inline
hybrid_rational<IntType> operator * (const hybrid_integer<IntType> & cr1,
				     const hybrid_rational<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_mul_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(cr1.get_hardware_value() *
					MPQ(cr2.get_hardware_value().get_num(),
						  cr2.get_hardware_value().get_den()));      	
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_mpz_value() * MPQ(cr2.get_hardware_value().get_num(),
								    cr2.get_hardware_value().get_den()));
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_hardware_value() * cr2.get_mpq_value()); 
  else
    return hybrid_rational<IntType>(cr1.get_mpz_value() * cr2.get_mpq_value());    
}

template <typename IntType, typename T>
inline
hybrid_rational<IntType> operator * (const hybrid_rational<IntType> & cr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_mul_overflow(cr.get_hardware_value(), it, res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ(cr.get_hardware_value().get_num(),
						  cr.get_hardware_value().get_den()) * it);
    }
  else
    return hybrid_rational<IntType>(cr.get_mpq_value() * it);
}

template <typename IntType, typename T>
inline
hybrid_rational<IntType> operator * (T t, const hybrid_rational<IntType> & cr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_mul_overflow(it, cr.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(it * MPQ(cr.get_hardware_value().get_num(),
						       cr.get_hardware_value().get_den()));
    }
  else
    return hybrid_rational<IntType>(it * cr.get_mpq_value());
}

template <typename IntType>
inline
hybrid_rational<IntType> operator * (const hybrid_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_mul_overflow(cr.get_hardware_value(), t, res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ1(cr.get_hardware_value().get_num(),
						  cr.get_hardware_value().get_den()) *
					MPQ2(t.get_num(), t.get_den()));
    }
  else
    return hybrid_rational<IntType>(cr.get_mpq_value() * MPQ(t.get_num(), t.get_den()));
}

template <typename IntType>
inline
hybrid_rational<IntType> operator * (const hardware_rational<IntType> & t, const hybrid_rational<IntType> & cr)
{  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_mul_overflow(t, cr.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ1(t.get_num(), t.get_den()) *
					MPQ2(cr.get_hardware_value().get_num(),
						  cr.get_hardware_value().get_den()));
    }
  else
    return hybrid_rational<IntType>(MPQ(t.get_num(), t.get_den()) * cr.get_mpq_value());
}

template <typename IntType>
inline
hybrid_rational<IntType> operator / (const hybrid_rational<IntType> & cr1,
				     const hybrid_rational<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_div_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ1(cr1.get_hardware_value().get_num(),
						  cr1.get_hardware_value().get_den()) /
					MPQ2(cr2.get_hardware_value().get_num(),
						  cr2.get_hardware_value().get_den()));      	
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_mpq_value() /
				    MPQ(cr2.get_hardware_value().get_num(),
					      cr2.get_hardware_value().get_den()));
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return hybrid_rational<IntType>(MPQ(cr1.get_hardware_value().get_num(),
					      cr1.get_hardware_value().get_den()) /
				    cr2.get_mpq_value());
  else
    return hybrid_rational<IntType>(cr1.get_mpq_value() / cr2.get_mpq_value());    
}

template <typename IntType>
inline
hybrid_rational<IntType> operator / (const hybrid_rational<IntType> & cr1,
				     const hybrid_integer<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_div_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ(cr1.get_hardware_value().get_num(),
						  cr1.get_hardware_value().get_den()) /
					cr2.get_hardware_value());      	
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_mpq_value() / cr2.get_hardware_value());
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return hybrid_rational<IntType>(MPQ(cr1.get_hardware_value().get_num(),
					      cr1.get_hardware_value().get_den()) /
				    cr2.get_mpz_value());
  else
    return hybrid_rational<IntType>(cr1.get_mpq_value() / cr2.get_mpz_value());    
}

template <typename IntType>
inline
hybrid_rational<IntType> operator / (const hybrid_integer<IntType> & cr1,
				     const hybrid_rational<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_div_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(cr1.get_hardware_value() /
					MPQ(cr2.get_hardware_value().get_num(),
						  cr2.get_hardware_value().get_den()));      	
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_mpz_value() / MPQ(cr2.get_hardware_value().get_num(),
								    cr2.get_hardware_value().get_den()));
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return hybrid_rational<IntType>(cr1.get_hardware_value() / cr2.get_mpq_value()); 
  else
    return hybrid_rational<IntType>(cr1.get_mpz_value() / cr2.get_mpq_value());    
}


template <typename IntType, typename T>
inline
hybrid_rational<IntType> operator / (const hybrid_rational<IntType> & cr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_div_overflow(cr.get_hardware_value(), it, res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ(cr.get_hardware_value().get_num(),
						  cr.get_hardware_value().get_den()) / it);
    }
  else
    return hybrid_rational<IntType>(cr.get_mpq_value() / it);
}

template <typename IntType, typename T>
inline
hybrid_rational<IntType> operator / (T t, const hybrid_rational<IntType> & cr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_div_overflow(it, cr.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(it / MPQ(cr.get_hardware_value().get_num(),
						       cr.get_hardware_value().get_den()));
    }
  else
    return hybrid_rational<IntType>(it / cr.get_mpq_value());
}

template <typename IntType>
inline
hybrid_rational<IntType> operator / (const hybrid_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_div_overflow(cr.get_hardware_value(), t, res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ1(cr.get_hardware_value().get_num(),
						  cr.get_hardware_value().get_den()) /
					MPQ2(t.get_num(), t.get_den()));
    }
  else
    return hybrid_rational<IntType>(cr.get_mpq_value() / MPQ(t.get_num(), t.get_den()));
}

template <typename IntType>
inline
hybrid_rational<IntType> operator / (const hardware_rational<IntType> & t, const hybrid_rational<IntType> & cr)
{  
  if(!cr.is_gmp())
    {
      hardware_rational<IntType> res;
      if(!is_div_overflow(t, cr.get_hardware_value(), res))
	return hybrid_rational<IntType>(res);
      else
	return hybrid_rational<IntType>(MPQ1(t.get_num(), t.get_den()) /
					MPQ2(cr.get_hardware_value().get_num(),
						  cr.get_hardware_value().get_den()));
    }
  else
    return hybrid_rational<IntType>(MPQ(t.get_num(), t.get_den()) / cr.get_mpq_value());
}


template <typename IntType>
inline
bool operator == (const hybrid_rational<IntType> & cr1,
		  const hybrid_rational<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    return cr1.get_hardware_value() == cr2.get_hardware_value();
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return cr1.get_mpq_value().get_num() == cr2.get_hardware_value().get_num() &&
      cr1.get_mpq_value().get_den() == cr2.get_hardware_value().get_den();
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return cr1.get_hardware_value().get_num() == cr2.get_mpq_value().get_num() &&
      cr1.get_hardware_value().get_den() == cr2.get_mpq_value().get_den();
  else
    return cr1.get_mpq_value() == cr2.get_mpq_value();
}

template <typename IntType>
inline
bool operator == (const hybrid_rational<IntType> & cr1,
		  const hybrid_integer<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    return cr1.get_hardware_value() == cr2.get_hardware_value();
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return cr1.get_mpq_value() == cr2.get_hardware_value();
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return cr1.get_hardware_value().get_num() == cr2.get_mpz_value() &&
      cr1.get_hardware_value().get_den() == 1;
  else
    return cr1.get_mpq_value() == cr2.get_mpz_value();
}

template <typename IntType>
inline
bool operator == (const hybrid_integer<IntType> & cr1,
		  const hybrid_rational<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    return cr1.get_hardware_value() == cr2.get_hardware_value();
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return cr1.get_mpz_value() == cr2.get_hardware_value().get_num() &&
      cr2.get_hardware_value().get_den() == 1;
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return cr1.get_hardware_value() == cr2.get_mpq_value();    
  else
    return cr1.get_mpz_value() == cr2.get_mpq_value();
}

template <typename IntType, typename T>
inline
bool operator == (const hybrid_rational<IntType> & cr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  
  if(!cr.is_gmp())
    return cr.get_hardware_value() == it;
  else
    return cr.get_mpq_value() == it;
}

template <typename IntType, typename T>
inline
bool operator == (T t, const hybrid_rational<IntType> & cr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  
  if(!cr.is_gmp())
    return it == cr.get_hardware_value();
  else
    return it == cr.get_mpq_value();
}

template <typename IntType>
inline
bool operator == (const hybrid_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  if(!cr.is_gmp())
    return cr.get_hardware_value() == t;
  else
    return cr.get_mpq_value().get_num() == t.get_num() &&
      cr.get_mpq_value().get_den() == t.get_den();
}

template <typename IntType>
inline
bool operator == (const hardware_rational<IntType> & t, const hybrid_rational<IntType> & cr)
{  
  if(!cr.is_gmp())
    return t == cr.get_hardware_value();
  else
    return t.get_num() == cr.get_mpq_value().get_num() &&
      t.get_den() == cr.get_mpq_value().get_den();
}


template <typename IntType>
inline
bool operator < (const hybrid_rational<IntType> & cr1,
		 const hybrid_rational<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      bool res;
      if(!is_lt_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return res;
      else
	return MPQ1(cr1.get_hardware_value().get_num(),
			 cr1.get_hardware_value().get_den()) <
	  MPQ2(cr2.get_hardware_value().get_num(),
		    cr2.get_hardware_value().get_den());
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return cr1.get_mpq_value() <
      MPQ(cr2.get_hardware_value().get_num(),
		cr2.get_hardware_value().get_den());
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return MPQ(cr1.get_hardware_value().get_num(),
		     cr1.get_hardware_value().get_den()) < cr2.get_mpq_value();
  else
    return cr1.get_mpq_value() < cr2.get_mpq_value();
}

template <typename IntType>
inline
bool operator < (const hybrid_rational<IntType> & cr1,
		 const hybrid_integer<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      bool res;
      if(!is_lt_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return res;
      else
	return MPQ(cr1.get_hardware_value().get_num(),
			 cr1.get_hardware_value().get_den()) < cr2.get_hardware_value();
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return cr1.get_mpq_value() < cr2.get_hardware_value();
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return MPQ(cr1.get_hardware_value().get_num(),
		     cr1.get_hardware_value().get_den()) < cr2.get_mpz_value();
  else
    return cr1.get_mpq_value() < cr2.get_mpz_value();
}

template <typename IntType>
inline
bool operator < (const hybrid_integer<IntType> & cr1,
		 const hybrid_rational<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      bool res;
      if(!is_lt_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return res;
      else
	return cr1.get_hardware_value() < MPQ(cr2.get_hardware_value().get_num(),
						    cr2.get_hardware_value().get_den());
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return cr1.get_mpz_value() < MPQ(cr2.get_hardware_value().get_num(),
					   cr2.get_hardware_value().get_den());
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return cr1.get_hardware_value() < cr2.get_mpq_value();    
  else
    return cr1.get_mpz_value() < cr2.get_mpq_value();
}

template <typename IntType, typename T>
inline
bool operator < (const hybrid_rational<IntType> & cr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  
  if(!cr.is_gmp())
    {
      bool res;
      if(!is_lt_overflow(cr.get_hardware_value(), it, res))
	return res;
      else
	return MPQ(cr.get_hardware_value().get_num(),
			 cr.get_hardware_value().get_den()) < it;				       
    }
  else
    return cr.get_mpq_value() < it;
}

template <typename IntType, typename T>
inline
bool operator < (T t, const hybrid_rational<IntType> & cr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  
  if(!cr.is_gmp())
    {
      bool res;
      if(!is_lt_overflow(it, cr.get_hardware_value(), res))
	return res;
      else
	return it < MPQ(cr.get_hardware_value().get_num(),
			      cr.get_hardware_value().get_den());
    }
  else
    return it < cr.get_mpq_value();
}

template <typename IntType>
inline
bool operator < (const hybrid_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  if(!cr.is_gmp())
    {
      bool res;
      if(!is_lt_overflow(cr.get_hardware_value(), t, res))
	return res;
      else
	return MPQ1(cr.get_hardware_value().get_num(),
		    cr.get_hardware_value().get_den()) <
	  MPQ2(t.get_num(), t.get_den());
    }
  else
    return cr.get_mpq_value() < MPQ(t.get_num(), t.get_den());
}

template <typename IntType>
inline
bool operator < (const hardware_rational<IntType> & t, const hybrid_rational<IntType> & cr)
{  
  if(!cr.is_gmp())
    {
      bool res;
      if(!is_lt_overflow(t, cr.get_hardware_value(), res))
	return res;
      else
	return MPQ1(t.get_num(), t.get_den()) <
	  MPQ2(cr.get_hardware_value().get_num(),
	       cr.get_hardware_value().get_den());
    }
  else
    return MPQ(t.get_num(), t.get_den()) < cr.get_mpq_value();
}

template <typename IntType>
inline
bool operator <= (const hybrid_rational<IntType> & cr1,
		  const hybrid_rational<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      bool res;
      if(!is_le_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return res;
      else
	return MPQ1(cr1.get_hardware_value().get_num(),
		    cr1.get_hardware_value().get_den()) <=
	  MPQ2(cr2.get_hardware_value().get_num(),
	       cr2.get_hardware_value().get_den());
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return cr1.get_mpq_value() <=
      MPQ(cr2.get_hardware_value().get_num(),
		cr2.get_hardware_value().get_den());
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return MPQ(cr1.get_hardware_value().get_num(),
		     cr1.get_hardware_value().get_den()) <= cr2.get_mpq_value();
  else
    return cr1.get_mpq_value() <= cr2.get_mpq_value();
}

template <typename IntType>
inline
bool operator <= (const hybrid_rational<IntType> & cr1,
		  const hybrid_integer<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      bool res;
      if(!is_le_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return res;
      else
	return MPQ(cr1.get_hardware_value().get_num(),
			 cr1.get_hardware_value().get_den()) <= cr2.get_hardware_value();
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return cr1.get_mpq_value() <= cr2.get_hardware_value();
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return MPQ(cr1.get_hardware_value().get_num(),
		     cr1.get_hardware_value().get_den()) <= cr2.get_mpz_value();
  else
    return cr1.get_mpq_value() <= cr2.get_mpz_value();
}

template <typename IntType>
inline
bool operator <= (const hybrid_integer<IntType> & cr1,
		  const hybrid_rational<IntType> & cr2)
{
  if(!cr1.is_gmp() && !cr2.is_gmp())
    {
      bool res;
      if(!is_le_overflow(cr1.get_hardware_value(), cr2.get_hardware_value(), res))
	return res;
      else
	return cr1.get_hardware_value() <= MPQ(cr2.get_hardware_value().get_num(),
						     cr2.get_hardware_value().get_den());
    }
  else if(cr1.is_gmp() && !cr2.is_gmp())
    return cr1.get_mpz_value() <= MPQ(cr2.get_hardware_value().get_num(),
					    cr2.get_hardware_value().get_den());
  else if(!cr1.is_gmp() && cr2.is_gmp())
    return cr1.get_hardware_value() <= cr2.get_mpq_value();    
  else
    return cr1.get_mpz_value() <= cr2.get_mpq_value();
}

template <typename IntType, typename T>
inline
bool operator <= (const hybrid_rational<IntType> & cr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  
  if(!cr.is_gmp())
    {
      bool res;
      if(!is_le_overflow(cr.get_hardware_value(), it, res))
	return res;
      else
	return MPQ(cr.get_hardware_value().get_num(),
			 cr.get_hardware_value().get_den()) <= it;				       
    }
  else
    return cr.get_mpq_value() <= it;
}

template <typename IntType, typename T>
inline
bool operator <= (T t, const hybrid_rational<IntType> & cr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  
  if(!cr.is_gmp())
    {
      bool res;
      if(!is_le_overflow(it, cr.get_hardware_value(), res))
	return res;
      else
	return it <= MPQ(cr.get_hardware_value().get_num(),
			       cr.get_hardware_value().get_den());
    }
  else
    return it <= cr.get_mpq_value();
}

template <typename IntType>
inline
bool operator <= (const hybrid_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  if(!cr.is_gmp())
    {
      bool res;
      if(!is_le_overflow(cr.get_hardware_value(), t, res))
	return res;
      else
	return MPQ1(cr.get_hardware_value().get_num(),
		    cr.get_hardware_value().get_den()) <=
	  MPQ2(t.get_num(), t.get_den());
    }
  else
    return cr.get_mpq_value() <= MPQ(t.get_num(), t.get_den());
}

template <typename IntType>
inline
bool operator <= (const hardware_rational<IntType> & t, const hybrid_rational<IntType> & cr)
{  
  if(!cr.is_gmp())
    {
      bool res;
      if(!is_le_overflow(t, cr.get_hardware_value(), res))
	return res;
      else
	return MPQ1(t.get_num(), t.get_den()) <=
	  MPQ2(cr.get_hardware_value().get_num(),
	       cr.get_hardware_value().get_den());
    }
  else
    return MPQ(t.get_num(), t.get_den()) <= cr.get_mpq_value();
}

template <typename IntType>
inline
bool operator != (const hybrid_rational<IntType> & cr1,
		  const hybrid_rational<IntType> & cr2)
{
  return !(cr1 == cr2);
}

template <typename IntType>
inline
bool operator != (const hybrid_rational<IntType> & cr1,
		  const hybrid_integer<IntType> & cr2)
{
  return !(cr1 == cr2);
}

template <typename IntType>
inline
bool operator != (const hybrid_integer<IntType> & cr1,
		  const hybrid_rational<IntType> & cr2)
{
  return !(cr1 == cr2);
}

template <typename IntType, typename T>
inline
bool operator != (const hybrid_rational<IntType> & cr, T t)
{
  return !(cr == t);
}

template <typename IntType, typename T>
inline
bool operator != (T t, const hybrid_rational<IntType> & cr)
{
  return !(t == cr);
}

template <typename IntType>
inline
bool operator != (const hybrid_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  return !(cr == t);
}

template <typename IntType>
inline
bool operator != (const hardware_rational<IntType> & t, const hybrid_rational<IntType> & cr)
{  
  return !(t == cr);
}

template <typename IntType>
inline
bool operator > (const hybrid_rational<IntType> & cr1,
		 const hybrid_rational<IntType> & cr2)
{
  return cr2 < cr1;
}

template <typename IntType>
inline
bool operator > (const hybrid_rational<IntType> & cr1,
		 const hybrid_integer<IntType> & cr2)
{
  return cr2 < cr1;
}

template <typename IntType>
inline
bool operator > (const hybrid_integer<IntType> & cr1,
		 const hybrid_rational<IntType> & cr2)
{
  return cr2 < cr1;
}

template <typename IntType, typename T>
inline
bool operator > (const hybrid_rational<IntType> & cr, T t)
{
  return t < cr;
}

template <typename IntType, typename T>
inline
bool operator > (T t, const hybrid_rational<IntType> & cr)
{
  return cr < t;
}

template <typename IntType>
inline
bool operator > (const hybrid_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  return t < cr;
}

template <typename IntType>
inline
bool operator > (const hardware_rational<IntType> & t, const hybrid_rational<IntType> & cr)
{  
  return cr < t;
}


template <typename IntType>
inline
bool operator >= (const hybrid_rational<IntType> & cr1,
		  const hybrid_rational<IntType> & cr2)
{
  return cr2 <= cr1;
}

template <typename IntType>
inline
bool operator >= (const hybrid_rational<IntType> & cr1,
		  const hybrid_integer<IntType> & cr2)
{
  return cr2 <= cr1;
}

template <typename IntType>
inline
bool operator >= (const hybrid_integer<IntType> & cr1,
		  const hybrid_rational<IntType> & cr2)
{
  return cr2 <= cr1;
}

template <typename IntType, typename T>
inline
bool operator >= (const hybrid_rational<IntType> & cr, T t)
{
  return t <= cr;
}

template <typename IntType, typename T>
inline
bool operator >= (T t, const hybrid_rational<IntType> & cr)
{
  return cr <= t;
}

template <typename IntType>
inline
bool operator >= (const hybrid_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  return t <= cr;
}

template <typename IntType>
inline
bool operator >= (const hardware_rational<IntType> & t, const hybrid_rational<IntType> & cr)
{  
  return cr <= t;
}

template <typename IntType>
inline
hybrid_rational<IntType> abs(const hybrid_rational<IntType> & cr)
{
  return cr >= 0 ? cr : -cr;
}

template <typename IntType>
inline
void swap(hybrid_rational<IntType> & cr1,
	  hybrid_rational<IntType> & cr2)
{
  cr1.swap(cr2);
}

template <typename IntType>
inline
std::ostream & operator << (std::ostream & ostr, const hybrid_rational<IntType> & cr)
{
  if(!cr.is_gmp())
    ostr << cr.get_hardware_value();
  else
    ostr << cr.get_mpq_value();
  return ostr;
}

#endif // _HYBRID_TYPES_H
