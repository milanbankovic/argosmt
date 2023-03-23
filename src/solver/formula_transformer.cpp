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

expression formula_transformer::simplification(const expression & expr)
{
  /* In case of special constants, constant functions and variables */
  if(expr->is_special_constant() || expr->is_variable() || expr->is_constant_function())
    return expr;

  /* Quantifiers */
  if(expr->is_quantifier())
    {
      expression simp_op = simplification(expr->get_subexpression());
      const sorted_variable_set & fvars = simp_op->get_free_variables();

      if(fvars.empty())
	return simp_op;
      
      sorted_variable_vector vars;
      for(auto it = expr->get_quantified_variables().begin(),
	    it_end = expr->get_quantified_variables().end(); it != it_end; ++it)
	{
	  if(fvars.find(*it) != fvars.end())
	    vars.push_back(*it);
	}
      if(vars.empty())
	return simp_op;
      else
	return _exp_factory->create_expression(expr->get_quantifier(), std::move(vars), simp_op);					     
    }

  /* Otherwise, it is a (non-constant) function */
  
  /* Binarization of chainables */
  if(is_chainable(expr) && expr->get_operands().size() > 2)
    {
      const function_symbol & fs = expr->get_symbol();
      expression_vector ops;
      for(expression_vector::const_iterator it = expr->get_operands().begin(),
	    it_end = expr->get_operands().end() - 1; it != it_end; ++it)
	{
	  ops.push_back(_exp_factory->create_expression(fs, *it, *(it + 1)));
	}
      return  simplification(_exp_factory->create_expression(function_symbol::AND, std::move(ops)));	
    }

  /* Binarization of pairwise */
  if(is_pairwise(expr) && expr->get_operands().size() > 2)
    {
      const function_symbol & fs = expr->get_symbol();
      expression_vector ops;
      for(expression_vector::const_iterator it = expr->get_operands().begin(),
	    it_end = expr->get_operands().end() - 1; it != it_end; ++it)
	for(expression_vector::const_iterator jt = it + 1, 
	      jt_end = expr->get_operands().end(); jt != jt_end; ++jt)
	  {
	    ops.push_back(_exp_factory->create_expression(fs, *it, *jt));
	  }
      return simplification(_exp_factory->create_expression(function_symbol::AND, std::move(ops)));
    }

  const expression_vector & ops = expr->get_operands();
  expression_vector simp_ops;
  simp_ops.reserve(ops.size());
  for(expression_vector::const_iterator it = ops.begin(), it_end = ops.end();
      it != it_end; ++it)
    {
      simp_ops.push_back(simplification(*it));
    }

  if(expr->is_not())
    {
      if(simp_ops[0]->is_true())
	return _exp_factory->FALSE_EXPRESSION();
      else if(simp_ops[0]->is_false())
	return _exp_factory->TRUE_EXPRESSION();
      else 
	return _exp_factory->create_expression(function_symbol::NOT, std::move(simp_ops));
    }
  if(expr->is_and())
    {
      /* Removing trues and falses from and */
      expression_vector::iterator wt = simp_ops.begin();
      for(expression_vector::const_iterator it = simp_ops.begin(),
	    it_end = simp_ops.end(); it != it_end; ++it)
	{
	  if((*it)->is_true())
	    continue;
	    
	  *wt++ = *it;
	    
	  if((*it)->is_false())
	    break;
	}
      simp_ops.erase(wt, simp_ops.end());
      if(simp_ops.empty())
	return _exp_factory->TRUE_EXPRESSION();
      else if(simp_ops.back()->is_false())
	return _exp_factory->FALSE_EXPRESSION();
      else if(simp_ops.size() == 1)
	return simp_ops[0];
      else
	return _exp_factory->create_expression(function_symbol::AND, std::move(simp_ops));
    }
  else if(expr->is_or())
    {
      /* Removing trues and falses from or */
      expression_vector::iterator wt = simp_ops.begin();
      for(expression_vector::const_iterator it = simp_ops.begin(),
	    it_end = simp_ops.end(); it != it_end; ++it)
	{
	  if((*it)->is_false())
	    continue;
	  
	  *wt++ = *it;
	  
	  if((*it)->is_true())
	    break;
	}
      simp_ops.erase(wt, simp_ops.end());
      if(simp_ops.empty())
	return _exp_factory->FALSE_EXPRESSION();
      else if(simp_ops.back()->is_true())
	return _exp_factory->TRUE_EXPRESSION();
      else if(simp_ops.size() == 1)
	return simp_ops[0];
      else
	return _exp_factory->create_expression(function_symbol::OR, std::move(simp_ops));
    }
  else if(expr->is_implication())
    {
      /* Removing trues and falses from imp */
      expression right_side = simp_ops.back();
      simp_ops.pop_back();
      expression_vector::iterator wt = simp_ops.begin();
      for(expression_vector::const_iterator it = simp_ops.begin(),
	    it_end = simp_ops.end(); it != it_end; ++it)
	{
	  if((*it)->is_true())
	    continue;
	    
	  *wt++ = *it;
	    
	  if((*it)->is_false())
	    break;
	}
      simp_ops.erase(wt, simp_ops.end());
	
      if(simp_ops.empty())
	return right_side; 
      else if(simp_ops.back()->is_false())
	return _exp_factory->TRUE_EXPRESSION();
      else if(right_side->is_true())
	return _exp_factory->TRUE_EXPRESSION();
      else if(right_side->is_false())
	{
	  expression left_side = simp_ops.size() > 1 ? 			      
	    _exp_factory->create_expression(function_symbol::AND, std::move(simp_ops)) : simp_ops[0];
	  return _exp_factory->create_expression(function_symbol::NOT, left_side);
	}
      else
	{
	  simp_ops.push_back(right_side);
	  return _exp_factory->create_expression(function_symbol::IMPLIES, std::move(simp_ops));
	}
    }
  else if(expr->is_equivalence())
    {
      if(simp_ops[0]->is_true())
	{
	  if(simp_ops[1]->is_true())
	    return _exp_factory->TRUE_EXPRESSION();
	  else if(simp_ops[1]->is_false())
	    return _exp_factory->FALSE_EXPRESSION();
	  else
	    return simp_ops[1];
	}
      else if(simp_ops[0]->is_false())
	{
	  if(simp_ops[1]->is_true())
	    return _exp_factory->FALSE_EXPRESSION();
	  else if(simp_ops[1]->is_false())
	    return _exp_factory->TRUE_EXPRESSION();
	  else 
	    return _exp_factory->create_expression(function_symbol::NOT, simp_ops[1]);
	}
      else if(simp_ops[1]->is_true())
	return simp_ops[0];
      else if(simp_ops[1]->is_false())
	return _exp_factory->create_expression(function_symbol::NOT, simp_ops[0]);
      else
	return _exp_factory->create_expression(function_symbol::EQ, std::move(simp_ops));
    }
  else if(expr->is_inequivalence())
    {
      if(simp_ops[0]->is_true())
	{
	  if(simp_ops[1]->is_true())
	    return _exp_factory->FALSE_EXPRESSION();
	  else if(simp_ops[1]->is_false())
	    return _exp_factory->TRUE_EXPRESSION();
	  else
	    return _exp_factory->create_expression(function_symbol::NOT, simp_ops[1]);
	}
      else if(simp_ops[0]->is_false())
	{
	  if(simp_ops[1]->is_true())
	    return _exp_factory->TRUE_EXPRESSION();
	  else if(simp_ops[1]->is_false())
	    return _exp_factory->FALSE_EXPRESSION();
	  else
	    return simp_ops[1];
	}
      else if(simp_ops[1]->is_true())
	return _exp_factory->create_expression(function_symbol::NOT, simp_ops[0]);
      else if(simp_ops[1]->is_false())
	return simp_ops[0];
      else 
	return  _exp_factory->create_expression(function_symbol::DISTINCT, std::move(simp_ops));
    }
  else if(expr->is_xor())
    {
      // s1 ^ s2 ^ ... ^ sn ^ T   <=> ~(s1 ^ ... ^ sn) <=> s1 ^ ... ^ ~sn
      // s1 ^ s2 ^ ... ^ sn ^ F   <=> s1 ^ ... ^ sn
      // even number of T's do not change anything, even number of T's 
      // flips the last literal. F's are ignored.

      unsigned count_trues = 0;
      expression_vector::iterator wt = simp_ops.begin();
      for(expression_vector::const_iterator it = simp_ops.begin(),
	    it_end = simp_ops.end(); it != it_end; ++it)
	{
	  if((*it)->is_true())
	    count_trues++;
	  else if(!(*it)->is_false())
	    *wt++ = *it;
	}
      simp_ops.erase(wt, simp_ops.end());

      if(count_trues % 2 == 0)
	{
	  if(simp_ops.empty())
	    return _exp_factory->FALSE_EXPRESSION();
	  else if(simp_ops.size() == 1)
	    return simp_ops[0];
	  else
	    return _exp_factory->create_expression(function_symbol::XOR, std::move(simp_ops));
	}
      else
	{
	  if(simp_ops.empty())
	    return _exp_factory->TRUE_EXPRESSION();
	  else if(simp_ops.size() == 1)
	    return _exp_factory->create_expression(function_symbol::NOT, simp_ops[0]);
	  else 
	    {
	      simp_ops.back() = _exp_factory->create_expression(function_symbol::NOT, simp_ops.back());
	      return _exp_factory->create_expression(function_symbol::XOR, std::move(simp_ops));
	    }
	}

    }
  else if(expr->is_ite())
    {
      // the first two works both form terms and formulae
      // if T then A else B  = A
      // if F then A else B  = B
      // just formulae
      // if C then T else T  = T
      // if C then T else F  = C
      // if C then T else B  = C /\ T \/ ~C /\ B = C \/ (~C /\ B) = C \/ B
      // if C then F else F  = F
      // if C then F else T  = ~C
      // if C then F else B  = C /\ F \/ ~C /\ B = F \/ (~C /\ B) = ~C /\ B
      // if C then A else T  = C /\ A \/ ~C /\ T = (C /\ A) \/ ~C = ~C \/ A
      // if C then A else F  = C /\ A \/ ~C /\ F = (C /\ A) \/ F = C /\ A
      if(simp_ops[0]->is_true())
	return simp_ops[1];
      else if(simp_ops[0]->is_false())
	return simp_ops[2];
      else if(simp_ops[1]->is_true())
	{
	  if(simp_ops[2]->is_true())
	    return _exp_factory->TRUE_EXPRESSION();
	  else if(simp_ops[2]->is_false())
	    return simp_ops[0];
	  else
	    return _exp_factory->create_expression(function_symbol::OR, simp_ops[0], simp_ops[2]);
	}
      else if(simp_ops[1]->is_false())
	{
	  if(simp_ops[2]->is_true())
	    return _exp_factory->create_expression(function_symbol::NOT, simp_ops[0]);
	  else if(simp_ops[2]->is_false())
	    return _exp_factory->FALSE_EXPRESSION();
	  else 
	    {
	      expression not_c = _exp_factory->create_expression(function_symbol::NOT, simp_ops[0]);
	      return _exp_factory->create_expression(function_symbol::AND, not_c, simp_ops[2]);
	    }									 
	}
      else if(simp_ops[2]->is_true())
	{
	  expression not_c = _exp_factory->create_expression(function_symbol::NOT, simp_ops[0]);
	  return _exp_factory->create_expression(function_symbol::OR, not_c, simp_ops[1]);
	}
      else if(simp_ops[2]->is_false())
	return _exp_factory->create_expression(function_symbol::AND, simp_ops[0], simp_ops[1]);
      else
	return _exp_factory->create_expression(function_symbol::ITE, simp_ops[0], simp_ops[1], simp_ops[2]);
    }
  else 
    return _exp_factory->create_expression(expr->get_symbol(), std::move(simp_ops));
}

void formula_transformer::cnf_transformation(const expression & expr, 
					     std::vector<clause *> & clauses, 
					     expression & name)
{
  static const std::string cnf_prefix = std::string("uniq");
  static const std::string ite_prefix = std::string("ite");
  
  // This covers special constants, variables, constant functions and quantifiers
  if(!expr->is_function() || expr->get_operands().empty())
    {
      name = expr;
      if(!expr->is_variable())
	set_sort_constant(expr);
      return;
    }

  auto it = _names.find(expr);
  if(it != _names.end())
    {
      name = it->second;
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
	 (~s1 \/ ~s2 \/ ... \/ ~sn \/ s) /\ (~s \/ (s1 /\ s2 /\ ... /\ sn))
	 (~s1 \/ ~s2 \/ ... \/ ~sn \/ s) /\ (~s \/ s1) /\ (~s \/ s2) /\ ...
	 /\ (~s \/ sn) */

      name = get_unique_constant(cnf_prefix, _sort_factory->BOOL_SORT());
      expression not_name = negate_name(name);
      clause * long_clause = new clause();
	
      for(expression_vector::const_iterator it = op_names.begin(),
	    it_end = op_names.end(); it != it_end; ++it)
	{
	  clause * short_clause = new clause();
	  short_clause->push_back(not_name);
	  short_clause->push_back(*it);
	  clauses.push_back(short_clause);
	  long_clause->push_back(negate_name(*it));
	}
      long_clause->push_back(name);
      clauses.push_back(long_clause);	
    }
  else if(expr->is_or())
    {
      /* s1 \/ s2 \/ ... \/ sn <=> s
	 (~s1 /\ ~s2 /\ ... /\ ~sn) \/ s) /\ (s1 \/ s2 \/ ... \/ sn \/ ~s)
	 (~s1 \/ s) /\ (~s2 \/ s) /\ ... /\ (~sn \/ s) /\ 
	 (s1 \/ s2 \/ ... \/ sn \/ ~s) */
	
      name = get_unique_constant(cnf_prefix, _sort_factory->BOOL_SORT());
      expression not_name = negate_name(name);
      clause * long_clause = new clause();
	
      for(expression_vector::const_iterator it = op_names.begin(),
	    it_end = op_names.end(); it != it_end; ++it)
	{
	  clause * short_clause = new clause();

	  short_clause->push_back(name);
	  short_clause->push_back(negate_name(*it));
	  clauses.push_back(short_clause);
	  long_clause->push_back(*it);
	}
      long_clause->push_back(not_name);
      clauses.push_back(long_clause);	
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
	
      for(expression_vector::const_iterator it = op_names.begin(),
	    it_end = op_names.end(); it != it_end; ++it)
	{
	  clause * short_clause = new clause();
	    
	  short_clause->push_back(name);
	  if(it != it_end - 1)
	    {
	      short_clause->push_back(*it);
	      long_clause->push_back(negate_name(*it));
	    }
	  else
	    {
	      short_clause->push_back(negate_name(*it));
	      long_clause->push_back(*it);
	    }
	  clauses.push_back(short_clause);	   
	}
      long_clause->push_back(not_name);
      clauses.push_back(long_clause);	
    }
  else if(expr->is_equivalence()) 
    {
      /* (s1 <=> s2) <=> s
	 (~s1 \/ s2) /\ (s1 \/ ~s2) <=> s
	 ((~s1 \/ s2) /\ (s1 \/ ~s2))  \/ ~s    /\
	 (s1 /\ ~s2) \/ (~s1 /\ s2) \/ s
	   
	 (~s1 \/ s2  \/ ~s) /\ (s1 \/ ~s2 \/ ~s) /\
	 (s1 \/ s2 \/ s) /\ (~s2 \/ ~s1 \/ s)  */
	
      name = get_unique_constant(cnf_prefix, _sort_factory->BOOL_SORT());
      expression not_name = negate_name(name);

      expression not_first = negate_name(op_names[0]);
      expression not_second = negate_name(op_names[1]);

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
  else if(expr->is_inequivalence())
    {
      /* (s1 != s2) <=> s
	 (s1 <=> s2) <=> ~s
	 (~s1 \/ s2) /\ (s1 \/ ~s2) <=> ~s
	 ((~s1 \/ s2) /\ (s1 \/ ~s2))  \/ s    /\
	 (s1 /\ ~s2) \/ (~s1 /\ s2) \/ ~s
	   
	 (~s1 \/ s2  \/ s) /\ (s1 \/ ~s2 \/ s) /\
	 (s1 \/ s2 \/ ~s) /\ (~s2 \/ ~s1 \/ ~s)  */
      name = get_unique_constant(cnf_prefix, _sort_factory->BOOL_SORT());
      expression not_name = negate_name(name);

      expression not_first = negate_name(op_names[0]);
      expression not_second = negate_name(op_names[1]);

      clause * cl = new clause();	
      cl->push_back(not_first);
      cl->push_back(op_names[1]);
      cl->push_back(name);
      clauses.push_back(cl);
	
      cl = new clause();
      cl->push_back(op_names[0]);
      cl->push_back(not_second);
      cl->push_back(name);
      clauses.push_back(cl);

      cl = new clause();
      cl->push_back(op_names[0]);
      cl->push_back(op_names[1]);
      cl->push_back(not_name);
      clauses.push_back(cl);
	
      cl = new clause();
      cl->push_back(not_first);
      cl->push_back(not_second);
      cl->push_back(not_name);
      clauses.push_back(cl);
    }
  else if(expr->is_xor())
    {
      // The same as for inequivalence, but with binarization.
      expression_vector::const_iterator it = op_names.begin(), it_end = op_names.end();
      name = *it;
      it++;
      for(;it != it_end; ++it)
	{
	  expression first = name;
	  expression second = *it;
	    
	  name = get_unique_constant(cnf_prefix, _sort_factory->BOOL_SORT());
	  expression not_name = negate_name(name);

	    
	  expression not_first = negate_name(first);
	  expression not_second = negate_name(second);

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
    }
  else if(expr->is_ite())
    {
      if(expr->is_formula())
	{
	  /* Bool case:
	  // if C then A else B  <=> (C /\ A) \/ (~C /\ B) <=> s
	  // ((~C \/ ~A) /\ (C \/ ~B)) \/ s    /\
	  // ~s \/ (C /\ A) \/ (~C /\ B)   <=>
	  // (~C \/ ~A \/ s) /\ (C \/ ~B \/ s)   /\
	  // (~s \/ C \/ B) /\ (~s \/ A \/ ~C) /\ (~s \/ A \/ B) <=>
	  // (~C \/ ~A \/ s) /\ (C \/ ~B \/ s)  /\
	  // (~s \/ C \/ B) /\ (~s \/ A \/ ~C)
	  // (the last clause ~s \/ A \/ B is the resolvent of previous two)
	  */
	  name = get_unique_constant(cnf_prefix, _sort_factory->BOOL_SORT());
	  expression not_name = negate_name(name);
	  const expression & c = op_names[0];
	  const expression & a = op_names[1];
	  const expression & b = op_names[2];
	  expression not_c = negate_name(c);
	  expression not_a = negate_name(a);
	  expression not_b = negate_name(b);

	  clause * cl = new clause();
	  cl->push_back(not_c);
	  cl->push_back(not_a);
	  cl->push_back(name);
	  clauses.push_back(cl);
	    
	  cl = new clause();
	  cl->push_back(c);
	  cl->push_back(not_b);
	  cl->push_back(name);
	  clauses.push_back(cl);

	  cl = new clause();
	  cl->push_back(not_name);
	  cl->push_back(c);
	  cl->push_back(b);
	  clauses.push_back(cl);

	  cl = new clause();
	  cl->push_back(not_name);
	  cl->push_back(a);
	  cl->push_back(not_c);
	  clauses.push_back(cl);

	  // this clause is the resolvent of the previous two
	  //cl = new clause();
	  //cl->push_back(not_name);
	  //cl->push_back(a);
	  //cl->push_back(b);
	  //clauses.push_back(cl);
	}
      else {
	// Non-bool case:
	// if C then A else B = s,  (C /\ s = A)  \/ (~C /\ s = B)
	// C /\ sA \/ ~C /\ sB  <=> 
	// C \/ sB /\ sA \/ ~C  /\  sA \/ sB <=>
	// C \/ sB /\ sA \/ ~C  (sA \/ sB is the resolvent)
	name = get_unique_constant(ite_prefix, expr->get_inferred_sort());
	expression sA = _exp_factory->create_expression(function_symbol::EQ, name, op_names[1]);
	expression sB = _exp_factory->create_expression(function_symbol::EQ, name, op_names[2]);
	const expression & c = op_names[0];
	expression not_c = negate_name(c);

	clause * cl = new clause();
	cl->push_back(c);
	cl->push_back(sB);
	clauses.push_back(cl);

	cl = new clause();
	cl->push_back(not_c);
	cl->push_back(sA);
	clauses.push_back(cl);

	// sA \/ sB is the resolvent of previous two clauses
	//cl = new clause();
	//cl->push_back(sA);
	//cl->push_back(sB);
	//clauses.push_back(cl);
      }
    }
  else
    {
      name = _exp_factory->create_expression(expr->get_symbol(), std::move(op_names));    
    }

  _names.insert(std::make_pair(expr, name));
  _named_exprs.insert(std::make_pair(name, expr));
  //  std::cout << "NAME: " << name << " FOR: " << expr << std::endl;
}

void formula_transformer::top_level_cnf_transformation(const expression & expr, 
						       std::vector<clause *> & clauses)
{
  if(expr->is_true())
    return;
  else if(expr->is_false())
    {
      clauses.clear();
      clauses.push_back(new clause());
      return;
    }
  else if(expr->is_and())
    {
      for(expression_vector::const_iterator it = expr->get_operands().begin(),
	    it_end = expr->get_operands().end(); it != it_end; ++it)
	{
	  top_level_cnf_transformation(*it, clauses);
	  if(clauses.back()->size() == 0)
	    break;
	}
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
