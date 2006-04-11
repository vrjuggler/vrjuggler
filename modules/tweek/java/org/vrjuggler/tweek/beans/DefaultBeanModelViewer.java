/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.beans;

import javax.swing.event.EventListenerList;


/**
 * Default implementation of the <code>BeanModelViewer</code> interface
 * from which authors of Viewer Beans may derive their custom implementations.
 */
public abstract class DefaultBeanModelViewer implements BeanModelViewer
{
   /**
    * Sets the Bean model to the given value and initializes the Viewer user
    * interface by invoking <code>initGUI()</code>.
    */
   public final void init(BeanTreeModel model)
   {
      setModel(model);
      initGUI();
   }

   /**
    * Adds the given listener to the local collection of BeanFOcusChangeListener
    * objects.
    */
   public final synchronized void addBeanFocusChangeListener(BeanFocusChangeListener l)
   {
      this.listenerList.add(BeanFocusChangeListener.class, l);
   }

   /**
    * Removed the given listener from the local collection of
    * BeanFOcusChangeListener objects.
    */
   public final synchronized void removeBeanFocusChangeListener(BeanFocusChangeListener l)
   {
      this.listenerList.remove(BeanFocusChangeListener.class, l);
   }

   /**
    * Fires a BeanFocusChangeEvent that informs listeners that the given Bean
    * has lost focus.
    */
   protected void fireBeanUnfocusEvent(PanelBean bean)
   {
      Object[] listeners = this.listenerList.getListenerList();

      BeanFocusChangeEvent event = null;

      for ( int i = listeners.length - 2; i >= 0; i -= 2 )
      {
         if ( listeners[i] == BeanFocusChangeListener.class )
         {
            if ( event == null )
            {
               event = new BeanFocusChangeEvent(this, bean);
            }

            ((BeanFocusChangeListener) listeners[i + 1]).beanUnfocused(event);
         }
      }
   }

   /**
    * Fires a BeanFocusChangeEvent that informs listeners that the given Bean
    * now has focus.
    */
   protected void fireBeanFocusEvent(PanelBean bean)
   {
      Object[] listeners = this.listenerList.getListenerList();

      BeanFocusChangeEvent event = null;

      for ( int i = listeners.length - 2; i >= 0; i -= 2 )
      {
         if ( listeners[i] == BeanFocusChangeListener.class )
         {
            if ( event == null )
            {
               event = new BeanFocusChangeEvent(this, bean);
            }

            ((BeanFocusChangeListener) listeners[i + 1]).beanFocused(event);
         }
      }
   }

   private EventListenerList listenerList = new EventListenerList();
}
