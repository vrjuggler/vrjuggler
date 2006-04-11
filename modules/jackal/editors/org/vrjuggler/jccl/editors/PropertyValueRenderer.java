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

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.table.*;
import org.vrjuggler.jccl.config.ConfigChunk;
import org.vrjuggler.jccl.config.DescEnum;
import org.vrjuggler.jccl.config.PropertyDesc;
import org.vrjuggler.jccl.config.ValType;

/**
 * A cell renderer that knows how to render ConfigChunk property values.
 */
public class PropertyValueRenderer
   extends DefaultTableCellRenderer
{
   /**
    * Gets the cell renderer component for the given table cell.
    */
   public Component getTableCellRendererComponent(JTable table,
                                                  Object value,
                                                  boolean selected,
                                                  boolean focus,
                                                  int row,
                                                  int column)
   {
      Component comp = super.getTableCellRendererComponent(table, value,
                                                           selected, focus,
                                                           row, column);
      if (table.getModel() instanceof ConfigChunkTableModel)
      {

         // First row is the chunk name, second is the chunk type
         if (row == 0 || row == 1)
         {
            ((JLabel)comp).setText((String)value);
            if (row == 1)
            {
               ((JComponent)comp).setBorder(
                  BorderFactory.createMatteBorder(0, 0, 2, 0, getForeground()));
            }
         }
         else
         {
            ConfigChunkTableModel data_model = (ConfigChunkTableModel)table.getModel();
            PropertyDesc desc = data_model.getPropertyDesc(row);
            ConfigChunk src_chunk = data_model.getConfigChunk();

            ((JComponent)comp).setBorder(null);

            // If the property has variable values and this row is past the end
            // of the number of values, render an add button
            int num_values = src_chunk.getNumPropertyValues(desc.getToken());
            int value_idx = row - data_model.getRowFor(desc);
            if (desc.hasVariableNumberOfValues() && (value_idx == num_values))
            {
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

               comp = btn;
            }

            // If the cell value is an embedded chunk, render a hyperlink.
            else if (desc.getValType() == ValType.EMBEDDEDCHUNK)
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

               comp = btn;
            }

            // If the value has enumerated values, render the name instead
            else if (desc.getNumEnums() > 0 && desc.getValType() != ValType.CHUNK)
            {
               for (int i=0; i<desc.getNumEnums(); ++i)
               {
                  DescEnum de = desc.getEnumAt(i);
                  if (de.getValue().equals(value))
                  {
                     setText(de.getName());
                     break;
                  }
               }
            }
         }
      }
      return comp;
   }
}
