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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.event;


/**
 * The listener interface for events related to the main Tweek GUI window.
 *
 * @since 0.0.4
 */
public interface TweekFrameListener extends java.util.EventListener
{
   /**
    * Invoked when the Tweek GUI has been closed as the result of calling
    * dispose on the window.
    */
   public void frameClosed(TweekFrameEvent e);

   /**
    * Invoke when the user has attempted to close the Tweek GUI.  This method
    * can be used to prevent the Tweek Java GUI from closing by returning
    * false. 
    *
    * @return If true is returned, then the Tweek GUI Frame will close.
    *         Otherwise, the window closing operation is cancelled.
    */
   public boolean frameClosing(TweekFrameEvent e);

   /**
    * Invoked when the Tweek GUI is changed from a minimized to a normal
    * state.
    */
   public void frameDeiconified(TweekFrameEvent e);

   /**
    * Invoked when the Tweek GUI is set to be the active Window (when it gains
    * focus).  The native windowing system may denote the active Window or its
    * children with special decorations, such as a highlighted title bar.  The
    * active Window is always either the focused Window, or the first Frame or
    * Dialog that is an owner of the focused Window.
    */
   public void frameFocused(TweekFrameEvent e);

   /**
    * Invoked when the Tweek GUI is changed from a normal to a minimized
    * state.
    */
   public void frameIconified(TweekFrameEvent e);

   /**
    * Invoked the first time the Tweek GUI is made visible.
    */
   public void frameOpened(TweekFrameEvent e);

   /**
    * Invoked when the Tweek GUI is no longer the active Window (when it loses
    * focus).  The native windowing system may denote the active Window or its
    * children with special decorations, such as a highlighted title bar.  The
    * active Window is always either the focused Window, or the first Frame or
    * Dialog that is an owner of the focused Window.
    */
   public void frameUnfocused(TweekFrameEvent e);
}
