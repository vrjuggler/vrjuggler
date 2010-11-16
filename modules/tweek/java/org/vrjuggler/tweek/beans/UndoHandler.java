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
 * The interface used to identify Beans as being able to handle undo and redo
 * events.  This interface must be implemented by the Tweek Panel Bean that
 * will be loaded and instantiated by the Tweek Java GUI.  The Tweek Java GUI
 * only learns of UndoHandler implementations when the Panel Bean is focused.
 *
 * @since 0.92.4
 */
public interface UndoHandler
{
   public javax.swing.undo.UndoManager getUndoManager();

   /**
    * Invoked when the user wants to undo an action.
    *
    * @return true is returned if another undo operation can be performed;<br>
    *         false is returned otherwise.
    */
   public void undoRequested();

   /**
    * Invoked when the user wants to redo an action that was undone.
    *
    * @return true is returned if another redo operation can be performed;<br>
    *         false is returned otherwise.
    */
   public void redoRequested();
}
