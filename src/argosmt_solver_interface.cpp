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
#include "argosmt_solver_interface.hpp"
#include "solver_configuration.hpp"
#ifdef _PARALLEL_PORTFOLIO
#include <tbb/tbb.h>
#include <tbb/task.h>
#endif

expression argosmt_solver_interface::simplification(const expression & expr)
{
  /* In case of special constants, constant functions and variables 
     (assume that there is no quantifiers) */
  if(!expr->is_function() || expr->get_operands().empty())
    return expr;
    
  /* Binarization of chainables */
  if(is_chainable(expr) && expr->get_operands().size() > 2)
    {
      const function_symbol & fs = expr->get_symbol();
      expression_vector ops;
      for(expression_vector::const_iterator it = expr->get_operands().begin(),
	    it_end = expr->get_operands().end() - 1; it != it_end; ++it)
	{
	  ops.push_back(_smt_lib_api->
			get_expression_factory()->
			create_expression(fs, *it, *(it + 1)));
	}
      return  simplification(_smt_lib_api->get_expression_factory()->create_expression(function_symbol::AND, std::move(ops)));	
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
	    ops.push_back(_smt_lib_api->
			  get_expression_factory()->
			  create_expression(fs, *it, *jt));
	  }
      return simplification(_smt_lib_api->get_expression_factory()->create_expression(function_symbol::AND, std::move(ops)));
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
	return _smt_lib_api->get_expression_factory()->FALSE_EXPRESSION();
      else if(simp_ops[0]->is_false())
	return _smt_lib_api->get_expression_factory()->TRUE_EXPRESSION();
      else 
	return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::NOT, std::move(simp_ops));
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
	return _smt_lib_api->get_expression_factory()->TRUE_EXPRESSION();
      else if(simp_ops.back()->is_false())
	return _smt_lib_api->get_expression_factory()->FALSE_EXPRESSION();
      else if(simp_ops.size() == 1)
	return simp_ops[0];
      else
	return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::AND, std::move(simp_ops));
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
	return _smt_lib_api->get_expression_factory()->FALSE_EXPRESSION();
      else if(simp_ops.back()->is_true())
	return _smt_lib_api->get_expression_factory()->TRUE_EXPRESSION();
      else if(simp_ops.size() == 1)
	return simp_ops[0];
      else
	return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::OR, std::move(simp_ops));
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
	return _smt_lib_api->get_expression_factory()->TRUE_EXPRESSION();
      else if(right_side->is_true())
	return _smt_lib_api->get_expression_factory()->TRUE_EXPRESSION();
      else if(right_side->is_false())
	{
	  expression left_side = simp_ops.size() > 1 ? 			      
	    _smt_lib_api->get_expression_factory()->create_expression(function_symbol::AND, std::move(simp_ops)) : simp_ops[0];
	  return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::NOT, left_side);
	}
      else
	{
	  simp_ops.push_back(right_side);
	  return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::IMPLIES, std::move(simp_ops));
	}
    }
  else if(expr->is_equivalence())
    {
      if(simp_ops[0]->is_true())
	{
	  if(simp_ops[1]->is_true())
	    return _smt_lib_api->get_expression_factory()->TRUE_EXPRESSION();
	  else if(simp_ops[1]->is_false())
	    return _smt_lib_api->get_expression_factory()->FALSE_EXPRESSION();
	  else
	    return simp_ops[1];
	}
      else if(simp_ops[0]->is_false())
	{
	  if(simp_ops[1]->is_true())
	    return _smt_lib_api->get_expression_factory()->FALSE_EXPRESSION();
	  else if(simp_ops[1]->is_false())
	    return _smt_lib_api->get_expression_factory()->TRUE_EXPRESSION();
	  else 
	    return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::NOT, simp_ops[1]);
	}
      else if(simp_ops[1]->is_true())
	return simp_ops[0];
      else if(simp_ops[1]->is_false())
	return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::NOT, simp_ops[0]);
      else
	return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::EQ, std::move(simp_ops));
    }
  else if(expr->is_inequivalence())
    {
      if(simp_ops[0]->is_true())
	{
	  if(simp_ops[1]->is_true())
	    return _smt_lib_api->get_expression_factory()->FALSE_EXPRESSION();
	  else if(simp_ops[1]->is_false())
	    return _smt_lib_api->get_expression_factory()->TRUE_EXPRESSION();
	  else
	    return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::NOT, simp_ops[1]);
	}
      else if(simp_ops[0]->is_false())
	{
	  if(simp_ops[1]->is_true())
	    return _smt_lib_api->get_expression_factory()->TRUE_EXPRESSION();
	  else if(simp_ops[1]->is_false())
	    return _smt_lib_api->get_expression_factory()->FALSE_EXPRESSION();
	  else
	    return simp_ops[1];
	}
      else if(simp_ops[1]->is_true())
	return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::NOT, simp_ops[0]);
      else if(simp_ops[1]->is_false())
	return simp_ops[0];
      else 
	return  _smt_lib_api->get_expression_factory()->create_expression(function_symbol::DISTINCT, std::move(simp_ops));
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
	    return _smt_lib_api->get_expression_factory()->FALSE_EXPRESSION();
	  else if(simp_ops.size() == 1)
	    return simp_ops[0];
	  else
	    return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::XOR, std::move(simp_ops));
	}
      else
	{
	  if(simp_ops.empty())
	    return _smt_lib_api->get_expression_factory()->TRUE_EXPRESSION();
	  else if(simp_ops.size() == 1)
	    return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::NOT, simp_ops[0]);
	  else 
	    {
	      simp_ops.back() = _smt_lib_api->get_expression_factory()->create_expression(function_symbol::NOT, simp_ops.back());
	      return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::XOR, std::move(simp_ops));
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
	    return _smt_lib_api->get_expression_factory()->TRUE_EXPRESSION();
	  else if(simp_ops[2]->is_false())
	    return simp_ops[0];
	  else
	    return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::OR, simp_ops[0], simp_ops[2]);
	}
      else if(simp_ops[1]->is_false())
	{
	  if(simp_ops[2]->is_true())
	    return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::NOT, simp_ops[0]);
	  else if(simp_ops[2]->is_false())
	    return _smt_lib_api->get_expression_factory()->FALSE_EXPRESSION();
	  else 
	    {
	      expression not_c = _smt_lib_api->get_expression_factory()->create_expression(function_symbol::NOT, simp_ops[0]);
	      return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::AND, not_c, simp_ops[2]);
	    }									 
	}
      else if(simp_ops[2]->is_true())
	{
	  expression not_c = _smt_lib_api->get_expression_factory()->create_expression(function_symbol::NOT, simp_ops[0]);
	  return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::OR, not_c, simp_ops[1]);
	}
      else if(simp_ops[2]->is_false())
	return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::AND, simp_ops[0], simp_ops[1]);
      else
	return _smt_lib_api->get_expression_factory()->create_expression(function_symbol::ITE, simp_ops[0], simp_ops[1], simp_ops[2]);
    }
  else 
    return _smt_lib_api->get_expression_factory()->create_expression(expr->get_symbol(), std::move(simp_ops));
}

void argosmt_solver_interface::cnf_transformation_rec(const expression & expr, 
						      std::vector<clause *> & clauses, 
						      expression & name)
{
  static const std::string cnf_prefix = std::string("uniq");
  static const std::string ite_prefix = std::string("ite");

  // This covers special constants, variables and constant functions
  if(!expr->is_function() || expr->get_operands().empty())
    {
      name = expr;
      return;
    }
    
  expression_vector op_names;
  for(expression_vector::const_iterator it = expr->get_operands().begin(),
	it_end = expr->get_operands().end(); it != it_end; ++it)
    {
      expression it_name;
      cnf_transformation_rec(*it, clauses, it_name);
      op_names.push_back(it_name);
    }

  if(expr->is_not())
    name = negate_name(op_names[0]);
  else if(expr->is_and())
    {
      /* s1 /\ s2 /\ ... /\ sn <=> s
	 (~s1 \/ ~s2 \/ ... \/ ~sn \/ s) /\ (~s \/ (s1 /\ s2 /\ ... /\ sn))
	 (~s1 \/ ~s2 \/ ... \/ ~sn \/ s) /\ (~s \/ s1) /\ (~s \/ s2) /\ ...
	 /\ (~s \/ sn) */

      name = get_unique_constant(cnf_prefix, _smt_lib_api->get_sort_factory()->BOOL_SORT());    
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
	
      name = get_unique_constant(cnf_prefix, _smt_lib_api->get_sort_factory()->BOOL_SORT());    
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
	
      name = get_unique_constant(cnf_prefix, _smt_lib_api->get_sort_factory()->BOOL_SORT());    
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
	
      name = get_unique_constant(cnf_prefix, _smt_lib_api->get_sort_factory()->BOOL_SORT());    
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
      name = get_unique_constant(cnf_prefix, _smt_lib_api->get_sort_factory()->BOOL_SORT());    
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
	    
	  name = get_unique_constant(cnf_prefix, _smt_lib_api->get_sort_factory()->BOOL_SORT());    
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
	  name = get_unique_constant(cnf_prefix, _smt_lib_api->get_sort_factory()->BOOL_SORT());    
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
	name = get_unique_constant(ite_prefix, expr->infer_sorts()->get_sort());
	expression sA = _smt_lib_api->get_expression_factory()->create_expression(function_symbol::EQ, name, op_names[1]);
	expression sB = _smt_lib_api->get_expression_factory()->create_expression(function_symbol::EQ, name, op_names[2]);
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
    name = _smt_lib_api->get_expression_factory()->create_expression(expr->get_symbol(), std::move(op_names));
}

void argosmt_solver_interface::cnf_transformation(const expression & expr, 
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
	  cnf_transformation(*it, clauses);
	  if(clauses.back()->size() == 0)
	    break;
	}
      return;
    }
    
  expression name;
  cnf_transformation_rec(expr, clauses, name);
  clause * name_unit = new clause();
  name_unit->push_back(name);
  clauses.push_back(name_unit);
}

check_sat_response argosmt_solver_interface::start_solver(const std::vector<clause *> & clauses, unsigned k)
{
  extern solver_configuration s_config;
#ifdef _PARALLEL_PORTFOLIO
  if(_solvers[k])
    {
      expression_factory * old_factory = _solvers[k]->get_factory();
      delete _solvers[k];
      delete old_factory;
    }
#else
  delete _solvers[k];
#endif
  solver * local_solver;

  // Creating new solver instance and adding clauses to the solver
#ifndef _PARALLEL_PORTFOLIO
  expression_factory * factory = _smt_lib_api->get_expression_factory();
  _solvers[k] = local_solver = new solver(factory, false);
#else
  expression_factory * factory = new expression_factory(_smt_lib_api->get_expression_factory()->get_signature());
  _solvers[k] = local_solver = new solver(factory, _solvers, k, s_config.share_size_limit(k), false);
#endif 

  for(unsigned i = 0; i < clauses.size(); i++)
    {
#ifndef _PARALLEL_PORTFOLIO
      clause * cl = clauses[i];
#else
      clause * cl = new clause();
      cl->reserve(clauses[i]->size());
      for(unsigned j = 0; j < clauses[i]->size(); j++)
	cl->push_back((*clauses[i])[j]->clone_expression(factory));
#endif
      //std::cout << "Clause: " << *cl << std::endl;
      local_solver->add_clause(cl);
    }

  local_solver->add_observer(_statistics_observers[k] = new statistics_solver_observer(*local_solver, std::cout, 10000));
  //local_solver->add_observer(new logging_solver_observer(*local_solver, std::cout));
  //local_solver->add_observer(new logging_solver_observer(*local_solver, std::cerr));

  //local_solver->set_literal_selection_strategy(new interactive_literal_selection_strategy(*local_solver));
  //local_solver->set_polarity_selection_strategy(new interactive_polarity_selection_strategy(*local_solver));
  //local_solver->set_polarity_selection_strategy(new always_positive_polarity_selection_strategy(*local_solver));
  //local_solver->set_polarity_selection_strategy(new always_negative_polarity_selection_strategy(*local_solver));
  //local_solver->set_restart_strategy(new never_restart_strategy());
  //local_solver->set_restart_strategy(new conflict_counting_restart_strategy(*local_solver,100, 1.5));

  std::mt19937 decide_rng(s_config.random_seed(k));
  std::mt19937 polarity_rng(s_config.random_polarity_seed(k));
  
  if(s_config.randomize_decide(k))
    {     
      double r = s_config.random_decides_percent(k);
      unsigned d = s_config.random_decides_after_restart(k);
      local_solver->set_literal_selection_strategy(new random_decorater_literal_selection_strategy<>
						   (*local_solver, 
						    new vsids_literal_selection_strategy(*local_solver), decide_rng, r, d));
    }
  else
    {
      local_solver->set_literal_selection_strategy(new vsids_literal_selection_strategy(*local_solver));
    }

  const std::string pss = s_config.polarity_selection_strategy(k);
  if(pss == "saved")
    local_solver->set_polarity_selection_strategy(new saved_polarity_selection_strategy(*local_solver));
  else if(pss == "positive")
    local_solver->set_polarity_selection_strategy(new always_positive_polarity_selection_strategy(*local_solver));
  else if(pss == "negative")
    local_solver->set_polarity_selection_strategy(new always_negative_polarity_selection_strategy(*local_solver));
  else if(pss == "random")
    local_solver->set_polarity_selection_strategy(new random_polarity_selection_strategy<>(*local_solver, polarity_rng,
											   s_config.random_polarity_probability(k)));
  else
    {
      std::cerr << "CONFIGURATION ERROR: Unknown polarity selection strategy: " << pss << std::endl;
      exit(1);
    }
  
  bool randomize_simplex = s_config.randomize_simplex(k);
  unsigned simplex_shuffle_seed = s_config.simplex_shuffle_seed(k);
  const std::string buss = s_config.basic_unknown_selection_strategy(k);
  const std::string nbuss = s_config.non_basic_unknown_selection_strategy(k);
  const std::string as_alg_str = s_config.alldiff_sum_algorithm(k);
  alldiff_sum_algorithm as_algorithm = AS_NONE;
  if(as_alg_str == "partition")
    as_algorithm = AS_PARTITION;
  else if(as_alg_str == "beldiceanu")
    as_algorithm = AS_BELDICEANU;
  else if(as_alg_str == "fast")
    as_algorithm = AS_FAST;
  else if(as_alg_str == "none")
    as_algorithm = AS_NONE;
  else
    {
      std::cerr << "CONFIGURATION ERROR: Unknown alldiff-sum algorithm: " << as_alg_str << std::endl;
      exit(1);
    }


  bool predefined_csp_bounds = s_config.predefined_csp_bounds(k);
  int lower_predefined_csp_bound = s_config.lower_predefined_csp_bound(k);
  int upper_predefined_csp_bound = s_config.upper_predefined_csp_bound(k);
  
  /* Adding theories */
  symbol s = _smt_lib_api->get_logic_symbol();
  if(s == "QF_UF")
    {
      local_solver->set_forget_strategy(new geometric_forget_strategy(*local_solver));
      local_solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*local_solver));
      local_solver->set_restart_strategy(new conflict_counting_restart_strategy(*local_solver,100, 1.5));

      local_solver->add_theory(new bool_theory(*local_solver));
      local_solver->add_theory(new euf_theory(*local_solver));
    }
  else if(s == "QF_LRA")
    {
      local_solver->set_forget_strategy(new geometric_forget_strategy(*local_solver));
      local_solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*local_solver));
      local_solver->set_restart_strategy(new conflict_counting_restart_strategy(*local_solver,100, 1.5));

      local_solver->add_theory(new bool_theory(*local_solver));
      local_solver->add_theory(new arithmetic_theory(*local_solver, buss, nbuss, 13, false, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_UFLRA")
    {
      local_solver->set_forget_strategy(new geometric_forget_strategy(*local_solver));
      local_solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*local_solver));
      local_solver->set_restart_strategy(new conflict_counting_restart_strategy(*local_solver,100, 1.5));

      local_solver->add_theory(new bool_theory(*local_solver));
      local_solver->add_theory(new euf_theory(*local_solver));
      local_solver->add_theory(new arithmetic_theory(*local_solver, buss, nbuss, 13, true, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_LIA")
    {
      local_solver->set_forget_strategy(new geometric_forget_strategy(*local_solver));
      local_solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*local_solver));
      local_solver->set_restart_strategy(new conflict_counting_restart_strategy(*local_solver,100, 1.5));

      local_solver->add_theory(new bool_theory(*local_solver));
      local_solver->add_theory(new arithmetic_theory(*local_solver, buss, nbuss, 13, false, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_UFLIA")
    {
      local_solver->set_forget_strategy(new geometric_forget_strategy(*local_solver));
      local_solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*local_solver));
      local_solver->set_restart_strategy(new conflict_counting_restart_strategy(*local_solver,100, 1.5));

      local_solver->add_theory(new bool_theory(*local_solver));
      local_solver->add_theory(new euf_theory(*local_solver));
      local_solver->add_theory(new arithmetic_theory(*local_solver, buss, nbuss, 13, true, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_LIRA")
    {
      local_solver->set_forget_strategy(new geometric_forget_strategy(*local_solver));
      local_solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*local_solver));
      local_solver->set_restart_strategy(new conflict_counting_restart_strategy(*local_solver,100, 1.5));

      local_solver->add_theory(new bool_theory(*local_solver));
      local_solver->add_theory(new arithmetic_theory(*local_solver, buss, nbuss, 13, false, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_UFLIRA" || s == "ALL")
    {
      local_solver->set_forget_strategy(new geometric_forget_strategy(*local_solver));
      local_solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*local_solver));
      local_solver->set_restart_strategy(new conflict_counting_restart_strategy(*local_solver,100, 1.5));

      local_solver->add_theory(new bool_theory(*local_solver));
      local_solver->add_theory(new euf_theory(*local_solver));
      local_solver->add_theory(new arithmetic_theory(*local_solver, buss, nbuss, 13, true, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_RDL")
    {
      local_solver->set_forget_strategy(new geometric_forget_strategy(*local_solver));
      local_solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*local_solver));
      local_solver->set_restart_strategy(new conflict_counting_restart_strategy(*local_solver,100, 1.5));

      local_solver->add_theory(new bool_theory(*local_solver));
      local_solver->add_theory(new arithmetic_theory(*local_solver, buss, nbuss, 13, false, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_IDL")
    {
      local_solver->set_forget_strategy(new geometric_forget_strategy(*local_solver));
      local_solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*local_solver));
      local_solver->set_restart_strategy(new conflict_counting_restart_strategy(*local_solver,100, 1.5));

      local_solver->add_theory(new bool_theory(*local_solver));
      local_solver->add_theory(new arithmetic_theory(*local_solver, buss, nbuss, 13, false, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_CSP")
    {
      //local_solver->set_forget_strategy(new geometric_forget_strategy(*local_solver, 1000U, 1.1));
      //local_solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*local_solver));
      local_solver->set_forget_strategy(new never_forget_strategy());
      
      local_solver->set_restart_strategy(new conflict_counting_restart_strategy(*local_solver,100, 1.5));

      local_solver->add_theory(new bool_theory(*local_solver));
      csp_theory * csp = new csp_theory(*local_solver, as_algorithm,
					predefined_csp_bounds, lower_predefined_csp_bound, upper_predefined_csp_bound);
      local_solver->add_theory(csp);
      csp->_sum_common_data._upper_bounds_only = true;
      //csp->set_variable_order_strategy(new simple_csp_variable_order_strategy(csp));
      csp->set_variable_order_strategy(new most_constrained_csp_variable_order_strategy(csp));
      //csp->set_value_order_strategy(new simple_csp_value_order_strategy(csp));
      csp->set_value_order_strategy(new most_preferred_csp_value_order_strategy(csp));
      //local_solver->add_theory(new arithmetic_theory(*local_solver, buss, nbuss, 0, false, 0, randomize_simplex, simplex_shuffle_seed));	
    }
  else
    {
      delete local_solver;
      throw command_unsupported_exception();
    }

  check_sat_response csr = local_solver->solve();

  if(_smt_lib_api->get_info(keyword::STATUS))
    {
      check_sat_response known_csr; 
      _smt_lib_api->get_info(keyword::STATUS, known_csr);
      if(known_csr != CSR_UNKNOWN && csr != CSR_UNKNOWN && known_csr != csr)
	{
	  std::cerr << "ERROR FATAL!! WRONG ANSWER!!" << std::endl;
	}
    }
    
  if(csr == CSR_SAT && !local_solver->verify_assignment())
    {
      std::cerr << "VERIFICATION OF ASSIGNMENT FAILED!!" << std::endl;
    }
       
  return csr;
}

#ifdef _PARALLEL_PORTFOLIO

class solver_task : public tbb::task {
private:
  argosmt_solver_interface * _asi;
  const std::vector<clause *> & _clauses;
  unsigned _k;
  check_sat_response & _csr;
public:
  solver_task(argosmt_solver_interface * asi, const std::vector<clause *> & clauses, unsigned k, check_sat_response & csr)
    :_asi(asi),
     _clauses(clauses),
     _k(k),
     _csr(csr)
  {}

  virtual tbb::task * execute()
  {
    const std::vector<clause *> & clauses = _clauses;
    const unsigned k = _k;
    extern bool solving_cancelled;
    argosmt_solver_interface * asi = _asi;
    check_sat_response & csr = _csr;

    try {
      csr = asi->start_solver(clauses, k);
      std::cout << "FINISHED: " << k << std::endl;
      solving_cancelled = true;
      asi->set_solver_instance_index(k);
    }
    catch(solving_cancelled_exception e)
      {
	std::cout << "CANCELLED: " << k << std::endl;
      }
    return 0;
  }
};

class root_task : public tbb::task {
private:
  argosmt_solver_interface * _asi;
  const std::vector<clause *> & _clauses;
  unsigned _n;
  check_sat_response & _csr;
public:
  root_task(argosmt_solver_interface * asi, const std::vector<clause *> & clauses, unsigned n, check_sat_response & csr)
    :_asi(asi),
     _clauses(clauses),
     _n(n),
     _csr(csr)
  {}

  virtual tbb::task * execute()
  {
    const std::vector<clause *> & clauses = _clauses;
    const unsigned n = _n;
    argosmt_solver_interface * asi = _asi;
    check_sat_response & csr = _csr;

    set_ref_count(n + 1);
    for(unsigned i = 0; i < n; i++)
      {
	solver_task & task = *new(allocate_child()) solver_task(asi, clauses, i, csr);
	spawn(task);
      }
    wait_for_all();
    return 0;
  }
};

#endif 

check_sat_response argosmt_solver_interface::check_sat() 
{
  _cl.start();     
  _smt_lib_api->push();
  const expression_vector & assertions = _smt_lib_api->assertions();
  std::vector<clause *> clauses;
  
  // Tzeitin transformation, xor elimination, distinct binarization and so on...
  for(unsigned i = 0; i < assertions.size(); i++)
    {
      cnf_transformation(simplification(assertions[i]->eliminate_let_binders()->expand_expression()), clauses);
    }
#ifndef _PARALLEL_PORTFOLIO  
  check_sat_response csr = start_solver(clauses, 0);
  _solver = _solvers[0];
  statistics_solver_observer * stat = _statistics_observers[0];
#else
  extern bool solving_cancelled;
  solving_cancelled = false;
  check_sat_response csr = CSR_UNKNOWN;
  root_task & task = *new(tbb::task::allocate_root()) root_task(this, clauses, _solvers.size(), csr);
  tbb::task::spawn_root_and_wait(task);
  _solver = _solvers[_instance_index];
  statistics_solver_observer * stat = _statistics_observers[_instance_index];
#endif
  stat->report();
#ifdef _PARALLEL_PORTFOLIO
  _smt_lib_api->cout() << "Imported clauses: " << _solver->count_imports() << std::endl;
#endif
  stat->all_theories_report();
  
  // write :all-statistics info
  attribute_set attr;
  stat->get_statistics(attr);
    
  double elapsed = _cl.seconds_elapsed();
    
  attr.insert(attribute(":solving-time", elapsed));
  _smt_lib_api->set_info(keyword::ALL_STATISTICS, attr);
  _smt_lib_api->pop();
  return csr;
}
  
expression_vector argosmt_solver_interface::get_value(const expression_vector & exps) 
{
#ifndef _PARALLEL_PORTFOLIO
  const expression_vector & local_exps = exps;
#else
  expression_vector _exps;
  _exps.reserve(exps.size());
  for(unsigned i = 0; i < exps.size(); i++)
    _exps.push_back(exps[i]->clone_expression(_solver->get_factory()));
  const expression_vector & local_exps = _exps;
#endif

  _solver->get_model(local_exps);
  expression_vector result;
  for(unsigned i = 0; i < local_exps.size(); i++)
    {
      if(local_exps[i]->is_assigned())
#ifndef _PARALLEL_PORTFOLIO
	result.push_back(local_exps[i]);
#else
      {
	result.push_back(local_exps[i]->clone_expression(_smt_lib_api->get_expression_factory()));
	result.back()->set_assigned(local_exps[i]->get_assigned()->clone_expression(_smt_lib_api->get_expression_factory()));
      }
#endif
    }
  return result;
}
