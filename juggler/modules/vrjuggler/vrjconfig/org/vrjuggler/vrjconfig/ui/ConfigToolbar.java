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
package org.vrjuggler.vrjconfig.ui;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.Iterator;
import javax.swing.*;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.vrjconfig.PopupButton;

/**
 * A specialized toolbar that pays attention to the ConfigManager.
 */
public class ConfigToolbar
   extends JComponent
{
   public ConfigToolbar()
   {
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
         ClassLoader loader = BeanJarClassLoader.instance();
         newBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/newchunk.gif")));
         openBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/open.gif")));
         saveBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/save.gif")));
         saveAsBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/saveas.gif")));
         saveAllBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/saveall.gif")));
         expandBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/expand_toolbar.gif")));
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         newBtn.setText("New");
         openBtn.setText("Open");
         saveBtn.setText("Save");
         saveAsBtn.setText("Save As");
         saveAllBtn.setText("Save All");
         expandBtn.setText("Expand");
      }
   }

   public void addToToolbar(Component comp)
   {
      toolbar.add(comp);
   }

   public void setConfigContext(ConfigContext context)
   {
      this.context = context;
   }

   public ConfigContext getConfigContext()
   {
      return context;
   }

   public void addActionListener(ActionListener listener)
   {
      listenerList.add(ActionListener.class, listener);
   }

   public void removeActionListener(ActionListener listener)
   {
      listenerList.remove(ActionListener.class, listener);
   }

   protected void fireAction(String command)
   {
      ActionEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ActionListener.class)
         {
            if (evt == null)
            {
               evt = new ActionEvent(this, 0, command);
            }
            ((ActionListener)listeners[i+1]).actionPerformed(evt);
         }
      }
   }

   private ConfigContext createDefaultConfigContext()
   {
      ConfigContext ctx = new ConfigContext();

      // Add the VR Juggler definitions into the context
      String default_desc_file = "${VJ_BASE_DIR}/share/vrjuggler/data/vrj-chunks.desc";
      default_desc_file = expandEnvVars(default_desc_file);
      if (getConfigBroker().containsDataSource(default_desc_file))
      {
         ctx.add(default_desc_file);
      }

      return ctx;
   }

   public boolean doNew()
   {
      // Create a new context
      ConfigContext ctx = createDefaultConfigContext();

      // Create a new data source and add it to the broker
      try
      {
         // Get the new filename from the user
         String filename = askUserForNewConfigFile();
         if (filename == null)
         {
            // User cancelled
            return false;
         }

         ChunkFactory.setDescs(getConfigBroker().getDescs(ctx));
         FileDataSource new_data_source = new FileDataSource(filename,
                                                             FileDataSource.ELEMENTS);
         getConfigBroker().add(filename, new_data_source);
         ctx.add(filename);
         setConfigContext(ctx);

         fireAction("New");
         return true;
      }
      catch (IOException ioe)
      {
         ioe.printStackTrace();
      }

      return false;
   }

   /**
    * Programmatically does an open action.
    */
   public boolean doOpen()
   {
      // Create a new context
      ConfigContext ctx = createDefaultConfigContext();

      int result = fileChooser.showOpenDialog(this);
      if (result == JFileChooser.APPROVE_OPTION)
      {
         try
         {
            File file = fileChooser.getSelectedFile();
            ConfigBroker broker = new ConfigBrokerProxy();
            String res_name = file.getAbsolutePath();

            ChunkFactory.setDescs(broker.getDescs(ctx));
            FileDataSource data_source = new FileDataSource(res_name,
                                                            FileDataSource.ELEMENTS);
            broker.add(res_name, data_source);
            ctx.add(res_name);
            setConfigContext(ctx);

            fireAction("Open");
            return true;
         }
         catch (IOException ioe)
         {
            JOptionPane.showMessageDialog(this, ioe.getMessage(), "Error",
                                          JOptionPane.ERROR_MESSAGE);
            ioe.printStackTrace();
         }
      }

      return false;
   }

   /**
    * Programmatically execute a save action.
    */
   public boolean doSave()
   {
      boolean success = false;
      try
      {
         ConfigBroker broker = new ConfigBrokerProxy();
         for (Iterator itr = context.getResources().iterator(); itr.hasNext(); )
         {
            DataSource data_source = broker.get((String)itr.next());
            data_source.commit();
         }
         success = true;
      }
      catch (IOException ioe)
      {
         JOptionPane.showMessageDialog(this, ioe.getMessage(), "Error",
                                       JOptionPane.ERROR_MESSAGE);
         ioe.printStackTrace();
      }

      return success;
   }

   /**
    * Programmatically execute a save as action.
    */
   public boolean doSaveAs()
   {
      System.err.println("ConfigToolbar.doSaveAs(): not implemented");
      return false;
   }

   /**
    * Programmatically execute a close action.
    */
   public boolean doClose()
   {
      System.err.println("ConfigToolbar.doSaveAs(): not implemented");
      return false;
   }

   protected void toggleContextEditor()
   {
      boolean show_editor = expandBtn.isSelected();
      EditContextPopup contextEditor = getContextEditor();
      if (! show_editor)
      {
         contextEditor.setVisible(false);
         contextEditor.setPreferredSize(null);
      }
      else
      {
         contextEditor.setConfigContext(getConfigContext());
         Dimension pref_size = contextEditor.getPreferredSize();
         pref_size.width = this.getWidth();
         contextEditor.setPreferredSize(pref_size);
         contextEditor.setBackground(Color.pink);
         contextEditor.show(this, 0, this.getHeight());
      }
   }

   private EditContextPopup getContextEditor()
   {
      if (contextEditor == null)
      {
         contextEditor = new EditContextPopup();
      }
      return contextEditor;
   }

   /**
    * Asks the user for a new config filename. If the user does not pick a
    * unique name, they are scolded and told to try again.
    */
   private String askUserForNewConfigFile()
   {
      boolean cancelled = false;
      String name = null;
      do
      {
         JFileChooser chooser = new JFileChooser();
         int result = chooser.showSaveDialog(this);
         if (result == JFileChooser.APPROVE_OPTION)
         {
            File file = chooser.getSelectedFile();
            String filename = file.getAbsolutePath();
            if (! getConfigBroker().containsDataSource(filename))
            {
               name = filename;
            }
            else
            {
               JOptionPane.showMessageDialog(this,
                                             "That resource is already open",
                                             "Oops!",
                                             JOptionPane.ERROR_MESSAGE);
            }
         }
         else
         {
            // The user cancelled the new
            cancelled = true;
         }
      }
      while (name == null && !cancelled);

      return name;
   }

   /**
    * Gets a handle to the configuration broker service.
    */
   private ConfigBroker getConfigBroker()
   {
      return new ConfigBrokerProxy();
   }

   /**
    * Returns a copy of the given string with all environment variables
    * expanded.
    */
   private String expandEnvVars(String str)
   {
      return EnvironmentService.expandEnvVars(str);
   }

   /**
    * JBuilder auto-generated GUI code.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(new BorderLayout());
      titleLbl.setBackground(UIManager.getColor("textHighlight"));
      titleLbl.setFont(new java.awt.Font("Serif", 1, 18));
      titleLbl.setForeground(Color.black);
      titleLbl.setBorder(BorderFactory.createRaisedBevelBorder());
      titleLbl.setOpaque(true);
      titleLbl.setHorizontalAlignment(SwingConstants.RIGHT);
      titleLbl.setText("VRJConfig");
      toolbar.setBorder(BorderFactory.createEtchedBorder());
      toolbar.setFloatable(false);
      newBtn.setToolTipText("New Configuration");
      newBtn.setActionCommand("New");
      newBtn.setFocusPainted(false);
      openBtn.setToolTipText("Open Configuration");
      openBtn.setActionCommand("Open");
      openBtn.setFocusPainted(false);
      saveBtn.setToolTipText("Save Configuration");
      saveBtn.setActionCommand("Save");
      saveBtn.setFocusPainted(false);
      saveAsBtn.setEnabled(false);
      saveAsBtn.setToolTipText("Save Configuration As");
      saveAsBtn.setActionCommand("SaveAs");
      saveAsBtn.setFocusPainted(false);
      saveAllBtn.setEnabled(false);
      saveAllBtn.setToolTipText("Save All Open Configurations");
      saveAllBtn.setActionCommand("SaveAll");
      saveAllBtn.setFocusPainted(false);
      expandBtn.setToolTipText("Expand Toolbar");
      expandBtn.setActionCommand("Expand");
      expandBtn.setFocusPainted(false);
      newBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            doNew();
         }
      });
      openBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            doOpen();
         }
      });
      saveBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            doSave();
         }
      });
      saveAsBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            doSaveAs();
         }
      });
      saveAllBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
//            saveAll();
         }
      });
      expandBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            toggleContextEditor();
         }
      });
      this.add(titleLbl, BorderLayout.NORTH);
      this.add(toolbar, BorderLayout.CENTER);
      toolbar.add(newBtn, null);
      toolbar.add(openBtn, null);
      toolbar.add(saveBtn, null);
      toolbar.add(saveAsBtn, null);
      toolbar.add(saveAllBtn, null);
      toolbar.add(Box.createHorizontalGlue(), null);
      toolbar.add(expandBtn, null);
   }

   // JBuilder GUI variables
   private JLabel titleLbl = new JLabel();
   private JToolBar toolbar = new JToolBar();
   private JButton newBtn = new JButton();
   private JButton openBtn = new JButton();
   private JButton saveBtn = new JButton();
   private JButton saveAsBtn = new JButton();
   private JButton saveAllBtn = new JButton();
   private JToggleButton expandBtn = new JToggleButton();
   private JFileChooser fileChooser = new JFileChooser();

   private ConfigContext context = new ConfigContext();
   private EditContextPopup contextEditor;
}
