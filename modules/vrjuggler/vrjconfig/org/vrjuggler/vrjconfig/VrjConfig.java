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

import org.vrjuggler.vrjconfig.ui.ConfigToolbar;

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
   }

   /**
    * JBuilder GUI initialization.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(baseLayout);
      desktop.setBorder(BorderFactory.createEtchedBorder());
      toolbar.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            String cmd = evt.getActionCommand();
            if (cmd.equals("New ConfigDB"))
            {
               createNewConfigChunkDB();
            }
            else if (cmd.equals("New DescDB"))
            {
               createNewChunkDescDB();
            }
            else if (cmd.equals("Open"))
            {
               open();
            }
            else if (cmd.equals("Save"))
            {
               save();
            }
            else if (cmd.equals("SaveAs"))
            {
               saveAs();
            }
            else if (cmd.equals("SaveAll"))
            {
               saveAll();
            }
         }
      });
      this.add(toolbar,  BorderLayout.NORTH);
      this.add(desktop, BorderLayout.CENTER);
   }

   // JBuilder GUI variables
   private BorderLayout baseLayout = new BorderLayout();
   private ConfigToolbar toolbar = new ConfigToolbar();
   private JDesktopPane desktop = new JDesktopPane();
   private JFileChooser fileChooser = new JFileChooser();

   protected void createNewConfigChunkDB()
   {
      ConfigChunkDB new_db = new ConfigChunkDB();

      ConfigManagerService mgr = (ConfigManagerService)BeanRegistry.instance().getBean("ConfigManager").getBean();
      mgr.add(new_db);

      ConfigChunkDBEditorIFrame frame = new ConfigChunkDBEditorIFrame();
      frame.getEditor().setConfigChunkDB(new_db);
      frame.pack();
      frame.setVisible(true);
      desktop.add(frame);
      try
      {
         frame.setSelected(true);
      }
      catch (java.beans.PropertyVetoException pve) { /*ignore*/ }
   }

   protected void createNewChunkDescDB()
   {
      ChunkDescDB new_db = new ChunkDescDB();

      ConfigManagerService mgr = (ConfigManagerService)BeanRegistry.instance().getBean("ConfigManager").getBean();
      mgr.add(new_db);

      ChunkDescDBEditorIFrame frame = new ChunkDescDBEditorIFrame();
      frame.getEditor().setChunkDescDB(new_db);
      frame.pack();
      frame.setVisible(true);
      desktop.add(frame);
      try
      {
         frame.setSelected(true);
      }
      catch (java.beans.PropertyVetoException pve) { /*ignore*/ }
   }

   protected void open()
   {
      int result = fileChooser.showOpenDialog(this);
      if (result == JFileChooser.APPROVE_OPTION)
      {
         try
         {
            File file = fileChooser.getSelectedFile();
            ConfigService config = (ConfigService)BeanRegistry.instance().getBean("Config").getBean();
            ConfigManagerService mgr = (ConfigManagerService)BeanRegistry.instance().getBean("ConfigManager").getBean();

            JInternalFrame new_frame = null;
            if (file.getName().endsWith(".desc"))
            {
               InputStream in = new BufferedInputStream(new FileInputStream(file));
               ChunkDescDB desc_db = config.loadChunkDescs(in);
               mgr.add(desc_db);

               ChunkDescDBEditorIFrame frame = new ChunkDescDBEditorIFrame();
               frame.setFilename(file.getAbsolutePath());
               frame.getEditor().setChunkDescDB(desc_db);
               new_frame = frame;
            }
            else if (file.getName().endsWith(".config"))
            {
               InputStream in = new BufferedInputStream(new FileInputStream(file));
               ConfigChunkDB chunk_db = config.loadConfigChunks(in, mgr.getAllChunkDescs());
               mgr.add(chunk_db);

               ConfigChunkDBEditorIFrame frame = new ConfigChunkDBEditorIFrame();
               frame.setFilename(file.getAbsolutePath());
               frame.getEditor().setConfigChunkDB(chunk_db);
               new_frame = frame;
            }
            else
            {
               System.err.println("ERROR: Unknown file");
            }

            // If we were able to create a new frame, add it to the desktop
            if (new_frame != null)
            {
               new_frame.pack();
               new_frame.setVisible(true);
               desktop.add(new_frame);
               try
               {
                  new_frame.setSelected(true);
               }
               catch (java.beans.PropertyVetoException pve) { /*ignore*/ }
            }
         }
         catch (IOException ioe)
         {
            ioe.printStackTrace();
         }
      }
   }

   protected void save()
   {
      JInternalFrame sel_frame = desktop.getSelectedFrame();
      if (sel_frame != null)
      {
         save(sel_frame);
      }
   }

   protected void saveAs()
   {
      JInternalFrame sel_frame = desktop.getSelectedFrame();
      if (sel_frame != null)
      {
         saveAs(sel_frame);
      }
   }

   protected void save(JInternalFrame saveFrame)
   {
      if (saveFrame instanceof ConfigChunkDBEditorIFrame)
      {
         ConfigChunkDBEditorIFrame frame = (ConfigChunkDBEditorIFrame)saveFrame;
         ConfigChunkDB chunk_db = frame.getEditor().getConfigChunkDB();
         if (frame.getFilename().equals(""))
         {
            saveAs(frame);
         }
         else
         {
            saveConfigChunkDB(chunk_db, frame.getFilename());
         }
      }
      else if (saveFrame instanceof ChunkDescDBEditorIFrame)
      {
         ChunkDescDBEditorIFrame frame = (ChunkDescDBEditorIFrame)saveFrame;
         ChunkDescDB desc_db = frame.getEditor().getChunkDescDB();
         // Check if we need to force a save as action
         if (frame.getFilename() == "")
         {
            saveAs(frame);
         }
         else
         {
            saveChunkDescDB(desc_db, frame.getFilename());
         }
      }
   }

   protected void saveAs(JInternalFrame saveFrame)
   {
      if (saveFrame instanceof ConfigChunkDBEditorIFrame)
      {
         ConfigChunkDBEditorIFrame frame = (ConfigChunkDBEditorIFrame)saveFrame;
         ConfigChunkDB chunk_db = frame.getEditor().getConfigChunkDB();
         fileChooser.setSelectedFile(new File(frame.getFilename()));
         int result = fileChooser.showSaveDialog(this);

         if (result == JFileChooser.APPROVE_OPTION)
         {
            String filename = fileChooser.getSelectedFile().getPath();
            if (saveConfigChunkDB(chunk_db, filename))
            {
               frame.setFilename(filename);
            }
         }
      }
      else if (saveFrame instanceof ChunkDescDBEditorIFrame)
      {
         ChunkDescDBEditorIFrame frame = (ChunkDescDBEditorIFrame)saveFrame;
         ChunkDescDB desc_db = frame.getEditor().getChunkDescDB();
         fileChooser.setSelectedFile(new File(frame.getFilename()));
         int result = fileChooser.showSaveDialog(this);

         if (result == JFileChooser.APPROVE_OPTION)
         {
            String filename = fileChooser.getSelectedFile().getPath();
            if (saveChunkDescDB(desc_db, filename))
            {
               frame.setFilename(filename);
            }
         }
      }
   }

   protected void saveAll()
   {
      JInternalFrame[] frames = desktop.getAllFrames();
      for (int i=0; i<frames.length; ++i)
      {
         save(frames[i]);
      }
   }

   private boolean saveChunkDescDB(ChunkDescDB db, String file)
   {
      // Get the config service
      ConfigService config = (ConfigService)BeanRegistry.instance().getBean("Config").getBean();
      try
      {
         OutputStream out = new BufferedOutputStream(new FileOutputStream(file));
         config.saveChunkDescs(out, db);
      }
      catch (IOException ioe)
      {
         ioe.printStackTrace();
         return false;
      }
      return true;
   }

   private boolean saveConfigChunkDB(ConfigChunkDB db, String file)
   {
      // Get the config service
      ConfigService config = (ConfigService)BeanRegistry.instance().getBean("Config").getBean();
      try
      {
         OutputStream out = new BufferedOutputStream(new FileOutputStream(file));
         config.saveConfigChunks(out, db);
      }
      catch (IOException ioe)
      {
         ioe.printStackTrace();
         return false;
      }
      return true;
   }

   /**
    * Customized renderer for the New combo box in the toolbar.
    */
   class IconCellRenderer
      extends JLabel
      implements ListCellRenderer
   {
      public IconCellRenderer()
      {
         setOpaque(true);
      }

      public Component getListCellRendererComponent(JList list,
                                                    Object value,
                                                    int idx,
                                                    boolean selected,
                                                    boolean focused)
      {
         if (selected)
         {
            setBackground(list.getSelectionBackground());
            setForeground(list.getSelectionForeground());
         }
         else
         {
            setBackground(list.getBackground());
            setForeground(list.getForeground());
         }

         ImageIcon icon = (ImageIcon)value;
         setIcon(icon);
         setText(icon.getDescription());

         return this;
      }
   }
}
