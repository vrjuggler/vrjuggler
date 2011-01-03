/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

package org.vrjuggler.tweek.beandelivery;

import java.util.EventObject;
import java.util.List;


/**
 * Event object delivered to classes implementing
 * BeanDeliverySubjectUpdateListener.  The source of these events will always
 * be the tweek.ObserverPOA instance that is associated with the Subject that
 * has changed.
 */
public class BeanDeliverySubjectUpdateEvent extends EventObject
{
   public BeanDeliverySubjectUpdateEvent(Object source, List newBeans)
   {
      super(source);
      this.newBeans = newBeans;
   }

   public BeanDeliverySubjectUpdateEvent(Object source, String activeBeanName)
   {
      super(source);
      this.activeBeanName = activeBeanName;
   }

   /**
    * Returns the list of newly created TweekBean objects.  The returned object
    * will be null if no new TweekBean objects were created.
    */
   public List getNewBeans()
   {
      return newBeans;
   }

   /**
    * Determines whether we have an active Bean or not.
    *
    * @return true is returned if there is an active Bean.  Otherwise, false
    *         is returned.
    */
   public boolean hasActiveBean()
   {
      // If activeBeanName is not null and its value is not "", then we have
      // an active Bean.
      return (null != activeBeanName && ! activeBeanName.equals(""));
   }

   /**
    * Returns the name of the active Bean, if there is one.  The returned
    * object will be null if there is no active Bean.
    */
   public String getActiveBeanName()
   {
      return activeBeanName;
   }

   private List   newBeans       = null;
   private String activeBeanName = null;
}
