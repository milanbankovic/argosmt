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

#ifndef _SORTS_H
#define _SORTS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <algorithm>

#include "basic_types.hpp"
#include "search_mode.hpp"

class signature;
class sort_factory;
class sort_node;
class sort_visitor;

/** Represents a shared pointer to a sort node. Sorts are represented as
    tree like structures that reside within the sort factory. Each sort
    node is unique in the factory and represents a distinct sort. Common
    subsorts are shared between different sort nodes. Sort nodes can 
    be accessed by the user only through Sort shared pointers. Two shared
    pointers represent the same sort if and only if they point to the same
    sort node in the factory. Each sort node is destroyed automatically by
    the factory when the last shared pointer pointing to it is destroyed. */
typedef std::shared_ptr<sort_node> sort;

/** Vector of sorts. */
typedef std::vector <sort> sort_vector;

/** Empty sort vector */
extern const sort_vector EMPTY_SORT_VECTOR;

/** Represents an instantiation (a mapping of sort parameters to sorts). */
typedef std::unordered_map <sort_parameter, sort> instantiation;

/** Represents a node within a sort factory. It is the
    base class for several other classes representing different types
    of sort nodes. Its interface is available through Sort shared 
    pointers that are used for manipulation with sort nodes. This class
    should never be used directly by the user. Sort nodes can be created 
    only within a sort factory and should be accessed only by using 
    Sort shared pointers. */
class sort_node : public std::enable_shared_from_this<sort_node> {
protected:
  sort_factory * _factory;
  attribute_set * _attributes;

  std::size_t _hash_code;

  /** Constructor is protected. */
  sort_node(sort_factory * factory);
public:

  /** Represents types of sort nodes. */
  enum type {
    /** Represents a node consisting of one sort parameter */
    T_PARAMETER, 
    /** Represents a node consisting of sort symbol (in case of
	arity 0), or sort symbol applied to subsorts (in case of arity
	greater than 0) */
    T_SYMBOL,
    /** Represents undefined sort (used when syntax checking is off)  */
    T_UNDEF
  };

  /** Get sort factory in which the node resides. */
  sort_factory * get_factory() const;
    
  /** Returns true if the node has assigned attributes. */
  bool has_attributes() const;

  /** Returns the reference to the set of attributes. Through this reference
      the set can be changed (if we want to add some new attributes, 
      for instance). */
  attribute_set & get_attributes();

  /** Returns true if attribute with the given keyword is set for the sort. */
  bool is_attribute_set(const keyword & kw) const;

  /** Returns a pointer to the attribute value object of the sort attribute
      with the given keyword, or 0 if the attribute (or its value) is not
      set. */
  attribute_value * get_attribute_value(const keyword & kw) const;

  /** Gets a value of type T that corresoponds to the given
      keyword (the second argument is a non-const reference to T object
      to which the value is assigned). If there is no such attribute, or
      its value is not of the type T, an exception is thrown. */
  template <typename T>
  void get_attribute_value(const keyword & kw, T & value) const;

  /** Sets the value of the sort attribute with the given keyword to 
      the given value of type T. */
  template <typename T>
  void set_attribute_value(const keyword & kw, T && value);

  /** Sets the sort attribute. */ 
  void set_attribute(const attribute & attr);

  /** Sets the sort attribute. */ 
  void set_attribute(attribute && attr);
  
  /** Returns the type of the node. */
  virtual type get_type() const = 0;

  /** Equivalent to get_type() == T_SYMBOL. */
  bool is_symbol() const;
    
  /** Equivalent to get_type() == T_SYMBOL && get_operands().empty(). */
  bool is_simple() const;

  /** Equivalent to get_type() == T_PARAMETER. */
  bool is_parameter() const;
    
  /** Equivalent to get_type() == T_UNDEF. */
  bool is_undefined() const;


  /** Returns the top-level symbol of the node (in case of T_SYMBOL), or
      sort_symbol::UNDEFINED, otherwise. */ 
  virtual const sort_symbol & get_symbol() const = 0;
  
  /** Returns the parameter represented by the node (in case of T_PARAMETER),
      or sort_parameter::UNDEFINED, otherwise. */
  virtual const sort_parameter & get_parameter() const = 0;

  /** Returns the set of all parameters appearing in the sort represented by 
      the node. */
  virtual const sort_parameter_set & get_parameters() = 0;
 
  /** Returns true if the sort represented by the node contains parameters. */
  bool has_parameters();
  
  /** Returns the vector of direct subsorts of the sort represented by 
      the node (in case of T_SYMBOL), or EMPTY_SORT_VECTOR, otherwise. */
  virtual const sort_vector & get_operands() const = 0;

  /** Returns the sort obtained by instantiating the parameters with sorts
      given in instantiation in the sort represented by the node. Recall
      that instantiation is a map that maps sort_parameters to sorts which
      replace them. */
  virtual sort get_instance(const instantiation & in) const = 0;

  /** Returns true if the sort represented by the argument 
      can be obtained from this sort, using some instantiation (which is then
      returned in the instantiation argument). If the method is called with
      a non-empty instantiation, then mappings found in instantiation at the
      time of call must be preserved, i.e. we check whether the sort given
      by the argument is an instance of this sort with respect to the given
      instantiation. */
  virtual bool is_instance(const sort & sr,
			   instantiation & in) const = 0;
  
  /** Returns true if the sort is syntactically correct with respect to the
      given signature. Notice that undefined sort is _not_ syntactically
      correct in any signature. The argument smode defines the search mode. 
      For more info about search modes, see search_mode type documentation. */
  virtual bool check_sort(const signature * sg,
			  search_mode smode = S_CONTEXTUAL) const = 0;

  /** Returns the sort obtained from this by expanding sort symbol 
      definitions. The smode argument defines in which signatures the 
      definitions should be searched (starting with the signature given 
      by the first argument). */
  virtual sort expand_sort(const signature * sg, 
			   search_mode smode = S_CONTEXTUAL) const = 0;
 
  /** Expands sort with respect to the main signature and _contextual_ search
      mode (default mode). */
  sort expand_sort() const;

  /** Returns true if the sort given by the argument is equivalent to 
      this sort. Two sorts s1 and s2 are equivalent if 
      s1->expand_sort() == s2->expand_sort(). */
  bool is_equivalent(const sort & sr) const;
  
  /** Returns true if the sort given by the first argument is instance 
      equivalent to this sort. Two sorts s1 and s2 are instance equivalent if 
      s1->expand_sort()->is_instance(s2->expand_sort(), ins). */
  bool is_instance_equivalent(const sort & sr, instantiation & ins) const;

  /** Support for visitor design pattern. Subsorts are traversed in a
      depth-first order, applying the visitor to each node. */
  virtual void accept_visitor(sort_visitor & visitor) = 0;
    
  /** Class iterator enables iteration through all subsorts (recursively).
      It is a forward-only iterator. Subsorts are traversed in a pre-order
      fashion. */
  class iterator {
    friend class sort_node;
  private:
    std::vector<unsigned> _inds;
    std::vector<sort> _sorts;
    
    /** private constructor */
    iterator(const sort & s);
  public:

    /** default constructor */
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
    const sort & operator * () const;
      
    /** returns sort currently pointed to by this iterator.
	This enables using operator -> directly with iterators 
	(for instance it->getSymbol() instead of (*it)->getSymbol()). */
    const sort & operator -> () const;
  };

  /** All iterators are also const_iterators. */
  typedef iterator const_iterator;

  /** Returns the iterator that points to the root sort node (i.e. this
      sort). */
  iterator begin();

  /** Returns the invalid iterator (i.e. iterator obtained by applying
      ++ operator to the last valid iterator). */
  iterator end();

  virtual bool equal_to(const sort_node * node) const = 0;
  std::size_t hash_code() const;
  virtual void out(std::ostream & ostr) const = 0;
  virtual bool less_than(const sort_node * node) const = 0;

  /** Returns the complexity of the sort. */
  virtual unsigned get_complexity() const = 0;

  /** Returns a string representation of the sort. */
  std::string to_string() const;

  /** Virtual destructor. */
  virtual ~sort_node();
};

namespace std {
  
    template<>
    struct hash<sort_node*> :
      public unary_function<sort_node *, std::size_t> {
      std::size_t operator() (sort_node * s) const
      {
	return s->hash_code();
      }
    };
    
  
  template<>
  struct equal_to<sort_node*> :
    public binary_function<sort_node *, 
			   sort_node *, bool> {
    bool operator () (sort_node * s, 
		      sort_node * t) const
    {
      return  s->equal_to(t);
    }
  };

    template<>
    struct hash< ::sort > :
      public unary_function< ::sort, std::size_t > {
      std::size_t operator() (const ::sort & s) const
      {
	return s->hash_code();
      }
    };
    
}


/** Returns true if s1 is lexicographically lower than  s2. */
bool operator < (const sort & s1, const sort & s2);

/** Returns true if s1 is lexicographically greater than  s2. */
bool operator > (const sort & s1, const sort & s2);

/** Returns true if s1 is lexicographically lower or equal to s2. */
bool operator <= (const sort & s1, const sort & s2);

/** Returns true if s1 is lexicographically greater or equal to s2. */
bool operator >= (const sort & s1, const sort & s2);

namespace std {
  template <>
  struct less<::sort> {    
    bool operator () (const ::sort & s1, const ::sort & s2) const
    {
      return s1 < s2;
    }
  };
}

/** Prints the sort to the output stream. */
std::ostream & operator << (std::ostream & ostr, const sort & sr);

/** Set of sorts. */
typedef std::unordered_set<sort> sort_set;

/** Represents undefined sort. */
class undefined_sort_node : public sort_node {
  friend class sort_factory;
private:
  
  /** The constructor is private. The node can be only created within 
      the factory. */
  undefined_sort_node(sort_factory * factory);
public:
  virtual type get_type() const;
  virtual const sort_symbol & get_symbol() const;
  virtual const sort_parameter & get_parameter() const;
  virtual const sort_parameter_set & get_parameters();
  virtual const sort_vector & get_operands() const;
  virtual sort get_instance(const instantiation & in) const;
  virtual bool is_instance(const sort & sr, 
			   instantiation & in) const;
  virtual bool check_sort(const signature * sg,
			  search_mode smode = S_CONTEXTUAL) const;
  virtual sort expand_sort(const signature * sg, 
			   search_mode smode = S_CONTEXTUAL) const;
  virtual void accept_visitor(sort_visitor & visitor);

  virtual unsigned get_complexity() const;

  virtual bool equal_to(const sort_node * node) const;
  virtual void out(std::ostream & ostr) const;
  virtual bool less_than(const sort_node * node) const;

};


/** Represents parameter sort nodes. */
class parameter_sort_node : public sort_node {
  friend class sort_factory;
private:
  sort_parameter _parameter;

  const sort_parameter_set * _parameters;

  void calculate_hash();

  parameter_sort_node(sort_factory * factory);

  /** The constructor is private. The node can be only created within 
      the factory. */
  parameter_sort_node(sort_factory * factory, const sort_parameter & p);
public:
  virtual type get_type() const;  
  virtual const sort_symbol & get_symbol() const;
  virtual const sort_parameter & get_parameter() const;
  virtual const sort_parameter_set & get_parameters();
  virtual const sort_vector & get_operands() const;  
  virtual sort get_instance(const instantiation & in) const;
  virtual bool is_instance(const sort & sr,
			   instantiation & in) const;
  virtual bool check_sort(const signature * sg,
			  search_mode smode = S_CONTEXTUAL) const;
  virtual sort expand_sort(const signature * sg, 
			   search_mode smode = S_CONTEXTUAL) const;
  virtual void accept_visitor(sort_visitor & visitor);
    
  virtual unsigned get_complexity() const;

  virtual bool equal_to(const sort_node * node) const;
  virtual void out(std::ostream & ostr) const;
  virtual bool less_than(const sort_node * node) const;

  virtual ~parameter_sort_node();
};


/** Represents symbol sort nodes. */
class symbol_sort_node : public sort_node {
  friend class sort_factory;
private:
  sort_symbol _symbol;
  
  const sort_parameter_set * _parameters;
  const sort_vector * _operands;

  void calculate_hash();

  symbol_sort_node(sort_factory * factory);
    
  /** The constructor is private. The node can be only created within 
      the factory. */
  symbol_sort_node(sort_factory * factory,
		   const sort_symbol & symbol, 
		   const sort_vector & operands = EMPTY_SORT_VECTOR);

  /** The constructor is private. The node can be only created within 
      the factory. */
  symbol_sort_node(sort_factory * factory,
		   const sort_symbol & symbol, 
		   sort_vector && operands);

  static sort_vector _dummy_sort_vector;
  
public:
  virtual type get_type() const;
  virtual const sort_symbol & get_symbol() const;
  virtual const sort_parameter & get_parameter() const;
  virtual const sort_parameter_set & get_parameters();
  virtual const sort_vector & get_operands() const;
  virtual sort get_instance(const instantiation & in) const;
  virtual bool is_instance(const sort & sr,
			   instantiation & in) const;
  virtual bool check_sort(const signature * sg,
			  search_mode smode = S_CONTEXTUAL) const;
  virtual sort expand_sort(const signature * sg, 
			   search_mode smode = S_CONTEXTUAL) const;
  virtual void accept_visitor(sort_visitor & visitor);

  virtual unsigned get_complexity() const;

  virtual bool equal_to(const sort_node * node) const;  
  virtual void out(std::ostream & ostr) const; 
  virtual bool less_than(const sort_node * node) const;

  virtual ~symbol_sort_node();
};

/** Represents a definition of the sort symbol. 
    A sort symbol is defined with a vector of parameters and a
    (parametric) sort in which these parameters (and only these) exist.
    Such definitions are provided in SMT-LIB 2.0 scripts, and are 
    used for sort expansion. */
class sort_definition {
  const sort_parameter_vector * _pars;
  sort _sort;
public:
  /** Constructor. */
  sort_definition(const sort_parameter_vector & pars, const sort & sr);

  /** Constructor. */
  sort_definition(sort_parameter_vector && pars, const sort & sr);

  
  /** Copy constructor. */
  sort_definition(const sort_definition & def);

  /** Move constructor. */
  sort_definition(sort_definition && def);

  
  /** Assignment operator. */
  sort_definition & operator = (const sort_definition & def);

  /** Move assignment operator. */
  sort_definition & operator = (sort_definition && def);

  
  /** Gets the parameters of the definition. */
  const sort_parameter_vector & get_parameters() const;
  
  /** Gets the sort of the definition. */
  const sort & get_sort() const;
 
  void out(std::ostream & ostr) const;
  
  /** Destructor. */
  ~sort_definition();
};

/** Prints the definition to the output stream */
std::ostream & operator << (std::ostream & ostr, const sort_definition & def);

/** Standard attribute value for sort_definition. Sort definitions are 
    attached to sort symbols as the value of :definition attribute */
typedef standard_attribute_value<sort_definition> sort_definition_attribute_value;


/** Type of exception thrown when bad sort symbol is used. */
class bad_sort_symbol_exception : public base_exception {
private:
  sort_symbol _symbol;
public:
  /** Constructor. */
  bad_sort_symbol_exception(const std::string & message, 
			    const sort_symbol & symbol)
    :base_exception(message + 
		    std::string(": bad sort symbol: ") + symbol.to_string()),
     _symbol(symbol)
  {}
  
  /** Gets the (bad) sort symbol. */
  const sort_symbol & get_symbol() const
  {
    return _symbol;
  }

};



/** Type of exception thrown when bad sort is used. */
class bad_sort_exception : public base_exception {
protected:
  sort _sort;
public:
  /** Constructor. */
  bad_sort_exception(const std::string & message, const sort & sr)
    :base_exception(message + std::string(": bad sort: ") + sr->to_string()),
     _sort(sr)
  {}
  
  /** Gets the (bad) sort. */
  const sort & get_sort() const
  {
    return _sort;
  }
};

/** Represents the factory in which sort nodes reside. 
    It implements hash table in which nodes are stored. It is responsible for
    hash consing and garbage collecting of sort nodes. */
class sort_factory {
  friend class signature;
private:
  class sort_deleter {
  private:
    sort_factory * _factory;
  public:
    sort_deleter(sort_factory * factory)
      :_factory(factory)
    {}

    void operator () (sort_node * node)
    {
      _factory->remove_sort(node);
    }
  };


  std::unordered_set<sort_node *> _sorts;

  void remove_sort(sort_node * node);
  sort insert_sort(sort_node * node);

  sort _undefined_sort;
  sort _bool_sort;

  signature * _signature;
  bool _destruction;

  parameter_sort_node _par_node;
  symbol_sort_node _sym_node;

  /** Sort factory cannot be created by the user. It is created by the
      main signature automatically. See documentation for signature class. */
  sort_factory(signature * sg);
public:

  /** Returns a pointer to the main signature in which this factory
      is created. */
  signature * get_signature() const;

  /** Returns undefined sort. */
  const sort & UNDEFINED_SORT() const;

  /** Returns Bool sort. */
  const sort & BOOL_SORT() const;

   
  /** Creates sort parameter node and returns a shared
      pointer to the corresponding sort. The sort is created
      if it does not already exist in the factory. Otherwise, 
      a shared pointer to existing node is returned. */
  sort create_sort(const sort_parameter & par);
   
  /** Creates sort symbol node (with optional subsorts) and returns 
      a shared pointer to the corresponding sort. The sort is created
      if it does not already exist in the factory. Otherwise, 
      a shared pointer to existing node is returned. */
  sort create_sort(const sort_symbol & symbol, 
		   const sort_vector & sorts = EMPTY_SORT_VECTOR);

  /** Creates sort symbol node (with optional subsorts) and returns 
      a shared pointer to the corresponding sort. The sort is created
      if it does not already exist in the factory. Otherwise, 
      a shared pointer to existing node is returned. */
  sort create_sort(const sort_symbol & symbol, 
		   sort_vector && sorts);

  
  /** Creates sort symbol node with one subsort and returns a shared
      pointer to the corresponding sort. The sort is created
      if it does not already exist in the factory. Otherwise, 
      a shared pointer to existing node is returned. */
  sort create_sort(const sort_symbol & symbol, 
		   const sort & s);

  /** Creates sort symbol node with two subsorts and returns a shared
      pointer to the corresponding sort. The sort is created
      if it does not already exist in the factory. Otherwise, 
      a shared pointer to existing node is returned.  */
  sort create_sort(const sort_symbol & symbol, 
		   const sort & s1, 
		   const sort & s2);
  
  /** Destructor (deletes all sort nodes). */
  ~sort_factory();
};

/** Represents an abstract visitor that can be applied to all sort nodes
    in a given sort. The visitor is applied to all nodes in the sort 
    in a depth-first order. */
class sort_visitor {
public:
  /** Method is applied to the undefined sort node. */
  virtual void visit_undefined_sort_node(const sort & node) = 0;

  /** Method is applied to a symbol sort node before its 
      subsorts are visited. */
  virtual void visit_symbol_sort_node_preorder(const sort & node) = 0;
    
  /** Method is applied to a symbol sort node after its 
      subsorts are visited. */
  virtual void visit_symbol_sort_node_postorder(const sort & node) = 0;

  /** Method is applied to parameter sort nodes. */
  virtual void visit_parameter_sort_node(const sort & node) = 0;
    
  /** Virtual destructor. */
  virtual ~sort_visitor() {}
};


// INLINE FUNCTION DEFINITIONS 

inline
sort_node::sort_node(sort_factory * factory)
  :_factory(factory),
   _attributes(0),
   _hash_code(0)
{}

inline
bool sort_node::has_attributes() const
{
  return _attributes != 0 && !_attributes->empty();
}

inline
attribute_set & sort_node::get_attributes()
{
  if(_attributes)
    return *_attributes;
  else 
    return *(_attributes = new attribute_set(HASH_TABLE_SIZE));
}

inline 
bool sort_node::is_attribute_set(const keyword & kw) const
{
  if(_attributes == 0)
    return false;
    
  if(_attributes->find(kw) == _attributes->end())
    return false;
    
  return true;
}

inline
sort_factory * 
sort_node::get_factory() const
{
  return _factory;
}

inline
std::string sort_node::to_string() const
{
  return ::to_string(this);
}

inline
bool sort_node::has_parameters()
{
  return get_parameters().size() != 0;
}

inline
attribute_value * 
sort_node::get_attribute_value(const keyword & kw) const
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
sort_node::get_attribute_value(const keyword & kw, T & value) const
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
sort_node::set_attribute_value(const keyword & kw, T && value)
{
  set_attribute(attribute(kw, std::forward<T>(value)));
}


inline
void 
sort_node::set_attribute(const attribute & attr)
{
  attribute_set & attributes = get_attributes();
  attributes.erase(attr);
  attributes.insert(attr);
}

inline
void 
sort_node::set_attribute(attribute && attr)
{
  attribute_set & attributes = get_attributes();
  attributes.erase(attr);
  attributes.insert(std::move(attr));
}


inline
bool sort_node::is_symbol() const
{
  return get_type() == T_SYMBOL;
}
   
inline
bool sort_node::is_simple() const
{
  return get_type() == T_SYMBOL && get_operands().empty();
}

inline
bool sort_node::is_parameter() const
{
  return get_type() == T_PARAMETER;
}
    
inline
bool sort_node::is_undefined() const
{
  return get_type() == T_UNDEF;
}

inline
bool operator < (const sort & s1, const sort & s2)
{
  return s1->less_than(s2.get());
}

inline
bool operator > (const sort & s1, const sort & s2)
{
  return s2 < s1;
}

inline
bool operator <= (const sort & s1, const sort & s2)
{
  return s1 == s2 || s1 < s2;
}

inline
bool operator >= (const sort & s1, const sort & s2)
{
  return s1 == s2 || s1 > s2;
}

inline 
std::ostream & operator << (std::ostream & ostr, const sort & sr)
{
  sr->out(ostr);
  return ostr;
}

inline
sort sort_node::expand_sort() const
{
  return this->expand_sort(_factory->get_signature(), S_CONTEXTUAL);
}

inline
bool sort_node::is_equivalent(const sort & sr) const
{
  return this == sr.get() || this->expand_sort() == sr->expand_sort();
}

inline
bool sort_node::
is_instance_equivalent(const sort & sr, instantiation & ins) const
{
  return this == sr.get() || 
    this->expand_sort()->is_instance(sr->expand_sort(), ins);
}

inline
std::size_t sort_node::hash_code() const
{
  return _hash_code;
}

inline 
sort_node::~sort_node()
{
  delete _attributes;
}
  
inline
sort_node::iterator::iterator(const sort & s)
{
  _sorts.push_back(s);
  _inds.push_back((unsigned)(-1));
}
   
inline
sort_node::iterator::iterator()
{}
	
  
inline
const sort_node::iterator sort_node::iterator::operator ++ (int)
{
  iterator it(*this);
  ++(*this);
  return it;
}
  
inline
bool sort_node::iterator::operator == (const iterator & it) const
{
  return _sorts == it._sorts && _inds == it._inds;
}
  
inline
bool sort_node::iterator::operator != (const iterator & it) const
{
  return _sorts != it._sorts || _inds != it._inds;
}
  
inline
const sort & sort_node::iterator::operator * () const
{
  return _sorts.back();
}
  
inline
const sort & sort_node::iterator::operator -> () const
{
  return _sorts.back();
}
  
inline
sort_node::iterator sort_node::begin()
{
  return iterator(this->shared_from_this());
}
  
inline
sort_node::iterator sort_node::end()
{
  return iterator();
}
  


inline
undefined_sort_node::undefined_sort_node(sort_factory * factory)
  :sort_node(factory)
{}

inline
sort_node::type undefined_sort_node::get_type() const
{
  return T_UNDEF;
}

inline
const sort_symbol & undefined_sort_node::get_symbol() const
{
  return sort_symbol::UNDEFINED;
}

inline
const sort_parameter & undefined_sort_node::get_parameter() const
{
  return sort_parameter::UNDEFINED;
}

inline
const sort_parameter_set & undefined_sort_node::get_parameters()
{
  return EMPTY_SORT_PARAMETER_SET;
}


inline
const sort_vector & undefined_sort_node::get_operands() const
{
  return EMPTY_SORT_VECTOR;
}

inline
sort undefined_sort_node::get_instance(const instantiation & in) const
{
  return const_cast<undefined_sort_node*>(this)->shared_from_this();
}

inline
bool undefined_sort_node::is_instance(const sort & sr,
				      instantiation & in) const
{
  return sr->is_undefined();
}

inline
bool undefined_sort_node::equal_to(const sort_node * node) const
{
  return node->get_type() == T_UNDEF;
}

inline
void undefined_sort_node::out(std::ostream & ostr) const
{
  ostr << "UNDEFINED";
}

inline
bool undefined_sort_node::less_than(const sort_node * node) const
{
  return false;
}

inline
bool undefined_sort_node::check_sort(const signature * sg,
				     search_mode smode) const
{
  return false;
}

inline
sort undefined_sort_node::expand_sort(const signature * sg, 
				      search_mode smode) const
{
  return const_cast<undefined_sort_node*>(this)->shared_from_this();
}

inline
void undefined_sort_node::accept_visitor(sort_visitor & visitor)
{
  visitor.visit_undefined_sort_node(this->shared_from_this());
}

inline
unsigned undefined_sort_node::get_complexity() const
{
  return 0;
}


inline 
void
parameter_sort_node::calculate_hash()
{
  _hash_code = _parameter.hash_code();
}

inline
parameter_sort_node::parameter_sort_node(sort_factory * factory, 
					 const sort_parameter & p)
  :sort_node(factory),
   _parameter(p),
   _parameters(0)
{
  calculate_hash();
}

inline
parameter_sort_node::parameter_sort_node(sort_factory * factory)
  :sort_node(factory),
   _parameter(sort_parameter::UNDEFINED),
   _parameters(0)
{}


inline
sort_node::type parameter_sort_node::get_type() const
{
  return T_PARAMETER;
}

inline
const sort_symbol & parameter_sort_node::get_symbol() const
{
  return sort_symbol::UNDEFINED;
}

inline
const sort_parameter & parameter_sort_node::get_parameter() const
{
  return _parameter;
}

inline
const sort_parameter_set & parameter_sort_node::get_parameters()
{
  if(_parameters == 0)
    {
      sort_parameter_set * temp = new sort_parameter_set(HASH_TABLE_SIZE);
      temp->insert(_parameter);
      return *(_parameters = temp);
    }
  
  return *_parameters;
}


inline
const sort_vector & parameter_sort_node::get_operands() const
{
  return EMPTY_SORT_VECTOR;
}

inline
sort parameter_sort_node::get_instance(const instantiation & in) const
{
  instantiation::const_iterator it = in.find(_parameter);
  
  if(it != in.end())
    {
      return it->second;
    }
  else
    return const_cast<parameter_sort_node*>(this)->shared_from_this();
}

inline
bool parameter_sort_node::is_instance(const sort & sr,
				      instantiation & in) const
{
  instantiation::const_iterator it = in.find(_parameter);
  
  if(it != in.end())
    return it->second == sr;
  else
    {
      in.insert(std::pair<sort_parameter, sort> 
		(_parameter, sr));
      return true;
    }
}

inline
bool parameter_sort_node::equal_to(const sort_node * node) const
{
  return node->get_type() == T_PARAMETER && 
    node->get_parameter() == _parameter;
}

inline
void parameter_sort_node::out(std::ostream & ostr) const
{
  _parameter.out(ostr);
}

inline
bool parameter_sort_node::less_than(const sort_node * node) const
{
  return get_type() < node->get_type() ||
    (get_type() == node->get_type() && _parameter < node->get_parameter());
}
   
inline
bool parameter_sort_node::check_sort(const signature * sg,
				     search_mode smode) const
{
  return true;
}

inline
sort parameter_sort_node::expand_sort(const signature * sg, 
				      search_mode smode) const
{
  return const_cast<parameter_sort_node*>(this)->shared_from_this();
}

inline
void parameter_sort_node::accept_visitor(sort_visitor & visitor)
{
  visitor.visit_parameter_sort_node(this->shared_from_this());
}

inline
unsigned parameter_sort_node::get_complexity() const
{
  return 1;
}

inline
parameter_sort_node::~parameter_sort_node()
{
  delete _parameters;
}

inline
symbol_sort_node::symbol_sort_node(sort_factory * factory)
  :sort_node(factory),
   _symbol(sort_symbol::UNDEFINED),
   _parameters(0),
   _operands(&EMPTY_SORT_VECTOR)
{}

inline
sort_node::type symbol_sort_node::get_type() const
{
  return T_SYMBOL;
}

inline
const sort_symbol & symbol_sort_node::get_symbol() const
{
  return _symbol;
}

inline
const sort_parameter & symbol_sort_node::get_parameter() const
{
  return sort_parameter::UNDEFINED;
}

inline
const sort_vector & symbol_sort_node::get_operands() const
{
  return *_operands;
}

inline
bool symbol_sort_node::equal_to(const sort_node * node) const
{
  return node->get_type() == T_SYMBOL && 
    node->get_symbol() == get_symbol() &&
    node->get_operands() == get_operands();
}

inline
bool symbol_sort_node::less_than(const sort_node * node) const
{
  return get_type() < node->get_type() ||
    (get_type() == node->get_type() && 
     (get_symbol() < node->get_symbol() || 
      (get_symbol() == node->get_symbol() &&
       get_operands() < node->get_operands())));
}

inline
symbol_sort_node::~symbol_sort_node()
{
  if(_operands != &EMPTY_SORT_VECTOR)
    delete _operands;
  if(_parameters != &EMPTY_SORT_PARAMETER_SET)
    delete _parameters;
}

inline
sort_definition::
sort_definition(const sort_parameter_vector & pars, const sort & sr)
  :_pars(!pars.empty() ? 
	 new sort_parameter_vector(pars) : 
	 &EMPTY_SORT_PARAMETER_VECTOR),
   _sort(sr)
{}

inline
sort_definition::
sort_definition(sort_parameter_vector && pars, const sort & sr)
  :_pars(!pars.empty() ? 
	 new sort_parameter_vector(std::move(pars)) : 
	 &EMPTY_SORT_PARAMETER_VECTOR),
   _sort(sr)
{}


inline
sort_definition::
sort_definition(const sort_definition & def)
  :_pars(def._pars != &EMPTY_SORT_PARAMETER_VECTOR ? 
	 new sort_parameter_vector(*def._pars) :
	 &EMPTY_SORT_PARAMETER_VECTOR),
   _sort(def._sort)
{}

inline
sort_definition::
sort_definition(sort_definition && def)
  :_pars(def._pars),
   _sort(std::move(def._sort))
{
  def._pars = nullptr;
}


inline
sort_definition & 
sort_definition::operator = (const sort_definition & def)
{
  if(&def == this)
    return *this;
    
  if(_pars != &EMPTY_SORT_PARAMETER_VECTOR)
    delete _pars;
    
  _pars = def._pars != &EMPTY_SORT_PARAMETER_VECTOR ? 
    new sort_parameter_vector(*def._pars) :
    &EMPTY_SORT_PARAMETER_VECTOR;
    
  _sort = def._sort;
    
  return *this;
}

inline
sort_definition & 
sort_definition::operator = (sort_definition && def)
{
  if(&def == this)
    return *this;
    
  if(_pars != &EMPTY_SORT_PARAMETER_VECTOR)
    delete _pars;
    
  _pars = def._pars;
  _sort = std::move(def._sort);
  def._pars = nullptr;
    
  return *this;
}


inline
const sort_parameter_vector & sort_definition::get_parameters() const
{
  return *_pars;
}
  
inline
const sort & sort_definition::get_sort() const
{
  return _sort;
}
  
inline
void sort_definition::out(std::ostream & ostr) const
{
  ostr << _sort;
}
  
inline
sort_definition::~sort_definition()
{
  if(_pars != &EMPTY_SORT_PARAMETER_VECTOR)
    delete _pars;
}

inline
std::ostream & operator << (std::ostream & ostr, const sort_definition & def)
{
  def.out(ostr);
  return ostr;
}


inline
void sort_factory::remove_sort(sort_node * node)
{
  if(_destruction)
    return;

  _sorts.erase(node);
  delete node;
}
  
inline
signature * 
sort_factory::get_signature() const
{
  return _signature;
}

inline
const sort & sort_factory::UNDEFINED_SORT() const
{
  return _undefined_sort;
}

inline
const sort & sort_factory::BOOL_SORT() const
{
  return _bool_sort;
}

inline
sort sort_factory::create_sort(const sort_parameter & par)
{
  _par_node._parameter = par;
  _par_node.calculate_hash();

  std::unordered_set<sort_node *>::iterator it = _sorts.find(&_par_node);
    
  if(it != _sorts.end())
    return (*it)->shared_from_this();

  return insert_sort(new parameter_sort_node(this, par));
}


inline
sort sort_factory::create_sort(const sort_symbol & symbol, const sort & s)
{
  return create_sort(symbol, sort_vector { s });
}

inline
sort sort_factory::create_sort(const sort_symbol & symbol, 
			       const sort & s1, const sort & s2)
{
  return create_sort(symbol, sort_vector { s1, s2 });
}

inline
sort sort_factory::create_sort(const sort_symbol & symbol, 
			       const sort_vector & sorts)
{
  return create_sort(symbol, sorts.empty() ? std::move(symbol_sort_node::_dummy_sort_vector) : std::move(sort_vector(sorts)));
}

inline
sort sort_factory::create_sort(const sort_symbol & symbol, 
			       sort_vector && sorts)
{
  _sym_node._symbol = symbol;
  _sym_node._operands = sorts.empty() ? &EMPTY_SORT_VECTOR : &sorts;
  _sym_node.calculate_hash();

  std::unordered_set<sort_node *>::iterator it = _sorts.find(&_sym_node);
  
  if(it != _sorts.end())
    return (*it)->shared_from_this();


  return insert_sort(new symbol_sort_node(this, symbol, std::move(sorts)));
}


#endif // _SORTS_H
