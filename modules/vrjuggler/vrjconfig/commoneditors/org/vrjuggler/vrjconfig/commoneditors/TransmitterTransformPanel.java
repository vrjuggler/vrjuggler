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

package org.vrjuggler.vrjconfig.commoneditors;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.Iterator;
import java.util.Map;
import javax.swing.*;
import javax.swing.border.TitledBorder;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.EventListenerList;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;
import org.vrjuggler.vrjconfig.commoneditors.event.*;


public class TransmitterTransformPanel
   extends JPanel
   implements ChangeListener
{
   public TransmitterTransformPanel()
   {
      mCoordFrames[0]  = new CoordFrame(0.0, 0.0, 0.0);
      mCoordFrames[1]  = new CoordFrame(90.0, 0.0, 0.0);
      mCoordFrames[2]  = new CoordFrame(180.0, 0.0, 0.0);
      mCoordFrames[3]  = new CoordFrame(180.0, 0.0, -90.0);
      mCoordFrames[4]  = new CoordFrame(180.0, 0.0, 90.0);
      mCoordFrames[5]  = new CoordFrame(0.0, 180.0, 0.0);
      mCoordFrames[6]  = new CoordFrame(180.0, -90.0, 0.0);
      mCoordFrames[7]  = new CoordFrame(0.0, 0.0, 90.0);
      mCoordFrames[8]  = new CoordFrame(0.0, 0.0, 180.0);
      mCoordFrames[9]  = new CoordFrame(0.0, 0.0, -90.0);
      mCoordFrames[10] = new CoordFrame(90.0, 0.0, 90.0);
      mCoordFrames[11] = new CoordFrame(90.0, 0.0, 180.0);
      mCoordFrames[12] = new CoordFrame(90.0, 0.0, -90.0);
      mCoordFrames[13] = new CoordFrame(90.0, 90.0, 0.0);
      mCoordFrames[14] = new CoordFrame(-180.0, 90.0, 0.0);
      mCoordFrames[15] = new CoordFrame(-90.0, 90.0, 0.0);
      mCoordFrames[16] = new CoordFrame(0.0, 90.0, 0.0);
      mCoordFrames[17] = new CoordFrame(-90.0, 0.0, 180.0);
      mCoordFrames[18] = new CoordFrame(-90.0, 0.0, -90.0);
      mCoordFrames[19] = new CoordFrame(-90.0, 0.0, 0.0);
      mCoordFrames[20] = new CoordFrame(-90.0, 0.0, 90.0);
      mCoordFrames[21] = new CoordFrame(90.0, -90.0, 0.0);
      mCoordFrames[22] = new CoordFrame(0.0, -90.0, 0.0);
      mCoordFrames[23] = new CoordFrame(-90.0, -90.0, 0.0);

      try
      {
         ClassLoader loader = getClass().getClassLoader();
         String img_base = "org/vrjuggler/vrjconfig/commoneditors/images";

         for ( int i = 0; i < mCoordFrames.length; ++i )
         {
            StringBuffer buf = new StringBuffer(Integer.toString(i));
            if ( buf.length() == 1 )
            {
               buf.insert(0, 0);
            }
            mCoordFrames[i].setIcon(
               new ImageIcon(loader.getResource(img_base + "/axis" + buf +
                                                ".png"))
            );
         }
      }
      catch(NullPointerException ex)
      {
         for ( int i = 0; i < mCoordFrames.length; ++i )
         {
            mCoordFrames[i].setIcon(null);
         }
      }

      for ( int i = 0; i < mCoordFrames.length; ++i )
      {
         mVrjCoordChooser.addItem(new Integer(i));
         mTrackerCoordChooser.addItem(new Integer(i));
      }

      mTranslationUnits    = new MeasurementUnit[4];
      mTranslationUnits[0] = new MeasurementUnit("Meters", 1.0);
      mTranslationUnits[1] = new MeasurementUnit("Centimeters", 0.1);
      mTranslationUnits[2] = new MeasurementUnit("Feet", 0.3048);
      mTranslationUnits[3] = new MeasurementUnit("Inches", 0.0254);

      for ( int i = 0; i < mTranslationUnits.length; ++i )
      {
         mTrackerPosUnitsChooser.addItem(mTranslationUnits[i].name);
      }
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
      mTrackerXRotSpinner.setModel(new SpinnerNumberModel(0.0, -180.0,
                                                          180.0, 0.1));
      mTrackerYRotSpinner.setModel(new SpinnerNumberModel(0.0, -180.0,
                                                          180.0, 0.1));
      mTrackerZRotSpinner.setModel(new SpinnerNumberModel(0.0, -180.0,
                                                          180.0, 0.1));

      mVrjCoordChooser.setRenderer(new CoordFrameRenderer(mCoordFrames));
      mTrackerCoordChooser.setRenderer(new CoordFrameRenderer(mCoordFrames));
   }

   public void stateChanged(ChangeEvent e)
   {
      JSpinner source = (JSpinner) e.getSource();

      int value_index;

      if ( source == mTrackerXRotSpinner )
      {
         value_index = 0;
      }
      else if ( source == mTrackerYRotSpinner )
      {
         value_index = 1;
      }
      else if ( source == mTrackerYRotSpinner )
      {
         value_index = 2;
      }
      else
      {
         System.err.println("[TransmitterTransformPanel.stateChanged()] " +
                            "Received state change event from unknown source " +
                            source);
         return;
      }

      // Using the object returned by source.getValue() seems to be safe
      // because every change in the spinner's value creates a new object.
      // NOTE: The object returned is of type Double rather than Float.
      mElement.setProperty("pre_rotation", value_index, source.getValue());
   }

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      if ( elt.getDefinition().getToken().equals("position_transform_filter") )
      {
         mContext = ctx;
         mElement = elt;

         PropertyDefinition prop_def =
            mElement.getDefinition().getPropertyDefinition("device_units");
         Map dev_unit_map = prop_def.getEnums();

         elt.addConfigElementListener(new ElementListener());
         mSensorUnitsChooser =
            new PropertyEditorPanel(ctx, elt.getProperty("device_units", 0),
                                    elt.getDefinition().getPropertyDefinition("device_units"),
                                    elt, 0, Color.white);

         // Set up the custom units field.
         mCustomUnitsField.setText(elt.getProperty("custom_scale", 0).toString());
         boolean enable_custom =
            ((Number) elt.getProperty("device_units", 0)).floatValue() == 0.0;
         mCustomUnitsField.setEnabled(enable_custom);

         mTrackerXPosField.setValue(elt.getProperty("pre_translate", 0));
         mTrackerYPosField.setValue(elt.getProperty("pre_translate", 1));
         mTrackerZPosField.setValue(elt.getProperty("pre_translate", 2));

         double x_rot =
            ((Number) elt.getProperty("pre_rotation", 0)).doubleValue();
         double y_rot =
            ((Number) elt.getProperty("pre_rotation", 1)).doubleValue();
         double z_rot =
            ((Number) elt.getProperty("pre_rotation", 2)).doubleValue();

         mTrackerXRotSpinner.setValue(new Double(x_rot));
         mTrackerYRotSpinner.setValue(new Double(y_rot));
         mTrackerZRotSpinner.setValue(new Double(z_rot));

         // This is slow, but I don't think that there is a better way to do
         // it.
         int i;
         for ( i = 0; i < mCoordFrames.length; ++i )
         {
            if ( mCoordFrames[i].getXRot() == x_rot &&
                 mCoordFrames[i].getYRot() == y_rot &&
                 mCoordFrames[i].getZRot() == z_rot )
            {
               mTrackerCoordChooser.setSelectedIndex(i);
               break;
            }
         }

         System.out.println("[TrackerTransformPanel.setConfig()] i == " + i);
         System.out.println("[TrackerTransformPanel.setConfig()] mCoordFrames.length == " + mCoordFrames.length);

         // If the search through mCoordFrames turned up no match, then we
         // will enable the manual rotation editing just to be friendly.
         if ( i == mCoordFrames.length )
         {
            mTrackerManualRotButton.setSelected(true);
         }

         try
         {
            jbInit();
         }
         catch(Exception ex)
         {
            ex.printStackTrace();
         }

         // The change listener should be added after the spinners are fully
         // set up.
         mTrackerXRotSpinner.addChangeListener(this);
         mTrackerYRotSpinner.addChangeListener(this);
         mTrackerZRotSpinner.addChangeListener(this);
      }
      else
      {
         throw new IllegalArgumentException("Cannot handle config element " +
                                            "of type '" +
                                            elt.getDefinition().getToken() +
                                            "'");
      }
   }

   public void addJugglerTransformChangeListener(TransformChangeListener l)
   {
      mJugglerListeners.add(TransformChangeListener.class, l);
   }

   public void removeJugglerTransformChangeListener(TransformChangeListener l)
   {
      mJugglerListeners.remove(TransformChangeListener.class, l);
   }

   public void addTrackerTransformChangeListener(TransformChangeListener l)
   {
      mTrackerListeners.add(TransformChangeListener.class, l);
   }

   public void removeTrackerTransformChangeListener(TransformChangeListener l)
   {
      mTrackerListeners.remove(TransformChangeListener.class, l);
   }

   public double getJugglerXRot()
   {
      return mCoordFrames[mVrjCoordChooser.getSelectedIndex()].getXRot();
   }

   public double getJugglerYRot()
   {
      return mCoordFrames[mVrjCoordChooser.getSelectedIndex()].getYRot();
   }

   public double getJugglerZRot()
   {
      return mCoordFrames[mVrjCoordChooser.getSelectedIndex()].getZRot();
   }

   public double getTrackerXRot()
   {
      double x_rot;

      if ( mTrackerManualRotButton.isSelected() )
      {
         x_rot = ((Double) mTrackerXRotSpinner.getValue()).doubleValue();
      }
      else
      {
         x_rot = mCoordFrames[mTrackerCoordChooser.getSelectedIndex()].getXRot();
      }

      return x_rot;
   }

   public double getTrackerYRot()
   {
      double y_rot;

      if ( mTrackerManualRotButton.isSelected() )
      {
         y_rot = ((Double) mTrackerYRotSpinner.getValue()).doubleValue();
      }
      else
      {
         y_rot = mCoordFrames[mTrackerCoordChooser.getSelectedIndex()].getYRot();
      }

      return y_rot;
   }

   public double getTrackerZRot()
   {
      double z_rot;

      if ( mTrackerManualRotButton.isSelected() )
      {
         z_rot = ((Double) mTrackerZRotSpinner.getValue()).doubleValue();
      }
      else
      {
         z_rot = mCoordFrames[mTrackerCoordChooser.getSelectedIndex()].getZRot();
      }

      return z_rot;
   }

   protected void fireTransformChanged(EventListenerList listenerList,
                                       double xRot, double yRot, double zRot)
   {
      TransformChangeEvent change_event = null;
      Object[] listeners = listenerList.getListenerList();

      // Process the listeners last to first, notifying
      // those that are interested in this event
      for ( int i = listeners.length - 2; i >= 0; i -= 2 )
      {
         if (listeners[i] == TransformChangeListener.class)
         {
            // Lazily create the event:
            if ( change_event == null)
            {
               change_event = new TransformChangeEvent(this, xRot, yRot, zRot);
            }
            ((TransformChangeListener) listeners[i + 1]).transformChanged(change_event);
         }
      }
   }

   private void jbInit()
      throws Exception
   {
      this.setLayout(mMainLayout);
      mRotationPanel.setBorder(mRotationBorder);
      mRotationPanel.setLayout(mRotationPanelLayout);
      mRotationBorder.setTitle("Orientation");
      mTransltationPanel.setBorder(mTranslationBorder);
      mTransltationPanel.setLayout(mTranslationPanelLayout);
      mTranslationBorder.setTitle("Position");
      mVrjCoordChooserLabel.setLabelFor(mVrjCoordChooser);
      mVrjCoordChooserLabel.setText("<html>VR Juggler Coordinate Frame</html>");
      mVrjCoordPanel.setLayout(mVrjCoordPanelLayout);
      mVrjCoordChooser.setEnabled(false);
      mVrjCoordChooser.setMinimumSize(new Dimension(64, 64));
      mVrjCoordChooser.setPreferredSize(new Dimension(64, 64));
      mVrjCoordChooser.addActionListener(new
         TransmitterTransformPanel_mVrjCoordChooser_actionAdapter(this));
      mVrjCoordAdvButton.setText("Advanced");
      mVrjCoordAdvButton.addActionListener(new
         TransmitterTransformPanel_mVrjCoordAdvButton_actionAdapter(this));
      mTrackerCoordPanel.setLayout(mTrackerCoordPanelLayout);
      mTrackerCoordChooserLabel.setLabelFor(mTrackerCoordChooser);
      mTrackerCoordChooserLabel.setText(
         "<html>Transmitter Coordinate Frame</html>");
      mTrackerAdvPanel.setLayout(mTrackerAdvPanelLayout);
      mTrackerAnglesLabel.setLabelFor(mTrackerAnglesPanel);
      mTrackerAnglesLabel.setText(
         "<html>Transmitter Pre-Rotation Angles</html>");
      mTrackerAnglesPanel.setLayout(mTrackerAnglesPanelLayout);
      mTrackerXRotLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerXRotLabel.setLabelFor(mTrackerXRotSpinner);
      mTrackerXRotLabel.setText("X Rotation:");
      mTrackerYRotLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerYRotLabel.setLabelFor(mTrackerYRotSpinner);
      mTrackerYRotLabel.setText("Y Rotation:");
      mTrackerZRotLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerZRotLabel.setLabelFor(mTrackerZRotSpinner);
      mTrackerZRotLabel.setText("Z Rotation:");
      mTrackerManualRotButton.setText("Manual Entry");
      mTrackerManualRotButton.addActionListener(new
         TransmitterTransformPanel_mTrackerManualRotButton_actionAdapter(this));
      mTrackerXRotSpinner.setEnabled(false);
      mTrackerYRotSpinner.setEnabled(false);
      mTrackerZRotSpinner.setEnabled(false);
      mTrackerPosLabel.setLabelFor(mTrackerPosPanel);
      mTrackerPosLabel.setText(
         "<html>Measurements of the tracker transmitter from VR Juggler origin (pre-translation)</html>");
      mTrackerPosPanel.setLayout(mTrackerPosPanelLayout);
      mTrackerXPosLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerXPosLabel.setLabelFor(mTrackerXPosField);
      mTrackerXPosLabel.setText("X:");
      mTrackerPosUnitsLabel.setLabelFor(mTrackerPosUnitsChooser);
      mTrackerPosUnitsLabel.setText("Measurement Units:");
      mTrackerXPosField.setText("0.0");
      mTrackerXPosField.addPropertyChangeListener(new
         TransmitterTransformPanel_mTrackerXPosField_propertyChangeAdapter(this));
      mTrackerXPosField.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerYPosLabel.setLabelFor(mTrackerYPosField);
      mTrackerYPosLabel.setText("Y:");
      mTrackerYPosField.setText("0.0");
      mTrackerYPosField.addPropertyChangeListener(new
         TransmitterTransformPanel_mTrackerYPosField_propertyChangeAdapter(this));
      mTrackerYPosField.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerZPosLabel.setLabelFor(mTrackerZPosField);
      mTrackerZPosLabel.setText("Z:");
      mTrackerZPosField.setText("0.0");
      mTrackerZPosField.addPropertyChangeListener(new
         TransmitterTransformPanel_mTrackerZPosField_propertyChangeAdapter(this));
      mTrackerZPosField.setHorizontalAlignment(SwingConstants.TRAILING);
      mSensorUnitsPanel.setBorder(mTrackerUnitsBorder);
      mSensorUnitsPanel.setLayout(mSensorUnitsPanelLayout);
      mTrackerUnitsBorder.setTitle("Tracker Units");
      mSensorUnitsLabel.setLabelFor(mSensorUnitsChooser);
      mSensorUnitsLabel.setText("Sensor Sample Units");
      mCustomUnitsField.setEnabled(false);
      mCustomUnitsField.setToolTipText(
         "Set custom unit conversion (to meters) for sensor samples");
      mCustomUnitsField.setText("1.0");
      mCustomUnitsField.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerPosUnitsChooser.setToolTipText(
         "Choose the units of the values entered below");
      mTrackerPosUnitsChooser.addActionListener(new
         TransmitterTransformPanel_mTrackerPosUnitsChooser_actionAdapter(this));
      mSensorUnitsChooser.setToolTipText(
         "Choose the units of samples collected from sensors");
      mTrackerCoordChooser.addActionListener(new
         TransmitterTransformPanel_mTrackerCoordChooser_actionAdapter(this));
      mTrackerCoordChooser.setMinimumSize(new Dimension(64, 64));
      mTrackerCoordChooser.setPreferredSize(new Dimension(64, 64));
      mTrackerAdvPanel.add(mTrackerAnglesPanel,
                           new GridBagConstraints(0, 1, 1, 1, 1.0, 1.0
                                                  , GridBagConstraints.CENTER,
                                                  GridBagConstraints.BOTH,
                                                  new Insets(0, 0, 0, 0), 0, 0));
      mTrackerCoordPanel.add(mTrackerCoordChooser,
                             new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0
         , GridBagConstraints.CENTER, GridBagConstraints.BOTH,
         new Insets(0, 0, 0, 0), 0, 0));
      mVrjCoordPanel.add(mVrjCoordChooser,
                         new GridBagConstraints(0, 1, 1, 1, 1.0, 0.0
                                                , GridBagConstraints.CENTER,
                                                GridBagConstraints.HORIZONTAL,
                                                new Insets(0, 0, 0, 0), 0, 0));
      mVrjCoordPanel.add(mVrjCoordAdvButton,
                         new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0
                                                , GridBagConstraints.CENTER,
                                                GridBagConstraints.HORIZONTAL,
                                                new Insets(0, 0, 0, 0), 0, 0));
      mVrjCoordPanel.add(mVrjCoordChooserLabel,
                         new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
                                                , GridBagConstraints.NORTH,
                                                GridBagConstraints.HORIZONTAL,
                                                new Insets(0, 0, 0, 0), 0, 0));
      mTrackerCoordPanel.add(mTrackerCoordChooserLabel,
                             new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
         , GridBagConstraints.NORTH, GridBagConstraints.HORIZONTAL,
         new Insets(0, 0, 0, 0), 0, 0));
      mRotationPanel.add(mTrackerAdvPanel,
                         new GridBagConstraints(2, 0, 1, 1, 1.0, 1.0
                                                , GridBagConstraints.CENTER,
                                                GridBagConstraints.BOTH,
                                                new Insets(0, 0, 0, 2), -66,
                                                -89));
      mTrackerAnglesPanel.add(mTrackerXRotLabel,
                              new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
         , GridBagConstraints.EAST, GridBagConstraints.NONE,
         new Insets(0, 0, 0, 3), 0, 0));
      mTrackerAnglesPanel.add(mTrackerYRotLabel,
                              new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0
         , GridBagConstraints.EAST, GridBagConstraints.NONE,
         new Insets(0, 0, 0, 3), 0, 0));
      mTrackerAnglesPanel.add(mTrackerZRotLabel,
                              new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0
         , GridBagConstraints.EAST, GridBagConstraints.NONE,
         new Insets(0, 0, 0, 3), 0, 0));
      mTrackerAdvPanel.add(mTrackerAnglesLabel,
                           new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
                                                  , GridBagConstraints.NORTH,
                                                  GridBagConstraints.HORIZONTAL,
                                                  new Insets(0, 0, 3, 0), 0, 0));
      mTrackerAdvPanel.add(mTrackerManualRotButton,
                           new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0
                                                  , GridBagConstraints.CENTER,
                                                  GridBagConstraints.HORIZONTAL,
                                                  new Insets(3, 0, 0, 0), 0, 0));
      mTransltationPanel.add(mTrackerPosLabel,
                             new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
         , GridBagConstraints.WEST, GridBagConstraints.BOTH,
         new Insets(0, 0, 0, 0), 0, 0));
      mTrackerPosPanel.add(mTrackerYPosLabel,
                           new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0
                                                  , GridBagConstraints.EAST,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 3), 0, 0));
      mTrackerPosPanel.add(mTrackerZPosLabel,
                           new GridBagConstraints(0, 3, 1, 1, 0.0, 0.0
                                                  , GridBagConstraints.EAST,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 3), 0, 9));
      mTrackerPosPanel.add(mTrackerXPosLabel,
                           new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0
                                                  , GridBagConstraints.EAST,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 3), 0, 0));
      mTrackerPosPanel.add(mTrackerPosUnitsLabel,
                           new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
                                                  , GridBagConstraints.CENTER,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 3), 0, 0));
      mTransltationPanel.add(mTrackerPosPanel,
                             new GridBagConstraints(0, 1, 1, 1, 1.0, 1.0
         , GridBagConstraints.CENTER, GridBagConstraints.BOTH,
         new Insets(0, 0, 0, 0), 0, 0));
      mRotationPanel.add(mVrjCoordPanel,
                         new GridBagConstraints(0, 0, 1, 1, 1.0, 1.0
                                                , GridBagConstraints.NORTH,
                                                GridBagConstraints.HORIZONTAL,
                                                new Insets(0, 0, 0, 5), 0, 0));
      mRotationPanel.add(mTrackerCoordPanel,
                         new GridBagConstraints(1, 0, 1, 1, 1.0, 1.0
                                                , GridBagConstraints.NORTH,
                                                GridBagConstraints.HORIZONTAL,
                                                new Insets(0, 0, 0, 5), 0, 0));
      this.add(mTransltationPanel, new GridBagConstraints(0, 2, 1, 1, 1.0, 1.0
         , GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
         new Insets(0, 0, 2, 0), 0, 0));
      mTrackerPosPanel.add(mTrackerPosUnitsChooser,
                           new GridBagConstraints(1, 0, 1, 1, 0.0, 0.0
                                                  , GridBagConstraints.WEST,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 0), 0, 0));
      mTrackerPosPanel.add(mTrackerXPosField,
                           new GridBagConstraints(1, 1, 1, 1, 1.0, 0.0
                                                  , GridBagConstraints.WEST,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 1), 20, 0));
      mTrackerPosPanel.add(mTrackerYPosField,
                           new GridBagConstraints(1, 2, 1, 1, 1.0, 0.0
                                                  , GridBagConstraints.WEST,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 1), 20, 0));
      mTrackerPosPanel.add(mTrackerZPosField,
                           new GridBagConstraints(1, 3, 1, 1, 1.0, 0.0
                                                  , GridBagConstraints.WEST,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 1), 20, 0));
      mSensorUnitsPanel.add(mCustomUnitsField,
                            new GridBagConstraints(2, 0, 1, 1, 1.0, 0.0
         , GridBagConstraints.WEST, GridBagConstraints.NONE,
         new Insets(0, 3, 0, 2), 50, 0));
      mTrackerAnglesPanel.add(mTrackerYRotSpinner,
                              new GridBagConstraints(1, 1, 1, 1, 1.0, 1.0
         , GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
         new Insets(0, 0, 3, 0), 54, 10));
      mTrackerAnglesPanel.add(mTrackerZRotSpinner,
                              new GridBagConstraints(1, 2, 1, 1, 1.0, 1.0
         , GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
         new Insets(0, 0, 0, 0), 54, 10));
      this.add(mRotationPanel, new GridBagConstraints(0, 1, 1, 1, 1.0, 1.0
         , GridBagConstraints.CENTER, GridBagConstraints.BOTH,
         new Insets(0, 0, 0, 0), 0, 30));
      mSensorUnitsPanel.add(mSensorUnitsLabel,
                            new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
         , GridBagConstraints.EAST, GridBagConstraints.NONE,
         new Insets(0, 0, 0, 3), 0, 0));
      this.add(mSensorUnitsPanel, new GridBagConstraints(0, 0, 1, 1, 1.0, 1.0
         , GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
         new Insets(0, 0, 0, 0), 0, 0));
      mTrackerAnglesPanel.add(mTrackerXRotSpinner,
                              new GridBagConstraints(1, 0, 1, 1, 1.0, 1.0
         , GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL,
         new Insets(0, 0, 3, 0), 54, 10));
      mSensorUnitsPanel.add(mSensorUnitsChooser,
                            new GridBagConstraints(1, 0, 1, 1, 1.0, 0.0
         , GridBagConstraints.WEST, GridBagConstraints.NONE,
         new Insets(0, 0, 0, 0), 0, 0));
   }

   private ConfigContext mContext = null;
   private ConfigElement mElement = null;

   private CoordFrame[]      mCoordFrames      = new CoordFrame[24];
   private EventListenerList mJugglerListeners = new EventListenerList();
   private EventListenerList mTrackerListeners = new EventListenerList();
   private MeasurementUnit[] mTranslationUnits = null;

   private JPanel mRotationPanel = new JPanel();
   private TitledBorder mRotationBorder = new TitledBorder("");
   private JPanel mTransltationPanel = new JPanel();
   private TitledBorder mTranslationBorder = new TitledBorder("");
   private JPanel mVrjCoordPanel = new JPanel();
   private JLabel mVrjCoordChooserLabel = new JLabel();
   private JComboBox mVrjCoordChooser = new JComboBox();
   private JCheckBox mVrjCoordAdvButton = new JCheckBox();
   private JPanel mTrackerCoordPanel = new JPanel();
   private JLabel mTrackerCoordChooserLabel = new JLabel();
   private JComboBox mTrackerCoordChooser = new JComboBox();
   private JPanel mTrackerAdvPanel = new JPanel();
   private JLabel mTrackerAnglesLabel = new JLabel();
   private JPanel mTrackerAnglesPanel = new JPanel();
   private JLabel mTrackerXRotLabel = new JLabel();
   private JSpinner mTrackerXRotSpinner = new JSpinner();
   private JLabel mTrackerYRotLabel = new JLabel();
   private JSpinner mTrackerYRotSpinner = new JSpinner();
   private JLabel mTrackerZRotLabel = new JLabel();
   private JSpinner mTrackerZRotSpinner = new JSpinner();
   private JCheckBox mTrackerManualRotButton = new JCheckBox();
   private JLabel mTrackerPosLabel = new JLabel();
   private JPanel mTrackerPosPanel = new JPanel();
   private JLabel mTrackerXPosLabel = new JLabel();
   private JComboBox mTrackerPosUnitsChooser = new JComboBox();
   private JLabel mTrackerPosUnitsLabel = new JLabel();
   private JFormattedTextField mTrackerXPosField = new JFormattedTextField();
   private JLabel mTrackerYPosLabel = new JLabel();
   private JFormattedTextField mTrackerYPosField = new JFormattedTextField();
   private JLabel mTrackerZPosLabel = new JLabel();
   private JFormattedTextField mTrackerZPosField = new JFormattedTextField();
   private JPanel mSensorUnitsPanel = new JPanel();
   private TitledBorder mTrackerUnitsBorder = new TitledBorder("");
   private JLabel mSensorUnitsLabel = new JLabel();
   private PropertyEditorPanel mSensorUnitsChooser = null;
   private JTextField mCustomUnitsField = new JTextField();
   private GridBagLayout mVrjCoordPanelLayout = new GridBagLayout();
   private GridBagLayout mTrackerCoordPanelLayout = new GridBagLayout();
   private GridBagLayout mTrackerAdvPanelLayout = new GridBagLayout();
   private GridBagLayout mTrackerAnglesPanelLayout = new GridBagLayout();
   private GridBagLayout mRotationPanelLayout = new GridBagLayout();
   private GridBagLayout mSensorUnitsPanelLayout = new GridBagLayout();
   private GridBagLayout mTrackerPosPanelLayout = new GridBagLayout();
   private GridBagLayout mTranslationPanelLayout = new GridBagLayout();
   private GridBagLayout mMainLayout = new GridBagLayout();

   private class ElementListener
      extends ConfigElementAdapter
   {
      public void propertyValueChanged(ConfigElementEvent evt)
      {
         if ( evt.getProperty().equals("device_units") )
         {
            Object value = mElement.getProperty("device_units", 0);
            boolean enable = ((Number) value).floatValue() == 0.0;
            mCustomUnitsField.setEnabled(enable);
         }
      }
   }

   void mVrjCoordAdvButton_actionPerformed(ActionEvent actionEvent)
   {
      mVrjCoordChooser.setEnabled(mVrjCoordAdvButton.isSelected());
   }

   void mTrackerManualRotButton_actionPerformed(ActionEvent actionEvent)
   {
      boolean enabled = mTrackerManualRotButton.isSelected();
      mTrackerXRotSpinner.setEnabled(enabled);
      mTrackerYRotSpinner.setEnabled(enabled);
      mTrackerZRotSpinner.setEnabled(enabled);
   }

   void mVrjCoordChooser_actionPerformed(ActionEvent actionEvent)
   {
      CoordFrame cf = mCoordFrames[mVrjCoordChooser.getSelectedIndex()];
      fireTransformChanged(mJugglerListeners, cf.getXRot(), cf.getYRot(),
                           cf.getZRot());
   }

   void mTrackerCoordChooser_actionPerformed(ActionEvent actionEvent)
   {
      CoordFrame cf = mCoordFrames[mTrackerCoordChooser.getSelectedIndex()];
      mTrackerXRotSpinner.getModel().setValue(new Double(cf.getXRot()));
      mTrackerYRotSpinner.getModel().setValue(new Double(cf.getYRot()));
      mTrackerZRotSpinner.getModel().setValue(new Double(cf.getZRot()));

      fireTransformChanged(mTrackerListeners, cf.getXRot(), cf.getYRot(),
                           cf.getZRot());
   }

   void mTrackerPosUnitsChooser_actionPerformed(ActionEvent actionEvent)
   {
      double conv_factor =
         mTranslationUnits[mTrackerPosUnitsChooser.getSelectedIndex()].toMeters;
      double x_pos =
         ((Number) mTrackerXPosField.getValue()).doubleValue() * conv_factor;
      double y_pos =
         ((Number) mTrackerXPosField.getValue()).doubleValue() * conv_factor;
      double z_pos =
         ((Number) mTrackerXPosField.getValue()).doubleValue() * conv_factor;

      mTrackerXPosField.setValue(new Double(x_pos));
      mTrackerYPosField.setValue(new Double(y_pos));
      mTrackerZPosField.setValue(new Double(z_pos));
   }

   void mTrackerXPosField_propertyChange(PropertyChangeEvent propertyChangeEvent)
   {
      if ( propertyChangeEvent.getPropertyName().equals("value") )
      {
         mElement.setProperty("pre_translate", 0, mTrackerXPosField.getValue());
      }
   }

   void mTrackerYPosField_propertyChange(PropertyChangeEvent propertyChangeEvent)
   {
      if ( propertyChangeEvent.getPropertyName().equals("value") )
      {
         mElement.setProperty("pre_translate", 1, mTrackerYPosField.getValue());
      }
   }

   void mTrackerZPosField_propertyChange(PropertyChangeEvent propertyChangeEvent)
   {
      if ( propertyChangeEvent.getPropertyName().equals("value") )
      {
         mElement.setProperty("pre_translate", 2, mTrackerZPosField.getValue());
      }
   }
}

class TransmitterTransformPanel_mTrackerXPosField_propertyChangeAdapter
   implements PropertyChangeListener
{
   private TransmitterTransformPanel adaptee;
   TransmitterTransformPanel_mTrackerXPosField_propertyChangeAdapter(
      TransmitterTransformPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void propertyChange(PropertyChangeEvent propertyChangeEvent)
   {
      adaptee.mTrackerXPosField_propertyChange(propertyChangeEvent);
   }
}

class TransmitterTransformPanel_mTrackerYPosField_propertyChangeAdapter
   implements PropertyChangeListener
{
   private TransmitterTransformPanel adaptee;
   TransmitterTransformPanel_mTrackerYPosField_propertyChangeAdapter(
      TransmitterTransformPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void propertyChange(PropertyChangeEvent propertyChangeEvent)
   {
      adaptee.mTrackerYPosField_propertyChange(propertyChangeEvent);
   }
}

class TransmitterTransformPanel_mTrackerZPosField_propertyChangeAdapter
   implements PropertyChangeListener
{
   private TransmitterTransformPanel adaptee;
   TransmitterTransformPanel_mTrackerZPosField_propertyChangeAdapter(
      TransmitterTransformPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void propertyChange(PropertyChangeEvent propertyChangeEvent)
   {
      adaptee.mTrackerZPosField_propertyChange(propertyChangeEvent);
   }
}

class TransmitterTransformPanel_mTrackerPosUnitsChooser_actionAdapter
   implements ActionListener
{
   private TransmitterTransformPanel adaptee;
   TransmitterTransformPanel_mTrackerPosUnitsChooser_actionAdapter(
      TransmitterTransformPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mTrackerPosUnitsChooser_actionPerformed(actionEvent);
   }
}

class TransmitterTransformPanel_mTrackerCoordChooser_actionAdapter
   implements ActionListener
{
   private TransmitterTransformPanel adaptee;
   TransmitterTransformPanel_mTrackerCoordChooser_actionAdapter(
      TransmitterTransformPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {

      adaptee.mTrackerCoordChooser_actionPerformed(actionEvent);
   }
}

class MeasurementUnit
{
   public MeasurementUnit(String name, double toMeters)
   {
      this.name     = name;
      this.toMeters = toMeters;
   }

   public String name;
   public double toMeters;
}

class TransmitterTransformPanel_mVrjCoordChooser_actionAdapter
   implements ActionListener
{
   private TransmitterTransformPanel adaptee;
   TransmitterTransformPanel_mVrjCoordChooser_actionAdapter(
      TransmitterTransformPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mVrjCoordChooser_actionPerformed(actionEvent);
   }
}

class CoordFrame
{
   CoordFrame(double xRot, double yRot, double zRot)
   {
      this(xRot, yRot, zRot, null);
   }

   CoordFrame(double xRot, double yRot, double zRot, ImageIcon icon)
   {
      this.xRot = xRot;
      this.yRot = yRot;
      this.zRot = zRot;
      this.icon = icon;
   }

   public double getXRot()
   {
      return xRot;
   }

   public double getYRot()
   {
      return yRot;
   }

   public double getZRot()
   {
      return zRot;
   }

   public ImageIcon getIcon()
   {
      return icon;
   }

   public void setIcon(ImageIcon icon)
   {
      this.icon = icon;
   }

   public String toString()
   {
      return "X: " + Math.toDegrees(getXRot()) + "\u00B0, " +
             "Y: " + Math.toDegrees(getYRot()) + "\u00B0, " +
             "Z: " + Math.toDegrees(getZRot()) + "\u00B0";
   }

   private double xRot;
   private double yRot;
   private double zRot;
   private ImageIcon icon;
}

class CoordFrameRenderer
   extends JLabel
   implements ListCellRenderer
{
   public CoordFrameRenderer(CoordFrame[] coordFrames)
   {
      this.mCoordFrames = coordFrames;
      setOpaque(true);
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

      if ( mCoordFrames[selected_index].getIcon() != null )
      {
         setIcon(mCoordFrames[selected_index].getIcon());
         setToolTipText(mCoordFrames[selected_index].toString());
      }
      else
      {
         setText(mCoordFrames[selected_index].toString());
         setFont(list.getFont());
      }

      return this;
   }

   private CoordFrame[] mCoordFrames;
}

class TransmitterTransformPanel_mTrackerManualRotButton_actionAdapter
   implements ActionListener
{
   private TransmitterTransformPanel adaptee;
   TransmitterTransformPanel_mTrackerManualRotButton_actionAdapter(
      TransmitterTransformPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mTrackerManualRotButton_actionPerformed(actionEvent);
   }
}

class TransmitterTransformPanel_mVrjCoordAdvButton_actionAdapter
   implements ActionListener
{
   private TransmitterTransformPanel adaptee;
   TransmitterTransformPanel_mVrjCoordAdvButton_actionAdapter(
      TransmitterTransformPanel adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mVrjCoordAdvButton_actionPerformed(actionEvent);
   }
}
