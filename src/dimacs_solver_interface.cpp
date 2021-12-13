/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2015,2021 Milan Bankovic (milan@matf.bg.ac.rs)

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

#include "dimacs_solver_interface.hpp"
#include "solver_configuration.hpp"

void dimacs_solver_interface::read_dimacs(std::istream & istr, std::vector<clause *> & clauses, unsigned & num_of_vars)
{
  int c;
  unsigned num_of_clauses;
  
  while((c = skip_spaces(istr)) == 'c')
    skip_rest_of_line(istr);
    
  if(c != 'p')
    throw "Bad input";
  else
    {
      std::string s;
      istr >> s;
      if(s != "cnf")
	throw "Bad input";
	
      istr >> num_of_vars;
      istr >> num_of_clauses;
    }

  for(unsigned i = 0; i < num_of_clauses; i++)
    {

      clause * c = new clause();
      int n;
      istr >> n; 
      while(!istr.eof() && !istr.fail() && n != 0)
	{
	  sat_variable variable = n > 0 ? n : -n;
	  sat_literal_polarity polarity = n > 0 ? SLP_POSITIVE : SLP_NEGATIVE;
	  expression literal = 
	    _my_factory.create_expression(variable, polarity);
	    
	  c->push_back(literal);
	    
	  istr >> n;
	}
      
      if(istr.eof() || istr.fail())
	throw "Bad input";

      clauses.push_back(c);
    }
}

check_sat_response dimacs_solver_interface::start_solver(const std::vector<clause *> & clauses, unsigned num_of_vars)
{
  extern solver_configuration s_config;
  // Creating new solver instance and adding clauses to the solver
  _solver = new solver(&_my_factory, false);
  _solver->set_num_of_vars(num_of_vars);
  for(unsigned i = 0; i < clauses.size(); i++)
    _solver->add_clause(clauses[i]);
  


  //_solver->set_literal_selection_strategy(new random_literal_selection_strategy<>(*_solver, global_rng));
  //_solver->set_literal_selection_strategy(new interactive_literal_selection_strategy(*_solver));
  //_solver->set_polarity_selection_strategy(new interactive_polarity_selection_strategy(*_solver));
  //_solver->set_forget_strategy(new never_forget_strategy());
  //_solver->add_observer(new logging_solver_observer(*_solver, std::cout));
  //_solver->add_observer(new logging_solver_observer(*_solver, std::cerr));

std::mt19937 decide_rng(s_config.random_seed());
  
if(s_config.randomize_decide())
  {
      double r = s_config.random_decides_percent();
      unsigned d = s_config.random_decides_after_restart();

      _solver->set_literal_selection_strategy(new random_decorater_literal_selection_strategy<>
					      (*_solver, 
					       new vsids_literal_selection_strategy(*_solver), decide_rng, r, d));
    }
  else
    {
      _solver->set_literal_selection_strategy(new vsids_literal_selection_strategy(*_solver));
    }

  _solver->set_polarity_selection_strategy(new saved_polarity_selection_strategy(*_solver));
  _solver->set_forget_strategy(new geometric_forget_strategy(*_solver));
  _solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*_solver));
  _solver->set_restart_strategy(new conflict_counting_restart_strategy(*_solver,100, 1.5));
  _solver->add_observer(_stat = new statistics_solver_observer(*_solver, std::cout, 10000));
  _solver->add_theory_solver(new clause_theory_solver(*_solver));
  return _solver->solve();  
}


void dimacs_solver_interface::check_sat()
{
  _cl.start();
  std::vector<clause *> clauses;
  unsigned num_of_vars;
  read_dimacs(cmd_line_parser::input(), clauses, num_of_vars);
  std::cout << "Problem read in:" <<  _cl.seconds_elapsed() << "s" << std::endl;
  
  check_sat_response result = start_solver(clauses, num_of_vars);
  
  if(result == CSR_SAT)
    {
      std::cout << "SAT" << std::endl;
      std::cout << _solver->get_trail() << " 0 " << std::endl;

      if(cmd_line_parser::has_model_out())
	{
	  cmd_line_parser::model_out() << "SAT" << std::endl;
	  cmd_line_parser::model_out() << _solver->get_trail() << " 0 " << std::endl;
	}

      if(!_solver->verify_assignment())
	{
	  std::cout << "ERROR FATAL: BAD ASSIGNMENT" << std::endl;
	  exit(1);
	}
    }
  else if(result == CSR_UNSAT)
    {
      std::cout << "UNSAT" << std::endl;
    }
  else // CSR_UNKNOWN
    {
      std::cout << "UNKNOWN" << std::endl;
    }
  
  _stat->report();
  _stat->all_theories_report();  
  std::cout << "Trail final size: " << _solver->get_trail().size() << std::endl;
  double elapsed = _cl.seconds_elapsed();
  std::cout << "Total time: " << elapsed << std::endl;
}
