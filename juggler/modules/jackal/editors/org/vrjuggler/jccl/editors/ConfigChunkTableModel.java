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

import java.beans.*;
import java.util.*;
import javax.swing.table.*;
import org.vrjuggler.jccl.config.*;

/**
 * A table model for config chunks. This two-column model will return property
 * names for the first column and property values for the second column.
 */
public class ConfigChunkTableModel
   extends AbstractTableModel
{
   /**
    * Sets the config chunk this table model represents.
    */
   public void setConfigChunk(ConfigChunk chunk)
   {
      if (mChunk != null)
      {
         mChunk.removeConfigChunkListener(chunkListener);
      }
      mChunk = chunk;
      if (mChunk != null)
      {
         mChunk.addConfigChunkListener(chunkListener);
      }
      fireTableDataChanged();
   }

   /**
    * Gets the config chunk this table model represents.
    */
   public ConfigChunk getConfigChunk()
   {
      return mChunk;
   }

   /**
    * Gets the name of the given column.
    */
   public String getColumnName(int col)
   {
      return mColumnNames[col];
   }

   /**
    * Gets the number of rows in this model.
    */
   public int getRowCount()
   {
      int size = 0;
      if (mChunk != null)
      {
         List prop_descs = mChunk.getDesc().getPropertyDescs();
         for (Iterator itr = prop_descs.iterator(); itr.hasNext(); )
         {
            PropertyDesc desc = (PropertyDesc)itr.next();
            size += Math.max(mChunk.getNumPropertyValues(desc.getToken()),
                             desc.getItemsSize());

            // Add one for the add button if the values is variable
            if (desc.hasVariableNumberOfValues())
            {
               ++size;
            }
         }

         // Take into account rows for the chunk name and type
         size += 2;
      }
      return size;
   }

   /**
    * Gets the number of columns in this model.
    */
   public int getColumnCount()
   {
      return mColumnNames.length;
   }

   /**
    * Tests if the cell at the given (col,row) position in the table is
    * editable. In this case, only cells in the column 1 are editable.
    */
   public boolean isCellEditable(int row, int col)
   {
      // Can't edit the names column
      if (col == 0)
      {
         return false;
      }
      // The first row is always the chunk name
      if (row == 0)
      {
         return true;
      }
      // The second row is always the chunk type - NOT editable here
      if (row == 1)
      {
         return false;
      }
      // Can't edit embedded ConfigChunks directly
//      PropertyDesc desc = getPropertyDesc(row);
//      if (desc.getValType() == ValType.EMBEDDEDCHUNK)
//      {
//         return false;
//      }
      return true;
   }

   /**
    * Gets the PropertyDesc for the property at the given row index. This method
    * takes into account the number of values in each property.
    *
    * @return the requested property desc, null if row is invalid
    */
   public PropertyDesc getPropertyDesc(int row)
   {
      // Rows 0 and 1 are invalid since they are not explicit properties
      if (row == 0 || row == 1)
      {
         return null;
      }

      // We move our cur_row cursor along based on the number of values a
      // property has. Once we pass the row we're looking for, we've found the
      // PropertyDesc we want.
      int cur_row = 0;
      List prop_descs = mChunk.getDesc().getPropertyDescs();
      for (Iterator itr = prop_descs.iterator(); itr.hasNext(); )
      {
         // Move the cur_row cursor along by the number of values the current
         // property has
         PropertyDesc cur_desc = (PropertyDesc)itr.next();
         int num_values = Math.max(mChunk.getNumPropertyValues(cur_desc.getToken()),
                                   cur_desc.getItemsSize());
         cur_row += num_values;

         // Add one extra row for the add button if this desc has variable
         // number of values
         if (cur_desc.hasVariableNumberOfValues())
         {
            ++cur_row;
         }

         // If we've passed the row we're looking for, return that desc.
         if (row < (cur_row + 2))
         {
            return cur_desc;
         }
      }
      return null;
   }

   /**
    * Gets the row for the given property description's first value.
    */
   public int getRowFor(PropertyDesc desc)
   {
      // Start on the third row (row 2) because the first two are reserved for
      // the name and value of the chunk.
      int cursor = 2;
      List prop_descs = mChunk.getDesc().getPropertyDescs();
      for (Iterator itr = prop_descs.iterator(); itr.hasNext(); )
      {
         PropertyDesc cur_desc = (PropertyDesc)itr.next();
         // If this is not the desc we are looking for, move the cursor by the
         // number of values in the property
         if (! cur_desc.equals(desc))
         {
            cursor += Math.max(mChunk.getNumPropertyValues(cur_desc.getToken()),
                               cur_desc.getItemsSize());

            // Add one for the add button if this property is variable
            if (cur_desc.hasVariableNumberOfValues())
            {
               ++cursor;
            }
         }
         // If we found the desc we're looking for, add in the index and quit
         else
         {
            break;
         }
      }
      return cursor;
   }

   /**
    * Gets the value at the given (col,row) position in the table.
    */
   public Object getValueAt(int row, int col)
   {
      // First row is the chunk name
      if (row == 0)
      {
         return (col == 0) ? "Name" : mChunk.getName();
      }

      // Second row is the chunk type
      if (row == 1)
      {
         return (col == 0) ? "Type" : mChunk.getDesc().getName();
      }

      PropertyDesc desc = getPropertyDesc(row);
      if (col == 0)
      {
         return desc.getName();
      }
      else
      {
         // Get the right property value based on the offset of the base row for
         // the property to the index into that property
         int base_row = getRowFor(desc);
         int value_idx = row - base_row;
         int num_values = mChunk.getNumPropertyValues(desc.getToken());

         if (desc.hasVariableNumberOfValues() && (value_idx == num_values))
         {
            return "Add";
         }
         else
         {
            return mChunk.getProperty(desc.getToken(), value_idx);
         }
      }
   }

   /**
    * Sets the value at the given (col,row) position in the table.
    */
   public void setValueAt(Object value, int row, int col)
   {
      // We can only set values in the column zero
      if (col == 1)
      {
         // First row is always the chunk name
         if (row == 0)
         {
            mChunk.setName((String)value);
         }
         // Second row is always the chunk type ... invalid
         else if (row == 1)
         {
            // no-op
         }
         else
         {
            PropertyDesc desc = getPropertyDesc(row);

            // Figure out which property value we're editing
            int base_row = getRowFor(desc);
            int value_idx = row - base_row;

            // Modify the value and notify listeners
            mChunk.setProperty(desc.getToken(), value_idx, value);
            fireTableCellUpdated(row, col);
         }
      }
   }

   /**
    * Called by our chunk change listener whenever the chunk being modeled
    * changes its internal state.
    */
   private void chunkChanged()
   {
      // TODO: be smarter about deciding what has changed
      fireTableDataChanged();
   }

   /**
    * The name of the columns.
    */
   private String[] mColumnNames = { "Property", "Value" };

   /**
    * The config chunk this table model is representing.
    */
   private ConfigChunk mChunk = null;

   /**
    * Listener for changes made to the config chunk being displayed/edited.
    */
   private ChunkChangeListener chunkListener = new ChunkChangeListener();

   /**
    * Specialized property change listener to handle notifications when the
    * config chunk that is being edited changes.
    */
   private class ChunkChangeListener
      implements ConfigChunkListener
   {
      public void nameChanged(ConfigChunkEvent evt)
      {
         chunkChanged();
      }

      public void propertyValueChanged(ConfigChunkEvent evt)
      {
         chunkChanged();
      }

      public void propertyValueAdded(ConfigChunkEvent evt)
      {
         chunkChanged();
      }

      public void propertyValueRemoved(ConfigChunkEvent evt)
      {
         chunkChanged();
      }
   }
}
