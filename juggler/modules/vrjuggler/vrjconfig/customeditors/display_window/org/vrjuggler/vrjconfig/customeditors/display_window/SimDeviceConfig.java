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

package org.vrjuggler.vrjconfig.customeditors.display_window;

import java.util.Iterator;
import java.util.List;

import org.vrjuggler.jccl.config.*;


public class SimDeviceConfig
   implements EditorConstants
{
   public SimDeviceConfig(ConfigContext ctx, ConfigElement simDeviceElt)
   {
      device = simDeviceElt;

      findProxy(ctx);

      if ( proxy == null )
      {
         System.out.println("No proxy found for " + device.getName());
      }
      else
      {
         findAlias(ctx);

         if ( alias == null )
         {
            System.out.println("No alias found for " + proxy.getName());
         }
      }
   }

   public SimDeviceConfig(ConfigContext ctx, ConfigElement simDeviceElt,
                          ConfigElement proxyElt)
   {
      device = simDeviceElt;
      proxy  = proxyElt;
      findAlias(ctx);

      if ( alias == null )
      {
         System.out.println("No alias found for " + proxy.getName());
      }
   }

   public SimDeviceConfig(ConfigContext ctx, ConfigElement simDeviceElt,
                          ConfigElement proxyElt, ConfigElement aliasElt)
   {
      ConfigDefinition def = aliasElt.getDefinition();
      if ( ! def.getToken().equals(ALIAS_TYPE) )
      {
         throw new IllegalArgumentException("Invalid alias element type '" +
                                            def.getToken() + "'" +
                                            " (expected " + ALIAS_TYPE + ")");
      }

      device = simDeviceElt;
      proxy  = proxyElt;
      alias  = aliasElt;
   }

   public ConfigElement getAlias()
   {
      return alias;
   }

   public ConfigElement getProxy()
   {
      return proxy;
   }

   public ConfigElement getDevice()
   {
      return device;
   }

   public String toString()
   {
      String str;

      if ( alias != null )
      {
         str = alias.getName() + " \u2192 " + device.getName();
      }
      else if ( proxy != null )
      {
         str = proxy.getName() + " \u2192 " + device.getName();
      }
      else
      {
         str = device.getName();
      }

      return str;
   }

   private void findProxy(ConfigContext ctx)
   {
      boolean found_proxy = false;
      ConfigBroker broker = new ConfigBrokerProxy();
      List elts = broker.getElements(ctx);

      for ( Iterator i = elts.iterator(); i.hasNext(); )
      {
         ConfigElement cur_elt = (ConfigElement) i.next();
         List parents = cur_elt.getDefinition().getParents();

         for ( Iterator j = parents.iterator(); j.hasNext(); )
         {
            String p = (String) j.next();
            if ( p.equals(PROXY_TYPE) )
            {
               ConfigElementPointer dev_ptr =
                  (ConfigElementPointer) cur_elt.getProperty(DEVICE_PROPERTY, 0);

               if ( dev_ptr.getTarget() != null &&
                    dev_ptr.getTarget().equals(device.getName()) )
               {
                  proxy = cur_elt;
                  found_proxy = true;
                  break;
               }
            }
         }

         if ( found_proxy )
         {
            break;
         }
      }
   }

   private void findAlias(ConfigContext ctx)
   {
      ConfigBroker broker = new ConfigBrokerProxy();
      List elts = broker.getElements(ctx);

      for ( Iterator i = elts.iterator(); i.hasNext(); )
      {
         ConfigElement cur_elt = (ConfigElement) i.next();
         if ( cur_elt.getDefinition().getToken().equals(ALIAS_TYPE) )
         {
            ConfigElementPointer proxy_ptr =
               (ConfigElementPointer) cur_elt.getProperty("proxy", 0);

            if ( proxy_ptr.getTarget() != null &&
                 proxy_ptr.getTarget().equals(proxy.getName()) )
            {
               alias = cur_elt;
               break;
            }
         }
      }
   }

   private ConfigElement alias  = null;
   private ConfigElement proxy  = null;
   private ConfigElement device = null;
}
