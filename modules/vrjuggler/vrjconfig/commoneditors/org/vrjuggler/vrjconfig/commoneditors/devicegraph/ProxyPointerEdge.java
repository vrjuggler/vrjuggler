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


public class ProxyPointerEdge
   extends DefaultEdge
   implements org.vrjuggler.vrjconfig.commoneditors.EditorConstants
{
   public ProxyPointerEdge(Object userObject)
   {
      super(userObject);
   }

   public ConfigElement getProxyElement()
   {
      Object port = getSource();
      ConfigElement proxy_elt = null;

      if ( port != null && port instanceof DefaultPort )
      {
         DefaultPort def_port = (DefaultPort) port;
         DefaultGraphCell parent = (DefaultGraphCell) def_port.getParent();

         if ( parent.getUserObject() instanceof ProxyInfo )
         {
            proxy_elt =
               (ConfigElement) ((ProxyInfo) parent.getUserObject()).getElement();
         }
      }

      return proxy_elt;
   }

   public ConfigElement getDeviceElement()
   {
      Object port = getTarget();
      ConfigElement dev_elt = null;

      if ( port != null && port instanceof DefaultPort )
      {
         DefaultPort def_port = (DefaultPort) port;
         DefaultGraphCell parent = (DefaultGraphCell) def_port.getParent();

         if ( parent.getUserObject() instanceof DeviceInfo )
         {
            dev_elt = ((DeviceInfo) parent.getUserObject()).getElement();
         }
      }

      return dev_elt;
   }

   public void setSource(Object port)
   {
      if ( port == null )
      {
         super.setSource(port);
      }
      else if ( port instanceof DefaultPort )
      {
         DefaultPort def_port = (DefaultPort) port;
         DefaultGraphCell parent = (DefaultGraphCell) def_port.getParent();

         // The new source is a graph cell holding a ConfigElement for a
         // proxy.  This means that the target for this edge should be a
         // graph cell holding a ConfigElement for a device.
         if ( parent.getUserObject() instanceof ProxyInfo )
         {
            Object target_obj = getTarget();

            // If the target has not been set yet, there is nothing to do.
            // We just set the source that was provided.
            if ( target_obj != null )
            {
               super.setSource(port);
            }
            else
            {
               DefaultPort target = (DefaultPort) target_obj;
               DefaultGraphCell target_parent =
                  (DefaultGraphCell) target.getParent();

               if ( ! (target_parent.getUserObject() instanceof DeviceInfo) )
               {
                  throw new IllegalArgumentException("Invalid type for " +
                                                     "target's parent user " +
                                                     "object -> " +
                                                     target_parent.getUserObject().getClass());
               }

               // At this point, we have verified the correctness of the
               // source, so we can set port to be our new source.
               super.setSource(port);

               DeviceInfo dev_info = (DeviceInfo) target_parent.getUserObject();
               ProxyInfo  proxy_info = (ProxyInfo) parent.getUserObject();

               ConfigElement proxy_elt = proxy_info.getElement();
               ConfigElement dev_elt   = dev_info.getElement();

               ConfigElementPointer dev_ptr =
                  (ConfigElementPointer) proxy_elt.getProperty(DEVICE_PROPERTY, 0);

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
         else
         {
            throw new IllegalArgumentException("Invalid type for parent " +
                                               "user object -> " +
                                               parent.getUserObject().getClass());
         }
      }
      else
      {
         throw new IllegalArgumentException("Invalid type for source port: " +
                                            port.getClass());
      }
   }

   /**
    * Sets the target for this edge.  If the source is the port of a proxy
    * ConfigElement, then the target must refer to a port of a graph cell that
    * holds a ConfigElement reference.  The port of that cell corresponds with
    * an input source for the ConfigElement.  The ConfigElement of the target
    * must be a type that the source proxy is allowed to point to.
    */
   public void setTarget(Object port)
   {
      if ( port == null )
      {
         super.setTarget(port);
      }
      else if ( port instanceof DefaultPort )
      {
         DefaultPort def_port = (DefaultPort) port;
         DefaultGraphCell parent = (DefaultGraphCell) def_port.getParent();

         // The new target is a graph cell holding a ConfigElement for a
         // device.  This means that the source for this edge should be a
         // graph cell holding a ConfigElement for a proxy.
         if ( parent.getUserObject() instanceof DeviceInfo )
         {
            Object source_obj = getSource();

            // If the source has not been set yet, there is nothing to do.
            // We just set the target that was provided.
            if ( source_obj == null )
            {
               super.setTarget(port);
            }
            else
            {
               DefaultPort source = (DefaultPort) source_obj;
               DefaultGraphCell source_parent =
                  (DefaultGraphCell) source.getParent();

               if ( ! (source_parent.getUserObject() instanceof ProxyInfo) )
               {
                  throw new IllegalArgumentException("Invalid type for " +
                                                     "source's parent user " +
                                                     "object -> " +
                                                     source_parent.getUserObject().getClass());
               }

               // At this point, we have verified the correctness of the target,
               // so we can set port to be our new target.
               super.setTarget(port);

               DeviceInfo dev_info   = (DeviceInfo) parent.getUserObject();
               ProxyInfo  proxy_info = (ProxyInfo) source_parent.getUserObject();

               ConfigElement proxy_elt = proxy_info.getElement();
               ConfigElement dev_elt   = dev_info.getElement();

               ConfigElementPointer dev_ptr =
                  (ConfigElementPointer) proxy_elt.getProperty(DEVICE_PROPERTY, 0);

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
/*
         else if ( parent.getUsreObject() instanceof ProxyInfo )
         {
            ProxyInfo dev_info = (ProxyInfo) parent.getUserObject();
            super.setTarget(port);
         }
*/
         else
         {
            throw new IllegalArgumentException("Invalid type for parent " +
                                               "user object -> " +
                                               parent.getUserObject().getClass());
         }
      }
      else
      {
         throw new IllegalArgumentException("Invalid type for target port: " +
                                            port.getClass());
      }
   }
}
