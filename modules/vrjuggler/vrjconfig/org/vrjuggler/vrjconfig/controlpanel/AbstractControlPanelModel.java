/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

import javax.swing.event.*;

/**
 * The abstract definition for the data model that provides a ControlPanel with
 * its contents.
 *
 * @see ControlPanel
 */
public abstract class AbstractControlPanelModel
   implements ControlPanelModel
{
   public void addListDataListener(ListDataListener listener)
   {
      listenerList.add(ListDataListener.class, listener);
   }

   public void removeListDataListener(ListDataListener listener)
   {
      listenerList.remove(ListDataListener.class, listener);
   }

   protected void fireIntervalAdded(int index0, int index1)
   {
      ListDataEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i = listeners.length -2; i >= 0; i -= 2)
      {
         if (listeners[i] == ListDataListener.class)
         {
            if (evt == null)
            {
               evt = new ListDataEvent(this, ListDataEvent.INTERVAL_ADDED, index0, index1);
            }
            ((ListDataListener)listeners[i+1]).intervalAdded(evt);
         }
      }
   }

   protected void fireIntervalRemoved(int index0, int index1)
   {
      ListDataEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i = listeners.length -2; i >= 0; i -= 2)
      {
         if (listeners[i] == ListDataListener.class)
         {
            if (evt == null)
            {
               evt = new ListDataEvent(this, ListDataEvent.INTERVAL_REMOVED, index0, index1);
            }
            ((ListDataListener)listeners[i+1]).intervalRemoved(evt);
         }
      }
   }

   protected void fireInternalChanged(int index0, int index1)
   {
      ListDataEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i = listeners.length -2; i >= 0; i -= 2)
      {
         if (listeners[i] == ListDataListener.class)
         {
            if (evt == null)
            {
               evt = new ListDataEvent(this, ListDataEvent.CONTENTS_CHANGED, index0, index1);
            }
            ((ListDataListener)listeners[i+1]).contentsChanged(evt);
         }
      }
   }

   /**
    * The listeners of this model.
    */
   protected EventListenerList listenerList = new EventListenerList();
}
