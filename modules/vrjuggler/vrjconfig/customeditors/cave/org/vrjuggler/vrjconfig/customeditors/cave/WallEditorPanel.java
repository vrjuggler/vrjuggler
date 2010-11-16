/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
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

import java.awt.Color;

import org.vrjuggler.vrjconfig.customeditors.cave.event.*;

public class WallEditorPanel
   extends JPanel
   implements ChangeListener, EditorConstants
{
   private JScrollPane mWallListScrollPanel = new JScrollPane();
   
   private TableLayout mLowerPanelLayout = null;

   private Icon mAddIcon = null;
   private Icon mRemoveIcon = null;
   private Icon mDisabledRemoveIcon = null;
   private Icon mEditIcon = null;
   private Icon mDisabledEditIcon = null;

   private JButton mAddWallBtn = new JButton();
   private JButton mRemoveWallBtn = new JButton();
   private JButton mEditWallBtn = new JButton();
      
   private JList mWallList = new JList();
   private MainEditorPanel mMainEditorPanel = new MainEditorPanel();
      
   private List mWalls = new ArrayList();
   private ConfigContext mConfigContext = null;
   private CaveModel mCaveModel = null;
   private WallListModel mWallListModel = null;
   
   public void setConfig(ConfigContext ctx, CaveModel caveModel)
   {
      mConfigContext = ctx;

      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }
      
      mCaveModel = caveModel;
      //mCaveModel = new CaveModel(mConfigContext);
      mWallListModel = new WallListModel(mCaveModel);
      mCaveModel.addCaveModelListener(mWallListModel);

      mWallList.setModel(mWallListModel);
      mWallList.setCellRenderer(new PlaneRenderer());
      mWallList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      mWallList.addListSelectionListener(new ListSelectionListener()
         {
            public void valueChanged(ListSelectionEvent e)
            {
               if (e.getValueIsAdjusting() == false)
               {
                  if (mWallList.getSelectedIndex() == -1)
                  {
                     mRemoveWallBtn.setEnabled(false);
                     mEditWallBtn.setEnabled(false);
                  }
                  else
                  {
                     mMainEditorPanel.setWallConfig(mCaveModel, (CaveWall)mWallList.getSelectedValue());
                     mRemoveWallBtn.setEnabled(true);
                     mEditWallBtn.setEnabled(true);
                  }
               }
            }
         });

      // TODO: If there is atleast one wall, select it. Otherwise default to an empty panel.
      if (mCaveModel.getWalls().size() > 0)
      {
         mMainEditorPanel.setWallConfig(mCaveModel, (CaveWall)(mCaveModel.getWalls().get(0)));
      }

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

   public WallEditorPanel()
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
      double[][] lower_size = {{TableLayout.FILL, 25, TableLayout.PREFERRED},
                              {25, 25, 25, TableLayout.PREFERRED}};
      mLowerPanelLayout = new TableLayout(lower_size);
      
      this.setLayout(mLowerPanelLayout);
      
      this.add(mWallListScrollPanel,
                             new TableLayoutConstraints(0, 0, 0, 3,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      this.add(mAddWallBtn,
                             new TableLayoutConstraints(1, 0, 1, 0,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      this.add(mRemoveWallBtn,
                             new TableLayoutConstraints(1, 1, 1, 1,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      this.add(mEditWallBtn,
                             new TableLayoutConstraints(1, 2, 1, 2,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      this.add(mMainEditorPanel,
                             new TableLayoutConstraints(2, 0, 2, 3,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      
      mWallListScrollPanel.getViewport().setLayout(new BorderLayout());
      mWallListScrollPanel.getViewport().add(mWallList, BorderLayout.CENTER);
      
      /*
      mAddScreenBtn.setText("Add Screen...");
      mAddScreenBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mAddScreenBtn_actionPerformed(e);
         }
      });
      */

      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         mAddIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/add.gif"));
         mRemoveIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/remove.gif"));
         mDisabledRemoveIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/remove_disabled.gif"));
         mEditIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/edit.gif"));
         mDisabledEditIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/edit_disabled.gif"));
         
         mAddWallBtn.setIcon(mAddIcon);
         mRemoveWallBtn.setIcon(mRemoveIcon);
         mRemoveWallBtn.setDisabledIcon(mDisabledRemoveIcon);
         mEditWallBtn.setIcon(mEditIcon);
         mEditWallBtn.setDisabledIcon(mDisabledEditIcon);
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         mAddWallBtn.setText("Add");
         mRemoveWallBtn.setText("Remove");
         mEditWallBtn.setText("Edit");
      }

      mAddWallBtn.setToolTipText("Configure a new wall.");
      mAddWallBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mAddWallBtn_actionPerformed(e);
         }
      });
      
      mRemoveWallBtn.setEnabled(false);
      mRemoveWallBtn.setToolTipText("Remove the currently selected wall.");
      mRemoveWallBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mRemoveWallBtn_actionPerformed(e);
         }
      });
      
      mEditWallBtn.setEnabled(false);
      mEditWallBtn.setToolTipText("Edit the currently selected wall.");
      mEditWallBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            mEditWallBtn_actionPerformed(e);
         }
      });
   }
   
   void mAddWallBtn_actionPerformed(ActionEvent e)
   {
      /*
      Frame parent =
         (Frame) SwingUtilities.getAncestorOfClass(Frame.class, this);

      String wall_name =
         JOptionPane.showInputDialog(parent,
                                     "Enter a name for the new wall",
                                     "New Wall Name",
                                     JOptionPane.QUESTION_MESSAGE);
      */
      //if ( wall_name != null )
      //{
         CaveWall cw = mCaveModel.makeNewWall("");
         mWallList.setSelectedValue(cw, true);
         
         int status = editWall(cw);

         // If the user cancels while editing, then we want to remove
         // the new wall that we just created.
         if ( status != DisplayWindowStartDialog.OK_OPTION )
         {
            mCaveModel.removeWall(cw);
         }
      //}
   }
   
   void mRemoveWallBtn_actionPerformed(ActionEvent e)
   {
      CaveWall cw = (CaveWall)mWallList.getSelectedValue();
      if (null != cw)
      {
         mCaveModel.removeWall(cw);
      }
   }
   
   void mEditWallBtn_actionPerformed(ActionEvent e)
   {
      Container owner = (Container) SwingUtilities.getRoot(this);
      /*
      float old_origin_x =
         ((Number) mSelectedViewport.getProperty(ORIGIN_PROPERTY, 0)).floatValue();
      float old_origin_y =
         ((Number) mSelectedViewport.getProperty(ORIGIN_PROPERTY, 1)).floatValue();
      float old_width =
         ((Number) mSelectedViewport.getProperty(SIZE_PROPERTY, 0)).floatValue();
      float old_height =
         ((Number) mSelectedViewport.getProperty(SIZE_PROPERTY, 1)).floatValue();
      */
      //float origin_x = 0.0f, origin_y = 0.0f, width = 0.0f, height = 0.0f;

      Object value = mWallList.getSelectedValue();
      
      if (null == value || !(value instanceof CaveWall))
      {
         return;
      }

      CaveWall selected_wall = (CaveWall)value;

      editWall(selected_wall);
   }
   
   int editWall(CaveWall selected_wall)
   {
      int status;

      Container parent =
         (Container) SwingUtilities.getAncestorOfClass(Container.class,
                                                       this);
      WallEditorDialog dlg =
         new WallEditorDialog(parent, mConfigContext, selected_wall);

      status = dlg.showDialog();
      if ( status == DisplayWindowStartDialog.OK_OPTION )
      {
         selected_wall.setName(dlg.getWallName());
         Point3D[] corners = dlg.getCorners();
         selected_wall.setCorners(corners, mConfigContext);
         selected_wall.setTracked(dlg.isTracked(), mConfigContext);
         selected_wall.setUser(dlg.getUser(), mConfigContext);
         //selected_wall.setPlane(dlg.getPlane());
         selected_wall.updateWidthHeight();

         if ( dlg.isTracked() == Boolean.TRUE )
         {
            selected_wall.setTrackerProxy(dlg.getTrackerProxy(), mConfigContext);
         }
      }

      return status;
   }
   
   public class WallListModel
      extends AbstractListModel
      implements CaveModelListener
   {
      private CaveModel mCaveModel = null;
       
      public WallListModel( CaveModel cm )
      {
         mCaveModel = cm;
      }
      
      public void wallAdded(CaveModelEvent evt)
      {
         fireIntervalAdded(evt.getSource(), evt.getIndex(), evt.getIndex());
      }
      
      public void wallRemoved(CaveModelEvent evt)
      {
         fireIntervalRemoved(evt.getSource(), evt.getIndex(), evt.getIndex());
      }

      public void screenAdded(CaveModelEvent evt)
      {;}
      
      public void screenRemoved(CaveModelEvent evt)
      {;}
      
      public Object getElementAt(int index)
      {
         if ( index >= 0 && index < mCaveModel.getWalls().size() )
         {
            return mCaveModel.getWalls().get(index);
         }
         else
         {
            return null;
         }
      }

      public int getSize()
      {
         return mCaveModel.getWalls().size();
      }
   }

   public class PlaneRenderer
      extends JLabel
      implements ListCellRenderer
   {
      private String[]    mPlaneLabels = {"Front", "Back", "Left", "Right", "Bottom", "Top", "Custom"};
      private Integer[]   mIntArray    = new Integer[mPlaneLabels.length];
      private ImageIcon[] mPlaneImages = new ImageIcon[mPlaneLabels.length];

      public PlaneRenderer()
      {
         setOpaque(true);
         setHorizontalAlignment(LEFT);
         setVerticalAlignment(CENTER);
         initUI();
      }
      
      protected void initUI()
      {
         ClassLoader loader = getClass().getClassLoader();
         
         String resourceBase = "org/vrjuggler/vrjconfig/customeditors/cave";
         String img_base = resourceBase + "/images";

         try
         {
            mPlaneImages[0] = new ImageIcon(loader.getResource(img_base + "/front-plane-icon.png"));
            mPlaneImages[1] = new ImageIcon(loader.getResource(img_base + "/back-plane-icon.png"));
            mPlaneImages[2] = new ImageIcon(loader.getResource(img_base + "/left-plane-icon.png"));
            mPlaneImages[3] = new ImageIcon(loader.getResource(img_base + "/right-plane-icon.png"));
            mPlaneImages[4] = new ImageIcon(loader.getResource(img_base + "/bottom-plane-icon.png"));
            mPlaneImages[5] = new ImageIcon(loader.getResource(img_base + "/top-plane-icon.png"));
            mPlaneImages[6] = new ImageIcon(loader.getResource(img_base + "/custom-plane-icon.png"));
         }
         catch (NullPointerException ex)
         {
            System.out.println("mPlaneLabels: " + mPlaneLabels);
            for ( int i = 0; i < mPlaneLabels.length; ++i )
            {
               mPlaneImages[i] = null;
            }
         }
      }

      public Component getListCellRendererComponent(JList list, Object value,
                                                    int index,
                                                    boolean isSelected,
                                                    boolean cellHasFocus)
      {
         CaveWall wall = (CaveWall)value;
         int selected_index = wall.getPlane();

         if ( isSelected )
         {
            setBackground(list.getSelectionBackground());
            setForeground(list.getSelectionForeground());
         }
         else
         {
            setBackground(list.getBackground());
            setForeground(list.getForeground());
         }

         ImageIcon icon = mPlaneImages[selected_index];
         String name   = wall.getName();
         setIcon(icon);
         setText(name);
         setFont(list.getFont());

         return this;
      }
   }
}
