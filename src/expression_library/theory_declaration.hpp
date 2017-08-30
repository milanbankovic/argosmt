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

#ifndef _THEORY_DECLARATION
#define _THEORY_DECLARATION

#include "signature.hpp"


/** Represents an abstract class that implements theory declarations. */
class theory_declaration {

public:
  /** Function should create and populate the signature specified in the
      corresponding theory declaration and return a pointer to it. 
      The created signature should be a subsignature of logic_signature. */
  virtual signature * create_signature(signature * logic_signature) = 0;
  
  /** Virtual destructor. */
  virtual ~theory_declaration() {}
};
  
/** Core theory. */
class core_theory_declaration : public theory_declaration {

public:
  virtual signature * create_signature(signature * logic_signature);
};


/** Ints theory. */
class ints_theory_declaration : public theory_declaration {
public:
  virtual signature * create_signature(signature * logic_signature);

};

/** Reals theory. */
class reals_theory_declaration : public theory_declaration {
public:
  virtual signature * create_signature(signature * logic_signature);
  
};

/** Reals_Ints theory. */
class reals_ints_theory_declaration : public theory_declaration {
public:
  virtual signature * create_signature(signature * logic_signature);
  
};

/** ArraysEx theory. */
class arrays_ex_theory_declaration : public theory_declaration {
public:
  virtual signature * create_signature(signature * logic_signature);

};

/** Fixed_Size_BitVectors theory. */
class bit_vec_theory_declaration : public theory_declaration {
public:
  virtual signature * create_signature(signature * logic_signature);
  
};

/* Non-standard theories */

/** Constraints theory. */
class constraints_theory_declaration : public theory_declaration {
private:
  ints_theory_declaration _itd;
public:
  virtual signature * create_signature(signature * logic_signature);
};

#endif // _THEORY_DECLARATION
