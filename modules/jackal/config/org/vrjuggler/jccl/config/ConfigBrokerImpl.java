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
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.jccl.config.io.*;

/**
 * Implementation of the ConfigBroker interface for the server side.
 *
 * @see ConfigBroker
 */
public class ConfigBrokerImpl
   implements ConfigBroker
{
   /**
    * Creates a new configuration broker implementation. All definitions found
    * in the configuration definition search path will be loaded and stored in
    * the repository.
    */
   public ConfigBrokerImpl()
   {
      initRepository();
   }

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
      if (mResources.containsKey(name))
      {
         // The key is in use ... is it the same datasource?
         if (mResources.get(name).equals(dataSource))
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
         mResources.put(name, dataSource);
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
      if (! mResources.containsKey(name))
      {
         throw new IllegalArgumentException(name + " is not open");
      }

      // Check if this is the last reference to the resource
      if (getReferenceCount(name) == 1)
      {
         // Hey, nobody wants this baby anymore. Remove it
         DataSource data_source = (DataSource)mResources.remove(name);
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
      return (DataSource)mResources.get(name);
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
      return mResources.containsKey(name);
   }

   /**
    * Adds the given configuration element to the current context. If the
    * context contains more than one resource, a dialog will prompt the user for
    * which resource they wish to add the element to.
    *
    * @param context    the context in which to add the element
    * @param elt        the element to add
    *
    * @return  true if the addition was successful, false otherwise
    */
   public boolean add(ConfigContext context, ConfigElement elt)
   {
      // Get a list of the resources in this context that can handle
      // ConfigElements.
      List resources = context.getResources();
      DataSource target_ds = null;

      // Check if this context has nothing to add to
      if (resources.size() == 0)
      {
         return false;
      }

      // If there is more than one file to add to, ask the user which one they
      // want to add it to.
      if (resources.size() > 1)
      {
         // Present the user with a dialog from which to choose a resource
         ResourceChooser chooser = new ResourceChooser();
         chooser.setDialogTitle("Choose a Configuration resource");
         List res_names = new ArrayList();
         for (Iterator itr = resources.iterator(); itr.hasNext(); )
         {
            res_names.add(getNameFor(itr.next()));
         }
         chooser.setResources(res_names);
         int result = chooser.showDialog(null);
         if (result == ResourceChooser.APPROVE_OPTION)
         {
            String data_source_name = (String)mResources.get(chooser.getSelectedResource());
            target_ds = get(data_source_name);
         }
         else
         {
            // The user cancelled the action
            return false;
         }
      }
      else
      {
         String data_source_name = (String)resources.get(0);
         target_ds = get(data_source_name);
      }

      target_ds.add(elt);
      fireConfigElementAdded(getNameFor(target_ds), elt);
      return true;
   }

   /**
    * Removes the given configuration element from the current context. If the
    * element appears in more than one resource in the context, a dialog will
    * prompt the user for which resource they wish to remove the element from.
    * If the element does not appear in any resource in the context, this method
    * will return false.
    *
    * @param context    the context from which to remove the element
    * @param elt        the element to remove
    *
    * @return  true if the removal was successful, false if the user cancelled
    *          the removal or the element does not exist in any resource
    */
   public boolean remove(ConfigContext context, ConfigElement elt)
   {
      List resources = context.getResources();
      for (Iterator itr = resources.iterator(); itr.hasNext(); )
      {
         String data_source_name = (String)itr.next();
         DataSource data_source = get(data_source_name);
         if (data_source.contains(elt))
         {
            data_source.remove(elt);
            fireConfigElementRemoved(getNameFor(data_source), elt);
            return true;
         }
      }
      return false;
   }

   /**
    * Gets a list of all the configuration elements within the given context.
    *
    * @param context    the context from which to retrieve elements
    *
    * @return  a list of the configuration elements
    */
   public List getElements(ConfigContext context)
   {
      List all_elts = new ArrayList();
      for (Iterator itr = context.getResources().iterator(); itr.hasNext(); )
      {
         String name = (String)itr.next();
         DataSource data_source = (DataSource)mResources.get(name);
         all_elts.addAll(data_source.getElements());
      }

      return all_elts;
   }
   
   /**
    * Gets a list of all the configuration elements within the given context
    * includeing embedded elements.
    *
    * @param context    the context from which to retrieve elements
    *
    * @return  a list of the configuration elements
    */
   public List getElementsIncludingEmbedded(ConfigContext context)
   {
      List all_elts = getElements(context);
      List result = new ArrayList();
      
      // Get the elements embedded within the current element
      for (Iterator itr = all_elts.iterator(); itr.hasNext(); )
      {
         ConfigElement elt = (ConfigElement)itr.next();
         result.add(elt);
         result.addAll(ConfigUtilities.getEmbeddedElementsRecursive(elt));
      }

      return result;
   }

   /**
    * Gets a list of all the configuration elements within the given
    * resource.
    *
    * @param resource   the name of the resource in which to get elements
    *
    * @return  a list of the ConfigElements in the resource
    */
   public List getElementsIn(String resource)
   {
      DataSource data_source = (DataSource)mResources.get(resource);
      if (data_source != null)
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
      result.addAll(mResources.keySet());
      return result;
   }

   /**
    * Gets the repository in which configuration definitions are stored.
    *
    * @return  the repository of config definitions
    */
   public ConfigDefinitionRepository getRepository()
   {
      return mRepos;
   }

   /**
    * Adds the given listener to receive config events from this broker.
    */
   public void addConfigListener(ConfigListener listener)
   {
      mListeners.add(ConfigListener.class, listener);
   }

   /**
    * Removes the given listener from receiving config events from this broker.
    */
   public void removeConfigListener(ConfigListener listener)
   {
      mListeners.remove(ConfigListener.class, listener);
   }

   /**
    * Adds the given listener to receive config broker events from this broker.
    */
   public void addConfigBrokerListener(ConfigBrokerListener listener)
   {
      mListeners.add(ConfigBrokerListener.class, listener);
   }

   /**
    * Removes the given resource listener from receiving confi broker events
    * from this broker.
    */
   public void removeConfigBrokerListener(ConfigBrokerListener listener)
   {
      mListeners.remove(ConfigBrokerListener.class, listener);
   }

   /**
    * Notifies all listeners that the given resource has been added.
    */
   protected void fireResourceAdded(String resource, DataSource dataSource)
   {
      ConfigBrokerEvent evt = null;
      Object[] listener_list = mListeners.getListenerList();
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
      Object[] listener_list = mListeners.getListenerList();
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
    * Notifies all listeners that the given configuration element has been added
    * to the given resource.
    */
   protected void fireConfigElementAdded(String resource, ConfigElement elt)
   {
      ConfigEvent evt = null;
      Object[] listener_list = mListeners.getListenerList();
      for (int i=listener_list.length-2; i>=0; i-=2)
      {
         if (listener_list[i] == ConfigListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigEvent(this, resource, elt);
            }
            ((ConfigListener)listener_list[i+1]).configElementAdded(evt);
         }
      }
   }

   /**
    * Notifies all listeners that the given configuration element has been
    * removed from the given resource.
    */
   protected void fireConfigElementRemoved(String resource, ConfigElement elt)
   {
      ConfigEvent evt = null;
      Object[] listener_list = mListeners.getListenerList();
      for (int i=listener_list.length-2; i>=0; i-=2)
      {
         if (listener_list[i] == ConfigListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigEvent(this, resource, elt);
            }
            ((ConfigListener)listener_list[i+1]).configElementRemoved(evt);
         }
      }
   }

   /**
    * Does a reverse lookup to get the name mapped to the given resource.
    *
    * @return  the name of the resource or null if the resource could not be
    *          found
    */
   private String getNameFor(Object resource)
   {
      for (Iterator itr = mResources.keySet().iterator(); itr.hasNext(); )
      {
         String name = (String)itr.next();
         Object res = mResources.get(name);
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
      if (! mReferences.containsKey(name))
      {
         return 0;
      }
      return ((Integer)mReferences.get(name)).intValue();
   }

   private synchronized void addReference(String name)
   {
      // Increment the ref count and put it in the references map
      int refcount = getReferenceCount(name);
      ++refcount;
      mReferences.put(name, new Integer(refcount));
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
      mReferences.put(name, new Integer(refcount));
      System.out.println("removeReference: Data Source '"+name+"' now has "+refcount+" references");
   }

   /**
    * Fills the repository with all the definitions it can find in the
    * configuration definition search path.
    */
   private void initRepository()
   {
      mRepos = new ConfigDefinitionRepository();

      // Get a list of the definition files to load
      List def_file_list = new ArrayList();
      List def_path = getDefinitionPath();
      for (Iterator itr = def_path.iterator(); itr.hasNext(); )
      {
         // Check if this part of the path is a valid directory we can read
         String dir_name = (String)itr.next();
         File dir = new File(dir_name);
         if (dir.exists() && dir.isDirectory() && dir.canRead())
         {
            // Get a list of all the config definition files in the directory
            File[] def_files = dir.listFiles(new FilenameFilter()
            {
               public boolean accept(File dir, String file)
               {
                  // Only accept files with a .jdef extension
                  if (file.endsWith(".jdef"))
                  {
                     File def_file = new File(dir, file);
                     if (def_file.canRead())
                     {
                        return true;
                     }
                  }
                  return false;
               }
            });

            // Add the files to the list of files to load
            for (int i=0; i<def_files.length; ++i)
            {
               def_file_list.add(def_files[i]);
            }
         }
      }

      // Load in the definitions for each file and place them in the repository
      for (Iterator itr = def_file_list.iterator(); itr.hasNext(); )
      {
         try
         {
            // Attempt to load in the definitions in the file
            File def_file = (File)itr.next();
            ConfigDefinitionReader reader = new ConfigDefinitionReader(def_file);
            List defs = reader.readDefinition();
            for (Iterator def_itr = defs.iterator(); def_itr.hasNext(); )
            {
               ConfigDefinition def = (ConfigDefinition)def_itr.next();
               mRepos.add(def);
            }
         }
         catch (ParseException pe)
         {
            pe.printStackTrace();
         }
         catch (IOException ioe)
         {
            ioe.printStackTrace();
         }
      }
   }

   /**
    * Save a new version of the ConfigDefinition with the given token.
    */
   public void saveDefinition(String token)
   {
      List def_file_list = new ArrayList();
      List def_path = getDefinitionPath();
      final String temp_token = token;
      
      for (Iterator itr = def_path.iterator(); itr.hasNext(); )
      {
         // Check if this part of the path is a valid directory we can read
         String dir_name = (String)itr.next();
         File dir = new File(dir_name);
         if (dir.exists() && dir.isDirectory() && dir.canRead())
         {
            // Get a list of all the config definition files in the directory
            File[] def_files = dir.listFiles(new FilenameFilter()
            {
               public boolean accept(File dir, String file)
               {
                  // Only accept files with a .jdef extension
                  if (file.equals(temp_token + ".jdef"))
                  {
                     File def_file = new File(dir, file);
                     if (def_file.canRead())
                     {
                        return true;
                     }
                  }
                  return false;
               }
            });

            for (int i=0; i<def_files.length; ++i)
            {
               def_file_list.add(def_files[i]);
            }
         }
      }
     
      List defs = null;
      ConfigDefinition edited_def = mRepos.get(token);
      int new_version = 1;
      File def_file = null;

      if(def_file_list.size() > 1)
      {
         // Error
         System.out.println("ERROR: Multiple files for token " + token + " found.");
         return;
      }
      else if(1 > def_file_list.size())
      {
         System.out.println("Saving a new configuration not yet implemented.");
         return;
         //def_file = new File();
      }
      else if(1 == def_file_list.size())
      {
         def_file = (File)def_file_list.get(0);
         System.out.println("Found it:" + def_file.getAbsolutePath());

         try
         {
            // Attempt to load in the definitions in the file
            ConfigDefinitionReader reader = new ConfigDefinitionReader(def_file);
            defs = reader.readDefinition();
            
            // Get the maximal version number
            try
            {
               new_version = mRepos.getNewestVersionNumber(token).intValue() + 1;
            }
            catch(DefinitionLookupException dle)
            {
               dle.printStackTrace();
            }
         }
         catch (ParseException pe)
         {
            pe.printStackTrace();
         }
         catch (IOException ioe)
         {
            ioe.printStackTrace();
         }
      }

      try
      {
         // We want to set the XSLT to null since we do not have a way to
         // create it here.
         ConfigDefinition new_def = new ConfigDefinition(edited_def.getName(),
                                                        edited_def.getToken(),
                                                        edited_def.getIconLocation(),
                                                        new_version,
                                                        edited_def.getParents(),
                                                        edited_def.getHelp(),
                                                        edited_def.getCategories(),
                                                        edited_def.getPropertyDefinitions(),
                                                        null);
         defs.add(new_def);
         
         ConfigDefinitionWriter writer = new ConfigDefinitionWriter(def_file);
         writer.writeDefinition(defs);
      }
      catch (IOException ioe)
      {
         ioe.printStackTrace();
      }
   }

   /**
    * Gets a list of the directories in which to look for configuration
    * definitions.
    */
   private List getDefinitionPath()
   {
      List dirs = new ArrayList();

      // Get the path from the environment
      String default_path = "${VJ_BASE_DIR}/share/vrjuggler/data/definitions";
      String path = System.getProperty("JCCL_DEFINITION_PATH", default_path);
      path = (new EnvironmentServiceProxy()).expandEnvVars(path);

      // Split the path on the path separator
      StringTokenizer tokenizer = new StringTokenizer(path, File.pathSeparator);
      while (tokenizer.hasMoreTokens())
      {
         dirs.add(tokenizer.nextToken());
      }

      return dirs;
   }

   /** The mapping of resource names to the resources it refers to. */
   private Map mResources = new HashMap();

   /** The repository in which configuration definitions are stored. */
   private ConfigDefinitionRepository mRepos = new ConfigDefinitionRepository();

   /** The mapping of data source names to their reference count. */
   private Map mReferences = new HashMap();

   /** All listeners interested in this broker. */
   private EventListenerList mListeners = new EventListenerList();
}
