/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2015,2021 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _ARITHMETIC_THEORY_SOLVER_H
#define _ARITHMETIC_THEORY_SOLVER_H

#include "theory_solver.hpp"
#include <queue>
#include <map>
#include <set>
#include <random>
#include "solver.hpp"
#include "linear_polynomial.hpp"
#include "arithmetic_canonizer.hpp"
#include "wall_clock.hpp"
#include "history_saver.hpp"

#if defined(_HYBRID_INT_VALUE) || defined(_HYBRID_LONG_VALUE)
#include "hybrid_types.hpp"
#endif


enum infinity_type { I_NONE, I_POSITIVE, I_NEGATIVE };

#ifdef _MPQ_VALUE
typedef mpq_class unknown_value;
typedef mpz_class unknown_integer_value;
#elif defined(_LONG_DOUBLE_VALUE)
typedef long double unknown_value;
typedef long unknown_integer_value;
#elif defined(_LONG_VALUE)
typedef long unknown_value;
typedef long unknown_integer_value;
#elif defined(_HYBRID_INT_VALUE)
typedef hybrid_rational<int> unknown_value;
typedef hybrid_integer<int> unknown_integer_value;
#elif defined(_HYBRID_LONG_VALUE)
typedef hybrid_rational<long> unknown_value;
typedef hybrid_integer<long> unknown_integer_value;
#endif

#ifdef _MPQ_VALUE
inline unknown_value && trunc_value(const unknown_value & value)
{
  static mpz_class temp;
  static mpq_class result;
  temp = value;
  result = temp;
  return std::move(result);
}
#elif defined(_LONG_DOUBLE_VALUE)
#include <cmath>
inline unknown_value trunc_value(const unknown_value & value)
{
  return trunc(value);
}
#elif defined(_LONG_VALUE)
inline unknown_value trunc_value(const unknown_value & value)
{
  return value;
}
#elif defined (_HYBRID_INT_VALUE)
inline unknown_value && trunc_value(const unknown_value & value)
{
  static hybrid_integer<int> temp;
  static hybrid_rational<int> result;
  temp = (hybrid_integer<int>)value;
  result = temp;
  return std::move(result);  
}
#elif defined (_HYBRID_LONG_VALUE)
inline unknown_value && trunc_value(const unknown_value & value)
{
  static hybrid_integer<long> temp;
  static hybrid_rational<long> result;
  temp = (hybrid_integer<long>)value;
  result = temp;
  return std::move(result);  
}
#endif

inline bool is_integer_value(const unknown_value & value)
{
  return trunc_value(value) == value;
}

inline unknown_value && floor_value(const unknown_value & value)
{
  static unknown_value retval;
  retval = trunc_value(value);
  if(retval != value && value < 0)
    {
      retval -= 1;
    }
  return std::move(retval);
}

inline unknown_value && ceil_value(const unknown_value & value)
{
  static unknown_value retval;
  retval = trunc_value(value);
  if(retval != value && value > 0)
    {
      retval += 1;
    }
  return std::move(retval);
}

struct value_pair : public std::pair<unknown_value, unknown_value> {

  typedef std::pair<unknown_value, unknown_value> base_class_type;

  infinity_type infinity;

  value_pair(infinity_type inf = I_NONE) : base_class_type(0,0), infinity(inf) {}
  value_pair(const unknown_value & f, const unknown_value & s = 0) : base_class_type(f, s), infinity(I_NONE) {}
  value_pair(unknown_value && f, unknown_value && s = 0) : base_class_type(std::move(f), std::move(s)), infinity(I_NONE) {}
  value_pair(unknown_value && f, const unknown_value & s = 0) : base_class_type(std::move(f), s), infinity(I_NONE) {}
  value_pair(const unknown_value & f, unknown_value && s = 0) : base_class_type(f, std::move(s)), infinity(I_NONE) {}
  
  void add_product(const value_pair & vp, const unknown_value & c);
};

inline
value_pair operator + (const value_pair & rp1, const value_pair & rp2) 
{
  return value_pair(rp1.first + rp2.first, rp1.second + rp2.second);
}

inline
value_pair operator - (const value_pair & rp)
{
  return value_pair(-rp.first, -rp.second);
}

inline
value_pair operator - (const value_pair & rp1, const value_pair & rp2)
{
  return value_pair(rp1.first - rp2.first, rp1.second - rp2.second);
}
  
inline
value_pair operator * (const unknown_value & alpha, const value_pair & rp) 
{
  return value_pair(alpha * rp.first, alpha * rp.second);
}  

inline
value_pair operator * (const value_pair & rp, const unknown_value & alpha) 
{
  return value_pair(alpha * rp.first, alpha * rp.second);
}  

inline
value_pair operator / (const value_pair & rp, const unknown_value & alpha)
{
  return value_pair(rp.first / alpha, rp.second / alpha);
}

inline
value_pair & operator += (value_pair & rp1, const value_pair & rp2)
{
  rp1.first += rp2.first;
  rp1.second += rp2.second;
  return rp1;
}

inline
value_pair & operator -= (value_pair & rp1, const value_pair & rp2)
{
  rp1.first -= rp2.first;
  rp1.second -= rp2.second;
  return rp1;
}

inline
value_pair & operator *= (value_pair & rp, const unknown_value & alpha)
{
  rp.first *= alpha;
  rp.second *= alpha;
  return rp;
}

inline
value_pair & operator /= (value_pair & rp, const unknown_value & alpha)
{
  rp.first /= alpha;
  rp.second /= alpha;
  return rp;
}

inline
bool operator == (const value_pair & rp1, const value_pair & rp2)
{
  return rp1.infinity == rp2.infinity && 
    (rp1.infinity != I_NONE || (rp1.first == rp2.first && rp1.second == rp2.second));
}

inline
bool operator != (const value_pair & rp1, const value_pair & rp2)
{
  return !(rp1 == rp2);
}

inline
bool operator <= (const value_pair & rp1, const value_pair & rp2)
{
  return rp1.infinity == I_NEGATIVE || rp2.infinity == I_POSITIVE ||  
    (rp1.infinity == I_NONE && rp2.infinity == I_NONE && 
     (rp1.first < rp2.first || (rp1.first == rp2.first && rp1.second <= rp2.second)));
}

inline
bool operator >= (const value_pair & rp1, const value_pair & rp2)
{
  return rp2 <= rp1;
}

inline
bool operator < (const value_pair & rp1, const value_pair & rp2)
{
  return rp1 <= rp2 && rp1 != rp2;
}

inline
bool operator > (const value_pair & rp1, const value_pair & rp2)
{
  return rp1 >= rp2 && rp1 != rp2;
}

inline
void value_pair::add_product(const value_pair & vp, const unknown_value & c)
{
  static value_pair temp;
  temp = vp;
  temp *= c;
  *this += temp;
}


inline 
std::ostream & operator << (std::ostream & ostr, const value_pair & rp)
{
  if(rp.infinity == I_NONE)
    ostr << "(" << rp.first << "," << rp.second << ")";
  else if(rp.infinity == I_NEGATIVE)
    ostr << "-inf";
  else
    ostr << "+inf";
  return ostr;
}

#ifdef _MPQ_VALUE
inline
void swap(unknown_value & q1, unknown_value & q2)
{
  mpq_swap(q1.get_mpq_t(), q2.get_mpq_t());
}
#elif !defined(_HYBRID_INT_VALUE) && !defined(_HYBRID_LONG_VALUE)
inline
void swap(unknown_value & q1, unknown_value & q2)
{
  std::swap(q1, q2);
}
#endif

inline
void swap(value_pair & p1, value_pair & p2)
{
  swap(p1.first, p2.first);
  swap(p1.second, p2.second);
  std::swap(p1.infinity, p2.infinity);
}


template<>
inline
linear_polynomial_template< std::unordered_map<expression, unknown_value> >::linear_polynomial_template()
  :_map(HASH_TABLE_SIZE)
{}
typedef linear_polynomial_template< std::unordered_map<expression, unknown_value> > hashed_linear_polynomial;
typedef linear_polynomial_template< std::map<expression, unknown_value> > ordered_linear_polynomial;
typedef hashed_linear_polynomial linear_polynomial;

class arithmetic_theory_solver : public theory_solver, private arithmetic_canonizer<unknown_value> {
private:
  enum bound_type {
    B_LOWER, B_UPPER, B_BOTH, B_SPLIT
  };
  
  struct constraint {
    value_pair bound;
    bound_type type; 
    
    constraint() {}
    constraint(value_pair && b, bound_type t)
      :bound(std::move(b)), type(t)
    {}

    bool implies(const constraint & c) const
    {
      switch(type)
	{
	case B_LOWER:
	  return (c.type == B_LOWER && c.bound <= bound) || (c.type == B_SPLIT && c.bound < bound);
	  break;
	case B_UPPER:
	  return (c.type == B_UPPER && c.bound >= bound) || (c.type == B_SPLIT && c.bound > bound);
	  break;
	case B_BOTH:
	  return (c.type == B_BOTH && c.bound == bound) || (c.type == B_SPLIT && c.bound != bound) 
	    || (c.type == B_UPPER && c.bound >= bound) || (c.type == B_LOWER && c.bound <= bound);
	  break;
	case B_SPLIT:
	  return c.type == B_SPLIT && c.bound == bound;
	  break;
	}
      return false;
    }

    bool implied_by(const constraint & c) const
    {
      return c.implies(*this);
    }

    bool conflicts_with(const constraint & c) const
    {
      switch(type)
	{
	case B_LOWER:
	  return (c.type == B_UPPER && c.bound < bound) || (c.type == B_BOTH && c.bound < bound);
	  break;
	case B_UPPER:
	  return (c.type == B_LOWER && c.bound > bound) || (c.type == B_BOTH && c.bound > bound);
	  break;
	case B_BOTH:
	  return (c.type == B_BOTH && c.bound != bound) || (c.type == B_LOWER && c.bound > bound) 
	    || (c.type == B_UPPER && c.bound < bound) || (c.type == B_SPLIT && c.bound == bound);
	  break;
	case B_SPLIT:
	  return c.type == B_BOTH && c.bound == bound;
	  break;
	}
      return false;
    }

    bool operator == (const constraint & c) const
    {
      return c.bound == bound && c.type == type;
    }

    bool operator < (const constraint & c) const
    {
      return bound < c.bound || (bound == c.bound && type < c.type);
    }
    
    bool operator > (const constraint & c) const
    {
      return bound > c.bound || (bound == c.bound && type > c.type);
    }
    
    bool operator <= (const constraint & c) const
    {
      return bound < c.bound || (bound == c.bound && type <= c.type);
    }

    bool operator >= (const constraint & c) const
    {
      return bound > c.bound || (bound == c.bound && type >= c.type);
    }
  };
  
  class arithmetic_theory_solver_data : public expression_data {
  private:

    struct bound_data {
      value_pair bound;
      expression relation;
      bound_data(infinity_type inf = I_NONE) : bound(inf) {}
    };
    
    struct unknown_data {
      linear_polynomial _row;
      value_pair _valuation;
      history_saver<bound_data> _lower_bounds;
      history_saver<bound_data> _upper_bounds;
      value_pair _calculated_lower_bound;
      value_pair _calculated_upper_bound;
      expression _lower_bu;
      expression _upper_bu;
      expression_vector _constraints;
      unsigned _first_not_inserted_constraint_index;
      unsigned _index;
      bool _is_basic;
      bool _is_integer;
      bool _is_eq_relevant;

      unknown_data(const unsigned & level, unsigned index, bool is_integer)
	:_lower_bounds(level, bound_data(I_NEGATIVE)),
	 _upper_bounds(level, bound_data(I_POSITIVE)),
	 _first_not_inserted_constraint_index(0),
	 _index(index),
	 _is_basic(false),
	 _is_integer(is_integer),
	 _is_eq_relevant(false)
      {}
    };

    struct constraint_data  {
      expression _unknown;
      constraint _constraint;
      bool _split_learned;
      explanation _expl;

      constraint_data() {}
      constraint_data(const expression & u, value_pair && b, bound_type t)
	:_unknown(u),
	 _constraint(std::move(b), t),
	 _split_learned(false)
      {}
    };
    
    constraint_data * _constraint_data;
    unknown_data * _unknown_data;

  public:
    arithmetic_theory_solver_data()
      :_constraint_data(0),
       _unknown_data(0)
    {}

    arithmetic_theory_solver_data(const unsigned & level, unsigned index, bool is_integer = false)
      :_constraint_data(0),
       _unknown_data(new unknown_data(level, index, is_integer))
    {}

    arithmetic_theory_solver_data(const expression & u, value_pair && b, bound_type t)
      :_constraint_data(new constraint_data(u, std::move(b), t)),
       _unknown_data(0)
    {}
    
    ~arithmetic_theory_solver_data()
    {
      delete _unknown_data;
      delete _constraint_data;
    }

    bool has_unknown_data() const
    {
      return _unknown_data != 0;
    }

    bool has_constraint_data() const
    {
      return _constraint_data != 0;
    }

    linear_polynomial & row()
    {
      return _unknown_data->_row;
    }

    bool is_integer() const
    {
      return _unknown_data->_is_integer;
    }

    bool is_eq_relevant() const
    {
      return _unknown_data->_is_eq_relevant;
    }

    void set_eq_relevant(bool value)
    {
      _unknown_data->_is_eq_relevant = value;
    }

    const value_pair & get_valuation() const
    {
      return _unknown_data->_valuation;
    }

    void set_valuation(const value_pair & value)
    {
      _unknown_data->_valuation = value;
    }

    void set_valuation(value_pair && value)
    {
      _unknown_data->_valuation = std::move(value);
    }

    void add_to_valuation(const value_pair & value)
    {
      _unknown_data->_valuation += value;
    }

    const value_pair & get_lower_bound() const
    {
      return _unknown_data->_lower_bounds.const_ref().bound;
    }

    const expression & get_lower_bound_relation() const
    {
      return _unknown_data->_lower_bounds.const_ref().relation;
    }

    void set_lower_bound(const value_pair & value, const expression & rel)
    {
      _unknown_data->_lower_bounds.ref().bound = value;
      _unknown_data->_lower_bounds.ref().relation = rel;
    }

    void set_lower_bound(value_pair && value, const expression & rel)
    {
      _unknown_data->_lower_bounds.ref().bound = std::move(value);
      _unknown_data->_lower_bounds.ref().relation = rel;
    }
    
    void reset_lower_bound()
    {
      _unknown_data->_lower_bounds.ref().bound = value_pair(I_NEGATIVE);
    }

    const value_pair & get_upper_bound() const
    {
      return _unknown_data->_upper_bounds.const_ref().bound;
    }


    const expression & get_upper_bound_relation() const
    {
      return _unknown_data->_upper_bounds.const_ref().relation;
    }


    void set_upper_bound(const value_pair & value, const expression & rel)
    {
      _unknown_data->_upper_bounds.ref().bound = value;
      _unknown_data->_upper_bounds.ref().relation = rel;      
    }

    void set_upper_bound(value_pair && value, const expression & rel)
    {
      _unknown_data->_upper_bounds.ref().bound = std::move(value);
      _unknown_data->_upper_bounds.ref().relation = rel;      
    }
    
    void reset_upper_bound()
    {
      _unknown_data->_upper_bounds.ref().bound = value_pair(I_POSITIVE);
    }

    value_pair & calculated_lower_bound()
    {
      return _unknown_data->_calculated_lower_bound;
    }

    value_pair & calculated_upper_bound()
    {
      return _unknown_data->_calculated_upper_bound;
    }

    expression & upper_bu()
    {
      return _unknown_data->_upper_bu;
    }

    expression & lower_bu()
    {
      return _unknown_data->_lower_bu;
    }
    
    const expression_vector & get_constraints() const
    {
      return _unknown_data->_constraints;
    }

    void add_constraint(const expression & c)
    {
      _unknown_data->_constraints.push_back(c);
    }

    void sort_constraints(arithmetic_theory_solver * theory_solver)
    {
      expression_vector & csts = _unknown_data->_constraints;
      unsigned & index = _unknown_data->_first_not_inserted_constraint_index;
      
      for(; index < csts.size(); index++)
	{
	  expression c = csts[index];
	  const constraint & c_cst = theory_solver->_data.get_data(c)->get_constraint();
	  int i;
	  for(i = (int)index - 1; i >= 0 && 
		c_cst < theory_solver->_data.get_data(csts[i])->get_constraint(); i--)
	    csts[i + 1] = csts[i];
	  csts[i + 1] = c;
	}
    }
    
    unsigned get_index() const
    {
      return _unknown_data->_index;
    }

    void set_index(unsigned index)
    {
      _unknown_data->_index = index;
    }

    void set_basic(bool value)
    {
      _unknown_data->_is_basic = value;
    }

    bool is_basic() const
    {
      return _unknown_data->_is_basic;
    }

    const expression & get_unknown() const
    {
      return _constraint_data->_unknown;
    }

    const constraint & get_constraint() const
    {
      return _constraint_data->_constraint;
    }

    bool is_split_learned() const
    {
      return _constraint_data->_split_learned;
    }

    void set_split_learned(bool value) 
    {
      _constraint_data->_split_learned = value;
    }

    explanation & literal_explanation()
    {
      return _constraint_data->_expl;
    }

    void restore_state()
    {
      if(_unknown_data != 0)
	{
	  _unknown_data->_lower_bounds.restore_state();
	  _unknown_data->_upper_bounds.restore_state();
	}
    }
  };


  struct unknown_bounds {
    value_pair lower;
    expression lower_bu;
    value_pair upper;
    expression upper_bu;
    unknown_bounds()
      :lower(I_NEGATIVE),
       upper(I_POSITIVE)
    {}
  };

  typedef std::unordered_map<expression, unknown_bounds> bounds_map;
    
  class unknown_comparator {
  private:
    arithmetic_theory_solver * _theory_solver;
  public:
    unknown_comparator(arithmetic_theory_solver * theory_solver)
      :_theory_solver(theory_solver)
    {}

    bool operator () (const expression & e1, const expression & e2) const
    {
      return _theory_solver->_data.get_data(e1)->get_index() < _theory_solver->_data.get_data(e2)->get_index();
    }
  };
  
  typedef std::set<expression, unknown_comparator> unknown_set;

  class basic_unknown_selector {
  public:
    virtual void enter_check() = 0;
    virtual bool select_basic_unknown(expression & bu) = 0;
    virtual ~basic_unknown_selector() {}
  };

  class first_basic_unknown_selector : public basic_unknown_selector {
  private:
    arithmetic_theory_solver * _theory_solver;
  public:
    first_basic_unknown_selector(arithmetic_theory_solver * th)
      :_theory_solver(th)
    {}

    virtual void enter_check()
    {}
    
    virtual bool select_basic_unknown(expression & bu)
    {
      return _theory_solver->find_first_basic_unknown(bu);
    }

  };

  class closest_basic_unknown_selector : public basic_unknown_selector {
  private:
    arithmetic_theory_solver * _theory_solver;
    unsigned long long _counter;
    unsigned long long _limit;

  public:
    closest_basic_unknown_selector(arithmetic_theory_solver * th, unsigned lmt = 0)
      :_theory_solver(th),
       _counter(0),
       _limit(lmt)
    {}

    virtual void enter_check()
    {
      _counter = 0;
      _limit = _theory_solver->_non_basic_unknowns.size();
    }

    void set_limit(unsigned lmt)
    {
      _limit = lmt;
    }
    
    virtual bool select_basic_unknown(expression & bu);
  };


  
  class non_basic_unknown_selector {
  public:
    virtual void enter_check() = 0;
    virtual bool select_non_basic_unknown(const linear_polynomial & lp, bool lb_violated, expression & nbu) = 0;
    virtual ~non_basic_unknown_selector() {}
  };

  class first_non_basic_unknown_selector : public non_basic_unknown_selector {
  private:
    arithmetic_theory_solver * _theory_solver;
  public:
    first_non_basic_unknown_selector(arithmetic_theory_solver * th)
      :_theory_solver(th)
    {}
        
    virtual void enter_check()
    {}
    
    virtual bool select_non_basic_unknown(const linear_polynomial & lp, bool lb_violated, expression & nbu)
    {  
      return _theory_solver->find_first_non_basic_unknown(lp, lb_violated, nbu);
    }
  };

  class least_frequent_non_basic_unknown_selector : public non_basic_unknown_selector {
  private:
    arithmetic_theory_solver * _theory_solver;
    unsigned long long _counter;
    unsigned long long _limit;    
  public:
    least_frequent_non_basic_unknown_selector(arithmetic_theory_solver * th, unsigned lmt = 0)
      :_theory_solver(th),
       _counter(0),
       _limit(lmt)       
    {}

    void set_limit(unsigned lmt)
    {
      _limit = lmt;
    }
    
    virtual void enter_check()
    {
      _counter = 0;
      _limit = _theory_solver->_non_basic_unknowns.size();
    }
    
    virtual bool select_non_basic_unknown(const linear_polynomial & lp, bool lb_violated, expression & nbu);
  };


  struct nbu_lower_comp {
    arithmetic_theory_solver * _theory_solver;
    nbu_lower_comp(arithmetic_theory_solver * th) :_theory_solver(th) {}
    bool operator () (const value_pair & bound1, const expression & bu1,
		      const value_pair & bound2, const expression & bu2) const
    {
      return bound1 > bound2 || (bound1 == bound2 &&
				 bound1.infinity == I_NONE && 
				 _theory_solver->_data.get_data(bu1)->get_index() <
				 _theory_solver->_data.get_data(bu2)->get_index());     
    }
  };

  struct nbu_upper_comp {
    arithmetic_theory_solver * _theory_solver;
    nbu_upper_comp(arithmetic_theory_solver * th) :_theory_solver(th) {}
    bool operator () (const value_pair & bound1, const expression & bu1,
		      const value_pair & bound2, const expression & bu2) const
    {
      return bound1 < bound2 || (bound1 == bound2 &&
				 bound1.infinity == I_NONE && 
				 _theory_solver->_data.get_data(bu1)->get_index() <
				 _theory_solver->_data.get_data(bu2)->get_index());     
    }
  };

  
  data_extractor<arithmetic_theory_solver_data> _data;
  unknown_set _basic_unknowns;
  unknown_set _non_basic_unknowns;
  std::unordered_map<expression, bool> _inactive_unknowns;
  expression_vector _all_unknowns;
  expression_vector _eq_relevant_unknowns;
  expression_vector _integer_unknowns;
  unsigned _current_level;
  expression_vector _private_expressions;
  unknown_value _delta;
  bool _delta_calculated;
  expression_vector _trivial_propagations;
  expression_vector _new_trivial_propagations;
  unsigned _check_count;
  const unsigned _check_period;
  unsigned _bound_refinements_count;
  const unsigned _bound_ref_period;
  const bool _eq_intro;
  unsigned _current_assertion_pos;
  bool _just_backjumped;
  bool _bound_changed;
  bool _disequality_asserted;
  expression_set _learnt_integer_splits;
  bool _shuffled;
  std::mt19937 _rng;

  basic_unknown_selector * _bu_selector;
  non_basic_unknown_selector * _nbu_selector;
  
  wall_clock _check_and_prop_time_spent;
  wall_clock _parallel_time_spent;
  wall_clock _backjump_time_spent;
  wall_clock _new_level_time_spent;
  wall_clock _explain_time_spent;
  wall_clock _check_int_time_spent;
  wall_clock _check_eq_time_spent;
  wall_clock _process_assertions_time_spent;
  wall_clock _bound_refinments_time_spent;
  wall_clock _check_time_spent;
  wall_clock _weaker_time_spent;
  wall_clock _pivot_and_update_time_spent;
  wall_clock _exp_gen_time_spent;
  
  // Tableau statistics
  double _curr_density_percent;
  double _cum_density_percent;
  unknown_integer_value _curr_max_num;
  unknown_integer_value _curr_max_den;
  unknown_integer_value _cum_max_num;
  unknown_integer_value _cum_max_den;
  unsigned _stat_calls;
  unsigned long long _pivot_call_count;
  unsigned long long _check_call_count;
  
#if defined(_HYBRID_INT_VALUE) || defined(_HYBRID_LONG_VALUE)
  bool _is_tableau_gmp = false;
#endif

  void add_private_expression(const expression & e)
  {
    _private_expressions.push_back(e);
    _solver.init_expression_data(e);
  }

  void clear_private_expressions()
  {
    for(unsigned i = 0; i < _private_expressions.size(); i++)
      _solver.clear_expression_data(_private_expressions[i]);
    _private_expressions.clear();
  }

  bool is_lra_term(const expression & e)
  {
    common_data * e_common = _solver.get_common_data(e);
    return e_common->get_owner() == this || e_common->is_shared();
  }

  value_pair calculate_polynomial_value(const linear_polynomial & lp);
  void expand_polynomial(linear_polynomial & lp);
  void init_monomial_unknowns(const linear_polynomial & lp);
  void invalidate_inactive_unknowns();
  const value_pair & recalculate_unknown_value(const expression & unknown);
  void init_unknown(const expression & unknown);
  void activate_unknown(const expression & unknown);
  void search_unknowns_rec(const expression & e);
  void found_trivial_propagation(const unknown_value & c, const function_symbol & fs, 
				 expression l_pos, expression l_neg);
  void apply_trivial_propagation(const expression & l);
  void process_assertion(const expression & l);
  void process_assertions();
  void pivot(const expression & bu, const expression & nbu);
  void update(const expression & nbu, const value_pair & v);
  void pivot_and_update(const expression & bu, const expression & nbu, const value_pair & v);
  bool find_first_non_basic_unknown(const linear_polynomial & lp, bool lb_violated, expression & first);
  bool find_first_basic_unknown(expression & first);
  bool check(expression & bu);
  void assert_upper(const expression & l);
  void assert_lower(const expression & l);
  void assert_both(const expression & l);
  void assert_split(const expression & l);
  void split_on_demand(const expression & l);
  void tighten_upper_integer_bound(value_pair & bound);
  void tighten_lower_integer_bound(value_pair & bound);
  void tighten_integer_bound(value_pair & bound, bound_type bt);
  void calculate_bounds(const expression & bu, bounds_map & bmap);
  void propagate_bound_refinements();
  void propagate_weaker_constraints(const expression & unknown, const constraint & c);
  void introduce_equalities();
  int find_constraint_r(const expression_vector & csts, const constraint & cst);
  int find_constraint_l(const expression_vector & csts, const constraint & cst);
  void check_equalities();
  bool check_integer_unknowns();
  void generate_bound_explanation(const expression & unknown, bound_type bt, explanation & expl);
  bool generate_simple_explanation(const expression & e, explanation & expl);
  void generate_propagation_explanation(const expression & e, const constraint & sc, explanation & expl);
  void generate_conflict_explanation(const expression & bu, explanation & expl);


  void add_eq_relevant_unknown(const expression & unknown)
  {
    arithmetic_theory_solver_data * u_data = _data.get_data(unknown);
    if(!u_data->is_eq_relevant())
      {
	_eq_relevant_unknowns.push_back(unknown);
	u_data->set_eq_relevant(true);
      }
  }

  bool is_fixed_by_constraints(const expression & unknown)
  {
    arithmetic_theory_solver_data * u_data = _data.get_data(unknown);
    return u_data->get_lower_bound() == u_data->get_upper_bound();
  }

  bool is_poly_fixed_by_constraints(const expression & unknown)
  {
    arithmetic_theory_solver_data * u_data = _data.get_data(unknown);
    const linear_polynomial & lp = u_data->row();
    for(linear_polynomial::const_iterator it = lp.begin(), it_end = lp.end();
	it != it_end; ++it)
      {
	if(it->first == _one_expression)
	  continue;

	if(!is_fixed_by_constraints(it->first))
	  return false;
      }
    return true;
  }

  void calculate_delta();

  unknown_value get_valuation(const expression & unknown)
  {
    const value_pair & val = _data.get_data(unknown)->get_valuation();
    return val.first + _delta * val.second;
  }

  void shuffle_unknowns();

  /* Invariant checking (debug only) */
  value_pair get_strongest_lower_bound_on_trail(arithmetic_theory_solver_data * data);
  value_pair get_strongest_upper_bound_on_trail(arithmetic_theory_solver_data * data);
  void expand_to_lra_unknowns(linear_polynomial & lp);
  bool check_invariant_nb1();
  bool check_invariant_nb2();
  bool check_invariant_nb3();
  bool check_invariant_nb4();
  bool check_invariant_nb5();
  bool check_invariant_b1();
  bool check_invariant_b2();
  bool check_invariant_b3();
  bool check_invariant_b4();
  bool check_invariant_b5();
  bool check_all_invariants();
    
  void print_tableau();
  void get_tableau_statistics();
  void tableau_report(std::ostream & ostr);
  
public:
  arithmetic_theory_solver(solver & sl, const std::string & buss = "closest",
		    const std::string & nbuss = "least_frequent",
		    unsigned check_period = 13, bool eq_intro = true,
		    unsigned bound_ref_period = 0, bool should_shuffle = false,
		    unsigned shuffle_seed = 0)
    :theory_solver(sl, "Arithmetic", 2),
     arithmetic_canonizer<unknown_value>(sl.get_factory()),
    _data(sl.extractor_counter()),
    _basic_unknowns(unknown_comparator(this)),
    _non_basic_unknowns(unknown_comparator(this)),
    _inactive_unknowns(HASH_TABLE_SIZE),
    _current_level(0),
    _delta_calculated(false),
    _check_count(0),
    _check_period(check_period),
    _bound_refinements_count(0),
    _bound_ref_period(bound_ref_period),
    _eq_intro(eq_intro),
    _current_assertion_pos(0),
    _just_backjumped(false),
    _bound_changed(true),
    _disequality_asserted(true),
    _learnt_integer_splits(HASH_TABLE_SIZE),
    _shuffled(!should_shuffle),
    _rng(shuffle_seed),
    _bu_selector(0),
    _nbu_selector(0),
    _curr_density_percent(0.0),
    _cum_density_percent(0.0),
    _curr_max_num(0),
    _curr_max_den(0),
    _cum_max_num(0),
    _cum_max_den(0),
    _stat_calls(0),
    _pivot_call_count(0),
    _check_call_count(0)
  {
    if(buss == "first")
      _bu_selector = new first_basic_unknown_selector(this);
    else if(buss == "closest")
      _bu_selector = new closest_basic_unknown_selector(this);
    else
      {
	std::cerr << "CONFIGURATION ERROR: Unknown basic selection strategy: " << buss << std::endl;
	exit(1);
      }
    
    if(nbuss == "first")
      _nbu_selector = new first_non_basic_unknown_selector(this);
    else if(nbuss == "least_frequent")
      _nbu_selector = new least_frequent_non_basic_unknown_selector(this);
    else
      {
	std::cerr << "CONFIGURATION ERROR: Unknown non-basic selection strategy: " << nbuss << std::endl;
	exit(1);
      }
  }
      
  virtual void add_literal(const expression & l_pos, const expression & l_neg);
  virtual void new_level();
  virtual void backjump(unsigned level); 

  virtual void check_and_propagate(unsigned layer);
  
  virtual void explain_literal(const expression & l);
  
  virtual bool is_owned_expression(const expression & e)
  {
    return e->is_numeral_constant() || e->is_decimal_constant() || is_arithmetic(e)
      || (e->is_constant_function() && e->get_inferred_sort()->get_symbol() == sort_symbol::REAL)
      || (e->is_constant_function() && e->get_inferred_sort()->get_symbol() == sort_symbol::INT);
  }
  
  virtual expression canonize_expression(const expression & e);

  virtual void get_literal_pair(const expression & l, expression & l_pos, expression & l_neg);

  virtual bool verify_assignment(const trail & tr)
  {
    // TODO
    return true;
  }

  virtual void get_model(const expression_vector & exps);

  virtual ~arithmetic_theory_solver()
  {
    delete _bu_selector;
    delete _nbu_selector;
    clear_private_expressions();
  }

  virtual void print_report(std::ostream & ostr)
  {
    ostr << "-------- ARITHMETIC THEORY_SOLVER ---------" << std::endl;
    ostr << "Time spent in check_and_propagate: " << _check_and_prop_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in process_assertions: " << _process_assertions_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in check: " << _check_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in pivot_and_update: " << _pivot_and_update_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in check_integer_unknowns: " << _check_int_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in check_equalities: " << _check_eq_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in bound_refinments: " << _bound_refinments_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in weaker constraints check: " << _weaker_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in parallel processing: " << _parallel_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in new_level: " << _new_level_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in backjump: " << _backjump_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in explain_literal: " << _explain_time_spent.cumulative_time() << std::endl;
    ostr << "Time spent in generate_explanation: " << _exp_gen_time_spent.cumulative_time() << std::endl;
    get_tableau_statistics();
    tableau_report(ostr);
    /*    ostr << "UNKNOWNS: " << _all_unknowns.size() << std::endl;
    unsigned count_unconstrained = 0;
    for(unsigned i = 0; i < _all_unknowns.size(); i++)
      {
	if(_inactive_unknowns.find(_all_unknowns[i]) != _inactive_unknowns.end())
	  continue;

	arithmetic_theory_solver_data * cdata = _data.get_data(_all_unknowns[i]);
	count_unconstrained += cdata->get_constraints().empty() ? 1 : 0;
	if(cdata->get_constraints().empty())
	  std::cout << "  --- " << _all_unknowns[i] << " : " << cdata->is_basic() << " : " << cdata->get_lower_bound() << " , " << cdata->get_upper_bound() << std::endl;       	
      }
    std::cout << "UNCONSTRAINED: " << count_unconstrained << std::endl;

    for(expression u : _inactive_unknowns)
      {
	std::cout << _data.get_data(u)->get_constraints().size() << " : " << _data.get_data(u)->get_lower_bound() << " , " << _data.get_data(u)->get_upper_bound() << std::endl;
      }
    */
  }
};

#endif // _ARITHMETIC_THEORY_SOLVER_H
