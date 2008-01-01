/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
package org.vrjuggler.vrjconfig.customeditors.intersense;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import java.util.*;

import org.vrjuggler.jccl.config.*;


/**
 * A JTable to display all Intersense stations.
 */
public class StationTable
   extends JTable
{
   
   private StationEditor mStationEditor = new StationEditor();
   private StationCellRenderer mStationCellRenderer = new StationCellRenderer();
   private StationCellEditor   mStationCellEditor   = new StationCellEditor();

   /**
    * Constructor.
    */
   public StationTable()
   {
      // Setup the table defaults
      setShowGrid(true);
      setIntercellSpacing(new Dimension(1,1));
      setBackground(UIManager.getColor("Menu"));
      getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
   }

   /**
    * Set the model to view with this JTable.
    */
   public void setModel(TableModel dataModel)
   {
      super.setModel(dataModel);

      // If the StationEditor is not null set the column height/width
      // to correctly show the entire StationEditor.
      if(null != mStationEditor)
      {
         setRowHeight(mStationEditor.getPreferredSize().height+4);
         TableColumn clm = getColumnModel().getColumn(0);
         clm.setMinWidth(mStationEditor.getMinimumSize().width); 
         clm.setMaxWidth(mStationEditor.getPreferredSize().width);
         clm.setPreferredWidth(mStationEditor.getPreferredSize().width); 
      }
   }

   public boolean getScrollableTracksViewportWidth() 
   {
      return false;
   }
   
   /**
    * Gets the cell renderer for the cell at (col, row). This is overridden such
    * that parent nodes (that print the summary value list) for multi-valued
    * properties do not render the summary when they have been expanded.
    */
   public TableCellRenderer getCellRenderer(int row, int col)
   {
      return mStationCellRenderer;
   }

   /**
    * Gets the cell editor for the cell at (col, row).
    */
   public TableCellEditor getCellEditor(int row, int col)
   {
      return mStationCellEditor;
   }

   /**
    * Tests if the cell at (col, row) is editable. This is overridden such that
    * parent nodes (that print the summary value list) for multi-valued
    * properties are not editable when they have been expanded.
    */
   public boolean isCellEditable(int row, int col)
   {
      // Since this table only has one column, we always want 
      // to be able to edit it.
      return true;
   }

   /**
    * Custom TableCellRenderer to display the Intersense station editor.
    */
   protected class StationCellRenderer extends DefaultTableCellRenderer
   {
      public Component getTableCellRendererComponent(
          JTable table,
          Object value,            // value to display
          boolean isSelected,      // is the cell selected
          boolean cellHasFocus,    // the table and the cell have the focus
          int rox, int col)
      {
            //String s = value.toString();
            //setText(s);
            //setIcon((s.length() > 10) ? longIcon : shortIcon);
         if (isSelected || cellHasFocus)
         {
            mEditor.setBackground(table.getSelectionBackground());
            mEditor.setForeground(table.getSelectionForeground());
         }
         else
         {
            mEditor.setBackground(table.getBackground());
            mEditor.setForeground(table.getForeground());
         }
         setEnabled(table.isEnabled());
         //setFont(table.getFont());
         setOpaque(true);
         
         mEditor.setModel((StationModel)value);
         return mEditor;
      }      
      private StationEditor mEditor = new StationEditor();
   }
   protected class StationCellEditor extends AbstractCellEditor implements TableCellEditor
   {
      public Component getTableCellEditorComponent(JTable table, Object value, boolean isSelected, int row, int column)
      {
         if (isSelected)
         {
            mEditor.setBackground(table.getSelectionBackground());
            mEditor.setForeground(table.getSelectionForeground());
         }
         else
         {
            mEditor.setForeground(table.getForeground());
            mEditor.setBackground(table.getBackground());
         }
         mEditor.setEnabled(table.isEnabled());
         //setFont(table.getFont());
         mEditor.setOpaque(true);
         mEditor.setModel((StationModel)value);
         return mEditor;
      }
       
      public Object getCellEditorValue()
      { return "Hello"; }
      
      private StationEditor mEditor = new StationEditor();
   }
}

