/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
      throws Exception
   {
      TweekBean bean = BeanRegistry.instance().getBean("ConfigBroker");
      if (bean == null)
      {
         throw new Exception("Could not find ConfigBroker service");
      }
      if (bean.getBean() instanceof ConfigBroker)
      {
         broker = (ConfigBroker)bean.getBean();
      }
      else
      {
         throw new Exception("The bean registered as ConfigBroker is not a ConfigBroker implementation.");
      }
   }

   /**
    * Opens a new configuration resource using the given unique name from the
    * given input stream.
    *
    * @param context    the context in which to open the resource
    * @param name       the unique name to assign to the resource
    * @param input      the stream from which to retrieve the resource data
    *
    * @throws IOException  if there is an error opening the resource
    */
   public void open(ConfigContext context, String name, InputStream input)
      throws IOException
   {
      broker.open(context, name, input);
   }

   /**
    * Closes the configuration resource associated with the given name.
    *
    * @param name    the name of the resource to close
    *
    * @throws IOException  if there is an error closing the resource
    */
   public void close(String name)
      throws IOException
   {
      broker.close(name);
   }

   /**
    * Saves the configuration resource associated with the given name.
    *
    * @param name    the name of the resource to save
    *
    * @throws IOException  if there is an error saving the resource
    */
   public void save(String name)
      throws IOException
   {
      broker.save(name);
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
    * The config broker instance this object is proxy to.
    */
   private ConfigBroker broker;
}
