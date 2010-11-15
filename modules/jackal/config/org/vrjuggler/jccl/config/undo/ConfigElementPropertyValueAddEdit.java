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

package org.vrjuggler.jccl.config.undo;

import javax.swing.undo.AbstractUndoableEdit;
import javax.swing.undo.CannotUndoException;
import javax.swing.undo.CannotRedoException;

import org.vrjuggler.jccl.config.ConfigElement;


/**
 * Undoable edit type for the addition of a value to a variable-valued
 * config element property.
 */
public class ConfigElementPropertyValueAddEdit
   extends AbstractUndoableEdit
{
   public ConfigElementPropertyValueAddEdit(ConfigElement elm, String name,
                                            int addedIndex, Object value)
   {
      super();
      mElement    = elm;
      mPropName   = name;
      mAddedIndex = addedIndex;
      mPropValue  = value;
   }

   public void undo() throws CannotUndoException 
   {      
      super.undo();
      mElement.removeProperty(mPropName, mAddedIndex);
   }

   public void redo() throws CannotRedoException
   {
      super.redo();
      mElement.addProperty(mPropName, mAddedIndex, mPropValue);
   }

   private ConfigElement mElement;
   private String        mPropName;
   private int           mAddedIndex;
   private Object        mPropValue;
}
