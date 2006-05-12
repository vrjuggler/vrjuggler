/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

package org.vrjuggler.jccl.rtrc;

import java.io.*;
import java.util.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.jccl.config.io.*;

import jccl.*;

/**
 * This is a DataSource implementation that uses the RemoteReconfigSubject to
 * communicate with a running VR Juggler application in order reconfigure it.
 *
 * @version 1.0
 */
public class RTRCDataSource
   implements DataSource, ConfigElementListener, ConfigListener
{
   /**
    * Creates a data source for a new RemoteReconfigSubject.
    */
   public RTRCDataSource(ConfigDefinitionRepository repos,
                         RemoteReconfigSubject subject)
   {
      mDefinitionRepos = repos;
      mSubject = subject;
      try
      {
         // Load the configuration from the CORBA Subject so that we will at
         // least know the name of the configuration. Which is needed when
         // selecting the RTRCDataSource to use.
         reload();
      }
      catch(IOException ioe)
      {
         ioe.printStackTrace();
      }
   }
   
   /**
    * Provides a method to identify this RTRCDataSource. An example of this
    * would be when you select an RTRCDataSource from the list of CORBA
    * Subjects.
    */
   public String toString()
   {
      if (null == mConfig)
      {
         return("RTRCDataSource");
      }
      return(mConfig.getName());
   }

   /**
    * Loads the data from the application using the RemoteReconfigSubject.
    *
    * @throws IOException
    *             if there was a problem parsing the string from CORBA
    */
   private void reload()
      throws IOException
   {
      try
      {
         // Get the ConfigElements from the application.
         String config_str = mSubject.getElements();
         
         // Read the elements into a configuration.
         ConfigurationReader reader = new ConfigurationReader(config_str, mDefinitionRepos);
         mConfig = reader.readConfiguration();
                     
         // Save the elements for comparison later.
         ConfigurationReader old_reader = new ConfigurationReader(config_str, mDefinitionRepos);
         Configuration old_configuration = old_reader.readConfiguration();
        
         List old_elms = (List)((ArrayList)old_configuration.getElements()).clone();
         List current_elms = (List)((ArrayList)mConfig.getElements()).clone();
         
         while(current_elms.size() > 0)
         {
            ((ConfigElement)current_elms.get(0)).addConfigElementListener(this);
            mHistoryHash.put(current_elms.get(0), old_elms.get(0));
            current_elms.remove(0);
            old_elms.remove(0);
         }

         ConfigBroker broker = new ConfigBrokerProxy();
         broker.addConfigListener(this);
      }
      catch (ParseException pe)
      {
         throw new IOException(pe.getMessage());
      }
   }

   /**
    * Adds the given configuration element to this resource.
    *
    * @param elt        the configuration element to add
    */
   public void add(ConfigElement elt)
   {
      if (!isReadOnly())
      {
         mConfig.addElement(elt);
      }
      else
      {
         throw new UnsupportedOperationException("Data source is read only.");
      }
   }

   /**
    * Removes the given configuration element from this resource.
    *
    * @param elt     the configuration element to remove
    */
   public void remove(ConfigElement elt)
   {
      if (!isReadOnly())
      {
         mConfig.removeElement(elt);
      }
      else
      {
         throw new UnsupportedOperationException("Data source is read only.");
      }
   }

   /**
    * Tests if the given configuration element is in this resource.
    *
    * @param elt     the configuration element to look for
    */
   public boolean contains(ConfigElement elt)
   {
      return mConfig.getElements().contains(elt);
   }

   /**
    * Gets a list of all the elements in this resource.
    *
    * @return  a list of the ConfigElements in this resource
    */
   public List getElements()
   {
      return mConfig.getElements();
   }
   
   /**
    * Takes a ConfigElement and creates a Document in the form of a 
    * String to be used for RTRC.
    *
    * @param elm  the configuration element to convert.
    * @return     a String representation of a Document containg this element.
    */
   private String convertElementToString(ConfigElement elm)
   {
      try
      {
         StringWriter string_writer = new StringWriter();
         ConfigurationWriter writer = new ConfigurationWriter(string_writer);
         
         Configuration config = new Configuration("TEMP");
         config.addElement(elm);
         
         writer.writeConfiguration(config);
         return(string_writer.toString());
      }
      catch(IOException ioe)
      {
         ioe.printStackTrace();
         return(new String());
      }
   }
   
   /**
    * Commits all pending changes to this resource. This sends across 
    * new data to the running application, etc.
    */
   public void commit()
      throws IOException
   {      
      // Handle all ConfigElement additions
      for(Iterator itr = mAddedConfigElements.iterator() ; itr.hasNext() ; )
      {
         ConfigElement new_elm = (ConfigElement)itr.next();
         mSubject.add(convertElementToString(new_elm));
      }
      mAddedConfigElements.clear();

      // Handle all ConfigElement removals
      for(Iterator itr = mRemovedConfigElements.iterator() ; itr.hasNext() ; )
      {
         ConfigElement removed_elm = (ConfigElement)itr.next();
         mSubject.remove(convertElementToString(removed_elm));
      }
      mRemovedConfigElements.clear();
      
      // Handle all CofigElements that have changed
      for(Enumeration itr = mChangedHash.keys() ; itr.hasMoreElements() ; )
      {
         // Get the new ConfigElement from mChangedHash
         ConfigElement new_elm = (ConfigElement)itr.nextElement();
         // Use the new element as a pointer to get the original element.
         ConfigElement old_elm = (ConfigElement)mChangedHash.get(new_elm);
         // Send both the old and the new elements to the application.
         mSubject.swap(convertElementToString(new_elm), convertElementToString(old_elm));

         //TODO: Find a way that we can update the mHistoryHash to contain the
         //the new element as a key, and a copy of itself as the value. This
         //would effectively update the HistroyHash.
      }
      mChangedHash.clear();
   }

   /**
    * Rolls back all pending changes to this resource. This retrieves the
    * data from the application, etc.
    */
   public void rollback()
      throws IOException
   {
      reload();
   }

   /**
    * Checks if this data source is read only.
    */
   public boolean isReadOnly()
   {
      // We can always write to the application using CORBA.
      return false;
   }

   /**
    * Gets a list of the URLs for the resources included by this data source.
    *
    * @return  a list of URLs for the included resources
    */
   public List getIncludes()
   {
      return mConfig.getIncludes();
   }

   /**
    * File data sources are equal if they refer to the same CORBA subject.
    */
   public boolean equals(Object obj)
   {
      if (obj instanceof RTRCDataSource)
      {
         RTRCDataSource ds = (RTRCDataSource)obj;
         if (mSubject.equals(ds.mSubject))
         {
            return true;
         }
      }
      return false;
   }

   //XXX: Debug when this data source gets finalized
   protected void finalize()
      throws Throwable
   {
      System.out.println("RTRCDataSource for being finalized.");
      super.finalize();
   }
   
   /* ConfigElementListener interface */
   public void nameChanged(ConfigElementEvent evt)
   {
      /* Do nothing since we will find this difference later*/;
   }
   public void propertyValueChanged(ConfigElementEvent evt)
   {
      ConfigElement new_elm = (ConfigElement)evt.getSource();
      if(mHistoryHash.containsKey(new_elm))
      {
         System.out.println("RTRC: Element " + new_elm.getName() + " changed.");
         if(!mChangedHash.containsKey(new_elm))
         {
            System.out.println("RTRC: Adding " + new_elm.getName() + " to changed list.");
            ConfigElement old_elm = (ConfigElement)mHistoryHash.get(new_elm);
            mChangedHash.put(new_elm, old_elm);
         }
      }
      else
      {
         System.out.println("Something is very wrong here...");
      }
   }

   public void propertyValueOrderChanged(ConfigElementEvent evt)
   {
      propertyValueChanged(evt);
   }

   public void propertyValueAdded(ConfigElementEvent evt)
   {
      propertyValueChanged(evt);
   }

   public void propertyValueRemoved(ConfigElementEvent evt)
   {
      propertyValueChanged(evt);
   }
  
   /* ConfigListener Interface */

   /**
    * Gives notification that a configuration element was added to the broker.
    *
    * @param evt     the config event
    */
   public void configElementAdded(ConfigEvent evt)
   {
      ConfigElement new_elm = evt.getElement();
      
      if(!mAddedConfigElements.contains(new_elm));
      {
         System.out.println("RTRC: Adding element: " + new_elm.getName() +
               " to " + evt.getResource());
         mAddedConfigElements.add(new_elm);
      }
   }

   /**
    * Gives notification that a config element was removed from the broker.
    *
    * @param evt     the config event
    */
   public void configElementRemoved(ConfigEvent evt)
   {
      ConfigElement removed_elm = evt.getElement();
      
      if(!mRemovedConfigElements.contains(removed_elm));
      {
         System.out.println("RTRC: Removing element: " + removed_elm.getName() +
               " from " + evt.getResource());
         mRemovedConfigElements.add(removed_elm);
      }
   }
   
   /** The configuration contained in this data source. */
   private Configuration mConfig;
   /** Holds the original elements that will be needed when calling swap(new, old) */ 
   private Hashtable mHistoryHash = new Hashtable();
   /** Holds the config elements and their original after a change, and before commiting */
   private Hashtable mChangedHash = new Hashtable();
   /** List of ConfigElements that need to be added when commiting */
   private List mAddedConfigElements = new ArrayList();
   /** List of ConfigElements that need to be removed when commiting */
   private List mRemovedConfigElements = new ArrayList();
   /** The repository from which configuration definitions are retrieved. */
   private ConfigDefinitionRepository mDefinitionRepos = null;
   /** The CORBA Subject that is used to communicate with the appliation */
   private RemoteReconfigSubject mSubject = null;
}
