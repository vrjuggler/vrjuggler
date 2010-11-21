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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.customeditors.display_window;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import info.clearthought.layout.*;

import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;


public class InputWindowStartDialog
   extends JDialog
   implements EditorConstants
{
   public InputWindowStartDialog(Container parent, ConfigContext ctx,
                                 Dimension resolution)
      throws HeadlessException
   {
      super();
      this.setTitle("Basic Input Window Parameters");
      this.setModal(true);
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);

      mResolution = resolution;

      ConfigBroker cfg_broker = new ConfigBrokerProxy();
      ConfigDefinition my_def =
         cfg_broker.getRepository().get(INPUT_AREA_TYPE);

      // Create a dummy config element to hand off to a new
      // PropertyEditorPanel instance.
      ConfigElementFactory factory =
         new ConfigElementFactory(cfg_broker.getRepository().getAllLatest());
      ConfigElement win_elt = factory.createUnique(my_def, ctx);

      Object value = win_elt.getProperty(KEYBOARD_MOUSE_PTR_PROPERTY, 0);
      PropertyDefinition prop_def =
         my_def.getPropertyDefinition(KEYBOARD_MOUSE_PTR_PROPERTY);

      mKbdDevEditor = new PropertyEditorPanel(ctx, value, prop_def, win_elt,
                                              0, Color.white);

      value    = win_elt.getProperty(LOCK_KEY_PROPERTY, 0);
      prop_def = my_def.getPropertyDefinition(LOCK_KEY_PROPERTY);

      mLockKeyEditor = new PropertyEditorPanel(ctx, value, prop_def, win_elt,
                                               0, Color.white);

      // Hang onto our dummy config element so that we can get back the value
      // of the lock key later.
      mWinElement = win_elt;

      try
      {
         jbInit();
         setupButtonPanel();
         addResources(ctx);

         mSleepTimeField.setValue(
            mWinElement.getProperty(SLEEP_TIME_PROPERTY, 0)
         );

         Object start_locked =
            mWinElement.getProperty(START_LOCKED_PROPERTY, 0);
         mStartLockedCB.setSelected(Boolean.TRUE.equals(start_locked));

         // Validate the default values for the various text fields.
         validateUserInput();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      this.getRootPane().setDefaultButton(mOkButton);

      this.setResizable(false);
      this.pack();
      this.setLocationRelativeTo(parent);
   }

   public InputWindowStartDialog(Container parent, ConfigContext ctx,
                                 ConfigElement winElt, Dimension resolution)
      throws HeadlessException
   {
      super();
      this.setTitle(winElt.getName() + " Properties");
      this.setModal(true);
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);

      mWinElement = winElt;
      mResolution = resolution;

      String prop_name = KEYBOARD_MOUSE_PTR_PROPERTY;
      PropertyDefinition prop_def =
         winElt.getDefinition().getPropertyDefinition(prop_name);
      mKbdDevEditor =
         new PropertyEditorPanel(ctx, winElt.getProperty(prop_name, 0),
                                 prop_def, winElt, 0, Color.white);

      prop_def =
         winElt.getDefinition().getPropertyDefinition(LOCK_KEY_PROPERTY);
      mLockKeyEditor =
         new PropertyEditorPanel(ctx,
                                 winElt.getProperty(LOCK_KEY_PROPERTY, 0),
                                 prop_def, winElt, 0, Color.white);

      try
      {
         jbInit();
         setupButtonPanel();
         addResources(ctx);

         mResourceLabel.setEnabled(false);
         mResourceChooser.setEnabled(false);

         mNameField.setText(winElt.getName());

         mPositionXField.setValue(winElt.getProperty(ORIGIN_PROPERTY, 0));
         mPositionYField.setValue(winElt.getProperty(ORIGIN_PROPERTY, 1));
         mWidthField.setValue(winElt.getProperty(SIZE_PROPERTY, 0));
         mHeightField.setValue(winElt.getProperty(SIZE_PROPERTY, 1));

         mSleepTimeField.setValue(winElt.getProperty(SLEEP_TIME_PROPERTY, 0));
         Object start_locked = winElt.getProperty(START_LOCKED_PROPERTY, 0);
         mStartLockedCB.setSelected(Boolean.TRUE.equals(start_locked));

         // Validate the default values for the various text fields.
         validateUserInput();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      this.getRootPane().setDefaultButton(mOkButton);

      this.setResizable(false);
      this.pack();
      this.setLocationRelativeTo(parent);
   }

   public int showDialog()
   {
      setVisible(true);
      return status;
   }

   public DataSource getResource()
   {
      String resource_name = (String) mResourceChooser.getSelectedItem();
      return (new ConfigBrokerProxy()).get(resource_name);
   }

   public String getInputWindowTitle()
   {
      return mNameField.getText();
   }

   public Rectangle getInputWindowBounds()
   {
      int x = ((Number) mPositionXField.getValue()).intValue();
      int y = ((Number) mPositionYField.getValue()).intValue();
      int width = ((Number) mWidthField.getValue()).intValue();
      int height = ((Number) mHeightField.getValue()).intValue();
      return new Rectangle(x, y, width, height);
   }

   public Object getKeyboardMousePointer()
   {
      return mWinElement.getProperty(KEYBOARD_MOUSE_PTR_PROPERTY, 0);
   }

   public Object getLockKey()
   {
      return mWinElement.getProperty(LOCK_KEY_PROPERTY, 0);
   }

   public Boolean shouldStartLocked()
   {
      return (mStartLockedCB.isSelected() ? Boolean.TRUE : Boolean.FALSE);
   }

   public Number getSleepTime()
   {
      return (Number) mSleepTimeField.getValue();
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
      // Columns are as follows:
      //    0 -> panel start
      //    1 -> window name label
      //    2 -> spacer between window name label and window name field
      //    3 -> window name field, resource chooser
      //    4 -> panel end
      double[][] main_size =
         {
            {5, TableLayout.MINIMUM, 5, TableLayout.FILL, 5},
            {TableLayout.PREFERRED, TableLayout.PREFERRED,
             TableLayout.PREFERRED, TableLayout.PREFERRED}
         };
      mMainLayout = new TableLayout(main_size);

      double[][] bounds_size =
         {
            {TableLayout.MINIMUM, TableLayout.MINIMUM},
            {TableLayout.PREFERRED, TableLayout.PREFERRED}
         };
      mBoundsPanelLayout = new TableLayout(bounds_size);

      // Colums are as follows:
      //    0 -> editor label
      //    1 -> spacer betweeen label and editor
      //    2 -> start for editors; end for sleep time editor
      //    3 -> spacer between sleep time editor and sleep time units label
      //    4 -> end for lock key editor; unit label for sleep time editor
      double[][] window_props_size =
         {
            {TableLayout.PREFERRED, 5, TableLayout.MINIMUM, 5,
             TableLayout.MINIMUM},
            {TableLayout.PREFERRED, TableLayout.PREFERRED,
             TableLayout.PREFERRED, TableLayout.PREFERRED}
         };
      mWindowPropsPanelLayout = new TableLayout(window_props_size);

      mBoundsPanelBorder =
         new TitledBorder(
            BorderFactory.createEtchedBorder(Color.white, new Color(161, 161,
                                                                    161)),
            "Window Bounds"
        );
      mWindowPropsPanelBorder =
         new TitledBorder(
            BorderFactory.createEtchedBorder(Color.white, new Color(142, 142,
                                                                    142)),
            "Window Behavior Settings"
         );
      mMainPanel.setLayout(mMainLayout);
      mResourceLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mResourceLabel.setLabelFor(mResourceChooser);
      mResourceLabel.setText("Resource");
      int max_width = 350;
      Dimension max_size = mResourceChooser.getMaximumSize();
      Dimension pref_size = mResourceChooser.getPreferredSize();
      mResourceChooser.setMaximumSize(new Dimension(max_width,
                                                    max_size.height));
      mResourceChooser.setPreferredSize(new Dimension(max_width,
                                                      pref_size.height));
      mNameLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mNameLabel.setLabelFor(mNameField);
      mNameLabel.setText("Window Name");
      mNameField.setMinimumSize(new Dimension(60, 21));
      mNameField.setPreferredSize(new Dimension(60, 21));
      mNameField.setText("Input Window");
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
      mSizeXLabel.setText("\u00D7");
      mHeightField.setMinimumSize(new Dimension(40, 21));
      mHeightField.setPreferredSize(new Dimension(40, 21));
      mHeightField.setValue(new Integer(200));
      mPositionXField.setMinimumSize(new Dimension(40, 21));
      mPositionXField.setPreferredSize(new Dimension(40, 21));
      mPositionXField.setValue(new Integer(0));
      mPositionXField.setHorizontalAlignment(SwingConstants.TRAILING);
      mPositionXLabel.setText(",");
      mPositionYField.setMinimumSize(new Dimension(40, 21));
      mPositionYField.setPreferredSize(new Dimension(40, 21));
      mPositionYField.setValue(new Integer(0));
      mPositionYField.setHorizontalAlignment(SwingConstants.TRAILING);
      mKbdDevLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mKbdDevLabel.setText("Keyboard/Mouse");
      mLockKeyLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mLockKeyLabel.setText("Lock Key");
      mStartLockedLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mStartLockedLabel.setText("Start Locked");
      mStartLockedLabel.setLabelFor(mStartLockedCB);
      mStartLockedCB.setText("");
      mStartLockedCB.setHorizontalTextPosition(SwingConstants.RIGHT);
      mSleepTimeLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mSleepTimeLabel.setLabelFor(mSleepTimeField);
      mSleepTimeLabel.setText("Sleep Time");
      mSleepTimeField.setHorizontalAlignment(SwingConstants.RIGHT);
      mSleepMSLabel.setText("ms");
      mOkButton.setEnabled(false);
      mOkButton.setSelected(false);
      mOkButton.setText("OK");
      mOkButton.addActionListener(
         new InputWindowStartDialog_mOkButton_actionAdapter(this)
      );
      mCancelButton.setText("Cancel");
      mCancelButton.addActionListener(
         new InputWindowStartDialog_mCancelButton_actionAdapter(this)
      );
      mHelpButton.setEnabled(false);
      mHelpButton.setText("Help");
      mHelpButton.addActionListener(
         new InputWindowStartDialog_mHelpButton_actionAdapter(this)
      );
      mBoundsPanel.setLayout(mBoundsPanelLayout);
      mBoundsPanel.setBorder(mBoundsPanelBorder);
      mWindowPropsPanel.setLayout(mWindowPropsPanelLayout);
      mWindowPropsPanel.setBorder(mWindowPropsPanelBorder);
      mButtonPanel.setLayout(mButtonPanelLayout);
      this.getContentPane().add(mMainPanel, BorderLayout.CENTER);
      mBoundsPanel.add(
         mSizeLabel,
         new TableLayoutConstraints(0, 0, 0, 0,
                                    TableLayoutConstraints.RIGHT,
                                    TableLayoutConstraints.CENTER)
      );
      mBoundsPanel.add(
         mSizePanel,
         new TableLayoutConstraints(1, 0, 1, 0,
                                    TableLayoutConstraints.LEFT,
                                    TableLayoutConstraints.CENTER)
      );
      mBoundsPanel.add(
         mPositionLabel,
         new TableLayoutConstraints(0, 1, 0, 1,
                                    TableLayoutConstraints.RIGHT,
                                    TableLayoutConstraints.CENTER)
      );
      mBoundsPanel.add(
         mPositionPanel,
         new TableLayoutConstraints(1, 1, 1, 1,
                                    TableLayoutConstraints.LEFT,
                                    TableLayoutConstraints.CENTER)
      );
      mWindowPropsPanel.add(
         mKbdDevLabel,
         new TableLayoutConstraints(0, 0, 0, 0,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );
      mWindowPropsPanel.add(
         mKbdDevEditor,
         new TableLayoutConstraints(2, 0, 4, 0,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );
      mWindowPropsPanel.add(
         mLockKeyLabel,
         new TableLayoutConstraints(0, 1, 0, 1,
                                    TableLayoutConstraints.RIGHT,
                                    TableLayoutConstraints.CENTER)
      );
      mWindowPropsPanel.add(
         mLockKeyEditor,
         new TableLayoutConstraints(2, 1, 4, 1,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );
      mWindowPropsPanel.add(
         mStartLockedLabel,
         new TableLayoutConstraints(0, 2, 0, 2,
                                    TableLayoutConstraints.RIGHT,
                                    TableLayoutConstraints.CENTER)
      );
      mWindowPropsPanel.add(
         mStartLockedCB,
         new TableLayoutConstraints(2, 2, 2, 2,
                                    TableLayoutConstraints.LEFT,
                                    TableLayoutConstraints.CENTER)
      );
      mWindowPropsPanel.add(
         mSleepTimeLabel,
         new TableLayoutConstraints(0, 3, 0, 3,
                                    TableLayoutConstraints.RIGHT,
                                    TableLayoutConstraints.CENTER)
      );
      mWindowPropsPanel.add(
         mSleepTimeField,
         new TableLayoutConstraints(2, 3, 2, 3,
                                    TableLayoutConstraints.FULL,
                                    TableLayoutConstraints.CENTER)
      );
      mWindowPropsPanel.add(
         mSleepMSLabel,
         new TableLayoutConstraints(4, 3, 4, 3,
                                    TableLayoutConstraints.LEFT,
                                    TableLayoutConstraints.CENTER)
      );
      mPositionPanel.add(mPositionXField, null);
      mPositionPanel.add(mPositionXLabel, null);
      mPositionPanel.add(mPositionYField, null);
      mSizePanel.add(mWidthField, null);
      mSizePanel.add(mSizeXLabel, null);
      mSizePanel.add(mHeightField, null);
      mMainPanel.add(mResourceLabel,
                     new TableLayoutConstraints(1, 0, 1, 0,
                                                TableLayout.RIGHT,
                                                TableLayout.CENTER));
      mMainPanel.add(mResourceChooser,
                     new TableLayoutConstraints(3, 0, 3, 0,
                                                TableLayout.LEFT,
                                                TableLayout.CENTER));
      mMainPanel.add(mNameLabel,
                     new TableLayoutConstraints(1, 1, 1, 1,
                                                TableLayoutConstraints.RIGHT,
                                                TableLayoutConstraints.CENTER));
      mMainPanel.add(mNameField,
                     new TableLayoutConstraints(3, 1, 3, 1,
                                                TableLayoutConstraints.FULL,
                                                TableLayoutConstraints.CENTER));
      mMainPanel.add(mBoundsPanel,
                     new TableLayoutConstraints(0, 2, 4, 2,
                                                TableLayoutConstraints.FULL,
                                                TableLayoutConstraints.FULL));
      mMainPanel.add(mWindowPropsPanel,
                     new TableLayoutConstraints(0, 3, 4, 3,
                                                TableLayoutConstraints.FULL,
                                                TableLayoutConstraints.FULL));
      this.getContentPane().add(mButtonPanel, BorderLayout.SOUTH);
   }

   private void addResources(ConfigContext ctx)
   {
      for ( java.util.Iterator i = ctx.getResources().iterator();
            i.hasNext(); )
      {
         mResourceChooser.addItem(i.next());
      }
   }

   private void setupButtonPanel()
   {
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
   }

   private int status = CANCEL_OPTION;
   private ConfigElement mWinElement = null;
   private Dimension mResolution = null;

   private JPanel mMainPanel = new JPanel();
   private TableLayout mMainLayout = null;
   private JLabel mResourceLabel = new JLabel();
   private JComboBox mResourceChooser = new JComboBox();
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
   private JPanel mWindowPropsPanel = new JPanel();
   private TableLayout mWindowPropsPanelLayout = null;
   private TitledBorder mWindowPropsPanelBorder;
   private JLabel mKbdDevLabel = new JLabel();
   private PropertyEditorPanel mKbdDevEditor = null;
   private JLabel mLockKeyLabel = new JLabel();
   private PropertyEditorPanel mLockKeyEditor = null;
   private JLabel mStartLockedLabel = new JLabel();
   private JCheckBox mStartLockedCB = new JCheckBox();
   private JLabel mSleepTimeLabel = new JLabel();
   private JFormattedTextField mSleepTimeField = new JFormattedTextField();
   private JLabel mSleepMSLabel = new JLabel();
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
      width_valid  = ((Number) mWidthField.getValue()).intValue() > 0;
      height_valid = ((Number) mHeightField.getValue()).intValue() > 0;
      x_pos_valid  = ((Number) mPositionXField.getValue()).intValue() >= 0;
      y_pos_valid  = ((Number) mPositionYField.getValue()).intValue() >= 0;

      if ( name_valid && width_valid && height_valid && x_pos_valid &&
           y_pos_valid )
      {
         mOkButton.setEnabled(true);
//         validateWindowBounds();
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
    */
/*
   private void validateWindowBounds()
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
            JOptionPane.showConfirmDialog(
               this,
               "Do you want the window to open outside the managed area?",
               "Input Window Outside Managed Area",
               JOptionPane.YES_NO_OPTION,
               JOptionPane.WARNING_MESSAGE
            );

         if ( answer == JOptionPane.YES_OPTION )
         {
         }
         else
         {
         }
      }
   }
*/
}

class InputWindowStartDialog_mOkButton_actionAdapter
   implements ActionListener
{
   private InputWindowStartDialog adaptee;

   InputWindowStartDialog_mOkButton_actionAdapter(InputWindowStartDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.okPressed(e);
   }
}

class InputWindowStartDialog_mCancelButton_actionAdapter
   implements ActionListener
{
   private InputWindowStartDialog adaptee;

   InputWindowStartDialog_mCancelButton_actionAdapter(InputWindowStartDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.cancelPressed(e);
   }
}

class InputWindowStartDialog_mHelpButton_actionAdapter
   implements ActionListener
{
   private InputWindowStartDialog adaptee;

   InputWindowStartDialog_mHelpButton_actionAdapter(InputWindowStartDialog adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.helpPressed(e);
   }
}
