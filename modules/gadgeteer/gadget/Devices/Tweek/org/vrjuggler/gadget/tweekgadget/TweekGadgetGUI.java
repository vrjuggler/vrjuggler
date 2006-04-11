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

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import org.omg.CORBA.BAD_PARAM;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.beans.FileLoader;
import org.vrjuggler.tweek.net.*;
import org.vrjuggler.tweek.net.corba.*;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.ExtensionFileFilter;
import tweek.Subject;
import gadget.*;
import javax.swing.*;
import java.awt.*;


public class TweekGadgetGUI
   extends JPanel
      implements java.io.Serializable, FileLoader, CommunicationListener
{
   public TweekGadgetGUI()
   {
      String desc_file = "${VJ_BASE_DIR}/share/vrjuggler/data/vrj-chunks.desc";
      String full_desc_file = EnvironmentService.expandEnvVars(desc_file);
      ChunkDescDB desc_db = new ChunkDescDB();

      try
      {
         desc_db.build(new java.io.File(full_desc_file));
         ChunkFactory.setDescs(desc_db.getAll());
      }
      catch (IOException ex)
      {
         System.err.println("Shit!");
      }

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
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
      filter.addExtension("config");
      filter.addExtension("cfg");
      chooser.addChoosableFileFilter(filter);

      int status = chooser.showOpenDialog(this);

      if ( status == JFileChooser.APPROVE_OPTION )
      {
         try
         {
            mConfigFile.build(chooser.getSelectedFile());
            SwingUtilities.invokeLater(new Runnable()
            {
               public void run()
               {
                  List elts =
                     ConfigUtilities.getChunksWithDescToken(mConfigFile.getAll(),
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
      ConfigChunk cur_elem;
      TweekDigitalButton button;

      while ( i.hasNext() )
      {
         cur_elem = (ConfigChunk) i.next();

         for ( int j = 0; j < cur_elem.getNumPropertyValues("digitalDevice"); ++j )
         {
            String ptr_name = (String)cur_elem.getProperty("digitalDevice", j);
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

   private static String mLayoutConfigType = "TweekGadgetGUI";

   private int openFileCount = 0;
   private ConfigChunkDB mConfigFile   = new ConfigChunkDB();
   private CorbaService  mCorbaService = null;
   private List          mPosDevs      = new ArrayList();
   private List          mDigitalDevs  = new ArrayList();
   private List          mAnalogDevs   = new ArrayList();
   private JPanel mMainPanel = new JPanel();
   private JPanel mTitlePanel = new JPanel();
   private BorderLayout mTopLayout = new BorderLayout();
   private JLabel mTitleLabel = new JLabel();
}
