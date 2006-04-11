/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
package org.vrjuggler.jccl.config;

import java.io.*;
import java.util.*;
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.TweekBean;

/**
 * Imeplementation of the ConfigBroker interface that marshalls requests off to
 * the registered ConfigBroker Tweek service.
 *
 * @see ConfigBroker
 */
public class ConfigBrokerProxy
   implements ConfigBroker
{
   /**
    * Creates a new ConfigBroker proxy to the ConfigBroker Tweek service.
    *
    * @throws Exception    if the ConfigBroker service could not be found
    */
   public ConfigBrokerProxy()
      throws RuntimeException
   {
      TweekBean bean = BeanRegistry.instance().getBean("ConfigBroker");
      if (bean == null)
      {
         throw new RuntimeException("Could not find ConfigBroker service");
      }
      if (bean.getBean() instanceof ConfigBroker)
      {
         broker = (ConfigBroker)bean.getBean();
      }
      else
      {
         throw new RuntimeException("The bean registered as ConfigBroker is not a ConfigBroker implementation.");
      }
   }

   /**
    * Adds the given resources in the given data source to this broker.
    *
    * @param name          the unique name to assign to the resource
    * @param dataSource    the data source from which to retrieve the data
    */
   public void add(String name, DataSource dataSource)
   {
      broker.add(name, dataSource);
   }

   /**
    * Removes the resources in the given data source from this broker.
    *
    * @param name          the name of the data source to remove
    *
    * @return  the data source that was removed
    */
   public DataSource remove(String name)
   {
      return broker.remove(name);
   }

   /**
    * Gets the data source associated with the given name in this broker.
    *
    * @param name          the unique name assigned to the data source
    *
    * @return  the DataSource if one exists; null if no DataSource has that name
    */
   public DataSource get(String name)
   {
      return broker.get(name);
   }

   /**
    * Tests if the data source with the given name is being managed by this
    * broker.
    *
    * @param name    the name of the data source to check
    *
    * @return  true if the data source resource is in this broker;
    *          false otherwise
    */
   public boolean containsDataSource(String name)
   {
      return broker.containsDataSource(name);
   }

   /**
    * Adds the given config chunk to the current context. If the context
    * contains more than one resource, a dialog will prompt the user for which
    * resource they wish to add the chunk to.
    *
    * @param context    the context in which to add the chunk
    * @param chunk      the chunk to add
    *
    * @return  true if the addition was successful, false otherwise
    */
   public boolean add(ConfigContext context, ConfigChunk chunk)
   {
      return broker.add(context, chunk);
   }

   /**
    * Removes the given config chunk from the current context. If the chunk
    * appears in more than one resource in the context, a dialog will prompt the
    * user for which resource they wish to remove the chunk from. If the chunk
    * does not appear in any resource in the context, this method will return
    * false.
    *
    * @param context    the context from which to remove the chunk
    * @param chunk      the chunk to remove
    *
    * @return  true if the removal was successful, false if the user cancelled
    *          the removal or the chunk does not exist in any resource
    */
   public boolean remove(ConfigContext context, ConfigChunk chunk)
   {
      return broker.remove(context, chunk);
   }

   /**
    * Adds the given chunk description to the current context. If the context
    * contains more than one resource, a dialog will prompt the user for which
    * resource they wish to add the description to.
    *
    * @param context    the context in which to add the description
    * @param desc       the description to add
    *
    * @return  true if the addition was successful, false otherwise
    */
   public boolean add(ConfigContext context, ChunkDesc desc)
   {
      return broker.add(context, desc);
   }

   /**
    * Removes the given chunk description from the current context. If the
    * description appears in more than one resource in the context, a dialog
    * will prompt the user for which resource they wish to remove the
    * description from. If the description does not appear in any resource in
    * the context, this method will return false.
    *
    * @param context    the context from which to remove the description
    * @param desc       the description to remove
    *
    * @return  true if the removal was successful, false if the user cancelled
    *          the removal or the description does not exist in any resource
    */
   public boolean remove(ConfigContext context, ChunkDesc desc)
   {
      return broker.remove(context, desc);
   }

   /**
    * Gets a list of all the configuration descriptions within the given
    * context.
    *
    * @param context    the context from which to retrieve chunk descs
    *
    * @return  a list of the chunk descs
    */
   public List getDescs(ConfigContext context)
   {
      return broker.getDescs(context);
   }

   /**
    * Gets a list of all the configuration elements within the given context.
    *
    * @param context    the context from which to retrieve config chunks
    *
    * @return  a list of the config chunks
    */
   public List getChunks(ConfigContext context)
   {
      return broker.getChunks(context);
   }

   /**
    * Gets a list of all the configuration descriptions within the given
    * resource.
    *
    * @param resource   the name of the resource in which to get descriptions
    *
    * @return  a list of the chunk descs in the resource if it has any
    */
   public List getDescsIn(String resource)
   {
      return broker.getDescsIn(resource);
   }

   /**
    * Gets a list of all the configuration elements within the given
    * resource.
    *
    * @param resource   the name of the resource in which to get elements
    *
    * @return  a list of the config chunks in the resource if it has any
    */
   public List getChunksIn(String resource)
   {
      return broker.getChunksIn(resource);
   }

   /**
    * Gets a list of the names all the resources currently being managed by this
    * broker.
    *
    * @return  a list of the resource names
    */
   public List getResourceNames()
   {
      return broker.getResourceNames();
   }

   /**
    * Adds the given listener to receive config events from this broker.
    */
   public void addConfigListener(ConfigListener listener)
   {
      broker.addConfigListener(listener);
   }

   /**
    * Removes the given listener from receiving config events from this broker.
    */
   public void removeConfigListener(ConfigListener listener)
   {
      broker.removeConfigListener(listener);
   }

   /**
    * Adds the given listener to receive config broker events from this broker.
    */
   public void addConfigBrokerListener(ConfigBrokerListener listener)
   {
      broker.addConfigBrokerListener(listener);
   }

   /**
    * Removes the given resource listener from receiving confi broker events
    * from this broker.
    */
   public void removeConfigBrokerListener(ConfigBrokerListener listener)
   {
      broker.removeConfigBrokerListener(listener);
   }

   /**
    * The config broker instance this object is proxy to.
    */
   private ConfigBroker broker;
}
