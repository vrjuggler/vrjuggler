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
import javax.swing.*;
import javax.swing.border.TitledBorder;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;
import org.vrjuggler.vrjconfig.commoneditors.event.*;


public class TransmitterTransformPanel
   extends JPanel
{
   public TransmitterTransformPanel()
   {
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
   }

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      if ( elt.getDefinition().getToken().equals("position_transform_filter") )
      {
         mContext = ctx;
         mElement = elt;

         elt.addConfigElementListener(new ElementListener());
         mSensorUnitsEditor =
            new PropertyEditorPanel(ctx, elt.getProperty("device_units", 0),
                                    elt.getDefinition().getPropertyDefinition("device_units"),
                                    elt, 0, Color.white);

         mCustomUnitsEditor =
            new PropertyEditorPanel(ctx, elt.getProperty("custom_scale", 0),
                                    elt.getDefinition().getPropertyDefinition("custom_scale"),
                                    elt, 0, Color.white);

         boolean enable_custom =
            ((Number) elt.getProperty("device_units", 0)).floatValue() == 0.0;
         mCustomUnitsEditor.setEnabled(enable_custom);

         mTrackerXPosField.setValue(elt.getProperty("pre_translate", 0));
         mTrackerYPosField.setValue(elt.getProperty("pre_translate", 1));
         mTrackerZPosField.setValue(elt.getProperty("pre_translate", 2));

         mRotationPanel.setConfig(ctx, elt);

         try
         {
            jbInit();
         }
         catch(Exception ex)
         {
            ex.printStackTrace();
         }
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
      mRotationPanel.addJugglerTransformChangeListener(l);
   }

   public void removeJugglerTransformChangeListener(TransformChangeListener l)
   {
      mRotationPanel.removeJugglerTransformChangeListener(l);
   }

   public void addTrackerTransformChangeListener(TransformChangeListener l)
   {
      mRotationPanel.addTrackerTransformChangeListener(l);
   }

   public void removeTrackerTransformChangeListener(TransformChangeListener l)
   {
      mRotationPanel.removeTrackerTransformChangeListener(l);
   }

   public double getJugglerXRot()
   {
      return mRotationPanel.getJugglerXRot();
   }

   public double getJugglerYRot()
   {
      return mRotationPanel.getJugglerYRot();
   }

   public double getJugglerZRot()
   {
      return mRotationPanel.getJugglerZRot();
   }

   public double getTrackerXRot()
   {
      return mRotationPanel.getTrackerXRot();
   }

   public double getTrackerYRot()
   {
      return mRotationPanel.getTrackerYRot();
   }

   public double getTrackerZRot()
   {
      return mRotationPanel.getTrackerZRot();
   }

   private void jbInit()
      throws Exception
   {
      this.setLayout(mMainLayout);
      mRotationPanel.setBorder(mRotationBorder);
      mRotationBorder.setTitle("Orientation");
      mTranslationPanel.setBorder(mTranslationBorder);
      mTranslationPanel.setLayout(mTranslationPanelLayout);
      mTranslationBorder.setTitle("Position");
      mTrackerPosLabel.setLabelFor(mTrackerPosPanel);
      mTrackerPosLabel.setText(
         "<html>Position of the tracker transmitter from VR Juggler origin (pre-translation)</html>");
      mTrackerPosPanel.setLayout(mTrackerPosPanelLayout);
      mTrackerXPosLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerXPosLabel.setLabelFor(mTrackerXPosField);
      mTrackerXPosLabel.setText("X:");
      mTrackerPosUnitsLabel.setLabelFor(mTrackerPosUnitsChooser);
      mTrackerPosUnitsLabel.setText("Measurement Units:");
      mTrackerXPosField.addPropertyChangeListener(new
         TransmitterTransformPanel_mTrackerXPosField_propertyChangeAdapter(this));
      mTrackerXPosField.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerXPosField.setPreferredSize(new Dimension(75, 22));
      mTrackerYPosLabel.setLabelFor(mTrackerYPosField);
      mTrackerYPosLabel.setText("Y:");
      mTrackerYPosField.addPropertyChangeListener(new
         TransmitterTransformPanel_mTrackerYPosField_propertyChangeAdapter(this));
      mTrackerYPosField.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerYPosField.setPreferredSize(new Dimension(75, 22));
      mTrackerZPosLabel.setLabelFor(mTrackerZPosField);
      mTrackerZPosLabel.setText("Z:");
      mTrackerZPosField.addPropertyChangeListener(new
         TransmitterTransformPanel_mTrackerZPosField_propertyChangeAdapter(this));
      mTrackerZPosField.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerZPosField.setPreferredSize(new Dimension(75, 22));
      mSensorUnitsPanel.setBorder(mTrackerUnitsBorder);
      mSensorUnitsPanel.setLayout(mSensorUnitsPanelLayout);
      mTrackerUnitsBorder.setTitle("Tracker Units");
      mSensorUnitsEditorPanel.setLayout(mSensorUnitsEditorPanelLayout);
      mSensorUnitsLabel.setLabelFor(mSensorUnitsEditor);
      mSensorUnitsLabel.setText("Sensor(s) Sample Units:");
      mCustomUnitsEditor.setEnabled(false);
      mCustomUnitsEditor.setToolTipText(
         "Set custom unit conversion (to meters) for sensor samples");
      mTrackerPosUnitsChooser.setToolTipText(
         "Choose the units of the values entered below");
      mTrackerPosUnitsChooser.addActionListener(new
         TransmitterTransformPanel_mTrackerPosUnitsChooser_actionAdapter(this));
      mSensorUnitsEditor.setToolTipText(
         "Choose the units of samples collected from sensors");
      mSensorUnitsEditorPanel.add(mSensorUnitsLabel,
                                  new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
                                                         GridBagConstraints.EAST,
                                                         GridBagConstraints.NONE,
                                                         new Insets(0, 0, 0, 3),
                                                         0, 0));
      mSensorUnitsEditorPanel.add(mSensorUnitsEditor,
                                  new GridBagConstraints(1, 0, 1, 1, 1.0, 0.0,
                                                         GridBagConstraints.WEST,
                                                         GridBagConstraints.NONE,
                                                         new Insets(0, 0, 0, 0),
                                                         0, 0));
      mSensorUnitsEditorPanel.add(mCustomUnitsEditor,
                                  new GridBagConstraints(2, 0, 1, 1, 1.0, 0.0,
                                                         GridBagConstraints.WEST,
                                                         GridBagConstraints.NONE,
                                                         new Insets(0, 3, 0, 2),
                                                         50, 0));
      mSensorUnitsPanel.add(mSensorUnitsEditorPanel,
                            new GridBagConstraints(0, 0, 1, 1, 1.0, 1.0,
                                                   GridBagConstraints.WEST,
                                                   GridBagConstraints.NONE,
                                                   new Insets(0, 3, 0, 0),
                                                   0, 0));
      mTranslationPanel.add(mTrackerPosLabel,
                            new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
                                                   GridBagConstraints.WEST,
                                                   GridBagConstraints.BOTH,
                                                   new Insets(0, 3, 0, 0),
                                                   0, 0));
      mTrackerPosPanel.add(mTrackerPosUnitsLabel,
                           new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
                                                  GridBagConstraints.CENTER,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 3),
                                                  0, 0));
      mTrackerPosPanel.add(mTrackerPosUnitsChooser,
                           new GridBagConstraints(1, 0, 1, 1, 0.0, 0.0,
                                                  GridBagConstraints.WEST,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 0),
                                                  0, 0));
      mTrackerPosPanel.add(mTrackerXPosLabel,
                           new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0,
                                                  GridBagConstraints.EAST,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 3),
                                                  0, 0));
      mTrackerPosPanel.add(mTrackerXPosField,
                           new GridBagConstraints(1, 1, 1, 1, 1.0, 0.0,
                                                  GridBagConstraints.WEST,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 1),
                                                  0, 0));
      mTrackerPosPanel.add(mTrackerYPosLabel,
                           new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0,
                                                  GridBagConstraints.EAST,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 3),
                                                  0, 0));
      mTrackerPosPanel.add(mTrackerYPosField,
                           new GridBagConstraints(1, 2, 1, 1, 1.0, 0.0,
                                                  GridBagConstraints.WEST,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 1),
                                                  0, 0));
      mTrackerPosPanel.add(mTrackerZPosLabel,
                           new GridBagConstraints(0, 3, 1, 1, 0.0, 0.0,
                                                  GridBagConstraints.EAST,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 3),
                                                  0, 9));
      mTrackerPosPanel.add(mTrackerZPosField,
                           new GridBagConstraints(1, 3, 1, 1, 1.0, 0.0,
                                                  GridBagConstraints.WEST,
                                                  GridBagConstraints.NONE,
                                                  new Insets(0, 0, 0, 1),
                                                  0, 0));
      mTranslationPanel.add(mTrackerPosPanel,
                            new GridBagConstraints(0, 1, 1, 1, 1.0, 1.0,
                                                   GridBagConstraints.CENTER,
                                                   GridBagConstraints.BOTH,
                                                   new Insets(0, 3, 0, 0),
                                                   0, 0));
      this.add(mSensorUnitsPanel,
               new GridBagConstraints(0, 0, 1, 1, 1.0, 1.0,
                                      GridBagConstraints.CENTER,
                                      GridBagConstraints.HORIZONTAL,
                                      new Insets(0, 0, 0, 0), 0, 0));
      this.add(mRotationPanel,
               new GridBagConstraints(0, 1, 1, 1, 1.0, 1.0,
                                      GridBagConstraints.CENTER,
                                      GridBagConstraints.BOTH,
                                      new Insets(0, 0, 0, 0), 0, 30));
      this.add(mTranslationPanel,
               new GridBagConstraints(0, 2, 1, 1, 1.0, 1.0,
                                      GridBagConstraints.CENTER,
                                      GridBagConstraints.HORIZONTAL,
                                      new Insets(0, 0, 2, 0), 0, 0));
   }

   private ConfigContext mContext = null;
   private ConfigElement mElement = null;

   private MeasurementUnit[] mTranslationUnits = null;

   private CoordinateFrameEditor mRotationPanel =
      new CoordinateFrameEditor(CoordinateFrameEditor.TRANSMITTER);
   private TitledBorder mRotationBorder = new TitledBorder("");
   private JPanel mTranslationPanel = new JPanel();
   private TitledBorder mTranslationBorder = new TitledBorder("");
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
   private JPanel mSensorUnitsEditorPanel = new JPanel();
   private JLabel mSensorUnitsLabel = new JLabel();
   private PropertyEditorPanel mSensorUnitsEditor = null;
   private PropertyEditorPanel mCustomUnitsEditor = null;
   private GridBagLayout mSensorUnitsPanelLayout = new GridBagLayout();
   private GridBagLayout mSensorUnitsEditorPanelLayout = new GridBagLayout();
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
            mCustomUnitsEditor.setEnabled(enable);
         }
      }
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
