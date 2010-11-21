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


/**
 * Implementation of <code>TweekBean</code> that is for Service Beans.  A
 * Service Bean is one where the accessing code has full access to the
 * interface of the Bean, but the implementation is loaded dynamically.
 *
 * <p>
 * Examples of Service Beans that come with the Tweek Java GUI are the
 * Environment Service and the Global Preferences Service.  Both of these
 * Beans are accessed indirectly through proxy types.  This approach hides
 * the details of looking up the Service Bean in the Bean Registry while
 * still giving access to the full Service Bean interface.
 * </p>
 *
 * @see org.vrjuggler.tweek.services.EnvironmentService
 * @see org.vrjuggler.tweek.services.EnvironmentServiceProxy
 * @see org.vrjuggler.tweek.services.GlobalPreferencesService
 * @see org.vrjuggler.tweek.services.GlobalPreferencesServiceProxy
 * @see BeanRegistry
 */
public class ServiceBean extends TweekBean
{
   /**
    * Creates a new Service bean with the given common TweekBean attributes.
    *
    * @param attrs   the TweekBean attributes of this bean
    */
   public ServiceBean (BeanAttributes attrs)
   {
      super( attrs );
   }

   public void instantiate ()
      throws org.vrjuggler.tweek.beans.loader.BeanInstantiationException
   {
      // This should work fairly well as a way to make a Bean a singleton.
      if ( ! instantiated )
      {
         doInstantiation();
      }
   }
}
