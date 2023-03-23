/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2015 Milan Bankovic (milan@matf.bg.ac.rs)

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

#include "conflict_set.hpp"
#include "explanation.hpp"
#include "solver.hpp"

conflict_set::conflict_set(solver & sl)
  :_solver(sl),
   _data(sl.extractor_counter()),
   _next_to_explain(0),
   _backjump_level(0),
   _last_level((unsigned)(-1)),
   _uip_literal(sl.get_factory()->UNDEFINED_EXPRESSION()),
   _is_conflict(false)
{
  _explained.reserve(1000);
  _conflicting.reserve(1000);
}
  


void conflict_set::reset_conflict()
{
  for(unsigned i = 0; i < _conflicting.size(); i++)
    {
      conflict_data * l_data = _data.get_data(_conflicting[i]);
      l_data->_pair_data->_conflicting = false;
      l_data->_pair_data->_subsumed = false;
    }
  _conflicting.clear();
  for(unsigned i = 0; i < _explained.size(); i++)
    {
      conflict_data * l_data = _data.get_data(_explained[i]);
      l_data->_pair_data->_explained = false;
    }
  _explained.clear();
  _next_to_explain = 0;
  _backjump_level = 0;
  _last_level = (unsigned)(-1);
  _uip_literal = _solver.get_factory()->UNDEFINED_EXPRESSION();
  _is_conflict = false;
}


void conflict_set::add_new_literals(const explanation & new_lits)
{
  unsigned nl_size = new_lits.size();
  for(unsigned i = 0; i < nl_size; i++)
    {
      expression literal = new_lits[i];
      conflict_data * l_data = _data.get_data(literal);
      unsigned level = _solver.get_trail().get_trail_level(literal);

      if(_last_level == 0) 
	{
	  if(!l_data->_pair_data->_explained)
	    {
	      _explained.push_back(std::move(literal));
	      l_data->_pair_data->_explained = true;
	    }
	}
      else if(level < _last_level)
	{
	  if(!l_data->_pair_data->_conflicting)
	    {
	      _conflicting.push_back(std::move(literal));
	      l_data->_pair_data->_conflicting = true;
	      if(level > _backjump_level)
		_backjump_level = level;
	    }
	}
      else if(_uip_literal->is_undefined())
	{
	  _uip_literal = std::move(literal);
	  continue;
	}
      else if(literal == _uip_literal)
	continue;
      else
	{
	  if(_solver.get_trail().get_trail_index(literal) < _solver.get_trail().get_trail_index(_uip_literal))
	    {
	      literal = std::move(_uip_literal);
	      _uip_literal = new_lits[i];
	    }
	  
	  l_data = _data.get_data(literal);
	  
	  if(!l_data->_pair_data->_explained)
	    {
	      _explained.push_back(std::move(literal));
	      l_data->_pair_data->_explained = true;
	    }	
	}
    }
}

bool conflict_set::check_subsumed(const expression & l, 
				  const explanation & expl)
{
  assert(std::find(_conflicting.begin(), _conflicting.end(), l) != _conflicting.end());
  
  unsigned expl_size = expl.size();
  for(unsigned i = 0; i < expl_size; i++)
    if(!_data.get_data(expl[i])->_pair_data->_conflicting)
      return false;

  return true;
}

void conflict_set::calculate_last_level(const explanation & conflicting)
{
  _last_level = 0;
  unsigned c_size = conflicting.size();
  for(unsigned i = 0; i < c_size; i++)
    {
      unsigned level = _solver.get_trail().get_trail_level(conflicting[i]);
      if(level > _last_level)
	_last_level = level;
    }
}


clause * conflict_set::get_clause() const
{
  clause * cl = new clause();
  cl->reserve(_conflicting.size());
  if(!_uip_literal->is_undefined())
    cl->push_back(_solver.get_literal_data(_uip_literal)->get_opposite());
  for(unsigned i = 0; i < _conflicting.size(); i++)
    if(!_data.get_data(_conflicting[i])->_pair_data->_subsumed)
      cl->push_back(_solver.get_literal_data(_conflicting[i])->get_opposite());
  return cl;
}
