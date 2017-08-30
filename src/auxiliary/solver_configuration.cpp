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
#include "solver_configuration.hpp"

bool solver_configuration::get_next_assignment(std::istream & istr, std::string & key, int & index, std::string & value)
{
  key.clear();
  value.clear();
  index = -1;
    
  int c;
  while((c = skip_spaces(istr)) == '#')
    {
      skip_rest_of_line(istr);
    }
  if(c == EOF)
    return false;
    
  key.push_back(c);
  while((c = istr.get()) != EOF && c != ' ' && c != '\t' && c != '\n' && c != '[' && c != '=')
    key.push_back(c);

  if(c == ' ' || c == '\n' || c == '\t')
    c = skip_spaces(istr);

  if(c == EOF)
    throw "config file syntax error: expected \'[\' or \'=\'";
    
  if(c == '[')
    {
      std::string in_str;
      while((c = istr.get()) != ']')
	{
	  if(c == EOF)
	    throw "config file syntax error: expected \']\'";
	  in_str.push_back(c);
	}
      index = atoi(in_str.c_str());
    }
  if(c == ']')
    c = skip_spaces(istr);
    
  if(c != '=') throw "config file syntax error: expected \'=\'";
  c = skip_spaces(istr);
  if(c == EOF)
    throw "config file syntax error: expected a value";

  value.push_back(c);
  while((c = istr.get()) != EOF && c != ' ' && c != '\t' && c != '\n')
    value.push_back(c);
  if(c != '\n' && c != EOF)
    skip_rest_of_line(istr);

  return true;
}

void solver_configuration::print_option_list(std::ostream & ostr)
{
  ostr << "The following options are supported in the config file:" << std::endl;
  ostr << "number_of_threads = <unsigned> (default: <number_of_solvers>)" << std::endl;
  ostr << "number_of_solvers = <unsigned> (default: 1)" << std::endl;
  ostr << "random_decide = yes|no (default: yes)" << std::endl;
  ostr << "random_seed = <num> (default: 1)" << std::endl;
  ostr << "random_decides_percent = <double> (default: 0.05)" << std::endl;
  ostr << "random_decides_after_restart = <unsigned> (default: 0)" << std::endl;
  ostr << "randomize_simplex = yes|no (default: no)"  << std::endl;
  ostr << "simplex_shuffle_seed = <unsigned> (default: 0)" << std::endl;
  ostr << "basic_unknown_selection_strategy = first|closest (default: closest)" << std::endl;
  ostr << "non_basic_unknown_selection_strategy = first|least_frequent (default: least_frequent)" << std::endl;
  ostr << "share_size_limit = <unsigned> (default: 0)" << std::endl;
  ostr << "polarity_selection_strategy = saved|positive|negative|random (default: saved)" << std::endl;
  ostr << "random_polarity_seed = <unsigned> (default: 0)" << std::endl;
  ostr << "random_polarity_probability = <double> (default: 0.5)" << std::endl;
  ostr << "alldiff_sum_algorithm = none|partition|beldiceanu|fast (default: none)" << std::endl;
  ostr << "predefined_csp_bounds = yes|no (default: no)" << std::endl;
  ostr << "lower_predefined_csp_bound = <int> (default: 0)" << std::endl;
  ostr << "upper_predefined_csp_bound = <int> (default: 0)" << std::endl;
}


void solver_configuration::parse_config_file(std::istream & istr)
{
  std::string key, value;
  int index = -1;
  while(get_next_assignment(istr, key, index, value))
    {
      //std::cout << key << "[" << index << "]" << " = " << value << std::endl;
      if(key == "number_of_threads")
	{
	  _num_of_threads = atoi(value.c_str());
	  if(_num_of_threads == 0)
	    _num_of_threads = 1;
	}
      else if(key == "number_of_solvers")
	{
	  _num_of_solvers = atoi(value.c_str());
	  if(_num_of_solvers == 0)
	    _num_of_solvers = 1;
	  if(_num_of_solvers > 1)
	    {
	      resize_vectors();
	    }
	}
      else if(key == "randomize_decide")
	{
	  if(value == "yes")
	    set_value(_randomize_decide, true, index);
	  else
	    set_value(_randomize_decide, false, index);
	}
      else if(key == "random_seed")
	{
	  unsigned seed = atoi(value.c_str());
	  set_value(_random_seed, seed, index);
	}
      else if(key == "random_decides_percent")
	{
	  double percent = atof(value.c_str());
	  set_value(_random_decides_percent, percent, index);	    
	}
      else if(key == "random_decides_after_restart")
	{
	  unsigned num = atoi(value.c_str());
	  set_value(_random_decides_after_restart, num, index);
	}
      else if(key == "randomize_simplex")
	{
	  if(value == "yes")
	    set_value(_randomize_simplex, true, index);
	  else
	    set_value(_randomize_simplex, false, index);
	}
      else if(key == "simplex_shuffle_seed")
	{
	  unsigned seed = atoi(value.c_str());
	  set_value(_simplex_shuffle_seed, seed, index);
	}
      else if(key == "basic_unknown_selection_strategy")
	{
	  set_value(_basic_unknown_selection_strategy, value, index);
	}
      else if(key == "non_basic_unknown_selection_strategy")
	{
	  set_value(_non_basic_unknown_selection_strategy, value, index);
	}      
      else if(key == "share_size_limit")
	{
	  unsigned limit = atoi(value.c_str());
	  set_value(_share_size_limit, limit, index);
	}
      else if(key == "polarity_selection_strategy")
	{
	  set_value(_polarity_selection_strategy, value, index);
	}
      else if(key == "random_polarity_seed")
	{
	  unsigned seed = atoi(value.c_str());
	  set_value(_random_polarity_seed, seed, index);
	}
      else if(key == "random_polarity_probability")
	{
	  double p = atof(value.c_str());
	  set_value(_random_polarity_probability, p, index);	    
	}
      else if(key == "alldiff_sum_algorithm")
	{
	  set_value(_alldiff_sum_algorithm, value, index);
	}
      else if(key == "predefined_csp_bounds")
	{
	  if(value == "yes")
	    set_value(_predefined_csp_bounds, true, index);
	  else
	    set_value(_predefined_csp_bounds, false, index);
	}
      else if(key == "lower_predefined_csp_bound")
	{
	  int limit = atoi(value.c_str());
	  set_value(_lower_predefined_csp_bound, limit, index);
	}
      else if(key == "upper_predefined_csp_bound")
	{
	  int limit = atoi(value.c_str());
	  set_value(_upper_predefined_csp_bound, limit, index);
	}      
      else
	{
	  std::cout << "WARNING: unknown option: " << key << " (possible typing error)" << std::endl;
	}
    }
}
