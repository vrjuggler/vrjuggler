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

package org.vrjuggler.vrjconfig.controlpanel;

import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import javax.swing.Box;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JOptionPane;
import javax.swing.event.UndoableEditEvent;
import javax.swing.event.UndoableEditListener;

import org.vrjuggler.tweek.TweekCore;
import org.vrjuggler.tweek.beans.FileLoader;
import org.vrjuggler.tweek.beans.UndoHandler;
import org.vrjuggler.tweek.event.*;
import org.vrjuggler.tweek.services.GlobalPreferencesServiceProxy;
import org.vrjuggler.tweek.services.GlobalPreferencesService;
import org.vrjuggler.tweek.text.MessageDocument;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.rtrc.*;

import org.vrjuggler.vrjconfig.ui.ConnectionDialog;
import org.vrjuggler.vrjconfig.ui.ContextToolbar;


/**
 */
public class ControlPanelToolbar
   extends ContextToolbar
{
   public static final String NEW_ACTION      = "New";
   public static final String OPEN_ACTION     = "Open";
   public static final String RTRC_ACTION     = "RTRC";
   public static final String SAVE_ACTION     = "Save";
   public static final String SAVE_AS_ACTION  = "SaveAs";

   public ControlPanelToolbar(ConfigContext ctx, FileLoader fileLoader,
                              UndoHandler undoHandler)
   {
      super(ctx, fileLoader, undoHandler);

      try
      {
         GlobalPreferencesService prefs = new GlobalPreferencesServiceProxy();

         // Using the global user preferences from Tweek, set the start
         // directory for fileChooser.
         File f = new File(prefs.getChooserStartDir());
         fileChooser.setCurrentDirectory(f);
      }
      catch(Exception ex)
      {
         MessageDocument doc = TweekCore.instance().getMessageDocument();
         doc.printWarningnl("[ControlPanelToolbar()] WARNING: Failed to set " +
                            "file chooser start directory: " +
                            ex.getMessage());
      }

      try
      {
         jbInit();
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }

      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         String img_root = "org/vrjuggler/vrjconfig/images";
         rtrcBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                          "/vrjuggler.gif")));
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         rtrcBtn.setText("RTRC");
      }

      toolbar.add(newBtn, null);
      toolbar.add(openBtn, null);
      toolbar.add(rtrcBtn, null);
      toolbar.add(saveBtn, null);
      toolbar.add(saveAsBtn, null);
      toolbar.addSeparator();
      toolbar.add(undoBtn, null);
      toolbar.add(redoBtn, null);
      toolbar.addSeparator();
      toolbar.add(Box.createHorizontalGlue(), null);
      toolbar.add(expandBtn, null);
   }

   public boolean doRTRC()
   {
      try
      {
         ConfigBroker broker = getBroker();
         RTRCDataSourceBroker RTRCBroker = new RTRCDataSourceBrokerProxy();
         ConnectionDialog dialog =
            new ConnectionDialog((Frame) getParentFrame(),
                                 "Remote Run-Time Reconfiguration Connections");
         dialog.setVisible(true);

         if ( dialog.getStatus() == ConnectionDialog.OK_OPTION )
         {
            RTRCDataSource data_src = dialog.getDataSource();

            broker.add(data_src.toString(), data_src);
            this.context.add(data_src.toString());

            fireAction(RTRC_ACTION);

            return true;
         }
      }
      catch (Exception ioe)
      {
         JOptionPane.showMessageDialog(getParentFrame(), ioe.getMessage(),
                                       "Error", JOptionPane.ERROR_MESSAGE);
         ioe.printStackTrace();
      }

      return false;
   }

   /**
    * JBuilder auto-generated GUI code.
    */
   private void jbInit()
      throws Exception
   {
      rtrcBtn.setToolTipText("Acquire the config elements of a running " +
                             "VR Juggler application");
      rtrcBtn.setActionCommand(RTRC_ACTION);
      rtrcBtn.setFocusPainted(false);
      rtrcBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            doRTRC();
         }
      });
   }

   // JBuilder GUI variables
   private JButton rtrcBtn = new JButton();
}
