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
package org.vrjuggler.jccl.config;

import java.util.*;
import javax.swing.event.EventListenerList;
import org.jdom.*;
import org.vrjuggler.jccl.config.event.*;

/**
 * A configuration definition defines what properties are allowed in a
 * particular configuration element. Other metadata describing parts of an
 * element other than its properties are also defined here.
 */
public class ConfigDefinition
{
   /**
    * Creates a new configuration definition using the given metadata.
    */
   public ConfigDefinition(String name, String token, String icon_location, 
                           int version, boolean isAbstract, List parents,
                           String help, List categories, List propDefs,
                           Element xslt_element)
   {
      mName = name;
      mToken = token;
      mIconLocation = icon_location;
      mVersion = version;
      mIsAbstract = isAbstract;
      mParents = parents;
      mHelp = help;
      mCategories = categories;
      mPropertyDefs = propDefs;
      mXsltElement = xslt_element;
   }

   /**
    * Gets the display name for this configuration definition. This is nothing
    * more than a human-friendly name for the definition.
    */
   public String getName()
   {
      return mName;
   }

   public Element getXsltElement()
   {
      return mXsltElement;
   }

   /**
    * Sets the display name for this configuration definition.
    *
    * @param name    the new name for the configuration definition
    */
   public synchronized void setName(String name)
   {
      String old_name = mName;
      mName = name;
      fireNameChanged(old_name);
   }

   /**
    * Gets the unique token for this definition. This token is used as the name
    * of the XML element used when writing configuration elements to a file.
    */
   public String getToken()
   {
      return mToken;
   }

   /**
    * Sets the unique token for this definition.
    */
   public synchronized void setToken(String token)
   {
      String old_token = mToken;
      mToken = token;
      fireTokenChanged(old_token);
   }

   /**
    * Gets the location of the icon that is associated with this
    * ConfigDefinition.
    */
   public String getIconLocation()
   {
      return mIconLocation;
   }

   /**
    * Gets the version of this definition. Note that there is intentionally not
    * a setVersion() method since the version of a configuration definition is
    * intended to be immutable.
    */
   public int getVersion()
   {
      return mVersion;
   }

   public boolean isAbstract()
   {
      return mIsAbstract;
   }

   public void setAbstract(boolean b)
   {
      boolean old_is_abstract = mIsAbstract;
      mIsAbstract = b;
      fireAbstractChanged(Boolean.valueOf(old_is_abstract));
   }

   /**
    * Sets the help text for this definition.
    *
    * @param help    the help text for this definition
    */
   public synchronized void setHelp(String help)
   {
      String old_help = mHelp;
      mHelp = help;
      fireHelpChanged(old_help);
   }

   /**
    * Gets the help text for this definition.
    */
   public String getHelp()
   {
      return mHelp;
   }
   
   /**
    * Gets the parent for this definition.
    */
   public List getParents()
   {
      return mParents;
   }

   /**
    * Adds the named parent to this definition. If the parent already exists
    * in this definition, it will not be added a second time.
    *
    * @param parent     the parent to add
    *
    * @since 0.92.7
    */
   public synchronized void addParent(String parent)
   {
      if ( ! mParents.contains(parent) )
      {
         mParents.add(parent);
         fireParentAdded(parent);
      }
   }

   /**
    * Removes the named parent from this definition. If the parent does not
    * exist in this definition, this method will fail silently.
    *
    * @param parent     the parent to remove
    *
    * @since 0.92.7
    */
   public synchronized void removeParent(String parent)
   {
      if ( mParents.contains(parent) )
      {
         mParents.remove(parent);
         fireParentRemoved(parent);
      }
   }

   /**
    * Gets a list of all the categories this definition belongs to.
    */
   public List getCategories()
   {
      return mCategories;
   }

   /**
    * Adds the given category to this definition. If the category already exists
    * in this definition, it will not be added a second time.
    *
    * @param category      the category to add
    */
   public synchronized void addCategory(Category category)
   {
      if (!mCategories.contains(category))
      {
         mCategories.add(category);
         fireCategoryAdded(category);
      }
   }

   /**
    * Removes the given category from this definition. If the category does not
    * exist in this definition, this method will fail silently.
    *
    * @param category      the category to remove
    */
   public synchronized void removeCategory(Category category)
   {
      if (mCategories.contains(category))
      {
         mCategories.remove(category);
         fireCategoryRemoved(category);
      }
   }

   /**
    * Gets the property definition with the given token.
    *
    * @param token      the token of the property definition to retrieve
    *
    * @throws IllegalArgumentException
    *             if this definition does not have a property definition with
    *             the given token
    */
   public synchronized PropertyDefinition getPropertyDefinition(String token)
      throws IllegalArgumentException
   {
      for (Iterator itr = getPropertyDefinitions().iterator(); itr.hasNext(); )
      {
         PropertyDefinition def = (PropertyDefinition)itr.next();
         if (def.getToken().equals(token))
         {
            return def;
         }
      }

      throw new IllegalArgumentException("Invalid property definition: " + token);
   }
   
   private ConfigBroker mBroker = null;

   /**
    * Gets a handle to the configuration broker.
    */
   protected ConfigBroker getBroker()
   {
      if (mBroker == null)
      {
         synchronized (this)
         {
            if (mBroker == null)
            {
               mBroker = new ConfigBrokerProxy();
            }
         }
      }
      return mBroker;
   }

   /**
    * Adds the given property definition to this configuration definition. The
    * property definition must have a unique token.
    *
    * @param propDef    the property definition to add
    *
    * @throws IllegalArgumentException
    *             if this configuration definition already has a property with
    *             the same token
    */
   public synchronized void addPropertyDefinition(PropertyDefinition propDef)
      throws IllegalArgumentException
   {
      // Check if this definition already has a property definition with the
      // same token
      try
      {
         getPropertyDefinition(propDef.getToken());
      }
      catch (IllegalArgumentException iae)
      {
         mPropertyDefs.add(propDef);
         firePropertyDefinitionAdded(propDef);
         return;
      }

      // If we get here, this definition already has a property definition
      // with the same token
      throw new IllegalArgumentException("A property definition with the token, "
                                    + propDef.getToken() + ", already exists.");
   }

   /**
    * Removes the given property definition from this configuration definition.
    * If the property definition does not exist in this configuration
    * definition, this method fails silently.
    *
    * @param propDef    the propertyDefinition to remove
    */
   public synchronized void removePropertyDefinition(PropertyDefinition propDef)
   {
      if (mPropertyDefs.contains(propDef))
      {
         mPropertyDefs.remove(propDef);
         firePropertyDefinitionRemoved(propDef);
      }
   }

   /**
    * Determines if this config definition is of the given type.
    *
    * @param token      the config definition type that may or may not be a
    *                   base type of this config definition
    *
    * @return true is returned if this config definition is of the given type;
    *         false is returned otherwise
    *
    * @since 0.92.9
    */
   public synchronized boolean isOfType(String token)
   {
      ConfigDefinitionRepository repos = getBroker().getRepository();

      if ( getToken().equals(token) )
      {
         return true;
      }
      else
      {
         for ( Iterator p = mParents.iterator(); p.hasNext(); )
         {
            ConfigDefinition parent_def = repos.get((String) p.next());
            if ( parent_def.isOfType(token) )
            {
               return true;
            }
         }
      }

      return false;
   }

   /**
    * Get a list of all ConfigDefinitions that derive from this definition.
    */
   public synchronized List getSubDefinitions()
   {
      ConfigDefinitionRepository repos = getBroker().getRepository();
      List result = new ArrayList();
      getSubDefinitionsHelper(result, repos);

      return result;
   }
   private synchronized void getSubDefinitionsHelper(List result, ConfigDefinitionRepository repos)
   {
      // Passing in the list defs to use recursivly to save copying if I were
      // to return it from each recursive call.
     
      // Iterate over repository finding children
      for (Iterator itr = repos.getAllLatest().iterator(); itr.hasNext(); )
      {
         ConfigDefinition def = (ConfigDefinition)itr.next();
         if(def.getParents().contains(mToken) && !result.contains(def))
         {
            result.add(def);
         }
      }
   }

   /**
    * Gets a list of all the property definitions.
    */
   public synchronized List getPropertyDefinitions()
   {
      ConfigDefinitionRepository repos = getBroker().getRepository();
      
      // This will use recursion to find a property definition in a parent. 
         
      // - Get property definitions from parent.
      // - Create a copy local list of definition.
      // - Iterate over parent property definitions.
      //   - If we do not have a local property definition
      //     - Inherit parent's definiton

      List temp_list = new ArrayList();
      temp_list.addAll(mPropertyDefs);


      for (Iterator parent_itr = mParents.iterator(); parent_itr.hasNext(); )
      {
         ConfigDefinition parent = repos.get((String)parent_itr.next());
         
         if(null != parent)
         {   
            List parent_defs = parent.getPropertyDefinitions();
            for (Iterator itr = parent_defs.iterator(); itr.hasNext(); )
            {
               Object definition = itr.next();
               if(!temp_list.contains(definition))
               {
                  temp_list.add(definition);
               }
            }
         }
      }
      return temp_list;
   }

   /**
    * Gets the number of property definitions.
    */
   public synchronized int getPropertyDefinitionsCount()
   {
      return getPropertyDefinitions().size();
   }


   /**
    * Tests if this configuration definition equals the given one.
    */
   public boolean equals(Object obj)
   {
      if (obj instanceof ConfigDefinition)
      {
         ConfigDefinition d = (ConfigDefinition)obj;
         if (mName.equals(d.mName) &&
             mToken.equals(d.mToken) &&
             mIconLocation.equals(d.mIconLocation) &&
             mVersion == d.mVersion &&
             mIsAbstract == d.mIsAbstract &&
             mCategories.equals(d.mCategories) &&
             mHelp.equals(d.mHelp) &&
             mParents.equals(d.mParents) &&
             mPropertyDefs.equals(d.mPropertyDefs) &&
             mParents.equals(d.mParents))
         {
            return true;
         }
      }
      return false;
   }

   /**
    * Adds the given listener to be notified when this config definition
    * changes.
    */
   public void addConfigDefinitionListener(ConfigDefinitionListener listener)
   {
      listenerList.add(ConfigDefinitionListener.class, listener);
   }

   /**
    * Removes the given listener that was registered to be notified when this
    * config definition changed.
    */
   public void removeConfigDefinitionListener(ConfigDefinitionListener listener)
   {
      listenerList.remove(ConfigDefinitionListener.class, listener);
   }

   /**
    * Notifies listeners that this definition's name has changed.
    */
   protected void fireNameChanged(String old_name)
   {
      ConfigDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigDefinitionEvent(this, old_name);
            }
            ((ConfigDefinitionListener)listeners[i+1]).nameChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners that this definition's token has changed.
    */
   protected void fireTokenChanged(String old_token)
   {
      ConfigDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigDefinitionEvent(this, old_token);
            }
            ((ConfigDefinitionListener)listeners[i+1]).tokenChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners that this definition's abstract state has changed.
    */
   protected void fireAbstractChanged(Boolean oldIsAbstract)
   {
      ConfigDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigDefinitionEvent(this, oldIsAbstract);
            }
            ((ConfigDefinitionListener)listeners[i+1]).abstractChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners that this definition's help text has changed.
    */
   protected void fireHelpChanged(String old_help)
   {
      ConfigDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigDefinitionEvent(this, old_help);
            }
            ((ConfigDefinitionListener)listeners[i+1]).helpChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners that a parent has been added to this definition.
    *
    * @since 0.92.7
    */
   protected void fireParentAdded(String parent)
   {
      ConfigDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();

      for ( int i = listeners.length - 2; i >= 0; i -= 2 )
      {
         if ( listeners[i] == ConfigDefinitionListener.class )
         {
            if ( evt == null )
            {
               evt = new ConfigDefinitionEvent(this, parent);
            }

            ((ConfigDefinitionListener) listeners[i + 1]).parentAdded(evt);
         }
      }
   }

   /**
    * Notifies listeners that a parent has been removed from this definition.
    *
    * @since 0.92.7
    */
   protected void fireParentRemoved(String parent)
   {
      ConfigDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();

      for ( int i = listeners.length - 2; i >= 0; i -= 2 )
      {
         if ( listeners[i] == ConfigDefinitionListener.class )
         {
            if ( evt == null )
            {
               evt = new ConfigDefinitionEvent(this, parent);
            }

            ((ConfigDefinitionListener) listeners[i + 1]).parentRemoved(evt);
         }
      }
   }

   /**
    * Notifies listeners that a category has been added to this definition.
    */
   protected void fireCategoryAdded(Category category)
   {
      ConfigDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigDefinitionEvent(this, category);
            }
            ((ConfigDefinitionListener)listeners[i+1]).categoryAdded(evt);
         }
      }
   }

   /**
    * Notifies listeners that a category has been removed from this definition.
    */
   protected void fireCategoryRemoved(Category category)
   {
      ConfigDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigDefinitionEvent(this, category);
            }
            ((ConfigDefinitionListener)listeners[i+1]).categoryRemoved(evt);
         }
      }
   }

   /**
    * Notifies listeners that a property definition has been added to this
    * definition.
    */
   protected void firePropertyDefinitionAdded(PropertyDefinition propDef)
   {
      ConfigDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigDefinitionEvent(this, propDef);
            }
            ((ConfigDefinitionListener)listeners[i+1]).propertyDefinitionAdded(evt);
         }
      }
   }

   /**
    * Notifies listeners that a property definition has been removed from this
    * definition.
    */
   protected void firePropertyDefinitionRemoved(PropertyDefinition propDef)
   {
      ConfigDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigDefinitionEvent(this, propDef);
            }
            ((ConfigDefinitionListener)listeners[i+1]).propertyDefinitionRemoved(evt);
         }
      }
   }

   /** The display name for this definition. */
   private String mName;

   /** The unique token for this definition. */
   private String mToken;

   /** The location of the icon that will be used for the definition */
   private String mIconLocation;
   
   /** The version string for this definition. */
   private int mVersion;
   
   /** Whether this definition can be instantiated as a config element. */
   private boolean mIsAbstract;
   
   /** The short help string for this definition. */
   private String mHelp;

   /** The category list for this definition. */
   private List mCategories;

   /** The property definitions for this config definition. */
   private List mPropertyDefs;

   /** The definition this definition inherits from. */
   private List mParents;

   /** The XSLT transformations source. */
   private Element mXsltElement;
   
   /** List of listeners interested in this definition. */
   private EventListenerList listenerList = new EventListenerList();
}
