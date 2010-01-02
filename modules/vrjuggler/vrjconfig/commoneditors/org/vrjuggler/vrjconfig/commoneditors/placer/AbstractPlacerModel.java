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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors.placer;

import javax.swing.event.EventListenerList;

/**
 * Partial implementation of placer models that helps take care of implementing
 * the mundane stuff. Extend this class to easily create new placer data models.
 */
public abstract class AbstractPlacerModel
   implements PlacerModel
{
   /**
    * Adds a listener that's notified each time this data model changes.
    */
   public void addPlacerModelListener(PlacerModelListener listener)
   {
      listeners.add(PlacerModelListener.class, listener);
   }

   /**
    * Removes a listener that's notified each time this data model changes.
    */
   public void removePlacerModelListener(PlacerModelListener listener)
   {
      listeners.remove(PlacerModelListener.class, listener);
   }

   /**
    * Notifies all listeners that items were changed.
    */
   public void fireItemsChanged(int[] indicies)
   {
      // Get all the objects inserted
      Object[] values = new Object[indicies.length];
      for (int i=0; i<indicies.length; ++i)
      {
         values[i] = getElement(indicies[i]);
      }
      // Notify all interested listeners
      PlacerModelEvent evt = null;
      Object[] listenerList = listeners.getListenerList();
      for (int i=listenerList.length-2; i>=0; i-=2)
      {
         if (listenerList[i] == PlacerModelListener.class)
         {
            // lazily instantiate the list
            if (evt == null)
            {
               evt = new PlacerModelEvent(this, indicies, values);
            }
            ((PlacerModelListener)listenerList[i+1]).placerItemsChanged(evt);
         }
      }
   }

   /**
    * Notifies all listeners that items were inserted.
    */
   public void fireItemsInserted(int[] indicies)
   {
      // Get all the objects inserted
      Object[] values = new Object[indicies.length];
      for (int i=0; i<indicies.length; ++i)
      {
         values[i] = getElement(indicies[i]);
      }
      // Notify all interested listeners
      PlacerModelEvent evt = null;
      Object[] listenerList = listeners.getListenerList();
      for (int i=listenerList.length-2; i>=0; i-=2)
      {
         if (listenerList[i] == PlacerModelListener.class)
         {
            // lazily instantiate the list
            if (evt == null)
            {
               evt = new PlacerModelEvent(this, indicies, values);
            }
            ((PlacerModelListener)listenerList[i+1]).placerItemsInserted(evt);
         }
      }
   }

   /**
    * Notifies all listeners that items were removed.
    */
   public void fireItemsRemoved(int[] indicies, Object[] values)
   {
      // Notify all interested listeners
      PlacerModelEvent evt = null;
      Object[] listenerList = listeners.getListenerList();
      for (int i=listenerList.length-2; i>=0; i-=2)
      {
         if (listenerList[i] == PlacerModelListener.class)
         {
            // lazily instantiate the list
            if (evt == null)
            {
               evt = new PlacerModelEvent(this, indicies, values);
            }
            ((PlacerModelListener)listenerList[i+1]).placerItemsRemoved(evt);
         }
      }
   }

   /**
    * Notifies all listeners that the size of the desktop has changed.
    */
   public void fireDesktopSizeChanged()
   {
      // Notify all interested listeners
      PlacerModelEvent evt = null;
      Object[] listenerList = listeners.getListenerList();
      for (int i=listenerList.length-2; i>=0; i-=2)
      {
         if (listenerList[i] == PlacerModelListener.class)
         {
            // lazily instantiate the list
            if (evt == null)
            {
               evt = new PlacerModelEvent(this, null, null);
            }
            ((PlacerModelListener)listenerList[i+1]).placerDesktopSizeChanged(evt);
         }
      }
   }

   /**
    * List of all listeners of this model.
    */
   protected EventListenerList listeners = new EventListenerList();
}
