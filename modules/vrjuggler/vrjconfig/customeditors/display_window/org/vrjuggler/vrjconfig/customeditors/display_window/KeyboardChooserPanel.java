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

package org.vrjuggler.vrjconfig.customeditors.display_window;

import java.awt.Color;
import java.awt.Container;
import java.awt.Font;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;
import javax.swing.*;
import info.clearthought.layout.TableLayout;
import info.clearthought.layout.TableLayoutConstraints;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;


public class KeyboardChooserPanel
   extends JPanel
   implements ConfigElementListener
            , EditorConstants
{
   /**
    *
    * @param ctx        the config context that contains elt
    * @param elt        a config element whose definition has
    *                   EditorConstants.INPUT_AREA_TYPE as a parent type
    */
   public KeyboardChooserPanel(ConfigContext ctx, ConfigElement elt,
                               boolean allowKbdEdit)
   {
      mContext = ctx;

      // elt's config definition must have EditorConstants.INPUT_AREA_TYPE as
      // a base type.
      if ( ! elt.getDefinition().getParents().contains(INPUT_AREA_TYPE) )
      {
         throw new IllegalArgumentException("Invalid config element of type '" +
                                            elt.getDefinition().getToken() +
                                            "' -- must have " +
                                            INPUT_AREA_TYPE + " as a parent");
      }

      mInputAreaElement = elt;

      // Extract the value of the keyboard/mouse property from the given
      // INPUT_AREA_TYPE config element.
      String prop_name = KEYBOARD_MOUSE_PTR_PROPERTY;
      ConfigElementPointer kbd_dev_ptr =
         (ConfigElementPointer) elt.getProperty(prop_name, 0);
      updateKeyboardDeviceElement(kbd_dev_ptr);

      mInputAreaElement.addConfigElementListener(this);

      try
      {
         PropertyDefinition prop_def =
            elt.getDefinition().getPropertyDefinition(prop_name);
         mKeyboardEditor = new PropertyEditorPanel(ctx, kbd_dev_ptr, prop_def,
                                                   elt, 0, Color.white);

         // Use a relatively small font for our components that display text.
         mKeyboardEditor.setFont(new Font("Dialog", 0, 10));
         mEditButton.setFont(new Font("Dialog", 0, 10));

         jbInit();

         if ( allowKbdEdit )
         {
            this.add(mKeyboardEditor,
                     new TableLayoutConstraints(0, 0, 0, 0,
                                                TableLayoutConstraints.FULL,
                                                TableLayoutConstraints.CENTER));
            this.add(mEditButton,
                     new TableLayoutConstraints(1, 0, 1, 0,
                                                TableLayoutConstraints.FULL,
                                                TableLayoutConstraints.CENTER));
         }
         else
         {
            this.add(mKeyboardEditor,
                     new TableLayoutConstraints(0, 0, 1, 0,
                                                TableLayoutConstraints.FULL,
                                                TableLayoutConstraints.CENTER));
         }
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }
   }

   /**
    * Unregisters this object as a ConfigElementListener from the contained
    * mElement reference.  This is needed to prevent a circular reference
    * from existing forever between this object and its ConfigElement.
    *
    * Is this the best place (or way) to do this?
    */
/*
   public void removeNotify()
   {
      super.removeNotify();
      mInputAreaElement.removeConfigElementListener(this);
   }
*/

   public void propertyValueRemoved(ConfigElementEvent e)
   {
   }

   public void propertyValueAdded(ConfigElementEvent e)
   {
   }

   public void propertyValueChanged(ConfigElementEvent e)
   {
      if ( e.getProperty().equals(KEYBOARD_MOUSE_PTR_PROPERTY) )
      {
         System.out.println("[KeyboardChooserPanel.propertyValueChanged()] " +
                            e.getProperty());
         Object value = 
            mInputAreaElement.getProperty(KEYBOARD_MOUSE_PTR_PROPERTY, 0);
         if ( value instanceof ConfigElementPointer )
         {
            updateKeyboardDeviceElement((ConfigElementPointer) value);
         }
         else
         {
            System.err.println("WARNING: Unexpected type for value of " +
                               "property " + KEYBOARD_MOUSE_PTR_PROPERTY +
                               " in config element " +
                               mInputAreaElement.getName());
            System.out.println("\tvalue == " + value + "(" + value.getClass() +
                               ")");
         }
//         ConfigElementPointer kbd_dev_ptr =
//            (ConfigElementPointer) mInputAreaElement.getProperty(KEYBOARD_MOUSE_PTR_PROPERTY,
//                                                                 0);
//         updateKeyboardDeviceElement(kbd_dev_ptr);
      }
   }

   public void nameChanged(ConfigElementEvent e)
   {
   }

   private void updateKeyboardDeviceElement(ConfigElementPointer kbdDevPtr)
   {
      // Make sure that the "Edit" button is enabled in case a previous
      // setting for keyboard/mouse device pointer was invalid.
      mEditButton.setEnabled(true);

      String kbd_dev_name = kbdDevPtr.getTarget();

      // The config element pointer has no value at this time.  We set
      // mKeyboardDevElement to null so that we can keep track of this fact
      // for later.
      if ( kbd_dev_name == null )
      {
         mKeyboardDevElement = null;
      }
      // The config element pointer has a value, so we need to try to find
      // the ConfigElement object to which the pointer refers.
      else
      {
         ConfigBroker cfg_broker = new ConfigBrokerProxy();
         List all_elts = cfg_broker.getElements(mContext);
         List kbd_devs =
            ConfigUtilities.getElementsWithName(all_elts, kbd_dev_name);

         // The config element pointer refers to a config element not in our
         // context.  Pop up a dialog box informing the user that the
         // configuration is incomplete.
         if ( kbd_devs.size() == 0 )
         {
/*
            Frame parent =
               (Frame) SwingUtilities.getAncestorOfClass(Frame.class, this);
            JOptionPane.showMessageDialog(parent,
                                          "No config element named '" +
                                             kbd_dev_name +
                                             "' in this configuration",
                                          "Incomplete Configuration Error",
                                          JOptionPane.WARNING_MESSAGE);
            mEditButton.setEnabled(false);
*/
            mKeyboardDevElement = null;
         }
         else
         {
            // Pull the first object out of the list.  For now, we will not
            // worry about the case when we have a context with multiple
            // elements having the same name.
            ConfigElement kbd_dev = (ConfigElement) kbd_devs.get(0);

            // Verify that kbd_dev is the right type of config element.
            if ( kbd_dev.getDefinition().getToken().equals(KEYBOARD_MOUSE_TYPE) )
            {
               mKeyboardDevElement = kbd_dev;
            }
            // If it is not the right type of element, pop up a dialog box
            // saying so.
            else
            {
               Frame parent =
                  (Frame) SwingUtilities.getAncestorOfClass(Frame.class, this);
               JOptionPane.showMessageDialog(parent,
                                             "Conifg elements named '" +
                                                kbd_dev_name + "'\n" +
                                                "is not a keyboard/mouse device element",
                                             "Invalid Configuration Error",
                                             JOptionPane.WARNING_MESSAGE);
               mEditButton.setEnabled(false);
               mKeyboardDevElement = null;
            }
         }
      }
   }

   private void jbInit() throws Exception
   {
      double[][] main_size =
         {{TableLayout.FILL, TableLayout.MINIMUM},
          {TableLayout.PREFERRED}};
      mMainLayout = new TableLayout(main_size);

      mEditButton.setText("...");
      mEditButton.addActionListener(
         new KeyboardChooserPanel_mEditButton_actionAdapter(this)
      );
      this.setLayout(mMainLayout);
   }

   private ConfigContext       mContext            = null;
   private ConfigElement       mInputAreaElement   = null;
   private ConfigElement       mKeyboardDevElement = null;
   private PropertyEditorPanel mKeyboardEditor     = null;

   private JButton mEditButton = new JButton();
   private TableLayout mMainLayout = new TableLayout();

   /**
    * Opens a dialog box showing all the simulated devices referring to the
    * keyboard selected in mKeyboardEditor.
    */
   void mEditButton_actionPerformed(ActionEvent actionEvent)
   {
      Container parent =
         (Container) SwingUtilities.getAncestorOfClass(Container.class, this);
      SimKeyboardEditorDialog dlg =
         new SimKeyboardEditorDialog(parent, mContext, mKeyboardDevElement);
      ConfigElement kbd_dev_elt = dlg.showDialog();

      // If the ConfigElement returned by SimKeyboardEditorDialog.showDialog()
      // is not the same as what we currently have, then the user changed the
      // keyboard/device element using the dialog box.  We need to update our
      // INPUT_AREA_TYPE config element to reflect that change.
      // If null was returned, then it is because mKeyboardDevElement is also
      // null and the user did not create a new config element.
      if ( mKeyboardDevElement != kbd_dev_elt )
      {
         // We are a listener for property changes on this config element, so
         // we can wait for the side effects of this method call to propagate
         // back to us to update the UI.
         mInputAreaElement.setProperty(KEYBOARD_MOUSE_PTR_PROPERTY, 0,
                                       kbd_dev_elt.getName());
      }
   }
}

class KeyboardChooserPanel_mEditButton_actionAdapter
   implements ActionListener
{
   private KeyboardChooserPanel adaptee;
   KeyboardChooserPanel_mEditButton_actionAdapter(KeyboardChooserPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mEditButton_actionPerformed(actionEvent);
   }
}
