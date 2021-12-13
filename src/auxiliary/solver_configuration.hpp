/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2016 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _SOLVER_CONFIGURATION_H
#define _SOLVER_CONFIGURATION_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>

class solver_configuration {
private:
  bool _randomize_decide;
  unsigned _random_seed;
  double _random_decides_percent;
  unsigned _random_decides_after_restart;
  bool _randomize_simplex;
  unsigned _simplex_shuffle_seed;
  std::string _basic_unknown_selection_strategy;
  std::string _non_basic_unknown_selection_strategy;
  std::string _polarity_selection_strategy;
  unsigned _random_polarity_seed;
  double _random_polarity_probability;
  std::string _alldiff_sum_algorithm;
  bool _predefined_csp_bounds;
  int _lower_predefined_csp_bound;
  int _upper_predefined_csp_bound;
  unsigned _quantifier_instantiation_count_limit;
  unsigned _quantifier_instantiation_term_size_limit;
  
  static int skip_spaces(std::istream & istr)
  {
    int c;
    while((c = istr.get()) == ' ' || c == '\t' || c == '\n');
    return c;

  }
  
  static void skip_rest_of_line(std::istream & istr)
  {
    int c;
    while((c = istr.get()) != '\n' && c != EOF);
  }

  
  static bool get_next_assignment(std::istream & istr, std::string & key, std::string & value);
    

  template <typename T>
  void set_value(T & var, const T & val)
  {   
    var = val;
  }
  
public:
  solver_configuration()
    :_randomize_decide(true),
     _random_seed(1),
     _random_decides_percent(0.05),
     _random_decides_after_restart(0),
     _randomize_simplex(false),
     _simplex_shuffle_seed(0),
     _basic_unknown_selection_strategy("closest"),
     _non_basic_unknown_selection_strategy("least_frequent"),
     _polarity_selection_strategy("saved"),
     _random_polarity_seed(0),
     _random_polarity_probability(0.5),
     _alldiff_sum_algorithm("none"),
     _predefined_csp_bounds(false),
     _lower_predefined_csp_bound(0),
     _upper_predefined_csp_bound(0),
     _quantifier_instantiation_count_limit((unsigned)(-1)),
     _quantifier_instantiation_term_size_limit((unsigned)(-1))
  {}

  static void print_option_list(std::ostream & ostr);
  
  void parse_config_file(std::istream & istr);
  
  bool randomize_decide() const
  {
    return _randomize_decide;
  }

  unsigned random_seed() const
  {
    return _random_seed;
  }

  double random_decides_percent() const
  {
    return _random_decides_percent;
  }
  
  unsigned random_decides_after_restart() const
  {
    return _random_decides_after_restart;
  }

  bool randomize_simplex() const
  {
    return _randomize_simplex;
  }

  unsigned simplex_shuffle_seed() const
  {
    return _simplex_shuffle_seed;
  }

  const std::string & basic_unknown_selection_strategy() const
  {
    return _basic_unknown_selection_strategy;
  }

  const std::string & non_basic_unknown_selection_strategy() const
  {
    return _non_basic_unknown_selection_strategy;
  }

  const std::string & polarity_selection_strategy() const
  {
    return _polarity_selection_strategy;
  }

  unsigned random_polarity_seed() const
  {
    return _random_polarity_seed;
  }

  double random_polarity_probability() const
  {
    return _random_polarity_probability;
  }

  const std::string & alldiff_sum_algorithm() const
  {
    return _alldiff_sum_algorithm;
  }

  bool predefined_csp_bounds() const
  {
    return _predefined_csp_bounds;
  }

  int lower_predefined_csp_bound() const
  {
    return _lower_predefined_csp_bound;
  }

  int upper_predefined_csp_bound() const
  {
    return _upper_predefined_csp_bound;
  }

  unsigned quantifier_instantiation_count_limit() const
  {
    return _quantifier_instantiation_count_limit;
  }

  unsigned quantifier_instantiation_term_size_limit() const
  {
    return _quantifier_instantiation_term_size_limit;
  }
};

#endif // _SOLVER_CONFIGURATION_H
