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

import java.util.*;
import javax.swing.event.EventListenerList;

import org.vrjuggler.jccl.config.event.*;

/**
 * A configuration element is a logical collection of configuration information
 * for a particular system component. It stores a number of properties that
 * describe the configuration.
 */
public class ConfigElement
{
   /**
    * Create a new configuration element with the given properties.
    */
   public ConfigElement(ConfigDefinition def, String name, Map props)
   {
      mName = name;
      mDefinition = def;
      mProps = props;
   }

   /**
    * Gets the name of this configuration element. The name is used to
    * "somewhat" uniquely identify the configuration element.
    */
   public String getName()
   {
      return mName;
   }

   /**
    * Sets the name of this configuration element.
    *
    * @param name       the new name for the configuration element
    */
   public synchronized void setName(String name)
   {
      String old_name = mName;
      mName = name;
      fireNameChanged(old_name);
   }

   /**
    * Gets the object that defines the contents of this element.
    */
   public ConfigDefinition getDefinition()
   {
      return mDefinition;
   }

   /**
    * Gets the value of the property with the given name. The type of the result
    * depends on the type of the property as described by this element's
    * configuration definition object.
    *
    * @param name    the name of the property to retrieve
    * @param index   the index of the property value to retrieve
    *
    * @return the value contained within the property
    *
    * @throws IllegalArgumentException
    *             if this element does not have a property by that name.
    * @throws ArrayIndexOutOfBoundsException
    *             if index is greater than the number of items allowed for the
    *             property as defined by this element's definition.
    */
   public synchronized Object getProperty(String name, int index)
      throws IllegalArgumentException,
             ArrayIndexOutOfBoundsException
   {
      // Get the particular property value
      return getPropertyValues(name).get(index);
   }

   /**
    * Sets the value for the property with the given name at the given index.
    *
    * @param name    the name of the property to set
    * @param index   the index of the property value to set
    */
   public synchronized void setProperty(String name, int index, Object value)
      throws IllegalArgumentException,
             ArrayIndexOutOfBoundsException
   {
      // Get the list of property values
      List values = getPropertyValues(name);

      // TODO: Validate that value is of the correct type for this property.

      // Set the value in the list
      values.set(index, value);

      // Notify listeners of the change
      firePropertyValueChanged(name, index, value);
   }

   /**
    * Appends the given value to the list of values for this property. This only
    * applies if the property supports a variable number of values.
    *
    * @param name    the name of the property to set
    * @param value   the property value to add
    */
   public synchronized void addProperty(String name, Object value)
      throws IllegalArgumentException
   {
      // Get the list of property values
      List values = getPropertyValues(name);

      // Verify that this property supports variable values
      PropertyDefinition prop_def = mDefinition.getPropertyDefinition(name);
      if (! prop_def.isVariable())
      {
         throw new IllegalArgumentException(name + " does not support a variable number of values");
      }

      // TODO: Validate that value is of the correct type for this property

      // Add the value to the list
      int index = values.size();
      values.add(value);

      // Notify listeners of the addition
      firePropertyValueAdded(name, index, value);
   }

   /**
    * Removes the value at the given index for the given property. This only
    * applies if the property supports a variable number of values.
    *
    * @param name    the name of the property to set
    * @param index   the index of the property value to set
    */
   public synchronized void removeProperty(String name, int index)
      throws IllegalArgumentException,
             ArrayIndexOutOfBoundsException
   {
      // Get the list of property values
      List values = getPropertyValues(name);

      // Verify that this property supports variable values
      PropertyDefinition prop_def = mDefinition.getPropertyDefinition(name);
      if (! prop_def.isVariable())
      {
         throw new IllegalArgumentException(name + " does not support a variable number of values");
      }

      // TODO: Validate that value is of the correct type for this property

      // Remove the value from the list
      Object old_value = values.remove(index);

      // Notify listeners of the removal
      firePropertyValueRemoved(name, index, old_value);
   }

   /**
    * Gets the number of values for the given property.
    *
    * @param name    the name of the property to retrieve
    *
    * @return the number of values for the property
    *
    * @throws IllegalArgumentException
    *             if this element does not have a property by that name.
    */
   public synchronized int getPropertyValueCount(String name)
      throws IllegalArgumentException
   {
      // Get the list of property values out of the map
      return getPropertyValues(name).size();
   }

   /**
    * Gets a list of all the values for the given property.
    *
    * @param name    the name of the property to retrieve
    *
    * @return a list containing the values for the property
    *
    * @throws IllegalArgumentException
    *             if this element does not have a property by that name.
    */
   public synchronized List getPropertyValues(String name)
      throws IllegalArgumentException
   {
      // If the property does not exist in the element, bail
      if (!mProps.containsKey(name))
      {
         throw new IllegalArgumentException("Invalid property: " + name);
      }

      // Get the list of property values out of the map
      return (List)mProps.get(name);
   }

   /**
    * Tests if this configuration element equals the given one.
    */
   public boolean equals(Object obj)
   {
      if (obj instanceof ConfigElement)
      {
         ConfigElement c = (ConfigElement)obj;
         if (mName.equals(c.mName) &&
             mDefinition.equals(c.mDefinition) &&
             mProps.equals(c.mProps))
         {
            return true;
         }
      }
      return false;
   }

   /**
    * Adds the given listener to be notified when this config element changes.
    */
   public void addConfigElementListener(ConfigElementListener listener)
   {
      listenerList.add(ConfigElementListener.class, listener);
   }

   /**
    * Removes the given listener that was registered to be notified when this
    * config element changed.
    */
   public void removeConfigElementListener(ConfigElementListener listener)
   {
      listenerList.remove(ConfigElementListener.class, listener);
   }

   /**
    * Notifies listeners that this element's name has changed.
    */
   protected void fireNameChanged(String old_name)
   {
      ConfigElementEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigElementListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigElementEvent(this, null, 0, old_name);
            }
            ((ConfigElementListener)listeners[i+1]).nameChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners that this element's property has changed.
    */
   protected void firePropertyValueChanged(String prop_token, int index,
                                           Object old_value)
   {
      ConfigElementEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigElementListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigElementEvent(this, prop_token, index, old_value);
            }
            ((ConfigElementListener)listeners[i+1]).propertyValueChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners that a new property value has been added to this
    * element.
    */
   protected void firePropertyValueAdded(String prop_token, int index,
                                         Object value)
   {
      ConfigElementEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigElementListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigElementEvent(this, prop_token, index, value);
            }
            ((ConfigElementListener)listeners[i+1]).propertyValueAdded(evt);
         }
      }
   }

   /**
    * Notifies listeners that a new property value has been removed from this
    * element.
    */
   protected void firePropertyValueRemoved(String prop_token, int index,
                                         Object old_value)
   {
      ConfigElementEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigElementListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigElementEvent(this, prop_token, index, old_value);
            }
            ((ConfigElementListener)listeners[i+1]).propertyValueRemoved(evt);
         }
      }
   }

   /** The name of this element. */
   private String mName;

   /** The definition of the format of this element. */
   private ConfigDefinition mDefinition;

   /** The properties in this configuration element. */
   private Map mProps;

   /** Listeners interested in this configuration element. */
   private EventListenerList listenerList = new EventListenerList();
}
