package org.vrjuggler.jccl.editors;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.Font;
import java.awt.Insets;
import java.beans.PropertyEditor;
import java.beans.PropertyEditorManager;
import javax.swing.*;
import javax.swing.border.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;

public class PropertyEditorPanel extends PropertyComponent 
                                 implements ConfigElementListener
{   
   public PropertyEditorPanel(Object value, PropertyDefinition prop_def, ConfigElement elm, 
                              int prop_num, Color color)
   {
      mPropName = prop_def.getToken();
      mPropDef = prop_def;
      mConfigElement = elm;
      mPropNum = prop_num;
      mColor = color;

      fillEditorComponent(value, prop_def);
     
      mConfigElement.addConfigElementListener(this);
      
      this.setLayout(new BorderLayout());
      this.add(mEditorComponent, BorderLayout.CENTER);
   }

   public void refresh()
   {
      updateMyRow();
      this.getLayout().layoutContainer(this);
      this.repaint();
      updateMyRow();
   }

   public void nameChanged(ConfigElementEvent evt)
   {;}
   public void propertyValueChanged(ConfigElementEvent evt)
   {
      if(evt.getProperty().equals(mPropName))
      {
         mEditor.setValue(mConfigElement.getProperty(mPropName, mPropNum));

         if (mEditorComponent instanceof JComboBox)
         {
            JComboBox combo = (JComboBox)mEditorComponent;
            combo.setSelectedItem(mEditor.getAsText());
         }
         else if (mEditorComponent instanceof JTextField)
         {
            JTextField txt_field = (JTextField)mEditorComponent;
            txt_field.setText(mEditor.getAsText());
         }
      }
   }
   public void propertyValueAdded(ConfigElementEvent evt)
   {;}
   public void propertyValueRemoved(ConfigElementEvent evt)
   {;}
   
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
        
         ConfigElementPropertyEdit new_edit = mConfigElement.setProperty(mPropName, mPropNum, mEditor.getValue());
         System.out.println("Adding: " + new_edit);
         ConfigUndoManager.instance().addEdit(new_edit);
         
      
         //return super.stopCellEditing();
         return true;
      }
      catch (IllegalArgumentException iae)
      {
         //XXX Print this out to the Tweek console
         System.out.println(iae.getMessage());
         JOptionPane.showMessageDialog(null, "Warning: " + iae.getMessage());
         if (mEditorComponent instanceof JTextField)
         {
            ((JTextField)mEditorComponent).setText(mEditor.getAsText());
         }
      }
      return false;
   }
   
   /**
    * Creates a combo box containing the tags a property editor supports.
    */
   protected Component createComboBox()
   {
      // Populate the box with the tags
      final JComboBox box = new JComboBox(mEditor.getTags());
      //box.setBorder(BorderFactory.createLoweredBevelBorder());
      box.setSelectedItem(mEditor.getAsText());
      box.setFont(box.getFont().deriveFont(Font.PLAIN));
      box.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            stopCellEditing();
         }
      });
      box.addFocusListener(new FocusAdapter()
      {
         public void focusGained(FocusEvent evt)
         {
            // Set the help text.
            ConfigContextEditor context_editor =
               (ConfigContextEditor)SwingUtilities.getAncestorOfClass(ConfigContextEditor.class,
                                                                      box);
            if (null != context_editor)
            {
               context_editor.getHelpPane().setText(mPropDef.getHelp());
            }
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
      final JTextField txtField = new JTextField(mEditor.getAsText());
      txtField.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));
      txtField.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            txtField.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));
            // Force the focus to be lost.
            txtField.transferFocusUpCycle();
            // Force the focus to be transfered to the next component.
            //txtField.transferFocus();

            // This is no longer needed since the above line will force a
            // focusLostEvent. But I have choosen to leave this line here in
            // case it might become useful later.
            //stopCellEditing();
         }
      });
      txtField.addFocusListener(new FocusAdapter()
      {
         public void focusGained(FocusEvent evt)
         {
            txtField.setBorder(BorderFactory.createLineBorder(Color.black, 2));

            // Set the help text.
            ConfigContextEditor context_editor =
               (ConfigContextEditor)SwingUtilities.getAncestorOfClass(ConfigContextEditor.class,
                                                                      txtField);
            if (null != context_editor)
            {
               context_editor.getHelpPane().setText(mPropDef.getHelp());
            }
         }
         public void focusLost(FocusEvent evt)
         {
            txtField.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));
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
                                    (Component)evt.getSource(),
                                    mEditor.getCustomEditor(),
                                    "Custom Editor",
                                    JOptionPane.OK_CANCEL_OPTION,
                                    JOptionPane.PLAIN_MESSAGE);
            // XXX Handle the result appropriately here
         }
      });

      return moreBtn;
   }

   /**
    * Gets the component to edits the given value.
    */
   public void fillEditorComponent(Object value, PropertyDefinition prop_def)
   {
      //XXX: Never going to happen.
      // If the value is an embedded ConfigElement, just use a button
      
      if (value instanceof ConfigElement)
      {
         /*
         ConfigElement elt = (ConfigElement)value;
         String name = elt.getName();
         JButton btn = new JButton(name);
         btn.setMargin(new Insets(0,0,0,0));
         btn.setBorderPainted(false);
         */
         return;
      }

      // Get the property editor for the given value
      mEditor = findEditor(value, prop_def);
      if (mEditor == null)
      {
         // We still can't find an editor for the value! Our lives have failed
         return;
      }

      System.out.println("Using editor "+mEditor.getClass()+" for value of type "+value.getClass());
      mEditor.setValue(value);

      // If the property editor supports known tagged values, use a combobox
      if (mEditor.getTags() != null)
      {
         mEditorComponent = createComboBox();
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
      //mEditorComponent.setForeground(table.getForeground());

      //return mEditorComponent;
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
            cpe.setPropertyDefinition(propDef);
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

   protected PropertyDefinition mPropDef = null;
   protected ConfigElement      mConfigElement = null;
   protected int                mPropNum = 0;
   protected PropertyEditor     mEditor = null;
   protected Component          mEditorComponent = null;
   protected String             mPropName = null;
}
