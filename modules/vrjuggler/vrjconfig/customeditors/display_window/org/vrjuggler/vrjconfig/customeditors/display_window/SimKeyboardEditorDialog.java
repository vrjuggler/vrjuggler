/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
         this.setTitle("Keyboard Editor: " + kbdDevElt.getName());
      }
      else
      {
         this.setTitle("Keyboard Editor");
      }

      this.getRootPane().setDefaultButton(mCloseButton);

      this.pack();

      Dimension dlg_size    = this.getSize();
      Dimension parent_size = parent.getSize();
      this.setSize(Math.min(dlg_size.width, parent_size.width),
                   Math.min(dlg_size.height, parent_size.height));

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
      ConfigElement kbd_dev_elt = mKbdEditor.getKeyboardDeviceElement();
      mKbdEditor.editorClosing();
      mKbdEditor = null;
      return kbd_dev_elt;
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
      mTopButtonPanel.add(mResourceChooser);
      mButtonPanel.setLayout(mButtonPanelLayout);
      mCloseButton.setText("Close");
      mCloseButton.addActionListener(
         new SimKeyboardEditorDialog_mCloseButton_actionAdapter(this)
      );
      this.getContentPane().add(mTopButtonPanel, BorderLayout.NORTH);
      this.getContentPane().add(mButtonPanel, BorderLayout.SOUTH);
   }

   private SimKeyboardEditorPanel mKbdEditor = null;

   private BorderLayout mMainLayout = new BorderLayout();
   private JPanel mTopButtonPanel = new JPanel();
   private JComboBox mResourceChooser = new JComboBox();
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
