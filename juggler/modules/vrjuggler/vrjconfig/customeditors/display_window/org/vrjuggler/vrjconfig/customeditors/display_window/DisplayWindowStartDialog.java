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
import javax.swing.*;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.PlainDocument;
import info.clearthought.layout.*;
import javax.swing.border.*;

import org.vrjuggler.jccl.config.ConfigElement;


public class DisplayWindowStartDialog
   extends JDialog
{
   public DisplayWindowStartDialog(Dimension resolution)
      throws HeadlessException
   {
      super();
      this.setTitle("Basic Display Window Parameters");
      this.setModal(true);
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);

      mResolution = resolution;
      mVisualIdField.setDocument(new HexidecimalDocument());

      try
      {
         jbInit();
         setSpinnerModel(mRedDepthSpinner, 1, 1, 8);
         setSpinnerModel(mGreenDepthSpinner, 1, 1, 8);
         setSpinnerModel(mBlueDepthSpinner, 1, 1, 8);
         setSpinnerModel(mAlphaDepthSpinner, 1, 1, 8);
         setSpinnerModel(mDepthBufferSpinner, 1, 1, 32);

         // Validate the default values for the various text fields.
         validateUserInput();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      this.setResizable(false);
      this.pack();
   }

   public DisplayWindowStartDialog(ConfigElement winElt, Dimension resolution)
      throws HeadlessException
   {
      super();
      this.setTitle(winElt.getName() + " Properties");
      this.setModal(true);
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);

      mResolution = resolution;
      mVisualIdField.setDocument(new HexidecimalDocument());

      try
      {
         jbInit();

         mNameField.setText(winElt.getName());

         ConfigElement fb_cfg =
            (ConfigElement) winElt.getProperty("frame_buffer_config", 0);

         int val;
         val = ((Integer) fb_cfg.getProperty("red_size", 0)).intValue();
         setSpinnerModel(mRedDepthSpinner, val, 1, 8);
         val = ((Integer) fb_cfg.getProperty("green_size", 0)).intValue();
         setSpinnerModel(mGreenDepthSpinner, val, 1, 8);
         val = ((Integer) fb_cfg.getProperty("blue_size", 0)).intValue();
         setSpinnerModel(mBlueDepthSpinner, val, 1, 8);
         val = ((Integer) fb_cfg.getProperty("alpha_size", 0)).intValue();
         setSpinnerModel(mAlphaDepthSpinner, val, 1, 8);
         val = ((Integer) fb_cfg.getProperty("depth_buffer_size", 0)).intValue();
         setSpinnerModel(mDepthBufferSpinner, val, 1, 32);

         mVisualIdField.setText(((Integer) fb_cfg.getProperty("visual_id", 0)).toString());
         mFSAACheckbox.setSelected(fb_cfg.getProperty("fsaa_enable", 0).equals(Boolean.TRUE));

         mPositionXField.setValue(winElt.getProperty("origin", 0));
         mPositionYField.setValue(winElt.getProperty("origin", 1));
         mWidthField.setValue(winElt.getProperty("size", 0));
         mHeightField.setValue(winElt.getProperty("size", 1));

         mStereoCheckbox.setSelected(winElt.getProperty("stereo", 0).equals(Boolean.TRUE));
         mBorderCheckbox.setSelected(winElt.getProperty("border", 0).equals(Boolean.TRUE));
         mHideMouseCheckbox.setSelected(winElt.getProperty("hide_mouse", 0).equals(Boolean.TRUE));
         mEventSourceCheckbox.setSelected(winElt.getProperty("act_as_event_source", 0).equals(Boolean.TRUE));

         // Validate the default values for the various text fields.
         validateUserInput();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      this.setResizable(false);
      this.pack();
   }

   private void setSpinnerModel(JSpinner spinner, int value, int min, int max)
   {
      SpinnerNumberModel model =
         new SpinnerNumberModel(new Integer(value), new Integer(min),
                                new Integer(max), new Integer(1));
      spinner.setModel(model);
   }

   public int getStatus()
   {
      return status;
   }

   public String getDisplayWindowTitle()
   {
      return mNameField.getText();
   }

   public Rectangle getDisplayWindowBounds()
   {
      int x = ((Integer) mPositionXField.getValue()).intValue();
      int y = ((Integer) mPositionYField.getValue()).intValue();
      int width = ((Integer) mWidthField.getValue()).intValue();
      int height = ((Integer) mHeightField.getValue()).intValue();
      return new Rectangle(x, y, width, height);
   }

   public Integer getVisualID()
   {
      return Integer.valueOf(mVisualIdField.getText());
   }

   public Integer getRedDepth()
   {
      return (Integer) mRedDepthSpinner.getModel().getValue();
   }

   public Integer getGreenDepth()
   {
      return (Integer) mGreenDepthSpinner.getModel().getValue();
   }

   public Integer getBlueDepth()
   {
      return (Integer) mBlueDepthSpinner.getModel().getValue();
   }

   public Integer getAlphaDepth()
   {
      return (Integer) mAlphaDepthSpinner.getModel().getValue();
   }

   public Integer getDepthBufferSize()
   {
      return (Integer) mDepthBufferSpinner.getModel().getValue();
   }

   public Boolean getFSAAEnable()
   {
      return (mFSAACheckbox.isSelected() ? Boolean.TRUE : Boolean.FALSE);
   }

   public Boolean hasBorder()
   {
      return (mBorderCheckbox.isSelected() ? Boolean.TRUE : Boolean.FALSE);
   }

   public Boolean inStereo()
   {
      return (mStereoCheckbox.isSelected() ? Boolean.TRUE : Boolean.FALSE);
   }

   public Boolean hideMouse()
   {
      return (mHideMouseCheckbox.isSelected() ? Boolean.TRUE : Boolean.FALSE);
   }

   public Boolean isEventSource()
   {
      return (mEventSourceCheckbox.isSelected() ? Boolean.TRUE : Boolean.FALSE);
   }

   public static final int OK_OPTION     = JOptionPane.OK_OPTION;
   public static final int CANCEL_OPTION = JOptionPane.CANCEL_OPTION;
   public static final int CLOSED_OPTION = JOptionPane.CLOSED_OPTION;

   protected void processWindowEvent(WindowEvent e)
   {
      if (e.getID() == WindowEvent.WINDOW_CLOSING)
      {
         status = CLOSED_OPTION;
         dispose();
      }

      super.processWindowEvent(e);
   }

   private void jbInit() throws Exception
   {
      double main_size[][] =
         {{TableLayout.MINIMUM, 5, TableLayout.FILL},
          {TableLayout.PREFERRED, TableLayout.PREFERRED, TableLayout.PREFERRED,
           TableLayout.PREFERRED}};
      mMainLayout = new TableLayout(main_size);

      double bounds_size[][] =
         {{TableLayout.MINIMUM, TableLayout.MINIMUM},
          {TableLayout.PREFERRED, TableLayout.PREFERRED}};
      mBoundsPanelLayout = new TableLayout(bounds_size);

      double fb_size[][] = {{TableLayout.MINIMUM, TableLayout.FILL, 5},
                            {TableLayout.PREFERRED}};
      mFrameBufferPanelLayout = new TableLayout(fb_size);

      double fb_settings_size[][] =
         {{TableLayout.FILL, TableLayout.MINIMUM},
          {TableLayout.PREFERRED, TableLayout.PREFERRED, TableLayout.PREFERRED,
           TableLayout.PREFERRED, TableLayout.PREFERRED, TableLayout.PREFERRED,
           TableLayout.PREFERRED}};
      mFBSettingsLayout = new TableLayout(fb_settings_size);

      double window_props_size[][] =
         {{TableLayout.FILL},
          {TableLayout.PREFERRED, TableLayout.PREFERRED, TableLayout.PREFERRED,
           TableLayout.PREFERRED}};
      mWindowPropsPanelLayout = new TableLayout(window_props_size);

      mBoundsPanelBorder =
         new TitledBorder(BorderFactory.createEtchedBorder(Color.white,
                                                           new Color(161, 161, 161)),
                                                           "Window Bounds");
      mFrameBufferPanelBorder =
         new TitledBorder(BorderFactory.createEtchedBorder(Color.white,
                                                           new Color(142, 142, 142)),
                                                           "Visual Quality Settings");
      mWindowPropsPanelBorder =
         new TitledBorder(BorderFactory.createEtchedBorder(Color.white,
                                                           new Color(142, 142, 142)),
                                                           "Window Behavior Settings");
      mFBSettingsPanelBorder =
         new EtchedBorder(EtchedBorder.RAISED, Color.white,
                          new Color(142, 142, 142));
      mMainPanel.setLayout(mMainLayout);
      mNameLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mNameLabel.setLabelFor(mNameField);
      mNameLabel.setText("Window Name");
      mNameField.setMinimumSize(new Dimension(60, 21));
      mNameField.setPreferredSize(new Dimension(60, 21));
      mNameField.setText("Display Window");
/*
      mNameField.addFocusListener(new FocusAdapter() {
         public void focusLost(FocusEvent e)
         {
            validateUserInput();
         }
      });
*/
      mSizeLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mSizeLabel.setLabelFor(mSizePanel);
      mSizeLabel.setText("Size");
      mPositionLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mPositionLabel.setLabelFor(mPositionPanel);
      mPositionLabel.setText("Position");
      mWidthField.setMinimumSize(new Dimension(40, 21));
      mWidthField.setPreferredSize(new Dimension(40, 21));
      mWidthField.setValue(new Integer(200));
      mWidthField.setHorizontalAlignment(SwingConstants.TRAILING);
/*
      mWidthField.addFocusListener(new FocusAdapter() {
         public void focusLost(FocusEvent e)
         {
            validateUserInput();
         }
      });
*/
      mSizeXLabel.setText("\u00D7");
      mHeightField.setMinimumSize(new Dimension(40, 21));
      mHeightField.setPreferredSize(new Dimension(40, 21));
      mHeightField.setValue(new Integer(200));
/*
      mHeightField.addFocusListener(new FocusAdapter() {
         public void focusLost(FocusEvent e)
         {
            validateUserInput();
         }
      });
*/
      mPositionXField.setMinimumSize(new Dimension(40, 21));
      mPositionXField.setPreferredSize(new Dimension(40, 21));
      mPositionXField.setValue(new Integer(0));
      mPositionXField.setHorizontalAlignment(SwingConstants.TRAILING);
/*
      mPositionXField.addFocusListener(new FocusAdapter() {
         public void focusLost(FocusEvent e)
         {
            validateUserInput();
         }
      });
*/
      mPositionXLabel.setText("\u00D7");
      mPositionYField.setMinimumSize(new Dimension(40, 21));
      mPositionYField.setPreferredSize(new Dimension(40, 21));
      mPositionYField.setValue(new Integer(0));
/*
      mPositionYField.addFocusListener(new FocusAdapter() {
         public void focusLost(FocusEvent e)
         {
            validateUserInput();
         }
      });
*/
      mFBSettingsLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mFBSettingsLabel.setLabelFor(mFBSettingsPanel);
      mFBSettingsLabel.setText("OpenGL Frame Buffer Settings");
      mFBSettingsPanel.setLayout(mFBSettingsLayout);
      mRedDepthLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mRedDepthLabel.setLabelFor(mRedDepthSpinner);
      mRedDepthLabel.setText("Red Depth");
      mGreenDepthLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mGreenDepthLabel.setLabelFor(mGreenDepthSpinner);
      mGreenDepthLabel.setText("Green Depth");
      mBlueDepthLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mBlueDepthLabel.setLabelFor(mBlueDepthSpinner);
      mBlueDepthLabel.setText("Blue Depth");
      mAlphaDepthLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mAlphaDepthLabel.setLabelFor(mAlphaDepthSpinner);
      mAlphaDepthLabel.setText("Alpha Depth");
      mDepthBufferLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mDepthBufferLabel.setLabelFor(mDepthBufferSpinner);
      mDepthBufferLabel.setText("Depth Buffer Size");
//      mMainPanel.setMinimumSize(new Dimension(435, 350));
//      mMainPanel.setPreferredSize(new Dimension(435, 350));
      mBorderCheckbox.setSelected(true);
      mBorderCheckbox.setText("Has border");
      mStereoCheckbox.setText("Render in stereo");
      mHideMouseCheckbox.setText("Hide mouse inside window");
      mEventSourceCheckbox.setText("Act as an event source");
      mOkButton.setEnabled(false);
      mOkButton.setMnemonic('0');
      mOkButton.setSelected(false);
      mOkButton.setText("OK");
      mOkButton.addActionListener(new DisplayWindowStartDialog_mOkButton_actionAdapter(this));
      mCancelButton.setMnemonic('0');
      mCancelButton.setText("Cancel");
      mCancelButton.addActionListener(new DisplayWindowStartDialog_mCancelButton_actionAdapter(this));
      mHelpButton.setEnabled(false);
      mHelpButton.setMnemonic('0');
      mHelpButton.setText("Help");
      mHelpButton.addActionListener(new DisplayWindowStartDialog_mHelpButton_actionAdapter(this));
      mFSAACheckbox.setText("Enable full-screen anti-aliasing");
      mBoundsPanel.setLayout(mBoundsPanelLayout);
      mBoundsPanel.setBorder(mBoundsPanelBorder);
      mFrameBufferPanel.setLayout(mFrameBufferPanelLayout);
      mFrameBufferPanel.setBorder(mFrameBufferPanelBorder);
      mWindowPropsPanel.setLayout(mWindowPropsPanelLayout);
      mWindowPropsPanel.setBorder(mWindowPropsPanelBorder);
      mFBSettingsPanel.setBorder(mFBSettingsPanelBorder);
      mButtonPanel.setLayout(mButtonPanelLayout);
      mButtonPanelLayout.setAlignment(FlowLayout.RIGHT);
      mVisualIdLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mVisualIdLabel.setLabelFor(mVisualIdField);
      mVisualIdLabel.setText("Visual ID");
      mVisualIdField.setMinimumSize(new Dimension(19, 20));
      mVisualIdField.setPreferredSize(new Dimension(28, 20));
      mVisualIdField.setToolTipText("Enter a GLX or WGL visual ID in hexadecimal notation or -1 to ignore");
      mVisualIdField.setText("-1");
      mVisualIdField.addActionListener(new DisplayWindowStartDialog_mVisualIdField_actionAdapter(this));
      mVisualIdField.addFocusListener(new DisplayWindowStartDialog_mVisualIdField_focusAdapter(this));
      this.getContentPane().add(mMainPanel, BorderLayout.CENTER);
      mMainPanel.add(mNameLabel, new TableLayoutConstraints(0, 0, 0, 0,
                                                            TableLayout.RIGHT,
                                                            TableLayout.CENTER));
      mMainPanel.add(mNameField, new TableLayoutConstraints(2, 0, 2, 0,
                                                            TableLayout.FULL,
                                                            TableLayout.CENTER));
      mMainPanel.add(mBoundsPanel, new TableLayoutConstraints(0, 1, 2, 1,
                                                              TableLayout.FULL,
                                                              TableLayout.FULL));
      mBoundsPanel.add(mSizeLabel, new TableLayoutConstraints(0, 0, 0, 0,
                                                            TableLayout.RIGHT,
                                                            TableLayout.CENTER));
      mBoundsPanel.add(mSizePanel, new TableLayoutConstraints(1, 0, 1, 0,
                                                            TableLayout.LEFT,
                                                            TableLayout.CENTER));
      mBoundsPanel.add(mPositionLabel,
                       new TableLayoutConstraints(0, 1, 0, 1,
                                                  TableLayout.RIGHT,
                                                  TableLayout.CENTER));
      mBoundsPanel.add(mPositionPanel,
                       new TableLayoutConstraints(1, 1, 1, 1,
                                                  TableLayout.LEFT,
                                                  TableLayout.CENTER));
      mPositionPanel.add(mPositionXField, null);
      mPositionPanel.add(mPositionXLabel, null);
      mPositionPanel.add(mPositionYField, null);
      mSizePanel.add(mWidthField, null);
      mSizePanel.add(mSizeXLabel, null);
      mSizePanel.add(mHeightField, null);
      mFrameBufferPanel.setMinimumSize(new Dimension(450, 180));
      mFrameBufferPanel.setPreferredSize(new Dimension(450, 180));
//      mFBSettingsPanel.setMinimumSize(new Dimension(400, 100));
//      mFBSettingsPanel.setPreferredSize(new Dimension(400, 100));
      mMainPanel.add(mFrameBufferPanel,
                     new TableLayoutConstraints(0, 2, 2, 2,
                                                TableLayout.FULL,
                                                TableLayout.FULL));
      mFrameBufferPanel.add(mFBSettingsLabel,
                            new TableLayoutConstraints(0, 0, 0, 0,
                                                       TableLayout.RIGHT,
                                                       TableLayout.CENTER));
      mFrameBufferPanel.add(mFBSettingsPanel,
                            new TableLayoutConstraints(1, 0, 1, 0,
                                                       TableLayout.RIGHT,
                                                       TableLayout.FULL));
      mFBSettingsPanel.add(mVisualIdLabel,
                           new TableLayoutConstraints(0, 0, 0, 0,
                                                      TableLayout.FULL,
                                                      TableLayout.CENTER));
      mFBSettingsPanel.add(mVisualIdField,
                           new TableLayoutConstraints(1, 0, 1, 0,
                                                      TableLayout.FULL,
                                                      TableLayout.CENTER));
      mFBSettingsPanel.add(mRedDepthLabel,
                           new TableLayoutConstraints(0, 1, 0, 1,
                                                      TableLayout.FULL,
                                                      TableLayout.CENTER));
      mFBSettingsPanel.add(mRedDepthSpinner,
                           new TableLayoutConstraints(1, 1, 1, 1,
                                                      TableLayout.FULL,
                                                      TableLayout.CENTER));
      mFBSettingsPanel.add(mGreenDepthLabel,
                           new TableLayoutConstraints(0, 2, 0, 2,
                                                      TableLayout.FULL,
                                                      TableLayout.CENTER));
      mFBSettingsPanel.add(mGreenDepthSpinner,
                           new TableLayoutConstraints(1, 2, 1, 2,
                                                      TableLayout.FULL,
                                                      TableLayout.CENTER));
      mFBSettingsPanel.add(mBlueDepthLabel,
                           new TableLayoutConstraints(0, 3, 0, 3,
                                                      TableLayout.FULL,
                                                      TableLayout.CENTER));
      mFBSettingsPanel.add(mBlueDepthSpinner,
                           new TableLayoutConstraints(1, 3, 1, 3,
                                                      TableLayout.FULL,
                                                      TableLayout.CENTER));
      mFBSettingsPanel.add(mAlphaDepthLabel,
                           new TableLayoutConstraints(0, 4, 0, 4,
                                                      TableLayout.FULL,
                                                      TableLayout.CENTER));
      mFBSettingsPanel.add(mAlphaDepthSpinner,
                           new TableLayoutConstraints(1, 4, 1, 4,
                                                      TableLayout.FULL,
                                                      TableLayout.CENTER));
      mFBSettingsPanel.add(mDepthBufferLabel,
                           new TableLayoutConstraints(0, 5, 0, 5,
                                                      TableLayout.FULL,
                                                      TableLayout.CENTER));
      mFBSettingsPanel.add(mDepthBufferSpinner,
                           new TableLayoutConstraints(1, 5, 1, 5,
                                                      TableLayout.FULL,
                                                      TableLayout.CENTER));
      mFBSettingsPanel.add(mFSAACheckbox,
                           new TableLayoutConstraints(0, 6, 1, 6,
                                                      TableLayout.LEFT,
                                                      TableLayout.CENTER));
      mMainPanel.add(mWindowPropsPanel,
                     new TableLayoutConstraints(0, 3, 2, 3,
                                                TableLayout.FULL,
                                                TableLayout.FULL));
      mWindowPropsPanel.add(mBorderCheckbox,
                     new TableLayoutConstraints(0, 0, 0, 0,
                                                TableLayout.LEFT,
                                                TableLayout.CENTER));
      mWindowPropsPanel.add(mStereoCheckbox,
                     new TableLayoutConstraints(0, 1, 0, 1,
                                                TableLayout.LEFT,
                                                TableLayout.CENTER));
      mWindowPropsPanel.add(mHideMouseCheckbox,
                     new TableLayoutConstraints(0, 2, 0, 2,
                                                TableLayout.LEFT,
                                                TableLayout.CENTER));
      mWindowPropsPanel.add(mEventSourceCheckbox,
                     new TableLayoutConstraints(0, 3, 0, 3,
                                                TableLayout.LEFT,
                                                TableLayout.CENTER));
      this.getContentPane().add(mButtonPanel, BorderLayout.SOUTH);
      mButtonPanel.add(mOkButton, null);
      mButtonPanel.add(mCancelButton, null);
      mButtonPanel.add(mHelpButton, null);
   }

   private int status = CANCEL_OPTION;
   private Dimension mResolution = null;

   private JPanel mMainPanel = new JPanel();
   private TableLayout mMainLayout = null;
   private JLabel mNameLabel = new JLabel();
   private JTextField mNameField = new JTextField();
   private JPanel mBoundsPanel = new JPanel();
   private TableLayout mBoundsPanelLayout = null;
   private TitledBorder mBoundsPanelBorder;
   private JLabel mSizeLabel = new JLabel();
   private JPanel mSizePanel = new JPanel();
   private JLabel mPositionLabel = new JLabel();
   private JPanel mPositionPanel = new JPanel();
   private JFormattedTextField mWidthField = new JFormattedTextField();
   private JLabel mSizeXLabel = new JLabel();
   private JFormattedTextField mHeightField = new JFormattedTextField();
   private JFormattedTextField mPositionXField = new JFormattedTextField();
   private JLabel mPositionXLabel = new JLabel();
   private JFormattedTextField mPositionYField = new JFormattedTextField();
   private JPanel mFrameBufferPanel = new JPanel();
   private TableLayout mFrameBufferPanelLayout = null;
   private TitledBorder mFrameBufferPanelBorder;
   private JLabel mFBSettingsLabel = new JLabel();
   private JPanel mFBSettingsPanel = new JPanel();
   private Border mFBSettingsPanelBorder;
   private TableLayout mFBSettingsLayout = null;
   private JLabel mVisualIdLabel = new JLabel();
   private JTextField mVisualIdField = new JTextField();
   private JLabel mRedDepthLabel = new JLabel();
   private JSpinner mRedDepthSpinner = new JSpinner();
   private JLabel mGreenDepthLabel = new JLabel();
   private JSpinner mGreenDepthSpinner = new JSpinner();
   private JLabel mBlueDepthLabel = new JLabel();
   private JSpinner mBlueDepthSpinner = new JSpinner();
   private JLabel mAlphaDepthLabel = new JLabel();
   private JSpinner mAlphaDepthSpinner = new JSpinner();
   private JLabel mDepthBufferLabel = new JLabel();
   private JSpinner mDepthBufferSpinner = new JSpinner();
   private JCheckBox mFSAACheckbox = new JCheckBox();
   private JPanel mWindowPropsPanel = new JPanel();
   private TableLayout mWindowPropsPanelLayout = null;
   private TitledBorder mWindowPropsPanelBorder;
   private JCheckBox mBorderCheckbox = new JCheckBox();
   private JCheckBox mStereoCheckbox = new JCheckBox();
   private JCheckBox mHideMouseCheckbox = new JCheckBox();
   private JCheckBox mEventSourceCheckbox = new JCheckBox();
   private JPanel mButtonPanel = new JPanel();
   private JButton mOkButton = new JButton();
   private JButton mCancelButton = new JButton();
   private JButton mHelpButton = new JButton();
   private FlowLayout mButtonPanelLayout = new FlowLayout();

   void okPressed(ActionEvent e)
   {
      status = OK_OPTION;
      dispose();
   }

   void cancelPressed(ActionEvent e)
   {
      status = CANCEL_OPTION;
      dispose();
   }

   void helpPressed(ActionEvent e)
   {

   }

   private void validateUserInput()
   {
      boolean name_valid, width_valid, height_valid, x_pos_valid, y_pos_valid;

      // The name cannot be an empty string.
      name_valid   = ! mNameField.getText().equals("");
      width_valid  = ((Integer) mWidthField.getValue()).intValue() > 0;
      height_valid = ((Integer) mHeightField.getValue()).intValue() > 0;
      x_pos_valid  = ((Integer) mPositionXField.getValue()).intValue() >= 0;
      y_pos_valid  = ((Integer) mPositionYField.getValue()).intValue() >= 0;

      if ( name_valid && width_valid && height_valid && x_pos_valid &&
           y_pos_valid )
      {
         mOkButton.setEnabled(true);
         validateWindowBounds();
      }
      else
      {
         mOkButton.setEnabled(false);
      }
   }

   /**
    * Ensures that the bounds of the display window are within the managed
    * area of the desktop.  If this is not the case, user confirmation is
    * requested.
    *
    * @param e DocumentEvent
    */
   private void validateWindowBounds(/*DocumentEvent e*/)
   {
      int window_width  = Integer.parseInt(mWidthField.getText());
      int window_height = Integer.parseInt(mHeightField.getText());
      int window_pos_x  = Integer.parseInt(mPositionXField.getText());
      int window_pos_y  = Integer.parseInt(mPositionYField.getText());

      if ( window_pos_x < 0 || window_pos_y < 0 ||
           window_width + window_pos_x > mResolution.width ||
           window_height + window_pos_y > mResolution.height )
      {
         int answer =
            JOptionPane.showConfirmDialog(this,
                                          "Do you want the window to open outside the managed area?",
                                          "Display Window Outside Managed Area",
                                          JOptionPane.YES_NO_OPTION,
                                          JOptionPane.WARNING_MESSAGE);

         if ( answer == JOptionPane.YES_OPTION )
         {
         }
         else
         {
         }
      }
   }

   void mVisualIdField_focusLost(FocusEvent e)
   {
      validateVisualID();
   }

   void mVisualIdField_actionPerformed(ActionEvent e)
   {
      validateVisualID();
   }

   private void validateVisualID()
   {
      String id = mVisualIdField.getText();

      if ( ! id.equals("-1") && ! id.startsWith("0x") )
      {
         mVisualIdField.setText("0x" + id);
      }

      // If the value in the visual ID field is "-1", then the individual
      // frame buffer settings editors must be active.  Otherwise, they will
      // be deactivated since the values will be ignored at run time.
      boolean enabled = mVisualIdField.getText().equals("-1");
      mRedDepthLabel.setEnabled(enabled);
      mRedDepthSpinner.setEnabled(enabled);
      mGreenDepthLabel.setEnabled(enabled);
      mGreenDepthSpinner.setEnabled(enabled);
      mBlueDepthLabel.setEnabled(enabled);
      mBlueDepthSpinner.setEnabled(enabled);
      mAlphaDepthLabel.setEnabled(enabled);
      mAlphaDepthSpinner.setEnabled(enabled);
      mDepthBufferLabel.setEnabled(enabled);
      mDepthBufferSpinner.setEnabled(enabled);
      mFSAACheckbox.setEnabled(enabled);
   }

   private static class HexidecimalDocument extends PlainDocument
   {
      public void insertString(int offset, String string,
                               AttributeSet attributes)
      {
         if ( string == null )
         {
            return;
         }
         else
         {
            String new_value = "";
            int length = getLength();

            if ( length == 0 )
            {
               new_value = string;
            }
            else
            {
               try
               {
                  String currentContent = getText(0, length);
                  StringBuffer currentBuffer = new StringBuffer(currentContent);
                  currentBuffer.insert(offset, string);
                  new_value = currentBuffer.toString();
               }
               catch (BadLocationException locEx)
               {
                  locEx.printStackTrace();
               }
            }

            try
            {
               // If the user is not in the process of entering "-1" or "0x",
               // validate the input as a base 16 integer.
               if ( ! (new_value.equals("-") || new_value.equals("-1") ||
                       new_value.equals("0x")) )
               {
                  if ( new_value.startsWith("0x") )
                  {
                     new_value = new_value.substring(2);
                  }

                  Integer.parseInt(new_value, 16);
               }

               try
               {
                  super.insertString(offset, string, attributes);
               }
               catch (BadLocationException locEx)
               {
                  locEx.printStackTrace();
               }
            }
            catch (NumberFormatException exception)
            {
               Toolkit.getDefaultToolkit().beep();
            }
         }
      }
   }
}

class DisplayWindowStartDialog_mOkButton_actionAdapter
   implements ActionListener
{
   private DisplayWindowStartDialog adaptee;

   DisplayWindowStartDialog_mOkButton_actionAdapter(DisplayWindowStartDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.okPressed(e);
   }
}

class DisplayWindowStartDialog_mCancelButton_actionAdapter
   implements ActionListener
{
   private DisplayWindowStartDialog adaptee;

   DisplayWindowStartDialog_mCancelButton_actionAdapter(DisplayWindowStartDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.cancelPressed(e);
   }
}

class DisplayWindowStartDialog_mHelpButton_actionAdapter
   implements ActionListener
{
   private DisplayWindowStartDialog adaptee;

   DisplayWindowStartDialog_mHelpButton_actionAdapter(DisplayWindowStartDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.helpPressed(e);
   }
}

class DisplayWindowStartDialog_mVisualIdField_focusAdapter extends FocusAdapter
{
   private DisplayWindowStartDialog adaptee;

   DisplayWindowStartDialog_mVisualIdField_focusAdapter(DisplayWindowStartDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void focusLost(FocusEvent e)
   {
      adaptee.mVisualIdField_focusLost(e);
   }
}

class DisplayWindowStartDialog_mVisualIdField_actionAdapter
   implements ActionListener
{
   private DisplayWindowStartDialog adaptee;

   DisplayWindowStartDialog_mVisualIdField_actionAdapter(DisplayWindowStartDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.mVisualIdField_actionPerformed(e);
   }
}
