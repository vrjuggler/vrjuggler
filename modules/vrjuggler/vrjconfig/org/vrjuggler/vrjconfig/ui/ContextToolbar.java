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
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.tweek.services.GlobalPreferencesServiceProxy;
import org.vrjuggler.tweek.services.GlobalPreferencesService;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.vrjconfig.PopupButton;
import org.vrjuggler.vrjconfig.VrjConfigConstants;

import org.vrjuggler.jccl.rtrc.*;

/**
 * A specialized toolbar for configuration contexts that pays attention to the
 * ConfigManager.
 */
public class ContextToolbar
   extends JComponent
   implements VrjConfigConstants
{
   public ContextToolbar(File curDir, ConfigContext ctx)
   {
      this.setConfigContext(ctx);

      try
      {
         jbInit();
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }

      fileChooser.setCurrentDirectory(curDir);

      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         newBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/newchunk.gif")));
         openBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/open.gif")));
         saveBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/save.gif")));
         saveAsBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/saveas.gif")));
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
         undoBtn.setText("Undo");
         redoBtn.setText("Redo");
         expandBtn.setText("Expand");
      }
   }

   public void addToToolbar(Component comp)
   {
      toolbar.add(comp);
   }

   private void setConfigContext(ConfigContext ctx)
   {
      this.context.removeContextListener(contextListener);
      this.context = ctx;

      boolean nonempty_context = true;
      if(getBroker().getResourceNames().size() == 0)
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

   private boolean openInContext(String filename, ConfigContext ctx)
   {
      try
      {
         String file = expandEnvVars(filename);
         FileDataSource data_source = FileDataSource.open(file, getBroker().getRepository());
         getBroker().add(file, data_source);
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
      NewConfigDialog new_dlg = new NewConfigDialog(fileChooser.getCurrentDirectory());
      int option = new_dlg.showDialog(this);
      if (option == NewConfigDialog.APPROVE_OPTION)
      {
         // Create the new config file
         try
         {
            File new_file = new File(new_dlg.getDirectory(), new_dlg.getName());
            String new_filename = new_file.getAbsolutePath();
            FileDataSource data_source = FileDataSource.create(new_filename, getBroker().getRepository());
            getBroker().add(new_filename, data_source);
            this.context.add(new_filename);
         }
         catch (IOException ioe)
         {
            ioe.printStackTrace();
            return false;
         }

         fireAction("New");
         return true;
      }
      return false;
   }

   /**
    * Programmatically does an open action into our context.
    */
   public boolean doOpen()
   {
      boolean result = doOpenWork();
      if (result)
      {
         fireAction("Open");
      }
      return result;
   }

   /**
    * Do the real work of opening a file into our context.
    */
   private boolean doOpenWork()
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
               System.out.println("Opening included resource: "+res_name);

               FileDataSource data_source = FileDataSource.open(res_name, getBroker().getRepository());
               broker.add(res_name, data_source);
               this.context.add(res_name);

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
            if (! getBroker().containsDataSource(filename))
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
   private ConfigBroker getBroker()
   {
      return new ConfigBrokerProxy();
   }

   /**
    * Returns a copy of the given string with all environment variables
    * expanded.
    */
   private String expandEnvVars(String str)
   {
      return mEnvService.expandEnvVars(str);
   }

   /**
    * JBuilder auto-generated GUI code.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(new BorderLayout());
      toolbar.setBorder(BorderFactory.createEtchedBorder());
      toolbar.setFloatable(false);
      newBtn.setToolTipText("Add New File");
      newBtn.setActionCommand("New");
      newBtn.setFocusPainted(false);
      openBtn.setToolTipText("Add Existing File");
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
      this.add(toolbar, BorderLayout.CENTER);
      toolbar.add(newBtn, null);
      toolbar.add(openBtn, null);
      toolbar.add(saveBtn, null);
      toolbar.add(saveAsBtn, null);
      toolbar.addSeparator();
      toolbar.add(undoBtn, null);
      toolbar.add(redoBtn, null);
      toolbar.add(Box.createHorizontalGlue(), null);
      toolbar.add(expandBtn, null);
   }

   // JBuilder GUI variables
   private JToolBar toolbar = new JToolBar();
   private JButton newBtn = new JButton();
   private JButton openBtn = new JButton();
   private JButton saveBtn = new JButton();
   private JButton saveAsBtn = new JButton();
   private JButton undoBtn = new JButton();
   private JButton redoBtn = new JButton();
   private JToggleButton expandBtn = new JToggleButton();
   private JFileChooser fileChooser = new JFileChooser();

   private ConfigContext context = new ConfigContext();
   private EditContextPopup contextEditor;
   private ContextChangeListener contextListener = new ContextChangeListener();

   private EnvironmentService mEnvService = new EnvironmentServiceProxy();

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
         }
         if (getBroker().getResourceNames().size() == 0)
         {
            expandBtn.setEnabled(false);
         }
      }
   }
}
