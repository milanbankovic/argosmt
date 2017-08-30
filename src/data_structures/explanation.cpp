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

#include "explanation.hpp"
#include "solver.hpp"

const expression & explanation::operator [] (unsigned i) const 
{
  if(_sl)
    return _sl->get_literal_data((*_cl)[i + (i >= _index)])->get_opposite();
  
  return (*_lits)[i];
}

unsigned explanation::size() const
{
  if(_sl)
    return _cl->size() - (_index != (unsigned)(-1));

  return (*_lits).size();
}

void explanation::push_back(const expression & l)
{
  if(!_sl)
    (*_lits).push_back(l);
  else
    throw "push_back() not applicable";
}

void explanation::push_back(expression && l)
{
  if(!_sl)
    (*_lits).push_back(std::move(l));
  else
    throw "push_back() not applicable";
}


void explanation::reserve(unsigned n)
{
  if(!_sl)
    (*_lits).reserve(n);
  else
    throw "reserve() not applicable";
}

void explanation::out(std::ostream & ostr) const
{
  ostr << "[ ";
  for(unsigned i = 0; i < size() - 1; i++)
    ostr << (*this)[i] << ", ";
  ostr << (*this)[size() - 1] << " ]";
}
