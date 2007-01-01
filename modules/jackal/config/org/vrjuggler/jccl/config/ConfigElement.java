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

import java.util.*;
import javax.swing.event.EventListenerList;

import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.jccl.config.undo.*;


/**
 * A configuration element is a logical collection of configuration information
 * for a particular system component. It stores a number of properties that
 * describe the configuration.
 */
public class ConfigElement implements ConfigElementPointerListener
{
   /**
    * Create a new configuration element with the given properties.
    */
   public ConfigElement(ConfigDefinition def, String name, Map props)
   {
      mName = name;
      mDefinition = def;
      mProps = props;
      mReadOnly = false;

      // Iterate over all properties to find ConfigElementPointers
      Iterator itr = props.values().iterator();
      while(itr.hasNext())
      {
         Iterator list_itr = ((List)itr.next()).iterator();
         while(list_itr.hasNext())
         {
            Object prop = list_itr.next();
            // If the new property is a ConfigElementPointer, add alistener to
            // relay the change events.
            if(prop instanceof ConfigElementPointer)
            {
               ((ConfigElementPointer)prop).addConfigElementPointerListener(this);
            }
         }
      }
   }

   /**
    * Constucts a ConfigElement that is identical to the given ConfigElement.
    */
   public ConfigElement(ConfigElement oldElm)
   {
      mName = new String(oldElm.mName);
      mDefinition = oldElm.mDefinition;
      mReadOnly = false;

      mProps = new TreeMap();

      Set map_set = oldElm.mProps.entrySet();

      for(Iterator itr = map_set.iterator() ; itr.hasNext() ; )
      {
         Map.Entry entry = (Map.Entry)itr.next();

         List values = new ArrayList();

         String prop_token = (String)entry.getKey();

         for( Iterator list_iterator = ((List)entry.getValue()).iterator();
              list_iterator.hasNext(); )
         {
            Object obj = list_iterator.next();

            if(obj instanceof Boolean)
            {
               Boolean temp = new Boolean( ((Boolean)obj).booleanValue() );
               values.add(temp);
            }
            else if(obj instanceof Integer)
            {
               Integer temp = new Integer( ((Integer)obj).intValue() );
               values.add(temp);
            }
            else if(obj instanceof Float)
            {
               Float temp = new Float( ((Float)obj).floatValue() );
               values.add(temp);
            }
            else if(obj instanceof String)
            {
               String temp = new String((String)obj);
               values.add(temp);
            }
            else if(obj instanceof ConfigElementPointer)
            {
               ConfigElementPointer temp =
                  new ConfigElementPointer(
                     ((ConfigElementPointer) obj).getTarget()
                  );
               values.add(temp);
            }
            else if(obj instanceof ConfigElement)
            {
               ConfigElement temp = new ConfigElement((ConfigElement)obj);
               values.add(temp);
            }
         }
         mProps.put(prop_token, values);
      }
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
    * Sets the name of this configuration element.  Then, a
    * ConfigElementNameEdit is automatically created and registered with the
    * UndoManager associated with the given ConfigContext.
    *
    * @param name       the new name for the configuration element
    * @param ctx        the context
    *
    * @see #setName(String)
    *
    * @since 0.92.6
    */
   public synchronized void setName(String name, ConfigContext ctx)
   {
      String old_name = mName;
      ConfigElementNameEdit new_edit =
         new ConfigElementNameEdit(this, old_name, name);
      setName(name);
      ctx.postEdit(new_edit);
   }

   /**
    * Gets the full name of this configuration element.
    *
    * NOTE:Currently this is not implemented!!!!
    */
   public String getFullName()
   {
      return mName;
   }

   /**
    * Returns true if the ConfigElement is read only.
    */
   public boolean isReadOnly()
   {
      return mReadOnly;
   }

   /**
    * Specify if ConfigElement is read only.
    */
   public void setReadOnly(boolean readOnly)
   {
      mReadOnly = readOnly;
   }

   /**
    * Gets the object that defines the contents of this element.
    */
   public ConfigDefinition getDefinition()
   {
      return mDefinition;
   }

   /**
    * Gets the value of the property with the given name. The type of the
    * result depends on the type of the property as described by this element's
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
      throws IllegalArgumentException
           , ArrayIndexOutOfBoundsException
   {
      // Get the particular property value
      return getPropertyValues(name).get(index);
   }

   /**
    * Sets the value for the property with the given name at the given index
    * only after first ensuring that the new value is different from the
    * current value.  Then, a ConfigElementPropertyEdit is automatically
    * created and registered with the UndoManager associated with the given
    * context.
    *
    * @param name    the name of the property to set
    * @param index   the index of the property value to set
    * @param value   the enw value for the property index
    * @param ctx     the context
    *
    * @see #setProperty(String, int, Object)
    */
   public synchronized void setProperty(String name, int index, Object value,
                                        ConfigContext ctx)
   {
      Object old_value = getProperty(name, index);

      if (old_value instanceof ConfigElementPointer)
      {
         old_value = ((ConfigElementPointer)old_value).getTarget();
         if (null == old_value)
         {
            old_value = "";
         }
      }
      // Make sure that the value actually changed.
      if ( !old_value.equals(value) )
      {
         ConfigElementPropertyEdit new_edit =
            new ConfigElementPropertyEdit(this, name, index, old_value, value);
         setProperty(name, index, value);
         ctx.postEdit(new_edit);
      }
   }

   /**
    * Sets the value for the property with the given name at the given index.
    * The type of <code>value</code> must match the type of the property as
    * specified by the property definition for the named property.  The mapping
    * is fairly straightfoward:
    *
    * <ul>
    *   <li>Property type string: <code>value</code> type must be
    *       java.lang.String</li>
    *   <li>Property type integer: <code>value</code> type must be
    *       java.lang.Integer</li>
    *   <li>Property type float: <code>value</code> type must be
    *       java.lang.Float</li>
    *   <li>Property type config element (i.e., embedded element):
    *       <code>value</code> type must be ConfigElement</li>
    *   <li>Property type config element pointer: <code>value</code> type
    *       must be java.lang.String or ConfigElementPointer<br>
    *   </li>
    * </ul>
    *
    * @param name    the name of the property to set
    * @param index   the index of the property value to set
    * @param value   the new value for the property index
    */
   public synchronized void setProperty(String name, int index, Object value)
      throws IllegalArgumentException
           , ArrayIndexOutOfBoundsException
   {
      // Get the list of property values
      List values = getPropertyValues(name);

      // TODO: Validate that value is of the correct type for this property.
      Object old_value = values.get(index);

      if (old_value instanceof ConfigElementPointer)
      {
         ConfigElementPointer cep = (ConfigElementPointer)old_value;
         old_value = cep.getTarget();
         if (null == old_value)
         {
            old_value = "";
         }

         if ( value instanceof String )
         {
            cep.setTarget((String) value);
         }
         else
         {
            cep.setTarget(((ConfigElementPointer) value).getTarget());
         }
      }
      else
      {
         // Set the value in the list
         values.set(index, value);
      }

      // Notify listeners of the change
      firePropertyValueChanged(name, index, old_value);
      System.out.println(getName() + " setProperty("+name+","+index+","+value+")");
   }

   /**
    * Appends the given value to the list of values for this property. This
    * only applies if the property supports a variable number of values.
    * Then, a ConfigElementPropertyValueAddEdit is automatically created and
    * registered with the UndoManager associated with the given context.
    *
    * @param name    the name of the property to set
    * @param value   the enw value for the property index
    * @param ctx     the context
    *
    * @return The index of the newly added property in the property list.
    *
    * @see #addProperty(String, int, Object)
    */
   public synchronized int addProperty(String name, Object value,
                                       ConfigContext ctx)
   {
      List values = getPropertyValues(name);
      return addProperty(name, values.size(), value, ctx);
   }

   /**
    * Inserts the given value to the list of values for this property at the
    * specified index.  If the given index is larger than the current list of
    * values, the new value is simply appended to the list of values.  This
    * only applies if the property supports a variable number of values.
    * Then, a ConfigElementPropertyValueAddEdit is automatically created and
    * registered with the UndoManager associated with the given context.
    *
    * @param name    the name of the property to set
    * @param index   the index in the property list where this value will be
    *                inserted
    * @param value   the enw value for the property index
    * @param ctx     the context
    *
    * @return The index of the newly added property in the property list.
    *
    * @see #addProperty(String, int, Object)
    */
   public synchronized int addProperty(String name, int index, Object value,
                                       ConfigContext ctx)
   {
      int added_index = addProperty(name, index, value);
      ctx.postEdit(new ConfigElementPropertyValueAddEdit(this, name,
                                                         added_index, value));

      return added_index;
   }

   /**
    * Appends the given value to the list of values for this property. This
    * only applies if the property supports a variable number of values.
    *
    * @param name    the name of the property to set
    * @param value   the property value to add
    *
    * @return The index of the newly added property in the property list.
    *
    * @see #addProperty(String, int, Object)
    */
   public synchronized int addProperty(String name, Object value)
      throws IllegalArgumentException
   {
      List values = getPropertyValues(name);
      return addProperty(name, values.size(), value);
   }

   /**
    * Inserts the given value to the list of values for this property at the
    * specified index.  If the given index is larger than the current list of
    * values, the new value is simply appended to the list of values.  This
    * only applies if the property supports a variable number of values.
    *
    * @param name    the name of the property to set
    * @param index   the index into the property list
    * @param value   the property value to add
    *
    * @return The index of the newly added property in the property list.
    */
   public synchronized int addProperty(String name, int index, Object value)
      throws IllegalArgumentException
   {
      // Get the list of property values
      List values = getPropertyValues(name);

      // Verify that this property supports variable values
      PropertyDefinition prop_def = mDefinition.getPropertyDefinition(name);
      if (! prop_def.isVariable())
      {
         throw new IllegalArgumentException(name + " does not support a " +
                                            "variable number of values");
      }

      // TODO: Validate that value is of the correct type for this property

      int added_index;

      // Add the value to the list.  If the given index is greater than or
      // equal to the current size of the list, simply append the new value.
      // XXX: It might be useful to allow the list to grow to accomodate the
      // an index that is larger than the current size of the list.
      if ( index >= values.size() )
      {
         added_index = values.size();
         values.add(value);
      }
      else
      {
         added_index = index;
         values.add(index, value);
      }

      // If the new property is a ConfigElementPointer, add alistener to relay
      // the change events.
      if(value instanceof ConfigElementPointer)
      {
         ((ConfigElementPointer)value).addConfigElementPointerListener(this);
      }

      // Notify listeners of the addition
      firePropertyValueAdded(name, index, value);

      return added_index;
   }

   /**
    * Removes the value at the given index for the given property. This only
    * applies if the property supports a variable number of values.  Then, a
    * ConfigElementPropertyValueRemoveEdit is automatically created and
    * registered with the UndoManager associated with the given context.
    *
    * @param name    the name of the property to set
    * @param index   the index of the property value to set
    * @param ctx     the context
    *
    * @return The index of the property removed is returned.  -1 is returned
    *         if the given value is not in the list of values for the named
    *         property.
    */
   public synchronized int removeProperty(String name, int index,
                                          ConfigContext ctx)
   {
      List values = getPropertyValues(name);
      Object old_value = values.get(index);
      int removed_index = removeProperty(name, index);

      if ( removed_index >= 0 )
      {
         ctx.postEdit(new ConfigElementPropertyValueRemoveEdit(this, name,
                                                               removed_index,
                                                               old_value));
      }

      return removed_index;
   }

   /**
    * Removes the given value from the list of values associated with the
    * named property.  The first occurrence of the given value is removed if
    * it exists in the list of values for the named property.  This only
    * applies if the property supports a variable number of values.  Then, a
    * ConfigElementPropertyValueRemoveEdit is automatically created and
    * registered with the UndoManager associated with the given context.
    *
    * @param name     the name of the property to modify
    * @param oldValue the value to remove
    * @param ctx      the context
    *
    * @return The index of the property removed is returned.  -1 is returned
    *         if the given value is not in the list of values for the named
    *         property.
    *
    * @see #removeProperty(String, int)
    */
   public synchronized int removeProperty(String name, Object oldValue,
                                          ConfigContext ctx)
   {
      List values = getPropertyValues(name);
      int index   = values.indexOf(oldValue);

      if ( index != -1 )
      {
         index = removeProperty(name, index, ctx);
      }

      return index;
   }

   /**
    * Removes the value at the given index for the given property. This only
    * applies if the property supports a variable number of values.
    *
    * @param name    the name of the property to modify
    * @param index   the index of the property value to remove
    *
    * @return The index of the property removed is returned.  -1 is returned
    *         if the given value is not in the list of values for the named
    *         property.
    */
   public synchronized int removeProperty(String name, int index)
      throws IllegalArgumentException
           , ArrayIndexOutOfBoundsException
   {
      // Get the list of property values
      List values = getPropertyValues(name);

      // Verify that this property supports variable values
      PropertyDefinition prop_def = mDefinition.getPropertyDefinition(name);
      if (! prop_def.isVariable())
      {
         throw new IllegalArgumentException(
            name + " does not support a variable number of values"
         );
      }

      // TODO: Validate that value is of the correct type for this property

      // Remove the value from the list
      Object old_value = values.remove(index);

      // Notify listeners of the removal
      firePropertyValueRemoved(name, index, old_value);

      return index;
   }

   /**
    * Removes the given value from the list of values associated with the
    * named property.  The first occurrence of the given value is removed if
    * it exists in the list of values for the named property.  This only
    * applies if the property supports a variable number of values.
    *
    * @param name     the name of the property to modify
    * @param oldValue the value to remove
    *
    * @return The index of the property removed is returned.  -1 is returned
    *         if the given value is not in the list of values for the named
    *         property.
    *
    * @see #removeProperty(String, int)
    */
   public synchronized int removeProperty(String name, Object oldValue)
      throws IllegalArgumentException
           , ArrayIndexOutOfBoundsException
   {
      // Get the list of property values
      List values = getPropertyValues(name);

      int index = values.indexOf(oldValue);

      // Only perform the removal if the given value is actually in the list
      // of values.
      if ( index != -1 )
      {
         removeProperty(name, index);
      }

      return index;
   }

   /**
    * Changes the index of the given property value to <code>newIndex</code>
    * within the named property.  This only applies if the property supports
    * a variable number of values.  Then, a
    * <code>ConfigElementPropertyValueIndexEdit</code> is automatically
    * created and registered with the UndoManager associated with the given
    * context.
    *
    * @param name       the name of the property to set
    * @param value      the value whose index will be changed
    * @param newIndex   the index of the property value to set
    * @param ctx        the context
    *
    * @throws IllegalArgumentException
    *             if this element does not have a property by that name, if
    *             <code>newIndex</code> is invalid, or if the named property
    *             does not support a variable number of values
    *
    * @see #setPropertyValueIndex(String,int,int,ConfigContext)
    *
    * @since 0.92.11
    */
   public synchronized void setPropertyValueIndex(String name, Object value,
                                                  int newIndex,
                                                  ConfigContext ctx)
      throws IllegalArgumentException
   {
      setPropertyValueIndex(name, getPropertyValueIndex(name, value),
                            newIndex, ctx);
   }

   /**
    * Changes the index of the identified property value from
    * <code>oldValue</code> to <code>newValue</code> within the named
    * property.  This only applies if the property supports a variable number
    * of values.  Then, a <code>ConfigElementPropertyValueIndexEdit</code> is
    * automatically created and registered with the UndoManager associated
    * with the given context.
    *
    * @param name       the name of the property to set
    * @param oldIndex   the value whose index will be changed
    * @param newIndex   the index of the property value to set
    * @param ctx        the context
    *
    * @throws IllegalArgumentException
    *             if this element does not have a property by that name, if
    *             <code>newIndex</code> is invalid, or if the named property
    *             does not support a variable number of values
    *
    * @since 0.92.11
    */
   public synchronized void setPropertyValueIndex(String name, int oldIndex,
                                                  int newIndex,
                                                  ConfigContext ctx)
      throws IllegalArgumentException
   {
      if ( oldIndex != newIndex )
      {
         try
         {
            setPropertyValueIndex(name, oldIndex, newIndex);
            ctx.postEdit(new ConfigElementPropertyValueIndexEdit(this, name,
                                                                 oldIndex,
                                                                 newIndex));
         }
         catch (IllegalArgumentException ex)
         {
            throw ex;
         }
      }
   }

   /**
    * Changes the index of the given property value to <code>newIndex</code>
    * within the named property.  This only applies if the property supports
    * a variable number of values.
    *
    * @param name       the name of the property to set
    * @param value      the value whose index will be changed
    * @param newIndex   the index of the property value to set
    *
    * @throws IllegalArgumentException
    *             if this element does not have a property by that name, if
    *             <code>newIndex</code> is invalid, or if the named property
    *             does not support a variable number of values
    *
    * @see #setPropertyValueIndex(String,int,int)
    *
    * @since 0.92.11
    */
   public synchronized void setPropertyValueIndex(String name, Object value,
                                                  int newIndex)
      throws IllegalArgumentException
   {
      setPropertyValueIndex(name, getPropertyValueIndex(name, value),
                            newIndex);
   }

   /**
    * Changes the index of the identified property value from
    * <code>oldValue</code> to <code>newValue</code> within the named
    * property.  This only applies if the property supports a variable number
    * of values.
    *
    * @param name       the name of the property to set
    * @param oldIndex   the value whose index will be changed
    * @param newIndex   the index of the property value to set
    *
    * @throws IllegalArgumentException
    *             if this element does not have a property by that name, if
    *             <code>newIndex</code> is invalid, or if the named property
    *             does not support a variable number of values
    *
    * @since 0.92.11
    */
   public synchronized void setPropertyValueIndex(String name, int oldIndex,
                                                  int newIndex)
      throws IllegalArgumentException
   {
      PropertyDefinition prop_def = getDefinition().getPropertyDefinition(name);

      if ( ! prop_def.isVariable() )
      {
         throw new IllegalArgumentException(
            "Value reordering not allowed for property type " + name +
            " which has a static value count and ordering"
         );
      }

      int value_count = getPropertyValueCount(name);
      if ( oldIndex >= value_count || newIndex >= value_count )
      {
         throw new IllegalArgumentException(
            "Value indices out of range [0," + (value_count - 1) + "]: " +
            "oldIndex == " + oldIndex + ", newIndex == " + newIndex
         );
      }

      if ( oldIndex != newIndex )
      {
         List prop_values = getPropertyValues(name);
         Object value = prop_values.remove(oldIndex);
         prop_values.add(newIndex, value);

         firePropertyValueOrderChanged(name, oldIndex, newIndex);
      }
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
    * Returns the index of the given value in the list of values for the
    * named property or -1 if the named property has no such value.  The
    * value lookup is performed using reference equality rather than object
    * equality.  Hence, <code>value</code> must be the actual object reference
    * that is among the values for the named property.
    *
    * @param name       the name of the property to retrieve
    * @param value      the value whose index will be returned
    *
    * @throws IllegalArgumentException
    *             if this element does not have a property by that name.
    *
    * @since 0.92.11
    */
   public synchronized int getPropertyValueIndex(String name, Object value)
      throws IllegalArgumentException
   {
      List values = getPropertyValues(name);

      int value_index = -1;
      for ( int v = 0; v < values.size(); ++v )
      {
         Object cur_value = values.get(v);

         if ( cur_value == value )
         {
            value_index = v;
            break;
         }
      }

      return value_index;
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
    * 
    * @throws IllegalArgumentException
    *             if listener is null
    */
   public void addConfigElementListener(ConfigElementListener listener)
      throws IllegalArgumentException
   {
      if (null == listener)
      {
         throw new IllegalArgumentException("Can not add a null ConfigElementListener.");
      }

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
   protected void fireNameChanged(String oldName)
   {
      ConfigElementEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigElementListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigElementEvent(this, null, 0, oldName);
            }
            ((ConfigElementListener)listeners[i+1]).nameChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners that this element's property has changed.
    */
   protected void firePropertyValueChanged(String propToken, int index,
                                           Object oldValue)
   {
      // Get the new value of the changed property.
      List values = getPropertyValues(propToken);
      Object new_value = values.get(index);

      if (new_value instanceof ConfigElementPointer)
      {
         new_value = ((ConfigElementPointer)new_value).getTarget();
      }


      // If the value did not actually change, then do not fire the change
      // event.
      if ( ! oldValue.equals(new_value) )
      {
         ConfigElementEvent evt = null;
         Object[] listeners = listenerList.getListenerList();
         for (int i=listeners.length-2; i>=0; i-=2)
         {
            if (listeners[i] == ConfigElementListener.class)
            {
               if (evt == null)
               {
                  evt = new ConfigElementEvent(this, propToken, index,
                                               oldValue);
               }
               ((ConfigElementListener)listeners[i+1]).propertyValueChanged(evt);
            }
         }
      }
   }

   /**
    * Notifies listeners that the ordering of the named property has changed
    * within the given range.  The "index0" and "index1" values do not have
    * to be the respective start and end of that range, but one must be the
    * start and one the end.
    */
   protected void firePropertyValueOrderChanged(String propToken, int index0,
                                                int index1)
   {
      ConfigElementEvent evt = null;
      Object[] listeners = listenerList.getListenerList();

      for ( int i = listeners.length - 2; i >= 0; i -= 2 )
      {
         if ( listeners[i] == ConfigElementListener.class )
         {
            if (evt == null)
            {
               evt = new ConfigElementEvent(this, propToken, index0, index1);
            }

            ((ConfigElementListener) listeners[i + 1]).propertyValueOrderChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners that a new property value has been added to this
    * element.
    */
   protected void firePropertyValueAdded(String propToken, int index,
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
               evt = new ConfigElementEvent(this, propToken, index, value);
            }
            ((ConfigElementListener)listeners[i+1]).propertyValueAdded(evt);
         }
      }
   }

   /**
    * Notifies listeners that a new property value has been removed from this
    * element.
    */
   protected void firePropertyValueRemoved(String propToken, int index,
                                           Object oldValue)
   {
      ConfigElementEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigElementListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigElementEvent(this, propToken, index, oldValue);
            }
            ((ConfigElementListener)listeners[i+1]).propertyValueRemoved(evt);
         }
      }
   }

   public void targetChanged(ConfigElementPointerEvent evt)
   {
      // XXX: Relaying this event should not be needed anymore
      //      since all target changes should be treated just
      //      like property changes.
   }

   /** The name of this element. */
   private String mName;

   /** Is the ConfigElement read only. */
   private boolean mReadOnly;

   /** The definition of the format of this element. */
   private ConfigDefinition mDefinition;

   /** The properties in this configuration element. */
   private Map mProps;

   /** Listeners interested in this configuration element. */
   private EventListenerList listenerList = new EventListenerList();
}
