/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
package org.vrjuggler.tweek.wizard.builder;

import java.awt.*;
import java.awt.event.*;
import java.io.Serializable;
import javax.swing.*;
import javax.swing.table.*;

/**
 * Specialized cell editor for the property sheet that will show a different
 * editor or a cell by cell basis depending on the cell's value's type rather
 * than depending on the column's class type.
 */
public class PropertySheetCellEditor
   extends AbstractCellEditor
   implements TableCellEditor
{
   /**
    * We delegate the actual editing to another component through a delegate
    * helper instance.
    */
   protected EditorComponentDelegate delegate;

   /**
    * Creates a new PropertySheet cell editor with no special editing component.
    */
   public PropertySheetCellEditor()
   {
      System.out.println("PropertySheetCellEditor constructor");
   }

   /**
    * Gets the value of the cell this editor is currently editing.
    */
   public Object getCellEditorValue()
   {
      return delegate.getValue();
   }

   /**
    * Gets the component used to edit the given cell.
    */
   public Component getTableCellEditorComponent(JTable table,
                                                Object value,
                                                boolean selected,
                                                int row,
                                                int column)
   {
      Class propType = value.getClass();
      System.out.println("Type for ("+row+", "+column+") is "+propType);
      // This property is an integer
      if (Integer.class.isAssignableFrom(propType))
      {
         JTextField txt = new JTextField();
         delegate = new EditorComponentDelegate(txt)
         {
            public void setValue(Object value)
            {
               JTextField comp = (JTextField)editorComponent;
               comp.setText((value != null) ? value.toString() : "");
            }

            public Object getValue()
            {
               JTextField comp = (JTextField)editorComponent;
               try
               {
                  Integer value = new Integer(comp.getText());
                  return value;
               }
               catch (NumberFormatException nfe)
               {
                  return null;
               }
            }
         };
         txt.addActionListener(delegate);
      }
      // This property is a string
      if (String.class.isAssignableFrom(propType))
      {
         JTextField txt = new JTextField();
         delegate = new EditorComponentDelegate(txt)
         {
            public void setValue(Object value)
            {
               JTextField comp = (JTextField)editorComponent;
               comp.setText((value != null) ? value.toString() : "");
            }

            public Object getValue()
            {
               JTextField comp = (JTextField)editorComponent;
               return comp.getText();
            }
         };
         txt.addActionListener(delegate);
      }
      // This property is a boolean
      if (Boolean.class.isAssignableFrom(propType))
      {
         JCheckBox box = new JCheckBox();
         delegate = new EditorComponentDelegate(box)
         {
            public void setValue(Object value)
            {
               JCheckBox comp = (JCheckBox)editorComponent;
               comp.setSelected(((Boolean)value).booleanValue());
            }

            public Object getValue()
            {
               JCheckBox comp = (JCheckBox)editorComponent;
               return new Boolean(comp.isSelected());
            }
         };
         box.addActionListener(delegate);
      }
      return delegate.getComponent();
   }

   /**
    * Helper class that manages the UI that actually edits the cell's value.
    */
   protected abstract class EditorComponentDelegate
      implements ActionListener,
                 ItemListener,
                 Serializable
   {
      protected Component editorComponent;

      public EditorComponentDelegate(Component comp)
      {
         this.editorComponent = comp;
      }

      public abstract Object getValue();
      public abstract void setValue(Object value);

      public Component getComponent()
      {
         return editorComponent;
      }

      public void actionPerformed(ActionEvent evt)
      {
         stopCellEditing();
      }

      public void itemStateChanged(ItemEvent evt)
      {
         stopCellEditing();
      }
   }
}
