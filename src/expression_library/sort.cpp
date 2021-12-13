/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2015,2021 Milan Bankovic (milan@matf.bg.ac.rs)

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
#include "vector_hash_code.hpp"

const sort_vector EMPTY_SORT_VECTOR = sort_vector();
sort_vector symbol_sort_node::_dummy_sort_vector = sort_vector();

const sort_node::iterator & sort_node::iterator::operator ++ ()
{
  if(_sorts.empty())
    return *this;
    
  if(!_sorts.back()->get_operands().empty())
    {
      _inds.back() = 0;
      _sorts.push_back(_sorts.back()->get_operands()[0]);
      _inds.push_back((unsigned)(-1));
    }
  else
    {
      while(true)
	{
	  _sorts.pop_back();
	  _inds.pop_back();
	  if(_sorts.empty())
	    break;
	    
	  _inds.back()++;
	  if(_inds.back() < _sorts.back()->get_operands().size())
	    {
	      _sorts.push_back(_sorts.back()->get_operands()[_inds.back()]);
	      _inds.push_back((unsigned)(-1));
	      break;
	    }		
	}
    }
    
  return *this;
}


void
symbol_sort_node::calculate_hash()
{
  _hash_code = default_hash_combiner()(!_operands->empty() ?  
				       vector_hash_code<sort>()(*_operands) : 0,
				       _symbol.hash_code());
}
  

symbol_sort_node::symbol_sort_node(sort_factory * factory, 
				   const sort_symbol & symbol, 
				   const sort_vector & operands)
  : symbol_sort_node(factory, symbol, !operands.empty() ? std::move(sort_vector(operands)) : std::move(_dummy_sort_vector))
{}

symbol_sort_node::symbol_sort_node(sort_factory * factory, 
				   const sort_symbol & symbol, 
				   sort_vector && operands)
  :sort_node(factory),
   _symbol(symbol),
   _parameters(0),
   _operands(!operands.empty() ? new sort_vector(std::move(operands)) : &EMPTY_SORT_VECTOR)
{
  if(factory->get_signature()->get_syntax_checking() &&
     !factory->get_signature()->check_sort_symbol(symbol, _operands->size()))
    throw bad_sort_symbol_exception("Sort cannot be instantiated", _symbol);

  calculate_hash();
}


bool symbol_sort_node::check_sort(const signature * sg,
				  search_mode smode) const
{
  if(!sg->check_sort_symbol(_symbol, _operands->size(), smode))
    return false;

  if(_operands->empty())
    return true;

  for(sort_vector::const_iterator it = _operands->begin(), 
	it_end = _operands->end(); it != it_end; ++it)
    {
      if(!(*it)->check_sort(sg, smode))
	return false;
    }
  
  return true;
}


const sort_parameter_set & symbol_sort_node::get_parameters()
{
  if(_parameters == 0)
    {
      if(_operands->empty())
	return *(_parameters = &EMPTY_SORT_PARAMETER_SET);

      sort_parameter_set * temp = 0;
      
      for(sort_vector::const_iterator 
	    it = _operands->begin(), it_end = _operands->end();
	  it != it_end; ++it)
	{
	  const sort_parameter_set & pars = (*it)->get_parameters();
	  
	  if(!pars.empty())
	    {
	      if(temp == 0)
		temp = new sort_parameter_set(HASH_TABLE_SIZE);
	      
	      temp->insert(pars.begin(), pars.end());
	    }
	}
      if(temp != 0)
	return *(_parameters = temp);
      else
	return *(_parameters = &EMPTY_SORT_PARAMETER_SET);
    }
  return *_parameters;
}

sort symbol_sort_node::get_instance(const instantiation & in) const
{
  if(_operands->empty())
    return const_cast<symbol_sort_node*>(this)->shared_from_this();

  sort_vector ops;
  
  for(sort_vector::const_iterator 
	it = _operands->begin(), end = _operands->end(); it != end; ++it)
    ops.push_back((*it)->get_instance(in));
  
  return this->get_factory()->create_sort(_symbol, std::move(ops));
}


bool symbol_sort_node::is_instance(const sort & sr,
				   instantiation & in) const
{
  if(sr->get_type() != T_SYMBOL || sr->get_symbol() != _symbol)
    return false;
 
  const sort_vector & sort_ops = sr->get_operands();

  if(sort_ops.size() != _operands->size())
    return false;

  if(_operands->empty())
    return true;
  
  for(sort_vector::const_iterator 
	it = _operands->begin(), end = _operands->end(), jt = sort_ops.begin(); 
      it != end; ++it, ++jt)
    if(!(*it)->is_instance(*jt, in))
      return false;
  
  return true;
}
  
sort symbol_sort_node::expand_sort(const signature * sg, 
				   search_mode smode) const
{
  sort_vector expanded_operands;
  for(sort_vector::const_iterator it = _operands->begin(), 
	it_end = _operands->end();
      it != it_end; ++it)
    expanded_operands.push_back((*it)->expand_sort(sg, smode));
  

  sort_symbol_checker * checker = 
    sg->check_sort_symbol(_symbol, _operands->size(), smode);

  if(checker == 0)
    {
      return _factory->create_sort(_symbol, std::move(expanded_operands));
    }
  
  const attribute_set & attr = checker->get_attributes(_symbol);

  attribute_set::const_iterator it = attr.find(keyword::DEFINITION);

  if(it != attr.end())
    {
      sort_definition_attribute_value * value = 
	dynamic_cast<sort_definition_attribute_value *>(it->get_value());
      assert(value != 0);
      const sort_definition & def = value->get_value();

      const sort_parameter_vector & pars = def.get_parameters();
      sort def_sort = def.get_sort();

      instantiation ins;
      sort_parameter_vector::const_iterator 
	pit = pars.begin(), 
	pit_end = pars.end();
      sort_vector::const_iterator sit = expanded_operands.begin();

      assert(pars.size() == expanded_operands.size());
      
      for(; pit != pit_end; ++pit, ++sit)
	{
	  ins.insert(std::make_pair(*pit, *sit)); 
	}

      return def_sort->expand_sort(sg, smode)->get_instance(ins);
    }
  else
    return _factory->create_sort(_symbol, std::move(expanded_operands));
}

void symbol_sort_node::out(std::ostream & ostr) const 
{
  if(_operands->empty())
    _symbol.out(ostr);
  else 
    {
      ostr << "( ";
      _symbol.out(ostr);
      ostr << " ";
      for(sort_vector::const_iterator 
	    it = _operands->begin(), end = _operands->end(); it != end; ++it)
	{
	  ostr << (*it) << " ";
	}
      ostr << ")";
    }
}

inline
void symbol_sort_node::accept_visitor(sort_visitor & visitor)
{
  visitor.visit_symbol_sort_node_preorder(this->shared_from_this());
  if(!_operands->empty())
    {
      for(sort_vector::const_iterator it = _operands->begin(), 
	    it_end = _operands->end(); it != it_end; ++it)
	(*it)->accept_visitor(visitor);
    }
  visitor.visit_symbol_sort_node_postorder(this->shared_from_this());
}

unsigned symbol_sort_node::get_complexity() const
{
  unsigned cpl = 1;
  if(!_operands->empty())
    {
      for(sort_vector::const_iterator it = _operands->begin(),
	    it_end = _operands->end(); it != it_end; ++it)
	cpl += (*it)->get_complexity();
    }
  return cpl;
}




sort_factory::sort_factory(signature * sg)
  :_sorts(HASH_TABLE_SIZE),
   _signature(sg),
   _destruction(false),
   _par_node(this),
   _sym_node(this)
{
  sg->add_sort_symbol(sort_symbol::BOOL, 0);
  _undefined_sort = insert_sort(new undefined_sort_node(this));
  _bool_sort = insert_sort(new symbol_sort_node(this, sort_symbol::BOOL));
}


sort sort_factory::insert_sort(sort_node * node)
{
  std::shared_ptr<sort_node> ptr;
  _sorts.insert(node);
  ptr.reset(node, sort_deleter(this));
  return ptr;
}

sort_factory::~sort_factory()
{
  _destruction = true;
  _sym_node._operands = &EMPTY_SORT_VECTOR;
  for(std::unordered_set<sort_node *>::iterator it = _sorts.begin(),
	it_end = _sorts.end(); it != it_end;
      ++it)
    {
      delete *it;
    }
}
