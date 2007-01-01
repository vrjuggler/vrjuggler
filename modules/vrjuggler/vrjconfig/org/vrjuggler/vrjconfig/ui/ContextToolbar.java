/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Stack;
import javax.swing.*;
import javax.swing.event.UndoableEditEvent;
import javax.swing.event.UndoableEditListener;

import org.vrjuggler.tweek.TweekCore;
import org.vrjuggler.tweek.beans.FileLoader;
import org.vrjuggler.tweek.beans.UndoHandler;
import org.vrjuggler.tweek.event.*;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigContextEvent;
import org.vrjuggler.jccl.config.event.ConfigContextListener;

import org.vrjuggler.vrjconfig.PopupButton;
import org.vrjuggler.vrjconfig.VrjConfigConstants;


/**
 * A specialized toolbar for configuration contexts that pays attention to the
 * ConfigManager.
 */
public class ContextToolbar
   extends JComponent
   implements VrjConfigConstants
            , UndoableEditListener
{
   public ContextToolbar(ConfigContext ctx, FileLoader fileLoader,
                         UndoHandler undoHandler)
   {
      this(null, ctx, fileLoader, undoHandler);
   }

   public ContextToolbar(File curDir, ConfigContext ctx, FileLoader fileLoader,
                         UndoHandler undoHandler)
   {
      // Set up the stuff needed for communication with the Tweek Java GUI.
      mFileLoaderBean  = fileLoader;
      mUndoHandlerBean = undoHandler;

      try
      {
         jbInit();
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }

      // This must happen after we initialize the GUI in jbInit
      // to ensure that buttons are enabled if appropriate.
      this.setConfigContext(ctx);

      if ( null != curDir )
      {
         fileChooser.setCurrentDirectory(curDir);
      }

      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         String img_root = "org/vrjuggler/vrjconfig/images";
         newBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                         "/Add16.gif")));
         openBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                          "/OpenAdd16.gif")));
         saveBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                          "/save.gif")));
         saveAsBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                            "/saveas.gif")));
         undoBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                          "/undo.gif")));
         redoBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                          "/redo.gif")));
         expandBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                            "/ContextEdit16.png")));
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

      mTweekFrameListener = new TweekFrameAdapter()
         {
            public boolean frameClosing(TweekFrameEvent e)
            {
               return doClose();
            }
            public void frameFocused(TweekFrameEvent e)
            {
               doCheckReload();
            }
         };

      EventListenerRegistry.instance().registerListener(mTweekFrameListener,
                                                        TweekFrameListener.class);
   }

   public void addNotify()
   {
      super.addNotify();
      TweekCore.instance().registerFileActionGenerator(mFileActionGen);
      TweekCore.instance().registerUndoActionGenerator(mUndoActionGen);
   }

   public void removeNotify()
   {
      TweekCore.instance().unregisterFileActionGenerator(mFileActionGen);
      TweekCore.instance().unregisterUndoActionGenerator(mUndoActionGen);
      super.removeNotify();
   }

   public void undoableEditHappened(UndoableEditEvent e)
   {
      undoBtn.setEnabled(true);
      saveBtn.setEnabled(true);
      redoBtn.setEnabled(false);
      mFileActionGen.fireChangePerformed(e.getSource(), mFileLoaderBean);
      mUndoActionGen.fireUndoActionPerformed(e.getSource(), mUndoHandlerBean);
   }

   public void addToToolbar(Component comp)
   {
      toolbar.add(comp);
   }

   private void setConfigContext(ConfigContext ctx)
   {
      this.context.removeUndoableEditListener(this);
      this.context.removeConfigContextListener(contextListener);
      this.context = ctx;

      boolean nonempty_context = true;
      if(getBroker().getResourceNames().size() == 0)
      {
         nonempty_context = false;
      }
      saveBtn.setEnabled(ctx.getConfigUndoManager().getUnsavedChanges());
      saveAsBtn.setEnabled(nonempty_context);
      expandBtn.setEnabled(nonempty_context);
      undoBtn.setEnabled(ctx.getConfigUndoManager().canUndo());
      redoBtn.setEnabled(ctx.getConfigUndoManager().canRedo());
      context.addConfigContextListener(contextListener);
      context.addUndoableEditListener(this);
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
      fileChooser.setAcceptAllFileFilterUsed(false);
      fileChooser.setFileFilter(new ConfigFileFilter());
      fileChooser.setFileView(new ConfigFileView());
      fileChooser.setMultiSelectionEnabled(true);

      boolean status = true;
      int result = fileChooser.showOpenDialog(getParentFrame());
      if (result == JFileChooser.APPROVE_OPTION)
      {
         ConfigBroker broker = new ConfigBrokerProxy();

         File[] files = fileChooser.getSelectedFiles();
         int open_count = 0;

         for ( int i = 0; i < files.length; ++i )
         {
            File file = files[i];

            // Make sure the selected file actually exists.
            if (! file.exists())
            {
               JOptionPane.showMessageDialog(getParentFrame(),
                                             "You must open an existing file.",
                                             "Error",
                                             JOptionPane.ERROR_MESSAGE);
               continue;
            }

            try
            {
               // We want to follow include directives automatically.  Keep
               // track of all the URLs on a stack and read them one at a time
               // in the order that we come across them.
               Stack urls = new Stack();
               urls.push(file);
               while (! urls.isEmpty())
               {
                  // Expand env vars in the URL
                  File res_file = (File)urls.pop();
                  String res_name = expandEnvVars(res_file.getAbsolutePath());
                  System.out.println("Opening resource: " + res_name);

                  FileDataSource data_source =
                     FileDataSource.open(res_name,
                                         getBroker().getRepository());
                  broker.add(res_name, data_source);
                  this.context.add(res_name);

                  // Look through the elements in the newly loaded file and
                  // see if any of them are include directives
                  java.util.List includes = data_source.getIncludes();
                  for (Iterator itr = includes.iterator(); itr.hasNext(); )
                  {
                     // Make sure the file reference it created relative to the
                     // current file
                     urls.push(new File(res_file.getParentFile().getAbsolutePath(),
                                        (String)itr.next()));
                  }

                  open_count++;
               }
            }
            catch (IOException ioe)
            {
               JOptionPane.showMessageDialog(getParentFrame(), ioe.getMessage(),
                                             "Error",
                                             JOptionPane.ERROR_MESSAGE);
               ioe.printStackTrace();
            }
         }

         status = (open_count > 0);
      }
      else
      {
         status = false;
      }

      return status;
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

         // Inform the ConfigUndoManager that we have saved changes.
         context.getConfigUndoManager().saveHappened();

         // Disable the save button now that the save operation is done.
         saveBtn.setEnabled(false);
      }
      catch (IOException ioe)
      {
         JOptionPane.showMessageDialog(getParentFrame(), ioe.getMessage(),
                                       "Error", JOptionPane.ERROR_MESSAGE);
         ioe.printStackTrace();
      }

      return success;
   }

   /**
    * Programmatically execute an undo action.
    */
   public void doUndo()
   {
      if (context.getConfigUndoManager().canUndo())
      {
         context.getConfigUndoManager().undo();
         undoBtn.setEnabled(context.getConfigUndoManager().canUndo());
         redoBtn.setEnabled(true);
         if (context.getConfigUndoManager().getUnsavedChanges())
         {
            saveBtn.setEnabled(true);
         }
         else
         {
            saveBtn.setEnabled(false);
         }
      }
   }

   /**
    * Programmatically execte a redo action.
    */
   public void doRedo()
   {
      if (context.getConfigUndoManager().canRedo())
      {
         context.getConfigUndoManager().redo();
         undoBtn.setEnabled(true);
         redoBtn.setEnabled(context.getConfigUndoManager().canRedo());
      }
   }

   public boolean doCheckReload()
   {
      ConfigBroker broker = new ConfigBrokerProxy();
      java.util.List resources = getConfigContext().getResources();

      for (Iterator itr = resources.iterator() ; itr.hasNext() ; )
      {
         String ds_name = (String)itr.next();
         DataSource ds = broker.get(ds_name);
         if ( ds instanceof FileDataSource )
         {
            FileDataSource fds = (FileDataSource)ds;

            if ( fds.isOutOfDate() )
            {
               int result = JOptionPane.showConfirmDialog(
                  SwingUtilities.getAncestorOfClass(Frame.class, this),
                  "DataSource \"" + ds_name +
                  "\" has been changed by an external program. Do you want " +
                  "to reload the file?",
                  "VRJConfig", JOptionPane.YES_NO_OPTION);

               if(JOptionPane.YES_OPTION == result)
               {
                  try
                  {
                     fds.rollback();
                  }
                  catch (IOException ex)
                  {
                     System.out.println(ex);
                     ex.printStackTrace();
                  }
               }
               else
               {
                  fds.resetLastModified();
               }
            }
         }
      }

      System.out.println("Reload?");
      return false;
   }

   /**
    * Programmatically execute a close action.
    */
   public boolean doClose()
   {
      if (context.getConfigUndoManager().getUnsavedChanges())
      {
         String file_names = new String();
         for (Iterator itr = context.getResources().iterator(); itr.hasNext(); )
         {
            file_names = file_names + "\n" + itr.next();
         }

         int result =
            JOptionPane.showConfirmDialog(
               null,
               "You have unsaved changes, do you want to save them?\n" +
                  file_names,
               "Unsaved Changes", JOptionPane.YES_NO_CANCEL_OPTION,
               JOptionPane.INFORMATION_MESSAGE
            );

         switch (result)
         {
            case JOptionPane.YES_OPTION:
               doSave();
               break;
            case JOptionPane.NO_OPTION:
               break;
            case JOptionPane.CANCEL_OPTION:
               System.out.println("Cancel...");
               return false;
         }
      }

      ConfigBroker broker = new ConfigBrokerProxy();
      for (Iterator itr = context.getResources().iterator(); itr.hasNext(); )
      {
         try
         {
            broker.remove((String) itr.next());
         }
         // If the current file is no open in the Config Broker, an illegal
         // argument exception will be thrown. We just ignore that occurrence.
         catch (IllegalArgumentException ex)
         {
            /* Do nothing. */ ;
         }
      }

      EventListenerRegistry.instance().unregisterListener(mTweekFrameListener,
                                                          TweekFrameListener.class);

      // Indicate that a close operation was performed successfully.
      mFileActionGen.fireClosePerformed(mFileLoaderBean);

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
   protected ConfigBroker getBroker()
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
      newBtn.setToolTipText("Add a new file to this configuration context");
      newBtn.setActionCommand("New");
      newBtn.setFocusPainted(false);
      openBtn.setToolTipText("Add an existing file to this configuration " +
                             "context");
      openBtn.setActionCommand("Open");
      openBtn.setFocusPainted(false);
      saveBtn.setEnabled(false);
      saveBtn.setToolTipText("Save this configuration");
      saveBtn.setActionCommand("Save");
      saveBtn.setFocusPainted(false);
      saveAsBtn.setEnabled(false);
      saveAsBtn.setToolTipText("Save this whole configuration as a single " +
                               "new file");
      saveAsBtn.setActionCommand("SaveAs");
      saveAsBtn.setFocusPainted(false);
      undoBtn.setEnabled(true);
      undoBtn.setToolTipText("Undo the last edit");
      undoBtn.setActionCommand("Undo");
      undoBtn.setFocusPainted(false);
      redoBtn.setEnabled(true);
      redoBtn.setToolTipText("Redo the last edit");
      redoBtn.setActionCommand("Redo");
      redoBtn.setFocusPainted(false);
      expandBtn.setEnabled(false);
      expandBtn.setToolTipText("Show the files in this configuration context");
      expandBtn.setActionCommand("Expand");
      expandBtn.setFocusPainted(false);

      newBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            // We do not fire a file open action for this event because it
            // cannot be invoked except through the use of newBtn, so no one
            // else needs to know about this activity.
            doNew();
         }
      });

      openBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            // We do not fire a file open action for this event because it
            // cannot be invoked except through the use of openBtn, so no one
            // else needs to know about this activity.
            doOpen();
         }
      });

      saveBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            if ( doSave() )
            {
               // Indicate that a save operation was completed successfully.
               // We only do this here because we can guarantee that a child
               // of this component is the true source of the save event.
               mFileActionGen.fireSavePerformed(saveBtn, mFileLoaderBean);
            }
         }
      });

      saveAsBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            if ( doSaveAs() )
            {
               // Indicate that a save-as operation was completed successfully.
               // We only do this here because we can guarantee that a child
               // of this component is the true source of the save event.
               mFileActionGen.fireSaveAsPerformed(saveAsBtn, mFileLoaderBean);
            }
         }
      });

      undoBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            doUndo();
            mFileActionGen.fireChangePerformed(undoBtn, mFileLoaderBean);
            mUndoActionGen.fireUndoActionPerformed(undoBtn, mUndoHandlerBean);
         }
      });

      redoBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            doRedo();
            mFileActionGen.fireChangePerformed(redoBtn, mFileLoaderBean);
            mUndoActionGen.fireUndoActionPerformed(redoBtn, mUndoHandlerBean);
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
   }

   /**
    * Pragmatically executs a save as operation.  This is implemented as a
    * "consolidation" sort of operation.  All resources in our context are
    * saved into a single file resource.  The old resources are removed and
    * replaced with the new single file resource.
    */
   public boolean doSaveAs()
   {
      boolean status = true;

      String new_resource_name = pickFileToSave();

      // If new_resource_name is not null, it is the absolute path to a
      // file that we can write to.  If the file already exists, it will
      // be safe for us to replace it.
      if ( null != new_resource_name )
      {
         // removed_sources contains the names of all the resources that
         // will be removed and replaced by the single new resource, named
         // in new_resource_name.
         ArrayList removed_sources = new ArrayList();
         ArrayList all_elts = new ArrayList();

         ConfigBroker broker = getBroker();

         // First, we need to collect all the DataSource objects that are
         // in our context so that they can be removed.  While doing that,
         // we need to pull together all the config elements in all the
         // data sources.
         for ( Iterator itr = context.getResources().iterator();
               itr.hasNext(); )
         {
            String old_name = (String)itr.next();
            removed_sources.add(old_name);
            DataSource cur_source = broker.get(old_name);

            for ( Iterator elt_itr = cur_source.getElements().iterator();
                  elt_itr.hasNext(); )
            {
               all_elts.add(elt_itr.next());
            }
         }

         // If the file to which we are saving everything already exists,
         // we have to get the old file out of the way first.  This is
         // because org.vrjuggler.jccl.config.FileDataSource.create() will
         // throw an exception if the file to create already exists.
         File temp_file = new File(new_resource_name);
         boolean have_backup = false;
         if ( temp_file.exists() )
         {
            temp_file.renameTo(new File(new_resource_name + ".bak"));
            have_backup = true;
         }

         try
         {
            FileDataSource new_resource =
               FileDataSource.create(new_resource_name,
                                     broker.getRepository());

            // Now, we can add all the config elements from the old
            // (to-be-removed) resources into the newly created data source.
            for ( Iterator i = all_elts.iterator(); i.hasNext(); )
            {
               new_resource.add((ConfigElement) i.next());
            }

            // Write out the new file.
            new_resource.commit();

            // Now, remove all the old data sources from the broker and from
            // our context.  This is delayed as long as possible in case
            // anything goes wrong with the construction of the replacement
            // file data source.  With any luck, no more exceptions will be
            // thrown after this point.
            for ( Iterator i = removed_sources.iterator(); i.hasNext(); )
            {
               String cur = (String) i.next();
               context.remove(cur);
               broker.remove(cur);
            }

            // Add the new resource into the context as a replacement for the
            // old resources that were just removed.
            broker.add(new_resource_name, new_resource);
            context.add(new_resource_name);

            // Inform the ConfigUndoManager that we have saved changes.
            context.getConfigUndoManager().saveHappened();

            status = true;
         }
         catch(Exception ex)
         {
            String msg = "Save failed: " + ex.getMessage();

            // Restore the backup copy of the file if we have one.
            if ( have_backup )
            {
               int choice =
                  JOptionPane.showConfirmDialog(getParent(),
                                                msg + "\nRestore backup?",
                                                "Save Failed",
                                                JOptionPane.YES_NO_OPTION,
                                                JOptionPane.ERROR_MESSAGE);

               if ( choice == JOptionPane.YES_OPTION )
               {
                  temp_file.renameTo(new File(new_resource_name));
               }
            }
            else
            {
               JOptionPane.showMessageDialog(getParent(), msg,
                                             "Save Failed",
                                             JOptionPane.ERROR_MESSAGE);
            }

            status = false;
         }
      }
      else
      {
         status = false;
      }

      return status;
   }

   protected Container getParentFrame()
   {
      if ( null == mParentFrame )
      {
         mParentFrame = SwingUtilities.getAncestorOfClass(Frame.class, this);
      }

      return mParentFrame;
   }

   /**
    * Handles the process of choosing a file name for saving and verifying
    * that the chosen file name is valie.
    *
    * @return A String object containing the chosen file name is returned.
    *         If null is returned, then no choice was made.
    */
   private String pickFileToSave()
   {
      fileChooser.setDialogTitle("Save Configuration As...");
      fileChooser.setDialogType(JFileChooser.SAVE_DIALOG);
      fileChooser.setFileFilter( new ConfigFileFilter() );
      fileChooser.setFileView( new ConfigFileView() );
      fileChooser.setFileHidingEnabled(false);
      fileChooser.setAcceptAllFileFilterUsed(false);

      String chosen_filename = null;

      // Loop until the user selects a valid file name to which data can be
      // written.
      while ( true )
      {
         int result = fileChooser.showSaveDialog(getParentFrame());

         // If the user pressed the approve option in the file chooser,
         // then validate the chosen file name and save everything in our
         // context to that file.
         if ( result == JFileChooser.APPROVE_OPTION )
         {
            File new_file = fileChooser.getSelectedFile();

            // JFileChooser implements File Filters, but if the user
            // types in a name, the JFile Chooser does NOT automatically
            // add the selected file extension to the name.  Go figure.
            if ( ! new_file.getName().endsWith(".jconf") )
            {
               new_file = new File(new_file.getAbsolutePath() + ".jconf");
            }

            if ( new_file.exists() )
            {
               result =
                  JOptionPane.showConfirmDialog(getParentFrame(),
                                                "Overwrite existing file " +
                                                   new_file.getName() + "?",
                                                "Confirm File Overwrite",
                                                JOptionPane.YES_NO_CANCEL_OPTION);

               // The user chose to cancel the whole save operationl.
               if ( result == JOptionPane.CANCEL_OPTION )
               {
                  chosen_filename = null;
                  break;
               }
               // The user chose not to overwrite the existing file, so we
               // go back to the top of the loop and open up the file chooser
               // again.
               else if ( result == JOptionPane.NO_OPTION )
               {
                  chosen_filename = null;
                  continue;
               }
            }

            // Verify that we can write to the file represented by new_file.
            boolean can_write = false;
            if ( new_file.exists() && new_file.canWrite() )
            {
               can_write = true;
            }
            else
            {
               try
               {
                  // At this point, we know that the file represented by
                  // new_file does not exist, so we can ignore the return type
                  // of java.io.File.createNewFile().
                  new_file.createNewFile();
                  new_file.delete();
                  can_write = true;
               }
               catch(Exception ex)
               {
                  can_write = false;
               }
            }

            // If we cannot write to the file represented by new_file, ask
            // the user what to do next.
            if ( ! can_write )
            {
               result =
                  JOptionPane.showConfirmDialog(getParentFrame(),
                                                "Cannot write to " +
                                                   new_file.getAbsolutePath() +
                                                   "!\n" +
                                                   "Pick another file name?",
                                                "Write Access Denied",
                                                JOptionPane.YES_NO_OPTION);

               // The user chose to pick another file name, so we go back to
               // the top of the loop and open up the file chooser again.
               if ( result == JOptionPane.YES_OPTION )
               {
                  chosen_filename = null;
                  continue;
               }
               // Ther user chose to cancel the whole save operation.
               else
               {
                  chosen_filename = null;
                  break;
               }
            }

            // If we have reached this point, then we have our file name, and
            // we can break out of this loop.
            chosen_filename = new_file.getAbsolutePath();
            break;
         }
         // The user chose to cancel the file selection, so we are done.
         else
         {
            chosen_filename = null;
            break;
         }
      }

      return chosen_filename;
   }

   // JBuilder GUI variables
   protected JToolBar toolbar = new JToolBar();
   protected JButton newBtn = new JButton();
   protected JButton openBtn = new JButton();
   protected JButton saveBtn = new JButton();
   protected JButton saveAsBtn = new JButton();
   protected JButton undoBtn = new JButton();
   protected JButton redoBtn = new JButton();
   protected JToggleButton expandBtn = new JToggleButton();
   protected JFileChooser fileChooser = new JFileChooser();

   protected ConfigContext context = new ConfigContext();

   private EditContextPopup contextEditor;
   private ContextChangeListener contextListener = new ContextChangeListener();

   private EnvironmentService mEnvService = new EnvironmentServiceProxy();

   private Container mParentFrame = null;
   private TweekFrameAdapter mTweekFrameListener = null;

   private FileLoader          mFileLoaderBean = null;
   private FileActionGenerator mFileActionGen = new FileActionGenerator();
   private UndoHandler         mUndoHandlerBean = null;
   private UndoActionGenerator mUndoActionGen = new UndoActionGenerator();

   /**
    * Our special context change listener used to toggle the save and expand
    * toolbar buttons.
    */
   private class ContextChangeListener
      implements ConfigContextListener
   {
      public void resourceAdded(ConfigContextEvent evt)
      {
         saveAsBtn.setEnabled(true);
         expandBtn.setEnabled(true);
      }

      public void resourceRemoved(ConfigContextEvent evt)
      {
         if (context.getResources().size() == 0)
         {
            saveBtn.setEnabled(false);
            saveAsBtn.setEnabled(false);
         }
         if (getBroker().getResourceNames().size() == 0)
         {
            expandBtn.setEnabled(false);
         }
      }
   }
}
