/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
import java.awt.Cursor;
import java.awt.Font;
import java.awt.event.*;
import java.util.EventObject;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;

import org.vrjuggler.jccl.config.ConfigChunk;
import org.vrjuggler.jccl.config.PropertyDesc;

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

      // Rows 0 and 1 are special
      if (row == 0 || row == 1)
      {
         delegate = new ConfigChunkCellEditor();
      }
      else
      {
         // Handle special case for the add property value button at the end of
         // property with variable values
         ConfigChunkTableModel data_model = (ConfigChunkTableModel)table.getModel();
         PropertyDesc desc = data_model.getPropertyDesc(row);
         ConfigChunk src_chunk = data_model.getConfigChunk();
         int num_values = src_chunk.getNumPropertyValues(desc.getToken());
         int value_idx = row - data_model.getRowFor(desc);
         if (desc.hasVariableNumberOfValues() && (value_idx == num_values))
         {
            delegate = new AddButtonCellEditor();
         }

         // Handle embedded chunks specially
         else if (value instanceof ConfigChunk)
         {
            delegate = new EmbeddedChunkCellEditor();
         }
         else
         {
            delegate = new ConfigChunkCellEditor();
         }
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

   /**
    * Specialized cell editor for the cells that are just an button.
    */
   private class AddButtonCellEditor
      extends AbstractCellEditor
      implements TableCellEditor
   {
      private Object value = null;
      private JTable table = null;
      private Component editorComponent = null;
      private int row = -1;

      /**
       * Gets the value contained in the editor.
       */
      public Object getCellEditorValue()
      {
         return value;
      }

      /**
       * Gets the component to edits the given value.
       */
      public Component getTableCellEditorComponent(JTable table, Object value,
                                                   boolean selected,
                                                   int row, int col)
      {
         this.value = value;
         this.table = table;
         this.row = row;

         // Assume that the table model is an instance of ConfigChunkTableModel and
         // get the property description for the row this cell is in
         ConfigChunkTableModel data_model = (ConfigChunkTableModel)table.getModel();
         PropertyDesc desc = data_model.getPropertyDesc(row);
         ConfigChunk src_chunk = data_model.getConfigChunk();

         int num_values = src_chunk.getNumPropertyValues(desc.getToken());
         int value_idx = row - data_model.getRowFor(desc);

         JButton btn = new JButton();
         btn.setText("Add New");
         btn.setFont(table.getFont().deriveFont(Font.BOLD));
         if (selected)
         {
            btn.setForeground(table.getSelectionForeground());
            btn.setBackground(table.getSelectionBackground());
         }
         else
         {
            btn.setForeground(table.getForeground());
            btn.setBackground(table.getBackground());
         }
         btn.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
         btn.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent evt)
            {
               AddButtonCellEditor.this.cancelCellEditing();
               addNewValue();
            }
         });

         editorComponent = btn;
         return editorComponent;
      }

      protected void addNewValue()
      {
         ConfigChunkTableModel data_model = (ConfigChunkTableModel)table.getModel();
         PropertyDesc desc = data_model.getPropertyDesc(row);
         ConfigChunk src_chunk = data_model.getConfigChunk();

         int add_index = src_chunk.getNumPropertyValues(desc.getToken());

         // Get the default value
         Object default_value = desc.getDefaultValue(add_index);

         src_chunk.setProperty(desc.getToken(), add_index, default_value);
      }
   }
}
