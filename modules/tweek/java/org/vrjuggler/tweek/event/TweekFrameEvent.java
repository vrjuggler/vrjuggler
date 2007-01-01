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

package org.vrjuggler.tweek.event;

import java.awt.Window;


/**
 * Event type associated with window events that are forwarded from the Tweek
 * Java GUi frame to listeners.
 *
 * @see TweekFrameListener
 *
 * @since 0.0.4
 */
public class TweekFrameEvent extends java.awt.event.WindowEvent
{
   public TweekFrameEvent(Window source, int id)
   {
      super(source, id);
   }

   public TweekFrameEvent(Window source, int id, int oldState, int newState)
   {
      super(source, id, oldState, newState);
   }

   public TweekFrameEvent(Window source, int id, Window opposite)
   {
      super(source, id, opposite);
   }

   public TweekFrameEvent(Window source, int id, Window opposite, int oldState,
                          int newState)
   {
      super(source, id, opposite, oldState, newState);
   }
}
