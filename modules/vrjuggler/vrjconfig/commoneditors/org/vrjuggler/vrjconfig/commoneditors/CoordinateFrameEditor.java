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
import javax.swing.*;
import java.awt.Color;
import javax.swing.event.ListSelectionEvent;
import java.awt.event.ActionEvent;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.EventListenerList;
import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.vrjconfig.commoneditors.event.TransformChangeListener;
import org.vrjuggler.vrjconfig.commoneditors.event.TransformChangeEvent;
import java.awt.Insets;
import java.awt.event.ActionListener;
import java.awt.Component;


public class CoordinateFrameEditor
   extends JPanel
   implements ChangeListener
            , EditorConstants
{
   public static final int TRANSMITTER = 0;
   public static final int SENSOR      = 1;

   public CoordinateFrameEditor(int coordinateType)
   {
      mCoordinateType = coordinateType;
      String coord_name, rot_type;

      if ( coordinateType == TRANSMITTER )
      {
         coord_name = "Transmitter";
         rot_type   = "Pre-rotation";
      }
      else if ( coordinateType == SENSOR )
      {
         coord_name = "Sensor";
         rot_type   = "Post-rotation";
      }
      else
      {
         throw new IllegalArgumentException("");
      }

      mTrackerCoordChooserLabel.setText("<html>" + coord_name +
                                        " Coordinate Frame</html>");
      mTrackerAnglesLabel.setText("<html>" + coord_name + " Angles (" +
                                  rot_type + ")</html>");

      mCoordFrames[0]  = new CoordFrame(0.0f, 0.0f, 0.0f);
      mCoordFrames[1]  = new CoordFrame(90.0f, 0.0f, 0.0f);
      mCoordFrames[2]  = new CoordFrame(180.0f, 0.0f, 0.0f);
      mCoordFrames[3]  = new CoordFrame(180.0f, 0.0f, -90.0f);
      mCoordFrames[4]  = new CoordFrame(180.0f, 0.0f, 90.0f);
      mCoordFrames[5]  = new CoordFrame(0.0f, 180.0f, 0.0f);
      mCoordFrames[6]  = new CoordFrame(180.0f, -90.0f, 0.0f);
      mCoordFrames[7]  = new CoordFrame(0.0f, 0.0f, 90.0f);
      mCoordFrames[8]  = new CoordFrame(0.0f, 0.0f, 180.0f);
      mCoordFrames[9]  = new CoordFrame(0.0f, 0.0f, -90.0f);
      mCoordFrames[10] = new CoordFrame(90.0f, 0.0f, 90.0f);
      mCoordFrames[11] = new CoordFrame(90.0f, 0.0f, 180.0f);
      mCoordFrames[12] = new CoordFrame(90.0f, 0.0f, -90.0f);
      mCoordFrames[13] = new CoordFrame(90.0f, 90.0f, 0.0f);
      mCoordFrames[14] = new CoordFrame(-180.0f, 90.0f, 0.0f);
      mCoordFrames[15] = new CoordFrame(-90.0f, 90.0f, 0.0f);
      mCoordFrames[16] = new CoordFrame(0.0f, 90.0f, 0.0f);
      mCoordFrames[17] = new CoordFrame(-90.0f, 0.0f, 180.0f);
      mCoordFrames[18] = new CoordFrame(-90.0f, 0.0f, -90.0f);
      mCoordFrames[19] = new CoordFrame(-90.0f, 0.0f, 0.0f);
      mCoordFrames[20] = new CoordFrame(-90.0f, 0.0f, 90.0f);
      mCoordFrames[21] = new CoordFrame(90.0f, -90.0f, 0.0f);
      mCoordFrames[22] = new CoordFrame(0.0f, -90.0f, 0.0f);
      mCoordFrames[23] = new CoordFrame(-90.0f, -90.0f, 0.0f);

      // Create the renderers for mVrjCoordChooser and mTrackerCoordChooser.
      CoordFrameRenderer r0 = new CoordFrameRenderer(mCoordFrames);
      CoordFrameRenderer r1 = new CoordFrameRenderer(mCoordFrames);

      try
      {
         ClassLoader loader = getClass().getClassLoader();
         String img_base = EditorConstants.COMMON_IMG_ROOT;

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

         // Only set up preferred sizes if all the icons were loaded
         // successfully.
         int w = 70, h = 70;
         r0.setPreferredSize(new Dimension(w, h));
         r1.setPreferredSize(new Dimension(w, h));
      }
      // If any one icon failed to load, clear all the coordinate frame icons.
      catch(NullPointerException ex)
      {
         for ( int i = 0; i < mCoordFrames.length; ++i )
         {
            mCoordFrames[i].setIcon(null);
         }
      }

      // Set up the list models for the coordinate frame choosers.
      DefaultComboBoxModel vrj_model     = new DefaultComboBoxModel();
      DefaultComboBoxModel tracker_model = new DefaultComboBoxModel();

      for ( int i = 0; i < mCoordFrames.length; ++i )
      {
         vrj_model.addElement(new Integer(i));
         tracker_model.addElement(new Integer(i));
      }

      mVrjCoordChooser.setModel(vrj_model);
      mVrjCoordChooser.setRenderer(r0);
      mTrackerCoordChooser.setModel(tracker_model);
      mTrackerCoordChooser.setRenderer(r1);

      mTrackerXRotSpinner.setModel(new SpinnerNumberModel(0.0, -180.0,
                                                          180.0, 0.1));
      mTrackerYRotSpinner.setModel(new SpinnerNumberModel(0.0, -180.0,
                                                          180.0, 0.1));
      mTrackerZRotSpinner.setModel(new SpinnerNumberModel(0.0, -180.0,
                                                          180.0, 0.1));

      try
      {
         jbInit();
      }
      catch (Exception ex)
      {
         ex.printStackTrace();
      }
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
      mElement.setProperty(mRotateProp, value_index, source.getValue(),
                           mContext);
   }

   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      if ( ! elt.getDefinition().getToken().equals(POSITION_TRANSFORM_FILTER_TYPE) )
      {
         throw new IllegalArgumentException("Config element of type '" +
                                            elt.getDefinition().getToken() +
                                            "' is not allowed");
      }

      mContext = ctx;
      mElement = elt;

      if ( mCoordinateType == TRANSMITTER )
      {
         mRotateProp = PRE_ROTATION_PROPERTY;
      }
      else
      {
         mRotateProp = POST_ROTATION_PROPERTY;
      }

      float x_rot = ((Number) elt.getProperty(mRotateProp, 0)).floatValue();
      float y_rot = ((Number) elt.getProperty(mRotateProp, 1)).floatValue();
      float z_rot = ((Number) elt.getProperty(mRotateProp, 2)).floatValue();

      mTrackerXRotSpinner.setValue(new Float(x_rot));
      mTrackerYRotSpinner.setValue(new Float(y_rot));
      mTrackerZRotSpinner.setValue(new Float(z_rot));

      // This is slow, but I don't think that there is a better way to do
      // it.
      // NOTE: This must be done *after* jbInit() is called so that the
      // scroll pane will be updated to display the selected coordinate
      // frame.
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

      mVrjCoordChooser.setSelectedIndex(0);

      System.out.println("[TrackerTransformPanel.setConfig()] i == " + i);
      System.out.println("[TrackerTransformPanel.setConfig()] mCoordFrames.length == " + mCoordFrames.length);

      // If the search through mCoordFrames turned up no match, then we
      // will enable the manual rotation editing just to be friendly.
      if ( i == mCoordFrames.length )
      {
         mTrackerManualRotButton.setSelected(true);
      }

      // The change listener should be added after the spinners are fully
      // set up.  This will help avoid needless event handling on our part.
      mTrackerXRotSpinner.addChangeListener(this);
      mTrackerYRotSpinner.addChangeListener(this);
      mTrackerZRotSpinner.addChangeListener(this);
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

   public float getJugglerXRot()
   {
      return mCoordFrames[mVrjCoordChooser.getSelectedIndex()].getXRot();
   }

   public float getJugglerYRot()
   {
      return mCoordFrames[mVrjCoordChooser.getSelectedIndex()].getYRot();
   }

   public float getJugglerZRot()
   {
      return mCoordFrames[mVrjCoordChooser.getSelectedIndex()].getZRot();
   }

   public float getTrackerXRot()
   {
      float x_rot;

      if ( mTrackerManualRotButton.isSelected() )
      {
         x_rot = ((Number) mTrackerXRotSpinner.getValue()).floatValue();
      }
      else
      {
         x_rot = mCoordFrames[mTrackerCoordChooser.getSelectedIndex()].getXRot();
      }

      return x_rot;
   }

   public float getTrackerYRot()
   {
      float y_rot;

      if ( mTrackerManualRotButton.isSelected() )
      {
         y_rot = ((Number) mTrackerYRotSpinner.getValue()).floatValue();
      }
      else
      {
         y_rot = mCoordFrames[mTrackerCoordChooser.getSelectedIndex()].getYRot();
      }

      return y_rot;
   }

   public float getTrackerZRot()
   {
      float z_rot;

      if ( mTrackerManualRotButton.isSelected() )
      {
         z_rot = ((Number) mTrackerZRotSpinner.getValue()).floatValue();
      }
      else
      {
         z_rot = mCoordFrames[mTrackerCoordChooser.getSelectedIndex()].getZRot();
      }

      return z_rot;
   }

   protected void fireTransformChanged(EventListenerList listenerList,
                                       float xRot, float yRot, float zRot)
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
      mVrjCoordChooserLabel.setLabelFor(mVrjCoordChooser);
      mVrjCoordChooserLabel.setText("<html>VR Juggler Coordinate Frame</html>");
      mVrjCoordChooserLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mVrjCoordPanel.setLayout(mVrjCoordPanelLayout);
      mVrjCoordChooser.setEnabled(false);
      mVrjCoordChooser.addActionListener(new
         CoordinateFrameEditor_mVrjCoordChooser_actionAdapter(this));
      mVrjCoordAdvButton.setText("Advanced");
      mVrjCoordAdvButton.addActionListener(new
         CoordinateFrameEditor_mVrjCoordAdvButton_actionAdapter(this));
      mTrackerCoordPanel.setLayout(mTrackerCoordPanelLayout);
      mTrackerCoordChooserLabel.setLabelFor(mTrackerCoordChooser);
      mTrackerCoordChooserLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mTrackerAdvPanel.setLayout(mTrackerAdvPanelLayout);
      mTrackerAnglesLabel.setLabelFor(mTrackerAnglesPanel);
      mTrackerAnglesLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mTrackerAnglesPanel.setLayout(mTrackerAnglesPanelLayout);
      mTrackerXRotLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerXRotLabel.setLabelFor(mTrackerXRotSpinner);
      mTrackerXRotLabel.setText("X Rotation:");
      mTrackerXRotLabel.setForeground(Color.red);
      mTrackerYRotLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerYRotLabel.setLabelFor(mTrackerYRotSpinner);
      mTrackerYRotLabel.setText("Y Rotation:");
      mTrackerYRotLabel.setForeground(Color.green);
      mTrackerZRotLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mTrackerZRotLabel.setLabelFor(mTrackerZRotSpinner);
      mTrackerZRotLabel.setText("Z Rotation:");
      mTrackerZRotLabel.setForeground(Color.blue);
      mTrackerManualRotButton.setText("Manual Entry");
      mTrackerManualRotButton.addActionListener(new
         CoordinateFrameEditor_mTrackerManualRotButton_actionAdapter(this));
      mTrackerXRotSpinner.setEnabled(false);
      mTrackerYRotSpinner.setEnabled(false);
      mTrackerZRotSpinner.setEnabled(false);
      mTrackerCoordChooser.addActionListener(new
         CoordinateFrameEditor_mTrackerCoordChooser_actionAdapter(this));
      mVrjCoordPanel.add(mVrjCoordChooserLabel,
                         new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
                                                GridBagConstraints.CENTER,
                                                GridBagConstraints.HORIZONTAL,
                                                new Insets(0, 0, 0, 0), 0, 0));
      mVrjCoordPanel.add(mVrjCoordChooser,
                         new GridBagConstraints(0, 1, 1, 1, 1.0, 1.0,
                                                GridBagConstraints.CENTER,
                                                GridBagConstraints.BOTH,
                                                new Insets(0, 0, 0, 2), 0, 0));
      mVrjCoordPanel.add(mVrjCoordAdvButton,
                         new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0,
                                                GridBagConstraints.CENTER,
                                                GridBagConstraints.HORIZONTAL,
                                                new Insets(0, 0, 0, 0), 0, 0));
      mTrackerCoordPanel.add(mTrackerCoordChooserLabel,
                             new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
                                                    GridBagConstraints.CENTER,
                                                    GridBagConstraints.HORIZONTAL,
                                                    new Insets(0, 0, 0, 0),
                                                    0, 0));
      mTrackerCoordPanel.add(mTrackerCoordChooser,
                             new GridBagConstraints(0, 1, 1, 1, 1.0, 0.0,
                                                    GridBagConstraints.CENTER,
                                                    GridBagConstraints.BOTH,
                                                    new Insets(0, 0, 0, 2),
                                                    0, 0));
      mTrackerAnglesPanel.add(mTrackerXRotLabel,
                              new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
                                                     GridBagConstraints.EAST,
                                                     GridBagConstraints.NONE,
                                                     new Insets(0, 0, 0, 3),
                                                     0, 0));
      mTrackerAnglesPanel.add(mTrackerXRotSpinner,
                              new GridBagConstraints(1, 0, 1, 1, 1.0, 1.0,
                                                     GridBagConstraints.WEST,
                                                     GridBagConstraints.NONE,
                                                     new Insets(2, 0, 3, 0),
                                                     0, 0));
      mTrackerAnglesPanel.add(mTrackerYRotLabel,
                              new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0,
                                                     GridBagConstraints.EAST,
                                                     GridBagConstraints.NONE,
                                                     new Insets(0, 0, 0, 3),
                                                     0, 0));
      mTrackerAnglesPanel.add(mTrackerYRotSpinner,
                              new GridBagConstraints(1, 1, 1, 1, 1.0, 1.0,
                                                     GridBagConstraints.WEST,
                                                     GridBagConstraints.NONE,
                                                     new Insets(0, 0, 3, 0),
                                                     0, 0));
      mTrackerAnglesPanel.add(mTrackerZRotLabel,
                              new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0,
                                                     GridBagConstraints.EAST,
                                                     GridBagConstraints.NONE,
                                                     new Insets(0, 0, 0, 3),
                                                     0, 0));
      mTrackerAnglesPanel.add(mTrackerZRotSpinner,
                              new GridBagConstraints(1, 2, 1, 1, 1.0, 1.0,
                                                     GridBagConstraints.WEST,
                                                     GridBagConstraints.NONE,
                                                     new Insets(0, 0, 2, 0),
                                                     0, 0));
      mTrackerAdvPanel.add(mTrackerAnglesLabel,
                           new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0,
                                                  GridBagConstraints.CENTER,
                                                  GridBagConstraints.HORIZONTAL,
                                                  new Insets(0, 0, 3, 0),
                                                  0, 0));
      mTrackerAdvPanel.add(mTrackerAnglesPanel,
                           new GridBagConstraints(0, 1, 1, 1, 1.0, 1.0,
                                                  GridBagConstraints.CENTER,
                                                  GridBagConstraints.BOTH,
                                                  new Insets(0, 0, 0, 0),
                                                  0, 0));
      mTrackerAdvPanel.add(mTrackerManualRotButton,
                           new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0,
                                                  GridBagConstraints.CENTER,
                                                  GridBagConstraints.HORIZONTAL,
                                                  new Insets(3, 0, 0, 0),
                                                  0, 0));
      this.add(mVrjCoordPanel,
               new GridBagConstraints(0, 0, 1, 1, 1.0, 1.0,
                                      GridBagConstraints.NORTH,
                                      GridBagConstraints.HORIZONTAL,
                                      new Insets(0, 3, 0, 5), 0, 0));
      this.add(mTrackerCoordPanel,
               new GridBagConstraints(1, 0, 1, 1, 1.0, 1.0,
                                      GridBagConstraints.NORTH,
                                      GridBagConstraints.HORIZONTAL,
                                      new Insets(0, 0, 0, 5), 0, 0));
      this.add(mTrackerAdvPanel,
               new GridBagConstraints(2, 0, 1, 1, 1.0, 1.0,
                                      GridBagConstraints.CENTER,
                                      GridBagConstraints.BOTH,
                                      new Insets(0, 0, 0, 2),
                                      -66, -89));
   }

   private ConfigContext mContext;
   private ConfigElement mElement;
   private int mCoordinateType;
   private String mRotateProp;

   private CoordFrame[]      mCoordFrames      = new CoordFrame[24];
   private EventListenerList mJugglerListeners = new EventListenerList();
   private EventListenerList mTrackerListeners = new EventListenerList();

   private GridBagLayout mMainLayout = new GridBagLayout();
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
   private GridBagLayout mVrjCoordPanelLayout = new GridBagLayout();
   private GridBagLayout mTrackerCoordPanelLayout = new GridBagLayout();
   private GridBagLayout mTrackerAdvPanelLayout = new GridBagLayout();
   private GridBagLayout mTrackerAnglesPanelLayout = new GridBagLayout();

   void mVrjCoordAdvButton_actionPerformed(ActionEvent actionEvent)
   {
      mVrjCoordChooser.setEnabled(mVrjCoordAdvButton.isSelected());
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
      mTrackerXRotSpinner.getModel().setValue(new Float(cf.getXRot()));
      mTrackerYRotSpinner.getModel().setValue(new Float(cf.getYRot()));
      mTrackerZRotSpinner.getModel().setValue(new Float(cf.getZRot()));

      fireTransformChanged(mTrackerListeners, cf.getXRot(), cf.getYRot(),
                           cf.getZRot());
   }

   void mTrackerManualRotButton_actionPerformed(ActionEvent actionEvent)
   {
      boolean enabled = mTrackerManualRotButton.isSelected();
      mTrackerXRotSpinner.setEnabled(enabled);
      mTrackerYRotSpinner.setEnabled(enabled);
      mTrackerZRotSpinner.setEnabled(enabled);
   }
}

class CoordinateFrameEditor_mTrackerCoordChooser_actionAdapter
   implements ActionListener
{
   private CoordinateFrameEditor adaptee;
   CoordinateFrameEditor_mTrackerCoordChooser_actionAdapter(
      CoordinateFrameEditor adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mTrackerCoordChooser_actionPerformed(actionEvent);
   }
}

class CoordinateFrameEditor_mVrjCoordChooser_actionAdapter
   implements ActionListener
{
   private CoordinateFrameEditor adaptee;
   CoordinateFrameEditor_mVrjCoordChooser_actionAdapter(
      CoordinateFrameEditor adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mVrjCoordChooser_actionPerformed(actionEvent);
   }
}

class CoordinateFrameEditor_mTrackerManualRotButton_actionAdapter
   implements ActionListener
{
   private CoordinateFrameEditor adaptee;
   CoordinateFrameEditor_mTrackerManualRotButton_actionAdapter(
      CoordinateFrameEditor adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mTrackerManualRotButton_actionPerformed(actionEvent);
   }
}

class CoordinateFrameEditor_mVrjCoordAdvButton_actionAdapter
   implements ActionListener
{
   private CoordinateFrameEditor adaptee;
   CoordinateFrameEditor_mVrjCoordAdvButton_actionAdapter(
      CoordinateFrameEditor adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mVrjCoordAdvButton_actionPerformed(actionEvent);
   }
}

class CoordFrame
{
   CoordFrame(float xRot, float yRot, float zRot)
   {
      this(xRot, yRot, zRot, null);
   }

   CoordFrame(float xRot, float yRot, float zRot, ImageIcon icon)
   {
      this.xRot = xRot;
      this.yRot = yRot;
      this.zRot = zRot;
      this.icon = icon;
   }

   public float getXRot()
   {
      return xRot;
   }

   public float getYRot()
   {
      return yRot;
   }

   public float getZRot()
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
      return "X: " + getXRot() + "\u00B0, " +
             "Y: " + getYRot() + "\u00B0, " +
             "Z: " + getZRot() + "\u00B0";
   }

   private float xRot;
   private float yRot;
   private float zRot;
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
      setHorizontalAlignment(LEFT);
      setVerticalAlignment(CENTER);
   }

   public Component getListCellRendererComponent(JList list, Object value,
                                                 int index,
                                                 boolean isSelected,
                                                 boolean cellHasFocus)
   {
      int selected_index = ((Integer) value).intValue();
      
      // Make sure to disable JLabel whrn JList is disabled.
      setEnabled(list.isEnabled());
      
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
         setHorizontalTextPosition(RIGHT);
         setVerticalTextPosition(CENTER);
      }

      setText(mCoordFrames[selected_index].toString());
      setFont(list.getFont());

      return this;
   }

   private CoordFrame[] mCoordFrames;
}
