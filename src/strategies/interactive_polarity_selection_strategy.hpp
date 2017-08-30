/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2014 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _INTERACTIVE_POLARITY_STRATEGY_H
#define _INTERACTIVE_POLARITY_STRATEGY_H

#include "polarity_selection_strategy.hpp"
#include "solver.hpp"

class interactive_polarity_selection_strategy : public polarity_selection_strategy {
private:
  solver & _solver;
public:
  interactive_polarity_selection_strategy(solver & sl)
    :_solver(sl)
  {}

  virtual expression get_literal(const expression & lit)
  {
    int polarity = 0;
    literal_data * l_data = _solver.get_literal_data(lit);
    std::cerr << "Enter polarity: ";
    std::cin >> polarity;
    return polarity >= 0 ? l_data->get_positive() : l_data->get_negative();
  }

};

#endif // _INTERACTIVE_POLARITY_STRATEGY_H
