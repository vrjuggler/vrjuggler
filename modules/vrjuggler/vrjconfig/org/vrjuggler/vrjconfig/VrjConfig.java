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

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import javax.swing.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;

import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.FileLoader;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;

public class VrjConfig
   extends JPanel
{
   public VrjConfig()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = BeanJarClassLoader.instance();
         newBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/new.gif")));
         openBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/open.gif")));
         saveBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/save.gif")));
         saveAsBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/saveas.gif")));
         saveAllBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/saveall.gif")));
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         newBtn.setText("New");
         openBtn.setText("Open");
         saveBtn.setText("Save");
         saveAsBtn.setText("Save As");
         saveAllBtn.setText("Save All");
      }
   }

   /**
    * JBuilder GUI initialization.
    */
   private void jbInit() throws Exception
   {
      titleLbl.setBackground(UIManager.getColor("textHighlight"));
      titleLbl.setFont(new java.awt.Font("Serif", 1, 18));
      titleLbl.setForeground(Color.black);
      titleLbl.setBorder(BorderFactory.createRaisedBevelBorder());
      titleLbl.setOpaque(true);
      titleLbl.setHorizontalAlignment(SwingConstants.RIGHT);
      titleLbl.setText("VRJConfig");
      this.setLayout(baseLayout);
      mainPnl.setLayout(mainLayout);
      toolbar.setBorder(BorderFactory.createEtchedBorder());
      toolbar.setFloatable(false);
      openBtn.setToolTipText("Open Configuration");
      openBtn.setActionCommand("Open");
      openBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            openBtn_actionPerformed(e);
         }
      });
      newBtn.setToolTipText("New Configuration");
      newBtn.setActionCommand("New");
      saveBtn.setToolTipText("Save Configuration");
      saveBtn.setActionCommand("Save");
      saveAsBtn.setToolTipText("Save Configuration As");
      saveAsBtn.setActionCommand("SaveAs");
      desktop.setBorder(BorderFactory.createEtchedBorder());
      saveAllBtn.setActionCommand("SaveAll");
      this.add(titleLbl,  BorderLayout.NORTH);
      this.add(mainPnl, BorderLayout.CENTER);
      mainPnl.add(toolbar, BorderLayout.NORTH);
      toolbar.add(newBtn, null);
      toolbar.add(openBtn, null);
      toolbar.add(saveBtn, null);
      toolbar.add(saveAsBtn, null);
      toolbar.add(saveAllBtn, null);
      mainPnl.add(desktop, BorderLayout.CENTER);
   }

   // JBuilder GUI variables
   private BorderLayout baseLayout = new BorderLayout();
   private JLabel titleLbl = new JLabel();
   private JPanel mainPnl = new JPanel();
   private BorderLayout mainLayout = new BorderLayout();
   private JToolBar toolbar = new JToolBar();
   private JButton newBtn = new JButton();
   private JButton openBtn = new JButton();
   private JButton saveBtn = new JButton();
   private JButton saveAsBtn = new JButton();
   private JButton saveAllBtn = new JButton();
   private JDesktopPane desktop = new JDesktopPane();
   private JFileChooser fileChooser = new JFileChooser();

   void openBtn_actionPerformed(ActionEvent e)
   {
      int result = fileChooser.showOpenDialog(JOptionPane.getFrameForComponent(this));
      if (result == JFileChooser.APPROVE_OPTION)
      {
         try
         {
            File file = fileChooser.getSelectedFile();
            ConfigService config = (ConfigService)BeanRegistry.instance().getBean("Config").getBean();
            ConfigManagerService mgr = (ConfigManagerService)BeanRegistry.instance().getBean("ConfigManager").getBean();
            if (file.getName().endsWith(".desc"))
            {
               InputStream in = new BufferedInputStream(new FileInputStream(file));
               ChunkDescDB desc_db = config.loadChunkDescs(in);
               mgr.add(desc_db);

               ChunkDescDBEditorIFrame frame = new ChunkDescDBEditorIFrame();
               frame.getEditor().setChunkDescDB(desc_db);
               frame.pack();
               frame.setVisible(true);
               desktop.add(frame);
            }
            else if (file.getName().endsWith(".config"))
            {
               InputStream in = new BufferedInputStream(new FileInputStream(file));
               ConfigChunkDB chunk_db = config.loadConfigChunks(in, mgr.getAllChunkDescs());
               ConfigChunkDBEditorIFrame frame = new ConfigChunkDBEditorIFrame();
               frame.getEditor().setConfigChunkDB(chunk_db);
               frame.pack();
               frame.setVisible(true);
               desktop.add(frame);
            }
            else
            {
               System.err.println("Unknown file");
            }
         }
         catch (IOException ioe)
         {
            ioe.printStackTrace();
         }
      }
   }
}