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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.customeditors.display_window;

import java.awt.*;
import java.awt.event.*;
import java.beans.*;
import java.util.Iterator;
import javax.swing.*;
import javax.swing.border.*;
import info.clearthought.layout.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;


public class SurfaceViewportCreateDialog
   extends ViewportCreateDialog
   implements EditorConstants
{
   public SurfaceViewportCreateDialog(Container parent, ConfigContext ctx)
   {
      this(parent, ctx, null);
   }

   public SurfaceViewportCreateDialog(Container parent, ConfigContext ctx,
                                      ConfigElement elt)
   {
      super("Basic Surface Viewport Parameters", ctx, elt,
            SURFACE_VIEWPORT_TYPE);

      mCorners[Plane.LL_CORNER] = "Lower Left Corner";
      mCorners[Plane.LR_CORNER] = "Lower Right Corner";
      mCorners[Plane.UR_CORNER] = "Upper Right Corner";
      mCorners[Plane.UL_CORNER] = "Upper Left Corner";

      ConfigBrokerProxy broker = new ConfigBrokerProxy();
      ConfigDefinition vp_def =
         broker.getRepository().get(SURFACE_VIEWPORT_TYPE);

      mTrackerProxyEditor =
         new PropertyEditorPanel(
            ctx, mViewportElement.getProperty(TRACKER_PROXY_PROPERTY, 0),
            vp_def.getPropertyDefinition(TRACKER_PROXY_PROPERTY),
            mViewportElement, 0, Color.white
         );

      initUI();

      mUnitsList.add(new UnitConversion("Meters", 1.0));
      mUnitsList.add(new UnitConversion("Centimeters", 0.01));
      mUnitsList.add(new UnitConversion("Feet", 0.30487804878048780487));
      mUnitsList.add(new UnitConversion("Inches", 0.02540005080010160020));

      for ( Iterator i = mUnitsList.iterator(); i.hasNext(); )
      {
         mUnitsComboBox.addItem(((UnitConversion) i.next()).name);
      }

      // If we were given nothing for default input values, then we set up
      // the physical units to be in feet for general convenience (at least
      // for those of us in the United States).
      if ( elt == null )
      {
         mUnitsComboBox.setSelectedIndex(2);
      }
      // For input coming in through a config element, all units will be in
      // meters.  We will not attempt to guess what units the user would
      // prefer.
      else
      {
         mUnitsComboBox.setSelectedIndex(0);

         float[] ll_corner = new float[3], lr_corner = new float[3],
                 ul_corner = new float[3];
         ll_corner[0] =
            ((Number) mViewportElement.getProperty(LOWER_LEFT_CORNER_PROPERTY, 0)).floatValue();
         ll_corner[1] =
            ((Number) mViewportElement.getProperty(LOWER_LEFT_CORNER_PROPERTY, 1)).floatValue();
         ll_corner[2] =
            ((Number) mViewportElement.getProperty(LOWER_LEFT_CORNER_PROPERTY, 2)).floatValue();
         lr_corner[0] =
            ((Number) mViewportElement.getProperty(LOWER_RIGHT_CORNER_PROPERTY, 0)).floatValue();
         lr_corner[1] =
            ((Number) mViewportElement.getProperty(LOWER_RIGHT_CORNER_PROPERTY, 1)).floatValue();
         lr_corner[2] =
            ((Number) mViewportElement.getProperty(LOWER_RIGHT_CORNER_PROPERTY, 2)).floatValue();
         ul_corner[0] =
            ((Number) mViewportElement.getProperty(UPPER_LEFT_CORNER_PROPERTY, 0)).floatValue();
         ul_corner[1] =
            ((Number) mViewportElement.getProperty(UPPER_LEFT_CORNER_PROPERTY, 1)).floatValue();
         ul_corner[2] =
            ((Number) mViewportElement.getProperty(UPPER_LEFT_CORNER_PROPERTY, 2)).floatValue();

         float wall_width, wall_height;

         if ( ll_corner[0] == lr_corner[0] )
         {
            mPlaneChooser.setSelectedIndex(BACK_PLANE);
            wall_width  = ll_corner[2] - lr_corner[2];
            wall_height = ul_corner[1] - ll_corner[1];
         }
         else if ( ll_corner[2] == lr_corner[2] )
         {
            mPlaneChooser.setSelectedIndex(FRONT_PLANE);
            wall_width  = lr_corner[0] - ll_corner[0];
            wall_height = ul_corner[1] - ll_corner[1];
         }
         else if ( ll_corner[1] == ul_corner [1] )
         {
            mPlaneChooser.setSelectedIndex(LEFT_PLANE);
            wall_width  = lr_corner[0] - ll_corner[0];
            wall_height = ll_corner[2] - lr_corner[2];
         }
         else
         {
            mPlaneChooser.setSelectedIndex(CUSTOM_PLANE);

            float x_diff = lr_corner[0] - ll_corner[0];
            float y_diff = lr_corner[1] - ll_corner[1];
            float z_diff = lr_corner[2] - ll_corner[2];
            float len_sq = x_diff * x_diff + y_diff * y_diff + z_diff * z_diff;
            wall_width = (float) Math.sqrt(len_sq);

            x_diff = ul_corner[0] - ll_corner[0];
            y_diff = ul_corner[1] - ll_corner[1];
            z_diff = ul_corner[2] - ll_corner[2];
            float width_sq = x_diff * x_diff + y_diff * y_diff + z_diff * z_diff;
            wall_height = (float) Math.sqrt(width_sq);

            // XXX: At this point, there needs to be code that figures out the
            // rotational angles for the custom-defined plane.  Those angles
            // then need to go into the text fields.
         }

         mCornerXField.setValue(new Float(ll_corner[0]));
         mCornerYField.setValue(new Float(ll_corner[1]));
         mCornerZField.setValue(new Float(ll_corner[2]));
         mWallWidthField.setValue(new Float(wall_width));
         mWallHeightField.setValue(new Float(wall_height));
      }

      validateUserInput();
      this.pack();
      this.setLocationRelativeTo(parent);
   }

   public Boolean isTracked()
   {
      return (mTracked ? Boolean.TRUE : Boolean.FALSE);
   }

   public Object getTrackerProxy()
   {
      return mViewportElement.getProperty(TRACKER_PROXY_PROPERTY, 1);
   }

   public Point3D[] getCorners()
   {
      float wall_width = ((Number) mWallWidthField.getValue()).floatValue();
      float wall_height = ((Number) mWallHeightField.getValue()).floatValue();

      Plane surface_plane = new Plane(wall_width, wall_height);
      float[] orientation = getSelectedOrientation();
      surface_plane.setOrientation(orientation[0], orientation[1],
                                   orientation[2]);

      // Pull the value for the corner that the user entered.  This information
      // will be used to reposition surface_plane relative to the selected
      // corner.
      float corner_x = ((Number) mCornerXField.getValue()).floatValue();
      float corner_y = ((Number) mCornerYField.getValue()).floatValue();
      float corner_z = ((Number) mCornerZField.getValue()).floatValue();
      surface_plane.setCorner(new Point3D(corner_x, corner_y, corner_z),
                              mCornerChooser.getSelectedIndex());

      // Before returning, scale the corners by the unit conversion factor
      // chosen by the user.
      Point3D[] corners = surface_plane.getCorners();
      Point3D[] scaled_corners = new Point3D[corners.length];
      for ( int i = 0; i < corners.length; ++i )
      {
         scaled_corners[i] = new Point3D(corners[i]);
         scaled_corners[i].scale(getUnitConversionFactor());
//         System.out.println(corners[i]);
      }

      return scaled_corners;
   }

   public double getUnitConversionFactor()
   {
      return mConversionFactor.doubleValue();
   }

   protected void initUI()
   {
      super.initUI();

      mCornerChooser = new JComboBox(mCorners);
      mCornerChooser.setSelectedIndex(Plane.LL_CORNER);

      double[][] main_size = {{TableLayout.PREFERRED},
                              {TableLayout.PREFERRED, TableLayout.PREFERRED,
                               TableLayout.PREFERRED}};

      mMainPanelLayout = new TableLayout(main_size);

      double[][] phys_size = {{TableLayout.PREFERRED, 10, TableLayout.PREFERRED},
                              {TableLayout.PREFERRED, 5, TableLayout.PREFERRED,
                               5, TableLayout.PREFERRED, TableLayout.PREFERRED,
                               TableLayout.PREFERRED, TableLayout.PREFERRED}};

      mPhysicalCharPanelLayout = new TableLayout(phys_size);

      double[][] corner_size = {{TableLayout.FILL},
                                {TableLayout.PREFERRED, 5,
                                 TableLayout.PREFERRED}};
      mCornerPanelLayout = new TableLayout(corner_size);

      ClassLoader loader = getClass().getClassLoader();
      String img_base = IMAGE_BASE;

      try
      {
         mPlaneImages[FRONT_PLANE] =
            new ImageIcon(loader.getResource(img_base +
                                             "/front-plane-icon.png"));
         mPlaneImages[BACK_PLANE] =
            new ImageIcon(loader.getResource(img_base +
                                             "/back-plane-icon.png"));
         mPlaneImages[LEFT_PLANE] =
            new ImageIcon(loader.getResource(img_base +
                                             "/left-plane-icon.png"));
         mPlaneImages[RIGHT_PLANE] =
            new ImageIcon(loader.getResource(img_base +
                                             "/right-plane-icon.png"));
         mPlaneImages[BOTTOM_PLANE] =
            new ImageIcon(loader.getResource(img_base +
                                             "/bottom-plane-icon.png"));
         mPlaneImages[TOP_PLANE] =
            new ImageIcon(loader.getResource(img_base +
                                             "/top-plane-icon.png"));
         mPlaneImages[CUSTOM_PLANE] =
            new ImageIcon(loader.getResource(img_base +
                                             "/custom-plane-icon.png"));
      }
      catch (NullPointerException ex)
      {
         System.out.println("mPlaneLabels: " + mPlaneLabels);
         for ( int i = 0; i < mPlaneLabels.length; ++i )
         {
            mPlaneImages[i] = null;
         }
      }

      for ( int i = 0; i < mPlaneLabels.length; ++i )
      {
         mIntArray[i] = new Integer(i);
      }

      mPlaneChooser = new JComboBox(mIntArray);

      try
      {
         jbInit();
         mCustomPlaneXField.setValue(new Float(0.0));
         mCustomPlaneYField.setValue(new Float(0.0));
         mCustomPlaneZField.setValue(new Float(0.0));
         mWallWidthField.setValue(new Float(0.0));
         mWallHeightField.setValue(new Float(0.0));
         mPlaneChooser.setRenderer(new PlaneRenderer());
         mCornerXField.setValue(new Float(0.0));
         mCornerYField.setValue(new Float(0.0));
         mCornerZField.setValue(new Float(0.0));
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      try
      {
         mCornerIcons[FRONT_PLANE][Plane.LL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/front-plane-ll.png"));
         mCornerIcons[FRONT_PLANE][Plane.LR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/front-plane-lr.png"));
         mCornerIcons[FRONT_PLANE][Plane.UR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/front-plane-ur.png"));
         mCornerIcons[FRONT_PLANE][Plane.UL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/front-plane-ul.png"));
         mCornerIcons[BACK_PLANE][Plane.LL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/back-plane-ll.png"));
         mCornerIcons[BACK_PLANE][Plane.LR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/back-plane-lr.png"));
         mCornerIcons[BACK_PLANE][Plane.UR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/back-plane-ur.png"));
         mCornerIcons[BACK_PLANE][Plane.UL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/back-plane-ul.png"));
         mCornerIcons[LEFT_PLANE][Plane.LL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/left-plane-ll.png"));
         mCornerIcons[LEFT_PLANE][Plane.LR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/left-plane-lr.png"));
         mCornerIcons[LEFT_PLANE][Plane.UR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/left-plane-ur.png"));
         mCornerIcons[LEFT_PLANE][Plane.UL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/left-plane-ul.png"));
         mCornerIcons[RIGHT_PLANE][Plane.LL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/right-plane-ll.png"));
         mCornerIcons[RIGHT_PLANE][Plane.LR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/right-plane-lr.png"));
         mCornerIcons[RIGHT_PLANE][Plane.UR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/right-plane-ur.png"));
         mCornerIcons[RIGHT_PLANE][Plane.UL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/right-plane-ul.png"));
         mCornerIcons[BOTTOM_PLANE][Plane.LL_CORNER] =
            new ImageIcon(loader.getResource(img_base +
                                             "/bottom-plane-ll.png"));
         mCornerIcons[BOTTOM_PLANE][Plane.LR_CORNER] =
            new ImageIcon(loader.getResource(img_base +
                                             "/bottom-plane-lr.png"));
         mCornerIcons[BOTTOM_PLANE][Plane.UR_CORNER] =
            new ImageIcon(loader.getResource(img_base +
                                             "/bottom-plane-ur.png"));
         mCornerIcons[BOTTOM_PLANE][Plane.UL_CORNER] =
            new ImageIcon(loader.getResource(img_base +
                                             "/bottom-plane-ul.png"));
         mCornerIcons[TOP_PLANE][Plane.LL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/top-plane-ll.png"));
         mCornerIcons[TOP_PLANE][Plane.LR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/top-plane-lr.png"));
         mCornerIcons[TOP_PLANE][Plane.UR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/top-plane-ur.png"));
         mCornerIcons[TOP_PLANE][Plane.UL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/top-plane-ul.png"));
         mCornerIcons[CUSTOM_PLANE][Plane.LL_CORNER] =
            new ImageIcon(loader.getResource(img_base +
                                             "/custom-plane-ll.png"));
         mCornerIcons[CUSTOM_PLANE][Plane.LR_CORNER] =
            new ImageIcon(loader.getResource(img_base +
                                             "/custom-plane-lr.png"));
         mCornerIcons[CUSTOM_PLANE][Plane.UR_CORNER] =
            new ImageIcon(loader.getResource(img_base +
                                             "/custom-plane-ur.png"));
         mCornerIcons[CUSTOM_PLANE][Plane.UL_CORNER] =
            new ImageIcon(loader.getResource(img_base +
                                             "/custom-plane-ul.png"));
         mCornerLabel.setIcon(
            mCornerIcons[mPlaneChooser.getSelectedIndex()][mCornerChooser.getSelectedIndex()]
         );
      }
      catch(NullPointerException ex)
      {
         for ( int i = 0; i < 4; ++i )
         {
            for ( int j = 0; j < 4; ++j )
            {
               mCornerIcons[i][j] = null;
            }
         }
      }
   }

   private void jbInit() throws Exception
   {
      mBoundsPanelBorder = new TitledBorder(BorderFactory.createEtchedBorder(Color.white,new Color(142, 142, 142)),"Viewport Bounds");
      mUserPanelBorder = new TitledBorder(BorderFactory.createEtchedBorder(Color.white,new Color(142, 142, 142)),"User Settings");
      mBoundsPanel.setBorder(mBoundsPanelBorder);
      mUserPanel.setBorder(mUserPanelBorder);
      mPhysicalCharPanelBorder = new TitledBorder(BorderFactory.createEtchedBorder(Color.white,new Color(142, 142, 142)),"Physical Surface Characteristics");
      mMainPanel.setLayout(mMainPanelLayout);
      mPhysicalCharPanel.setBorder(mPhysicalCharPanelBorder);
      mPhysicalCharPanel.setLayout(mPhysicalCharPanelLayout);
      mCornerPanel.setLayout(mCornerPanelLayout);
      mUnitsLabel.setLabelFor(mUnitsComboBox);
      mUnitsLabel.setText("Physical Units");
      mPlaneLabel.setLabelFor(mPlanePanel);
      mPlaneLabel.setText("Plane of the Surface");
      mCustomPlaneXLabel.setEnabled(false);
      mCustomPlaneXLabel.setLabelFor(mCustomPlaneXField);
      mCustomPlaneXLabel.setText("X");
      mCustomPlaneXField.setEnabled(false);
      mCustomPlaneXField.setMinimumSize(new Dimension(40, 19));
      mCustomPlaneXField.setPreferredSize(new Dimension(45, 19));
      mCustomPlaneXField.setToolTipText("Rotation about the X-axis in degrees");
      mCustomPlaneXField.setHorizontalAlignment(SwingConstants.TRAILING);
      mCustomPlaneYLabel.setEnabled(false);
      mCustomPlaneYLabel.setLabelFor(mCustomPlaneYField);
      mCustomPlaneYLabel.setText("Y");
      mCustomPlaneYField.setEnabled(false);
      mCustomPlaneYField.setMinimumSize(new Dimension(40, 19));
      mCustomPlaneYField.setPreferredSize(new Dimension(45, 19));
      mCustomPlaneYField.setToolTipText("Rotation about the Y-axis in degrees");
      mCustomPlaneYField.setHorizontalAlignment(SwingConstants.TRAILING);
      mCustomPlaneZLabel.setEnabled(false);
      mCustomPlaneZLabel.setLabelFor(mCustomPlaneZField);
      mCustomPlaneZLabel.setText("Z");
      mCustomPlaneZField.setEnabled(false);
      mCustomPlaneZField.setMinimumSize(new Dimension(40, 19));
      mCustomPlaneZField.setPreferredSize(new Dimension(45, 19));
      mCustomPlaneZField.setToolTipText("Rotation about the Z-axis in degrees");
      mCustomPlaneZField.setHorizontalAlignment(SwingConstants.TRAILING);
      jLabel1.setText("(");
      mCornerXField.setMinimumSize(new Dimension(35, 19));
      mCornerXField.setPreferredSize(new Dimension(60, 19));
      mCornerXField.setToolTipText("X-coordinate for the surface corner");
      mCornerXField.setHorizontalAlignment(SwingConstants.TRAILING);
      jLabel2.setText(",");
      mCornerYField.setMinimumSize(new Dimension(35, 19));
      mCornerYField.setPreferredSize(new Dimension(60, 19));
      mCornerYField.setToolTipText("Y-coordinate for the surface corner");
      mCornerYField.setHorizontalAlignment(SwingConstants.TRAILING);
      jLabel3.setText(",");
      mCornerZField.setMinimumSize(new Dimension(35, 19));
      mCornerZField.setPreferredSize(new Dimension(60, 19));
      mCornerZField.setToolTipText("Z-coordinate for the surface corner");
      mCornerZField.setHorizontalAlignment(SwingConstants.TRAILING);
      jLabel4.setText(")");
      mDimensionsLabel.setLabelFor(mDimensionsPanel);
      mDimensionsLabel.setText("Surface Dimensions");
      mWallWidthField.setMinimumSize(new Dimension(40, 19));
      mWallWidthField.setPreferredSize(new Dimension(60, 19));
      mWallWidthField.addPropertyChangeListener(new SurfaceViewportCreateDialog_mWallWidthField_propertyChangeAdapter(this));
      mWallWidthField.setHorizontalAlignment(SwingConstants.TRAILING);
      mDimensionsXLabel.setText("\u00d7");
      mWallHeightField.setMinimumSize(new Dimension(40, 19));
      mWallHeightField.setPreferredSize(new Dimension(60, 19));
      mWallHeightField.addPropertyChangeListener(new SurfaceViewportCreateDialog_mWallHeightField_propertyChangeAdapter(this));
      mCornerLabel.setHorizontalTextPosition(SwingConstants.LEADING);
      mCornerLabel.setLabelFor(mCornerPanel);
      mCornerLabel.setText("Set Single Corner");
      mCornerLabel.setVerticalTextPosition(SwingConstants.TOP);
      mUnitsComboBox.addActionListener(new SurfaceViewportCreateDialog_mUnitsComboBox_actionAdapter(this));
      mPlaneChooser.addActionListener(new SurfaceViewportCreateDialog_mPlaneChooser_actionAdapter(this));
      mUnitsComboBox.setToolTipText("The units of measurement used for physical locations to be entered below");
      mPlaneChooser.setToolTipText("The physical plane along which the surface is aligned");
      mSurfaceTypeLabel.setLabelFor(mSurfaceTypeChooser);
      mSurfaceTypeLabel.setText("Surface Type");
      mSurfaceTypeChooser.addActionListener(new SurfaceViewportCreateDialog_mSurfaceTypeChooser_actionAdapter(this));
      mTrackerProxyLabel.setEnabled(false);
      mTrackerProxyLabel.setLabelFor(mTrackerProxyEditor);
      mTrackerProxyLabel.setText("Surface Tracker Proxy");
      mTrackerProxyEditor.setEnabled(false);
      mCornerChooser.addActionListener(new SurfaceViewportCreateDialog_mCornerChooser_actionAdapter(this));
      mMainPanel.add(mBoundsPanel,
                     new TableLayoutConstraints(0, 0, 0, 0,
                                                TableLayout.FULL,
                                                TableLayout.FULL));
      mMainPanel.add(mUserPanel,
                     new TableLayoutConstraints(0, 1, 0, 1,
                                                TableLayout.FULL,
                                                TableLayout.FULL));
      mMainPanel.add(mPhysicalCharPanel,
                     new TableLayoutConstraints(0, 2, 0, 2,
                                                TableLayout.FULL,
                                                TableLayout.FULL));
      mPhysicalCharPanel.add(mSurfaceTypeLabel,
                             new TableLayoutConstraints(0, 0, 0, 0,
                                                        TableLayout.LEFT,
                                                        TableLayout.FULL));
      mPhysicalCharPanel.add(mSurfaceTypeChooser,
                             new TableLayoutConstraints(2, 0, 2, 0,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mPhysicalCharPanel.add(mTrackerProxyLabel,
                             new TableLayoutConstraints(0, 2, 0, 2,
                                                        TableLayout.LEFT,
                                                        TableLayout.FULL));
      mPhysicalCharPanel.add(mTrackerProxyEditor,
                             new TableLayoutConstraints(2, 2, 2, 2,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mPhysicalCharPanel.add(mUnitsLabel,
                             new TableLayoutConstraints(0, 4, 0, 4,
                                                        TableLayout.LEFT,
                                                        TableLayout.FULL));
      mPhysicalCharPanel.add(mUnitsComboBox,
                             new TableLayoutConstraints(2, 4, 2, 4,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mPhysicalCharPanel.add(mPlaneLabel,
                             new TableLayoutConstraints(0, 5, 0, 5,
                                                        TableLayout.LEFT,
                                                        TableLayout.FULL));
      mPhysicalCharPanel.add(mPlanePanel,
                             new TableLayoutConstraints(2, 5, 2, 5,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mPhysicalCharPanel.add(mCornerLabel,
                             new TableLayoutConstraints(0, 6, 0, 6,
                                                        TableLayout.LEFT,
                                                        TableLayout.FULL));
      mPhysicalCharPanel.add(mCornerPanel,
                             new TableLayoutConstraints(2, 6, 2, 6,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mPhysicalCharPanel.add(mDimensionsLabel,
                             new TableLayoutConstraints(0, 7, 0, 7,
                                                        TableLayout.LEFT,
                                                        TableLayout.FULL));
      mPhysicalCharPanel.add(mDimensionsPanel,
                             new TableLayoutConstraints(2, 7, 2, 7,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mCornerPanel.add(mCornerChooser,
                       new TableLayoutConstraints(0, 0, 0, 0, TableLayout.FULL,
                                                  TableLayout.FULL));
      mCornerPanel.add(mCustomCornerPanel,
                       new TableLayoutConstraints(0, 2, 0, 2, TableLayout.FULL,
                                                  TableLayout.FULL));
      mPlanePanel.add(mPlaneChooser, null);
      mPlanePanel.add(mCustomPlaneXLabel, null);
      mPlanePanel.add(mCustomPlaneXField, null);
      mPlanePanel.add(mCustomPlaneYLabel, null);
      mPlanePanel.add(mCustomPlaneYField, null);
      mPlanePanel.add(mCustomPlaneZLabel, null);
      mPlanePanel.add(mCustomPlaneZField, null);
      mCustomCornerPanel.add(jLabel1, null);
      mCustomCornerPanel.add(mCornerXField, null);
      mCustomCornerPanel.add(jLabel2, null);
      mCustomCornerPanel.add(mCornerYField, null);
      mCustomCornerPanel.add(jLabel3, null);
      mCustomCornerPanel.add(mCornerZField, null);
      mCustomCornerPanel.add(jLabel4, null);
      mDimensionsPanel.add(mWallWidthField, null);
      mDimensionsPanel.add(mDimensionsXLabel, null);
      mDimensionsPanel.add(mWallHeightField, null);
   }

   protected boolean validateCustomInput()
   {
      boolean wall_width_set = false, wall_height_set = false, tracker_set;

      if ( mWallWidthField.getValue() != null )
      {
         wall_width_set = ((Number) mWallWidthField.getValue()).floatValue() > 0.0;
      }

      if ( mWallHeightField.getValue() != null )
      {
         wall_height_set = ((Number) mWallHeightField.getValue()).floatValue() > 0.0;
      }

      if ( mTracked )
      {
         tracker_set = (((ConfigElementPointer) getTrackerProxy()).getTarget() != null);
      }
      else
      {
         tracker_set = true;
      }

      return wall_width_set && wall_height_set && tracker_set;
   }

   private float[] getSelectedOrientation()
   {
      float[] orientation = new float[3];

      if ( mPlaneChooser.getSelectedIndex() == FRONT_PLANE )
      {
         orientation[0] = 0.0f;
         orientation[1] = 0.0f;
         orientation[2] = 0.0f;
      }
      else if ( mPlaneChooser.getSelectedIndex() == BACK_PLANE )
      {
         orientation[0] = 0.0f;
         orientation[1] = 180.0f;
         orientation[2] = 0.0f;
      }
      else if ( mPlaneChooser.getSelectedIndex() == LEFT_PLANE )
      {
         orientation[0] = 0.0f;
         orientation[1] = 90.0f;
         orientation[2] = 0.0f;
      }
      else if ( mPlaneChooser.getSelectedIndex() == RIGHT_PLANE )
      {
         orientation[0] = 0.0f;
         orientation[1] = -90.0f;
         orientation[2] = 0.0f;
      }
      else if ( mPlaneChooser.getSelectedIndex() == BOTTOM_PLANE )
      {
         orientation[0] = -90.0f;
         orientation[1] = 0.0f;
         orientation[2] = 0.0f;
      }
      else if ( mPlaneChooser.getSelectedIndex() == BOTTOM_PLANE )
      {
         orientation[0] = 90.0f;
         orientation[1] = 0.0f;
         orientation[2] = 0.0f;
      }
      else
      {
         orientation[0] = ((Number) mCustomPlaneXField.getValue()).floatValue();
         orientation[1] = ((Number) mCustomPlaneYField.getValue()).floatValue();
         orientation[2] = ((Number) mCustomPlaneZField.getValue()).floatValue();
      }

      return orientation;
   }

   private void changeCornerIcon()
   {
      int plane = mPlaneChooser.getSelectedIndex();
      int corner = mCornerChooser.getSelectedIndex();
      if ( mCornerIcons[plane][corner] != null )
      {
         mCornerLabel.setIcon(mCornerIcons[plane][corner]);
      }
   }

   private static final int FRONT_PLANE  = 0;
   private static final int BACK_PLANE   = 1;
   private static final int LEFT_PLANE   = 2;
   private static final int RIGHT_PLANE  = 3;
   private static final int BOTTOM_PLANE = 4;
   private static final int TOP_PLANE    = 5;
   private static final int CUSTOM_PLANE = 6;

   private java.util.List mUnitsList = new java.util.ArrayList();
   private Double mConversionFactor = null;

   private String[]    mPlaneLabels = {"Front", "Back", "Left", "Right",
                                       "Bottom", "Top", "Custom"};
   private Integer[]   mIntArray    = new Integer[mPlaneLabels.length];
   private ImageIcon[] mPlaneImages = new ImageIcon[mPlaneLabels.length];

   private boolean  mTracked      = false;
   private String[] mSurfaceTypes = {"Fixed-Position Wall/Desk",
                                     "Adjustable Desk/Table", "HMD Eye"};

   private String[] mCorners = new String[4];

   private ImageIcon[][] mCornerIcons = new ImageIcon[mPlaneLabels.length][4];

   private TitledBorder mBoundsPanelBorder;
   private TitledBorder mUserPanelBorder;
   private JPanel mPhysicalCharPanel = new JPanel();
   private TableLayout mPhysicalCharPanelLayout = null;
   private TitledBorder mPhysicalCharPanelBorder;
   private JComboBox mUnitsComboBox = new JComboBox();
   private JLabel mUnitsLabel = new JLabel();
   private JLabel mPlaneLabel = new JLabel();
   private JPanel mPlanePanel = new JPanel();
   private JComboBox mPlaneChooser;
   private JLabel mCustomPlaneXLabel = new JLabel();
   private JFormattedTextField mCustomPlaneXField = new JFormattedTextField();
   private JLabel mCustomPlaneYLabel = new JLabel();
   private JFormattedTextField mCustomPlaneYField = new JFormattedTextField();
   private JLabel mCustomPlaneZLabel = new JLabel();
   private JFormattedTextField mCustomPlaneZField = new JFormattedTextField();
   private JLabel mCornerLabel = new JLabel();
   private JPanel mCornerPanel = new JPanel();
   private TableLayout mCornerPanelLayout = null;
   private JComboBox mCornerChooser = null;
   private JPanel mCustomCornerPanel = new JPanel();
   private JLabel jLabel1 = new JLabel();
   private JFormattedTextField mCornerXField = new JFormattedTextField();
   private JLabel jLabel2 = new JLabel();
   private JFormattedTextField mCornerYField = new JFormattedTextField();
   private JLabel jLabel3 = new JLabel();
   private JFormattedTextField mCornerZField = new JFormattedTextField();
   private JLabel jLabel4 = new JLabel();
   private JLabel mDimensionsLabel = new JLabel();
   private JPanel mDimensionsPanel = new JPanel();
   private JFormattedTextField mWallWidthField = new JFormattedTextField();
   private JLabel mDimensionsXLabel = new JLabel();
   private JFormattedTextField mWallHeightField = new JFormattedTextField();
   private JLabel mSurfaceTypeLabel = new JLabel();
   private JComboBox mSurfaceTypeChooser = new JComboBox(mSurfaceTypes);
   private JLabel mTrackerProxyLabel = new JLabel();
   private PropertyEditorPanel mTrackerProxyEditor = null;

   private static class UnitConversion
   {
      public String name;
      public Double factor;

      public UnitConversion(String name, double toMetersConversion)
      {
         this.name = name;
         this.factor = new Double(toMetersConversion);
      }
   }

   private class PlaneRenderer
      extends JLabel
      implements ListCellRenderer
   {
      public PlaneRenderer()
      {
         setOpaque(true);
         setHorizontalAlignment(LEFT);
         setVerticalAlignment(CENTER);
      }

      public Component getListCellRendererComponent(JList list, Object value,
                                                    int index,
                                                    boolean isSelected,
                                                    boolean cellHasFocus)
      {
         int selected_index = ((Integer) value).intValue();

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
         String plane   = mPlaneLabels[selected_index];
         setIcon(icon);
         setText(plane);
         setFont(list.getFont());

         return this;
      }
   }

   void physicalUnitsChanged(ActionEvent e)
   {
      mConversionFactor =
         ((UnitConversion) mUnitsList.get(mUnitsComboBox.getSelectedIndex())).factor;
   }

   void surfacePlaneChanged(ActionEvent e)
   {
      boolean enabled = (mPlaneChooser.getSelectedIndex() == CUSTOM_PLANE);
      mCustomPlaneXLabel.setEnabled(enabled);
      mCustomPlaneXField.setEnabled(enabled);
      mCustomPlaneYLabel.setEnabled(enabled);
      mCustomPlaneYField.setEnabled(enabled);
      mCustomPlaneZLabel.setEnabled(enabled);
      mCustomPlaneZField.setEnabled(enabled);

      if ( mPlaneChooser.getSelectedIndex() != CUSTOM_PLANE )
      {
         float[] orientation = getSelectedOrientation();
         mCustomPlaneXField.setValue(new Float(orientation[0]));
         mCustomPlaneYField.setValue(new Float(orientation[1]));
         mCustomPlaneZField.setValue(new Float(orientation[2]));
      }

      changeCornerIcon();
   }

   void surfaceTypeChanged(ActionEvent e)
   {
      mTracked = (mSurfaceTypeChooser.getSelectedIndex() != 0);

      // The tracker proxy editor parts of the UI are only needed when mTracked
      // is true.
      mTrackerProxyLabel.setEnabled(mTracked);
      mTrackerProxyEditor.setEnabled(mTracked);
      validateUserInput();
   }

   void wallWidthPropertyChanged(PropertyChangeEvent e)
   {
      // XXX: Detecting changes this way is slow and awkward.  It would be
      // better if we could listen for an event that occurs after the freshly
      // formatted input is committed.
      if ( e.getPropertyName().equals("value") && e.getNewValue() != null )
      {
         validateUserInput();
      }
   }

   void wallHeightPropertyChanged(PropertyChangeEvent e)
   {
      // XXX: Detecting changes this way is slow and awkward.  It would be
      // better if we could listen for an event that occurs after the freshly
      // formatted input is committed.
      if ( e.getPropertyName().equals("value") && e.getNewValue() != null )
      {
         validateUserInput();
      }
   }

   void cornerChanged(ActionEvent e)
   {
      changeCornerIcon();
   }

}

class SurfaceViewportCreateDialog_mUnitsComboBox_actionAdapter
   implements ActionListener
{
   private SurfaceViewportCreateDialog adaptee;

   SurfaceViewportCreateDialog_mUnitsComboBox_actionAdapter(SurfaceViewportCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.physicalUnitsChanged(e);
   }
}

class SurfaceViewportCreateDialog_mPlaneChooser_actionAdapter
   implements ActionListener
{
   private SurfaceViewportCreateDialog adaptee;

   SurfaceViewportCreateDialog_mPlaneChooser_actionAdapter(SurfaceViewportCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.surfacePlaneChanged(e);
   }
}

class SurfaceViewportCreateDialog_mSurfaceTypeChooser_actionAdapter
   implements ActionListener
{
   private SurfaceViewportCreateDialog adaptee;

   SurfaceViewportCreateDialog_mSurfaceTypeChooser_actionAdapter(SurfaceViewportCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.surfaceTypeChanged(e);
   }
}

class SurfaceViewportCreateDialog_mWallWidthField_propertyChangeAdapter implements java.beans.PropertyChangeListener
{
   private SurfaceViewportCreateDialog adaptee;

   SurfaceViewportCreateDialog_mWallWidthField_propertyChangeAdapter(SurfaceViewportCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void propertyChange(PropertyChangeEvent e)
   {
      adaptee.wallWidthPropertyChanged(e);
   }
}

class SurfaceViewportCreateDialog_mWallHeightField_propertyChangeAdapter implements java.beans.PropertyChangeListener
{
   private SurfaceViewportCreateDialog adaptee;

   SurfaceViewportCreateDialog_mWallHeightField_propertyChangeAdapter(SurfaceViewportCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void propertyChange(PropertyChangeEvent e)
   {
      adaptee.wallWidthPropertyChanged(e);
   }
}

class SurfaceViewportCreateDialog_mCornerChooser_actionAdapter implements java.awt.event.ActionListener
{
   private SurfaceViewportCreateDialog adaptee;

   SurfaceViewportCreateDialog_mCornerChooser_actionAdapter(SurfaceViewportCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.cornerChanged(e);
   }
}
