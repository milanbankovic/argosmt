/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2015 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _BASIC_TYPES_H
#define _BASIC_TYPES_H

#include <memory>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <cstdlib>
#include <iomanip>

#include "common.hpp"
#include "object_factory.hpp"
#include "vector_hash_code.hpp"

#ifdef _USE_GMP
#include <gmpxx.h>
#endif 

template <typename T>
inline
std::string to_string(T * t)
{
  std::stringstream sstr;
  t->out(sstr);
  return sstr.str();
}
  
/** Base class for all exception classes. */
  
class base_exception {
private:
  std::string _description;

public:
  /** Constructor that initializes text description of the 
      exception with desc. */
  base_exception(const std::string & desc)
    :_description(desc)
  {}
  
  /** Returns the description of the exception. */
  const std::string & get_description() const
  {
    return _description;
  }
};

/** Represents lexical elements defined by the standard as 
    "special constants" -- numerals, hexadecimals, decimals,
    binaries, string literals. All special constants are internally 
    represented as strings.
*/
class special_constant {
public:
  /** Defines posible values for the type of a constant. 
      This value must be supplied during the constraction of a special 
      constant, and is used to distinguish lexically different constants 
      (e.g. numerals from binaries).
  */
  enum type { 
    /** numeral constant (e.g. 593) */ 
    T_NUMERAL, 
    /** hexadecimal constant (e.g. #xaf4c) */
    T_HEXADECIMAL,
    /** binary constant (e.g. #b0110) */
    T_BINARY, 
    /** decimal constant (e.g. 12.54) */ 
    T_DECIMAL, 
    /** string literal constant (e.g. "some text") */
    T_STRING_LITERAL, 
    /** undefined constant type (for UNDEFINED constant) */ 
    T_UNDEF 
  };
    
  /** Static value that represents undefined special constant. */
  const static special_constant UNDEFINED;
    
private:
  struct data : public std::enable_shared_from_this<data> {
    type _type;
    std::string _value;    
    std::size_t _hash_code;

    data(type t, std::string && v)
      :_type(t),
       _value(std::move(v)),
       _hash_code(0)
    {}

    
    bool operator == (const data & d) const
    {
      return _type == d._type && _value == d._value;
    }
      
    void calculate_hash()
    {
      _hash_code = std::hash<std::string>()(_value); 
    }

    void out(std::ostream & ostr) const
    {
      ostr << _value << std::endl;
    }
      
  };

  static object_factory<data> * _factory;
  static object_factory<data> & get_factory()
  {
    if(_factory == 0)
      _factory = new object_factory<data>();
    return *_factory;
  }
  std::shared_ptr<const data> _data;
public:
  /** Constructor that initializes the constant of the given type with
      the given value. The value should be in the form that is appropriate
      for the given type of the constant. For instance, a binary 
      constant should be prefixed with #b and consist of 0's and 1's,
      and a string literal constant should be double-quoted. */
  special_constant(type tp, const std::string & value);

  /** Constructor that initializes the constant of the given type with
      the given value. The value should be in the form that is appropriate
      for the given type of the constant. For instance, a binary 
      constant should be prefixed with #b and consist of 0's and 1's,
      and a string literal constant should be double-quoted. */  
  special_constant(type tp, std::string && value);

  /** Constructor that initializes the constant of the given type with
      the given value. The value should be in the form that is appropriate
      for the given type of the constant. For instance, a binary 
      constant should be prefixed with #b and consist of 0's and 1's,
      and a string literal constant should be double-quoted. */
  special_constant(type tp, const char * value);

  /** Constructor that initializes the constant with the given 
      unsigned long value. The type argument must be set to 
      T_NUMERAL, T_BINARY or T_HEXADECIMAL. Otherwise, 
      an exception is thrown. The value of 
      the constant will be the string representation of the given value in the
      corresponding base (10, 2, or 16, respectively), with the appropriate
      prefix (#b for binary, #x for hexadecimal). */
  special_constant(type tp, unsigned long value);

  /** Constructor that initializes the constant with the given 
      unsigned  value. The type argument must be set to 
      T_NUMERAL, T_BINARY or T_HEXADECIMAL. Otherwise, 
      an exception is thrown. The value of 
      the constant will be the string representation of the given value in the
      corresponding base (10, 2, or 16, respectively), with the appropriate
      prefix (#b for binary, #x for hexadecimal). */
  special_constant(type tp, unsigned value);

  /** Constructor that initializes the constant with the given 
      int value. It is assumed that the value is non-negative (otherwise, the sign
      is ignored). The type argument must be set to T_NUMERAL, T_BINARY or T_HEXADECIMAL. 
      Otherwise, an exception is thrown. The value of 
      the constant will be the string representation of the given value in the
      corresponding base (10, 2, or 16, respectively), with the appropriate
      prefix (#b for binary, #x for hexadecimal). */
  special_constant(type tp, int value);

  
  /** Constructor that initializes the constant with the given long double
      value. The type argument must be set to T_DECIMAL. Otherwise, 
      an exception is thrown. The value of the
      constant will be the string representation of the given value in
      the usual fixed decimal format, with at most 'precision' digits after
      the decimal point. */
  special_constant(type tp, long double value, 
		   unsigned precision = MAX_SC_PREC);

  /** Constructor that initializes the constant with the given double
      value. The type argument must be set to T_DECIMAL. Otherwise, 
      an exception is thrown. The value of the
      constant will be the string representation of the given value in
      the usual fixed decimal format, with at most 'precision' digits after
      the decimal point. */
  special_constant(type tp, double value, 
		   unsigned precision = MAX_SC_PREC);
  
#ifdef _USE_GMP    
  /** Constructor that initializes the constant with the value of 
      GMP mpz_class object. The type argument must be set to 
      T_NUMERAL, T_BINARY or T_HEXADECIMAL. Otherwise, 
      an exception is thrown. The value of the
      constant will be the string representation of the given value in the
      corresponding base (10, 2, or 16, respectively), with the appropriate
      prefix (#b for binary, #x for hexadecimal). */
  special_constant(type tp, const mpz_class & value);
  
  /** Constructor that initializes the constant with the value of GMP
      mpf_class object. The type argument must be set to T_DECIMAL. 
      Otherwise, an exception is thrown. 
      The value of the constant will be the string representation of the 
      given value in the usual fixed decimal format, with at most 
      'precision' digits after the decimal point. */
  special_constant(type tp, const mpf_class & value, 
		   unsigned precision = MAX_SC_PREC);
  
  /** Constructor that initializes the constant with the value of GMP
      mpq_class object. The type argument must be set to T_DECIMAL or
      some of T_NUMERAL, T_BINARY, T_HEXADECIMAL (in which case, 
      denominator of the given rational must be 1). Otherwise, 
      an exception is thrown.
      The value of the constant will be the string representation of the
      given value in the usual fixed decimal point, with at most 'precision'
      digits afger the decimal point, or in appropriate integer format. */
  special_constant(type tp, const mpq_class & value,
		   unsigned precision = MAX_SC_PREC);
  
#endif
    
  /** Gets the type of the constant. */
  type get_type() const;

  /** Returns true if the constant is of T_NUMERAL type. */
  bool is_numeral() const;

  /** Returns true if the constant is of T_HEXADECIMAL type. */
  bool is_hexadecimal() const;

  /** Returns true if the constant is of T_BINARY type. */
  bool is_binary() const;

  /** Returns true if the constant is of T_DECIMAL type. */
  bool is_decimal() const;

  /** Returns true if the constant is of T_STRING_LITERAL type. */
  bool is_string_literal() const;

  /** Gets the value of the constant. */
  const std::string & get_value() const;

  /** Returns an unsigned long value of the constant. The constant
      _must_ be of T_NUMERAL or T_HEXADECIMAL or T_BINARY type. 
      Otherwise, an exception
      is thrown. Notice that the value of the constant may not fit into the
      unsigned long data type. */
  unsigned long get_ul_value() const;

  /** Returns an unsigned value of the constant. The constant
      _must_ be of T_NUMERAL or T_HEXADECIMAL or T_BINARY type. 
      Otherwise, an exception
      is thrown. Notice that the value of the constant may not fit into the
      unsigned data type. */
  unsigned get_u_value() const;

  /** Returns an int value of the constant. The value is always non-negative.
      The constant _must_ be of T_NUMERAL or T_HEXADECIMAL or T_BINARY type. 
      Otherwise, an exception
      is thrown. Notice that the value of the constant may not fit into the
      int data type. */
  int get_i_value() const;

  /** Returns a long double value of the constant. The constant
      _must_ be of T_DECIMAL type. Otherwise, an exception
      is thrown. Notice that the value of the constant may not fit into the
      long double data type. */
  long double get_ld_value() const;

  /** Returns a double value of the constant. The constant
      _must_ be of T_DECIMAL type. Otherwise, an exception
      is thrown. Notice that the value of the constant may not fit into the
       double data type. */
  double get_d_value() const;

  
#ifdef _USE_GMP    
  /** Returns GMP mpz_class value of the constant. 
      The constant _must_ be of T_NUMERAL or T_HEXADECIMAL or T_BINARY type. 
      Otherwise, an exception is thrown. */
  mpz_class get_gmpz_value() const;

  /** Returns GMP mpf_class value of the constant. 
      The constant _must_ be of T_DECIMAL type. Otherwise, 
      an exception is thrown. If 'precision' 
      is not 0, then the resulting mpf_class object's precision (in bits)
      is set to that value, otherwise, current default GMP 
      precision is used. Notice that if the precision is
      not sufficient, this could cause the loss of data
      when converting to mpf_class. */
  mpf_class get_gmpf_value(unsigned precision = 0) const;

  /** Returns GMP mpq_class value of the constant. The constant _must_ be of
      T_DECIMAL, T_NUMERAL, T_BINARY or T_HEXADECIMAL type. Otherwise, 
      an exception is thrown. The returned rational 
      number is cannonized. */
  mpq_class get_gmpq_value() const;


#endif 
    
  /** Returns true if c is equal to this constant. */
  bool operator == (const special_constant & c) const;

  /** Returns true if c is not equal to this constant. */
  bool operator != (const special_constant & c) const;

  /** Returns true if this constant is lexicographically lower than c. */
  bool operator < (const special_constant & c) const;

  /** Returns true if this constant is lexicographically greater than c. */
  bool operator > (const special_constant & c) const;

  /** Returns true if this constant is lexicographically lower or
      equal to c. */
  bool operator <= (const special_constant & c) const;

  /** Returns true if this constant is lexicographically greater or
      equal to c. */
  bool operator >= (const special_constant & c) const;

  void out(std::ostream & ostr) const;
  std::string to_string() const;    
  std::size_t hash_code() const;
};
  


namespace std {
    template<>
    struct hash<special_constant> : 
      public unary_function<special_constant, std::size_t> {
      std::size_t operator ()(const special_constant & c) const
      {
	return c.hash_code();
      }
    };
}


/** Prints the special constant to the output stream. */
std::ostream & operator << (std::ostream & ostr, 
			    const special_constant & constant);

inline
std::string to_string(special_constant::type tp)
{
  switch(tp)
    {
    case special_constant::T_NUMERAL:
      return std::string("NUMERAL");
      break;
    case special_constant::T_BINARY:
      return std::string("BINARY");
      break;
    case special_constant::T_HEXADECIMAL:
      return std::string("HEXADECIMAL");
      break;
    case special_constant::T_DECIMAL:
      return std::string("DECIMAL");
      break;
    case special_constant::T_STRING_LITERAL:
      return std::string("STRING");
      break;
    default:
      return std::string("UNDEF");
      break;
    }  
}
  
/** Type of the exception thrown when a special constant is of one type, 
    and the user requests the value of another type. */
class bad_special_constant_type_exception : public base_exception {
private:
  special_constant _spec_const;    
public:
  /** Constructor. */
  bad_special_constant_type_exception(const std::string & message,
				      const special_constant & spec_const)
    :base_exception(message + std::string(": bad special constant type: ") +
		    spec_const.to_string() + " of type: " + 
		    to_string(spec_const.get_type()) + 
		    std::string(" (different type of value requested)")),
     _spec_const(spec_const)
  {}

  /** Gets constant. */
  const special_constant & get_special_constant() const
  {
    return _spec_const;
  }
};
  
/** Type of exception thrown when a special constant is initialized
    with a value of wrong type. */
class special_constant_initialization_exception : public base_exception {
private:
  special_constant::type _type;
public:
  /** Constructor */
  special_constant_initialization_exception(const std::string & message,
					    special_constant::type tp)
    :base_exception(message + 
		    std::string(": bad special constant initialization (type ")
		    + to_string(tp) + ")"),
     _type(tp)
  {}
    
  /** Gets the (wrong) type of the constant. */
  special_constant::type get_type() const
  {
    return _type;
  }    
};
  

class identifier;

/** Represents symbol lexical category. It serves as a base
    class for classes Variable and SortParameter. Internally, it is
    represented as string. */
class symbol {
  friend class identifier;
protected:
  struct data : public std::enable_shared_from_this<data> {
    std::string _name;
    std::size_t _hash_code;

    data(std::string && name)
      :_name(std::move(name)),
       _hash_code(0)
    {}
    
    bool operator == (const data & d) const
    {
      return _name == d._name;
    }

    void calculate_hash()
    {
      _hash_code = std::hash<std::string>() (_name);
    }

    void out(std::ostream & ostr) const
    {
      ostr << _name << std::endl;
    }
  };

  static object_factory<data> * _factory;

  static object_factory<data> & get_factory()
  {
    if(_factory == 0)
      _factory = new object_factory<data>();
    return *_factory;
  }

  std::shared_ptr<const data> _data;

  /** Constructor that creates uninitialized symbol */
  symbol();    
  
public:
  
  /** Constructor that initializes the symbol with the given string. */
  symbol(const std::string & name);

  /** Constructor that initializes the symbol with the given string. */
  symbol(std::string && name);
  
  /** Constructor that initializes the symbol with the given string. */
  symbol(const char * name);
    
  /** Returns the string representation of the symbol. */
  const std::string & get_name() const;
    
  /** Returns true if s is equal to this symbol. */
  bool operator == (const symbol & s) const;

  /** Returns true if s is not equal to this symbol. */
  bool operator != (const symbol & s) const;

  /** Returns if this symbol is lexicographically lower than s. */
  bool operator < (const symbol & s) const;

  /** Returns if this symbol is lexicographically greater than s. */
  bool operator > (const symbol & s) const;

  /** Returns if this symbol is lexicographically lower or equal to s. */
  bool operator <= (const symbol & s) const;

  /** Returns if this symbol is lexicographically greater or equal to s. */
  bool operator >= (const symbol & s) const;
    
  std::size_t hash_code() const;
  void out(std::ostream & ostr) const;
  std::string to_string() const;
};
  

namespace std {
    template<>
    struct hash<symbol> : 
      public unary_function<symbol, std::size_t> {
      std::size_t operator ()(const symbol & s) const
      {
	return s.hash_code();
      }
    };
}

  
/** Prints the symbol to the output stream. */
std::ostream & operator << (std::ostream & ostr, const symbol & s);
  

/** Represents keyword lexical category. It is internally
    represented as string. It is used for attribute names. */
class keyword {
private:
  struct data : public std::enable_shared_from_this<data> {
    std::string _name;
    std::size_t _hash_code;

    data(std::string && name)
      :_name(std::move(name)),
       _hash_code(0)
    {}
    
    bool operator == (const data & d) const
    {
      return _name == d._name;
    }

    void calculate_hash()
    {
      _hash_code = std::hash<std::string>() (_name);
    }

    void out(std::ostream & ostr) const
    {
      ostr << _name << std::endl;
    }
  };
    
  static object_factory<data> * _factory;
    
  static object_factory<data> & get_factory()
  {
    if(_factory == 0)
      _factory = new object_factory<data>();
    return *_factory;
  }
    
  std::shared_ptr<const data> _data;

public:

  /** :left-assoc keyword. */
  static const keyword LEFT_ASSOC;

  /** :right-assoc keyword. */
  static const keyword RIGHT_ASSOC;

  /** :chainable keyword. */
  static const keyword CHAINABLE;

  /** :pairwise keyword. */
  static const keyword PAIRWISE;

  /** :named keyword. */
  static const keyword NAMED;

  /** :pattern keyword. */
  static const keyword PATTERN;

  /** :definition keyword. */
  static const keyword DEFINITION;

  /** :status keyword. */
  static const keyword STATUS;

  /** :all-statistics keyword. */
  static const keyword ALL_STATISTICS;

  /** :print-success keyword. */
  static const keyword PRINT_SUCCESS;

  /** :expand-definitions keyword. */
  static const keyword EXPAND_DEFINITIONS;

  /** :interactive-mode keyword. */
  static const keyword INTERACTIVE_MODE;

  /** :produce-assertions keyword */
  static const keyword PRODUCE_ASSERTIONS;
  
  /** :produce-proofs keyword. */
  static const keyword PRODUCE_PROOFS;

  /** :produce-unsat-cores keyword. */
  static const keyword PRODUCE_UNSAT_CORES;

  /** :produce-models keyword. */
  static const keyword PRODUCE_MODELS;

  /** :produce-assignments keyword. */
  static const keyword PRODUCE_ASSIGNMENTS;

  /** :global-declarations keyword */
  static const keyword GLOBAL_DECLARATIONS;
  
  /** :syntax-checking keyword. */
  static const keyword SYNTAX_CHECKING;

  /** :logic-syntax-checking keyword. */
  static const keyword LOGIC_SYNTAX_CHECKING;

  /** :regular-output-channel keyword. */
  static const keyword REGULAR_OUTPUT_CHANNEL;

  /** :diagnostic-output-channel keyword. */
  static const keyword DIAGNOSTIC_OUTPUT_CHANNEL;

  /** :random-seed keyword. */
  static const keyword RANDOM_SEED;

  /** :verbosity keyword. */
  static const keyword VERBOSITY;

  /** :error-behavior keyword. */
  static const keyword ERROR_BEHAVIOR;

  /** :reason-unknown keyword. */
  static const keyword REASON_UNKNOWN;


  /** Constructor that initializes the keyword with the given string. */
  keyword(const std::string & name);

  /** Constructor that initializes the keyword with the given string. */
  keyword(std::string && name);
  
  /** Constructor that initializes the keyword with the given string. */
  keyword(const char * name);
    
  /** Returns the string representation of the keyword. */
  const std::string & get_name() const;

  /** Returns true if k is equal to this keyword. */
  bool operator == (const keyword & k) const;

  /** Returns true if k is not equal to this keyword. */
  bool operator != (const keyword & k) const;

  /** Returns if this keyword is lexicographically lower than k. */
  bool operator < (const keyword & k) const;

  /** Returns if this keyword is lexicographically greater than k. */
  bool operator > (const keyword & k) const;

  /** Returns if this keyword is lexicographically lower or equal to k. */
  bool operator <= (const keyword & k) const;

  /** Returns if this keyword is lexicographically greater or equal to k. */
  bool operator >= (const keyword & k) const;

  std::size_t hash_code() const;
  void out(std::ostream & ostr) const;
  std::string to_string() const;
};
  

namespace std {    
    template<>
    struct hash<keyword> : 
      public unary_function<keyword, std::size_t> {
      std::size_t operator ()(const keyword & k) const
      {
	return k.hash_code();
      }
    };
}

  
/** Prints the keyword to the output stream. */
std::ostream & operator << (std::ostream & ostr, const keyword & k);
  

/** Represents identifiers. Each identifier is either a symbol, or an 
    indexed identifier. The symbol part of the identifier
    is represented as a string, and optional indices are represented as
    a vector of unsigned numbers. */
class identifier {
public:

  class index {
  public:
    enum type { UNSIGNED = 0, SYMBOL = 1, UNSPECIFIED = 2 } ;
  private:
    type _tp;
    unsigned _u_val;
    symbol _s_val;
    std::size_t _hash_code;

    void calculate_hash() 
    {
      _hash_code = _tp == UNSIGNED ? std::hash<unsigned>()(_u_val) :
	(_tp == SYMBOL ? _s_val.hash_code() : 0); 
    }
    
  public:
    index(unsigned u_val)
      :_tp(UNSIGNED),
       _u_val(u_val),
       _s_val()
    {
      calculate_hash();
    }
    
    index(const symbol & s)
      :_tp(SYMBOL),
       _u_val((unsigned)(-1)),
       _s_val(s)
    {
      calculate_hash();
    }
    
    index(symbol && s)
      :_tp(SYMBOL),
       _u_val((unsigned)(-1)),
       _s_val(std::move(s))
    {
      calculate_hash();
    }


    index()
      :_tp(UNSPECIFIED),
       _u_val((unsigned)(-1)),
       _s_val()
    {
      calculate_hash();
    }


    type get_type() const
    {
      return _tp;
    }
    
    bool is_unsigned() const
    {
      return _tp == UNSIGNED;
    }

    bool is_symbol() const
    {
      return _tp == SYMBOL;
    }

    bool is_unspecified() const
    {
      return _tp == UNSPECIFIED;
    }
    
    unsigned get_unsigned_value() const; // defined below

    const symbol & get_symbol_value() const; // defined below

    bool operator == (const index & ind) const
    {
      return _tp == ind._tp && _u_val == ind._u_val && _s_val == ind._s_val;
    }

    bool operator != (const index & ind) const
    {
      return !(*this == ind);
    }

    bool operator < (const index & ind) const
    {
      return _tp < ind._tp ||
		   (_tp == UNSIGNED && ind._tp == UNSIGNED && _u_val < ind._u_val) ||
	(_tp == SYMBOL && ind._tp == SYMBOL && _s_val < ind._s_val);
    }

    bool operator <= (const index & ind) const
    {
      return _tp < ind._tp ||
		   (_tp == UNSIGNED && ind._tp == UNSIGNED && _u_val <= ind._u_val) ||
	(_tp == SYMBOL && ind._tp == SYMBOL && _s_val <= ind._s_val) ||
	(_tp == UNSPECIFIED && ind._tp == UNSPECIFIED);
    }

    bool operator > (const index & ind) const
    {
      return ind < *this;
    }

    bool operator >= (const index & ind) const
    {
      return ind <= *this;
    }

    void out(std::ostream & ostr) const
    {
      if(_tp == UNSIGNED)
	ostr << _u_val;
      else if(_tp == SYMBOL)
	ostr << _s_val;
      else
	ostr << "*";
    }

    std::size_t hash_code() const
    {
      return _hash_code;
    }

    std::string to_string() const
    {
      return ::to_string(this);
    }
    
  };
  
  /** Type representing vector of indices. */
  typedef std::vector<index> index_vector; 
    
  /** Unspecified index */
  static const index UNSPECIFIED_INDEX;
    
  /** Empty vector of indices (for those identifiers that are not indexed). */
  static const index_vector EMPTY_INDEX_VECTOR;
    
private:

  static index_vector _dummy_index_vector;
  
  struct data : public std::enable_shared_from_this<data> {
    std::string _name;
    std::shared_ptr<const index_vector> _indices; 
    bool _is_pattern;
    std::size_t _hash_code;

    data(std::string && name, std::shared_ptr<const index_vector> && indices)
      :_name(std::move(name)),
       _indices(std::move(indices)),
       _is_pattern(false),
       _hash_code(0)
    {}
    
    
    bool operator == (const data & d) const
    {
      return _name == d._name && 
	(_indices == d._indices || *_indices == *d._indices);
    }
    
    void check_pattern()
    {
      _is_pattern = false;
      for(index_vector::const_iterator it = _indices->begin(), 
	    it_end = _indices->end(); it != it_end; ++it)
	{
	  if(*it == UNSPECIFIED_INDEX)
	    {
	      _is_pattern = true;
	      break;
	    }
	}
    }

    void calculate_hash()
    {
      _hash_code = std::hash<std::string>() (_name);
      if(!_indices->empty())
	_hash_code += vector_hash_code<index>() (*_indices);
    }

    void out(std::ostream & ostr) const
    {
      ostr << _name << std::endl;
    }
  };    
    
  struct no_op {
    void operator () (const index_vector * iv) const
    {}
  };

  static object_factory<data> * _factory;

  static object_factory<data> & get_factory()
  {
    if(_factory == 0)
      _factory = new object_factory<data>();
    return *_factory;
  }

  std::shared_ptr<const data> _data;

public:
  /** Constructor that initializes the identifier with a given string, and,
      optionally, with a vector of indices. */
  identifier(const std::string & name, const index_vector & indices = EMPTY_INDEX_VECTOR);

  /** Constructor that initializes the identifier with a given string
      with a vector of indices. */
  identifier(std::string && name, index_vector && indices);

  /** Constructor that initializes the identifier with a given string, and,
      optionally, with a vector of indices. */
  identifier(std::string && name, const index_vector & indices = EMPTY_INDEX_VECTOR);
  
  /** Constructor that initializes the identifier with a given string, and
      with a vector of indices. */
  identifier(const std::string & name, index_vector && indices);

  /** Constructor that initializes the identifier with a given string, and,
      optionally, with a vector of indices. */
  identifier(const char * name, const index_vector & indices = EMPTY_INDEX_VECTOR);

  /** Constructor that initializes the identifier with a given string, and,
      optionally, with a vector of indices. */
  identifier(const char * name, index_vector && indices);
    
  /** Gets the name of the identifier. */
  const std::string & get_name() const;

  /** Gets the vector of indices of the identifier (or empty vector, if 
      the identifier has no indices). */
  const index_vector & get_indices() const;
    
  /** Returns true if the identifier has indices. */
  bool has_indices() const;

  /** Returns true if the identifier is a pattern identifier. A pattern
      identifier is an identifier that has at least one unspecified index.
      Pattern identifiers represent wildcards (for instance,
      identifier ( _ BitVec *) may represent all indexed identifiers named
      'BitVec', where * is replaced with an arbitrary numeral, and 
      (_ extract * 2) may represent all indexed identifiers named 'extract', 
      where the second index is 2, and the first index can be arbitrary 
      numeral). We say that a non-pattern identifier is an instance of some
      pattern identifier if it can be obtained by replacing its unspecified
      indices with some concrete indices. Pattern identifiers can be used
      in signatures, when we want to add a family of sort or function
      symbols to the signature at once. For instance, if we add the sort
      symbol ( _ BitVec *) of arity 0 to the signature, we have actually 
      added the whole family of sort symbols (that is, ( _ BitVec 0), 
      (_ BitVec 1), (_ BitVec 2) and so on) of arity 0. On the other side, 
      pattern identifiers cannot be used for sort and function symbols in
      sorts and expressions, that is, each sort or expression must have
      non-pattern identifier as its top sort or function symbol. */
  bool is_pattern() const;
    
  /** Returns true if id is equal to this identifier. */
  bool operator == (const identifier & id) const;

  /** Returns true if id is not equal to this identifier. */
  bool operator != (const identifier & id) const;

  /** Returns if this identifier is lexicographically lower than id. */
  bool operator < (const identifier & id) const;

  /** Returns if this identifier is lexicographically greater than id. */
  bool operator > (const identifier & id) const;

  /** Returns if this identifier is lexicographically lower or equal to id. */
  bool operator <= (const identifier & id) const;

  /** Returns if this identifier is lexicographically greater or equal to id. */
  bool operator >= (const identifier & id) const;

    
  std::size_t hash_code() const;
  void out(std::ostream & ostr) const;
  std::string to_string() const;
};


inline
std::string to_string(identifier::index::type tp)
{
  switch(tp)
    {
    case identifier::index::UNSIGNED:
      return std::string("UNSIGNED");
      break;
    case identifier::index::SYMBOL:
      return std::string("SYMBOL");
      break;
    case identifier::index::UNSPECIFIED:
      return std::string("UNSPECIFIED");
      break;
    default:
      return std::string("UNDEF");
      break;
    }  
}


/** Type of the exception thrown when an indefier index is of one type, 
    and the user requests the value of another type. */
class bad_index_type_exception : public base_exception {
private:
  identifier::index _index; 
public:
  /** Constructor. */
  bad_index_type_exception(const std::string & message,
			   const identifier::index & ind)
    :base_exception(message + std::string(": bad index type: ") +
		    ind.to_string() + " of type: " + 
		    to_string(ind.get_type()) + 
		    std::string(" (different type of value requested)")),
     _index(ind)
  {}

  /** Gets index. */
  const identifier::index & get_index() const
  {
    return _index;
  }
};

inline
unsigned identifier::index::get_unsigned_value() const
{
  if(_tp != UNSIGNED)
    throw bad_index_type_exception("Index is not of appropriate type", *this);
  return _u_val;
}

inline
const symbol & identifier::index::get_symbol_value() const
{
  if(_tp != SYMBOL)
    throw bad_index_type_exception("Index is not of appropriate type", *this);
  return _s_val;
}



namespace std {
    template<>
    struct hash<identifier> : 
      public unary_function<identifier, std::size_t> {
      std::size_t operator ()(const identifier & id) const
      {
	return id.hash_code();
      }
    };
}


/** Prints the identifier to the output stream. */
std::ostream & operator << (std::ostream & ostr, const identifier & id);

  
class identifier_weak_comparator {
public:
  bool operator () (const identifier & id1, const identifier & id2) const;
};

/** Type of exception thrown if the attribute value is requested, and
    the attribute has no value. */
class no_attribute_value_exception : public base_exception {
private:
  keyword _keyword;
public:
  /** Constructor. */
  no_attribute_value_exception(const std::string & message, 
			       const keyword & kw)
    :base_exception(message + std::string(": no attribute value: ") + 
		    kw.to_string()),
     _keyword(kw)
  {}
    
  /** Gets the keyword of the attribute. */
  const keyword & get_keyword() const
  {
    return _keyword;
  }
};

/** Type of exception thrown when the requested type of the attribute value
    is wrong. */
class bad_attribute_value_type_exception : public base_exception {
private:
  keyword _keyword;
  std::string _requested_type;
  std::string _real_type;
public:
  /** Constructor. */
  bad_attribute_value_type_exception(const std::string & message, 
				     const keyword & kw,
				     const std::string & requested_type,
				     const std::string & real_type)
    :base_exception(message + std::string(": bad attribute value type: ") +
		    kw.to_string() + " of type " + real_type + " (and "
		    + requested_type + " requested)"),
     _keyword(kw),
     _requested_type(requested_type),
     _real_type(real_type)
  {}
    
  /** Gets the keyword of the attribute. */
  const keyword & get_keyword() const
  {
    return _keyword;
  }
    
  /** Gets the requested type (its std::type_info string representation). */
  const std::string & get_requested_type() const
  {
    return _requested_type;
  }
    
  /** Gets the real type (its std::type_info string representation). */
  const std::string & get_real_type() const
  {
    return _real_type;
  }
};
  
/** Represents value of an attribute. It is an abstract class that can 
    be inherited by a subclass that can implement any kind of value. */
class attribute_value {
public:
  /** Gets the type_info of dynamic type of the value class. */
  const std::type_info & type_info() const
  {
    return typeid(*this);
  }
    
  /** Clones the value object. */
  virtual attribute_value * clone() const = 0;
    
  /** Prints the value object to the output stream. */
  virtual void print_value(std::ostream & ostr) const = 0;
    
  /** Virtual destructor */
  virtual ~attribute_value() {}
};

/** Vector of pointers to attribute value objects. */
typedef std::vector<attribute_value *> attribute_value_vector;
  
/** Represents a template that acts as an attribute_value wrapper for
    standard C++ types (i.e. any C++ type that is assignable, copy
    constructible and has defined << operator for printing). For
    any such type T, the template instance standard_attribute_value<T>
    represents an attribute_value object that contain a value of type T. */
template <typename T>
class standard_attribute_value : public attribute_value {
public:
  /** The C++ type of the value contained in the object. */
  typedef T value_type;
private:
  value_type _value;
public:
  /** Constructor that initializes the value object with the given value. */
  standard_attribute_value(const value_type & value)
    :_value(value)
  {}

  /** Constructor that initializes the value object with the given value. */
  standard_attribute_value(value_type && value)
    :_value(std::move(value))
  {}

  
  /** Gets the value contained in the value object. */
  const value_type & get_value() const
  {
    return _value;
  }
    
  /** Implements the clone() virtual method. */
  virtual standard_attribute_value * clone() const
  {
    return new standard_attribute_value(_value);
  }

  /** Implements the print_value() virtual method. */
  virtual void print_value(std::ostream & ostr) const
  {
    ostr << _value;
  }
};

/* Explicit specialization for standard_attribute_value<bool>::print_value() */
template <>
inline
void standard_attribute_value<bool>::print_value(std::ostream & ostr) const
{
  ostr << std::boolalpha << _value << std::noboolalpha;
}

/** Represents a composite attribute value, i.e. a value consisting
    of a sequence of arbitrary attribute values. */
class composite_attribute_value : public attribute_value {
private:
  attribute_value_vector _values;
public:
  /** Constructor. Accepts a vector of attribute values that are
      components of the value being constructed. The composite
      value owns its components, i.e. it deletes them when it is
      deleted. */
  composite_attribute_value(const attribute_value_vector & values)
    :_values(values)
  {}

  composite_attribute_value(attribute_value_vector && values)
    :_values(std::move(values))
  {}

  composite_attribute_value(const composite_attribute_value & cav)
  {
    _values.reserve(cav._values.size());
    for(attribute_value_vector::const_iterator it = cav._values.begin(), 
	  it_end = cav._values.end(); it != it_end; ++it)
      _values.push_back((*it)->clone());
  }

  composite_attribute_value(composite_attribute_value && cav)
    :_values(std::move(cav._values))
  {}
  
  
  /** Gets the vector of components. */
  const attribute_value_vector & get_components() const
  {
    return _values;
  }

  /** Clones the value object. Components are also cloned (deep copying). */
  virtual attribute_value * clone() const
  {
    return new composite_attribute_value(*this);
  }
    
  /** Prints the value object to the output stream. */
  virtual void print_value(std::ostream & ostr) const
  {
    ostr << "(";
    for(attribute_value_vector::const_iterator it = _values.begin(), 
	  it_end = _values.end(); it != it_end; ++it)
      {
	(*it)->print_value(ostr);
	ostr << " ";
      }
    ostr << ")";
  }
    
  /** Virtual destructor (destroys the components recursively). */
  virtual ~composite_attribute_value() 
  {
    for(attribute_value_vector::const_iterator it = _values.begin(), 
	  it_end = _values.end(); it != it_end; ++it)
      delete (*it);
  }

};

/** Represents attributes. Each attribute has its keyword and (optionally) 
    a value. */
class attribute {
private:
  keyword _keyword;
  attribute_value * _value;
public:
  /** Constructor that initializes the attribute with the given keyword
      and (optionally) the given value. It the value is provided, then
      it becomes owned by the attribute, i.e. it is deleted (by invoking 
      delete operator) when the attribute object itself is deleted. */
  attribute(const keyword & kw, attribute_value * value = 0);

  /** Constructor that initializes the attribute with the given keyword
      and the given value of an arbitrary C++ assignable, copy-constructible,
      and "operator <<" printable type. The value object assigned to the
      attribute is of type standard_attribute_value<T>. The value object is
      owned by the attribute object, and it is deleted (by invoking delete
      operator) when the attribute object itself is deleted. */
  template <typename T> 
  attribute(const keyword & kw, T && value);

  /** Copy constructor. It implements deep copying, i.e. the value object is
      cloned, if it exists. */
  attribute(const attribute & attr);

  /** Move constructor. */
  attribute(attribute && attr);

  
  /** Assignment operator. It implements deep copying, i.e. the value object
      is cloned, if it exists. */
  attribute & operator = (const attribute & attr);

  /** Move assignment operator. */
  attribute & operator = (attribute && attr);
  
  /** Returns true if the keyword of the attribute attr is equal to the
      keyword of this attribute. */
  bool operator == (const attribute & attr) const;

  /** Returns true if the keyword of the attribute attr is not equal to the
      keyword of this attribute. */
  bool operator != (const attribute & attr) const;
  
  /** Gets the keyword of the attribute. */
  const keyword & get_keyword() const;

  /** Gets the value of the attribute (or 0 if the attribute has no value).
      Notice that the value object is owned by the attribute object. You
      should not invoke delete operator on the value object, or make
      the value object owned by another attribute object (if you need 
      the same value for another attribute, clone() the value 
      object, and use the copy). */
  attribute_value * get_value() const;

  /** If the value of the attribute is of type standard_attribute_value<T>,
      the method assigns the value of the attribute (of type T) 
      to the value argument. The method throws an exception 
      if the attribute has no value or
      the value is not of the requested type. This also happens if the
      value assigned to the attribute is not an instance of 
      standard_attribute_value<T>. */
  template <typename T>
  void get_value(T & value) const;

  /** Returns true if the attribute has value. */
  bool has_value() const;
    
  /** Sets the value of the attribute to the given value. The previous 
      value (if existed) is deleted, and the new value object becomes owned
      by the attribute object. */
  void set_value(attribute_value * value);

  /** Set the value of the attribute to the given value of type T, where
      T is any C++ type that is copy-constructible, assignable and 
      "operator <<" printable. The previous value (if existed) is deleted,
      and the new value object (of type standard_attribute_value<T>) becomes
      owned by the attribute object. */
  template <typename T>
  void set_value(T && value);

  /** Resets the value of the attribute. The old value (if existed) is
      deleted. */
  void reset_value();

  void out(std::ostream & ostr) const;
    
  /** Destructor. */
  ~attribute();  
};
  

namespace std {
    template<>
    struct hash<attribute> : 
      public unary_function<attribute, std::size_t> {
      std::size_t operator ()(const attribute & attr) const
      {
	return attr.get_keyword().hash_code();
      }
    };
}
  
/** Prints the attribute to the output stream. */
std::ostream & operator << (std::ostream & ostr, const attribute & attr);
  
/** Set of attributes. */
typedef std::unordered_set<attribute> attribute_set;
  
/** Empty attribute set. */
extern const attribute_set EMPTY_ATTRIBUTE_SET;
  
/** Prints the attribute set to the output. */
std::ostream & operator << (std::ostream & ostr, const attribute_set & attr); 


/** Represent a sort symbol. It inherits identifier. */
class sort_symbol : public identifier {  
public:
    
  /** Undefined sort symbol. */
  static const sort_symbol UNDEFINED;

  /** Bool sort symbol. */
  static const sort_symbol BOOL;
    
  /** Int sort symbol. */
  static const sort_symbol INT;

  /** Real sort symbol. */
  static const sort_symbol REAL;

  /** Array sort symbol. */
  static const sort_symbol ARRAY;
    
  /** (_ BitVec *) sort symbol (pattern). */
  static const sort_symbol BITVEC;

  using identifier::identifier;
  
  /** Initializing sort_symbol using id */
  sort_symbol(const identifier & id);
  
  /** Initializing sort_symbol using id */
  sort_symbol(identifier && id);
    
};
  

namespace std {
    template<>
    struct hash<sort_symbol> : 
      public unary_function<sort_symbol, std::size_t> {
      std::size_t operator ()(const sort_symbol & s) const
      {
	return s.hash_code();
      }
    };
}

  
/** Represents a sort parameter. Inherits symbol. */
class sort_parameter : public symbol {
public:
    
  /** Undefined sort parameter */
  static const sort_parameter UNDEFINED;

  using symbol::symbol;  
};
  

namespace std {
    template<>
    struct hash<sort_parameter> : 
      public unary_function<sort_parameter, std::size_t> {
      std::size_t operator ()(const sort_parameter & p) const
      {
	return p.hash_code();
      }
    };
}

  
/** Vector of sort parameters. */
typedef std::vector <sort_parameter> sort_parameter_vector;

/** Empty sort parameter vector. */
extern const sort_parameter_vector EMPTY_SORT_PARAMETER_VECTOR;
  
/** Set of sort parameters. */
typedef std::unordered_set<sort_parameter> sort_parameter_set;
  
/** Empty sort parameter set. */
extern const sort_parameter_set EMPTY_SORT_PARAMETER_SET;
  
/** Represents a function symbol. Inherits identifier. */
class function_symbol : public identifier {
    
public:
    
  /** Undefined function symbol. */
  static const function_symbol UNDEFINED;

  /** true function symbol. */
  static const function_symbol TRUE;

  /** false function symbol. */
  static const function_symbol FALSE;

  /** not function symbol. */
  static const function_symbol NOT;

  /** => function symbol. */
  static const function_symbol IMPLIES;

  /** and function symbol. */
  static const function_symbol AND;

  /** or function symbol. */
  static const function_symbol OR;

  /** xor function symbol. */
  static const function_symbol XOR;

  /** = function symbol. */
  static const function_symbol EQ;

  /** distinct function symbol. */
  static const function_symbol DISTINCT;

  /** ite function symbol. */
  static const function_symbol ITE;

  /** - function symbol. */
  static const function_symbol MINUS;

  /** + function symbol. */
  static const function_symbol PLUS;

  /** * function symbol. */
  static const function_symbol TIMES;

  /** div function symbol. */
  static const function_symbol DIV;

  /** / function symbol. */
  static const function_symbol SLASH;

  /** mod function symbol. */
  static const function_symbol MOD;

  /** abs function symbol. */
  static const function_symbol ABS;

  /** < function symbol. */
  static const function_symbol LT;

  /** <= function symbol. */
  static const function_symbol LE;

  /** > function symbol. */
  static const function_symbol GT;

  /** >= function symbol. */
  static const function_symbol GE;

  /** (_ divisible *) function symbol (pattern). */
  static const function_symbol DIVISIBLE;

  /** to_real function symbol. */
  static const function_symbol TO_REAL;

  /** to_int function symbol. */
  static const function_symbol TO_INT;

  /** is_int function symbol. */
  static const function_symbol IS_INT;

  /** select function symbol. */
  static const function_symbol SELECT;

  /** store function symbol. */
  static const function_symbol STORE;

  /** concat function symbol. */
  static const function_symbol CONCAT;

  /** (_ extract * *) function symbol (pattern). */
  static const function_symbol EXTRACT;

  /** bvnot function symbol. */
  static const function_symbol BVNOT;

  /** bvneg function symbol. */
  static const function_symbol BVNEG;

  /** bvand function symbol. */
  static const function_symbol BVAND;

  /** bvor function symbol. */
  static const function_symbol BVOR;

  /** bvadd function symbol. */
  static const function_symbol BVADD;

  /** bvmul function symbol. */
  static const function_symbol BVMUL;

  /** bvudiv function symbol. */
  static const function_symbol BVUDIV;

  /** bvurem function symbol. */
  static const function_symbol BVUREM;

  /** bvshl function symbol. */
  static const function_symbol BVSHL;

  /** bvlshr function symbol. */
  static const function_symbol BVLSHR;

  /** bvult function symbol. */
  static const function_symbol BVULT;
    
  /** bvnand function symbol. */
  static const function_symbol BVNAND;
    
  /** bvnor function symbol. */
  static const function_symbol BVNOR;
    
  /** bvxor function symbol. */
  static const function_symbol BVXOR;
    
  /** bvxnor function symbol. */
  static const function_symbol BVXNOR;
    
  /** bvcomp function symbol. */
  static const function_symbol BVCOMP;
    
  /** bvsub function symbol. */
  static const function_symbol BVSUB;
    
  /** bvsdiv function symbol. */
  static const function_symbol BVSDIV;
    
  /** bvsrem function symbol. */
  static const function_symbol BVSREM;
    
  /** bvsmod function symbol. */
  static const function_symbol BVSMOD;
    
  /** bvashr function symbol. */
  static const function_symbol BVASHR;
    
  /** (_ repeat *) function symbol (pattern). */
  static const function_symbol REPEAT;

  /** (_ zero_extend *) function symbol (pattern). */
  static const function_symbol ZERO_EXTEND;

  /** (_ sign_extend *) function symbol (pattern). */
  static const function_symbol SIGN_EXTEND;

  /** (_ rotate_left *) function symbol (pattern). */
  static const function_symbol ROTATE_LEFT;

  /** (_ rotate_right *) function symbol (pattern). */
  static const function_symbol ROTATE_RIGHT;

  /** bvule function symbol. */
  static const function_symbol BVULE;
    
  /** bvugt function symbol. */
  static const function_symbol BVUGT;
    
  /** bvuge function symbol. */
  static const function_symbol BVUGE;
    
  /** bvslt function symbol. */
  static const function_symbol BVSLT;
    
  /** bvsle function symbol. */
  static const function_symbol BVSLE;

  /** bvsgt function symbol. */
  static const function_symbol BVSGT;
    
  /** bvsge function symbol. */
  static const function_symbol BVSGE;

  /* Non-standard symbols. */

  /** alldiff function symbol. */
  static const function_symbol ALLDIFF;

  /** not_alldiff function symbol. */
  static const function_symbol NOT_ALLDIFF;


  using identifier::identifier;
  
  /** Constructs the function symbol using an identifier id */
  function_symbol(const identifier & id);

  /** Constructs the function symbol using an identifier id */
  function_symbol(identifier && id);

};
  

namespace std {
    template<>
    struct hash<function_symbol> : 
      public unary_function<function_symbol, std::size_t> {
      std::size_t operator ()(const function_symbol & s) const
      {
	return s.hash_code();
      }
    };
}

/** Represents a variable. Inherits symbol. */  
class variable : public symbol {
    
public:
    
  /** Undefined variable */
  static const variable UNDEFINED;

  using symbol::symbol;
};


namespace std {
    template<>
    struct hash<variable> : 
      public unary_function<variable, std::size_t> {
      std::size_t operator ()(const variable & v) const
      {
	return v.hash_code();
      }
    };
}


/** Vector of variables. */
typedef std::vector<variable> variable_vector;
  
/** Empty vector of variables. */
extern const variable_vector EMPTY_VARIABLE_VECTOR;
  
  
  
// INLINE FUNCTION DEFINITIONS
  
inline
special_constant::special_constant(type tp, const std::string & value)
  : special_constant(tp, std::string(value))
{}

inline
special_constant::special_constant(type tp, std::string && value)
{
  data d(tp, std::move(value));
  d.calculate_hash();
  _data = get_factory().add_object(std::move(d));
}

inline
special_constant::special_constant(type tp, const char * value)
  : special_constant(tp, std::string(value))
{}
  
inline
special_constant::type special_constant::get_type() const
{
  return _data->_type;
}


inline
bool special_constant::is_numeral() const
{
  return _data->_type == T_NUMERAL;
}


inline
bool special_constant::is_hexadecimal() const
{
  return _data->_type == T_HEXADECIMAL;
}


inline  
bool special_constant::is_binary() const
{
  return _data->_type == T_BINARY;
}

inline
bool special_constant::is_decimal() const
{
  return _data->_type == T_DECIMAL;
}


inline
bool special_constant::is_string_literal() const
{
  return _data->_type == T_STRING_LITERAL;
}

inline
const std::string & special_constant::get_value() const
{
  return _data->_value;
}

  
inline
void special_constant::out(std::ostream & ostr) const
{
  if(_data->_type != T_UNDEF)
    ostr << _data->_value;
  else
    ostr << "UNDEF_CONSTANT";
}
  
inline
std::string special_constant::to_string() const
{
  return ::to_string(this);
}
  
inline
bool special_constant::operator == (const special_constant & c) const
{
  return _data == c._data;
}
  
inline 
bool special_constant::operator != (const special_constant & c) const
{
  return !(*this == c);
}

inline
bool special_constant::operator < (const special_constant & c) const
{
  return _data->_type < c._data->_type || 
				  (_data->_type == c._data->_type && 
				   _data->_value < c._data->_value);
}
  
inline
bool special_constant::operator > (const special_constant & c) const
{
  return c < *this;
}

inline
bool special_constant::operator <= (const special_constant & c) const
{
  return *this == c || *this < c;
}

inline
bool special_constant::operator >= (const special_constant & c) const
{
  return *this == c || *this > c;
}

inline
std::size_t special_constant::hash_code() const
{
  return _data->_hash_code;
}
  
inline
std::ostream & operator << (std::ostream & ostr, 
			    const special_constant & constant)
{
  constant.out(ostr);
  return ostr;
}


inline
symbol::symbol()
  : _data(0)
{}

inline
symbol::symbol(const std::string & name)
  : symbol(std::string(name))
{}

inline
symbol::symbol(std::string && name)
{
  data d(std::move(name));
  d.calculate_hash();
  _data = get_factory().add_object(std::move(d));
}

inline
symbol::symbol(const char * name)
  : symbol(std::string(name))
{}
  
inline
const std::string & symbol::get_name() const
{ 
  return _data->_name;
}
  
inline
bool symbol::operator == (const symbol & s) const
{
  return _data == s._data;
}
  
inline
bool symbol::operator != (const symbol & s) const
{
  return !(*this == s);
}
  
inline
bool symbol::operator < (const symbol & s) const
{
  return _data->_name < s._data->_name;
}

inline
bool symbol::operator > (const symbol & s) const
{
  return _data->_name > s._data->_name;
}

inline
bool symbol::operator <= (const symbol & s) const
{
  return _data->_name <= s._data->_name;
}
  
inline
bool symbol::operator >= (const symbol & s) const
{
  return _data->_name >= s._data->_name;
}

inline
std::size_t symbol::hash_code() const
{
  return _data->_hash_code;
}
  
inline
void symbol::out(std::ostream & ostr) const
{
  ostr << _data->_name;
}
  
inline
std::string symbol::to_string() const
{
  return ::to_string(this);
}
  
  
inline
std::ostream & operator << (std::ostream & ostr, const symbol & s)
{
  s.out(ostr);
  return ostr;
}
  
  
inline
keyword::keyword(const std::string & name)
  : keyword(std::string(name))
{}

inline
keyword::keyword(std::string && name)
{
  data d(std::move(name));
  d.calculate_hash();
  _data = get_factory().add_object(std::move(d));
}


inline
keyword::keyword(const char * name)
  : keyword(std::string(name))
{}
  
inline
const std::string & keyword::get_name() const
{ 
  return _data->_name;
}
  
inline
bool keyword::operator == (const keyword & k) const
{
  return _data == k._data;
}
  
inline
bool keyword::operator != (const keyword & k) const
{
  return !(*this == k);
}

inline
bool keyword::operator < (const keyword & k) const
{
  return _data->_name < k._data->_name;
}

inline
bool keyword::operator > (const keyword & k) const
{
  return _data->_name > k._data->_name;
}

inline
bool keyword::operator <= (const keyword & k) const
{
  return _data->_name <= k._data->_name;
}
  
inline
bool keyword::operator >= (const keyword & k) const
{
  return _data->_name >= k._data->_name;
}

  
inline
std::size_t keyword::hash_code() const
{
  return _data->_hash_code;
}
  
inline
void keyword::out(std::ostream & ostr) const
{
  ostr << _data->_name;
}
  
inline
std::string keyword::to_string() const
{
  return ::to_string(this);
}
  
  
inline
std::ostream & operator << (std::ostream & ostr, const keyword & k)
{
  k.out(ostr);
  return ostr;
}

inline
identifier::identifier(const std::string & name, const index_vector & indices)
  : identifier(std::string(name), !indices.empty() ? std::move(index_vector(indices)) : std::move(_dummy_index_vector))	       
{}

inline
identifier::identifier(std::string && name, index_vector && indices)
{
 data d(std::move(name),
	!indices.empty() ? std::shared_ptr<const index_vector>(new index_vector(std::move(indices)))
	: std::shared_ptr<const index_vector>(&EMPTY_INDEX_VECTOR, no_op()));
  d.check_pattern();
  d.calculate_hash();    
  _data = get_factory().add_object(std::move(d));
}

inline
identifier::identifier(std::string && name, const index_vector & indices)
  : identifier(std::move(name), !indices.empty() ? std::move(index_vector(indices)) : std::move(_dummy_index_vector))
{}
  
inline
identifier::identifier(const std::string & name, index_vector && indices)
  : identifier(std::string(name), std::move(indices))
{}


inline
identifier::identifier(const char * name, const index_vector & indices)
  : identifier(std::string(name), !indices.empty() ? std::move(index_vector(indices)) : std::move(_dummy_index_vector))
{}

inline
identifier::identifier(const char * name, index_vector && indices)
  : identifier(std::string(name), std::move(indices))    
{}
      
inline
const std::string & identifier::get_name() const
{
  return _data->_name;
}
  
inline
const identifier::index_vector & identifier::get_indices() const
{
  return *_data->_indices;
}
  
inline
bool identifier::has_indices() const
{
  return _data->_indices.get() != &EMPTY_INDEX_VECTOR;
}

inline
bool identifier::is_pattern() const
{
  return _data->_is_pattern;
}
  
inline
std::size_t identifier::hash_code() const
{
  return _data->_hash_code;
}
  
inline
std::string identifier::to_string() const
{
  return ::to_string(this);
}
  
  
inline
bool identifier::operator == (const identifier & id) const
{
  return _data == id._data;
}
  
inline
bool identifier::operator != (const identifier & id) const
{
  return !((*this) == id);
}
  
inline
bool identifier::operator < (const identifier & id) const
{
  return _data->_name < id._data->_name ||
			    (_data->_name == id._data->_name && 
			     *_data->_indices < *id._data->_indices);
}
  
inline
bool identifier::operator > (const identifier & id) const
{
  return id < *this;
}

inline
bool identifier::operator <= (const identifier & id) const
{
  return *this == id || *this < id;
}

inline
bool identifier::operator >= (const identifier & id) const
{
  return *this == id || *this > id;
}

  
inline 
std::ostream & operator << (std::ostream & ostr, const identifier & id)
{
  id.out(ostr);
  return ostr;
}

inline 
attribute::attribute(const keyword & kw, attribute_value * value)
  :_keyword(kw),
   _value(value)
{}

template <typename T>
inline
attribute::attribute(const keyword & kw, T && value)
  :_keyword(kw),
   _value(new standard_attribute_value<std::remove_cv_t<std::remove_reference_t<T>>>(std::forward<T>(value)))
{}
  
inline 
attribute::attribute(const attribute & attr)
  :_keyword(attr._keyword),
   _value(attr._value != 0 ? attr._value->clone() : 0)
{}

inline 
attribute::attribute(attribute && attr)
  :_keyword(std::move(attr._keyword)),
   _value(attr._value)
{
  attr._value = 0;
}


inline 
attribute & attribute::operator = (const attribute & attr)
{
  if(&attr == this)
    return *this;
    
  delete _value;
    
  _keyword = attr._keyword;
  _value = attr._value != 0 ? attr._value->clone() : 0;
  
  return *this;
}

inline 
attribute & attribute::operator = (attribute && attr)
{
  if(&attr == this)
    return *this;
    
  delete _value;
    
  _keyword = std::move(attr._keyword);
  _value = attr._value;
  attr._value = 0;
  
  return *this;
}


inline 
bool attribute::operator == (const attribute & attr) const
{
  return _keyword == attr._keyword;
}
  
inline 
bool attribute::operator != (const attribute & attr) const
{
  return _keyword != attr._keyword;
}
  
inline 
const keyword & attribute::get_keyword() const
{
  return _keyword;
}
  
inline 
attribute_value * attribute::get_value() const
{
  return _value;
}

template <typename T>
inline
void attribute::get_value(T & value) const
{
  if(_value == 0)
    throw no_attribute_value_exception("Value not set", _keyword);
    
  standard_attribute_value<T> * standard_value = 
    dynamic_cast<standard_attribute_value<T> *>(_value);
    
  if(standard_value == 0)
    throw bad_attribute_value_type_exception("Attribute value cannot be read",
					     _keyword, 
					     _value->type_info().name(),
					     typeid(T).name());
  
  value = standard_value->get_value();
}

inline 
bool attribute::has_value() const
{
  return _value != 0;
}

inline 
void attribute::set_value(attribute_value * value) 
{
  delete _value;
  _value = value;
}

template <typename T>
inline
void attribute::set_value(T && value)
{
  delete _value;
  _value = new standard_attribute_value<std::remove_cv_t<std::remove_reference_t<T>>>(std::forward<T>(value));
}

inline
void attribute::reset_value()
{
  delete _value;
  _value = 0;
}

inline
void attribute::out(std::ostream & ostr) const
{
  ostr << _keyword;
  if(_value)
    {
      ostr << " ";
      _value->print_value(ostr);
    }
}

inline 
attribute::~attribute()
{
  delete _value;
}

inline
std::ostream & operator << (std::ostream & ostr, const attribute & attr)
{
  attr.out(ostr);
  return ostr;
}

inline
sort_symbol::sort_symbol(const identifier & id)
  :identifier(id)
{}

inline
sort_symbol::sort_symbol(identifier && id)
  :identifier(std::move(id))
{}

inline
function_symbol::
function_symbol(const identifier & id)
  :identifier(id)
{}

inline
function_symbol::
function_symbol(identifier && id)
  :identifier(std::move(id))
{}

#endif // _BASIC_TYPES_H
