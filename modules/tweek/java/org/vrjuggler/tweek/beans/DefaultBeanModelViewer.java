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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.beans;

import java.util.Vector;


/**
 * @since 1.0
 */
public abstract class DefaultBeanModelViewer implements BeanModelViewer
{
   /**
    *
    */
   public final void init (BeanTreeModel model)
   {
      setModel(model);
      initGUI();
   }

   /**
    * Adds the given listener to the local collection of BeanFOcusChangeListener
    * objects.
    */
   public final synchronized void addBeanFocusChangeListener (BeanFocusChangeListener l)
   {
      m_focus_listeners.add(l);
   }

   /**
    * Removed the given listener from the local collection of
    * BeanFOcusChangeListener objects.
    */
   public final synchronized void removeBeanFocusChangeListener (BeanFocusChangeListener l)
   {
      m_focus_listeners.remove(l);
   }

   /**
    * Fires a BeanFocusChangeEvent that informs listeners that the given Bean
    * has lost focus.
    */
   public final void fireBeanUnfocusEvent (PanelBean bean)
   {
      BeanFocusChangeEvent e =
         new BeanFocusChangeEvent(this, bean,
                                  BeanFocusChangeEvent.BEAN_UNFOCUSED);
      fireEvent(e);
   }

   /**
    * Fires a BeanFocusChangeEvent that informs listeners that the given Bean
    * now has focus.
    */
   public final void fireBeanFocusEvent (PanelBean bean)
   {
      BeanFocusChangeEvent e =
         new BeanFocusChangeEvent(this, bean,
                                  BeanFocusChangeEvent.BEAN_FOCUSED);
      fireEvent(e);
   }

   /**
    * Performs the actual event firing.  This is here so that the public
    * event-firing methods can create the event and then share the code that
    * performs the delivery.
    */
   private void fireEvent (BeanFocusChangeEvent e)
   {
      BeanFocusChangeListener l = null;
      Vector listeners;

      synchronized (this)
      {
         listeners = (Vector) m_focus_listeners.clone();
      }

      for ( int i = 0; i < listeners.size(); i++ )
      {
         l = (BeanFocusChangeListener) listeners.elementAt(i);
         l.beanFocusChanged(e);
      }
   }

   private Vector m_focus_listeners = new Vector();
}
