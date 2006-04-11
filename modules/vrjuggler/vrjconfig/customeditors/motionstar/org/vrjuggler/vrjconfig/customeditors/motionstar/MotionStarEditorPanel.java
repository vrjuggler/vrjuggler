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

package org.vrjuggler.vrjconfig.customeditors.motionstar;

import java.awt.*;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import javax.swing.*;
import javax.swing.border.TitledBorder;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;
import org.vrjuggler.vrjconfig.commoneditors.DeviceTypeEditor;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import org.vrjuggler.vrjconfig.commoneditors.SimpleProxyEditor;
import org.vrjuggler.vrjconfig.commoneditors.TransmitterTransformPanel;


public class MotionStarEditorPanel
   extends JPanel
   implements ChangeListener
            , DeviceTypeEditor
            , EditorConstants
{
   public MotionStarEditorPanel()
   {
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
      mReportRateSpinner.setModel(new SpinnerNumberModel(1, 1, 16, 1));
      mMeasurementRateSpinner.setModel(new SpinnerNumberModel(86.1, 70.0,
                                                              120.0, 0.1));
   }

   public void stateChanged(ChangeEvent e)
   {
      JSpinner source = (JSpinner) e.getSource();

      // Using the object returned by source.getValue() seems to be safe
      // because every change in the spinner's value creates a new object.
      if ( source == mReportRateSpinner )
      {
         mElement.setProperty("report_rate", 0, source.getValue(), mContext);
      }
      else if ( source == mMeasurementRateSpinner )
      {
         // NOTE: The object returned is of type Double rather than Float.
         mElement.setProperty("measurement_rate", 0, source.getValue(),
                              mContext);
      }
   }

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      mContext = ctx;
      mElement = elt;

      mAddressField.setValue(elt.getProperty("address", 0));
      mMeasurementRateSpinner.setValue(elt.getProperty("measurement_rate", 0));
      mReportRateSpinner.setValue(elt.getProperty("report_rate", 0));

      ConfigDefinition mstar_def = elt.getDefinition();
      mServerPortEditor =
         new PropertyEditorPanel(ctx, elt.getProperty("server_port", 0),
                                 mstar_def.getPropertyDefinition("server_port"),
                                 elt, 0, Color.white);
      mMasterEditor =
         new PropertyEditorPanel(ctx, elt.getProperty("is_master", 0),
                                 mstar_def.getPropertyDefinition("is_master"),
                                 elt, 0, Color.white);
      mHemisphereEditor =
         new PropertyEditorPanel(ctx, elt.getProperty("hemisphere", 0),
                                 mstar_def.getPropertyDefinition("hemisphere"),
                                 elt, 0, Color.white);
      mDataFormatEditor =
         new PropertyEditorPanel(ctx, elt.getProperty("data_format", 0),
                                 mstar_def.getPropertyDefinition("data_format"),
                                 elt, 0, Color.white);
      mModeEditor =
         new PropertyEditorPanel(ctx, elt.getProperty("mode", 0),
                                 mstar_def.getPropertyDefinition("mode"),
                                 elt, 0, Color.white);
      mDeviceHostEditor =
         new PropertyEditorPanel(ctx, elt.getProperty("device_host", 0),
                                 mstar_def.getPropertyDefinition("device_host"),
                                 elt, 0, Color.white);

      ConfigBrokerProxy broker = new ConfigBrokerProxy();
      java.util.List filters = elt.getPropertyValues("position_filters");

      if ( filters == null || filters.size() == 0 )
      {
         // Pop up a warning message saying that the config element is
         // missing information.
         Frame parent =
            (Frame) SwingUtilities.getAncestorOfClass(Frame.class, this);
         JOptionPane.showMessageDialog(parent,
                                       "WARNING: Config element '" +
                                          elt.getName() +
                                          "' has no position filters!\n" +
                                          "Will create default position " +
                                          "transform filter ...",
                                       "Correcting Incomplete Configuration Element",
                                       JOptionPane.WARNING_MESSAGE);

         // Create the new position_transform_filter config element.
         ConfigDefinition filter_def =
            broker.getRepository().get("position_transform_filter");
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());
         ConfigElement xform_filter = factory.create("Transform Filter 0",
                                                     filter_def);

         // Add the new config element to the position_filters property.
         elt.addProperty("position_filters", xform_filter);

         // Go ahead and set up the position transform filter editor panel
         // using the newly created config element.
         mPosXformFilterPanel.setConfig(ctx, xform_filter);
      }
      else
      {
         // Pass off the embedded position_transform_filter config element.
         // For now, we assume that there will be at most one such element.
         for ( java.util.Iterator i = filters.iterator(); i.hasNext(); )
         {
            ConfigElement filter = (ConfigElement) i.next();

            if ( filter.getDefinition().getToken().equals("position_transform_filter") )
            {
               mPosXformFilterPanel.setConfig(ctx, filter);
               break;
            }
         }
      }

      mProxyEditorPanel.setConfig(ctx, elt,
                                  broker.getRepository().get("position_proxy"));

      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }

      mReportRateSpinner.addChangeListener(this);
      mMeasurementRateSpinner.addChangeListener(this);
   }

   private void jbInit()
      throws Exception
   {


      this.setLayout(mMainLayout);
      mHardwarePanel.setBorder(mHardwarePanelTitle);
      mHardwarePanel.setLayout(mHardwarePanelLayout);
      mHardwarePanelTitle.setTitle("Hardware Settings");
      mRealHardwarePanel.setLayout(mRealHardwarePanelLayout);
      mAddressLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mAddressLabel.setLabelFor(mAddressField);
      mAddressLabel.setText("Server Address:");
      mAddressField.addPropertyChangeListener(new
         MotionStarEditorPanel_mAddressField_propertyChangeAdapter(this));
      mServerPortLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mServerPortLabel.setLabelFor(mServerPortEditor);
      mServerPortLabel.setText("Server Network Port:");
      mMasterLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mMasterLabel.setLabelFor(mMasterEditor);
      mMasterLabel.setText("Is Master?");
      mHemisphereLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mHemisphereLabel.setLabelFor(mHemisphereEditor);
      mHemisphereLabel.setText("Hemisphere:");
      mDataFormatLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mDataFormatLabel.setLabelFor(mDataFormatEditor);
      mDataFormatLabel.setText("Data Format:");
      mModeLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mModeLabel.setText("Sampling Mode:");
      mMeasurementRateLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mMeasurementRateLabel.setLabelFor(mMeasurementRateSpinner);
      mMeasurementRateLabel.setText("Chassis Measurement Rate:");
      mReportRateLabel.setForeground(Color.black);
      mReportRateLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mReportRateLabel.setLabelFor(mReportRateSpinner);
      mReportRateLabel.setText("Sensor Report Rate:");
      mReportRateSpinner.setMinimumSize(new Dimension(70, 24));
      mReportRateSpinner.setPreferredSize(new Dimension(70, 24));
      mMeasurementRateSpinner.setMinimumSize(new Dimension(70, 24));
      mMeasurementRateSpinner.setPreferredSize(new Dimension(70, 24));
      mDeviceHostLabel.setForeground(Color.black);
      mDeviceHostLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mDeviceHostLabel.setLabelFor(mDeviceHostEditor);
      mDeviceHostLabel.setText("Device Host:");
      mRealHardwarePanel.add(mAddressLabel,
                             new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
                                                    GridBagConstraints.EAST,
                                                    GridBagConstraints.NONE,
                                                    new Insets(0, 0, 0, 3),
                                                    0, 0));
      mRealHardwarePanel.add(mAddressField,
                             new GridBagConstraints(1, 0, 1, 1, 1.0, 0.0,
                                                    GridBagConstraints.WEST,
                                                    GridBagConstraints.HORIZONTAL,
                                                    new Insets(0, 0, 0, 0),
                                                    0, 0));
      mRealHardwarePanel.add(mServerPortLabel,
                             new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0,
                                                    GridBagConstraints.EAST,
                                                    GridBagConstraints.NONE,
                                                    new Insets(0, 0, 0, 3),
                                                    0, 0));
      mRealHardwarePanel.add(mServerPortEditor,
                             new GridBagConstraints(1, 1, 1, 1, 1.0, 1.0,
                                                    GridBagConstraints.WEST,
                                                    GridBagConstraints.HORIZONTAL,
                                                    new Insets(0, 0, 0, 0),
                                                    0, 0));
      mRealHardwarePanel.add(mMasterLabel,
                             new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0,
                                                    GridBagConstraints.EAST,
                                                    GridBagConstraints.NONE,
                                                    new Insets(0, 0, 0, 3),
                                                    0, 0));
      mRealHardwarePanel.add(mMasterEditor,
                             new GridBagConstraints(1, 2, 1, 1, 1.0, 1.0,
                                                    GridBagConstraints.WEST,
                                                    GridBagConstraints.HORIZONTAL,
                                                    new Insets(0, 0, 0, 0),
                                                    0, 0));
      mRealHardwarePanel.add(mHemisphereLabel,
                             new GridBagConstraints(0, 3, 1, 1, 0.0, 0.0,
                                                    GridBagConstraints.EAST,
                                                    GridBagConstraints.NONE,
                                                    new Insets(0, 0, 0, 3),
                                                    0, 0));
      mRealHardwarePanel.add(mHemisphereEditor,
                             new GridBagConstraints(1, 3, 1, 1, 1.0, 1.0,
                                                    GridBagConstraints.WEST,
                                                    GridBagConstraints.HORIZONTAL,
                                                    new Insets(0, 0, 0, 0),
                                                    0, 0));
      mRealHardwarePanel.add(mDataFormatLabel,
                             new GridBagConstraints(0, 4, 1, 1, 0.0, 0.0,
                                                    GridBagConstraints.EAST,
                                                    GridBagConstraints.NONE,
                                                    new Insets(0, 0, 0, 3),
                                                    0, 0));
      mRealHardwarePanel.add(mDataFormatEditor,
                             new GridBagConstraints(1, 4, 1, 1, 1.0, 1.0,
                                                    GridBagConstraints.WEST,
                                                    GridBagConstraints.HORIZONTAL,
                                                    new Insets(0, 0, 0, 0),
                                                    0, 0));
      mRealHardwarePanel.add(mModeLabel,
                             new GridBagConstraints(0, 5, 1, 1, 0.0, 0.0,
                                                    GridBagConstraints.EAST,
                                                    GridBagConstraints.NONE,
                                                    new Insets(0, 0, 0, 3),
                                                    0, 0));
      mRealHardwarePanel.add(mModeEditor,
                             new GridBagConstraints(1, 5, 1, 1, 1.0, 1.0,
                                                    GridBagConstraints.WEST,
                                                    GridBagConstraints.HORIZONTAL,
                                                    new Insets(0, 0, 0, 0),
                                                    0, 0));
      mRealHardwarePanel.add(mReportRateLabel,
                             new GridBagConstraints(0, 6, 1, 1, 0.0, 0.0,
                                                    GridBagConstraints.EAST,
                                                    GridBagConstraints.NONE,
                                                    new Insets(0, 0, 0, 3),
                                                    0, 0));
      mRealHardwarePanel.add(mReportRateSpinner,
                             new GridBagConstraints(1, 6, 1, 1, 1.0, 1.0,
                                                    GridBagConstraints.WEST,
                                                    GridBagConstraints.NONE,
                                                    new Insets(0, 0, 2, 0),
                                                    0, 0));
      mRealHardwarePanel.add(mMeasurementRateLabel,
                             new GridBagConstraints(0, 7, 1, 1, 0.0, 0.0,
                                                    GridBagConstraints.EAST,
                                                    GridBagConstraints.NONE,
                                                    new Insets(0, 3, 0, 3),
                                                    0, 0));
      mRealHardwarePanel.add(mMeasurementRateSpinner,
                             new GridBagConstraints(1, 7, 1, 1, 1.0, 1.0,
                                                    GridBagConstraints.WEST,
                                                    GridBagConstraints.NONE,
                                                    new Insets(0, 0, 2, 0),
                                                    0, 0));
      mRealHardwarePanel.add(mDeviceHostLabel,
                             new GridBagConstraints(0, 8, 1, 1, 1.0, 1.0,
                                                    GridBagConstraints.EAST,
                                                    GridBagConstraints.NONE,
                                                    new Insets(0, 3, 2, 3),
                                                    0, 0));
      mRealHardwarePanel.add(mDeviceHostEditor,
                             new GridBagConstraints(1, 8, 1, 1, 1.0, 1.0,
                                                    GridBagConstraints.WEST,
                                                    GridBagConstraints.HORIZONTAL,
                                                    new Insets(0, 0, 2, 0),
                                                    0, 0));
      mHardwarePanel.add(mRealHardwarePanel,
                         new GridBagConstraints(0, 0, 1, 1, 1.0, 1.0,
                                                GridBagConstraints.WEST,
                                                GridBagConstraints.VERTICAL,
                                                new Insets(0, 0, 0, 0), 0, 0));
      this.add(mHardwarePanel,
               new GridBagConstraints(0, 0, 1, 1, 1.0, 1.0,
                                      GridBagConstraints.CENTER,
                                      GridBagConstraints.BOTH,
                                      new Insets(0, 0, 2, 0), 0, 0));
      this.add(mTabbedPane,
               new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0,
                                      GridBagConstraints.CENTER,
                                      GridBagConstraints.BOTH,
                                      new Insets(0, 0, 0, 0), 0, 0));
      
      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         mTransIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/transmitter16.png"));
         mPositionIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/position16.png"));
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
      }
      mTabbedPane.addTab("Transmitter", mTransIcon, mPosXformFilterPanel, "Transmitter Settings");
      mTabbedPane.addTab("Sensors", mPositionIcon, mProxyEditorPanel, "Sensor Settings");
   }

   private ConfigContext mContext = null;
   private ConfigElement mElement = null;

   private TransmitterTransformPanel mPosXformFilterPanel =
      new TransmitterTransformPanel();
   private SimpleProxyEditor mProxyEditorPanel = new SimpleProxyEditor();
   private JPanel mHardwarePanel = new JPanel();
   private TitledBorder mHardwarePanelTitle = new TitledBorder("");
   private JPanel mRealHardwarePanel = new JPanel();
   private JLabel mAddressLabel = new JLabel();
   private JFormattedTextField mAddressField = new JFormattedTextField();
   private JLabel mServerPortLabel = new JLabel();
   private PropertyEditorPanel mServerPortEditor = null;
   private JLabel mMasterLabel = new JLabel();
   private PropertyEditorPanel mMasterEditor = null;
   private JLabel mHemisphereLabel = new JLabel();
   private PropertyEditorPanel mHemisphereEditor = null;
   private JLabel mDataFormatLabel = new JLabel();
   private PropertyEditorPanel mDataFormatEditor = null;
   private JLabel mModeLabel = new JLabel();
   private PropertyEditorPanel mModeEditor = null;
   private JLabel mReportRateLabel = new JLabel();
   private JSpinner mReportRateSpinner = new JSpinner();
   private JLabel mMeasurementRateLabel = new JLabel();
   private JSpinner mMeasurementRateSpinner = new JSpinner();
   private JLabel mDeviceHostLabel = new JLabel();
   private PropertyEditorPanel mDeviceHostEditor = null;
   private GridBagLayout mHardwarePanelLayout = new GridBagLayout();
   private GridBagLayout mRealHardwarePanelLayout = new GridBagLayout();
   private GridBagLayout mMainLayout = new GridBagLayout();
   private JTabbedPane mTabbedPane = new JTabbedPane(JTabbedPane.BOTTOM );
   private Icon mTransIcon = null;
   private Icon mPositionIcon = null;

   public void mAddressField_propertyChange(PropertyChangeEvent propertyChangeEvent)
   {
      if ( propertyChangeEvent.getPropertyName().equals("value") )
      {
         try
         {
            java.net.InetAddress.getByName(mAddressField.getText());
            mAddressField.setForeground(Color.black);
         }
         catch(java.net.UnknownHostException ex)
         {
            mAddressField.setForeground(Color.red);
         }
         mElement.setProperty("address", 0, mAddressField.getText(), mContext);
      }
   }
}

class MotionStarEditorPanel_mAddressField_propertyChangeAdapter
   implements PropertyChangeListener
{
   private MotionStarEditorPanel adaptee;
   MotionStarEditorPanel_mAddressField_propertyChangeAdapter(
      MotionStarEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void propertyChange(PropertyChangeEvent propertyChangeEvent)
   {
      adaptee.mAddressField_propertyChange(propertyChangeEvent);
   }
}
