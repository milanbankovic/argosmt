/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2013 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _THEORY_H
#define _THEORY_H

#include "clause.hpp"

class solver;
class trail;
class explanation;

/* This class is the base class for all theory solvers */
class theory {
protected:
  solver & _solver;
  std::string _name;
  unsigned _index;
  unsigned _num_of_layers;
 
public:
  theory(solver & sl, const std::string & name, unsigned num_of_layers)
    :_solver(sl),
     _name(name),
     _index(0),
     _num_of_layers(num_of_layers)
  {}

  const std::string & get_name() const
  {
    return _name;
  }

  unsigned get_index() const
  {
    return _index;
  }

  void set_index(unsigned index)
  {
    _index = index;
  }

  unsigned get_num_of_layers() const
  {
    return _num_of_layers;
  }

  solver & get_solver()
  {
    return _solver;
  }

  /* Theory interface to be implemented. */
  
  /* Informs the theory that a new literal pair is introduced. */
  virtual void add_literal(const expression & l_pos, const expression & l_neg) = 0;

  /* Informs the theory that a new clause is added to the clause set. */
  virtual void add_clause(clause * cl) {}
  
  /* Informs the theory that the clauses are removed from the clause set. */
  virtual void remove_clauses(const std::vector<clause *> & learnt_clauses,
			      unsigned number_of_clauses) {}

  /* Informs the theory that new decision level is established.  */
  virtual void new_level() = 0;
  
  /* Informs the theory that the state has backjumped to given level.  */
  virtual void backjump(unsigned level) = 0;
  
  /* Informs the theory that new literal is asserted to the trail. */
  virtual void assert_literal(const expression & l) = 0;

  /* Asks the theory to check the consistency of the current assertions.
     If the assertions are T-inconsistent, the conflict explanation should
     be generated and then Conflict rule should be applied. Otherwise, the
     theory should check for T-infered literals that should be propagated
     to the assertion trail by applying Propagate rule. Parameter layer
     determines the layer at which the above checks are performed. Greater
     layers should be deductively stronger (and invoke more expensive
     algorithms). If the trail is complete (i.e. all literals are assigned)
     the theory _must_ guarantee that the assertions are T-consistent. Otherwise,
     the theory may choose to only periodically check for propagations and 
     conflicts at some layer. */
  virtual void check_and_propagate(unsigned layer) = 0;
 
  /* Asks the theory to explain the literal l. The theory should generate
     explanation and then apply Explain rule */
  virtual void explain_literal(const expression & l) = 0;
  
  /* Asks the theory whether the given expression is owned by the theory. */
  virtual bool is_owned_expression(const expression & e) = 0;
  
  /* Asks the theory to canonize the expression e.  */
  virtual expression canonize_expression(const expression & e) = 0;
  
  /* Asks the theory to provide the literal pair for l. */
  virtual void get_literal_pair(const expression & l, 
				expression & l_pos,
				expression & l_neg) = 0;

  /* Asks the theory to verify the assignment given by the trail. Should
     return true if the model is consistent, and false otherwise. */
  virtual bool verify_assignment(const trail & tr) = 0;
  
  /* Asks the theory to set the values assigned to the expressions. */
  virtual void get_model(const expression_vector & exps) = 0;

  /* Asks the theory to print its specific report about solving */
  virtual void print_report(std::ostream & ostr) = 0;
  
  /* Virtual destructor. */
  virtual ~theory() {}
};

#endif // _THEORY_H
