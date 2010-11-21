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

import org.jgraph.graph.DefaultEdge;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultPort;

import org.vrjuggler.jccl.config.ConfigDefinition;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.ConfigElementPointer;
import org.vrjuggler.jccl.config.PropertyDefinition;


/**
 * A special-purpose edge that is used for the connection between a relative
 * device (source) and a device proxy (target).  The validity of source/target
 * connections through the user interface is verified by
 * <code>DeviceGraphModel</code>, but sources and targets can still be set
 * programatically using the <code>org.jgraph.graph.Edge</code> interface.
 *
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.DeviceGraphModel
 */
public class RelativeDeviceToProxyEdge
   extends DefaultEdge
   implements org.vrjuggler.vrjconfig.commoneditors.EditorConstants
{
   public RelativeDeviceToProxyEdge()
   {
      super();
   }

   public RelativeDeviceToProxyEdge(Object userObject)
   {
      super(userObject);
   }

   /**
    * Sets the source for this edge.  If the target is the port of a vertex
    * containing a <code>ProxyInfo</code> object, then the source must refer
    * to a port that holds a <code>ProxyPointerInfo</code> object.  The
    * <code>ConfigElement</code> of the target must be a type that the source
    * relative device is allowed to point to.
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.DeviceGraphModel#acceptsSource(Object,Object)
    *
    * @throws InvalidArgumentException
    *   thrown if the given source port (when not null) does not contain a
    *   <code>ProxyPointerInfo</code> object or if the target (when not null)
    *   is not for a vertex containing a <code>ProxyInfo</code> object.
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
         // Use the old, non-null source to update the relative proxy pointer
         // that no longer points to anything.
         DefaultPort old_source = (DefaultPort) getSource();
         ProxyPointerInfo ptr_info =
            (ProxyPointerInfo) old_source.getUserObject();
         String prop_name = ptr_info.getPointerPropertyDefinition().getToken();
         ptr_info.getElement().setProperty(prop_name, 0, "",
                                           ptr_info.getContext());
         super.setSource(port);
      }
      // Otherwise, we need to perform updates on all relative devices
      // involved.
      else
      {
         try
         {
            // Save the old source for later.  This may or may not be null.
            DefaultPort old_source = (DefaultPort) getSource();

            DefaultPort relative_dev_port = (DefaultPort) port;
            ProxyPointerInfo ptr_info =
               (ProxyPointerInfo) relative_dev_port.getUserObject();
            ConfigElement ptr_elt = ptr_info.getElement();
            String ptr_prop_name =
               ptr_info.getPointerPropertyDefinition().getToken();

            // At this point, we have verified the correctness of the source,
            // so we can set port to be our new source.
            super.setSource(port);

            // Get the proxy (target) information.
            Object proxy_port_obj = getTarget();

            // If the proxy is null, then we need to set our proxy pointer to
            // point to nothing.
            if ( proxy_port_obj == null )
            {
               ConfigElementPointer cur_val =
                  (ConfigElementPointer) ptr_elt.getProperty(ptr_prop_name, 0);

               // Do not change the property value unless the new value is
               // different than the current value.
               if ( ! cur_val.getTarget().equals("") )
               {
                  ptr_elt.setProperty(ptr_prop_name, 0, "",
                                      ptr_info.getContext());
               }
            }
            else
            {
               DefaultPort proxy_port = (DefaultPort) proxy_port_obj;
               DefaultGraphCell proxy_cell =
                  (DefaultGraphCell) proxy_port.getParent();

               if ( ! (proxy_cell.getUserObject() instanceof ProxyInfo) )
               {
                  throw new IllegalArgumentException(
                     "Invalid type for target's parent user object -> " +
                     proxy_cell.getUserObject().getClass()
                  );
               }

               ProxyInfo proxy_info = (ProxyInfo) proxy_cell.getUserObject();
               ConfigElement proxy_elt = proxy_info.getElement();

               ConfigElementPointer cur_ptr_val =
                  (ConfigElementPointer) ptr_elt.getProperty(ptr_prop_name, 0);

               // If the property value is null or it has a target other than
               // proxy_elt, update it.  Otherwise, we just leave it as is.
               if ( cur_ptr_val == null ||
                    ! cur_ptr_val.getTarget().equals(proxy_elt.getName()) )
               {
                  ptr_elt.setProperty(ptr_prop_name, 0, proxy_elt.getName(),
                                      ptr_info.getContext());
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
    * Sets the target for this edge.  If the source is a port containing a
    * <code>ProxyPointerInfo</code> object, then the target must refer to a
    * port of a vertex that holds a <code>ProxyInfo</code> object.  The
    * <code>ConfigElement</code> of the target must be a type that the source
    * relative device is allowed to point to.
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.DeviceGraphModel#acceptsTarget(Object,Object)
    *
    * @throws InvalidArgumentException
    *   thrown if the given target port (when not null) is for a vertex not
    *   contaning a <code>ProxyInfo</code> object or if the source (when
    *   not null) does not contain a <code>ProxyPointerInfo</code> object.
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
            Object relative_dev_port_obj = getSource();

            if ( relative_dev_port_obj != null )
            {
               DefaultPort proxy_port = (DefaultPort) port;

               DefaultPort relative_dev_port =
                  (DefaultPort) relative_dev_port_obj;
               ProxyPointerInfo ptr_info =
                  (ProxyPointerInfo) relative_dev_port.getUserObject();
               ConfigElement ptr_elt = ptr_info.getElement();
               String ptr_prop_name =
                  ptr_info.getPointerPropertyDefinition().getToken();

               if ( proxy_port == null )
               {
                  ConfigElementPointer cur_val =
                     (ConfigElementPointer) ptr_elt.getProperty(ptr_prop_name,
                                                                0);

                  // Do not change the property value unless the new value is
                  // different than the current value.
                  if ( ! cur_val.getTarget().equals("") )
                  {
                     ptr_elt.setProperty(ptr_prop_name, 0, "",
                                         ptr_info.getContext());
                  }
               }
               else
               {
                  DefaultGraphCell proxy_cell =
                     (DefaultGraphCell) proxy_port.getParent();

                  // The new target is a graph cell holding a ProxyInfo object
                  // for a device proxy.  This means that the source for this
                  // edge should be a graph cell holding a RelativeDeviceInfo
                  // object for a relative device.
                  if ( ! (proxy_cell.getUserObject() instanceof ProxyInfo) )
                  {
                     throw new IllegalArgumentException(
                        "Invalid type for parent user object -> " +
                        proxy_cell.getUserObject().getClass()
                     );
                  }

                  ProxyInfo proxy_info = (ProxyInfo) proxy_cell.getUserObject();
                  ConfigElement proxy_elt = proxy_info.getElement();

                  ConfigElementPointer cur_ptr_val =
                     (ConfigElementPointer) ptr_elt.getProperty(ptr_prop_name,
                                                                0);

                  // Do not change the property value unless the new value is
                  // different than the current value.
                  if ( ! cur_ptr_val.getTarget().equals(proxy_elt.getName()) )
                  {
                     ptr_elt.setProperty(ptr_prop_name, 0, proxy_elt.getName(),
                                         ptr_info.getContext());
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
}
