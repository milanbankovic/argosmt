/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2014 Milan Bankovic (milan@matf.bg.ac.rs)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
***************************************************************************/

#include "logic_declaration.hpp"

/** Base class for expression-checking visitors (private class) */
class check_expression_visitor : public expression_visitor {
protected:
  bool _valid;
    
  bool check_constant_function_or_variable(const expression & e)
  {
    return e->is_constant_function() || e->is_variable();
  }

  bool check_integer_constant(const expression & e)
  {
    return e->is_numeral_constant() ||
      (e->is_function() && 
       e->get_symbol() == function_symbol::MINUS &&
       e->get_operands().size() == 1 && 
       e->get_operands()[0]->is_numeral_constant());
  }
    
  bool has_free_top_symbol(const expression & exp)
  {
    if(!exp->is_function())
      return false;

    sort_vector sv;
    for(expression_vector::const_iterator it = exp->get_operands().begin(),
	  it_end = exp->get_operands().end(); it != it_end; ++it)
      {
	sv.push_back((*it)->get_sort());
      }
    sort sr = exp->get_sort();
    const signature_vector & sigs = exp->get_factory()->
      get_signature()->get_sub_signatures();
    const function_symbol & f = exp->get_symbol();
      
    for(signature_vector::const_iterator it = sigs.begin(), 
	  it_end = sigs.end(); it != it_end; ++it)
      {
	if((*it)->check_function_symbol(f, sv, sr, S_STANDALONE))
	  {
	    return false;
	  }
      }
    return true;
  }
    
    
  bool check_function_or_variable(const expression & e)
  {

    return has_free_top_symbol(e) || e->is_variable();
  }

  bool check_non_zero_numeral(const expression & e)
  {
    static const std::string zero("0");


    return e->is_numeral_constant() && 
      e->get_special_constant().get_value() != zero;
  }
    
    
  bool check_rational_constant(const expression & e)
  {


    if(e->is_decimal_constant())
      return true;

    if(e->is_function() &&
       e->get_symbol() == function_symbol::MINUS &&
       e->get_operands().size() == 1 &&
       e->get_operands()[0]->is_decimal_constant())
      return true;

    if(e->is_function() &&
       e->get_symbol() == function_symbol::SLASH &&
       e->get_operands().size() == 2 &&
       check_integer_constant(e->get_operands()[0]) &&
       check_non_zero_numeral(e->get_operands()[1]))
      return true;

    return false;
  }


  bool check_int_diff(const expression & e)
  {
    return e->is_function() &&
      e->get_symbol() == function_symbol::MINUS &&
      e->get_operands().size() == 2 &&
      check_constant_function_or_variable(e->get_operands()[0]) &&
      check_constant_function_or_variable(e->get_operands()[1]);
  }

  bool check_equivalence(const expression & e)
  {
    const function_symbol & f = e->get_symbol();
      
    return 
      (f == function_symbol::EQ ||  
       f == function_symbol::DISTINCT) &&
      e->get_operands().size() > 0 &&
      e->get_operands()[0]->
      get_sort()->is_equivalent(e->get_factory()->
				get_signature()->get_sort_factory()->
				BOOL_SORT());
  }
    
  bool check_fraction(const expression & e)
  {
    return e->is_function() &&
      e->get_symbol() == function_symbol::SLASH &&
      e->get_operands().size() == 2 &&
      check_integer_constant(e->get_operands()[0]) &&
      check_non_zero_numeral(e->get_operands()[1]);
  }

    
  bool check_real_diff(const expression & e)
  {
    expression r1, r2;
    return e->is_function() &&
      e->get_symbol() == function_symbol::MINUS &&
      e->get_operands().size() == 2 &&
      check_plus(r1 = e->get_operands()[0]) &&
      check_plus(r2 = e->get_operands()[1]) &&
      r1->get_operands().size() == r2->get_operands().size();
  }



  bool check_plus(const expression & exp)
  {
    if(check_constant_function_or_variable(exp))
      return true;
      
    if(exp->get_symbol() != function_symbol::PLUS)
      return false;

    const expression_vector & ops = exp->get_operands();
    if(ops.size() < 2)
      return false;


    const expression & x = ops[0];
    if(!check_constant_function_or_variable(x))
      return false;
      
    for(expression_vector::const_iterator it = ops.begin(),
	  it_end = ops.end(); it != it_end; ++it)
      {
	if(*it != x)
	  return false;
      }
    return true;
  }
    
  check_expression_visitor()
    :_valid(true)
  {}

public:
  virtual bool is_valid() const
  {
    return _valid;
  }
    
};

/** Checks whether the expression is closed, i.e. it does not contain
    free variables */
class check_closed_expression_visitor : public check_expression_visitor {
private:
  sorted_variable_vector _qvars;

public:

  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    // DO NOTHING
  }
    
  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    if(!_valid)
      return;
      
    for(sorted_variable_vector::const_reverse_iterator it = _qvars.rbegin(),
	  it_end = _qvars.rend(); it != it_end; ++it)
      {
	if(it->get_variable() == node->get_variable() &&
	   it->get_sort()->is_equivalent(node->get_sort()))
	  return;
      }
    _valid = false;
  }
    
  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    // DO NOTHING
  }
    
  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }
    
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }
    
  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    if(!_valid)
      return;
    _qvars.insert(_qvars.end(), node->get_quantified_variables().begin(), 
		  node->get_quantified_variables().end());
  }
    
  virtual void visit_quantifier_expression_node_postorder(const expression & node) 
  {
    if(!_valid)
      return;
    _qvars.erase(_qvars.begin() + 
		 (_qvars.size() - node->get_quantified_variables().size()),
		 _qvars.end());
  }
    
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    if(!_valid)
      return;

    sorted_variable_vector v;
    for(variable_binding_vector::const_iterator 
	  it = node->get_variable_bindings().begin(), 
	  it_end = node->get_variable_bindings().end(); it != it_end; ++it)
      v.push_back(sorted_variable(it->get_variable(), 
				  it->get_expression()->get_sort()));

    _qvars.insert(_qvars.end(), v.begin(), v.end());   
  }


  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    if(!_valid)
      return;

    _qvars.erase(_qvars.begin() + 
		 (_qvars.size() - node->get_variable_bindings().size()),
		 _qvars.end());
  }
    
};

/** Checks whether there are no quantifiers */
class check_q_free_expression_visitor : public check_expression_visitor {
public:

  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }
    
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    _valid = false;
  }

  virtual void visit_quantifier_expression_node_postorder(const expression & node) 
  {
    // DO NOTHING
  }
    
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    // DO NOTHING
  }


  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

};

/** Checks whether there are no free sort symbols in the sort */
class check_free_sort_visitor : public sort_visitor {
protected:
  bool _valid;
public:
  check_free_sort_visitor()
    :_valid(true)
  {}

  virtual bool is_valid() const
  {
    return _valid;
  }

  virtual void visit_undefined_sort_node(const sort & node) 
  {
    // DO NOTHING
  }


  virtual void visit_symbol_sort_node_preorder(const sort & node)
  {
    if(!_valid)
      return;

    const signature_vector & sigs = node->get_factory()->
      get_signature()->get_sub_signatures(); 

    const sort_symbol & s = node->get_symbol();
    unsigned arity = node->get_operands().size();

    _valid = false;
    for(signature_vector::const_iterator it = sigs.begin(), 
	  it_end = sigs.end(); it != it_end; ++it)
      {
	if((*it)->check_sort_symbol(s, arity, S_STANDALONE))
	  {
	    _valid = true;
	    break;
	  }
      }
  }
    

  virtual void visit_symbol_sort_node_postorder(const sort & node)
  {
    // DO NOTHING
  }


  virtual void visit_parameter_sort_node(const sort & node)
  {
    // DO NOTHING
  }
    
};

  
/** Checks whether there are no free sort symbols in the expression's
    sort and in sorts of its subexpressions. */
class check_free_sort_expression_visitor : public check_expression_visitor {
private:
  check_free_sort_visitor _sort_visitor;
    
  void check_sort(const expression & node)
  {
    sort sr = node->get_sort()->expand_sort();
    sr->accept_visitor(_sort_visitor);
    _valid = _sort_visitor.is_valid();
  }

public:

  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }
    
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_postorder(const expression & node) 
  {
    // DO NOTHING
  }
    
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    // DO NOTHING
  }


  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

};

/** Checks whether all free function symbols in the expression are 
    constants */ 
class check_constant_expression_visitor : public check_expression_visitor {
public:
    
  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    if(!_valid)
      return;

    if(!node->is_constant_function() && 
       has_free_top_symbol(node))
      _valid = false;
  }
    
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_postorder(const expression & node) 
  {
    // DO NOTHING
  }
    
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    // DO NOTHING
  }


  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

};


/** Checks if Int expression is linear */
class check_ints_linear_expression_visitor : public check_expression_visitor {
  
public:

  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    if(!_valid)
      return;

    const function_symbol & f = node->get_symbol();

    if(f == function_symbol::SLASH ||
       f == function_symbol::DIV ||
       f == function_symbol::MOD ||
       f == function_symbol::ABS)
      {
	_valid = false;
	return;
      }

    if(f == function_symbol::TIMES)
      {
	if(node->get_operands().size() != 2)
	  {
	    _valid = false;
	    return;
	  }
	expression op1 = node->get_operands()[0];
	expression op2 = node->get_operands()[1];

   
	if(check_integer_constant(op1))
	  return;

	if(check_integer_constant(op2))
	  return;

	_valid = false;
	return;
      }
  }
    
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_postorder(const expression & node) 
  {
    // DO NOTHING
  }
    
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    // DO NOTHING
  }


  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

};


/** Checks if Int expression is linear (permits non-constant terms with
    free top function symbols in multiplications) */
class check_ints2_linear_expression_visitor : public check_expression_visitor {
    
public:

  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    if(!_valid)
      return;

    const function_symbol & f = node->get_symbol();

    if(f == function_symbol::SLASH ||
       f == function_symbol::DIV ||
       f == function_symbol::MOD ||
       f == function_symbol::ABS)
      {
	_valid = false;
	return;
      }

    if(f == function_symbol::TIMES)
      {
	if(node->get_operands().size() != 2)
	  {
	    _valid = false;
	    return;
	  }
	expression op1 = node->get_operands()[0];
	expression op2 = node->get_operands()[1];

	if(check_integer_constant(op1))
	  return;
	  
	if(check_integer_constant(op2))
	  return;

	_valid = false;
	return;
      }
  }
    
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_postorder(const expression & node) 
  {
    // DO NOTHING
  }
    
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    // DO NOTHING
  }


  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

};



/** Checks if Real expression is linear */
class check_reals_linear_expression_visitor : public check_expression_visitor {

public:

  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    if(!_valid)
      return;

    const function_symbol & f = node->get_symbol();

    if(f == function_symbol::DIV ||
       f == function_symbol::MOD ||
       f == function_symbol::ABS)
      {
	_valid = false;
	return;
      }

    if(f == function_symbol::SLASH && 
       !check_rational_constant(node))
      {
	_valid = false;
	return;
      }

    if(f == function_symbol::TIMES)
      {
	if(node->get_operands().size() != 2)
	  {
	    _valid = false;
	    return;
	  }
	expression op1 = node->get_operands()[0];
	expression op2 = node->get_operands()[1];

	if(op1->get_sort() != op2->get_sort())
	  {
	    _valid = false;
	    return;
	  }

	if(check_integer_constant(op1))
	  return;

	if(check_rational_constant(op1))
	  return;
	  
	if(check_integer_constant(op2))
	  return;
	  
	if(check_rational_constant(op2))
	  return;
	  
	_valid = false;
	return;
      }
  }
    
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_postorder(const expression & node) 
  {
    // DO NOTHING
  }
    
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    // DO NOTHING
  }


  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

};


/** Checks if all Array sorts are (Array Int Int) */
class check_int_array_sort_expression_visitor : public check_expression_visitor {
private:
  void check_sort(const expression & node)
  {
    sort sr = node->get_sort()->expand_sort();

    if(sr->get_symbol() == sort_symbol::ARRAY)
      {
	sort_factory * sfactory = node->get_factory()->
	  get_signature()->get_sort_factory();
	sort int_sort = sfactory->create_sort(sort_symbol::INT);
	sort a_sort = sfactory->create_sort(sort_symbol::ARRAY, int_sort, 
					    int_sort);
	if(sr != a_sort)
	  {
	    _valid = false;
	  }
      }
  }
    
public:

  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }
    
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void visit_quantifier_expression_node_postorder(const expression & node) 
  {
    // DO NOTHING
  }
    
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    // DO NOTHING
  }


  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

};


/** Checks whether all Array sorts are either (Array Int Real) or 
    (Array Int (Array Int Real)) */
class check_real_array_sort_expression_visitor : public check_expression_visitor {
  
  void check_sort(const expression & node)
  {
    sort sr = node->get_sort()->expand_sort();

    if(sr->get_symbol() == sort_symbol::ARRAY)
      {
	sort_factory * sfactory = node->get_factory()->
	  get_signature()->get_sort_factory();
	sort int_sort = sfactory->create_sort(sort_symbol::INT);
	sort real_sort = sfactory->create_sort(sort_symbol::REAL);

	sort oda_sort = sfactory->create_sort(sort_symbol::ARRAY, int_sort, 
					      real_sort);
	sort tda_sort = sfactory->create_sort(sort_symbol::ARRAY, int_sort,
					      oda_sort);
	  
	if(sr != oda_sort && sr != tda_sort)
	  {
	    _valid = false;
	  }
      }
  }
    
public:


  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }
    
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void visit_quantifier_expression_node_postorder(const expression & node) 
  {
    // DO NOTHING
  }
    
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    // DO NOTHING
  }


  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

};


/** Checks whether all Array sorts are of the form 
    (Array (_ Bitvec i) (_ Bitvec j)) */
class check_bit_vec_array_sort_expression_visitor : public check_expression_visitor {
private:  
  void check_sort(const expression & node)
  {
    sort sr = node->get_sort()->expand_sort();

    if(sr->get_symbol() == sort_symbol::ARRAY)
      {
	sort s1 = sr->get_operands()[0];
	sort s2 = sr->get_operands()[1];
	  
	if(s1->get_symbol().get_name() != sort_symbol::BITVEC.get_name() ||
	   s2->get_symbol().get_name() != sort_symbol::BITVEC.get_name())
	  {
	    _valid = false;
	  }
      }
  }
    
public:

  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }
    
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void visit_quantifier_expression_node_postorder(const expression & node) 
  {
    // DO NOTHING
  }
    
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    if(_valid)
      check_sort(node);
  }

  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    // DO NOTHING
  }


  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

};

/** Checks whether all atoms are of Int difference logic form 
    (as in QF_IDL) */
class check_ints_diff_expression_visitor : public check_expression_visitor {
    
public:

  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    if(!_valid)
      return;
      
    if(check_equivalence(node))
      return;

    const function_symbol & f = node->get_symbol();
    if(f == function_symbol::LT ||
       f == function_symbol::LE ||
       f == function_symbol::GT ||
       f == function_symbol::GE ||
       f == function_symbol::EQ ||
       f == function_symbol::DISTINCT)
      {
	if(node->get_operands().size() != 2)
	  {
	    _valid = false;
	    return;
	  }

	expression op1 = node->get_operands()[0];
	expression op2 = node->get_operands()[1];

	if((check_constant_function_or_variable(op1) || check_integer_constant(op1)) && 
	   (check_constant_function_or_variable(op2) || check_integer_constant(op2)))
	  return;
	
	if(check_int_diff(op1) &&
	   check_integer_constant(op2))
	  return;
	    
	_valid = false;
	return;
      }
  }
    
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_postorder(const expression & node) 
  {
    // DO NOTHING
  }
    
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    // DO NOTHING
  }


  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

};

/** Checks whether all atoms are of Int difference logic form 
    (as in QF_UFIDL) */
class check_ints2_diff_expression_visitor : public check_expression_visitor {

public:

  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    if(!_valid)
      return;

    const function_symbol & f = node->get_symbol();
    if(f == function_symbol::TIMES ||
       f == function_symbol::MOD ||
       f == function_symbol::DIV ||
       f == function_symbol::ABS)
      {
	_valid = false;
	return;
      }

    if(f == function_symbol::PLUS ||
       f == function_symbol::MINUS)
      {

	if(node->get_operands().size() != 2)
	  {
	    _valid = false;
	    return;
	  }

	expression op1 = node->get_operands()[0];
	expression op2 = node->get_operands()[1];

	if(op1->is_numeral_constant() ||
	   op2->is_numeral_constant())
	  return;
	    
	_valid = false;
	return;
      }
  }
    
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_postorder(const expression & node) 
  {
    // DO NOTHING
  }
    
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    // DO NOTHING
  }


  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

};



/** Checks whether all atoms are of Real difference logic form 
    (as in QF_RDL) */
class check_reals_diff_expression_visitor : public check_expression_visitor {
 
public:
  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    if(!_valid)
      return;

    if(check_equivalence(node))
      return;

    const function_symbol & f = node->get_symbol();

    if(f == function_symbol::LT ||
       f == function_symbol::LE ||
       f == function_symbol::GT ||
       f == function_symbol::GE ||
       f == function_symbol::EQ ||
       f == function_symbol::DISTINCT)
      {
	if(node->get_operands().size() != 2)
	  {
	    _valid = false;
	    return;
	  }

	expression op1 = node->get_operands()[0];
	expression op2 = node->get_operands()[1];

	if((check_constant_function_or_variable(op1) ||
	    check_integer_constant(op1) ||
	    check_rational_constant(op1)) && 
	   (check_constant_function_or_variable(op2) ||
	    check_integer_constant(op2) ||
	    check_rational_constant(op2)))
	  return;
	  
	if(check_real_diff(op1) && 
	   (check_integer_constant(op2) || check_fraction(op2)))
	  return;

	_valid = false;
	return;
      }
  }
    
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_quantifier_expression_node_postorder(const expression & node) 
  {
    // DO NOTHING
  }
    
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    // DO NOTHING
  }


  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

};


/** Composite checker base class */
class composite_check_expression_visitor : public check_expression_visitor {
protected:
  typedef std::vector<check_expression_visitor *> visitor_vector;
  visitor_vector _visitors;

public:
  virtual bool is_valid() const
  {
    for(visitor_vector::const_iterator it = _visitors.begin(), 
	  it_end = _visitors.end(); it != it_end; ++it)
      if(!(*it)->is_valid())
	return false;

    return true;
  }

  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    for(visitor_vector::const_iterator it = _visitors.begin(), 
	  it_end = _visitors.end(); it != it_end; ++it)
      (*it)->visit_undefined_expression_node(node);
  }

  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    for(visitor_vector::const_iterator it = _visitors.begin(), 
	  it_end = _visitors.end(); it != it_end; ++it)
      (*it)->visit_variable_expression_node(node);
  }

  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    for(visitor_vector::const_iterator it = _visitors.begin(), 
	  it_end = _visitors.end(); it != it_end; ++it)
      (*it)->visit_special_constant_expression_node(node);
  }

  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    for(visitor_vector::const_iterator it = _visitors.begin(), 
	  it_end = _visitors.end(); it != it_end; ++it)
      (*it)->visit_function_expression_node_preorder(node);
      
  }
    
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    for(visitor_vector::const_iterator it = _visitors.begin(), 
	  it_end = _visitors.end(); it != it_end; ++it)
      (*it)->visit_function_expression_node_postorder(node);
  }

  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    for(visitor_vector::const_iterator it = _visitors.begin(), 
	  it_end = _visitors.end(); it != it_end; ++it)
      (*it)->visit_quantifier_expression_node_preorder(node);

  }

  virtual void visit_quantifier_expression_node_postorder(const expression & node) 
  {
    for(visitor_vector::const_iterator it = _visitors.begin(), 
	  it_end = _visitors.end(); it != it_end; ++it)
      (*it)->visit_quantifier_expression_node_postorder(node);
  }
    
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    for(visitor_vector::const_iterator it = _visitors.begin(), 
	  it_end = _visitors.end(); it != it_end; ++it)
      (*it)->visit_let_expression_node_preorder(node);
  }

  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    for(visitor_vector::const_iterator it = _visitors.begin(), 
	  it_end = _visitors.end(); it != it_end; ++it)
      (*it)->visit_let_expression_node_inorder(node);
  }


  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    for(visitor_vector::const_iterator it = _visitors.begin(), 
	  it_end = _visitors.end(); it != it_end; ++it)
      (*it)->visit_let_expression_node_postorder(node);
  }

  ~composite_check_expression_visitor()
  {
    for(visitor_vector::const_iterator it = _visitors.begin(), 
	  it_end = _visitors.end(); it != it_end; ++it)
      delete (*it);
  }
};
  
/** Concrete composite checker visitors for each of the logics */

class check_auflia_expression_visitor : public composite_check_expression_visitor {
public:
  check_auflia_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_ints2_linear_expression_visitor());
    _visitors.push_back(new check_int_array_sort_expression_visitor());
  }
    
};


class check_auflira_expression_visitor : public composite_check_expression_visitor {
public:
  check_auflira_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_reals_linear_expression_visitor());
    _visitors.push_back(new check_real_array_sort_expression_visitor());
  }
    
};

class check_aufnira_expression_visitor : public composite_check_expression_visitor {
public:
  check_aufnira_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());      
  }
    
};


class check_lra_expression_visitor : public composite_check_expression_visitor {
public:
  check_lra_expression_visitor()
  {
    _visitors.push_back(new check_free_sort_expression_visitor());
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_reals_linear_expression_visitor());
    _visitors.push_back(new check_constant_expression_visitor());
  }
    
};

class check_qfabv_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfabv_expression_visitor()
  {
    _visitors.push_back(new check_free_sort_expression_visitor());
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_constant_expression_visitor());
    _visitors.push_back(new check_bit_vec_array_sort_expression_visitor());
  }
    
};
  
class check_qfaufbv_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfaufbv_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_bit_vec_array_sort_expression_visitor());
  }
    
};

class check_qfauflia_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfauflia_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_ints2_linear_expression_visitor());
    _visitors.push_back(new check_int_array_sort_expression_visitor());
  }
    
};

class check_qfax_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfax_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_constant_expression_visitor());
  }
    
};


 
class check_qfbv_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfbv_expression_visitor()
  {
    _visitors.push_back(new check_free_sort_expression_visitor());
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_constant_expression_visitor());
  }
    
};

class check_qfidl_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfidl_expression_visitor()
  {
    _visitors.push_back(new check_free_sort_expression_visitor());
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_ints_diff_expression_visitor());
  }
    
};


class check_qflia_expression_visitor : public composite_check_expression_visitor {
public:
  check_qflia_expression_visitor()
  {
    _visitors.push_back(new check_free_sort_expression_visitor());
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_ints_linear_expression_visitor());
    _visitors.push_back(new check_constant_expression_visitor());
  }
    
};
 
class check_qflra_expression_visitor : public composite_check_expression_visitor {
public:
  check_qflra_expression_visitor()
  {
    _visitors.push_back(new check_free_sort_expression_visitor());
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_reals_linear_expression_visitor());
    _visitors.push_back(new check_constant_expression_visitor());
  }
    
};

class check_qfnia_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfnia_expression_visitor()
  {
    _visitors.push_back(new check_free_sort_expression_visitor());
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_constant_expression_visitor());
  }
    
};
 
class check_qfnra_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfnra_expression_visitor()
  {
    _visitors.push_back(new check_free_sort_expression_visitor());
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_constant_expression_visitor());
  }
    
};
 
class check_qfrdl_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfrdl_expression_visitor()
  {
    _visitors.push_back(new check_free_sort_expression_visitor());
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_reals_diff_expression_visitor());
  }
    
};

class check_uf_expression_visitor : public composite_check_expression_visitor {
public:
  check_uf_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
  }
    
};


class check_qfuf_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfuf_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
  }
    
};
 
class check_qfufbv_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfufbv_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
  }
    
};


class check_qfufidl_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfufidl_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_ints2_diff_expression_visitor());
  }
    
};
 

class check_qfuflia_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfuflia_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_ints_linear_expression_visitor());
  }
    
};



  
class check_qfuflra_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfuflra_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_reals_linear_expression_visitor());
  }
    
};


  
class check_qfufnra_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfufnra_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
  }
    
};


  
class check_uflra_expression_visitor : public composite_check_expression_visitor {
public:
  check_uflra_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_reals_linear_expression_visitor());
  }
    
};

  
class check_ufnia_expression_visitor : public composite_check_expression_visitor {
public:
  check_ufnia_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
  }
    
};


class check_qfuflira_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfuflira_expression_visitor()
  {
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_reals_linear_expression_visitor());
  }
    
};


class check_qflira_expression_visitor : public composite_check_expression_visitor {
public:
  check_qflira_expression_visitor()
  {
    _visitors.push_back(new check_free_sort_expression_visitor());
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_reals_linear_expression_visitor());
    _visitors.push_back(new check_constant_expression_visitor());
  }
    
};


class check_qfcsp_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfcsp_expression_visitor()
  {
    _visitors.push_back(new check_free_sort_expression_visitor());
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_ints_linear_expression_visitor());
    _visitors.push_back(new check_constant_expression_visitor());
  }
};

class check_qfliacsp_expression_visitor : public composite_check_expression_visitor {
public:
  check_qfliacsp_expression_visitor()
  {
    _visitors.push_back(new check_free_sort_expression_visitor());
    _visitors.push_back(new check_closed_expression_visitor());
    _visitors.push_back(new check_q_free_expression_visitor());
    _visitors.push_back(new check_ints_linear_expression_visitor());
    _visitors.push_back(new check_constant_expression_visitor());
  }
};



void add_reals_ints_syntactic_sugar(signature * ri_sig)
{
  sort real_sort = ri_sig->get_sort_factory()->create_sort(sort_symbol::REAL);
  sort int_sort = ri_sig->get_sort_factory()->create_sort(sort_symbol::INT);
  sort bool_sort = ri_sig->get_sort_factory()->BOOL_SORT();

  /* Syntactic sugar */
  ri_sig->add_function_symbol(function_symbol::MINUS, int_sort, real_sort, real_sort);
  ri_sig->add_function_symbol(function_symbol::MINUS, real_sort, int_sort, real_sort);
  ri_sig->add_function_symbol(function_symbol::PLUS, int_sort, real_sort, real_sort);
  ri_sig->add_function_symbol(function_symbol::PLUS, real_sort, int_sort, real_sort);
  ri_sig->add_function_symbol(function_symbol::TIMES, int_sort, real_sort, real_sort);
  ri_sig->add_function_symbol(function_symbol::TIMES, real_sort, int_sort, real_sort);
  ri_sig->add_function_symbol(function_symbol::SLASH, int_sort, real_sort, real_sort);
  ri_sig->add_function_symbol(function_symbol::SLASH, real_sort, int_sort, real_sort);
  ri_sig->add_function_symbol(function_symbol::GT, int_sort, real_sort, bool_sort);
  ri_sig->add_function_symbol(function_symbol::GT, real_sort, int_sort, bool_sort);
  ri_sig->add_function_symbol(function_symbol::LT, int_sort, real_sort, bool_sort);
  ri_sig->add_function_symbol(function_symbol::LT, real_sort, int_sort, bool_sort);
  ri_sig->add_function_symbol(function_symbol::LE, int_sort, real_sort, bool_sort);
  ri_sig->add_function_symbol(function_symbol::LE, real_sort, int_sort, bool_sort);
  ri_sig->add_function_symbol(function_symbol::GE, int_sort, real_sort, bool_sort);
  ri_sig->add_function_symbol(function_symbol::GE, real_sort, int_sort, bool_sort);
  ri_sig->add_function_symbol(function_symbol::EQ, int_sort, real_sort, bool_sort);
  ri_sig->add_function_symbol(function_symbol::EQ, real_sort, int_sort, bool_sort);
  ri_sig->add_function_symbol(function_symbol::DISTINCT, int_sort, real_sort, bool_sort);
  ri_sig->add_function_symbol(function_symbol::DISTINCT, real_sort, int_sort, bool_sort);
  ri_sig->add_function_symbol(function_symbol::SLASH, int_sort, int_sort, real_sort);
   
}

class bit_vec_unary_function_checker : public function_symbol_checker {
private:
  signature * _signature;
public:
  bit_vec_unary_function_checker(signature * sg)
    :_signature(sg)
  {}
    
  virtual bool check_symbol(const function_symbol & symbol,
			    const sort_vector & operand_sorts,
			    sort & return_sort)
  {
    if(operand_sorts.size() != 1)
      return false;
      
    sort op0 = operand_sorts[0]->expand_sort();
      
    if(op0->get_type() != sort_node::T_SYMBOL ||
       op0->get_symbol().get_name() != sort_symbol::BITVEC.get_name() ||
       op0->get_symbol().get_indices().size() != 1)
      return false;
      
    sort res_sort = op0;
      
    if(return_sort == _signature->get_sort_factory()->UNDEFINED_SORT())
      {
	return_sort = res_sort;
	return true;
      }
    else return return_sort->is_equivalent(res_sort);
  }
};
  
class bit_vec_binary_function_checker : public function_symbol_checker {
private:
  signature * _signature;
public:
  bit_vec_binary_function_checker(signature * sg)
    :_signature(sg)
  {}
    
  virtual bool check_symbol(const function_symbol & symbol,
			    const sort_vector & operand_sorts,
			    sort & return_sort)
  {
    if(operand_sorts.size() != 2)
      return false;
      
    sort op0 = operand_sorts[0]->expand_sort();
    sort op1 = operand_sorts[1]->expand_sort();
      
    if(op0->get_type() != sort_node::T_SYMBOL ||
       op0->get_symbol().get_name() != sort_symbol::BITVEC.get_name() ||
       op0->get_symbol().get_indices().size() != 1)
      return false;
      
    if(op1 != op0)
      return false;
      
    sort res_sort = op0;
      
    if(return_sort == _signature->get_sort_factory()->UNDEFINED_SORT())
      {
	return_sort = res_sort;
	return true;
      }
    else return return_sort->is_equivalent(res_sort);
  }
};
  
class bit_vec_bvult_function_checker : public function_symbol_checker {
private:
  signature * _signature;
public:
  bit_vec_bvult_function_checker(signature * sg)
    :_signature(sg)
  {}
    
  virtual bool check_symbol(const function_symbol & symbol,
			    const sort_vector & operand_sorts,
			    sort & return_sort)
  {
    if(operand_sorts.size() != 2)
      return false;
      
    sort op0 = operand_sorts[0]->expand_sort();
    sort op1 = operand_sorts[1]->expand_sort();
      
    if(op0->get_type() != sort_node::T_SYMBOL ||
       op0->get_symbol().get_name() != sort_symbol::BITVEC.get_name() ||
       op0->get_symbol().get_indices().size() != 1)
      return false;
      
    if(op1 != op0)
      return false;
    
    sort res_sort = _signature->get_sort_factory()->BOOL_SORT();
    
    if(return_sort == _signature->get_sort_factory()->UNDEFINED_SORT())
      {
	return_sort = res_sort;
	return true;
      }
    else return return_sort->is_equivalent(res_sort);
  }
};


class bit_vec_constant_function_checker : public function_symbol_checker {
private:
  signature * _signature;
    
  bool is_bv_num(const std::string & name)
  {
    unsigned x;
      
    if(name.size() < 3)
      return false;
      
    if(name[0] != 'b' || name[1] != 'v')
      return false;
      
    std::istringstream istr;
    istr.str(name.substr(2));
    istr >> x;
    return !istr.fail() && istr.eof();
  }
    
public:
  bit_vec_constant_function_checker(signature * sg)
    :_signature(sg)
  {}
    
  virtual bool check_symbol(const function_symbol & symbol,
			    const sort_vector & operand_sorts,
			    sort & return_sort)
  {    
    if(operand_sorts.size() != 0)
      return false;
      
    if(!is_bv_num(symbol.get_name()))
      return false;
      
    if(!symbol.has_indices() || symbol.get_indices().size() != 1)
      return false;
      
    sort_symbol res_sym(sort_symbol::BITVEC.get_name(), symbol.get_indices());
    sort res_sort = _signature->get_sort_factory()->create_sort(res_sym);
      
    if(return_sort == _signature->get_sort_factory()->UNDEFINED_SORT())
      {
	return_sort = res_sort;
	return true;
      }
    else return return_sort->is_equivalent(res_sort);
  }
};
  
  
class bit_vec_comp_function_checker : public function_symbol_checker {
private:
  signature * _signature;
public:
  bit_vec_comp_function_checker(signature * sg)
    :_signature(sg)
  {}
    
  virtual bool check_symbol(const function_symbol & symbol,
			    const sort_vector & operand_sorts,
			    sort & return_sort)
  {
    if(operand_sorts.size() != 2)
      return false;
      
    sort op0 = operand_sorts[0]->expand_sort();
    sort op1 = operand_sorts[1]->expand_sort();
      
    if(op0->get_type() != sort_node::T_SYMBOL ||
       op0->get_symbol().get_name() != sort_symbol::BITVEC.get_name() ||
       op0->get_symbol().get_indices().size() != 1)
      return false;
      
    if(op1 != op0)
      return false;
      
    sort_symbol s(sort_symbol::BITVEC.get_name(), { 1 });
    sort res_sort = _signature->get_sort_factory()->create_sort(s);
      
    if(return_sort == _signature->get_sort_factory()->UNDEFINED_SORT())
      {
	return_sort = res_sort;
	return true;
      }
    else return return_sort->is_equivalent(res_sort);
  }
};
  
class bit_vec_repeat_function_checker : public function_symbol_checker {
private:
  signature * _signature;
public:
  bit_vec_repeat_function_checker(signature * sg)
    :_signature(sg)
  {}
  
  virtual bool check_symbol(const function_symbol & symbol,
			    const sort_vector & operand_sorts,
			    sort & return_sort)
  {
    if(operand_sorts.size() != 1)
      return false;
    
    sort op0 = operand_sorts[0]->expand_sort();
    
    if(op0->get_type() != sort_node::T_SYMBOL ||
       op0->get_symbol().get_name() != sort_symbol::BITVEC.get_name() ||
       op0->get_symbol().get_indices().size() != 1)
      return false;
    
    unsigned m = op0->get_symbol().get_indices()[0].get_unsigned_value();
    
    if(symbol.get_indices().size() != 1)
      return false;

    unsigned i = symbol.get_indices()[0].get_unsigned_value();

    sort_symbol res_sym(sort_symbol::BITVEC.get_name(), { i*m });
    sort res_sort = _signature->get_sort_factory()->create_sort(res_sym);
    
    if(return_sort == _signature->get_sort_factory()->UNDEFINED_SORT())
      {
	return_sort = res_sort;
	return true;
      }
    else return return_sort->is_equivalent(res_sort);
  }
};

class bit_vec_extend_function_checker : public function_symbol_checker {
private:
  signature * _signature;
public:
  bit_vec_extend_function_checker(signature * sg)
    :_signature(sg)
  {}
    
  virtual bool check_symbol(const function_symbol & symbol,
			    const sort_vector & operand_sorts,
			    sort & return_sort)
  {
    if(operand_sorts.size() != 1)
      return false;
    
    sort op0 = operand_sorts[0]->expand_sort();
    
    if(op0->get_type() != sort_node::T_SYMBOL ||
       op0->get_symbol().get_name() != sort_symbol::BITVEC.get_name() ||
       op0->get_symbol().get_indices().size() != 1)
      return false;
    
    unsigned m = op0->get_symbol().get_indices()[0].get_unsigned_value();

    if(symbol.get_indices().size() != 1)
      return false;

    unsigned i = symbol.get_indices()[0].get_unsigned_value();

    sort_symbol res_sym(sort_symbol::BITVEC.get_name(), { i + m });
    sort res_sort = _signature->get_sort_factory()->create_sort(res_sym);
    
    if(return_sort == _signature->get_sort_factory()->UNDEFINED_SORT())
      {
	return_sort = res_sort;
	return true;
      }
    else return return_sort->is_equivalent(res_sort);
  }
};




void add_bit_vec_syntactic_sugar(signature * bv_sig)
{

  bv_sig->add_function_symbol_checker(new bit_vec_constant_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVNAND, 
				      new bit_vec_binary_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVNOR, 
				      new bit_vec_binary_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVXOR, 
				      new bit_vec_binary_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVXNOR, 
				      new bit_vec_binary_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVSUB, 
				      new bit_vec_binary_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVSDIV, 
				      new bit_vec_binary_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVSREM, 
				      new bit_vec_binary_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVSMOD, 
				      new bit_vec_binary_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVASHR, 
				      new bit_vec_binary_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVCOMP, 
				      new bit_vec_comp_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::REPEAT, 
				      new bit_vec_repeat_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::ZERO_EXTEND, 
				      new bit_vec_extend_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::SIGN_EXTEND, 
				      new bit_vec_extend_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::ROTATE_LEFT, 
				      new bit_vec_unary_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::ROTATE_RIGHT, 
				      new bit_vec_unary_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVULE, 
				      new bit_vec_bvult_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVUGT, 
				      new bit_vec_bvult_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVUGE, 
				      new bit_vec_bvult_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVSLT, 
				      new bit_vec_bvult_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVSLE, 
				      new bit_vec_bvult_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVSGT, 
				      new bit_vec_bvult_function_checker(bv_sig));
  bv_sig->add_function_symbol_checker(function_symbol::BVSGE, 
				      new bit_vec_bvult_function_checker(bv_sig));
}


/** Definitions of functions create_signature() and check_expression() for
    each of the logics */

signature *   
auflia_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
    
  _ints.create_signature(sig);
  _arrays.create_signature(sig);
    
  return sig;
}

bool 
auflia_logic_declaration::check_expression(const expression & exp)
{
  check_auflia_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}


signature * 
auflira_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
   
  signature * ri_sig = _reals_ints.create_signature(sig);
  _arrays.create_signature(sig);

  add_reals_ints_syntactic_sugar(ri_sig);
   
  return sig;
}

bool 
auflira_logic_declaration::check_expression(const expression & exp)
{
  check_auflira_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();

}


signature * 
aufnira_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
   
  signature * ri_sig = _reals_ints.create_signature(sig);
  _arrays.create_signature(sig);

  add_reals_ints_syntactic_sugar(ri_sig);
    
  return sig;
}

bool 
aufnira_logic_declaration::check_expression(const expression & exp)
{
  check_aufnira_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}


signature * 
lra_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
  _reals.create_signature(sig);
    
  return sig;
}

bool 
lra_logic_declaration::check_expression(const expression & exp)
{
  check_lra_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();

}



signature * 
qfabv_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
    
  signature * bv_sig = _bvs.create_signature(sig);
  _arrays.create_signature(sig);

  add_bit_vec_syntactic_sugar(bv_sig);
    
  return sig;
}

bool 
qfabv_logic_declaration::check_expression(const expression & exp)
{
  check_qfabv_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}


signature *  
qfaufbv_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
    
  signature * bv_sig = _bvs.create_signature(sig);
  _arrays.create_signature(sig);
    
  add_bit_vec_syntactic_sugar(bv_sig);

  return sig;
}

bool 
qfaufbv_logic_declaration::check_expression(const expression & exp)
{
  check_qfaufbv_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}


signature * 
qfauflia_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
    
  _ints.create_signature(sig);
  _arrays.create_signature(sig);
    
  return sig;
}

bool 
qfauflia_logic_declaration::check_expression(const expression & exp)
{
  check_qfauflia_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();

}


signature * 
qfax_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
   
  _arrays.create_signature(sig);
    
  return sig;
}
  
bool 
qfax_logic_declaration::check_expression(const expression & exp)
{
  check_qfax_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}


signature * 
qfbv_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
   
  signature * bv_sig = _bvs.create_signature(sig);
    
  add_bit_vec_syntactic_sugar(bv_sig);

  return sig;
}

bool 
qfbv_logic_declaration::check_expression(const expression & exp)
{
  check_qfbv_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}


signature * 
qfidl_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
    
  _ints.create_signature(sig);
    
  return sig;
}

bool 
qfidl_logic_declaration::check_expression(const expression & exp)
{
  check_qfidl_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();

}


signature * 
qflia_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
    
  _ints.create_signature(sig);
    
  return sig;
}
  
bool 
qflia_logic_declaration::check_expression(const expression & exp)
{
  check_qflia_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();

}


signature * 
qflra_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
  _reals.create_signature(sig);
    
  return sig;
}
  
bool 
qflra_logic_declaration::check_expression(const expression & exp)
{
  check_qflra_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}


signature * 
qfnia_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
   
  _ints.create_signature(sig);
    
  return sig;
}
  
bool 
qfnia_logic_declaration::check_expression(const expression & exp)
{
  check_qfnia_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();

}


signature * 
qfnra_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
  _reals.create_signature(sig);
    
  return sig;
}

bool 
qfnra_logic_declaration::check_expression(const expression & exp)
{
  check_qfnra_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}


signature * 
qfrdl_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
  _reals.create_signature(sig);
    
  return sig;
}
  
bool 
qfrdl_logic_declaration::check_expression(const expression & exp)
{
  check_qfrdl_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();

}


signature * 
uf_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
  _core.create_signature(sig);
    
  return sig;
}
  
bool 
uf_logic_declaration::check_expression(const expression & exp)
{
  check_uf_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}


signature * 
qfuf_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
  _core.create_signature(sig);
    
  return sig;
}
  
bool 
qfuf_logic_declaration::check_expression(const expression & exp)
{
  check_qfuf_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}
 
 
signature * 
qfufbv_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
    
  signature * bv_sig = _bvs.create_signature(sig);

  add_bit_vec_syntactic_sugar(bv_sig);
    
  return sig;
}

bool 
qfufbv_logic_declaration::check_expression(const expression & exp)
{
  check_qfufbv_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();

}

  
signature * 
qfufidl_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
    
  _ints.create_signature(sig);
    
  return sig;
}
  
bool 
qfufidl_logic_declaration::check_expression(const expression & exp)
{
    
  check_qfufidl_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();

}


signature * 
qfuflia_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
    
  _ints.create_signature(sig);
    
  return sig;
}
  
bool 
qfuflia_logic_declaration::check_expression(const expression & exp)
{
  check_qfuflia_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}


signature * 
qfuflra_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
  _reals.create_signature(sig);
    
  return sig;
}
  
bool 
qfuflra_logic_declaration::check_expression(const expression & exp)
{
  check_qfuflra_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();

}

signature * 
qfufnra_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
  _reals.create_signature(sig);
    
  return sig;
}
  
bool 
qfufnra_logic_declaration::check_expression(const expression & exp)
{
  check_qfufnra_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();

}


signature * 
uflra_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
  _reals.create_signature(sig);
    
  return sig;
}

bool 
uflra_logic_declaration::check_expression(const expression & exp)
{
  check_uflra_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
    
}


signature * 
ufnia_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
    
  _ints.create_signature(sig);
    
  return sig;
}

bool 
ufnia_logic_declaration::check_expression(const expression & exp)
{
  check_ufnia_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}

signature * 
qfuflira_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
   
  signature * ri_sig = _reals_ints.create_signature(sig);
  add_reals_ints_syntactic_sugar(ri_sig);
   
  return sig;
}

bool 
qfuflira_logic_declaration::check_expression(const expression & exp)
{
  check_qfuflira_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}

signature * 
qflira_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
   
  signature * ri_sig = _reals_ints.create_signature(sig);
  add_reals_ints_syntactic_sugar(ri_sig);
   
  return sig;
}

bool 
qflira_logic_declaration::check_expression(const expression & exp)
{
  check_qflira_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}  


signature * 
qfcsp_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
   
  _constraints.create_signature(sig);
  return sig;
}

bool 
qfcsp_logic_declaration::check_expression(const expression & exp)
{
  check_qfcsp_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}  


signature * 
qfliacsp_logic_declaration::create_signature(bool syntax_checking)
{
  signature * sig = new signature(signature::T_MAIN_SIGNATURE, 0, 
				  syntax_checking);
    
  _constraints.create_signature(sig);
  return sig;
}

bool 
qfliacsp_logic_declaration::check_expression(const expression & exp)
{
  check_qfliacsp_expression_visitor visitor;
  exp->accept_visitor(visitor);
  return visitor.is_valid();
}  

