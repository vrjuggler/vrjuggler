/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
package org.vrjuggler.vrjconfig.ui;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.Iterator;
import java.util.Stack;
import javax.swing.*;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.vrjconfig.PopupButton;
import org.vrjuggler.vrjconfig.VrjConfigConstants;

/**
 * A specialized toolbar that pays attention to the ConfigManager.
 */
public class ConfigToolbar
   extends JComponent
   implements VrjConfigConstants
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
         undoBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/undo.gif")));
         redoBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/redo.gif")));
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
         undoBtn.setText("Undo");
         redoBtn.setText("Redo");
         expandBtn.setText("Expand");
      }
   }

   public void addToToolbar(Component comp)
   {
      toolbar.add(comp);
   }

   public void setConfigContext(ConfigContext ctx)
   {
      this.context.removeContextListener(contextListener);
      this.context = ctx;

      boolean nonempty_context = true;
      if (context.getResources().size() == 0)
      {
         nonempty_context = false;
      }
      saveBtn.setEnabled(nonempty_context);
      expandBtn.setEnabled(nonempty_context);
      context.addContextListener(contextListener);
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

   /**
    * Creates the default configuration context used as the base of all
    * configurations.
    */
   private ConfigContext createDefaultConfigContext()
   {
      ConfigContext ctx = new ConfigContext();

      try
      {
         String default_def_file = expandEnvVars(DEFAULT_DEFINITION_FILE);
         // Add the VR Juggler definitions into the context
         FileDataSource defs_data_source = new FileDataSource(default_def_file,
                                                              FileDataSource.DEFINITIONS);
         getConfigBroker().add(default_def_file, defs_data_source);
         ctx.add(default_def_file);
      }
      catch (IOException ioe)
      {
         ioe.printStackTrace();
      }

      return ctx;
   }

   private boolean openInContext(String filename, ConfigContext ctx)
   {
      try
      {
         String file = expandEnvVars(filename);
         FileDataSource data_source = new FileDataSource(file, FileDataSource.DEFINITIONS);
         getConfigBroker().add(file, data_source);
         ctx.add(file);
      }
      catch (IOException ioe)
      {
         ioe.printStackTrace();
         return false;
      }
      return true;
   }

   /**
    * Programmatically does a new action in a new ConfigContext.
    */
   public boolean doNew()
   {
      NewConfigDialog new_dlg = new NewConfigDialog();
      int option = new_dlg.showDialog(this);
      if (option == NewConfigDialog.APPROVE_OPTION)
      {
         // Open all the included files first
         ConfigContext ctx = new ConfigContext();
         for (Iterator itr = new_dlg.getIncludes().iterator(); itr.hasNext(); )
         {
            if (! openInContext((String)itr.next(), ctx))
            {
               return false;
            }
         }

         // Create the new config file
         try
         {
            File new_file = new File(new_dlg.getDirectory(), new_dlg.getName());
            String new_filename = new_file.getAbsolutePath();
            FileDataSource data_source = new FileDataSource(new_filename,
                                                            FileDataSource.ELEMENTS);
            getConfigBroker().add(new_filename, data_source);
            ctx.add(new_filename);
         }
         catch (IOException ioe)
         {
            ioe.printStackTrace();
            return false;
         }

         // Make sure descs are available
         ChunkFactory.setDescs(getConfigBroker().getDescs(ctx));

         setConfigContext(ctx);
         fireAction("New");
         return true;
      }
      return false;
   }

   /**
    * Programmatically does a new action in the given ConfigContext.
    */
   public boolean doNew(ConfigContext ctx)
   {
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

         // Make sure any new descs are available
         ChunkFactory.setDescs(getConfigBroker().getDescs(ctx));

         // Add in the new data source
         FileDataSource new_data_source = new FileDataSource(filename,
                                                             FileDataSource.ELEMENTS);
         getConfigBroker().add(filename, new_data_source);
         ctx.add(filename);
         setConfigContext(ctx);

         return true;
      }
      catch (IOException ioe)
      {
         ioe.printStackTrace();
      }

      return false;
   }

   /**
    * Programmatically does an open action into a new context.
    */
   public boolean doOpen()
   {
      boolean result = doOpen(createDefaultConfigContext());
      if (result)
      {
         fireAction("Open");
      }
      return result;
   }

   /**
    * Programmatically executes an open action into the given context.
    */
   public boolean doOpen(ConfigContext ctx)
   {
      // Only allow the user to choose files
      fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
      fileChooser.setFileHidingEnabled(false);

      int result = fileChooser.showOpenDialog(this);
      if (result == JFileChooser.APPROVE_OPTION)
      {
         try
         {
            // Make sure the selected file actually exists
            File file = fileChooser.getSelectedFile();
            if (! file.exists())
            {
               JOptionPane.showMessageDialog(this, "You must open an existing file.",
                                             "Error", JOptionPane.ERROR_MESSAGE);
               return false;
            }

            ConfigBroker broker = new ConfigBrokerProxy();

            ChunkFactory.setDescs(broker.getDescs(ctx));

            // We want to automatically follow include directives. Keep track of
            // all the URLs on a stack and read them one at a time in the order
            // that we come across them
            Stack urls = new Stack();
            urls.push(file);
            while (! urls.isEmpty())
            {
               // Expand env vars in the URL
               File res_file = (File)urls.pop();
               String res_name = expandEnvVars(res_file.getAbsolutePath());

               FileDataSource data_source = new FileDataSource(res_name,
                                                               FileDataSource.ELEMENTS);
               broker.add(res_name, data_source);
               ctx.add(res_name);

               // Look through the chunks in the newly loaded file and see if
               // any of them are include directives
               java.util.List includes = data_source.getIncludes();
               for (Iterator itr = includes.iterator(); itr.hasNext(); )
               {
                  // Make sure the file reference it created relative to the
                  // current file
                  urls.push(new File(res_file.getParentFile().getAbsolutePath(),
                                     (String)itr.next()));
               }
            }

            setConfigContext(ctx);
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
            if (! data_source.isReadOnly())
            {
               data_source.commit();
            }
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
    * Programmatically execute an undo action.
    */
   public void doUndo()
   {
      System.err.println("Undo not yet implemented");
   }

   /**
    * Programmatically execte a redo action.
    */
   public void doRedo()
   {
      System.err.println("Redo not yet implemented");
   }

   /**
    * Programmatically execute a close action.
    */
   public boolean doClose()
   {
      System.out.println("ConfigToolbar.doClose()");
      //XXX: Check if we need to save first
      ConfigBroker broker = new ConfigBrokerProxy();
      for (Iterator itr = context.getResources().iterator(); itr.hasNext(); )
      {
         broker.remove((String)itr.next());
      }
      return true;
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
         contextEditor.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent evt)
            {
               String cmd = evt.getActionCommand();
               if (cmd.equals("New"))
               {
                  doNew(getConfigContext());
               }
               else if (cmd.equals("Open"))
               {
                  doOpen(getConfigContext());
               }
            }
         });
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
         int result = fileChooser.showSaveDialog(this);
         if (result == JFileChooser.APPROVE_OPTION)
         {
            File file = fileChooser.getSelectedFile();
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
      saveBtn.setEnabled(false);
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
      undoBtn.setEnabled(false);
      undoBtn.setToolTipText("Undo");
      undoBtn.setActionCommand("Undo");
      undoBtn.setFocusPainted(false);
      redoBtn.setEnabled(false);
      redoBtn.setToolTipText("Redo");
      redoBtn.setActionCommand("Redo");
      redoBtn.setFocusPainted(false);
      expandBtn.setEnabled(false);
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
      undoBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            doUndo();
         }
      });
      redoBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            doRedo();
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
      toolbar.addSeparator();
      toolbar.add(undoBtn, null);
      toolbar.add(redoBtn, null);
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
   private JButton undoBtn = new JButton();
   private JButton redoBtn = new JButton();
   private JToggleButton expandBtn = new JToggleButton();
   private JFileChooser fileChooser = new JFileChooser();

   private ConfigContext context = new ConfigContext();
   private EditContextPopup contextEditor;
   private ContextChangeListener contextListener = new ContextChangeListener();


   /**
    * Our special context change listener used to toggle the save and expand
    * toolbar buttons.
    */
   private class ContextChangeListener
      implements ContextListener
   {
      public void resourceAdded(ContextEvent evt)
      {
         saveBtn.setEnabled(true);
         expandBtn.setEnabled(true);
      }

      public void resourceRemoved(ContextEvent evt)
      {
         if (context.getResources().size() == 0)
         {
            saveBtn.setEnabled(false);
            expandBtn.setEnabled(false);
         }
      }
   }
}
