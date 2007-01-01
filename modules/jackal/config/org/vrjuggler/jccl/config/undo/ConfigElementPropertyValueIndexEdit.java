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

package org.vrjuggler.jccl.config.undo;

import javax.swing.undo.AbstractUndoableEdit;
import javax.swing.undo.CannotUndoException;
import javax.swing.undo.CannotRedoException;

import org.vrjuggler.jccl.config.ConfigElement;


/**
 * Undoable edit type for changing the index of a value within a
 * variable-valued config element property.
 *
 * @since 0.92.11
 */
public class ConfigElementPropertyValueIndexEdit
   extends AbstractUndoableEdit
{
   /**
    * Creates a new undoable edit object for a change in the index of a
    * property value within the given config element.
    *
    * @param elm        the element where the change was made
    * @param name       the name of the property within <code>elm</code>
    *                   that had its property values reordered
    * @param oldIndex   the old index of the property value
    * @param newIndex   the new index of the property value
    */
   public ConfigElementPropertyValueIndexEdit(ConfigElement elm, String name,
                                              int oldIndex, int newIndex)
   {
      super();
      mElement  = elm;
      mPropName = name;
      mOldIndex = oldIndex;
      mNewIndex = newIndex;
   }

   public void undo() throws CannotUndoException 
   {      
      super.undo();
      mElement.setPropertyValueIndex(mPropName, mNewIndex, mOldIndex);
   }

   public void redo() throws CannotRedoException
   {
      super.redo();
      mElement.setPropertyValueIndex(mPropName, mOldIndex, mNewIndex);
   }

   private ConfigElement mElement;
   private String        mPropName;
   private int           mOldIndex;
   private int           mNewIndex;
}
