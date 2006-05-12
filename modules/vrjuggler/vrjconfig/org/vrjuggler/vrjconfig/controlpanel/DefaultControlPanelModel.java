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

package org.vrjuggler.vrjconfig.controlpanel;

import java.util.ArrayList;
import java.util.List;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import org.vrjuggler.vrjconfig.controlpanel.*;

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
   public void add(String label, Icon icon)
   {
      this.add(label, icon, null);
   }
   
   /**
    * Appends a new object to the end of this model.
    *
    * @param value      the object to append
    * @param icon       the icon to associate with the value
    * @param user_object the object to be associated with this item 
    */
   public void add(String label, Icon icon, Object user_object)
   {
      int index0 = getSize();
      data.add(new Item(label, icon, user_object));
      fireIntervalAdded(index0, index0);
   }

   public void add(ControlPanelNode node)
   {
      add(node.getLabel(), 
          node.getIcon(),
          node);
   }

   /**
    * Removes the given object and its icon from this model.
    */
   public void remove(String label)
   {
      int index0 = getIndexOf(label);
      if (index0 != -1)
      {
         data.remove(index0);
         fireIntervalRemoved(index0, index0);
      }
   }

   /**
    * Gets the label at the given index.
    *
    * @param index   the index of the label to retrieve
    */
   public String getLabelAt(int index)
   {
      return ((Item)data.get(index)).mLabel;
   }

   /**
    * Gets the object at the given index.
    *
    * @param index   the index of the object to retrieve
    */
   public Object getUserObjectAt(int index)
   {
      return ((Item)data.get(index)).mUserObject;
   }

   /**
    * Gets the icon at the given index.
    *
    * @param index   the index of the icon to retrieve
    */
   public Icon getIconAt(int index)
   {
      return ((Item)data.get(index)).mIcon;
   }

   /**
    * Gets the index of the given object in this model.
    *
    * @param value   the object to search for
    *
    * @return  the index if successful, -1 if value is not in the model
    */
   public int getIndexOf(String label)
   {
      for (int i=0; i<data.size(); ++i)
      {
         String elt_label = getLabelAt(i);
         if (label == elt_label)
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
      public Item(String label, Icon icon, Object user_object)
      {
         this.mLabel = label;
         this.mIcon = icon;
         this.mUserObject = user_object;
      }

      public Object mUserObject;
      public String mLabel;
      public Icon   mIcon;
   }
}
