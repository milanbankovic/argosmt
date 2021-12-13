/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2014 Milan Bankovic (milan@matf.bg.ac.rs)

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

#ifndef _LOGIC_DECLARATION
#define _LOGIC_DECLARATION

#include "theory_declaration.hpp"
#include "expression.hpp"


/** Represents an abstract class that implements logic declarations. */
class logic_declaration {
public:
  /** Method should create and populate the signature of the corresponding
      logic declaration, and return a pointer to it. The signature should
      be the main signature in the hierarchy. The syntax_checking argument
      determines whether syntax checking is turned on or off. */
  virtual signature * create_signature(bool syntax_checking = true) = 0;
  
  /** The method should check whether the expression conforms to the
      language restrictions of the corresponding logic. By default,
      there are no specific language restrictions. */
  virtual bool check_expression(const expression & exp)
  {
    return true; 
  }
    
  /** Virtual destructor. */
  virtual ~logic_declaration() {}
};

/** AUFLIA logic. */
class auflia_logic_declaration : public logic_declaration {
private:
  ints_theory_declaration _ints;
  arrays_ex_theory_declaration _arrays;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** AUFLIRA logic. */
class auflira_logic_declaration : public logic_declaration {
private:
  reals_ints_theory_declaration _reals_ints;
  arrays_ex_theory_declaration _arrays;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** AUFNIRA logic. */
class aufnira_logic_declaration : public logic_declaration {
private:
  reals_ints_theory_declaration _reals_ints;
  arrays_ex_theory_declaration _arrays;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** LRA logic. */
class lra_logic_declaration : public logic_declaration {
private:
  reals_theory_declaration _reals;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_ABV logic. */
class qfabv_logic_declaration : public logic_declaration {
private:
  bit_vec_theory_declaration _bvs;
  arrays_ex_theory_declaration _arrays;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_AUFABV logic. */
class qfaufbv_logic_declaration : public logic_declaration {
private:
  bit_vec_theory_declaration _bvs;
  arrays_ex_theory_declaration _arrays;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_AUFLIA logic. */
class qfauflia_logic_declaration : public logic_declaration {
private:
  ints_theory_declaration _ints;
  arrays_ex_theory_declaration _arrays;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_AX logic. */
class qfax_logic_declaration : public logic_declaration {
private:
  arrays_ex_theory_declaration _arrays;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_BV logic. */
class qfbv_logic_declaration : public logic_declaration {
private:
  bit_vec_theory_declaration _bvs;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_IDL logic. */
class qfidl_logic_declaration  : public logic_declaration {
private:
  ints_theory_declaration _ints;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_LIA logic. */
class qflia_logic_declaration : public logic_declaration {
private:
  ints_theory_declaration _ints;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_LRA logic. */
class qflra_logic_declaration : public logic_declaration {
private:
  reals_theory_declaration _reals;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_NIA logic. */
class qfnia_logic_declaration : public logic_declaration {
private:
  ints_theory_declaration _ints;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_NRA logic. */
class qfnra_logic_declaration : public logic_declaration {
private:
  reals_theory_declaration _reals;

public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_RDL logic. */
class qfrdl_logic_declaration : public logic_declaration {
private:
  reals_theory_declaration _reals;

public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** UF logic. */
class uf_logic_declaration : public logic_declaration {
private:
  core_theory_declaration  _core;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};


/** QF_UF logic. */
class qfuf_logic_declaration : public logic_declaration {
private:
  core_theory_declaration  _core;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_UFBV logic. */
class qfufbv_logic_declaration : public logic_declaration {
private:
  bit_vec_theory_declaration _bvs;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_UFIDL logic. */
class qfufidl_logic_declaration : public logic_declaration {
private:
  ints_theory_declaration _ints;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_UFLIA logic. */
class qfuflia_logic_declaration : public logic_declaration {
private:
  ints_theory_declaration _ints;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};


/** QF_UFLRA logic. */
class qfuflra_logic_declaration : public logic_declaration {
private:
  reals_theory_declaration _reals;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_UFNRA logic. */
class qfufnra_logic_declaration : public logic_declaration {
private:
  reals_theory_declaration _reals;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** UFLRA logic. */
class uflra_logic_declaration : public logic_declaration {
private:
  reals_theory_declaration _reals;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** UFNIA logic. */
class ufnia_logic_declaration : public logic_declaration {
private:
  ints_theory_declaration _ints;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/* Non-standard logics */

/** QF_UFLIRA logic. */
class qfuflira_logic_declaration : public logic_declaration {
private:
  reals_ints_theory_declaration _reals_ints;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_LIRA logic. */
class qflira_logic_declaration : public logic_declaration {
private:
  reals_ints_theory_declaration _reals_ints;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_CSP logic. */
class qfcsp_logic_declaration : public logic_declaration {
private:
  constraints_theory_declaration _constraints;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

/** QF_LIACSP logic. */
class qfliacsp_logic_declaration : public logic_declaration {
private:
  constraints_theory_declaration _constraints;
public:
  virtual signature * create_signature(bool syntax_checking = true);
  virtual bool check_expression(const expression & exp);
};

#endif // _LOGIC_DECLARATION
