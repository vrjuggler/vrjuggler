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

package org.vrjuggler.vrjconfig.commoneditors;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.vrjuggler.jccl.config.*;


/**
 * This is a helper class designed to collect together all the config elements
 * associated with a given config element for a Juggler device type.  It
 * finds all the proxies pointing at the given config element and all the
 * aliases that refer to those proxies.
 */
public class DeviceConfig
   implements EditorConstants
{
   /**
    * Simple constructor for a device type's config element.  This looks in
    * the context for all proxies pointing at the given device's config element
    * and all aliases referring to those discovered proxies.
    *
    * @param ctx        the context where the config element is used
    * @param deviceElt  the config element for a Juggler device type
    */
   public DeviceConfig(ConfigContext ctx, ConfigElement deviceElt)
   {
      device = deviceElt;

      findProxies(ctx);

      if ( proxies.isEmpty() )
      {
         System.out.println("No proxy found for " + device.getName());
      }
      else
      {
         findAliases(ctx);
      }
   }

   /**
    * Simple constructor for a device type's config element.  This looks in
    * the context for all proxies of then given type pointing at the given
    * device's config element and all aliases referring to those discovered
    * proxies.  For device types that allow multiple proxy types to be used,
    * use the constructor overload that performs an untyped proxy search.
    *
    * @param ctx                the context where the config element is used
    * @param deviceElt          the config element for a Juggler device type
    * @param proxyTypeName      the name of the proxy type that can point at
    *                           <code>deviceElt</code>
    *
    * @see #DeviceConfig(ConfigContext,ConfigElement)
    */
   public DeviceConfig(ConfigContext ctx, ConfigElement deviceElt,
                       String proxyTypeName)
   {
      device = deviceElt;

      findTypedProxies(ctx, proxyTypeName);

      if ( proxies.isEmpty() )
      {
         System.out.println("No proxy found for " + device.getName());
      }
      else
      {
         findAliases(ctx);
      }
   }

   /**
    * Simple constructor for a device type's config element.  This looks in
    * the context for all proxies of then given type pointing at the given
    * device's config element and all aliases referring to those discovered
    * proxies.  For device types that allow multiple proxy types to be used,
    * use the constructor overload that performs an untyped proxy search.
    *
    * @param ctx        the context where the config element is used
    * @param deviceElt  the config element for a Juggler device type
    * @param proxyType  the config definition for the proxy type that can
    *                   point at <code>deviceElt</code>
    *
    * @see #DeviceConfig(ConfigContext,ConfigElement)
    */
   public DeviceConfig(ConfigContext ctx, ConfigElement deviceElt,
                       ConfigDefinition proxyType)
   {
      device = deviceElt;

      findTypedProxies(ctx, proxyType);

      if ( proxies.isEmpty() )
      {
         System.out.println("No proxy found for " + device.getName());
      }
      else
      {
         findAliases(ctx);
      }
   }

   /**
    * Device + proxy list constructor.  This constructor assumes that the
    * given list of proxy config elements is both correct (the proxies
    * refer to the given input device config element) and complete (the
    * proxies in the list are all those proxies in the context that refer
    * to the given input device config element).  This looks in the context
    * for all aliases referring to the given list of proxies.
    *
    * @param ctx        the context where the config element is used
    * @param deviceElt  the config element for a Juggler device type
    * @param proxyElts  the complete list of proxy config elements in
    *                   <code>ctx</code> that point to <code>deviceElt</code>
    */
   public DeviceConfig(ConfigContext ctx, ConfigElement deviceElt,
                       List proxyElts)
   {
      device  = deviceElt;
      proxies = proxyElts;
      findAliases(ctx);
   }

   /**
    * Device + proxy list constructor.  This constructor assumes that the
    * given list of proxy config elements is both correct (the proxies
    * refer to the given input device config element) and complete (the
    * proxies in the list are all those proxies in the context that refer
    * to the given input device config element).  This looks in the context
    * for all aliases referring to the given list of proxies.
    *
    * @param ctx        the context where the config element is used
    * @param deviceElt  the config element for a Juggler device type
    * @param proxyElts  the complete array of proxy config elements in
    *                   <code>ctx</code> that point to <code>deviceElt</code>
    */
   public DeviceConfig(ConfigContext ctx, ConfigElement deviceElt,
                       ConfigElement[] proxyElts)
   {
      device = deviceElt;

      for ( int i = 0; i < proxyElts.length; ++i )
      {
         proxies.add(proxyElts[i]);
      }

      findAliases(ctx);
   }

   /**
    * Device + proxy list + alias list constructor.  This constructor assumes
    * that the given lists of proxy and alias config elements are correct
    * (the proxies refer to the given input device config element and the
    * aliases refer to the given list of proxies) and complete (the proxies in
    * the list are all those proxies in the context that refer to the given
    * input device config element and the aliases are all those in the context
    * that refer to those proxies).
    *
    * @param ctx        the context where the config element is used
    * @param deviceElt  the config element for a Juggler device type
    * @param proxyElts  the complete list of proxy config elements in
    *                   <code>ctx</code> that point to <code>deviceElt</code>
    * @param aliasElts  the complete list of alias config elements in
    *                   <code>ctx</code> that refer to the proxies in
    *                   <code>proxyElts</code>
    */
   public DeviceConfig(ConfigContext ctx, ConfigElement deviceElt,
                       List proxyElts, List aliasElts)
   {
/*
      ConfigDefinition def = aliasElt.getDefinition();
      if ( ! def.getToken().equals(ALIAS_TYPE) )
      {
         throw new IllegalArgumentException("Invalid alias element type '" +
                                            def.getToken() + "'" +
                                            " (expected " + ALIAS_TYPE + ")");
      }
*/
      device  = deviceElt;
      proxies = proxyElts;
      fillAliasMap(aliasElts);
   }

   /**
    * Device + proxy list + alias list constructor.  This constructor assumes
    * that the given lists of proxy and alias config elements are correct
    * (the proxies refer to the given input device config element and the
    * aliases refer to the given list of proxies) and complete (the proxies in
    * the list are all those proxies in the context that refer to the given
    * input device config element and the aliases are all those in the context
    * that refer to those proxies).
    *
    * @param ctx        the context where the config element is used
    * @param deviceElt  the config element for a Juggler device type
    * @param proxyElts  the complete array of proxy config elements in
    *                   <code>ctx</code> that point to simDeviceElt
    * @param aliasElts  the complete array of alias config elements in
    *                   <code>ctx</code> that refer to the proxies in
    *                   <code>proxyElts</code>
    */
   public DeviceConfig(ConfigContext ctx, ConfigElement deviceElt,
                       ConfigElement[] proxyElts, ConfigElement[] aliasElts)
   {
      device = deviceElt;

      for ( int i = 0; i < proxyElts.length; ++i )
      {
         proxies.add(proxyElts[i]);
      }

      List alias_list = new ArrayList();
      for ( int i = 0; i < aliasElts.length; ++i )
      {
         alias_list.add(aliasElts[i]);
      }

      fillAliasMap(alias_list);
   }

   /**
    * Returns the list of aliases referring to this device's proxy config
    * elements.
    */
   public List getAliases()
   {
      List alias_list = new ArrayList();

      // Values is a Collection of List objects.
      Collection values = aliases.values();

      for ( Iterator i = values.iterator(); i.hasNext(); )
      {
         alias_list.addAll((List) i.next());
      }

      return alias_list;
   }

   /**
    * Returns the list of aliases referring to the given proxy config element.
    * The proxy config element should be in the List returned by getProxies().
    * If it is not, then null is guaranteed to be returned.
    */
   public List getAliases(ConfigElement proxyElt)
   {
      return (List) aliases.get(proxyElt);
   }

   /**
    * Returns the list of proxies referring to this device's config element.
    */
   public List getProxies()
   {
      return proxies;
   }

   /**
    * Returns the config element for this device's configuration.
    */
   public ConfigElement getDevice()
   {
      return device;
   }

   /**
    * Presents a string representation of this device's configuration.
    */
   public String toString()
   {
      return device.getName();
   }

   /**
    * Searches the context for any proxy that points to our config element.
    * Proxy config elements are determined by checking the element's config
    * definition for a parent of type EditorConstants.PROXY_TYPE.  This is a
    * slow search, but it is exhaustive.
    */
   private void findProxies(ConfigContext ctx)
   {
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
                  proxies.add(cur_elt);
               }
            }
         }
      }
   }

   private void findTypedProxies(ConfigContext ctx, String proxyTypeName)
   {
      ConfigBroker broker = new ConfigBrokerProxy();
      findTypedProxies(ctx, broker.getRepository().get(proxyTypeName));
   }

   private void findTypedProxies(ConfigContext ctx, ConfigDefinition proxyType)
   {
      ConfigBroker broker = new ConfigBrokerProxy();
      List proxy_elts =
         ConfigUtilities.getElementsWithDefinition(broker.getElements(ctx),
                                                   proxyType);

      for ( Iterator i = proxy_elts.iterator(); i.hasNext(); )
      {
         ConfigElement cur_elt = (ConfigElement) i.next();
         ConfigElementPointer dev_ptr =
            (ConfigElementPointer) cur_elt.getProperty(DEVICE_PROPERTY, 0);

         if ( dev_ptr.getTarget() != null &&
              dev_ptr.getTarget().equals(device.getName()) )
         {
            proxies.add(cur_elt);
         }
      }
   }

   private void findAliases(ConfigContext ctx)
   {
      ConfigBroker broker = new ConfigBrokerProxy();
      List elts = broker.getElements(ctx);
      List alias_elts = ConfigUtilities.getElementsWithDefinition(elts,
                                                                  ALIAS_TYPE);
      fillAliasMap(alias_elts);
   }

   private void fillAliasMap(List aliasElts)
   {
      for ( Iterator i = aliasElts.iterator(); i.hasNext(); )
      {
         ConfigElement alias_elt = (ConfigElement) i.next();
         ConfigElementPointer proxy_ptr =
            (ConfigElementPointer) alias_elt.getProperty(PROXY_PROPERTY, 0);

         if ( proxy_ptr != null && proxy_ptr.getTarget() != null &&
              ! proxy_ptr.getTarget().equals("") )
         {
            for ( Iterator j = proxies.iterator(); j.hasNext(); )
            {
               ConfigElement proxy_elt = (ConfigElement) j.next();

               if ( proxy_ptr.getTarget().equals(proxy_elt.getName()) )
               {
                  aliases.put(proxy_elt, alias_elt);
                  break;
               }
            }
         }
      }
   }

   private Map           aliases = new HashMultiMap();
   private List          proxies = new ArrayList();
   private ConfigElement device  = null;
}
