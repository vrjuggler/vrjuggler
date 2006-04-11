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


/**
 * This class is used to hold information about "generic" Beans loaded into the
 * system.  Such Beans are generic in that they are not Bean viewers or
 * services.  The functionality they provide is entirely up to the implementing
 * author, and it is expected that they will be handled and managed by
 * user-level code.  Hence, the main purpose of Generic Beans is to hide the
 * details of loading the Bean class and its dependencies and to simplify
 * Bean instantiation.
 *
 * @see PanelBean
 * @see ServiceBean
 * @see ViewerBean
 */
public class GenericBean extends TweekBean
{
   /**
    * Creates a new Generic bean with the given name and common TweekBean
    * attributes.
    *
    * @param attrs   the TweekBean attributes of this bean
    */
   public GenericBean( BeanAttributes attrs )
   {
      super( attrs );
   }

   public void instantiate ()
       throws org.vrjuggler.tweek.beans.loader.BeanInstantiationException
   {
      doInstantiation();
   }
}
