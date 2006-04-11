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

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.*;
import javax.swing.*;

import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;


public class KeyPressEditorDialog
   extends JDialog
{
   public KeyPressEditorDialog(java.awt.Window parent, ConfigContext ctx,
                               ConfigElement elt)
   {
      super();
      this.setTitle("Key Press Editor for " + elt.getName());
      this.setModal(true);
      enableEvents(java.awt.AWTEvent.WINDOW_EVENT_MASK);

      this.getContentPane().setLayout(new BorderLayout());

      mEditor.setConfig(ctx, elt);
      this.getContentPane().add(mEditor, BorderLayout.CENTER);
      JButton close_button = new JButton();
      close_button.setText("Close");
      close_button.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               KeyPressEditorDialog.this.setVisible(false);
            }
         }
      );

      JPanel button_panel = new JPanel();
      button_panel.setLayout(new FlowLayout(FlowLayout.RIGHT));
      button_panel.add(close_button);
      this.getContentPane().add(button_panel, BorderLayout.SOUTH);

      this.getRootPane().setDefaultButton(close_button);

      this.setResizable(false);
      this.pack();
      this.setLocationRelativeTo(parent);
   }

   public void dispose()
   {
      if ( mEditor != null )
      {
         mEditor.editorClosing();
         mEditor = null;
      }

      super.dispose();
   }

   protected void processWindowEvent(WindowEvent e)
   {
      if ( e.getID() == WindowEvent.WINDOW_CLOSING )
      {
         dispose();
      }

      super.processWindowEvent(e);
   }

   private KeyPressEditor mEditor = new KeyPressEditor();
}
