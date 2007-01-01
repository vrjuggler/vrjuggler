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

package org.vrjuggler.vrjconfig.customeditors.cave;

import java.awt.*;
import java.awt.event.*;
import java.beans.*;
import java.util.Iterator;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import info.clearthought.layout.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;

public class WallEditorDialog
   extends JDialog
   implements ConfigElementListener, EditorConstants
{
   private static int VP_ELT_COUNT = 0;
   protected CaveWall mCaveWall = null;
   protected ConfigContext mConfigContext = null;
   private int mPlane;
   
   public WallEditorDialog(Container parent, ConfigContext ctx)
   {
      this(parent, ctx, null);
   }

   public WallEditorDialog(Container parent, ConfigContext ctx,
                                      CaveWall cw)
   {
      super();
      this.setTitle("Projection Surface Editor");
      this.setModal(true);

      if ( cw == null )
      {
         cw = new CaveWall();
      }

      /*
      if ( elt == null )
      {
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         ConfigDefinition vp_def = broker.getRepository().get(SURFACE_VIEWPORT_TYPE);
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());
         elt = factory.create("WallEditorDialog Element " + VP_ELT_COUNT,
                              vp_def);
      }
      */
      
      mCaveWall = cw;
      mConfigContext = ctx;
      
      // Listen for changes to left view.
      mCaveWall.getLeftView().addConfigElementListener(this);

      mCorners[Plane.LL_CORNER] = "Lower Left Corner";
      mCorners[Plane.LR_CORNER] = "Lower Right Corner";
      mCorners[Plane.UR_CORNER] = "Upper Right Corner";
      mCorners[Plane.UL_CORNER] = "Upper Left Corner";

      ConfigBrokerProxy broker = new ConfigBrokerProxy();
      ConfigDefinition vp_def =
         broker.getRepository().get(SURFACE_VIEWPORT_TYPE);
      
      //XXX: Choose left view for editing since mono, stereo, and passive stereo
      //     all use the left view Make sure to set the tracker & user for the
      //     right view when done.
      mTrackerProxyEditor =
         new PropertyEditorPanel(mConfigContext, mCaveWall.getLeftView().getProperty("tracker_proxy", 0),
                                 vp_def.getPropertyDefinition("tracker_proxy"),
                                 mCaveWall.getLeftView(), 0, Color.white);
      mUserEditor =
         new PropertyEditorPanel(mConfigContext, mCaveWall.getLeftView().getProperty("user", 0),
                                 vp_def.getPropertyDefinition("user"),
                                 mCaveWall.getLeftView(), 0, Color.white);

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
      if ( mCaveWall == null )
      {
         mUnitsComboBox.setSelectedIndex(2);
      }
      // For input coming in through a config element, all units will be in
      // meters.  We will not attempt to guess what units the user would
      // prefer.
      else
      {
         mUnitsComboBox.setSelectedIndex(0);
         mPlaneChooser.setSelectedIndex(mCaveWall.getPlane());

         float[] ll_corner = new float[3], lr_corner = new float[3],
                 ul_corner = new float[3];
         /*
         ll_corner[0] = ((Number) mViewportElement.getProperty("lower_left_corner", 0)).floatValue();
         ll_corner[1] = ((Number) mViewportElement.getProperty("lower_left_corner", 1)).floatValue();
         ll_corner[2] = ((Number) mViewportElement.getProperty("lower_left_corner", 2)).floatValue();
         lr_corner[0] = ((Number) mViewportElement.getProperty("lower_right_corner", 0)).floatValue();
         lr_corner[1] = ((Number) mViewportElement.getProperty("lower_right_corner", 1)).floatValue();
         lr_corner[2] = ((Number) mViewportElement.getProperty("lower_right_corner", 2)).floatValue();
         ul_corner[0] = ((Number) mViewportElement.getProperty("upper_left_corner", 0)).floatValue();
         ul_corner[1] = ((Number) mViewportElement.getProperty("upper_left_corner", 1)).floatValue();
         ul_corner[2] = ((Number) mViewportElement.getProperty("upper_left_corner", 2)).floatValue();
         */
         ll_corner[0] = (float)mCaveWall.getCorners()[0].x;
         ll_corner[1] = (float)mCaveWall.getCorners()[0].y;
         ll_corner[2] = (float)mCaveWall.getCorners()[0].z;
         lr_corner[0] = (float)mCaveWall.getCorners()[1].x;
         lr_corner[1] = (float)mCaveWall.getCorners()[1].y;
         lr_corner[2] = (float)mCaveWall.getCorners()[1].z;
         ul_corner[0] = (float)mCaveWall.getCorners()[2].x;
         ul_corner[1] = (float)mCaveWall.getCorners()[2].y;
         ul_corner[2] = (float)mCaveWall.getCorners()[2].z;
         
         float wall_width, wall_height;
         
         mCaveWall.updateWidthHeight();
         wall_width = (float)mCaveWall.getWidth();
         wall_height = (float)mCaveWall.getHeight();
         /*
         if ( ll_corner[0] == lr_corner[0] )
         {
            if ( ll_corner[2] > lr_corner[2] )
            {
               mPlaneChooser.setSelectedIndex(LEFT_PLANE);
               mPlane = LEFT_PLANE;
               wall_width  = ll_corner[2] - lr_corner[2];
               wall_height = ul_corner[1] - ll_corner[1];
            }
            else
            {
               mPlaneChooser.setSelectedIndex(RIGHT_PLANE);
               mPlane = RIGHT_PLANE;
               wall_width  = lr_corner[2] - ll_corner[2];
               wall_height = ul_corner[1] - ll_corner[1];
            }
         }
         else if ( ll_corner[2] == lr_corner[2] )
         {
            if ( lr_corner[0] > ll_corner[0] )
            {
               mPlaneChooser.setSelectedIndex(FRONT_PLANE);
               mPlane = FRONT_PLANE;
               wall_width  = lr_corner[0] - ll_corner[0];
               wall_height = ul_corner[1] - ll_corner[1];
            }
            else
            {
               mPlaneChooser.setSelectedIndex(BACK_PLANE);
               mPlane = BACK_PLANE;
               wall_width  = ll_corner[0] - lr_corner[0];
               wall_height = ul_corner[1] - ll_corner[1];
            }
         }
         else if ( ll_corner[1] == ul_corner [1] )
         {
            if ( ll_corner[2] > ul_corner[2] )
            {
               mPlaneChooser.setSelectedIndex(BOTTOM_PLANE);
               mPlane = BOTTOM_PLANE;
               wall_width  = lr_corner[0] - ll_corner[0];
               wall_height = ll_corner[2] - ul_corner[2];
            }
            else
            {
               mPlaneChooser.setSelectedIndex(TOP_PLANE);
               mPlane = TOP_PLANE;
               wall_width  = lr_corner[0] - ll_corner[0];
               wall_height = ul_corner[2] - ll_corner[2];
            }
         }
         else
         {
            mPlaneChooser.setSelectedIndex(CUSTOM_PLANE);
            mPlane = CUSTOM_PLANE;

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
         */

         mCornerXField.setValue(new Float(ll_corner[0]));
         mCornerYField.setValue(new Float(ll_corner[1]));
         mCornerZField.setValue(new Float(ll_corner[2]));
         mWallWidthField.setValue(new Float(wall_width));
         mWallHeightField.setValue(new Float(wall_height));
      }
      
      validateUserInput();
      this.setResizable(false);
      this.getRootPane().setDefaultButton(mOkButton);
      this.pack();
   }
   
   private int status = CANCEL_OPTION;
   protected JPanel mMainPanel = new JPanel();
   protected TableLayout mMainPanelLayout = null;

   private JPanel mButtonPanel = new JPanel();
   private FlowLayout mButtonPanelLayout = new FlowLayout();
   private JButton mOkButton = new JButton();
   private JButton mCancelButton = new JButton();
   private JButton mHelpButton = new JButton();
      
   void okClicked(ActionEvent e)
   {
      status = OK_OPTION;
      dispose();
   }

   void cancelClicked(ActionEvent e)
   {
      status = CANCEL_OPTION;
      dispose();
   }

   void helpClicked(ActionEvent e)
   {

   }

   public static final int OK_OPTION     = JOptionPane.OK_OPTION;
   public static final int CANCEL_OPTION = JOptionPane.CANCEL_OPTION;
   public static final int CLOSED_OPTION = JOptionPane.CLOSED_OPTION;
   
   public int showDialog()
   {
      setVisible(true);
      return status;
   }
   
   public Object getUser()
   {
      return mCaveWall.getLeftView().getProperty(USER_PROPERTY, 0);
   }
   
   public Object getTrackerProxy()
   {
      return mCaveWall.getLeftView().getProperty(TRACKER_PROXY_PROPERTY, 0);
   }
   
   public int getPlane()
   {
      return mPlane;
   }
      
   public void nameChanged(ConfigElementEvent e)
   {
   }

   public void propertyValueChanged(ConfigElementEvent e)
   {
      validateUserInput();
   }

   public void propertyValueOrderChanged(ConfigElementEvent e)
   {
   }

   public void propertyValueAdded(ConfigElementEvent e)
   {
   }

   public void propertyValueRemoved(ConfigElementEvent e)
   {
   }
   
   public Boolean isTracked()
   {
      return (mTracked ? Boolean.TRUE : Boolean.FALSE);
   }
   
   public Point3D[] getCorners()
   {
      float wall_width = ((Number) mWallWidthField.getValue()).floatValue();
      float wall_height = ((Number) mWallHeightField.getValue()).floatValue();

      Plane surface_plane = new Plane(wall_width, wall_height);

      if ( mPlaneChooser.getSelectedIndex() == FRONT_PLANE )
      {
         surface_plane.setOrientation(0.0, 0.0, 0.0);
      }
      else if ( mPlaneChooser.getSelectedIndex() == BACK_PLANE )
      {
         surface_plane.setOrientation(0.0, 180.0, 0.0);
      }
      else if ( mPlaneChooser.getSelectedIndex() == LEFT_PLANE )
      {
         surface_plane.setOrientation(0.0, 90.0, 0.0);
      }
      else if ( mPlaneChooser.getSelectedIndex() == RIGHT_PLANE )
      {
         surface_plane.setOrientation(0.0, -90.0, 0.0);
      }
      else if ( mPlaneChooser.getSelectedIndex() == BOTTOM_PLANE )
      {
         surface_plane.setOrientation(-90.0, 0.0, 0.0);
      }
      else if ( mPlaneChooser.getSelectedIndex() == TOP_PLANE )
      {
         surface_plane.setOrientation(90.0, 0.0, 0.0);
      }
      else
      {
         float x_angle = ((Number) mCustomPlaneXField.getValue()).floatValue();
         float y_angle = ((Number) mCustomPlaneYField.getValue()).floatValue();
         float z_angle = ((Number) mCustomPlaneZField.getValue()).floatValue();
         surface_plane.setOrientation(x_angle, y_angle, z_angle);
      }
      
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
         scaled_corners[i].round();
         //System.out.println(scaled_corners[i]);
      }

      return scaled_corners;
   }

   public double getUnitConversionFactor()
   {
      return mConversionFactor.doubleValue();
   }
   
   public String getWallName()
   {
      return mNameField.getText();
   }
   
   protected void initUI()
   {
      mButtonPanel.setLayout(mButtonPanelLayout);
      mOkButton.setEnabled(false);
      mOkButton.setText("OK");
      mOkButton.addActionListener(new WallEditorDialog_mOkButton_actionAdapter(this));
      mCancelButton.setText("Cancel");
      mCancelButton.addActionListener(new WallEditorDialog_mCancelButton_actionAdapter(this));
      mHelpButton.setEnabled(false);
      mHelpButton.setDoubleBuffered(false);
      mHelpButton.setText("Help");
      mHelpButton.addActionListener(new WallEditorDialog_mHelpButton_actionAdapter(this));
      this.getContentPane().add(mMainPanel, BorderLayout.CENTER);
      this.getContentPane().add(mButtonPanel, BorderLayout.SOUTH);

      if ( (new EnvironmentServiceProxy()).getOS() == EnvironmentService.Windows )
      {
         mButtonPanelLayout.setAlignment(FlowLayout.CENTER);
         mButtonPanel.add(mOkButton, null);
         mButtonPanel.add(mCancelButton, null);
         mButtonPanel.add(mHelpButton, null);
      }
      else
      {
         mButtonPanelLayout.setAlignment(FlowLayout.RIGHT);
         mButtonPanel.add(mHelpButton, null);
         mButtonPanel.add(mCancelButton, null);
         mButtonPanel.add(mOkButton, null);
      }

      //super.initUI();

      mCornerChooser = new JComboBox(mCorners);
      mCornerChooser.setSelectedIndex(Plane.LL_CORNER);

      double[][] main_size = {{TableLayout.PREFERRED},
                              {TableLayout.PREFERRED, TableLayout.PREFERRED,
                               TableLayout.PREFERRED}};

      double[][] phys_size = {{TableLayout.PREFERRED, 10, TableLayout.PREFERRED},
                              {TableLayout.PREFERRED, TableLayout.PREFERRED,
                               5, TableLayout.PREFERRED,
                               5, TableLayout.PREFERRED, TableLayout.PREFERRED,
                               TableLayout.PREFERRED, TableLayout.PREFERRED,
                               TableLayout.PREFERRED, TableLayout.PREFERRED,
                               TableLayout.PREFERRED}};

      mMainPanelLayout = new TableLayout(phys_size);

      double[][] corner_size = {{TableLayout.FILL},
                                {TableLayout.PREFERRED, 5,
                                 TableLayout.PREFERRED}};
      mCornerPanelLayout = new TableLayout(corner_size);

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
         mNameField.setText(mCaveWall.getName());
         javax.swing.event.DocumentListener myListener = new javax.swing.event.DocumentListener()
            {
               public void changedUpdate(DocumentEvent e)
               {
                  validateUserInput();
               }
               public void insertUpdate(DocumentEvent e)
               {
                  validateUserInput();
               }
               public void removeUpdate(DocumentEvent e)
               {
                  validateUserInput();
               }
            };
         mNameField.getDocument().addDocumentListener(myListener);
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
            new ImageIcon(loader.getResource(img_base + "/bottom-plane-ll.png"));
         mCornerIcons[BOTTOM_PLANE][Plane.LR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/bottom-plane-lr.png"));
         mCornerIcons[BOTTOM_PLANE][Plane.UR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/bottom-plane-ur.png"));
         mCornerIcons[BOTTOM_PLANE][Plane.UL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/bottom-plane-ul.png"));
         
         mCornerIcons[TOP_PLANE][Plane.LL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/top-plane-ll.png"));
         mCornerIcons[TOP_PLANE][Plane.LR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/top-plane-lr.png"));
         mCornerIcons[TOP_PLANE][Plane.UR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/top-plane-ur.png"));
         mCornerIcons[TOP_PLANE][Plane.UL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/top-plane-ul.png"));


         mCornerIcons[CUSTOM_PLANE][Plane.LL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/custom-plane-ll.png"));
         mCornerIcons[CUSTOM_PLANE][Plane.LR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/custom-plane-lr.png"));
         mCornerIcons[CUSTOM_PLANE][Plane.UR_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/custom-plane-ur.png"));
         mCornerIcons[CUSTOM_PLANE][Plane.UL_CORNER] =
            new ImageIcon(loader.getResource(img_base + "/custom-plane-ul.png"));
         mCornerIconLabel.setIcon(mCornerIcons[mPlaneChooser.getSelectedIndex()][mCornerChooser.getSelectedIndex()]);
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
      mButtonPanel.setLayout(mButtonPanelLayout);
      mOkButton.setEnabled(false);
      mOkButton.setText("OK");
      mOkButton.addActionListener(new WallEditorDialog_mOkButton_actionAdapter(this));
      mCancelButton.setText("Cancel");
      mCancelButton.addActionListener(new WallEditorDialog_mCancelButton_actionAdapter(this));
      mHelpButton.setEnabled(false);
      mHelpButton.setDoubleBuffered(false);
      mHelpButton.setText("Help");
      mHelpButton.addActionListener(new WallEditorDialog_mHelpButton_actionAdapter(this));
      mMainPanel.setLayout(mMainPanelLayout);
      
      mNameLabel.setHorizontalAlignment(SwingConstants.LEADING);
      mNameLabel.setLabelFor(mNameField);
      mNameLabel.setText("Wall Name");
      mNameField.setMinimumSize(new Dimension(60, 21));
      mNameField.setPreferredSize(new Dimension(60, 21));
      mNameField.setText("Cave Wall");

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
      mWallWidthField.addPropertyChangeListener(new WallEditorDialog_mWallWidthField_propertyChangeAdapter(this));
      mWallWidthField.setHorizontalAlignment(SwingConstants.TRAILING);
      mDimensionsXLabel.setText("\u00d7");
      mWallHeightField.setMinimumSize(new Dimension(40, 19));
      mWallHeightField.setPreferredSize(new Dimension(60, 19));
      mWallHeightField.addPropertyChangeListener(new WallEditorDialog_mWallHeightField_propertyChangeAdapter(this));
      mCornerIconLabel.setText("");
      mCornerLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mCornerLabel.setHorizontalTextPosition(SwingConstants.LEADING);
      mCornerLabel.setLabelFor(mCornerPanel);
      mCornerLabel.setText("Set Single Corner");
      mCornerLabel.setVerticalAlignment(javax.swing.SwingConstants.TOP);
      mCornerLabel.setVerticalTextPosition(javax.swing.SwingConstants.CENTER);
      mUnitsComboBox.addActionListener(new WallEditorDialog_mUnitsComboBox_actionAdapter(this));
      mPlaneChooser.addActionListener(new WallEditorDialog_mPlaneChooser_actionAdapter(this));
      mUnitsComboBox.setToolTipText("The units of measurement used for physical locations to be entered below");
      mPlaneChooser.setToolTipText("The physical plane along which the surface is aligned");
      mSurfaceTypeLabel.setLabelFor(mSurfaceTypeChooser);
      mSurfaceTypeLabel.setText("Surface Type");
      mSurfaceTypeChooser.addActionListener(new WallEditorDialog_mSurfaceTypeChooser_actionAdapter(this));
      mTrackerProxyLabel.setEnabled(false);
      mTrackerProxyLabel.setLabelFor(mTrackerProxyEditor);
      mTrackerProxyLabel.setText("Surface Tracker Proxy");
      mTrackerProxyEditor.setEnabled(false);
      mUserLabel.setLabelFor(mUserEditor);
      mUserLabel.setText("User");
      mCornerChooser.addActionListener(new WallEditorDialog_mCornerChooser_actionAdapter(this));

      mMainPanel.add(mNameLabel,
                     new TableLayoutConstraints(0, 0, 0, 0,
                                                TableLayout.RIGHT,
                                                TableLayout.CENTER));
      mMainPanel.add(mNameField,
                     new TableLayoutConstraints(2, 0, 2, 0,
                                                TableLayout.FULL,
                                                TableLayout.CENTER));      
      mMainPanel.add(mSurfaceTypeLabel,
                     new TableLayoutConstraints(0, 1, 0, 1,
                                                TableLayout.LEFT,
                                                TableLayout.FULL));
      mMainPanel.add(mSurfaceTypeChooser,
                     new TableLayoutConstraints(2, 1, 2, 1,
                                                TableLayout.FULL,
                                                TableLayout.FULL));
      mMainPanel.add(mTrackerProxyLabel,
                     new TableLayoutConstraints(0, 3, 0, 3,
                                                TableLayout.LEFT,
                                                TableLayout.FULL));
      mMainPanel.add(mTrackerProxyEditor,
                     new TableLayoutConstraints(2, 3, 2, 3,
                                                TableLayout.FULL,
                                                TableLayout.FULL));
      mMainPanel.add(mUnitsLabel,
                     new TableLayoutConstraints(0, 5, 0, 5,
                                                TableLayout.LEFT,
                                                TableLayout.FULL));
      mMainPanel.add(mUnitsComboBox,
                     new TableLayoutConstraints(2, 5, 2, 5,
                                                TableLayout.FULL,
                                                TableLayout.FULL));
      mMainPanel.add(mPlaneLabel,
                     new TableLayoutConstraints(0, 6, 0, 6,
                                                TableLayout.LEFT,
                                                TableLayout.FULL));
      mMainPanel.add(mPlanePanel,
                     new TableLayoutConstraints(2, 7, 2, 7,
                                                TableLayout.FULL,
                                                TableLayout.FULL));
      mMainPanel.add(mCustomPlanePanel,
                     new TableLayoutConstraints(2, 7, 2, 7,
                                                TableLayout.FULL,
                                                TableLayout.FULL));
      mMainPanel.add(mCornerLabel,
                     new TableLayoutConstraints(0, 8, 0, 8,
                                                TableLayout.LEFT,
                                                TableLayout.FULL));
      mMainPanel.add(mCornerPanel,
                     new TableLayoutConstraints(2, 8, 2, 9,
                                                TableLayout.FULL,
                                                TableLayout.FULL));
      mMainPanel.add(mCornerIconLabel,
                     new TableLayoutConstraints(0, 9, 0, 9,
                                                TableLayout.LEFT,
                                                TableLayout.FULL));
      mMainPanel.add(mDimensionsLabel,
                     new TableLayoutConstraints(0, 10, 0, 10,
                                                TableLayout.LEFT,
                                                TableLayout.FULL));
      mMainPanel.add(mDimensionsPanel,
                     new TableLayoutConstraints(2, 10, 2, 10,
                                                TableLayout.FULL,
                                                TableLayout.FULL));
      mMainPanel.add(mUserLabel,
                     new TableLayoutConstraints(0, 11, 0, 11,
                                                TableLayout.LEFT,
                                                TableLayout.FULL));
      mMainPanel.add(mUserEditor,
                     new TableLayoutConstraints(2, 11, 2, 11,
                                                TableLayout.FULL,
                                                TableLayout.FULL));
      mCornerPanel.add(mCornerChooser,
                       new TableLayoutConstraints(0, 0, 0, 0, TableLayout.FULL,
                                                  TableLayout.FULL));
      mCornerPanel.add(mCustomCornerPanel,
                       new TableLayoutConstraints(0, 2, 0, 2, TableLayout.FULL,
                                                  TableLayout.FULL));
      mPlanePanel.add(mPlaneChooser, null);
      mCustomPlanePanel.add(mCustomPlaneXLabel, null);
      mCustomPlanePanel.add(mCustomPlaneXField, null);
      mCustomPlanePanel.add(mCustomPlaneYLabel, null);
      mCustomPlanePanel.add(mCustomPlaneYField, null);
      mCustomPlanePanel.add(mCustomPlaneZLabel, null);
      mCustomPlanePanel.add(mCustomPlaneZField, null);
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
   
   protected final void validateUserInput()
   {
      boolean name_valid, user_set, custom_valid;
      boolean enabled;
      
      // The name cannot be an empty string.
      name_valid   = ! mNameField.getText().equals("");
      String user = ((ConfigElementPointer) getUser()).getTarget();
      user_set = (user != null && !user.equals(""));
      custom_valid = validateCustomInput();

      // All of the above conditions must be true in order for the OK button
      // to be enabled.
      enabled = name_valid && user_set && custom_valid;
      mOkButton.setEnabled(enabled);
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
         String proxy = ((ConfigElementPointer) getTrackerProxy()).getTarget();
         tracker_set = (proxy != null && !proxy.equals(""));
      }
      else
      {
         tracker_set = true;
      }

      return wall_width_set && wall_height_set && tracker_set;
   }

   private void changeCornerIcon()
   {
      int plane = mPlaneChooser.getSelectedIndex();
      int corner = mCornerChooser.getSelectedIndex();
      if ( mCornerIcons[plane][corner] != null )
      {
         mCornerIconLabel.setIcon(mCornerIcons[plane][corner]);
      }
   }

   private java.util.List mUnitsList = new java.util.ArrayList();
   private Double mConversionFactor = null;

   private String[]    mPlaneLabels = {"Front", "Back", "Left", "Right", "Bottom", "Top", "Custom"};
   private Integer[]   mIntArray    = new Integer[mPlaneLabels.length];
   private ImageIcon[] mPlaneImages = new ImageIcon[mPlaneLabels.length];

   private boolean  mTracked      = false;
   private String[] mSurfaceTypes = {"Fixed-Position Wall/Desk",
                                     "Adjustable Desk/Table", "HMD Eye"};

   private String[] mCorners = new String[4];

   private ImageIcon[][] mCornerIcons = new ImageIcon[7][4];

   private JLabel mNameLabel = new JLabel();
   private JTextField mNameField = new JTextField();
   private TableLayout mPhysicalCharPanelLayout = null;
//   private TitledBorder mPhysicalCharPanelBorder;
   private JComboBox mUnitsComboBox = new JComboBox();
   private JLabel mUnitsLabel = new JLabel();
   private JLabel mPlaneLabel = new JLabel();
   private JPanel mPlanePanel = new JPanel();
   private JPanel mCustomPlanePanel = new JPanel();
   private JComboBox mPlaneChooser;
   private JLabel mCustomPlaneXLabel = new JLabel();
   private JFormattedTextField mCustomPlaneXField = new JFormattedTextField();
   private JLabel mCustomPlaneYLabel = new JLabel();
   private JFormattedTextField mCustomPlaneYField = new JFormattedTextField();
   private JLabel mCustomPlaneZLabel = new JLabel();
   private JFormattedTextField mCustomPlaneZField = new JFormattedTextField();
   private JLabel mCornerLabel = new JLabel();
   private JLabel mCornerIconLabel = new JLabel();
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
   private JLabel mUserLabel = new JLabel();
   private PropertyEditorPanel mUserEditor = null;
   
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

class WallEditorDialog_mUnitsComboBox_actionAdapter
   implements ActionListener
{
   private WallEditorDialog adaptee;

   WallEditorDialog_mUnitsComboBox_actionAdapter(WallEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.physicalUnitsChanged(e);
   }
}

class WallEditorDialog_mPlaneChooser_actionAdapter
   implements ActionListener
{
   private WallEditorDialog adaptee;

   WallEditorDialog_mPlaneChooser_actionAdapter(WallEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.surfacePlaneChanged(e);
   }
}

class WallEditorDialog_mSurfaceTypeChooser_actionAdapter
   implements ActionListener
{
   private WallEditorDialog adaptee;

   WallEditorDialog_mSurfaceTypeChooser_actionAdapter(WallEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.surfaceTypeChanged(e);
   }
}

class WallEditorDialog_mWallWidthField_propertyChangeAdapter implements java.beans.PropertyChangeListener
{
   private WallEditorDialog adaptee;

   WallEditorDialog_mWallWidthField_propertyChangeAdapter(WallEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void propertyChange(PropertyChangeEvent e)
   {
      adaptee.wallWidthPropertyChanged(e);
   }
}

class WallEditorDialog_mWallHeightField_propertyChangeAdapter implements java.beans.PropertyChangeListener
{
   private WallEditorDialog adaptee;

   WallEditorDialog_mWallHeightField_propertyChangeAdapter(WallEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void propertyChange(PropertyChangeEvent e)
   {
      adaptee.wallWidthPropertyChanged(e);
   }
}

class WallEditorDialog_mCornerChooser_actionAdapter implements java.awt.event.ActionListener
{
   private WallEditorDialog adaptee;

   WallEditorDialog_mCornerChooser_actionAdapter(WallEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.cornerChanged(e);
   }
}

class WallEditorDialog_mOkButton_actionAdapter implements ActionListener
{
   private WallEditorDialog adaptee;

   WallEditorDialog_mOkButton_actionAdapter(WallEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.okClicked(e);
   }
}

class WallEditorDialog_mCancelButton_actionAdapter
   implements ActionListener
{
   private WallEditorDialog adaptee;

   WallEditorDialog_mCancelButton_actionAdapter(WallEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.cancelClicked(e);
   }
}

class WallEditorDialog_mHelpButton_actionAdapter
   implements ActionListener
{
   private WallEditorDialog adaptee;

   WallEditorDialog_mHelpButton_actionAdapter(WallEditorDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.helpClicked(e);
   }
}
