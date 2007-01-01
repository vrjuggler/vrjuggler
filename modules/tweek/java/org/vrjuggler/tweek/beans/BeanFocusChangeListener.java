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


/**
 * Interface to be implemented by classes that want to be informed when there
 * is a change in which Panel Bean is currently focused.
 *
 * @see PanelBean
 * @see ViewerBean
 */
public interface BeanFocusChangeListener extends java.util.EventListener
{
   /**
    * A new Panel Bean has received focus within the Viewer Bean.
    *
    * @since 0.92.7
    */
   public void beanFocused(BeanFocusChangeEvent event);

   /**
    * The Panel Bean that had focus within the Viewer Bean is no longer
    * focused.
    *
    * @since 0.92.7
    */
   public void beanUnfocused(BeanFocusChangeEvent event);
}
