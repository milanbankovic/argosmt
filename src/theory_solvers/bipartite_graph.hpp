/****************************************************************************
argosmt SMT solver  
Copyright (C) 2008, 2013-2014  Milan Bankovic

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

#ifndef _BIPARTITE_GRAPH_H
#define _BIPARTITE_GRAPH_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <cassert>
#include <sstream>

#include "csp_common_data.hpp"

template <typename T>
class matrix {
private:
  std::vector<T> _matrix;
  unsigned _m;
  unsigned _n;
public:
  matrix(unsigned m, unsigned n, const T & value = T());
  void assign(unsigned m, unsigned n, const T & value = T());
  unsigned m() const;
  unsigned n() const;
  T & operator () (unsigned i, unsigned j);
  const T & operator () (unsigned i, unsigned j) const;
  
  unsigned find_first_in_row(unsigned i, const T & value = T(), 
			     unsigned j = 0) const;
  unsigned find_first_in_column(unsigned j, const T & value = T(), 
				unsigned i = 0) const;
  
};

template <>
class matrix <bool> {
private:
  std::vector<bool> _matrix;
  unsigned _m;
  unsigned _n;
public:
  matrix(unsigned m, unsigned n, bool value = false);
  void assign(unsigned m, unsigned n, bool value = false);
  unsigned m() const;
  unsigned n() const;
  std::vector<bool>::reference operator () (unsigned i, unsigned j);
  std::vector<bool>::const_reference operator () (unsigned i, unsigned j) const;

  unsigned find_first_in_row(unsigned i, bool value = false, 
			     unsigned j = 0) const;
  unsigned find_first_in_column(unsigned j, bool value = false, 
				unsigned i = 0) const;
   
};

class bipartite_graph {
  
public:
  class vertex_iterator;
  class edge_iterator;

  // Side of the graph 
  enum side { LEFT = 0, RIGHT = 1, UNSPECIFIED = 2 };

  // Position of vertex or edge in the graph
  enum position { BEGIN = 0, END = 1};

  class vertex {
    friend class vertex_iterator;
  private:
    unsigned _num;
    side _side;
  public:
    vertex();
    vertex(unsigned num, side s);
    
    side vertex_side() const;
    unsigned vertex_number() const;
    
    bool operator == (const vertex & v) const;
    bool operator != (const vertex & v) const;
    
    void out(std::ostream & ostr) const;    
  };
  
  class vertex_iterator {
    friend class bipartite_graph;
  private:
    vertex _vertex;
    const bipartite_graph * _graph;
    
    vertex_iterator(side s, 
		    const bipartite_graph * g, 
		    position pos = BEGIN);
  public:
    vertex_iterator();
    const vertex & operator * () const;
    const vertex * operator -> () const;
    vertex_iterator operator ++ ();
    vertex_iterator operator ++ (int);
    
    bool operator == (const vertex_iterator & it) const;
    bool operator != (const vertex_iterator & it) const;
  };
    
  class vertex_marking {
  private:
    std::vector<bool> _marks;
    unsigned _num_of_left;
  public:
    vertex_marking(const bipartite_graph * g = 0, bool directed = false, bool value = false);
    
    void mark_vertex(const vertex & v, bool value = true);
    bool is_marked(const vertex & v) const;
    vertex find_unmarked(side s = UNSPECIFIED) const;
    vertex find_unmarked_left() const;
    vertex find_unmarked_right() const;
    void reset_marking(const bipartite_graph * g = 0, bool directed = false, bool value = false);
  };

  template <class T>
  class vertex_attribute {
  private:
    std::vector<T> _attr;
    unsigned _num_of_left;
  public:
    vertex_attribute(const bipartite_graph *g = 0, bool directed = false, const T & value = T());
    
    void set_attribute(const vertex & v, const T & value = T());
    const T & get_attribute(const vertex & v) const;
    void reset_attributes(const bipartite_graph * g = 0, bool directed = false, const T & value = T());
  };

 
  class edge {
    friend  class edge_iterator;
    friend class bipartite_graph;
  private:
    unsigned _left;
    unsigned _right;
  public:
    edge();
    edge(const vertex & u, const vertex & v);
    edge(unsigned left, unsigned right);
    
    vertex left_vertex() const;
    vertex right_vertex() const;

    vertex begin_vertex(const bipartite_graph * g) const;
    vertex end_vertex(const bipartite_graph * g) const;
    
    bool operator == (const edge & e) const;
    bool operator != (const edge & e) const;
    
    void out(std::ostream & ostr) const;
  };

  class edge_iterator_impl {
  public:
    virtual void first_edge(const bipartite_graph * g, unsigned v_num, edge & e) const = 0;
    virtual void last_edge(const bipartite_graph * g, unsigned v_num, edge & e) const = 0;
    virtual void next_edge(const bipartite_graph * g, edge & e) const = 0;
    
    virtual ~edge_iterator_impl() {}

  };
    
  class undirected_left_iterator_impl : public edge_iterator_impl {
  public:
    virtual void first_edge(const bipartite_graph * g, unsigned v_num, edge & e) const;
    virtual void last_edge(const bipartite_graph * g, unsigned v_num, edge & e) const;
    virtual void next_edge(const bipartite_graph * g, edge & e) const;
  };

  class undirected_right_iterator_impl : public edge_iterator_impl {
  public:
    virtual void first_edge(const bipartite_graph * g, unsigned v_num, edge & e) const;
    virtual void last_edge(const bipartite_graph * g, unsigned v_num, edge & e) const;
    virtual void next_edge(const bipartite_graph * g, edge & e) const;
  };

  class directed_left_iterator_impl : public edge_iterator_impl {
  public:
    virtual void first_edge(const bipartite_graph * g, unsigned v_num, edge & e) const;
    virtual void last_edge(const bipartite_graph * g, unsigned v_num, edge & e) const;
    virtual void next_edge(const bipartite_graph * g, edge & e) const;
  };

  class directed_right_iterator_impl : public edge_iterator_impl {
  public:
    virtual void first_edge(const bipartite_graph * g, unsigned v_num, edge & e) const;
    virtual void last_edge(const bipartite_graph * g, unsigned v_num, edge & e) const;
    virtual void next_edge(const bipartite_graph * g, edge & e) const;
  };

  class directed_source_iterator_impl : public edge_iterator_impl {
  public:
    virtual void first_edge(const bipartite_graph * g, unsigned v_num, edge & e) const;
    virtual void last_edge(const bipartite_graph * g, unsigned v_num, edge & e) const;
    virtual void next_edge(const bipartite_graph * g, edge & e) const;
  };


  class directed_sink_iterator_impl : public edge_iterator_impl {
  public:
    virtual void first_edge(const bipartite_graph * g, unsigned v_num, edge & e) const;
    virtual void last_edge(const bipartite_graph * g, unsigned v_num, edge & e) const;
    virtual void next_edge(const bipartite_graph * g, edge & e) const;
  };

  class edge_iterator {
    friend class bipartite_graph;
  private:
    edge _edge;
    const side _side;
    bool _directed;
    const bipartite_graph * _graph;
    const edge_iterator_impl * _impl;
    edge_iterator(const vertex & v, 
		  const bipartite_graph * g, 
		  position pos = BEGIN,
		  bool directed = false);
    
  public:
    const edge & operator * () const;
    const edge * operator -> () const;
    const edge_iterator & operator ++ ();
    edge_iterator operator ++ (int);
 
    bool operator == (const edge_iterator & it) const;
    bool operator != (const edge_iterator & it) const;
  };
  
  class edge_marking {
  private:
    matrix<bool> _marks;
  public:
    edge_marking(const bipartite_graph * g = 0, bool directed = false, bool value = false);
    
    void mark_edge(const edge & e, bool value = true);
    void mark_all_edges(const vertex & v, bool value = true);
    bool is_marked(const edge & e) const;
    void reset_marking(const bipartite_graph * g = 0, bool directed = false, bool value = false);
    void marked_edges_vector(std::vector<edge> & edges) const;
  };

  template <typename T>
  class edge_attribute {
  private:
    matrix<T> _attributes;
  public:
    edge_attribute(const bipartite_graph * g = 0, bool directed = false, const T & value = T());
    
    void set_attribute(const edge & e, const T & value = T());
    const T & get_attribute(const edge & e) const;
    void set_all_attributes(const vertex & v, const T & value = T());
    void reset_attributes(const bipartite_graph * g = 0, bool directed = false, const T & value = T());  
  };

private:
  struct matching_entry {
    vertex _matched_with;
    bool _vital_match;
    matching_entry(const vertex & matched_with = vertex(), 
		   bool vital_match = false);
  };
  
public:
  vertex_iterator begin(side s) const;
  vertex_iterator end(side s) const;
  vertex_iterator begin_left() const;
  vertex_iterator begin_right() const;
  vertex_iterator end_left() const;
  vertex_iterator end_right() const;
  edge_iterator begin_edge(const vertex & v, bool directed = false) const;
  edge_iterator end_edge(const vertex & v, bool directed = false) const;
  vertex source_vertex() const;
  vertex sink_vertex() const;
  
private:
  matrix<bool> _graph;  
  vertex_attribute <matching_entry> _matching;
  edge_marking _important_edges;
  bool _important_removed;
  bipartite_graph_common_data * _common_data;

  static undirected_left_iterator_impl _ul_impl;
  static undirected_right_iterator_impl _ur_impl;
  static directed_left_iterator_impl _dl_impl;
  static directed_right_iterator_impl _dr_impl;
  static directed_source_iterator_impl _ds_impl;
  static directed_sink_iterator_impl _dt_impl;


public:
  bipartite_graph(unsigned num_left, unsigned num_right, bipartite_graph_common_data * cd);
  void assign(unsigned num_left, unsigned num_right, bipartite_graph_common_data * cd);
  
  bool is_valid() const;
  
  bool vertex_exists(const vertex & v, bool directed = false) const;
  unsigned number_of_vertices(side s) const;
  unsigned number_of_left_vertices() const;
  unsigned number_of_right_vertices() const;
  
  void add_edge(const edge & e);
  void remove_edge(const edge & e);
  bool edge_exists(const edge & e, bool directed = false) const;
  unsigned number_of_edges(const vertex & v, bool directed = false) const;

  bool is_in_matching(const edge & e) const;
  bool are_matched(const vertex & u, const vertex & v) const;
  vertex matched_with(const vertex & v) const;
  bool is_matched(const vertex & v) const;
  void add_to_matching(const edge & e);
  void unmatch_vertex(const vertex & v);
  void remove_from_matching(const edge & e);
  void reset_matching();
  unsigned matching_size() const;

  vertex find_unmatched(side s = UNSPECIFIED) const;
  vertex find_unmatched_left() const;
  vertex find_unmatched_right() const;

  bool is_covered(side s) const;
  bool is_covered_left() const;
  bool is_covered_right() const;
  
  bool is_vital(const edge & e) const;
  bool is_matched_vital(const vertex & v) const;
  void add_to_vital(const edge & e);
  void remove_from_vital(const edge & e);
  void reset_vital();

  bool is_important(const edge & e) const;
  void add_to_important(const edge & e);
  void remove_from_important(const edge & e);
  void reset_important(bool value = false);
  
private:
  void tarjan_cycles(const vertex & v, 
		     unsigned & count, 
		     vertex_attribute<unsigned> & index, 
		     vertex_attribute<unsigned> & lowlink, 
		     std::stack<vertex> & vertex_stack, 
		     vertex_marking & stack_marking, 
		     vertex_marking & marking,
		     edge_marking & used);

  void mark_even_alternating_cycles(edge_marking & used);

  vertex find_unmatchable_left_vertex(const edge_marking & obstacles);

  void find_reachable_obstacles(const vertex & v, 
				const edge_marking & obstacles, 
				edge_marking & reached_obstacles,
				vertex_marking & marking) const;
  
  void tarjan_unblocked_vertices(const vertex & v,
				 const vertex & vp,
				 unsigned & count, 
				 vertex_attribute<unsigned> & index, 
				 vertex_attribute<unsigned> & lowlink, 
				 std::stack<vertex> & vertex_stack, 
				 vertex_marking & stack_marking, 
				 vertex_marking & marking,
				 const edge_marking & obstacles,
				 vertex_marking & unblocked) const;
  
  void tarjan_hall_obstacles(const vertex & v,
			     unsigned & count, 
			     vertex_attribute<unsigned> & index, 
			     vertex_attribute<unsigned> & lowlink, 
			     std::stack<vertex> & vertex_stack, 
			     vertex_marking & stack_marking, 
			     vertex_marking & marking,
			     const edge_marking & obstacles,
			     vertex_attribute<unsigned> & scc_root) const;
  
public:
  void greedy_matching();
  void max_cardinality_matching();
  void make_arc_consistent(std::vector<edge> & removed_edges, 
			   std::vector<edge> & new_vital_edges);
  
  void find_minimal_obstacle_set(const vertex & v_start, 
				 const vertex & v_end,
				 const edge_marking & obstacles,
				 edge_marking & mos) const;


  void reduce_prop_expl_to_mos(const edge & e,
			       const edge_marking & obstacles, 
			       edge_marking & reason) const;  
  void reduce_cflt_expl_to_mos(const edge_marking & obstacles,
			       edge_marking & reason);

  void reduce_prop_expl_to_kat(const edge & e,
			       const edge_marking & obstacles,
			       edge_marking & reason) const;
  
  void reduce_cflt_expl_to_kat(const edge_marking & obstacles,
			       edge_marking & reason);

  bool do_propagation(const std::vector<edge> & remove,
		      const std::vector<edge> & vital,
		      std::vector<edge> & removed_edges, 
		      std::vector<edge> & new_vital_edges,
		      bool strong = true);
  
  void out(std::ostream & ostr) const;

};


std::ostream & operator << (std::ostream & ostr, 
			    const bipartite_graph & g);
std::ostream & operator << (std::ostream & ostr, 
			    const bipartite_graph::vertex & v);
std::ostream & operator << (std::ostream & ostr, 
			    const bipartite_graph::edge & e);


template <typename T>
inline
matrix<T>::matrix(unsigned m, unsigned n, const T & value)
  :_matrix(m * n, value),
   _m(m),
   _n(n)
{}

template <typename T>
inline 
void matrix<T>::assign(unsigned m, unsigned n, const T & value)
{
  _matrix.assign(m * n, value);
  _m = m;
  _n = n;
}

template <typename T>
inline
unsigned matrix<T>::m() const
{
  return _m;
}

template <typename T>
inline
unsigned matrix<T>::n() const
{
  return _n;
}

template <typename T>
inline
T & matrix<T>::operator () (unsigned i, unsigned j)
{
  static T dummy;
  return i < _m && j < _n ? _matrix[_n * i + j] : (dummy = T());
}

template <typename T>
inline
const T & matrix<T>::operator () (unsigned i, unsigned j) const
{
  static T dummy;
  return i < _m && j < _n ? _matrix[_n * i + j] : (dummy = T());
}


template <typename T>
inline
unsigned 
matrix<T>::find_first_in_row(unsigned i, const T & value, 
			     unsigned j) const
{
  if(j >= _n)
    return _n;

  unsigned i_n = i * _n;
  unsigned k = i_n  + j;
  unsigned d = i_n + _n;

  for(; k < d; k++)
    if(_matrix[k] == value)
      break;
  
  return k - i_n;
}

template <typename T>
inline
unsigned 
matrix<T>::find_first_in_column(unsigned j, const T & value,
				unsigned i) const
{
  if(i >= _m)
    return _m;

  unsigned k = i * _n + j;
  unsigned m_n = _m * _n;

  for( ; k < m_n ; k += _n)
    if(_matrix[k] == value)
      break;
  
  return k / _n;
}

inline
matrix<bool>::matrix(unsigned m, unsigned n, bool value)
:_matrix(m * n, value),
 _m(m),
 _n(n)
{}

inline 
void matrix<bool>::assign(unsigned m, unsigned n, bool value)
{
  _matrix.assign(m * n, value);
  _m = m;
  _n = n;
}

inline
unsigned matrix<bool>::m() const
{
  return _m;
}

inline
unsigned matrix<bool>::n() const
{
  return _n;
}

inline
std::vector<bool>::reference matrix<bool>::operator () (unsigned i, unsigned j)
{
  static std::vector<bool> dummy(1);
  return i < _m && j < _n ? _matrix[_n * i + j] : (dummy[0] = false);
}

inline
std::vector<bool>::const_reference matrix<bool>::operator () (unsigned i, unsigned j) const
{
  static std::vector<bool> dummy(1);
  return i < _m && j < _n ? _matrix[_n * i + j] : (dummy[0] = false);
}

inline
unsigned 
matrix<bool>::find_first_in_row(unsigned i, bool value, 
				unsigned j) const
{
  if(j >= _n)
    return _n;

  unsigned i_n = i * _n;
  unsigned k = i_n  + j;
  unsigned d = i_n + _n;

  for(; k < d; k++)
    if(_matrix[k] == value)
      break;
  
  return k - i_n;
}

inline
unsigned 
matrix<bool>::find_first_in_column(unsigned j, bool value,
				   unsigned i) const
{
  if(i >= _m)
    return _m;

  unsigned k = i * _n + j;
  unsigned m_n = _m * _n;

  for( ; k < m_n ; k += _n)
    if(_matrix[k] == value)
      break;
  
  return k / _n;
}

inline 
bipartite_graph::vertex::vertex()
  :_num((unsigned)(-1)),
   _side(UNSPECIFIED) 
{}

inline 
bipartite_graph::vertex::vertex(unsigned num, side s)
  :_num(num),
   _side(s)
{}

inline 
bipartite_graph::side 
bipartite_graph::vertex::vertex_side() const
{
  return _side;
}

inline 
unsigned bipartite_graph::vertex::vertex_number() const
{
  return _num;
} 

inline 
bool bipartite_graph::vertex::operator == (const vertex & v) const
{
  return _num == v._num && _side == v._side;
}

inline     
bool bipartite_graph::vertex::operator != (const vertex & v) const
{
  return !(*this == v);
}
    
inline 
void 
bipartite_graph::vertex::out(std::ostream & ostr) const
{
  ostr << _num << (_side == LEFT ? "(left)" : "(right)");
}

inline 
bipartite_graph::vertex_iterator::vertex_iterator(side s, 
						  const bipartite_graph * g, 
						  position pos)
  :_vertex(pos == BEGIN ? 0 : g->number_of_vertices(s), s),
   _graph(g) 
{}

inline 
bipartite_graph::vertex_iterator::vertex_iterator() 
  :_vertex(),
   _graph(0)
{}

inline 
const bipartite_graph::vertex & 
bipartite_graph::vertex_iterator::operator * () const
{
  assert(_graph->vertex_exists(_vertex));
  return _vertex;
}
    
inline 
const bipartite_graph::vertex * 
bipartite_graph::vertex_iterator::operator -> () const 
{
  assert(_graph->vertex_exists(_vertex));
  return &_vertex;
}

inline 
bipartite_graph::vertex_iterator 
bipartite_graph::vertex_iterator::operator ++ ()
{
  _vertex._num++;
  return *this;
}

inline 
bipartite_graph::vertex_iterator 
bipartite_graph::vertex_iterator::operator ++ (int)
{
  vertex_iterator old_iter(*this);
  _vertex._num++;
  return old_iter;
}

inline 
bool 
bipartite_graph::vertex_iterator::operator == (const vertex_iterator & it) const
{
  return _vertex == it._vertex;
}

inline 
bool 
bipartite_graph::vertex_iterator::operator != (const vertex_iterator & it) const
{
  return !(*this == it);
}

inline 
bipartite_graph::vertex_marking::vertex_marking(const bipartite_graph * g, bool directed, bool value)
  :_marks(g != 0 ? g->number_of_left_vertices() + g->number_of_right_vertices() + 2*((int)directed) : 0, value),
   _num_of_left(g != 0 ? g->number_of_left_vertices() + (int)directed : 0)
{}

inline
void 
bipartite_graph::vertex_marking::mark_vertex(const vertex & v, bool value)
{
  _marks[(unsigned)v.vertex_side() * _num_of_left + v.vertex_number()] = value;
}

inline
bool 
bipartite_graph::vertex_marking::is_marked(const vertex & v) const
{
  return _marks[(unsigned)v.vertex_side() * _num_of_left + v.vertex_number()];
}

inline
bipartite_graph::vertex 
bipartite_graph::vertex_marking::find_unmarked_left() const
{
  return find_unmarked(LEFT);
}

inline
bipartite_graph::vertex 
bipartite_graph::vertex_marking::find_unmarked_right() const
{
  return find_unmarked(RIGHT);
}

inline
void 
bipartite_graph::vertex_marking::reset_marking(const bipartite_graph * g, bool directed, bool value)
{
  _marks.assign(g != 0 ? g->number_of_left_vertices() + g->number_of_right_vertices() + 2*((int)directed) : 0, value);
  _num_of_left = g != 0 ? g->number_of_left_vertices() + (int)directed : 0;
}


template <class T>
inline
bipartite_graph::vertex_attribute<T>::vertex_attribute(const bipartite_graph *g, 
						       bool directed, 
						       const T & value)
  :_attr(g != 0 ? g->number_of_left_vertices() + g->number_of_right_vertices() + 2*((int)directed) : 0, value),
   _num_of_left(g != 0 ? g->number_of_left_vertices() + (int)directed : 0)
{}

template <class T>
inline
void 
bipartite_graph::vertex_attribute<T>::set_attribute(const vertex & v, 
						    const T & value)
{
  _attr[(unsigned) v.vertex_side() * _num_of_left + v.vertex_number()] = value;
}

template <class T>
inline
const T & 
bipartite_graph::vertex_attribute<T>::get_attribute(const vertex & v) const
{
  return _attr[(unsigned) v.vertex_side() * _num_of_left + v.vertex_number()];
}

template <class T>
inline
void 
bipartite_graph::vertex_attribute<T>::reset_attributes(const bipartite_graph * g,
						       bool directed, 
						       const T & value)
{
  _attr.assign(g != 0 ? g->number_of_left_vertices() + g->number_of_right_vertices() + 2*((int)directed) : 0, value);
  _num_of_left = g != 0 ? g->number_of_left_vertices() + (int)directed : 0;
}

inline
bipartite_graph::edge::edge()
  :_left((unsigned)(-1)),
   _right((unsigned)(-1)){}

inline
bipartite_graph::edge::edge(const vertex & u, const vertex & v)
{
  assert(u.vertex_side() != v.vertex_side());
  
  if(u.vertex_side() == LEFT)
    {
      _left = u.vertex_number();
      _right = v.vertex_number();
    }
  else
    {
      _left = v.vertex_number();
      _right = u.vertex_number();
    }
}

inline
bipartite_graph::edge::edge(unsigned left, unsigned right)
  :_left(left),
   _right(right)
{}

inline
bipartite_graph::vertex 
bipartite_graph::edge::left_vertex() const
{
  return vertex(_left, LEFT);
}

inline
bipartite_graph::vertex 
bipartite_graph::edge::right_vertex() const
{
  return vertex(_right, RIGHT);
}

inline
bipartite_graph::vertex 
bipartite_graph::edge::begin_vertex(const bipartite_graph * g) const
{
  if(_right < g->number_of_right_vertices() && _left < g->number_of_left_vertices())
    return g->is_in_matching(*this) ? vertex(_right, RIGHT) : vertex(_left, LEFT);
  else if(_right == g->number_of_right_vertices()) // source vertex
    return g->is_matched(vertex(_left, LEFT)) ? vertex(_left, LEFT) : vertex(_right, RIGHT);
  else // if(_left == g->number_of_left_vertices()) // sink vertex
    return g->is_matched(vertex(_right, RIGHT)) ? vertex(_left, LEFT) : vertex(_right, RIGHT);
}

inline
bipartite_graph::vertex 
bipartite_graph::edge::end_vertex(const bipartite_graph * g) const
{
  if(_right < g->number_of_right_vertices() && _left < g->number_of_left_vertices())
    return !g->is_in_matching(*this) ? vertex(_right, RIGHT) : vertex(_left, LEFT);
  else if(_right == g->number_of_right_vertices()) // source vertex
    return !g->is_matched(vertex(_left, LEFT)) ? vertex(_left, LEFT) : vertex(_right, RIGHT);
  else // if(_left == g->number_of_left_vertices()) // sink vertex
    return !g->is_matched(vertex(_right, RIGHT)) ? vertex(_left, LEFT) : vertex(_right, RIGHT);
}


inline
bool 
bipartite_graph::edge::operator == (const edge & e) const
{
  return _left == e._left && _right == e._right;
}

inline
bool 
bipartite_graph::edge::operator  != (const edge & e) const
{
  return !(*this == e);
}

inline
void 
bipartite_graph::edge::out(std::ostream & ostr) const
{
  ostr << "(" << _left << "," << _right << ")";
}


inline
const bipartite_graph::edge & 
bipartite_graph::edge_iterator::operator * () const
{
  assert(_graph->edge_exists(_edge, _directed));
  return _edge;
}

inline
const bipartite_graph::edge * 
bipartite_graph::edge_iterator::operator -> () const
{
  assert(_graph->edge_exists(_edge, _directed));
  return &_edge;
}


inline
bool 
bipartite_graph::edge_iterator::operator == (const edge_iterator & it) const
{
  return _edge == it._edge;
}

inline
bool 
bipartite_graph::edge_iterator::operator != (const edge_iterator & it) const
{
  return !(*this == it);
}

inline
const bipartite_graph::edge_iterator &
bipartite_graph::edge_iterator::operator ++ ()
{
  _impl->next_edge(_graph, _edge);
  return *this;
}

inline
bipartite_graph::edge_iterator 
bipartite_graph::edge_iterator::operator ++ (int)
{
  edge_iterator old_iter(*this);
  _impl->next_edge(_graph, _edge);
  return old_iter;
}


inline
bipartite_graph::edge_marking::edge_marking(const bipartite_graph * g, bool directed, bool value)
  :_marks(g != 0 ? g->number_of_left_vertices() + (int)directed : 0, g != 0 ? g->number_of_right_vertices() + (int)directed : 0, value) 
{}

inline
void 
bipartite_graph::edge_marking::mark_edge(const edge & e, bool value)
{
  _marks
    (e.left_vertex().vertex_number(),
     e.right_vertex().vertex_number()) = value;
}

inline
bool 
bipartite_graph::edge_marking::is_marked(const edge & e) const
{
  return 
    _marks(e.left_vertex().vertex_number(), e.right_vertex().vertex_number());
}

inline
void 
bipartite_graph::edge_marking::reset_marking(const bipartite_graph * g, bool directed, bool value)
{
  _marks.assign(g != 0 ? g->number_of_left_vertices() + (int)directed : 0, g != 0 ? g->number_of_right_vertices() + (int)directed : 0, value);
}

template <typename T>
inline
bipartite_graph::edge_attribute<T>::edge_attribute(const bipartite_graph * g, 
						   bool directed, 
						   const T & value)
  :_attributes(g != 0 ? g->number_of_left_vertices() + (int)directed : 0, g != 0 ? g->number_of_right_vertices() + (int)directed : 0, value) 
{}

template <typename T>
inline
void 
bipartite_graph::edge_attribute<T>::set_attribute(const edge & e, const T & value)
{
  _attributes
    (e.left_vertex().vertex_number(),
     e.right_vertex().vertex_number()) = value;
}

template <typename T>
inline
const T & 
bipartite_graph::edge_attribute<T>::get_attribute(const edge & e) const
{
  return _attributes
    (e.left_vertex().vertex_number(),
     e.right_vertex().vertex_number());
}

template <typename T>
inline
void 
bipartite_graph::edge_attribute<T>::set_all_attributes(const vertex & v, const T & value)
{
  if(v.vertex_side() == LEFT)
    {
      unsigned i = v.vertex_number();
      for(unsigned j = 0; j < _attributes.n(); j++)
	_attributes(i, j) = value;
    }
  else if(v.vertex_side() == RIGHT)
    {
      unsigned j = v.vertex_number();
      for(unsigned i = 0; i < _attributes.m(); i++)
	_attributes(i, j) = value;
    }
}

template <typename T>
inline
void 
bipartite_graph::edge_attribute<T>::reset_attributes(const bipartite_graph * g,
						     bool directed, 
						     const T & value)
{
  _attributes.assign(g != 0 ? g->number_of_left_vertices() + (int)directed : 0, 
		     g != 0 ? g->number_of_right_vertices() + (int)directed : 0, 
		     value);
}


inline
bipartite_graph::matching_entry::matching_entry(const vertex & matched_with, 
						bool vital_match)
  :_matched_with(matched_with),
   _vital_match(vital_match)
{}


inline
bipartite_graph::vertex_iterator 
bipartite_graph::begin(side s) const
{
  return vertex_iterator(s, this, BEGIN);
}

inline
bipartite_graph::vertex_iterator 
bipartite_graph::end(side s) const
{
  return vertex_iterator(s, this, END);
}

inline
bipartite_graph::vertex_iterator 
bipartite_graph::begin_left() const
{
  return begin(LEFT);
}

inline
bipartite_graph::vertex_iterator 
bipartite_graph::begin_right() const
{
  return begin(RIGHT);
}

inline
bipartite_graph::vertex_iterator 
bipartite_graph::end_left() const
{
  return end(LEFT);
}

inline
bipartite_graph::vertex_iterator 
bipartite_graph::end_right() const
{
  return end(RIGHT);
}

inline
bipartite_graph::edge_iterator 
bipartite_graph::begin_edge(const vertex & v, bool directed) const
{
  assert(vertex_exists(v, directed));
  
  return edge_iterator(v, this, BEGIN, directed);
}

inline
bipartite_graph::edge_iterator 
bipartite_graph::end_edge(const vertex & v, bool directed) const
{
  assert(vertex_exists(v, directed));
  
  return edge_iterator(v, this, END, directed);
}

inline
bipartite_graph::vertex bipartite_graph::source_vertex() const
{
  return vertex(number_of_right_vertices(), RIGHT);
}

inline
bipartite_graph::vertex bipartite_graph::sink_vertex() const
{
  return vertex(number_of_left_vertices(), LEFT);
}

inline
bool 
bipartite_graph::vertex_exists(const vertex & v, bool directed) const
{
  unsigned k = v.vertex_number();
  side side = v.vertex_side();
  
  switch(side)
    {
    case LEFT:
      return k < number_of_left_vertices() + (int)directed;
      break;
    case RIGHT:
      return k < number_of_right_vertices() + (int)directed; 
      break;
    default:
      return false;
      break;
    }
  return false;
}

inline
bipartite_graph::bipartite_graph(unsigned num_left, unsigned num_right, bipartite_graph_common_data * cd)
  :_graph(num_left, num_right, false),
   _matching(this),
   _important_edges(this, true, true),
   _important_removed(false),
   _common_data(cd)
{}

inline
void bipartite_graph::assign(unsigned num_left, unsigned num_right, bipartite_graph_common_data * cd)
{
  _graph.assign(num_left, num_right, false);
  _matching.reset_attributes(this);
  _important_edges.reset_marking(this, true, true);
  _important_removed = false;
  _common_data = cd;
}


inline
bool
bipartite_graph::is_valid() const
{
  return number_of_left_vertices() > 0 && number_of_right_vertices() > 0;
}

inline
unsigned 
bipartite_graph::number_of_vertices(side s) const
{
  switch(s)
    {
    case LEFT:
      return _graph.m();
      break;
    case RIGHT:
      return _graph.n();
      break;
    default:
      break;
    }
  return 0;
}

inline
unsigned 
bipartite_graph::number_of_left_vertices() const
{
  return _graph.m();
}

inline
unsigned 
bipartite_graph::number_of_right_vertices() const
{
  return _graph.n();
}

inline
void 
bipartite_graph::add_edge(const edge & e)
{
  assert(vertex_exists(e.left_vertex()));
  assert(vertex_exists(e.right_vertex()));
  _graph
    (e.left_vertex().vertex_number(),
     e.right_vertex().vertex_number()) = true;
}

inline
void 
bipartite_graph::remove_edge(const edge & e)
{
  assert(vertex_exists(e.left_vertex()));
  assert(vertex_exists(e.right_vertex()));
  
  remove_from_matching(e);
  
  _graph
    (e.left_vertex().vertex_number(),
     e.right_vertex().vertex_number()) = false;  
}

inline
bool 
bipartite_graph::edge_exists(const edge & e, bool directed) const
{
  assert(vertex_exists(e.left_vertex(), directed));
  assert(vertex_exists(e.right_vertex(), directed));
  if(!directed)
    return _graph(e.left_vertex().vertex_number(), e.right_vertex().vertex_number());
  else if(e.left_vertex().vertex_number() < number_of_left_vertices() &&
	  e.right_vertex().vertex_number() < number_of_right_vertices())
    return _graph(e.left_vertex().vertex_number(), e.right_vertex().vertex_number());
  else
    return !(e.left_vertex().vertex_number() == number_of_left_vertices() &&
		 e.right_vertex().vertex_number() == number_of_right_vertices());
}

inline
void bipartite_graph::undirected_left_iterator_impl::first_edge(const bipartite_graph * g, unsigned v_num, edge & e) const
{
  e._left = v_num;
  e._right = g->_graph.find_first_in_row(e._left, true);
}

inline
void bipartite_graph::undirected_left_iterator_impl::last_edge(const bipartite_graph * g, unsigned v_num, edge & e) const
{
  e._left = v_num;
  e._right = g->number_of_right_vertices();
}

inline
void bipartite_graph::undirected_left_iterator_impl::next_edge(const bipartite_graph * g, edge & e) const
{
  e._right = g->_graph.find_first_in_row(e._left, true, e._right + 1);
}

inline
void bipartite_graph::undirected_right_iterator_impl::first_edge(const bipartite_graph * g, unsigned v_num, edge & e) const
{
  e._right = v_num;
  e._left = g->_graph.find_first_in_column(e._right, true);
}

inline
void bipartite_graph::undirected_right_iterator_impl::last_edge(const bipartite_graph * g, unsigned v_num, edge & e) const
{
  e._right = v_num;
  e._left = g->number_of_left_vertices();
}

inline
void bipartite_graph::undirected_right_iterator_impl::next_edge(const bipartite_graph * g, edge & e) const
{
  e._left = g->_graph.find_first_in_column(e._right, true, e._left + 1);
}

inline
void bipartite_graph::directed_left_iterator_impl::first_edge(const bipartite_graph * g, unsigned v_num, edge & e) const
{
  e._left = v_num;
  e._right = g->_graph.find_first_in_row(e._left, true);
  if(e._right < g->number_of_right_vertices() && g->is_in_matching(e))
    e._right = g->_graph.find_first_in_row(e._left, true, e._right + 1);
  if(e._right == g->number_of_right_vertices() && !g->is_matched(vertex(e._left, LEFT)))
    e._right++;
}

inline
void bipartite_graph::directed_left_iterator_impl::last_edge(const bipartite_graph * g, unsigned v_num, edge & e) const
{
  e._left = v_num;
  e._right = g->number_of_right_vertices() + 1;
}

inline
void bipartite_graph::directed_left_iterator_impl::next_edge(const bipartite_graph * g, edge & e) const
{
  if(e._right == g->number_of_right_vertices())
    {
      e._right++;
    }
  else if(e._right < g->number_of_right_vertices())
    {
      e._right = g->_graph.find_first_in_row(e._left, true, e._right + 1);
      if(e._right < g->number_of_right_vertices() && g->is_in_matching(e))
	e._right = g->_graph.find_first_in_row(e._left, true, e._right + 1);
      if(e._right == g->number_of_right_vertices() && !g->is_matched(vertex(e._left, LEFT)))
	e._right++;
    }
}

inline
void bipartite_graph::directed_right_iterator_impl::first_edge(const bipartite_graph * g, unsigned v_num, edge & e) const
{
  e._right = v_num;
  vertex u = g->matched_with(vertex(v_num, RIGHT));
  if(g->vertex_exists(u))
    e._left = u.vertex_number();
  else
    e._left = g->number_of_left_vertices();
}

inline
void bipartite_graph::directed_right_iterator_impl::last_edge(const bipartite_graph * g, unsigned v_num, edge & e) const
{
  e._right = v_num;
  e._left = g->number_of_left_vertices() + 1;
}

inline
void bipartite_graph::directed_right_iterator_impl::next_edge(const bipartite_graph * g, edge & e) const
{
  e._left = g->number_of_left_vertices() + 1;
}

inline
void bipartite_graph::directed_source_iterator_impl::first_edge(const bipartite_graph * g, unsigned v_num, edge & e) const
{
  e._right = v_num;
  e._left = 0;
  while(e._left < g->number_of_left_vertices() && g->is_matched(vertex(e._left, LEFT)))
    e._left++;
  if(e._left == g->number_of_left_vertices())
    e._left++;	      
}

inline
void bipartite_graph::directed_source_iterator_impl::last_edge(const bipartite_graph * g, unsigned v_num, edge & e) const
{
  e._right = v_num;
  e._left = g->number_of_left_vertices() + 1;
}

inline
void bipartite_graph::directed_source_iterator_impl::next_edge(const bipartite_graph * g, edge & e) const
{
  do {
    e._left++;
  } while(e._left < g->number_of_left_vertices() && g->is_matched(vertex(e._left, LEFT)));
  if(e._left >= g->number_of_left_vertices())
    e._left = g->number_of_left_vertices() + 1;
}

inline
void bipartite_graph::directed_sink_iterator_impl::first_edge(const bipartite_graph * g, unsigned v_num, edge & e) const
{
  e._left = v_num;
  e._right = 0;
  while(e._right < g->number_of_right_vertices() && !g->is_matched(vertex(e._right, RIGHT)))
    e._right++;	      
  if(e._right == g->number_of_right_vertices())
    e._right++;	      
}

inline
void bipartite_graph::directed_sink_iterator_impl::last_edge(const bipartite_graph * g, unsigned v_num, edge & e) const
{
  e._left = v_num;
  e._right = g->number_of_right_vertices() + 1;
}

inline
void bipartite_graph::directed_sink_iterator_impl::next_edge(const bipartite_graph * g, edge & e) const
{
  do {
    e._right++;
  } while(e._right < g->number_of_right_vertices() && !g->is_matched(vertex(e._right, RIGHT)));
  if(e._right >= g->number_of_right_vertices())
    e._right = g->number_of_right_vertices() + 1;
}

inline
bool 
bipartite_graph::is_in_matching(const edge & e) const
{
  return are_matched(e.left_vertex(), e.right_vertex());
}

inline
bool 
bipartite_graph::are_matched(const vertex & u, const vertex & v) const
{
  assert(vertex_exists(u));
  assert(vertex_exists(v));
  return _matching.get_attribute(u)._matched_with == v;
}

inline
bipartite_graph::vertex 
bipartite_graph::matched_with(const vertex & v) const
{
  assert(vertex_exists(v));
  return _matching.get_attribute(v)._matched_with;
}

inline
bool 
bipartite_graph::is_matched(const vertex & v) const
{
  return vertex_exists(matched_with(v));
}

inline
void 
bipartite_graph::add_to_matching(const edge & e)
{
  assert(edge_exists(e));
  
  if(!edge_exists(e) || is_in_matching(e))
    return;
  
  unmatch_vertex(e.left_vertex());
  unmatch_vertex(e.right_vertex());
  
  _matching.set_attribute(e.left_vertex(), matching_entry(e.right_vertex()));
  _matching.set_attribute(e.right_vertex(), matching_entry(e.left_vertex()));
}

inline
void 
bipartite_graph::unmatch_vertex(const vertex & v)
{
  vertex u = matched_with(v);
  
  if(vertex_exists(u))
    {
      _matching.set_attribute(v, matching_entry());
      _matching.set_attribute(u, matching_entry());
    }
}

inline
void 
bipartite_graph::remove_from_matching(const edge & e)
{
  assert(vertex_exists(e.left_vertex()));
  assert(vertex_exists(e.right_vertex()));
  
  if(is_in_matching(e))
    {
      _matching.set_attribute(e.left_vertex(), matching_entry());
      _matching.set_attribute(e.right_vertex(), matching_entry());
    }
}

inline
void 
bipartite_graph::reset_matching()
{
  _matching.reset_attributes(this);
}

inline
bipartite_graph::vertex 
bipartite_graph::find_unmatched_left() const
{
  return find_unmatched(LEFT);
}

inline
bipartite_graph::vertex 
bipartite_graph::find_unmatched_right() const
{
  return find_unmatched(RIGHT);
}

inline
bool 
bipartite_graph::is_covered(side s) const
{
  return !vertex_exists(find_unmatched(s));
}

inline
bool 
bipartite_graph::is_covered_left() const
{
  return is_covered(LEFT);
}

inline
bool 
bipartite_graph::is_covered_right() const
{
  return is_covered(RIGHT);
}

inline
bool 
bipartite_graph::is_vital(const edge & e) const
{
  assert(vertex_exists(e.left_vertex()));
  assert(vertex_exists(e.right_vertex()));
  
  return is_in_matching(e) && 
    _matching.get_attribute(e.left_vertex())._vital_match;
}

inline
bool 
bipartite_graph::is_matched_vital(const vertex & v) const
{
  return is_matched(v) && _matching.get_attribute(v)._vital_match;
}

inline
void 
bipartite_graph::add_to_vital(const edge & e)
{
  assert(is_in_matching(e));
  
  if(!is_in_matching(e))
    return;
  
  _matching.set_attribute(e.left_vertex(), matching_entry(e.right_vertex(), true));
  _matching.set_attribute(e.right_vertex(), matching_entry(e.left_vertex(), true));

}

inline
void 
bipartite_graph::remove_from_vital(const edge & e)
{
  if(is_vital(e))
    {
      _matching.set_attribute(e.left_vertex(), 
			      matching_entry(e.right_vertex(), false));
      _matching.set_attribute(e.right_vertex(), 
			      matching_entry(e.left_vertex(), false));
    }
}


inline
bool bipartite_graph::is_important(const edge & e) const
{
  assert(vertex_exists(e.left_vertex(), true));
  assert(vertex_exists(e.right_vertex(), true));
  return _important_edges.is_marked(e);
}

inline
void bipartite_graph::add_to_important(const edge & e)
{
  assert(vertex_exists(e.left_vertex(), true));
  assert(vertex_exists(e.right_vertex(), true));
  _important_edges.mark_edge(e, true);
}

inline
void bipartite_graph::remove_from_important(const edge & e)
{
  assert(vertex_exists(e.left_vertex(), true));
  assert(vertex_exists(e.right_vertex(), true));
  _important_edges.mark_edge(e, false);
}

inline
void bipartite_graph::reset_important(bool value)
{
  _important_edges.reset_marking(this, true, value);
}



inline
std::ostream & 
operator << (std::ostream & ostr, const bipartite_graph & g)
{
  g.out(ostr);
  return ostr;
} 

inline
std::ostream & 
operator << (std::ostream & ostr, const bipartite_graph::vertex & v)
{
  v.out(ostr);
  return ostr;
}

inline
std::ostream & 
operator << (std::ostream & ostr, const bipartite_graph::edge & e)
{
  e.out(ostr);
  return ostr;
}


#endif // _BIPARTITE_GRAPH_H
