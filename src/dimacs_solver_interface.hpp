/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2014,2021 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _DIMACS_SOLVER_INTERFACE_H
#define _DIMACS_SOLVER_INTERFACE_H

#include "solver.hpp"
#include "cmd_line_parser.hpp"
#include "clause_theory_solver.hpp"
#include "logging_solver_observer.hpp"
#include "statistics_solver_observer.hpp"
#include "first_undefined_literal_selection_strategy.hpp"
#include "random_literal_selection_strategy.hpp"
#include "random_decorater_literal_selection_strategy.hpp"
#include "vsids_literal_selection_strategy.hpp"
#include "interactive_literal_selection_strategy.hpp"
#include "never_forget_strategy.hpp"
#include "never_restart_strategy.hpp"
#include "conflict_counting_restart_strategy.hpp"
#include "always_positive_polarity_selection_strategy.hpp"
#include "always_negative_polarity_selection_strategy.hpp"
#include "saved_polarity_selection_strategy.hpp"
#include "random_polarity_selection_strategy.hpp"
#include "interactive_polarity_selection_strategy.hpp"
#include "geometric_forget_strategy.hpp"
#include "all_forget_selection_strategy.hpp"
#include "clause_activity_forget_selection_strategy.hpp"
#include "clause_length_forget_selection_strategy.hpp"
#include "wall_clock.hpp"
#include "timeout.hpp"

class dimacs_solver_interface {
private:
  solver * _solver;
  statistics_solver_observer * _stat;
  wall_clock _cl;
  
  signature _my_signature;
  expression_factory _my_factory;
  
  static int skip_spaces(std::istream & istr)
  {
    int c;
    while((c = istr.get()) == ' ' || c == '\t' || c == '\n');
    return c;
  }
  
  static void skip_rest_of_line(std::istream & istr)
  {
    int c;
    while((c = istr.get()) != '\n' && c != EOF);
  }
  
  void read_dimacs(std::istream & istr, std::vector<clause *> & clauses, unsigned & num_of_vars);
  
public:
  dimacs_solver_interface()
    :_solver(nullptr),
     _stat(nullptr),
     _my_factory(&_my_signature)
  {}

  check_sat_response start_solver(const std::vector<clause *> & clauses, unsigned num_of_vars);
  
  void check_sat();  
};
#endif // _DIMACS_SOLVER_INTERFACE_H
