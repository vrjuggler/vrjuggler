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
package org.vrjuggler.vrjconfig.ui;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Stack;
import javax.swing.*;

import org.vrjuggler.tweek.TweekCore;
import org.vrjuggler.tweek.beans.FileLoader;
import org.vrjuggler.tweek.event.FileActionGenerator;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.tweek.services.GlobalPreferencesServiceProxy;
import org.vrjuggler.tweek.services.GlobalPreferencesService;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigContextEvent;
import org.vrjuggler.jccl.config.event.ConfigContextListener;
import org.vrjuggler.jccl.rtrc.*;
import org.vrjuggler.vrjconfig.PopupButton;
import org.vrjuggler.vrjconfig.VrjConfigConstants;

/**
 * A specialized toolbar that pays attention to the ConfigManager.
 */
public class ConfigToolbar
   extends JComponent
   implements VrjConfigConstants
{
   public static final String NEW_ACTION      = "New";
   public static final String OPEN_ACTION     = "Open";
   public static final String RTRC_ACTION     = "RTRC";
   public static final String SAVE_ALL_ACTION = "SaveAll";
   public static final String CUT_ACTION      = "cut";
   public static final String COPY_ACTION     = "copy";
   public static final String PASTE_ACTION    = "paste";

   public ConfigToolbar(FileLoader fileLoader)
   {
      mFileLoaderBean = fileLoader;

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
         String img_root = "org/vrjuggler/vrjconfig/images";
         ClassLoader loader = getClass().getClassLoader();
         newBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                         "/newchunk.gif")));
         openBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                          "/open.gif")));
         saveAllBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                             "/saveall.gif")));
         cutBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                         "/Cut16.gif")));
         copyBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                          "/Copy16.gif")));
         pasteBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                           "/Paste16.gif")));

         RTRCBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                          "/vrjuggler.gif")));
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         newBtn.setText("New");
         openBtn.setText("Open");
         saveAllBtn.setText("Save All");
         copyBtn.setText("Copy");
         pasteBtn.setText("Paste");
         RTRCBtn.setText("RTRC");
      }

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
         System.err.println("ConfigToolbar(): WARNING: Failed to set file " +
                            "chooser start directory: " + ex.getMessage());
      }
   }

   public void addNotify()
   {
      super.addNotify();
      TweekCore.instance().registerFileActionGenerator(mFileActionGen);
   }

   public void removeNotify()
   {
      TweekCore.instance().unregisterFileActionGenerator(mFileActionGen);
      super.removeNotify();
   }

   public void addToToolbar(Component comp)
   {
      toolbar.add(comp);
   }

   public void setConfigContext(ConfigContext ctx)
   {
      this.context = ctx;
   }

   public ConfigContext getConfigContext()
   {
      return context;
   }

   public File getCurrentDirectory()
   {
      return fileChooser.getCurrentDirectory();
   }

   public void setRemoteReconfigEnabled(boolean enabled)
   {
      RTRCBtn.setEnabled(enabled);
   }

   public void addActionListener(ActionListener listener)
   {
      listenerList.add(ActionListener.class, listener);
   }

   public void removeActionListener(ActionListener listener)
   {
      listenerList.remove(ActionListener.class, listener);
   }

   public void fireAction(String command)
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
      return new ConfigContext();
   }

   private boolean openInContext(String filename, ConfigContext ctx)
   {
      try
      {
         String file = expandEnvVars(filename);
         FileDataSource data_source =
            FileDataSource.open(file, getBroker().getRepository());
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
      NewConfigDialog new_dlg =
         new NewConfigDialog(fileChooser.getCurrentDirectory());

      int option = new_dlg.showDialog(getParentFrame());
      if (option == NewConfigDialog.APPROVE_OPTION)
      {
         // Open all the included files first
         ConfigContext ctx = new ConfigContext();
         // TODO: Add support for jconf includes.
         /*
         for (Iterator itr = new_dlg.getIncludes().iterator(); itr.hasNext(); )
         {
            if (! openInContext((String)itr.next(), ctx))
            {
               return false;
            }
         }
         */

         // Create the new config file
         try
         {
            File new_file = new File(new_dlg.getDirectory(),
                                     new_dlg.getFileName());
            String new_filename = new_file.getAbsolutePath();
            FileDataSource data_source =
               FileDataSource.create(new_filename,
                                     getBroker().getRepository());
            getBroker().add(new_filename, data_source);
            ctx.add(new_filename);
         }
         catch (IOException ioe)
         {
            ioe.printStackTrace();
            return false;
         }

         setConfigContext(ctx);
         fireAction(NEW_ACTION);
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

         // Add in the new data source
         FileDataSource new_data_source =
            FileDataSource.create(filename, getBroker().getRepository());
         getBroker().add(filename, new_data_source);
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
         fireAction(OPEN_ACTION);
      }
      return result;
   }

   public boolean doRTRC()
   {
      boolean result = doRTRC(createDefaultConfigContext());
      if (result)
      {
         //XXX: "RTRC"
         fireAction("Open");
      }

      return result;
   }

   /**
    * Programmatically executes an open action into the given context.
    */
   public boolean doOpen(ConfigContext ctx)
   {
      int result = fileChooser.showOpenDialog(getParentFrame());
      if (result == JFileChooser.APPROVE_OPTION)
      {
         // Make sure the selected file actually exists
         File file = fileChooser.getSelectedFile();
         if ( ! file.exists() )
         {
            JOptionPane.showMessageDialog(getParentFrame(),
                                          "You must open an existing file.",
                                          "Error",
                                          JOptionPane.ERROR_MESSAGE);
            return false;
         }

         ConfigBroker broker = new ConfigBrokerProxy();

         // We want to automatically follow include directives. Keep track of
         // all the URLs on a stack and read them one at a time in the order
         // that we come across them.
         Stack urls = new Stack();
         urls.push(file);
         while ( ! urls.isEmpty() )
         {
            // Expand env vars in the URL
            File res_file = (File)urls.pop();
            String res_name = expandEnvVars(res_file.getAbsolutePath());
            System.out.println("Opening included resource: "+res_name);

            try
            {
               FileDataSource data_source =
                  FileDataSource.open(res_name, getBroker().getRepository());
               broker.add(res_name, data_source);
               ctx.add(res_name);

               // Look through the elements in the newly loaded file and
               // see if any of them are include directives.
               java.util.List includes = data_source.getIncludes();
               for ( Iterator itr = includes.iterator(); itr.hasNext(); )
               {
                  // Make sure the file reference it created relative to the
                  // current file.
                  urls.push(
                     new File(res_file.getParentFile().getAbsolutePath(),
                              (String) itr.next())
                  );
               }
            }
            catch (IOException ioe)
            {
               showLoadErrorMessage(res_name, ioe);
            }
         }

         setConfigContext(ctx);
         return true;
      }

      return false;
   }

   /**
    * Programmatically executes an open action on configuration files
    * passed on the command line.
    */
   public boolean doOpenCmdArgs(ConfigContext ctx)
   {
      try
      {
         EnvironmentServiceProxy env_service = new EnvironmentServiceProxy();

         String[] args = env_service.getCommandLineArgs();
         if (null == args)
         {
            return false;
         }

         for (int i = 0 ; i < args.length ; ++i)
         {
            // Check if this part of the path is a valid directory we can read
            String file_name = args[i];
            File file = new File(file_name);
            if ( file.exists() && file.isFile() && file.canRead() &&
                 file_name.endsWith(".jconf") )
            {
               System.out.println("Found configuration file in args: " + file);

               ConfigBroker broker = new ConfigBrokerProxy();

               // We want to automatically follow include directives. Keep
               // track of all the URLs on a stack and read them one at a time
               // in the order that we come across them
               Stack urls = new Stack();
               urls.push(file);
               while (! urls.isEmpty())
               {
                  // Expand env vars in the URL
                  File res_file = (File)urls.pop();
                  String res_name = expandEnvVars(res_file.getAbsolutePath());
                  System.out.println("Opening included resource: "+res_name);

                  try
                  {
                     FileDataSource data_source =
                        FileDataSource.open(res_name,
                                            getBroker().getRepository());
                     broker.add(res_name, data_source);
                     ctx.add(res_name);

                     // Look through the elements in the newly loaded file and
                     // see if any of them are include directives
                     java.util.List includes = data_source.getIncludes();
                     for (Iterator itr = includes.iterator(); itr.hasNext(); )
                     {
                        // Make sure the file reference it created relative to
                        // the current file.
                        urls.push(
                           new File(res_file.getParentFile().getAbsolutePath(),
                           (String)itr.next())
                        );
                     }
                  }
                  catch (IOException ioe)
                  {
                     showLoadErrorMessage(res_name, ioe);
                  }
               }
            }
         }
         setConfigContext(ctx);
         fireAction(OPEN_ACTION);
         return true;
      }
      catch(Exception ex)
      {
         System.out.println(ex);
         ex.printStackTrace();
      }

      return false;
   }

   /**
    * Adds a RTRCDataSource into the given context.
    */
   public boolean doRTRC(ConfigContext ctx)
   {
      try
      {
         ConfigBroker broker = new ConfigBrokerProxy();
         RTRCDataSourceBroker RTRCBroker = new RTRCDataSourceBrokerProxy();
         ConnectionDialog dialog =
            new ConnectionDialog((Frame) getParentFrame(),
                                 "Remote Run-Time Reconfiguration Connections");
         dialog.setVisible(true);

         if ( dialog.getStatus() == ConnectionDialog.OK_OPTION )
         {
            RTRCDataSource data_src = dialog.getDataSource();

            broker.add(data_src.toString(), data_src);
            ctx.add(data_src.toString());

            setConfigContext(ctx);
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
    * Programmatically execute a save action.
    */
   public boolean doSaveAll()
   {
      // Send a SaveAll action to all ConfigIFrames.
      fireAction(SAVE_ALL_ACTION);
      return true;
   }

   /**
    * Programmatically execute a close action.
    */
   public boolean doClose()
   {
      return true;
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
         int result = fileChooser.showSaveDialog(getParentFrame());
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
               JOptionPane.showMessageDialog(getParentFrame(),
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
      titleLbl.setBackground(UIManager.getColor("textHighlight"));
      titleLbl.setFont(new java.awt.Font("Serif", 1, 18));
      titleLbl.setForeground(UIManager.getColor("textHighlightText"));
      titleLbl.setBorder(BorderFactory.createRaisedBevelBorder());
      titleLbl.setOpaque(true);
      titleLbl.setHorizontalAlignment(SwingConstants.RIGHT);
      titleLbl.setText("VRJConfig");
      toolbar.setBorder(BorderFactory.createEtchedBorder());
      toolbar.setFloatable(false);
      newBtn.setToolTipText("Create a new configuration file");
      newBtn.setActionCommand(NEW_ACTION);
      newBtn.setFocusPainted(false);
      openBtn.setToolTipText("Open a configuration file");
      openBtn.setActionCommand(OPEN_ACTION);
      openBtn.setFocusPainted(false);
      RTRCBtn.setToolTipText("Acquire the config elements of a running " +
                             "VR Juggler application");
      RTRCBtn.setActionCommand(RTRC_ACTION);
      RTRCBtn.setFocusPainted(false);

      saveAllBtn.setEnabled(true);
      saveAllBtn.setToolTipText("Save all open configurations");
      saveAllBtn.setActionCommand(SAVE_ALL_ACTION);
      saveAllBtn.setFocusPainted(false);

      cutBtn.setEnabled(true);
      cutBtn.setToolTipText("Cut config element to the clipboard");
      cutBtn.setActionCommand(CUT_ACTION);
      cutBtn.setFocusPainted(false);
      copyBtn.setEnabled(true);
      copyBtn.setToolTipText("Copy config element to the clipboard");
      copyBtn.setActionCommand(COPY_ACTION);
      copyBtn.setFocusPainted(false);
      pasteBtn.setEnabled(true);
      pasteBtn.setToolTipText("Paste config element");
      pasteBtn.setActionCommand(PASTE_ACTION);
      pasteBtn.setFocusPainted(false);

      newBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            if ( doNew() )
            {
               // Indicate that an open operation was completed successfully.
               // We only do this here because we can guarantee that a child
               // of this component is the true source of the open event.
               mFileActionGen.fireOpenPerformed(newBtn, mFileLoaderBean);
            }
         }
      });
      openBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            if ( doOpen() )
            {
               // Indicate that an open operation was completed successfully.
               // We only do this here because we can guarantee that a child
               // of this component is the true source of the open event.
               mFileActionGen.fireOpenPerformed(openBtn, mFileLoaderBean);
            }
         }
      });
      saveAllBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            if ( doSaveAll() )
            {
               // Indicate that a save operation was completed successfully.
               // We only do this here because we can guarantee that a child
               // of this component is the true source of the save event.
               mFileActionGen.fireSaveAllPerformed(saveAllBtn,
                                                   mFileLoaderBean);
            }
         }
      });
      cutBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            fireAction(evt.getActionCommand());
         }
      });
      copyBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            fireAction(evt.getActionCommand());
         }
      });
      pasteBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            fireAction(evt.getActionCommand());
         }
      });
      RTRCBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            doRTRC();
         }
      });
      this.add(titleLbl, BorderLayout.NORTH);
      this.add(toolbar, BorderLayout.CENTER);
      toolbar.add(newBtn, null);
      toolbar.add(openBtn, null);
      toolbar.add(RTRCBtn, null);
      toolbar.add(saveAllBtn, null);
      toolbar.addSeparator();
      toolbar.add(cutBtn, null);
      toolbar.add(copyBtn, null);
      toolbar.add(pasteBtn, null);
      toolbar.addSeparator();
      toolbar.add(Box.createHorizontalGlue(), null);

      fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
      fileChooser.setFileHidingEnabled(false);
      fileChooser.setAcceptAllFileFilterUsed(false);
      fileChooser.setFileFilter(new ConfigFileFilter());
      fileChooser.setFileView(new ConfigFileView());
   }

   private Container getParentFrame()
   {
      if ( null == mParentFrame )
      {
         mParentFrame = SwingUtilities.getAncestorOfClass(Frame.class, this);
      }

      return mParentFrame;
   }

   private void showLoadErrorMessage(String fileName, Exception ex)
   {
      JOptionPane.showMessageDialog(getParentFrame(),
         "Caught an I/O exception while trying to load\n" + fileName + "\n" +
            ex.getMessage() + "\n" +
            "The contents of this file will not be available!",
         "Config File Load Failure",
         JOptionPane.WARNING_MESSAGE);
   }

   // JBuilder GUI variables
   private JLabel titleLbl = new JLabel();
   private JToolBar toolbar = new JToolBar();
   private JButton newBtn = new JButton();
   private JButton openBtn = new JButton();
   private JButton saveAllBtn = new JButton();
   private JButton cutBtn = new JButton();
   private JButton copyBtn = new JButton();
   private JButton pasteBtn = new JButton();
   private JButton RTRCBtn = new JButton();
   private JFileChooser fileChooser = new JFileChooser();
   private ConfigContext context = new ConfigContext();
   private EnvironmentService mEnvService = new EnvironmentServiceProxy();
   private Container mParentFrame = null;

   private FileLoader mFileLoaderBean = null;
   private FileActionGenerator mFileActionGen = new FileActionGenerator();
}
