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
import javax.swing.event.EventListenerList;
import org.jdom.Document;
import org.jdom.JDOMException;
import org.jdom.input.SAXBuilder;

import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.services.EnvironmentService;

/**
 * Implementation of the ConfigBroker interface for the server side. This
 * particular implementation will try to load in the default VRJuggler Chunk
 * description if they are available.
 *
 * @see ConfigBroker
 */
public class ConfigBrokerImpl
   implements ConfigBroker
{
//   /**
//    * Creates a new ConfigBroker service. This will try to load the VRJuggler
//    * chunk desc file by default upon initialization.
//    *
//    * <b>TODO: This is the wrong place to be auto-loading the VRJuggler chunk
//    * desc file. Look to put this elsewhere.</b>
//    */
//   public ConfigBrokerImpl()
//   {
//      String desc_filename = EnvironmentService.expandEnvVars("${VJ_BASE_DIR}/share/vrjuggler/data/vrj-chunks.desc");
//      File desc_file = new File(desc_filename);
//
//      try
//      {
//         if (desc_file.exists() && desc_file.canRead())
//         {
//            // File exists and is readable. Lets see if we can load it.
//            System.out.println("Trying to load "+desc_filename);
//            InputStream in = new BufferedInputStream(new FileInputStream(desc_file));
//
////            String res_name = "VRJuggler Configuration Definitions";
//            String res_name = desc_file.getAbsolutePath();
//            open(new ConfigContext(), res_name, in);
//         }
//      }
//      catch (IOException ioe)
//      {
//         System.err.println("Failed to load base VRJuggler Chunk Descriptions: "+ioe.getMessage());
//      }
//   }
//
//   /**
//    * Opens a new configuration resource using the given unique name from the
//    * given input stream.
//    *
//    * @param context    the context in which to open the resource
//    * @param name       the unique name to assign to the resource
//    * @param input      the stream from which to retrieve the resource data
//    *
//    * @throws IOException  if there is an error opening the resource
//    */
//   public void open(ConfigContext context, String name, InputStream input)
//     throws IOException
//   {
//      // Check if a resource is already open under that name
//      if (resources.containsKey(name))
//      {
//         throw new IllegalArgumentException(name + " already in use");
//      }
//
//      // Try to load in the resource
//      Document resource_doc = loadResource(input);
//      if (resource_doc.getRootElement().getName().equals("ChunkDescDB"))
//      {
//         // We just loaded a ChunkDescDB
//         ChunkDescDB desc_db = new ChunkDescDB();
//         desc_db.build(resource_doc);
//         resources.put(name, desc_db);
//      }
//      else if (resource_doc.getRootElement().getName().equals("ConfigChunkDB"))
//      {
//         // We just loaded a ConfigChunkDB
//         ChunkFactory.setDescs(getDescs(context));
//         ConfigChunkDB chunk_db = new ConfigChunkDB();
//         chunk_db.build(resource_doc);
//         resources.put(name, chunk_db);
//      }
//      else
//      {
//         throw new IOException("Invalid file");
//      }
//
//      // Let listeners know the resource was opened
//      fireResourceOpened(name);
//   }

   /**
    * Adds the given resources in the given data source to this broker.
    *
    * @param name          the unique name to assign to the resource
    * @param dataSource    the data source from which to retrieve the data
    */
   public void add(String name, DataSource dataSource)
   {
      System.out.println("Adding data source: "+name);
      // Check if a resource is already open under that name
      if (resources.containsKey(name))
      {
         // The key is in use ... is it the same datasource?
         if (resources.get(name).equals(dataSource))
         {
            // Same data source, increment the reference
            addReference(name);
         }
         else
         {
            throw new IllegalArgumentException(name + " already in use");
         }
      }
      else
      {
         // This is a new data source, add it in to this broker
         resources.put(name, dataSource);
         addReference(name);
         fireResourceAdded(name, dataSource);
      }
   }

   /**
    * Removes the resources in the given data source from this broker.
    *
    * @param name          the name of the data source to remove
    *
    * @return  the data source that was removed, null if the data source's
    *          reference count was merely decremented
    */
   public DataSource remove(String name)
   {
      System.out.println("Removing data source: "+name);
      // Check if a resource is open under that name
      if (! resources.containsKey(name))
      {
         throw new IllegalArgumentException(name + " is not open");
      }

      // Check if this is the last reference to the resource
      if (getReferenceCount(name) == 1)
      {
         // Hey, nobody wants this baby anymore. Remove it
         DataSource data_source = (DataSource)resources.remove(name);
         removeReference(name);
         fireResourceRemoved(name, data_source);
         return data_source;
      }
      else
      {
         // There are still more references to the data source. Don't really
         // remove it; just decrement the reference count.
         removeReference(name);
         System.out.println("Detected last usage of a data source. Removing it from the broker.");
         return null;
      }
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
      return (DataSource)resources.get(name);
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

//   /**
//    * Closes the configuration resource associated with the given name.
//    *
//    * @param name    the name of the resource to close
//    *
//    * @throws IOException  if there is an error closing the resource
//    */
//   public void close(String name)
//     throws IOException
//   {
//      // Check if a resource is actually open under that name
//      if (resources.containsKey(name))
//      {
//         throw new IllegalArgumentException(name + " is not open");
//      }
//
//      // Close the resource
//      resources.remove(name);
//
//      // Let listeners know the resource was closed
//      fireResourceClosed(name);
//   }
//
//   /**
//    * Saves the configuration resource associated with the given name.
//    *
//    * @param name    the name of the resource to save
//    *
//    * @throws IOException  if there is an error saving the resource
//    */
//   public void save(String name)
//     throws IOException
//   {
//      // Check if a resource is actually open under that name
//      if (! resources.containsKey(name))
//      {
//         throw new IllegalArgumentException(name + " is not open");
//      }
//
//      System.out.println("Saving resource: "+name);
//
//      // Try to save the resource
//      OutputStream out = new BufferedOutputStream(new FileOutputStream(name));
//      Object resource = resources.get(name);
//      if (resource instanceof ChunkDescDB)
//      {
//         ((ChunkDescDB)resource).write(out);
//      }
//      else if (resource instanceof ConfigChunkDB)
//      {
//         ((ConfigChunkDB)resource).write(out);
//      }
//      else
//      {
//         throw new IOException("Unkown resource type");
//      }
//      fireResourceSaved(name);
//   }

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
      return resources.containsKey(name);
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
      // Get a list of the resources in this context that can handle ConfigChunks
      List chunk_res = getElementResources(context);
      DataSource target = null;

      // Check if this context has nothing to add to
      if (chunk_res.size() == 0)
      {
         return false;
      }

      // If there is more than one file to add to, ask the user which one they
      // want to add it to.
      if (chunk_res.size() > 1)
      {
         // Present the user with a dialog from which to choose a resource
         ResourceChooser chooser = new ResourceChooser();
         chooser.setDialogTitle("Choose a Configuration resource");
         List res_names = new ArrayList();
         for (Iterator itr = chunk_res.iterator(); itr.hasNext(); )
         {
            res_names.add(getNameFor(itr.next()));
         }
         chooser.setResources(res_names);
         int result = chooser.showDialog(null);
         if (result == ResourceChooser.APPROVE_OPTION)
         {
            target = (DataSource)resources.get(chooser.getSelectedResource());
         }
         else
         {
            // The user cancelled the action
            return false;
         }
      }
      else
      {
         target = (DataSource)chunk_res.get(0);
      }

      target.add(chunk);
      fireConfigChunkAdded(getNameFor(target), chunk);
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
      // Get a list of the resources in this context that can handle ConfigChunks
      List chunk_res = getElementResources(context);

      for (Iterator itr = chunk_res.iterator(); itr.hasNext(); )
      {
         DataSource data_source = (DataSource)itr.next();
         if (data_source.contains(chunk))
         {
            data_source.remove(chunk);
            fireConfigChunkRemoved(getNameFor(data_source), chunk);
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
      // Get a list of the resources in this context that can handle ChunkDescs
      List desc_res = getDefinitionResources(context);
      DataSource target = null;

      // Check if this context has nothing to add to
      if (desc_res.size() == 0)
      {
         return false;
      }

      // If there is more than one file to add to, ask the user which one they
      // want to add it to.
      if (desc_res.size() > 1)
      {
         // Present the user with a dialog from which to choose a resource
         ResourceChooser chooser = new ResourceChooser();
         chooser.setDialogTitle("Choose a Configuration Definition resource");
         List res_names = new ArrayList();
         for (Iterator itr = desc_res.iterator(); itr.hasNext(); )
         {
            res_names.add(getNameFor(itr.next()));
         }
         chooser.setResources(res_names);
         int result = chooser.showDialog(null);
         if (result == ResourceChooser.APPROVE_OPTION)
         {
            target = (DataSource)resources.get(chooser.getSelectedResource());
         }
         else
         {
            // The user cancelled the action
            return false;
         }
      }
      else
      {
         target = (DataSource)desc_res.get(0);
      }

      target.add(desc);
      fireChunkDescAdded(getNameFor(target), desc);
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
      // Get a list of the resources in this context that can handle ChunkDescs
      List desc_res = getDefinitionResources(context);

      for (Iterator itr = desc_res.iterator(); itr.hasNext(); )
      {
         DataSource data_source = (DataSource)itr.next();
         if (data_source.contains(desc))
         {
            data_source.remove(desc);
            fireChunkDescRemoved(getNameFor(data_source), desc);
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
         DataSource data_source = (DataSource)resources.get(name);
         if (data_source.acceptsDefinitions())
         {
            all_descs.addAll(data_source.getDefinitions());
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
         DataSource data_source = (DataSource)resources.get(name);
         if (data_source.acceptsElements())
         {
            all_chunks.addAll(data_source.getElements());
         }
      }

      return all_chunks;
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
      DataSource data_source = (DataSource)resources.get(resource);
      if (data_source != null && data_source.acceptsDefinitions())
      {
         return data_source.getDefinitions();
      }
      return new ArrayList();
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
      DataSource data_source = (DataSource)resources.get(resource);
      if (data_source != null && data_source.acceptsElements())
      {
         return data_source.getElements();
      }
      return new ArrayList();
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
    * Adds the given listener to receive config events from this broker.
    */
   public void addConfigListener(ConfigListener listener)
   {
      listeners.add(ConfigListener.class, listener);
   }

   /**
    * Removes the given listener from receiving config events from this broker.
    */
   public void removeConfigListener(ConfigListener listener)
   {
      listeners.remove(ConfigListener.class, listener);
   }

   /**
    * Adds the given listener to receive config broker events from this broker.
    */
   public void addConfigBrokerListener(ConfigBrokerListener listener)
   {
      listeners.add(ConfigBrokerListener.class, listener);
   }

   /**
    * Removes the given resource listener from receiving confi broker events
    * from this broker.
    */
   public void removeConfigBrokerListener(ConfigBrokerListener listener)
   {
      listeners.remove(ConfigBrokerListener.class, listener);
   }

   /**
    * Notifies all listeners that the given resource has been added.
    */
   protected void fireResourceAdded(String resource, DataSource dataSource)
   {
      ConfigBrokerEvent evt = null;
      Object[] listener_list = listeners.getListenerList();
      for (int i=listener_list.length-2; i>=0; i-=2)
      {
         if (listener_list[i] == ConfigBrokerListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigBrokerEvent(this, resource, dataSource);
            }
            ((ConfigBrokerListener)listener_list[i+1]).resourceAdded(evt);
         }
      }
   }

   /**
    * Notifies all listeners that the given resource has been removed.
    */
   protected void fireResourceRemoved(String resource, DataSource dataSource)
   {
      ConfigBrokerEvent evt = null;
      Object[] listener_list = listeners.getListenerList();
      for (int i=listener_list.length-2; i>=0; i-=2)
      {
         if (listener_list[i] == ConfigBrokerListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigBrokerEvent(this, resource, dataSource);
            }
            ((ConfigBrokerListener)listener_list[i+1]).resourceRemoved(evt);
         }
      }
   }

   /**
    * Notifies all listeners that the given chunk desc has been added to the
    * given resource.
    */
   protected void fireChunkDescAdded(String resource, ChunkDesc desc)
   {
      ConfigEvent evt = null;
      Object[] listener_list = listeners.getListenerList();
      for (int i=listener_list.length-2; i>=0; i-=2)
      {
         if (listener_list[i] == ConfigListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigEvent(this, resource, desc);
            }
            ((ConfigListener)listener_list[i+1]).chunkDescAdded(evt);
         }
      }
   }

   /**
    * Notifies all listeners that the given chunk desc has been removed from the
    * given resource.
    */
   protected void fireChunkDescRemoved(String resource, ChunkDesc desc)
   {
      ConfigEvent evt = null;
      Object[] listener_list = listeners.getListenerList();
      for (int i=listener_list.length-2; i>=0; i-=2)
      {
         if (listener_list[i] == ConfigListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigEvent(this, resource, desc);
            }
            ((ConfigListener)listener_list[i+1]).chunkDescRemoved(evt);
         }
      }
   }

   /**
    * Notifies all listeners that the given config chunk has been added to the
    * given resource.
    */
   protected void fireConfigChunkAdded(String resource, ConfigChunk chunk)
   {
      ConfigEvent evt = null;
      Object[] listener_list = listeners.getListenerList();
      for (int i=listener_list.length-2; i>=0; i-=2)
      {
         if (listener_list[i] == ConfigListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigEvent(this, resource, chunk);
            }
            ((ConfigListener)listener_list[i+1]).configChunkAdded(evt);
         }
      }
   }

   /**
    * Notifies all listeners that the given config chunk has been removed from
    * the given resource.
    */
   protected void fireConfigChunkRemoved(String resource, ConfigChunk chunk)
   {
      ConfigEvent evt = null;
      Object[] listener_list = listeners.getListenerList();
      for (int i=listener_list.length-2; i>=0; i-=2)
      {
         if (listener_list[i] == ConfigListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigEvent(this, resource, chunk);
            }
            ((ConfigListener)listener_list[i+1]).configChunkRemoved(evt);
         }
      }
   }

   /**
    * Gets a list of all the resources that can handle ConfigChunks.
    *
    * @param context       the context in which to search
    *
    * @return  a list of the matching resources
    */
   protected List getElementResources(ConfigContext context)
   {
      List results = new ArrayList();

      for (Iterator itr = context.getResources().iterator(); itr.hasNext(); )
      {
         String name = (String)itr.next();
         DataSource data_source = (DataSource)resources.get(name);
         if (data_source != null)
         {
            if (data_source.acceptsElements())
            {
               results.add(data_source);
            }
         }
      }

      return results;
   }

   /**
    * Gets a list of all the resources that can handle ChunkDescs.
    *
    * @param context       the context in which to search
    *
    * @return  a list of the matching resources
    */
   protected List getDefinitionResources(ConfigContext context)
   {
      List results = new ArrayList();

      for (Iterator itr = context.getResources().iterator(); itr.hasNext(); )
      {
         String name = (String)itr.next();
         DataSource data_source = (DataSource)resources.get(name);
         if (data_source != null)
         {
            if (data_source.acceptsDefinitions())
            {
               results.add(data_source);
            }
         }
      }

      return results;
   }

   /**
    * Does a reverse lookup to get the name mapped to the given resource.
    *
    * @return  the name of the resource or null if the resource could not be
    *          found
    */
   private String getNameFor(Object resource)
   {
      for (Iterator itr = resources.keySet().iterator(); itr.hasNext(); )
      {
         String name = (String)itr.next();
         Object res = resources.get(name);
         if (res != null)
         {
            if (res == resource)
            {
               return name;
            }
         }
      }
      return null;
   }

   private synchronized int getReferenceCount(String name)
   {
      // Ensure that the reference is valid
      if (! references.containsKey(name))
      {
         return 0;
      }
      return ((Integer)references.get(name)).intValue();
   }

   private synchronized void addReference(String name)
   {
      // Increment the ref count and put it in the references map
      int refcount = getReferenceCount(name);
      ++refcount;
      references.put(name, new Integer(refcount));
      System.out.println("addReference: Data Source '"+name+"' now has "+refcount+" references");
   }

   private synchronized void removeReference(String name)
   {
      // Ensure that the reference is valid
      int refcount = getReferenceCount(name);
      if (refcount == 0)
      {
         throw new IllegalArgumentException("Invalid reference name: "+name);
      }

      // Decrement the refcount and put it back in the references map
      --refcount;
      references.put(name, new Integer(refcount));
      System.out.println("removeReference: Data Source '"+name+"' now has "+refcount+" references");
   }

   /**
    * The mapping of resource names to the resources it refers to.
    */
   private Map resources = new HashMap();

   /**
    * The mapping of data source names to their reference count.
    */
   private Map references = new HashMap();

   /**
    * All listeners interested in this broker.
    */
   private EventListenerList listeners = new EventListenerList();
}
