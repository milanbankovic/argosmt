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

#ifndef _CSP_COMMON_DATA_H
#define _CSP_COMMON_DATA_H

#include "wall_clock.hpp"

struct domain_common_data {
  unsigned _count_propagate;
  unsigned _count_conflict; 
  unsigned _count_inequalities;
  unsigned _count_equalities;
  domain_common_data()
    :_count_propagate(0),
     _count_conflict(0),
     _count_inequalities(0),
     _count_equalities(0)
  {}
};

struct alldiff_common_data {
  unsigned _count_propagate;
  unsigned _count_conflict;
  unsigned _count_trivial_explain;
  unsigned _count_hall_explain;
  unsigned _expl_depth_count;
  double _cumulative_expl_depth;
  unsigned _instance_count;
  alldiff_common_data()
    :_count_propagate(0),
     _count_conflict(0),
     _count_trivial_explain(0),
     _count_hall_explain(0),
     _expl_depth_count(0),
     _cumulative_expl_depth(0.0),
     _instance_count(0)
  {}
};

struct sum_common_data {
  unsigned _count_propagate;
  unsigned _count_conflict;
  unsigned _instance_count;
  unsigned _count_bound_change;
  unsigned _count_improved_bound_change;
  unsigned _cumulative_improvement;
  bool _upper_bounds_only;
  wall_clock _as_algorithm_time_spent;
  sum_common_data()
    :_count_propagate(0),
     _count_conflict(0),
     _instance_count(0),
     _count_bound_change(0),
     _count_improved_bound_change(0),
     _cumulative_improvement(0),
     _upper_bounds_only(false)
  {}
};

struct bipartite_graph_common_data {
  unsigned _count_trivial_prunings;
  unsigned _count_hall_prunings;  
  unsigned _count_tarjan;
  unsigned _count_tarjan_productive;
  bipartite_graph_common_data()
    :_count_trivial_prunings(0),
     _count_hall_prunings(0),
     _count_tarjan(0),
     _count_tarjan_productive(0)
  {}
};

#endif // _CSP_COMMON_DATA_H
