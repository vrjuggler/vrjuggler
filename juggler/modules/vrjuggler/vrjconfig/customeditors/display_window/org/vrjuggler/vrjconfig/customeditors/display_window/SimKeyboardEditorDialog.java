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

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import javax.swing.*;

import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;

import org.vrjuggler.jccl.config.*;


public class SimKeyboardEditorDialog
   extends JDialog
   implements EditorConstants
{
   public SimKeyboardEditorDialog(Container parent, ConfigContext ctx,
                                  ConfigElement kbdDevElt)
   {
      super();
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);

      this.setModal(true);

      mContext = ctx;

      try
      {
         jbInit();
         addResources(ctx);

         mKbdEditor = new SimKeyboardEditorPanel();
         mKbdEditor.setConfig(ctx, kbdDevElt,
                              (String) mResourceChooser.getSelectedItem());
         this.getContentPane().add(mKbdEditor, BorderLayout.CENTER);

         EnvironmentService env_svc = new EnvironmentServiceProxy();

         if ( env_svc.getOS() == EnvironmentService.Windows )
         {
            mButtonPanelLayout.setAlignment(FlowLayout.CENTER);
         }
         else
         {
            mButtonPanelLayout.setAlignment(FlowLayout.RIGHT);
         }

         mButtonPanel.add(mCloseButton);
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }

      if ( null != kbdDevElt )
      {
         mKbdDevElement = kbdDevElt;
         mResourceChooser.setEnabled(false);
         mKbdCreateButton.setEnabled(false);
         this.setTitle("Keyboard Editor: " + kbdDevElt.getName());
      }
      else
      {
         mKbdDevElement = null;
         mResourceChooser.setEnabled(true);
         mKbdCreateButton.setEnabled(true);
         this.setTitle("Keyboard Editor");
      }

      this.getRootPane().setDefaultButton(mCloseButton);

      this.pack();
      this.setLocationRelativeTo(parent);
   }

   /**
    * Displays this dialog window and returns the keyboard/mouse input
    * handler element (type EditorConstants.KEYBOARD_MOUSE_TYPE) that was
    * edited.  This method will only return a null reference if the dialog box
    * was given a null reference to begin with and the user did not create a
    * new keyboard/mouse input handler element.  That case would be the
    * equivalent of the user opening the dialog box and choosing to do
    * nothing.
    */
   public ConfigElement showDialog()
   {
      setVisible(true);
      return mKbdDevElement;
   }

   protected void processWindowEvent(WindowEvent e)
   {
      if ( e.getID() == WindowEvent.WINDOW_CLOSING )
      {
         dispose();
      }

      super.processWindowEvent(e);
   }

   private void addResources(ConfigContext ctx)
   {
      for ( java.util.Iterator i = ctx.getResources().iterator();
            i.hasNext(); )
      {
         mResourceChooser.addItem(i.next());
      }
   }

   private void jbInit() throws Exception
   {
      this.getContentPane().setLayout(mMainLayout);
      mResourceChooser.addActionListener(
         new SimKeyboardEditorDialog_mResourceChooser_actionAdapter(this)
      );
      mKbdCreateButton.setText("Create Keyboard/Mouse Input Handler");
      mKbdCreateButton.addActionListener(
         new SimKeyboardEditorDialog_mKbdCreateButton_actionAdapter(this)
      );
      mTopButtonPanel.add(mResourceChooser);
      mTopButtonPanel.add(mKbdCreateButton);
      mButtonPanel.setLayout(mButtonPanelLayout);
      mCloseButton.setText("Close");
      mCloseButton.addActionListener(
         new SimKeyboardEditorDialog_mCloseButton_actionAdapter(this)
      );
      this.getContentPane().add(mTopButtonPanel, BorderLayout.NORTH);
      this.getContentPane().add(mButtonPanel, BorderLayout.SOUTH);
   }

   private ConfigContext mContext       = null;
   private ConfigElement mKbdDevElement = null;

   private SimKeyboardEditorPanel mKbdEditor = null;

   private BorderLayout mMainLayout = new BorderLayout();
   private JPanel mTopButtonPanel = new JPanel();
   private JComboBox mResourceChooser = new JComboBox();
   private JButton mKbdCreateButton = new JButton();
   private JPanel mButtonPanel = new JPanel();
   private FlowLayout mButtonPanelLayout = new FlowLayout();
   private JButton mCloseButton = new JButton();

   /**
    * Updates the keyboard editor panel to know about a change in the currently
    * selected resource.
    */
   void mResourceChooser_actionPerformed(ActionEvent actionEvent)
   {
      if ( null != mKbdEditor )
      {
         String resource_name = (String) mResourceChooser.getSelectedItem();
         mKbdEditor.setDataSourceName(resource_name);
      }
   }

   void mKbdCreateButton_actionPerformed(ActionEvent actionEvent)
   {
      ConfigBrokerProxy broker = new ConfigBrokerProxy();
      ConfigDefinition kbd_def =
         broker.getRepository().get(KEYBOARD_MOUSE_TYPE);
      ConfigElementFactory factory =
         new ConfigElementFactory(broker.getRepository().getAllLatest());

      // Create the new KEYBOARD_MOUSE_TYPE config element.  We won't bother
      // to ask the user for a name since they probably won't care anyway.
      mKbdDevElement = factory.createUnique(kbd_def, mContext);

      // Create a proxy to the new KEYBOARD_MOUSE_TYPE config element.
      ConfigDefinition kbd_proxy_def =
         broker.getRepository().get(KEYBOARD_MOUSE_PROXY_TYPE);
      ConfigElement kbd_proxy =
         factory.create(mKbdDevElement.getName() + " Proxy", kbd_proxy_def);
      kbd_proxy.setProperty(DEVICE_PROPERTY, 0, kbd_proxy.getName());

      String resource_name = (String) mResourceChooser.getSelectedItem();

      // Add the newly created config elements to the data source that the
      // user selected.
      broker.add(mContext, mKbdDevElement, resource_name);
      broker.add(mContext, kbd_proxy, resource_name);

      // Remove the old SimKeyboardEditorPanel instance as it is holding onto
      // a config element that is no longer associated with this dialog.
      this.getContentPane().remove(mKbdEditor);

      // Create a new SimKeyboardEditorPanel, associate it with the newly
      // created config element, and add it to as our new child.
      mKbdEditor = new SimKeyboardEditorPanel();
      mKbdEditor.setConfig(mContext, mKbdDevElement, resource_name);
      this.getContentPane().add(mKbdEditor, BorderLayout.CENTER);

      mResourceChooser.setEnabled(false);
      mKbdCreateButton.setEnabled(false);
   }

   void mCloseButton_actionPerformed(ActionEvent actionEvent)
   {
      dispose();
   }
}

class SimKeyboardEditorDialog_mResourceChooser_actionAdapter
   implements ActionListener
{
   private SimKeyboardEditorDialog adaptee;
   SimKeyboardEditorDialog_mResourceChooser_actionAdapter(SimKeyboardEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mResourceChooser_actionPerformed(actionEvent);
   }
}

class SimKeyboardEditorDialog_mKbdCreateButton_actionAdapter
   implements ActionListener
{
   private SimKeyboardEditorDialog adaptee;
   SimKeyboardEditorDialog_mKbdCreateButton_actionAdapter(SimKeyboardEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mKbdCreateButton_actionPerformed(actionEvent);
   }
}

class SimKeyboardEditorDialog_mCloseButton_actionAdapter
   implements ActionListener
{
   private SimKeyboardEditorDialog adaptee;
   SimKeyboardEditorDialog_mCloseButton_actionAdapter(SimKeyboardEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mCloseButton_actionPerformed(actionEvent);
   }
}
