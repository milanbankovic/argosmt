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

#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include "signature.hpp"
#include "vector_hash_code.hpp"


class expression_node;

/** Represents a shared pointer to an expression node. Expressions are 
    represented as tree like structures that reside within the expression 
    factory. Each expression node is unique in the factory and represents a 
    distinct expression. Common subexpressions are shared between different 
    expression nodes. Expression nodes can be accessed by the user only t
    hrough expression shared pointers. Two shared pointers represent the 
    same expression if and only if they point to the same expression 
    node in the factory. Each expression  node is destroyed automatically by
    the factory when the last shared pointer pointing to it is destroyed. */
typedef std::shared_ptr<expression_node> expression;

/** Vector of expressions. */
typedef std::vector <expression> expression_vector;

/** Empty expression vector. */
extern expression_vector EMPTY_EXPRESSION_VECTOR;

/** Represents a substitution (a mapping of variables to expressions). */
typedef std::unordered_map<variable, expression> substitution;


sort_vector get_sorts(const expression_vector & expressions);


/** Represents a pair (variable, sort). This is used in expressions that
    include quantifiers (each quantified variable has assigned sort). */
class sorted_variable {
private:
  variable _var;
  sort _sort;
public:
  /** Constructor. */
  sorted_variable(const variable & var, const sort & sr);

  /** Gets the variable. */
  const variable & get_variable() const;

  /** Gets the sort. */
  const sort & get_sort() const;

  /** Returns true if sv is equal to this variable and of the same sort. */
  bool operator == (const sorted_variable & sv) const;

  /** Returns true if sv is not equal to this variable or is of different
      sort */
  bool operator != (const sorted_variable & sv) const;

  /** Returns true if this variable is lexicographically lower than sv. */
  bool operator < (const sorted_variable & sv) const;

  /** Returns true if this variable is lexicographically greater than sv. */
  bool operator > (const sorted_variable & sv) const;

  /** Returns true if this variable is lexicographically lower or equal to sv. */
  bool operator <= (const sorted_variable & sv) const;

  /** Returns true if this variable is lexicographically greater or equal to sv. */
  bool operator >= (const sorted_variable & sv) const;


  std::size_t hash_code() const;
  void out(std::ostream & ostr) const;
  std::string to_string() const;
};

namespace std {
  template<>
  struct hash<sorted_variable> : 
    public unary_function<sorted_variable, std::size_t> {
    std::size_t operator ()(const sorted_variable & sv) const
    {
      return sv.hash_code();
    }
  };
}
  
/** Prints the sorted variable to the output stream. */
std::ostream & operator << (std::ostream & ostr, const sorted_variable & sv);

/** Vector of sorted variables. */
typedef std::vector<sorted_variable> sorted_variable_vector;

/** Set of sorted variables. */
typedef std::unordered_set<sorted_variable> sorted_variable_set;

/** Empty sorted variable vector. */
extern const sorted_variable_vector EMPTY_SORTED_VARIABLE_VECTOR;

/** Empty sorted variable set. */
extern const sorted_variable_set EMPTY_SORTED_VARIABLE_SET;

/** Represents a pair (variable, expression). Used in let expressions. */
class variable_binding {
private:
  variable _var;
  expression _expr;
public:
  /** Constructor. */
  variable_binding(const variable & var, const expression & expr);
  
  /** Gets the variable. */
  const variable & get_variable() const;

  /** Gets the expression. */
  const expression & get_expression() const;

  /** Returns true if the given binding is equal to this binding. */
  bool operator == (const variable_binding & binding) const;

  /** Returns true if the given binding is not equal to this binding. */
  bool operator != (const variable_binding & binding) const;

  /** Returns true if this binding is lexicographically lower than given
      binding. */
  bool operator < (const variable_binding & binding) const;

  /** Returns true if this binding is lexicographically greater than given
      binding. */
  bool operator > (const variable_binding & binding) const;
  
  /** Returns true if this binding is lexicographically lower or equal to
      given binding. */
  bool operator <= (const variable_binding & binding) const;

  /** Returns true if this binding is lexicographically greater or equal to
      given binding. */
  bool operator >= (const variable_binding & binding) const;


  std::size_t hash_code() const;
  void out(std::ostream & ostr) const;
  std::string to_string() const;

};


namespace std {    
  template<>
  struct hash<variable_binding> : 
    public unary_function<variable_binding, std::size_t> {
    std::size_t operator ()(const variable_binding & vb) const
    {
      return vb.hash_code();
    }
  };
}


/** An empty class that represents external data that is assigned to some
    expression. This class can be inherited by some solver/tool specific
    class that contains data needed to operate with expressions. */
class expression_data 
{
public:
  virtual ~expression_data() {}
};
  
/** Prints the binding to the output stream. */
std::ostream & operator << (std::ostream & ostr, const variable_binding & vb);

/** Vector of variable bindings. */
typedef std::vector<variable_binding> variable_binding_vector;

/** Empty vector of variable bindings. */
extern const variable_binding_vector EMPTY_VARIABLE_BINDING_VECTOR;

/** Represents types of quantifiers. */
enum quantifier 
  {
    /** Exsistentional quantifier */
    Q_EXISTS, 
    /** Universal quantifier */
    Q_FORALL, 
    /** Undefined quantifier */
    Q_UNDEFINED
  };

/** SAT variable. */
typedef unsigned sat_variable;

/** Undefined SAT variable. */
extern const sat_variable UNDEFINED_SAT_VARIABLE;

/** Polarity of SAT literal. */
enum sat_literal_polarity { 
  /** Positive polarity. */
  SLP_POSITIVE, 
  /** Negative polarity. */
  SLP_NEGATIVE, 
  /** Undefined polarity. */
  SLP_UNDEFINED };
  
class expression_factory;
class expression_visitor;

/** Represents a node within an expression factory. 
    It is the base class for several other classes representing different 
    types of expression nodes. Its interface is available through 
    Expression shared pointers that are used for manipulation with 
    expression nodes. This class should never be used directly by the 
    user. Expression nodes can be created only within an expression 
    factory and should be accessed only by using Expression 
    shared pointers.*/ 
class expression_node : 
  public std::enable_shared_from_this<expression_node> {    
protected:
  expression_factory * _factory;
  attribute_set * _attributes;

  std::size_t _hash_code;

  expression _assigned;
  expression_data * _expression_data;

  bool sorts_equal(const sort & s1, const sort & s2) const;
    
  /** Constructor is protected. */
  expression_node(expression_factory * factory);
    
public:

  /** Represents types of expression nodes. */
  enum type 
    {
      /** Represents a node consisting of a variable  */
      T_VARIABLE, 
      /** Represents a node consisting of a special constant */
      T_SPECIAL_CONSTANT, 
      /** Represents a node consisting of a function symbol
	  (if rank consists of only one sort) or a function symbol applied to
	  n-1 subexpressions (in case that rank consists of n sorts) */
      T_FUNCTION, 
      /** Represents a node consisting of a quantifier applied to a 
	  subexpression (of sort Bool) */
      T_QUANTIFIER, 
      /** Represents a node consisting of a let binder applied to
	  a subexpression */
      T_LET,
      /** Represents a node consisting of a SAT variable and its polarity */
      T_SAT,
      /** Represents an undefined expression */
      T_UNDEF
    };


  /** Returns the pointer to the expression factory in which the expression
      resides. */
  expression_factory * get_factory() const;

  /** Returns true if the expression has assigned attributes. */
  bool has_attributes() const;
    
  /** Returns a reference to the set of attributes of the expression.
      Through this reference the set can be changed. */
  attribute_set & get_attributes();

  /** Returns true if attribute with the given keyword is set for the 
      expression. */
  bool is_attribute_set(const keyword & kw) const;

  /** Returns a pointer to the attribute value object of the expression
      attribute with the given keyword, or 0 if the attribute 
      (or its value) is not set. */
  attribute_value * get_attribute_value(const keyword & kw) const;

  /** Gets a value of type T that corresoponds to the given
      keyword (the second argument is a non-const reference to T object
      to which the value is assigned). If there is no such attribute, or
      its value is not of the type T, an exception is thrown. */
  template <typename T>    
  void get_attribute_value(const keyword & kw, T & value) const;
    
  /** Sets the expression attribute. */ 
  void set_attribute(const attribute & attr);

  /** Sets the expression attribute. */ 
  void set_attribute(attribute && attr);
  
  /** Sets the value of the expression attribute with the given keyword to 
      the given value of type T. */
  template <typename T>
  void set_attribute_value(const keyword & kw, T && value);

  /** Returns the expression assigned to this expression. The assigned expression is
      the value of this expression, and should be one of the values defined for
      the sort of this expression. If the value of this expression is not defined, then
      the assigned expression should be the undefined expression (this is also the
      initial value of the assigned expression). */
  const expression & get_assigned() const;

  /** Sets the assigned expression. The assigned expression is
      the value of this expression, and should be one of the values defined for
      the sort of this expression. If the value of this expression is not defined, then
      the assigned expression should be the undefined expression (this is also the
      initial value of the assigned expression). */
  void set_assigned(const expression & assigned);

  /** Returns true if the expression assigned to this expression is not undefined
      expression. */
  bool is_assigned() const;

    
  /** Returns pointer to structure that contains solver/tool specific data. */
  expression_data * get_expression_data() const;

  /** Sets solver/tool specific data for the expression. */
  void set_expression_data(expression_data * ed);

  /** Returns true if the solver/tool specific data has been assigned 
      to the expression. */
  bool has_expression_data() const;

  /** Returns the type of the expression node. */
  virtual type get_type() const = 0;

  /** Returns the top-level function symbol (in case of T_FUNCTION type)
      or function_symbol::UNDEFINED, otherwise. */
  virtual const function_symbol & get_symbol() const = 0;
    
  /** Returns the sort of the expression. Notice that sort can be undefined,
      if syntax checking is turned off.  */
  virtual const sort & get_sort() const = 0;

  /** Returns the sort of the expression _after_ the sort inference is
      performed on the expression (that is, it is never undefined) */
  const sort & get_inferred_sort() const;
  
  /** Returns true if the sort of the expression is Bool. */
  bool is_formula() const;

  /** Returns the variable represented by the node (in case of T_VARIABLE
      type), or variable::UNDEFINED, otherwise. */
  virtual const variable & get_variable() const = 0;

  /** Returns the special constant represented by the node (in case of
      T_SPECIAL_CONSTANT type), or special_constant::UNDEFINED, otherwise. */
  virtual const special_constant & get_special_constant() const = 0;

  /** Returns the quantifier type of the expression represented by the
      node (in case of T_QUANTIFIER type), or Q_UNDEFINED, otherwise. */
  virtual quantifier get_quantifier() const = 0;

  /** Returns the set of (both free and bound) sorted variables that 
      occur in the expression represented by the node. */
  virtual const sorted_variable_set & get_variables() = 0;

  /** Returns true if the expression represented by the node contains
      no variables. */
  bool is_ground();

  /** Returns the set of free (non-quantified) sorted variables that occur
      in the expression represented by the node. */
  virtual const sorted_variable_set & get_free_variables() = 0;

  /** Returns true if the expression represented by the node contains
      no free variables. */
  bool is_closed();

  /** Returns the vector of (top-level) quantified variables (in case of
      T_QUANTIFIER type), or EMPTY_SORTED_VARIABLE_VECTOR, otherwise. */
  virtual const sorted_variable_vector & get_quantified_variables() const = 0;

  /** Returns the subexpression to which the quantifier (or let-binder)
      is applied (in case of T_QUANTIFIER or T_LET types), or undefined
      expression, otherwise. */
  virtual const expression & get_subexpression() const = 0;

  /** Returns the vector of (top-level) variable bindings (in case of
      T_LET type), or EMPTY_VARIABLE_BINDING_VECTOR, otherwise. */
  virtual const variable_binding_vector & get_variable_bindings() const = 0;

  /** Returns the operands (direct subexpressions) to which the top-level 
      function symbol is applied (in case of T_FUNCTION type), or 
      EMPTY_EXPRESSION_VECTOR, otherwise. */
  virtual const expression_vector & get_operands() const = 0;

  /** Returns the variable number of the SAT literal represented by this
      node. SAT variables are enumerated starting from 1. If the type of the 
      node is not T_SAT, returns UNDEFINED_SAT_VARIABLE 
      (defined as (unsigned)(-1). */
  virtual sat_variable get_sat_variable() const = 0;

  /** Returns the polarity of the SAT literal represented by this node.
      If the type of the node is not T_SAT, returns SLP_UNDEFINED. */
  virtual sat_literal_polarity get_sat_literal_polarity() const = 0;

  /** Returns the expression obtained by instantiating the (free) variables
      with expressions given in substitution in the expression represented 
      by the node. Recall that substitution is a map that maps variables
      with expressions which replace them. */
  virtual expression get_instance(const substitution & sb) const = 0;

  /** Returns true if the expression represented by the 
      argument can be obtained from this expression, using some substitution
      (which is then returned by the substitution argument). If the method 
      is called with a non-empty substitution, then mappings found in 
      substitution at the time of call must be preserved, i.e. we check 
      whether the expression given by the argument is an instance of this 
      expression with respect to the given substitution. */
  virtual bool is_instance(const expression & ex,
			   substitution & sb) const = 0;

  /** Returns true if the expression is syntactically correct with respect 
      to the given signature. Notice that undefined expression is _not_ 
      syntactically correct in any signature. The argument smode defines
      the search mode. For more info about search modes, see search_mode
      type documentation. */
  virtual bool check_expression(const signature * sg,
				search_mode smode = S_CONTEXTUAL) const = 0; 

  /** Returns the expression obtained from this expression by applying
      sort inference, i.e. all undefined sorts are changed to the sorts
      defined by the ranks of the symbols declared in the signature (or
      the sorts of the special constants). If sort inference is not
      possible (for instance, if the expression is syntactically incorrect)
      an appropriate exception is thrown. This method makes sense only if
      syntax checking is turned off (otherwise, sort inference is performed
      during the construction of expressions). */
  virtual expression infer_sorts() const = 0;

  /** Returns the expression obtained from this by expanding function symbol 
      definitions. The smode argument defines in which signatures the 
      definitions should be searched (starting with the signature given 
      by the first argument). Sort inference is performed during the
      expansion. */
  virtual expression 
  expand_expression(const signature * sg,
		    search_mode smode = S_CONTEXTUAL) const = 0;

  /** Expands expression with respect to the main signature and 
      _contextual_ search mode (default mode). */
  expression expand_expression() const;

  /** Support for visitor design pattern. Subexpressions are traversed in a
      depth-first order, applying the visitor to each node. */
  virtual void accept_visitor(expression_visitor & visitor) = 0;


  /** Returns the expression obtained from this expression by elimination
      of let binders. */
  virtual expression eliminate_let_binders(const substitution & sub = substitution()) const = 0;
  
  /** Equivalent to get_type() == T_UNDEFINED. */
  bool is_undefined() const;

  /** Equivalent to get_type() == T_VARIABLE. */
  bool is_variable() const;

  /** Equivalent to get_type() == T_SPECIAL_CONSTANT. */
  bool is_special_constant() const;

  /** Equivalent to get_type() == T_SPECIAL_CONSTANT &&
      get_special_constant().get_type() == special_constant::T_NUMERAL. */
  bool is_numeral_constant() const;

  /** Equivalent to get_type() == T_SPECIAL_CONSTANT &&
      get_special_constant().get_type() == special_constant::T_BINARY. */
  bool is_binary_constant() const;

  /** Equivalent to get_type() == T_SPECIAL_CONSTANT &&
      get_special_constant().get_type() == special_constant::T_HEXADECIMAL. */
  bool is_hexadecimal_constant() const;

  /** Equivalent to get_type() == T_SPECIAL_CONSTANT &&
      get_special_constant().get_type() == special_constant::T_DECIMAL. */
  bool is_decimal_constant() const;

  /** Equivalent to get_type() == T_SPECIAL_CONSTANT &&
      get_special_constant().get_type() == special_constant::T_STRING_LITERAL. */
  bool is_string_literal_constant() const;

  /** Equivalent to get_type() == T_FUNCTION. */
  bool is_function() const;

  /** Equivalent to get_type() == T_FUNCTION && get_operands().size() == 0. */
  bool is_constant_function() const;

  /** Equivalent to get_type() == T_QUANTIFIER. */
  bool is_quantifier() const;

  /** Equivalent to get_type() == T_QUANTIFIER && 
      get_quantifier() == Q_FORALL. */
  bool is_forall() const;

  /** Equivalent to get_type() == T_QUANTIFIER && 
      get_quantifier() == Q_EXISTS. */
  bool is_exists() const;

  /** Equivalent to get_type() == T_LET. */
  bool is_let() const;

  /** Equivalent to get_type() == T_SAT. */
  bool is_sat_literal() const;

  /** Equivalent to get_sat_literal_polarity() == SLP_POSITIVE. */
  bool is_positive_sat_literal() const;

  /** Equivalent to get_sat_literal_polarity() == SLP_NEGATIVE. */
  bool is_negative_sat_literal() const;

  /** Equivalent to get_symbol() == "true". */
  bool is_true() const;

  /** Equivalent to get_symbol() == "false". */
  bool is_false() const;

  /** Equivalent to get_symbol() == "not". */
  bool is_not() const;

  /** Equivalent to get_symbol() == "=>". */
  bool is_implication() const;

  /** Equivalent to get_symbol() == "and". */
  bool is_and() const;

  /** Equivalent to get_symbol() == "or". */
  bool is_or() const;

  /** Equivalent to get_symbol() == "xor". */
  bool is_xor() const;

  /** Equivalent to get_symbol() == "=". */
  bool is_equality() const;

  /** Equivalent to get_symbol() == "=" && get_operands()[0]->is_formula(). */
  bool is_equivalence() const;

  /** Equivalent to get_operands().size() == 2 && get_symbol() == "distinct". */
  bool is_disequality() const;
    
  /** Equivalent to get_symbol() == "distinct". */
  bool is_distinct() const;

  /** Equivalent to get_symbol() == "distinct" && get_operands()[0]->is_formula() */
  bool is_inequivalence();

  /** Equivalent to get_symbol() == "ite". */
  bool is_ite() const;

  /** Create identical expression within another expression factory */
  virtual expression clone_expression(expression_factory * other_factory) const = 0;

  /** Class iterator enables iteration through all subexpressions 
      (recursively). It is a forward-only iterator. Subexpressions 
      are traversed in a pre-order fashion. */
  class iterator {
    friend class expression_node;
  private:
    std::vector<unsigned> _inds;
    std::vector<expression> _exps;

    /** private constructor. */
    iterator(const expression & e);

  public:

    /** default constructor. */
    iterator();
	
    /** prefix ++ operator (moving forward). Returns a reference
	to this iterator (the new value of the iterator). */
    const iterator & operator ++ ();

    /** postfix ++ operator (moving forward). Returns the old
	value of the iterator. */
    const iterator operator ++ (int);

    /** returns true if iterator it is equal to this iterator. */
    bool operator == (const iterator & it) const;

    /** returns true if iterator it is not equal to this iterator. */
    bool operator != (const iterator & it) const;

    /** returns sort currently pointed to by this iterator. */
    const expression & operator * () const;

    /** returns sort currently pointed to by this iterator.
	This enables using operator -> directly with iterators 
	(for instance it->get_symbol() instead of (*it)->get_symbol()). */
    const expression & operator -> () const;
  };

  /** All iterators are also const_iterators. */
  typedef iterator const_iterator;

  /** Returns the iterator that points to the root sort node (i.e. this
      sort). */
  iterator begin();

  /** Returns the invalid iterator (i.e. iterator obtained by applying
      ++ operator to the last valid iterator). */
  iterator end();

  /** Returns a string representation of the expression node. */
  std::string to_string() const;

  /** Returns the complexity of the expression node. */
  virtual unsigned get_complexity() const = 0;
  


  virtual bool equal_to(const expression_node * node) const = 0;  
  std::size_t hash_code() const;
  virtual void out(std::ostream & ostr) const = 0;
  virtual bool less_than(const expression_node * node) const = 0;

  /** Virtual destructor. */
  virtual ~expression_node();
  
};


namespace std {

  template<>
  struct hash<expression_node*> :
    public unary_function<expression_node *, std::size_t> {
    std::size_t operator() (expression_node * s) const
    {
      return s->hash_code();
    }
  };
    
  template<>
  struct equal_to<expression_node*> :
    public binary_function<expression_node *, 
			   expression_node *, bool> {
    bool operator () (expression_node * s, 
		      expression_node * t) const
    {
      return  s->equal_to(t);
    }
  };
  
  template<>
  struct hash<expression> :
    public unary_function<expression, std::size_t> {
    std::size_t operator() (const expression & s) const
    {
      return s->hash_code();
    }
  };
}


/** Prints the expression to the output stream. If the expression has
    at least one non-private attribute, all such attributes will be printed
    (in the form (! exp <attributes>)). Private attributes are the attributes
    with keywords beginning with :! (for instance, :!private). All 
    other attributes are non-private. Private attributes are useful when 
    the user wants to attach some meta information to the expression that
    should not be printed. */
std::ostream & operator << (std::ostream & ostr, const expression & exp);

/** Returns true if e1 is lexicographically lower than e2. */
bool operator < (const expression & e1, const expression & e2);

/** Returns true if e1 is lexicographically greater than e2. */
bool operator > (const expression & e1, const expression & e2);

/** Returns true if e1 is lexicographically lower or equal to e2. */
bool operator <= (const expression & e1, const expression & e2);

/** Returns true if e1 is lexicographically greater or equal to e2. */
bool operator >= (const expression & e1, const expression & e2);

namespace std {
  template <>
  struct less<expression> {
    
    bool operator () (const expression & e1, const expression & e2) const
    {
      return e1 < e2;
    }
  };
}




/** Set of expressions. */
typedef std::unordered_set<expression> expression_set;

/** Represents the undefined expression. */
class undefined_expression_node : public expression_node {
  friend class expression_factory;
private:
  /** The constructor is private. The node can be created only
      within the expression factory. */
  undefined_expression_node(expression_factory * factory);
public:
  virtual type get_type() const;
  virtual const function_symbol & get_symbol() const;
  virtual const sort & get_sort() const;
  virtual const variable & get_variable() const;
  virtual const special_constant & get_special_constant() const;
  virtual quantifier get_quantifier() const;
  virtual const sorted_variable_set & get_variables();
  virtual const sorted_variable_set & get_free_variables();
  virtual const sorted_variable_vector & get_quantified_variables() const;
  virtual const variable_binding_vector & get_variable_bindings() const;
  virtual const expression & get_subexpression() const;
  virtual const expression_vector & get_operands() const;
  virtual sat_variable get_sat_variable() const;
  virtual sat_literal_polarity get_sat_literal_polarity() const;
  virtual expression get_instance(const substitution & sb) const;
  virtual bool is_instance(const expression & exp,
			   substitution & sb) const;
  
  virtual expression clone_expression(expression_factory * other_factory) const;

  virtual bool check_expression(const signature * sg,
				search_mode smode = S_CONTEXTUAL)  const; 
  virtual expression infer_sorts() const;
  virtual expression expand_expression(const signature * sg,
				       search_mode smode = S_CONTEXTUAL) const;
  virtual void accept_visitor(expression_visitor & visitor);

  virtual unsigned get_complexity() const;

  virtual bool equal_to(const expression_node * node) const;
  virtual void out(std::ostream & ostr) const; 
  virtual bool less_than(const expression_node * node) const;
  virtual expression eliminate_let_binders(const substitution & sub) const;
};


/** Represents the variable sort node. */
class variable_expression_node : public expression_node {
  friend class expression_factory;
private:
  variable _var;
  sort _sort;

  const sorted_variable_set * _all_vars;
  const sorted_variable_set * _free_vars;

  void calculate_hash();

  variable_expression_node(expression_factory * factory);

  /** The constructor is private. The node can be created only
      within the expression factory. */
  variable_expression_node(expression_factory * factory, 
			   const variable & var,
			   const sort & sr);
public:
  
  virtual type get_type() const;
  virtual const function_symbol & get_symbol() const;
  virtual const sort & get_sort() const;
  virtual const variable & get_variable() const;
  virtual const special_constant & get_special_constant() const;
  virtual quantifier get_quantifier() const;
  virtual const sorted_variable_set & get_variables();
  virtual const sorted_variable_set & get_free_variables();
  virtual const sorted_variable_vector & get_quantified_variables() const;
  virtual const variable_binding_vector & get_variable_bindings() const;
  virtual const expression & get_subexpression() const;
  virtual const expression_vector & get_operands() const;
  virtual sat_variable get_sat_variable() const;
  virtual sat_literal_polarity get_sat_literal_polarity() const;
  virtual expression get_instance(const substitution & sb) const;
  virtual bool is_instance(const expression & exp,
			   substitution & sb) const;
  virtual bool check_expression(const signature * sg,
				search_mode smode = S_CONTEXTUAL) const; 
  virtual expression infer_sorts() const;
  virtual expression expand_expression(const signature * sg,
				       search_mode smode = S_CONTEXTUAL) const;
  virtual void accept_visitor(expression_visitor & visitor);

  virtual expression clone_expression(expression_factory * other_factory) const;

  virtual unsigned get_complexity() const;

  virtual bool equal_to(const expression_node * node) const;
  virtual void out(std::ostream & ostr) const; 
  virtual bool less_than(const expression_node * node) const;
  virtual expression eliminate_let_binders(const substitution & sub) const;
};

/** Represents the special constant expression node. */
class special_constant_expression_node : public expression_node {
  friend class expression_factory;
private:
  special_constant _spec_const;
  sort _sort;

  void calculate_hash();

  special_constant_expression_node(expression_factory * factory);
  
  /** The constructor is private. The node can be created only
      within the expression factory. */
  special_constant_expression_node(expression_factory * factory,
				   const special_constant & c,
				   const sort & sr);
public:
  
  virtual type get_type() const;
  virtual const function_symbol & get_symbol() const;
  virtual const sort & get_sort() const;
  virtual const variable & get_variable() const;
  virtual const special_constant & get_special_constant() const;
  virtual quantifier get_quantifier() const;
  virtual const sorted_variable_set & get_variables();
  virtual const sorted_variable_set & get_free_variables();
  virtual const sorted_variable_vector & get_quantified_variables() const;
  virtual const variable_binding_vector & get_variable_bindings() const;
  virtual const expression & get_subexpression() const;
  virtual const expression_vector & get_operands() const;
  virtual sat_variable get_sat_variable() const;
  virtual sat_literal_polarity get_sat_literal_polarity() const;
  virtual expression get_instance(const substitution & sb) const;
  virtual bool is_instance(const expression & exp,
			   substitution & sb) const;  
  virtual bool check_expression(const signature * sg,
				search_mode smode = S_CONTEXTUAL) const;
  virtual expression infer_sorts() const; 
  virtual expression expand_expression(const signature * sg,
				       search_mode smode = S_CONTEXTUAL) const;
  virtual void accept_visitor(expression_visitor & visitor);

  virtual expression clone_expression(expression_factory * other_factory) const;

  virtual unsigned get_complexity() const;

  virtual bool equal_to(const expression_node * node) const;
  virtual void out(std::ostream & ostr) const; 
  virtual bool less_than(const expression_node * node) const;
  virtual expression eliminate_let_binders(const substitution & sub) const;
};

/** Represents function expression node.  */
class function_expression_node : public expression_node {
  friend class expression_factory;
private:
  function_symbol _symbol;
  sort _sort;
  const expression_vector * _operands;

  const sorted_variable_set * _all_vars;
  const sorted_variable_set * _free_vars;

  static expression_vector _dummy_expression_vector;
  
  void calculate_hash();

  function_expression_node(expression_factory * factory);


  /** The constructor is private. The node can be created only
      within the expression factory. */
  function_expression_node(expression_factory * factory,
			   const function_symbol & symbol, 
			   const expression_vector & operands,
			   const sort & sr);

  /** The constructor is private. The node can be created only
      within the expression factory. */
  function_expression_node(expression_factory * factory,
			   const function_symbol & symbol, 
			   expression_vector && operands,
			   const sort & sr);

public:
  virtual type get_type() const;
  virtual const function_symbol & get_symbol() const;
  virtual const sort & get_sort() const;
  virtual const variable & get_variable() const;
  virtual const special_constant & get_special_constant() const;
  virtual quantifier get_quantifier() const;
  virtual const sorted_variable_set & get_variables();
  virtual const sorted_variable_set & get_free_variables();
  virtual const sorted_variable_vector & get_quantified_variables() const;
  virtual const variable_binding_vector & get_variable_bindings() const;
  virtual const expression & get_subexpression() const;
  virtual const expression_vector & get_operands() const;
  virtual sat_variable get_sat_variable() const;
  virtual sat_literal_polarity get_sat_literal_polarity() const;
  virtual expression get_instance(const substitution & sb) const;
  virtual bool is_instance(const expression & exp,
			   substitution & sb) const;

  virtual bool check_expression(const signature * sg,
				search_mode smode = S_CONTEXTUAL) const;
  virtual expression infer_sorts() const; 
  virtual expression expand_expression(const signature * sg,
				       search_mode smode = S_CONTEXTUAL) const;
  virtual void accept_visitor(expression_visitor & visitor);

  virtual expression clone_expression(expression_factory * other_factory) const;

  virtual unsigned get_complexity() const;

  virtual bool equal_to(const expression_node * node) const;
  virtual void out(std::ostream & ostr) const; 
  virtual bool less_than(const expression_node * node) const;
  virtual expression eliminate_let_binders(const substitution & sub) const;

  virtual ~function_expression_node();
};

/** Represents the quantifier expression node. */
class quantifier_expression_node : public expression_node {
  friend class expression_factory;
private:
  quantifier _quantifier;
  sorted_variable_vector _vars;
  expression _expr;

  const sorted_variable_set * _all_vars;
  const sorted_variable_set * _free_vars;
  
  void calculate_hash();

  quantifier_expression_node(expression_factory * factory);


  /** The constructor is private. The node can be created only
      within the expression factory. */
  quantifier_expression_node(expression_factory * factory,
			     quantifier qn, 
			     const sorted_variable_vector & vars, 
			     const expression & expr);

  /** The constructor is private. The node can be created only
      within the expression factory. */
  quantifier_expression_node(expression_factory * factory,
			     quantifier qn, 
			     sorted_variable_vector && vars, 
			     const expression & expr);
  
public:
  virtual type get_type() const;
  virtual const function_symbol & get_symbol() const;
  virtual const sort & get_sort() const;
  virtual const variable & get_variable() const;
  virtual const special_constant & get_special_constant() const;
  virtual quantifier get_quantifier() const;
  virtual const sorted_variable_set & get_variables();
  virtual const sorted_variable_set & get_free_variables();
  virtual const sorted_variable_vector & get_quantified_variables() const;
  virtual const variable_binding_vector & get_variable_bindings() const;
  virtual const expression & get_subexpression() const;
  virtual const expression_vector & get_operands() const;
  virtual sat_variable get_sat_variable() const;
  virtual sat_literal_polarity get_sat_literal_polarity() const;
  virtual expression get_instance(const substitution & sb) const;
  virtual bool is_instance(const expression & exp,
			   substitution & sb) const;
  virtual bool check_expression(const signature * sg,
				search_mode smode = S_CONTEXTUAL) const;
  virtual expression infer_sorts() const;
  virtual expression expand_expression(const signature * sg,
				       search_mode smode = S_CONTEXTUAL) const;
  virtual void accept_visitor(expression_visitor & visitor);
    
  virtual expression clone_expression(expression_factory * other_factory) const;

  virtual unsigned get_complexity() const;

  virtual bool equal_to(const expression_node * node) const;
  virtual void out(std::ostream & ostr) const; 
  virtual bool less_than(const expression_node * node) const;
  virtual expression eliminate_let_binders(const substitution & sub) const;    

  virtual ~quantifier_expression_node();
};


/** Represents let expression node. */
class let_expression_node : public expression_node {
  friend class expression_factory;
private:  
  variable_binding_vector _bind;
  expression _expr;

  const sorted_variable_set * _all_vars;
  const sorted_variable_set * _free_vars;
  
  void calculate_hash();

  let_expression_node(expression_factory * factory);

  /** The constructor is private. The node can be created only
      within the expression factory. */
  let_expression_node(expression_factory * factory,
		      const variable_binding_vector & bind, 
		      const expression & expr);

  /** The constructor is private. The node can be created only
      within the expression factory. */
  let_expression_node(expression_factory * factory,
		      variable_binding_vector && bind, 
		      const expression & expr);

public:
  virtual type get_type() const;
  virtual const function_symbol & get_symbol() const;
  virtual const sort & get_sort() const;
  virtual const variable & get_variable() const;
  virtual const special_constant & get_special_constant() const;
  virtual quantifier get_quantifier() const;
  virtual const sorted_variable_set & get_variables();
  virtual const sorted_variable_set & get_free_variables();
  virtual const sorted_variable_vector & get_quantified_variables() const;
  virtual const variable_binding_vector & get_variable_bindings() const;
  virtual const expression & get_subexpression() const;
  virtual const expression_vector & get_operands() const;
  virtual sat_variable get_sat_variable() const;
  virtual sat_literal_polarity get_sat_literal_polarity() const;
  virtual expression get_instance(const substitution & sb) const;
  virtual bool is_instance(const expression & exp,
			   substitution & sb) const;
  virtual bool check_expression(const signature * sg,
				search_mode smode = S_CONTEXTUAL) const;
  virtual expression infer_sorts() const;
  virtual expression expand_expression(const signature * sg,
				       search_mode smode = S_CONTEXTUAL) const;
  virtual void accept_visitor(expression_visitor & visitor);
    
  virtual expression clone_expression(expression_factory * other_factory) const;

  virtual unsigned get_complexity() const;

  virtual bool equal_to(const expression_node * node) const;
  virtual void out(std::ostream & ostr) const; 
  virtual bool less_than(const expression_node * node) const;
  virtual expression eliminate_let_binders(const substitution & sub) const;    

  virtual ~let_expression_node();
};

/** Represents SAT literal expression node. */
class sat_literal_expression_node : public expression_node {
  friend class expression_factory;
private:  
  sat_variable _variable;
  sat_literal_polarity _polarity;

  void calculate_hash();

  sat_literal_expression_node(expression_factory * factory);

  /** The constructor is private. The node can be created only
      within the expression factory. */
  sat_literal_expression_node(expression_factory * factory,
			      sat_variable vr, 
			      sat_literal_polarity polarity);
public:
  virtual type get_type() const;
  virtual const function_symbol & get_symbol() const;
  virtual const sort & get_sort() const;
  virtual const variable & get_variable() const;
  virtual const special_constant & get_special_constant() const;
  virtual quantifier get_quantifier() const;
  virtual const sorted_variable_set & get_variables();
  virtual const sorted_variable_set & get_free_variables();
  virtual const sorted_variable_vector & get_quantified_variables() const;
  virtual const variable_binding_vector & get_variable_bindings() const;
  virtual const expression & get_subexpression() const;
  virtual const expression_vector & get_operands() const;
  virtual sat_variable get_sat_variable() const;
  virtual sat_literal_polarity get_sat_literal_polarity() const;
  virtual expression get_instance(const substitution & sb) const;
  virtual bool is_instance(const expression & exp,
			   substitution & sb) const;
  virtual bool check_expression(const signature * sg,
				search_mode smode = S_CONTEXTUAL) const;
  virtual expression infer_sorts() const;
  virtual expression expand_expression(const signature * sg,
				       search_mode smode = S_CONTEXTUAL) const;
  virtual void accept_visitor(expression_visitor & visitor);
    
  virtual expression clone_expression(expression_factory * other_factory) const;

  virtual unsigned get_complexity() const;

  virtual bool equal_to(const expression_node * node) const;
  virtual void out(std::ostream & ostr) const; 
  virtual bool less_than(const expression_node * node) const;
  virtual expression eliminate_let_binders(const substitution & sub) const;

};


/** Represents a definition of a function symbol.  
    A function symbol is defined with a vector of sorted variables and an
    expression in which these free sorted variables (and only these) exist.
    Such definitions are provided by the SMT-LIB 2.0 scripts and are 
    used for expression expansion. */
class function_definition {
  const sorted_variable_vector * _vars;
  expression _exp;
public:
  /** Constructor. */
  function_definition(const sorted_variable_vector & vars, 
		      const expression & exp);

  /** Constructor. */
  function_definition(sorted_variable_vector && vars, 
		      const expression & exp);

  
  /** Copy constructor. */
  function_definition(const function_definition & def);

  /** Move constructor. */
  function_definition(function_definition && def);

  
  /** Assignment operator. */
  function_definition & operator = (const function_definition & def);

  /** Move assignment operator. */
  function_definition & operator = (function_definition && def);
  
  /** Gets the variables of the definition. */
  const sorted_variable_vector & get_variables() const;

  /** Gets the expression of the definition. */
  const expression & get_expression() const;

  void out(std::ostream & ostr) const;

  /** Destructor. */
  ~function_definition();

};

/** Prints the definition to the output stream. */
std::ostream & operator << (std::ostream & ostr, 
			    const function_definition & def);

/** Standard attribute value for function_definition. Function definitions are 
    attached to function symbols as the value of :definition attribute. */
typedef standard_attribute_value<function_definition> 
function_definition_attribute_value;
  

/** Type of exception thrown when quantifiers are applied to non-boolean
    subexpression. */
class quantified_non_boolean_exception : public base_exception {
  expression _exp;
public:
  /** Constructor. */
  quantified_non_boolean_exception(const std::string & message, 
				   const expression & exp)
    :base_exception(message + 
		    std::string(": quantified non-boolean expression: ") +
		    exp->to_string() + " of sort: " + exp->get_sort()->to_string()),
     _exp(exp)
  {}

  /** Gets the (non-boolean) expression. */
  const expression & get_expression() const
  {
    return _exp;
  }

};

/** Type of exception thrown when bad function symbol is used. */
class bad_function_symbol_exception : public base_exception {
private:
  function_symbol _symbol;
public:
  /** Constructor. */
  bad_function_symbol_exception(const std::string & message, 
				const function_symbol & symbol)
    :base_exception(message + 
		    std::string(": bad function symbol: ") + symbol.to_string()),
     _symbol(symbol)
  {}
  
  /** Gets the (bad) function symbol. */
  const function_symbol & get_symbol() const
  {
    return _symbol;
  }
};


/** Type of exception thrown when bad special constant is used. */
class bad_special_constant_exception : public base_exception {
private:
  special_constant _spec_const;
public:
  /** Constructor. */
  bad_special_constant_exception(const std::string & message, 
				 const special_constant & spec_const)
    :base_exception(message + std::string(": bad special constant: ") + 
		    spec_const.to_string()),
     _spec_const(spec_const)
  {}
  
  /** Gets the (bad) special constant. */
  const special_constant & get_constant() const
  {
    return _spec_const;
  }
};

/** Type of exception thrown when bad expression is used. */
class bad_expression_exception : public base_exception {
protected:
  expression _exp;
public:

  /** Constructor. */
  bad_expression_exception(const std::string & message, const expression & exp)
    :base_exception(message + std::string(": bad expression: ") + exp->to_string()),
     _exp(exp)
  {}
  
  /** Gets the (bad) expression. */
  const expression & get_expression() const
  {
    return _exp;
  }
};


/** Represents the factory in which expression nodes reside. 
    It implements hash table in which nodes are stored. It is 
    responsible for hash consing and garbage collecting of expression 
    nodes. */
class expression_factory {
  friend class expression_node;

private:
  class expression_deleter {
  private:
    expression_factory * _factory;
  public:
    expression_deleter(expression_factory * factory)
      :_factory(factory)
    {}

    void operator () (expression_node * node)
    {
      _factory->remove_expression(node);
    }
  };


  std::unordered_set<expression_node *> _expressions;

  void remove_expression(expression_node * node);
  expression insert_expression(expression_node * node);


  expression _undefined_expression;
  expression _true_expression;
  expression _false_expression;

  sort set_sort(const sort & sr) const;

  signature * _signature;
  bool _destruction;

  variable_expression_node _var_node;
  special_constant_expression_node _spec_node;
  function_expression_node _fun_node;
  quantifier_expression_node _quant_node;
  let_expression_node _let_node;
  sat_literal_expression_node _sat_node;
    
  bool _weak_compare;

public:
  /** Constructor creates a factory over the given signature. */
  expression_factory(signature * sg);

  /** Returns a pointer to the signature over which the factory is 
      created. */
  signature * get_signature() const;

  /** Returns undefined expression. */
  const expression & UNDEFINED_EXPRESSION() const;

  /** Returns true constant expression (a value of Bool sort). */
  const expression & TRUE_EXPRESSION() const;

  /** Returns false constant expression (a value of Bool sort). */
  const expression & FALSE_EXPRESSION() const;


  /** Creates variable expression node and returns a shared
      pointer to the corresponding expression. The expression is created
      if it does not already exist in the factory. Otherwise, a shared
      pointer to existing node is returned. If the sort argument
      is provided, then it must not be parametric, and it determines the 
      sort of the variable expression. Otherwise, the sort of the 
      expression is set to undefined. Notice that sort must be provided 
      when well-sortedness checking is on (which is default). */
  expression create_expression(const variable & var, 
			       const sort & sr = sort());

  /** Creates special constant expression node and returns a shared
      pointer to the corresponding expression. The expression is created
      if it does not already exist in the factory. Otherwise, a shared
      pointer to existing node is returned. If the sort argument is 
      provided, then it must not be parametric, and it determines the
      sort of the special constant expression. If sort is 
      not provided, then it is infered (if syntax checking is on),
      or set to undefined (if syntax checking is off). The 
      sort should be provided if the special constant's sort is not uniquely
      determined in the signature. */
  expression create_expression(const special_constant & c,
			       const sort & sr = sort());

  /** Creates function expression node and returns a shared
      pointer to the corresponding expression. The expression is created
      if it does not already exist in the factory. Otherwise, a shared
      pointer to existing node is returned. If sort argument is 
      provided, then it must not be parametric, and it determines the 
      sort of the function expression. If sort is not provided, then 
      it is infered (if syntax checking is on), or set to undefined 
      (if syntax checking is off). The sort should be provided if the
      sort of created expression cannot be uniquely determined in the
      signature. The second arguments represents the subexpressions to 
      which the function symbol is applied. */
  expression create_expression(const function_symbol & symbol, 
			       const expression_vector & expressions,
			       const sort & sr = sort());

  /** Creates function expression node and returns a shared
      pointer to the corresponding expression. The expression is created
      if it does not already exist in the factory. Otherwise, a shared
      pointer to existing node is returned. If sort argument is 
      provided, then it must not be parametric, and it determines the 
      sort of the function expression. If sort is not provided, then 
      it is infered (if syntax checking is on), or set to undefined 
      (if syntax checking is off). The sort should be provided if the
      sort of created expression cannot be uniquely determined in the
      signature. The second arguments represents the subexpressions to 
      which the function symbol is applied. */
  expression create_expression(const function_symbol & symbol, 
			       expression_vector && expressions,
			       const sort & sr = sort());

  
  /** Creates function expression node with no subexpressions (constant
      function) and returns a shared pointer to the corresponding 
      expression. The expression is created if it does not already 
      exist in the factory. Otherwise, a shared pointer to existing node 
      is returned. If sort argument is provided, then it must not 
      be parametric, and it determines the sort of the constant function
      expression. If sort is not provided, then it is infered 
      (if syntax checking is on), or set to undefined (if syntax checking 
      is off). The sort must be provided if the sort of created expression
      cannot be uniquely determined in the signature. */
  expression create_expression(const function_symbol & symbol,
			       const sort & sr = sort());

  /** Creates function expression node with one subexpression and returns 
      a shared pointer to the corresponding expression. The expression is 
      created if it does not already exist in the factory. Otherwise, a shared
      pointer to existing node is returned. If sort argument is provided, 
      then it must not be parametric, and it determines the sort of the 
      function expression. If sort is not provided, then it is infered 
      (if syntax checking is on), or set to undefined (if syntax checking 
      is off). The sort must be provided if the sort of created expression
      cannot be uniquely determined in the signature. The parameter exp
      represents a subexpression to which the symbol is applied. */
  expression create_expression(const function_symbol & symbol, 
			       const expression & exp,
			       const sort & sr = sort());


  /** Creates function expression node with two subexpressions and returns 
      a shared pointer to the corresponding expression. The expression is 
      created if it does not already exist in the factory. Otherwise, a shared
      pointer to existing node is returned. If sort argument is provided, 
      then it must not be parametric, and it determines the sort of the 
      function expression. If sort is not provided, then it is infered 
      (if syntax checking is on), or set to undefined (if syntax checking 
      is off). The sort must be provided if the sort of created expression
      cannot be uniquely determined in the signature. The parameters exp1
      and exp2 represent subexpressions to which the symbol is applied. */
  expression create_expression(const function_symbol & symbol, 
			       const expression & exp1, 
			       const expression & exp2,
			       const sort & sr = sort());


  /** Creates function expression node with three subexpressions and returns 
      a shared pointer to the corresponding expression. The expression is 
      created if it does not already exist in the factory. Otherwise, a shared
      pointer to existing node is returned. If sort argument is provided, 
      then it must not be parametric, and it determines the sort of the 
      function expression. If sort is not provided, then it is infered 
      (if syntax checking is on), or set to undefined (if syntax checking 
      is off). The sort must be provided if the sort of created expression
      cannot be uniquely determined in the signature. The parameters exp1,
      exp2 and exp3 represent subexpressions to which the symbol is 
      applied. */
  expression create_expression(const function_symbol & symbol, 
			       const expression & exp1, 
			       const expression & exp2,
			       const expression & exp3,
			       const sort & sr = sort());

  /** Creates quantifier expression node with given quantifier, quantified
      sorted variables and (boolean) subexpression and returns 
      a shared pointer to the corresponding expression. The expression is 
      created if it does not already exist in the factory. 
      Otherwise, a shared pointer to existing node is returned.  */
  expression create_expression(quantifier qn, 
			       const sorted_variable_vector & vars,
			       const expression & exp);

  /** Creates quantifier expression node with given quantifier, quantified
      sorted variables and (boolean) subexpression and returns 
      a shared pointer to the corresponding expression. The expression is 
      created if it does not already exist in the factory. 
      Otherwise, a shared pointer to existing node is returned.  */
  expression create_expression(quantifier qn, 
			       sorted_variable_vector && vars,
			       const expression & exp);

  
  /** Creates let expression node with given variable bindings
      and subexpression and returns a shared pointer to the 
      corresponding expression. The expression is created if it 
      does not already exist in the factory. Otherwise, a shared
      pointer to existing node is returned.  */
  expression create_expression(const variable_binding_vector & bind,
			       const expression & exp);

  /** Creates let expression node with given variable bindings
      and subexpression and returns a shared pointer to the 
      corresponding expression. The expression is created if it 
      does not already exist in the factory. Otherwise, a shared
      pointer to existing node is returned.  */
  expression create_expression(variable_binding_vector && bind,
			       const expression & exp);

  
  /** Creates SAT literal expression node with given variable number
      and polarity and returns a shared pointer to the 
      corresponding expression. The expression is created if it 
      does not already exist in the factory. Otherwise, a shared
      pointer to existing node is returned.  */
  expression create_expression(sat_variable vr, 
			       sat_literal_polarity polarity);

    
  /** Destructor */
  ~expression_factory();
  
};

/** Represents an abstract visitor that can 
    be applied to expression nodes. The visitor is applied to all nodes 
    in the expression in a depth-first order. */
class expression_visitor {
public:
  /** Method is applied to the undefined expression node. */
  virtual void 
  visit_undefined_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  /** Method is applied to a variable expression node. */
  virtual void 
  visit_variable_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  /** Method is applied to a special constant expression node. */
  virtual void
  visit_special_constant_expression_node(const expression & node)
  {
    // DO NOTHING
  }

  /** Method is applied to a function expression node before its 
      subexpressions are visited. */
  virtual void visit_function_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  /** Method is applied to a function expression node after its subexpressions
      are visited. */
  virtual void visit_function_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  /** Method is applied to a quantifier expression node before its 
      subexpression is visited. */
  virtual void visit_quantifier_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }

  /** Method is applied to a quantifier expression node after its 
      subexpression is visited. */
  virtual void visit_quantifier_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  /** Method is applied to a let expression node before its subexpression
      and binding expressions are visited. */
  virtual void visit_let_expression_node_preorder(const expression & node)
  {
    // DO NOTHING
  }
    
  /** Method is applied to a let expression node before its subexpression
      is visited, but after the binding expressions are visited. */
  virtual void visit_let_expression_node_inorder(const expression & node)
  {
    // DO NOTHING
  }
    
  /** Method is applied to a let expression node after its subexpression
      and binding expressions are visited. */
  virtual void visit_let_expression_node_postorder(const expression & node)
  {
    // DO NOTHING
  }

  /** Method is applied to a SAT literal expression node. */
  virtual void visit_sat_literal_expression_node(const expression & node)
  {
    // DO NOTHING
  }
    
  /** Virtual destructor */
  virtual ~expression_visitor() {}
};


// INLINE FUNCTION DEFINITIONS


inline
sorted_variable::sorted_variable(const variable & var, const sort & sr)
  :_var(var),
   _sort(sr)
{}
  
inline
const variable & sorted_variable::get_variable() const
{
  return _var;
}

inline
const sort & sorted_variable::get_sort() const
{
  return _sort;
}

inline
bool sorted_variable::operator == (const sorted_variable & sv) const
{
  return _var == sv._var && _sort == sv._sort;
}

inline
bool sorted_variable::operator != (const sorted_variable & sv) const
{
  return _var != sv._var || _sort != sv._sort;
}

inline
bool sorted_variable::operator < (const sorted_variable & sv) const
{
  return _var < sv._var || (_var == sv._var && _sort < sv._sort);
}

inline
bool sorted_variable::operator > (const sorted_variable & sv) const
{
  return sv < *this;
}

inline
bool sorted_variable::operator <= (const sorted_variable & sv) const
{
  return *this == sv || *this < sv;
}

inline
bool sorted_variable::operator >= (const sorted_variable & sv) const
{
  return *this == sv || *this > sv;
}


inline
std::size_t sorted_variable::hash_code() const
{
  return _var.hash_code();
}

inline
void sorted_variable::out(std::ostream & ostr) const
{
  ostr << "(" << _var << " " << _sort << ")";
}

inline
std::string sorted_variable::to_string() const
{
  return ::to_string(this);
}

inline
std::ostream & operator << (std::ostream & ostr, const sorted_variable & sv)
{
  sv.out(ostr);
  return ostr;
}

  

inline
expression_node::expression_node(expression_factory * factory)
  :_factory(factory),
   _attributes(0),
   _hash_code(0),
   _assigned(factory->UNDEFINED_EXPRESSION()),
   _expression_data(0)
{}

inline
bool expression_node::sorts_equal(const sort & s1, const sort & s2) const
{
  if(s1 == s2)
    return true;
    
  if(_factory->_weak_compare)
    return 
      s1 == _factory->get_signature()->get_sort_factory()->UNDEFINED_SORT() || 
      s2 == _factory->get_signature()->get_sort_factory()->UNDEFINED_SORT();
  else
    return false;
}
  
inline
bool expression_node::has_attributes() const
{
  return _attributes != 0 && !_attributes->empty();
}

inline
attribute_set & expression_node::get_attributes()
{
  if(_attributes)
    return *_attributes;
  else
    return *(_attributes = new attribute_set(HASH_TABLE_SIZE));
}

inline
bool expression_node::is_attribute_set(const keyword & kw) const
{
  return get_attribute_value(kw) != 0;
}

inline
attribute_value * 
expression_node::get_attribute_value(const keyword & kw) const
{
  if(_attributes == 0)
    return 0;
    
  attribute_set::const_iterator it = _attributes->find(kw);
  if(it != _attributes->end())
    {
      return it->get_value();
    }
  else
    return 0;
}
  
  
template <typename T>
inline
void 
expression_node::get_attribute_value(const keyword & kw, T & value) const
{
  if(_attributes == 0)
    throw no_attribute_value_exception("Sort attribute not set", kw);
    
  attribute_set::const_iterator it = _attributes->find(kw);
  if(it != _attributes->end())
    {
      it->get_value(value);
    }
  else
    throw no_attribute_value_exception("Sort attribute not set", kw);
}
  
template <typename T>
inline
void 
expression_node::set_attribute_value(const keyword & kw, T && value)
{
  set_attribute(attribute(kw, std::forward<T>(value)));
}
  
inline
void 
expression_node::set_attribute(const attribute & attr)
{
  attribute_set & attributes = get_attributes();
  attributes.erase(attr);
  attributes.insert(attr);
}

inline
void 
expression_node::set_attribute(attribute && attr)
{
  attribute_set & attributes = get_attributes();
  attributes.erase(attr);
  attributes.insert(std::move(attr));
}

inline
const expression & expression_node::get_assigned() const
{
  return _assigned;
}

inline
void expression_node::set_assigned(const expression & assigned)
{
  _assigned = assigned;
}

inline
bool expression_node::is_assigned() const
{
  return _assigned != _factory->UNDEFINED_EXPRESSION();
}

inline
expression_data * expression_node::get_expression_data() const
{
  return _expression_data;
}

inline
void expression_node::set_expression_data(expression_data * ed)
{
  _expression_data = ed;
}
  
inline
bool expression_node::has_expression_data() const
{
  return _expression_data != 0;
}

inline
expression_factory * expression_node::get_factory() const
{
  return _factory;
}

inline 
const sort & expression_node::get_inferred_sort() const
{
  return get_sort() != _factory->get_signature()->get_sort_factory()->UNDEFINED_SORT() ?
    get_sort() : infer_sorts()->get_sort();
}

inline
bool expression_node::is_formula() const
{
  return get_inferred_sort() == _factory->get_signature()->get_sort_factory()->BOOL_SORT(); 
}

inline
bool expression_node::is_ground()
{
  return get_variables().empty();
}

inline
bool expression_node::is_closed()
{
  return get_free_variables().empty();
}

inline
std::string expression_node::to_string() const
{
  return ::to_string(this);
}

inline
expression expression_node::expand_expression() const
{
  return this->expand_expression(_factory->get_signature(), S_CONTEXTUAL);
}


inline
bool expression_node::is_undefined() const
{
  return get_type() == T_UNDEF;
}

inline
bool expression_node::is_variable() const
{
  return get_type() == T_VARIABLE;
}
  
inline
bool expression_node::is_special_constant() const
{
  return get_type() == T_SPECIAL_CONSTANT;
}


inline
bool expression_node::is_numeral_constant() const
{
  return get_special_constant().get_type() == special_constant::T_NUMERAL;
}

inline
bool expression_node::is_binary_constant() const
{
  return get_special_constant().get_type() == special_constant::T_BINARY;
}
  
inline
bool expression_node::is_hexadecimal_constant() const
{
  return get_special_constant().get_type() == special_constant::T_HEXADECIMAL;	
}

inline
bool expression_node::is_decimal_constant() const
{
  return get_special_constant().get_type() == special_constant::T_DECIMAL;
}

inline
bool expression_node::is_string_literal_constant() const
{
  return get_special_constant().get_type() == special_constant::T_STRING_LITERAL;
}

inline
bool expression_node::is_function() const
{
  return get_type() == T_FUNCTION;
}

inline
bool expression_node::is_constant_function() const
{
  return get_type() == T_FUNCTION && get_operands().size() == 0;
}

inline
bool expression_node::is_quantifier() const
{
  return get_type() == T_QUANTIFIER;
}

inline
bool expression_node::is_forall() const
{
  return get_quantifier() == Q_FORALL;
}

inline
bool expression_node::is_exists() const
{
  return get_quantifier() == Q_EXISTS;
}
  
inline
bool expression_node::is_let() const
{
  return get_type() == T_LET;
}

inline
bool expression_node::is_sat_literal() const
{
  return get_type() == T_SAT;
}

inline
bool expression_node::is_positive_sat_literal() const
{
  return get_sat_literal_polarity() == SLP_POSITIVE;
}

inline
bool expression_node::is_negative_sat_literal() const
{
  return get_sat_literal_polarity() == SLP_NEGATIVE;
}

inline
bool expression_node::is_true() const
{
  return get_symbol() == function_symbol::TRUE;
}

inline
bool expression_node::is_false() const
{
  return get_symbol() == function_symbol::FALSE;
}

inline
bool expression_node::is_not() const
{
  return get_symbol() == function_symbol::NOT;
}

inline
bool expression_node::is_implication() const
{
  return get_symbol() == function_symbol::IMPLIES;
}

inline
bool expression_node::is_and() const
{
  return get_symbol() == function_symbol::AND;
}

inline
bool expression_node::is_or() const
{
  return get_symbol() == function_symbol::OR;
}

inline
bool expression_node::is_xor() const
{
  return get_symbol() == function_symbol::XOR;
}

inline
bool expression_node::is_equality() const
{
  return get_symbol() == function_symbol::EQ;
}

inline
bool expression_node::is_equivalence() const
{
  return get_symbol() == function_symbol::EQ && get_operands()[0]->is_formula();
}

inline
bool expression_node::is_disequality() const
{
  return get_operands().size() == 2 && 
    get_symbol() == function_symbol::DISTINCT; 
}
 
inline
bool expression_node::is_distinct() const
{
  return get_symbol() == function_symbol::DISTINCT;
}

inline
bool expression_node::is_inequivalence()
{
  return get_symbol() == function_symbol::DISTINCT &&
    get_operands()[0]->is_formula();
}

inline
bool expression_node::is_ite() const
{
  return get_symbol() == function_symbol::ITE;
}

inline
std::size_t expression_node::hash_code() const
{
  return _hash_code;
}

inline
expression_node::~expression_node()
{
  delete _attributes;
  delete _expression_data;
}

inline
expression_node::iterator::iterator(const expression & e)
{
  _exps.push_back(e);
  _inds.push_back((unsigned)(-1));
}
    
inline
expression_node::iterator::iterator()
{}
	
inline
const expression_node::iterator expression_node::iterator::operator ++ (int)
{
  iterator it(*this);
  ++(*this);
  return it;
}
  
inline
bool expression_node::iterator::operator == (const iterator & it) const
{
  return _exps == it._exps && _inds == it._inds;
}
  
inline
bool expression_node::iterator::operator != (const iterator & it) const
{
  return _exps != it._exps || _inds != it._inds;
}
  
inline
const expression & expression_node::iterator::operator * () const
{
  return _exps.back();
}
  
inline
const expression & expression_node::iterator::operator -> () const
{
  return _exps.back();
}
  
inline
expression_node::iterator expression_node::begin()
{
  return iterator(this->shared_from_this());
}
  
inline
expression_node::iterator expression_node::end()
{
  return iterator();
}

inline
bool operator < (const expression & e1, const expression & e2)
{
  return e1->less_than(e2.get());
}

inline
bool operator > (const expression & e1, const expression & e2)
{
  return e2 < e1;
}

inline
bool operator <= (const expression & e1, const expression & e2)
{
  return e1 == e2 || e1 < e2;
}

inline
bool operator >= (const expression & e1, const expression & e2)
{
  return e1 == e2 || e1 > e2;
}

  
inline
undefined_expression_node::
undefined_expression_node(expression_factory * factory)
  :expression_node(factory)
{}

inline
expression_node::type 
undefined_expression_node::get_type() const
{
  return T_UNDEF;
}

inline
const function_symbol & 
undefined_expression_node::get_symbol() const
{
  return function_symbol::UNDEFINED;
}

inline
const sort & 
undefined_expression_node::get_sort() const
{
  return _factory->get_signature()->get_sort_factory()->UNDEFINED_SORT();
}

inline
const variable & 
undefined_expression_node::get_variable() const
{
  return variable::UNDEFINED;
}

inline
const special_constant & 
undefined_expression_node::get_special_constant() const
{
  return special_constant::UNDEFINED;
}

inline
quantifier undefined_expression_node::get_quantifier() const
{
  return Q_UNDEFINED;
}

inline
const sorted_variable_set & 
undefined_expression_node::get_variables()
{
  return EMPTY_SORTED_VARIABLE_SET;
}

inline
const sorted_variable_set & 
undefined_expression_node::get_free_variables()
{
  return EMPTY_SORTED_VARIABLE_SET;
}

inline
const sorted_variable_vector & 
undefined_expression_node::get_quantified_variables() const
{
  return EMPTY_SORTED_VARIABLE_VECTOR;
}

inline
const variable_binding_vector & 
undefined_expression_node::get_variable_bindings() const
{
  return EMPTY_VARIABLE_BINDING_VECTOR;
}

inline
const expression & undefined_expression_node::get_subexpression() const
{
  return this->get_factory()->UNDEFINED_EXPRESSION();
}

inline
const expression_vector & 
undefined_expression_node::get_operands() const
{
  return EMPTY_EXPRESSION_VECTOR;
}

inline
sat_variable undefined_expression_node::get_sat_variable() const
{
  return UNDEFINED_SAT_VARIABLE;
}

inline
sat_literal_polarity undefined_expression_node::get_sat_literal_polarity() const
{
  return SLP_UNDEFINED;
}


inline
bool 
undefined_expression_node::equal_to(const expression_node * node) const
{
  return node->get_type() == T_UNDEF;
}
  
inline
void 
undefined_expression_node::out(std::ostream & ostr) const
{
  ostr << "UNDEFINED";
}

inline
bool undefined_expression_node::less_than(const expression_node * node) const
{
  return false;
}

inline
expression undefined_expression_node::
get_instance(const substitution & sb) const
{
  return const_cast<undefined_expression_node*>(this)->shared_from_this();
}

inline
bool undefined_expression_node::is_instance(const expression & exp,
					    substitution & sb) const
{
  return exp->is_undefined();
}


inline
bool 
undefined_expression_node::check_expression(const signature * sg,
					    search_mode smode) const
{
  return false;
}

inline
expression undefined_expression_node::infer_sorts() const
{
  return const_cast<undefined_expression_node*>(this)->shared_from_this();
}

inline
expression undefined_expression_node::
expand_expression(const signature * sg,
		  search_mode smode) const
{
  return const_cast<undefined_expression_node*>(this)->shared_from_this();
}

inline
expression undefined_expression_node::eliminate_let_binders(const substitution & sub) const
{
  return const_cast<undefined_expression_node *>(this)->shared_from_this();
}

inline
void undefined_expression_node::accept_visitor(expression_visitor & visitor)
{
  visitor.visit_undefined_expression_node(this->shared_from_this());
}

inline
expression undefined_expression_node::clone_expression(expression_factory * other_factory) const
{
  return other_factory->UNDEFINED_EXPRESSION();
}

inline
unsigned undefined_expression_node::get_complexity() const
{
  return 0;
}


inline
void variable_expression_node::calculate_hash()
{
  _hash_code = _var.hash_code();
}

inline
variable_expression_node::
variable_expression_node(expression_factory * factory,
			 const variable & var,
			 const sort & sr)
  :expression_node(factory),
   _var(var),
   _sort(sr),
   _all_vars(0),
   _free_vars(0)
{
  if(_factory->get_signature()->get_syntax_checking() &&
     (_sort == _factory->get_signature()->get_sort_factory()->UNDEFINED_SORT()
      || _sort->has_parameters()))
    throw bad_sort_exception("Expression (variable) cannot be instantiated", _sort);
  
  calculate_hash();
}

inline
variable_expression_node::
variable_expression_node(expression_factory * factory)
  :expression_node(factory),
   _var(variable::UNDEFINED),
   _sort(),
   _all_vars(0),
   _free_vars(0)
{}


inline
expression_node::type 
variable_expression_node::get_type() const
{
  return T_VARIABLE;
}

inline
const function_symbol & 
variable_expression_node::get_symbol() const
{
  return function_symbol::UNDEFINED;
}

inline
const sort & 
variable_expression_node::get_sort() const
{
  return _sort;
}

inline
const variable & 
variable_expression_node::get_variable() const
{
  return _var;
}

inline
const special_constant & 
variable_expression_node::get_special_constant() const
{
  return special_constant::UNDEFINED;
}

inline
quantifier variable_expression_node::get_quantifier() const
{
  return Q_UNDEFINED;
}

inline
const sorted_variable_set & 
variable_expression_node::get_variables()
{
  if(_all_vars == 0)
    {
      sorted_variable_set * temp = new sorted_variable_set(HASH_TABLE_SIZE);
      temp->insert(sorted_variable(_var, _sort));
      return *(_all_vars = temp);
    }
  return *_all_vars;
}

inline
const sorted_variable_set & 
variable_expression_node::get_free_variables()
{
  if(_free_vars == 0)
    {
      sorted_variable_set * temp = new sorted_variable_set(HASH_TABLE_SIZE);
      temp->insert(sorted_variable(_var, _sort));
      return *(_free_vars = temp);
    }
  return *_free_vars;
}


inline
const sorted_variable_vector & 
variable_expression_node::get_quantified_variables() const
{
  return EMPTY_SORTED_VARIABLE_VECTOR;
}

inline
const variable_binding_vector & 
variable_expression_node::get_variable_bindings() const
{
  return EMPTY_VARIABLE_BINDING_VECTOR;
}

inline
const expression & variable_expression_node::get_subexpression() const
{
  return this->get_factory()->UNDEFINED_EXPRESSION();
}

inline
const expression_vector & 
variable_expression_node::get_operands() const
{
  return EMPTY_EXPRESSION_VECTOR;
}

inline
sat_variable variable_expression_node::get_sat_variable() const
{
  return UNDEFINED_SAT_VARIABLE;
}

inline
sat_literal_polarity variable_expression_node::get_sat_literal_polarity() const
{
  return SLP_UNDEFINED;
}

inline
bool 
variable_expression_node::equal_to(const expression_node * node) const
{
  return node->get_type() == T_VARIABLE && 
    _var == node->get_variable() &&
    sorts_equal(_sort, node->get_sort());
}

inline
bool variable_expression_node::less_than(const expression_node * node) const
{
  return get_type() < node->get_type() ||
    (get_type() == node->get_type() &&
     (_var < node->get_variable() || 
      (_var == node->get_variable() &&
       _sort < node->get_sort())));
}
  
inline
expression variable_expression_node::
get_instance(const substitution & sb) const
{
  substitution::const_iterator it = 
    sb.find(_var);
  
  if(it != sb.end() && _sort == it->second->get_sort())
    return it->second;      
  else
    return const_cast<variable_expression_node*>(this)->shared_from_this();
}

inline
bool 
variable_expression_node::check_expression(const signature * sg,
					   search_mode smode) const
{
  return _sort->check_sort(sg, smode) && !_sort->has_parameters(); 
}

inline
expression variable_expression_node::infer_sorts() const
{
  if(_factory->get_signature()->get_syntax_checking())
    return const_cast<variable_expression_node*>(this)->shared_from_this();

  if(_sort != _factory->get_signature()->get_sort_factory()->UNDEFINED_SORT()
     && !_sort->has_parameters())
    return const_cast<variable_expression_node*>(this)->shared_from_this();
  else
    throw bad_sort_exception("Cannot infer expression (variable) sort", _sort);
}


inline
bool variable_expression_node::is_instance(const expression & exp,
					   substitution & sb) const
{
  substitution::const_iterator it = 
    sb.find(_var);
  
  if(it != sb.end())
    return it->second == exp;
  else if(_sort == exp->get_sort())
    {
      sb.insert(std::make_pair(_var, exp));
      return true;
    }
  else
    return false;
}


inline
void 
variable_expression_node::out(std::ostream & ostr) const
{
  ostr << _var.get_name();
}

inline
expression variable_expression_node::
expand_expression(const signature * sg,
		  search_mode smode) const
{
  if(_sort == _factory->get_signature()->get_sort_factory()->UNDEFINED_SORT()
     || _sort->has_parameters())
    throw bad_sort_exception("Cannot expand expression (variable)", _sort);
  
  return _factory->create_expression(_var, _sort->expand_sort(sg, smode));
}

inline
expression variable_expression_node::eliminate_let_binders(const substitution & sub) const
{
  substitution::const_iterator it = 
    sub.find(_var);
  
  if(it != sub.end() && _sort == it->second->get_sort())
    return it->second;      
  else
    return const_cast<variable_expression_node*>(this)->shared_from_this();
}

inline
void 
variable_expression_node::accept_visitor(expression_visitor & visitor)
{
  visitor.visit_variable_expression_node(this->shared_from_this());
}

inline
expression variable_expression_node::clone_expression(expression_factory * other_factory) const
{
  return other_factory->create_expression(_var, _sort);
}

inline
unsigned variable_expression_node::get_complexity() const
{
  return 1;
}


inline
void special_constant_expression_node::calculate_hash()
{
  _hash_code = _spec_const.hash_code();
}

inline
special_constant_expression_node::
special_constant_expression_node(expression_factory * factory)
  :expression_node(factory),
   _spec_const(special_constant::UNDEFINED),
   _sort()
{}


inline
special_constant_expression_node::
special_constant_expression_node(expression_factory * factory,
				 const special_constant & spec_const,
				 const sort & sr)
  :expression_node(factory),
   _spec_const(spec_const),
   _sort(sr)
{
  if(factory->get_signature()->get_syntax_checking() &&
     !factory->get_signature()->check_special_constant(spec_const, _sort))
    throw bad_special_constant_exception("Expression (special constant) cannot be instantiated", _spec_const);

  calculate_hash();
}

inline
expression_node::type 
special_constant_expression_node::get_type() const
{
  return T_SPECIAL_CONSTANT;
}

inline
const function_symbol & 
special_constant_expression_node::get_symbol() const
{
  return function_symbol::UNDEFINED;
}

inline
const sort & 
special_constant_expression_node::get_sort() const
{
  return _sort;
}

inline
const variable & 
special_constant_expression_node::get_variable() const
{
  return variable::UNDEFINED;
}

inline
const special_constant & 
special_constant_expression_node::get_special_constant() const
{
  return _spec_const;
}

inline
quantifier special_constant_expression_node::get_quantifier() const
{
  return Q_UNDEFINED;
}

inline
const sorted_variable_set & 
special_constant_expression_node::get_variables()
{
  return EMPTY_SORTED_VARIABLE_SET;
}

inline
const sorted_variable_set & 
special_constant_expression_node::get_free_variables()
{
  return EMPTY_SORTED_VARIABLE_SET;
}


inline
const sorted_variable_vector & 
special_constant_expression_node::get_quantified_variables() const
{
  return EMPTY_SORTED_VARIABLE_VECTOR;
}

inline
const variable_binding_vector & 
special_constant_expression_node::get_variable_bindings() const
{
  return EMPTY_VARIABLE_BINDING_VECTOR;
}


inline
const expression & 
special_constant_expression_node::get_subexpression() const
{
  return this->get_factory()->UNDEFINED_EXPRESSION();
}

inline
const expression_vector & 
special_constant_expression_node::get_operands() const
{
  return EMPTY_EXPRESSION_VECTOR;
}

inline
sat_variable special_constant_expression_node::get_sat_variable() const
{
  return UNDEFINED_SAT_VARIABLE;
}

inline
sat_literal_polarity special_constant_expression_node::get_sat_literal_polarity() const
{
  return SLP_UNDEFINED;
}

inline
bool 
special_constant_expression_node::equal_to(const expression_node * node) const
{
  return node->get_type() == T_SPECIAL_CONSTANT && 
    _spec_const == node->get_special_constant() &&
    sorts_equal(_sort, node->get_sort());
}

inline
void 
special_constant_expression_node::out(std::ostream & ostr) const
{
  _spec_const.out(ostr);
}

inline
bool 
special_constant_expression_node::less_than(const expression_node * node) const
{
  return get_type() < node->get_type() ||
    (get_type() == node->get_type() &&
     (_spec_const < node->get_special_constant() ||
      (_spec_const == node->get_special_constant() &&
       _sort < node->get_sort())));
}

inline
expression special_constant_expression_node::
get_instance(const substitution & sb) const
{
  return const_cast<special_constant_expression_node*>(this)->shared_from_this();
}

inline
bool special_constant_expression_node::
is_instance(const expression & exp,
	    substitution & sb) const
{
  return this->shared_from_this() == exp;
}

inline
bool 
special_constant_expression_node::check_expression(const signature * sg,
						   search_mode smode) const
{
  sort sr = _sort;
  if(!sr->check_sort(sg, smode))
    return false;
  
  return sg->check_special_constant(_spec_const, sr, smode);
}

inline
expression special_constant_expression_node::infer_sorts() const
{
  if(_factory->get_signature()->get_syntax_checking())
    return const_cast<special_constant_expression_node*>(this)->shared_from_this();

  sort sr = _sort;
  if(_factory->get_signature()->check_special_constant(_spec_const, sr))
    return _factory->create_expression(_spec_const, sr);
  else
    throw bad_special_constant_exception("Cannot infer expression (special constant) sort", _spec_const);
}

inline
expression special_constant_expression_node::
expand_expression(const signature * sg,
		  search_mode smode) const
{

  sort sr = _sort;
  if(!_factory->get_signature()->check_special_constant(_spec_const, sr))
    throw bad_special_constant_exception("Cannot expand expression (special constant)", _spec_const);
  
  return _factory->create_expression(_spec_const, 
				     sr->expand_sort(sg, smode));
}


inline
expression special_constant_expression_node::eliminate_let_binders(const substitution & sub) const
{
  return const_cast<special_constant_expression_node*>(this)->shared_from_this();
}

inline
void special_constant_expression_node::
accept_visitor(expression_visitor & visitor)
{
  visitor.visit_special_constant_expression_node(this->shared_from_this());
}

inline
expression special_constant_expression_node::clone_expression(expression_factory * other_factory) const
{
  return other_factory->create_expression(_spec_const, _sort);
}

inline
unsigned special_constant_expression_node::get_complexity() const
{
  return 1;
}

inline
void function_expression_node::calculate_hash()
{
  _hash_code = default_hash_combiner()(!_operands->empty() ? 
				     vector_hash_code<expression>()(*_operands) : 0,
				     _symbol.hash_code());
}

inline
function_expression_node::
function_expression_node(expression_factory * factory,
			 const function_symbol & symbol, 
			 const expression_vector & operands,
			 const sort & sr)
  : function_expression_node(factory,
			     symbol,
			     !operands.empty() ? std::move(expression_vector(operands)) : std::move(_dummy_expression_vector),
			     sr)
{}

inline
function_expression_node::
function_expression_node(expression_factory * factory,
			 const function_symbol & symbol, 
			 expression_vector && operands,
			 const sort & sr)
  :expression_node(factory),
   _symbol(symbol),
   _sort(sr),
   _operands(!operands.empty() ? 
	     new expression_vector(std::move(operands)) : 
	     &EMPTY_EXPRESSION_VECTOR),
   _all_vars(0),
   _free_vars(0)
{
  if(factory->get_signature()->get_syntax_checking() &&
     !factory->get_signature()->
     check_function_symbol(symbol, get_sorts(*_operands), _sort))
    throw bad_function_symbol_exception("Expression (function) cannot be instantiated", _symbol);
    
  calculate_hash();
}


inline
function_expression_node::
function_expression_node(expression_factory * factory)
  :expression_node(factory),
   _symbol(function_symbol::UNDEFINED),
   _sort(),
   _operands(&EMPTY_EXPRESSION_VECTOR),
   _all_vars(0),
   _free_vars(0)
{}


inline
expression_node::type 
function_expression_node::get_type() const
{
  return T_FUNCTION;
}

inline
const function_symbol & 
function_expression_node::get_symbol() const
{
  return _symbol;
}

inline
const sort & 
function_expression_node::get_sort() const
{
  return _sort;
}

inline
const variable & 
function_expression_node::get_variable() const
{
  return variable::UNDEFINED;
}

inline
const special_constant & 
function_expression_node::get_special_constant() const
{
  return special_constant::UNDEFINED;
}

inline
quantifier function_expression_node::get_quantifier() const
{
  return Q_UNDEFINED;
}

inline
const sorted_variable_vector & 
function_expression_node::get_quantified_variables() const
{
  return EMPTY_SORTED_VARIABLE_VECTOR;
}

inline
const variable_binding_vector & 
function_expression_node::get_variable_bindings() const
{
  return EMPTY_VARIABLE_BINDING_VECTOR;
}


inline
const expression & function_expression_node::get_subexpression() const
{
  return this->get_factory()->UNDEFINED_EXPRESSION();
}


inline
const expression_vector & 
function_expression_node::get_operands() const
{
  return *_operands;
}

inline
sat_variable function_expression_node::get_sat_variable() const
{
  return UNDEFINED_SAT_VARIABLE;
}

inline
sat_literal_polarity function_expression_node::get_sat_literal_polarity() const
{
  return SLP_UNDEFINED;
}


inline
bool 
function_expression_node::equal_to(const expression_node * node) const
{
  return 
    node->get_type() == T_FUNCTION &&
    node->get_symbol() == get_symbol() && 
    sorts_equal(_sort, node->get_sort()) &&
    node->get_operands() == get_operands();
}

inline
bool function_expression_node::less_than(const expression_node * node) const
{
  return get_type() < node->get_type() ||
    (get_type() == node->get_type() &&
     (get_symbol() < node->get_symbol() ||
      (get_symbol() == node->get_symbol() &&
       (get_operands().size() < node->get_operands().size() ||
	(get_operands().size() == node->get_operands().size() &&
	 (get_operands() < node->get_operands() ||
	  (get_operands() == node->get_operands() &&
	   get_sort() < node->get_sort())))))));
}


inline
function_expression_node::~function_expression_node()
{
  if(_operands != &EMPTY_EXPRESSION_VECTOR)
    delete _operands;

  if(_all_vars != &EMPTY_SORTED_VARIABLE_SET)
    delete _all_vars;

  if(_free_vars != &EMPTY_SORTED_VARIABLE_SET)
    delete _free_vars;
}



inline
expression_node::type 
quantifier_expression_node::get_type() const
{
  return T_QUANTIFIER;
}

inline
const function_symbol & 
quantifier_expression_node::get_symbol() const
{
  return function_symbol::UNDEFINED;
}

inline
const sort & 
quantifier_expression_node::get_sort() const
{
  return _factory->get_signature()->get_sort_factory()->BOOL_SORT(); 
}

inline
const variable & 
quantifier_expression_node::get_variable() const
{
  return variable::UNDEFINED;
}

inline 
const special_constant & 
quantifier_expression_node::get_special_constant() const
{
  return special_constant::UNDEFINED;
}

inline
quantifier 
quantifier_expression_node::get_quantifier() const
{
  return _quantifier;
}

inline
const sorted_variable_vector & 
quantifier_expression_node::get_quantified_variables() const
{
  return _vars;
}

inline
const variable_binding_vector & 
quantifier_expression_node::get_variable_bindings() const
{
  return EMPTY_VARIABLE_BINDING_VECTOR;
}


inline
const expression & 
quantifier_expression_node::get_subexpression() const
{
  return _expr;
}

inline
const expression_vector & 
quantifier_expression_node::get_operands() const
{
  return EMPTY_EXPRESSION_VECTOR;
}

inline
sat_variable quantifier_expression_node::get_sat_variable() const
{
  return UNDEFINED_SAT_VARIABLE;
}

inline
sat_literal_polarity quantifier_expression_node::get_sat_literal_polarity() const
{
  return SLP_UNDEFINED;
}

inline
bool 
quantifier_expression_node::equal_to(const expression_node * node) const
{
  return 
    node->get_type() == T_QUANTIFIER &&
    node->get_quantifier() == _quantifier && 
    node->get_quantified_variables() == _vars &&
    node->get_subexpression() == _expr;
}

inline
bool quantifier_expression_node::less_than(const expression_node * node) const
{
  return get_type() < node->get_type() ||
    (get_type() == node->get_type() &&
     (get_quantified_variables() < node->get_quantified_variables() ||
      (get_quantified_variables() == node->get_quantified_variables() &&
       get_subexpression() < node->get_subexpression())));
}

inline
unsigned quantifier_expression_node::get_complexity() const
{
  return 1 + _vars.size() + _expr->get_complexity();
}

inline
quantifier_expression_node::~quantifier_expression_node()
{
  if(_all_vars != &EMPTY_SORTED_VARIABLE_SET)
    delete _all_vars;
	
  if(_free_vars != &EMPTY_SORTED_VARIABLE_SET)
    delete _free_vars;
}


inline
expression_node::type 
let_expression_node::get_type() const
{
  return T_LET;
}

inline
const function_symbol & 
let_expression_node::get_symbol() const
{
  return function_symbol::UNDEFINED;
}

inline
const sort & 
let_expression_node::get_sort() const
{
  return _expr->get_sort();
}

inline
const variable & 
let_expression_node::get_variable() const
{
  return variable::UNDEFINED;
}

inline 
const special_constant & 
let_expression_node::get_special_constant() const
{
  return special_constant::UNDEFINED;
}

inline
quantifier 
let_expression_node::get_quantifier() const
{
  return Q_UNDEFINED;
}

inline
const sorted_variable_vector & 
let_expression_node::get_quantified_variables() const
{
  return EMPTY_SORTED_VARIABLE_VECTOR;
}

inline
const variable_binding_vector & 
let_expression_node::get_variable_bindings() const
{
  return _bind;
}


inline
const expression & 
let_expression_node::get_subexpression() const
{
  return _expr;
}

inline
const expression_vector & 
let_expression_node::get_operands() const
{
  return EMPTY_EXPRESSION_VECTOR;
}

inline
sat_variable let_expression_node::get_sat_variable() const
{
  return UNDEFINED_SAT_VARIABLE;
}

inline
sat_literal_polarity let_expression_node::get_sat_literal_polarity() const
{
  return SLP_UNDEFINED;
}

inline
bool 
let_expression_node::equal_to(const expression_node * node) const
{
  return 
    node->get_type() == T_LET && 
    node->get_variable_bindings() == _bind &&
    node->get_subexpression() == _expr;
}

inline
bool let_expression_node::less_than(const expression_node * node) const
{
  return get_type() < node->get_type() ||
    (get_type() == node->get_type() &&
     (get_variable_bindings() < node->get_variable_bindings() ||
      (get_variable_bindings() == node->get_variable_bindings() &&
       get_subexpression() < node->get_subexpression())));
}

inline
let_expression_node::~let_expression_node()
{
  if(_all_vars != &EMPTY_SORTED_VARIABLE_SET)
    delete _all_vars;

  if(_free_vars != &EMPTY_SORTED_VARIABLE_SET)
    delete _free_vars;
}


inline
void sat_literal_expression_node::calculate_hash()
{
  _hash_code = std::hash<sat_variable>()(_variable);
}

inline
sat_literal_expression_node::
sat_literal_expression_node(expression_factory * factory,
			    sat_variable vr,
			    sat_literal_polarity polarity)
  :expression_node(factory),
   _variable(vr),
   _polarity(polarity)
{  
  calculate_hash();
}

inline
sat_literal_expression_node::
sat_literal_expression_node(expression_factory * factory)
  :expression_node(factory),
   _variable(UNDEFINED_SAT_VARIABLE),
   _polarity(SLP_UNDEFINED)
{}
  

inline
expression_node::type 
sat_literal_expression_node::get_type() const
{
  return T_SAT;
}

inline
const function_symbol & 
sat_literal_expression_node::get_symbol() const
{
  return function_symbol::UNDEFINED;
}

inline
const sort & 
sat_literal_expression_node::get_sort() const
{
  return _factory->get_signature()->get_sort_factory()->BOOL_SORT();
}

inline
const variable & 
sat_literal_expression_node::get_variable() const
{
  return variable::UNDEFINED;
}

inline
const special_constant & 
sat_literal_expression_node::get_special_constant() const
{
  return special_constant::UNDEFINED;
}

inline
quantifier sat_literal_expression_node::get_quantifier() const
{
  return Q_UNDEFINED;
}

inline
const sorted_variable_set & 
sat_literal_expression_node::get_variables()
{
  return EMPTY_SORTED_VARIABLE_SET;
}

inline
const sorted_variable_set & 
sat_literal_expression_node::get_free_variables()
{
  return EMPTY_SORTED_VARIABLE_SET;
}


inline
const sorted_variable_vector & 
sat_literal_expression_node::get_quantified_variables() const
{
  return EMPTY_SORTED_VARIABLE_VECTOR;
}

inline
const variable_binding_vector & 
sat_literal_expression_node::get_variable_bindings() const
{
  return EMPTY_VARIABLE_BINDING_VECTOR;
}

inline
const expression & sat_literal_expression_node::get_subexpression() const
{
  return this->get_factory()->UNDEFINED_EXPRESSION();
}

inline
const expression_vector & 
sat_literal_expression_node::get_operands() const
{
  return EMPTY_EXPRESSION_VECTOR;
}

inline
sat_variable sat_literal_expression_node::get_sat_variable() const
{
  return _variable;
}

inline
sat_literal_polarity sat_literal_expression_node::get_sat_literal_polarity() const
{
  return _polarity;
}

inline
bool 
sat_literal_expression_node::equal_to(const expression_node * node) const
{
  return node->get_type() == T_SAT && 
    _variable == node->get_sat_variable() &&
    _polarity == node->get_sat_literal_polarity();
}
  
inline
expression sat_literal_expression_node::
get_instance(const substitution & sb) const
{
  return const_cast<sat_literal_expression_node*>(this)->shared_from_this();
}

inline
bool 
sat_literal_expression_node::check_expression(const signature * sg,
					      search_mode smode) const
{
  return true;
}

inline
expression sat_literal_expression_node::infer_sorts() const
{
  return const_cast<sat_literal_expression_node*>(this)->shared_from_this();
}


inline
bool sat_literal_expression_node::is_instance(const expression & exp,
					      substitution & sb) const
{
  return exp == this->shared_from_this();
}


inline
void 
sat_literal_expression_node::out(std::ostream & ostr) const
{
  ostr << (_polarity == SLP_NEGATIVE ? -(int)_variable : (int)_variable);
}

inline
bool sat_literal_expression_node::less_than(const expression_node * node) const
{
  return get_type() < node->get_type() ||
    (get_type() == node->get_type() &&
     (_variable < node->get_sat_variable() ||
      (_variable == node->get_sat_variable() &&
       _polarity < node->get_sat_literal_polarity())));
}


inline
expression sat_literal_expression_node::
expand_expression(const signature * sg,
		  search_mode smode) const
{
  return const_cast<sat_literal_expression_node*>(this)->shared_from_this();
}

inline
expression sat_literal_expression_node::eliminate_let_binders(const substitution & sub) const
{
  return const_cast<sat_literal_expression_node*>(this)->shared_from_this();
}


inline
void 
sat_literal_expression_node::accept_visitor(expression_visitor & visitor)
{
  visitor.visit_sat_literal_expression_node(this->shared_from_this());
}

inline
expression sat_literal_expression_node::clone_expression(expression_factory * other_factory) const
{
  return other_factory->create_expression(_variable, _polarity);
}


inline
unsigned sat_literal_expression_node::get_complexity() const
{
  return 1;
}



inline
function_definition::
function_definition(const sorted_variable_vector & vars, 
		    const expression & exp)
  :_vars(!vars.empty() ? 
	 new sorted_variable_vector(vars) : 
	 &EMPTY_SORTED_VARIABLE_VECTOR),
   _exp(exp)
{}

inline
function_definition::
function_definition(sorted_variable_vector && vars, 
		    const expression & exp)
  :_vars(!vars.empty() ? 
	 new sorted_variable_vector(std::move(vars)) : 
	 &EMPTY_SORTED_VARIABLE_VECTOR),
   _exp(exp)
{}

inline
function_definition::
function_definition(const function_definition & def)
  :_vars(def._vars != &EMPTY_SORTED_VARIABLE_VECTOR ? 
	 new sorted_variable_vector(*def._vars) :
	 &EMPTY_SORTED_VARIABLE_VECTOR),
   _exp(def._exp)
{}

inline
function_definition::
function_definition(function_definition && def)
  :_vars(def._vars),
   _exp(std::move(def._exp))
{
  def._vars = nullptr;
}

inline
function_definition & 
function_definition::operator = (const function_definition & def)
{
  if(&def == this)
    return *this;
    
  if(_vars != &EMPTY_SORTED_VARIABLE_VECTOR)
    delete _vars;
    
  _vars = def._vars != &EMPTY_SORTED_VARIABLE_VECTOR ? 
    new sorted_variable_vector(*def._vars) :
    &EMPTY_SORTED_VARIABLE_VECTOR;
    
  _exp = def._exp;
    
  return *this;
}

inline
function_definition & 
function_definition::operator = (function_definition && def)
{
  if(&def == this)
    return *this;
    
  if(_vars != &EMPTY_SORTED_VARIABLE_VECTOR)
    delete _vars;
    
  _vars = def._vars;    
  _exp = std::move(def._exp);
  def._vars = nullptr;
    
  return *this;
}

inline
const sorted_variable_vector & function_definition::get_variables() const
{
  return *_vars;
}
  
inline
const expression & function_definition::get_expression() const
{
  return _exp;
}
  
inline
void function_definition::out(std::ostream & ostr) const
{
  ostr << _exp;
}
  
inline
function_definition::~function_definition()
{
  if(_vars != &EMPTY_SORTED_VARIABLE_VECTOR)
    delete _vars;
}


inline
std::ostream & operator << (std::ostream & ostr, 
			    const function_definition & def)
{
  def.out(ostr);
  return ostr;
}




inline
variable_binding::
variable_binding(const variable & var, const expression & expr)
  :_var(var),
   _expr(expr)
{}

inline
const variable & 
variable_binding::get_variable() const
{
  return _var;
}

inline
const expression & 
variable_binding::get_expression() const
{
  return _expr;
}

inline
bool 
variable_binding::operator == (const variable_binding & binding) const
{
  return _var == binding._var && _expr == binding._expr;
}

inline
bool 
variable_binding::operator != (const variable_binding & binding) const
{
  return _var != binding._var || _expr == binding._expr;
}
  
inline
bool 
variable_binding::operator < (const variable_binding & binding) const
{
  return _var < binding._var || (_var == binding._var && _expr < binding._expr);  
}
    
inline
bool variable_binding::operator > (const variable_binding & binding) const
{
  return binding < *this;
}
  
inline
bool variable_binding::operator <= (const variable_binding & binding) const
{
  return *this == binding || *this < binding;
}
  
inline
bool variable_binding::operator >= (const variable_binding & binding) const
{
  return *this == binding || *this > binding;
}

inline
std::size_t 
variable_binding::hash_code() const
{
  return default_hash_combiner()(_expr->hash_code(), _var.hash_code());
}

inline
void 
variable_binding::out(std::ostream & ostr) const
{
  ostr << "(" << _var << " " << _expr << ")";
}

inline
std::string variable_binding::to_string() const
{
  return ::to_string(this);
}


inline
std::ostream & operator << (std::ostream & ostr, const variable_binding & vb)
{
  vb.out(ostr);
  return ostr;
}

inline
void expression_factory::remove_expression(expression_node * node)
{
  if(_destruction)
    return;

  _expressions.erase(node);
  delete node;
}

inline
sort 
expression_factory::set_sort(const sort & sr) const
{
  if(sr == sort())
    return this->get_signature()->get_sort_factory()->UNDEFINED_SORT();
  else
    return sr;
}
  
inline
expression_factory::expression_factory(signature * sg)
  :_expressions(HASH_TABLE_SIZE),
   _signature(sg),
   _destruction(false),
   _var_node(this),
   _spec_node(this),
   _fun_node(this),
   _quant_node(this),
   _let_node(this),
   _sat_node(this),
   _weak_compare(false)
{
  _undefined_expression = insert_expression(new undefined_expression_node(this));
  _undefined_expression->set_assigned(_undefined_expression);

  _true_expression = 
    insert_expression(new function_expression_node(this, 
						   function_symbol::TRUE,
						   expression_vector(),
						   _signature->
						   get_sort_factory()->
						   BOOL_SORT()));
  _false_expression = 
    insert_expression(new function_expression_node(this, 
						   function_symbol::FALSE,
						   expression_vector(),
						   _signature->
						   get_sort_factory()->
						   BOOL_SORT()));
}

inline
signature * 
expression_factory::get_signature() const
{
  return _signature;
}

inline
const expression & expression_factory::UNDEFINED_EXPRESSION() const
{
  return _undefined_expression;
}

inline
const expression & expression_factory::TRUE_EXPRESSION() const
{
  return _true_expression;
}

inline
const expression & expression_factory::FALSE_EXPRESSION() const
{
  return _false_expression;
}

inline
expression expression_factory::create_expression(const variable & var,
						 const sort & sr)
{
  _var_node._var = var;
  _var_node._sort = set_sort(sr);
  _var_node.calculate_hash();
    
  if(_var_node._sort == _signature->get_sort_factory()->UNDEFINED_SORT())
    _weak_compare = true;

  std::unordered_set<expression_node *>::iterator it = _expressions.find(&_var_node);

  _weak_compare = false;

  if(it != _expressions.end())
    return (*it)->shared_from_this();
       
  return insert_expression(new variable_expression_node(this, var, 
							_var_node._sort));
}

inline
expression expression_factory::create_expression(const special_constant & c,
						 const sort & sr)
{
  _spec_node._spec_const = c;
  _spec_node._sort = set_sort(sr);
  _spec_node.calculate_hash();
    
  if(_spec_node._sort == _signature->get_sort_factory()->UNDEFINED_SORT())
    _weak_compare = true;

  std::unordered_set<expression_node *>::iterator it = _expressions.find(&_spec_node);

  _weak_compare = false;

  if(it != _expressions.end())
    return (*it)->shared_from_this();
    

  return insert_expression(new special_constant_expression_node(this, 
								c,
								_spec_node._sort));
}

inline
expression expression_factory::create_expression(const function_symbol & symbol,
						 const sort & sr)
{
  return create_expression(symbol, EMPTY_EXPRESSION_VECTOR, sr);
}


inline
expression expression_factory::create_expression(const function_symbol & symbol,
						 const expression & exp,
						 const sort & sr)
{
  return create_expression(symbol, expression_vector { exp }, sr);
}

inline
expression expression_factory::create_expression(const function_symbol & symbol, 
						 const expression & exp1, 
						 const expression & exp2,
						 const sort & sr)
{
  return create_expression(symbol, expression_vector { exp1, exp2 }, sr);
}

inline
expression expression_factory::create_expression(const function_symbol & symbol, 
						 const expression & exp1, 
						 const expression & exp2,
						 const expression & exp3,
						 const sort & sr)
{
  return create_expression(symbol, expression_vector { exp1, exp2, exp3 }, sr);
}


inline
expression expression_factory::
create_expression(const function_symbol & symbol, 
		  const expression_vector & expressions,
		  const sort & sr)
{
  return create_expression(symbol,
			   expressions.empty() ? std::move(function_expression_node::_dummy_expression_vector)
			   : std::move(expression_vector(expressions)),
			   sr);
}

inline
expression expression_factory::
create_expression(const function_symbol & symbol, 
		  expression_vector && expressions,
		  const sort & sr)
{  
  _fun_node._symbol = symbol;
  _fun_node._sort = set_sort(sr);
  _fun_node._operands = expressions.empty() ? &EMPTY_EXPRESSION_VECTOR : &expressions;
  _fun_node.calculate_hash();
    
  if(_fun_node._sort == _signature->get_sort_factory()->UNDEFINED_SORT())
    _weak_compare = true;

  std::unordered_set<expression_node *>::iterator it = _expressions.find(&_fun_node);

  _weak_compare = false;

  if(it != _expressions.end())
    return (*it)->shared_from_this();
 

  return insert_expression(new function_expression_node
			   (this, symbol, std::move(expressions), _fun_node._sort));
}


inline
expression expression_factory::create_expression(quantifier qn, 
						 const sorted_variable_vector & vars,
						 const expression & exp)
{
  return create_expression(qn, sorted_variable_vector(vars), exp);
}


inline
expression expression_factory::create_expression(quantifier qn, 
						 sorted_variable_vector && vars,
						 const expression & exp)
{
  _quant_node._quantifier = qn;
  _quant_node._vars = std::move(vars);
  _quant_node._expr = exp;
  _quant_node.calculate_hash();
    
  std::unordered_set<expression_node *>::iterator it = _expressions.find(&_quant_node);

  if(it != _expressions.end())
    return (*it)->shared_from_this();

  return insert_expression(new quantifier_expression_node
			   (this, qn, std::move(_quant_node._vars), exp));
}


inline
expression expression_factory::
create_expression(const variable_binding_vector & bind,
		  const expression & exp)
{
  return create_expression(variable_binding_vector(bind), exp);
}

inline
expression expression_factory::
create_expression(variable_binding_vector && bind,
		  const expression & exp)
{
  _let_node._bind = std::move(bind);
  _let_node._expr = exp;
  _let_node.calculate_hash();
    
  std::unordered_set<expression_node *>::iterator it = _expressions.find(&_let_node);

  if(it != _expressions.end())
    return (*it)->shared_from_this();

  return insert_expression(new let_expression_node(this, std::move(_let_node._bind), exp));
}


inline
expression expression_factory::
create_expression(sat_variable vr, 
		  sat_literal_polarity polarity)
{
  _sat_node._variable = vr;
  _sat_node._polarity = polarity;
  _sat_node.calculate_hash();
    
  std::unordered_set<expression_node *>::iterator it = _expressions.find(&_sat_node);

  if(it != _expressions.end())
    return (*it)->shared_from_this();

  return insert_expression(new sat_literal_expression_node(this, vr, polarity));
}

#endif // _EXPRESSION_H
