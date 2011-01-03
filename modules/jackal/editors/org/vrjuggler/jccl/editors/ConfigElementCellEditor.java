/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.jccl.editors;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Font;
import java.awt.Insets;
import java.awt.event.*;
import java.beans.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;
import org.vrjuggler.jccl.config.*;

/**
 * Specialized cell editor for the property sheet that uses PropertyEditors
 * originally intended for use with JavaBeans.
 */
public class ConfigElementCellEditor
   extends AbstractCellEditor
   implements TableCellEditor
{
   private PropertyEditor mEditor = null;
   private Component mEditorComponent = null;
   private PropertyDefinition mPropDefinition = null;

   /**
    * Gets the value contained in the editor.
    */
   public Object getCellEditorValue()
   {
      return mEditor.getValue();
   }

   /**
    * Sets the property definition this editor should use for the value it is
    * editing.
    */
   public void setPropertyDefinition(PropertyDefinition propDef)
   {
      this.mPropDefinition = propDef;
   }


   /**
    * When it has been requested to stop editing the cell, validate that the
    * current value in the editor is ok.
    *
    * @return  true if editing was stopped successfully, false if the value
    *          currently in the editor is not valid for the property
    */
   public boolean stopCellEditing()
   {
      try
      {
         if (mEditorComponent instanceof JComboBox)
         {
            JComboBox combo = (JComboBox)mEditorComponent;
            mEditor.setAsText((String)combo.getSelectedItem());
         }
         else if (mEditorComponent instanceof JTextField)
         {
            JTextField txt_field = (JTextField)mEditorComponent;
            mEditor.setAsText(txt_field.getText());
         }
         return super.stopCellEditing();
      }
      catch (IllegalArgumentException iae)
      {
         //XXX Print this out to the Tweek console
         System.out.println(iae.getMessage());
      }
      return false;
   }

   /**
    * Gets the component to edits the given value.
    */
   public Component getTableCellEditorComponent(JTable table, Object value,
                                                boolean selected,
                                                int row, int col)
   {
      // The first row is always the element name
      if (row == 0)
      {
         mEditor = PropertyEditorManager.findEditor(value.getClass());
      }
      else
      {
         // If the value is an embedded ConfigElement, just use a button
         if (value instanceof ConfigElement)
         {
            ConfigElement elt = (ConfigElement)value;
            String name = elt.getName();
            JButton btn = new JButton(name);
            btn.setMargin(new Insets(0,0,0,0));
            btn.setBorderPainted(false);
            return btn;
         }

         // Get the property editor for the given value
         mEditor = findEditor(value, mPropDefinition);
      }
      if (mEditor == null)
      {
         // We still can't find an editor for the value! Our lives have failed
         return null;
      }

//      System.out.println("Using editor "+mEditor.getClass()+" for value of type "+value.getClass());
      mEditor.setValue(value);

      // If the property editor supports known tagged values, use a combobox
      if (mEditor.getTags() != null)
      {
         mEditorComponent = createComboBox();
         mEditorComponent.setBackground(table.getBackground());
      }
      // Property editor does not have known tagged values, use a textfield
      else
      {
         mEditorComponent = createTextField();
      }

      // If the editor supports a custom editor, slap a decorator on the
      // default editor that has a button that will open the custom editor in
      // a new dialog
      if (mEditor.supportsCustomEditor())
      {
         Component btn = getCustomEditorButton();
         JPanel pnl = new JPanel();
         pnl.setLayout(new BorderLayout());
         pnl.add(mEditorComponent, BorderLayout.CENTER);
         pnl.add(btn, BorderLayout.EAST);

         mEditorComponent = pnl;
      }

      // Make the editor component look like the table
      mEditorComponent.setForeground(table.getForeground());

      return mEditorComponent;
   }

   /**
    * Gets a property editor for the given property value described by the given
    * property definition.
    */
   protected PropertyEditor findEditor(Object value, PropertyDefinition propDef)
   {
      PropertyEditor ed = null;

      // First check if we have a specialized editor for the PropertyDefitition
      // based on its unique token.
      ed = PropertyDefinitionEditorRegistry.findEditor(propDef.getToken());

      // If there is no specialized editor for the PropertyDefinition based on
      // its token, try to find one based on its type.
      if (ed == null)
      {
         // If the value is an element pointer, use a special editor
         if (propDef.getType() == ConfigElementPointer.class)
         {
            ConfigElementPointerEditor cpe = new ConfigElementPointerEditor();
            //cpe.setPropertyDefinition(propDef);
            ed = cpe;
         }
      }

      // If there is no specialized editor for the PropertyDefinition, check if
      // this editor has enumerated values.
      if (ed == null)
      {
         // This editor has enumerated values, use a special editor
         if (propDef.getEnumsCount() > 0)
         {
            EnumeratedPropertyValueEditor epve = new EnumeratedPropertyValueEditor();
            epve.setPropertyDefinition(propDef);
            ed = epve;
         }
      }

      // As a last resort, find an editor for the value class
      if (ed == null)
      {
         ed = PropertyEditorManager.findEditor(value.getClass());
      }
      return ed;
   }

   /**
    * Creates a combo box containing the tags a property editor supports.
    */
   protected Component createComboBox()
   {
      // Populate the box with the tags
      JComboBox box = new JComboBox(mEditor.getTags());
      box.setSelectedItem(mEditor.getAsText());
      box.setFont(box.getFont().deriveFont(Font.PLAIN));
      box.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            stopCellEditing();
         }
      });
      return box;
   }

   /**
    * Creates a text field allowing the user to edit the property as a
    * string.
    */
   protected Component createTextField()
   {
      // Populate the text field with the object's string value
      JTextField txtField = new JTextField(mEditor.getAsText());
      txtField.setBorder(BorderFactory.createEmptyBorder(0,0,0,0));
      txtField.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            stopCellEditing();
         }
      });
      txtField.addFocusListener(new FocusAdapter()
      {
         public void focusLost(FocusEvent evt)
         {
            stopCellEditing();
         }
      });
      return txtField;
   }

   /**
    * Creates a button that will open open the custom editor for the given
    * PropertyEditor.
    */
   protected Component getCustomEditorButton()
   {

      // Create a button that will open the custom editor dialog
      JButton moreBtn = new JButton("...");
      moreBtn.setMargin(new Insets(0,0,0,0));
      moreBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            // Create a new dialog to contain the custom editor
            int result = JOptionPane.showConfirmDialog(
                                    SwingUtilities.getAncestorOfClass(java.awt.Frame.class,
                                                                      (Component)evt.getSource()),
                                    mEditor.getCustomEditor(),
                                    "Custom Editor",
                                    JOptionPane.OK_CANCEL_OPTION,
                                    JOptionPane.PLAIN_MESSAGE);
            // XXX Handle the result appropriately here
         }
      });

      return moreBtn;
   }
}
