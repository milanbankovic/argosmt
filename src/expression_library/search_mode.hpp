/****************************************************************************
argosmt (an open source SMT solver)
Copyright (C) 2010-2014 Milan Bankovic (milan@matf.bg.ac.rs)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
***************************************************************************/

#ifndef _SEARCH_MODE
#define _SEARCH_MODE

/** search_mode type defines possible values for search modes in the
    signature hierarchy. Search mode determines the set of signatures in
    the signature hieararchy that should be consulted when some sort or
    function symbol or special constant is checked during the well-sortedness
    checking. From user perspective, the only two important
    modes are:

    1) Contextual mode: default mode. It means that when some symbol is searched
    within the signature hieararchy, it is searched in the following signatures
    (in that order):

    -- the signature for which the check function (e.g. check_function_symbol()) 
    is invoked

    -- all its subsignatures (direct or indirect), and

    -- all signatures that are (direct or indirect) expansions of 
    that signature.

    2) Standalone: In that case, the symbol is searched only in the siganture
    for which the check function is invoked, and its (direct or indirect) 
    subsignatures (not in expansion signatures).

    Other two modes are (likely not used by the user):
  
    3) Strict mode: searches only in the signature for which the check function is
    invoked.

    4) Extensional mode: searches only in the signature for which the check 
    function is invoked and its (direct or indirect) expansions.
*/ 
enum search_mode { 
  /** Strict search mode */
  S_STRICT, 
  /** Standalone search mode */
  S_STANDALONE, 
  /** Extensional search mode */
  S_EXTENSIONAL, 
  /** Contextual search mode */
  S_CONTEXTUAL 
};

#endif // _SEARCH_MODE
