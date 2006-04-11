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
 * A table model for chunk descs.
 */
public class ChunkDescTableModel
   extends AbstractTableModel
   implements PropertyChangeListener
{
   /**
    * Sets the chunk desc this table model represents.
    */
   public void setChunkDesc(ChunkDesc desc)
   {
      if (mDesc != null)
      {
         mDesc.removePropertyChangeListener(this);
      }
      mDesc = desc;
      if (mDesc != null)
      {
         mDesc.addPropertyChangeListener(this);
      }
      fireTableDataChanged();
   }

   /**
    * Gets the chunk desc this table model represents.
    */
   public ChunkDesc getChunkDesc()
   {
      return mDesc;
   }

   /**
    * This gets called whenever the contained ChunkDesc has been modified.
    */
   public void propertyChange(PropertyChangeEvent evt)
   {
      System.out.println("ChunkDescTableModel.propertyChanged("+evt.getPropertyName()+")");
      for (int i=0; i<getRowCount(); ++i)
      {
         PropertyDesc desc = getPropertyDesc(i);
         if (desc.getToken().equals(evt.getPropertyName()))
         {
            this.fireTableRowsUpdated(i, i);
         }
      }
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
      if (mDesc != null)
      {
         List prop_descs = mDesc.getPropertyDescs();
         size = prop_descs.size();
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
      return true;
   }

   /**
    * Gets the PropertyDesc for the property at the given row index.
    *
    * @return the requested property desc, null if row is invalid
    */
   public PropertyDesc getPropertyDesc(int row)
   {
      return (PropertyDesc)mDesc.getPropertyDescs().get(row);
   }

   /**
    * Gets the value at the given (col,row) position in the table.
    */
   public Object getValueAt(int row, int col)
   {
      PropertyDesc desc = getPropertyDesc(row);
      switch (col)
      {
      case 0: // Name
         return desc.getName();
      case 1: // Type
         return desc.getValType();
      case 2: // Token
         return desc.getToken();
      case 3: // Has variable values
         return new Boolean(desc.hasVariableNumberOfValues());
      default:
         return null;
      }
   }

   /**
    * Sets the value at the given (col,row) position in the table.
    */
   public void setValueAt(Object value, int row, int col)
   {
      PropertyDesc desc = getPropertyDesc(row);

      switch (col)
      {
      case 0: // Name
         desc.setName((String)value);
         break;
      case 1: // Type
         desc.setValType((ValType)value);
         break;
      case 2: // Token
         desc.setToken((String)value);
         break;
      case 3: // Has variable values
         desc.setHasVariableNumberOfValues(((Boolean)value).booleanValue());
         break;
      default:
         System.err.println("ChunkDescTableModel.setValueAt(): Bad col: "+col);
      }
      fireTableCellUpdated(row, col);
   }

   /**
    * The name of the columns.
    */
   private String[] mColumnNames = { "Property Name"
                                   , "Type"
                                   , "Token"
                                   , "Variable"
                                   };

   /**
    * The chunk desc this table model is representing.
    */
   private ChunkDesc mDesc = null;
}
