/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
package org.vrjuggler.vrjconfig;

import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.vrjconfig.ui.*;

/**
 * Provides a control panel view into a config chunk collection.
 */
public class ControlPanelView
   extends JPanel
{
   public ControlPanelView()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      // Init the watermark in the control panel
      ClassLoader loader = BeanJarClassLoader.instance();
      control.setWatermark(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/watermark_logo.png")));
      showMainPanel();

      control.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            Object value = control.getModel().getElementAt(evt.getID());
            String item = (String)value;
            if (item.equals("D e v i c e s"))
            {
               showDevicesPanel();
//               JDialog dlg = new JDialog(
//                     (Frame)SwingUtilities.getAncestorOfClass(Frame.class, ControlPanelView.this),
//                     "Sim Device Editor",
//                     true);
//               dlg.getContentPane().setLayout(new BorderLayout());
//               SimPosDeviceEditor editor = new SimPosDeviceEditor();
//
//               ConfigManagerService mgr = getConfigManager();
//               ConfigChunk device = mgr.getActiveConfig().get("SimHeadPos");
//               editor.setDevice(device);
//               dlg.getContentPane().add(editor, BorderLayout.CENTER);
//               dlg.pack();
//               dlg.setVisible(true);
            }
            else if (item.equals("C l u s t e r"))
            {
               JDialog dlg = new JDialog(
                     (Frame)SwingUtilities.getAncestorOfClass(Frame.class, ControlPanelView.this),
                     "Sim Device Editor",
                     true);
               dlg.getContentPane().setLayout(new BorderLayout());
               SimAnalogDeviceEditor editor = new SimAnalogDeviceEditor();

               ConfigManagerService mgr = getConfigManager();
               ConfigChunk device = mgr.getActiveConfig().get("Analog Simulator");
               editor.setDevice(device);
               dlg.getContentPane().add(editor, BorderLayout.CENTER);
               dlg.pack();
               dlg.setVisible(true);
            }
//            else if (item.equals("Simulator"))
//            {
//               showSimEditor();
//            }
            else
            {
               JOptionPane.showMessageDialog(ControlPanelView.this, "Selected "+String.valueOf(value));
            }
         }
      });
   }

//   protected void showSimEditor()
//   {
//      JDialog dlg = new JDialog(
//            (Frame)SwingUtilities.getAncestorOfClass(Frame.class, this),
//            "Simulator Editor",
//            true);
//      dlg.getContentPane().setLayout(new BorderLayout());
//      dlg.getContentPane().add(new SimEditor(), BorderLayout.CENTER);
//      dlg.pack();
//      dlg.setVisible(true);
//   }

   /**
    * Helper class for getting the config manager serivce.
    */
   private ConfigManagerService getConfigManager()
   {
      Object bean = BeanRegistry.instance().getBean("ConfigManager").getBean();
      return (ConfigManagerService)bean;
   }

   private void createNewChunkDB()
   {
   }

   private void open()
   {
      int result = fileChooser.showOpenDialog(this);
      if (result == JFileChooser.APPROVE_OPTION)
      {
         try
         {
            File file = fileChooser.getSelectedFile();
            ConfigService config = (ConfigService)BeanRegistry.instance().getBean("Config").getBean();
            ConfigManagerService mgr = (ConfigManagerService)BeanRegistry.instance().getBean("ConfigManager").getBean();

            if (file.getName().endsWith(".config"))
            {
               InputStream in = new BufferedInputStream(new FileInputStream(file));
               ConfigChunkDB chunk_db = config.loadConfigChunks(in, mgr.getAllChunkDescs());
               mgr.add(chunk_db);
               mgr.setActiveConfig(chunk_db);
               this.add(control, BorderLayout.CENTER);
               this.updateUI();
               control.invalidate();
            }
         }
         catch (IOException ioe)
         {
            JOptionPane.showMessageDialog(this,
                  "Failed to open the file: "+ioe.getMessage(),
                  "Error",
                  JOptionPane.ERROR_MESSAGE);
            ioe.printStackTrace();
         }
      }
   }

   private void showMainPanel()
   {
      ClassLoader loader = BeanJarClassLoader.instance();
      DefaultControlPanelModel model = new DefaultControlPanelModel();
      model.add("D i s p l a y",       new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/display64.png")));
      model.add("S i m u l a t o r",   new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/simulator64.png")));
      model.add("C l u s t e r",       new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/cluster64.png")));
      model.add("D e v i c e s",       new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/devices64.png")));
      model.add("A u d i o",           new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/audio64.png")));
      control.setModel(model);
      control.setTitle("Choose a category");
   }

   private void showDevicesPanel()
   {
      ClassLoader loader = BeanJarClassLoader.instance();
      DefaultControlPanelModel model = new DefaultControlPanelModel();
      model.add("A n a l o g",            new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/analog_devices64.png")));
      model.add("D i g i t a l",          new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/digital_devices64.png")));
      model.add("P o s i t i o n a l",    new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/positional_devices64.png")));
      control.setModel(model);
      control.setTitle("Choose a device type");
   }

   /**
    * Autogenerated code for the JBuilder GUI.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(baseLayout);
      control.setModel(model);
      toolbar.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            String cmd = evt.getActionCommand();
            if (cmd.equals("New ChunkDB"))
            {
               createNewChunkDB();
            }
            else if (cmd.equals("Open"))
            {
               open();
            }
         }
      });
      this.add(toolbar,  BorderLayout.NORTH);
//      this.add(control,  BorderLayout.CENTER);
   }

   //--- JBuilder GUI variables ---//
   private BorderLayout baseLayout = new BorderLayout();
   private ConfigToolbar toolbar = new ConfigToolbar();
   private ControlPanel control = new ControlPanel();
   private JFileChooser fileChooser = new JFileChooser();

   /**
    * The data model for the control panel.
    */
   private DefaultControlPanelModel model = new DefaultControlPanelModel();
}
