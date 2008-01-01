/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

package org.vrjuggler.tweek.event;


/**
 * Default implementation of <code>TweekFrameListener</code>.  The methods
 * of this class do nothing, but this class does provide a means for users
 * to implement only the methods of <code>TweekFrameListener</code> that
 * they care about.
 *
 * @since 0.20.0
 */
public class TweekFrameAdapter implements TweekFrameListener
{
   public void frameOpened(TweekFrameEvent e)
   {
   }

   public void frameClosed(TweekFrameEvent e)
   {
   }

   public boolean frameClosing(TweekFrameEvent e)
   {
      return true;
   }

   public void frameFocused(TweekFrameEvent e)
   {
   }

   public void frameUnfocused(TweekFrameEvent e)
   {
   }

   public void frameIconified(TweekFrameEvent e)
   {
   }

   public void frameDeiconified(TweekFrameEvent e)
   {
   }
}
