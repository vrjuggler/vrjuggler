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

import org.jgraph.graph.DefaultEdge;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultPort;

import org.vrjuggler.jccl.config.ConfigDefinition;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.ConfigElementPointer;
import org.vrjuggler.jccl.config.PropertyDefinition;


/**
 * A special-purpose edge that is used for the connection between a proxy
 * (source) and a device (target).  The validity of source/target connections
 * through the user interface is verified by <code>DeviceGraphModel</code>,
 * but sources and targets can still be set programatically using the
 * <code>org.jgraph.graph.Edge</code> interface.
 *
 * @see DeviceGraphModel
 */
public class ProxyToDeviceEdge
   extends DefaultEdge
   implements org.vrjuggler.vrjconfig.commoneditors.EditorConstants
{
   public ProxyToDeviceEdge(Object userObject)
   {
      super(userObject);
   }

   /**
    * Sets the source for this edge.  If the target is the port of a vertex
    * containing a <code>BaseDeviceInfo</code> object, then the source must
    * refer to a port of a vertex that holds a <code>ProxyInfo</code> object.
    * The <code>ConfigElement</code> of the target must be a type that the
    * source proxy is allowed to point to.
    *
    * @see DeviceGraphModel#acceptsSource(Object,Object)
    *
    * @throws InvalidArgumentException
    *   thrown if the given source port (when not null) is for a vertex not
    *   contaning a <code>ProxyInfo</code> object or if the target (when not
    *   null) is not for a vertex containing a <code>BaseDeviceInfo</code>
    *   object.
    */
   public void setSource(Object port)
   {
      // If there is no change to the source, we cannot do anything.
      if ( getSource() == port )
      {
         super.setSource(port);
      }
      else if ( port == null )
      {
         // Use the old, non-null source to update the proxy that no longer
         // points to anything.
         clearOldProxy((DefaultPort) getSource());
         super.setSource(port);
      }
      // Otherwise, we need to perform updates on all proxies involved.
      else
      {
         try
         {
            // Save the old source for later.  This may or may not be null.
            DefaultPort old_source = (DefaultPort) getSource();

            DefaultPort proxy_port = (DefaultPort) port;
            DefaultGraphCell proxy_cell =
               (DefaultGraphCell) proxy_port.getParent();

            // The new source is a graph cell holding a ProxyInfo object for a
            // proxy.  This means that the target for this edge should be a
            // graph cell holding a BaseDeviceInfo for a device.
            if ( ! (proxy_cell.getUserObject() instanceof ProxyInfo) )
            {
               throw new IllegalArgumentException(
                  "Invalid type for parent user object -> " +
                  proxy_cell.getUserObject().getClass()
               );
            }

            ProxyInfo proxy_info = (ProxyInfo) proxy_cell.getUserObject();
            ConfigElement proxy_elt = proxy_info.getElement();

            // At this point, we have verified the correctness of the source,
            // so we can set port to be our new source.
            super.setSource(port);

            // If the old source for this edge is non-null, then we need to
            // update the old proxy so that it points at nothing.
            if ( old_source != null )
            {
               clearOldProxy(old_source);
            }

            // Get the device (target) information.
            Object dev_port_obj = getTarget();

            // If the device is null, then we need to set our proxy's
            // DEVICE_PROPERTY to point to nothing.
            if ( dev_port_obj == null )
            {
               proxy_elt.setProperty(DEVICE_PROPERTY, 0, "",
                                     proxy_info.getContext());
            }
            else
            {
               DefaultPort dev_port = (DefaultPort) dev_port_obj;
               DefaultGraphCell dev_cell =
                  (DefaultGraphCell) dev_port.getParent();

               if ( ! (dev_cell.getUserObject() instanceof BaseDeviceInfo) )
               {
                  throw new IllegalArgumentException(
                     "Invalid type for target's parent user object -> " +
                     dev_cell.getUserObject().getClass()
                  );
               }

               BaseDeviceInfo dev_info =
                  (BaseDeviceInfo) dev_cell.getUserObject();
               ConfigElement dev_elt = dev_info.getElement();

               ConfigElementPointer dev_ptr =
                  (ConfigElementPointer) proxy_elt.getProperty(DEVICE_PROPERTY,
                                                               0);

               // If the property value is null or it has a target other than
               // dev_elt, update it.  Otherwise, we just leave it as is.
               if ( dev_ptr == null ||
                    ! dev_ptr.getTarget().equals(dev_elt.getName()) )
               {
                  proxy_elt.setProperty(DEVICE_PROPERTY, 0, dev_elt.getName(),
                                        proxy_info.getContext());
               }
            }
         }
         catch (ClassCastException ex)
         {
            throw new IllegalArgumentException("Invalid type for port: " +
                                               port.getClass());
         }
      }
   }

   /**
    * Sets the target for this edge.  If the source is the port of a vertex
    * containing a <code>ProxyInfo</code> object, then the target must refer
    * to a port of a vertex that holds a <code>BaseDeviceInfo</code> object.
    * The port of that cell corresponds with an input source for the device.
    * The <code>ConfigElement</code> of the target must be a type that the
    * source proxy is allowed to point to.
    *
    * @see DeviceGraphModel#acceptsTarget(Object,Object)
    *
    * @throws InvalidArgumentException
    *   thrown if the given target port (when not null) is for a vertex not
    *   contaning a <code>BaseDeviceInfo</code> object or if the source (when
    *   not null) is not for a vertex containing a <code>ProxyInfo</code>
    *   object.
    */
   public void setTarget(Object port)
   {
      // If there is no change to the source, we cannot do anything.
      if ( getTarget() == port )
      {
         super.setTarget(port);
      }
      // Otherwise, we need to perform updates on all proxies involved.
      else
      {
         try
         {
            // Get the proxy (source) information.
            Object proxy_port_obj = getSource();

            if ( proxy_port_obj != null )
            {
               DefaultPort dev_port = (DefaultPort) port;

               DefaultPort proxy_port = (DefaultPort) proxy_port_obj;
               DefaultGraphCell proxy_cell =
                  (DefaultGraphCell) proxy_port.getParent();

               if ( ! (proxy_cell.getUserObject() instanceof ProxyInfo) )
               {
                  throw new IllegalArgumentException(
                     "Invalid type for source's parent user object -> " +
                     proxy_cell.getUserObject().getClass()
                  );
               }

               ProxyInfo proxy_info    = (ProxyInfo) proxy_cell.getUserObject();
               ConfigElement proxy_elt = proxy_info.getElement();

               if ( dev_port == null )
               {
                  proxy_elt.setProperty(DEVICE_PROPERTY, 0, "",
                                        proxy_info.getContext());
               }
               else
               {
                  DefaultGraphCell dev_cell =
                     (DefaultGraphCell) dev_port.getParent();

                  // The new target is a graph cell holding a BaseDeviceInfo
                  // object for a device.  This means that the source for this
                  // edge should be a graph cell holding a ProxyInfo object
                  // for a proxy.
                  if ( ! (dev_cell.getUserObject() instanceof BaseDeviceInfo) )
                  {
                     throw new IllegalArgumentException(
                        "Invalid type for parent user object -> " +
                        dev_cell.getUserObject().getClass()
                     );
                  }

                  BaseDeviceInfo dev_info =
                     (BaseDeviceInfo) dev_cell.getUserObject();
                  UnitInfo unit_info    = (UnitInfo) dev_port.getUserObject();
                  ConfigElement dev_elt = dev_info.getElement();
                  proxy_elt.setProperty(DEVICE_PROPERTY, 0, dev_elt.getName(),
                                        proxy_info.getContext());

                  // Aggrivatingly enough, not all proxy types have a property
                  // EditorConstants.UNIT_PROPERTY.
                  try
                  {
                     proxy_elt.setProperty(UNIT_PROPERTY, 0,
                                           unit_info.getUnitNumber(),
                                           proxy_info.getContext());
                  }
                  catch (IllegalArgumentException ex)
                  {
                  }
               }
            }

            // At this point, we have verified the correctness of the target,
            // so we can set port to be our new target.
            super.setTarget(port);
         }
         catch (ClassCastException ex)
         {
            throw new IllegalArgumentException("Invalid type for port: " +
                                               port.getClass());
         }
      }
   }

   private void clearOldProxy(DefaultPort oldSource)
   {
      DefaultGraphCell old_proxy_cell =
         (DefaultGraphCell) oldSource.getParent();
      ProxyInfo old_proxy_info = (ProxyInfo) old_proxy_cell.getUserObject();
      ConfigElement old_proxy_elt = old_proxy_info.getElement();
      old_proxy_elt.setProperty(DEVICE_PROPERTY, 0, "",
                                old_proxy_info.getContext());
   }
}
