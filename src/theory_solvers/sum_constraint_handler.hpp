/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2016,2021 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _SUM_CONSTRAINT_HANDLER_H
#define _SUM_CONSTRAINT_HANDLER_H

#include "constraint_handler.hpp"
#include "backjump_stack.hpp"
#include "linear_polynomial.hpp"
#include "arithmetic_canonizer.hpp"
#include <unordered_map>

class sum_constraint_handler : public constraint_handler {
  friend class algorithm_comparator;
public:
  typedef bool (sum_constraint_handler::*calc_bounds_ptr_type)(int, int &, std::unordered_map<expression, int> &);
private:
  typedef linear_polynomial_template< std::unordered_map<expression, int> > linear_polynomial;
  enum bound_type { B_LOWER, B_UPPER, B_BOTH, B_SPLIT };
  linear_polynomial _lp;
  bound_type _bt_pos;
  bound_type _bt_neg;
  int _bound_pos;
  int _bound_neg;

  struct propagation_info {
    bound_type bt;
    unsigned trail_size;
  };

  typedef std::unordered_map<expression, propagation_info> propagation_info_map;
  propagation_info_map _stored_info;
  bound_type _self_bt;
  sum_common_data * _common_data;

  expression_vector _pos_vars;
  expression_vector _neg_vars;

  std::vector<expression_vector> _pos_partitions;
  std::vector<expression_vector> _neg_partitions;
  
  typedef std::unordered_map<expression, expression_set> cache_map;
  cache_map _cached_intersections;
  bool _intersections_inited;
  expression _pos_covering_alldiff;
  bool _pos_covered;
  bool _all_coeffs_one;
  
  void propagate_literal(const expression & l, bound_type bt, unsigned trail_size);
  void generate_explanation(unsigned trail_size, bound_type bt, explanation & expl, const expression & l = expression());
  void generate_propagation_explanation(const expression & l, explanation & expl);
  void generate_conflict_explanation(bound_type bt, explanation & expl);
  void generate_self_explanation(explanation & expl);
  
  bool calculate_min(int & min);
  bool calculate_max(int & max);

  calc_bounds_ptr_type _calculate_bounds_min;
  calc_bounds_ptr_type _calculate_bounds_max;
  
  bool calculate_bounds_standard_min(int ubound, int & lp_min, std::unordered_map<expression, int> & cb);
  bool calculate_bounds_standard_max(int lbound, int & lp_max, std::unordered_map<expression, int> & cb);

  bool calculate_bounds_partition_min(int ubound, int & lp_min, std::unordered_map<expression, int> & cb);
  bool calculate_bounds_partition_max(int lbound, int & lp_max, std::unordered_map<expression, int> & cb);

  bool calculate_bounds_beldiceanu_min(int ubound, int & lp_min, std::unordered_map<expression, int> & cb);
  bool calculate_bounds_beldiceanu_max(int lbound, int & lp_max, std::unordered_map<expression, int> & cb);

  bool calculate_bounds_fast_min(int ubound, int & lp_min, std::unordered_map<expression, int> & cb);
  bool calculate_bounds_fast_max(int lbound, int & lp_max, std::unordered_map<expression, int> & cb);

  bool calculate_bounds_multifast_min(int ubound, int & lp_min, std::unordered_map<expression, int> & cb);
  bool calculate_bounds_multifast_max(int lbound, int & lp_max, std::unordered_map<expression, int> & cb);
  
  template <typename Comparator, typename Getter, typename Adjuster>
  void adjust(expression_vector & vars, int & val, std::unordered_map<expression, int> & correction);

  void bel_priority_min(expression_vector & vars, int & val, std::vector<std::pair<expression, int>> & matching);
  void bel_priority_max(expression_vector & vars, int & val, std::vector<std::pair<expression, int>> & matching);

  void fast_priority_min(expression_vector & vars, int & val,
			   std::vector<std::pair<expression, int>> & matching, std::vector<unsigned> & p);
  void fast_priority_max(expression_vector & vars, int & val,
			   std::vector<std::pair<expression, int>> & matching, std::vector<unsigned> & p);


  
  void init_alldiff_intersections();
  void find_disjoint_cliques(const expression_vector & vars, std::vector<expression_vector> & cliques);
  
  bool check_upper(int bound, unsigned trail_size);
  bool check_lower(int bound, unsigned trail_size);
  void check_split(int bound, unsigned trail_size);

  
  bool is_similar_constraint(const expression & c, bound_type & bt, int & bound);
  bool is_weaker(const bound_type bt1, const int bound1, const bound_type bt2, const int bound2);
  bool holds(const int min_value, const int max_value, const bound_type bt, const int bound);
  void set_self_bt(const int min_value, const int max_value, const bound_type bt, const int bound);
public:
  sum_constraint_handler(csp_theory_solver * th, const expression & l_pos, const expression & l_neg);

  linear_polynomial & lp()
  {
    return _lp;
  }
  
  virtual void new_level() {}
  virtual void backjump(unsigned level) {}
  virtual void assert_literal(const expression & l) {}
  virtual void check_and_propagate(unsigned layer);
  virtual void explain_literal(const expression & l);

  virtual int get_variable_value_hint(const expression & var);
  virtual bool is_weaker_from_pos(const expression & c);
  virtual bool is_weaker_from_neg(const expression & c);
  virtual void check_implied();
};

#endif // _SUM_CONSTRAINT_HANDLER_H
