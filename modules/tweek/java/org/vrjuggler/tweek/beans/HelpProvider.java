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
 * The interface used to identify Beans as being able to provide help to users.
 *
 * @since 0.92.1
 */
public interface HelpProvider
{
   /**
    * Returns the Bean-specific (terse) description of the help that will be
    * provided.  This string will be displayed in a GUI and should therefore
    * be concise and meaningful.  If the implementation of this method returns 
    * null or "", then the help description string will be the composition of
    * the Bean's name and the string " Help".
    */
   public String getHelpDescription();

   /**
    * Invoked when the user of the Tweek Java GUI requests Bean-specific
    * help.  There are no recommendations for the "best" way to implement
    * this method.
    */
   public void helpRequested();
}
