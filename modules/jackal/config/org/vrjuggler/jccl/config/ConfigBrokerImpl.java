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
import org.jdom.Document;
import org.jdom.JDOMException;
import org.jdom.input.SAXBuilder;

/**
 * Implementation of the ConfigBroker interface for the server side.
 *
 * @see ConfigBroker
 */
public class ConfigBrokerImpl
   implements ConfigBroker
{
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
      // Check if a resource is already open under that name
      if (resources.containsKey(name))
      {
         throw new IllegalArgumentException(name + " already in use");
      }

      // Try to load in the resource
      Document resource_doc = loadResource(input);
      if (resource_doc.getRootElement().getName().equals("ChunkDescDB"))
      {
         // We just loaded a ChunkDescDB
         ChunkDescDB desc_db = new ChunkDescDB();
         desc_db.build(resource_doc);
         resources.put(name, desc_db);
      }
      else if (resource_doc.getRootElement().getName().equals("ConfigChunkDB"))
      {
         // We just loaded a ConfigChunkDB
         ChunkDescDB all_descs = new ChunkDescDB();
         all_descs.addAll(getDescs(context));
         ChunkFactory.setChunkDescDB(all_descs);
         ConfigChunkDB chunk_db = new ConfigChunkDB();
         chunk_db.build(resource_doc);
         resources.put(name, chunk_db);
      }
      else
      {
         throw new IOException("Invalid file");
      }
   }

   /**
    * Loads the DOM document from the given input stream.
    *
    * @param input      the stream to read from
    *
    * @return  the DOM document corresponding to the resource's data
    *
    * @throws IOException     if there was an error loading the resource
    */
   private Document loadResource(InputStream input)
      throws IOException
   {
      Document doc = null;

      try
      {
         SAXBuilder builder = new SAXBuilder();
         doc = builder.build(input);
      }
      catch (JDOMException e)
      {
         throw new IOException(e.getMessage());
      }

      return doc;
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
      throw new IOException("not implemented");
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
      throw new IOException("not implemented");
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
      // Get a list of the resources in this context that are config chunk DBs
      List chunk_res = getResourcesOfType(context, ConfigChunkDB.class);
      ConfigChunkDB target = null;

      // Check if this context has nothing to add to
      if (chunk_res.size() == 0)
      {
         return false;
      }

      // If there is more than one file to add to, ask the user which one they
      // want to add it to.
      if (chunk_res.size() > 1)
      {
         // TODO: ask the user which file they want to add to, for now pick the
         // first
         target = (ConfigChunkDB)chunk_res.get(0);
      }
      else
      {
         target = (ConfigChunkDB)chunk_res.get(0);
      }

      target.add(chunk);
      return true;
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
      // Get a list of the resources in this context that are config chunk DBs
      List chunk_res = getResourcesOfType(context, ConfigChunkDB.class);

      for (Iterator itr = chunk_res.iterator(); itr.hasNext(); )
      {
         ConfigChunkDB db = (ConfigChunkDB)itr.next();
         if (db.contains(chunk))
         {
            db.remove(chunk);
            return true;
         }
      }
      return false;
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
      // Get a list of the resources in this context that are chunk desc DBs
      List desc_res = getResourcesOfType(context, ChunkDescDB.class);
      ChunkDescDB target = null;

      // Check if this context has nothing to add to
      if (desc_res.size() == 0)
      {
         return false;
      }

      // If there is more than one file to add to, ask the user which one they
      // want to add it to.
      if (desc_res.size() > 1)
      {
         // TODO: ask the user which file they want to add to, for now pick the
         // first
         target = (ChunkDescDB)desc_res.get(0);
      }
      else
      {
         target = (ChunkDescDB)desc_res.get(0);
      }

      target.add(desc);
      return true;
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
      // Get a list of the resources in this context that are config chunk DBs
      List desc_res = getResourcesOfType(context, ChunkDescDB.class);

      for (Iterator itr = desc_res.iterator(); itr.hasNext(); )
      {
         ChunkDescDB db = (ChunkDescDB)itr.next();
         if (db.contains(desc))
         {
            db.remove(desc);
            return true;
         }
      }
      return false;
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
      List all_descs = new ArrayList();
      for (Iterator itr = context.getResources().iterator(); itr.hasNext(); )
      {
         String name = (String)itr.next();
         Object resource = resources.get(name);
         if (resource instanceof ChunkDescDB)
         {
            all_descs.addAll(((ChunkDescDB)resource).getAll());
         }
      }

      return all_descs;
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
      List all_chunks = new ArrayList();
      for (Iterator itr = context.getResources().iterator(); itr.hasNext(); )
      {
         String name = (String)itr.next();
         Object resource = resources.get(name);
         if (resource instanceof ConfigChunkDB)
         {
            all_chunks.addAll(((ConfigChunkDB)resource).getAll());
         }
      }

      return all_chunks;
   }

   /**
    * Gets a list of the names all the resources currently being managed by this
    * broker.
    *
    * @return  a list of the resource names
    */
   public List getResourceNames()
   {
      List result = new ArrayList();
      result.addAll(resources.keySet());
      return result;
   }

   /**
    * Gets a list of all the resources managed by this broker that are of the
    * given class.
    *
    * @param context       the context in which to search
    * @param testClass     the class to test for
    *
    * @return  a list of the matching resources
    */
   protected List getResourcesOfType(ConfigContext context, Class testClass)
   {
      List result = new ArrayList();

      for (Iterator itr = context.getResources().iterator(); itr.hasNext(); )
      {
         String name = (String)itr.next();
         Object resource = resources.get(name);
         if (resource != null)
         {
            if (testClass.isInstance(resource))
            {
               result.add(resource);
            }
         }
      }

      return result;
   }

   /**
    * The mapping of resources names to the resources it refers to.
    */
   private Map resources = new HashMap();
}
