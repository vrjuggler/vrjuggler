/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
import java.awt.event.*;
import java.beans.PropertyVetoException;
import java.io.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;
import org.vrjuggler.jccl.editors.net.TinyBrowser;

import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.ClipboardUser;
import org.vrjuggler.tweek.beans.FileLoader;
import org.vrjuggler.tweek.beans.HelpProvider;
import org.vrjuggler.tweek.beans.UndoHandler;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.event.*;
import org.vrjuggler.tweek.net.CommunicationEvent;
import org.vrjuggler.tweek.net.CommunicationListener;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;

import org.vrjuggler.vrjconfig.ui.ConfigToolbar;
import org.vrjuggler.vrjconfig.ui.ContextToolbar;


public class VrjConfig
   extends JPanel
   implements FileLoader
            , HelpProvider
            , ClipboardUser
            , UndoHandler
            , CommunicationListener
{
   public VrjConfig()
   {
      mToolbar = new ConfigToolbar(this);
      mToolbar.setRemoteReconfigEnabled(false);

      // Init the GUI
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      mHelpBrowserFrame.setContentPane(mHelpBrowser);
      mHelpBrowserFrame.setSize(new java.awt.Dimension(640, 480));
      mHelpBrowserFrame.setTitle("VR Juggler Configuration Help Browser");
      mHelpBrowserFrame.validate();

      // Parsing command line arguments
      try
      {
         EnvironmentService env_service = new EnvironmentServiceProxy();

         String[] args = env_service.getCommandLineArgs();
         if ( null != args )
         {
            for (int i = 0; i < args.length; i++)
            {
               System.out.println(args[i]);
            }
         }
         ConfigContext ctx = new ConfigContext();
         mToolbar.doOpenCmdArgs(ctx);
      }
      catch(Exception ex)
      {
         System.out.println(ex);
         ex.printStackTrace();
      }
   }

   private static EnvironmentService mEnvService;

   static
   {
      mEnvService = new EnvironmentServiceProxy();
   }

   public String getFileType()
   {
      return "VR Juggler Configuration";
   }

   public boolean canOpenMultiple()
   {
      return true;
   }

   public boolean openRequested()
   {
      return mToolbar.doOpen();
   }

   public boolean canSave()
   {
      return true;
   }

   public boolean hasUnsavedChanges()
   {
      JInternalFrame[] frames = mDesktop.getAllFrames();

      boolean has_unsaved = false;
      for ( int i = 0; i < frames.length; ++i )
      {
         ConfigContext ctx = ((ConfigIFrame) frames[i]).getConfigContext();
         if ( ctx.getConfigUndoManager().getUnsavedChanges() )
         {
            has_unsaved = true;
            break;
         }
      }

      return has_unsaved;
   }

   public boolean saveRequested()
   {
      return mToolbar.doSaveAll();
   }

   public boolean saveAsRequested()
   {
      boolean status = false;

      if ( mCurContextFrame != null )
      {
         status = mCurContextFrame.doSaveAs();
      }

      return status;
   }

   public boolean saveAllRequested()
   {
      return mToolbar.doSaveAll();
   }

   public boolean closeRequested()
   {
      ConfigIFrame frame = (ConfigIFrame)mDesktop.getSelectedFrame();
      try
      {
         frame.setClosed(true);
      }
      catch (PropertyVetoException pve) {}
      removeFrame(frame);
      frame.dispose();
      return true;
   }

   public int getOpenFileCount()
   {
      return mDesktop.getAllFrames().length;
   }

   public String getHelpDescription()
   {
      return "VR Juggler Configuration Help ...";
   }

   public void helpRequested()
   {
      try
      {
         mHelpBrowser.setPage(
            new java.net.URL(VrjConfigConstants.HELP_URL_STR)
         );
      }
      catch(java.net.MalformedURLException ex)
      {
         /* Ignore the exception. */ ;
      }

      mHelpBrowserFrame.setVisible(true);
   }

   public void cutRequested()
   {
      mToolbar.fireAction(ConfigToolbar.CUT_ACTION);
   }

   public void copyRequested()
   {
      mToolbar.fireAction(ConfigToolbar.COPY_ACTION);
   }

   public void pasteRequested()
   {
      mToolbar.fireAction(ConfigToolbar.PASTE_ACTION);
   }

   public javax.swing.undo.UndoManager getUndoManager()
   {
      if ( mCurContextFrame == null )
      {
         return null;
      }
      else
      {
         return mCurContextFrame.getConfigContext().getConfigUndoManager();
      }
   }

   public void undoRequested()
   {
      if ( mCurContextFrame != null )
      {
         mCurContextFrame.doUndo();
      }
   }

   public void redoRequested()
   {
      if ( mCurContextFrame != null )
      {
         mCurContextFrame.doRedo();
      }
   }

   public void connectionOpened(CommunicationEvent e)
   {
      mOrbList.add(e.getCorbaService());
      mToolbar.setRemoteReconfigEnabled(true);
   }

   public void connectionClosed(CommunicationEvent e)
   {
      mOrbList.remove(e.getCorbaService());
      mToolbar.setRemoteReconfigEnabled(mOrbList.size() > 0);
   }

   /**
    * Responding to notification that from the toolbar that a configuration has
    * been opened or a new configuration has been created, this method creates
    * a new configuration window to hold it.
    */
   private ConfigIFrame toolbarContextChanged()
   {
      ConfigIFrame frame = new ConfigIFrame(mToolbar.getCurrentDirectory(),
                                            mToolbar.getConfigContext(), this,
                                            this);
      frame.getEditor().setConfigContext(mToolbar.getConfigContext());
      addFrame(frame);
      return frame;
   }

   /**
    * Adds the given frame to the desktop and selects it.
    */
   private void addFrame(JInternalFrame frame)
   {
      frame.addInternalFrameListener(mActivationListener);
      frame.pack();
      frame.setVisible(true);
      mDesktop.add(frame);
      try
      {
         frame.setSelected(true);
      }
      catch (PropertyVetoException pve) { /*ignore*/ }
   }

   /**
    * Removes the given frame from the desktop.
    */
   private void removeFrame(JInternalFrame frame)
   {
      mDesktop.remove(frame);
      frame.removeInternalFrameListener(mActivationListener);
   }

   /**
    * Gets a handle to the configuration broker service.
    */
   private synchronized ConfigBroker getBroker()
   {
      if (mBroker == null)
      {
         mBroker = new ConfigBrokerProxy();
      }
      return mBroker;
   }

   /**
    * Returns a copy of the given string with all environment variables
    * expanded.
    */
   private static String expandEnvVars(String str)
   {
      return mEnvService.expandEnvVars(str);
   }

   /**
    * JBuilder GUI initialization.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(mBaseLayout);
      mDesktop.setBorder(BorderFactory.createEtchedBorder());
      mToolbar.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            String cmd = evt.getActionCommand();
            if (cmd.equals("New") || cmd.equals("Open"))
            {
               toolbarContextChanged();
            }
         }
      });
      this.add(mToolbar,  BorderLayout.NORTH);
      this.add(mDesktop, BorderLayout.CENTER);
   }

   private TinyBrowser mHelpBrowser      = new TinyBrowser();
   private JFrame      mHelpBrowserFrame = new JFrame();

   private ConfigIFrame mCurContextFrame = null;

   // JBuilder GUI variables
   private BorderLayout mBaseLayout = new BorderLayout();
   private ConfigToolbar mToolbar = null;
   private JDesktopPane mDesktop = new JDesktopPane();

   /** A handle to the configuration broker. */
   private ConfigBroker mBroker;

   /** Used to keep track of the open CORBA connections. */
   private List mOrbList = new ArrayList();

   /** Our listener for activation changes to the internal frames. */
   private InternalFrameListener mActivationListener = new ActivationListener();

   /**
    * The special internal frame used to hold configuration editors.
    */
   private class ConfigIFrame
      extends JInternalFrame
      implements ActionListener
   {
      public ConfigIFrame(File curDir, ConfigContext ctx,
                          FileLoader fileLoader, UndoHandler undoHandler)
      {
         super("Configuration Editor",
               true,
               true,
               true,
               true);
         getContentPane().setLayout(new BorderLayout());
         mContextToolbar = new ConfigIFrameToolbar(curDir, ctx, this,
                                                   fileLoader, undoHandler);

         mToolbar.addActionListener(this);
         addActionListener(editor.getContextEditor().getElementTree());
        
         setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
         addInternalFrameListener(new InternalFrameAdapter()
            {
               public void internalFrameClosing(InternalFrameEvent evt)
               {
                  if ( mContextToolbar.doClose() )
                  {
                     dispose();
                  }
               }
               public void internalFrameClosed(InternalFrameEvent evt)
               {
                  mToolbar.removeActionListener(ConfigIFrame.this);
                  mToolbar.setConfigContext(new ConfigContext());
               }
            });
               
         getContentPane().add(mContextToolbar, BorderLayout.NORTH);
         getContentPane().add(editor, BorderLayout.CENTER);
         setDefaultCloseOperation(JInternalFrame.DO_NOTHING_ON_CLOSE);
      }

      /**
       * Gets the editor for this frame.
       */
      public GenericConfigEditor getEditor()
      {
         return editor;
      }

      public boolean doSaveAs()
      {
         return mContextToolbar.doSaveAs();
      }

      public void doUndo()
      {
         mContextToolbar.doUndo();
      }
      public void doRedo()
      {
         mContextToolbar.doRedo();
      }

      /**
       * When the ConfigToolbar performs an action, we only want to propogate it
       * to our listeners if we are the currently selected JInternalFrame.
       */
      public void actionPerformed(ActionEvent evt)
      {
         // Intercept SaveAll action, otherwise we only want to propogate
         // ActionEvents if we are the selected frame.
         if(evt.getActionCommand().equals("SaveAll"))
         {
            mContextToolbar.doSave();
         }
         if(isSelected())
         {
            fireAction(evt.getActionCommand());
         }
      }
      
      /**
       * Add an ActionListener that will recieve events that we recieve only if
       * we are the currently selecdted JInternalFrame.
       */
      public void addActionListener(ActionListener listener)
      {
         listenerList.add(ActionListener.class, listener);
      }
      
      /**
       * Remove an ActionListener.
       */
      public void removeActionListener(ActionListener listener)
      {
         listenerList.remove(ActionListener.class, listener);
      }

      /**
       * Return the ConfigContext associated with the ConfigIFrame.
       */
      public ConfigContext getConfigContext()
      {
         return ( mToolbar.getConfigContext() );
      }
      
      /**
       * Propogate an ActionEvent to our listeners.
       */
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

      private ConfigIFrameToolbar   mContextToolbar = null;
      private GenericConfigEditor   editor   = new GenericConfigEditor();
   }

   private class ConfigIFrameToolbar
      extends ContextToolbar
   {
      private static final String SAVED_TITLE   = "Configuration Editor";
      private static final String UNSAVED_TITLE = SAVED_TITLE + " < Unsaved >";

      public ConfigIFrameToolbar(File curDir, ConfigContext ctx,
                                 ConfigIFrame frame, FileLoader fileLoader,
                                 UndoHandler undoHandler)
      {
         super(curDir, ctx, fileLoader, undoHandler);
         mConfigIFrame = frame;

         toolbar.add(newBtn, null);
         toolbar.add(openBtn, null);
         toolbar.add(saveBtn, null);
         toolbar.add(saveAsBtn, null);
         toolbar.addSeparator();
         toolbar.add(undoBtn, null);
         toolbar.add(redoBtn, null);
         toolbar.addSeparator();
         toolbar.add(Box.createHorizontalGlue(), null);
         toolbar.add(expandBtn, null);
      }

      public void undoableEditHappened(UndoableEditEvent e)
      {
         super.undoableEditHappened(e);
         mConfigIFrame.setTitle(UNSAVED_TITLE);
      }

      public boolean doSave()
      {
         boolean saved = super.doSave();

         if ( saved )
         {
            mConfigIFrame.setTitle(SAVED_TITLE);
         }

         return saved;
      }

      public void doUndo()
      {
         super.doUndo();

         if (context.getConfigUndoManager().getUnsavedChanges())
         {
            mConfigIFrame.setTitle(UNSAVED_TITLE);
         }
         else
         {
            mConfigIFrame.setTitle(SAVED_TITLE);
         }
      }

      public void doRedo()
      {
         super.doRedo();
         if (context.getConfigUndoManager().getUnsavedChanges())
         {
            mConfigIFrame.setTitle(UNSAVED_TITLE);
         }
         else
         {
            mConfigIFrame.setTitle(SAVED_TITLE);
         }
      }

      public boolean doSaveAs()
      {
         boolean saved = super.doSaveAs();

         if ( saved )
         {
            mConfigIFrame.setTitle(SAVED_TITLE);
         }

         return saved;
      }

      private ConfigIFrame mConfigIFrame = null;
   }

   /**
    * The special internal frame listener updates the toolbar whenever an
    * internal frame is de/activated.
    */
   private class ActivationListener
      extends InternalFrameAdapter
   {
      public void internalFrameActivated(InternalFrameEvent evt)
      {
         ConfigIFrame frame = (ConfigIFrame)evt.getInternalFrame();
         mToolbar.setConfigContext(frame.getEditor().getConfigContext());
         mCurContextFrame = frame;
      }

      public void internalFrameDeactivated(InternalFrameEvent evt)
      {
         ConfigIFrame frame = (ConfigIFrame)evt.getInternalFrame();
         mCurContextFrame = null;
      }
   }
}
