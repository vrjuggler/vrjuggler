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

package org.vrjuggler.vrjconfig.ui;

import java.util.ArrayList;
import java.util.List;
import javax.swing.Icon;

/**
 * This is an implementation of the ControlPanelModel that uses a List to store
 * the cell value objects.
 */
public class DefaultControlPanelModel
   extends AbstractControlPanelModel
{
   /**
    * Creates a new, empty model.
    */
   public DefaultControlPanelModel()
   {
      data = new ArrayList();
   }

   /**
    * Appends a new object to the end of this model.
    *
    * @param value      the object to append
    * @param icon       the icon to associate with the value
    */
   public void add(Object value, Icon icon)
   {
      int index0 = getSize();
      data.add(new Item(value, icon));
      fireIntervalAdded(index0, index0);
   }

   /**
    * Removes the given object and its icon from this model.
    */
   public void remove(Object value)
   {
      int index0 = getIndexOf(value);
      if (index0 != -1)
      {
         data.remove(index0);
         fireIntervalRemoved(index0, index0);
      }
   }

   /**
    * Gets the object at the given index.
    *
    * @param index   the index of the object to retrieve
    */
   public Object getElementAt(int index)
   {
      return ((Item)data.get(index)).value;
   }

   /**
    * Gets the icon at the given index.
    *
    * @param index   the index of the icon to retrieve
    */
   public Icon getIconAt(int index)
   {
      return ((Item)data.get(index)).icon;
   }

   /**
    * Gets the index of the given object in this model.
    *
    * @param value   the object to search for
    *
    * @return  the index if successful, -1 if value is not in the model
    */
   public int getIndexOf(Object value)
   {
      for (int i=0; i<data.size(); ++i)
      {
         Object elt_value = getElementAt(i);
         if (value == elt_value)
         {
            return i;
         }
      }
      return -1;
   }

   /**
    * Gets the number of objects in this model.
    */
   public int getSize()
   {
      return data.size();
   }

   /**
    * The list items in this model.
    */
   private List data;

   /**
    * Internal class to pair values with their icons.
    */
   private class Item
   {
      public Item(Object value, Icon icon)
      {
         this.value = value;
         this.icon = icon;
      }

      public Object value;
      public Icon icon;
   }
}
