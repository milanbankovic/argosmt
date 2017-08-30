/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2015 Milan Bankovic (milan@matf.bg.ac.rs)

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

#include "signature.hpp"

const arity UNDEFINED_ARITY = ((unsigned)(0xffffffff));

attribute_set default_function_symbol_checker::_dummy_attribute_set = attribute_set();
attribute_set default_special_constant_checker::_dummy_attribute_set = attribute_set();
attribute_set special_constant_type_checker::_dummy_attribute_set = attribute_set();

sort_symbol_checker *
sort_symbol_registry::check_symbol(const sort_symbol & symbol,
				   arity ar) const
{
  if(symbol.is_pattern())
    return 0;
  
  sort_symbol_map::const_iterator it;
  
  if((it = _symbols.find(symbol)) != _symbols.end())
    {
      if(it->second->check_symbol(symbol, ar))
	return it->second;
    }
  else {
    for(sort_symbol_checker_vector::const_iterator it = _general_checkers.begin(),
	  it_end = _general_checkers.end(); it != it_end; ++it)
      {
	if((*it)->check_symbol(symbol, ar))
	  return *it;
      }
  }
  return 0;
}

sort_symbol_registry::~sort_symbol_registry()
{
  for(sort_symbol_map::iterator it = _symbols.begin(), it_end = _symbols.end();
      it != it_end; ++it)
    {
      delete it->second;
    }

  for(sort_symbol_checker_vector::iterator
	it = _general_checkers.begin(),
	it_end = _general_checkers.end();
      it != it_end; ++it)
    {
      delete *it;
    }
}


bool default_function_symbol_checker::check_regular(const sort_vector & operand_sorts,
						    sort & return_sort)
{
  if(_rank.size() != operand_sorts.size() + 1)
    return false;
  
  instantiation ins(HASH_TABLE_SIZE);
  
  if(return_sort != _signature->get_sort_factory()->UNDEFINED_SORT() && 
     !_rank.back()->is_instance_equivalent(return_sort, ins))
    return false;
    
  sort_vector::const_iterator 
    sort_vec_it = operand_sorts.begin(),
    sort_vec_it_end = operand_sorts.end(),
    rank_it = _rank.begin();
    
  for( ; sort_vec_it != sort_vec_it_end;
       ++sort_vec_it, ++rank_it)
    {
      if(!(*rank_it)->is_instance_equivalent(*sort_vec_it, ins))
	return false;
    } 
    
  return deduce_return_sort(return_sort, ins);
}

bool 
default_function_symbol_checker::
check_left_associativity(const sort_vector & operand_sorts,
			 sort & return_sort)
{
  if(_rank.size() != 3 || operand_sorts.size() <= 2)
    return false;
  
  if(attributes().find(keyword::LEFT_ASSOC) == attributes().end())
    return false;
  
 
  
  instantiation ins(HASH_TABLE_SIZE);
  
  if(return_sort != _signature->get_sort_factory()->UNDEFINED_SORT() && 
     !_rank[2]->is_instance_equivalent(return_sort, ins))
    return false;
  
  if(!_rank[0]->is_instance_equivalent(operand_sorts[0], ins))
    return false;
  
  sort_vector::const_iterator 
    sort_vec_it = operand_sorts.begin() + 1, 
    sort_vec_it_end = operand_sorts.end();
  for(; sort_vec_it != sort_vec_it_end;
      ++sort_vec_it)
    {
      if(!_rank[1]->is_instance_equivalent(*sort_vec_it, ins))
	return false;
    }	    
  
  return deduce_return_sort(return_sort, ins);
}

bool 
default_function_symbol_checker::
check_right_associativity(const sort_vector & operand_sorts,
			  sort & return_sort)
{
  if(_rank.size() != 3 || operand_sorts.size() <= 2)
    return false;
  
  if(attributes().find(keyword::RIGHT_ASSOC) == attributes().end())
    return false;
    
  instantiation ins(HASH_TABLE_SIZE);
  
  if(return_sort != _signature->get_sort_factory()->UNDEFINED_SORT() 
     && !_rank[2]->is_instance_equivalent(return_sort, ins))
    return false;
  
  
  if(!_rank[1]->is_instance_equivalent(operand_sorts.back(), ins))
    return false;
  
  sort_vector::const_iterator 
    sort_vec_it = operand_sorts.begin(), 
    sort_vec_it_end = operand_sorts.end() - 1;
  for(; sort_vec_it != sort_vec_it_end;
      ++sort_vec_it)
    {
      if(!_rank[0]->is_instance_equivalent(*sort_vec_it, ins))
	return false;
    }
  
  return deduce_return_sort(return_sort, ins);
}


bool 
default_function_symbol_checker::
check_chainable_pairwise(const sort_vector & operand_sorts,
			 sort & return_sort)
{
  if(_rank.size() != 3 || operand_sorts.size() <= 2)
    return false;
  
  
  if(attributes().find(keyword::CHAINABLE) == attributes().end() &&
     attributes().find(keyword::PAIRWISE) == attributes().end())
    return false;
  
  
  if(return_sort != _signature->get_sort_factory()->UNDEFINED_SORT() && 
     return_sort != _signature->get_sort_factory()->BOOL_SORT())
    return false;
  
  instantiation ins(HASH_TABLE_SIZE);

  sort_vector::const_iterator 
    sort_vec_it = operand_sorts.begin(), 
    sort_vec_it_end = operand_sorts.end();
  for(; sort_vec_it != sort_vec_it_end;
      ++sort_vec_it)
    {
      if(!_rank[0]->is_instance_equivalent(*sort_vec_it, ins))
	return false;
    }
  
  return deduce_return_sort(return_sort, ins);
}

bool 
default_function_symbol_checker::
deduce_return_sort(sort & return_sort, const instantiation & ins)
{
  if(return_sort == _signature->get_sort_factory()->UNDEFINED_SORT())
    {
      return_sort = _rank.back()->get_instance(ins);
    }
  
  if(!return_sort->check_sort(_signature) || 
     return_sort->has_parameters())
    {
      return_sort = _signature->get_sort_factory()->UNDEFINED_SORT();
      return false;
    }
  
  return true;
}

default_function_symbol_checker::
default_function_symbol_checker(signature * sg, 
				const rank & rn, 
				const attribute_set & attr) 
  : default_function_symbol_checker(sg,
				    rank(rn),
				    !attr.empty() ? std::move(attribute_set(attr)) : std::move(_dummy_attribute_set))
{}

default_function_symbol_checker::
default_function_symbol_checker(signature * sg, 
				rank && rn, 
				const attribute_set & attr) 
  : default_function_symbol_checker(sg,
				    std::move(rn),
				    !attr.empty() ? std::move(attribute_set(attr)) : std::move(_dummy_attribute_set))
{}


default_function_symbol_checker::
default_function_symbol_checker(signature * sg, 
				const rank & rn, 
				attribute_set && attr) 
  : default_function_symbol_checker(sg,
				    rank(rn),
				    std::move(attr))
{}


default_function_symbol_checker::
default_function_symbol_checker(signature * sg, 
				rank && rn, 
				attribute_set && attr) 
  :_signature(sg),
   _rank(std::move(rn)),
   _attr(!attr.empty() ? new attribute_set(std::move(attr)) : &EMPTY_ATTRIBUTE_SET) 
{
  if(!_signature->get_syntax_checking())
    return;

  for(rank::const_iterator it = _rank.begin(), it_end = _rank.end();
      it != it_end; 
      ++it)
    {
      assert((*it)->get_factory() == _signature->get_sort_factory());
      if((*it) == _signature->get_sort_factory()->UNDEFINED_SORT() ||
	 !(*it)->check_sort(_signature, S_STANDALONE))
	{
	  throw bad_sort_exception("Function symbol has bad sorts in its rank", *it);
	}
    }
}


bool
default_function_symbol_checker::
check_symbol(const function_symbol & symbol,
	     const sort_vector & operand_sorts,
	     sort & return_sort)
{
  if(check_regular(operand_sorts, return_sort) ||
     check_left_associativity(operand_sorts, return_sort) ||
     check_right_associativity(operand_sorts, return_sort) ||
     check_chainable_pairwise(operand_sorts, return_sort))
    return true;
  
  return false;
}

const attribute_set & 
default_function_symbol_checker::
get_attributes(const function_symbol & symbol,
	       const sort_vector & operand_sorts,
	       const sort & return_sort)
{
  return attributes();
}



function_symbol_checker * 
function_symbol_registry::check_symbol(const function_symbol & symbol,
				       const sort_vector & operand_sorts,
				       sort & return_sort) const
{
  if(symbol.is_pattern())
    return 0;

  for(sort_vector::const_iterator it = operand_sorts.begin(), 
	it_end = operand_sorts.end(); it != it_end; ++it)
    {
      if((*it)->has_parameters())
	return 0;
    }
  
  if(return_sort->has_parameters())
    return 0;
  
  
  std::pair<function_symbol_map::const_iterator, 
	    function_symbol_map::const_iterator> its 
    = _symbols.equal_range(symbol);
  
  for(function_symbol_map::const_iterator funs_it = its.first,
	funs_it_end = its.second;
      funs_it != funs_it_end;
      ++funs_it)
    {
      if(funs_it->second->check_symbol(symbol, operand_sorts, return_sort))
	return funs_it->second;
    }	
    
  for(function_symbol_checker_vector::const_iterator 
	it = _general_checkers.begin(),
	it_end = _general_checkers.end();
      it != it_end;
      ++it)
    {
      if((*it)->check_symbol(symbol, operand_sorts, return_sort))
	return *it;
    }	

  return 0;
}

function_symbol_registry::~function_symbol_registry()
{
  for(function_symbol_map::iterator 
	it = _symbols.begin(), 
	it_end = _symbols.end();
      it != it_end; ++it)
    {
      delete it->second;
    }

  for(function_symbol_checker_vector::iterator
	it = _general_checkers.begin(),
	it_end = _general_checkers.end();
      it != it_end; ++it)
    {
      delete *it;
    }
}

const rank UNDEFINED_RANK = rank();

default_special_constant_checker::
default_special_constant_checker(signature * sg,
				 const sort & sr,
				 const attribute_set & attr)
  : default_special_constant_checker(sg, sr,
				     !attr.empty() ? std::move(attribute_set(attr)) : std::move(_dummy_attribute_set))
{}

default_special_constant_checker::
default_special_constant_checker(signature * sg,
				 const sort & sr,
				 attribute_set && attr)
  :_signature(sg),
   _sort(sr),
   _attr(!attr.empty() ? new attribute_set(std::move(attr)) : &EMPTY_ATTRIBUTE_SET)
{
  if(sr == _signature->get_sort_factory()->UNDEFINED_SORT() ||
     !sr->check_sort(_signature, S_STANDALONE))
    throw bad_sort_exception("Special constant is assigned a bad sort", sr);
}


const attribute_set &  
default_special_constant_checker::
get_attributes(const special_constant & sc, const sort & sr)
{
  return attributes();
}

bool
default_special_constant_checker::
check_constant(const special_constant & sc, sort & sr)
{
  if(sr == _signature->get_sort_factory()->UNDEFINED_SORT())
    {
      if(!_sort->has_parameters())
	{
	  sr = _sort;
	  return true;
	}
      else
	return false;
    }
  else {
    instantiation ins;
    return _sort->is_instance_equivalent(sr, ins);
  }
}

special_constant_type_checker::
special_constant_type_checker(signature * sg,
			      special_constant::type tp,
			      const sort & sr,
			      const attribute_set & attr)
  : special_constant_type_checker(sg, tp, sr,
				  !attr.empty() ? std::move(attribute_set(attr)) : std::move(_dummy_attribute_set))
{}
  
special_constant_type_checker::
special_constant_type_checker(signature * sg,
			      special_constant::type tp,
			      const sort & sr,
			      attribute_set && attr)
  :_signature(sg),
   _type(tp),
   _sort(sr),
   _attr(!attr.empty() ? new attribute_set(std::move(attr)) : &EMPTY_ATTRIBUTE_SET)
{
  if(sr == _signature->get_sort_factory()->UNDEFINED_SORT() ||
     !sr->check_sort(_signature, S_STANDALONE))
    throw bad_sort_exception("Special constant type is assigned a bad sort", sr);
}


const attribute_set &
special_constant_type_checker::
get_attributes(const special_constant & sc, const sort & sr)
{
  return attributes();
}

bool 
special_constant_type_checker::
check_constant(const special_constant & sc, sort & sr)
{
  if(sc.get_type() != _type)
    return false;

  if(sr == _signature->get_sort_factory()->UNDEFINED_SORT())
    {
      if(!_sort->has_parameters())
	{
	  sr = _sort;
	  return true;
	}
      else
	return false;
    }
  else {
    instantiation ins;
    return _sort->is_instance_equivalent(sr, ins);
  }  
}


special_constant_checker *
special_constant_registry::check_constant(const special_constant & c,
					  sort & sr) const
{
    
  if(sr->has_parameters())
    return 0;
  
  std::pair<constant_checker_map::const_iterator, 
	    constant_checker_map::const_iterator> its 
    = _constant_checker_map.equal_range(c);
  
  for(constant_checker_map::const_iterator it = its.first,
	it_end = its.second;
      it != it_end;
      ++it)
    {
      if(it->second->check_constant(c, sr))
	return it->second;
    }	
  
  for(constant_checker_vector::const_iterator 
	it = _general_constant_checkers.begin(),
	it_end = _general_constant_checkers.end();
      it != it_end;
      ++it)
    {
      if((*it)->check_constant(c, sr))
	return *it;
    }	
  
  return 0;
}

special_constant_registry::~special_constant_registry()
{
  for(constant_checker_map::iterator 
	it = _constant_checker_map.begin(), 
	it_end = _constant_checker_map.end();
      it != it_end; ++it)
    {
      delete it->second;
    }

  for(constant_checker_vector::iterator
	it = _general_constant_checkers.begin(),
	it_end = _general_constant_checkers.end();
      it != it_end; ++it)
    {
      delete *it;
    }
}

signature::signature(type tp, signature * sig, bool syntax_checking)
{
  _type = tp;
  _sort_symbol_registry = new sort_symbol_registry(this);

  switch(_type)
    {
    case T_MAIN_SIGNATURE:
      _expansion_signature = 0;
      _syntax_checking = syntax_checking;
      _sort_factory = new sort_factory(this);
      break;
    case T_SUB_SIGNATURE:
      assert(sig != 0);
      _expansion_signature = sig;
      _syntax_checking = _expansion_signature->_syntax_checking;
      _expansion_signature->_sub_signatures.push_back(this);
      _sort_factory = _expansion_signature->_sort_factory;
      break;
    case T_EXPANSION_SIGNATURE:
      assert(sig != 0);
      assert(sig->_expansion_signature == 0);
      _expansion_signature = 0;
      _syntax_checking = sig->_syntax_checking;
      _sort_factory = sig->_sort_factory;
      sig->_expansion_signature = this;
      _sub_signatures.push_back(sig);
      break;
    }

  _function_symbol_registry = new function_symbol_registry(this);
  _spec_const_registry = new special_constant_registry(this);

  if(_type != T_EXPANSION_SIGNATURE)
    init_core_signature();
}

void signature::init_core_signature()
{
  add_sort_symbol(sort_symbol::BOOL, 0);
  sort bool_sort = _sort_factory->BOOL_SORT();
  add_function_symbol(function_symbol::TRUE, bool_sort);
  add_function_symbol(function_symbol::FALSE, bool_sort);
  add_function_symbol(function_symbol::NOT, bool_sort, bool_sort);
  
  attribute_set attr;
  attr.insert(keyword::LEFT_ASSOC);      
  add_function_symbol(function_symbol::AND, bool_sort, bool_sort, bool_sort, attr); 
  add_function_symbol(function_symbol::OR, bool_sort, bool_sort, bool_sort, attr);  
  add_function_symbol(function_symbol::XOR, bool_sort, bool_sort, bool_sort, attr);
  
  attr.clear();
  attr.insert(keyword::RIGHT_ASSOC);
  add_function_symbol(function_symbol::IMPLIES, bool_sort, bool_sort, bool_sort, attr);
  
  static const sort_parameter u_par("u");
  sort u_sort = _sort_factory->create_sort(u_par);
  attr.clear();
  attr.insert(keyword::CHAINABLE);
  add_function_symbol(function_symbol::EQ, u_sort, u_sort, bool_sort, attr);
  
  attr.clear();
  attr.insert(keyword::PAIRWISE);
  add_function_symbol(function_symbol::DISTINCT, u_sort, u_sort, bool_sort, attr);
  
  add_function_symbol(function_symbol::ITE, bool_sort, u_sort, u_sort, u_sort);
}

sort_symbol_checker * 
signature::check_sort_symbol(const sort_symbol & symbol,
			     arity ar, 
			     search_mode smode) const
{
  sort_symbol_checker * checker = 
    _sort_symbol_registry->check_symbol(symbol, ar); 
    
  if(checker)
    return checker;

   
  if(smode == S_STANDALONE || smode == S_CONTEXTUAL)
    {
      for(signature_vector::const_iterator 
	    it = _sub_signatures.begin(),
	    it_end = _sub_signatures.end();
	  it != it_end; ++it)
	{
	  sort_symbol_checker * checker = 
	    (*it)->check_sort_symbol(symbol, ar, S_STANDALONE);
	  if(checker)
	    return checker; 
	}
    }
 
  if(smode == S_CONTEXTUAL || smode == S_EXTENSIONAL)
    {
      if(_expansion_signature != 0)
	return _expansion_signature->check_sort_symbol(symbol, ar, smode);
    }
 
  return 0;
}

function_symbol_checker *
signature::check_function_symbol(const function_symbol & symbol,
				 const sort_vector & operand_sorts,
				 sort & sr,
				 search_mode smode) const
{
  function_symbol_checker * checker = 
    _function_symbol_registry->check_symbol(symbol, 
					    operand_sorts,
					    sr);
  
  if(checker)
    return checker;

  if(smode == S_STANDALONE || smode == S_CONTEXTUAL)
    {
      for(signature_vector::const_iterator 
	    it = _sub_signatures.begin(),
	    it_end = _sub_signatures.end();
	  it != it_end;
	  ++it)
	{
	  function_symbol_checker * checker =  
	    (*it)->check_function_symbol(symbol,
					 operand_sorts,
					 sr, S_STANDALONE);
	  if(checker)
	    return checker;
	}
    }
  
  if(smode == S_CONTEXTUAL || smode == S_EXTENSIONAL)
    {
      if(_expansion_signature != 0)
	return _expansion_signature->
	  check_function_symbol(symbol,
				operand_sorts,
				sr, S_EXTENSIONAL); 
    }

  return 0;
}


signature::~signature()
{
  if(_type != T_EXPANSION_SIGNATURE)
    {
      for(signature_vector::iterator it = _sub_signatures.begin(), 
	    it_end = _sub_signatures.end(); it != it_end; ++it)
	delete (*it);
    }

  if(_type == T_EXPANSION_SIGNATURE)
    {
      _sub_signatures[0]->_expansion_signature = 0;
    }

  if(_type == T_EXPANSION_SIGNATURE || _type == T_MAIN_SIGNATURE)
    {
      delete _expansion_signature;
    }

  delete _sort_symbol_registry;
  delete _function_symbol_registry;
  delete _spec_const_registry;
  
  if(_type == T_MAIN_SIGNATURE)
    delete _sort_factory;
}

special_constant_checker *
signature::check_special_constant(const special_constant & c,
				  sort & sr,
				  search_mode smode) const
{
  special_constant_checker * checker = 
    _spec_const_registry->check_constant(c, sr);

  if(checker)
    return checker;

  if(smode == S_STANDALONE || smode == S_CONTEXTUAL)
    {
      for(signature_vector::const_iterator 
	    it = _sub_signatures.begin(),
	    it_end = _sub_signatures.end();
	  it != it_end; ++it)
	{
	  special_constant_checker * checker = 
	    (*it)->check_special_constant(c, sr, S_STANDALONE);
	 
	  if(checker)
	    return checker;
	}
    }
 
  if(smode == S_CONTEXTUAL || smode == S_EXTENSIONAL)
    {
      if(_expansion_signature != 0)
	return _expansion_signature->
	  check_special_constant(c, sr, S_EXTENSIONAL);
    }
  
  return 0;
}
