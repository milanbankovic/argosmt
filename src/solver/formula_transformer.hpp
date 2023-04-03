/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2021, 2023 Milan Bankovic (milan@matf.bg.ac.rs)

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
#include "proofs.hpp"

/* Transforms formulae to CNF form */
class formula_transformer {
private:
  expression_factory * _exp_factory;
  sort_factory * _sort_factory;
  signature * _sig;  
  static unsigned long _count;
  std::unordered_map<expression, expression> _names;
  std::unordered_map<expression, expression> _named_exprs;
  expression_vector _names_vector;
  std::unordered_map<sort, expression> _sort_constants;
  bool _produce_proofs;
  
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
  
  expression try_expand_def(const expression & e);

  void check_definition(const expression & expr, std::vector<clause *> & clauses);
  
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

  void register_name(const expression & expr, const expression & name)
  {
    _names.insert(std::make_pair(expr, name));
    _named_exprs.insert(std::make_pair(name, expr));
    _names_vector.push_back(name);
  }

  void retract_names(unsigned size)
  {
    while(_names_vector.size() > size)
      {
	expression name = _names_vector.back();
	expression expr = _named_exprs[name];
	_names.erase(expr);
	_named_exprs.erase(name);
	_names_vector.pop_back();
      }
  }


  void retract_clauses(unsigned size, std::vector<clause *> & clauses)
  {
    while(clauses.size() > size)
      {
	clause * cl = clauses.back();
	delete cl;
	clauses.pop_back();
      }
  }

public:
  formula_transformer(expression_factory * factory)
    :_exp_factory(factory),
     _names(HASH_TABLE_SIZE),
     _named_exprs(HASH_TABLE_SIZE),
     _sort_constants(HASH_TABLE_SIZE),
     _produce_proofs(false)
  {
    _sig = _exp_factory->get_signature();
    _sort_factory = _sig->get_sort_factory();
    while(_sig->get_expansion_signature() != nullptr)
      _sig = _sig->get_expansion_signature();
  }

  bool is_true(const expression & e)
  {
    static const expression TRUE = _exp_factory->TRUE_EXPRESSION();
    static const expression NOT_FALSE = _exp_factory->create_expression(function_symbol::NOT,
									_exp_factory->FALSE_EXPRESSION());
    return e == TRUE || e == NOT_FALSE;
  }


  bool is_false(const expression & e)
  {
    static const expression FALSE = _exp_factory->FALSE_EXPRESSION();
    static const expression NOT_TRUE = _exp_factory->create_expression(function_symbol::NOT,
								       _exp_factory->TRUE_EXPRESSION());
    return e == FALSE || e == NOT_TRUE;
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
    if(s == _sort_factory->BOOL_SORT())
      return _exp_factory->TRUE_EXPRESSION();

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

  bool produce_proofs() const
  {
    return _produce_proofs;
  }

  void set_produce_proofs(bool pp)
  {
    _produce_proofs = pp;
  }
  
  void cnf_transformation(const expression & expr, 
			  std::vector<clause *> & clauses, 
			  expression & name);
  
  void top_level_cnf_transformation(const expression & expr, 
				    std::vector<clause *> & clauses);

  expression expand_names(const expression & expr);

};

#endif // _FORMULA_TRANSFORMER_H
