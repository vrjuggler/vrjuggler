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
package org.vrjuggler.gadget.tweekgadget;

import java.awt.*;
import javax.swing.*;
import java.io.*;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.StringTokenizer;
import org.omg.CORBA.BAD_PARAM;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.io.*;
import org.vrjuggler.tweek.beans.FileLoader;
import org.vrjuggler.tweek.net.*;
import org.vrjuggler.tweek.net.corba.*;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.tweek.services.ExtensionFileFilter;
import tweek.Subject;
import gadget.*;


public class TweekGadgetGUI
   extends JPanel
      implements java.io.Serializable, FileLoader, CommunicationListener
{
   public TweekGadgetGUI()
   {
      initRepository();

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
    * Fills the repository with all the definitions it can find in the
    * configuration definition search path.
    */
   private void initRepository()
   {
      mRepos = new ConfigDefinitionRepository();

      // Get a list of the definition files to load
      List def_file_list = new ArrayList();
      List def_path = getDefinitionPath();
      for (Iterator itr = def_path.iterator(); itr.hasNext(); )
      {
         // Check if this part of the path is a valid directory we can read
         String dir_name = (String)itr.next();
         File dir = new File(dir_name);
         if (dir.exists() && dir.isDirectory() && dir.canRead())
         {
            // Get a list of all the config definition files in the directory
            File[] def_files = dir.listFiles(new FilenameFilter()
            {
               public boolean accept(File dir, String file)
               {
                  // Only accept files with a .jdef extension
                  if (file.endsWith(".jdef"))
                  {
                     File def_file = new File(dir, file);
                     if (def_file.canRead())
                     {
                        return true;
                     }
                  }
                  return false;
               }
            });

            // Add the files to the list of files to load
            for (int i=0; i<def_files.length; ++i)
            {
               def_file_list.add(def_files[i]);
            }
         }
      }

      // Load in the definitions for each file and place them in the repository
      for (Iterator itr = def_file_list.iterator(); itr.hasNext(); )
      {
         try
         {
            // Attempt to load in the definitions in the file
            File def_file = (File)itr.next();
            ConfigDefinitionReader reader = new ConfigDefinitionReader(def_file);
            List defs = reader.readDefinition();
            for (Iterator def_itr = defs.iterator(); def_itr.hasNext(); )
            {
               ConfigDefinition def = (ConfigDefinition)def_itr.next();
               mRepos.add(def);
            }
         }
         catch (ParseException pe)
         {
            pe.printStackTrace();
         }
         catch (IOException ioe)
         {
            ioe.printStackTrace();
         }
      }
   }

   /**
    * Gets a list of the directories in which to look for configuration
    * definitions.
    */
   private List getDefinitionPath()
   {
      List dirs = new ArrayList();

      // Get the path from the environment
      String default_path = "${VJ_BASE_DIR}/share/vrjuggler/data/definitions";
      String path = System.getProperty("JCCL_DEFINITION_PATH", default_path);
      path = (new EnvironmentServiceProxy()).expandEnvVars(path);

      // Split the path on the path separator
      StringTokenizer tokenizer = new StringTokenizer(path, File.pathSeparator);
      while (tokenizer.hasMoreTokens())
      {
         dirs.add(tokenizer.nextToken());
      }

      return dirs;
   }

   public String getFileType()
   {
      return "JCCL Config File";
   }

   public boolean canOpenMultiple()
   {
      return false;
   }

   public boolean openRequested()
   {
      JFileChooser chooser = new JFileChooser();
      chooser.setMultiSelectionEnabled(false);
      chooser.setDialogTitle("JCCL Config File Chooser");
      chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);

      ExtensionFileFilter filter = new ExtensionFileFilter("JCCL Config Files");
      filter.addExtension("jconf");
      chooser.addChoosableFileFilter(filter);

      int status = chooser.showOpenDialog(this);

      if ( status == JFileChooser.APPROVE_OPTION )
      {
         try
         {
            ConfigurationReader reader = new ConfigurationReader(chooser.getSelectedFile(),
                                                                 mRepos);
            mConfig = reader.readConfiguration();
            SwingUtilities.invokeLater(new Runnable()
            {
               public void run()
               {
                  List elts =
                     ConfigUtilities.getElementsWithDefinition(mConfig.getElements(),
                                                               mLayoutConfigType);
                  fillInGUI(elts);
               }
            });
         }
         catch (IOException ioEx)
         {
            JOptionPane.showMessageDialog(null, "Failed to load config file: '" +
                                          ioEx.getMessage() + "'",
                                          "I/O Error",
                                          JOptionPane.ERROR_MESSAGE);
         }
         catch (ParseException pe)
         {
            JOptionPane.showMessageDialog(null, "Failed to load config file: '" +
                                          pe.getMessage() + "'",
                                          "I/O Error",
                                          JOptionPane.ERROR_MESSAGE);
         }
      }

      return true;
   }

   public boolean canSave()
   {
      return false;
   }

   public boolean saveRequested()
   {
      return false;
   }

   public boolean saveAsRequested()
   {
      return false;
   }

   public boolean closeRequested()
   {
      return true;
   }

   public int getOpenFileCount()
   {
      return openFileCount;
   }

   public void connectionOpened(CommunicationEvent e)
   {
      mCorbaService = e.getCorbaService();
   }

   public void connectionClosed(CommunicationEvent e)
   {
      Iterator i = mDigitalDevs.iterator();
      TweekDigitalButton button;

      while ( i.hasNext() )
      {
         button = (TweekDigitalButton) i.next();
         button.detach();
      }

      mDigitalDevs.clear();

      i = mPosDevs.iterator();

      while ( i.hasNext() )
      {
         /* Do something. */ ;
      }

      mPosDevs.clear();

      i = mAnalogDevs.iterator();

      while ( i.hasNext() )
      {
         /* Do something. */ ;
      }

      mAnalogDevs.clear();
   }

   protected void fillInGUI(List elements)
   {
      Iterator i = elements.iterator();
      ConfigElement cur_elem;
      TweekDigitalButton button;

      while ( i.hasNext() )
      {
         cur_elem = (ConfigElement) i.next();

         for ( int j = 0; j < cur_elem.getPropertyValueCount("digital_device"); ++j )
         {
            String ptr_name = (String)cur_elem.getProperty("digital_device", j);
            int index = ptr_name.lastIndexOf("/");
            String subj_name = null;

            if ( -1 == index )
            {
               subj_name = ptr_name;
            }
            else
            {
               subj_name = ptr_name.substring(index + 1);
               System.out.println("Got subject named '" + subj_name +
                                  "' from '" + ptr_name + "'");
            }

            Subject subj =
               mCorbaService.getSubjectManager().getSubject(subj_name);
            TweekDigitalSubject dig_subj = null;

            // Try to narrow the Subjet object to a SliderSubject object.  If this
            // fails, it throws a CORBA BAD_PARAM exception.  In that case, we open
            // a dialog box saying that the narrowing failed.
            try
            {
               dig_subj = TweekDigitalSubjectHelper.narrow(subj);
            }
            catch (BAD_PARAM narrow_ex)
            {
               JOptionPane.showMessageDialog(null,
                                             "Failed to narrow subject to TweekDigitalSubject for " + subj_name,
                                             "TweekDigitalSubject Narrow Error",
                                             JOptionPane.ERROR_MESSAGE);
            }

            button = new TweekDigitalButton(mCorbaService, dig_subj);
            button.setText(subj_name);
            mMainPanel.add(button);
            mMainPanel.revalidate();
         }
      }
   }

   private void jbInit() throws Exception
   {
      this.setLayout(mTopLayout);
      mTitleLabel.setText("TweekGadget GUI");
      mTitlePanel.setBorder(BorderFactory.createRaisedBevelBorder());
      this.add(mMainPanel, BorderLayout.CENTER);
      this.add(mTitlePanel, BorderLayout.NORTH);
      mTitlePanel.add(mTitleLabel, null);
   }

   private static String mLayoutConfigType = "tweek_gadget_gui";

   private int openFileCount = 0;
   private Configuration mConfig;
   private CorbaService  mCorbaService = null;
   private List          mPosDevs      = new ArrayList();
   private List          mDigitalDevs  = new ArrayList();
   private List          mAnalogDevs   = new ArrayList();
   private JPanel mMainPanel = new JPanel();
   private JPanel mTitlePanel = new JPanel();
   private BorderLayout mTopLayout = new BorderLayout();
   private JLabel mTitleLabel = new JLabel();

   private ConfigDefinitionRepository mRepos;
}
