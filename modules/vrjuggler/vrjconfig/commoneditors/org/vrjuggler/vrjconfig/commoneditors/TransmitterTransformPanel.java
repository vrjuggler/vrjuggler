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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors;

import java.awt.*;
import javax.swing.*;
import javax.swing.border.TitledBorder;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;
import org.vrjuggler.vrjconfig.commoneditors.event.*;


public class TransmitterTransformPanel
   extends JPanel
   implements EditorConstants
{
   public TransmitterTransformPanel()
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

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      if ( elt.getDefinition().getToken().equals(POSITION_TRANSFORM_FILTER_TYPE) )
      {
         mContext = ctx;
         mElement = elt;

         elt.addConfigElementListener(new ElementListener());
         String dev_units = DEVICE_UNITS_PROPERTY;
         mSensorUnitsEditor =
            new PropertyEditorPanel(ctx, elt.getProperty(dev_units, 0),
                                    elt.getDefinition().getPropertyDefinition(dev_units),
                                    elt, 0, Color.white);

         String cust_scale = CUSTOM_SCALE_PROPERTY;
         mCustomUnitsEditor =
            new PropertyEditorPanel(ctx, elt.getProperty(cust_scale, 0),
                                    elt.getDefinition().getPropertyDefinition(cust_scale),
                                    elt, 0, Color.white);

         boolean enable_custom =
            ((Number) elt.getProperty(DEVICE_UNITS_PROPERTY, 0)).floatValue() == 0.0;
         mCustomUnitsEditor.setEnabled(enable_custom);

         mRotationPanel.setConfig(ctx, elt);
         mTranslationPanel.setConfig(ctx, elt);

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
      mTranslationBorder.setTitle("Position");
      mSensorUnitsPanel.setBorder(mTrackerUnitsBorder);
      mSensorUnitsPanel.setLayout(mSensorUnitsPanelLayout);
      mTrackerUnitsBorder.setTitle("Tracker Units");
      mSensorUnitsEditorPanel.setLayout(mSensorUnitsEditorPanelLayout);
      mSensorUnitsLabel.setLabelFor(mSensorUnitsEditor);
      mSensorUnitsLabel.setText("Sensor(s) Sample Units:");
      mCustomUnitsEditor.setEnabled(false);
      mCustomUnitsEditor.setToolTipText(
         "Set custom unit conversion (to meters) for sensor samples");
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
      this.add(mRotationPanel,
               new GridBagConstraints(0, 0, 1, 1, 1.0, 1.0,
                                      GridBagConstraints.CENTER,
                                      GridBagConstraints.BOTH,
                                      new Insets(0, 0, 0, 0), 0, 30));
      this.add(mTranslationPanel,
               new GridBagConstraints(0, 1, 1, 1, 1.0, 1.0,
                                      GridBagConstraints.CENTER,
                                      GridBagConstraints.HORIZONTAL,
                                      new Insets(0, 0, 2, 0), 0, 0));
      this.add(mSensorUnitsPanel,
               new GridBagConstraints(0, 2, 1, 1, 1.0, 1.0,
                                      GridBagConstraints.CENTER,
                                      GridBagConstraints.HORIZONTAL,
                                      new Insets(0, 0, 0, 0), 0, 0));
   }

   private ConfigContext mContext = null;
   private ConfigElement mElement = null;

   private CoordinateFrameEditor mRotationPanel =
      new CoordinateFrameEditor(CoordinateFrameEditor.TRANSMITTER);
   private TitledBorder mRotationBorder = new TitledBorder("");
   private TransformTranslationEditor mTranslationPanel =
      new TransformTranslationEditor(TransformTranslationEditor.TRANSMITTER);
   private TitledBorder mTranslationBorder = new TitledBorder("");
   private JPanel mSensorUnitsPanel = new JPanel();
   private TitledBorder mTrackerUnitsBorder = new TitledBorder("");
   private JPanel mSensorUnitsEditorPanel = new JPanel();
   private JLabel mSensorUnitsLabel = new JLabel();
   private PropertyEditorPanel mSensorUnitsEditor = null;
   private PropertyEditorPanel mCustomUnitsEditor = null;
   private GridBagLayout mSensorUnitsPanelLayout = new GridBagLayout();
   private GridBagLayout mSensorUnitsEditorPanelLayout = new GridBagLayout();
   private GridBagLayout mMainLayout = new GridBagLayout();

   private class ElementListener
      extends ConfigElementAdapter
   {
      public void propertyValueChanged(ConfigElementEvent evt)
      {
         if ( evt.getProperty().equals(DEVICE_UNITS_PROPERTY) )
         {
            Object value = mElement.getProperty(DEVICE_UNITS_PROPERTY, 0);
            boolean enable = ((Number) value).floatValue() == 0.0;
            mCustomUnitsEditor.setEnabled(enable);
         }
      }
   }
}
