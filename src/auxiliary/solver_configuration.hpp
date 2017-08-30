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
  unsigned _num_of_threads;
  unsigned _num_of_solvers;
  std::vector<bool> _randomize_decide;
  std::vector<unsigned> _random_seed;
  std::vector<double> _random_decides_percent;
  std::vector<unsigned> _random_decides_after_restart;
  std::vector<bool> _randomize_simplex;
  std::vector<unsigned> _simplex_shuffle_seed;
  std::vector<std::string> _basic_unknown_selection_strategy;
  std::vector<std::string> _non_basic_unknown_selection_strategy;
  std::vector<unsigned> _share_size_limit;
  std::vector<std::string> _polarity_selection_strategy;
  std::vector<unsigned> _random_polarity_seed;
  std::vector<double> _random_polarity_probability;
  std::vector<std::string> _alldiff_sum_algorithm;
  std::vector<bool> _predefined_csp_bounds;
  std::vector<int> _lower_predefined_csp_bound;
  std::vector<int> _upper_predefined_csp_bound;
  
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

  
  static bool get_next_assignment(std::istream & istr, std::string & key, int & index, std::string & value);
    
  void resize_vectors()
  {
    _randomize_decide.resize(_num_of_solvers, true);
    _random_seed.resize(_num_of_solvers, 1);
    _random_decides_percent.resize(_num_of_solvers, 0.05);
    _random_decides_after_restart.resize(_num_of_solvers, 0);
    _randomize_simplex.resize(_num_of_solvers, false);
    _simplex_shuffle_seed.resize(_num_of_solvers, 0);
    _basic_unknown_selection_strategy.resize(_num_of_solvers, "closest");
    _non_basic_unknown_selection_strategy.resize(_num_of_solvers, "least_frequent");
    _share_size_limit.resize(_num_of_solvers, 0);
    _polarity_selection_strategy.resize(_num_of_solvers, "saved");
    _random_polarity_seed.resize(_num_of_solvers, 0);
    _random_polarity_probability.resize(_num_of_solvers, 0.5);
    _alldiff_sum_algorithm.resize(_num_of_solvers, "none");
    _predefined_csp_bounds.resize(_num_of_solvers, false);
    _lower_predefined_csp_bound.resize(_num_of_solvers, 0);
    _upper_predefined_csp_bound.resize(_num_of_solvers, 0);
  }

  template <typename T>
  void set_value(std::vector<T> & vec, const T & val, int index)
  {
    // std::cout << "Setting value: " << val << " on index: " << index << std::endl;
    if(index == -1)
      vec.assign(_num_of_solvers, val);
    else
      vec[index] = val;
  }
  
public:
  solver_configuration()
    :_num_of_threads(1),
     _num_of_solvers(1)
  {
    resize_vectors();
  }

  static void print_option_list(std::ostream & ostr);
  
  void parse_config_file(std::istream & istr);
  
  unsigned number_of_threads() const
  {
    return _num_of_threads;
  }
  
  unsigned number_of_solvers() const
  {
    return _num_of_solvers;
  }

  bool randomize_decide(unsigned k = 0) const
  {
    return _randomize_decide[k];
  }

  unsigned random_seed(unsigned k = 0) const
  {
    return _random_seed[k];
  }

  double random_decides_percent(unsigned k = 0) const
  {
    return _random_decides_percent[k];
  }
  
  unsigned random_decides_after_restart(unsigned k = 0) const
  {
    return _random_decides_after_restart[k];
  }

  bool randomize_simplex(unsigned k = 0) const
  {
    return _randomize_simplex[k];
  }

  unsigned simplex_shuffle_seed(unsigned k = 0) const
  {
    return _simplex_shuffle_seed[k];
  }

  const std::string & basic_unknown_selection_strategy(unsigned k = 0) const
  {
    return _basic_unknown_selection_strategy[k];
  }

  const std::string & non_basic_unknown_selection_strategy(unsigned k = 0) const
  {
    return _non_basic_unknown_selection_strategy[k];
  }

  unsigned share_size_limit(unsigned k = 0) const
  {
    return _share_size_limit[k];
  }

  const std::string & polarity_selection_strategy(unsigned k = 0) const
  {
    return _polarity_selection_strategy[k];
  }

  unsigned random_polarity_seed(unsigned k = 0) const
  {
    return _random_polarity_seed[k];
  }

  double random_polarity_probability(unsigned k = 0) const
  {
    return _random_polarity_probability[k];
  }

  const std::string & alldiff_sum_algorithm(unsigned k = 0) const
  {
    return _alldiff_sum_algorithm[k];
  }

  bool predefined_csp_bounds(unsigned k = 0) const
  {
    return _predefined_csp_bounds[k];
  }

  int lower_predefined_csp_bound(unsigned k = 0) const
  {
    return _lower_predefined_csp_bound[k];
  }

  int upper_predefined_csp_bound(unsigned k = 0) const
  {
    return _upper_predefined_csp_bound[k];
  }
};

#endif // _SOLVER_CONFIGURATION_H
