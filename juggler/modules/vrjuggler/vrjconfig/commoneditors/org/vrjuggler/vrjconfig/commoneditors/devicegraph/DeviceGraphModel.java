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

import java.util.Iterator;
import java.util.List;

import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.Edge;

import org.vrjuggler.jccl.config.ConfigDefinition;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.PropertyDefinition;


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
    * Verifies that the given port can be a source of the given edge.  This
    * determination is made by testing to see if the parent of the potential
    * source holds a <code>ProxyInfo</code> object.  If it does, then the
    * parent of the target port (which must have parent containing a
    * <code>DeviceInfo</code> object) is used to verify that the chosen proxy
    * is allowed to point at the device unit.
    */
   public boolean acceptsSource(Object edge, Object port)
   {
      boolean accepts = false;

      if ( port != null )
      {
         Object[] out_edges = getOutgoingEdges(this, port);

         // Restrict proxies to having at most one out-going edge.  If the
         // given edge is the same as the proxy port's out-going edge, that
         // is fine.
         if ( out_edges == null || out_edges.length == 0 ||
              (out_edges.length == 1 && out_edges[0] == edge) )
         {
            try
            {
               DefaultPort proxy_port = (DefaultPort) port;
               DefaultPort unit_port  = (DefaultPort) ((Edge) edge).getTarget();
               DefaultGraphCell dev_cell =
                  (DefaultGraphCell) unit_port.getParent();
               DefaultGraphCell proxy_cell =
                  (DefaultGraphCell) proxy_port.getParent();

               accepts =
                  checkProxyDeviceConnection(
                     (ProxyInfo) proxy_cell.getUserObject(),
                     (DeviceInfo) dev_cell.getUserObject()
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
      }

      return accepts;
   }

   /**
    * Verifies that the given port can be a target of the given edge.  This
    * determination is made by testing to see if the parent of the potential
    * target holds a <code>DeviceInfo</code> object.  If it does, then the
    * parent of the source port (which must have parent containing a
    * <code>ProxyInfo</code> object) is used to verify that the proxy is
    * allowed to point at the chosen device unit.
    */
   public boolean acceptsTarget(Object edge, Object port)
   {
      boolean accepts = false;

      if ( port != null )
      {
         try
         {
            DefaultPort unit_port  = (DefaultPort) port;
            DefaultPort proxy_port = (DefaultPort) ((Edge) edge).getSource();
            DefaultGraphCell dev_cell =
               (DefaultGraphCell) unit_port.getParent();
            DefaultGraphCell proxy_cell =
               (DefaultGraphCell) proxy_port.getParent();

            accepts =
               checkProxyDeviceConnection(
                  (ProxyInfo) proxy_cell.getUserObject(),
                  (DeviceInfo) dev_cell.getUserObject()
               );
         }
         // If we catch a ClassCastException at any point, then we are not
         // working with the cells and/or user objects that we expect.  Hence,
         // the target cannot be accepted.
         catch (ClassCastException ex)
         {
            /* Oh well. */ ;
         }
      }

      return accepts;
   }

   private boolean checkProxyDeviceConnection(ProxyInfo proxyInfo,
                                              DeviceInfo deviceInfo)
   {
      boolean valid = false; 

      ConfigDefinition dev_def   = deviceInfo.getElement().getDefinition();
      ConfigDefinition proxy_def = proxyInfo.getElement().getDefinition();

      PropertyDefinition dev_prop_def =
         proxy_def.getPropertyDefinition(DEVICE_PROPERTY);

      List allowed_types = dev_prop_def.getAllowedTypes();

      if ( allowed_types.contains(dev_def.getToken()) )
      {
         valid = true;
      }
      else
      {
         for ( Iterator p = dev_def.getParents().iterator();
               p.hasNext(); )
         {
            if ( allowed_types.contains(p.next()) )
            {
               valid = true;
               break;
            }
         }
      }

      return valid;
   }
}
