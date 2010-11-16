/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.Edge;

import org.vrjuggler.jccl.config.*;


/**
 * A special-purpose graph model used for proxy/device connections.  In this
 * model, proxies are always sources, and devices (rather, input sources
 * within the device) are always targets.
 */
public class DeviceGraphModel
   extends DefaultGraphModel
   implements org.vrjuggler.vrjconfig.commoneditors.EditorConstants
{
   /**
    * Verifies that the given port can be a source of the given edge.  The
    * actual work is handed off to the user-defined object associated with
    * the parent of <code>port</code>&mdash;if said object is of type
    * <code>ConfigElementHolder</code>.
    *
    * @see ConfigElementHolder#acceptsSource(org.jgraph.graph.GraphModel,Object,Object)
    */
   public boolean acceptsSource(Object edge, Object port)
   {
      boolean accepts = false;

      if ( port != null )
      {
         try
         {
            DefaultPort port_obj = (DefaultPort) port;
            DefaultGraphCell cell = (DefaultGraphCell) port_obj.getParent();
            ConfigElementHolder h = (ConfigElementHolder) cell.getUserObject();
            accepts = h.acceptsSource(this, edge, port);
         }
         catch (Exception ex)
         {
            /* Oh well. */ ;
         }
      }

      return accepts;
   }

   /**
    * Verifies that the given port can be a target of the given edge.  The
    * actual work is handed off to the user-defined object associated with
    * the parent of <code>port</code>&mdash;if said object is of type
    * <code>ConfigElementHolder</code>.
    *
    * @see ConfigElementHolder#acceptsTarget(org.jgraph.graph.GraphModel,Object,Object)
    */
   public boolean acceptsTarget(Object edge, Object port)
   {
      boolean accepts = false;

      if ( port != null )
      {
         try
         {
            DefaultPort port_obj = (DefaultPort) port;
            DefaultGraphCell cell = (DefaultGraphCell) port_obj.getParent();
            ConfigElementHolder h = (ConfigElementHolder) cell.getUserObject();
            accepts = h.acceptsTarget(this, edge, port);
         }
         catch (Exception ex)
         {
            /* Oh well. */ ;
         }
      }

      return accepts;
   }

   /**
    * Removes all the given roots from this model and any
    * <code>ConfigElement</code> objects associated with those roots.  For
    * (somewhat) finer grained control of this, see
    * {@linkplain #remove(Object[],boolean) the overload of
    * <code>remove()</code>} that takes a <code>booelan</code> parameter.
    *
    * @param roots      the roots of the graph model to remove
    *
    * @see #remove(Object[],boolean)
    */
   public void remove(Object[] roots)
   {
      this.remove(roots, true);
   }

   /**
    * Removes all the given roots from this model and, depending on the value
    * of <code>removeElements</code>, any <code>ConfigElement</code> objects
    * associated with those roots.
    *
    * @param roots              the roots of the graph model to remove
    * @param removeElements     a flag indicating whether config elements
    *                           associated with the given graph roots should
    *                           be removed from their context and their data
    *                           source
    */
   public void remove(Object[] roots, boolean removeElements)
   {
      Set removed_edges = new HashSet(), removed_vertices = new HashSet();

      for ( int i = 0; i < roots.length; ++i )
      {
         if ( roots[i] instanceof Edge )
         {
            removed_edges.add(roots[i]);
         }
         else if ( roots[i] instanceof DefaultGraphCell )
         {
            removed_vertices.add(roots[i]);

            DefaultGraphCell cur_cell = (DefaultGraphCell) roots[i];

            // Remove edges connected to the port(s) of the cell.
            for ( Iterator c = cur_cell.getChildren().iterator(); c.hasNext(); )
            {
               for ( Iterator e = this.edges(c.next()); e.hasNext(); )
               {
                  removed_edges.add(e.next());
               }
            }
         }
      }

      this.remove(removed_edges.toArray(), removed_vertices.toArray(),
                  removeElements);
   }

   /**
    * Removes all the given edge and vertex roots from this model and,
    * depending on the value of <code>removeElements</code>, any
    * <code>ConfigElement</code> objects associated with those roots.
    *
    * @param edges              the edge roots of the graph model to remove
    * @param vertices           the vertex roots of the graph model to remove
    *                           (must be instances of
    *                           <code>org.jgraph.graph.DefaultGraphCell</code>)
    * @param removeElements     a flag indicating whether config elements
    *                           associated with the given graph roots should
    *                           be removed from their context and their data
    *                           source
    *
    * @see org.jgraph.graph.DefaultGraphCell
    */
   public void remove(Object[] edges, Object[] vertices,
                      boolean removeElements)
   {
      if ( edges.length > 0 )
      {
         super.remove(edges);
      }

      if ( vertices.length > 0 )
      {
         if ( removeElements )
         {
            ConfigBroker broker = new ConfigBrokerProxy();

            for ( int i = 0; i < vertices.length; ++i )
            {
               DefaultGraphCell cell = (DefaultGraphCell) vertices[i];

               if ( cell.getUserObject() instanceof ConfigElementHolder )
               {
                  ConfigElementHolder holder =
                     (ConfigElementHolder) cell.getUserObject();

                  if ( holder instanceof ProxyInfo )
                  {
                     ProxyInfo proxy_info = (ProxyInfo) holder;
                     List aliases = proxy_info.getAliases();
                     for ( Iterator a = aliases.iterator(); a.hasNext(); )
                     {
                        broker.remove(proxy_info.getContext(),
                                      (ConfigElement) a.next());
                     }
                  }

                  broker.remove(holder.getContext(), holder.getElement());
               }
            }
         }

         for ( int i = 0; i < vertices.length; ++i )
         {
            ((DefaultGraphCell) vertices[i]).removeAllChildren();
         }

         super.remove(vertices);
      }
   }
}
