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

import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.FileLoader;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;

import org.vrjuggler.vrjconfig.ui.ConfigToolbar;
import org.vrjuggler.vrjconfig.ui.ContextToolbar;

public class VrjConfig
   extends JPanel
   implements FileLoader
{
   public VrjConfig()
   {
      // Init the GUI
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
   }

   private static EnvironmentService mEnvService;

   static
   {
      mEnvService = new EnvironmentServiceProxy();
   }

   //--------------------------------------------------------------------------
   // FileLoader implementation
   //--------------------------------------------------------------------------
   public String getFileType()
   {
      return "VR Juggler Configuration";
   }

   public boolean canOpenMultiple() { return true; }

   public boolean openRequested()
   {
      return mToolbar.doOpen();
   }

   public boolean canSave() { return true; }

   public boolean saveRequested()
   {
      return mToolbar.doSaveAll();
   }

   public boolean saveAsRequested()
   {
      return mToolbar.doSaveAs();
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

   /**
    * Responding to notification that from the toolbar that a configuration has
    * been opened or a new configuration has been created, this method creates
    * a new configuration window to hold it.
    */
   private ConfigIFrame toolbarContextChanged()
   {
      ConfigIFrame frame = new ConfigIFrame(mToolbar.getCurrentDirectory(),
                                            mToolbar.getConfigContext());
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
      frame.addInternalFrameListener(mCloseListener);
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
      frame.removeInternalFrameListener(mCloseListener);
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

   // JBuilder GUI variables
   private BorderLayout mBaseLayout = new BorderLayout();
   private ConfigToolbar mToolbar = new ConfigToolbar();
   private JDesktopPane mDesktop = new JDesktopPane();

   /** A handle to the configuration broker. */
   private ConfigBroker mBroker;

   /** Our listener for activation changes to the internal frames. */
   private InternalFrameListener mActivationListener = new ActivationListener();

   /** Our listener for close notifications from the internal frames. */
   private InternalFrameListener mCloseListener = new CloseListener();

   /**
    * The special internal frame used to hold configuration editors.
    */
   private class ConfigIFrame
      extends JInternalFrame implements ActionListener
   {
      public ConfigIFrame(File curDir, ConfigContext ctx)
      {
         super("Configuration Editor",
               true,
               true,
               true,
               true);
         getContentPane().setLayout(new BorderLayout());
         mContextToolbar = new ContextToolbar(curDir, ctx);

         mToolbar.addActionListener(this);
         addActionListener(editor.getContextEditor().getElementTree());
               
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

      /**
       * When the ConfigToolbar performs an action, we only want to propogate it
       * to our listeners if we are the currently selected JInternalFrame.
       */
      public void actionPerformed(ActionEvent evt)
      {
         // We only want to propogate ActionEvents if we are selected.
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
      
      private ContextToolbar        mContextToolbar = null;
      private GenericConfigEditor   editor   = new GenericConfigEditor();
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
      }

      public void internalFrameDeactivated(InternalFrameEvent evt)
      {
         ConfigIFrame frame = (ConfigIFrame)evt.getInternalFrame();
      }
   }

   /**
    * The special internal frame listener detects when a frame has been closed
    * and cleans up the files that are open.
    */
   private class CloseListener
      extends InternalFrameAdapter
   {
      /**
       * User wants to close the frame. Check if there are any unsaved changes
       * first.
       */
      public void internalFrameClosing(InternalFrameEvent evt)
      {
         JInternalFrame src = evt.getInternalFrame();
         src.dispose();
      }

      /**
       * The frame has closed. Clean up after all the files that were open.
       */
      public void internalFrameClosed(InternalFrameEvent evt)
      {
         ConfigIFrame frame = (ConfigIFrame)evt.getInternalFrame();
         mToolbar.setConfigContext(frame.getEditor().getConfigContext());
         mToolbar.doClose();
         mToolbar.setConfigContext(new ConfigContext());
      }
   }
}
