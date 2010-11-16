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

package org.vrjuggler.tweek.beans.loader;


/**
 * Exception type thrown when a Bean cannot be instantiated.  As of Tweek
 * 0.92.6, This exception type supports the <i>cause</i> mechanism defined by
 * <code>java.lang.Throwable</code>.
 *
 * @version $Revision$
 */
public class BeanInstantiationException extends Exception
{
   public BeanInstantiationException(String msg)
   {
      super(msg);
   }

   /**
    * Create a Bean instantiation exception that has a cause.
    *
    * @since 0.92.6
    */
   public BeanInstantiationException(String msg, Throwable cause)
   {
      super(msg, cause);
   }
}
