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

package org.vrjuggler.jccl.config.undo;

import java.util.*;
import javax.swing.undo.AbstractUndoableEdit;
import javax.swing.undo.CannotUndoException;
import javax.swing.undo.CannotRedoException;

import org.vrjuggler.jccl.config.ConfigElement;

public class ConfigElementPropertyEdit extends AbstractUndoableEdit
{
   public ConfigElementPropertyEdit(ConfigElement elm, String name, int index, Object old_val, Object new_val)
   {
      super();
      mElement = elm;
      mPropName = name;
      mPropIndex = index;
      mOldValue = old_val;
      mNewValue = new_val;
   }

   public void undo() throws CannotUndoException 
   {      
      super.undo();
      mElement.setProperty(mPropName, mPropIndex, mOldValue);
   }

   public void redo() throws CannotRedoException
   {
      System.out.println("redoing..");
      super.redo();
      System.out.println("redoing...");
      
      mElement.setProperty(mPropName, mPropIndex, mNewValue);
   }

   private String          mPropName;
   private int             mPropIndex;
   private Object          mNewValue;
   private Object          mOldValue;
   private ConfigElement   mElement;
}
