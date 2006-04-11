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

import java.awt.*;
import java.awt.event.*;
import java.beans.PropertyVetoException;
import java.io.*;
import java.util.Iterator;
import javax.swing.*;
import javax.swing.event.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;

import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.FileLoader;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.services.EnvironmentService;

import org.vrjuggler.vrjconfig.ui.ConfigToolbar;

public class VrjConfig
   extends JPanel
   implements FileLoader
{
   public VrjConfig()
   {
      // Load in the VR Juggler chunk definitions if it has not already been
      // loaded.
      String desc_filename = EnvironmentService.expandEnvVars("${VJ_BASE_DIR}/share/vrjuggler/data/vrj-chunks.desc");
      File desc_file = new File(desc_filename);

      try
      {
         System.out.println("Trying to load VR Juggler configuration definitions ...");
         System.out.println("\tExamining '" + desc_filename + "'");
         if (desc_file.exists() && desc_file.canRead())
         {
            // File exists and is readable. Lets see if we can load it.
            FileDataSource data_source = new FileDataSource(desc_filename,
                                                            FileDataSource.DEFINITIONS);
            getConfigBroker().add(desc_filename, data_source);
         }
         else
         {
            System.out.println("\tFAILED: File is unreadable.");
         }
      }
      catch (IOException ioe)
      {
         System.out.println("\tFAILED: "+ioe.getMessage());
      }

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
      return toolbar.doOpen();
   }

   public boolean canSave() { return true; }

   public boolean saveRequested()
   {
      return toolbar.doSave();
   }

   public boolean saveAsRequested()
   {
      return toolbar.doSaveAs();
   }

   public boolean closeRequested()
   {
      ConfigIFrame frame = (ConfigIFrame)desktop.getSelectedFrame();
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
      return desktop.getAllFrames().length;
   }

   /**
    * Responding to notification that from the toolbar that a configuration has
    * been opened or a new configuration has been created, this method creates
    * a new configuration window to hold it.
    */
   private ConfigIFrame toolbarContextChanged()
   {
      ConfigIFrame frame = new ConfigIFrame();
      frame.getEditor().setConfigContext(toolbar.getConfigContext());
      addFrame(frame);
      return frame;
   }

   /**
    * Adds the given frame to the desktop and selects it.
    */
   private void addFrame(JInternalFrame frame)
   {
      frame.addInternalFrameListener(activationListener);
      frame.addInternalFrameListener(closeListener);
      frame.pack();
      frame.setVisible(true);
      desktop.add(frame);
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
      desktop.remove(frame);
      frame.removeInternalFrameListener(activationListener);
      frame.removeInternalFrameListener(closeListener);
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
            if (cmd.equals("New") || cmd.equals("Open"))
            {
               toolbarContextChanged();
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

   /**
    * Our listener for activation changes to the internal frames.
    */
   private InternalFrameListener activationListener = new ActivationListener();

   /**
    * Our listener for close notifications from the internal frames.
    */
   private InternalFrameListener closeListener = new CloseListener();

   /**
    * The special internal frame used to hold configuration editors.
    */
   private class ConfigIFrame
      extends JInternalFrame
   {
      public ConfigIFrame()
      {
         super("Configuration Editor",
               true,
               true,
               true,
               true);
         getContentPane().setLayout(new BorderLayout());
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

      private GenericConfigEditor editor = new GenericConfigEditor();
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
         toolbar.setConfigContext(frame.getEditor().getConfigContext());
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
         toolbar.setConfigContext(frame.getEditor().getConfigContext());
         toolbar.doClose();
         toolbar.setConfigContext(new ConfigContext());
      }
   }
}
