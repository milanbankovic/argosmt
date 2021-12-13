/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2021 Milan Bankovic (milan@matf.bg.ac.rs)

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
#ifndef _FORMULA_TRANSFORMER_H
#define _FORMULA_TRANSFORMER_H

#include "clause.hpp"

/* Simplifies formula and transforms it to CNF form */
class formula_transformer {
private:
  expression_factory * _exp_factory;
  sort_factory * _sort_factory;
  signature * _sig;  
  static unsigned long _count;
  std::unordered_map<expression, expression> _names;
  std::unordered_map<sort, expression> _sort_constants;
  
  bool is_chainable(const expression & e)
  {
    const function_symbol & fs = e->get_symbol();
    return 
      fs == function_symbol::EQ ||
      fs == function_symbol::LT ||
      fs == function_symbol::LE ||
      fs == function_symbol::GT ||
      fs == function_symbol::GE;
  }

  bool is_pairwise(const expression & e)
  {
    const function_symbol & fs = e->get_symbol();
    return 
      fs == function_symbol::DISTINCT;
  }

  expression negate_name(const expression & name)
  {
    if(name->is_not())
      return name->get_operands()[0];
    else
      return _exp_factory->create_expression(function_symbol::NOT, name);
  }

  void set_sort_constant(const expression & expr)
  {
    expression iexpr = expr->infer_sorts();
    sort es = iexpr->get_sort();
    if(es != _sort_factory->BOOL_SORT() && _sort_constants.find(es) == _sort_constants.end())
      {
	_sort_constants.insert(std::make_pair(es, iexpr));
      }
  }
  
public:
  formula_transformer(expression_factory * factory)
    :_exp_factory(factory),
     _names(HASH_TABLE_SIZE),
     _sort_constants(HASH_TABLE_SIZE)
  {
    _sig = _exp_factory->get_signature();
    _sort_factory = _sig->get_sort_factory();
    while(_sig->get_expansion_signature() != nullptr)
      _sig = _sig->get_expansion_signature();
  }

  expression get_unique_constant(const std::string & prefix, const sort & s)
  {
    function_symbol uniq_name(prefix + std::to_string(++_count));
    _sig->add_function_symbol(uniq_name, s);
    //std::cout << "NEW UNIQUE NAME: " << uniq_name << std::endl;
    return _exp_factory->create_expression(uniq_name);
  }

  expression get_sort_constant(const sort & s)
  {
    auto it = _sort_constants.find(s);
    if(it != _sort_constants.end())
      return it->second;
    else
      {
	expression sc = get_unique_constant(std::string("sc"), s);
	_sort_constants.insert(std::make_pair(s, sc));
	return sc;										 
      }
  }
  
  expression simplification(const expression & expr);
  
  void cnf_transformation(const expression & expr, 
			  std::vector<clause *> & clauses, 
			  expression & name);
  
  void top_level_cnf_transformation(const expression & expr, 
				    std::vector<clause *> & clauses);

  formula_transformer * clone(expression_factory * factory)
  {
    formula_transformer * ft = new formula_transformer(factory);
    for(auto it = _names.begin(), it_end = _names.end(); it != it_end; ++it)
      {
	ft->_names.insert(std::make_pair(it->first->clone_expression(factory),
					 it->second->clone_expression(factory)));
      }
    return ft;
  }
};

#endif // _FORMULA_TRANSFORMER_H
