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

package org.vrjuggler.vrjconfig.controlpanel;

import javax.swing.Icon;
import javax.swing.event.ListDataListener;

/**
 * This interface defines the methods components like ControlPanel use to get
 * the value of each cell in the control panel and the number of items in the
 * control panel. Any change to the contents or length of the data model must be
 * reported to all of the ControlPanelDataListeners.
 *
 * @see ControlPanel
 */
public interface ControlPanelModel
{
   /**
    * Gets the element at the given index in the model.
    */
   public String getLabelAt(int index);

   /**
    * Gets the element at the given index in the model.
    */
   public Object getUserObjectAt(int index);
   
   /**
    * Gets the icon for the element at the given index.
    */
   public Icon getIconAt(int index);

   /**
    * Gets the index of the given value in this model.
    *
    * @param value     the value to search for
    *
    * @return  the index of the object if it is contained in the model; -1 if
    *          <code>value</code> is not in the model
    */
   public int getIndexOf(String label);

   /**
    * Gets the number of elements in this model.
    */
   public int getSize();

   /**
    * Registers the given listener to be notified when this model changes.
    */
   public void addListDataListener(ListDataListener listener);

   /**
    * Unregisters the given listeners so that it will no longer be notified
    * when this model changes.
    */
   public void removeListDataListener(ListDataListener listener);
}
