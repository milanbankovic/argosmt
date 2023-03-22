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

#ifndef _ARITHMETIC_CANONIZER_H
#define _ARITHMETIC_CANONIZER_H

#include "expression_library.hpp"
#include "linear_polynomial.hpp"
#include <map>
#include <unordered_map>

template <typename C>
class arithmetic_canonizer {
private:
  expression_factory * _factory;

protected:
  expression _one_expression;
  expression _zero_expression;
  
public:
  typedef C coeff_type;

  typedef linear_polynomial_template< std::map<expression, coeff_type> > ordered_linear_polynomial;
  typedef linear_polynomial_template< std::unordered_map<expression, coeff_type> > linear_polynomial;
public:
  arithmetic_canonizer(expression_factory * factory)
    :_factory(factory)
  {
    _one_expression = _factory->create_expression(special_constant(special_constant::T_NUMERAL, "1"));    
    _zero_expression = _factory->create_expression(special_constant(special_constant::T_NUMERAL, "0"));
  }

  bool is_arithmetic_relation_sym(const function_symbol & fs)
  {
    return  
      fs == function_symbol::LT ||
      fs == function_symbol::GT ||
      fs == function_symbol::GE ||
      fs == function_symbol::LE;
  }
  
  bool is_arithmetic_operation_sym(const function_symbol & fs)
  {
    return 
      fs == function_symbol::PLUS ||
      fs == function_symbol::MINUS ||
      fs == function_symbol::TIMES ||
      fs == function_symbol::SLASH;
  }

  bool is_arithmetic_relation(const expression & e)
  {
    return is_arithmetic_relation_sym(e->get_symbol());
  }
  
  bool is_arithmetic_operation(const expression & e)
  {
    return is_arithmetic_operation_sym(e->get_symbol());
  }
  
  bool is_arithmetic(const expression & e)
  {
    const function_symbol & fs = e->get_symbol();
    return is_arithmetic_relation_sym(fs) || is_arithmetic_operation_sym(fs);
  }
  
  bool is_coefficient(const expression & e)
  {
    return 
      e->is_numeral_constant() || 
      e->is_decimal_constant() ||
      (e->get_symbol() == function_symbol::MINUS &&
       e->get_operands().size() == 1 &&
       (e->get_operands()[0]->is_numeral_constant() ||
	e->get_operands()[0]->is_decimal_constant())) ||
      e->get_symbol() == function_symbol::SLASH;
  }
  
  
  bool is_atomic(const expression & e)
  {
    return is_coefficient(e) || !is_arithmetic(e);
  }
  
  bool is_atom(const expression & e)
  {
    return !is_arithmetic(e) && !is_coefficient(e);
  }
    
  const function_symbol & get_opposite_symbol(const function_symbol & fs)
  {
    if(fs == function_symbol::GT)
      return function_symbol::LE;
    if(fs == function_symbol::LT)
      return function_symbol::GE;
    if(fs == function_symbol::GE)
      return function_symbol::LT;
    if(fs == function_symbol::LE)
      return function_symbol::GT;
    if(fs == function_symbol::EQ)
      return function_symbol::DISTINCT;
    if(fs == function_symbol::DISTINCT)
      return function_symbol::EQ;

    return fs;
  }

  const function_symbol & get_reversed_symbol(const function_symbol & fs)
  {
    if(fs == function_symbol::GT)
      return function_symbol::LT;
    if(fs == function_symbol::LT)
      return function_symbol::GT;
    if(fs == function_symbol::GE)
      return function_symbol::LE;
    if(fs == function_symbol::LE)
      return function_symbol::GE;
    if(fs == function_symbol::EQ)
      return function_symbol::EQ;
    if(fs == function_symbol::DISTINCT)
      return function_symbol::DISTINCT;
    
    return fs;
  }

  /* Canonization functions */

  coeff_type get_value_from_expr(const expression & e);
  expression get_expr_from_value(const coeff_type & q);


  expression create_monomial(const expression & coeff, const expression & atom)
  {
    if(atom == _one_expression)
      return coeff;
    
    if(coeff == _one_expression)
      return atom;
    else
      return _factory->create_expression(function_symbol::TIMES, coeff, atom);
  }

  bool is_monomial(const expression & e)
  {
    return is_atomic(e) || (e->get_symbol() == function_symbol::TIMES && 
			    is_coefficient(e->get_operands()[0]) && 
			    is_atom(e->get_operands()[1]));
  }

  expression get_monomial_coeff(const expression & e)
  {
    if(e->get_symbol() == function_symbol::TIMES)
      {
	return e->get_operands()[0];
      }
    else if(is_atom(e))
      {
	return _one_expression;
      }
    else
      return e;
  }

  expression get_monomial_unknown(const expression & e)
  {
    if(e->get_symbol() == function_symbol::TIMES)
      {
	return e->get_operands()[1];
      }
    else if(is_atom(e))
      {
	return e;
      }
    else
      return _one_expression;
  }

  template <typename T>
  void get_polynomial_from_expression(const expression & e, linear_polynomial_template<T> & lp, bool negate = false)
  {
    assert(is_monomial(e) || e->get_symbol() == function_symbol::PLUS);
  
    if(is_monomial(e))
      {
	expression x = get_monomial_unknown(e);
	coeff_type c = get_value_from_expr(get_monomial_coeff(e));
	if(!negate)
	  lp.add_to_coeff(x, c);
	else
	  lp.add_to_coeff(x, -c);
	return;
      }
  
    if(e->get_symbol() == function_symbol::PLUS)
      {
	const expression_vector & ops = e->get_operands();
      
	for(unsigned i = 0; i < ops.size(); i++)
	  {	    
	    expression x = get_monomial_unknown(ops[i]);
	    coeff_type c = get_value_from_expr(get_monomial_coeff(ops[i]));
	    if(!negate)
	      lp.add_to_coeff(x, c);
	    else
	      lp.add_to_coeff(x, -c);
	  }
	return;
      }
  }

  template <typename T>
  expression get_expression_from_polynomial(const linear_polynomial_template<T> & lp)
  {
    if(lp.empty())
      {
	return _zero_expression;
      }
  
    expression_vector res_ops;
    res_ops.reserve(lp.size());
    for(typename linear_polynomial_template<T>::const_iterator it = lp.begin(), it_end = lp.end(); it != it_end; ++it)
      {
	expression monomial = create_monomial(get_expr_from_value(it->second), it->first);
	res_ops.push_back(monomial);
      }
  
    if(res_ops.size() == 1)
      return res_ops[0];
    else
      return _factory->create_expression(function_symbol::PLUS, std::move(res_ops));
  }

  expression canonize_decimal_constant(const expression & e);
  
  expression canonize_slash(const expression & e)
  {
    coeff_type q = get_value_from_expr(e);
    return get_expr_from_value(q);
  }
  
  expression canonize_times(const expression & e)
  {
    const expression_vector & ops = e->get_operands();
    expression coeff, atom;
  
    if(is_coefficient(ops[0]))
      {
	coeff = ops[0];
	atom = ops[1];
      }
    else
      {
	assert(is_coefficient(ops[1]));
	coeff = ops[1];
	atom = ops[0];
      }
  
    if(is_atom(atom))
      {
	return create_monomial(coeff, atom);
      }
    else 
      {
	ordered_linear_polynomial lp;
	get_polynomial_from_expression(atom, lp);
	lp.scale_polynomial(get_value_from_expr(coeff));
	return get_expression_from_polynomial(lp);
      }
  }

  expression canonize_plus(const expression & e)
  {
    const expression_vector & ops = e->get_operands();
    ordered_linear_polynomial lp;
        
    for(unsigned i = 0; i < ops.size(); i++)
      {
	get_polynomial_from_expression(ops[i], lp, false);
      }
    
    return get_expression_from_polynomial(lp);
  }

  expression canonize_unary_minus(const expression & e)
  {
    const expression & op = e->get_operands()[0];
  
    if(op->is_numeral_constant())
      return e;
  
    ordered_linear_polynomial lp;
  
    get_polynomial_from_expression(op, lp, true);
  
    return get_expression_from_polynomial(lp);
  }

  expression canonize_binary_minus(const expression & e)
  {
    const expression_vector & ops = e->get_operands();
    ordered_linear_polynomial lp;
  
    get_polynomial_from_expression(ops[0], lp, false);
  
    for(unsigned i = 1; i < ops.size(); i++)
      {
	get_polynomial_from_expression(ops[i], lp, true);
      }
  
    return get_expression_from_polynomial(lp);
  }

  template <typename T>
  void get_polynomial_from_relation(const expression & e, linear_polynomial_template<T> & lp, coeff_type & c, function_symbol & fs)
  {
    get_polynomial_from_expression(e->get_operands()[0], lp);
    get_polynomial_from_expression(e->get_operands()[1], lp, true);
    
    fs = e->get_symbol();
    c = -lp.get_coeff(_one_expression);
    lp.set_coeff(_one_expression, 0);
  }

  expression canonize_relation(const expression & e)
  {
    ordered_linear_polynomial lp;
    coeff_type c;
    function_symbol fs = function_symbol::UNDEFINED;
  
    get_polynomial_from_relation(e, lp, c, fs);
    
    if(e->get_operands()[0]->get_inferred_sort()->get_symbol() == sort_symbol::REAL)
      {
	if(!lp.empty() && lp.begin()->second != 1)
	  {
	    const coeff_type s = lp.begin()->second;
	    lp.scale_polynomial(1/s);
	    c /= s;
	    if(s < 0)
	      fs = get_reversed_symbol(fs);
	  }
      }
    else
      {
	if(!lp.empty() && lp.begin()->second < 0)
	  {
	    lp.scale_polynomial(-1);
	    c = -c;
	    fs = get_reversed_symbol(fs);
	  }
      }

    
    expression right_side = get_expr_from_value(c);
    expression left_side = get_expression_from_polynomial(lp);
    expression ret_val = _factory->create_expression(fs, left_side, right_side);    
    return ret_val;
  }

  expression canonize_eq_diseq(const expression & l)
  {
    assert(l->is_equality() || l->is_disequality());

    expression le = l->get_operands()[0];
    expression re = l->get_operands()[1];
    if(!is_coefficient(re) && (is_coefficient(le) || re < le))
      return _factory->create_expression(l->get_symbol(), re, le);
    else
      return l;
  }

  expression canonize_expression(const expression & e)
  {
    if(e->is_numeral_constant())
      return e; // numerals stay as they are
  
    if(e->is_decimal_constant())
      return canonize_decimal_constant(e);
  
    const function_symbol & efs = e->get_symbol();
  
    if(efs == function_symbol::SLASH)
      return canonize_slash(e);
  
    if(efs == function_symbol::TIMES)
      return canonize_times(e);
  
    if(efs == function_symbol::PLUS)
      return canonize_plus(e);
  
    if(efs == function_symbol::MINUS)
      {
	if(e->get_operands().size() == 1) // unary case
	  return canonize_unary_minus(e);
	else // binary case
	  return canonize_binary_minus(e);
      }
  
    if(is_arithmetic_relation_sym(efs))
      return canonize_relation(e);
  
    if(efs == function_symbol::EQ || efs == function_symbol::DISTINCT)
      return canonize_eq_diseq(e);

    if(efs == function_symbol::NOT)
      {
	const expression & op = e->get_operands()[0];
	const function_symbol & opfs = op->get_symbol();
	if(is_arithmetic_relation_sym(opfs) || opfs == function_symbol::EQ || opfs == function_symbol::DISTINCT)
	  return _factory->create_expression(get_opposite_symbol(opfs), op->get_operands()[0], op->get_operands()[1]);
	else
	  return e;
      }
  
    return e; // otherwise, do nothing!
  }

  void get_literal_pair(const expression & l, expression & l_pos, expression & l_neg)
  {
    const function_symbol & fs = l->get_symbol();
    
    if(fs == function_symbol::LE || fs == function_symbol::GE || fs == function_symbol::EQ)
      {
	l_pos = l;
	l_neg = _factory->create_expression(get_opposite_symbol(fs), l->get_operands()[0], l->get_operands()[1]);
      }
    else if(fs == function_symbol::LT || fs == function_symbol::GT || fs == function_symbol::DISTINCT)
      {
	l_neg = l;
	l_pos = _factory->create_expression(get_opposite_symbol(fs), l->get_operands()[0], l->get_operands()[1]);
      }
  }
};

template <>
inline
arithmetic_canonizer<mpq_class>::coeff_type arithmetic_canonizer<mpq_class>::get_value_from_expr(const expression & e)
{
  /* e is one of the following: n, (- n), d, (- d), (/ n m), (/ (- n) m) */
  
  if(e->is_numeral_constant() || e->is_decimal_constant())
    return e->get_special_constant().get_gmpq_value();
  
  if(e->get_symbol() == function_symbol::MINUS)
    return -e->get_operands()[0]->get_special_constant().get_gmpq_value();
  
  assert(e->get_symbol() == function_symbol::SLASH);
  
  const expression_vector & ops = e->get_operands();

  std::string s;
  if(ops[0]->is_numeral_constant())
    {
      s = ops[0]->get_special_constant().get_value();
      s += "/";
      s += ops[1]->get_special_constant().get_value();
    }
  else
    {
      s = "-";
      s += ops[0]->get_operands()[0]->get_special_constant().get_value();
      s += "/";
      s += ops[1]->get_special_constant().get_value();
    }
  coeff_type ret(s, 10);
  ret.canonicalize();
  return ret;
}

template <>
inline
expression arithmetic_canonizer<mpq_class>::get_expr_from_value(const coeff_type & q)
{
  const mpz_class & num = q.get_num();
  const mpz_class & den = q.get_den();
  
  expression num_exp, den_exp;
  
  if(num >= 0)
    {
      num_exp = _factory->create_expression(special_constant(special_constant::T_NUMERAL, num.get_str()));
    }
  else
    {
      expression tmp = _factory->create_expression(special_constant(special_constant::T_NUMERAL, num.get_str().substr(1)));
      num_exp = _factory->create_expression(function_symbol::MINUS, tmp);	  
    }
  
  if(den != 1)
    {
      den_exp = _factory->create_expression(special_constant(special_constant::T_NUMERAL, den.get_str())); 
      return _factory->create_expression(function_symbol::SLASH, num_exp, den_exp);
    }
  else
    return num_exp;
}

template <>
inline 
expression arithmetic_canonizer<mpq_class>::canonize_decimal_constant(const expression & e)
{
  coeff_type q = e->get_special_constant().get_gmpq_value();
  return get_expr_from_value(q);
}




template <>
inline
arithmetic_canonizer<long double>::coeff_type arithmetic_canonizer<long double>::get_value_from_expr(const expression & e)
{
  /* e is one of the following: n, (- n), d, (- d), (/ n m), (/ (- n) m) */
  
  if(e->is_numeral_constant())
    return (coeff_type) e->get_special_constant().get_ul_value();
  
  if(e->is_decimal_constant())
    return (coeff_type) e->get_special_constant().get_ld_value();
  
  if(e->get_symbol() == function_symbol::MINUS)
    {
      if(e->get_operands()[0]->is_numeral_constant())
	return -(coeff_type)e->get_operands()[0]->get_special_constant().get_ul_value();
      
      if(e->get_operands()[0]->is_decimal_constant())
	return -e->get_operands()[0]->get_special_constant().get_ld_value();
    }
  assert(e->get_symbol() == function_symbol::SLASH);
  
  const expression_vector & ops = e->get_operands();
  
  if(ops[0]->is_numeral_constant())
    {
      coeff_type den = (coeff_type) ops[0]->get_special_constant().get_ul_value();
      coeff_type nom = (coeff_type) ops[1]->get_special_constant().get_ul_value();
      return den / nom;
    }
  else
    {
      coeff_type den = (coeff_type) ops[0]->get_operands()[0]->get_special_constant().get_ul_value();
      coeff_type nom = (coeff_type) ops[1]->get_special_constant().get_ul_value();
      return - (den / nom);
    }
}

template <>
inline
expression arithmetic_canonizer<long double>::get_expr_from_value(const coeff_type & q)
{
  if((long double)(long int)q == q)
    {
      if(q >= 0)
	{
	  return  _factory->create_expression(special_constant(special_constant::T_NUMERAL, (unsigned long)q));
	}
      else
	{
	  expression tmp = _factory->create_expression(special_constant(special_constant::T_NUMERAL, (unsigned long)-q));
	  return _factory->create_expression(function_symbol::MINUS, tmp);	  
	}
    }
  else
    {
      if(q >= 0)
	{
	  return _factory->create_expression(special_constant(special_constant::T_DECIMAL, q));
	}
      else
	{
	  expression tmp = _factory->create_expression(special_constant(special_constant::T_DECIMAL, -q));
	  return _factory->create_expression(function_symbol::MINUS, tmp);	  
	}
    }
}

template <>
inline 
expression arithmetic_canonizer<long double>::canonize_decimal_constant(const expression & e)
{
  coeff_type q = e->get_special_constant().get_ld_value();
  return get_expr_from_value(q);
}


template <>
inline
arithmetic_canonizer<long>::coeff_type arithmetic_canonizer<long>::get_value_from_expr(const expression & e)
{
  /* e is one of the following: n, (- n), d, (- d), (/ n m), (/ (- n) m) */
  
  if(e->is_numeral_constant())
    return (coeff_type) e->get_special_constant().get_ul_value();
  
  if(e->is_decimal_constant())
    return (coeff_type) e->get_special_constant().get_ld_value();
  
  if(e->get_symbol() == function_symbol::MINUS)
    {
      if(e->get_operands()[0]->is_numeral_constant())
	return -(coeff_type)e->get_operands()[0]->get_special_constant().get_ul_value();
      
      if(e->get_operands()[0]->is_decimal_constant())
	return -(coeff_type)e->get_operands()[0]->get_special_constant().get_ld_value();
    }
  assert(e->get_symbol() == function_symbol::SLASH);
  
  const expression_vector & ops = e->get_operands();
  
  if(ops[0]->is_numeral_constant())
    {
      coeff_type den = (coeff_type) ops[0]->get_special_constant().get_ul_value();
      coeff_type nom = (coeff_type) ops[1]->get_special_constant().get_ul_value();
      return den / nom;
    }
  else
    {
      coeff_type den = (coeff_type) ops[0]->get_operands()[0]->get_special_constant().get_ul_value();
      coeff_type nom = (coeff_type) ops[1]->get_special_constant().get_ul_value();
      return -(den / nom);
    }
}

template <>
inline
expression arithmetic_canonizer<long>::get_expr_from_value(const coeff_type & q)
{
  if(q >= 0)
    {
      return _factory->create_expression(special_constant(special_constant::T_NUMERAL, (unsigned long)q));
    }
  else
    {
      expression tmp = _factory->create_expression(special_constant(special_constant::T_NUMERAL, (unsigned long)-q));
      return _factory->create_expression(function_symbol::MINUS, tmp);	  
    }
}

template <>
inline 
expression arithmetic_canonizer<long>::canonize_decimal_constant(const expression & e)
{
  coeff_type q = (coeff_type) e->get_special_constant().get_ld_value();
  return get_expr_from_value(q);
}


template <>
inline
arithmetic_canonizer<int>::coeff_type arithmetic_canonizer<int>::get_value_from_expr(const expression & e)
{
  /* e is one of the following: n, (- n), d, (- d), (/ n m), (/ (- n) m) */
  
  if(e->is_numeral_constant())
    return (coeff_type) e->get_special_constant().get_ul_value();
  
  if(e->is_decimal_constant())
    return (coeff_type) e->get_special_constant().get_ld_value();
  
  if(e->get_symbol() == function_symbol::MINUS)
    {
      if(e->get_operands()[0]->is_numeral_constant())
	return -(coeff_type)e->get_operands()[0]->get_special_constant().get_ul_value();
      
      if(e->get_operands()[0]->is_decimal_constant())
	return -(coeff_type)e->get_operands()[0]->get_special_constant().get_ld_value();
    }
  assert(e->get_symbol() == function_symbol::SLASH);
  
  const expression_vector & ops = e->get_operands();
  
  if(ops[0]->is_numeral_constant())
    {
      coeff_type den = (coeff_type) ops[0]->get_special_constant().get_ul_value();
      coeff_type nom = (coeff_type) ops[1]->get_special_constant().get_ul_value();
      return den / nom;
    }
  else
    {
      coeff_type den = (coeff_type) ops[0]->get_operands()[0]->get_special_constant().get_ul_value();
      coeff_type nom = (coeff_type) ops[1]->get_special_constant().get_ul_value();
      return -(den / nom);
    }
}

template <>
inline
expression arithmetic_canonizer<int>::get_expr_from_value(const coeff_type & q)
{
  if(q >= 0)
    {
      return _factory->create_expression(special_constant(special_constant::T_NUMERAL, (unsigned long)q));
    }
  else
    {
      expression tmp = _factory->create_expression(special_constant(special_constant::T_NUMERAL, (unsigned long)-q));
      return _factory->create_expression(function_symbol::MINUS, tmp);	  
    }
}

template <>
inline 
expression arithmetic_canonizer<int>::canonize_decimal_constant(const expression & e)
{
  coeff_type q = (coeff_type) e->get_special_constant().get_ld_value();
  return get_expr_from_value(q);
}


#if defined(_HYBRID_INT_VALUE) || defined(_HYBRID_LONG_VALUE)  
#include "hybrid_types.hpp"

template <typename IntType>
hybrid_rational<IntType> get_rational_from_decimal(const std::string & value)
{
  bool fraction = false;
  unsigned frac_count = 0;
  std::string num_str, den_str;
  for(unsigned i = 0; i < value.size(); i++)
    {
      if(fraction)
	frac_count++;

      if(value[i] == '.')
	fraction = true;
      else
	num_str.push_back(value[i]);
    }

  den_str.push_back('1');
  while(frac_count--)
    {
      den_str.push_back('0');
    }
  
  hybrid_rational<IntType> ret_val(num_str, den_str);
  ret_val.canonicalize();
  return ret_val;
}
#endif

#ifdef _HYBRID_INT_VALUE

template <>
inline
arithmetic_canonizer< hybrid_rational<int> >::coeff_type
arithmetic_canonizer< hybrid_rational<int> >::get_value_from_expr(const expression & e)
{
  /* e is one of the following: n, (- n), d, (- d), (/ n m), (/ (- n) m) */

  if(e->is_numeral_constant())
    return hybrid_rational<int>(e->get_special_constant().get_value());

  if(e->is_decimal_constant())
    return get_rational_from_decimal<int>(e->get_special_constant().get_value());
    
  if(e->get_symbol() == function_symbol::MINUS)
    {
      if(e->get_operands()[0]->is_numeral_constant())
	return -hybrid_rational<int>(e->get_operands()[0]->get_special_constant().get_value());
      if(e->get_operands()[0]->is_decimal_constant())
	return -get_rational_from_decimal<int>(e->get_operands()[0]->get_special_constant().get_value());
    }
  assert(e->get_symbol() == function_symbol::SLASH);
  
  const expression_vector & ops = e->get_operands();

  std::string num;
  if(ops[0]->is_numeral_constant())
    {
      num = ops[0]->get_special_constant().get_value();
    }
  else
    {	
      num = "-";
      num += ops[0]->get_operands()[0]->get_special_constant().get_value();
    }
  const std::string & den = ops[1]->get_special_constant().get_value();
  coeff_type ret(num, den);
  ret.canonicalize();
  return ret;
}

template <>
inline
expression arithmetic_canonizer< hybrid_rational<int> >::get_expr_from_value(const coeff_type & q)
{
  const hybrid_integer<int> num = q.get_num();
  const hybrid_integer<int> den = q.get_den();
  
  expression num_exp, den_exp;
  
  if(num >= 0)
    {
      num_exp = _factory->create_expression(special_constant(special_constant::T_NUMERAL, num.get_str()));
    }
  else
    {
      expression tmp = _factory->create_expression(special_constant(special_constant::T_NUMERAL, num.get_str().substr(1)));
      num_exp = _factory->create_expression(function_symbol::MINUS, tmp);	  
    }
  
  if(den != 1)
    {
      den_exp = _factory->create_expression(special_constant(special_constant::T_NUMERAL, den.get_str())); 
      return _factory->create_expression(function_symbol::SLASH, num_exp, den_exp);
    }
  else
    return num_exp;
}

template <>
inline 
expression arithmetic_canonizer< hybrid_rational<int> >::canonize_decimal_constant(const expression & e)
{
  coeff_type q(get_rational_from_decimal<int>(e->get_special_constant().get_value()));
  return get_expr_from_value(q);
}
#endif

#ifdef _HYBRID_LONG_VALUE

template <>
inline
arithmetic_canonizer< hybrid_rational<long> >::coeff_type
arithmetic_canonizer< hybrid_rational<long> >::get_value_from_expr(const expression & e)
{
  /* e is one of the following: n, (- n), d, (- d), (/ n m), (/ (- n) m) */

  if(e->is_numeral_constant())
    return hybrid_rational<long>(e->get_special_constant().get_value());

  if(e->is_decimal_constant())
    return get_rational_from_decimal<long>(e->get_special_constant().get_value());
    
  if(e->get_symbol() == function_symbol::MINUS)
    {
      if(e->get_operands()[0]->is_numeral_constant())
	return -hybrid_rational<long>(e->get_operands()[0]->get_special_constant().get_value());
      if(e->get_operands()[0]->is_decimal_constant())
	return -get_rational_from_decimal<long>(e->get_operands()[0]->get_special_constant().get_value());
    }
  assert(e->get_symbol() == function_symbol::SLASH);
  
  const expression_vector & ops = e->get_operands();

  std::string num;
  if(ops[0]->is_numeral_constant())
    {
      num = ops[0]->get_special_constant().get_value();
    }
  else
    {	
      num = "-";
      num += ops[0]->get_operands()[0]->get_special_constant().get_value();
    }
  const std::string & den = ops[1]->get_special_constant().get_value();
  coeff_type ret(num, den);
  ret.canonicalize();
  return ret;

}

template <>
inline
expression arithmetic_canonizer< hybrid_rational<long> >::get_expr_from_value(const coeff_type & q)
{
  const hybrid_integer<long> num = q.get_num();
  const hybrid_integer<long> den = q.get_den();
  
  expression num_exp, den_exp;
  
  if(num >= 0)
    {
      num_exp = _factory->create_expression(special_constant(special_constant::T_NUMERAL, num.get_str()));
    }
  else
    {
      expression tmp = _factory->create_expression(special_constant(special_constant::T_NUMERAL, num.get_str().substr(1)));
      num_exp = _factory->create_expression(function_symbol::MINUS, tmp);	  
    }
  
  if(den != 1)
    {
      den_exp = _factory->create_expression(special_constant(special_constant::T_NUMERAL, den.get_str())); 
      return _factory->create_expression(function_symbol::SLASH, num_exp, den_exp);
    }
  else
    return num_exp;
}

template <>
inline 
expression arithmetic_canonizer< hybrid_rational<long> >::canonize_decimal_constant(const expression & e)
{
  coeff_type q(get_rational_from_decimal<long>(e->get_special_constant().get_value()));
  return get_expr_from_value(q);
}
#endif


#endif // _ARITHMETIC_CANONIZER_H
