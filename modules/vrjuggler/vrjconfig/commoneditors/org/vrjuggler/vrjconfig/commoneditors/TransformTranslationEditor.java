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

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import javax.swing.*;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.ConfigContext;


public class TransformTranslationEditor
   extends JPanel
{
   public static final int TRANSMITTER = CoordinateFrameEditor.TRANSMITTER;
   public static final int SENSOR      = CoordinateFrameEditor.SENSOR;

   public TransformTranslationEditor(int coordinateType)
   {
      if ( coordinateType == TRANSMITTER )
      {
         mTranslateProp = EditorConstants.pre_translation_prop;
         mTrackerPosLabel.setText(
            "<html>Position of the tracker transmitter from VR Juggler origin (pre-translation)</html>");
      }
      else
      {
         mTranslateProp = EditorConstants.post_translation_prop;
         mTrackerPosLabel.setText(
            "<html>Position of the sensor from VR Juggler tracked origin (post-translation)</html>");
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

      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }
   }

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      if ( ! elt.getDefinition().getToken().equals(EditorConstants.position_transform_filter_type) )
      {
         throw new IllegalArgumentException("Cannot handle config element " +
                                            "of type '" +
                                            elt.getDefinition().getToken() +
                                            "'");
      }

      mElement = elt;

      mTrackerXPosField.setValue(elt.getProperty(mTranslateProp, 0));
      mTrackerYPosField.setValue(elt.getProperty(mTranslateProp, 1));
      mTrackerZPosField.setValue(elt.getProperty(mTranslateProp, 2));

      mTrackerXPosField.addPropertyChangeListener(new
         TransformTranslationEditor_mTrackerXPosField_propertyChangeAdapter(this));
      mTrackerYPosField.addPropertyChangeListener(new
         TransformTranslationEditor_mTrackerYPosField_propertyChangeAdapter(this));
      mTrackerZPosField.addPropertyChangeListener(new
         TransformTranslationEditor_mTrackerZPosField_propertyChangeAdapter(this));
   }

   private void jbInit()
      throws Exception
   {
      this.setLayout(mTranslationPanelLayout);
      mTrackerPosLabel.setLabelFor(mTrackerPosPanel);
      mTrackerPosPanel.setLayout(mTrackerPosPanelLayout);
      mTrackerXPosLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerXPosLabel.setLabelFor(mTrackerXPosField);
      mTrackerXPosLabel.setText("X:");
      mTrackerPosUnitsLabel.setLabelFor(mTrackerPosUnitsChooser);
      mTrackerPosUnitsLabel.setText("Measurement Units:");
      mTrackerXPosField.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerXPosField.setPreferredSize(new Dimension(75, 22));
      mTrackerYPosLabel.setLabelFor(mTrackerYPosField);
      mTrackerYPosLabel.setText("Y:");
      mTrackerYPosField.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerYPosField.setPreferredSize(new Dimension(75, 22));
      mTrackerZPosLabel.setLabelFor(mTrackerZPosField);
      mTrackerZPosLabel.setText("Z:");
      mTrackerZPosField.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerZPosField.setPreferredSize(new Dimension(75, 22));
      mTrackerPosUnitsChooser.setToolTipText(
         "Choose the units of the values entered below");
      mTrackerPosUnitsChooser.addActionListener(new
         TransformTranslationEditor_mTrackerPosUnitsChooser_actionAdapter(this));
      this.add(mTrackerPosLabel,
               new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
                                      GridBagConstraints.WEST,
                                      GridBagConstraints.BOTH,
                                      new Insets(0, 3, 0, 0), 0, 0));
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
      this.add(mTrackerPosPanel,
               new GridBagConstraints(0, 1, 1, 1, 1.0, 1.0,
                                      GridBagConstraints.CENTER,
                                      GridBagConstraints.BOTH,
                                      new Insets(0, 3, 0, 0), 0, 0));
   }

   private ConfigElement mElement;
   private MeasurementUnit[] mTranslationUnits = null;
   private String mTranslateProp;

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
   private GridBagLayout mTrackerPosPanelLayout = new GridBagLayout();
   private GridBagLayout mTranslationPanelLayout = new GridBagLayout();

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
         mElement.setProperty(mTranslateProp, 0, mTrackerXPosField.getValue());
      }
   }

   void mTrackerYPosField_propertyChange(PropertyChangeEvent propertyChangeEvent)
   {
      if ( propertyChangeEvent.getPropertyName().equals("value") )
      {
         mElement.setProperty(mTranslateProp, 1, mTrackerYPosField.getValue());
      }
   }

   void mTrackerZPosField_propertyChange(PropertyChangeEvent propertyChangeEvent)
   {
      if ( propertyChangeEvent.getPropertyName().equals("value") )
      {
         mElement.setProperty(mTranslateProp, 2, mTrackerZPosField.getValue());
      }
   }
}

class TransformTranslationEditor_mTrackerXPosField_propertyChangeAdapter
   implements PropertyChangeListener
{
   private TransformTranslationEditor adaptee;
   TransformTranslationEditor_mTrackerXPosField_propertyChangeAdapter(TransformTranslationEditor adaptee)
   {
      this.adaptee = adaptee;
   }

   public void propertyChange(PropertyChangeEvent propertyChangeEvent)
   {
      adaptee.mTrackerXPosField_propertyChange(propertyChangeEvent);
   }
}

class TransformTranslationEditor_mTrackerYPosField_propertyChangeAdapter
   implements PropertyChangeListener
{
   private TransformTranslationEditor adaptee;
   TransformTranslationEditor_mTrackerYPosField_propertyChangeAdapter(TransformTranslationEditor adaptee)
   {
      this.adaptee = adaptee;
   }

   public void propertyChange(PropertyChangeEvent propertyChangeEvent)
   {
      adaptee.mTrackerYPosField_propertyChange(propertyChangeEvent);
   }
}

class TransformTranslationEditor_mTrackerZPosField_propertyChangeAdapter
   implements PropertyChangeListener
{
   private TransformTranslationEditor adaptee;
   TransformTranslationEditor_mTrackerZPosField_propertyChangeAdapter(TransformTranslationEditor adaptee)
   {
      this.adaptee = adaptee;
   }

   public void propertyChange(PropertyChangeEvent propertyChangeEvent)
   {
      adaptee.mTrackerZPosField_propertyChange(propertyChangeEvent);
   }
}

class TransformTranslationEditor_mTrackerPosUnitsChooser_actionAdapter
   implements ActionListener
{
   private TransformTranslationEditor adaptee;
   TransformTranslationEditor_mTrackerPosUnitsChooser_actionAdapter(TransformTranslationEditor adaptee)
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
