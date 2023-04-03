/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2016 Milan Bankovic (milan@matf.bg.ac.rs)

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
#include "argosmt_solver_interface.hpp"
#include "solver_configuration.hpp"

check_sat_response argosmt_solver_interface::start_solver(const std::vector<clause *> & clauses, formula_transformer * ft)
{
  extern solver_configuration s_config;
  delete _solver;

  // Creating new solver instance and adding clauses to the solver
  expression_factory * factory = _smt_lib_api->get_expression_factory();
  _solver = new solver(factory, false);
  _solver->set_produce_proofs(_smt_lib_api->produce_proofs());

  
  for(unsigned i = 0; i < clauses.size(); i++)
    _solver->add_clause(clauses[i]);
  
  _solver->set_formula_transformer(ft);
  _solver->add_observer(_stat = new statistics_solver_observer(*_solver, std::cout, 10000));
  //_solver->add_observer(new logging_solver_observer(*_solver, std::cout));
  //_solver->add_observer(new logging_solver_observer(*_solver, std::cerr));

  //_solver->set_literal_selection_strategy(new interactive_literal_selection_strategy(*_solver));
  //_solver->set_polarity_selection_strategy(new interactive_polarity_selection_strategy(*_solver));
  //_solver->set_polarity_selection_strategy(new always_positive_polarity_selection_strategy(*_solver));
  //_solver->set_polarity_selection_strategy(new always_negative_polarity_selection_strategy(*_solver));
  //_solver->set_restart_strategy(new never_restart_strategy());
  //_solver->set_restart_strategy(new conflict_counting_restart_strategy(*_solver,100, 1.5));

  std::mt19937 decide_rng(s_config.random_seed());
  std::mt19937 polarity_rng(s_config.random_polarity_seed());
  
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

  const std::string pss = s_config.polarity_selection_strategy();
  if(pss == "saved")
    _solver->set_polarity_selection_strategy(new saved_polarity_selection_strategy(*_solver));
  else if(pss == "positive")
    _solver->set_polarity_selection_strategy(new always_positive_polarity_selection_strategy(*_solver));
  else if(pss == "negative")
    _solver->set_polarity_selection_strategy(new always_negative_polarity_selection_strategy(*_solver));
  else if(pss == "random")
    _solver->set_polarity_selection_strategy(new random_polarity_selection_strategy<>(*_solver, polarity_rng,
										      s_config.random_polarity_probability()));
  else
    {
      std::cerr << "CONFIGURATION ERROR: Unknown polarity selection strategy: " << pss << std::endl;
      exit(1);
    }
  
  bool randomize_simplex = s_config.randomize_simplex();
  unsigned simplex_shuffle_seed = s_config.simplex_shuffle_seed();
  const std::string buss = s_config.basic_unknown_selection_strategy();
  const std::string nbuss = s_config.non_basic_unknown_selection_strategy();
  const std::string as_alg_str = s_config.alldiff_sum_algorithm();
  alldiff_sum_algorithm as_algorithm = AS_NONE;
  if(as_alg_str == "partition")
    as_algorithm = AS_PARTITION;
  else if(as_alg_str == "beldiceanu")
    as_algorithm = AS_BELDICEANU;
  else if(as_alg_str == "fast")
    as_algorithm = AS_FAST;
  else if(as_alg_str == "none")
    as_algorithm = AS_NONE;
  else
    {
      std::cerr << "CONFIGURATION ERROR: Unknown alldiff-sum algorithm: " << as_alg_str << std::endl;
      exit(1);
    }


  bool predefined_csp_bounds = s_config.predefined_csp_bounds();
  int lower_predefined_csp_bound = s_config.lower_predefined_csp_bound();
  int upper_predefined_csp_bound = s_config.upper_predefined_csp_bound();

  unsigned quantifier_instantiation_count_limit = s_config.quantifier_instantiation_count_limit();
  unsigned quantifier_instantiation_term_size_limit = s_config.quantifier_instantiation_term_size_limit();

  _solver->set_quantifier_instantiation_count_limit(quantifier_instantiation_count_limit);
  _solver->set_quantifier_instantiation_term_size_limit(quantifier_instantiation_term_size_limit);

  
  /* Adding theories */
  symbol s = _smt_lib_api->get_logic_symbol();
  if(s == "QF_UF")
    {
      _solver->set_forget_strategy(new geometric_forget_strategy(*_solver));
      _solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*_solver));
      _solver->set_restart_strategy(new conflict_counting_restart_strategy(*_solver,100, 1.5));

      _solver->add_theory_solver(new clause_theory_solver(*_solver));
      _solver->add_theory_solver(new euf_theory_solver(*_solver));
    }
  else if(s == "UF")
    {
      _solver->set_forget_strategy(new geometric_forget_strategy(*_solver));
      _solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*_solver));
      _solver->set_restart_strategy(new conflict_counting_restart_strategy(*_solver,100, 1.5));
      
      _solver->add_theory_solver(new clause_theory_solver(*_solver));
      _solver->add_theory_solver(new euf_theory_solver(*_solver));
      //      _solver->add_quantifiers_processor(new e_matching_quantifiers_processor(*_solver));
      _solver->add_quantifiers_processor(new enumerative_quantifiers_processor(*_solver));
    }
  else if(s == "QF_LRA")
    {
      _solver->set_forget_strategy(new geometric_forget_strategy(*_solver));
      _solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*_solver));
      _solver->set_restart_strategy(new conflict_counting_restart_strategy(*_solver,100, 1.5));

      _solver->add_theory_solver(new clause_theory_solver(*_solver));
      _solver->add_theory_solver(new arithmetic_theory_solver(*_solver, buss, nbuss, 13, false, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_UFLRA")
    {
      _solver->set_forget_strategy(new geometric_forget_strategy(*_solver));
      _solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*_solver));
      _solver->set_restart_strategy(new conflict_counting_restart_strategy(*_solver,100, 1.5));

      _solver->add_theory_solver(new clause_theory_solver(*_solver));
      _solver->add_theory_solver(new euf_theory_solver(*_solver));
      _solver->add_theory_solver(new arithmetic_theory_solver(*_solver, buss, nbuss, 13, true, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_LIA")
    {
      _solver->set_forget_strategy(new geometric_forget_strategy(*_solver));
      _solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*_solver));
      _solver->set_restart_strategy(new conflict_counting_restart_strategy(*_solver,100, 1.5));

      _solver->add_theory_solver(new clause_theory_solver(*_solver));
      _solver->add_theory_solver(new arithmetic_theory_solver(*_solver, buss, nbuss, 13, false, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_UFLIA")
    {
      _solver->set_forget_strategy(new geometric_forget_strategy(*_solver));
      _solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*_solver));
      _solver->set_restart_strategy(new conflict_counting_restart_strategy(*_solver,100, 1.5));

      _solver->add_theory_solver(new clause_theory_solver(*_solver));
      _solver->add_theory_solver(new euf_theory_solver(*_solver));
      _solver->add_theory_solver(new arithmetic_theory_solver(*_solver, buss, nbuss, 13, true, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_LIRA")
    {
      _solver->set_forget_strategy(new geometric_forget_strategy(*_solver));
      _solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*_solver));
      _solver->set_restart_strategy(new conflict_counting_restart_strategy(*_solver,100, 1.5));

      _solver->add_theory_solver(new clause_theory_solver(*_solver));
      _solver->add_theory_solver(new arithmetic_theory_solver(*_solver, buss, nbuss, 13, false, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_UFLIRA" || s == "ALL")
    {
      _solver->set_forget_strategy(new geometric_forget_strategy(*_solver));
      _solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*_solver));
      _solver->set_restart_strategy(new conflict_counting_restart_strategy(*_solver,100, 1.5));

      _solver->add_theory_solver(new clause_theory_solver(*_solver));
      _solver->add_theory_solver(new euf_theory_solver(*_solver));
      _solver->add_theory_solver(new arithmetic_theory_solver(*_solver, buss, nbuss, 13, true, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_RDL")
    {
      _solver->set_forget_strategy(new geometric_forget_strategy(*_solver));
      _solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*_solver));
      _solver->set_restart_strategy(new conflict_counting_restart_strategy(*_solver,100, 1.5));

      _solver->add_theory_solver(new clause_theory_solver(*_solver));
      _solver->add_theory_solver(new arithmetic_theory_solver(*_solver, buss, nbuss, 13, false, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_IDL")
    {
      _solver->set_forget_strategy(new geometric_forget_strategy(*_solver));
      _solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*_solver));
      _solver->set_restart_strategy(new conflict_counting_restart_strategy(*_solver,100, 1.5));

      _solver->add_theory_solver(new clause_theory_solver(*_solver));
      _solver->add_theory_solver(new arithmetic_theory_solver(*_solver, buss, nbuss, 13, false, 0, randomize_simplex, simplex_shuffle_seed));
    }
  else if(s == "QF_CSP")
    {
      //_solver->set_forget_strategy(new geometric_forget_strategy(*_solver, 1000U, 1.1));
      //_solver->set_forget_selection_strategy(new clause_activity_forget_selection_strategy(*_solver));
      _solver->set_forget_strategy(new never_forget_strategy());
      
      _solver->set_restart_strategy(new conflict_counting_restart_strategy(*_solver,100, 1.5));

      _solver->add_theory_solver(new clause_theory_solver(*_solver));
      csp_theory_solver * csp = new csp_theory_solver(*_solver, as_algorithm,
					predefined_csp_bounds, lower_predefined_csp_bound, upper_predefined_csp_bound);
      _solver->add_theory_solver(csp);
      csp->_sum_common_data._upper_bounds_only = true;
      //csp->set_variable_order_strategy(new simple_csp_variable_order_strategy(csp));
      csp->set_variable_order_strategy(new most_constrained_csp_variable_order_strategy(csp));
      //csp->set_value_order_strategy(new simple_csp_value_order_strategy(csp));
      csp->set_value_order_strategy(new most_preferred_csp_value_order_strategy(csp));
      //_solver->add_theory_solver(new arithmetic_theory_solver(*_solver, buss, nbuss, 0, false, 0, randomize_simplex, simplex_shuffle_seed));	
    }
  else
    {
      delete _solver;
      throw command_unsupported_exception();
    }

  check_sat_response csr = _solver->solve();

  if(_smt_lib_api->get_info(keyword::STATUS))
    {
      check_sat_response known_csr; 
      _smt_lib_api->get_info(keyword::STATUS, known_csr);
      if(known_csr != CSR_UNKNOWN && csr != CSR_UNKNOWN && known_csr != csr)
	{
	  std::cerr << "ERROR FATAL!! WRONG ANSWER!!" << std::endl;
	}
    }
    
  if(csr == CSR_SAT && !_solver->verify_assignment())
    {
      std::cerr << "VERIFICATION OF ASSIGNMENT FAILED!!" << std::endl;
    }
       
  return csr;
}

check_sat_response argosmt_solver_interface::check_sat() 
{
  _cl.start();     
  _smt_lib_api->push();
  const expression_vector & assertions = _smt_lib_api->assertions();
  std::vector<clause *> clauses;

  formula_transformer * ft = new formula_transformer(_smt_lib_api->get_expression_factory());
  ft->set_produce_proofs(_smt_lib_api->produce_proofs());
  
  
  // CNF transformation
  for(unsigned i = 0; i < assertions.size(); i++)
    {
      ft->top_level_cnf_transformation(assertions[i]->infer_sorts()->eliminate_let_binders(), clauses);
    }

  check_sat_response csr = start_solver(clauses, ft);
  _stat->report();
  _stat->all_theories_report();
  
  // write :all-statistics info
  attribute_set attr;
  _stat->get_statistics(attr);
    
  double elapsed = _cl.seconds_elapsed();
    
  attr.insert(attribute(":solving-time", elapsed));
  _smt_lib_api->set_info(keyword::ALL_STATISTICS, attr);
  _smt_lib_api->pop();
  return csr;
}
  
expression_vector argosmt_solver_interface::get_value(const expression_vector & exps) 
{
  _solver->get_model(exps);
  expression_vector result;
  for(unsigned i = 0; i < exps.size(); i++)
    {
      if(exps[i]->is_assigned())
	result.push_back(exps[i]);
    }
  return result;
}
