/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

package org.vrjuggler.vrjconfig.customeditors.cave;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dialog;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.Point;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;
import javax.swing.border.TitledBorder;
import javax.swing.event.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;
import info.clearthought.layout.*;


public class MainEditorPanel
   extends JPanel
   implements EditorConstants
{
   private JPanel mStereoPanel = new JPanel();
   private JRadioButton mActiveStereoBtn = new JRadioButton();
   private JRadioButton mPassiveStereoBtn = new JRadioButton();
   private JRadioButton mMonoBtn = new JRadioButton();
   private ButtonGroup mStereoGroup = new ButtonGroup();

   private JPanel mViewPanel = new JPanel();
   private ViewEditorPanel mLeftViewEditorPanel = null;
   private ViewEditorPanel mRightViewEditorPanel = null;
   
   private TitledBorder mStereoBorder;
   private TitledBorder mViewBorder;
   private CaveWall mCaveWall = null;
   private CaveModel mCaveModel = null;

   public void setWallConfig(CaveModel cm, CaveWall in_wall)
   {
      mCaveModel = cm;
      mCaveWall = in_wall;
      
      /*
      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }
      */

      mLeftViewEditorPanel.setCaveModel(mCaveModel);
      mRightViewEditorPanel.setCaveModel(mCaveModel);
      
      updateStereoGUI();
      updateViewGUI();

      this.revalidate();
      this.repaint();
   }



   public MainEditorPanel()
   {
      try
      {
         jbInit();
      }
      catch(Exception ex)
      {
         ex.printStackTrace();
      }
   }
   
   private Container getOwner()
   {
      Object owner = SwingUtilities.getRoot(this);
      return (Container) owner;
   }
   
   /**
    * Positions the given Dialog object relative to this window frame.
    */
   private void positionDialog(Dialog dialog, Container parent)
   {
      Dimension dlg_size   = dialog.getPreferredSize();
      Dimension frame_size = parent.getSize();
      Point loc            = parent.getLocation();

      // Set the location of the dialog so that it is centered with respect
      // to this frame.
      dialog.setLocation((frame_size.width - dlg_size.width) / 2 + loc.x,
                         (frame_size.height - dlg_size.height) / 2 + loc.y);
   }
   
   void jbInit() throws Exception
   {
      ClassLoader loader = getClass().getClassLoader();
      
      try
      {
         mViewBorder = new TitledBorder("View");
         mStereoBorder = new TitledBorder("Stereo/Mono");
      }
      catch (NullPointerException ex)
      {
      }
      
      mActiveStereoBtn.setText("Active Stereo Display");
      mPassiveStereoBtn.setText("Passive Stereo Display");
      mMonoBtn.setText("Mono Display");

      mActiveStereoBtn.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent e)
               {
                  mCaveWall.setStereoMode(CaveWall.ACTIVE_STEREO, mCaveModel.getConfigContext());
                  updateViewGUI();
               }
            });
      
      mPassiveStereoBtn.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent e)
               {
                  mCaveWall.setStereoMode(CaveWall.PASSIVE_STEREO, mCaveModel.getConfigContext());
                  updateViewGUI();
               }
            });
      
      mMonoBtn.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent e)
               {
                  mCaveWall.setStereoMode(CaveWall.MONO, mCaveModel.getConfigContext());
                  updateViewGUI();
               }
            });


      
      double[][] main_size = {{TableLayout.PREFERRED, TableLayout.PREFERRED},
                              {TableLayout.PREFERRED, TableLayout.PREFERRED}};
      
      this.setLayout(new TableLayout(main_size));
      
      /*
      this.add(mWallPanel, new TableLayoutConstraints(0, 0, 0, 1,
                                       TableLayout.FULL,
                                       TableLayout.FULL));
      */
      this.add(mViewPanel, new TableLayoutConstraints(1, 1, 1, 1,
                                               TableLayout.FULL,
                                               TableLayout.FULL));
      
      double[][] vp_size = {{TableLayout.PREFERRED},
                            {TableLayout.PREFERRED, TableLayout.PREFERRED}};
      mViewPanel.setLayout(new TableLayout(vp_size));
      mViewPanel.setBorder(mViewBorder);

      mLeftViewEditorPanel = new ViewEditorPanel(new Dimension(200, 200), "Left Eye");
      mViewPanel.add(mLeftViewEditorPanel, new TableLayoutConstraints(0, 0, 0, 0,
                                               TableLayout.FULL,
                                               TableLayout.FULL));
      
      mRightViewEditorPanel = new ViewEditorPanel(new Dimension(200, 200), "Right Eye");
      mViewPanel.add(mRightViewEditorPanel, new TableLayoutConstraints(0, 1, 0, 1,
                                               TableLayout.FULL,
                                               TableLayout.FULL));
      
      this.add(mStereoPanel, new TableLayoutConstraints(1, 0, 1, 0,
                                               TableLayout.FULL,
                                               TableLayout.FULL));

      double[][] sp_size = {{TableLayout.PREFERRED},
                            {TableLayout.PREFERRED, TableLayout.PREFERRED, TableLayout.PREFERRED}};
      mStereoPanel.setLayout(new TableLayout(sp_size));
      mStereoPanel.setBorder(mStereoBorder);
      
      mStereoGroup.add(mActiveStereoBtn);
      mStereoGroup.add(mPassiveStereoBtn);
      mStereoGroup.add(mMonoBtn);
      
      mStereoPanel.add(mActiveStereoBtn, new TableLayoutConstraints(0, 0, 0, 0,
                                         TableLayout.FULL,
                                         TableLayout.FULL));;
      mStereoPanel.add(mPassiveStereoBtn, new TableLayoutConstraints(0, 1, 0, 1,
                                          TableLayout.FULL,
                                          TableLayout.FULL));
      mStereoPanel.add(mMonoBtn, new TableLayoutConstraints(0, 2, 0, 2,
                                 TableLayout.FULL,
                                 TableLayout.FULL));
   }

   private void setStereoMode(int stereo_mode)
   {
      int old_stereo_mode = mCaveWall.getStereoMode();
      
      if (old_stereo_mode == stereo_mode)
      {
         return;
      }
      else if (CaveWall.PASSIVE_STEREO == old_stereo_mode)
      {
         System.out.println("Loseing a view.");
      }
      
      mCaveWall.setStereoMode(stereo_mode, mCaveModel.getConfigContext());
   }

   private void updateStereoGUI()
   {
      if (CaveWall.ACTIVE_STEREO == mCaveWall.getStereoMode())
      {
         mActiveStereoBtn.setSelected(true);
      }
      else if (CaveWall.PASSIVE_STEREO == mCaveWall.getStereoMode())
      {
         mPassiveStereoBtn.setSelected(true);
      }
      else
      {
         mMonoBtn.setSelected(true);
      }
   }

   private void updateViewGUI()
   {
      if (CaveWall.ACTIVE_STEREO == mCaveWall.getStereoMode())
      {
         mLeftViewEditorPanel.setConfig(mCaveWall.getLeftView());
         mRightViewEditorPanel.setConfig(null);
         
         mActiveStereoBtn.setSelected(true);
         mLeftViewEditorPanel.setTitle("Stereo");
         mRightViewEditorPanel.setTitle("");
         mRightViewEditorPanel.setEnabled(false);
      }
      else if (CaveWall.PASSIVE_STEREO == mCaveWall.getStereoMode())
      {
         mLeftViewEditorPanel.setConfig(mCaveWall.getLeft());
         mRightViewEditorPanel.setConfig(mCaveWall.getRight());
         
         mLeftViewEditorPanel.setTitle("Left Eye");
         mRightViewEditorPanel.setTitle("Right Eye");
         mRightViewEditorPanel.setEnabled(true);
      }
      else
      {
         mLeftViewEditorPanel.setConfig(mCaveWall.getLeft());
         mRightViewEditorPanel.setConfig(null);
         
         mLeftViewEditorPanel.setTitle("Mono");
         mRightViewEditorPanel.setTitle("");
         mRightViewEditorPanel.setEnabled(false);
      }
      
      this.revalidate();
      this.repaint();
   }
}
