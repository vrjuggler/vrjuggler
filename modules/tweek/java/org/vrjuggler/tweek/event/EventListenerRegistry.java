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

package org.vrjuggler.tweek.event;

import java.util.*;


/**
 * A singleton that centralizes registration of events handled by the
 * Tweek Java GUI and related classes.  Tweek Beans that implement listener
 * interfaces of interest are automatically registered when the Bean is
 * instantiated, but this does not address all cases.  For example, a Bean
 * may wish to have an inner class that implements a Tweek interface.  An
 * instance of such a class can be registered using this registry.
 */
public class EventListenerRegistry
{
   private EventListenerRegistry()
   {
   }

   /**
    * Singleton accessor function.
    */
   public static EventListenerRegistry instance()
   {
      return mInstance;
   }

   /**
    * Registers the given object as a listener of the given type.
    *
    * @param listener     The object implementing some listener interface.
    * @param listenerType The Java Class object that represents the listener
    *                     type of the given listener object.
    */
   public void registerListener(EventListener listener, Class listenerType)
   {
      if ( mRegistry.get(listenerType) == null )
      {
         mRegistry.put(listenerType, new ArrayList());
      }

      ((List) mRegistry.get(listenerType)).add(listener);
      fireRegistrationEvent(listener, listenerType);
   }

   /**
    * Un-registers the given object as a listener of the given type.
    *
    * @param listener     The object implementing some listener interface.
    * @param listenerType The Java Class object that represents the listener
    *                     type of the given listener object.
    */
   public void unregisterListener(EventListener listener, Class listenerType)
   {
      if ( mRegistry.get(listenerType) == null )
      {
      }
      else
      {
         ((List) mRegistry.get(listenerType)).remove(listener);
         fireUnregistrationEvent(listener, listenerType);
      }
   }

   /**
    * Adds the given object to the list of listeners for registration
    * events.
    */
   public synchronized void addRegistrationListener(RegistrationListener listener)
   {
      mListeners.add(listener);
   }

   /**
    * Removes the given object from the list of listeners for registration
    * events.
    */
   public synchronized void removeRegistrationListener(RegistrationListener listener)
   {
      mListeners.remove(listener);
   }

   private synchronized void fireRegistrationEvent(EventListener registrant,
                                                   Class listenerType)
   {
      RegistrationEvent e = new RegistrationEvent(this, registrant,
                                                  listenerType);

      for ( Iterator i = mListeners.iterator(); i.hasNext(); )
      {
         RegistrationListener l = (RegistrationListener) i.next();
         l.listenerRegistered(e);
      }
   }

   private synchronized void fireUnregistrationEvent(EventListener registrant,
                                                     Class listenerType)
   {
      RegistrationEvent e = new RegistrationEvent(this, registrant,
                                                  listenerType);

      for ( Iterator i = mListeners.iterator(); i.hasNext(); )
      {
         RegistrationListener l = (RegistrationListener) i.next();
         l.listenerUnregistered(e);
      }
   }

   private static EventListenerRegistry mInstance = new EventListenerRegistry();

   private Map  mRegistry  = new HashMap();
   private List mListeners = new ArrayList();
}
