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
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.PlainDocument;
import info.clearthought.layout.*;

import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;

import org.vrjuggler.vrjconfig.commoneditors.ConfigPtrComboBoxModel;
import org.vrjuggler.vrjconfig.commoneditors.ConfigPtrComboBoxRenderer;

public class DisplayWindowStartDialog
   extends JDialog
   implements EditorConstants
{
   public DisplayWindowStartDialog(Container parent, ConfigContext ctx,
                                   ConfigElement winElt, Dimension resolution,
                                   CaveModel caveModel)
   {
      this(parent, ctx, winElt, resolution, caveModel, false);
   }

   public DisplayWindowStartDialog(Container parent, ConfigContext ctx,
                                   ConfigElement winElt, Dimension resolution,
                                   CaveModel caveModel, boolean clusterConfig)
      throws HeadlessException
   {
      super();
      
      mClusterConfig = clusterConfig;
      mConfigContext = ctx;

      if (winElt.getName().equals(""))
      {
         this.setTitle("Screen Properties");
      }
      else
      {
         this.setTitle(winElt.getName() + " Properties");
      }
      this.setModal(true);
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);

      mWinElement = winElt;
      mResolution = resolution;

      try
      {
         jbInit();
         setupButtonPanel();
         addResources(ctx);

         mResourceLabel.setEnabled(false);
         mResourceChooser.setEnabled(false);

         mNameField.setText(winElt.getName());

         ConfigElement node = (ConfigElement)caveModel.getScreenToNodeMap().get(winElt);
         
         if (null != node)
         {
            mNodeComboBox.setSelectedItem(node);
         }
         else
         {
            mNodeComboBox.setSelectedItem("None");
         }
         
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
         
         ConfigElement fb_cfg =
            (ConfigElement) winElt.getProperty("frame_buffer_config", 0);

         mPositionXField.setValue(winElt.getProperty(ORIGIN_PROPERTY, 0));
         mPositionYField.setValue(winElt.getProperty(ORIGIN_PROPERTY, 1));
         mWidthField.setValue(winElt.getProperty(SIZE_PROPERTY, 0));
         mHeightField.setValue(winElt.getProperty(SIZE_PROPERTY, 1));

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

   public ConfigElement getSelectedNode()
   {
      Object obj = mNodeComboBox.getSelectedItem();
      if (obj instanceof ConfigElement)
      {
         return (ConfigElement)obj;
      }
      else
      {
         return null;
      }
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

   public String getDisplayWindowTitle()
   {
      return mNameField.getText();
   }

   public Rectangle getDisplayWindowBounds()
   {
      int x = ((Number) mPositionXField.getValue()).intValue();
      int y = ((Number) mPositionYField.getValue()).intValue();
      int width = ((Number) mWidthField.getValue()).intValue();
      int height = ((Number) mHeightField.getValue()).intValue();
      return new Rectangle(x, y, width, height);
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
      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         mDisplayIcon = new ImageIcon(loader.getResource(IMAGE_BASE +
                                                          "/window_size.png"));
         
         mDisplayIconLabel.setIcon(mDisplayIcon);
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         mDisplayIconLabel.setText("");
      }

      // Columns are as follows:
      //    0 -> panel start
      //    1 -> window name label
      //    2 -> spacer between window name label and window name field
      //    3 -> window name field start, lame space for mResourceChooser
      //    4 -> window name field end
      //    5 -> panel end
      double[][] main_size =
         {
            {5, TableLayout.MINIMUM, 5, 300, TableLayout.FILL, 5},
            {TableLayout.PREFERRED, TableLayout.PREFERRED,
             TableLayout.PREFERRED, TableLayout.PREFERRED,
             TableLayout.PREFERRED, TableLayout.PREFERRED}
         };
      mMainLayout = new TableLayout(main_size);

      double[][] bounds_size =
         {
            {TableLayout.PREFERRED, 10, TableLayout.PREFERRED, TableLayout.FILL},
            {TableLayout.PREFERRED}
         };
      mBoundsPanelLayout = new TableLayout(bounds_size);

      // Colums are as follows:
      //    0 -> editor label
      //    1 -> spacer betweeen label and editor
      //    2 -> start for lock key and sleep time editors; end for checkbox
      //         editor
      //    3 -> end for sleep time editor
      //    4 -> spacer between sleep time editor and sleep time units label
      //    5 -> end for lock key editor; unit label for sleep time editor
      //
      // NOTE: Column 2 is a hack to make it appear as though the check box
      // is two separate UI components.
      double[][] window_props_size =
         {
            {TableLayout.PREFERRED, 5, 20, TableLayout.MINIMUM, 5,
             TableLayout.MINIMUM},
            {TableLayout.PREFERRED, TableLayout.PREFERRED,
             TableLayout.PREFERRED, TableLayout.PREFERRED,
             TableLayout.PREFERRED, TableLayout.PREFERRED,
             TableLayout.PREFERRED}
         };
      mWindowPropsPanelLayout = new TableLayout(window_props_size);
      
      mMainPanel.setLayout(mMainLayout);
      mResourceLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mResourceLabel.setLabelFor(mResourceChooser);
      mResourceLabel.setText("Resource");
      mNameLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mNameLabel.setLabelFor(mNameField);
      mNameLabel.setText("Screen Name");
      mNameField.setMinimumSize(new Dimension(60, 21));
      mNameField.setPreferredSize(new Dimension(60, 21));
      mNameField.setText("Display Window");
      mNodeLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mNodeLabel.setLabelFor(mNameField);
      mNodeLabel.setText("Cluster Node");
      mNodeComboBox.setMinimumSize(new Dimension(60, 21));
      mNodeComboBox.setPreferredSize(new Dimension(60, 21));
      mNodeComboBoxModel = new ConfigPtrComboBoxModel(mConfigContext);
      mNodeComboBoxModel.addElementType(CLUSTER_NODE_TYPE);
      mNodeComboBox.setModel(mNodeComboBoxModel);
      ConfigPtrComboBoxRenderer node_combo_box_renderer
         = new ConfigPtrComboBoxRenderer();
      mNodeComboBox.setRenderer(node_combo_box_renderer);
      mSizeLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mSizeLabel.setLabelFor(mSizePanel);
      mSizeLabel.setText("Size");
      mPositionLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mPositionLabel.setLabelFor(mPositionPanel);
      mPositionLabel.setText("Position");
      mWidthField.setMinimumSize(new Dimension(50, 21));
      mWidthField.setPreferredSize(new Dimension(50, 21));
      mWidthField.setValue(new Integer(200));
      mWidthField.setHorizontalAlignment(SwingConstants.TRAILING);
      mSizeXLabel.setText("\u00D7");
      mHeightField.setMinimumSize(new Dimension(50, 21));
      mHeightField.setPreferredSize(new Dimension(50, 21));
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
      mBoundsPanel.setLayout(mBoundsPanelLayout);
      mWindowPropsPanel.setLayout(mWindowPropsPanelLayout);
      mButtonPanel.setLayout(mButtonPanelLayout);
      this.getContentPane().add(mMainPanel, BorderLayout.CENTER);
      mBoundsPanel.add(mSizeLabel, new TableLayoutConstraints(2, 0, 2, 0,
                                                            TableLayout.RIGHT,
                                                            TableLayout.CENTER));
      mBoundsPanel.add(mSizePanel, new TableLayoutConstraints(3, 0, 3, 0,
                                                            TableLayout.LEFT,
                                                            TableLayout.CENTER));
      mDisplayIconLabel.setPreferredSize(new Dimension(32, 32));
      mBoundsPanel.add(mDisplayIconLabel,
                       new TableLayoutConstraints(0, 0, 0, 0,
                                                  TableLayout.CENTER,
                                                  TableLayout.CENTER));
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
                                                TableLayout.RIGHT,
                                                TableLayout.CENTER));
      mMainPanel.add(mNameField,
                     new TableLayoutConstraints(3, 1, 4, 1,
                                                TableLayout.FULL,
                                                TableLayout.CENTER));
      if (mClusterConfig)
      {
         mMainPanel.add(mNodeLabel,
                        new TableLayoutConstraints(1, 2, 1, 2,
                                                   TableLayout.RIGHT,
                                                   TableLayout.CENTER));
         mMainPanel.add(mNodeComboBox,
                        new TableLayoutConstraints(3, 2, 4, 2,
                                                   TableLayout.FULL,
                                                   TableLayout.CENTER));
      }
      mMainPanel.add(mBoundsPanel,
                     new TableLayoutConstraints(0, 3, 5, 3,
                                                TableLayout.FULL,
                                                TableLayout.FULL));
      this.getContentPane().add(mButtonPanel, BorderLayout.SOUTH);
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

   private void addResources(ConfigContext ctx)
   {
      for ( java.util.Iterator i = ctx.getResources().iterator();
            i.hasNext(); )
      {
         mResourceChooser.addItem(i.next());
      }
   }

   private int status = CANCEL_OPTION;
   private ConfigElement mWinElement = null;
   private ConfigContext mConfigContext = null;
   private Dimension mResolution = null;
   
   private boolean mClusterConfig = false;
   private JPanel mMainPanel = new JPanel();
   private TableLayout mMainLayout = null;
   private JLabel mResourceLabel = new JLabel();
   private JComboBox mResourceChooser = new JComboBox();
   private JLabel mNameLabel = new JLabel();
   private JTextField mNameField = new JTextField();
   
   private JLabel mNodeLabel = new JLabel();
   private JComboBox mNodeComboBox = new JComboBox();
   private ConfigPtrComboBoxModel mNodeComboBoxModel = null;
   
   private JPanel mBoundsPanel = new JPanel();
   private TableLayout mBoundsPanelLayout = null;
   private TitledBorder mBoundsPanelBorder;
   private JLabel mDisplayIconLabel = new JLabel();
   private Icon mDisplayIcon = null;
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
   private JPanel mButtonPanel = new JPanel();
   private FlowLayout mButtonPanelLayout = new FlowLayout();
   private JButton mOkButton = new JButton();
   private JButton mCancelButton = new JButton();
   private JButton mHelpButton = new JButton();

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
      int width = ((Number) mWidthField.getValue()).intValue();
      int window_width  = ((Number) mWidthField.getValue()).intValue();
      int window_height = ((Number) mHeightField.getValue()).intValue();
      int window_pos_x  = ((Number) mPositionXField.getValue()).intValue();
      int window_pos_y  = ((Number) mPositionYField.getValue()).intValue();

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
