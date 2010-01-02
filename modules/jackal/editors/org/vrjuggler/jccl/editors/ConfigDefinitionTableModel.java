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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.jccl.editors;

import java.beans.*;
import java.util.*;
import javax.swing.table.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.jccl.config.event.ConfigDefinitionEvent;

/**
 * A table model for configuration definitions.
 */
public class ConfigDefinitionTableModel
   extends AbstractTableModel
   implements PropertyChangeListener
{
   /**
    * Sets the configuration definition this table model represents.
    */
   public void setDefinition(ConfigDefinition def)
   {
      if (mDefinition != null)
      {
         mDefinition.removeConfigDefinitionListener(mDefListener);
      }
      mDefinition = def;
      if (mDefinition != null)
      {
         mDefinition.addConfigDefinitionListener(mDefListener);
      }
      fireTableDataChanged();
   }

   /**
    * Gets the config definition this table model represents.
    */
   public ConfigDefinition getDefinition()
   {
      return mDefinition;
   }

   /**
    * This gets called whenever the contained definition has been modified.
    */
   public void propertyChange(PropertyChangeEvent evt)
   {
      System.out.println("ConfigDefinitionTableModel.propertyChanged("+evt.getPropertyName()+")");
      for (int i=0; i<getRowCount(); ++i)
      {
         PropertyDefinition prop_def = getPropertyDefinition(i);
         if (prop_def.getToken().equals(evt.getPropertyName()))
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
      if (mDefinition != null)
      {
         size = mDefinition.getPropertyDefinitionsCount();
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
    * Gets the PropertyDefinition for the property at the given row index.
    *
    * @return the requested property definition, null if row is invalid
    */
   public PropertyDefinition getPropertyDefinition(int row)
   {
      return (PropertyDefinition)mDefinition.getPropertyDefinitions().get(row);
   }

   /**
    * Gets the value at the given (col,row) position in the table.
    */
   public Object getValueAt(int row, int col)
   {
      PropertyDefinition prop_def = getPropertyDefinition(row);
      switch (col)
      {
      case 0: // Type
         return prop_def.getType();
      case 1: // Token
         return prop_def.getToken();
      case 2: // Has variable values
         return new Boolean(prop_def.isVariable());
      default:
         return null;
      }
   }

   /**
    * Sets the value at the given (col,row) position in the table.
    */
   public void setValueAt(Object value, int row, int col)
   {
      PropertyDefinition prop_def = getPropertyDefinition(row);

      switch (col)
      {
      case 0: // Type
         prop_def.setType((Class)value);
         break;
      case 1: // Token
         prop_def.setToken((String)value);
         break;
      case 2: // Has variable values
         prop_def.setVariable(((Boolean)value).booleanValue());
         break;
      default:
         System.err.println("ConfigDefinitionTableModel.setValueAt(): Bad col: "+col);
      }
      fireTableCellUpdated(row, col);
   }

   /** The name of the columns. */
   private String[] mColumnNames = { "Type"
                                   , "Token"
                                   , "Variable"
                                   };

   /** The configuration definition this table model is representing. */
   private ConfigDefinition mDefinition = null;

   /** Custom config definition listener. */
   private ChangeListener mDefListener = new ChangeListener();

   private class ChangeListener
      extends ConfigDefinitionAdapter
   {
      public void propertyDefinitionAdded(ConfigDefinitionEvent evt)
      {
         // TODO
      }

      public void propertyDefinitionRemoved(ConfigDefinitionEvent evt)
      {
         // TODO
      }
   }
}
