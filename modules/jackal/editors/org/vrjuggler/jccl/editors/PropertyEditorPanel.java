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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.jccl.editors;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
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
   public void finalize()
   {
      mConfigElement.removeConfigElementListener(this);

      if (mEditorComponent instanceof JComboBox)
      {
         JComboBox combo = (JComboBox)mEditorComponent;
         ActionListener[] listeners = combo.getActionListeners();
         for (int i = 0 ; i < listeners.length ; ++i)
         {
            combo.removeActionListener(listeners[i]);
         }
         
         FocusListener[] focus_listeners = combo.getFocusListeners();
         for (int i = 0 ; i < focus_listeners.length ; ++i)
         {
            combo.removeFocusListener(focus_listeners[i]);
         }
      }
      else if (mEditorComponent instanceof JTextField)
      {
         JTextField txt_field = (JTextField)mEditorComponent;
         ActionListener[] listeners = txt_field.getActionListeners();
         for (int i = 0 ; i < listeners.length ; ++i)
         {
            txt_field.removeActionListener(listeners[i]);
         }
         
         FocusListener[] focus_listeners = txt_field.getFocusListeners();
         for (int i = 0 ; i < focus_listeners.length ; ++i)
         {
            txt_field.removeFocusListener(focus_listeners[i]);
         }
      }
         

      super.finalize();
   }

   public PropertyEditorPanel(ConfigContext ctx, Object value, PropertyDefinition prop_def,
                              ConfigElement elm, int prop_num, Color color)
   {
      mConfigContext = ctx;
      mPropName = prop_def.getToken();
      mPropDef = prop_def;
      mConfigElement = elm;
      mPropIndex = prop_num;
      mColor = color;

      fillEditorComponent(value, prop_def);

      // Disable the editor component if the ConfigElement is read only.
      mEditorComponent.setEnabled(!mConfigElement.isReadOnly());

      mConfigElement.addConfigElementListener(this);

      this.setLayout(new BorderLayout());
      this.add(mEditorComponent, BorderLayout.CENTER);

      // Use the help information from the property definition set a tooltip
      // for this panel.  The first sentence of the help text will be used as
      // the tooltip, which is very much like the JavaDoc "brief" description.
      String help = mPropDef.getHelp();
      if ( help != null && ! help.equals("") )
      {
         help = help.trim();
         String[] split_help = help.split("[?.!]");
         String tooltip;

         // If split_help[0] is a substring of help, set the tooltip string to
         // be split_help[0] plus the character matched in the above regular
         // expression.
         if ( split_help[0].length() < help.length() )
         {
            tooltip = help.substring(0, split_help[0].length() + 1);
         }
         // Otherwise, just use split_help[0] directly.
         else
         {
            tooltip = split_help[0];
         }

         mEditorComponent.setToolTipText(tooltip);
      }
   }

   /**
    * Provides package visible access to the value edited by this panel;
    *
    * @since 0.92.11
    */
   Object getValue()
   {
      return mEditor.getValue();
   }

   /**
    * Enables or disables the editor component contained within this panel.
    *
    * @param enabled If true, this component and its editor component are
    *                enabled; otherwise they are disabled.
    */
   public void setEnabled(boolean enabled)
   {
      super.setEnabled(enabled);

      if ( mEditorComponent != null )
      {
         mEditorComponent.setEnabled(enabled);
      }
   }

   /**
    * Sets the font for the editor component contained within this panel.
    *
    * @param font The desired java.awt.Font for the contained editor
    *             component.
    */
   public void setFont(Font font)
   {
      super.setFont(font);

      if ( mEditorComponent != null )
      {
         mEditorComponent.setFont(font);
      }
   }

   public void nameChanged(ConfigElementEvent evt)
   {;}

   public void propertyValueChanged(ConfigElementEvent evt)
   {
      if(evt.getProperty().equals(mPropName) &&
         evt.getIndex() == mPropIndex)
      {
         Object value = mConfigElement.getProperty(mPropName, mPropIndex);
         if (value instanceof ConfigElementPointer)
         {
            value = ((ConfigElementPointer)value).getTarget();
         }

         // XXX: If this editor actually made the change to the porperty
         //      this will set the value twice. We could test to see if
         //      mEditor.getValue().equals(value), but this would waste
         //      cycles that are not needed since setting the value twice
         //      does not currently cause any errors. This could cause
         //      errors in the future though.
         mEditor.setValue(value);

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
   public void propertyValueOrderChanged(ConfigElementEvent evt)
   {;}
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

         mConfigElement.setProperty(mPropName, mPropIndex, mEditor.getValue(), mConfigContext);
      
         //return super.stopCellEditing();
         return true;
      }
      catch (IllegalArgumentException iae)
      {
         //XXX Print this out to the Tweek console
         System.out.println(iae.getMessage());
         JOptionPane.showMessageDialog(getParentFrame(),
                                       "Warning: Invalid property value '" +
                                          iae.getMessage() + "'",
                                       "Invalid Property Value Set",
                                       JOptionPane.WARNING_MESSAGE);

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
   protected JComponent createComboBox()
   {
      // Populate the box with the tags
      JComboBox box;

      if ( mPropDef.getType() == ConfigElementPointer.class )
      {
         final PropertyEditor editor = mEditor;
         box = new JComboBox(
            new DefaultComboBoxModel()
            {
               public Object getElementAt(int index)
               {
                  return editor.getTags()[index];
               }

               public int getSize()
               {
                  return editor.getTags().length;
               }
            }
         );
      }
      else
      {
         box = new JComboBox(mEditor.getTags());
      }

      mEditorComponent = box;
      //box.setBorder(BorderFactory.createLoweredBevelBorder());
      box.setEditable(mPropDef.enumIsEditable());
      box.setSelectedItem(mEditor.getAsText());
      box.setFont(box.getFont().deriveFont(Font.PLAIN));
      box.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            System.out.println("Action event coming from combox: " 
               + evt.getSource().getClass().getName() + "@" + Integer.toHexString(evt.getSource().hashCode()));
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
                                                                      mEditorComponent);
            if (null != context_editor)
            {
               context_editor.getHelpPane().setText(mPropDef.getHelp());
            }
         }
      });
      return mEditorComponent;
   }

   /**
    * Creates a text field allowing the user to edit the property as a
    * string.
    */
   protected JComponent createTextField()
   {
      // Populate the text field with the object's string value
      final JTextField txtField = new JTextField(mEditor.getAsText());

      // Editors for numeric data typically present the number value as
      // right justified text.
      if ( mEditor instanceof IntegerEditor || mEditor instanceof FloatEditor )
      {
         txtField.setHorizontalAlignment(SwingConstants.TRAILING);
      }

      mEditorComponent = txtField;
      txtField.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));
      txtField.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            txtField.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));
            // Force the focus to be lost.
            mEditorComponent.transferFocusUpCycle();
            // Force the focus to be transfered to the next component.
            //mEditorComponent.transferFocus();

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
                                                                      mEditorComponent);
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
      return mEditorComponent;
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
                                    getParentFrame(),
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

//      System.out.println("Using editor "+mEditor.getClass()+" for value of type "+value.getClass());
      
      if (value instanceof ConfigElementPointer)
      {
         value = ((ConfigElementPointer)value).getTarget();
      }

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
            cpe.setPropertyDefinition(propDef, mConfigContext);
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

   private java.awt.Container getParentFrame()
   {
      return SwingUtilities.getAncestorOfClass(java.awt.Frame.class, this);
   }

   protected PropertyDefinition mPropDef = null;
   protected ConfigContext      mConfigContext = null;
   protected ConfigElement      mConfigElement = null;
   protected int                mPropIndex = 0;
   protected PropertyEditor     mEditor = null;
   protected JComponent         mEditorComponent = null;
   protected String             mPropName = null;
}
