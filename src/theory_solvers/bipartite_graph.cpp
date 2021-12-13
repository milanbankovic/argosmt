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

#include "bipartite_graph.hpp"

bipartite_graph::undirected_left_iterator_impl bipartite_graph::_ul_impl;
bipartite_graph::undirected_right_iterator_impl bipartite_graph::_ur_impl;
bipartite_graph::directed_left_iterator_impl bipartite_graph::_dl_impl;
bipartite_graph::directed_right_iterator_impl bipartite_graph::_dr_impl;
bipartite_graph::directed_source_iterator_impl bipartite_graph::_ds_impl;
bipartite_graph::directed_sink_iterator_impl bipartite_graph::_dt_impl;



bipartite_graph::vertex 
bipartite_graph::vertex_marking::find_unmarked(side s) const
{   
  unsigned index = s == RIGHT ? 
    _num_of_left : 0;
  
  unsigned end = s == LEFT ? 
    _num_of_left : _marks.size();
  
  for(; index < end && _marks[index]; index++);
  
  if(index == end)
    return vertex();
  
  if(index < _num_of_left)
    return vertex(index, LEFT);
  else
    return vertex(index - _num_of_left, RIGHT); 
}


void bipartite_graph::edge_marking::marked_edges_vector(std::vector<edge> & edges) const
{
  for(unsigned i = 0; i <  _marks.m(); i++)
    for(unsigned j = 0; j < _marks.n(); j++)
      if(_marks(i,j))
	edges.push_back(edge(i,j));  
}  

bipartite_graph::edge_iterator::edge_iterator(const vertex & v, 
					      const bipartite_graph * g, 
					      position pos,
					      bool directed)
  :_side(v.vertex_side()),
   _directed(directed),
   _graph(g) 
{

  if(!directed)
    {
      if(v.vertex_side() == LEFT)
	_impl = &bipartite_graph::_ul_impl;
      else
	_impl = &bipartite_graph::_ur_impl;      
    }
  else
    {
      if(v.vertex_side() == LEFT)
	{
	  if(v.vertex_number() < g->number_of_left_vertices())
	    _impl = &bipartite_graph::_dl_impl;
	  else
	    _impl = &bipartite_graph::_dt_impl;
	}
      else
	{
	  if(v.vertex_number() < g->number_of_right_vertices())
	    _impl = &bipartite_graph::_dr_impl;
	  else
	    _impl = &bipartite_graph::_ds_impl;
	}      
    }
  
  if(pos == BEGIN)
    _impl->first_edge(g, v.vertex_number(), _edge);
  else
    _impl->last_edge(g, v.vertex_number(), _edge);
}

bipartite_graph::vertex 
bipartite_graph::find_unmatched(side s) const
{
  switch(s)
    {
    case LEFT:
      for(vertex_iterator vi = begin_left(), end_vi = end_left(); 
	  vi != end_vi; 
	  ++vi)
	if(!is_matched(*vi))
	  return *vi;
      break;
    case RIGHT:
      for(vertex_iterator vi = begin_right(), end_vi = end_right(); 
	  vi != end_vi; 
	  ++vi)
	if(!is_matched(*vi))
	  return *vi;
      break;
    case UNSPECIFIED:
      for(vertex_iterator vi = begin_left(), end_vi = end_left(); 
	  vi != end_vi; 
	  ++vi)
	if(!is_matched(*vi))
	  return *vi;
      for(vertex_iterator vi = begin_right(), end_vi = end_right(); 
	  vi != end_vi; 
	  ++vi)
	if(!is_matched(*vi))
	  return *vi;
      break;
    }
  return vertex();
}

void 
bipartite_graph::edge_marking::mark_all_edges(const vertex & v, bool value)
{
  if(v.vertex_side() == LEFT)
    {
      unsigned i = v.vertex_number();
      for(unsigned j = 0; j < _marks.n(); j++)
	_marks(i, j) = value;
    }
  else if(v.vertex_side() == RIGHT)
    {
      unsigned j = v.vertex_number();
      for(unsigned i = 0; i < _marks.m(); i++)
	_marks(i, j) = value;
    }
}


unsigned bipartite_graph::matching_size() const
{
  unsigned count = 0;
  for(vertex_iterator vi = begin_left(), end_vi = end_left(); 
      vi != end_vi; 
      ++vi)
    if(is_matched(*vi))
      count++;
  return count;
}

void bipartite_graph::reset_vital()
{
  for(vertex_iterator vi = begin_left(), end_vi = end_left(); 
      vi != end_vi; 
      ++vi)
    {
      vertex v = matched_with(*vi);
      
      if(vertex_exists(v))
	remove_from_vital(edge(*vi, v));
    }
}

unsigned bipartite_graph::number_of_edges(const vertex & v, bool directed) const
{
  unsigned count = 0;

  for(edge_iterator it = begin_edge(v, directed), it_end = end_edge(v, directed);
      it != it_end; ++it)
    count++;
 
 return count;
}
  
void 
bipartite_graph::greedy_matching()
{
    for(vertex_iterator vi = begin_left(), end_vi = end_left(); 
      vi != end_vi; 
      ++vi)
    if(!is_matched(*vi))
      for(edge_iterator ei = begin_edge(*vi), end_ei = end_edge(*vi); 
	  ei != end_ei; 
	  ++ei)
	if(!is_matched(ei->right_vertex()))
	  {
	    add_to_matching(*ei);
	    break;
	  } 
}

// Ford-Fulkerson's algorithm for optimal matching
void 
bipartite_graph::max_cardinality_matching()
{
  bool restart = true;
  vertex source = source_vertex();
  vertex sink = sink_vertex();

  while(restart)
    {      
      std::queue<vertex> vertex_queue;
      vertex_marking marking(this, true);
      vertex_attribute<vertex> previous(this, true);
      
      restart = false;
      vertex_queue.push(source);
      marking.mark_vertex(source);

      while(!vertex_queue.empty())
	{
	  vertex u = vertex_queue.front();
	  vertex_queue.pop();

	  if(u == sink)
	    {
	      vertex current = previous.get_attribute(u);
	      vertex prev;
	      while((prev = previous.get_attribute(current)) != source)
		{
		  if(current.vertex_side() == RIGHT)
		    add_to_matching(edge(prev, current));
		  else
		    remove_from_matching(edge(current, prev));
		  current = prev;
		}	      
	      restart = true;
	      break;
	    }

	  for(edge_iterator ei = begin_edge(u, true), end_ei = end_edge(u, true); 
	      ei != end_ei; ++ei)
	    {
	      vertex v = ei->end_vertex(this);
	      if(!marking.is_marked(v))
		{
		  vertex_queue.push(v);
		  marking.mark_vertex(v);
		  previous.set_attribute(v, u);
		}		  
	    }
	}
    }
}

// Tarjan's SCC algorithm that marks edges belonging to directed cycles (used)
void
bipartite_graph::tarjan_cycles(const vertex & v, 
			       unsigned & count, 
			       vertex_attribute<unsigned> & index, 
			       vertex_attribute<unsigned> & lowlink, 
			       std::stack<vertex> & vertex_stack, 
			       vertex_marking & stack_marking, 
			       vertex_marking & marking,
			       edge_marking & used)
{  
  marking.mark_vertex(v);
  index.set_attribute(v, count);
  lowlink.set_attribute(v, count);
  count++;
  vertex_stack.push(v);
  stack_marking.mark_vertex(v);
  edge lastlink;

  for(edge_iterator ei = begin_edge(v, true), end_ei = end_edge(v, true); 
      ei != end_ei; ++ei)
    {
      vertex u = ei->end_vertex(this);
      if(!marking.is_marked(u))
	{
	  tarjan_cycles(u, 
			count, 
			index, 
			lowlink, 
			vertex_stack, 
			stack_marking, 
			marking, 
			used);
	 
	  // The condition seems not to have any effect, since such edges
	  // would be removed as inconsistent!
	  //if(stack_marking.is_marked(u))
	  add_to_important(*ei);

	  unsigned v_low = lowlink.get_attribute(v);
	  unsigned u_low = lowlink.get_attribute(u);
	  if(u_low < v_low)
	    {
	      lowlink.set_attribute(v, u_low);
	      lastlink = *ei;
	    }
	}
      else if(stack_marking.is_marked(u))
	{
	  unsigned v_low = lowlink.get_attribute(v);
	  unsigned u_index = index.get_attribute(u);
	  if(u_index < v_low)
	    {
	      lowlink.set_attribute(v, u_index);
	      lastlink = *ei;
	    }
	}
    }
  if(lastlink != edge())
    add_to_important(lastlink);

  if(index.get_attribute(v) == lowlink.get_attribute(v))
    {
      std::vector <vertex> component;
      vertex_marking component_marking(this, true);
      vertex u;

      do {
	u = vertex_stack.top();
	vertex_stack.pop();
	stack_marking.mark_vertex(u, false);
	component.push_back(u);
	component_marking.mark_vertex(u);	
      } while(u != v);
      
      for(unsigned i = 0; i < component.size(); i++)
	{
	  u = component[i];
	  if(!vertex_exists(u)) // ignore source and sink
	    continue;

	  for(edge_iterator ei = begin_edge(u, true), end_ei = end_edge(u, true); 
	      ei != end_ei; ++ei)
	    {
	      vertex eu = ei->end_vertex(this);
	      if(vertex_exists(eu) && component_marking.is_marked(eu))
		{
		  used.mark_edge(*ei);
		}
	    }
      	}
    }
}


// Calls Tarjan's algorithm until the entire graph is traversed and all
// SCCs are found. Returns the set of edges used in directed cycles.
void
bipartite_graph::mark_even_alternating_cycles(edge_marking & used)
{
  vertex_attribute<unsigned> index(this, true);
  vertex_attribute<unsigned> lowlink(this, true);
  std::stack<vertex> vertex_stack;
  vertex_marking stack_marking(this, true);
  vertex_marking marking(this, true);
  vertex v;
  unsigned count = 0;

  while(vertex_exists((v = marking.find_unmarked()), true))
    {
      tarjan_cycles(v, 
		    count, 
		    index, 
		    lowlink, 
		    vertex_stack, 
		    stack_marking, 
		    marking, 
		    used);
    }
}

// Regin's algorithm
void 
bipartite_graph::make_arc_consistent(std::vector<edge> & removed_edges, 
				     std::vector<edge> & new_vital_edges)
{ 
  _common_data->_count_tarjan++;

  assert(is_covered_left());
  edge_marking used(this);
  reset_important();
  _important_removed = false;
  mark_even_alternating_cycles(used);

  for(vertex_iterator vi = begin_left(), end_vi = end_left(); 
      vi != end_vi; 
      ++vi)
    for(edge_iterator ei = begin_edge(*vi), end_ei = end_edge(*vi); 
	ei != end_ei; 
	++ei)
      {
	if(!used.is_marked(*ei))
	  {
	    if(is_in_matching(*ei))
	      {
		if(!is_vital(*ei))
		  {
		    add_to_vital(*ei);
		    new_vital_edges.push_back(*ei);
		  }		
	      }
	    else
	      {
		removed_edges.push_back(*ei);
		remove_edge(*ei);
	      }	  
	  }
      }
}

// Finds obstacles reachable from v (BFS based)
void bipartite_graph::find_reachable_obstacles(const vertex & v,
					       const edge_marking & obstacles, 
					       edge_marking & reached_obstacles,
					       vertex_marking & marking) const
{
  std::queue<vertex> vertex_queue;
  vertex_queue.push(v);
  marking.mark_vertex(v);

  while(!vertex_queue.empty())
    {
      vertex u = vertex_queue.front();
      vertex_queue.pop();
      
      for(edge_iterator ei = begin_edge(u, true), end_ei = end_edge(u, true); 
	  ei != end_ei; 
	  ++ei)
	{
	  vertex v = ei->end_vertex(this);
	  if(obstacles.is_marked(*ei))
	    {
	      reached_obstacles.mark_edge(*ei);
	    }
	  else if(!marking.is_marked(v))
	    {
	      vertex_queue.push(v);
	      marking.mark_vertex(v);
	    }
	}
    }
}


// Tarjan's algorithm for finding the set of unblocked vertices
void
bipartite_graph::tarjan_unblocked_vertices(const vertex & v,
					   const vertex & v_end,
					   unsigned & count, 
					   vertex_attribute<unsigned> & index, 
					   vertex_attribute<unsigned> & lowlink, 
					   std::stack<vertex> & vertex_stack, 
					   vertex_marking & stack_marking, 
					   vertex_marking & marking,
					   const edge_marking & obstacles,
					   vertex_marking & unblocked) const
{  
  marking.mark_vertex(v);
  index.set_attribute(v, count);
  lowlink.set_attribute(v, count);
  count++;
  
  if(v == v_end)
    {
      unblocked.mark_vertex(v);
      return;
    }

  vertex_stack.push(v);
  stack_marking.mark_vertex(v);
  	
  for(edge_iterator ei = begin_edge(v, true), end_ei = end_edge(v, true); 
      ei != end_ei; 
      ++ei)
    {
      if(obstacles.is_marked(*ei))
	{
	  continue;
	}
      
      vertex u = ei->end_vertex(this);
      if(!marking.is_marked(u))
	{
	  tarjan_unblocked_vertices(u,
				    v_end,
				    count, 
				    index, 
				    lowlink, 
				    vertex_stack, 
				    stack_marking, 
				    marking,
				    obstacles,
				    unblocked);
	  
	  unsigned v_low = lowlink.get_attribute(v);
	  unsigned u_low = lowlink.get_attribute(u);
	  if(u_low < v_low)
	    lowlink.set_attribute(v, u_low);
	}
      else if(stack_marking.is_marked(u))
	{
	  unsigned v_low = lowlink.get_attribute(v);
	  unsigned u_index = index.get_attribute(u);
	  if(u_index < v_low)
	    lowlink.set_attribute(v, u_index);
	}
      
      if(unblocked.is_marked(u))
	unblocked.mark_vertex(v);
    }
    
  if(index.get_attribute(v) == lowlink.get_attribute(v))
    {
      vertex u;
      
      do {
	u = vertex_stack.top();
	vertex_stack.pop();
	stack_marking.mark_vertex(u, false);
	
	if(unblocked.is_marked(v))
	  unblocked.mark_vertex(u);
	
      } while(u != v);
    }
}

// Minimal obstacle set algorithm (MOS)
void bipartite_graph::find_minimal_obstacle_set(const vertex & v_start, 
						const vertex & v_end,
						const edge_marking & obstacles,
						edge_marking & mos) const
{
  unsigned count = 0;
  vertex_attribute<unsigned> index(this, true); 
  vertex_attribute<unsigned> lowlink(this, true); 
  std::stack<vertex> vertex_stack; 
  vertex_marking stack_marking(this, true); 
  vertex_marking marking(this, true); 
  vertex_marking unblocked(this, true);

  // For efficiency, source_vertex should not be checked
  marking.mark_vertex(source_vertex());

  // Finds reachable obstacles from v_start (v_end should stay unreached)
  find_reachable_obstacles(v_start, obstacles, mos, marking);
  assert(!marking.is_marked(v_end));

  static std::vector<edge> obstacles_vector;
  obstacles_vector.clear();
  mos.marked_edges_vector(obstacles_vector);
  
  for(unsigned i = 0; i < obstacles_vector.size(); i++)
    {
      vertex v = obstacles_vector[i].end_vertex(this);
      
      // For each end vector of reached obstacles, checks if it is unblocked
      // (with respect to the vertex v_end). 
      if(!marking.is_marked(v))
	{
	  tarjan_unblocked_vertices(v,
				    v_end,
				    count, 
				    index, 
				    lowlink, 
				    vertex_stack, 
				    stack_marking, 
				    marking,
				    mos,
				    unblocked);
	}
      
      // Removes blocked reachable obstacles as redundant
      if(!unblocked.is_marked(v))
	{
	  mos.mark_edge(obstacles_vector[i], false);
	}
    }  
}

// Reduces propagation explaining to MOS
void 
bipartite_graph::reduce_prop_expl_to_mos(const edge & e,
					 const edge_marking & obstacles, 
					 edge_marking & reason) const
{
  assert(is_covered_left());
  assert(edge_exists(e));
  assert(!is_vital(e));
  
  assert(!obstacles.is_marked(e));
  
  if(!is_in_matching(e) && 
     obstacles.is_marked(edge(e.left_vertex(), 
			      matched_with(e.left_vertex()))))
    {
      reason.mark_edge(edge(e.left_vertex(), 
			    matched_with(e.left_vertex())));
      return;
    }
  
  vertex x,y;
  
  x = e.begin_vertex(this);
  y = e.end_vertex(this);

  find_minimal_obstacle_set(y, x, obstacles, reason);
}


// For all unmatched vertices, the procedure tries to match them using augmenting paths. The
// first vertex that cannot be matched this way (it certainly exists) is returned.
bipartite_graph::vertex bipartite_graph::find_unmatchable_left_vertex(const edge_marking & obstacles)
{
  bool restart = true;
  vertex_iterator vi = begin_left(), end_vi = end_left();
  vertex sink = sink_vertex();
  vertex source = source_vertex();
  while(restart)
    {
      std::queue<vertex> vertex_queue;
      vertex_attribute<vertex> previous(this, true);
      vertex_marking marking(this, true);
      restart = false;
      
      for(; vi != end_vi; ++vi)	
	if(!is_matched(*vi))
	  {
	    vertex_queue.push(*vi);
	    marking.mark_vertex(*vi);
	    previous.set_attribute(*vi, source);
	    marking.mark_vertex(source);
	    break;
	  }
      assert(!vertex_queue.empty());
      while(!vertex_queue.empty())
	{
	  vertex u = vertex_queue.front();
	  vertex_queue.pop();

	  if(u == sink)
	    {
	      vertex current = previous.get_attribute(u);
	      vertex prev;
		    
	      while((prev = previous.get_attribute(current)) != source)
		{
		  if(current.vertex_side() == RIGHT)
		    add_to_matching(edge(prev, current));
		  else
		    remove_from_matching(edge(current, prev));
		  current = prev;
		}
	      restart = true;
	      break;	      
	    }
	  
	  for(edge_iterator ei = begin_edge(u, true), end_ei = end_edge(u, true); 
	      ei != end_ei; 
	      ++ei)
	    {
	      vertex v = ei->end_vertex(this);
	      if(!obstacles.is_marked(*ei) && !marking.is_marked(v))
		{
		  vertex_queue.push(v);
		  marking.mark_vertex(v);
		  previous.set_attribute(v, u);
		}
	    }
	}
    }
  assert(vi != end_vi);
  return *vi;
}


// Reduces conflict explaining to mos
void bipartite_graph::reduce_cflt_expl_to_mos(const edge_marking & obstacles, 
					      edge_marking & reason)
{
  vertex unmatchable = find_unmatchable_left_vertex(obstacles);
  find_minimal_obstacle_set(unmatchable, sink_vertex(), obstacles, reason);
}

// Classical Tarjan's algorithm that finds SCCs (using edges not in obstacles).
// Used for Katsirelos-based propagation explaining.
void 
bipartite_graph::tarjan_hall_obstacles(const vertex & v,
				       unsigned & count, 
				       vertex_attribute<unsigned> & index, 
				       vertex_attribute<unsigned> & lowlink, 
				       std::stack<vertex> & vertex_stack, 
				       vertex_marking & stack_marking, 
				       vertex_marking & marking,
				       const edge_marking & obstacles,
				       vertex_attribute<unsigned> & scc_root) const
{
  marking.mark_vertex(v);
  index.set_attribute(v, count);
  lowlink.set_attribute(v, count);
  count++;

  vertex_stack.push(v);
  stack_marking.mark_vertex(v);
  
  for(edge_iterator ei = begin_edge(v, true), end_ei = end_edge(v, true); 
      ei != end_ei; ++ei)
    {
      if(obstacles.is_marked(*ei))
	{
	  continue;
	}
      
      vertex u = ei->end_vertex(this);

      if(!marking.is_marked(u))
	{
	  tarjan_hall_obstacles(u,
				count, 
				index, 
				lowlink, 
				vertex_stack, 
				stack_marking, 
				marking,
				obstacles,
				scc_root);
	  
	  unsigned v_low = lowlink.get_attribute(v);
	  unsigned u_low = lowlink.get_attribute(u);
	  if(u_low < v_low)
	    lowlink.set_attribute(v, u_low);
	}
      else if(stack_marking.is_marked(u))
	{
	  unsigned v_low = lowlink.get_attribute(v);
	  unsigned u_index = index.get_attribute(u);
	  if(u_index < v_low)
	    lowlink.set_attribute(v, u_index);
	}
    }
  
  if(index.get_attribute(v) == lowlink.get_attribute(v))
    {
      unsigned v_low = lowlink.get_attribute(v);
      //std::cout << "Root: " << v << "(low: " << lowlink.get_attribute(v) << ", " << index.get_attribute(v) << ")" << std::endl;
      vertex u;     
      do {
	u = vertex_stack.top();
	scc_root.set_attribute(u, v_low); 
	//	std::cout << "Vertex: " << u << " (low: " << lowlink.get_attribute(u) << ", " << index.get_attribute(u) << ")" << std::endl;
	vertex_stack.pop();
	stack_marking.mark_vertex(u, false);
      } while(u != v);
    }
}

// Reduces propagation explaining to Katsirelos' algorithm
void bipartite_graph::reduce_prop_expl_to_kat(const edge & e,
					      const edge_marking & obstacles,
					      edge_marking & reason) const
{

  std::vector<edge> to_explain;
  
  if(is_in_matching(e))
    {
      vertex u = e.left_vertex();
      for(bipartite_graph::edge_iterator ei = begin_edge(u), ei_end = end_edge(u);
	  ei != ei_end; ++ei)
	{
	  if(is_in_matching(*ei))
	    continue;
	  
	  if(obstacles.is_marked(*ei))
	    {
	      reason.mark_edge(*ei);
	    }
	  else
	    to_explain.push_back(*ei);
	}
    }
  else
    {
      to_explain.push_back(e);
    }
  
  unsigned count = 0;
  std::stack<vertex> vertex_stack; 
  vertex_marking stack_marking(this, true); 
  vertex_attribute<unsigned> index(this, true); 
  vertex_attribute<unsigned> lowlink(this, true); 
  vertex_attribute<unsigned> scc_root(this, true, 0xffffffff);
  vertex_marking marking(this, true); 

  for(unsigned k = 0; k < to_explain.size(); k++)
    {
      edge pe = to_explain[k];
      assert(!obstacles.is_marked(pe));
      assert(!is_in_matching(pe));
      vertex x = pe.right_vertex();
      assert(is_matched(x));
     
      if(!marking.is_marked(x))
	tarjan_hall_obstacles(x,
			      count, 
			      index, 
			      lowlink, 
			      vertex_stack, 
			      stack_marking, 
			      marking,
			      obstacles,
			      scc_root);
      
      unsigned xl = scc_root.get_attribute(x);
      std::vector<vertex> hall_set;
      for(vertex_iterator vi = begin_left(), vi_end = end_left(); vi != vi_end; ++vi)
	{     
	  if(scc_root.get_attribute(*vi) == xl)
	    {
	      //std::cout << "Adding to set: " << *vi << " (low: " << lowlink.get_attribute(*vi) << ")" << std::endl;
	      hall_set.push_back(*vi);
	    }
	}
      if(hall_set.empty())
	hall_set.push_back(matched_with(x));
      
      //  std::cout << "HALL SET: ";
      for(unsigned i = 0; i < hall_set.size(); i++)
	{
	  //   std::cout << hall_set[i] << ", ";
	  for(edge_iterator ei = begin_edge(hall_set[i]), ei_end = end_edge(hall_set[i]);
	      ei != ei_end; ++ei)
	    {
	      vertex v = ei->right_vertex();
	      if(scc_root.get_attribute(v) != xl)
		reason.mark_edge(*ei);       
	    }      
	}
      //  std::cout << std::endl;
    }
}


// Reduces conflict explaining to Katsirelos' algorithm. 
void bipartite_graph::reduce_cflt_expl_to_kat(const edge_marking & obstacles, 
					      edge_marking & reason)
{
  vertex unmatchable = find_unmatchable_left_vertex(obstacles);
  vertex_marking marking(this, true);  
  marking.mark_vertex(source_vertex());
  find_reachable_obstacles(unmatchable, obstacles, reason, marking);

  for(vertex_iterator vi = begin_left(), vi_end = end_left(); vi != vi_end; ++vi)
    {
      if(marking.is_marked(*vi))
	{
	  for(edge_iterator ei = begin_edge(*vi), ei_end = end_edge(*vi); 
	      ei != ei_end; ++ei)
	    {
	      if(marking.is_marked(ei->right_vertex()))
		{
		  reason.mark_edge(*ei, false);
		}
	      else
		{
		  assert(reason.is_marked(*ei));
		}
	    }
	}
    }
}

// Removes inconsistent edges from the graph (caused by assertions in remove and vital)
// and populates removed_edges with inconsistent edges and new_vital_edges with newly
// found vital edges. Returns true if the graph is in a consistent state.
bool bipartite_graph::do_propagation(const std::vector<edge> & remove,
				     const std::vector<edge> & vital,
				     std::vector<edge> & removed_edges, 
				     std::vector<edge> & new_vital_edges,
				     bool strong)
{
  bool ret_value = true;
  bool compute_matching = !is_covered_left();

  for(unsigned i = 0; i < remove.size(); i++)
    { 
      if(!edge_exists(remove[i]))
	continue;
      
      if(is_in_matching(remove[i]))
	compute_matching = true;
  
      if(is_vital(remove[i]))
	ret_value = false;
    
      if(!_important_removed && is_important(remove[i]))
	_important_removed = true;

      remove_edge(remove[i]);
    }

  for(unsigned i = 0; i < vital.size(); i++)
    {       
      vertex u = vital[i].left_vertex();
      for(edge_iterator ei = begin_edge(u), end_ei = end_edge(u); 
	  ei != end_ei; 
	  ++ei)
	{
	  if(*ei == vital[i])
	    continue;
	  
	  if(is_in_matching(*ei))      
	    compute_matching = true;
	  
	  if(!_important_removed && is_important(*ei))
	    _important_removed = true;

	  removed_edges.push_back(*ei);
	  remove_edge(*ei);
	}
      
      u = vital[i].right_vertex();
      for(edge_iterator ei = begin_edge(u), end_ei = end_edge(u); 
	  ei != end_ei; 
	  ++ei)
	{
	  if(*ei == vital[i])
	    continue;
	  
	  if(is_in_matching(*ei))      
	    compute_matching = true;
	  
	  if(!_important_removed && is_important(*ei))
	    _important_removed = true;

	  removed_edges.push_back(*ei);
	  remove_edge(*ei);
	}
      
      if(edge_exists(vital[i]))
	{
	  add_to_matching(vital[i]);
	  add_to_vital(vital[i]);
	}
      else
	ret_value = false;
    }

  unsigned rme_size = removed_edges.size();
  _common_data->_count_trivial_prunings += rme_size;

  
  if(ret_value == false)
    {
      max_cardinality_matching();
      assert(!is_covered_left());
      return false;
    }

  if(strong)
    {
      if(compute_matching)
	max_cardinality_matching();
      
      ret_value = is_covered_left();
      
      if(ret_value && _important_removed)
	{	  
	  make_arc_consistent(removed_edges, new_vital_edges);
	  _common_data->_count_hall_prunings += removed_edges.size() - rme_size;
	  if(removed_edges.size() > rme_size || new_vital_edges.size() > 0)
	    _common_data->_count_tarjan_productive++;
	}
    }

  return ret_value;
}

void 
bipartite_graph::out(std::ostream & ostr) const
{
  ostr << "graph: " << std::endl;
  for(vertex_iterator vi = begin_left(), end_vi = end_left(); 
      vi != end_vi; 
      ++vi)
    {
      for(vertex_iterator vj = begin_right(), end_vj = end_right(); 
	  vj != end_vj; 
	  ++vj)
	ostr << edge_exists(edge(*vi, *vj)) << ' ';
      ostr << std::endl;
    }
  
  ostr << "matching: " << std::endl;
  for(vertex_iterator vi = begin_left(), end_vi = end_left(); 
      vi != end_vi; 
      ++vi)
    {
      for(vertex_iterator vj = begin_right(), end_vj = end_right(); 
	  vj != end_vj; 
	  ++vj)
	ostr << are_matched(*vi, *vj) << ' ';
      ostr << std::endl;
    }
  
  ostr << "vital: " << std::endl;
  for(vertex_iterator vi = begin_left(), end_vi = end_left(); 
      vi != end_vi; 
      ++vi)
    {
      for(vertex_iterator vj = begin_right(), end_vj = end_right(); 
	  vj != end_vj; 
	  ++vj)
	ostr << is_vital(edge(*vi, *vj)) << ' ';
      ostr << std::endl;
    }

}
