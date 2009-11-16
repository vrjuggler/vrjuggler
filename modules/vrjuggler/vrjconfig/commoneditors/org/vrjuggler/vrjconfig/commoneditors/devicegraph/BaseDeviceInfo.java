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
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.Edge;
import org.jgraph.graph.GraphModel;

import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;


/**
 * A place holder type for all graph user objects associated with devices
 * of any kind.  Such object types must derive from this class in order for
 * proper recognition by <code>ProxyInfo</code> instances.
 *
 * @see ProxyInfo
 */
public abstract class BaseDeviceInfo
   extends ConfigElementHolder
{
   protected BaseDeviceInfo(ConfigElement elt, ConfigContext ctx)
   {
      super(elt, ctx);
   }

   /**
    * Verifies that the given port can be a source of the given edge within
    * the given model.  This always returns false because a device cannot be
    * a source (typically).
    */
   public boolean acceptsSource(GraphModel model, Object edge, Object port)
   {
      return false;
   }

   /**
    * Verifies that the given port can be a target of the given edge within
    * the given model.  This determination is made by testing to see if the
    * parent of the source port (which must have parent containing a
    * <code>ProxyInfo</code> object) is used to verify that the proxy is
    * allowed to point at the chosen device unit.
    */
   public boolean acceptsTarget(GraphModel model, Object edge, Object port)
   {
      boolean accepts = false;

      try
      {
         DefaultPort unit_port  = (DefaultPort) port;
         DefaultPort proxy_port = (DefaultPort) ((Edge) edge).getSource();
         DefaultGraphCell proxy_cell =
            (DefaultGraphCell) proxy_port.getParent();

         accepts =
            GraphHelpers.checkProxyDeviceConnection(
               (ProxyInfo) proxy_cell.getUserObject(), this,
               (UnitInfo) unit_port.getUserObject()
            );
      }
      // If we catch a ClassCastException at any point, then we are not
      // working with the cells and/or user objects that we expect.  Hence,
      // the target cannot be accepted.
      catch (ClassCastException ex)
      {
         /* Oh well. */ ;
      }

      return accepts;
   }
}
