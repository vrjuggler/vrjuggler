/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import java.util.List;

import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.Edge;
import org.jgraph.graph.GraphModel;

import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;


/**
 * A holder for ConfigElements that is used to provide run-time identification
 * of a ConfigElement reference for a device proxy configuration.  Proper use
 * of this type is critical for proper handling of ConfigElement references
 * held in a JGraph graph model.
 *
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.DeviceGraphLayoutCache
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.DeviceGraphModel
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.ProxiedDeviceCellViewFactory
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.ProxyPointerEdge
 */
public class ProxyInfo
   extends ConfigElementHolder
{
   /**
    * Creates a new container for the given ConfigElement reference for a
    * device proxy configuration.
    *
    * @param proxyElt   the config element for the device proxy
    * @param ctx        the context in which proxyElt is active
    * @param aliases    the full list of aliases (instances of
    *                   org.vrjuggler.jccl.config.ConfigElement) for proxyElt
    *                   in ctx
    */
   public ProxyInfo(ConfigElement proxyElt, ConfigContext ctx, List aliases)
   {
      super(proxyElt, ctx);

      if ( aliases != null )
      {
         this.aliases = aliases;
      }
   }

   /**
    * Verifies that the given port can be a source of the given edge within
    * the given model.  This determination is made by testing to see if the
    * parent of the target port (which must have parent containing a
    * <code>BaseDeviceInfo</code> object) is used to verify that the chosen
    * proxy is allowed to point at the device unit.
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
            DefaultPort unit_port  = (DefaultPort) ((Edge) edge).getTarget();
            DefaultGraphCell dev_cell =
               (DefaultGraphCell) unit_port.getParent();

            accepts =
               GraphHelpers.checkProxyDeviceConnection(
                  this, (BaseDeviceInfo) dev_cell.getUserObject(),
                  (UnitInfo) unit_port.getUserObject()
               );
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
    * the given model.  This always returns false because a proxy cell cannot
    * be a target.
    */
   public boolean acceptsTarget(GraphModel model, Object edge, Object port)
   {
      return false;
   }

   /**
    * Returns the list of aliases for the device proxy config element held by
    * this object.  This will never return null.
    *
    * @return A non-null reference to a java.util.List instance.  The list
    *         contains instances of org.vrjuggler.jccl.config.ConfigElement.
    */
   public List getAliases()
   {
      return aliases;
   }

   public void addAlias(ConfigElement aliasElt)
   {
      aliases.add(aliasElt);
   }

   public void removeAlias(ConfigElement aliasElt)
   {
      aliases.remove(aliasElt);
   }

   private List aliases = new java.util.ArrayList();
}
