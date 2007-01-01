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

import javax.swing.event.EventListenerList;
import org.vrjuggler.tweek.beans.UndoHandler;


/**
 * Interface for informing Tweek objects about file action events that occur
 * as a result of Tweek Beans manipulating files.  The Tweek Beans must
 * implement the BeanLoader interface.  Instances of this class must be
 * registered with org.vrjuggler.tweek.TweekCore using the method
 * TweekCore.registerUndoActionGenerator().  When the generator is no longer
 * in use, it must be unregistered using the method
 * TweekCore.unregisterUndoActionGenerator().
 *
 * @see UndoHandler
 * @see org.vrjuggler.tweek.TweekCore
 *
 * @since 0.92.4
 */
public class UndoActionGenerator
{
   /**
    * Adds the given UndoActionListener implementation to the list of listeners
    * for undo action events.
    *
    * @param l  the listener to add
    */
   public void addUndoActionListener(UndoActionListener l)
   {
      mListenerList.add(UndoActionListener.class, l);
   }

   /**
    * Removes the given UndoActionListener implementation from the list of
    * listeners for undo action events.
    *
    * @param l  the listener to remove
    */
   public void removeUndoActionListener(UndoActionListener l)
   {
      mListenerList.remove(UndoActionListener.class, l);
   }

   /**
    * Generates an undo action event and delivers it to all the listeners
    * registered with this generator.  The method
    * UndoActionListener.undoActionPerformed() is invoked on all listeners.
    *
    * @param source     a reference to the Tweek Bean implementing the
    *                   FileLoader interface that is responsible for opening
    *                   the file
    *
    * @see UndoActionListener
    */
   public void fireUndoActionPerformed(UndoHandler source)
   {
      fireUndoActionPerformed(source, source);
   }

   /**
    * Generates an undo action event and delivers it to all the listeners
    * registered with this generator.  The method
    * UndoActionListener.undoActionPerformed() is invoked on all listeners.
    *
    * @param source     the source of the event to be generated
    * @param handler    a reference to the Tweek Bean implementing the
    *                   FileLoader interface that is responsible for opening
    *                   the file
    *
    * @see UndoActionListener
    */
   public void fireUndoActionPerformed(Object source, UndoHandler handler)
   {
      UndoActionEvent e = null;
      Object[] listeners = mListenerList.getListenerList();
      for ( int i = listeners.length - 2; i >= 0; i -= 2 )
      {
         if ( listeners[i] == UndoActionListener.class )
         {
            if ( e == null )
            {
               e = new UndoActionEvent(source, handler);
            }

            ((UndoActionListener) listeners[i + 1]).undoActionPerformed(e);
         }
      }
   }

   private EventListenerList mListenerList = new EventListenerList();
}
