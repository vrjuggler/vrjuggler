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

package org.vrjuggler.vrjconfig.commoneditors;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.AncestorEvent;
import javax.swing.event.AncestorListener;

import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.jccl.editors.CustomEditor;


public class SimpleEditorDialog
   extends JDialog
{
   public static final int OK_OPTION     = JOptionPane.OK_OPTION;
   public static final int CANCEL_OPTION = JOptionPane.CANCEL_OPTION;
   public static final int CLOSED_OPTION = JOptionPane.CLOSED_OPTION;

   public SimpleEditorDialog(Frame parent, Container container, String title)
   {
      super(parent, title, true);

      try
      {
         jbInit();
         this.getContentPane().add(container, BorderLayout.CENTER);

         EnvironmentService env_svc = new EnvironmentServiceProxy();

         // Use platform-specific conventions for laying out the button panel.
         if ( env_svc.getOS() == EnvironmentService.Windows )
         {
            mButtonLayout.setAlignment(FlowLayout.CENTER);
            mButtonPanel.add(mOkayButton);
            mButtonPanel.add(mCancelButton);
            mButtonPanel.add(mHelpButton);
         }
         else
         {
            mButtonLayout.setAlignment(FlowLayout.RIGHT);
            mButtonPanel.add(mHelpButton);
            mButtonPanel.add(mCancelButton);
            mButtonPanel.add(mOkayButton);
         }

         //mHelpActionListener = mEditor.getHelpActionListener();
         mHelpButton.setEnabled(mHelpActionListener != null);
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }

      this.pack();
      this.setLocationRelativeTo(parent);
   }

   public int showDialog()
   {
      setVisible(true);
      return status;
   }

   public void dispose()
   {
      /*
      if ( mEditor != null )
      {
         mEditor.editorClosing();
         mEditor = null;
      }
      */

      super.dispose();
   }

   private void jbInit() throws Exception
   {
      this.getContentPane().setLayout(mMainLayout);
      mButtonPanel.setLayout(mButtonLayout);
      mOkayButton.setText("OK");
      mOkayButton.addActionListener(
         new SimpleEditorDialog_mOkayButton_actionAdapter(this)
      );
      mCancelButton.setText("Cancel");
      mCancelButton.addActionListener(
         new SimpleEditorDialog_mCancelButton_actionAdapter(this)
      );
      mHelpButton.setText("Help");
      mHelpButton.addActionListener(
         new SimpleEditorDialog_mHelpButton_actionAdapter(this)
      );
      this.getContentPane().add(mButtonPanel, BorderLayout.SOUTH);
   }

   private int status;

   private ActionListener mHelpActionListener = null;

   private BorderLayout mMainLayout = new BorderLayout();
   private JPanel mButtonPanel = new JPanel();
   private FlowLayout mButtonLayout = new FlowLayout();
   private JButton mOkayButton = new JButton();
   private JButton mCancelButton = new JButton();
   private JButton mHelpButton = new JButton();

   void mOkayButton_actionPerformed(ActionEvent actionEvent)
   {
      status = OK_OPTION;
      dispose();
   }

   void mHelpButton_actionPerformed(ActionEvent actionEvent)
   {
      if ( mHelpActionListener != null )
      {
         mHelpActionListener.actionPerformed(actionEvent);
      }
   }

   void mCancelButton_actionPerformed(ActionEvent actionEvent)
   {
      status = CANCEL_OPTION;
      dispose();
   }
}

class SimpleEditorDialog_mCancelButton_actionAdapter
   implements ActionListener
{
   private SimpleEditorDialog adaptee;
   SimpleEditorDialog_mCancelButton_actionAdapter(SimpleEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mCancelButton_actionPerformed(actionEvent);
   }
}

class SimpleEditorDialog_mHelpButton_actionAdapter
   implements ActionListener
{
   private SimpleEditorDialog adaptee;
   SimpleEditorDialog_mHelpButton_actionAdapter(SimpleEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mHelpButton_actionPerformed(actionEvent);
   }
}

class SimpleEditorDialog_mOkayButton_actionAdapter
   implements ActionListener
{
   private SimpleEditorDialog adaptee;
   SimpleEditorDialog_mOkayButton_actionAdapter(SimpleEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mOkayButton_actionPerformed(actionEvent);
   }
}
