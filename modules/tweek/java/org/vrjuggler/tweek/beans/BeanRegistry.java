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
package org.vrjuggler.tweek.beans;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.Vector;

public class BeanRegistry
{
   /**
    * This class is a singleton, use instance() instead.
    */
   protected BeanRegistry ()
   {
      mBeans = new HashMap();
      mListeners = new Vector();
   }

   /**
    * Adds the given bean to this registry under the name of the bean. The name
    * of the bean must be unique. If another bean is already registered under
    * the same name, this operation will fail.
    *
    * @param bean    the bean to register
    *
    * @return  true if the bean was registered successfully, false if a bean is
    *          already registered under that name
    */
   public synchronized void registerBean( TweekBean bean )
   {
      mBeans.put( bean.getName(), bean );
      fireBeanRegistrationEvent( bean );
   }

   /**
    * Removes the given bean from this registry with the given name. If there
    * are no beans with that name in the registry, no beans will be removed.
    *
    * @param name    the name of the bean to remove
    */
   public synchronized void unregisterBean( String name )
   {
      mBeans.remove(name);
   }

   /**
    * Gets the bean that has been registered under the given name. If no beans
    * have been registered under the given name, this method will return null.
    *
    * @param name    the name of the bean to retrieve
    *
    * @return  the bean registered with the given name if successful, null if
    *          there are no beans with the given name
    */
   public synchronized TweekBean getBean (String name)
   {
      return (TweekBean) mBeans.get(name);
   }
   
   public synchronized Map getBeans ()
   {
      return mBeans;
   }

   /**
    * Gets all the beans that derive or implement the given class or interface.
    * Note that the class or interface name must be fully qualified.
    *
    * @param className     the class or interface name to test for matches
    *
    * @return  a list of registered beans that either derive or implement the
    *          given class or interface
    */
   public synchronized List getBeansOfType( String className )
   {
      List matches = new ArrayList();
      Class testClass;

      // try to get the class object for the given className
      try
      {
         testClass = Class.forName(className);
      }
      catch (ClassNotFoundException cnfe)
      {
         cnfe.printStackTrace();
         return matches;
      }

      // Check each bean in the registry
      Iterator beanItr = mBeans.values().iterator();
      while ( beanItr.hasNext() )
      {
         // Get the classes and interfaces of the bean and compare them to the
         // className argument to see if the bean derives/implements that class.
         TweekBean bean = (TweekBean)beanItr.next();
         if ( testClass.isAssignableFrom(bean.getClass()) )
         {
            // We have a match! Remember the bean and move on
            matches.add( bean );
         }
      }

      return matches;
   }

   /**
    * Tests if a bean is registered under the given name.
    *
    * @param name    the name of the bean to test for
    *
    * @return  true if the bean is registered; false otherwise
    */
   public synchronized boolean isRegistered(String name)
   {
      return mBeans.containsKey(name);
   }

   /**
    * Adds a new listener for bean registration events.
    *
    * @param l    the object that wishes to listen to this registry
    */
   public synchronized void addBeanRegistrationListener (BeanRegistrationListener l)
   {
      mListeners.add(l);
   }

   /**
    * Removes the given bean registration listener from this object. If the
    * object is not already a listener of this object no work is done.
    *
    * @param l    the listener to remove from this object
    */
   public synchronized void removeBeanRegistrationListener (BeanRegistrationListener l)
   {
      mListeners.remove(l);
   }

   /**
    * Fires off a BeanRegistrationEvent to all bean registration listeners.
    *
    * @param bean    the bean that got registered that is the cause of the event
    */
   protected void fireBeanRegistrationEvent( TweekBean bean )
   {
      BeanRegistrationEvent evt = new BeanRegistrationEvent( this, bean );

      BeanRegistrationListener l = null;
      Vector listenersCopy;

      synchronized( this )
      {
         listenersCopy = (Vector) mListeners.clone();
      }

      for ( Iterator itr = listenersCopy.iterator(); itr.hasNext(); )
      {
         l = (BeanRegistrationListener)itr.next();
         l.beanRegistered( evt );
      }
   }

   /**
    * Gets the singleton instance of this class. This implementation is thread
    * safe.
    */
   public static BeanRegistry instance ()
   {
      synchronized (BeanRegistry.class)
      {
         if ( mInstance == null )
         {
            mInstance = new BeanRegistry();
         }
      }
      return mInstance;
   }

   /**
    * The singleton instance of this class.
    *
    * @link
    * @shapeType PatternLink
    * @pattern Singleton
    * @supplierRole Singleton factory
    */
   /*# private BeanRegistry _beanRegistry; */
   private static BeanRegistry mInstance = null;

   /**
    * Maps a bean's name to the TweekBean implementation object.
    *
    * @link aggregation
    * @associates <{TweekBean}>
    */
   private Map mBeans;

   /**
    * The list of listeners of bean registration events for this object.
    */
   private Vector mListeners;
}
