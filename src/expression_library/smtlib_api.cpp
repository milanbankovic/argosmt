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

#include "smtlib_api.hpp"

smtlib_api::smtlib_api()
  :_map(HASH_TABLE_SIZE),
   _signature(0),
   _exp_factory(0),
   _logic(0),
   _logic_symbol(""),
   _solver_interface(0),
   _options(HASH_TABLE_SIZE),
   _cout(0),
   _cerr(0),
   _infos(HASH_TABLE_SIZE),
   _response(CSR_UNKNOWN),
   _expected_response(CSR_UNKNOWN)
{

  register_logic("ALL", new qfuflira_logic_declaration());
  
  register_logic("AUFLIA", new auflia_logic_declaration());
  register_logic("AUFLIRA", new auflira_logic_declaration());
  register_logic("AUFNIRA", new aufnira_logic_declaration());
  register_logic("LRA", new lra_logic_declaration());
  register_logic("QF_ABV", new qfabv_logic_declaration());
  register_logic("QF_AUFBV", new qfaufbv_logic_declaration());
  register_logic("QF_AUFLIA", new qfauflia_logic_declaration());
  register_logic("QF_AX", new qfax_logic_declaration());
  register_logic("QF_BV", new qfbv_logic_declaration());
  register_logic("QF_IDL", new qfidl_logic_declaration());
  register_logic("QF_LIA", new qflia_logic_declaration());
  register_logic("QF_LRA", new qflra_logic_declaration());
  register_logic("QF_NIA", new qfnia_logic_declaration());
  register_logic("QF_NRA", new qfnra_logic_declaration());
  register_logic("QF_RDL", new qfrdl_logic_declaration());
  register_logic("UF", new uf_logic_declaration());
  register_logic("QF_UF", new qfuf_logic_declaration());
  register_logic("QF_UFBV", new qfufbv_logic_declaration());
  register_logic("QF_UFIDL", new qfufidl_logic_declaration());
  register_logic("QF_UFLIA", new qfuflia_logic_declaration());
  register_logic("QF_UFLRA", new qfuflra_logic_declaration());
  register_logic("QF_UFNRA", new qfufnra_logic_declaration());
  register_logic("UFLRA", new uflra_logic_declaration());
  register_logic("UFNIA", new ufnia_logic_declaration());

  register_logic("QF_UFLIRA", new qfuflira_logic_declaration());
  register_logic("QF_LIRA", new qflira_logic_declaration());
  register_logic("QF_CSP", new qfcsp_logic_declaration());
  register_logic("QF_LIACSP", new qfliacsp_logic_declaration());

  set_option(keyword::PRINT_SUCCESS, true);
  set_option(keyword::EXPAND_DEFINITIONS, false);
  set_option(keyword::PRODUCE_ASSERTIONS, false);
  set_option(keyword::PRODUCE_PROOFS, false);
  set_option(keyword::PRODUCE_UNSAT_CORES, false);
  set_option(keyword::PRODUCE_MODELS, false);
  set_option(keyword::PRODUCE_ASSIGNMENTS, false);
  set_option(keyword::GLOBAL_DECLARATIONS, false);
  set_option(keyword::SYNTAX_CHECKING, true);
  set_option(keyword::LOGIC_SYNTAX_CHECKING, true);
  set_option(keyword::REGULAR_OUTPUT_CHANNEL, std::string("stdout"));
  set_option(keyword::DIAGNOSTIC_OUTPUT_CHANNEL, std::string("stderr"));
  set_option(keyword::RANDOM_SEED, 0UL);
  set_option(keyword::VERBOSITY, 0UL);

  set_info(keyword::ERROR_BEHAVIOR, EB_IMMEDIATE_EXIT);
  set_info(keyword::REASON_UNKNOWN, RU_NOT_UNKNOWN);
  set_info(keyword::STATUS, CSR_UNKNOWN);
}

void smtlib_api::set_logic(const symbol & name)
{
  _solver_interface->reset_interface();
  
  _assertions.clear();
  _assertion_levels.clear();

  _all_labels.clear();
  _all_labels_levels.clear();
  
  _assertion_labels.clear();
  _assertion_labels_levels.clear();

  delete _exp_factory;
  delete _signature;

  _expansion_signatures.clear();

  _response = CSR_UNKNOWN;
  _expected_response = CSR_UNKNOWN;

  set_info(keyword::STATUS, CSR_UNKNOWN);
  
  logics_map::iterator it = _map.find(name);
  if(it == _map.end())
    throw bad_logic_exception("set-logic", name);
  else
    {
      _logic = it->second;
      _logic_symbol = name;
    }

  _assertion_levels.push_back(0);
  _all_labels_levels.push_back(0);
  _assertion_labels_levels.push_back(0);
 
  if(!_syntax_checking)
    cerr() << "warning!! :syntax-checking is set to false" << std::endl;
  _signature = _logic->create_signature(_syntax_checking);
  _exp_factory = new expression_factory(_signature);
  signature * new_expansion = 
    new signature(signature::T_EXPANSION_SIGNATURE, _signature);
  _expansion_signatures.push_back(new_expansion);
}

bool set_bool_value(attribute_set & options, 
		    const attribute & attr, bool & opt_var)
{
  static const std::string str_true("true");
  static const std::string str_false("false");

  const keyword & option = attr.get_keyword();
  attribute_value * value = attr.get_value();

  bool_attribute_value * bool_value;
  string_attribute_value * string_value;
  symbol_attribute_value * s_value;
  
  if((bool_value = dynamic_cast<bool_attribute_value *>(value)) != 0)
    {
      opt_var = bool_value->get_value();
    }
  else if((s_value = dynamic_cast<symbol_attribute_value *>(value)) != 0)
    {
      if(s_value->get_value() == str_true)
	opt_var = true;	  	
      else if(s_value->get_value() == str_false)
	opt_var = false;
      else 
	return false;
    }
  else if((string_value = dynamic_cast<string_attribute_value *>(value)) != 0)
    {
      if(string_value->get_value() == str_true)
	opt_var = true;
      else if(string_value->get_value() == str_false)
	opt_var = false;
      else 
	return false;
    }
  else
    return false;

  attribute_set::iterator it = options.find(option);
  if(it != options.end())
    options.erase(option);
  options.insert(attribute(option, opt_var));
  return true;
}

bool set_numeral_value(attribute_set & options,
		       const attribute & attr, unsigned long & opt_var)
{
  const keyword & option = attr.get_keyword();
  attribute_value * value = attr.get_value();

  ul_attribute_value * ul_value;
  int_attribute_value * int_value;
  special_constant_attribute_value * sc_value;
  
  if((ul_value = dynamic_cast<ul_attribute_value *>(value)) != 0)
    {
      opt_var = ul_value->get_value();
    }
  else if((int_value = dynamic_cast<int_attribute_value *>(value)) != 0)
    {
      opt_var = (unsigned long) int_value->get_value();
    }
  else if((sc_value = dynamic_cast<special_constant_attribute_value *>(value)) != 0)
    {
      if(sc_value->get_value().get_type() == special_constant::T_NUMERAL)
	opt_var = sc_value->get_value().get_ul_value();
      else
	return false;
    }
  else
    return false;

  attribute_set::iterator it = options.find(option);
  if(it != options.end())
    options.erase(option);
  options.insert(attribute(option, opt_var));
  return true;  
}

bool set_stream_value(attribute_set & options, 
		      const attribute & attr, std::ostream * & ostr)
{
  const keyword & option = attr.get_keyword();
  attribute_value * value = attr.get_value();
  static const std::string str_stdout("stdout");
  static const std::string str_stderr("stderr");

  std::string output;

  string_attribute_value * string_value;
  special_constant_attribute_value * sc_value;

  if((string_value = dynamic_cast<string_attribute_value *>(value)) != 0)
    {
      output = string_value->get_value();
      
      if(ostr != &std::cout && ostr != &std::cerr)
	delete ostr;
      
      if(output == str_stdout)
	ostr = &std::cout;
      else if(output == str_stderr)
	ostr = &std::cerr;
      else
	ostr = new std::ofstream(output.c_str());
    }
  else if((sc_value = dynamic_cast<special_constant_attribute_value *>(value)) != 0)
    {
      if(sc_value->get_value().get_type() != special_constant::T_STRING_LITERAL)
	return false;
      else
	{
	  const std::string & temp = sc_value->get_value().get_value();
	  output = temp.substr(1, temp.size() - 2);
	}

      if(ostr != &std::cout && ostr != &std::cerr)
	delete ostr;
      
      if(output == str_stdout)
	ostr = &std::cout;
      else if(output == str_stderr)
	ostr = &std::cerr;
      else
	ostr = new std::ofstream(output.c_str());
    }
  else
    return false;

  attribute_set::iterator it = options.find(option);
  if(it != options.end())
    options.erase(option);
  options.insert(attribute(option, std::move(output)));
  return true;
}
		    

void smtlib_api::set_option(const attribute & attr)
{  
  const keyword & option = attr.get_keyword();
  
  if(option == keyword::PRINT_SUCCESS)
    {   
      if(!set_bool_value(_options, attr, _print_success))
	throw command_error_exception("set-option: :print-success option "
				      "should have a boolean value");
    }
  else if(option == keyword::EXPAND_DEFINITIONS)
    {    
      if(!set_bool_value(_options, attr, _expand_definitions))
	throw command_error_exception("set-option: :expand-definitions option "
				      "should have a boolean value");
    }
  else if(option == keyword::INTERACTIVE_MODE)
    {
      
      if(!set_bool_value(_options,
			 attribute(keyword::PRODUCE_ASSERTIONS,
				   attr.get_value()->clone()),
			 _produce_assertions))
	throw command_error_exception("set-option: :produce-assertions "
				      "(a.k.a. :interactive-mode) option "
				      "should have a boolean value");
    }
  else if(option == keyword::PRODUCE_ASSERTIONS)
    {
      if(!set_bool_value(_options, attr, _produce_assertions))
	throw command_error_exception("set-option: :produce-assertions option "
				      "should have a boolean value");
    }
  else if(option == keyword::PRODUCE_PROOFS)
    {
      if(!set_bool_value(_options, attr, _produce_proofs))
	throw command_error_exception("set-option: :produce-proofs option "
				      "should have a boolean value");
    }
  else if(option == keyword::PRODUCE_UNSAT_CORES)
    {
      if(!set_bool_value(_options, attr, _produce_unsat_cores))
	throw command_error_exception("set-option: :produce-unsat-cores option "
				      "should have a boolean value");
    }
  else if(option == keyword::PRODUCE_MODELS)
    {
      if(!set_bool_value(_options, attr, _produce_models))
	throw command_error_exception("set-option: :produce-models option "
				      "should have a boolean value");
    }
  else if(option == keyword::PRODUCE_ASSIGNMENTS)
    {
      if(!set_bool_value(_options, attr, _produce_assignments))
	throw command_error_exception("set-option: :produce-assignments option "
				      "should have a boolean value");
    }
  else if(option == keyword::GLOBAL_DECLARATIONS)
    {
      if(!set_bool_value(_options, attr, _global_declarations))
	throw command_error_exception("set-option: :global-declarations option "
				      "should have a boolean value");
    }
  else if(option == keyword::SYNTAX_CHECKING)
    {
      if(!set_bool_value(_options, attr, _syntax_checking))
	throw command_error_exception("set-option: :syntax-checking option "
				      "should have a boolean value");
    }
  else if(option == keyword::LOGIC_SYNTAX_CHECKING)
    {
      if(!set_bool_value(_options, attr, _logic_syntax_checking))
	throw command_error_exception("set-option: :logic-syntax-checking option "
				      "should have a boolean value");
    }
  else if(option == keyword::REGULAR_OUTPUT_CHANNEL)
    {
      if(!set_stream_value(_options, attr, _cout))
	throw command_error_exception("set-option: :regular-output-channel "
				      "option should have a string value");
    }
  else if(option == keyword::DIAGNOSTIC_OUTPUT_CHANNEL)
    {
      if(!set_stream_value(_options, attr, _cerr))
	throw command_error_exception("set-option: :diagonostic-output-channel " 
				      "option should have a string value");
    }
  else if(option == keyword::RANDOM_SEED)
    {
      if(!set_numeral_value(_options, attr, _random_seed))
	throw command_error_exception("set-option: :random-seed option "
				      "should have a numeral value");

    }
  else if(option == keyword::VERBOSITY)
    {
      if(!set_numeral_value(_options, attr, _verbosity))
	throw command_error_exception("set-option: :verbosity option "
				      "should have a numeral value");
    }
  else /* user defined options */
    {
      attribute_set::iterator it = _options.find(option);
      if(it != _options.end())
	_options.erase(option); 
      _options.insert(attr);
    }
}

bool set_eb_value(attribute_set & options,
		  const attribute & attr, error_behavior & opt_var)
{
  const keyword & option = attr.get_keyword();
  attribute_value * value = attr.get_value();
  static const std::string str_imm_exit("immediate-exit");
  static const std::string str_cont_exec("continued-execution");
  

  error_behavior_attribute_value * eb_value;
  symbol_attribute_value * s_value;
  string_attribute_value * string_value;

  if((eb_value = dynamic_cast<error_behavior_attribute_value *>(value)) != 0)
    {
      opt_var = eb_value->get_value();
    }
  else if((s_value = dynamic_cast<symbol_attribute_value *>(value)) != 0)
    {
      if(s_value->get_value() == str_imm_exit)
	opt_var = EB_IMMEDIATE_EXIT;
      else if(s_value->get_value() == str_cont_exec)
	opt_var = EB_CONTINUED_EXECUTION;
      else
	return false;
    }
  else if((string_value = dynamic_cast<string_attribute_value *>(value)) != 0)
    {
      if(string_value->get_value() == str_imm_exit)
	opt_var = EB_IMMEDIATE_EXIT;
      else if(string_value->get_value() == str_cont_exec)
	opt_var = EB_CONTINUED_EXECUTION;
      else
	return false;
    }
  else
    return false;

  attribute_set::iterator it = options.find(option);
  if(it != options.end())
    options.erase(option);
  options.insert(attribute(option, opt_var));
  return true;  
}


bool set_ru_value(attribute_set & options, 
		  const attribute & attr, reason_unknown & opt_var)
{
  const keyword & option = attr.get_keyword();
  attribute_value * value = attr.get_value();
  static const std::string str_memout("memout");
  static const std::string str_incomplete("incomplete");
  static const std::string str_not_unknown("not-unknown");

  reason_unknown_attribute_value * ru_value;
  symbol_attribute_value * s_value;
  string_attribute_value * string_value;

  if((ru_value = dynamic_cast<reason_unknown_attribute_value *>(value)) != 0)
    {
      opt_var = ru_value->get_value();
    }
  else if((s_value = dynamic_cast<symbol_attribute_value *>(value)) != 0)
    {
      if(s_value->get_value() == str_memout)
	opt_var = RU_MEMOUT;
      else if(s_value->get_value() == str_incomplete)
	opt_var = RU_INCOMPLETE;
      else if(s_value->get_value() == str_not_unknown)
	opt_var = RU_NOT_UNKNOWN;
      else
	return false;
    }
  else if((string_value = dynamic_cast<string_attribute_value *>(value)) != 0)
    {
      if(string_value->get_value() == str_memout)
	opt_var = RU_MEMOUT;
      else if(string_value->get_value() == str_incomplete)
	opt_var = RU_INCOMPLETE;
      else if(string_value->get_value() == str_not_unknown)
	opt_var = RU_NOT_UNKNOWN;
      else
	return false;
    }
  else
    return false;

  attribute_set::iterator it = options.find(option);
  if(it != options.end())
    options.erase(option);
  options.insert(attribute(option, opt_var));
  return true;  
}

bool set_csr_value(attribute_set & options,
		   const attribute & attr, check_sat_response & opt_var)
{
  const keyword & option = attr.get_keyword();
  attribute_value * value = attr.get_value();
  static const std::string str_sat("sat");
  static const std::string str_unsat("unsat");
  static const std::string str_unknown("unknown");
  

  check_sat_response_attribute_value * csr_value;
  symbol_attribute_value * s_value;
  string_attribute_value * string_value;

  if((csr_value = dynamic_cast<check_sat_response_attribute_value *>(value)) != 0)
    {
      opt_var = csr_value->get_value();
    }
  else if((s_value = dynamic_cast<symbol_attribute_value *>(value)) != 0)
    {
      if(s_value->get_value() == str_sat)
	opt_var = CSR_SAT;
      else if(s_value->get_value() == str_unsat)
	opt_var = CSR_UNSAT;
      else if(s_value->get_value() == str_unknown)
	opt_var = CSR_UNKNOWN;
      else
	return false;
    }
  else if((string_value = dynamic_cast<string_attribute_value *>(value)) != 0)
    {
      if(string_value->get_value() == str_sat)
	opt_var = CSR_SAT;
      else if(string_value->get_value() == str_unsat)
	opt_var = CSR_UNSAT;
      else if(string_value->get_value() == str_unknown)
	opt_var = CSR_UNKNOWN;
      else
	return false;
    }
  else
    return false;

  attribute_set::iterator it = options.find(option);
  if(it != options.end())
    options.erase(option);
  options.insert(attribute(option, opt_var));
  return true;  
}

void smtlib_api::set_info(const attribute & attr)
{
  const keyword & info = attr.get_keyword();
  
  if(info == keyword::ERROR_BEHAVIOR)
    {
      if(!set_eb_value(_infos, attr, _error_behavior))
	throw command_error_exception
	  ("set-info: :error-behavior info should have value "
	   "immediate-exit or continued-execution");
    }
  else if(info == keyword::REASON_UNKNOWN)
    {
      if(!set_ru_value(_infos, attr, _reason_unknown))
	throw command_error_exception
	  ("set-info: :reason-unknown info should have value "
	   "memout or incomplete");
    }
  else if(info == keyword::STATUS)
    {
      if(!set_csr_value(_infos, attr, _expected_response))
	throw command_error_exception
	  ("set-info: :status info should have value sat, unsat or unknown");
    }
  else /* user defined infos */
    {
      attribute_set::iterator it = _infos.find(info);
      if(it != _infos.end())
	{
	  _infos.erase(info);
	}
      _infos.insert(attr);
    }
}


void smtlib_api::push()
{ 
  if(!_logic)
    throw logic_not_set_exception("push");

  _assertion_levels.push_back(_assertions.size());
  _all_labels_levels.push_back(_all_labels.size());
  _assertion_labels_levels.push_back(_assertion_labels.size());

  signature * last_expansion = _expansion_signatures.back();
  signature * new_expansion = 
    new signature(signature::T_EXPANSION_SIGNATURE, last_expansion);
  _expansion_signatures.push_back(new_expansion);  
}

void smtlib_api::pop()
{
  if(!_logic)
    throw logic_not_set_exception("pop");

  if(_assertion_levels.size() <= 1)
    throw command_error_exception("pop: cannot pop the last assertion set");
  
  _assertions.resize(_assertion_levels.back());
  _assertion_levels.pop_back();
  _all_labels.resize(_all_labels_levels.back());
  _all_labels_levels.pop_back();
  _assertion_labels.resize(_assertion_labels_levels.back());
  _assertion_labels_levels.pop_back();

  delete _expansion_signatures.back();
  _expansion_signatures.pop_back();
  assert(_expansion_signatures.back()->get_expansion_signature() == 0);
}
  
expression_vector smtlib_api::get_value(const expression_vector & exps) const
{
  if(!_logic)
    throw logic_not_set_exception("get-value");

  if(!_solver_interface)
    throw solver_interface_not_set_exception("get-value");
  
  if(!_produce_models)
    throw command_error_exception("get-value: option :produce-models set to false");
  
  if(_response == CSR_UNSAT)
    throw command_error_exception("get-value: unsat");
  
  return _solver_interface->get_value(exps);   
}


void smtlib_api::
define_function(const function_symbol & sm,
		const sorted_variable_vector & vars,
		const sort & sr,
		const expression & exp)
{
  define_function(sm, sorted_variable_vector(vars), sr, exp);
}

void smtlib_api::
define_function(const function_symbol & sm,
		sorted_variable_vector && vars,
		const sort & sr,
		const expression & exp)
{
  if(!_logic)
    throw logic_not_set_exception("define-fun");

  if(_syntax_checking && exp->get_sort() != sr)
    throw bad_sort_exception("define-fun", exp->get_sort());

  rank rn;
  for(sorted_variable_vector::const_iterator it = vars.begin(), 
	it_end = vars.end(); it != it_end; ++it)
    {
      rn.push_back(it->get_sort());
    }
  rn.push_back(sr);
  
  attribute_set attr(HASH_TABLE_SIZE);
  attr.insert(attribute(keyword::DEFINITION, function_definition(std::move(vars), exp)));

  if(_global_declarations)
    _expansion_signatures[0]->add_function_symbol(sm, std::move(rn), std::move(attr));
  else
    _expansion_signatures.back()->add_function_symbol(sm, std::move(rn), std::move(attr));  
}


proof smtlib_api::get_proof() const
{
  if(!_logic)
    throw logic_not_set_exception("get-proof");

  if(!_solver_interface)
    throw solver_interface_not_set_exception("get-proof");
  
  if(!_produce_proofs)
    throw command_error_exception("get-proof: option :produce-proofs set to false");
  
  if(_response != CSR_UNSAT)
    throw command_error_exception("get-proof: not unsat");

  proof pr = _solver_interface->get_proof();
  if(!pr->check_proof())
    throw invalid_proof_exception();
    
  return pr;
}

expression_vector smtlib_api::get_unsat_core() const
{
  if(!_logic)
    throw logic_not_set_exception("get-unsat-core");

  if(!_solver_interface)
    throw solver_interface_not_set_exception("get-unsat-core");
  
  if(!_produce_unsat_cores)
    throw command_error_exception("get-unsat-core: option :produce-unsat-cores set to false");
  
  if(_response != CSR_UNSAT)
    throw command_error_exception("get-unsat-core: not unsat");
  
  return _solver_interface->get_unsat_core();  
}

expression_vector smtlib_api::get_assignment() const
{
  if(!_logic)
    throw logic_not_set_exception("get-assignment");

  if(!_solver_interface)
    throw solver_interface_not_set_exception("get-assignment");
  
  if(!_produce_assignments)
    throw command_error_exception("get-assignment: option :produce-assignments set to false");
  
  if(_response == CSR_UNSAT)
    throw command_error_exception("get-assignment: unsat");
  
  return _solver_interface->get_assignment();  
}


void smtlib_api::check_label(const expression & exp)
{
  symbol s("");
  if(check_named(exp, s))
    {
      function_symbol f(s.get_name());
      expression infered_exp = exp->infer_sorts();
      define_function(f, EMPTY_SORTED_VARIABLE_VECTOR, 
		      infered_exp->get_sort(), infered_exp);
	
      expression label = _exp_factory->
	create_expression(f, infered_exp->get_sort());
      _all_labels.push_back(label);
    }
}


void smtlib_api::check_assertion_label(const expression & exp)
{
  symbol s("");
  if(check_named(exp, s))
    {
      function_symbol f(s.get_name());
      expression infered_exp = exp->infer_sorts();
      expression label = _exp_factory->
	create_expression(f, infered_exp->get_sort());
      _assertion_labels.push_back(label);
    }
}


void smtlib_api::assert_expression(const expression & exp)
{
  if(!_logic)
    throw logic_not_set_exception("assert");

  if(_syntax_checking && !exp->get_sort()->is_equivalent(get_sort_factory()->BOOL_SORT()))
    throw bad_sort_exception("assert: non-boolean expression asserted", 
			     exp->get_sort());
    
  /*  if(_syntax_checking && 
     _logic_syntax_checking && 
     !_logic->check_expression(exp))
    throw bad_expression_exception("assert: logic restrictions violated", exp);
  */
  _assertions.push_back(exp);
    
  try 
    {
      check_assertion_label(exp);
    } 
  catch(bad_function_symbol_exception & exception)
    {
      check_label(exp);
      check_assertion_label(exp);
    }
}


void smtlib_api::reset_assertions()
{
  unsigned n = _assertion_levels.size();
  _assertions.clear();
  _assertion_levels.clear();
  _assertion_levels.push_back(0);
  
  _all_labels.clear();
  _all_labels_levels.clear();
  _all_labels_levels.push_back(0);
  
  _assertion_labels.clear();
  _assertion_labels_levels.clear();
  _assertion_labels_levels.push_back(0);


  while(--n)
    {
      delete _expansion_signatures.back();
      _expansion_signatures.pop_back();
      assert(_expansion_signatures.back()->get_expansion_signature() == 0);
    }
  
  if(!_global_declarations)
    {
      delete _expansion_signatures.back();
      _expansion_signatures.pop_back();
      signature * new_expansion = 
	new signature(signature::T_EXPANSION_SIGNATURE, _signature);
      _expansion_signatures.push_back(new_expansion);
    }
}

void smtlib_api::reset()
{
  _solver_interface->reset_interface();
  
  _assertions.clear();
  _assertion_levels.clear();

  _all_labels.clear();
  _all_labels_levels.clear();
  
  _assertion_labels.clear();
  _assertion_labels_levels.clear();

  delete _exp_factory;
  delete _signature;

  _exp_factory = nullptr;
  _signature = nullptr;
  _logic = nullptr;
  _expansion_signatures.clear();
  _logic_symbol = symbol("");
  _response = CSR_UNKNOWN;
  _expected_response = CSR_UNKNOWN;

  _options.clear();
  set_option(keyword::PRINT_SUCCESS, true);
  set_option(keyword::EXPAND_DEFINITIONS, false);
  set_option(keyword::PRODUCE_ASSERTIONS, false);
  set_option(keyword::PRODUCE_PROOFS, false);
  set_option(keyword::PRODUCE_UNSAT_CORES, false);
  set_option(keyword::PRODUCE_MODELS, false);
  set_option(keyword::PRODUCE_ASSIGNMENTS, false);
  set_option(keyword::GLOBAL_DECLARATIONS, false);
  set_option(keyword::SYNTAX_CHECKING, true);
  set_option(keyword::LOGIC_SYNTAX_CHECKING, true);
  set_option(keyword::REGULAR_OUTPUT_CHANNEL, std::string("stdout"));
  set_option(keyword::DIAGNOSTIC_OUTPUT_CHANNEL, std::string("stderr"));
  set_option(keyword::RANDOM_SEED, 0UL);
  set_option(keyword::VERBOSITY, 0UL);

  _infos.clear();
  set_info(keyword::ERROR_BEHAVIOR, EB_IMMEDIATE_EXIT);
  set_info(keyword::REASON_UNKNOWN, RU_NOT_UNKNOWN);
  set_info(keyword::STATUS, CSR_UNKNOWN);

  _solver_interface->init_interface();
}

void smtlib_api::parse_input(std::istream & istr)
{
  yyrestart(0);
  yy_istream = &istr;
  yy_api = this;
  
  while(true)
    {
      try {
	yyparse();
	return;
      }
      catch(const command_unsupported_exception & e)
	{
	  cout() << e.get_description() << std::endl;
	}
      catch(const base_exception & e)
	{
	  cout() << "(error \"" << e.get_description() << "\")" << std::endl;
	  if(_error_behavior == EB_IMMEDIATE_EXIT)
	    {
	      exit(1);
	    }
	  active_error = true;
	}
    }
}


smtlib_api::~smtlib_api ()
{
  _assertions.clear();
  _all_labels.clear();
  _assertion_labels.clear();

  delete _exp_factory;
  delete _signature;
  
  for(logics_map::iterator it = _map.begin(), it_end = _map.end();
      it != it_end; ++it)
    {
      delete it->second;
    }
}


