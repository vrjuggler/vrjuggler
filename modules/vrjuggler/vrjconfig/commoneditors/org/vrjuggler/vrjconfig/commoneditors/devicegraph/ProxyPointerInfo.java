/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.Edge;
import org.jgraph.graph.GraphModel;

import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.PropertyDefinition;


/**
 * An information object typed to be used as the user-defined object held
 * by the ports of a graph cell representing a device that is configured
 * relative to a device proxy.
 *
 * @see ProxyInfo
 */
public class ProxyPointerInfo
   extends ConfigElementHolder
{
   /**
    * Creates a new instance of the information object for a pointer to a
    * device proxy.  The pointer to the device proxy is described by the
    * given property definition.
    *
    * @param elt        the config element that is configured relative to a
    *                   proxy
    * @param ctx        the context containing <code>elt</code>
    * @param propDef    the property definition that describes the property of
    *                   <code>elt</code> that points at a device proxy
    */
   public ProxyPointerInfo(ConfigElement elt, ConfigContext ctx,
                           PropertyDefinition propDef)
   {
      super(elt, ctx);

      this.pointerPropertyDefinition = propDef;
   }

   /**
    * Returns the property definition that describes this object's pointer
    * to a device proxy.
    *
    * @see ConfigElementHolder#getElement()
    */
   public PropertyDefinition getPointerPropertyDefinition()
   {
      return this.pointerPropertyDefinition;
   }

   /**
    * Verifies that the given port can be a source of the given edge within
    * the given model.  This determination is made by testing to see if the
    * config elmeent held by the parent of the target port (which must have
    * parent containing a <code>ProxyInfo</code> object) is of a type at
    * which we are allowed to point.
    */
   public boolean acceptsSource(GraphModel model, Object edge, Object port)
   {
      boolean accepts = false;

      Object[] out_edges = DefaultGraphModel.getOutgoingEdges(model, port);

      // Restrict proxies to having at most one out-going edge.  If the given
      // edge is the same as the proxy port's out-going edge, that is fine.
      if ( out_edges == null || out_edges.length == 0 ||
           (out_edges.length == 1 && out_edges[0] == edge) )
      {
         try
         {
            DefaultPort proxy_port  = (DefaultPort) ((Edge) edge).getTarget();
            DefaultGraphCell proxy_cell =
               (DefaultGraphCell) proxy_port.getParent();
            ProxyInfo proxy_info = (ProxyInfo) proxy_cell.getUserObject();
            String proxy_type =
               proxy_info.getElement().getDefinition().getToken();

            java.util.List allowed_types =
               this.pointerPropertyDefinition.getAllowedTypes();
            accepts = allowed_types.contains(proxy_type);
         }
         // If we catch a ClassCastException at any point, then we are not
         // working with the cells and/or user objects that we expect.
         // Hence, the source cannot be accepted.
         catch (ClassCastException ex)
         {
            /* Oh well. */ ;
         }
      }

      return accepts;
   }

   /**
    * Verifies that the given port can be a target of the given edge within
    * the given model.  This always returns false because a port for an
    * element that points at a proxy cannot be a target for anything else.
    */
   public boolean acceptsTarget(GraphModel model, Object edge, Object port)
   {
      return false;
   }

   private PropertyDefinition pointerPropertyDefinition = null;
}
