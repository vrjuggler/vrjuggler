/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SIM_NETWORK_GRAPH_H_
#define _VPR_SIM_NETWORK_GRAPH_H_

#include <vpr/vprConfig.h>

#include <string>
#include <utility>
#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <vpr/vpr.h>

#include <vpr/md/SIM/Network/NetworkLine.h>
#include <vpr/md/SIM/Network/NetworkNode.h>


namespace vpr
{

namespace sim
{

/**
 * Container type used to define a BGL property for vertices (nodes) in the
 * simulated network graph.
 */
struct network_node_t
{
   enum { num = 1000 };
   typedef boost::vertex_property_tag kind;
};

/**
 * Container type used to define a BGL property for edges (network lines) in
 * the simulated network graph.
 */
struct network_line_t
{
   enum { num = 1001 };
   typedef boost::edge_property_tag kind;
};

typedef boost::property<network_node_t, vpr::sim::NetworkNode> NodeProperty;
typedef boost::property<network_line_t, vpr::sim::NetworkLine,
                        boost::property<boost::edge_weight_t, int> > LineProperty;

/**
 * Container for a BGL graph object that represents a simulated network
 * constructed from an input file.  In the overall sim socket scheme, this sits
 * at the hardware level (aka, the physical layer).
 */
class VPR_CLASS_API NetworkGraph
{
public:
   NetworkGraph (void)
      : mGraphValid(false)
   {
      /* Do nothing. */ ;
   }

   /**
    * Fills in the graph using the contents of the specified file.  The file
    * must be in the GT-ITM "alternative" format.
    */
   vpr::ReturnStatus construct(const std::string& path);

   bool isValid (void) const
   {
      return mGraphValid;
   }

   /**
    * Removes all the lines (edges) and nodes (vertices) from the network.
    */
   void clear (void)
   {
      mGraph.clear();
   }

   /**
    * Type of the BGL graph encapsulated by this class.  boost::vecS is chosen
    * because graph traversal speed is important, and memory consumption is
    * an issue for very large simulations.  boost::listS does not perform well
    * in those two areas.
    */
   typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, NodeProperty, LineProperty> net_graph_t;

   /// Type of an edge (network line) in the BGL graph.
   typedef boost::graph_traits<net_graph_t>::edge_descriptor net_edge_t;

   /// Type of a vertex (network node) in the BGL graph.
   typedef boost::graph_traits<net_graph_t>::vertex_descriptor net_vertex_t;

   typedef std::vector<net_vertex_t>     VertexList;
   typedef boost::shared_ptr<VertexList> VertexListPtr;

   typedef std::vector<std::pair<net_vertex_t, vpr::Uint32> > AddressList;

   /**
    * Retrieves the property (an instance of vpr::sim::NetworkLine) for the
    * given edge.
    *
    * @pre The given edge object must be in the graph.
    */
   vpr::sim::NetworkLine getLineProperty(const net_edge_t& e) const;

   void setLineProperty(const net_edge_t& e, const vpr::sim::NetworkLine& prop);

   net_vertex_t getNode (const vpr::Uint32 index)
   {
      return boost::vertex(index, mGraph);
   }

   /**
    * Looks up the network node with the given address (a 32-bit IPv4 address).
    * If the node exists in the network, success is returned, and the
    * by-reference parameter is assigned the discovered node.  Otherwise,
    * failure is returned to indicate that there is no node with the given
    * address.
    *
    * @param addr The address of the desired node as a 32-bit IPv4 address.
    * @param node Storage for the discovered node if one is found.
    */
   vpr::ReturnStatus getNodeWithAddr(const vpr::Uint32 addr,
                                     net_vertex_t& node);

   /**
    * Retrieves the property (an instance of vpr::sim::NetworkNode) for the
    * given vertex.
    *
    * @pre The given vertex object must be in the graph.
    */
   vpr::sim::NetworkNode getNodeProperty(const net_vertex_t& e) const;

   /**
    * Assigns the given property to the designated node.
    *
    * @pre The node object must be in the graph.
    */
   void setNodeProperty(const net_vertex_t& v,
                        const vpr::sim::NetworkNode& prop);

   /**
    * Returns the edge that connects vertices u and v in this graph.
    */
   std::pair<net_edge_t, bool> getEdge (const net_vertex_t& u,
                                        const net_vertex_t& v)
      const
   {
      return boost::edge(u, v, mGraph);
   }

   /**
    * Returns the shortest path (list of vertices) from src (source) to dest
    * (destination).
    *
    * @param src  The source node.
    * @param dest The destination node.
    *
    * @return
    */
   VertexListPtr getShortestPath(const net_vertex_t& src,
                                 const net_vertex_t& dest) const;

   /**
    * Reverses the given vertex path.  This is basically a way to save
    * computations by reducing the number of calls to getShortestPath().  The
    * result of calling this with a path returned by getShortestPath(u,v) is
    * the same as calling getShortestPath(v,u), but it runs in O(path->size())
    * time rather than O((V + E) * log V).
    */
   VertexListPtr reversePath(VertexListPtr path);

   /**
    * Returns the current number of nodes in this network.
    */
   vpr::Uint32 getNodeCount (void) const
   {
      return boost::num_vertices(mGraph);
   }

   /**
    * Checks to see if the given vertex is the source of the given edge.
    */
   bool isSource(const net_vertex_t& vertex, const net_edge_t& edge) const
   {
      NetworkGraph::net_vertex_t source = boost::source(edge, mGraph);

      return vertex == source;
   }

   /**
    * Returns all the addresses in the network via the by-reference parameter.
    */
   vpr::ReturnStatus getAllAddresses (AddressList& list);

private:
   net_graph_t mGraph;
   bool        mGraphValid;
};

} // End of sim namespace

} // End of vpr namespace


#endif /* _VPR_SIM_NETWORK_GRAPH_H_ */
