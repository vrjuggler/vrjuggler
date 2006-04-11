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
 * A property definition provides a description of a single property within a
 * configuration element. A configuration definition is built up using property
 * definitions.
 *
 * @see ConfigDefinition
 */
public class PropertyDefinition
{
   /**
    * Creates a new PropertyDefinition initialized with the given values.
    */
   public PropertyDefinition(String name, String token, Class type,
                             String help, List propValueDefs, 
                             Map enums, List allowedTypes, boolean variable)
   {
      mName = name;
      mToken = token;
      mType = type;
      mHelp = help;
      mPropValueDefs = propValueDefs;
      mEnums = enums;
      mAllowedTypes = allowedTypes;
      mVariable = variable;
   }

   /**
    * Gets the name for this property definition.
    */
   public String getName()
   {
      return mName;
   }

   /**
    * Sets the name for this property definition.
    */
   public synchronized void setName(String name)
   {
      String old_name = mName;
      mName = name;
      fireNameChanged(old_name);
   }

   /**
    * Gets the token for this property definition.
    */
   public String getToken()
   {
      return mToken;
   }

   /**
    * Sets the token for this property definition.
    */
   public synchronized void setToken(String token)
   {
      String old_token = mToken;
      mToken = token;
      fireTokenChanged(old_token);
   }

   /**
    * Gets the value type for this property definition.
    */
   public Class getType()
   {
      return mType;
   }

   /**
    * Sets the value type for this property definition.
    */
   public synchronized void setType(Class type)
   {
      Class old_type = mType;
      mType = type;
      fireTypeChanged(old_type);
   }

   /**
    * Gets the help text for this property definition.
    */
   public String getHelp()
   {
      return mHelp;
   }

   /**
    * Sets the help text for this property definition.
    */
   public synchronized void setHelp(String help)
   {
      String old_help = mHelp;
      mHelp = help;
      fireHelpChanged(old_help);
   }

   /**
    * Gets the property value definition at the given index.
    */
   public synchronized PropertyValueDefinition getPropertyValueDefinition(int index)
      throws ArrayIndexOutOfBoundsException
   {
      return (PropertyValueDefinition)mPropValueDefs.get(index);
   }

   /**
    * Appends the property value definition to this property definition.
    */
   public synchronized void addPropertyValueDefinition(PropertyValueDefinition def)
   {
      int index = mPropValueDefs.size();
      mPropValueDefs.add(def);
      firePropertyValueDefinitionAdded(def, index);
   }

   /**
    * Removes the property value definition at the given index.
    */
   public synchronized void removePropertyValueDefinition(int index)
      throws ArrayIndexOutOfBoundsException
   {
      Object val = mPropValueDefs.remove(index);
      firePropertyValueDefinitionRemoved((PropertyValueDefinition)val, index);
   }

   /**
    * Gets a list of all the property value definitions for this property
    * definition.
    */
   public List getPropertyValueDefinitions()
   {
      return mPropValueDefs;
   }

   /**
    * Gets the number of property value definitions for this property
    * definition.
    */
   public synchronized int getPropertyValueDefinitionCount()
   {
      return mPropValueDefs.size();
   }

   /**
    * Adds a new enumeration to this property definition.
    *
    * @param name       the name of the enumeration to add
    * @param value      the value for the enumeration
    *
    * @throws IllegalArgumentException
    *             if an enumeration with that name already exists
    */
   public synchronized void addEnum(String name, Object value)
      throws IllegalArgumentException
   {
      if (mEnums.containsKey(name))
      {
         throw new IllegalArgumentException("Enumeration already exists: "+name);
      }
      mEnums.put(name, value);
      fireEnumAdded(name);
   }

   /**
    * Removes the given enumeration from this property definition. If the
    * enumeration does not exist, this object is not changed.
    */
   public synchronized void removeEnum(String name)
   {
      if (mEnums.containsKey(name))
      {
         mEnums.remove(name);
         fireEnumRemoved(name);
      }
   }

   /**
    * Gets a map of all the enumerations for this property definition.
    */
   public Map getEnums()
   {
      return mEnums;
   }

   /**
    * Gets the number of enumerations for this property definition.
    */
   public synchronized int getEnumsCount()
   {
      return mEnums.size();
   }

   /**
    * Gets the allowed type at the given index.
    */
   public synchronized String getAllowedType(int index)
      throws ArrayIndexOutOfBoundsException
   {
      return (String)mAllowedTypes.get(index);
   }

   /**
    * Adds a new allowed type to this property definition. If the type is
    * already in this property definition, it is not added a second time.
    *
    * @param type       the token of the type to add
    */
   public synchronized void addAllowedType(String type)
   {
      if (! mAllowedTypes.contains(type))
      {
         mAllowedTypes.add(type);
         fireAllowedTypeAdded(type);
      }
   }

   /**
    * Removes the given allowed type from this property definition. If the
    * allowed type does not exist, this object is not changed.
    */
   public synchronized void removeAllowedType(String type)
   {
      if (mAllowedTypes.contains(type))
      {
         mAllowedTypes.remove(type);
         fireAllowedTypeRemoved(type);
      }
   }

   /**
    * Gets a list of all the allowed types for this property definition.
    */
   public List getAllowedTypes()
   {
      return mAllowedTypes;
   }

   /**
    * Gets the number of allowed types for this property definition.
    */
   public synchronized int getAllowedTypesCount()
   {
      return mAllowedTypes.size();
   }

   /**
    * Tests if this property definition supports a variable number of values.
    */
   public boolean isVariable()
   {
      return mVariable;
   }

   /**
    * Sets whether this property definition supports a variable number of
    * values.
    */
   public synchronized void setVariable(boolean variable)
   {
      boolean old_variable = mVariable;
      mVariable = variable;
      fireVariableChanged(old_variable);
   }
   /**
    * Adds the given listener to be notified when this property definition
    * changes.
    */
   public void addPropertyDefinitionListener(PropertyDefinitionListener listener)
   {
      listenerList.add(PropertyDefinitionListener.class, listener);
   }

   /**
    * Tests if this property definition equals the given one.
    */
   public boolean equals(Object obj)
   {
      if (obj instanceof PropertyDefinition)
      {
         PropertyDefinition d = (PropertyDefinition)obj;
         if (mName.equals(d.mName) &&
             mToken.equals(d.mToken) &&
             mType.equals(d.mType) &&
             mHelp.equals(d.mHelp) &&
             mPropValueDefs.equals(d.mPropValueDefs) &&
             mVariable == d.mVariable)
         {
            return true;
         }
      }
      return false;
   }

   /**
    * Removes the given listener that was registered to be notified when this
    * property definition changed.
    */
   public void removePropertyDefinitionListener(PropertyDefinitionListener listener)
   {
      listenerList.remove(PropertyDefinitionListener.class, listener);
   }

   /**
    * Notifies listeners that this definition's name has changed.
    */
   protected void fireNameChanged(String old_name)
   {
      PropertyDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == PropertyDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new PropertyDefinitionEvent(this, old_name);
            }
            ((PropertyDefinitionListener)listeners[i+1]).nameChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners that this definition's token has changed.
    */
   protected void fireTokenChanged(String old_token)
   {
      PropertyDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == PropertyDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new PropertyDefinitionEvent(this, old_token);
            }
            ((PropertyDefinitionListener)listeners[i+1]).tokenChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners that this definition's type has changed.
    */
   protected void fireTypeChanged(Class old_type)
   {
      PropertyDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == PropertyDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new PropertyDefinitionEvent(this, old_type);
            }
            ((PropertyDefinitionListener)listeners[i+1]).typeChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners that this definition's help has changed.
    */
   protected void fireHelpChanged(String old_help)
   {
      PropertyDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == PropertyDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new PropertyDefinitionEvent(this, old_help);
            }
            ((PropertyDefinitionListener)listeners[i+1]).helpChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners that a property value definition has been added to
    * this definition.
    */
   protected void firePropertyValueDefinitionAdded(PropertyValueDefinition def, int index)
   {
      PropertyDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == PropertyDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new PropertyDefinitionEvent(this, def);
            }
            ((PropertyDefinitionListener)listeners[i+1]).propertyValueDefinitionAdded(evt);
         }
      }
   }

   /**
    * Notifies listeners that a property value definition has been removed from
    * this definition.
    */
   protected void firePropertyValueDefinitionRemoved(PropertyValueDefinition def, int index)
   {
      PropertyDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == PropertyDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new PropertyDefinitionEvent(this, def);
            }
            ((PropertyDefinitionListener)listeners[i+1]).propertyValueDefinitionRemoved(evt);
         }
      }
   }

   /**
    * Notifies listeners that an enumeration has been added to this definition.
    */
   protected void fireEnumAdded(String name)
   {
      PropertyDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == PropertyDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new PropertyDefinitionEvent(this, name);
            }
            ((PropertyDefinitionListener)listeners[i+1]).enumAdded(evt);
         }
      }
   }

   /**
    * Notifies listeners that an enumeration has been removed to this definition.
    */
   protected void fireEnumRemoved(String name)
   {
      PropertyDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == PropertyDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new PropertyDefinitionEvent(this, name);
            }
            ((PropertyDefinitionListener)listeners[i+1]).enumRemoved(evt);
         }
      }
   }

   /**
    * Notifies listeners that an allowed type has been added to this definition.
    */
   protected void fireAllowedTypeAdded(String type)
   {
      PropertyDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == PropertyDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new PropertyDefinitionEvent(this, type);
            }
            ((PropertyDefinitionListener)listeners[i+1]).allowedTypeAdded(evt);
         }
      }
   }

   /**
    * Notifies listeners that an allowed type has been removed from this
    * definition.
    */
   protected void fireAllowedTypeRemoved(String type)
   {
      PropertyDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == PropertyDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new PropertyDefinitionEvent(this, type);
            }
            ((PropertyDefinitionListener)listeners[i+1]).allowedTypeRemoved(evt);
         }
      }
   }

   /**
    * Notifies listeners that this definition's variable status has changed.
    */
   protected void fireVariableChanged(boolean old_variable)
   {
      PropertyDefinitionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == PropertyDefinitionListener.class)
         {
            if (evt == null)
            {
               evt = new PropertyDefinitionEvent(this, old_variable);
            }
            ((PropertyDefinitionListener)listeners[i+1]).variableChanged(evt);
         }
      }
   }

   /** The name of this property definition. */
   private String mName;

   /** The token of this property definition. */
   private String mToken;

   /**
    * The type of object that can be stored in a property that is defined with
    * this property definition.
    */
   private Class mType;

   /** The help text for this property definition. */
   private String mHelp;

   /** A list of the property value definitions for this property definition. */
   private List mPropValueDefs;

   /** A map of all the enumerations for this property definition. */
   private Map mEnums;

   /**
    * A list of all the allowed types. This is only used if the value type is
    * ChunkPointer.class.
    */
   private List mAllowedTypes;

   /**
    * A flag determining whether this property definition defines a property
    * that will support a variable number of items.
    */
   private boolean mVariable;

   /** The listeners interested in this property definition. */
   private EventListenerList listenerList = new EventListenerList();
}
