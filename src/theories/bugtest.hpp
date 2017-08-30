#ifndef _BUGTEST_H
#define _BUGTEST_H

#include "hybrid_types.hpp"

template <typename IntType>
class bugtest_integer {
public:
  mpz_class _mpz_val;
  hybrid_integer<IntType> _hi_val;

  typedef typename hybrid_integer<IntType>::hardware_type hardware_type;
  
public:
  bugtest_integer(hardware_type n = 0)
    :_mpz_val(n), _hi_val(n)
  {
    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
  }
  bugtest_integer(const mpz_class & z)
    :_mpz_val(z), _hi_val(z)
  {
    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
  }

  explicit bugtest_integer(const std::string & str)
    :_mpz_val(str), _hi_val(str)
  {
    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
  }  

  bugtest_integer(const hybrid_integer<hardware_type> & hi)
    :_mpz_val((mpz_class)hi), _hi_val(hi)
  {}

  bugtest_integer & operator = (hardware_type n)
  {
    _mpz_val = n;
    _hi_val = n;

    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
    
    return *this;
  }

  bugtest_integer & operator = (const mpz_class & z)
  {
    _mpz_val = z;
    _hi_val = z;

    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
    
    return *this;
  }

  bool is_gmp() const
  {
    return _hi_val.is_gmp();
  }
  
  
  void swap(bugtest_integer & ci)
  {
    _mpz_val.swap(ci._mpz_val);
    _hi_val.swap(ci._hi_val);
  }

  std::string get_str() const
  {
    assert(_mpz_val.get_str() == _hi_val.get_str());     
    return _mpz_val.get_str();
  }



  bugtest_integer & operator += (const bugtest_integer & ci)
  {
    _mpz_val += ci._mpz_val;
    _hi_val += ci._hi_val;

    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
    
    return *this;
  }

  template <typename T>
  bugtest_integer & operator += (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    IntType it = (IntType)t;
    _mpz_val += it;
    _hi_val += it;

    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
    
    return *this;    
  }

  bugtest_integer & operator -= (const bugtest_integer & ci)
  {
    _mpz_val -= ci._mpz_val;
    _hi_val -= ci._hi_val;

    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
    
    return *this;
  }

  template <typename T>
  bugtest_integer & operator -= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    IntType it = (IntType)t;
    _mpz_val -= it;
    _hi_val -= it;

    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
    
    return *this;    
  }

  bugtest_integer & operator *= (const bugtest_integer & ci)
  {
    _mpz_val *= ci._mpz_val;
    _hi_val *= ci._hi_val;

    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
    
    return *this;
  }

  template <typename T>
  bugtest_integer & operator *= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    IntType it = (IntType)t;
    _mpz_val *= it;
    _hi_val *= it;

    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
    
    return *this;    
  }


  bugtest_integer & operator /= (const bugtest_integer & ci)
  {
    _mpz_val /= ci._mpz_val;
    _hi_val /= ci._hi_val;

    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
    
    return *this;
  }

  template <typename T>
  bugtest_integer & operator /= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    IntType it = (IntType)t;
    _mpz_val /= it;
    _hi_val /= it;

    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
    
    return *this;    
  }

  bugtest_integer & operator %= (const bugtest_integer & ci)
  {
    _mpz_val %= ci._mpz_val;
    _hi_val %= ci._hi_val;

    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
    
    return *this;
  }

  template <typename T>
  bugtest_integer & operator %= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    IntType it = (IntType)t;
    _mpz_val %= it;
    _hi_val %= it;

    assert(hybrid_integer<hardware_type>(_mpz_val) == _hi_val);
    
    return *this;    
  }

  bugtest_integer & change_sign()
  {
    *this *= -1;
    
    return *this;
  }
  
  explicit operator mpz_class () const
  {
    assert(_mpz_val == _hi_val.operator mpz_class ());
    return _mpz_val;
  }  

};


template <typename IntType>
inline
bugtest_integer<IntType> operator + (const bugtest_integer<IntType> & ci)
{
  return ci;
}

template <typename IntType>
inline
bugtest_integer<IntType> operator - (const bugtest_integer<IntType> & ci)
{
  assert(hybrid_integer<IntType>(-ci._mpz_val) == -ci._hi_val);
  return -ci._hi_val;
}

template <typename IntType>
inline
bugtest_integer<IntType> operator + (const bugtest_integer<IntType> & ci1,
				    const bugtest_integer<IntType> & ci2)
{
  assert(ci1._mpz_val + ci2._mpz_val == ci1._hi_val + ci2._hi_val);
  return ci1._hi_val + ci2._hi_val;
}

template <typename IntType, typename T>
inline
bugtest_integer<IntType> operator + (const bugtest_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(ci._mpz_val + it == ci._hi_val + it);
  return ci._hi_val + it;
}

template <typename IntType, typename T>
inline
bugtest_integer<IntType> operator + (T t, const bugtest_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(it + ci._mpz_val == it + ci._hi_val);
  return it + ci._hi_val;
}



template <typename IntType>
inline
bugtest_integer<IntType> operator - (const bugtest_integer<IntType> & ci1,
				    const bugtest_integer<IntType> & ci2)
{
  assert(ci1._mpz_val - ci2._mpz_val == ci1._hi_val - ci2._hi_val);
  return ci1._hi_val - ci2._hi_val;
}

template <typename IntType, typename T>
inline
bugtest_integer<IntType> operator - (const bugtest_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(ci._mpz_val - it == ci._hi_val - it);
  return ci._hi_val - it;
}

template <typename IntType, typename T>
inline
bugtest_integer<IntType> operator - (T t, const bugtest_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(it - ci._mpz_val == it - ci._hi_val);
  return it - ci._hi_val;
}


template <typename IntType>
inline
bugtest_integer<IntType> operator * (const bugtest_integer<IntType> & ci1,
				    const bugtest_integer<IntType> & ci2)
{
  assert(ci1._mpz_val * ci2._mpz_val == ci1._hi_val * ci2._hi_val);
  return ci1._hi_val * ci2._hi_val;
}

template <typename IntType, typename T>
inline
bugtest_integer<IntType> operator * (const bugtest_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(ci._mpz_val * it == ci._hi_val * it);
  return ci._hi_val * it;
}

template <typename IntType, typename T>
inline
bugtest_integer<IntType> operator * (T t, const bugtest_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(it * ci._mpz_val == it * ci._hi_val);
  return it * ci._hi_val;
}



template <typename IntType>
inline
bugtest_integer<IntType> operator / (const bugtest_integer<IntType> & ci1,
				    const bugtest_integer<IntType> & ci2)
{
  assert(ci1._mpz_val / ci2._mpz_val == ci1._hi_val / ci2._hi_val);
  return ci1._hi_val / ci2._hi_val;
}

template <typename IntType, typename T>
inline
bugtest_integer<IntType> operator / (const bugtest_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(ci._mpz_val / it == ci._hi_val / it);
  return ci._hi_val / it;
}

template <typename IntType, typename T>
inline
bugtest_integer<IntType> operator / (T t, const bugtest_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(it / ci._mpz_val == it / ci._hi_val);
  return it / ci._hi_val;
}


template <typename IntType>
inline
bugtest_integer<IntType> operator % (const bugtest_integer<IntType> & ci1,
				    const bugtest_integer<IntType> & ci2)
{
  assert(ci1._mpz_val % ci2._mpz_val == ci1._hi_val % ci2._hi_val);
  return ci1._hi_val % ci2._hi_val;
}

template <typename IntType, typename T>
inline
bugtest_integer<IntType> operator % (const bugtest_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(ci._mpz_val % it == ci._hi_val % it);
  return ci._hi_val % it;
}

template <typename IntType, typename T>
inline
bugtest_integer<IntType> operator % (T t, const bugtest_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(it % ci._mpz_val == it % ci._hi_val);
  return it % ci._hi_val;
}


template <typename IntType>
inline
bool operator == (const bugtest_integer<IntType> & ci1,
		  const bugtest_integer<IntType> & ci2)
{
  assert((ci1._mpz_val == ci2._mpz_val) == (ci1._hi_val == ci2._hi_val));
  return ci1._hi_val == ci2._hi_val;
}

template <typename IntType, typename T>
inline
bool operator == (const bugtest_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert((ci._mpz_val == it) == (ci._hi_val == it));
  return ci._hi_val == it;  
}

template <typename IntType, typename T>
inline
bool operator == (T t, const bugtest_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert((it == ci._mpz_val) == (it == ci._hi_val));
  return it == ci._hi_val;
}

template <typename IntType>
inline
bool operator != (const bugtest_integer<IntType> & ci1,
		  const bugtest_integer<IntType> & ci2)
{
  return !(ci1 == ci2);
}

template <typename IntType, typename T>
inline
bool operator != (const bugtest_integer<IntType> & ci, T t)
{
  return !(ci == t);
}

template <typename IntType, typename T>
inline
bool operator != (T t, const bugtest_integer<IntType> & ci)
{
  return !(t == ci);
}

template <typename IntType>
inline
bool operator < (const bugtest_integer<IntType> & ci1,
		 const bugtest_integer<IntType> & ci2)
{
  assert((ci1._mpz_val < ci2._mpz_val) == (ci1._hi_val < ci2._hi_val));
  return ci1._hi_val < ci2._hi_val;
}

template <typename IntType, typename T>
inline
bool operator < (const bugtest_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert((ci._mpz_val < it) == (ci._hi_val < it));
  return ci._hi_val < it;
}

template <typename IntType, typename T>
inline
bool operator < (T t, const bugtest_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert((it < ci._mpz_val) == (it < ci._hi_val));
  return it < ci._hi_val;
}

template <typename IntType>
inline
bool operator <= (const bugtest_integer<IntType> & ci1,
		  const bugtest_integer<IntType> & ci2)
{
  assert((ci1._mpz_val <= ci2._mpz_val) == (ci1._hi_val <= ci2._hi_val));
  return ci1._hi_val <= ci2._hi_val;
}

template <typename IntType, typename T>
inline
bool operator <= (const bugtest_integer<IntType> & ci, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert((ci._mpz_val <= it) == (ci._hi_val <= it));
  return ci._hi_val <= it;
}

template <typename IntType, typename T>
inline
bool operator <= (T t, const bugtest_integer<IntType> & ci)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert((it <= ci._mpz_val) == (it <= ci._hi_val));
  return it <= ci._hi_val;

}

template <typename IntType>
inline
bool operator > (const bugtest_integer<IntType> & ci1,
		 const bugtest_integer<IntType> & ci2)
{
  return ci2 < ci1;
}

template <typename IntType, typename T>
inline
bool operator > (const bugtest_integer<IntType> & ci, T t)
{
  return t < ci;
}

template <typename IntType, typename T>
inline
bool operator > (T t, const bugtest_integer<IntType> & ci)
{
  return ci < t;
}

template <typename IntType>
inline
bool operator >= (const bugtest_integer<IntType> & ci1,
		  const bugtest_integer<IntType> & ci2)
{
  return ci2 <= ci1;
}

template <typename IntType, typename T>
inline
bool operator >= (const bugtest_integer<IntType> & ci, T t)
{
  return t <= ci;
}

template <typename IntType, typename T>
inline
bool operator >= (T t, const bugtest_integer<IntType> & ci)
{
  return ci <= t;
}

template <typename IntType>
inline
bugtest_integer<IntType> abs(const bugtest_integer<IntType> & ci)
{
  return 0 <= ci ? ci : -ci;
}

template <typename IntType>
inline
void swap(bugtest_integer<IntType> & ci1, bugtest_integer<IntType> & ci2)
{
  ci1.swap(ci2);
}

template <typename IntType>
inline 
std::ostream & operator << (std::ostream & ostr, const bugtest_integer<IntType> & ci)
{
  ostr << ci._mpz_val;
  return ostr;
}


template <typename IntType>
class bugtest_rational {
public:
  mpq_class _mpq_val;
  hybrid_rational<IntType> _hr_val;
  using hardware_type = typename hybrid_rational<IntType>::hardware_type;
public:

  bugtest_rational(hardware_type n = 0, hardware_type d = 1)
    :_mpq_val(n, d), _hr_val(n, d)
  {
    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
  }

  bugtest_rational(const hardware_rational<hardware_type> & sr)
    :_mpq_val(sr.get_num(), sr.get_den()),
     _hr_val(sr)
  {
    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
  }
  
  bugtest_rational(const mpq_class & q)
    :_mpq_val(q), _hr_val(q)
  {
    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
  }

  bugtest_rational(const hybrid_rational<hardware_type> & hr)
    :_mpq_val((mpq_class)hr), _hr_val(hr)
  {
    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
  }
  
  bugtest_rational(const bugtest_integer<hardware_type> & n, const bugtest_integer<hardware_type> & d = 1)
    :_mpq_val(n._mpz_val, d._mpz_val), _hr_val(n._hi_val, d._hi_val)
  {
    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
  }

  bugtest_rational(const mpz_class & n, const mpz_class & d)
    :_mpq_val(n, d), _hr_val(n, d)
  {}
  
  explicit bugtest_rational(const std::string & num, const std::string & den = std::string("1"))
    : bugtest_rational(bugtest_integer<hardware_type>(num),
		       bugtest_integer<hardware_type>(den))
  {
    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
  }


  bugtest_rational & operator = (hardware_type t)
  {
    _mpq_val = t;
    _hr_val = t;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  bugtest_rational & operator = (const hardware_rational<hardware_type> & sr)
  {
    _mpq_val = mpq_class(sr.get_num(), sr.get_den());
    _hr_val = sr;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  bugtest_rational & operator = (const mpq_class & q)
  {
    _mpq_val = q;
    _hr_val = q;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);

    return *this;
  }

  bugtest_rational & operator = (const bugtest_integer<hardware_type> & ci)
  {
    _mpq_val = ci._mpz_val;
    _hr_val = ci._hi_val;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  bugtest_rational & operator = (const mpz_class & z)
  {
    _mpq_val = z;
    _hr_val = z;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }


  bugtest_rational & canonicalize()
  {
    _mpq_val.canonicalize();
    _hr_val.canonicalize();
    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    return *this;
  }

  
  const bugtest_integer<hardware_type> get_num() const
  {
    assert(hybrid_integer<hardware_type>(_mpq_val.get_num()) == _hr_val.get_num());
    return _hr_val.get_num();
  }

  const bugtest_integer<hardware_type> get_den() const
  {
    assert(hybrid_integer<hardware_type>(_mpq_val.get_den()) == _hr_val.get_den());
    return _hr_val.get_den();
  }

  bool is_gmp() const
  {
    return _hr_val.is_gmp();
  }

  
  std::string get_str() const
  {
    assert(_mpq_val.get_str() == _hr_val.get_str());
    return _hr_val.get_str();
  }
  
  void swap(bugtest_rational & cr)
  {
    _mpq_val.swap(cr._mpq_val);
    _hr_val.swap(cr._hr_val);
  }


  bugtest_rational & operator += (const bugtest_rational & cr)
  {
    std::cout << "BEFORE" << std::endl;
    std::cout << "MPQ: " << _mpq_val<< ", HR: " << _hr_val << std::endl;
    std::cout << "MPQ_ADD: " << cr._mpq_val << ", HR_ADD: " << cr._hr_val << std::endl;
    
    _mpq_val += cr._mpq_val;
    _hr_val += cr._hr_val;

    std::cout << "AFTER" << std::endl;
    std::cout << "MPQ: " << _mpq_val<< ", HR: " << _hr_val << std::endl;
    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  template <typename T>
  bugtest_rational & operator += (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    hardware_type it = (hardware_type)t;

    _mpq_val += it;
    _hr_val += it;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  bugtest_rational & operator += (const bugtest_integer<IntType> & ci)
  {    
    _mpq_val += ci._mpz_val;
    _hr_val += ci._hr_val;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  bugtest_rational & operator += (const hardware_rational<IntType> & sr)
  {
    _mpq_val += mpq_class(sr.get_num(), sr.get_den());
    _hr_val += sr;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }


  bugtest_rational & operator -= (const bugtest_rational & cr)
  {
    _mpq_val -= cr._mpq_val;
    _hr_val -= cr._hr_val;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  template <typename T>
  bugtest_rational & operator -= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    hardware_type it = (hardware_type)t;

    _mpq_val -= it;
    _hr_val -= it;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  bugtest_rational & operator -= (const bugtest_integer<IntType> & ci)
  {    
    _mpq_val -= ci._mpz_val;
    _hr_val -= ci._hr_val;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  bugtest_rational & operator -= (const hardware_rational<IntType> & sr)
  {
    _mpq_val -= mpq_class(sr.get_num(), sr.get_den());
    _hr_val -= sr;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }


  bugtest_rational & operator *= (const bugtest_rational & cr)
  {
    _mpq_val *= cr._mpq_val;
    _hr_val *= cr._hr_val;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  template <typename T>
  bugtest_rational & operator *= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    hardware_type it = (hardware_type)t;

    _mpq_val *= it;
    _hr_val *= it;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  bugtest_rational & operator *= (const bugtest_integer<IntType> & ci)
  {    
    _mpq_val *= ci._mpz_val;
    _hr_val *= ci._hr_val;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  bugtest_rational & operator *= (const hardware_rational<IntType> & sr)
  {
    _mpq_val *= mpq_class(sr.get_num(), sr.get_den());
    _hr_val *= sr;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  bugtest_rational & operator /= (const bugtest_rational & cr)
  {
    _mpq_val /= cr._mpq_val;
    _hr_val /= cr._hr_val;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  template <typename T>
  bugtest_rational & operator /= (T t)
  {
    static_assert(std::is_integral<T>::value, "T must be integral type");
    hardware_type it = (hardware_type)t;

    _mpq_val /= it;
    _hr_val /= it;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  bugtest_rational & operator /= (const bugtest_integer<IntType> & ci)
  {    
    _mpq_val /= ci._mpz_val;
    _hr_val /= ci._hr_val;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  bugtest_rational & operator /= (const hardware_rational<IntType> & sr)
  {
    _mpq_val /= mpq_class(sr.get_num(), sr.get_den());
    _hr_val /= sr;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  bugtest_rational & change_sign()
  {
    _mpq_val *= -1;
    _hr_val *= -1;

    assert(hybrid_rational<hardware_type>(_mpq_val) == _hr_val);
    
    return *this;
  }

  
  explicit operator bugtest_integer<IntType> () const
  {
    assert(hybrid_integer<IntType>((mpz_class)_mpq_val) == (hybrid_integer<IntType>)_hr_val);
    return (hybrid_integer<IntType>)_hr_val;    
  }

  explicit operator mpq_class () const
  {
    assert(_mpq_val == _hr_val.operator mpq_class ());
    return _hr_val.operator mpq_class ();
  }  
  
};



template <typename IntType>
inline
bugtest_rational<IntType> operator + (const bugtest_rational<IntType> & cr)
{
  return cr;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator - (const bugtest_rational<IntType> & cr)
{
  assert(hybrid_rational<IntType>(-cr._mpq_val) == -cr._hr_val);
  return -cr._hr_val;
}



template <typename IntType>
inline
bugtest_rational<IntType> operator + (const bugtest_rational<IntType> & cr1,
				      const bugtest_rational<IntType> & cr2)
{
  assert(hybrid_rational<IntType>(cr1._mpq_val + cr2._mpq_val) == (cr1._hr_val + cr2._hr_val)); 
  return cr1._hr_val + cr2._hr_val;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator + (const bugtest_rational<IntType> & cr1,
				      const bugtest_integer<IntType> & cr2)
{
  assert((cr1._mpq_val + cr2._mpz_val) == (cr1._hr_val + cr2._hi_val));
  return cr1._hr_val + cr2._hi_val;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator + (const bugtest_integer<IntType> & cr1,
				      const bugtest_rational<IntType> & cr2)
{
  assert((cr1._mpz_val + cr2._mpq_val) == (cr1._hi_val + cr2._hr_val));
  return cr1._hi_val + cr2._hr_val;
}

template <typename IntType, typename T>
inline
bugtest_rational<IntType> operator + (const bugtest_rational<IntType> & cr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(hybrid_rational<IntType>(cr._mpq_val + it) == cr._hr_val + it);
  return cr._hr_val + it;
}

template <typename IntType, typename T>
inline
bugtest_rational<IntType> operator + (T t, const bugtest_rational<IntType> & cr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(it + cr._mpq_val == it + cr._hr_val);
  return it + cr._hr_val;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator + (const bugtest_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  assert(cr._mpq_val + mpq_class(t.get_num(), t.get_den()) == cr._hr_val + t);
  return cr._hr_val + t;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator + (const hardware_rational<IntType> & t, const bugtest_rational<IntType> & cr)
{  
  assert(mpq_class(t.get_num(), t.get_den()) + cr._mpq_val == t + cr._hr_val);
  return t + cr._hr_val;
}



/////////////////////////////////////
template <typename IntType>
inline
bugtest_rational<IntType> operator - (const bugtest_rational<IntType> & cr1,
				      const bugtest_rational<IntType> & cr2)
{
  assert(hybrid_rational<IntType>(cr1._mpq_val - cr2._mpq_val) == (cr1._hr_val - cr2._hr_val)); 
  return cr1._hr_val - cr2._hr_val;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator - (const bugtest_rational<IntType> & cr1,
				      const bugtest_integer<IntType> & cr2)
{
  assert((cr1._mpq_val - cr2._mpz_val) == (cr1._hr_val - cr2._hi_val));
  return cr1._hr_val - cr2._hi_val;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator - (const bugtest_integer<IntType> & cr1,
				      const bugtest_rational<IntType> & cr2)
{
  assert((cr1._mpz_val - cr2._mpq_val) == (cr1._hi_val - cr2._hr_val));
  return cr1._hi_val - cr2._hr_val;
}

template <typename IntType, typename T>
inline
bugtest_rational<IntType> operator - (const bugtest_rational<IntType> & cr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(hybrid_rational<IntType>(cr._mpq_val - it) == cr._hr_val - it);
  return cr._hr_val - it;
}

template <typename IntType, typename T>
inline
bugtest_rational<IntType> operator - (T t, const bugtest_rational<IntType> & cr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(it - cr._mpq_val == it - cr._hr_val);
  return it - cr._hr_val;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator - (const bugtest_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  assert(cr._mpq_val - mpq_class(t.get_num(), t.get_den()) == cr._hr_val - t);
  return cr._hr_val - t;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator - (const hardware_rational<IntType> & t, const bugtest_rational<IntType> & cr)
{  
  assert(mpq_class(t.get_num(), t.get_den()) - cr._mpq_val == t - cr._hr_val);
  return t - cr._hr_val;
}


template <typename IntType>
inline
bugtest_rational<IntType> operator * (const bugtest_rational<IntType> & cr1,
				      const bugtest_rational<IntType> & cr2)
{
  assert(hybrid_rational<IntType>(cr1._mpq_val * cr2._mpq_val) == (cr1._hr_val * cr2._hr_val)); 
  return cr1._hr_val * cr2._hr_val;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator * (const bugtest_rational<IntType> & cr1,
				      const bugtest_integer<IntType> & cr2)
{
  assert((cr1._mpq_val * cr2._mpz_val) == (cr1._hr_val * cr2._hi_val));
  return cr1._hr_val * cr2._hi_val;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator * (const bugtest_integer<IntType> & cr1,
				      const bugtest_rational<IntType> & cr2)
{
  assert((cr1._mpz_val * cr2._mpq_val) == (cr1._hi_val * cr2._hr_val));
  return cr1._hi_val * cr2._hr_val;
}

template <typename IntType, typename T>
inline
bugtest_rational<IntType> operator * (const bugtest_rational<IntType> & cr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(cr._mpq_val * it == cr._hr_val * it);
  return cr._hr_val * it;
}

template <typename IntType, typename T>
inline
bugtest_rational<IntType> operator * (T t, const bugtest_rational<IntType> & cr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(it * cr._mpq_val == it * cr._hr_val);
  return it * cr._hr_val;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator * (const bugtest_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  assert(cr._mpq_val * mpq_class(t.get_num(), t.get_den()) == cr._hr_val * t);
  return cr._hr_val * t;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator * (const hardware_rational<IntType> & t, const bugtest_rational<IntType> & cr)
{  
  assert(mpq_class(t.get_num(), t.get_den()) * cr._mpq_val == t * cr._hr_val);
  return t * cr._hr_val;
}


template <typename IntType>
inline
bugtest_rational<IntType> operator / (const bugtest_rational<IntType> & cr1,
				      const bugtest_rational<IntType> & cr2)
{
  assert(hybrid_rational<IntType>(cr1._mpq_val / cr2._mpq_val) == (cr1._hr_val / cr2._hr_val)); 
  return cr1._hr_val / cr2._hr_val;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator / (const bugtest_rational<IntType> & cr1,
				      const bugtest_integer<IntType> & cr2)
{
  assert((cr1._mpq_val / cr2._mpz_val) == (cr1._hr_val / cr2._hi_val));
  return cr1._hr_val / cr2._hi_val;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator / (const bugtest_integer<IntType> & cr1,
				      const bugtest_rational<IntType> & cr2)
{
  assert((cr1._mpz_val / cr2._mpq_val) == (cr1._hi_val / cr2._hr_val));
  return cr1._hi_val / cr2._hr_val;
}

template <typename IntType, typename T>
inline
bugtest_rational<IntType> operator / (const bugtest_rational<IntType> & cr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(cr._mpq_val / it == cr._hr_val / it);
  return cr._hr_val / it;
}

template <typename IntType, typename T>
inline
bugtest_rational<IntType> operator / (T t, const bugtest_rational<IntType> & cr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert(hybrid_rational<IntType>(it / cr._mpq_val) == it / cr._hr_val);
  return it / cr._hr_val;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator / (const bugtest_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  assert(cr._mpq_val / mpq_class(t.get_num(), t.get_den()) == cr._hr_val / t);
  return cr._hr_val / t;
}

template <typename IntType>
inline
bugtest_rational<IntType> operator / (const hardware_rational<IntType> & t, const bugtest_rational<IntType> & cr)
{  
  assert(mpq_class(t.get_num(), t.get_den()) / cr._mpq_val == t / cr._hr_val);
  return t / cr._hr_val;
}


//////////////////////////////////////
template <typename IntType>
inline
bool operator == (const bugtest_rational<IntType> & cr1,
				       const bugtest_rational<IntType> & cr2)
{
  assert((cr1._mpq_val == cr2._mpq_val) == (cr1._hr_val == cr2._hr_val)); 
  return cr1._hr_val == cr2._hr_val;
}

template <typename IntType>
inline
bool operator == (const bugtest_rational<IntType> & cr1,
				       const bugtest_integer<IntType> & cr2)
{
  assert((cr1._mpq_val == cr2._mpz_val) == (cr1._hr_val == cr2._hi_val));
  return cr1._hr_val == cr2._hi_val;
}

template <typename IntType>
inline
bool operator == (const bugtest_integer<IntType> & cr1,
				      const bugtest_rational<IntType> & cr2)
{
  assert((cr1._mpz_val == cr2._mpq_val) == (cr1._hi_val == cr2._hr_val));
  return cr1._hi_val == cr2._hr_val;
}

template <typename IntType, typename T>
inline
bool operator == (const bugtest_rational<IntType> & cr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert((cr._mpq_val == it) == (cr._hr_val == it));
  return cr._hr_val == it;
}

template <typename IntType, typename T>
inline
bool operator == (T t, const bugtest_rational<IntType> & cr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert((it == cr._mpq_val) == (it == cr._hr_val));
  return it == cr._hr_val;
}

template <typename IntType>
inline
bool operator == (const bugtest_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  assert((cr._mpq_val == mpq_class(t.get_num(), t.get_den())) == (cr._hr_val == t));
  return cr._hr_val == t;
}

template <typename IntType>
inline
bool operator == (const hardware_rational<IntType> & t, const bugtest_rational<IntType> & cr)
{  
  assert((mpq_class(t.get_num(), t.get_den()) == cr._mpq_val) == (t == cr._hr_val));
  return t == cr._hr_val;
}


template <typename IntType>
inline
bool operator < (const bugtest_rational<IntType> & cr1,
				       const bugtest_rational<IntType> & cr2)
{
  assert((cr1._mpq_val < cr2._mpq_val) == (cr1._hr_val < cr2._hr_val)); 
  return cr1._hr_val < cr2._hr_val;
}

template <typename IntType>
inline
bool operator < (const bugtest_rational<IntType> & cr1,
				       const bugtest_integer<IntType> & cr2)
{
  assert((cr1._mpq_val < cr2._mpz_val) == (cr1._hr_val < cr2._hi_val));
  return cr1._hr_val < cr2._hi_val;
}

template <typename IntType>
inline
bool operator < (const bugtest_integer<IntType> & cr1,
				      const bugtest_rational<IntType> & cr2)
{
  assert((cr1._mpz_val < cr2._mpq_val) == (cr1._hi_val < cr2._hr_val));
  return cr1._hi_val < cr2._hr_val;
}

template <typename IntType, typename T>
inline
bool operator < (const bugtest_rational<IntType> & cr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert((cr._mpq_val < it) == (cr._hr_val < it));
  return cr._hr_val < it;
}

template <typename IntType, typename T>
inline
bool operator < (T t, const bugtest_rational<IntType> & cr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert((it < cr._mpq_val) == (it < cr._hr_val));
  return it < cr._hr_val;
}

template <typename IntType>
inline
bool operator < (const bugtest_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  assert((cr._mpq_val < mpq_class(t.get_num(), t.get_den())) == (cr._hr_val < t));
  return cr._hr_val < t;
}

template <typename IntType>
inline
bool operator < (const hardware_rational<IntType> & t, const bugtest_rational<IntType> & cr)
{  
  assert((mpq_class(t.get_num(), t.get_den()) < cr._mpq_val) == (t < cr._hr_val));
  return t < cr._hr_val;
}

//////////////////////
template <typename IntType>
inline
bool operator <= (const bugtest_rational<IntType> & cr1,
				       const bugtest_rational<IntType> & cr2)
{
  assert((cr1._mpq_val <= cr2._mpq_val) == (cr1._hr_val <= cr2._hr_val)); 
  return cr1._hr_val <= cr2._hr_val;
}

template <typename IntType>
inline
bool operator <= (const bugtest_rational<IntType> & cr1,
				       const bugtest_integer<IntType> & cr2)
{
  assert((cr1._mpq_val <= cr2._mpz_val) == (cr1._hr_val <= cr2._hi_val));
  return cr1._hr_val <= cr2._hi_val;
}

template <typename IntType>
inline
bool operator <= (const bugtest_integer<IntType> & cr1,
				      const bugtest_rational<IntType> & cr2)
{
  assert((cr1._mpz_val <= cr2._mpq_val) == (cr1._hi_val <= cr2._hr_val));
  return cr1._hi_val <= cr2._hr_val;
}

template <typename IntType, typename T>
inline
bool operator <= (const bugtest_rational<IntType> & cr, T t)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert((cr._mpq_val <= it) == (cr._hr_val <= it));
  return cr._hr_val <= it;
}

template <typename IntType, typename T>
inline
bool operator <= (T t, const bugtest_rational<IntType> & cr)
{
  static_assert(std::is_integral<T>::value, "T must be integral type");
  IntType it = (IntType)t;
  assert((it <= cr._mpq_val) == (it <= cr._hr_val));
  return it <= cr._hr_val;
}

template <typename IntType>
inline
bool operator <= (const bugtest_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  assert((cr._mpq_val <= mpq_class(t.get_num(), t.get_den())) == (cr._hr_val <= t));
  return cr._hr_val <= t;
}

template <typename IntType>
inline
bool operator <= (const hardware_rational<IntType> & t, const bugtest_rational<IntType> & cr)
{  
  assert((mpq_class(t.get_num(), t.get_den()) <= cr._mpq_val) == (t <= cr._hr_val));
  return t <= cr._hr_val;
}


////////////////////
template <typename IntType>
inline
bool operator != (const bugtest_rational<IntType> & cr1,
		  const bugtest_rational<IntType> & cr2)
{
  return !(cr1 == cr2);
}

template <typename IntType>
inline
bool operator != (const bugtest_rational<IntType> & cr1,
		  const bugtest_integer<IntType> & cr2)
{
  return !(cr1 == cr2);
}

template <typename IntType>
inline
bool operator != (const bugtest_integer<IntType> & cr1,
		  const bugtest_rational<IntType> & cr2)
{
  return !(cr1 == cr2);
}

template <typename IntType, typename T>
inline
bool operator != (const bugtest_rational<IntType> & cr, T t)
{
  return !(cr == t);
}

template <typename IntType, typename T>
inline
bool operator != (T t, const bugtest_rational<IntType> & cr)
{
  return !(t == cr);
}

template <typename IntType>
inline
bool operator != (const bugtest_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  return !(cr == t);
}

template <typename IntType>
inline
bool operator != (const hardware_rational<IntType> & t, const bugtest_rational<IntType> & cr)
{  
  return !(t == cr);
}

template <typename IntType>
inline
bool operator > (const bugtest_rational<IntType> & cr1,
		 const bugtest_rational<IntType> & cr2)
{
  return cr2 < cr1;
}

template <typename IntType>
inline
bool operator > (const bugtest_rational<IntType> & cr1,
		 const bugtest_integer<IntType> & cr2)
{
  return cr2 < cr1;
}

template <typename IntType>
inline
bool operator > (const bugtest_integer<IntType> & cr1,
		 const bugtest_rational<IntType> & cr2)
{
  return cr2 < cr1;
}

template <typename IntType, typename T>
inline
bool operator > (const bugtest_rational<IntType> & cr, T t)
{
  return t < cr;
}

template <typename IntType, typename T>
inline
bool operator > (T t, const bugtest_rational<IntType> & cr)
{
  return cr < t;
}

template <typename IntType>
inline
bool operator > (const bugtest_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  return t < cr;
}

template <typename IntType>
inline
bool operator > (const hardware_rational<IntType> & t, const bugtest_rational<IntType> & cr)
{  
  return cr < t;
}


template <typename IntType>
inline
bool operator >= (const bugtest_rational<IntType> & cr1,
		  const bugtest_rational<IntType> & cr2)
{
  return cr2 <= cr1;
}

template <typename IntType>
inline
bool operator >= (const bugtest_rational<IntType> & cr1,
		  const bugtest_integer<IntType> & cr2)
{
  return cr2 <= cr1;
}

template <typename IntType>
inline
bool operator >= (const bugtest_integer<IntType> & cr1,
		  const bugtest_rational<IntType> & cr2)
{
  return cr2 <= cr1;
}

template <typename IntType, typename T>
inline
bool operator >= (const bugtest_rational<IntType> & cr, T t)
{
  return t <= cr;
}

template <typename IntType, typename T>
inline
bool operator >= (T t, const bugtest_rational<IntType> & cr)
{
  return cr <= t;
}

template <typename IntType>
inline
bool operator >= (const bugtest_rational<IntType> & cr, const hardware_rational<IntType> & t)
{  
  return t <= cr;
}

template <typename IntType>
inline
bool operator >= (const hardware_rational<IntType> & t, const bugtest_rational<IntType> & cr)
{  
  return cr <= t;
}


template <typename IntType>
inline
bugtest_rational<IntType> abs(const bugtest_rational<IntType> & cr)
{
  return cr >= 0 ? cr : -cr;
}

template <typename IntType>
inline
void swap(bugtest_rational<IntType> & cr1,
	  bugtest_rational<IntType> & cr2)
{
  cr1.swap(cr2);
}

template <typename IntType>
inline
std::ostream & operator << (std::ostream & ostr, const bugtest_rational<IntType> & cr)
{
  ostr << cr._hr_val;
  return ostr;
}


#endif // _BUGTEST_H
