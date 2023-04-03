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
#include "formula_transformer.hpp"

unsigned long formula_transformer::_count = 0UL;

expression formula_transformer::try_expand_def(const expression & e)
{
  const function_symbol & fs = e->get_symbol();
  const expression_vector & ops = e->get_operands();
  sort_vector op_sorts;
  op_sorts.reserve(ops.size());
  for(unsigned i = 0; i < ops.size(); i++)
    op_sorts.push_back(ops[i]->get_sort());
  sort sr = e->get_sort();
  function_symbol_checker * checker = _sig->check_function_symbol(fs, op_sorts, sr);

  if(checker == nullptr)
    {
      return e;
    }
  
  const attribute_set & attr = checker->get_attributes(fs, op_sorts, sr);

  attribute_set::const_iterator it = attr.find(keyword::DEFINITION);

  if(it != attr.end())
    {
      function_definition_attribute_value * value = 
	dynamic_cast<function_definition_attribute_value *>(it->get_value());
      assert(value != 0);
      const function_definition & def = value->get_value();

      const sorted_variable_vector & vars = def.get_variables();
      expression def_exp = def.get_expression();

      substitution sub;
      sorted_variable_vector::const_iterator 
	vit = vars.begin(), 
	vit_end = vars.end();
      expression_vector::const_iterator eit = ops.begin();

      assert(vars.size() == ops.size());
      
      for(; vit != vit_end; ++vit, ++eit)
	{
	  sub.insert(std::make_pair(vit->get_variable(), *eit)); 
	}

      return def_exp->get_instance(sub)->eliminate_let_binders();
    }
  else
    return e;
}

void formula_transformer::check_definition(const expression & expr, std::vector<clause *> & clauses)
{
  expression expanded_expr = try_expand_def(expr);
  if(expanded_expr != expr)
    {
      //std::cout << "EXPANDING: " << expr << " TO " << expanded_expr << std::endl;
      expression eq = _exp_factory->create_expression(function_symbol::EQ, expr, expanded_expr);
      if(_names.find(eq) == _names.end())
	{
	  expression eq_name;
	  cnf_transformation(eq, clauses, eq_name);
	  clause * eq_unit = new clause();
	  eq_unit->push_back(eq_name);
	  clauses.push_back(eq_unit);
	}
    }
}


void formula_transformer::cnf_transformation(const expression & expr, 
					     std::vector<clause *> & clauses, 
					     expression & name)
{
  static const std::string cnf_prefix = std::string("uniq");
  static const std::string ite_prefix = std::string("ite");
  
  auto it = _names.find(expr);
  if(it != _names.end())
    {
      name = it->second;
      return;
    }
  
  // This covers special constants, variables, constant functions and quantifiers
  if(!expr->is_function() || expr->get_operands().empty())
    {
      name = expr;

      if(!expr->is_variable())
	set_sort_constant(expr);

      if(expr->is_constant_function())
	{
	  register_name(expr, name);
	  check_definition(expr, clauses);
	}            
      return;
    }
  
  unsigned clauses_size = clauses.size();
  unsigned names_size = _names_vector.size();

  /* Binarization of chainables */
  if(is_chainable(expr) && expr->get_operands().size() > 2)
    {

      // (fs t1 t2 ... tn) <=> (fs t1 t2) /\ (fs t2 t3) /\ ... /\ (fs tn-1 tn)
      //       s           <=>    s1      /\    s2      /\ ... /\     sn-1
      //  ~s \/ s1, ~s \/ s2, ...., ~s \/ s_n-1
      // ~s1 \/ ~s2 \/ ... \/ ~sn-1 \/ s

      const function_symbol & fs = expr->get_symbol();
      name = get_unique_constant(cnf_prefix, _sort_factory->BOOL_SORT());
      expression not_name = negate_name(name);
      clause * long_clause = new clause();

      for(expression_vector::const_iterator it = expr->get_operands().begin(),
	    it_end = expr->get_operands().end() - 1; it != it_end; ++it)
	{
	  expression it_name;
	  cnf_transformation(_exp_factory->create_expression(fs, *it, *(it + 1)), clauses, it_name);
	  long_clause->push_back(negate_name(it_name));
	  clause * short_clause = new clause();
	  short_clause->push_back(it_name);
	  short_clause->push_back(not_name);
	  clauses.push_back(short_clause);
	}
      long_clause->push_back(name);
      clauses.push_back(long_clause);
      register_name(expr, name);
      return;
    }

  /* Binarization of distinct and elimination of inequivalence */
  if(expr->is_distinct() && (expr->get_operands().size() > 2 || expr->is_inequivalence()))
    {
      // (!= t1 t2 ... tn) <=> t1 != t2 /\ t1 != t3 /\ ... /\ tn-1 != tn
      // (!= t1 t2 ... tn) <=> ~(t1 = t2) /\ ~(t1 = t3) /\ ... /\ ~(tn-1 = tn)
      //       s           <=>    ~s12    /\    ~s13     /\ ... /\   ~sn-1,n
      //  ~s \/ ~s12, ~s \/ ~s13, ... , ~s \/ ~sn-1,n
      // s12 \/ s13 \/ ... \/ sn-1,n \/ s      
      name = get_unique_constant(cnf_prefix, _sort_factory->BOOL_SORT());
      expression not_name = negate_name(name);
      clause * long_clause = new clause();      
      for(expression_vector::const_iterator it = expr->get_operands().begin(),
	    it_end = expr->get_operands().end() - 1; it != it_end; ++it)
	for(expression_vector::const_iterator jt = it + 1, 
	      jt_end = expr->get_operands().end(); jt != jt_end; ++jt)
	  {
	    expression it_name;
	    cnf_transformation(_exp_factory->create_expression(function_symbol::EQ, *it, *jt), clauses, it_name);
	    long_clause->push_back(it_name);
	    clause * short_clause = new clause();
	    short_clause->push_back(negate_name(it_name));
	    short_clause->push_back(not_name);
	    clauses.push_back(short_clause);	    
	  }
      long_clause->push_back(name);
      clauses.push_back(long_clause);
      register_name(expr, name);
      return;
    }

  /* Binarization and elimination of xor */
  if(expr->is_xor())
    {
      expression first;
      if(expr->get_operands().size() > 2)
	{
	  expression_vector ops;
	  ops.reserve(expr->get_operands().size() - 1);
	  for(expression_vector::const_iterator it = expr->get_operands().begin(),
		it_end = expr->get_operands().end() - 1; it != it_end; ++it)
	    ops.push_back(*it);
	  cnf_transformation(_exp_factory->create_expression(function_symbol::XOR, ops), clauses, first);	  
	}
      else
	{
	  cnf_transformation(expr->get_operands()[0], clauses, first);
	}

      expression second;
      cnf_transformation(expr->get_operands().back(), clauses, second);
      
      name = get_unique_constant(cnf_prefix, _sort_factory->BOOL_SORT());
      expression not_name = negate_name(name);
      expression not_first = negate_name(first);
      expression not_second = negate_name(second);
      
      if(first == second ||
	 (is_true(first) && is_true(second)) ||
	 (is_false(first) && is_false(second)))
	{
	  // SIMPLIFICATION: s1 === s2, only unit clause { ~s } is added
	  retract_names(names_size);
	  retract_clauses(clauses_size, clauses);
	  clause * cl = new clause();
	  cl->push_back(not_name);
	  clauses.push_back(cl);
	}
      else if(first == negate_name(second) ||
	      (is_true(first) && is_false(second)) ||
	      (is_false(first) && is_true(second)))
	{
	  // SIMPLIFICATION: s1 === ~s2, only unit clause { s } is added
	  retract_names(names_size);
	  retract_clauses(clauses_size, clauses);
	  clause *cl = new clause();
	  cl->push_back(name);
	  clauses.push_back(cl);
	}
      else if(is_true(first))
	{
	  // SIMPLIFICATION: s1 === true, then s <=> ~s2:  { ~s, ~s2 }, {s2, s }
	  clause * cl = new clause();
	  cl->push_back(not_second);
	  cl->push_back(not_name);
	  clauses.push_back(cl);
	  cl = new clause();
	  cl->push_back(second);
	  cl->push_back(name);
	  clauses.push_back(cl);
	}
      else if(is_false(first))
	{
	  // SIMPLIFICATION: s1 == false, then s <=> s2: { s, ~s2 }, {~s, s2}
	  clause * cl = new clause();
	  cl->push_back(not_second);
	  cl->push_back(name);
	  clauses.push_back(cl);
	  cl = new clause();
	  cl->push_back(second);
	  cl->push_back(not_name);
	  clauses.push_back(cl);	  
	}
      else if(is_true(second))
	{
	  // SIMPLIFICATION: s2 === true, then s <=> ~s1:  { ~s, ~s1 }, {s1, s }
	  clause * cl = new clause();
	  cl->push_back(first);
	  cl->push_back(name);
	  clauses.push_back(cl);
	  cl = new clause();
	  cl->push_back(not_first);
	  cl->push_back(not_name);
	  clauses.push_back(cl);
	}
      else if(is_false(second))
	{
	  // SIMPLIFICATION: s2 == false, then s <=> s1: { s, ~s1 }, {~s, s1}
	  clause * cl = new clause();
	  cl->push_back(not_first);
	  cl->push_back(name);
	  clauses.push_back(cl);
	  cl = new clause();
	  cl->push_back(first);
	  cl->push_back(not_name);
	  clauses.push_back(cl);	  
	}
      else {
	// GENERAL CASE:
	/* (s1 xor s2) <=> s	   
	   (s1 \/ s2  \/ ~s) /\ (~s1 \/ ~s2 \/ ~s) /\
	   (~s1 \/ s2 \/ s) /\ (~s2 \/ s1 \/ s)  */

	clause * cl = new clause();	
	cl->push_back(not_first);
	cl->push_back(second);
	cl->push_back(name);
	clauses.push_back(cl);
	
	cl = new clause();
	cl->push_back(first);
	cl->push_back(not_second);
	cl->push_back(name);
	clauses.push_back(cl);

	cl = new clause();
	cl->push_back(first);
	cl->push_back(second);
	cl->push_back(not_name);
	clauses.push_back(cl);
	
	cl = new clause();
	cl->push_back(not_first);
	cl->push_back(not_second);
	cl->push_back(not_name);
	clauses.push_back(cl);
      }
      
      register_name(expr, name);
      return;
    }


  /* Elimination of ite */
  if(expr->is_ite())
    {
      name = get_unique_constant(cnf_prefix, expr->get_sort());
      register_name(expr, name);
      expression cond = expr->get_operands()[0];
      expression first = expr->get_operands()[1];
      expression second = expr->get_operands()[2];

	          
      if(is_true(cond))
	{
	  expression first_name;
	  cnf_transformation(_exp_factory->create_expression(function_symbol::EQ, expr, first), clauses, first_name);
	  clause * cl = new clause();
	  cl->push_back(first_name);
	  clauses.push_back(cl);
	}
      else if(is_false(cond))
	{
	  expression second_name;
	  cnf_transformation(_exp_factory->create_expression(function_symbol::EQ, expr, second), clauses, second_name);
	  clause * cl = new clause();
	  cl->push_back(second_name);
	  clauses.push_back(cl);
	}
      else
	{
	  expression first_name;
	  expression second_name;
	  expression cond_name;
	  cnf_transformation(cond, clauses, cond_name);
	  cnf_transformation(_exp_factory->create_expression(function_symbol::EQ, expr, first), clauses, first_name);
	  cnf_transformation(_exp_factory->create_expression(function_symbol::EQ, expr, second), clauses, second_name);
	  clause * cl = new clause();
	  cl->push_back(negate_name(cond_name));
	  cl->push_back(first_name);
	  clauses.push_back(cl);
	  cl = new clause();
	  cl->push_back(cond_name);
	  cl->push_back(second_name);
	  clauses.push_back(cl);
	}
      return;
    }
  
  
  expression_vector op_names;
  for(expression_vector::const_iterator it = expr->get_operands().begin(),
	it_end = expr->get_operands().end(); it != it_end; ++it)
    {
      expression it_name;
      cnf_transformation(*it, clauses, it_name);	
      op_names.push_back(it_name);
    }

  set_sort_constant(expr);

  if(expr->is_not())
    {
      name = negate_name(op_names[0]);
    }
  else if(expr->is_and())
    {
      /* s1 /\ s2 /\ ... /\ sn <=> s
	 (~s1 \/ ~s2 \/ ... \/ ~sn \/ s) /\ (~s \/ s1) /\ (~s \/ s2) /\ ...
	 /\ (~s \/ sn) */

      name = get_unique_constant(cnf_prefix, _sort_factory->BOOL_SORT());
      expression not_name = negate_name(name);
      clause * long_clause = new clause();


      bool false_present = false;
      for(expression_vector::const_iterator it = op_names.begin(),
	    it_end = op_names.end(); it != it_end; ++it)
	{
	  if(is_false(*it))
	    false_present = true;
	}
      
      if(false_present)
	{
	  // SIMPLIFICATION: if one of si's is false, then only { ~s } unit
	  // clause should be added (we retract names and clauses introduced
	  // for si's)
	  retract_clauses(clauses_size, clauses);
	  retract_names(names_size);
	  long_clause->push_back(not_name);
	  clauses.push_back(long_clause);	  
	}
      else
	{
	  for(expression_vector::const_iterator it = op_names.begin(),
		it_end = op_names.end(); it != it_end; ++it)
	    {
	      // SIMPLIFICATION: if si is true, clause ~s \/ si can be omitted.
	      // Also, ~si is omitted from the long clause
	      if(is_true(*it))
		continue;
	      
	      clause * short_clause = new clause();
	      short_clause->push_back(not_name);
	      short_clause->push_back(*it);
	      clauses.push_back(short_clause);
	      long_clause->push_back(negate_name(*it));
	    }
	  long_clause->push_back(name);
	  clauses.push_back(long_clause);
	}
    }
  else if(expr->is_or())
    {
      /* s1 \/ s2 \/ ... \/ sn <=> s
	 (~s1 \/ s) /\ (~s2 \/ s) /\ ... /\ (~sn \/ s) /\ 
	 (s1 \/ s2 \/ ... \/ sn \/ ~s) */
	
      name = get_unique_constant(cnf_prefix, _sort_factory->BOOL_SORT());
      expression not_name = negate_name(name);
      clause * long_clause = new clause();

      bool true_present = false;
      for(expression_vector::const_iterator it = op_names.begin(),
	    it_end = op_names.end(); it != it_end; ++it)
	{
	  if(is_true(*it))
	    true_present = true;
	}

      
      if(true_present)
	{
	  // SIMPLIFICATION: if one of si's is true, then only { s } unit
	  // clause should be added (we retract names and clauses introduced
	  // for si's)
	  retract_clauses(clauses_size, clauses);
	  retract_names(names_size);
	  long_clause->push_back(name);
	  clauses.push_back(long_clause);	  
	}
      else
	{
	  for(expression_vector::const_iterator it = op_names.begin(),
		it_end = op_names.end(); it != it_end; ++it)
	    {

	      // SIMPLIFICATION: if si is false, clause ~si \/ s can be omitted.
	      // Also, si is omitted from the long clause
	      if(is_false(*it))
		continue;

	      clause * short_clause = new clause();
	      
	      short_clause->push_back(name);
	      short_clause->push_back(negate_name(*it));
	      clauses.push_back(short_clause);
	      long_clause->push_back(*it);
	    }
	  long_clause->push_back(not_name);
	  clauses.push_back(long_clause);
	}
    }
  else if(expr->is_implication())
    {
      /* s1 => s2 => ... => sn
	 s1 => s2 => ... (~sn-1 \/ sn)
	 s1 => s2 => ... (~sn-2 \/ ~sn-1 \/ sn)
	 ...
	 ~s1 \/ ~s2 \/ ... \/ sn   <=> s
	 (s1 \/ s) /\ (s2 \/ s) /\ ... /\ (sn-1 \/ s) /\ (~sn \/ s) /\ 
	 (~s1 \/ ~s2 \/ ... \/ ~sn-1 \/ sn \/ ~s) */
	
      name = get_unique_constant(cnf_prefix, _sort_factory->BOOL_SORT());
      expression not_name = negate_name(name);
      clause * long_clause = new clause();

      bool true_present = false;
      for(expression_vector::const_iterator it = op_names.begin(),
	    it_end = op_names.end(); it != it_end; ++it)
	{
	  expression si = it == it_end - 1 ? *it : negate_name(*it);
	  
	  if(is_true(si))
	    true_present = true;
	}
      
      if(true_present)
	{
	  // SIMPLIFICATION: if one of (~)si's is true, then only { s } unit
	  // clause should be added (we retract names and clauses introduced
	  // for si's)
	  retract_clauses(clauses_size, clauses);
	  retract_names(names_size);
	  long_clause->push_back(name);
	  clauses.push_back(long_clause);	  
	}
      else
	{
      
	  for(expression_vector::const_iterator it = op_names.begin(),
		it_end = op_names.end(); it != it_end; ++it)
	    {
	      expression si = it == it_end - 1 ? *it : negate_name(*it);

	      // SIMPLIFICATION: if (~)si is false, clause (~)si \/ s can be omitted.
	      // Also, (~)si is omitted from the long clause
	      if(is_false(si))
		continue;
	      
	      clause * short_clause = new clause();	      
	      short_clause->push_back(name);
	      short_clause->push_back(negate_name(si));
	      long_clause->push_back(si);
	      clauses.push_back(short_clause);	   
	    }
	  long_clause->push_back(not_name);
	  clauses.push_back(long_clause);
	}
    }
  else if(expr->is_equivalence()) 
    {	
      name = get_unique_constant(cnf_prefix, _sort_factory->BOOL_SORT());
      expression not_name = negate_name(name);
      expression not_first = negate_name(op_names[0]);
      expression not_second = negate_name(op_names[1]);
      
      if(op_names[0] == op_names[1] ||
	 (is_true(op_names[0]) && is_true(op_names[1])) ||
	 (is_false(op_names[0]) && is_false(op_names[1])))
	{
	  // SIMPLIFICATION: s1 === s2, only unit clause { s } is added
	  retract_names(names_size);
	  retract_clauses(clauses_size, clauses);
	  clause * cl = new clause();
	  cl->push_back(name);
	  clauses.push_back(cl);
	}
      else if(op_names[0] == negate_name(op_names[1]) ||
	      (is_true(op_names[0]) && is_false(op_names[1])) ||
	      (is_false(op_names[0]) && is_true(op_names[1])))
	
	{
	  // SIMPLIFICATION: s1 === ~s2, only unit clause { ~s } is added
	  retract_names(names_size);
	  retract_clauses(clauses_size, clauses);
	  clause *cl = new clause();
	  cl->push_back(not_name);
	  clauses.push_back(cl);
	}
      else if(is_true(op_names[0]))
	{
	  // SIMPLIFICATION: s1 === true, then s <=> s2:  { ~s, s2 }, {~s2, s }
	  clause * cl = new clause();
	  cl->push_back(op_names[1]);
	  cl->push_back(not_name);
	  clauses.push_back(cl);
	  cl = new clause();
	  cl->push_back(not_second);
	  cl->push_back(name);
	  clauses.push_back(cl);
	}
      else if(is_false(op_names[0]))
	{
	  // SIMPLIFICATION: s1 == false, then s <=> ~s2: { s, s2 }, {~s, ~s2}
	  clause * cl = new clause();
	  cl->push_back(op_names[1]);
	  cl->push_back(name);
	  clauses.push_back(cl);
	  cl = new clause();
	  cl->push_back(not_second);
	  cl->push_back(not_name);
	  clauses.push_back(cl);	  
	}
      else if(is_true(op_names[1]))
	{
	  // SIMPLIFICATION: s2 === true, then s <=> s1:  { ~s, s1 }, {~s1, s }
	  clause * cl = new clause();
	  cl->push_back(op_names[0]);
	  cl->push_back(not_name);
	  clauses.push_back(cl);
	  cl = new clause();
	  cl->push_back(not_first);
	  cl->push_back(name);
	  clauses.push_back(cl);
	}
      else if(is_false(op_names[1]))
	{
	  // SIMPLIFICATION: s2 == false, then s <=> ~s1: { s, s1 }, {~s, ~s1}
	  clause * cl = new clause();
	  cl->push_back(op_names[0]);
	  cl->push_back(name);
	  clauses.push_back(cl);
	  cl = new clause();
	  cl->push_back(not_first);
	  cl->push_back(not_name);
	  clauses.push_back(cl);	  
	}
      else {
	// GENERAL CASE:
	/* (s1 <=> s2) <=> s
	 (~s1 \/ s2  \/ ~s) /\ (s1 \/ ~s2 \/ ~s) /\
	 (s1 \/ s2 \/ s) /\ (~s2 \/ ~s1 \/ s)  */

	clause * cl = new clause();	
	cl->push_back(not_first);
	cl->push_back(op_names[1]);
	cl->push_back(not_name);
	clauses.push_back(cl);
	
	cl = new clause();
	cl->push_back(op_names[0]);
	cl->push_back(not_second);
	cl->push_back(not_name);
	clauses.push_back(cl);

	cl = new clause();
	cl->push_back(op_names[0]);
	cl->push_back(op_names[1]);
	cl->push_back(name);
	clauses.push_back(cl);
	
	cl = new clause();
	cl->push_back(not_first);
	cl->push_back(not_second);
	cl->push_back(name);
	clauses.push_back(cl);
      }
    }
  else
    {
      name = _exp_factory->create_expression(expr->get_symbol(), std::move(op_names));
      register_name(expr, name);
      check_definition(expr, clauses);
      return;
    }
  register_name(expr, name);
}


void formula_transformer::top_level_cnf_transformation(const expression & expr, 
						       std::vector<clause *> & clauses)
{
  if(is_true(expr))
    return;

  if(is_false(expr))
    {
      clauses.push_back(new clause());
      return;
    }
  
  expression name;
  cnf_transformation(expr, clauses, name);
  clause * name_unit = new clause();
  name_unit->push_back(name);
  clauses.push_back(name_unit);
}

expression formula_transformer::expand_names(const expression & expr)
{
  auto it = _named_exprs.find(expr);
  if(it != _named_exprs.end())
    return it->second;
    
  if(expr->is_function() && !expr->get_operands().empty())
    {
      unsigned n = expr->get_operands().size();
      expression_vector exp_ops;
      exp_ops.reserve(n);
      for(unsigned i = 0; i < n; i++)
	exp_ops.push_back(expand_names(expr->get_operands()[i]));
      return _exp_factory->create_expression(expr->get_symbol(), std::move(exp_ops));
    }
  else
    return expr;
}
