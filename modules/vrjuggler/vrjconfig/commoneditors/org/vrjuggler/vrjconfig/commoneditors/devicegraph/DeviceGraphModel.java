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
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.DefaultGraphModel;

import org.vrjuggler.jccl.config.ConfigDefinition;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.PropertyDefinition;


public class DeviceGraphModel
   extends DefaultGraphModel
   implements org.vrjuggler.vrjconfig.commoneditors.EditorConstants
{
   /**
    * Verifies that the given port holds a PropertyDefinition (that of the
    * proxy's EditorConstants.DEVICE_PROPERTY property) and that the target's
    * ConfigDefinition (that of the device being proxied) is among that
    * property's allowed types.
    */
   public boolean acceptsSource(Object edge, Object port)
   {
      boolean accepts = false;

      System.out.println("Checking if port " + port +
                         " can be a source for edge " + edge);

      if ( port != null && port instanceof DefaultPort &&
           ((DefaultPort) port).getUserObject() instanceof PropertyDefinition )
      {
         DefaultPort cell = (DefaultPort) port;
         PropertyDefinition device_ptr_def =
            (PropertyDefinition) cell.getUserObject();
         List allowed_types = device_ptr_def.getAllowedTypes();

         ProxyPointerEdge proxy_edge = (ProxyPointerEdge) edge;
         ConfigDefinition device_def =
            proxy_edge.getDeviceElement().getDefinition();
         System.out.println("Checking if '" + device_def.getToken() +
                            "' is in " + allowed_types);
         accepts = allowed_types.contains(device_def.getToken());
         System.out.println("\tResult -> " + accepts);
      }

      return accepts;
   }

   /**
    * Verifies that the given port holds a ConfigDefinition object (that of
    * the device config element) that is among the allowed types for the
    * source's property definition.
    */
   public boolean acceptsTarget(Object edge, Object port)
   {
      boolean accepts = false;

      System.out.println("Checking if port " + port +
                         " can be a target for edge " + edge);

      if ( port != null && port instanceof DefaultPort &&
           ((DefaultPort) port).getUserObject() instanceof ConfigDefinition )
      {
         DefaultPort cell = (DefaultPort) port;
         ConfigDefinition new_device_def =
            (ConfigDefinition) cell.getUserObject();

         try
         {
            ProxyPointerEdge proxy_edge = (ProxyPointerEdge) edge;
            ConfigDefinition proxy_def =
               proxy_edge.getProxyElement().getDefinition();

            PropertyDefinition device_ptr_def =
               proxy_def.getPropertyDefinition(DEVICE_PROPERTY);
            List allowed_types = device_ptr_def.getAllowedTypes();

            System.out.println("Checking if '" + new_device_def.getToken() +
                               "' is in " + allowed_types);
            accepts = allowed_types.contains(new_device_def.getToken());
            System.out.println("\tResult -> " + accepts);
         }
         catch (IllegalArgumentException ex)
         {
            ex.printStackTrace();
            /* Oh well. */ ;
         }
      }

      return accepts;
   }
}
