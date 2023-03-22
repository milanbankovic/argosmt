/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2015,2021-2023 Milan Bankovic (milan@matf.bg.ac.rs)

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

#include "expression.hpp"

expression_vector EMPTY_EXPRESSION_VECTOR = expression_vector();

const sorted_variable_vector EMPTY_SORTED_VARIABLE_VECTOR =  sorted_variable_vector();

const sorted_variable_set EMPTY_SORTED_VARIABLE_SET =  sorted_variable_set();

const sat_variable UNDEFINED_SAT_VARIABLE = (unsigned)(-1);

const variable_binding_vector 
EMPTY_VARIABLE_BINDING_VECTOR =   variable_binding_vector();

expression_vector function_expression_node::_dummy_expression_vector = expression_vector();



const expression_node::iterator & expression_node::iterator::operator ++ ()
{
  if(_exps.empty())
    return *this;
    
  if(!_exps.back()->get_operands().empty())
    {
      _inds.back() = 0;
      _exps.push_back(_exps.back()->get_operands()[0]);
      _inds.push_back((unsigned)(-1));
    }
  else if(_exps.back()->is_quantifier())
    {
      _inds.back() = 0;
      _exps.push_back(_exps.back()->get_subexpression());
      _inds.push_back((unsigned)(-1));
    }
  else if(_exps.back()->is_let())
    {
      _inds.back() = 0;
      _exps.push_back(_exps.back()->
		      get_variable_bindings()[0].get_expression());
      _inds.push_back((unsigned)(-1));
    }
  else
    {
      while(true)
	{
	  _exps.pop_back();
	  _inds.pop_back();
	  if(_exps.empty())
	    break;
	    
	  _inds.back()++;
	  if(_exps.back()->is_let())
	    {
	      if(_inds.back() < _exps.back()->
		 get_variable_bindings().size())
		{
		  _exps.push_back(_exps.back()->
				  get_variable_bindings()[_inds.back()].
				  get_expression());
		  _inds.push_back((unsigned)(-1));
		  break;
		}
	      else if(_inds.back() == _exps.back()->
		      get_variable_bindings().size())
		{
		  _exps.push_back(_exps.back()->get_subexpression());
		  _inds.push_back((unsigned)(-1));
		  break;
		}
	    }
	  else if(_inds.back() < _exps.back()->get_operands().size())
	    {
	      _exps.push_back(_exps.back()->get_operands()[_inds.back()]);
	      _inds.push_back((unsigned)(-1));
	      break;
	    }		
	}
    }
    
  return *this;
}


bool is_private_attribute(const attribute & a)
{
  return a.get_keyword().get_name()[1] == '!';
}
  
bool check_non_private_attributes(const attribute_set & attr)
{
  attribute_set::const_iterator it, it_end;

  for(it = attr.begin(), it_end = attr.end(); it != it_end; ++it)
    if(!is_private_attribute(*it))
      return true;
  return false;
}
  
std::ostream & operator << (std::ostream & ostr, const expression & exp)
{
    
  const attribute_set & attr = exp->get_attributes();
  if(check_non_private_attributes(attr))
    {
      ostr << "(! ";
      exp->out(ostr);
      ostr << " ";
      for(attribute_set::const_iterator it = attr.begin(), 
	    it_end = attr.end(); it != it_end; ++it)
	{
	  if(!is_private_attribute(*it)) 
	    {
	      ostr << *it << " ";
	    }
	}
      ostr << ")";
    }
  else
    {
      exp->out(ostr);
    }
    
  return ostr;
}
  
sort_vector get_sorts(const expression_vector & expressions)
{
  sort_vector sorts(expressions.size());
  sort_vector::iterator sorts_it = sorts.begin(), sorts_it_end = sorts.end();
  expression_vector::const_iterator exp_it = expressions.begin();

  for(; sorts_it != sorts_it_end; ++sorts_it, ++exp_it)
    *sorts_it = (*exp_it)->get_sort();

  return sorts;
}

inline
expression function_expression_node::
get_instance(const substitution & sb) const
{
  if(_operands->empty())
    return const_cast<function_expression_node*>(this)->shared_from_this();

  expression_vector ops;
  
  for(expression_vector::const_iterator 
	it = _operands->begin(), end = _operands->end(); it != end; ++it)
    ops.push_back((*it)->get_instance(sb));
  
  return this->get_factory()->create_expression(_symbol, std::move(ops), _sort);
}

bool function_expression_node::
is_instance(const expression & exp,
	    substitution & sb) const
{
  if(exp->get_type() != T_FUNCTION || exp->get_symbol() != _symbol)
    return false;

  if(_sort != exp->get_sort())
    return false;
  
  const expression_vector & exp_ops = exp->get_operands();
  
  if(exp_ops.size() != _operands->size())
    return false;
 
  if(_operands->empty())
    return true;
 
  for(expression_vector::const_iterator 
	it = _operands->begin(), end = _operands->end(), jt = exp_ops.begin(); 
      it != end; ++it, ++jt)
    if(!(*it)->is_instance(*jt, sb))
      return false;
  
  return true;
}


const sorted_variable_set & 
function_expression_node::get_variables()
{
  if(_all_vars == 0)
    {
      if(_operands->empty())
	return *(_all_vars = &EMPTY_SORTED_VARIABLE_SET);
      
      sorted_variable_set * temp = 0;

      for(expression_vector::const_iterator 
	    it = _operands->begin(), it_end = _operands->end();
	  it != it_end; ++it)
	{
	  const sorted_variable_set & vars = (*it)->get_variables();
	  if(!vars.empty())
	    {
	      if(temp == 0)
		temp = new sorted_variable_set(HASH_TABLE_SIZE);
	      
	      temp->insert(vars.begin(), vars.end());
	    }
	}
      
      if(temp != 0)
	return *(_all_vars = temp);
      else
	return *(_all_vars = &EMPTY_SORTED_VARIABLE_SET);
    }

  return *_all_vars;
}


const sorted_variable_set & 
function_expression_node::get_free_variables()
{
  if(_free_vars == 0)
    {
      if(_operands->empty())
	return *(_free_vars = &EMPTY_SORTED_VARIABLE_SET);
      
      sorted_variable_set * temp = 0;

      for(expression_vector::const_iterator 
	    it = _operands->begin(), it_end = _operands->end();
	  it != it_end; ++it)
	{
	  const sorted_variable_set & vars = (*it)->get_free_variables();
	  if(!vars.empty())
	    {
	      if(temp == 0)
		temp = new sorted_variable_set(HASH_TABLE_SIZE);
	      
	      temp->insert(vars.begin(), vars.end());
	    }
	}
      
      if(temp != 0)
	return *(_free_vars = temp);
      else
	return *(_free_vars = &EMPTY_SORTED_VARIABLE_SET);
    }

  return *_free_vars;
}

bool 
function_expression_node::check_expression(const signature * sg,
					   search_mode smode) const
{
  sort sr = _sort;

  if(!sr->check_sort(sg, smode))
    return false;
  
  const sort_vector & sv = get_sorts(*_operands);
  if(!sg->check_function_symbol(_symbol, sv, sr, smode))
    return false;

  if(_operands->empty())
    return true;

  for(expression_vector::const_iterator it = _operands->begin(),
	it_end = _operands->end();
      it != it_end;
      ++it)
    {
      if(!(*it)->check_expression(sg, smode))
	return false;
    }

  return true;
}


expression function_expression_node::infer_sorts() const
{
  if(_factory->get_signature()->get_syntax_checking())
    return const_cast<function_expression_node*>(this)->shared_from_this();

  expression_vector infered_ops;
  for(expression_vector::const_iterator it = _operands->begin(),
	it_end = _operands->end(); it != it_end; ++it)
    {
      infered_ops.push_back((*it)->infer_sorts());
    }

  sort sr = _sort;
  if(_factory->get_signature()->
     check_function_symbol(_symbol, get_sorts(infered_ops), sr))
    return _factory->create_expression(_symbol, std::move(infered_ops), sr);
  else
    throw bad_function_symbol_exception("Cannot infer expression (function) sort", 
					_symbol);
}

expression function_expression_node::
expand_expression(const signature * sg,
		  search_mode smode) const
{

  expression_vector expanded_operands;
  for(expression_vector::const_iterator it = _operands->begin(), 
	it_end = _operands->end(); it != it_end; ++it)
    {
      expanded_operands.push_back((*it)->expand_expression(sg, smode));
    }

  sort sr = _sort; 
  if(!_factory->get_signature()->
     check_function_symbol(_symbol, get_sorts(expanded_operands), sr))
    throw bad_function_symbol_exception("Cannot expand expression (function)", 
					_symbol);
    
  function_symbol_checker * checker = 
    sg->check_function_symbol(_symbol, get_sorts(expanded_operands), 
			      sr, smode);

  if(checker == 0)
    {
      return _factory->create_expression(_symbol, std::move(expanded_operands), 
					 sr->expand_sort(sg, smode));
    }
  
  const attribute_set & attr = 
    checker->get_attributes(_symbol, get_sorts(expanded_operands), sr);

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
      expression_vector::const_iterator eit = expanded_operands.begin();

      assert(vars.size() == expanded_operands.size());
      
      for(; vit != vit_end; ++vit, ++eit)
	{
	  sub.insert(std::make_pair(vit->get_variable(), *eit)); 
	}

      return def_exp->expand_expression(sg, smode)->get_instance(sub);
    }
  else
    return _factory->create_expression(_symbol, std::move(expanded_operands), 
				       sr->expand_sort(sg, smode));
}


void 
function_expression_node::out(std::ostream & ostr) const
{
  if(_operands->empty())
    _symbol.out(ostr);
  else
    {
      ostr << "(";
      _symbol.out(ostr);
      for(expression_vector::const_iterator it = _operands->begin(),
	    it_end = _operands->end();
	  it != it_end; ++it)
	{
	  ostr << " " << *it;
	}
      ostr << ")";
    }
}


expression function_expression_node::eliminate_let_binders(const substitution & sub) const
{
  if(_operands->empty())
    return const_cast<function_expression_node*>(this)->shared_from_this();

  expression_vector nolet_operands;
  for(expression_vector::const_iterator it = _operands->begin(),
	it_end = _operands->end(); it != it_end; ++it)
    {
      nolet_operands.push_back((*it)->eliminate_let_binders(sub));
    }

  return _factory->create_expression(_symbol, std::move(nolet_operands), _sort);
}

void 
function_expression_node::accept_visitor(expression_visitor & visitor)
{
  visitor.visit_function_expression_node_preorder(this->shared_from_this());
  if(!_operands->empty())
    {
      for(expression_vector::const_iterator it = _operands->begin(),
	    it_end = _operands->end(); it != it_end; ++it)
	(*it)->accept_visitor(visitor);
    }
  visitor.visit_function_expression_node_postorder(this->shared_from_this());
}

expression function_expression_node::clone_expression(expression_factory * other_factory) const
{
  if(!_operands->empty())
    {
      expression_vector v(_operands->size());
      for(unsigned i = 0; i < _operands->size(); i++)
	v[i] = (*_operands)[i]->clone_expression(other_factory);
      return other_factory->create_expression(_symbol, std::move(v), _sort);
    }
  else
    {
      return other_factory->create_expression(_symbol, _sort);
    }
}


unsigned function_expression_node::get_complexity() const
{
  unsigned cpl = 1;

  if(!_operands->empty())
    {
      for(expression_vector::const_iterator it = _operands->begin(),
	    it_end = _operands->end(); it != it_end; ++it)
	cpl += (*it)->get_complexity();
    }
  return cpl;
}

void quantifier_expression_node::calculate_hash()
{
  _hash_code = default_hash_combiner()(default_hash_combiner()(_expr->hash_code(),
							       vector_hash_code<sorted_variable>()(_vars)),
				       std::hash<std::size_t>()((std::size_t) _quantifier));
}

quantifier_expression_node::
quantifier_expression_node(expression_factory * factory,
			   quantifier qn, 
			   const sorted_variable_vector & vars, 
			   const expression & expr)
  : quantifier_expression_node(factory,
			       qn,
			       sorted_variable_vector(vars),
			       expr)
{}

quantifier_expression_node::
quantifier_expression_node(expression_factory * factory,
			   quantifier qn, 
			   sorted_variable_vector && vars, 
			   const expression & expr)
  :expression_node(factory),
   _quantifier(qn),
   _vars(std::move(vars)),
   _expr(expr),
   _all_vars(0),
   _free_vars(0)
{
  if(_factory->get_signature()->get_syntax_checking())
    {
      if(_expr->get_sort() != _factory->get_signature()->get_sort_factory()->BOOL_SORT())
	throw quantified_non_boolean_exception("Expression (quantifier) cannot be instantiated", _expr);
    }
  calculate_hash();
}


quantifier_expression_node::
quantifier_expression_node(expression_factory * factory)
  :expression_node(factory),
   _quantifier(Q_UNDEFINED),
   _vars(),
   _expr(),
   _all_vars(0),
   _free_vars(0)
{}

const sorted_variable_set & 
quantifier_expression_node::get_variables()
{
  if(_all_vars == 0)
    {
      sorted_variable_set * temp = new sorted_variable_set(_expr->get_variables()); 
      temp->insert(_vars.begin(), _vars.end());

      return *(_all_vars = temp);
    }

  return *_all_vars;
}


const sorted_variable_set & 
quantifier_expression_node::get_free_variables()
{
  if(_free_vars == 0)
    {
      const sorted_variable_set & vars = _expr->get_free_variables();

      sorted_variable_set * temp = 0;

      for(sorted_variable_set::const_iterator it = vars.begin(), 
	    it_end = vars.end(); it != it_end; ++it)
	{
	  sorted_variable_vector::const_iterator jt = 
	    std::find(_vars.begin(), _vars.end(), *it);

	  if(jt == _vars.end())
	    {
	      if(temp == 0)
		temp = new sorted_variable_set(HASH_TABLE_SIZE);
	      
	      temp->insert(*it);
	    }
	}

      if(temp != 0)
	return *(_free_vars = temp);
      else
	return *(_free_vars = &EMPTY_SORTED_VARIABLE_SET);
    }

  return *_free_vars;
}


bool 
quantifier_expression_node::check_expression(const signature * sg,
					     search_mode smode) const
{
  for(sorted_variable_vector::const_iterator it = _vars.begin(),
	it_end = _vars.end(); it != it_end; ++it)
    {
      if(!it->get_sort()->check_sort(sg, smode) || 
	 it->get_sort()->has_parameters())
	return false;
    }
 
  return _expr->get_sort() == _factory->get_signature()->
    get_sort_factory()->BOOL_SORT() && 
    _expr->check_expression(sg, smode);
}

void 
quantifier_expression_node::out(std::ostream & ostr) const
{
  sorted_variable_vector::const_iterator 
    it = _vars.begin(), 
    it_end = _vars.end();

  ostr << (_quantifier == Q_FORALL ? "(forall (" : "(exists (") << *it++;
  
  for(;it != it_end; ++it)
    ostr << " " << *it;
  ostr << ") ";
  ostr << _expr;
  ostr << ")";
} 

bool contains_variable(const sorted_variable_vector & vec, const sorted_variable & v)
{
  return std::find(vec.begin(), vec.end(), v) != vec.end();  
}

bool contains_variable(const variable_binding_vector & vec, const sorted_variable & v)
{
  return std::find_if(vec.begin(), vec.end(), [&v] (const variable_binding & vb) -> bool
  {
    return vb.get_variable() == v.get_variable() &&
      vb.get_expression()->get_sort() == v.get_sort();
    
  }) != vec.end();  
}


bool contains_variable(const sorted_variable_set & s, const sorted_variable & v)
{
  return s.find(v) != s.end();
}


sorted_variable get_unique_variable(const expression_vector & expv, 
				    const sort & sr)
{
  static int i = 0;
  
  while(true)
    {
      ++i;
      sorted_variable v(std::string("UV") + std::to_string(i), sr);
       
      expression_vector::const_iterator it = expv.begin(), it_end = expv.end();
      for(; it != it_end; ++it)
	{
	  if(contains_variable((*it)->get_variables(), v))
	    break;
	}
      
      if(it != it_end)
	continue;
      
      return v;
    }
  return sorted_variable(variable::UNDEFINED, sort());
}

expression quantifier_expression_node::
get_instance(const substitution & sb) const
{
  substitution s;
  for(substitution::const_iterator it = sb.begin(),
	it_end = sb.end(); it != it_end; ++it)
    {
      if(!contains_variable(_vars, sorted_variable(it->first, it->second->get_sort())))
	{
	  s.insert(*it);
	}
    }
 
  if(s.empty())
    return const_cast<quantifier_expression_node*>(this)->shared_from_this();

  substitution vsub;
  sorted_variable_vector sub_vars;
  expression_vector expv;
  expv.push_back(const_cast<quantifier_expression_node*>(this)->shared_from_this());
  for(substitution::const_iterator sit = s.begin(), 
	sit_end = s.end(); sit != sit_end; ++sit)
    expv.push_back(sit->second);
  
  for(sorted_variable_vector::const_iterator it = _vars.begin(), 
	it_end = _vars.end(); it != it_end; ++it)
    {
      substitution::const_iterator jt = s.begin(), jt_end = s.end();
      for(;jt != jt_end; ++jt)
	{
	  const sorted_variable_set & fvars = jt->second->get_free_variables();
	  if(contains_variable(fvars, *it))
	    break;
	}
      
      if(jt != jt_end)
	{
	  sorted_variable v = get_unique_variable(expv, it->get_sort());
	  expression v_exp = _factory->create_expression(v.get_variable(),
							 v.get_sort());

	  vsub[it->get_variable()] = v_exp;
	  sub_vars.push_back(v);
	}
      else
	{
	  sub_vars.push_back(*it);
	}
    }

  if(vsub.empty())
    return _factory->create_expression(_quantifier, _vars, 
				       _expr->get_instance(s));
  else
    return _factory->create_expression(_quantifier, std::move(sub_vars), 
				       _expr->get_instance(vsub)->
				       get_instance(s)); 
  
}


bool quantifier_expression_node::is_instance(const expression & exp,
					     substitution & sb) const
{
  if(exp->get_type() != T_QUANTIFIER)
    return false;
  if(exp->get_quantifier() != _quantifier)
    return false;

  const sorted_variable_vector & exp_vars = exp->get_quantified_variables();

  if(_vars != exp_vars)
    return false;

  substitution temp_sub;
  for(sorted_variable_vector::const_iterator it = _vars.begin(), 
	it_end = _vars.end();
      it != it_end; ++it)
    {
      substitution::const_iterator sit = sb.find(it->get_variable());
      if(sit != sb.end())
	{
	  temp_sub.insert(*sit);
	  sb.erase(sit);
	}
      sb.insert(std::make_pair(it->get_variable(), 
			       _factory->
			       create_expression(it->get_variable(),
						 it->get_sort())));
    }
  
  bool retval = _expr->is_instance(exp->get_subexpression(), sb);
  
  for(sorted_variable_vector::const_iterator it = _vars.begin(), 
	it_end = _vars.end();
      it != it_end; ++it)
    {
      sb.erase(it->get_variable());
    }
  
  for(substitution::const_iterator sit = temp_sub.begin(),
	sit_end = temp_sub.end(); sit != sit_end; ++sit)
    {
      sb.insert(*sit);
    }
  
  return retval;
}

expression quantifier_expression_node::eliminate_let_binders(const substitution & sub) const
{
  substitution s;
  for(substitution::const_iterator it = sub.begin(),
	it_end = sub.end(); it != it_end; ++it)
    {
      if(!contains_variable(_vars, sorted_variable(it->first, it->second->get_sort())))
	{
	  s.insert(*it);
	}
    }
 
  if(s.empty())
    {
      return _factory->create_expression(_quantifier, _vars, _expr->eliminate_let_binders(s));
    }
  
  substitution vsub;
  sorted_variable_vector sub_vars;
  expression_vector expv;
  expv.push_back(const_cast<quantifier_expression_node*>(this)->shared_from_this());
  for(substitution::const_iterator sit = s.begin(), 
	sit_end = s.end(); sit != sit_end; ++sit)
    expv.push_back(sit->second);
  
  for(sorted_variable_vector::const_iterator it = _vars.begin(), 
	it_end = _vars.end(); it != it_end; ++it)
    {
      substitution::const_iterator jt = s.begin(), jt_end = s.end();
      for(;jt != jt_end; ++jt)
	{
	  const sorted_variable_set & fvars = jt->second->get_free_variables();
	  if(contains_variable(fvars, *it))
	    break;
	}
      
      if(jt != jt_end)
	{
	  sorted_variable v = get_unique_variable(expv, it->get_sort());
	  expression v_exp = _factory->create_expression(v.get_variable(),
							 v.get_sort());

	  vsub[it->get_variable()] = v_exp;
	  sub_vars.push_back(v);
	}
      else
	{
	  sub_vars.push_back(*it);
	}
    }

  if(vsub.empty())
    return _factory->create_expression(_quantifier, _vars, 
				       _expr->eliminate_let_binders(s));
  else
    return _factory->create_expression(_quantifier, std::move(sub_vars), 
				       _expr->get_instance(vsub)->eliminate_let_binders(s)); 
}

expression quantifier_expression_node::infer_sorts() const
{
  if(_factory->get_signature()->get_syntax_checking())
    return const_cast<quantifier_expression_node*>(this)->shared_from_this();

  expression infered_expr = _expr->infer_sorts();

  if(infered_expr->get_sort() != _factory->get_signature()->get_sort_factory()->BOOL_SORT())
    throw quantified_non_boolean_exception("Cannot infer expression (quantifier) sort", infered_expr);
  for(sorted_variable_vector::const_iterator it = _vars.begin(),
	it_end = _vars.end(); it != it_end; ++it)
    if(it->get_sort() == _factory->get_signature()->
       get_sort_factory()->UNDEFINED_SORT() ||
       it->get_sort()->has_parameters())
      throw bad_sort_exception("Cannot infer expression (quantifier) sort", 
			       it->get_sort());
   
  return _factory->create_expression(_quantifier, _vars, infered_expr);
}

  
expression quantifier_expression_node::
expand_expression(const signature * sg,
		  search_mode smode) const
{ 
  sorted_variable_vector exp_vars;
    
  for(sorted_variable_vector::const_iterator it = _vars.begin(),
	it_end = _vars.end(); it != it_end; ++it)
    {
      exp_vars.push_back(sorted_variable(it->get_variable(),
					 it->get_sort()->expand_sort(sg,
								     smode)));
    }
    

  expression expanded_expr = _expr->expand_expression(sg, smode);

  if(expanded_expr->get_sort() != _factory->get_signature()->get_sort_factory()->BOOL_SORT())
    throw quantified_non_boolean_exception("Cannot expand expression (quantifier)", expanded_expr);
    

  return _factory->create_expression(_quantifier, std::move(exp_vars), expanded_expr); 
}


void 
quantifier_expression_node::accept_visitor(expression_visitor & visitor)
{
  visitor.visit_quantifier_expression_node_preorder(this->shared_from_this());
  _expr->accept_visitor(visitor);
  visitor.visit_quantifier_expression_node_postorder(this->shared_from_this());
}

expression quantifier_expression_node::clone_expression(expression_factory * other_factory) const
{
  expression os = _expr->clone_expression(other_factory);
  return other_factory->create_expression(_quantifier, _vars, os);
}


void let_expression_node::calculate_hash()
{
  _hash_code = default_hash_combiner()(_expr->hash_code(),
				       vector_hash_code<variable_binding>()(_bind)); 
}

let_expression_node::
let_expression_node(expression_factory * factory,
		    const variable_binding_vector & bind, 
		    const expression & expr)
  : let_expression_node(factory,
			variable_binding_vector(bind),
			expr)
{}

let_expression_node::
let_expression_node(expression_factory * factory,
		    variable_binding_vector && bind, 
		    const expression & expr)
  :expression_node(factory),
   _bind(std::move(bind)),
   _expr(expr),
   _all_vars(0),
   _free_vars(0)
{
  if(_factory->get_signature()->get_syntax_checking())
    {
      // DO NOTHING
    }
  
  calculate_hash();
}


let_expression_node::
let_expression_node(expression_factory * factory)
  :expression_node(factory),
   _bind(),
   _expr(),
   _all_vars(0),
   _free_vars(0)
{}



const sorted_variable_set & 
let_expression_node::get_variables()
{
  if(_all_vars == 0)
    {
      sorted_variable_set * temp = new sorted_variable_set(_expr->get_variables()); 

      for(variable_binding_vector::const_iterator it = _bind.begin(), 
	    it_end = _bind.end(); it != it_end; ++it)
	{
	  temp->insert(sorted_variable(it->get_variable(), 
				       it->get_expression()->get_inferred_sort()));
	  
	  const sorted_variable_set & vars = it->get_expression()->get_variables();
	  temp->insert(vars.begin(), vars.end());
	}

      return *(_all_vars = temp);
    }

  return *_all_vars;
}


const sorted_variable_set & 
let_expression_node::get_free_variables()
{
  if(_free_vars == 0)
    {
      const sorted_variable_set & vars = _expr->get_free_variables();

      sorted_variable_set * temp = 0;
      sorted_variable_set bound_vars(HASH_TABLE_SIZE);
      
      for(sorted_variable_set::const_iterator it = vars.begin(), 
	    it_end = vars.end(); it != it_end; ++it)
	{
	  variable_binding_vector::const_iterator 
	    jt = _bind.begin(),
	    jt_end = _bind.end();

	  for(; jt != jt_end; ++jt)
	    {
	      if(jt->get_variable() == it->get_variable() &&
		 jt->get_expression()->get_inferred_sort() == it->get_sort())
		break;
	    }

	  if(jt == jt_end)
	    {
	      if(temp == 0)
		temp = new sorted_variable_set(HASH_TABLE_SIZE);

	      temp->insert(*it);
	    }
	  else
	    bound_vars.insert(*it);
	}

      for(variable_binding_vector::const_iterator 
	    jt = _bind.begin(),
	    jt_end = _bind.end(); jt != jt_end; ++jt)
	{
	  if(bound_vars.find(sorted_variable(jt->get_variable(),
					     jt->get_expression()->
					     get_inferred_sort())) == bound_vars.end())
	    {
	      continue;
	    }
	  
	  const sorted_variable_set & vars = 
	    jt->get_expression()->get_free_variables();
	  
	  if(!vars.empty()) 
	    {
	      if(temp == 0)
		temp = new sorted_variable_set(HASH_TABLE_SIZE);
	      
	      temp->insert(vars.begin(), vars.end());
	    }
	}
      
      if(temp != 0)
	return *(_free_vars = temp);
      else
	return *(_free_vars = &EMPTY_SORTED_VARIABLE_SET);
    }

  return *_free_vars;
}


bool 
let_expression_node::check_expression(const signature * sg,
				      search_mode smode) const
{
  for(variable_binding_vector::const_iterator it = _bind.begin(),
	it_end = _bind.end();
      it != it_end;
      ++it)
    {
      if(!it->get_expression()->check_expression(sg, smode))
	return false;
    }

  return _expr->check_expression(sg, smode);
}

expression let_expression_node::infer_sorts() const
{
  if(_factory->get_signature()->get_syntax_checking())
    return const_cast<let_expression_node*>(this)->shared_from_this();

  expression infered_expr = _expr->infer_sorts();
  variable_binding_vector infered_bind;

  for(variable_binding_vector::const_iterator it = _bind.begin(),
	it_end = _bind.end(); it != it_end; ++it)
    {
      infered_bind.push_back(variable_binding(it->get_variable(), 
					      it->get_expression()->
					      infer_sorts()));
    }
  
  return _factory->create_expression(std::move(infered_bind), infered_expr);
}


expression let_expression_node::
get_instance(const substitution & sb) const
{
  substitution s;
  for(substitution::const_iterator it = sb.begin(),
	it_end = sb.end(); it != it_end; ++it)
    {
      if(!contains_variable(_bind, sorted_variable(it->first, it->second->get_sort())))
	{
	  s.insert(*it);
	}
    }


  if(s.empty())
    {
      variable_binding_vector sub_bind;
      for(variable_binding_vector::const_iterator it = _bind.begin(),
	    it_end = _bind.end();
	  it != it_end;
	  ++it)
	{
	  sub_bind.push_back(variable_binding(it->get_variable(),
					      it->get_expression()->get_instance(sb)));
	}

      return _factory->create_expression(std::move(sub_bind), _expr);
    }


  substitution vsub;
  variable_binding_vector sub_bind;
  expression_vector expv;
  expv.push_back(const_cast<let_expression_node*>(this)->shared_from_this());
  for(substitution::const_iterator sit = s.begin(), 
	sit_end = s.end(); sit != sit_end; ++sit)
    expv.push_back(sit->second);
  
  for(variable_binding_vector::const_iterator it = _bind.begin(), 
	it_end = _bind.end(); it != it_end; ++it)
    {
      sort it_sort = it->get_expression()->get_sort();
      
      substitution::const_iterator jt = s.begin(), jt_end = s.end();
      for(;jt != jt_end; ++jt)
	{
	  const sorted_variable_set & fvars = jt->second->get_free_variables();
	  if(contains_variable(fvars, sorted_variable(it->get_variable(), it_sort)))
	    {
	      break;
	    }
	}
      
      if(jt != jt_end)
	{
	  sorted_variable v = get_unique_variable(expv, it_sort);
	  expression v_exp = _factory->create_expression(v.get_variable(),
							 v.get_sort());

	  vsub[it->get_variable()] = v_exp;
	  sub_bind.push_back(variable_binding(v.get_variable(), it->get_expression()->get_instance(sb)));
	}
      else
	{
	  sub_bind.push_back(variable_binding(it->get_variable(), it->get_expression()->get_instance(sb)));
	}
    }

  if(vsub.empty())
    return _factory->create_expression(std::move(sub_bind), _expr->get_instance(s));
  else
    return _factory->create_expression(std::move(sub_bind), 
				       _expr->get_instance(vsub)->
				       get_instance(s)); 

}


bool let_expression_node::is_instance(const expression & exp,
				      substitution & sb) const
{
  if(exp->get_type() != T_LET)
    return false;

  const variable_binding_vector & exp_bind = exp->get_variable_bindings();
  
  if(_bind.size() != exp_bind.size())
    return false;
 
  sorted_variable_vector vars, exp_vars;

  for(variable_binding_vector::const_iterator it = _bind.begin(), 
	it_end = _bind.end(), nit = exp_bind.begin(); 
      it != it_end; ++it, ++nit)
    {
      if(it->get_variable() != nit->get_variable())
	return false;
      
      if(!it->get_expression()->is_instance(nit->get_expression(), sb))
	return false;

      vars.push_back(sorted_variable(it->get_variable(), 
				     it->get_expression()->get_sort()));
      exp_vars.push_back(sorted_variable(nit->get_variable(), 
					 nit->get_expression()->get_sort()));
    }
  
  expression q_this = _factory->create_expression(Q_FORALL, std::move(vars), _expr);
  expression q_exp = _factory->create_expression(Q_FORALL, std::move(exp_vars), 
						 exp->get_subexpression());
  
  return q_this->is_instance(q_exp, sb);
}

expression let_expression_node::
expand_expression(const signature * sg,
		  search_mode smode) const
{  
  variable_binding_vector expanded_bind;
  for(variable_binding_vector::const_iterator it = _bind.begin(),
	it_end = _bind.end(); it != it_end; ++it)
    {
      expanded_bind.push_back(variable_binding(it->get_variable(),
					       it->get_expression()->
					       expand_expression(sg, 
								 smode)));
    }
  return _factory->create_expression(std::move(expanded_bind), 
				     _expr->
				     expand_expression(sg, smode));
}



void 
let_expression_node::out(std::ostream & ostr) const
{
  variable_binding_vector::const_iterator 
    it = _bind.begin(),
    it_end = _bind.end();
  
  ostr << "(let (" << *it++;
  for(;it != it_end; ++it)
    ostr << " " << *it;
  ostr << ") ";
  ostr << _expr;
  ostr << ")";
} 


expression let_expression_node::eliminate_let_binders(const substitution & sub) const
{
  substitution s;
  
  for(variable_binding_vector::const_iterator it = _bind.begin(),
	it_end = _bind.end();
      it != it_end;
      ++it)
    {
      s.insert(std::make_pair(it->get_variable(), it->get_expression()->eliminate_let_binders(sub)));
    }

  for(substitution::const_iterator it = sub.begin(),
	it_end = sub.end(); it != it_end; ++it)
    {
      if(!contains_variable(_bind, sorted_variable(it->first, it->second->get_sort())))
	{
	  s.insert(*it);
	}
    }

  return _expr->eliminate_let_binders(s);  
}

void 
let_expression_node::accept_visitor(expression_visitor & visitor)
{
  visitor.visit_let_expression_node_preorder(this->shared_from_this());
    
  for(variable_binding_vector::const_iterator it = _bind.begin(),
	it_end = _bind.end(); it != it_end; ++it)
    it->get_expression()->accept_visitor(visitor);

  visitor.visit_let_expression_node_inorder(this->shared_from_this());

  _expr->accept_visitor(visitor);
    
  visitor.visit_let_expression_node_postorder(this->shared_from_this());
}  

expression let_expression_node::clone_expression(expression_factory * other_factory) const
{
  expression os = _expr->clone_expression(other_factory);

  variable_binding_vector ovb;
  ovb.reserve(_bind.size());
  for(unsigned i = 0; i < _bind.size(); i++)
    {
      ovb.push_back(variable_binding(_bind[i].get_variable(),
				     _bind[i].get_expression()->clone_expression(other_factory)));
    }
  return other_factory->create_expression(std::move(ovb), os);
}
  

unsigned let_expression_node::get_complexity() const
{
  unsigned cpl = 1 + _expr->get_complexity();
    
  for(variable_binding_vector::const_iterator it = _bind.begin(),
	it_end = _bind.end(); it != it_end; ++it)
    cpl += 1 + it->get_expression()->get_complexity();
    
  return cpl;
}


expression expression_factory::insert_expression(expression_node * node)
{
  std::shared_ptr<expression_node> ptr;
  _expressions.insert(node);
  ptr.reset(node, expression_deleter(this));
  return ptr;
}


expression_factory::~expression_factory()
{
  _destruction = true;
  _fun_node._operands = &EMPTY_EXPRESSION_VECTOR;
  for(std::unordered_set<expression_node *>::iterator it = 
	_expressions.begin(),
	it_end = _expressions.end(); it != it_end;
      ++it)
    {
      delete *it;
    }
}
