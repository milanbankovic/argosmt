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

You should have received a copy of the GNU General Public Lcense
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
***************************************************************************/

#ifndef _CONDITIONAL_TBB_WRAPPER_H
#define _CONDITIONAL_TBB_WRAPPER_H

#ifndef _USE_TBB
#define DECLARE_STATIC(type, name) static type name;
#else
#include <tbb/tbb.h>
#define DECLARE_STATIC(type, name) static tbb::combinable<type> name##_comb; type & name = name##_comb.local();
#endif

#ifndef _PARALLEL_PORTFOLIO
#define DEFINE_GLOBAL(type, name) type name;
#define DECLARE_EXTERN(type, name) extern type name;
#else
#define DEFINE_GLOBAL(type, name) tbb::combinable<type> name##_comb;
#define DECLARE_EXTERN(type, name) extern tbb::combinable<type> name##_comb; type & name = name##_comb.local();
#endif

#endif // _CONDITIONAL_TBB_WRAPPER_H
