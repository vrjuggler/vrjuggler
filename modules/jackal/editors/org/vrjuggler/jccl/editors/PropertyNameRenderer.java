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
import javax.swing.*;
import javax.swing.table.*;
import org.vrjuggler.jccl.config.ConfigChunk;
import org.vrjuggler.jccl.config.PropertyDesc;

/**
 * A cell renderer that knows how to render ConfigChunk property names.
 */
public class PropertyNameRenderer
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
         JLabel lbl = (JLabel)comp;
         lbl.setHorizontalAlignment(SwingConstants.LEFT);
         lbl.setFont(lbl.getFont().deriveFont(Font.BOLD));

         // First row is the chunk name, second is the chunk type
         if (row == 0 || row == 1)
         {
            lbl.setFont(lbl.getFont().deriveFont(Font.ITALIC));
            lbl.setText((String)value);
            if (row == 1)
            {
               ((JComponent)comp).setBorder(
                  BorderFactory.createMatteBorder(0, 0, 2, 0, getForeground()));
            }
         }
         // Remainder of the table is a literal chunk property
         else
         {
            ConfigChunkTableModel data_model = (ConfigChunkTableModel)table.getModel();
            ConfigChunk chunk = data_model.getConfigChunk();
            PropertyDesc desc = data_model.getPropertyDesc(row);

            ((JComponent)comp).setBorder(null);

            // If the property has more than one value, we shall specialize on it
            int num_values = chunk.getNumPropertyValues(desc.getToken());
            if (num_values > 1)
            {
               // Get the index of the value in the property
               int base_row = data_model.getRowFor(desc);
               int value_idx = row - base_row;

               // If this is the first property value, we need two labels ...
               if (value_idx == 0)
               {
                  // Property name on the left, index on the right.
                  JPanel pnl = new JPanel();
                  pnl.setForeground(comp.getForeground());
                  pnl.setBackground(comp.getBackground());
                  pnl.setLayout(new BorderLayout());
                  // Property name label
                  pnl.add(comp, BorderLayout.CENTER);
                  // Index number label
                  JLabel idx_lbl = new JLabel(Integer.toString(value_idx), SwingConstants.RIGHT);
                  idx_lbl.setForeground(comp.getForeground());
                  idx_lbl.setBackground(comp.getBackground());
                  idx_lbl.setFont(comp.getFont().deriveFont(Font.PLAIN));
                  pnl.add(idx_lbl, BorderLayout.EAST);

                  comp = pnl;
               }
               // If this is not the first property value, just display the index
               else
               {
                  lbl.setText(Integer.toString(value_idx));
                  lbl.setHorizontalAlignment(SwingConstants.RIGHT);
                  lbl.setFont(comp.getFont().deriveFont(Font.PLAIN));
               }
            }
         }
      }
      return comp;
   }
}
