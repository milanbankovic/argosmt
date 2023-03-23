/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2013,2021 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _THEORY_SOLVER_H
#define _THEORY_SOLVER_H

#include "clause.hpp"
#include "proofs.hpp"

class solver;
class trail;
class explanation;

/* This class is the base class for all theory solvers */
class theory_solver {
protected:
  solver & _solver;
  std::string _name;
  unsigned _index;
  unsigned _num_of_layers;
 
public:
  theory_solver(solver & sl, const std::string & name, unsigned num_of_layers)
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
  
  /* Informs the theory solver that a new literal pair is introduced.
     IMPORTANT: This method should not affect the solver's state
     (i.e. invoke _solver.apply_* methods). This method should be
     used only to initialize needed internal data structures of the
     theory solver. */
  virtual void add_literal(const expression & l_pos, const expression & l_neg) = 0;

  /* Informs the theory solver that a new decision level is
     established.  */
  virtual void new_level() = 0;
  
  /* Informs the theory solver that the state has backjumped to the given
     level.  */
  virtual void backjump(unsigned level) = 0;
  
  /* Asks the theory solver to check the consistency of the current
     assertions.  If the assertions are T-inconsistent, the conflict
     explanation should be generated and then Conflict rule should be
     applied. Otherwise, the theory solver should check for T-infered
     literals that should be propagated to the assertion trail by
     applying Propagate rule. Parameter layer determines the layer at
     which the above checks are performed. Greater layers should be
     deductively stronger (and invoke more expensive algorithms). If
     the trail is complete (i.e. all literals are assigned) the theory
     solver _must_ guarantee that the assertions are
     T-consistent. Otherwise, the theory solver may choose to only
     periodically check for propagations and conflicts at some
     layer. */
  virtual void check_and_propagate(unsigned layer) = 0;
 
  /* Asks the theory solver to explain the literal l. The theory
     solver should generate explanation and then apply Explain rule */
  virtual void explain_literal(const expression & l) = 0;
  
  /* Asks the theory solver whether the given expression is owned by it. */
  virtual bool is_owned_expression(const expression & e) = 0;
  
  /* Asks the theory solver to canonize the expression e.  */
  virtual expression canonize_expression(const expression & e) = 0;
  
  /* Asks the theory solver to provide the literal pair for l. */
  virtual void get_literal_pair(const expression & l, 
				expression & l_pos,
				expression & l_neg) = 0;

  /* Asks the theory solver to verify the assignment given by the
     trail. Should return true if the model is consistent, and false
     otherwise. */
  virtual bool verify_assignment(const trail & tr) = 0;
  
  /* Asks the theory solver to set the values assigned to the
     expressions. */
  virtual void get_model(const expression_vector & exps) = 0;

  /* Asks the theory solver to print its specific report about
     solving */
  virtual void print_report(std::ostream & ostr) = 0;
  
  /* Virtual destructor. */
  virtual ~theory_solver() {}
};

class clause_interface_theory_solver : public theory_solver {
public:
  using theory_solver::theory_solver;
  
  virtual void add_clause(clause * cl) = 0;
  virtual void remove_clauses(const std::vector<clause *> & learnt_clauses,
			      unsigned number_of_clauses) = 0;
  virtual ~clause_interface_theory_solver () {}
};

class congruence_closure_interface_theory_solver : public theory_solver {
public:
  using theory_solver::theory_solver;
  
  virtual bool are_congruent(const expression & e1, const expression & e2) const = 0;
  virtual const expression & get_representative(const expression & e) const = 0;
  virtual const expression_vector & get_congruence_class(const expression & e) const = 0;
  virtual const expression_vector & get_all_terms() const = 0;
  virtual void add_expression(const expression & e) = 0;
  virtual ~congruence_closure_interface_theory_solver () {}
};


#endif // _THEORY_SOLVER_H
