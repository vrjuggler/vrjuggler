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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.beans;

import java.util.*;

/**
 * Manages dependencies between beans. This class provides the functionality
 * necessary to do dependency analysis between beans so that they can be loaded
 * in a sane order.
 */
public class BeanDependencyManager
{
   /**
    * This class is a singleton, use instance() instead.
    */
   protected BeanDependencyManager()
   {
      mPending = new ArrayList();
   }

   /**
    * Gets the singleton instance of this class. This implementation is thread
    * safe.
    */
   public static BeanDependencyManager instance ()
   {
      synchronized (BeanDependencyManager.class)
      {
         if ( mInstance == null )
         {
            mInstance = new BeanDependencyManager();
         }
      }
      return mInstance;
   }
   
   /**
    * Adds the given bean to the pending list.
    */
   public void add(TweekBean bean)
   {
      mPending.add(bean);
   }

   /**
    * Gets the next bean that has its dependencies fulfilled and can be loaded.
    * The bean is then removed from the pending list.
    *
    * @return  the next bean with no remaining dependencies; null if there are
    *          no beans remaining whose dependencies have been satisfied
    */
   public synchronized TweekBean pop()
   {
      TweekBean result = null;
      for (Iterator itr = mPending.iterator(); itr.hasNext(); )
      {
         // Check if this bean has its dependencies satisfied
         TweekBean bean = (TweekBean)itr.next();
         int num_deps = getNumUnsatisfiedDependencies(bean);
         if (num_deps == 0)
         {
            result = bean;
            break;
         }
      }

      // Make sure we remove the bean from the pending list
      if (result != null)
      {
         mPending.remove(result);
      }

      return result;
   }

   /**
    * Tests if the given bean is in the pending list.
    */
   public synchronized boolean isPending(TweekBean bean)
   {
      return mPending.contains(bean);
   }

   /**
    * Tests if there hare any beans still pending.
    */
   public synchronized boolean hasBeansPending()
   {
      return (mPending.size() > 0);
   }

   /**
    * Gets the number of unsatisfied dependencies that the given bean has
    * remaining.
    */
   public static int getNumUnsatisfiedDependencies(TweekBean bean)
   {
      int num_unsatisfied_deps = 0;

      if ( null != bean.getAttributes().getBeanDependencies() )
      {
         for (Iterator itr = bean.getAttributes().getBeanDependencies().iterator();
              itr.hasNext(); )
         {
            BeanDependency dep = (BeanDependency)itr.next();
            String dep_name = dep.getBeanName();
            if (! BeanRegistry.instance().isRegistered(dep_name))
            {
               ++num_unsatisfied_deps;
            }
         }
      }

      return num_unsatisfied_deps;
   }

   public synchronized List getPendingBeans()
   {
      return mPending;
   }
   
   /**
    * The singleton instance of this class.
    */
   private static BeanDependencyManager mInstance = null;
   
   /**
    * List of pending beans.
    */
   private List mPending;
}
