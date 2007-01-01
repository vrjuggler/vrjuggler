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
package org.vrjuggler.tweek.ui;

import java.awt.*;
import java.awt.event.*;
import java.beans.*;
import java.io.Serializable;
import java.lang.reflect.*;
import javax.swing.*;
import javax.swing.table.*;

/**
 * A bean that can edit a bean's properties.
 */
public class PropertySheet
   extends JPanel
   implements Serializable
{
   /**
    * Creates a new PropertySheet with no bean set to edit.
    */
   public PropertySheet()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      model = new PropertySheetTableModel();
      table.setModel(model);
      table.getColumnModel().getColumn(1).setCellEditor(new PropertySheetCellEditor());
//      System.out.println("Cell Editor: "+table.getCellEditor());
      System.out.println("Column Editor: "+table.getColumnModel().getColumn(1).getCellEditor());
   }

   /**
    * Sets the bean this property sheet is editing.
    */
   public void setBean(Object bean)
   {
      this.bean = bean;
      model.setBean(bean);
   }

   /**
    * Gets the bean this property sheet is editing.
    */
   public Object getBean()
   {
      return bean;
   }

   /**
    * The target bean being edited.
    */
   private Object bean;

   /**
    * The table model for our bean's properties.
    */
   private PropertySheetTableModel model;


   BorderLayout baseLayout = new BorderLayout();
   JScrollPane tableScrollPane = new JScrollPane();
   JTable table = new JTable();

   private void jbInit() throws Exception
   {
      this.setLayout(baseLayout);
      this.add(tableScrollPane, BorderLayout.CENTER);
      tableScrollPane.getViewport().add(table, null);
   }

//   /**
//    * Specialized cell editor for our property sheet so that we can use a
//    * different editor for each individual property.
//    */
//   class PropertySheetCellEditor
//      extends DefaultCellEditor
//   {
//      /**
//       * The current cell's property type.
//       */
//      Class propType = null;
//
//      public PropertySheetCellEditor()
//      {
//         // stupid parent class expects a checkbox, combobox or textfield
//         super(new JCheckBox());
//      }
//
//      /**
//       * Gets the value of this cell editor.
//       */
//      public Object getCellEditorValue()
//      {
//         delegate.getValue();
//         // Property is a number.
//         if (Integer.class.isAssignableFrom(propType))
//         {
//            return Integer.parseInt(((JTextField)editorComponent).getText());
//         }
//         // Property is a string
//         if (String.class.isAssignableFrom(propType))
//         {
//            return ((JTextField)editorComponent).getText();
//         }
//         return null;
//      }
//
//      public Component getTableCellEditorComponent(JTable table,
//                                                   Object value,
//                                                   boolean selected,
//                                                   int row,
//                                                   int column)
//      {
//         propType = model.getPropertyType(row);
//         // Number class, convert to Number object
//         if (Number.class.isAssignableFrom(propType))
//         {
//         }
//         return super.getTableCellEditorComponent(table, value, selected, row, column);
//      }
//
//      /**
//       * Listens to an editor component and helps manage its value.
//       */
//      class EditorComponentHelper
//         implements ActionListener,
//                    ItemListener,
//                    Serializable
//      {
//         protected Object value = null;
//
//         public Object getValue()
//         {
//            return value;
//         }
//
//         public void setValue(Object value)
//         {
//            this.value = value;
//         }
//
//         public boolean startCellEditing(EventObject evt)
//         {
//            return true;
//         }
//
//         public boolean stopCellEditing()
//         {
//            fireEditingStopped();
//            return true;
//         }
//
//         public void cancelCellEditing()
//         {
//            fireEditingStoped();
//         }
//
//         public void actionPerformed(ActionEvent evt)
//         {
//            PropertySheetCellEditor.this.stopCellEditing();
//         }
//
//         public void itemStateChanged(ItemEvent evt)
//         {
//            PropertySheetCellEditor.this.stopCellEditing();
//         }
//      }
//   }
}
