/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.wizard.builder;

import java.beans.*;
import java.lang.reflect.*;
import java.util.ArrayList;
import java.util.List;
import javax.swing.*;
import javax.swing.table.*;

/**
 * Table model for a property sheet.
 */
public class PropertySheetTableModel
   extends AbstractTableModel
   implements PropertyChangeListener
{
   private String[] mColNames = { "Property", "Value" };
   private List mData = new ArrayList();
   private Object bean = null;

   /**
    * Sets the bean this property sheet model is describing.
    */
   public void setBean(Object bean)
   {
      releaseBeanResources();
      this.bean = bean;
      introspect();
   }

   /**
    * Gets the bean this property sheet model is describing.
    */
   public Object getBean()
   {
      return bean;
   }

   public String getColumnName(int col)
   {
      return mColNames[col];
   }

   public synchronized int getRowCount()
   {
      return mData.size();
   }

   public int getColumnCount()
   {
      return mColNames.length;
   }

   /**
    * Gets the class type of the property corresponding to the given row.
    */
   public synchronized Class getPropertyType(int row)
   {
      PropertyDescriptor prop = (PropertyDescriptor)mData.get(row);
      return prop.getPropertyType();
   }

   public synchronized Object getValueAt(int row, int col)
   {
      PropertyDescriptor prop = (PropertyDescriptor)mData.get(row);
      if (col == 0)
      {
         return prop.getName();
      }
      else
      {
         Object value = null;
         try
         {
            // Get the property's value
            Method getter = prop.getReadMethod();
            value = getter.invoke(bean, new Object[] {});
         }
         catch (InvocationTargetException ite)
         {
            ite.getTargetException().printStackTrace();
         }
         catch (Exception ex)
         {
            ex.printStackTrace();
         }
         return value;
      }
   }

   public boolean isCellEditable(int row, int col)
   {
      return (col != 0);
   }

   public synchronized void setValueAt(Object value, int row, int col)
   {
      if (col == 1)
      {
         PropertyDescriptor prop = (PropertyDescriptor)mData.get(row);
         try
         {
            // Set the property's value
            Object[] args = { value };
            Method setter = prop.getWriteMethod();
            setter.invoke(bean, args);
         }
         catch (InvocationTargetException ite)
         {
            ite.getTargetException().printStackTrace();
         }
         catch (Exception ex)
         {
            ex.printStackTrace();
         }
         fireTableCellUpdated(row, col);
      }
      fireTableCellUpdated(row, col);
   }

   /**
    * Clears all values out of this model.
    */
   public synchronized void clear()
   {
      int oldNumRows = getRowCount();
      mData.clear();
      if (oldNumRows > 0)
      {
         fireTableRowsDeleted(0, oldNumRows - 1);
      }
   }

   /**
    * Called when the current bean has had one of it's properties changed. This
    * will notify all listeners that the particular table cell has changed.
    */
   public void propertyChange(PropertyChangeEvent evt)
   {
      // Sanity check. Is this coming from the bean we are currently editing??
      if (evt.getSource() != bean)
      {
         System.err.println("WARNING: We are getting events from an unknown bean");
         return;
      }

      // Get the row that corresponds to this property
      String name = evt.getPropertyName();
      for (int i=0; i<mData.size(); ++i)
      {
         PropertyDescriptor desc = (PropertyDescriptor)mData.get(i);
         if (desc.getName().equals(name))
         {
            fireTableCellUpdated(i, 1);
         }
      }
   }

   /**
    * Query the bean for its properties and the events it fires.
    */
   private void introspect()
   {
      PropertyDescriptor[] props;
      EventSetDescriptor[] events;

      // Clear the table model
      clear();

      // Only query properties if the bean is not null!
      if (bean == null)
      {
         return;
      }

      // Try to get info on the bean's properties.
      try
      {
         BeanInfo bi = Introspector.getBeanInfo(bean.getClass());
         props = bi.getPropertyDescriptors();
      }
      catch (IntrospectionException ie)
      {
         ie.printStackTrace();
         return;
      }

      // Process each discovered property
      for (int i=0; i<props.length; ++i)
      {
         PropertyDescriptor desc = props[i];
         Method getter = desc.getReadMethod();
         Method setter = desc.getWriteMethod();

         // We only care about read/read properties
         if (getter == null || setter == null)
         {
            continue;
         }

         // Ignore hidden properties
         if (desc.isHidden())
         {
            continue;
         }

         // add the property to the model
         int pos = mData.size();
         mData.add(desc);
         fireTableRowsInserted(pos, pos);
      }

      // Try to get info on the bean's event support
      try
      {
         BeanInfo bi = Introspector.getBeanInfo(bean.getClass());
         events = bi.getEventSetDescriptors();
      }
      catch (IntrospectionException ie)
      {
         ie.printStackTrace();
         return;
      }

      // Proccess each discovered event set
      for (int i=0; i<events.length; ++i)
      {
         EventSetDescriptor desc = events[i];

         // Add ourselves as a listener to anything we can handle
         Method adder = desc.getAddListenerMethod();
         Class listenerClass = desc.getListenerType();

         if (adder == null)
         {
            continue;
         }

         if (listenerClass.isInstance(this))
         {
            System.out.println("Listening to "+desc.getName());
            // Make ourself a listener of the bean
            try
            {
               adder.invoke(bean, new Object[] { this });
            }
            catch (InvocationTargetException ite)
            {
               ite.getTargetException().printStackTrace();
            }
            catch (Exception ex)
            {
               ex.printStackTrace();
            }
         }
      }
   }

   /**
    * Stop listening to the current bean, etc.
    */
   private void releaseBeanResources()
   {
      EventSetDescriptor[] events;

      // Nothing to do if the bean is null
      if (bean == null)
      {
         return;
      }

      // Stop listening to this bean
      try
      {
         BeanInfo bi = Introspector.getBeanInfo(bean.getClass());
         events = bi.getEventSetDescriptors();
      }
      catch (IntrospectionException ie)
      {
         ie.printStackTrace();
         return;
      }

      // Process each discovered event set
      for (int i=0; i<events.length; ++i)
      {
         EventSetDescriptor desc = events[i];

         // Be safe and remove ourselves from each possible event type that
         // we could possibly care about.
         Method remover = desc.getRemoveListenerMethod();
         Class listenerClass = desc.getListenerType();

         if (remover == null)
         {
            continue;
         }

         // Only do removal if we are an instance of the listener
         if (listenerClass.isInstance(this))
         {
            try
            {
               remover.invoke(bean, new Object[] { listenerClass });
            }
            catch (InvocationTargetException ite)
            {
               ite.getTargetException().printStackTrace();
            }
            catch (Exception ex)
            {
               ex.printStackTrace();
            }
         }
      }
   }
}
