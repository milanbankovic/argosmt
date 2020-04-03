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

#include "theory_declaration.hpp"

signature * core_theory_declaration::
create_signature(signature * logic_signature)
{
  return new signature(signature::T_SUB_SIGNATURE, logic_signature);
}

class divisible_function_checker : public function_symbol_checker {
private:
  signature * _signature;
public:
  divisible_function_checker(signature * sg)
    :_signature(sg)
  {}
  
  virtual bool check_symbol(const function_symbol & symbol,
			    const sort_vector & operand_sorts,
			    sort & return_sort)
  {
    if(!symbol.has_indices() || symbol.get_indices()[0] == 0)
      return false;
    
    if(operand_sorts.size() != 1)
      return false;
    
    sort int_sort = _signature->get_sort_factory()->create_sort(sort_symbol::INT);
    
    if(!operand_sorts[0]->is_equivalent(int_sort))
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

signature * ints_theory_declaration::create_signature(signature * logic_signature)
{
  signature * ints_signature = 
    new signature(signature::T_SUB_SIGNATURE, logic_signature);
  attribute_set attr(HASH_TABLE_SIZE);
  
  ints_signature->add_sort_symbol(sort_symbol::INT, 0);
  
  sort int_sort = ints_signature->get_sort_factory()->
    create_sort(sort_symbol::INT);
  
  sort bool_sort = ints_signature->get_sort_factory()->BOOL_SORT();
  
  ints_signature->
    add_special_constant_type(special_constant::T_NUMERAL, int_sort);
  
  ints_signature->add_function_symbol(function_symbol::MINUS, int_sort, int_sort);
        
  attr.clear();
  attr.insert(keyword::LEFT_ASSOC);
  
  ints_signature->
    add_function_symbol(function_symbol::MINUS, int_sort, int_sort, int_sort, attr);
  
  ints_signature->
    add_function_symbol(function_symbol::PLUS, int_sort, int_sort, int_sort, attr);
  
  ints_signature->
    add_function_symbol(function_symbol::TIMES, int_sort, int_sort, int_sort, attr);
  
  ints_signature->
    add_function_symbol(function_symbol::DIV, int_sort, int_sort, int_sort, attr);
  
  ints_signature->
    add_function_symbol(function_symbol::MOD, int_sort, int_sort, int_sort);
  
  ints_signature->
    add_function_symbol(function_symbol::ABS, int_sort, int_sort);
  
  attr.clear();
  attr.insert(keyword::CHAINABLE);
  
  ints_signature->
    add_function_symbol(function_symbol::LE, int_sort, int_sort, bool_sort, attr);
  
  ints_signature->
    add_function_symbol(function_symbol::LT, int_sort, int_sort, bool_sort, attr);
  
  ints_signature->
    add_function_symbol(function_symbol::GE, int_sort, int_sort, bool_sort, attr);
  
  ints_signature->
    add_function_symbol(function_symbol::GT, int_sort, int_sort, bool_sort, attr);
  ints_signature->
    add_function_symbol_checker(function_symbol::DIVISIBLE, 
				new divisible_function_checker(ints_signature));
  
  return ints_signature;
}

signature * 
reals_theory_declaration::create_signature(signature * logic_signature)
{
  signature * reals_signature = 
    new signature(signature::T_SUB_SIGNATURE, logic_signature);
  attribute_set attr(HASH_TABLE_SIZE);
  
  reals_signature->add_sort_symbol(sort_symbol::REAL, 0);
  
  sort real_sort = reals_signature->get_sort_factory()->
    create_sort(sort_symbol::REAL);
  
  sort bool_sort = reals_signature->get_sort_factory()->BOOL_SORT();
  
  reals_signature->
    add_special_constant_type(special_constant::T_NUMERAL, real_sort);
  reals_signature->
    add_special_constant_type(special_constant::T_DECIMAL, real_sort);

  
  reals_signature->add_function_symbol(function_symbol::MINUS, real_sort, real_sort);
        
  attr.clear();
  attr.insert(keyword::LEFT_ASSOC);
  
  reals_signature->
    add_function_symbol(function_symbol::MINUS, real_sort, real_sort, real_sort, attr);
  
  reals_signature->
    add_function_symbol(function_symbol::PLUS, real_sort, real_sort, real_sort, attr);
  
  reals_signature->
    add_function_symbol(function_symbol::TIMES, real_sort, real_sort, real_sort, attr);
  
  reals_signature->
    add_function_symbol(function_symbol::SLASH, real_sort, real_sort, real_sort, attr);
  
  attr.clear();
  attr.insert(keyword::CHAINABLE);
  
  reals_signature->
    add_function_symbol(function_symbol::LE, real_sort, real_sort, bool_sort, attr);
  
  reals_signature->
    add_function_symbol(function_symbol::LT, real_sort, real_sort, bool_sort, attr);
  
  reals_signature->
    add_function_symbol(function_symbol::GE, real_sort, real_sort, bool_sort, attr);
  
  reals_signature->
    add_function_symbol(function_symbol::GT, real_sort, real_sort, bool_sort, attr);
  
  return reals_signature;
}

signature * 
reals_ints_theory_declaration::create_signature(signature * logic_signature)
{
  signature * ri_signature = 
    new signature(signature::T_SUB_SIGNATURE, logic_signature);
  attribute_set attr(HASH_TABLE_SIZE);
  
  ri_signature->add_sort_symbol(sort_symbol::INT, 0);
  ri_signature->add_sort_symbol(sort_symbol::REAL, 0);
  
  sort int_sort = ri_signature->get_sort_factory()->
    create_sort(sort_symbol::INT);
  
  sort real_sort = ri_signature->get_sort_factory()->
    create_sort(sort_symbol::REAL);
  
  sort bool_sort = ri_signature->get_sort_factory()->BOOL_SORT();
  
  ri_signature->
    add_special_constant_type(special_constant::T_NUMERAL, int_sort);
  
  ri_signature->add_function_symbol(function_symbol::MINUS, int_sort, int_sort);
        
  attr.clear();
  attr.insert(keyword::LEFT_ASSOC);
  
  ri_signature->
    add_function_symbol(function_symbol::MINUS, int_sort, int_sort, int_sort, attr);
  
  ri_signature->
    add_function_symbol(function_symbol::PLUS, int_sort, int_sort, int_sort, attr);
  
  ri_signature->
    add_function_symbol(function_symbol::TIMES, int_sort, int_sort, int_sort, attr);
  
  ri_signature->
    add_function_symbol(function_symbol::DIV, int_sort, int_sort, int_sort, attr);
  
  ri_signature->
    add_function_symbol(function_symbol::MOD, int_sort, int_sort, int_sort);
  
  ri_signature->
    add_function_symbol(function_symbol::ABS, int_sort, int_sort);
  
  attr.clear();
  attr.insert(keyword::CHAINABLE);
  
  ri_signature->
    add_function_symbol(function_symbol::LE, int_sort, int_sort, bool_sort, attr);
  
  ri_signature->
    add_function_symbol(function_symbol::LT, int_sort, int_sort, bool_sort, attr);
  
  ri_signature->
    add_function_symbol(function_symbol::GE, int_sort, int_sort, bool_sort, attr);
  
  ri_signature->
    add_function_symbol(function_symbol::GT, int_sort, int_sort, bool_sort, attr);
  
  ri_signature->
    add_special_constant_type(special_constant::T_DECIMAL, real_sort);
  
  ri_signature->add_function_symbol(function_symbol::MINUS, real_sort, real_sort);
        
  attr.clear();
  attr.insert(keyword::LEFT_ASSOC);
  
  ri_signature->
    add_function_symbol(function_symbol::MINUS, real_sort, real_sort, real_sort, attr);
  
  ri_signature->
    add_function_symbol(function_symbol::PLUS, real_sort, real_sort, real_sort, attr);
  
  ri_signature->
    add_function_symbol(function_symbol::TIMES, real_sort, real_sort, real_sort, attr);
  
  ri_signature->
    add_function_symbol(function_symbol::SLASH, real_sort, real_sort, real_sort, attr);
  
  attr.clear();
  attr.insert(keyword::CHAINABLE);
  
  ri_signature->
    add_function_symbol(function_symbol::LE, real_sort, real_sort, bool_sort, attr);
  
  ri_signature->
    add_function_symbol(function_symbol::LT, real_sort, real_sort, bool_sort, attr);
  
  ri_signature->
    add_function_symbol(function_symbol::GE, real_sort, real_sort, bool_sort, attr);
  
  ri_signature->
    add_function_symbol(function_symbol::GT, real_sort, real_sort, bool_sort, attr);
  
  ri_signature->
    add_function_symbol(function_symbol::TO_REAL, int_sort, real_sort);
  ri_signature->
    add_function_symbol(function_symbol::TO_INT, real_sort, int_sort);
  ri_signature->
    add_function_symbol(function_symbol::IS_INT, real_sort, bool_sort);


  ri_signature->
    add_function_symbol_checker(function_symbol::DIVISIBLE, 
				new divisible_function_checker(ri_signature));

  return ri_signature;
}

class csp_symbol_checker : public function_symbol_checker {
private:
  signature * _signature;
public:
  csp_symbol_checker(signature * sig)
    :_signature(sig)
  {}
    
  virtual bool check_symbol(const function_symbol & symbol,
			    const sort_vector & operand_sorts,
			    sort & return_sort)
  {
    if(operand_sorts.size() < 2)
      return false;
      
    sort int_sort = _signature->get_sort_factory()->create_sort(sort_symbol::INT);
      
    for(sort_vector::const_iterator i = operand_sorts.begin(), 
	  ie = operand_sorts.end(); i != ie; ++i)
      if((*i) != int_sort)
	return false;
      
    sort bool_sort = _signature->get_sort_factory()->BOOL_SORT();
      
    if(return_sort != _signature->get_sort_factory()->UNDEFINED_SORT())
      {
	if(return_sort != bool_sort)
	  return false;
	else
	  return true;
      }
    else
      {
	return_sort = bool_sort;
	return true;
      }
  }
};

signature * 
constraints_theory_declaration::create_signature(signature * logic_signature)
{
  signature * sig = _itd.create_signature(logic_signature);
    
  sig->add_function_symbol_checker(function_symbol::ALLDIFF, new csp_symbol_checker(sig));
  sig->add_function_symbol_checker(function_symbol::NOT_ALLDIFF, new csp_symbol_checker(sig));
  return sig;
}

signature * arrays_ex_theory_declaration::
create_signature(signature * logic_signature)
{
  signature * arrays_ex_signature = 
    new signature(signature::T_SUB_SIGNATURE, logic_signature);
  
  arrays_ex_signature->
    add_sort_symbol(sort_symbol::ARRAY, 2);
  
  static const sort_parameter p_x("X");
  sort x_par = arrays_ex_signature->
    get_sort_factory()->create_sort(p_x);
  
  static const sort_parameter p_y("Y");
  sort y_par = arrays_ex_signature->
    get_sort_factory()->create_sort(p_y);
  
  sort array_sort = arrays_ex_signature->
    get_sort_factory()->create_sort(sort_symbol::ARRAY, x_par, y_par);
  
  arrays_ex_signature->
    add_function_symbol(function_symbol::SELECT, array_sort, x_par, y_par);
  
  arrays_ex_signature->
    add_function_symbol(function_symbol::STORE, array_sort, x_par, y_par, array_sort);
  return arrays_ex_signature;
}

class bit_vec_sort_symbol_checker : public sort_symbol_checker {
public:
  virtual bool check_symbol(const sort_symbol & symbol, arity ar)
  {
    return ar == 0 && symbol.get_indices()[0] > 0;
  }
    
  virtual arity get_arity(const sort_symbol & symbol)
  {
    return 0;
  }
    
};
  
class bit_vec_constant_checker : public special_constant_checker {
  signature * _signature;
    
public:
  bit_vec_constant_checker(signature * sg)
    :_signature(sg)
  {}
    
  virtual bool check_constant(const special_constant & sc, 
			      sort & sr)
  {
    unsigned index;
      
    if(sc.get_type() == special_constant::T_BINARY)
      {
	index = sc.get_value().size() - 2;
      }
    else if(sc.get_type() == special_constant::T_HEXADECIMAL)
      {
	index = (sc.get_value().size() - 2) * 4;
      }
    else
      return false;
      
    sort_symbol bv_sym(sort_symbol::BITVEC.get_name(), { index });
    sort bv_sort = _signature->get_sort_factory()->create_sort(bv_sym);
      
    if(sr == _signature->get_sort_factory()->UNDEFINED_SORT())
      {
	sr = bv_sort;
	return true;
      }
    else return sr->is_equivalent(bv_sort);
  }
};  
  
class bit_vec_concat_function_checker : public function_symbol_checker {
private:
  signature * _signature;
public:
  bit_vec_concat_function_checker(signature * sg)
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
    if(op1->get_type() != sort_node::T_SYMBOL ||
       op1->get_symbol().get_name() != sort_symbol::BITVEC.get_name() ||
       op1->get_symbol().get_indices().size() != 1)
      return false;
      
    unsigned i = op0->get_symbol().get_indices()[0].get_unsigned_value();
    unsigned j = op1->get_symbol().get_indices()[0].get_unsigned_value();
      
    sort_symbol res_sym(sort_symbol::BITVEC.get_name(), { i + j });
    sort res_sort = _signature->get_sort_factory()->create_sort(res_sym);
      
    if(return_sort == _signature->get_sort_factory()->UNDEFINED_SORT())
      {
	return_sort = res_sort;
	return true;
      }
    else return return_sort->is_equivalent(res_sort);
  }
};
  
class bit_vec_extract_function_checker : public function_symbol_checker {
private:
  signature * _signature;
public:
  bit_vec_extract_function_checker(signature * sg)
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
      
    if(symbol.get_indices().size() != 2)
      return false;
    
    unsigned i = symbol.get_indices()[0].get_unsigned_value();
    unsigned j = symbol.get_indices()[1].get_unsigned_value();
      
    if(j > i || i >= m) return false;
      
    sort_symbol res_sym(sort_symbol::BITVEC.get_name(), { i - j + 1 });
    sort res_sort = _signature->get_sort_factory()->create_sort(res_sym);
      
    if(return_sort == _signature->get_sort_factory()->UNDEFINED_SORT())
      {
	return_sort = res_sort;
	return true;
      }
    else return return_sort->is_equivalent(res_sort);
  }
};
  
  
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


signature * 
bit_vec_theory_declaration::create_signature(signature * logic_signature)
{
  signature * bit_vec_signature = 
    new signature(signature::T_SUB_SIGNATURE, logic_signature);
  
  bit_vec_signature->
    add_sort_symbol_checker(sort_symbol::BITVEC, 
			    new bit_vec_sort_symbol_checker());
  
  bit_vec_signature->
    add_special_constant_checker(new bit_vec_constant_checker(bit_vec_signature));
  
  bit_vec_signature->
    add_function_symbol_checker(function_symbol::CONCAT,
				new bit_vec_concat_function_checker(bit_vec_signature));
  
  bit_vec_signature->
    add_function_symbol_checker(function_symbol::EXTRACT,
				new bit_vec_extract_function_checker(bit_vec_signature));

  bit_vec_signature->
    add_function_symbol_checker(function_symbol::BVNOT,
				new bit_vec_unary_function_checker(bit_vec_signature));
  bit_vec_signature->
    add_function_symbol_checker(function_symbol::BVNEG,
				new bit_vec_unary_function_checker(bit_vec_signature));


  bit_vec_signature->
    add_function_symbol_checker(function_symbol::BVAND,
				new bit_vec_binary_function_checker(bit_vec_signature));
  bit_vec_signature->
    add_function_symbol_checker(function_symbol::BVOR,
				new bit_vec_binary_function_checker(bit_vec_signature));
  bit_vec_signature->
    add_function_symbol_checker(function_symbol::BVADD,
				new bit_vec_binary_function_checker(bit_vec_signature));
  bit_vec_signature->
    add_function_symbol_checker(function_symbol::BVMUL,
				new bit_vec_binary_function_checker(bit_vec_signature));
  bit_vec_signature->
    add_function_symbol_checker(function_symbol::BVUDIV,
				new bit_vec_binary_function_checker(bit_vec_signature));
  bit_vec_signature->
    add_function_symbol_checker(function_symbol::BVUREM,
				new bit_vec_binary_function_checker(bit_vec_signature));
  bit_vec_signature->
    add_function_symbol_checker(function_symbol::BVSHL,
				new bit_vec_binary_function_checker(bit_vec_signature));
  bit_vec_signature->
    add_function_symbol_checker(function_symbol::BVLSHR,
				new bit_vec_binary_function_checker(bit_vec_signature));


  bit_vec_signature->
    add_function_symbol_checker(function_symbol::BVULT,
				new bit_vec_bvult_function_checker(bit_vec_signature));

  return bit_vec_signature;
}
