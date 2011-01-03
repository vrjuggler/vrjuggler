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


/**
 * The interface used to perform introspection on JAR files that are delivered
 * through the network.  Every JAR file that will be delivered in this method
 * must include a class that implements this interface.  Such a class will be
 * responsible for naming all the Beans contained in the delivered JAR file
 * and for creating instances of TweekBean that correspond to each Bean that
 * can be instantiated.
 *
 * @see org.vrjuggler.tweek.beans.TweekBean
 */
public interface TweekBeanIntrospector
{
   /**
    * Returns an instance of TweekBean (actually a subclass thereof) that is
    * valid for this package.
    */
   public org.vrjuggler.tweek.beans.TweekBean getBean();
}
