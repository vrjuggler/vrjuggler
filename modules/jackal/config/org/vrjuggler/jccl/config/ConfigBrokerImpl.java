/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.jccl.config;

import java.awt.BorderLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.io.*;
import java.util.*;
import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.event.EventListenerList;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.jccl.config.event.ConfigEvent;
import org.vrjuggler.jccl.config.event.ConfigListener;
import org.vrjuggler.jccl.config.event.ConfigBrokerEvent;
import org.vrjuggler.jccl.config.event.ConfigBrokerListener;
import org.vrjuggler.jccl.config.io.*;
import org.vrjuggler.jccl.config.undo.ConfigContextEdit;

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
            String resource_name = (String)itr.next();
            
            // Only add resources that are writable.
            if (!get(resource_name).isReadOnly())
            {
               res_names.add(resource_name);
            }
         }
         
         chooser.setResources(res_names);
         int result = chooser.showDialog(null);
         if (result == ResourceChooser.APPROVE_OPTION)
         {
            String data_source_name = chooser.getSelectedResource();
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

      return add(context, elt, target_ds);
   }

   /**
    * Adds the given configuration element to the given context and the named
    * resource within that context.  The named resource must exist and must
    * be associated with the given context.  Upon successful addition of the
    * element to the context and resource, an undoable edit is logged with
    * the ConfigUndoManager associated with the given context.
    *
    * @param context            the context in which to add the element
    * @param elt                the configuration element to add
    * @param dataSourceName     the name of the resource to which the element
    *                           will be added
    *
    * @return  true if the addition was successful, false otherwise
    *
    * @since 0.92.5
    */
   public boolean add(ConfigContext context, ConfigElement elt,
                      String dataSourceName)
   {
      DataSource data_source = get(dataSourceName);

      // Make sure that the named data source is known to us.
      if ( data_source == null )
      {
         throw new IllegalArgumentException("Unknown DataSource (" +
                                            dataSourceName + ") given");
      }

      return add(context, elt, data_source);
   }

   /**
    * Adds the given configuration element to the given context and the given
    * resource within that context.  The given resource must exist and must
    * be associated with the given context.  Upon successful addition of the
    * element to the context and resource, an undoable edit is logged with
    * the ConfigUndoManager associated with the given context.
    *
    * @param context    the context in which to add the element
    * @param elt        the configuration element to add
    * @param dataSource the resource to which the element will be added
    *
    * @return  true if the addition was successful, false otherwise
    *
    * @since 0.92.5
    */
   public boolean add(ConfigContext context, ConfigElement elt,
                      DataSource dataSource)
   {
      String data_source_name = getNameFor(dataSource);

      // Ensure that we were given a data source that is known to us.
      if ( data_source_name == null )
      {
         throw new IllegalArgumentException("Unknown DataSource given");
      }

      dataSource.add(elt);
      fireConfigElementAdded(data_source_name, elt);
      
      // Inform everyone of the new edit.
      ConfigContextEdit new_edit = new ConfigContextEdit(context, elt,
                                                         dataSource, true);
      context.postEdit(new_edit);
      return true;
   }

   /**
    * Removes the given configuration element from the current context.  All
    * the resources in the given context are searched for the given element.
    * If the element appears in more than one resource in the context, it will
    * be removed from the first resource found.  If the element does not
    * appear in any resource in the context, this method will return false.
    * Upon successful removal of the element from the context and resource, an
    * undoable edit is logged with the ConfigUndoManager associated with the
    * given context.
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
            return remove(context, elt, data_source);
         }
      }
      return false;
   }

   /**
    * Removes the given configuration element from the current context and the
    * named resource within that context.  The named resource must exist and
    * must be associated with the given context.  If the element does not
    * appear in any resource in the context, this method will return false.
    * Upon successful removal of the element from the context and resource, an
    * undoable edit is logged with the ConfigUndoManager associated with the
    * given context.
    *
    * @param context            the context from which to remove the element
    * @param elt                the element to remove
    * @param dataSourceName     the name of the resource from which the element
    *                           will be removed
    *
    * @return  true if the removal was successful, false if the user cancelled
    *          the removal or the element does not exist in any resource
    */
   public boolean remove(ConfigContext context, ConfigElement elt,
                         String dataSourceName)
   {
      return remove(context, elt, get(dataSourceName));
   }

   /**
    * Removes the given configuration element from the current context and the
    * given resource within that context.  The given resource must exist and
    * must be associated with the given context.  If the element does not
    * appear in any resource in the context, this method will return false.
    * Upon successful removal of the element from the context and resource, an
    * undoable edit is logged with the ConfigUndoManager associated with the
    * given context.
    *
    * @param context    the context from which to remove the element
    * @param elt        the element to remove
    * @param dataSource the resource from which the element will be removed
    *
    * @return  true if the removal was successful, false if the user cancelled
    *          the removal or the element does not exist in any resource
    */
   public boolean remove(ConfigContext context, ConfigElement elt,
                         DataSource dataSource)
   {
      if ( dataSource.contains(elt) )
      {
         dataSource.remove(elt);
         fireConfigElementRemoved(getNameFor(dataSource), elt);
         
         // Inform everyone of the new edit.
         ConfigContextEdit new_edit = new ConfigContextEdit(context, elt,
                                                            dataSource, false);
         context.postEdit(new_edit);
         return true;
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
      FilenameFilter filter = new DefinitionFilenameFilter();
      for (Iterator itr = def_path.iterator(); itr.hasNext(); )
      {
         // Check if this part of the path is a valid directory we can read
         String dir_name = (String)itr.next();
         File dir = new File(dir_name);
         if (dir.exists() && dir.isDirectory() && dir.canRead())
         {
            // Get a list of all the config definition files in the directory
            File[] def_files = dir.listFiles(filter);

            // Add the files to the list of files to load
            for (int i=0; i<def_files.length; ++i)
            {
               def_file_list.add(def_files[i]);
            }
         }
      }

      DefinitionLoaderDialog dlg = new DefinitionLoaderDialog(def_file_list);
      dlg.showDialog();
   }

   /**
    * Saves the given ConfigDefinition.
    */
   public void saveDefinition(ConfigDefinition def)
      throws IOException
           , ParseException
   {
      List def_file_list = new ArrayList();
      List def_path = getDefinitionPath();
      final String temp_token = def.getToken();

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
     
      List all_def_versions = null;
      File def_file = null;

      if ( def_file_list.size() > 1 )
      {
         // Error
         throw new IOException("ERROR: Multiple files for token " +
                                  def.getToken() + " found.");
      }
      // We are saving a new definition.
      else if ( def_file_list.size() == 0 )
      {
         all_def_versions = new ArrayList();
         all_def_versions.add(def);
         String file_name = def.getToken() + ".jdef";

         // Default to saving the new file in the first directory in the
         // definition path.
         if ( def_path.size() > 0 )
         {
            def_file = new File((String) def_path.get(0), file_name);
         }
         // If there is no definition path, then save in the current working
         // directory.
         else
         {
            def_file = new File(file_name);
         }

         System.out.println("NOTE: Saving new definition '" + def.getToken() +
                               "' in " + def_file.getAbsoluteFile());
      }
      // We are saving an existing definition.  It may or may not be a new
      // definition.
      else if ( def_file_list.size() == 1 )
      {
         def_file = (File)def_file_list.get(0);
         System.out.println("Found it: " + def_file.getAbsolutePath());

         // Attempt to load in the definitions in the file.
         ConfigDefinitionReader reader =
            new ConfigDefinitionReader(def_file);
         all_def_versions = reader.readDefinition();
         boolean overwriting = false;

         // Determine if there is an existing version of this definition to
         // overwrite.  If not, we will append the given definition version
         // to all_def_versions.
         for ( int i = 0; i < all_def_versions.size(); ++i )
         {
            ConfigDefinition d = (ConfigDefinition) all_def_versions.get(i);

            if ( d.getVersion() == def.getVersion() )
            {
               all_def_versions.set(i, def);
               overwriting = true;
               break;
            }
         }

         if ( ! overwriting )
         {
            all_def_versions.add(def);
         }
      }

      ConfigDefinitionWriter writer = new ConfigDefinitionWriter(def_file);
      writer.writeDefinition(all_def_versions);
   }

   /**
    * Gets a list of the directories in which to look for configuration
    * definitions.
    */
   private List getDefinitionPath()
   {
      List dirs = new ArrayList();
      EnvironmentService service = new EnvironmentServiceProxy();

      // Get the .jdef search path from the environment.
      String data_path = service.getenv("VJ_DATA_DIR");
      String base_path = service.getenv("VJ_BASE_DIR") +  
                         File.separator + data_path + File.separator +
                         "data" + File.separator + "definitions";
      String ext_path  = service.getenv("JCCL_DEFINITION_PATH");
      System.out.println(base_path);

      String path;

      // If $JCCL_DEFINITION_PATH is set, extend the base search path with its
      // value.
      if ( ext_path != null )
      {
         path = base_path + File.pathSeparator + ext_path;
      }
      // Otherwise, we will just use the standard base search path.
      else
      {
         path = base_path;
      }

      path = service.expandEnvVars(path);

      // Split the path on the path separator.
      StringTokenizer tokenizer = new StringTokenizer(path,
                                                      File.pathSeparator);
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

   private class DefinitionLoaderDialog extends JPanel
   {
      public DefinitionLoaderDialog(List defFileList)
      {
         super(new BorderLayout());
         mDefFileList = defFileList;

         // Create a basic border
         Border compound = BorderFactory.createCompoundBorder(
               BorderFactory.createRaisedBevelBorder(),
               BorderFactory.createLoweredBevelBorder());
         this.setBorder(compound);

         mTaskLabel.setHorizontalAlignment(SwingConstants.CENTER);
         mTaskLabel.setText("Loading config definition:");
         mTaskStatusLabel.setHorizontalAlignment(SwingConstants.CENTER);
         mTaskStatusLabel.setText("");

         mProgressBar = new JProgressBar(0, defFileList.size());
         mProgressBar.setValue(0);
         mProgressBar.setStringPainted(true);

         mProgressPanel.setLayout(mProgressPanelLayout);

         mProgressPanel.add(mTaskLabel,
                            new GridBagConstraints(0, 0, 1, 1, 1.0, 1.0,
                                                   GridBagConstraints.CENTER,
                                                   GridBagConstraints.NONE,
                                                   new Insets(0, 0, 0, 0), 0,
                                                   0));
         mProgressPanel.add(mTaskStatusLabel,
                            new GridBagConstraints(0, 1, 1, 1, 1.0, 1.0,
                                                   GridBagConstraints.CENTER,
                                                   GridBagConstraints.HORIZONTAL,
                                                   new Insets(0, 0, 0, 0), 0,
                                                   0));
         mProgressPanel.add(mProgressBar,
                            new GridBagConstraints(0, 2, 1, 1, 1.0, 1.0,
                                                   GridBagConstraints.CENTER,
                                                   GridBagConstraints.BOTH,
                                                   new Insets(0, 0, 0, 0), 0,
                                                   0));

         this.add(mProgressPanel, BorderLayout.SOUTH);

         // Load the icons for the popup menu.
         ClassLoader loader = getClass().getClassLoader();
         ImageIcon vrj_logo = null;

         try
         {
            mLogo = new ImageIcon(loader.getResource("org/vrjuggler/jccl/config/images/jugglerlogo.jpg"));
            mLogoLabel.setIcon(mLogo);
         }
         catch(Exception ex)
         {
            ex.printStackTrace();
         }

         mLogoLabel.setHorizontalAlignment(SwingConstants.CENTER);
         this.add(mLogoLabel, BorderLayout.CENTER);
      }

      public void showDialog()
      {
         final JDialog dialog = new JDialog();
         dialog.setTitle("Loading Config Definitions ...");
         dialog.setModal(true);
         dialog.setUndecorated(true);

         java.awt.Container content_pane = dialog.getContentPane();
         content_pane.setLayout(new BorderLayout());
         content_pane.add(this, BorderLayout.CENTER);

         final SwingWorker worker = new SwingWorker() {
            public Object construct()
            {
               return new Task(dialog);
            }

            public void finished()
            {
               mTaskFinished = true;
            }
         };
         worker.start();

         // Don't bother to display the JDialog if the task is already done
         // by this point.
         if ( ! mTaskFinished )
         {
            dialog.pack();
            dialog.setLocationRelativeTo(null);
            dialog.setVisible(true);
         }
      }

      private class Task
      {
         public Task(JDialog dialog)
         {
            int count = 0;

            // Load in the definitions for each file and place them in the
            // repository.
            for (Iterator itr = mDefFileList.iterator(); itr.hasNext(); )
            {
               // Attempt to load in the definitions in the file.
               File def_file = (File)itr.next();

               try
               {
                  mTaskStatusLabel.setText(def_file.getName());

                  ConfigDefinitionReader reader =
                     new ConfigDefinitionReader(def_file);
                  List defs = reader.readDefinition();
                  for (Iterator def_itr = defs.iterator(); def_itr.hasNext(); )
                  {
                     ConfigDefinition def = (ConfigDefinition)def_itr.next();
                     mRepos.add(def);
                  }

                  mProgressBar.setValue(++count);
               }
               catch (ParseException pe)
               {
                  JOptionPane.showMessageDialog(dialog,
                     "Caught a parse exception while trying to load\n" +
                        def_file.getAbsolutePath() + "\n" +
                        pe.getMessage() + "\n" +
                        "The contents of this file will not be available!",
                     "Definition Load Failure", JOptionPane.WARNING_MESSAGE);
               }
               catch (IOException ioe)
               {
                  JOptionPane.showMessageDialog(dialog,
                     "Caught an I/O exception while trying to load\n" +
                        def_file.getAbsolutePath() + "\n" +
                        ioe.getMessage() + "\n" +
                        "The contents of this file will not be available!",
                     "Definition Load Failure", JOptionPane.WARNING_MESSAGE);
               }
            }

            dialog.setVisible(false);
         }
      }

      private List mDefFileList;

      private JPanel mProgressPanel              = new JPanel();
      private GridBagLayout mProgressPanelLayout = new GridBagLayout();
      private JLabel mTaskLabel                  = new JLabel();
      private JLabel mTaskStatusLabel            = new JLabel();
      private JProgressBar mProgressBar;

      private JLabel mLogoLabel = new JLabel();
      private ImageIcon mLogo = null;
      private boolean mTaskFinished = false;
   }
}
