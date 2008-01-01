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

package org.vrjuggler.tweek.beans;

import java.util.Vector;


/**
 * This class serves as the communication medium between dynamically
 * instantiated JavaBeans and the existing objects that wish to listen
 * for BeanInstantationEvents.
 *
 * @see org.vrjuggler.tweek.beans.BeanInstantiationEvent
 * @see org.vrjuggler.tweek.beans.BeanInstantiationListener
 */
public class BeanInstantiationCommunicator
{
   public static BeanInstantiationCommunicator instance()
   {
      if ( m_instance == null )
      {
         m_instance = new BeanInstantiationCommunicator();
      }

      return m_instance;
   }

   public synchronized void addBeanInstantiationListener(BeanInstantiationListener l)
   {
      m_listeners.add(l);
   }

   public synchronized void removeBeanInstantiationListener(BeanInstantiationListener l)
   {
      m_listeners.remove(l);
   }

   public void fireBeanInstantiationEvent(TweekBean beanHolder)
   {
      BeanInstantiationEvent e = new BeanInstantiationEvent(this, beanHolder);

      BeanInstantiationListener l = null;
      Vector listeners;

      synchronized (this)
      {
         listeners = (Vector) m_listeners.clone();
      }

      for ( int i = 0; i < listeners.size(); i++ )
      {
         l = (BeanInstantiationListener) listeners.elementAt(i);
         l.beanInstantiated(e);
      }
   }

   protected BeanInstantiationCommunicator()
   {
   }

   protected static BeanInstantiationCommunicator m_instance = null;

   private Vector m_listeners = new Vector();
}
