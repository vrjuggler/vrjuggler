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

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.Insets;
import java.awt.event.*;
import java.beans.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.beans.BeanRegistry;

/**
 * Specialized cell editor for the property sheet that uses PropertyEditors
 * originally intended for use with JavaBeans.
 */
public class EmbeddedChunkCellEditor
   extends AbstractCellEditor
   implements TableCellEditor
{
   private Object value = null;
   private JTable table = null;
   private Component editorComponent = null;

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

      // Assume that the table model is an instance of ConfigChunkTableModel and
      // get the property description for the row this cell is in
      ConfigChunkTableModel data_model = (ConfigChunkTableModel)table.getModel();
      PropertyDesc desc = data_model.getPropertyDesc(row);

      // If the value is an embedded ConfigChunk, just use a button
      if (desc.getValType() == ValType.EMBEDDEDCHUNK)
      {
         ConfigChunk chunk = (ConfigChunk)value;

         JButton btn = new JButton();
         btn.setText("<html><font size=-1><a href=\"\">"+chunk.getName()+"</a></font></html>");
         btn.setHorizontalAlignment(JButton.LEFT);
         btn.setMargin(new Insets(0,0,0,0));
         btn.setBorderPainted(false);
         btn.setFocusPainted(false);
         btn.setFont(table.getFont());
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
               cancelCellEditing();
               embeddedChunkSelected();
            }
         });

         editorComponent = btn;
      }
      return editorComponent;
   }

   protected void embeddedChunkSelected()
   {
      ConfigChunkPropertySheet sheet = (ConfigChunkPropertySheet)table;
      sheet.embeddedChunkSelected((ConfigChunk)value);
   }
}
