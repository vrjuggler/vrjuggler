/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
package org.vrjuggler.jccl.editors;

import java.awt.Component;
import java.util.EventObject;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;

import org.vrjuggler.jccl.config.ConfigChunk;

/**
 * Base cell editor for a ConfigChunk property sheet. This is essentially
 * implemented as an adapter that hands off the real work to a concrete cell
 * editor.
 */
public class PropertyValueEditor
   extends AbstractCellEditor
   implements TableCellEditor, CellEditorListener
{
   /**
    * Gets the value contained in the editor.
    */
   public Object getCellEditorValue()
   {
      return delegate.getCellEditorValue();
   }

   /**
    * Forwards this request on to the delegate.
    */
   public boolean stopCellEditing()
   {
      return delegate.stopCellEditing();
   }

   /**
    * Forwards this request on to the delegate.
    */
   public void cancelCellEditing()
   {
      delegate.cancelCellEditing();
   }

   /**
    * Forwards this request on to the delegate.
    */
   public boolean shouldSelectCell(EventObject evt)
   {
      return delegate.shouldSelectCell(evt);
   }

   /**
    * Forwards this request on to the delegate.
    */
   public boolean isCellEditable(EventObject evt)
   {
      return delegate.isCellEditable(evt);
   }

   /**
    * Gets the component that is the actual editing component.
    */
   public Component getTableCellEditorComponent(JTable table, Object value,
                                                boolean selected,
                                                int row, int col)
   {
      delegate.removeCellEditorListener(this);

      // Handle embedded chunks specially
      if (value instanceof ConfigChunk)
      {
         delegate = new EmbeddedChunkCellEditor();
      }
      else
      {
         delegate = new ConfigChunkCellEditor();
      }

      // Add ourselves as a listener to this new delegate
      delegate.addCellEditorListener(this);
      return delegate.getTableCellEditorComponent(table, value, selected,
                                                  row, col);
   }

   /**
    * Forwards the event notification to listeners making it look like the event
    * came from us.
    */
   public void editingCanceled(ChangeEvent evt)
   {
      fireEditingCanceled();
   }

   /**
    * Forwards the event notification to listeners making it look like the event
    * came from us.
    */
   public void editingStopped(ChangeEvent evt)
   {
      fireEditingStopped();
   }

   /**
    * The adaptee delegate who does all the real work.
    */
   private TableCellEditor delegate = new ConfigChunkCellEditor();
}
