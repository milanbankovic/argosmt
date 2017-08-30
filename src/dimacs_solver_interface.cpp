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

check_sat_response dimacs_solver_interface::start_solver(const std::vector<clause *> & clauses, unsigned num_of_vars, unsigned k)
{
solver * local_solver = 0;
extern solver_configuration s_config;
// Creating new solver instance and adding clauses to the solver
#ifndef _PARALLEL_PORTFOLIO
expression_factory * factory = &_my_factory;
_solvers[k] = local_solver = new solver(factory, false);
#else
expression_factory * factory = new expression_factory(&_my_signature);
_solvers[k] = local_solver = new solver(factory, _solvers, k, s_config.share_size_limit(k), false);
#endif
  local_solver->set_num_of_vars(num_of_vars);
  for(unsigned i = 0; i < clauses.size(); i++)
    {
#ifndef _PARALLEL_PORTFOLIO
      clause * cl = clauses[i];
#else
      clause * cl = new clause();
      cl->reserve(clauses[i]->size());
      for(unsigned j = 0; j < clauses[i]->size(); j++)
	cl->push_back((*clauses[i])[j]->clone_expression(factory));
#endif
      local_solver->add_clause(cl);
    }


  //local_solver->set_literal_selection_strategy(new random_literal_selection_strategy<>(*local_solver, global_rng));
  //local_solver->set_literal_selection_strategy(new interactive_literal_selection_strategy(*local_solver));
  //local_solver->set_polarity_selection_strategy(new interactive_polarity_selection_strategy(*local_solver));
  //local_solver->set_forget_strategy(new never_forget_strategy());
  //local_solver->add_observer(new logging_solver_observer(*local_solver, std::cout));
  //local_solver->add_observer(new logging_solver_observer(*local_solver, std::cerr));

std::mt19937 decide_rng(s_config.random_seed(k));
  
if(s_config.randomize_decide(k))
  {
      double r = s_config.random_decides_percent(k);
      unsigned d = s_config.random_decides_after_restart(k);

      local_solver->set_literal_selection_strategy(new random_decorater_literal_selection_strategy<>
						   (*local_solver, 
						    new vsids_literal_selection_strategy(*local_solver), decide_rng, r, d));
    }
  else
    {
      local_solver->set_literal_selection_strategy(new vsids_literal_selection_strategy(*local_solver));
    }

  local_solver->set_polarity_selection_strategy(new saved_polarity_selection_strategy(*local_solver));
  local_solver->set_forget_strategy(new geometric_forget_strategy(*local_solver));
  local_solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*local_solver));
  local_solver->set_restart_strategy(new conflict_counting_restart_strategy(*local_solver,100, 1.5));
  local_solver->add_observer(_statistics_observers[k] = new statistics_solver_observer(*local_solver, std::cout, 10000));

  local_solver->add_theory(new bool_theory(*local_solver));
  
  return local_solver->solve();  
}

#ifdef _PARALLEL_PORTFOLIO

class solver_task : public tbb::task {
private:
  dimacs_solver_interface * _dsi;
  const std::vector<clause *> & _clauses;
  unsigned _k;
  unsigned _num_of_vars;
  check_sat_response & _result;
public:
  solver_task(dimacs_solver_interface * dsi, const std::vector<clause *> & clauses, unsigned k, unsigned num_of_vars, check_sat_response & result)
    :_dsi(dsi),
     _clauses(clauses),
     _k(k),
     _num_of_vars(num_of_vars),
     _result(result)
  {}

  virtual tbb::task * execute()
  {
    const std::vector<clause *> & clauses = _clauses;
    const unsigned k = _k;
    extern bool solving_cancelled;
    dimacs_solver_interface * dsi = _dsi;
    check_sat_response & result = _result;
    unsigned num_of_vars = _num_of_vars;
    
    try {
      result = dsi->start_solver(clauses, num_of_vars, k);
      std::cout << "FINISHED: " << k << std::endl;
      solving_cancelled = true;
      dsi->set_solver_instance_index(k);
    }
    catch(solving_cancelled_exception e)
      {
	std::cout << "CANCELLED: " << k << std::endl;
      }
    return 0;
  }
};

class root_task : public tbb::task {
private:
  dimacs_solver_interface * _dsi;
  const std::vector<clause *> & _clauses;
  unsigned _n;
  unsigned _num_of_vars;
  check_sat_response & _result;
public:
  root_task(dimacs_solver_interface * dsi, const std::vector<clause *> & clauses, unsigned n, unsigned num_of_vars, check_sat_response & result)
    :_dsi(dsi),
     _clauses(clauses),
     _n(n),
     _num_of_vars(num_of_vars),
     _result(result)
  {}

  virtual tbb::task * execute()
  {
    const std::vector<clause *> & clauses = _clauses;
    const unsigned n = _n;
    dimacs_solver_interface * dsi = _dsi;
    check_sat_response & result = _result;
    unsigned num_of_vars = _num_of_vars;
    set_ref_count(n + 1);
    for(unsigned i = 0; i < n; i++)
      {
	solver_task & task = *new(allocate_child()) solver_task(dsi, clauses, i, num_of_vars, result);
	spawn(task);
      }
    wait_for_all();
    return 0;
  }
};

#endif 


void dimacs_solver_interface::check_sat()
{
  _cl.start();
  std::vector<clause *> clauses;
  unsigned num_of_vars;
  read_dimacs(cmd_line_parser::input(), clauses, num_of_vars);
  std::cout << "Problem read in:" <<  _cl.seconds_elapsed() << "s" << std::endl;
  

#ifndef _PARALLEL_PORTFOLIO  
  check_sat_response result = start_solver(clauses, num_of_vars, 0);
  _solver = _solvers[0];
  statistics_solver_observer * stat = _statistics_observers[0];
#else
  extern bool solving_cancelled;
  solving_cancelled = false;
  check_sat_response result = CSR_UNKNOWN;
  root_task & task = *new(tbb::task::allocate_root()) root_task(this, clauses, _solvers.size(), num_of_vars, result);
  tbb::task::spawn_root_and_wait(task);
  _solver = _solvers[_instance_index];
  statistics_solver_observer * stat = _statistics_observers[_instance_index];
#endif
  
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
  
  stat->report();
#ifdef _PARALLEL_PORTFOLIO
  std::cout << "Imported clauses: " << _solver->count_imports() << std::endl;
#endif
  stat->all_theories_report();  
  std::cout << "Trail final size: " << _solver->get_trail().size() << std::endl;
  double elapsed = _cl.seconds_elapsed();
  std::cout << "Total time: " << elapsed << std::endl;
}
