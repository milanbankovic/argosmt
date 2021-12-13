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

#include "sum_constraint_handler.hpp"
#include "domain_handler.hpp"
#include "heap.hpp"
#include <cmath>
#include <algorithm>

bool sum_constraint_handler::calculate_min(int & min)
{
  min = 0;
  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      int coeff = it->second;
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();
      if(c_handler->is_finite())
	{
	  min += (coeff > 0 ? c_handler->get_min() : c_handler->get_max()) * coeff;
	}
      else
	{
	  return false;
	}
    }
  return true;
}

bool sum_constraint_handler::calculate_max(int & max)
{
  max = 0;
  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      int coeff = it->second;
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();
      if(c_handler->is_finite())
	max += (coeff > 0 ? c_handler->get_max() : c_handler->get_min()) * coeff;
      else
	{
	  return false;
	}
    }
  return true;
}

struct min_comparator {
  sum_constraint_handler * _handler;

  min_comparator(sum_constraint_handler * h)
    :_handler(h) {}

  bool operator () (const expression & v1, const expression & v2) const
  {
    int v1_min = _handler->get_theory_solver()->get_theory_solver_data(v1)->get_variable_domain_handler()->get_min();
    int v2_min = _handler->get_theory_solver()->get_theory_solver_data(v2)->get_variable_domain_handler()->get_min();
    int v1_coeff = _handler->lp().get_coeff(v1);
    int v2_coeff = _handler->lp().get_coeff(v2);
    return v1_min < v2_min || (v1_min == v2_min && abs(v1_coeff) < abs(v2_coeff));      
  }
};

struct max_comparator {
  sum_constraint_handler * _handler;
  
  max_comparator(sum_constraint_handler * h)
    :_handler(h) {}

  bool operator () (const expression & v1, const expression & v2) const
  {
    int v1_max = _handler->get_theory_solver()->get_theory_solver_data(v1)->get_variable_domain_handler()->get_max();
    int v2_max = _handler->get_theory_solver()->get_theory_solver_data(v2)->get_variable_domain_handler()->get_max();
    int v1_coeff = _handler->lp().get_coeff(v1);
    int v2_coeff = _handler->lp().get_coeff(v2);
    return v1_max < v2_max || (v1_max == v2_max && abs(v1_coeff) < abs(v2_coeff));      
  }
};

struct abs_coeff_comparator {
  sum_constraint_handler * _handler;
  abs_coeff_comparator(sum_constraint_handler * h = nullptr)
    :_handler(h)
  {}

  bool operator () (const expression & v1, const expression & v2) const
  {
    int v1_coeff = _handler->lp().get_coeff(v1);
    int v2_coeff = _handler->lp().get_coeff(v2);
    unsigned v1_index = _handler->get_theory_solver()->get_theory_solver_data(v1)->get_variable_index();
    unsigned v2_index = _handler->get_theory_solver()->get_theory_solver_data(v2)->get_variable_index();

    return abs(v1_coeff) < abs(v2_coeff) || (abs(v2_coeff) == abs(v1_coeff) &&
					     v1_index < v2_index);      
  }
};

struct abs_coeffs_min_comparator {
  sum_constraint_handler * _handler;

  abs_coeffs_min_comparator(sum_constraint_handler * h = nullptr)
    :_handler(h) {}

  bool operator () (const expression & v1, const expression & v2) const
  {
    int v1_min = _handler->get_theory_solver()->get_theory_solver_data(v1)->get_variable_domain_handler()->get_min();
    int v2_min = _handler->get_theory_solver()->get_theory_solver_data(v2)->get_variable_domain_handler()->get_min();
    int v1_coeff = _handler->lp().get_coeff(v1);
    int v2_coeff = _handler->lp().get_coeff(v2);
    unsigned v1_index = _handler->get_theory_solver()->get_theory_solver_data(v1)->get_variable_index();
    unsigned v2_index = _handler->get_theory_solver()->get_theory_solver_data(v2)->get_variable_index();
    return abs(v1_coeff) > abs(v2_coeff) || (abs(v1_coeff) == abs(v2_coeff) &&
					     (v1_min > v2_min || (v1_min == v2_min &&
								  v1_index < v2_index)));      
  }
};

struct abs_coeffs_max_comparator {
  sum_constraint_handler * _handler;
  
  abs_coeffs_max_comparator(sum_constraint_handler * h = nullptr)
    :_handler(h) {}

  bool operator () (const expression & v1, const expression & v2) const
  {
    int v1_max = _handler->get_theory_solver()->get_theory_solver_data(v1)->get_variable_domain_handler()->get_max();
    int v2_max = _handler->get_theory_solver()->get_theory_solver_data(v2)->get_variable_domain_handler()->get_max();
    int v1_coeff = _handler->lp().get_coeff(v1);
    int v2_coeff = _handler->lp().get_coeff(v2);
    unsigned v1_index = _handler->get_theory_solver()->get_theory_solver_data(v1)->get_variable_index();
    unsigned v2_index = _handler->get_theory_solver()->get_theory_solver_data(v2)->get_variable_index();
    return abs(v1_coeff) > abs(v2_coeff) || (abs(v1_coeff) == abs(v2_coeff) &&
					     (v1_max < v2_max || (v1_max == v2_max &&
								  v1_index < v2_index)));      
  }
};

template <typename T = std::less<int>>
struct bel_max_comparator {
  sum_constraint_handler * _handler;
  T _comp;
  bel_max_comparator(sum_constraint_handler * h = nullptr)
    :_handler(h) {}
  
  bool operator () (const expression & v1, const expression & v2) const
  {
    int v1_max = _handler->get_theory_solver()->get_theory_solver_data(v1)->get_variable_domain_handler()->get_max();
    int v2_max = _handler->get_theory_solver()->get_theory_solver_data(v2)->get_variable_domain_handler()->get_max();
    unsigned v1_index = _handler->get_theory_solver()->get_theory_solver_data(v1)->get_variable_index();
    unsigned v2_index = _handler->get_theory_solver()->get_theory_solver_data(v2)->get_variable_index();
    return _comp(v1_max, v2_max) || (v1_max == v2_max && v1_index < v2_index);
  }
};

template <typename T = std::less<int>>
struct bel_min_comparator {
  sum_constraint_handler * _handler;
  T _comp;
  bel_min_comparator(sum_constraint_handler * h = nullptr)
    :_handler(h) {}
  
  bool operator () (const expression & v1, const expression & v2) const
  {
    int v1_min = _handler->get_theory_solver()->get_theory_solver_data(v1)->get_variable_domain_handler()->get_min();
    int v2_min = _handler->get_theory_solver()->get_theory_solver_data(v2)->get_variable_domain_handler()->get_min();
    unsigned v1_index = _handler->get_theory_solver()->get_theory_solver_data(v1)->get_variable_index();
    unsigned v2_index = _handler->get_theory_solver()->get_theory_solver_data(v2)->get_variable_index();
    return _comp(v1_min, v2_min) || (v1_min == v2_min && v1_index < v2_index);
  }
};


struct min_getter {
  csp_theory_solver * _th;
  min_getter(csp_theory_solver * th)
    :_th(th) {}

  int operator () (const expression & var) const
  {
    return _th->get_theory_solver_data(var)->get_variable_domain_handler()->get_min();
  }

};

struct max_getter {
  csp_theory_solver * _th;
  max_getter(csp_theory_solver * th)
    :_th(th) {}

  int operator () (const expression & var) const
  {
    return _th->get_theory_solver_data(var)->get_variable_domain_handler()->get_max();
  }

};

struct min_adjuster {
  void operator () (int & val, int d) const
  {
    val += d;
  }
};

struct max_adjuster {
  void operator () (int & val, int d) const
  {
    val -= d;
  }
};

int div_floor(int x, int y)
{
  return x % y == 0 ? x / y : (int)floor((double) x / y);
}

int div_ceil(int x, int y)
{
  return x % y == 0 ? x / y : (int)ceil((double) x / y);
}

void sum_constraint_handler::init_alldiff_intersections()
{
  if(_intersections_inited)
    return;
  
  if(_theory_solver->get_alldiff_sum_algorithm() != AS_NONE)
    {
      const expression_vector & constraints = _theory_solver->get_constraints();
      for(unsigned i = 0; i < constraints.size(); i++)
	{
	  // Skip all non-alldiff constraints
	  if(constraints[i]->get_symbol() != function_symbol::ALLDIFF)
	    continue;
	  
	  expression_set is(HASH_TABLE_SIZE);
	  const expression_set & alldiff_vars = _theory_solver->get_theory_solver_data(constraints[i])->get_constraint_variables();
	  const expression_set & sum_vars = _theory_solver->get_theory_solver_data(_constraint)->get_constraint_variables();
	  for(expression_set::const_iterator it = sum_vars.begin(), it_end = sum_vars.end(); it != it_end; ++it)
	    {
	      if(alldiff_vars.find(*it) != alldiff_vars.end())
		is.insert(*it);
	    }

	  // Cache only alldiffs's that have at least two vars in common with the sum
	  if(is.size() >= 2)
	    _cached_intersections.insert(std::make_pair(constraints[i], is));

	  // Special case: all coeffs positive and covered with a single alldiff
	  if(!_pos_covered && _neg_vars.empty() && is.size() == sum_vars.size())
	    {
	      _pos_covering_alldiff = constraints[i];
	      _pos_covered = true;
	    }
	}
    }

  if(_theory_solver->get_alldiff_sum_algorithm() == AS_PARTITION)
    {
      _calculate_bounds_min = &sum_constraint_handler::calculate_bounds_partition_min;
      _calculate_bounds_max = &sum_constraint_handler::calculate_bounds_partition_max;      
    }
  else if(_theory_solver->get_alldiff_sum_algorithm() == AS_BELDICEANU)
    {
      if(_pos_covered && _all_coeffs_one)
	{	  
	  _calculate_bounds_min = &sum_constraint_handler::calculate_bounds_beldiceanu_min;
	  _calculate_bounds_max = &sum_constraint_handler::calculate_bounds_beldiceanu_max;      
	}
      else
	{
	  std::cerr << "Error: cannot use Beldiceanu's algorithm on this constraint: "
		    << _constraint << " (standard bound consistency algorithm will be used)" <<  std::endl;
	  _calculate_bounds_min = &sum_constraint_handler::calculate_bounds_standard_min;
	  _calculate_bounds_max = &sum_constraint_handler::calculate_bounds_standard_max;      
	}     
    }
  else if(_theory_solver->get_alldiff_sum_algorithm() == AS_FAST)
    {      
      if(_pos_covered)
	{
	  _calculate_bounds_min = &sum_constraint_handler::calculate_bounds_fast_min;
	  _calculate_bounds_max = &sum_constraint_handler::calculate_bounds_fast_max;      
	}
      else
	{
	  _calculate_bounds_min = &sum_constraint_handler::calculate_bounds_multifast_min;
	  _calculate_bounds_max = &sum_constraint_handler::calculate_bounds_multifast_max;      
	}
    }
  else // AS_NONE
    {
      _calculate_bounds_min = &sum_constraint_handler::calculate_bounds_standard_min;
      _calculate_bounds_max = &sum_constraint_handler::calculate_bounds_standard_max;      
    }

  
  /* TODO: Re-run this every time some alldiff constraint gets activated/deactivated */
  find_disjoint_cliques(_pos_vars, _pos_partitions);
  find_disjoint_cliques(_neg_vars, _neg_partitions);
  
  _intersections_inited = true;
}

void sum_constraint_handler::find_disjoint_cliques(const expression_vector & vars, std::vector<expression_vector> & cliques)
{
  static expression_vector v_temp;
  v_temp.assign(vars.begin(), vars.end());

  while(!v_temp.empty())
    {
      static expression_vector clique;
      clique.clear();
      // Look for the largest intersection of v_temp with relevant (and active) alldiff's
      for(cache_map::const_iterator it = _cached_intersections.begin(), it_end = _cached_intersections.end();
	  it != it_end; ++it)
	{
	  const expression & alldiff = it->first;
	  const expression_set & alldiff_vars = it->second;
	  
	  // Skip inactive constraints
	  if(_theory_solver->get_solver().get_trail().get_value(alldiff) != EB_TRUE)
	    continue;
	  
	  // Skip alldiff's with to few common variables
	  if(alldiff_vars.size() <= clique.size())
	    continue;

	  static expression_vector curr_clique;
	  curr_clique.clear();
	  	  
	  // Construct intersection of alldiff_vars and v_temp
	  for(unsigned j = 0; j < v_temp.size(); j++)
	    {
	      if(alldiff_vars.find(v_temp[j]) != alldiff_vars.end())
		curr_clique.push_back(v_temp[j]);
	    }
	  // Set as maximal clique, if larger
	  if(curr_clique.size() > clique.size())
	    clique = std::move(curr_clique);
	}

      if(clique.size() >= 2)
	{
	  // Erase the vars of the clique from v_temp
	  unsigned j = 0;
	  for(unsigned i = 0; i < v_temp.size(); i++)
	    {
	      if(std::find(clique.begin(), clique.end(), v_temp[i]) == clique.end())
		v_temp[j++] = v_temp[i]; 
	    }
	  v_temp.resize(j);
	  
	  // Add maximal found clique to cliques
	  cliques.push_back(std::move(clique));
	}
      else
	{
	  // Add remaining singleton cliques
	  for(unsigned i = 0; i < v_temp.size(); i++)
	    {
	      cliques.push_back(expression_vector { v_temp[i] });
	    }
	  v_temp.clear();
	}
    }
}

/* Partitioner that splits the sets of vars into equal-mins
   (equal-maxs) partitions. It is based on sorting the array of vars
   with respect to the mins (or maxs, depending on choosen comparator)
   and the abs. value of the corresponding coeff as the secondary
   criterion. */
template <typename Comparator, typename Getter>
class sort_partitioner {
private:
  expression_vector * _vars;
  Getter _getter;
  unsigned i;
  expression_vector _result;
public:
  sort_partitioner()
    :_vars(0),
     _getter(0),
     i(0)
  {}
    
  void init(expression_vector & vars, Comparator comp, Getter getter)
  {
    _vars = &vars;
    _getter = getter;
    std::sort(_vars->begin(), _vars->end(), comp);
    i = 0;
    _result.clear();
  }
  
  const expression_vector & next_partition()
  {
    _result.clear();
    unsigned size = _vars->size();
    bool inited = false;
    int pre_val, curr_val;
    while(i < size)
    {
      curr_val = _getter((*_vars)[i]);

      if(!inited)
	{
	  pre_val = curr_val;
	  inited = true;
	}

      if(curr_val != pre_val)
	break;
      
      _result.push_back((*_vars)[i++]);      
    }
    return _result;
  }
};

/* Partitioner that splits the sets of vars into equal-mins
   (equal-maxs) partitions. It is based on classification of
   the vars based on the value of their mins/maxs. The 
   procedure uses hash maps to fastly retrieve the sets to 
   which the vars should be inserted. */
template <typename Getter>
class hash_partitioner {
private:
  std::unordered_map<int, expression_vector*> _map;
  std::vector<expression_vector> _pool;
  std::vector<std::pair<int, expression_vector *>> _in_use;
  std::vector<std::pair<int, expression_vector *>>::const_iterator _it;
  expression_vector _dummy;
public:
  hash_partitioner()
    :_map(HASH_TABLE_SIZE)
  {}
    
  void init(const expression_vector & vars, Getter getter)
  {
    // Alocating space for vectors (done only initially)
    if(_pool.size() < vars.size())
      {
	unsigned ps = _pool.size();
	_pool.resize(vars.size());
	while(ps < _pool.size())
	  _pool[ps++].reserve(vars.size());
      }

    // Clearing or resetting the map
    if(_map.size() > std::max((std::size_t)(100), vars.size()))
      {
	_map.clear();
      }
    else
      {
	for(auto & pair : _in_use)
	  _map[pair.first] = nullptr;
      }    

    // Clearing the list of vectors in use
    _in_use.clear();
    unsigned _ind = 0;

    // Classification of vars based on their minimums/maximums
    for(const expression & v : vars)
      {
	int mv = getter(v);
	expression_vector * & rp = _map[mv];
	if(rp == 0)
	  {
	    rp = &_pool[_ind++];
	    rp->clear();
	    _in_use.push_back(std::make_pair(mv, rp));
	  }
	rp->push_back(v);
      }
    _it = _in_use.begin();
  }
  
  const expression_vector & next_partition()
  {    
    if(_it != _in_use.end())
      {
	const expression_vector & ret = *_it->second;
	_it++;
	return ret;
      }
    else
      return _dummy;
  }

  template <typename Comp>
  void export_to_vector(expression_vector & vars, Comp && comp)
  {
    std::sort(_in_use.begin(), _in_use.end(),
	      [&comp] (const auto & v1, const auto & v2)
	      {
		return comp(v1.first, v2.first);
	      });
    vars.clear();
    for(const auto & p : _in_use)
      for(const expression & v : *p.second)
	vars.push_back(v);
  }
  
};

template <typename Comparator, typename Getter, typename Adjuster>
void sum_constraint_handler::adjust(expression_vector & vars, int & val, std::unordered_map<expression, int> & correction)
{  
  if(vars.empty())
    return;

  //  expression_vector same_val_vars;
  static std::vector<expression_vector> cliques;
  Comparator comparator(this);
  Getter getter(_theory_solver);
  Adjuster adjuster;

  //typedef sort_partitioner<Comparator, Getter> partitioner_type;
  typedef hash_partitioner<Getter> partitioner_type;
  static partitioner_type part;

  //part.init(vars, comparator, getter);
  part.init(vars, getter);

  while(true)
    {
      const expression_vector & same_val_vars = part.next_partition();
      if(same_val_vars.empty()) break;
      
      if(same_val_vars.size() > 1)
	{	  	  
	  find_disjoint_cliques(same_val_vars, cliques);
	  for(unsigned j = 0; j < cliques.size(); j++)
	    {
	      unsigned k = cliques[j].size();
	      int adj_val = 0;
	      for(unsigned s = 0; s < k - 1; s++)
	      	{
		  adj_val += abs(_lp.get_coeff(cliques[j][s])) * (k - s - 1);
		}
	      
	      adjuster(val, adj_val);
	      int corr = abs(_lp.get_coeff(cliques[j][0])) * (k - 1);
	      for(unsigned s = 0; s < k; s++)
		{		  
		  correction[cliques[j][s]] = corr;
		  if(s < k - 1)
		    corr += (k - (s + 2)) * (abs(_lp.get_coeff(cliques[j][s + 1])) - abs(_lp.get_coeff(cliques[j][s])));
		}
	    }
	}
      cliques.clear();      
    }
}

void sum_constraint_handler::bel_priority_min(expression_vector & vars, int & val, std::vector<std::pair<expression, int>> & matching)
{
  std::sort(vars.begin(), vars.end(), bel_min_comparator<std::less<int>>(this));
  static hash_heap<bel_max_comparator<std::less<int>>> _heap;
  _heap.set_comparator(bel_max_comparator<std::less<int>>(this));
  _heap.clear();
  unsigned i = 0;
  val = 0;
  int curr_val = _theory_solver->get_theory_solver_data(vars[0])->get_variable_domain_handler()->get_min() - 1;
  for(const expression & var : vars)
    _heap.introduce_element(var);
  
  for(unsigned j = 0; j < vars.size(); j++)
    {
      curr_val = std::max(curr_val + 1, _theory_solver->get_theory_solver_data(vars[j])->get_variable_domain_handler()->get_min());
      for(; i < vars.size() && _theory_solver->get_theory_solver_data(vars[i])->get_variable_domain_handler()->get_min() <= curr_val; i++)
	{
	  _heap.add_to_heap(vars[i]);
	}
      expression top;
      _heap.get_top_element(top);
      matching.push_back(std::make_pair(top, curr_val));
      val += curr_val;
      if(curr_val > _theory_solver->get_theory_solver_data(top)->get_variable_domain_handler()->get_max())
	{
	  val = 0x7fffffff;
	  return;
	}
    }
}

void sum_constraint_handler::bel_priority_max(expression_vector & vars, int & val, std::vector<std::pair<expression, int>> & matching)
{
  std::sort(vars.begin(), vars.end(), bel_max_comparator<std::greater<int>>(this));
  static hash_heap<bel_min_comparator<std::greater<int>>> _heap;
  _heap.set_comparator(bel_min_comparator<std::greater<int>>(this));
  _heap.clear();
  unsigned i = 0;
  val = 0;
  int curr_val = _theory_solver->get_theory_solver_data(vars[0])->get_variable_domain_handler()->get_max() + 1;
  for(const expression & var : vars)
    _heap.introduce_element(var);
  
  for(unsigned j = 0; j < vars.size(); j++)
    {
      curr_val = std::min(curr_val - 1, _theory_solver->get_theory_solver_data(vars[j])->get_variable_domain_handler()->get_max());
      for(; i < vars.size() && _theory_solver->get_theory_solver_data(vars[i])->get_variable_domain_handler()->get_max() >= curr_val; i++)
	{
	  _heap.add_to_heap(vars[i]);
	}
      expression top;
      _heap.get_top_element(top);
      matching.push_back(std::make_pair(top, curr_val));
      val += curr_val;
      if(curr_val < _theory_solver->get_theory_solver_data(top)->get_variable_domain_handler()->get_min())
	{
	  val = 0x80000000;
	  return;
	}
    }
}

void sum_constraint_handler::fast_priority_min(expression_vector & vars, int & val,
					       std::vector<std::pair<expression, int>> & matching, std::vector<unsigned> & p)
{
  /* Sort vars with respect to the ascending order of minimums (O(nlogn)) */
  std::sort(vars.begin(), vars.end(), bel_min_comparator<std::less<int>>(this));   
  
  /* Heap of variables (the top variable is with the greatest abs. value of coefficient) */
  static simple_heap<abs_coeffs_max_comparator> _heap;                  
  _heap.set_comparator(abs_coeffs_max_comparator(this));
  _heap.clear();

  /* The map indices maps each variable to its index in the matching */
  typedef std::unordered_map<expression, unsigned> ind_map_type;
  static ind_map_type indices;
  indices.clear();

  /* The vector next_vars for each position in the matching vector stores
     the alternative variable (i.e. the variable that was the second choice
     for taking the corresponding value) */
  static expression_vector next_vars;
  next_vars.clear();

  /* Initialization */
  unsigned i = 0;
  val = 0;
  int curr_val = _theory_solver->get_theory_solver_data(vars[0])->get_variable_domain_handler()->get_min() - 1;  
  for(const expression & var : vars)
    _heap.introduce_element(var);

  /* The main loop */
  for(unsigned j = 0; j < vars.size(); j++)
    {
      /* Calculate next value */
      curr_val = std::max(curr_val + 1, _theory_solver->get_theory_solver_data(vars[j])->get_variable_domain_handler()->get_min());

      /* Push all new candidates to the heap */
      for(; i < vars.size() && _theory_solver->get_theory_solver_data(vars[i])->get_variable_domain_handler()->get_min() <= curr_val; i++)
	{
	  _heap.add_to_heap(vars[i]);
	}

      /* Take the top variable of the heap and assign the current value to that variable */
      expression top;
      _heap.get_top_element(top);
      matching.push_back(std::make_pair(top, curr_val));
      val += _lp.get_coeff(top) * curr_val;

      /* Store the index of the assigned variable */
      indices[top] = matching.size() - 1;

      /* The element on the top of the heap is the second choice for
	 the current value (if exists) */
      if(!_heap.empty())
	next_vars.push_back(_heap.top_element());
      else
	next_vars.push_back(expression());
    }

  /* Finally, we form p vector by replacing the vars in next_vars by their indices
     in the matching vector */
  for(const expression & next : next_vars)
    {
      if(next.get() != nullptr)
	p.push_back(indices[next]);
      else
	p.push_back(0xffffffff);
    }
}

void sum_constraint_handler::fast_priority_max(expression_vector & vars, int & val,
					       std::vector<std::pair<expression, int>> & matching, std::vector<unsigned> & p)
{
  /* Sort vars with respect to the descending order of maximums (O(nlogn)) */
  std::sort(vars.begin(), vars.end(), bel_max_comparator<std::greater<int>>(this));

  /* Heap of variables (the top variable is with the greatest abs. value of coefficient) */
  static simple_heap<abs_coeffs_min_comparator> _heap;
  _heap.set_comparator(abs_coeffs_min_comparator(this));
  _heap.clear();

  /* The map indices maps each variable to its index in the matching */
  typedef std::unordered_map<expression, unsigned> ind_map_type;
  static ind_map_type indices;
  indices.clear();

  
  /* The vector next_vars for each position in the matching vector stores
     the alternative variable (i.e. the variable that was the second choice
     for taking the corresponding value) */  
  static expression_vector next_vars;
  next_vars.clear();

  /* Initialization */
  unsigned i = 0;
  val = 0;
  int curr_val = _theory_solver->get_theory_solver_data(vars[0])->get_variable_domain_handler()->get_max() + 1;
  for(const expression & var : vars)
    _heap.introduce_element(var);

  /* The main loop */
  for(unsigned j = 0; j < vars.size(); j++)
    {
      /* Calculate next value */
      curr_val = std::min(curr_val - 1, _theory_solver->get_theory_solver_data(vars[j])->get_variable_domain_handler()->get_max());
      
      /* Push all new candidates to the heap */
      for(; i < vars.size() && _theory_solver->get_theory_solver_data(vars[i])->get_variable_domain_handler()->get_max() >= curr_val; i++)
	{
	  _heap.add_to_heap(vars[i]);
	}

      /* Take the top variable of the heap and assign the current value to that variable */
      expression top;
      _heap.get_top_element(top);
      matching.push_back(std::make_pair(top, curr_val));
      val += _lp.get_coeff(top) * curr_val;

      /* Store the index of the assigned variable */
      indices[top] = matching.size() - 1;

      /* The element on the top of the heap is the second choice for
	 the current value (if exists) */
      if(!_heap.empty())
	next_vars.push_back(_heap.top_element());
      else
	next_vars.push_back(expression());
    }

  /* Finally, we form p vector by replacing the vars in next_vars by their indices
     in the matching vector */
  for(const expression & next : next_vars)
    {
      if(next.get() != nullptr)
	p.push_back(indices[next]);
      else
	p.push_back(0xffffffff);
    }
}
  
bool sum_constraint_handler::calculate_bounds_standard_min(int ubound, int & lp_min, std::unordered_map<expression, int> & cb)
{
  if(!calculate_min(lp_min))
      return false;

  if(ubound < lp_min)
    return true;
  
  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      int coeff = it->second;
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();

      if(coeff > 0)
	{
	  int c_min = c_handler->get_min();
	  int minp = lp_min - coeff * c_min;
	  int calc_max = div_floor(ubound - minp, coeff);
	  cb[it->first] = calc_max;
	}
      else
	{
	  int c_max = c_handler->get_max();
	  int minp = lp_min - coeff * c_max;
	  int calc_min = div_ceil(ubound - minp, coeff);
	  cb[it->first] = calc_min;
	}
    }
  return true;
}

bool sum_constraint_handler::calculate_bounds_standard_max(int lbound, int & lp_max, std::unordered_map<expression, int> & cb)
{
  if(!calculate_max(lp_max))
    return false;

  if(lp_max < lbound)
    return true;

  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      int coeff = it->second;
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();

      if(coeff < 0)
	{
	  int c_min = c_handler->get_min();
	  int maxp = lp_max - coeff * c_min;
	  int calc_max = div_floor(lbound - maxp, coeff);
	  cb[it->first] = calc_max;
	}
      else
	{
	  int c_max = c_handler->get_max();
	  int maxp = lp_max - coeff * c_max;
	  int calc_min = div_ceil(lbound - maxp, coeff);
	  cb[it->first] = calc_min;
	}    
    }
  return true;
}

bool sum_constraint_handler::calculate_bounds_partition_min(int ubound, int & lp_min, std::unordered_map<expression, int> & cb)
{
  if(!calculate_min(lp_min))
    return false;

  int orig_min = lp_min;

  //  std::cout << "LP_MIN_BF: " << lp_min << std::endl;
  
  _theory_solver->_sum_common_data._as_algorithm_time_spent.start();
  adjust<min_comparator, min_getter, min_adjuster>(_pos_vars, lp_min, cb);
  adjust<max_comparator, max_getter, min_adjuster>(_neg_vars, lp_min, cb);
  _theory_solver->_sum_common_data._as_algorithm_time_spent.acumulate();

  // std::cout << "LP_MIN: " << lp_min << std::endl;
  
  if(ubound < lp_min)
    return true;

  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      int coeff = it->second;
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();

      if(coeff > 0)
	{
	  int c_min = c_handler->get_min();
	  int c_max = c_handler->get_max();
	  int minp = lp_min - cb[it->first] - coeff * c_min;
	  int calc_max = div_floor(ubound - minp, coeff);
	  cb[it->first] = calc_max;
	  if(calc_max < c_max)
	    {
	      int old_max = div_floor(ubound - orig_min + coeff * c_min, coeff);
	      if(calc_max < old_max)
		{
		  _common_data->_count_improved_bound_change++;
		  _common_data->_cumulative_improvement += (old_max - calc_max);
		  //std::cout << "BOOST: " << it->first << " : " << old_max << " : " << calc_max << std::endl;
		}
	      _common_data->_count_bound_change++;
	    }
	}
      else
	{
	  int c_max = c_handler->get_max();
	  int c_min = c_handler->get_min();
	  int minp = lp_min - cb[it->first] - coeff * c_max;
	  int calc_min = div_ceil(ubound - minp, coeff);
	  cb[it->first] = calc_min;
	  if(calc_min > c_min)
	    {
	      int old_min = div_ceil(ubound - orig_min + coeff * c_max, coeff)	    ;
	      if(calc_min > old_min)
		{
		  //std::cout << "BOOST12: " << it->first << " : " << old_min << " : " << calc_min << std::endl;
		  _common_data->_count_improved_bound_change++;
		  _common_data->_cumulative_improvement += (calc_min - old_min);
		}
	      _common_data->_count_bound_change++;
	    }
	}
    }
  return true;
}

bool sum_constraint_handler::calculate_bounds_partition_max(int lbound, int & lp_max, std::unordered_map<expression, int> & cb)
{
  if(!calculate_max(lp_max))
    return false;

  int orig_max = lp_max;

  _theory_solver->_sum_common_data._as_algorithm_time_spent.start();  
  adjust<max_comparator, max_getter, max_adjuster>(_pos_vars, lp_max, cb);
  adjust<min_comparator, min_getter, max_adjuster>(_neg_vars, lp_max, cb);
  _theory_solver->_sum_common_data._as_algorithm_time_spent.acumulate();

  if(lp_max < lbound)
    return true;

  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      int coeff = it->second;
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();

      if(coeff < 0)
	{
	  int c_min = c_handler->get_min();
	  int c_max = c_handler->get_max();
	  int maxp = lp_max + cb[it->first] - coeff * c_min;
	  int calc_max = div_floor(lbound - maxp, coeff);	  
	  cb[it->first] = calc_max;
	  
	  if(calc_max < c_max)
	    {
	      int old_max = div_floor(lbound - orig_max + coeff * c_min, coeff);
	      if(calc_max < old_max)
		{
		  _common_data->_count_improved_bound_change++;
		  _common_data->_cumulative_improvement += (old_max - calc_max);
		  //std::cout << "BOOST22: " << it->first << " : " << old_max << " : " << calc_max << std::endl;
		}
	      _common_data->_count_bound_change++;
	    }
	}
      else
	{
	  int c_max = c_handler->get_max();
	  int c_min = c_handler->get_min();
	  int maxp = lp_max + cb[it->first] - coeff * c_max;
	  int calc_min = div_ceil(lbound - maxp, coeff);
	  cb[it->first] = calc_min;
	  if(calc_min > c_min)
	    {
	      int old_min = div_ceil(lbound - orig_max + coeff * c_max, coeff);
	      if(calc_min > old_min)
		{
		  //std::cout << "BOOST2: " << it->first << " : " << old_min <<  " : " << calc_min << std::endl;
		  _common_data->_count_improved_bound_change++;
		  _common_data->_cumulative_improvement += (calc_min - old_min);
		}
	      _common_data->_count_bound_change++;		  	      
	    }
	}
    }
  return true;
}


bool sum_constraint_handler::calculate_bounds_beldiceanu_min(int ubound, int & lp_min, std::unordered_map<expression, int> & cb)
{
  if(!calculate_min(lp_min))
    return false;

  int orig_min = lp_min;

  typedef std::vector<std::pair<expression, int>> matching_vector_type;
  static matching_vector_type matching;
  matching.clear();

  _theory_solver->_sum_common_data._as_algorithm_time_spent.start();
  bel_priority_min(_pos_vars, lp_min, matching);
  _theory_solver->_sum_common_data._as_algorithm_time_spent.acumulate();
  
  if(ubound < lp_min)
    return true;

  min_getter getter(_theory_solver);

  _theory_solver->_sum_common_data._as_algorithm_time_spent.start();
  // DETERMINE BLOCKS
  std::deque<unsigned> low;
  std::deque<unsigned> up;
  int curr_min = 0x7fffffff;
  up.push_front(matching.size() - 1);

  for(int i = matching.size() - 1; i >= 0; i--)
    {
      if(getter(matching[i].first) < curr_min)
	curr_min = getter(matching[i].first);

      if(matching[i].second == curr_min)
	{
	  low.push_front(i);	  
	  if(i > 0)
	    up.push_front(i - 1);
	}
    }

  auto last = [&matching=matching, &up] (int i) -> int
    {
      return matching[up[i]].second;      
    };

  auto first = [&matching=matching, &low] (int i) -> int
    {
      return matching[low[i]].second;      
    };
  
  // CALCULATE BOUNDS
  for(int m = low.size(), b = m - 1, i = b; b >= 0; b--)
    {      
      bool found = false;
      int max = 0;
      int t = ubound - (lp_min - last(b));
      while(!found)
	{
	  if((i == m - 1 && t > last(i)) || (i > 0 && last(i-1) < t && t < first(i)))
	    {
	      found = true;
	      max = t;
	    }
	  else if(i > b && last(i - 1) < first(i) - 1 && t >= first(i))
	    {
	      found = true;
	      max = first(i) - 1;
	    }
	  else if(i == b)
	    {
	      found = true;
	      max = last(i);
	    }
	  else
	    i--;
	}
      for(unsigned j = low[b]; j <= up[b]; j++)
	{
	  cb[matching[j].first] = max;
	}
    }
  _theory_solver->_sum_common_data._as_algorithm_time_spent.acumulate();
  
  // IMPROVEMENT STATISTICS
  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();

      int c_min = c_handler->get_min();
      int c_max = c_handler->get_max();
      int calc_max = cb[it->first];
      if(calc_max < c_max)
	{	  
	  int old_max = ubound - (orig_min - c_min);
	  if(calc_max < old_max)
	    {
	      _common_data->_count_improved_bound_change++;
	      _common_data->_cumulative_improvement += (old_max - calc_max);
	      //std::cout << "BOOST: " << it->first << " : " << old_max << " : " << calc_max << std::endl;
	    }
	  _common_data->_count_bound_change++;
	}
    }
  
  return true;
}

bool sum_constraint_handler::calculate_bounds_beldiceanu_max(int lbound, int & lp_max, std::unordered_map<expression, int> & cb)
{
  if(!calculate_max(lp_max))
    return false;

  int orig_max = lp_max;

  typedef std::vector<std::pair<expression, int>> matching_vector_type;
  static matching_vector_type matching;
  matching.clear();

  _theory_solver->_sum_common_data._as_algorithm_time_spent.start();
  bel_priority_max(_pos_vars, lp_max, matching);
  _theory_solver->_sum_common_data._as_algorithm_time_spent.acumulate();
  
  if(lbound > lp_max)
    return true;

  max_getter getter(_theory_solver);

  _theory_solver->_sum_common_data._as_algorithm_time_spent.start();
  // DETERMINE BLOCKS
  std::deque<unsigned> low;
  std::deque<unsigned> up;
  int curr_max = 0x80000000;
  up.push_front(matching.size() - 1);
  for(int i = matching.size() - 1; i >= 0; i--)
    {
      if(getter(matching[i].first) > curr_max)
	curr_max = getter(matching[i].first);

      if(matching[i].second == curr_max)
	{
	  low.push_front(i);
	  if(i > 0)
	    up.push_front(i - 1);
	}
    }

  auto last = [&matching=matching, &up] (int i) -> int
    {
      return matching[up[i]].second;      
    };

  auto first = [&matching=matching, &low] (int i) -> int
    {
      return matching[low[i]].second;      
    };


  
  // CALCULATE BOUNDS
  for(int m = low.size(), b = m - 1, i = b; b >= 0; b--)
    {
      bool found = false;
      int min = 0;
      int t = lbound - (lp_max - last(b));
      while(!found)
	{
	  if((i == m - 1 && t < last(i)) || (i > 0 && last(i-1) > t && t > first(i)))
	    {
	      found = true;
	      min = t;
	    }
	  else if(i > b && last(i - 1) > first(i) + 1 && t <= first(i))
	    {
	      found = true;
	      min = first(i) + 1;
	    }
	  else if(i == b)
	    {
	      found = true;
	      min = last(i);
	    }
	  else
	    i--;
	}
      for(unsigned j = low[b]; j <= up[b]; j++)
	cb[matching[j].first] = min;	
    }
  _theory_solver->_sum_common_data._as_algorithm_time_spent.acumulate();
  
  // IMPROVEMENT STATISTICS
  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();

      int c_min = c_handler->get_min();
      int c_max = c_handler->get_max();
      int calc_min = cb[it->first];
      if(calc_min > c_min)
	{	  
	  int old_min = lbound - (orig_max - c_max);
	  if(calc_min > old_min)
	    {
	      //std::cout << "BOOST2: " << it->first << " : " << old_min <<  " : " << calc_min << std::endl;
	      _common_data->_count_improved_bound_change++;
	      _common_data->_cumulative_improvement += (calc_min - old_min);
	    }	  
	  _common_data->_count_bound_change++;
	}
    }  
  return true;
}

bool sum_constraint_handler::calculate_bounds_fast_min(int ubound, int & lp_min, std::unordered_map<expression, int> & cb)
{
  if(!calculate_min(lp_min))
    return false;

  int orig_min = lp_min;

  typedef std::vector<std::pair<expression, int>> matching_vector_type;
  static matching_vector_type matching;
  matching.clear();
  static std::vector<unsigned> p;
  p.clear();
  
  _theory_solver->_sum_common_data._as_algorithm_time_spent.start();
  fast_priority_min(_pos_vars, lp_min, matching, p);
  _theory_solver->_sum_common_data._as_algorithm_time_spent.acumulate();

  if(ubound < lp_min)
    return true;

  _theory_solver->_sum_common_data._as_algorithm_time_spent.start();
  static std::vector<int> c;
  c.assign(matching.size(), 0);

  for(int i = matching.size() - 1; i >= 0; i--)
    {      
      int coeff = _lp.get_coeff(matching[i].first);
      int val = matching[i].second;
            
      if(p[i] != 0xffffffff)
	c[i] = val * (coeff - _lp.get_coeff(matching[p[i]].first)) + c[p[i]];
      else
	c[i] = val * coeff;
      int lp_min_p = lp_min - c[i];
      int calc_max = div_floor(ubound - lp_min_p, coeff);
      cb[matching[i].first] = calc_max;
    }
  _theory_solver->_sum_common_data._as_algorithm_time_spent.acumulate();

  
  // IMPROVEMENT STATISTICS
  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();

      int c_min = c_handler->get_min();
      int c_max = c_handler->get_max();
      int calc_max = cb[it->first];
      int coeff = it->second;
      if(calc_max < c_max)
	{
	  int old_max = div_floor(ubound - orig_min + coeff * c_min, coeff);
	  if(calc_max < old_max)
	    {
	      _common_data->_count_improved_bound_change++;
	      _common_data->_cumulative_improvement += (old_max - calc_max);
	      //std::cout << "BOOST: " << it->first << " : " << old_max << " : " << calc_max << std::endl;
	    }
	  _common_data->_count_bound_change++;
	}
    }

  return true;
}

bool sum_constraint_handler::calculate_bounds_fast_max(int lbound, int & lp_max, std::unordered_map<expression, int> & cb)
{
  if(!calculate_max(lp_max))
    return false;

  int orig_max = lp_max;

  typedef std::vector<std::pair<expression, int>> matching_vector_type;
  static matching_vector_type matching;
  matching.clear();
  static std::vector<unsigned> p;
  p.clear();
  
  _theory_solver->_sum_common_data._as_algorithm_time_spent.start();
  fast_priority_max(_pos_vars, lp_max, matching, p);
  _theory_solver->_sum_common_data._as_algorithm_time_spent.acumulate();

  if(lbound > lp_max)
    return true;

  _theory_solver->_sum_common_data._as_algorithm_time_spent.start();
  static std::vector<int> c;
  c.assign(matching.size(), 0);

  for(int i = matching.size() - 1; i >= 0; i--)
    {      
      int coeff = _lp.get_coeff(matching[i].first);
      int val = matching[i].second;
      
      if(p[i] != 0xffffffff)
	c[i] = val * (coeff - _lp.get_coeff(matching[p[i]].first)) + c[p[i]];
      else
	c[i] = val * coeff;
      int lp_max_p = lp_max - c[i];
      int calc_min = div_ceil(lbound - lp_max_p, coeff);
      cb[matching[i].first] = calc_min;
    }
  _theory_solver->_sum_common_data._as_algorithm_time_spent.acumulate();

  // IMPROVEMENT STATISTICS
  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();

      int c_min = c_handler->get_min();
      int c_max = c_handler->get_max();
      int calc_min = cb[it->first];
      int coeff = it->second;
      if(calc_min > c_min)
	{
	  int old_min = div_ceil(lbound - orig_max + coeff * c_max, coeff);
	  if(calc_min > old_min)
	    {
	      //std::cout << "BOOST2: " << it->first << " : " << old_min <<  " : " << calc_min << std::endl;
	      _common_data->_count_improved_bound_change++;
	      _common_data->_cumulative_improvement += (calc_min - old_min);
	    }	  
	  _common_data->_count_bound_change++;
	}
    }  
  
  return true;
}

bool sum_constraint_handler::calculate_bounds_multifast_min(int ubound, int & lp_min, std::unordered_map<expression, int> & cb)
{
  if(!calculate_min(lp_min))
    return false;
  
  int orig_min = lp_min;

  typedef std::vector<std::pair<expression, int>> matching_vector_type;

  static std::vector<int> pos_mins;
  if(pos_mins.size() < _pos_partitions.size())
    pos_mins.resize(_pos_partitions.size());
  static std::vector<matching_vector_type> pos_matchings;
  if(pos_matchings.size() < _pos_partitions.size())
    pos_matchings.resize(_pos_partitions.size());
  static std::vector<std::vector<unsigned>> pos_p_vectors;
  if(pos_p_vectors.size() < _pos_partitions.size())
    pos_p_vectors.resize(_pos_partitions.size());
    
  static std::vector<int> neg_mins;
  if(neg_mins.size() < _neg_partitions.size())
    neg_mins.resize(_neg_partitions.size());
  static std::vector<matching_vector_type> neg_matchings;
  if(neg_matchings.size() < _neg_partitions.size())
    neg_matchings.resize(_neg_partitions.size());
  static std::vector<std::vector<unsigned>> neg_p_vectors;
  if(neg_p_vectors.size() < _neg_partitions.size())
    neg_p_vectors.resize(_neg_partitions.size());

  _theory_solver->_sum_common_data._as_algorithm_time_spent.start();
  lp_min = 0;
  for(unsigned i = 0; i < _pos_partitions.size(); i++)
    {
      expression_vector & partition = _pos_partitions[i];      
      int & part_min = pos_mins[i];
      part_min = 0;
      matching_vector_type & matching = pos_matchings[i];
      matching.clear();
      std::vector<unsigned> & p = pos_p_vectors[i];
      p.clear();
      fast_priority_min(partition, part_min, matching, p);
      lp_min += part_min;
    }

  for(unsigned i = 0; i < _neg_partitions.size(); i++)
    {
      expression_vector & partition = _neg_partitions[i];
      int & part_min = neg_mins[i];
      part_min = 0;
      matching_vector_type & matching = neg_matchings[i];
      matching.clear();
      std::vector<unsigned> & p = neg_p_vectors[i];
      p.clear();
      fast_priority_max(partition, part_min, matching, p);
      lp_min += part_min;      
    }
  _theory_solver->_sum_common_data._as_algorithm_time_spent.acumulate();

  if(ubound < lp_min)
    return true;     

  _theory_solver->_sum_common_data._as_algorithm_time_spent.start();
  for(unsigned i = 0; i < _pos_partitions.size(); i++)
    {
      int part_min = pos_mins[i];
      int rest_min = lp_min - part_min;
      int part_ubound = ubound - rest_min;
      const matching_vector_type & matching = pos_matchings[i];
      const std::vector<unsigned> & p = pos_p_vectors[i];
      static std::vector<int> c;
      c.assign(matching.size(), 0);

      for(int j = matching.size() - 1; j >= 0; j--)
	{      
	  int coeff = _lp.get_coeff(matching[j].first);
	  int val = matching[j].second;
            
	  if(p[j] != 0xffffffff)
	    c[j] = val * (coeff - _lp.get_coeff(matching[p[j]].first)) + c[p[j]];
	  else
	    c[j] = val * coeff;
	  int part_min_p = part_min - c[j];
	  int calc_max = div_floor(part_ubound - part_min_p, coeff);
	  cb[matching[j].first] = calc_max;
	}            
    }

    for(unsigned i = 0; i < _neg_partitions.size(); i++)
    {
      int part_min = neg_mins[i];
      int rest_min = lp_min - part_min;
      int part_ubound = ubound - rest_min;
      const matching_vector_type & matching = neg_matchings[i];
      const std::vector<unsigned> & p = neg_p_vectors[i];
      static std::vector<int> c;
      c.assign(matching.size(), 0);
      
      for(int j = matching.size() - 1; j >= 0; j--)
	{      
	  int coeff = _lp.get_coeff(matching[j].first);
	  int val = matching[j].second;
            
	  if(p[j] != 0xffffffff)
	    c[j] = val * (coeff - _lp.get_coeff(matching[p[j]].first)) + c[p[j]];
	  else
	    c[j] = val * coeff;
	  int part_min_p = part_min - c[j];
	  int calc_min = div_ceil(part_ubound - part_min_p, coeff);
	  cb[matching[j].first] = calc_min;
	}            
    }
    _theory_solver->_sum_common_data._as_algorithm_time_spent.acumulate();
       
  // IMPROVEMENT STATISTICS
  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      int coeff = it->second;
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();

      if(coeff > 0)
	{
	  int c_min = c_handler->get_min();
	  int c_max = c_handler->get_max();
	  int calc_max = cb[it->first];
	  if(calc_max < c_max)
	    {
	      int old_max = div_floor(ubound - orig_min + coeff * c_min, coeff);
	      if(calc_max < old_max)
		{
		  _common_data->_count_improved_bound_change++;
		  _common_data->_cumulative_improvement += (old_max - calc_max);
		  //std::cout << "BOOST: " << it->first << " : " << old_max << " : " << calc_max << std::endl;
		}
	      _common_data->_count_bound_change++;
	    }
	}
      else
	{
	  int c_max = c_handler->get_max();
	  int c_min = c_handler->get_min();
	  int calc_min = cb[it->first];
	  if(calc_min > c_min)
	    {
	      int old_min = div_ceil(ubound - orig_min + coeff * c_max, coeff);
	      if(calc_min > old_min)
		{
		  //std::cout << "BOOST12: " << it->first << " : " << old_min << " : " << calc_min << std::endl;
		  _common_data->_count_improved_bound_change++;
		  _common_data->_cumulative_improvement += (calc_min - old_min);
		}
	      _common_data->_count_bound_change++;
	    }
	}
    }  
  return true;
}

bool sum_constraint_handler::calculate_bounds_multifast_max(int lbound, int & lp_max, std::unordered_map<expression, int> & cb)
{
  if(!calculate_max(lp_max))
    return false;

  int orig_max = lp_max;

  typedef std::vector<std::pair<expression, int>> matching_vector_type;

  static std::vector<int> pos_maxs;
  if(pos_maxs.size() < _pos_partitions.size())
    pos_maxs.resize(_pos_partitions.size());
  static std::vector<matching_vector_type> pos_matchings;
  if(pos_matchings.size() < _pos_partitions.size())
    pos_matchings.resize(_pos_partitions.size());
  static std::vector<std::vector<unsigned>> pos_p_vectors;
  if(pos_p_vectors.size() < _pos_partitions.size())
    pos_p_vectors.resize(_pos_partitions.size());
    
  static std::vector<int> neg_maxs;
  if(neg_maxs.size() < _neg_partitions.size())
    neg_maxs.resize(_neg_partitions.size());
  static std::vector<matching_vector_type> neg_matchings;
  if(neg_matchings.size() < _neg_partitions.size())
    neg_matchings.resize(_neg_partitions.size());
  static std::vector<std::vector<unsigned>> neg_p_vectors;
  if(neg_p_vectors.size() < _neg_partitions.size())
    neg_p_vectors.resize(_neg_partitions.size());

  _theory_solver->_sum_common_data._as_algorithm_time_spent.start();
  lp_max = 0;
  for(unsigned i = 0; i < _pos_partitions.size(); i++)
    {
      expression_vector & partition = _pos_partitions[i];
      int & part_max = pos_maxs[i];
      part_max = 0;
      matching_vector_type & matching = pos_matchings[i];
      matching.clear();
      std::vector<unsigned> & p = pos_p_vectors[i];
      p.clear();
      fast_priority_max(partition, part_max, matching, p);
      lp_max += part_max;
    }

  for(unsigned i = 0; i < _neg_partitions.size(); i++)
    {
      expression_vector & partition = _neg_partitions[i];
      int & part_max = neg_maxs[i];
      part_max = 0;
      matching_vector_type & matching = neg_matchings[i];
      matching.clear();
      std::vector<unsigned> & p = neg_p_vectors[i];
      p.clear();
      fast_priority_min(partition, part_max, matching, p);
      lp_max += part_max;
    }
  _theory_solver->_sum_common_data._as_algorithm_time_spent.acumulate();

  if(lbound > lp_max)
    return true;     

  _theory_solver->_sum_common_data._as_algorithm_time_spent.start();
  for(unsigned i = 0; i < _pos_partitions.size(); i++)
    {
      int part_max = pos_maxs[i];
      int rest_max = lp_max - part_max;
      int part_lbound = lbound - rest_max;
      const matching_vector_type & matching = pos_matchings[i];
      const std::vector<unsigned> & p = pos_p_vectors[i];
      static std::vector<int> c;
      c.assign(matching.size(), 0);

      for(int j = matching.size() - 1; j >= 0; j--)
	{      
	  int coeff = _lp.get_coeff(matching[j].first);
	  int val = matching[j].second;
            
	  if(p[j] != 0xffffffff)
	    c[j] = val * (coeff - _lp.get_coeff(matching[p[j]].first)) + c[p[j]];
	  else
	    c[j] = val * coeff;
	  int part_max_p = part_max - c[j];
	  int calc_min = div_ceil(part_lbound - part_max_p, coeff);
	  cb[matching[j].first] = calc_min;
	}            
    }

    for(unsigned i = 0; i < _neg_partitions.size(); i++)
    {
      int part_max = neg_maxs[i];
      int rest_max = lp_max - part_max;
      int part_lbound = lbound - rest_max;
      const matching_vector_type & matching = neg_matchings[i];
      const std::vector<unsigned> & p = neg_p_vectors[i];
      static std::vector<int> c;
      c.assign(matching.size(), 0);
      
      for(int j = matching.size() - 1; j >= 0; j--)
	{      
	  int coeff = _lp.get_coeff(matching[j].first);
	  int val = matching[j].second;
            
	  if(p[j] != 0xffffffff)
	    c[j] = val * (coeff - _lp.get_coeff(matching[p[j]].first)) + c[p[j]];
	  else
	    c[j] = val * coeff;
	  int part_max_p = part_max - c[j];
	  int calc_max = div_floor(part_lbound - part_max_p, coeff);
	  cb[matching[j].first] = calc_max;
	}            
    }
    _theory_solver->_sum_common_data._as_algorithm_time_spent.acumulate();


    // IMPROVEMENTS STATISTICS
    for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      int coeff = it->second;
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();

      if(coeff < 0)
	{
	  int c_min = c_handler->get_min();
	  int c_max = c_handler->get_max();
	  int calc_max = cb[it->first];	  
	  if(calc_max < c_max)
	    {
	      int old_max = div_floor(lbound - orig_max + coeff * c_min, coeff);
	      if(calc_max < old_max)
		{
		  _common_data->_count_improved_bound_change++;
		  _common_data->_cumulative_improvement += (old_max - calc_max);
		  //std::cout << "BOOST22: " << it->first << " : " << old_max << " : " << calc_max << std::endl;
		}
	      _common_data->_count_bound_change++;
	    }
	}
      else
	{
	  int c_max = c_handler->get_max();
	  int c_min = c_handler->get_min();
	  int calc_min = cb[it->first];
	  if(calc_min > c_min)
	    {
	      int old_min = div_ceil(lbound - orig_max + coeff * c_max, coeff);
	      if(calc_min > old_min)
		{
		  //std::cout << "BOOST2: " << it->first << " : " << old_min <<  " : " << calc_min << std::endl;
		  _common_data->_count_improved_bound_change++;
		  _common_data->_cumulative_improvement += (calc_min - old_min);
		}
	      _common_data->_count_bound_change++;		  	      
	    }
	}
    }
  return true;
}

class algorithm_comparator {
public:
  typedef sum_constraint_handler::calc_bounds_ptr_type ptr_type;
private:
  unsigned _count_m_better = 0;
  unsigned _count_m_worse = 0;
  unsigned _count_m_same = 0;
  unsigned _cum_m_better = 0;
  unsigned _cum_m_worse = 0;
  unsigned _count_c = 0;
  unsigned _count_c_alt = 0;
  unsigned _count_c_better = 0;
  unsigned _count_c_worse = 0;
  unsigned _count_b_better = 0;
  unsigned _count_b_worse = 0;
  unsigned _count_b_same = 0;
  unsigned _cum_b_better = 0;
  unsigned _cum_b_worse = 0;
public:

  algorithm_comparator()
  {}

  void compare(sum_constraint_handler * h, sum_constraint_handler::bound_type bt, int bound, int m,
	       const std::unordered_map<expression, int> & cb, ptr_type alt_alg)
  {
    int m_alt;
    std::unordered_map<expression, int> cb_alt;
    if(!(h->*alt_alg)(bound, m_alt, cb_alt))
      return;
    
    if(bt == sum_constraint_handler::B_UPPER)
      {
	if(m_alt > m)
	  {
	    _count_m_worse++;
	    _cum_m_worse += m_alt - m;

	    //std::cout << "VARS: " << h->_lp << ", bound: " << bound << " (min: " << m << ", m_alt: " << m_alt << ")" << std::endl;
	    
	    /*	    for(sum_constraint_handler::linear_polynomial::const_iterator it = h->_lp.begin(), it_end = h->_lp.end();
		it != it_end; ++it)
	      {
		csp_theory_solver::csp_theory_solver_data * c_data = h->_theory_solver->get_theory_solver_data(it->first);
		domain_handler * c_handler = c_data->get_variable_domain_handler();

		int c_min = c_handler->get_min();
		int c_max = c_handler->get_max();
		std::cout << "Var: " << it->first <<", coeff: " << it->second << ", low: " << c_min << ", up: " << c_max << std::endl;
	      }
	    exit(1);
	    */
	    
	    if(m_alt > bound)
	      {
		_count_c_alt++;
		if(m <= bound)
		  _count_c_worse++;
		else
		  _count_c++;
		return;
	      }
	  }
	else if(m_alt < m)
	  {
	    _count_m_better++;
	    _cum_m_better += m - m_alt;
	    if(m > bound)
	      {
		_count_c++;
		if(m_alt <= bound)
		  _count_c_better++;
		else
		  _count_c_alt++;
		return;
	      }
	  }
	else
	  {
	    _count_m_same++;
	    if(m > bound)
	      {
		_count_c++;
		_count_c_alt++;
		return;
	      }
	  }
	
	for(const auto & e : cb)
	  {
	    const auto & e_alt = *cb_alt.find(e.first);
	    int coeff = h->_lp.get_coeff(e.first);
	    if(coeff > 0)
	      {
		if(e_alt.second < e.second)
		  {
		    _count_b_worse++;
		    _cum_b_worse += e.second - e_alt.second;
		  }
		else if(e.second < e_alt.second)
		  {
		    _count_b_better++;
		    _cum_b_better += e_alt.second - e.second;
		  }
		else
		  _count_b_same++;
	      }
	    else
	      {
		if(e_alt.second > e.second)
		  {
		    _count_b_worse++;
		    _cum_b_worse += e_alt.second - e.second;
		  }
		else if(e.second > e_alt.second)
		  {
		    _count_b_better++;
		    _cum_b_better += e.second - e_alt.second;
		  }
		else
		  _count_b_same++;
	      }
	  }
      }
    else if(bt == sum_constraint_handler::B_LOWER)
      {
	if(m_alt > m)
	  {
	    _count_m_better++;
	    _cum_m_better += m_alt - m;
	    if(m < bound)
	      {
		_count_c++;
		if(m_alt >= bound)		  
		  _count_c_better++;
		else
		  _count_c_alt++;
		return;
	      }
	  }
	else if(m_alt < m)
	  {
	    _count_m_worse++;
	    _cum_m_worse += m - m_alt;
	    if(m_alt < bound)
	      {
		_count_c_alt++;
		if(m >= bound)
		  _count_c_worse++;
		else
		  _count_c++;
		return;
	      }
	  }
	else
	  {
	    _count_m_same++;
	    if(m < bound)
	      {
		_count_c++;
		_count_c_alt++;
		return;
	      }
	  }
	
	for(const auto & e : cb)
	  {
	    const auto & e_alt = *cb_alt.find(e.first);
	    int coeff = h->_lp.get_coeff(e.first);
	    if(coeff < 0)
	      {
		if(e_alt.second < e.second)
		  {
		    _count_b_worse++;
		    _cum_b_worse += e.second - e_alt.second;
		  }
		else if(e.second < e_alt.second)
		  {
		    _count_b_better++;
		    _cum_b_better += e_alt.second - e.second;
		  }
		else
		  _count_b_same++;
	      }
	    else
	      {
		if(e_alt.second > e.second)
		  {
		    _count_b_worse++;
		    _cum_b_worse += e_alt.second - e.second;
		  }
		else if(e.second > e_alt.second)
		  {
		    _count_b_better++;
		    _cum_b_better += e.second - e_alt.second;
		  }
		else
		  _count_b_same++;
	      }
	  }
      }    
  }
  
  ~algorithm_comparator() {
    std::cout << "COMPARATION STATISTICS:" << std::endl;

    std::cout << "M BETTER COUNT: " << _count_m_better << std::endl;
    std::cout << "M WORSE COUNT: " <<  _count_m_worse << std::endl;
    std::cout << "M SAME COUNT: " <<  _count_m_same << std::endl;
    std::cout << "M DISTRIBUTION (BETTER, WORSE, SAME): "
	      << (double)_count_m_better / (_count_m_better + _count_m_worse + _count_m_same) << ", " 
      	      << (double)_count_m_worse / (_count_m_better + _count_m_worse + _count_m_same) << ", "
      	      << (double)_count_m_same / (_count_m_better + _count_m_worse + _count_m_same) << std::endl;    
    std::cout << "M AVERAGE BETTER: " << (double)_cum_m_better / _count_m_better << std::endl;
    std::cout << "M AVERAGE WORSE: " << (double)_cum_m_worse / _count_m_worse << std::endl;
    std::cout << "C COUNT: " << _count_c << std::endl;
    std::cout << "C COUNT ALT: " << _count_c_alt << std::endl;
    std::cout << "C BETTER COUNT: " <<  _count_c_better << std::endl;
    std::cout << "C WORSE COUNT: " <<  _count_c_worse << std::endl;
    std::cout << "C DISTRIBUTION (BETTER, WORSE): "
	      << (double)_count_c_better / (_count_c_better + _count_c_worse) << ", "
      	      << (double)_count_c_worse / (_count_c_better + _count_c_worse) << std::endl;
    std::cout << "B BETTER COUNT: " <<  _count_b_better << std::endl;    
    std::cout << "B WORSE COUNT: " <<  _count_b_worse << std::endl;
    std::cout << "B SAME COUNT: " <<  _count_b_same << std::endl;
    std::cout << "B DISTRIBUTION (BETTER, WORSE, SAME): "
	      << (double)_count_b_better / (_count_b_better + _count_b_worse + _count_b_same) << ", " 
      	      << (double)_count_b_worse / (_count_b_better + _count_b_worse + _count_b_same) << ", "
      	      << (double)_count_b_same / (_count_b_better + _count_b_worse + _count_b_same) << std::endl;
    std::cout << "B AVERAGE BETTER: " << (double)_cum_b_better / _count_b_better << std::endl;
    std::cout << "B AVERAGE WORSE: " << (double)_cum_b_worse / _count_b_worse << std::endl;
  }
};


//algorithm_comparator acomp;

bool sum_constraint_handler::check_upper(int ubound, unsigned trail_size)
{
  //  std::cout << "CHECK UPPER: " << _constraint << " : " << ubound << std::endl;
  int lp_min;
  std::unordered_map<expression, int> cb(HASH_TABLE_SIZE);
  (this->*_calculate_bounds_min)(ubound, lp_min, cb);
  //std::sort(_pos_vars.begin(), _pos_vars.end(), abs_coeff_comparator(this));
  //acomp.compare(this, B_UPPER, ubound, lp_min, cb, &sum_constraint_handler::calculate_bounds_beldiceanu_min);

  if(ubound < lp_min)
    {      
      explanation expl;
      bound_type bt = lp_min == (int)0x7fffffff ? B_BOTH : B_UPPER;      
      generate_conflict_explanation(bt, expl);
      _theory_solver->get_solver().apply_conflict(expl, _theory_solver);
      _common_data->_count_conflict++;
      return false;
    }
  
  bool retval = false;

  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      int coeff = it->second;
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();

      if(coeff > 0)
	{
	  int c_max = c_handler->get_max();
	  int calc_max = cb[it->first];
	  if(calc_max < c_max)
	    {
	      c_handler->init_upper_bound(calc_max);
	      propagate_literal(c_handler->get_lower_or_equal(calc_max), B_UPPER, trail_size);
	      retval = true;
	    }
	}
      else
	{
	  int c_min = c_handler->get_min();
	  int calc_min = cb[it->first];
	  if(calc_min > c_min)
	    {	 
	      if(_common_data->_upper_bounds_only)
		{
		  c_handler->init_upper_bound(calc_min - 1);
		  propagate_literal(c_handler->get_greater_than(calc_min - 1), B_UPPER, trail_size);
		}
	      else
		{
		  c_handler->init_lower_bound(calc_min);
		  propagate_literal(c_handler->get_greater_or_equal(calc_min), B_UPPER, trail_size);
		}
	      retval = true;
	    }
	}
    }
  return retval;
}

bool sum_constraint_handler::check_lower(int lbound, unsigned trail_size)
{
  //  std::cout << "CHECK LOWER: " << _constraint << " : " << lbound  << std::endl;

  int lp_max;
  std::unordered_map<expression, int> cb(HASH_TABLE_SIZE);
  (this->*_calculate_bounds_max)(lbound, lp_max, cb);
  //std::sort(_pos_vars.begin(), _pos_vars.end(), abs_coeff_comparator(this));
  //acomp.compare(this, B_LOWER, lbound, lp_max, cb, &sum_constraint_handler::calculate_bounds_beldiceanu_max);

  
  if(lp_max < lbound)
    {
      explanation expl;
      bound_type bt = lp_max == (int)0x80000000 ? B_BOTH : B_LOWER;
      generate_conflict_explanation(bt, expl);
      _theory_solver->get_solver().apply_conflict(expl, _theory_solver);
      _common_data->_count_conflict++;
      return false;
    }

  bool retval = false;

  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      int coeff = it->second;
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();
      
      if(coeff < 0)
	{
	  int c_max = c_handler->get_max();
	  int calc_max = cb[it->first];
	  if(calc_max < c_max)
	    {
	      c_handler->init_upper_bound(calc_max);
	      propagate_literal(c_handler->get_lower_or_equal(calc_max), B_LOWER, trail_size);
	      retval = true;
	    }
	}
      else
	{
	  int c_min = c_handler->get_min();	 
	  int calc_min = cb[it->first];
	  if(calc_min > c_min)
	    {
	      if(_common_data->_upper_bounds_only)
		{
		  c_handler->init_upper_bound(calc_min - 1);
		  propagate_literal(c_handler->get_greater_than(calc_min - 1), B_LOWER, trail_size);
		}
	      else
		{
		  c_handler->init_lower_bound(calc_min);
		  propagate_literal(c_handler->get_greater_or_equal(calc_min), B_LOWER, trail_size);
		}
	      retval = true;
	    }
	}
    }
  return retval;
}

void sum_constraint_handler::check_split(int bound, unsigned trail_size)
{
  //std::cout << "CHECK SPLIT: " << bound << std::endl;
  linear_polynomial::const_iterator found = _lp.end();
  int lp_value = 0;
  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      int coeff = it->second;
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();
      
      if(!c_handler->is_finite())
	return;

      if(c_handler->get_min() != c_handler->get_max())
	{
	  if(found == _lp.end())
	    found = it;
	  else
	    return;
	}
      else
	{
	  lp_value += coeff * c_handler->get_min();
	}
    }
  
  if(found == _lp.end())
    {
      if(lp_value == bound)
	{
	  explanation expl;
	  generate_conflict_explanation(B_SPLIT, expl);
	  _theory_solver->get_solver().apply_conflict(expl, _theory_solver);
	  _common_data->_count_conflict++;
	  return;
	}
    }
  else
    {
      int coeff = found->second;
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(found->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();
      int slack = bound - lp_value;
      if(slack % coeff == 0)
	{
	  int forbidden = slack / coeff;
	  if(c_handler->in_domain(forbidden))
	    {
	      c_handler->init_literal_pair(forbidden, true);
	      propagate_literal(c_handler->get_disequality(forbidden), B_SPLIT, trail_size);
	    }
	}
    }
}

void sum_constraint_handler::propagate_literal(const expression & l, bound_type bt, unsigned trail_size)
{
  const extended_boolean l_value = _theory_solver->get_solver().get_trail().get_value(l);

  if(l_value == EB_TRUE)
    return;

  propagation_info & pi = _stored_info[l];
  pi.bt = bt;
  pi.trail_size = trail_size;

  if(l_value == EB_UNDEFINED)
    {
      _theory_solver->get_theory_solver_data(l)->set_explanation_handler(this);      
      _theory_solver->get_solver().apply_propagate(l, _theory_solver);
      //      std::cout << "SUM: " << _constraint << " propagated: " << l << std::endl;
      _common_data->_count_propagate++;
    }
  else if(l_value == EB_FALSE)
    {
      explanation conflicting;
      expression l_opp = _theory_solver->get_solver().get_literal_data(l)->get_opposite();
      generate_propagation_explanation(l, conflicting);
      conflicting.push_back(l_opp);
      _theory_solver->get_solver().apply_conflict(conflicting, _theory_solver);
      _common_data->_count_conflict++;
    } 
}

void sum_constraint_handler::generate_explanation(unsigned trail_size, bound_type bt, explanation & expl, const expression & l)
{
  extended_boolean eb_value = _theory_solver->get_solver().get_trail().get_value(_constraint);
  if(eb_value == EB_TRUE)
    expl.push_back(_constraint);
  else if(eb_value == EB_FALSE)
    expl.push_back(_not_constraint);
  else
    throw "FATAL";

  //  std::cout << "EXPLAIN FROM: " << expl[0] << std::endl;
  
  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(it->first);
      domain_handler * c_handler = c_data->get_variable_domain_handler();
      int coeff = it->second;
      
      if(l.get() != 0 && l->get_operands()[0] == it->first)
	continue;

      //std::cout << "EXPLAINING BOUND: " << it->first << std::endl;
      
      if(bt == B_UPPER)
	{	
	  if(coeff > 0)
	    c_handler->explain_minimum_at_trail_size(trail_size, expl);
	  else
	    c_handler->explain_maximum_at_trail_size(trail_size, expl);
	}
      else if(bt == B_LOWER)
	{	  
	  if(coeff > 0)
	    c_handler->explain_maximum_at_trail_size(trail_size, expl);
	  else
	    c_handler->explain_minimum_at_trail_size(trail_size, expl);
	}
      else if(bt == B_BOTH)
	{
	  c_handler->explain_minmax_at_trail_size(trail_size, expl);
	}
      else // bt == B_SPLIT 
	{
	  c_handler->explain_domain_at_trail_size(trail_size, expl);	  
	}
      
      //      std::cout << "CURR: " << it->first << " : " << expl << std::endl;
    }
}

void sum_constraint_handler::generate_propagation_explanation(const expression & l, explanation & expl)
{
  generate_explanation(_stored_info[l].trail_size, _stored_info[l].bt, expl, l);
}

void sum_constraint_handler::generate_conflict_explanation(bound_type bt, explanation & expl)
{
  generate_explanation(_theory_solver->get_solver().get_trail().size(), bt, expl, expression());
}

sum_constraint_handler::sum_constraint_handler(csp_theory_solver * th, const expression & l_pos, const expression & l_neg)
  :constraint_handler(th, l_pos, l_neg),
   _stored_info(HASH_TABLE_SIZE),
   _self_bt(B_SPLIT),
   _common_data(&th->_sum_common_data),
   _cached_intersections(HASH_TABLE_SIZE),
   _intersections_inited(false),
   _pos_covering_alldiff(0),
   _pos_covered(false),
   _all_coeffs_one(false),
   _calculate_bounds_min(nullptr),
   _calculate_bounds_max(nullptr)
{
  //  std::cout << "INIT: " << l_pos << " : " << l_neg << std::endl;

  _common_data->_instance_count++;
  const function_symbol & fs = l_pos->get_symbol();
  expression left, right;
  if(fs == function_symbol::LE)
    {
      left = l_pos->get_operands()[0];
      right = l_pos->get_operands()[1];
      _bt_pos = B_UPPER;
      _bt_neg = B_LOWER;
      _bound_pos = _theory_solver->get_value_from_expr(right);
      _bound_neg = _bound_pos + 1;
      _theory_solver->get_polynomial_from_expression(left, _lp);
    }
  else if(fs == function_symbol::GE)
    {
      left = l_pos->get_operands()[0];
      right = l_pos->get_operands()[1];
      _bt_pos = B_LOWER;
      _bt_neg = B_UPPER;
      _bound_pos = _theory_solver->get_value_from_expr(right);
      _bound_neg = _bound_pos - 1;
      _theory_solver->get_polynomial_from_expression(left, _lp);
    }
  else if(fs == function_symbol::EQ)
    {
      expression eq = _theory_solver->canonize_relation(l_pos);
      left = eq->get_operands()[0];
      right = eq->get_operands()[1];
      _bt_pos = B_BOTH;
      _bt_neg = B_SPLIT;
      _bound_pos = _theory_solver->get_value_from_expr(right);
      _bound_neg = _bound_pos;
      _theory_solver->get_polynomial_from_expression(left, _lp);
    }

  csp_theory_solver::csp_theory_solver_data * cons_data = _theory_solver->get_theory_solver_data(_constraint);
  _all_coeffs_one = true;
  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      csp_theory_solver::csp_theory_solver_data * var_data = _theory_solver->add_variable(it->first);
      var_data->add_variable_constraint(_constraint);
      cons_data->add_constraint_variable(it->first);
      if(it->second >= 0)
	_pos_vars.push_back(it->first);
      else
	_neg_vars.push_back(it->first);

      if(it->second != 1)
	_all_coeffs_one = false;
    }

  std::sort(_pos_vars.begin(), _pos_vars.end(), abs_coeff_comparator(this));
  std::sort(_neg_vars.begin(), _neg_vars.end(), abs_coeff_comparator(this));
  
  //  std::cout << "POS: " << _bound_pos << " (" << _bt_pos << ")" << std::endl;
  //  std::cout << "NEG: " << _bound_neg << " (" << _bt_neg << ")" << std::endl;
  //std::cout << "POLY: " << _lp << std::endl;
}

void sum_constraint_handler::check_and_propagate(unsigned layer) 
{
  if(_theory_solver->get_solver().is_conflict())
    {
      return;
    }

  init_alldiff_intersections();
  extended_boolean eb = _theory_solver->get_solver().get_trail().get_value(_constraint);
  if(eb == EB_UNDEFINED)
    return; // never happens

  if(_just_activated)
    {
      if(eb == EB_TRUE)
	propagate_weaker_from_pos_constraints();
      else
	propagate_weaker_from_neg_constraints();
      _just_activated = false;
    }
    
  unsigned trail_size = _theory_solver->get_solver().get_trail().size();
  
  if(eb == EB_FALSE)
    {
      //std::cout << "CHECK CONSTRAINT: " << _not_constraint << " : " << _bound_neg << " (" << _bt_neg << ")" << std::endl;
      switch(_bt_neg)
	{
	case B_UPPER:
	  check_upper(_bound_neg, trail_size);
	  break;
	case B_LOWER:
	  check_lower(_bound_neg, trail_size);
	  break;
	case B_BOTH:
	  // B_BOTH is never negative constraint
	  break;
	case B_SPLIT:
	  check_split(_bound_neg, trail_size);
	  break;
	}
    }
  else if(eb == EB_TRUE)
    {
      //std::cout << "CHECK CONSTRAINT: " << _constraint << " : " << _bound_pos << " (" << _bt_pos << ")" << std::endl;
      switch(_bt_pos)
	{
	case B_UPPER:
	  check_upper(_bound_pos, trail_size);
	  break;
	case B_LOWER:
	  check_lower(_bound_pos, trail_size);
	  break;
	case B_BOTH:
	  {
	    bool cu = check_upper(_bound_pos, trail_size);
	    bool cl = check_lower(_bound_pos, trail_size);
	    _reprocessing_flag = cu || cl; 
	    //if(cu || cl)
	      //std::cout << "Constraint: " << _constraint << " must be reprocessed!!" << std::endl;
	  }
	  break;
	case B_SPLIT:
	  // B_SPLIT is never positive constraint
	  break;
	}
    }
  _theory_solver->process_main_trail();
}

void sum_constraint_handler::explain_literal(const expression & l) 
{
  assert(_theory_solver->has_theory_solver_data(l));
  explanation expl;
  if(_theory_solver->get_theory_solver_data(l)->has_constraint_data())
    {
      if(l == _constraint || l == _not_constraint)
	{
	  //	  std::cout << "Self explaining: " << l << std::endl;
	  generate_self_explanation(expl);
	}
      else
	{	  
	  //	  std::cout << "Explaining : " << l << " from: " << _constraint << std::endl;
	  expl.push_back(_theory_solver->get_solver().get_trail().get_value(_constraint) == EB_TRUE ?
			 _constraint : _not_constraint);
	}
    }
  else
    {
      generate_propagation_explanation(l, expl);
    }
  _theory_solver->get_solver().apply_explain(l, expl);
}

void sum_constraint_handler::generate_self_explanation(explanation & expl)
{
  for(linear_polynomial::const_iterator it = _lp.begin(), it_end = _lp.end();
      it != it_end; ++it)
    {
      const expression & var = it->first;
      int coeff = it->second;
      csp_theory_solver::csp_theory_solver_data * v_data = _theory_solver->get_theory_solver_data(var);
      assert(v_data != 0);
      domain_handler * d_handler = v_data->get_variable_domain_handler();
      if(_self_bt == B_BOTH)
	d_handler->explain_domain_at_trail_size(_theory_solver->get_solver().get_trail().get_trail_index(_constraint), expl);
      else if(_self_bt == B_UPPER)
	{
	  if(coeff > 0)
	    d_handler->explain_maximum_at_trail_size(_theory_solver->get_solver().get_trail().get_trail_index(_constraint), expl);
	  else
	    d_handler->explain_minimum_at_trail_size(_theory_solver->get_solver().get_trail().get_trail_index(_constraint), expl);
	}
      else if(_self_bt == B_LOWER)
	{
	  if(coeff > 0)
	    d_handler->explain_minimum_at_trail_size(_theory_solver->get_solver().get_trail().get_trail_index(_constraint), expl);
	  else
	    d_handler->explain_maximum_at_trail_size(_theory_solver->get_solver().get_trail().get_trail_index(_constraint), expl);
	}
    }
}

int sum_constraint_handler::get_variable_value_hint(const expression & var)
{
  extended_boolean eb = _theory_solver->get_solver().get_trail().get_value(_constraint);
  csp_theory_solver::csp_theory_solver_data * v_data = _theory_solver->get_theory_solver_data(var);
  domain_handler * v_handler = v_data->get_variable_domain_handler();
  int coeff = _lp.get_coeff(var);

  switch(eb == EB_FALSE ? _bt_neg : _bt_pos)
    {
    case B_UPPER:
      return coeff > 0 ? v_handler->get_min() : v_handler->get_max();
      break;
    case B_LOWER:
      return coeff > 0 ? v_handler->get_max() : v_handler->get_min();
      break;
    case B_BOTH:
      return v_handler->get_median();
      break;
    case B_SPLIT:
      return v_handler->get_median();
      break;
    }
  return 0;
}

bool sum_constraint_handler::is_similar_constraint(const expression & c, bound_type & bt, int & bound)
{
  csp_theory_solver::csp_theory_solver_data * c_data = _theory_solver->get_theory_solver_data(c);
  if(c_data == 0 || !c_data->has_constraint_data())
    return false;
  
  sum_constraint_handler * c_handler = dynamic_cast<sum_constraint_handler*>(c_data->get_constraint_handler());
  if(c_handler == 0)
    return false;
  
  if(c_handler->_lp != _lp)
    return false;
  
  if(c == c_handler->_constraint)
    {
      bt = c_handler->_bt_pos;
      bound = c_handler->_bound_pos;
    }
  else
    {
      bt = c_handler->_bt_neg;
      bound = c_handler->_bound_neg;
    }
  return true;
}

bool sum_constraint_handler::is_weaker(const bound_type bt1, const int bound1, const bound_type bt2, const int bound2)
{
  if(bt1 == B_BOTH)
    {
      if(bt2 == B_BOTH)
	return bound2 == bound1;
      else if(bt2 == B_UPPER)
	return bound2 >= bound1;
      else if(bt2 == B_LOWER)
	return bound2 <= bound1;
      else // if(bt2 == B_SPLIT)
	return bound2 != bound1;
    }
  else if(bt1 == B_UPPER)
    {
      if(bt2 == B_UPPER)
	return bound2 >= bound1;
      else if(bt2 == B_SPLIT)
	return bound2 > bound1;
      else
	return false;
    }
  else if(bt1 == B_LOWER)
    {
      if(bt2 == B_LOWER)
	return bound2 <= bound1;
      else if(bt2 == B_SPLIT)
	return bound2 < bound1;
      else 
	return false;
    }
  else // bt1 == B_SPLIT
    {
      if(bt2 == B_SPLIT)
	return bound2 == bound1;
      else
	return false;
    }
}

bool sum_constraint_handler::is_weaker_from_pos(const expression & c)
{
  int bound;
  bound_type bt;
  if(!is_similar_constraint(c, bt, bound))
    return false;

  return is_weaker(_bt_pos, _bound_pos, bt, bound);
}

bool sum_constraint_handler::is_weaker_from_neg(const expression & c)
{
  int bound;
  bound_type bt;
  if(!is_similar_constraint(c, bt, bound))
    return false;

  return is_weaker(_bt_neg, _bound_neg, bt, bound);
}

bool sum_constraint_handler::holds(const int min_value, const int max_value, const bound_type bt, const int bound)
{
  if(bt == B_BOTH)
    return min_value == bound && max_value == bound;
  else if(bt == B_UPPER)
    return max_value <= bound;
  else if(bt == B_LOWER)
    return min_value >= bound;
  else // bt == B_SPLIT
    return max_value < bound || min_value > bound;
}

void sum_constraint_handler::set_self_bt(const int min_value, const int max_value, const bound_type bt, const int bound)
{
  if(bt == B_SPLIT)
    _self_bt = max_value < bound ? B_UPPER : B_LOWER;
  else
    _self_bt = bt;
}

void sum_constraint_handler::check_implied() 
{
  assert(_theory_solver->get_solver().get_trail().get_value(_constraint) == EB_UNDEFINED);
  
  int min_value = 0, max_value = 0;
  if(!calculate_min(min_value))
    return;
  if(!calculate_max(max_value))
    return;

  if(holds(min_value, max_value, _bt_pos, _bound_pos))
    {
      set_self_bt(min_value, max_value, _bt_pos, _bound_pos);
      _theory_solver->get_theory_solver_data(_constraint)->set_explanation_handler(this);      
      _theory_solver->get_solver().apply_propagate(_constraint, _theory_solver);
      //std::cout << "Constraint: " << _constraint << " self propagated!" << std::endl;
    }
  else if(holds(min_value, max_value, _bt_neg, _bound_neg))
    {
      set_self_bt(min_value, max_value, _bt_neg, _bound_neg);
      _theory_solver->get_theory_solver_data(_not_constraint)->set_explanation_handler(this);      
      _theory_solver->get_solver().apply_propagate(_not_constraint, _theory_solver);
      //std::cout << "Constraint: " << _not_constraint << " self propagated!" << std::endl;
    }
}
