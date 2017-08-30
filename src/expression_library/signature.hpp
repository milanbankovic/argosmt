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

#ifndef _SIGNATURE_H
#define _SIGNATURE_H

#include "basic_types.hpp"
#include "search_mode.hpp"

class signature;

/** Arity of a sort symbol (an unsigned number). */
typedef unsigned arity;

/** Undefined arity (defined as (unsigned)(-1)). */
extern const arity UNDEFINED_ARITY;

/** Represents an abstract checker for sort symbols. */
class sort_symbol_checker {
public:
  /** Method should return true if the checker accepts the given symbol 
      with the given arity. */
  virtual bool check_symbol(const sort_symbol & symbol, arity ar) = 0;

  /** Method should return the arity of the symbol (or UNDEFINED_ARITY, if
      the symbol is not accepted). The method should be congruent with the
      method check_symbol, when arities are concerned. */
  virtual arity get_arity(const sort_symbol & symbol) = 0;

  /** Method should return the set of attributes assigned to the sort symbol.
      By default, symbols have no attributes. */
  virtual const attribute_set & get_attributes(const sort_symbol & symbol)
  {
    return EMPTY_ATTRIBUTE_SET;
  }
    
  /** Virtual destructor. */
  virtual ~sort_symbol_checker() {}
};

class default_sort_symbol_checker : public sort_symbol_checker {
private:
  arity _arity;
  const attribute_set * _attr;

  const attribute_set & attributes()
  {
    return *_attr;
  }

public:
  default_sort_symbol_checker(arity ar, const attribute_set & attr);

  default_sort_symbol_checker(arity ar, attribute_set && attr);
  
  virtual bool check_symbol(const sort_symbol & symbol, arity ar);
  virtual arity get_arity(const sort_symbol & symbol);
  virtual const attribute_set & get_attributes(const sort_symbol & symbol);  
 
  ~default_sort_symbol_checker();
 
};

class sort_symbol_registry {
private:

  typedef std::unordered_map<sort_symbol, 
				  sort_symbol_checker *,
				  std::hash<sort_symbol>,
				  identifier_weak_comparator> sort_symbol_map;


  typedef std::vector<sort_symbol_checker *> sort_symbol_checker_vector;

  sort_symbol_map _symbols;
  sort_symbol_checker_vector _general_checkers;
  signature * _signature;
  
public:
  
  sort_symbol_registry(signature * sg);
  
  void add_symbol(const sort_symbol & symbol, 
		  arity ar,
		  const attribute_set & attr);

  void add_symbol(const sort_symbol & symbol, 
		  arity ar,
		  attribute_set && attr);

  
  void add_symbol_checker(const sort_symbol & symbol,
			  sort_symbol_checker * checker);
  void add_symbol_checker(sort_symbol_checker * checker);
  sort_symbol_checker * check_symbol(const sort_symbol & symbol, arity ar) const;
  void remove_symbol(const sort_symbol & symbol);
  
  ~sort_symbol_registry();
  
};

#include "sort.hpp"

/** Rank is a vector of sorts. */
typedef sort_vector rank;

/** Undefined rank (defined as the empty vector of sorts). */
extern const rank UNDEFINED_RANK;


/** Represents an abstract checker for function symbols. */
class function_symbol_checker {
public:

  /** The method should return true if the symbol with given operand_sorts, 
      and sort is accepted by the checker. operand_sorts is the vector
      of sorts of the subexpressions to which the symbol is applied, and
      sort is the sort of the constructed expression (i.e. the result
      of application of the symbol to the subexpressions). If sort
      is set to undefined sort, then _sort inference_ should be performed,
      which means that infered sort should be assigned to sort argument
      (which is a reference to non-constant sort). If the sort cannot be 
      infered, the method should return false. If sort is not set to
      undefined sort, then the checker may perform arbitrary check on it
      (e.g. whether it is equivalent to some predefined sort). */
  virtual bool check_symbol(const function_symbol & symbol,
			    const sort_vector & operand_sorts,
			    sort & sr) = 0;

  /** The method should return the set of attributes assigned to the symbol.
      The meaning of the other two arguments is the same as in check_symbol(),
      except that no sort inference is performed (notice that sort
      is a reference to const sort). By default, symbols do not have
      any attributes. */
  virtual const attribute_set & 
  get_attributes(const function_symbol & symbol,
		 const sort_vector & operand_sorts,
		 const sort & sr = sort())
  {
    return EMPTY_ATTRIBUTE_SET;
  }
  
  /** Virtual destructor. */
  virtual ~function_symbol_checker() {}
};

  
class default_function_symbol_checker : public function_symbol_checker {
private:  
  signature * _signature;
  rank _rank;
  const attribute_set * _attr;
  
  const attribute_set & attributes()
  {
    return *_attr;
  }

  static attribute_set _dummy_attribute_set;
  
  bool check_regular(const sort_vector & operand_sorts,
		     sort & sr);
  bool check_left_associativity(const sort_vector & operand_sorts,
				sort & sr);
  bool check_right_associativity(const sort_vector & operand_sorts,
				 sort & sr);
  bool check_chainable_pairwise(const sort_vector & operand_sorts,
				sort & sr);
  bool deduce_return_sort(sort & sr, const instantiation & ins);
  
public:  

  default_function_symbol_checker(signature * sg, 
				  const rank & rn, 
				  const attribute_set & attr); 

  default_function_symbol_checker(signature * sg, 
				  rank && rn, 
				  attribute_set && attr); 

  default_function_symbol_checker(signature * sg, 
				  const rank & rn, 
				  attribute_set && attr); 

  default_function_symbol_checker(signature * sg, 
				  rank && rn, 
				  const attribute_set & attr); 

  
  virtual bool check_symbol(const function_symbol & symbol,
			    const sort_vector & operand_sorts,
			    sort & sr);

  virtual const attribute_set & 
  get_attributes(const function_symbol & symbol,
		 const sort_vector & operand_sorts,
		 const sort & sr = sort());

  ~default_function_symbol_checker();

};

  
class function_symbol_registry {  
private:
  typedef std::unordered_multimap<function_symbol, 
				       function_symbol_checker *,
				       std::hash<function_symbol>,
				       identifier_weak_comparator> 
  function_symbol_map;
  
  typedef std::vector<function_symbol_checker *> function_symbol_checker_vector;
  
  function_symbol_map _symbols;  
  function_symbol_checker_vector _general_checkers;
  signature * _signature;
  
public:
  
  function_symbol_registry(signature * sg);
  
  void add_symbol(const function_symbol & symbol, 
		  const rank & rn,
		  const attribute_set & attr);

  void add_symbol(const function_symbol & symbol, 
		  rank && rn,
		  const attribute_set & attr);

  void add_symbol(const function_symbol & symbol, 
		  const rank & rn,
		  attribute_set && attr);
  
  void add_symbol(const function_symbol & symbol, 
		  rank && rn,
		  attribute_set && attr);

  
  void add_symbol_checker(const function_symbol & symbol,
			  function_symbol_checker * checker);
  void add_symbol_checker(function_symbol_checker * checker);
  
  void remove_symbol(const function_symbol & symbol);  
  function_symbol_checker * check_symbol(const function_symbol & symbol,
					 const sort_vector & operand_sorts,
					 sort & sr) const;
  
  ~function_symbol_registry();
};

/** Represents an abstract checker for special constants. */
class special_constant_checker {
public:

  /** Method should return true if the special constant of the given sort is
      accepted by the checker. If sort is set to undefined sort, then sort
      inference should be performed, which means that infered sort of the
      constant should be assigned to sort argument (which is 
      a reference to non-const sort). If sort cannot be infered, false
      should be returned. If sort is not set to undefined sort,
      then the checker may perform arbitrary check on it (e.g. whether it
      is equivalent to some predefined sort). */
  virtual bool check_constant(const special_constant & sc, 
			      sort & sr) = 0;
    
  /** Method should return the set of attributes assigned to the special
      constant. The second argument have the same meaning as in 
      check_constant(), except no sort inference is performed. By
      default, special constants have no attributes. */
  virtual const attribute_set & get_attributes(const special_constant & sc, 
					       const sort & sr = sort())
  {
    return EMPTY_ATTRIBUTE_SET;
  }
  
  /** Virtual destructor. */
  virtual ~special_constant_checker() {}
};  

class default_special_constant_checker : public special_constant_checker {
private:
  signature * _signature;
  sort _sort;
  const attribute_set * _attr;

  const attribute_set & attributes()
  {
    return *_attr;
  }

  static attribute_set _dummy_attribute_set;
  
public:
  default_special_constant_checker(signature * sg,
				   const sort & sr,
				   const attribute_set & attr);

  default_special_constant_checker(signature * sg,
				   const sort & sr,
				   attribute_set && attr);


  virtual bool check_constant(const special_constant & sc, 
			      sort & sr);
  
  virtual const attribute_set & get_attributes(const special_constant & sc, 
					       const sort & sr = sort());

  ~default_special_constant_checker();
};


class special_constant_type_checker : public special_constant_checker {
private:
  signature * _signature;
  special_constant::type _type;
  sort _sort;
  const attribute_set * _attr;

  static attribute_set _dummy_attribute_set;
  
  const attribute_set & attributes()
  {
    return *_attr;
  }

public:
  special_constant_type_checker(signature * sg,
				special_constant::type tp,
				const sort & sr,
				const attribute_set & attr);

  special_constant_type_checker(signature * sg,
				special_constant::type tp,
				const sort & sr,
				attribute_set && attr);
  
  virtual bool check_constant(const special_constant & sc,
			      sort & sr);
  
  virtual const attribute_set & get_attributes(const special_constant & sc,
					       const sort & sr = sort());
  
  ~special_constant_type_checker();

};


class special_constant_registry
{
private:
  typedef std::unordered_multimap<special_constant, 
				       special_constant_checker *> constant_checker_map;
  typedef std::vector<special_constant_checker *> constant_checker_vector;
 
  constant_checker_map _constant_checker_map;
  constant_checker_vector _general_constant_checkers;

  signature * _signature;
  
public:
  special_constant_registry(signature * sg);

  void add_constant(const special_constant & c, 
		    const sort & s,
		    const attribute_set & attr);

  void add_constant(const special_constant & c, 
		    const sort & s,
		    attribute_set && attr);

  void add_type(special_constant::type tp, 
		const sort & s,
		const attribute_set & attr);

  void add_type(special_constant::type tp, 
		const sort & s,
		attribute_set && attr);

  
  void add_constant_checker(const special_constant & c,
			    special_constant_checker * checker);
  void add_constant_checker(special_constant_checker * checker);
  void remove_constant(const special_constant & c);
  special_constant_checker * check_constant(const special_constant & c,
					    sort & sr) const;

  ~special_constant_registry();
};


/** Vector of signatures. */
typedef std::vector<signature *> signature_vector;


/** Represents a signature (and contains databases of declared sort 
    and function symbols and special constants and their types). */
class signature {
public:

  /** Signatures can be combined in a tree-like structure called 
      _signature hierarchy_. In this structure, each signature can
      have zero or more _subsignatures_, and at most one _expansion_.
      One of the signatures in the hierarchy is distinguished as 
      _main_ signature. In general, there are three types of signatures
      that can appear in the hierarchy:

      1) The main signature: must be unique. This signature must be 
      created first, and during its construction, the sort factory
      common to all signatures in the hierarchy is constructed. When
      this signature is destroyed, it destroys all other signatures
      in its hierarchy.

      2) Subsignatures: all signatures created _below_ the main signature
      in the hierarchy (i.e. that are either direct or indirect 
      subsignatures of the main signature). 

      3) Expansions: all signatures created _above_ the main signature
      in the hierarchy (i.e. that are either direct or indirect expansions
      of the main signatures). They can be created and destroyed at any
      time, not affecting the main signature. If one expansion signature
      is destroyed, it recursively destroys all its (direct or indirect)
      expansions. 
       
      \verbatim
      Example of signature hierarchy:

      Expansionn

      |

      ...

      |

      Expansion2

      |

      Expansion1

      |

      Main

      /    \

      /        \

      Sub1        Sub2

      /   \           \

      /       \           \

      Sub11    Sub12       Sub21
      \endverbatim

      Possible order of creation is: Main, Sub1, Sub11, Sub12, Sub2, Sub21,
      Expansion1, Expansion2, ..., Expansionn.

      Each signature in the hieararchy implicitly contains Core signature
      defined by the standard.
  */
  enum type 
    { 
      /** Main signature */
      T_MAIN_SIGNATURE, 
      /** Subsignature */
      T_SUB_SIGNATURE, 
      /** Expansion signature */
      T_EXPANSION_SIGNATURE 
    };
  
private:
  type _type;
  sort_symbol_registry * _sort_symbol_registry;
  function_symbol_registry * _function_symbol_registry;
  special_constant_registry * _spec_const_registry;

  sort_factory * _sort_factory;

  signature_vector _sub_signatures;  
  signature * _expansion_signature;

  bool _syntax_checking;
  
  void init_core_signature();
public:

  /** Constructor. If no arguments are given, the main signature is created.
      If type is T_SUB_SIGNATURE, then sig argument represents its parent
      signature in the hierarchy. If type is T_EXPANSION_SIGNATURE, then
      sig argument represents its child signature in the hierarchy. If
      type is T_MAIN_SIGNATURE, then sig argument has no significance.
      The argument syntax_checking has significance only when main signature
      is created, and it determines whether syntax (well-sortedness) checking
      should be performed in that signature hierarchy (when sorts and 
      expressions are created over that signature hierarchy). */
  signature(type tp = T_MAIN_SIGNATURE, 
	    signature * sig = 0, 
	    bool syntax_checking = true);

  /** Returns the type of the signature. */
  type get_type() const;

  /** Returns true if syntax checking is on. */
  bool get_syntax_checking() const;

  /** Adds sort symbol to the signature, with given arity and attribute set.
      This adds a sort symbol checker to the signature that may accept 
      only symbols that are equal to the given symbol, or symbols that 
      are instances of the given symbol, in case the given symbol is a 
      pattern symbol. Such symbol is accepted if the number of subsorts
      to which the symbol is applied matches the arity given in this 
      declaration. */
  void add_sort_symbol(const sort_symbol & symbol, 
		       arity ar = 0,
		       const attribute_set & attr 
		       = EMPTY_ATTRIBUTE_SET);

  /** Adds sort symbol to the signature, with given arity and attribute set.
      This adds a sort symbol checker to the signature that may accept 
      only symbols that are equal to the given symbol, or symbols that 
      are instances of the given symbol, in case the given symbol is a 
      pattern symbol. Such symbol is accepted if the number of subsorts
      to which the symbol is applied matches the arity given in this 
      declaration. */
  void add_sort_symbol(const sort_symbol & symbol, 
		       arity ar,
		       attribute_set && attr);
  
  /** Adds an abstract checker for the given symbol. This checker 
      may accept sort symbols that are either equal to the given symbol,
      or symbols that are instances of the given symbol, if the given
      symbol is a pattern symbol. Such symbol is accepted by the 
      checker if checker->check_symbol(s, ar) returns true, where 
      ar is the number of subsorts to which the symbol s is applied. */
  void add_sort_symbol_checker(const sort_symbol & symbol, 
			       sort_symbol_checker * checker);
    
  /** Adds an abstract checker. This checker accepts all
      symbols for which checker->check_symbol(s, ar) returns true,
      where ar is the number of subsorts to which the symbol s is applied. */
  void add_sort_symbol_checker(sort_symbol_checker * checker);

  /** Checks for sort symbol. The symbol must not be a pattern symbol.
      The method returns the pointer to the sort symbol checker
      in the signature that accepts the symbol, if such checker exists.
      If there is no such checker, the method returns 0. If more than
      one such checkers exist, it returns the first one found. The
      smode argument defines the search mode used during the search for
      the symbol. */
  sort_symbol_checker * check_sort_symbol(const sort_symbol & symbol, 
					  arity ar,
					  search_mode smode = S_CONTEXTUAL) const;
  
  /** Adds function symbol with the given rank and (optionally) attribute 
      set. The rank can contain parametric sorts. This adds a function symbol
      checker to the signature that may accept only symbols equal to the 
      given symbol, or symbols that are instances of the given symbol, if the 
      given symbol is a pattern symbol. Such symbol is accepted if the sorts 
      of the subexpressions to which the symbol is applied are equivalent 
      to the first n-1 sorts in the here declared rank, or are instances of 
      these sorts (if rank contains parametric sorts). Here, n is the length 
      of the rank. The sort of constructed expression (i.e. the result of 
      application of the symbol to the subexpressions) is then infered from 
      the n-th sort in the rank (possibly, by applying the appropriate 
      instantiation). If the sort of the expression being constructed is
      explicitly provided, then it also must match the n-th sort in the
      rank (or be the appropriate instance of it). */
  void add_function_symbol(const function_symbol & symbol, 
			   const rank & rn,
			   const attribute_set & attr 
			   = EMPTY_ATTRIBUTE_SET);

  /** Adds function symbol with the given rank and (optionally) attribute 
      set. The rank can contain parametric sorts. This adds a function symbol
      checker to the signature that may accept only symbols equal to the 
      given symbol, or symbols that are instances of the given symbol, if the 
      given symbol is a pattern symbol. Such symbol is accepted if the sorts 
      of the subexpressions to which the symbol is applied are equivalent 
      to the first n-1 sorts in the here declared rank, or are instances of 
      these sorts (if rank contains parametric sorts). Here, n is the length 
      of the rank. The sort of constructed expression (i.e. the result of 
      application of the symbol to the subexpressions) is then infered from 
      the n-th sort in the rank (possibly, by applying the appropriate 
      instantiation). If the sort of the expression being constructed is
      explicitly provided, then it also must match the n-th sort in the
      rank (or be the appropriate instance of it). */
  void add_function_symbol(const function_symbol & symbol, 
			   rank && rn,
			   attribute_set && attr);

  /** Adds function symbol with the given rank and (optionally) attribute 
      set. The rank can contain parametric sorts. This adds a function symbol
      checker to the signature that may accept only symbols equal to the 
      given symbol, or symbols that are instances of the given symbol, if the 
      given symbol is a pattern symbol. Such symbol is accepted if the sorts 
      of the subexpressions to which the symbol is applied are equivalent 
      to the first n-1 sorts in the here declared rank, or are instances of 
      these sorts (if rank contains parametric sorts). Here, n is the length 
      of the rank. The sort of constructed expression (i.e. the result of 
      application of the symbol to the subexpressions) is then infered from 
      the n-th sort in the rank (possibly, by applying the appropriate 
      instantiation). If the sort of the expression being constructed is
      explicitly provided, then it also must match the n-th sort in the
      rank (or be the appropriate instance of it). */
  void add_function_symbol(const function_symbol & symbol, 
			   const rank & rn,
			   attribute_set && attr);

    /** Adds function symbol with the given rank and (optionally) attribute 
      set. The rank can contain parametric sorts. This adds a function symbol
      checker to the signature that may accept only symbols equal to the 
      given symbol, or symbols that are instances of the given symbol, if the 
      given symbol is a pattern symbol. Such symbol is accepted if the sorts 
      of the subexpressions to which the symbol is applied are equivalent 
      to the first n-1 sorts in the here declared rank, or are instances of 
      these sorts (if rank contains parametric sorts). Here, n is the length 
      of the rank. The sort of constructed expression (i.e. the result of 
      application of the symbol to the subexpressions) is then infered from 
      the n-th sort in the rank (possibly, by applying the appropriate 
      instantiation). If the sort of the expression being constructed is
      explicitly provided, then it also must match the n-th sort in the
      rank (or be the appropriate instance of it). */
  void add_function_symbol(const function_symbol & symbol, 
			   rank && rn,
			   const attribute_set & attr
			   = EMPTY_ATTRIBUTE_SET);
  
  
  /** The same as previous, with only one sort in the rank. */
  void add_function_symbol(const function_symbol & symbol, 
			   const sort & s,
			   const attribute_set & attr
			   = EMPTY_ATTRIBUTE_SET);

    /** The same as previous, with only one sort in the rank. */
  void add_function_symbol(const function_symbol & symbol, 
			   const sort & s,
			   attribute_set && attr);

  /** The same as previous, with two sorts in the rank. */
  void add_function_symbol(const function_symbol & symbol, 
			   const sort & s1,
			   const sort & s2,
			   const attribute_set & attr
			   = EMPTY_ATTRIBUTE_SET);

  /** The same as previous, with two sorts in the rank. */
  void add_function_symbol(const function_symbol & symbol, 
			   const sort & s1,
			   const sort & s2,
			   attribute_set && attr);

  
  /** The same as previous, with three sorts in the rank. */
  void add_function_symbol(const function_symbol & symbol, 
			   const sort & s1,
			   const sort & s2,
			   const sort & s3,
			   const attribute_set & attr
			   = EMPTY_ATTRIBUTE_SET);

  /** The same as previous, with three sorts in the rank. */
  void add_function_symbol(const function_symbol & symbol, 
			   const sort & s1,
			   const sort & s2,
			   const sort & s3,
			   attribute_set && attr);

  
  /** The same as previous, with four sorts in the rank. */
  void add_function_symbol(const function_symbol & symbol, 
			   const sort & s1,
			   const sort & s2,
			   const sort & s3,
			   const sort & s4,
			   const attribute_set & attr 
			   = EMPTY_ATTRIBUTE_SET);

  /** The same as previous, with four sorts in the rank. */
  void add_function_symbol(const function_symbol & symbol, 
			   const sort & s1,
			   const sort & s2,
			   const sort & s3,
			   const sort & s4,
			   attribute_set && attr);

  
  /** Adds an abstract checker for the symbol. This checker may accept
      symbols that are equal to the given symbol, or symbols that are 
      instances of the given symbol, if the given symbol is a pattern symbol.
      Such symbol s is accepted by the checker if 
      checker->check_symbol(s, op_sorts, exp_sort) returns true, where 
      op_sorts is the vector of sorts of the subexpressions to which the 
      symbol s is applied, and exp_sort is the sort of the constructed 
      expression (i.e. the result of application of the symbol to the
      subexpressions). If exp_sort is initialized to undefined, then sort
      inference may be performed by the checker. */
  void add_function_symbol_checker(const function_symbol & symbol, 
				   function_symbol_checker * checker);

  /** Adds an abstract checker. It accepts all symbols s for which 
      checker->check_symbol(s, op_sorts, exp_sort) returns true, where 
      op_sorts is the vector of sorts of the subexpressions to which the 
      symbol s is applied, and exp_sort is the sort of the constructed 
      expression (i.e. the result of application of the symbol to the 
      subexpressions). If exp_sort is initialized to undefined, then sort
      inference may be performed by the checker. */
  void add_function_symbol_checker(function_symbol_checker * checker);
  
      
  /** Checks for function symbol. The symbol must not be a pattern symbol.
      The method returns the pointer to the function symbol checker
      in the signature that accepts the symbol, if such checker exists. 
      If there is no such checker, the method returns 0. If more than
      one such checkers exists, it returns the first one found. The
      smode argument defines the search mode used during the search for
      the symbol. If sort is initialized to undefined sort, then sort 
      inference may be performed. */
  function_symbol_checker * 
  check_function_symbol(const function_symbol & symbol,
			const sort_vector & operand_sorts,
			sort & sr,
			search_mode smode = S_CONTEXTUAL) const;

  /** Adds a special constant c of given sort s and (possibly) attribute 
      set. sort should not be parametric. This adds a special constant
      checker that may accept only constants equal to c. Such constant
      is accepted only if its sort is equivalent to the given sort s, or
      its sort is initialized to undefined, in which case sort inference
      is performed. */
  void add_special_constant(const special_constant & c, 
			    const sort & s,
			    const attribute_set & attr 
			    = EMPTY_ATTRIBUTE_SET);

  /** Adds a special constant c of given sort s and (possibly) attribute 
      set. sort should not be parametric. This adds a special constant
      checker that may accept only constants equal to c. Such constant
      is accepted only if its sort is equivalent to the given sort s, or
      its sort is initialized to undefined, in which case sort inference
      is performed. */
  void add_special_constant(const special_constant & c, 
			    const sort & s,
			    attribute_set && attr);

  
  /** Adds a special constant type of given sort s and (possibly) attribute
      set. sort should not be paremetric. This adds a special constant
      checker that may accept all constants of given type 
      (e.g. all numerals). Such constant is accepted if its sort is 
      equivalent to the given sort s, or its sort is initialized to undefined,
      in which case sort inference is performed. */
  void add_special_constant_type(special_constant::type tp, 
				 const sort & s,
				 const attribute_set & attr = 
				 EMPTY_ATTRIBUTE_SET);

  /** Adds a special constant type of given sort s and (possibly) attribute
      set. sort should not be paremetric. This adds a special constant
      checker that may accept all constants of given type 
      (e.g. all numerals). Such constant is accepted if its sort is 
      equivalent to the given sort s, or its sort is initialized to undefined,
      in which case sort inference is performed. */
  void add_special_constant_type(special_constant::type tp, 
				 const sort & s,
				 attribute_set && attr);

  
  /** Adds an abstract checker for constant c. It may accept only constants
      equal to c. Such constant sc is accepted by the checker if 
      checker->check_constant(sc, sort) returns true, where sort is the
      sort of the constant sc. If sort is initialized to undefined, then
      sort inference may be performed by the checker. */
  void add_special_constant_checker(const special_constant & c,
				    special_constant_checker * checker);

  /** Adds an abstract checker. It accepts all constants sc for which
      checker->check_constant(sc, sort) returns true, where sort is the
      sort of the constant sc. If the sort is initialized to undefined, then
      sort inference may be performed by the checker. */
  void add_special_constant_checker(special_constant_checker * checker);
   
  /** Checks for special constant c. The method returns the pointer to 
      the special constant checker that accepts the constant, if such
      checker exists. If there is no such checker, the method returns 0. 
      If more than one such checkers exist, it returns the first one found. 
      The smode argument defines the search mode used during the search for
      the symbol. If sort is set to undefined sort, then sort 
      inference may be performed. */
  special_constant_checker *
  check_special_constant(const special_constant & c,
			 sort & sr, 
			 search_mode smode = S_CONTEXTUAL) const;
  
  /** Gets the sort factory of the signature. The sort factory is unique
      in the signature hierarchy. */
  sort_factory * get_sort_factory() const;
  
  /** Gets expansion signature (0 if there is no expansion). */
  signature * get_expansion_signature() const;

  /** Gets the vector of subsignatures (or empty vector, if there are
      no subsignatures). */
  const signature_vector & get_sub_signatures() const;

  /** Destructor. */
  ~signature();
};

// INLINE FUNCTIONS' DEFINITIONS


inline
default_sort_symbol_checker::
default_sort_symbol_checker(arity ar, const attribute_set & attr)
  :_arity(ar),
   _attr(!attr.empty() ? new attribute_set(attr) : &EMPTY_ATTRIBUTE_SET)
{}

inline
default_sort_symbol_checker::
default_sort_symbol_checker(arity ar, attribute_set && attr)
  :_arity(ar),
   _attr(!attr.empty() ? new attribute_set(std::move(attr)) : &EMPTY_ATTRIBUTE_SET)
{}


inline
bool 
default_sort_symbol_checker::check_symbol(const sort_symbol & symbol, arity ar)
{
  return ar == UNDEFINED_ARITY || ar == _arity;
}

inline
arity default_sort_symbol_checker::get_arity(const sort_symbol & symbol)
{
  return _arity;
}

inline
const attribute_set & 
default_sort_symbol_checker::
get_attributes(const sort_symbol & symbol)
{
  return attributes();
}

inline
default_sort_symbol_checker::~default_sort_symbol_checker()
{
  if(_attr != &EMPTY_ATTRIBUTE_SET)
    delete _attr;
}


inline
sort_symbol_registry::sort_symbol_registry(signature * sg)
  :_symbols(HASH_TABLE_SIZE),
   _signature(sg)
{}


inline
void sort_symbol_registry::add_symbol(const sort_symbol & symbol, 
				      arity ar,
				      const attribute_set & attr)
{
  add_symbol_checker(symbol, new default_sort_symbol_checker(ar, attr));
}

inline
void sort_symbol_registry::add_symbol(const sort_symbol & symbol, 
				      arity ar,
				      attribute_set && attr)
{
  add_symbol_checker(symbol, new default_sort_symbol_checker(ar, std::move(attr)));
}


inline
void sort_symbol_registry::add_symbol_checker(const sort_symbol & symbol,
					      sort_symbol_checker * checker)
{
  _symbols.insert(std::make_pair(symbol, checker));
}

inline
void sort_symbol_registry::add_symbol_checker(sort_symbol_checker * checker)
{
  _general_checkers.push_back(checker);
}


inline
void sort_symbol_registry::remove_symbol(const sort_symbol & symbol)
{
  sort_symbol_map::iterator it = _symbols.find(symbol);
  if(it != _symbols.end())
    {
      delete it->second;
      _symbols.erase(it);
    }
}


inline
default_function_symbol_checker::~default_function_symbol_checker()
{
  if(_attr != &EMPTY_ATTRIBUTE_SET)
    delete _attr;
}  

inline
function_symbol_registry::function_symbol_registry(signature * sg)
  :_symbols(HASH_TABLE_SIZE),
   _signature(sg)
{}

inline
void function_symbol_registry::add_symbol_checker(const function_symbol & symbol,
						  function_symbol_checker * checker)
{
  _symbols.insert(std::make_pair(symbol, checker));
}

inline
void function_symbol_registry::add_symbol_checker(function_symbol_checker * checker)
{
  _general_checkers.push_back(checker);
}


inline
void function_symbol_registry::add_symbol(const function_symbol & symbol,
					  const rank & rn,
					  const attribute_set & attr)
{
  add_symbol_checker(symbol, new default_function_symbol_checker(_signature,
								 rn, attr));  
}

inline
void function_symbol_registry::add_symbol(const function_symbol & symbol,
					  rank && rn,
					  const attribute_set & attr)
{
  add_symbol_checker(symbol, new default_function_symbol_checker(_signature,
								 std::move(rn), attr));  
}

inline
void function_symbol_registry::add_symbol(const function_symbol & symbol,
					  const rank & rn,
					  attribute_set && attr)
{
  add_symbol_checker(symbol, new default_function_symbol_checker(_signature,
								 rn, std::move(attr)));  
}

inline
void function_symbol_registry::add_symbol(const function_symbol & symbol,
					  rank && rn,
					  attribute_set && attr)
{
  add_symbol_checker(symbol, new default_function_symbol_checker(_signature,
								 std::move(rn),
								 std::move(attr)));  
}

  
inline
void function_symbol_registry::remove_symbol(const function_symbol & symbol)
{ 
  _symbols.erase(symbol);
}


inline
default_special_constant_checker::~default_special_constant_checker()
{
  if(_attr != &EMPTY_ATTRIBUTE_SET)
    delete _attr;
}


inline
special_constant_type_checker::~special_constant_type_checker()
{
  if(_attr != &EMPTY_ATTRIBUTE_SET)
    delete _attr;
}


inline
special_constant_registry::
special_constant_registry(signature * sg)
  :_constant_checker_map(HASH_TABLE_SIZE),
   _signature(sg)
{}

inline
void special_constant_registry::add_constant(const special_constant & c, 
					     const sort & s,
					     const attribute_set & attr)
{
  add_constant_checker(c, new default_special_constant_checker(_signature, s, attr));
}

inline
void special_constant_registry::add_constant(const special_constant & c, 
					     const sort & s,
					     attribute_set && attr)
{
  add_constant_checker(c, new default_special_constant_checker(_signature, s, std::move(attr)));
}


inline
void special_constant_registry::
add_constant_checker(const special_constant & c,
		     special_constant_checker * checker)
{
  _constant_checker_map.insert(std::make_pair(c, checker));
}

inline
void special_constant_registry::
add_constant_checker(special_constant_checker * checker)
{
  _general_constant_checkers.push_back(checker);
}
  
inline
void special_constant_registry::remove_constant(const special_constant & c)
{
  _constant_checker_map.erase(c);
}

inline
void special_constant_registry::
add_type(special_constant::type tp, const sort & s, const attribute_set & attr)
{
  add_constant_checker(new special_constant_type_checker(_signature,
							 tp, s, attr));
}

inline
void special_constant_registry::
add_type(special_constant::type tp, const sort & s, attribute_set && attr)
{
  add_constant_checker(new special_constant_type_checker(_signature,
							 tp, s, std::move(attr)));
}


inline
signature::type signature::get_type() const
{
  return _type;
}

inline
bool signature::get_syntax_checking() const
{
  return _syntax_checking;
}

inline 
sort_factory * signature::get_sort_factory() const
{
  return _sort_factory;
}

inline
void signature::add_sort_symbol(const sort_symbol & symbol, 
				arity ar,
				const attribute_set & attr)
{
  _sort_symbol_registry->add_symbol(symbol, ar, attr);
}

inline
void signature::add_sort_symbol(const sort_symbol & symbol, 
				arity ar,
			        attribute_set && attr)
{
  _sort_symbol_registry->add_symbol(symbol, ar, std::move(attr));
}


inline
void signature::add_sort_symbol_checker(const sort_symbol & symbol, 
					sort_symbol_checker * checker)
{
  _sort_symbol_registry->add_symbol_checker(symbol, checker);
}

inline
void signature::add_sort_symbol_checker(sort_symbol_checker * checker)
{

  _sort_symbol_registry->add_symbol_checker(checker);
}
  
inline
void signature::add_function_symbol(const function_symbol & symbol, 
				    const rank & rn,
				    const attribute_set & attr)
{
  _function_symbol_registry->add_symbol(symbol, rn, attr);
}

inline
void signature::add_function_symbol(const function_symbol & symbol, 
				    rank && rn,
				    attribute_set && attr)
{
  _function_symbol_registry->add_symbol(symbol, std::move(rn), std::move(attr));
}


inline
void signature::add_function_symbol(const function_symbol & symbol, 
				    const rank & rn,
				    attribute_set && attr)
{
  _function_symbol_registry->add_symbol(symbol, rn, std::move(attr));
}

inline
void signature::add_function_symbol(const function_symbol & symbol, 
				    rank && rn,
				    const attribute_set & attr)
{
  _function_symbol_registry->add_symbol(symbol, std::move(rn), attr);
}

inline
void signature::add_function_symbol_checker(const function_symbol & symbol, 
					    function_symbol_checker * checker)
{
  _function_symbol_registry->add_symbol_checker(symbol, checker);
}

inline
void signature::add_function_symbol_checker(function_symbol_checker * checker)
{
  _function_symbol_registry->add_symbol_checker(checker);
}


inline  
void signature::add_function_symbol(const function_symbol & symbol, 
				    const sort & s,
				    const attribute_set & attr)
{
  add_function_symbol(symbol, rank { s }, attr);
}

inline  
void signature::add_function_symbol(const function_symbol & symbol, 
				    const sort & s,
				    attribute_set && attr)
{
  add_function_symbol(symbol, rank { s }, std::move(attr));
}


inline  
void signature::add_function_symbol(const function_symbol & symbol, 
				    const sort & s1,
				    const sort & s2,
				    const attribute_set & attr)
{
  add_function_symbol(symbol, rank { s1, s2 }, attr);
}

inline  
void signature::add_function_symbol(const function_symbol & symbol, 
				    const sort & s1,
				    const sort & s2,
				    attribute_set && attr)
{
  add_function_symbol(symbol, rank { s1, s2 }, std::move(attr));
}


inline  
void signature::add_function_symbol(const function_symbol & symbol, 
				    const sort & s1,
				    const sort & s2,
				    const sort & s3,
				    const attribute_set & attr)
{
  add_function_symbol(symbol, rank { s1, s2, s3 }, attr);
}

inline  
void signature::add_function_symbol(const function_symbol & symbol, 
				    const sort & s1,
				    const sort & s2,
				    const sort & s3,
				    attribute_set && attr)
{
  add_function_symbol(symbol, rank { s1, s2, s3 }, std::move(attr));
}

inline  
void signature::add_function_symbol(const function_symbol & symbol, 
				    const sort & s1,
				    const sort & s2,
				    const sort & s3,
				    const sort & s4,
				    const attribute_set & attr)
{
  add_function_symbol(symbol, rank { s1, s2, s3, s4 }, attr);
}

inline  
void signature::add_function_symbol(const function_symbol & symbol, 
				    const sort & s1,
				    const sort & s2,
				    const sort & s3,
				    const sort & s4,
				    attribute_set && attr)
{
  add_function_symbol(symbol, rank { s1, s2, s3, s4 }, std::move(attr));
}


inline
void signature::add_special_constant(const special_constant & c, 
				     const sort & s,
				     const attribute_set & attr)
{
  _spec_const_registry->add_constant(c, s, attr);
}

inline
void signature::add_special_constant(const special_constant & c, 
				     const sort & s,
				     attribute_set && attr)
{
  _spec_const_registry->add_constant(c, s, std::move(attr));
}


inline
void signature::add_special_constant_type(special_constant::type tp, 
					  const sort & s,
					  const attribute_set & attr)
{
  _spec_const_registry->add_type(tp, s, attr);
}

inline
void signature::add_special_constant_type(special_constant::type tp, 
					  const sort & s,
					  attribute_set && attr)
{
  _spec_const_registry->add_type(tp, s, std::move(attr));
}

inline
void signature::add_special_constant_checker(const special_constant & c, 
					     special_constant_checker * checker)
{
  _spec_const_registry->add_constant_checker(c, checker);
}

inline
void signature::add_special_constant_checker(special_constant_checker * checker)
{
  _spec_const_registry->add_constant_checker(checker);
}


inline
signature * signature::get_expansion_signature() const
{
  return _expansion_signature;
}


inline
const signature_vector & signature::get_sub_signatures() const
{
  return _sub_signatures;
}

#endif // _SIGNATURE_H

