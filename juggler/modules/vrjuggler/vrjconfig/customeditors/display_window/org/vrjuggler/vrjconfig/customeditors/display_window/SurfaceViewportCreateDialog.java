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

package org.vrjuggler.vrjconfig.customeditors.display_window;

import java.awt.*;
import java.awt.event.*;
import java.util.Iterator;
import javax.swing.*;
import javax.swing.border.*;
import info.clearthought.layout.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;


public class SurfaceViewportCreateDialog
   extends ViewportCreateDialog
{
   public SurfaceViewportCreateDialog()
   {
      super("Basic Surface Viewport Parameters");

      initUI();

      mUnitsList.add(new UnitConversion("Meters", 1.0));
      mUnitsList.add(new UnitConversion("Feet", 0.30487804878048780487));
      mUnitsList.add(new UnitConversion("Inches", 0.02540005080010160020));

      for ( Iterator i = mUnitsList.iterator(); i.hasNext(); )
      {
         mUnitsComboBox.addItem(((UnitConversion) i.next()).name);
      }

      mUnitsComboBox.setSelectedIndex(0);

      this.pack();
   }

   public void propertyValueChanged(ConfigElementEvent e)
   {
      super.propertyValueChanged(e);

      if ( e.getProperty().equals("tracker_proxy") )
      {
         mTrackerProxy = e.getValue();
         validateUserInput();
      }
   }

   public Boolean isTracked()
   {
      return (mTracked ? Boolean.TRUE : Boolean.FALSE);
   }

   public Object getTrackerProxy()
   {
      return mTrackerProxy;
   }

   public double getUnitConversionFactor()
   {
      return mConversionFactor.floatValue();
   }

   protected void initUI()
   {
      super.initUI();

      ConfigBrokerProxy broker = new ConfigBrokerProxy();
      ConfigDefinition vp_def = broker.getRepository().get("surface_viewport");
      ConfigElementFactory factory =
         new ConfigElementFactory(broker.getRepository().getAllLatest());
      ConfigElement elt = factory.create("SurfaceViewportCreateDialog Junk",
                                         vp_def);
      elt.addConfigElementListener(this);

      mTrackerProxyEditor =
         new PropertyEditorPanel(elt.getProperty("tracker_proxy", 0),
                                 vp_def.getPropertyDefinition("tracker_proxy"),
                                 elt, 0, Color.white);

      double[][] main_size = {{TableLayout.PREFERRED},
                              {TableLayout.PREFERRED, TableLayout.PREFERRED,
                               TableLayout.PREFERRED}};

      mMainPanelLayout = new TableLayout(main_size);

      double[][] phys_size = {{TableLayout.PREFERRED, 10, TableLayout.PREFERRED},
                              {TableLayout.PREFERRED, 5, TableLayout.PREFERRED,
                               5, TableLayout.PREFERRED, TableLayout.PREFERRED,
                               TableLayout.PREFERRED, TableLayout.PREFERRED}};

      mPhysicalCharPanelLayout = new TableLayout(phys_size);

      ClassLoader loader = getClass().getClassLoader();
      String img_base = EditorConstants.imageBase;

      try
      {
         mPlaneImages[0] = new ImageIcon(loader.getResource(img_base + "/xy-plane-icon.png"));
         mPlaneImages[1] = new ImageIcon(loader.getResource(img_base + "/yz-plane-icon.png"));
         mPlaneImages[2] = new ImageIcon(loader.getResource(img_base + "/xz-plane-icon.png"));
         mPlaneImages[3] = new ImageIcon(loader.getResource(img_base + "/custom-plane-icon.png"));
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
         mPlaneChooser.setRenderer(new PlaneRenderer());
      }
      catch(Exception e)
      {
         e.printStackTrace();
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
      mCustomPlaneXField.setValue(new Double(0.0));
      mCustomPlaneXField.setHorizontalAlignment(SwingConstants.TRAILING);
      mCustomPlaneYLabel.setEnabled(false);
      mCustomPlaneYLabel.setLabelFor(mCustomPlaneYField);
      mCustomPlaneYLabel.setText("Y");
      mCustomPlaneYField.setEnabled(false);
      mCustomPlaneYField.setMinimumSize(new Dimension(40, 19));
      mCustomPlaneYField.setPreferredSize(new Dimension(45, 19));
      mCustomPlaneYField.setToolTipText("Rotation about the Y-axis in degrees");
      mCustomPlaneYField.setValue(new Double(0.0));
      mCustomPlaneYField.setHorizontalAlignment(SwingConstants.TRAILING);
      mCustomPlaneZLabel.setEnabled(false);
      mCustomPlaneZLabel.setLabelFor(mCustomPlaneZField);
      mCustomPlaneZLabel.setText("Z");
      mCustomPlaneZField.setEnabled(false);
      mCustomPlaneZField.setMinimumSize(new Dimension(40, 19));
      mCustomPlaneZField.setPreferredSize(new Dimension(45, 19));
      mCustomPlaneZField.setToolTipText("Rotation about the Z-axis in degrees");
      mCustomPlaneZField.setValue(new Double(0.0));
      mCustomPlaneZField.setHorizontalAlignment(SwingConstants.TRAILING);
      jLabel1.setText("(");
      mCornerXField.setMinimumSize(new Dimension(35, 19));
      mCornerXField.setPreferredSize(new Dimension(35, 19));
      mCornerXField.setToolTipText("X-coordinate for the surface corner");
      mCornerXField.setValue(new Double(0.0));
      mCornerXField.setHorizontalAlignment(SwingConstants.TRAILING);
      jLabel2.setText(",");
      mCornerYField.setMinimumSize(new Dimension(35, 19));
      mCornerYField.setPreferredSize(new Dimension(35, 19));
      mCornerYField.setToolTipText("Y-coordinate for the surface corner");
      mCornerYField.setValue(new Double(0.0));
      mCornerYField.setHorizontalAlignment(SwingConstants.TRAILING);
      jLabel3.setText(",");
      mCornerZField.setMinimumSize(new Dimension(35, 19));
      mCornerZField.setPreferredSize(new Dimension(35, 19));
      mCornerZField.setToolTipText("Z-coordinate for the surface corner");
      mCornerZField.setValue(new Double(0.0));
      mCornerZField.setHorizontalAlignment(SwingConstants.TRAILING);
      jLabel4.setText(")");
      mDimensionsLabel.setLabelFor(mDimensionsPanel);
      mDimensionsLabel.setText("Surface Dimensions");
      mWallWidthField.setMinimumSize(new Dimension(40, 19));
      mWallWidthField.setPreferredSize(new Dimension(40, 19));
      mWallWidthField.addFocusListener(new SurfaceViewportCreateDialog_mWallWidthField_focusAdapter(this));
      mWallWidthField.setValue(new Double(0.0));
      mWallWidthField.setHorizontalAlignment(SwingConstants.TRAILING);
      mDimensionsXLabel.setText("\u00d7");
      mWallHeightField.setMinimumSize(new Dimension(40, 19));
      mWallHeightField.setPreferredSize(new Dimension(40, 19));
      mWallHeightField.addFocusListener(new SurfaceViewportCreateDialog_mWallHeightField_focusAdapter(this));
      mWallHeightField.setValue(new Double(0.0));
      mCornerLabel.setText("Lower Left Corner");
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
      mPlanePanel.add(mPlaneChooser, null);
      mPlanePanel.add(mCustomPlaneXLabel, null);
      mPlanePanel.add(mCustomPlaneXField, null);
      mPlanePanel.add(mCustomPlaneYLabel, null);
      mPlanePanel.add(mCustomPlaneYField, null);
      mPlanePanel.add(mCustomPlaneZLabel, null);
      mPlanePanel.add(mCustomPlaneZField, null);
      mCornerPanel.add(jLabel1, null);
      mCornerPanel.add(mCornerXField, null);
      mCornerPanel.add(jLabel2, null);
      mCornerPanel.add(mCornerYField, null);
      mCornerPanel.add(jLabel3, null);
      mCornerPanel.add(mCornerZField, null);
      mCornerPanel.add(jLabel4, null);
      mDimensionsPanel.add(mWallWidthField, null);
      mDimensionsPanel.add(mDimensionsXLabel, null);
      mDimensionsPanel.add(mWallHeightField, null);
   }

   protected boolean validateCustomInput()
   {
      boolean wall_dim_set, tracker_set;

      wall_dim_set = ((Integer) mWallWidthField.getValue()).intValue() > 0 &&
                     ((Integer) mWallHeightField.getValue()).intValue() > 0;

      if ( mTracked )
      {
         tracker_set = mTrackerProxy != null;
      }
      else
      {
         tracker_set = true;
      }

      return wall_dim_set && tracker_set;
   }

   private static final int XY_PLANE = 0;
   private static final int YZ_PLANE = 1;
   private static final int XZ_PLANE = 2;
   private static final int CUSTOM_PLANE = 3;

   private java.util.List mUnitsList = new java.util.ArrayList();
   private Double mConversionFactor = null;

   private String[]    mPlaneLabels = {"XY", "YZ", "XZ", "Custom"};
   private Integer[]   mIntArray    = new Integer[mPlaneLabels.length];
   private ImageIcon[] mPlaneImages = new ImageIcon[mPlaneLabels.length];

   private Object   mTrackerProxy = null;
   private boolean  mTracked      = false;
   private String[] mSurfaceTypes = {"Fixed-Position Wall", "Desk/Bench",
                                     "HMD Eye"};

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
//   private JComboBox mCornerChooser = new JComboBox(new String[] {"Lower Left Corner", "Lower Right Corner", "Upper Right Corner", "Upper Left Corner"});
   private JPanel mCornerPanel = new JPanel();
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
   }

   void surfaceTypeChanged(ActionEvent e)
   {
      mTracked = (mSurfaceTypeChooser.getSelectedIndex() != 0);

      // The tracker proxy editor parts of the UI are only needed when mTracked
      // is true.
      mTrackerProxyLabel.setEnabled(mTracked);
      mTrackerProxyEditor.setEnabled(mTracked);
   }

   void wallDimensionsChanged(FocusEvent e)
   {
      validateUserInput();
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

class SurfaceViewportCreateDialog_mWallWidthField_focusAdapter extends java.awt.event.FocusAdapter
{
   private SurfaceViewportCreateDialog adaptee;

   SurfaceViewportCreateDialog_mWallWidthField_focusAdapter(SurfaceViewportCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void focusLost(FocusEvent e)
   {
      adaptee.wallDimensionsChanged(e);
   }
}

class SurfaceViewportCreateDialog_mWallHeightField_focusAdapter extends java.awt.event.FocusAdapter
{
   private SurfaceViewportCreateDialog adaptee;

   SurfaceViewportCreateDialog_mWallHeightField_focusAdapter(SurfaceViewportCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void focusLost(FocusEvent e)
   {
      adaptee.wallDimensionsChanged(e);
   }
}
