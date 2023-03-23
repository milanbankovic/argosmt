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

#ifndef _SMTLIB_API_H
#define _SMTLIB_API_H

#include <cstdlib>
#include <typeinfo>

#include "logic_declaration.hpp"

extern int yylex();
extern int yyparse();
extern void yyrestart(FILE *);

extern bool active_error;

/** Type of exception thrown when logic is not set. */
class logic_not_set_exception : public base_exception {
public:
  /** Constructor. */
  logic_not_set_exception(const std::string & message)
    :base_exception(message + std::string(": logic not set"))
  {}
};

/** Type of exception thrown when non-existing logic is set. */
class bad_logic_exception : public base_exception {
private:
  symbol _logic_name;
public:
  /** Constructor. */
  bad_logic_exception(const std::string & message, 
		      const symbol & logic_name)
    :base_exception(message + std::string(": bad logic: ") + logic_name.to_string()),
     _logic_name(logic_name)
  {}

  /** Gets the name of the (non-existing) logic. */
  const symbol & get_logic_name() const
  {
    return _logic_name;
  }
};

/** Type of exception thrown when solver_interface instance is not assigned
    to the API class. */
class solver_interface_not_set_exception : public base_exception {
public:
  /** Constructor. */
  solver_interface_not_set_exception(const std::string & message)
    :base_exception(message + std::string(": solver interface not set"))
  {}
};

/** Type of exception thrown when script commands encounters an error. */
class command_error_exception : public base_exception {
public:
  /** Constructor. */
  command_error_exception(const std::string & message)
    :base_exception(message)
  {}
};

/** Type of exception thrown when the solver does not support some 
    feature. */
class command_unsupported_exception : public base_exception {
public:
  /** Constructor. */
  command_unsupported_exception()
    :base_exception("unsupported")
  {}
};

/** Type of exception thrown when script has a syntax error. */
class syntax_error_exception : public base_exception {
public:
  /** Constructor. */
  syntax_error_exception()
    :base_exception("syntax error")
  {}
};

/** Type of exception thrown when the generated proof of
    unsatisfiability does not pass the validity check */
class invalid_proof_exception : public base_exception {
public:
  /** Constructor */
  invalid_proof_exception()
    :base_exception("proof not valid")
  {}
};


/** Represents an abstract proof of unsatisfiability provided by the 
    solver. */
class proof_node {
public:
  /** The method should print the proof to the output stream. */
  virtual void print_proof(std::ostream & ostr) = 0;

  /** This method should return true iff the proof is correct (useful for
      debugging, the real check should be done by an external tool) */
  virtual bool check_proof() = 0;
  
  /** Virtual destructor. */
  virtual ~proof_node () {}
};

using proof = std::shared_ptr<proof_node>;

/** Represents values for error behavior. */
enum error_behavior 
  { 
    /** immediate-exit */
    EB_IMMEDIATE_EXIT,
    /** continued-execution */
    EB_CONTINUED_EXECUTION 
  };

inline std::ostream & operator << (std::ostream & ostr, error_behavior eb)
{
  switch(eb)
    {
    case EB_IMMEDIATE_EXIT:
      ostr << "immediate-exit";
      break;
    case EB_CONTINUED_EXECUTION:
      ostr << "continued-execution";
      break;
    }
  return ostr;
}

/** Represents values for check-sat response. */
enum check_sat_response 
  { 
    /** unsat response */
    CSR_UNSAT,
    /** sat response */
    CSR_SAT, 
    /** unknown response */
    CSR_UNKNOWN 
  };

inline std::ostream & operator << (std::ostream & ostr, check_sat_response csr)
{
  switch(csr)
    {
    case CSR_SAT:
      ostr << "sat";
      break;
    case CSR_UNSAT:
      ostr << "unsat";
      break;
    case CSR_UNKNOWN:
      ostr << "unknown";
      break;
    }
  return ostr;
}

/** Represents values for reason of unknown response. */
enum reason_unknown 
  { 
    /** not-unknown */
    RU_NOT_UNKNOWN,
    /** memout */
    RU_MEMOUT, 
    /** incomplete */
    RU_INCOMPLETE 
  };

inline std::ostream & operator << (std::ostream & ostr, reason_unknown ru)
{
  switch(ru)
    {
    case RU_NOT_UNKNOWN:
      ostr << "not-unknown";
      break;
    case RU_MEMOUT:
      ostr << "memout";
      break;
    case RU_INCOMPLETE:
      ostr << "incomplete";
      break;
    }
  return ostr;
}

typedef standard_attribute_value<bool> bool_attribute_value;
typedef standard_attribute_value<std::string> string_attribute_value;
typedef standard_attribute_value<unsigned long> ul_attribute_value;
typedef standard_attribute_value<int> int_attribute_value;
typedef standard_attribute_value<symbol> symbol_attribute_value;
typedef standard_attribute_value<special_constant> special_constant_attribute_value;
typedef standard_attribute_value<error_behavior> error_behavior_attribute_value;
typedef standard_attribute_value<reason_unknown> reason_unknown_attribute_value;
typedef standard_attribute_value<check_sat_response> check_sat_response_attribute_value;
typedef standard_attribute_value<attribute_set> attribute_set_attribute_value;

class smtlib_api;

/** This class represents the abstract interface
    to the SMT solver. The user should derive a subclass of this class
    that either implements the SMT solver itself, or implements an adapter 
    to the SMT solver's code. */
class solver_interface {
protected:
  /** Holds the pointer to the smtlib_api class instance that this interface
      is assigned to. This pointer can be used to access the data of the
      API class (assertion set stack, defined labels, options, infos, and 
      so on). Alternative way is to use get_smtlib_api() method. */
  smtlib_api * _smt_lib_api;
public:
  /** Constructor. */
  solver_interface()
    :_smt_lib_api(0)
  {}

  /** Sets smtlib_api instance to use. This method is called automatically
      when the solver_interface instance is assigned to an smtlib_api instance
      (i.e. when set_solver_interface() method of the smtlib_api class is 
      invoked). The user should not invoke this method directly. It sets the 
      protected member _smt_lib_api to the given argument (assigned API).
  */
  void set_smt_lib_api(smtlib_api * smt_lib_api)
  {
    _smt_lib_api = smt_lib_api;
  }

  /** Returns a pointer to smtlib_api instance assigned to the solver.
      Using this method the solver can access and read the state of the 
      API class, including assertion stack, option and info values, 
      defined labels, etc. Alternative way is to use _smt_lib_api protected
      data member directly. */
  smtlib_api * get_smt_lib_api() const
  {
    return _smt_lib_api;
  }

  /** Method is called from set_solver_interface() method of the
      smtlib_api class after the set_smt_lib_api(). By default, this
      method do nothing, but the user can override this if some custom
      initialization of the solver is needed after the smtlib_api
      class instance is assigned to it. */
  virtual void init_interface()
  {}


  /** Method is called from smtlib_api when reset() command is invoked. 
      It does nothing by default, but it can be overriden by the custom
      solver. */
  virtual void reset_interface()
  {}
  
  /** Method should invoke SMT solver's algorithm. It should return 
      appropriate value (sat, unsat, or unknown). */
  virtual check_sat_response check_sat() = 0;
    
  /** Method should return the assignment of values to expressions from
      exps. For each t from exps, its value v should be a ground expression
      that belongs to the predefined set of values for the sort of t in the 
      corresponding logic, and that is interpreted equivalently as 
      the expression t in the model found by the solver. The values should be 
      assigned to expressions using the set_assigned() method defined in 
      the expression_node class (the values can be read using the 
      get_assigned() method of expression_node class). The return value is 
      the expression_vector containing (some) expressions from exps with assigned values 
      (this gives the developer the freedom to drop some expressions 
      from exps whose values are not defined or cannot be calculated). */
  virtual expression_vector get_value(const expression_vector & exps) = 0;
    
  /** Method should return the proof of unsatisfiability. */
  virtual proof get_proof() = 0;

  /** Method should return the vector of expressions from the set of all
      assertions that are the reason for unsatisfiability. According to
      the standard, the expressions should be labels, i.e. constant
      function symbol expressions that are introduced as values of :named 
      attribute for asserted expressions. */
  virtual expression_vector get_unsat_core() = 0;

  /** Method should return the assignment of values to constant function 
      expressions that are introduced by :named attribute values for 
      expressions of sort Bool (i.e. labels for Bool expressions). Values
      for all labels must be either true or false expressions. Values should 
      be assigned using set_assigned() method of expression_node class. The 
      return value is expression_vector containing all labels (constant 
      function expressions) of sort Bool with assigned values (that can
      be read using get_assigned() method of expression_node class). */
  virtual expression_vector get_assignment() = 0;
    
  /** Virtual destructor. */
  virtual ~solver_interface() {}
};


extern std::istream * yy_istream;
extern smtlib_api * yy_api;

/** class smtlib_api: represents an SMT-LIB 2.0 compliant API. */
class smtlib_api {
private:
  typedef std::unordered_map<symbol, logic_declaration *> logics_map;
  
  logics_map _map;
  signature * _signature;
  signature_vector _expansion_signatures;
  expression_factory * _exp_factory;
  logic_declaration * _logic;
  symbol _logic_symbol;
  expression_vector _assertions;
  std::vector<unsigned> _assertion_levels;
  solver_interface * _solver_interface;
  
  attribute_set _options;
    

  bool _print_success;
  bool _expand_definitions;
  bool _produce_assertions;
  bool _produce_proofs;
  bool _produce_unsat_cores;
  bool _produce_models;
  bool _produce_assignments;
  bool _global_declarations;
  bool _syntax_checking;
  bool _logic_syntax_checking;
  std::ostream * _cout;
  std::ostream * _cerr;
  unsigned long _random_seed;
  unsigned long _verbosity;

  attribute_set _infos;


  check_sat_response _response;
  check_sat_response _expected_response;
  error_behavior _error_behavior;
  reason_unknown _reason_unknown;

  expression_vector _all_labels;
  std::vector<unsigned> _all_labels_levels;
  expression_vector _assertion_labels;
  std::vector<unsigned> _assertion_labels_levels;

  bool check_named(const expression & exp, symbol & s);
  
public:

  /** Constructor. */
  smtlib_api();

  /** Method enables registering new logics. The user should assigned
      a (user-defined) logic_declaration instance to some logic name.
      When registered, logic can be easily set by set_logic(name) method.
      All standard SMT-LIB 2.0 logics are registered during the 
      construction of the smtlib_api object.  */
  void register_logic(const symbol & name, logic_declaration * ld);  

  /** Returns the sort factory of the current signature hierarchy. Notice
      that this method should not be called before set_logic() is invoked.
      In that case, an exception is thrown. */
  sort_factory * get_sort_factory() const;

  /** Returns the expression factory created when logic is chosen. Notice
      that this method should not be called before set_logic() is invoked.
      In that case, an exception is thrown. */
  expression_factory * get_expression_factory() const;

  /** Returns the main signature of the current signature hierarchy. Notice
      that this method should not be called before set_logic() is invoked.
      In that case, an exception is thrown. */
  signature * get_signature() const;

  /** Sets the logic. This method corresponds to set-logic API command. 
      It throws an exception if the logic with given name is not
      registered. When successful, it creates (fresh) logic signature 
      hierarchy, sort and expression factory, and empties the current
      assertion stack. */
  void set_logic(const symbol & name);

  /** Returns the pointer to the logic declaration instance of the current
      logic. */
  logic_declaration * get_logic() const;
  
  /** Returns the registered symbolic name of the current logic. It throws an
      exception if the logic is not set. */
  const symbol & get_logic_symbol() const;

  /** Sets option. Option is given as attribute whose keyword is the name
      of the option, and the attribute value is the value of the option. 
      Deep copy of the attribute is performed. Notice that for predefined 
      options, the type of the corresponding attribute value
      must be adequate. For instance, :produce-assertions option should 
      have a value of bool value (as well as other boolean options), 
      :verbosity option and :random-seed option should have a value of 
      unsigned long type, and :regular-output-channel and
      :diagonostic-output-channel option should have a value of string 
      type. Beside the standard options, the implementation supports
      options :syntax-checking and :logic-syntax-checking (both of type
      bool). The former turns syntax checking on or off (must be set before
      set-logic command is invoked) and the latter turns on or off checking
      for comformance to the logic syntax restrictions for asserted 
      expressions. The latter option does not have any effect if the former
      option is set to false (then no syntax checking is performed).*/
  void set_option(const attribute & option);
  
  /** Sets option. Option is given as pair (keyword, standard_value),
      where standard_value is any copy/move-constructible, assignable and
      << printable value. Notice that for predefined options, the type T
      must be adequate. For instance, :produce-assertions option should 
      have a value of bool value (as well as other boolean options), 
      :verbosity option and :random-seed option should have a value of 
      unsigned long type, and :regular-output-channel and
      :diagonostic-output-channel option should have a value of string 
      type. Beside the standard options, the implementation supports
      options :syntax-checking and :logic-syntax-checking (both of type
      bool). The former turns syntax checking on or off (must be set before
      set-logic command is invoked) and the latter turns on or off checking
      for comformance to the logic syntax restrictions for asserted 
      expressions. The latter option does not have any effect if the former
      option is set to false (then no syntax checking is performed). */
  template<typename T>
  void set_option(const keyword & option, T && value);

  /** Sets info. Info is given as attribute whose keyword is the name
      of the info, and the attribute value is the value of the info. 
      Deep copy of the attribute is performed. Notice that for predefined 
      infos, the type of the corresponding attribute value
      must be adequate. For instance, :error-behavior info should have a 
      value of error_behavior type, and :reason-unknown info should have a 
      value of reason_unknown type. Info :status should have a value of 
      check_sat_response type. */   
  void set_info(const attribute & info);

  /** Sets info. Info is given as pair (keyword, standard_value),
      where standard_value is any copy/move-constructible, assignable and
      << printable value.  Notice that for predefined 
      infos, the type of the corresponding attribute value
      must be adequate. For instance, :error-behavior info should have a 
      value of error_behavior type, and :reason-unknown info should have a 
      value of reason_unknown type. Info :status should have a value of 
      check_sat_response type. */   
  template<typename T>
  void set_info(const keyword & info, T && value);

  /** Gets the value of :print-success option. */
  bool print_success() const;

  /** Gets the value of :expand-definitions option. */
  bool expand_definitions() const;

  /** Gets the value of :interactive-mode option (the same as
      :produce-assertions option, as of SMTLIB 2.5). */
  bool interactive_mode() const;

  /** Gets the value of :produce-assertions option. */
  bool produce_assertions() const;
  
  /** Gets the value of :print-success option. */
  bool produce_proofs() const;
    
  /** Gets the value of :produce-unsat-cores option. */
  bool produce_unsat_cores() const;

  /** Gets the value of :produce-models option. */
  bool produce_models() const;

  /** Gets the value of :produce-assignments option. */
  bool produce_assignments() const;

  /** Gets the value of :global-declarations option. */
  bool global_declarations() const;
  
  /** Gets the value of :syntax-checking option. This is an extension
      of the standard -- setting this option value to true or false 
      the user can choose between well-sortedness checking on or off. */
  bool syntax_checking() const;

  /** Gets the value of :logic-syntax-checking option. This is an extension
      of the standard -- setting this option to true or false the user can
      choose between logic language restrictions checking on or off. Notice
      that :syntax-checking option must be also set to true, otherwise the
      logic language restrictions checking will not be performed. */
  bool logic_syntax_checking() const;

  /** Gets the regular output stream. */
  std::ostream & cout() const;

  /** Gets the diagnostic output stream. */
  std::ostream & cerr() const;

  /** Gets the value of :random-seed option. */
  unsigned long random_seed() const;

  /** Gets the value of :verbosity option. */
  unsigned long verbosity() const;

  /** Gets the value of :status info. */
  check_sat_response get_expected_response() const;

  /** Gets the response of the most recent call to check-sat command */
  check_sat_response get_response() const;
  
  /** Gets the value of :error-behavior info. */
  error_behavior get_error_behavior() const;

  /** Gets the value of :reason-unknown info. */
  reason_unknown get_reason_unknown() const;
  
  /** Returns a pointer to an abstract attribute value. 
      If the option is not set, it returns 0. */
  attribute_value * get_option(const keyword & option) const;

  /** Gets a value of the option and assigns it to the value argument of the
      type T. The user should provide a non-const data of type T as
      the second argument in which the value should be stored. The type
      T must be adequate, otherwise, the method throws an exception.  */
  template <typename T>
  void get_option(const keyword & option, T & value) const;

  /** Gets info. Similar to get_option(info), only info is read. */
  attribute_value * get_info(const keyword & info) const;

  /** Gets info. Similar to get_option(info, value), only info is read. */
  template <typename T>
  void get_info(const keyword & info, T & value) const;

  /** Declares sort symbol. Corresponds to declare-sort API command. */
  void declare_sort(const sort_symbol & sm, arity ar);

  /** Defines sort symbol. Corresponds to define-sort API command. Argument
      pars should contain all parameters that appear in sort, and only
      them. */
  void define_sort(const sort_symbol & sm, 
		   const sort_parameter_vector & pars, 
		   const sort & sr);

  /** Defines sort symbol. Corresponds to define-sort API command. Argument
      pars should contain all parameters that appear in sort, and only
      them. */
  void define_sort(const sort_symbol & sm, 
		   sort_parameter_vector && pars, 
		   const sort & sr);
  
  /** Invokes sort expansion. */
  sort expand_sort(const sort & sr) const;

  /** Declares constant symbol. Corresponds to declare-const API command. */
  void declare_const(const function_symbol & sm, const sort & s);
  
  /** Declares function symbol. Corresponds to declare-fun API command. */
  void declare_function(const function_symbol & sm, const rank & rn);

  /** Declares function symbol. Corresponds to declare-fun API command. */
  void declare_function(const function_symbol & sm, rank && rn);
  
  /** Declares function symbol with one sort in its rank (constant function
      symbol). */
  void declare_function(const function_symbol & sm, const sort & s);

  /** Declares function symbol with two sorts in its rank. */
  void declare_function(const function_symbol & sm, 
			const sort & s1, 
			const sort & s2);

  /** Declares function symbol with three sorts in its rank. */
  void declare_function(const function_symbol & sm, 
			const sort & s1, 
			const sort & s2, 
			const sort & s3);
    
  /** Declares function symbol with four sorts in its rank. */
  void declare_function(const function_symbol & sm, 
			const sort & s1, 
			const sort & s2, 
			const sort & s3,
			const sort & s4);

  /** Defines function symbol. Corresponds to define-fun API command.
      The argument vars should contain all (free) sorted variables that 
      appear in exp, and only them. The sort argument should be the
      return sort of the function symbol being defined. */
  void define_function(const function_symbol & sm,
		       const sorted_variable_vector & vars,
		       const sort & sr,
		       const expression & exp);

  /** Defines function symbol. Corresponds to define-fun API command.
      The argument vars should contain all (free) sorted variables that 
      appear in exp, and only them. The sort argument should be the
      return sort of the function symbol being defined. */
  void define_function(const function_symbol & sm,
		       sorted_variable_vector && vars,
		       const sort & sr,
		       const expression & exp);
  
  /** Invokes expression expansion. */
  expression expand_expression(const expression & exp) const;

  /** Pushs n empty assertion sets to the assertion stack. Corresponds to
      push API command. */
  void push(unsigned n);

  /** Pops n assertion sets from the assertion stack. Corresponds to
      pop API command. There must be at least n+1 assertion sets on the
      stack, otherwise it throws an exception. */
  void pop(unsigned n);

  /** Equivalent to push(1). */
  void push();

  /** Equivalent to pop(1). */
  void pop();

  /** Checks whether a label is assigned to the expression exp (using 
      :named attribute). If so, the corresponding function symbol is
      defined (with definition equal to exp), and the label is added to 
      the set of all labels. */
  void check_label(const expression & exp);

  /** Checks whether a label is assigned to the expression exp (using
      :named attribute). If so, the label is added to the set of 
      assertion labels. The corresponding function symbol should be 
      already declared. Otherwise, an exception is thrown. 
      Assertion labels are the labels assigned to top-level 
      expressions that are asserted using the assert_expression() method.
      The standard defines that those labels should be considered when
      get-assignment and get-unsat-core commands are invoked. */
  void check_assertion_label(const expression & exp);
  
  /** Asserts the expression to the current (top) assertion set. Corresponds
      to assert API command. This method also checks if the label is
      assigned to the expression exp (using :named attribute) and sets
      appropriate assertion label (using check_label() and 
      check_assertion_label() methods). */
  void assert_expression(const expression & exp);

  /** Returns the vector of all labels (registered with check_label() method). 
      Labels are represented by constant function symbol expressions. */
  const expression_vector & get_labels() const;

  /** Gets the vector of asserted labels (registered with 
      check_assertion_label() method). Labels are represented by contant 
      function symbol expressions. */
  const expression_vector & get_assertion_labels() const;

  /** Sets solver_interface instance. The method invokes set_smtlib_api() 
      method for the solver_interface instance (so, the user does not
      have to do this manually). This method should be called before
      any of the methods check_sat(), get_unsat_core(), get_proof(), get_value(),
      or get_assignment() is invoked. Otherwise, these methods throw
      an exception. */
  void set_solver_interface(solver_interface * sl);

  /** Gets a pointer to the solver interface. */
  solver_interface * get_solver_interface() const;
  
  /** Checks for satisfiability of the current set of all assertions.
      It corresponds to check-sat command. It invokes check_sat() method
      of the solver interface. */
  check_sat_response check_sat();

  /** Returns the vector containing the expressions from the set of 
      all assertions. This method can be invoked even if the 
      :produce-assertions option is set to false. */ 
  const expression_vector & assertions() const;

  /** Returns the vector containing the expressions from the set of 
      all assertions. It corresponds to get-assertions
      API command. It should be called only if :produce-assertions option
      is set to true (default is false). Otherwise, it throws 
      an exception. */
  const expression_vector & get_assertions() const;

  /** Empties the assertion stack. It corresponds to reset-assertions
      API command. If :global-declarations option is true,
      declarations and definitions that were introduced while
      :global-declaration option was true are not removed (since they
      are global). Otherwise, all declarations and definitions are also
      removed. */
  void reset_assertions();

  /** Resets the interface and the associated solver */
  void reset();
  
  /** Returns the assignment of values to the expressions from exps. It 
      corresponds to get-value API command. It invokes get_value() method
      of the solver interface. It should be called only if :produce-models
      option is set to true (default is false). Otherwise, it throws
      an exception.  */
  expression_vector get_value(const expression_vector & exps) const;
   
  /** Returns the assignment. It correpsonds to get-assignment API command.
      It invokes get_assignment() method of the solver interface. It should
      be called only if :produce-assignments is set to true (default is 
      false). Otherwise, it throws an exception. */
  expression_vector get_assignment() const; 

  /** Returns the proof object. It corresponds to get-proof API command.
      It invokes get_proof() method of the solver interface. It should be
      called only if :produce-proofs option is set to true (default is false).
      Otherwise, it throws an exception. */
  proof get_proof() const;  

  /** Returns the unsat core. It correspomds to get-unsat-core API command.
      It invokes get_unsat_core() method of the solver interface. It should
      be called only if :produce-unsat-cores option is set to true (default
      is false). Otherwise, it throws an exception. */
  expression_vector get_unsat_core() const;

  /** Prints the message back to the output stream */
  void echo(const std::string & message) const;
  
  /** Parse the input from the input stream. It invokes LEX/YACC generated
      parser. The parser catches all exceptions and prints appropriate
      messages. If :error-behavior info is set to immediate-exit (which
      is default), it exits when the first error is detected. If 
      :error-behavior info is set to continued-execution, then it does not
      exit, and the state of the API interface remains unchanged. */
  void parse_input(std::istream & istr);

  /** Destructor. */
  ~smtlib_api ();
};


inline 
std::istream & operator >> (std::istream & istr, smtlib_api & api)
{
  api.parse_input(istr);
  return istr;
}

// INLINE FUNCTIONS' DEFINITIONS -------------------------------------------

inline
void smtlib_api::register_logic(const symbol & name, logic_declaration * ld)
{
  _map.insert(std::make_pair(name, ld));
}

inline
sort_factory * smtlib_api::get_sort_factory() const
{
  if(_signature)
    return _signature->get_sort_factory();
  else
    throw logic_not_set_exception("Sort subsystem unaccessible");
}

inline
expression_factory * smtlib_api::get_expression_factory() const
{
  if(_exp_factory)
    return _exp_factory;
  else 
    throw logic_not_set_exception("Expression subsystem unaccessible");
}

inline
signature * smtlib_api::get_signature() const
{
  if(_signature)
    return _signature;
  else
    throw logic_not_set_exception("Signature unaccessible");
}

inline
logic_declaration * smtlib_api::get_logic() const
{
  if(_logic)
    return _logic;
  else
    throw logic_not_set_exception("Logic unaccessible");
}
  
inline
const symbol & smtlib_api::get_logic_symbol() const
{
  if(_logic)
    return _logic_symbol;
  else
    throw logic_not_set_exception("Logic unaccessible");
}

template<typename T>
inline
void smtlib_api::set_option(const keyword & option, T && value)
{
  set_option(attribute(option, std::forward<T>(value)));
}
  
template<typename T>
inline
void smtlib_api::set_info(const keyword & info, T && value)
{
  set_info(attribute(info, std::forward<T>(value)));
}
  
inline
bool smtlib_api::print_success() const
{
  return _print_success;
}

inline
bool smtlib_api::expand_definitions() const
{
  return _expand_definitions;
}

inline
bool smtlib_api::interactive_mode() const
{
  return _produce_assertions;
}

inline
bool smtlib_api::produce_assertions() const
{
  return _produce_assertions;
}


inline
bool smtlib_api::produce_proofs() const
{
  return _produce_proofs;
}

inline
bool smtlib_api::produce_unsat_cores() const
{
  return _produce_unsat_cores;
}

inline
bool smtlib_api::produce_models() const
{
  return _produce_models;
}

inline
bool smtlib_api::produce_assignments() const
{
  return _produce_assignments;
}

inline
bool smtlib_api::global_declarations() const
{
  return _global_declarations;
}

inline
bool smtlib_api::syntax_checking() const
{
  return _syntax_checking;
}

inline
bool smtlib_api::logic_syntax_checking() const
{
  return _logic_syntax_checking;
}

inline
std::ostream & smtlib_api::cout() const
{
  return *_cout;
}

inline
std::ostream & smtlib_api::cerr() const
{
  return *_cerr;
}

inline
unsigned long smtlib_api::random_seed() const
{
  return _random_seed;
}

inline
unsigned long smtlib_api::verbosity() const
{
  return _verbosity;
}

inline
check_sat_response smtlib_api::get_expected_response() const
{
  return _expected_response;
}

inline
check_sat_response smtlib_api::get_response() const
{
  return _response;
}

inline
error_behavior smtlib_api::get_error_behavior() const
{
  return _error_behavior;
}

inline
reason_unknown smtlib_api::get_reason_unknown() const
{
  return _reason_unknown;
}

inline
attribute_value * smtlib_api::get_option(const keyword & option) const
{
  attribute_set::const_iterator it = _options.find(option);
  if(it != _options.end())
    return it->get_value();
  else
    return 0;
}

inline
attribute_value * smtlib_api::get_info(const keyword & info) const
{
  attribute_set::const_iterator it = _infos.find(info);
  if(it != _infos.end())
    return it->get_value();
  else
    return 0;
}

template <typename T>
inline
void smtlib_api::get_option(const keyword & option, T & value) const
{
  attribute_set::const_iterator it = _options.find(option);
  if(it != _options.end())
    {
      try {
	it->get_value(value);
      }
      catch(const no_attribute_value_exception & e)
	{
	  throw command_error_exception("get-option: option value not set: " 
					+ e.get_description());
	}
      catch(const bad_attribute_value_type_exception & e)
	{
	  throw command_error_exception("get-option: option value type mismatch: " 
					+ e.get_description());
	}
    }
  else
    throw command_error_exception("get-option: option value not set");
}

  
template <typename T>
inline
void smtlib_api::get_info(const keyword & info, T & value) const
{
  attribute_set::const_iterator it = _infos.find(info);
  if(it != _infos.end())
    {
      try {
	it->get_value(value);
      }
      catch(const no_attribute_value_exception & e)
	{
	  throw command_error_exception("get-info: option value not set: " 
					+ e.get_description());
	}
      catch(const bad_attribute_value_type_exception & e)
	{
	  throw command_error_exception("get-info: option value type mismatch: " 
					+ e.get_description());
	}
    }
  else
    throw command_error_exception("get-info: option value not set");
}
  
  
  
inline
void smtlib_api::declare_sort(const sort_symbol & sm, arity ar)
{
  if(!_logic)
    throw logic_not_set_exception("declare-sort");

  if(_global_declarations)
    _expansion_signatures[0]->add_sort_symbol(sm, ar);
  else 
    _expansion_signatures.back()->add_sort_symbol(sm, ar);
}

inline
void smtlib_api::define_sort(const sort_symbol & sm, 
			     const sort_parameter_vector & pars, 
			     const sort & sr)
{
  define_sort(sm, sort_parameter_vector(pars), sr);
}

inline
void smtlib_api::define_sort(const sort_symbol & sm, 
			     sort_parameter_vector && pars, 
			     const sort & sr)
{
  if(!_logic)
    throw logic_not_set_exception("define-sort");
  
  attribute_set attr(HASH_TABLE_SIZE);
  attr.insert(attribute(keyword::DEFINITION, sort_definition(std::move(pars), sr)));
  if(_global_declarations)
    _expansion_signatures[0]->add_sort_symbol(sm, pars.size(), std::move(attr));
  else
    _expansion_signatures.back()->add_sort_symbol(sm, pars.size(), std::move(attr));
}


inline
sort smtlib_api::expand_sort(const sort & sr) const
{
  if(!_logic)
    throw logic_not_set_exception("expand-sort");
 
  return sr->expand_sort(); 
}


inline
void smtlib_api::
declare_const(const function_symbol & sm, const sort & s)
{
  if(!_logic)
    throw logic_not_set_exception("declare-const");

  if(_global_declarations)
    _expansion_signatures[0]->add_function_symbol(sm, s);
  else 
    _expansion_signatures.back()->add_function_symbol(sm, s);
}

inline
void smtlib_api::
declare_function(const function_symbol & sm, const rank & rn)
{
  if(!_logic)
    throw logic_not_set_exception("declare-fun");

  if(_global_declarations)
    _expansion_signatures[0]->add_function_symbol(sm, rn);
  else
    _expansion_signatures.back()->add_function_symbol(sm, rn);
}

inline
void smtlib_api::
declare_function(const function_symbol & sm, rank && rn)
{
  if(!_logic)
    throw logic_not_set_exception("declare-fun");

  if(_global_declarations)
    _expansion_signatures[0]->add_function_symbol(sm, std::move(rn));
  else
    _expansion_signatures.back()->add_function_symbol(sm, std::move(rn));
}

inline
void smtlib_api::
declare_function(const function_symbol & sm, const sort & s)
{
  if(!_logic)
    throw logic_not_set_exception("declare-fun");

  if(_global_declarations)
    _expansion_signatures[0]->add_function_symbol(sm, s);
  else
    _expansion_signatures.back()->add_function_symbol(sm, s);
}

inline
void smtlib_api::
declare_function(const function_symbol & sm, 
		 const sort & s1, 
		 const sort & s2)
{
  if(!_logic)
    throw logic_not_set_exception("declare-fun");

  if(_global_declarations)
    _expansion_signatures[0]->add_function_symbol(sm, s1, s2);
  else 
    _expansion_signatures.back()->add_function_symbol(sm, s1, s2);
}

inline
void smtlib_api::
declare_function(const function_symbol & sm, 
		 const sort & s1, 
		 const sort & s2, 
		 const sort & s3)
{
  if(!_logic)
    throw logic_not_set_exception("declare-fun");

  if(_global_declarations)
    _expansion_signatures[0]->add_function_symbol(sm, s1, s2, s3);
  else
    _expansion_signatures.back()->add_function_symbol(sm, s1, s2, s3);
}

inline
void smtlib_api::
declare_function(const function_symbol & sm, 
		 const sort & s1, 
		 const sort & s2, 
		 const sort & s3,
		 const sort & s4)
{
  if(!_logic)
    throw logic_not_set_exception("declare-fun");

  if(_global_declarations)
    _expansion_signatures[0]->add_function_symbol(sm, s1, s2, s3, s4);
  else
    _expansion_signatures.back()->add_function_symbol(sm, s1, s2, s3, s4);
}

inline
expression smtlib_api::expand_expression(const expression & exp) const
{
  if(!_logic)
    throw logic_not_set_exception("expand-term");

  return exp->expand_expression();  
}

inline
void smtlib_api::push(unsigned n)
{
  while(n--)
    push();
}

inline
void smtlib_api::pop(unsigned n)
{
  while(n--)
    pop();
}


inline
const expression_vector & smtlib_api::get_labels() const
{
  return _all_labels;
}

inline
const expression_vector & smtlib_api::get_assertion_labels() const
{
  return _assertion_labels;
}


inline
bool smtlib_api::check_named(const expression & exp, symbol & s)
{
  const attribute_set & attr = exp->get_attributes();
  attribute_set::const_iterator it = attr.find(keyword::NAMED);
  if(it != attr.end())
    {
      it->get_value(s);
      return true;
    }
  else
    return false;
}

inline
void smtlib_api::set_solver_interface(solver_interface * sl)
{
  _solver_interface = sl;
  _solver_interface->set_smt_lib_api(this);
  _solver_interface->init_interface();
}

inline
solver_interface * smtlib_api::get_solver_interface() const
{
  return _solver_interface;
}

inline
check_sat_response smtlib_api::check_sat()
{
  if(!_logic)
    throw logic_not_set_exception("check-sat");

  if(!_solver_interface)
    throw solver_interface_not_set_exception("check-sat");

  return (_response = _solver_interface->check_sat());
}

inline
const expression_vector & smtlib_api::assertions() const
{
  return _assertions;
}
  
inline
const expression_vector & smtlib_api::get_assertions() const
{
  if(!_produce_assertions)
    throw command_error_exception("get-assertions permitted only in when :produce-assertions option is on");
  return _assertions;
}


inline 
void smtlib_api::echo(const std::string & message) const
{
  cout() << message << std::endl;
}

#endif // _SMTLIB_API_H
