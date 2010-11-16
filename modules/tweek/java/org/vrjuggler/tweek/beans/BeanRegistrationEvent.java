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

import java.util.EventObject;

/**
 * Event that describes a bean that has just been registered with the
 * BeanRegistry. The source object with always be the registry.
 *
 * @see BeanRegistry
 *
 * @since 0.1.0
 */
public class BeanRegistrationEvent
   extends EventObject
{
   /**
    * Creates a new BeanRegistrationEvent object with the given source bean
    * registry and the given tweek bean.
    *
    * @param source     the source registry from which the event originated
    * @param bean       the bean that was registered
    */
   public BeanRegistrationEvent( Object source, TweekBean bean )
   {
      super(source);
      this.bean = bean;
   }

   /**
    * Gets the bean that was registered.
    *
    * @return  the bean that was registered
    */
   public TweekBean getBean()
   {
      return bean;
   }

   /**
    * The bean that was registered.
    */
   private TweekBean bean;
}
