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

package org.vrjuggler.vrjconfig.customeditors.cave;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dialog;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Point;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;
import javax.swing.event.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import info.clearthought.layout.*;

import org.netbeans.swing.tabcontrol.*;
import java.awt.Color;

public class CaveEditorPanel
   extends JPanel
   implements ChangeListener
{
   private JSplitPane mSplitPane = new JSplitPane();
   private JScrollPane mScreenScrollPanel = new JScrollPane();
   
   private ScreenDisplay mScreenDisplay = new ScreenDisplay();
   
   private JPanel mLowerPanel = new JPanel();
   private JPanel mAddPanel = new JPanel();
   private JButton mAddScreenBtn = new JButton();
   private JButton mAddWallBtn = new JButton();
      
   private DefaultTabDataModel mWallTabDataModel = new DefaultTabDataModel();
   private TabbedContainer mTabContainer = new TabbedContainer(mWallTabDataModel, TabbedContainer.TYPE_EDITOR);

   private List mWalls = new ArrayList();
   private ConfigContext mContext = null;
   private CaveModel mCaveModel = null;
   
   public void setConfig(ConfigContext ctx)
   {
      mContext = ctx;

      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }

      mCaveModel = new CaveModel( mContext );
      
      mScreenDisplay.setConfig( ctx, mCaveModel );
     
      // Test all Walls to ensure that they are valid and add tabs.
      for (Iterator itr = mCaveModel.getWalls().iterator() ; itr.hasNext() ; )
      {
         CaveWall cw = (CaveWall)itr.next();

         TabData td = new TabData(cw, null, cw.getName(), cw.getName());
         mWallTabDataModel.addTab(mWallTabDataModel.size(), td);
      }
      
      mTabContainer.setComponentConverter(new WallComponentConverter());
      mTabContainer.setActive(true);    
      SingleSelectionModel ssm = mTabContainer.getSelectionModel();
      if (mTabContainer.getTabCount() > 0)
      {
         ssm.setSelectedIndex(0);
      }

      mTabContainer.setMinimumSize(new Dimension(800,500));
      mTabContainer.setPreferredSize(new Dimension(800,500));
      mTabContainer.setMaximumSize(new Dimension(800,500));

      this.revalidate();
      this.repaint();
   }

   public void stateChanged(ChangeEvent e)
   {
      /*
      JSpinner source = (JSpinner) e.getSource();

      // Using the object returned by source.getValue() seems to be safe
      // because every change in the spinner's value creates a new object.
      if ( source == mReportRateSpinner )
      {
         mElement.setProperty("report_rate", 0, source.getValue());
      }
      else if ( source == mMeasurementRateSpinner )
      {
         // NOTE: The object returned is of type Double rather than Float.
         mElement.setProperty("measurement_rate", 0, source.getValue());
      }
      */
   }

   public CaveEditorPanel()
   {
      /*
      try
      {
         jbInit();
      }
      catch(Exception ex)
      {
         ex.printStackTrace();
      }
      */
   }
   
   private Container getOwner()
   {
      Object owner = SwingUtilities.getRoot(this);
      System.out.println("owner class: " + owner.getClass());
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
      this.setLayout(new BorderLayout());
      mSplitPane.setOrientation(JSplitPane.VERTICAL_SPLIT);

      
      this.add(mSplitPane, BorderLayout.CENTER);
      mSplitPane.setContinuousLayout(true);
      mSplitPane.add(mScreenScrollPanel, JSplitPane.TOP);
      mSplitPane.add(mLowerPanel, JSplitPane.BOTTOM);
      
      mLowerPanel.setLayout(new BorderLayout());
      mLowerPanel.add(mTabContainer, BorderLayout.CENTER);
      mLowerPanel.add(mAddPanel, BorderLayout.SOUTH);

      mAddScreenBtn.setText("Add Screen...");
      mAddScreenBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mAddScreenBtn_actionPerformed(e);
         }
      });
      
      mAddWallBtn.setText("Add Wall...");
      mAddWallBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mAddWallBtn_actionPerformed(e);
         }
      });
      
      mAddPanel.add( mAddScreenBtn );
      mAddPanel.add( mAddWallBtn );
      
      mScreenScrollPanel.getViewport().setLayout(new BorderLayout());
      mScreenScrollPanel.getViewport().add(mScreenDisplay, BorderLayout.CENTER);
   }
   
   void mAddScreenBtn_actionPerformed(ActionEvent e)
   {
      Frame parent =
         (Frame) SwingUtilities.getAncestorOfClass(Frame.class, this);

      String screen_name =
         JOptionPane.showInputDialog(parent,
                                     "Enter a name for the new screen",
                                     "New Screen Name",
                                     JOptionPane.QUESTION_MESSAGE);

      if ( screen_name != null )
      {
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         ConfigDefinition vp_def = broker.getRepository().get(EditorConstants.display_window_type);
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());
         ConfigElement elt = factory.create(screen_name, vp_def);

         // Make sure this add goes through successfully
         if (! broker.add(mContext, elt))
         {
            JOptionPane.showMessageDialog(SwingUtilities.getAncestorOfClass(Frame.class, this),
                                          "There are no configuration files active.",
                                          "Error",
                                          JOptionPane.ERROR_MESSAGE);
            return;
         }
      }
   }
   
   void mAddWallBtn_actionPerformed(ActionEvent e)
   {
      Frame parent =
         (Frame) SwingUtilities.getAncestorOfClass(Frame.class, this);

      String wall_name =
         JOptionPane.showInputDialog(parent,
                                     "Enter a name for the new wall",
                                     "New Wall Name",
                                     JOptionPane.QUESTION_MESSAGE);

      if ( wall_name != null )
      {
         CaveWall cw = new CaveWall(wall_name);
         TabData td = new TabData(cw, null, cw.getName(), cw.getName());
         mWallTabDataModel.addTab(mWallTabDataModel.size(), td);
         SingleSelectionModel ssm = mTabContainer.getSelectionModel();
         // Make sure that we select the newly created wall.
         ssm.setSelectedIndex( mWallTabDataModel.size() - 1 );
      }
   }
   
   public class WallComponentConverter implements ComponentConverter
   {
      private MainEditorPanel mMainEditorPanel = new MainEditorPanel();
      
      public Component getComponent(TabData data)
      {
         CaveWall cw = (CaveWall)data.getUserObject();
         mMainEditorPanel.setWallConfig(mCaveModel, cw);

         return mMainEditorPanel;
      }
   }
}
