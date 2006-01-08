/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.jccl.config;

import javax.swing.undo.*;

/**
 * Allows the user to undo/redo changes made to ConfigElements.
 */
public class ConfigUndoManager extends UndoManager
{
   public ConfigUndoManager ()
   {;}
   
   public synchronized boolean addEdit(UndoableEdit anEdit)
   {
      if (mUndoInProgress)
      {
         // Can not add edit while undoing.
         return true;
      }
      else
      {
         ++mEditOffsetFromSave;
         return(super.addEdit(anEdit));
      }
   }

   public synchronized void undo() throws CannotUndoException
   {
      mUndoInProgress = true;
      --mEditOffsetFromSave;
      super.undo();
      mUndoInProgress = false;
   }

   /**
    * Undoes every edit posted since the last save.
    *
    * @since 0.92.3
    */
   public synchronized void undoAll() throws CannotUndoException
   {
      undoUntil(0);
   }

   /**
    * Undoes every edit up until the specified save point.  The "save point"
    * is a value of the edit offset since the last save queried sometime prior
    * to invoking this method.  This is the inverse operation of redoUntil().
    *
    * @param savePoint  the "save point" to which the configuration state will
    *                   be reset
    *
    * @see #getEditOffsetFromSave()
    * @see #redoUntil(int)
    *
    * @since 0.92.3
    */
   public synchronized void undoUntil(int savePoint) throws CannotUndoException
   {
      mUndoInProgress = true;
      while ( mEditOffsetFromSave > savePoint )
      {
         --mEditOffsetFromSave;
         super.undo();
      }
      mUndoInProgress = false;
   }

   public synchronized void redo() throws CannotUndoException
   {
      mUndoInProgress = true;
      ++mEditOffsetFromSave;
      super.redo();
      mUndoInProgress = false;
   }

   /**
    * Re-applies every edit that has been undone.
    *
    * @since 0.92.3
    */
   public synchronized void redoAll() throws CannotUndoException
   {
      mUndoInProgress = true;
      while ( canRedo() )
      {
         ++mEditOffsetFromSave;
         super.redo();
      }
      mUndoInProgress = false;
   }

   /**
    * Re-applies every undone edit up until the specified save point.  The
    * "save point" is a value of the edit offset since the last save queried
    * sometime prior to invoking this method.  This is the inverse operation of
    * undoUntil().
    *
    * @param savePoint  the "save point" to which the configuration state will
    *                   be reset
    *
    * @see #getEditOffsetFromSave()
    * @see #undoUntil(int)
    *
    * @since 0.92.3
    */
   public synchronized void redoUntil(int savePoint) throws CannotUndoException
   {
      mUndoInProgress = true;
      while ( mEditOffsetFromSave < savePoint && canRedo() )
      {
         ++mEditOffsetFromSave;
         super.redo();
      }
      mUndoInProgress = false;
   }

   public void saveHappened()
   {
      mEditOffsetFromSave = 0;
   }

   public boolean getUnsavedChanges()
   {
      return ( 0 != mEditOffsetFromSave );
   }

   /**
    * Returns the current edit offset since the last time a save operation
    * was performed.  This can be used in conjunction with undoUntil() and
    * redoUntil().
    *
    * @see #undoUntil(int)
    * @see #redoUntil(int)
    *
    * @since 0.92.3
    */
   public int getEditOffsetFromSave()
   {
      return mEditOffsetFromSave;
   }

   private boolean mUndoInProgress = false;
   private int mEditOffsetFromSave = 0;
}
