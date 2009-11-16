/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

package org.vrjuggler.vrjconfig.customeditors.display_window;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.*;
import java.util.Iterator;
import java.util.Map;
import javax.swing.*;
import info.clearthought.layout.*;

import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.vrjconfig.commoneditors.DeviceConfig;


public class SimDeviceCreateDialog
   extends JDialog
   implements EditorConstants
{
   public SimDeviceCreateDialog(Container parent, ConfigContext ctx,
                                ConfigElement kbdProxyElt,
                                Map simDevProxyDefsMap, String dataSourceName)
      throws java.awt.HeadlessException
   {
      super();
      this.setTitle("Simulator Device Creation");
      this.setModal(true);
      enableEvents(java.awt.AWTEvent.WINDOW_EVENT_MASK);

      mContext            = ctx;
      mKbdProxyElement    = kbdProxyElt;
      mSimDevProxyDefsMap = simDevProxyDefsMap;
      mDataSourceName     = dataSourceName;

      try
      {
         jbInit();

         for ( Iterator i = simDevProxyDefsMap.keySet().iterator();
               i.hasNext(); )
         {
            mTypeChooser.addItem(i.next());
         }

         validateUserInput();
      }
      catch(Exception ex)
      {
         ex.printStackTrace();
      }

      mNameField.requestFocus();
      this.getRootPane().setDefaultButton(mCreateButton);

      this.pack();
      this.setLocationRelativeTo(parent);
   }

   public DeviceConfig showDialog()
   {
      setVisible(true);
      return mSimDevConfig;
   }

   protected void processWindowEvent(WindowEvent e)
   {
      if ( e.getID() == WindowEvent.WINDOW_CLOSING )
      {
         dispose();
      }

      super.processWindowEvent(e);
   }

   protected void processKeyEvent(KeyEvent e)
   {
      System.out.println("key code == " + e.getKeyCode());
      System.out.println("key char == " + e.getKeyChar());
      super.processKeyEvent(e);
   }

   private void validateUserInput()
   {
      mCreateButton.setEnabled(! mNameField.getText().equals(""));
   }

   private void jbInit()
      throws Exception
   {
      this.getContentPane().setLayout(new BorderLayout());

      double[][] table_size =
         {
            {5, TableLayout.MINIMUM, 5, TableLayout.FILL, 5},
            {5, TableLayout.PREFERRED, TableLayout.PREFERRED, TableLayout.FILL}
         };
      mMainPanel.setLayout(new TableLayout(table_size));

      mNameLabel.setText("Name:");
      mNameLabel.setLabelFor(mNameField);
      mNameField.addActionListener(
         new SimDeviceCreateDialog_mNameField_actionAdapter(this)
      );
      mNameField.addFocusListener(
         new SimDeviceCreateDialog_mNameField_focusAdapter(this)
      );
      mTypeLabel.setText("Device Type:");
      mTypeLabel.setLabelFor(mTypeChooser);
      mTypeChooser.setRenderer(new SimDeviceDefRenderer());
      mTypeChooser.addActionListener(
         new SimDeviceCreateDialog_mTypeChooser_actionAdapter(this)
      );
      mHelpPane.setContentType("text/html");
      mHelpPane.setEditable(false);
      mHelpPane.setMinimumSize(new Dimension(200, 200));
      mHelpPane.setPreferredSize(new Dimension(200, 200));
      mHelpScrollPane.setHorizontalScrollBarPolicy(
         JScrollPane.HORIZONTAL_SCROLLBAR_NEVER
      );
      mHelpScrollPane.setVerticalScrollBarPolicy(
         JScrollPane.VERTICAL_SCROLLBAR_ALWAYS
      );
      mMainPanel.add(mNameLabel,
                     new TableLayoutConstraints(1, 1, 1, 1,
                                                TableLayoutConstraints.RIGHT,
                                                TableLayoutConstraints.FULL));
      mMainPanel.add(mNameField,
                     new TableLayoutConstraints(3, 1, 3, 1,
                                                TableLayoutConstraints.FULL,
                                                TableLayoutConstraints.CENTER));
      mMainPanel.add(mTypeLabel,
                     new TableLayoutConstraints(1, 2, 1, 2,
                                                TableLayoutConstraints.RIGHT,
                                                TableLayoutConstraints.FULL));
      mMainPanel.add(mTypeChooser,
                     new TableLayoutConstraints(3, 2, 3, 2,
                                                TableLayoutConstraints.LEFT,
                                                TableLayoutConstraints.CENTER));
      mMainPanel.add(mHelpScrollPane,
                     new TableLayoutConstraints(3, 3, 3, 3,
                                                TableLayoutConstraints.FULL,
                                                TableLayoutConstraints.FULL));

      mCancelButton.setEnabled(true);
      mCancelButton.setText("Cancel");
      mCancelButton.addActionListener(
         new SimDeviceCreateDialog_mCancelButton_actionAdapter(this)
      );
      mCreateButton.setText("Create");
      mCreateButton.addActionListener(
         new SimDeviceCreateDialog_mCreateButton_actionAdapter(this)
      );
      FlowLayout button_panel_layout = new FlowLayout();
      mButtonPanel.setLayout(button_panel_layout);

      if ( (new EnvironmentServiceProxy()).getOS() == EnvironmentService.Windows )
      {
         button_panel_layout.setAlignment(FlowLayout.CENTER);
         mButtonPanel.add(mCreateButton, null);
         mButtonPanel.add(mCancelButton, null);
      }
      else
      {
         button_panel_layout.setAlignment(FlowLayout.RIGHT);
         mButtonPanel.add(mCancelButton, null);
         mButtonPanel.add(mCreateButton, null);
      }

      this.getContentPane().add(mMainPanel, BorderLayout.CENTER);
      this.getContentPane().add(mButtonPanel, BorderLayout.SOUTH);
   }

   private ConfigContext mContext            = null;
   private ConfigElement mKbdProxyElement    = null;
   private DeviceConfig  mSimDevConfig       = null;
   private Map           mSimDevProxyDefsMap = null;
   private String        mDataSourceName     = null;

   private JPanel mMainPanel = new JPanel();
   private JLabel mNameLabel = new JLabel();
   private JTextField mNameField = new JTextField();
   private JLabel mTypeLabel = new JLabel();
   private JComboBox mTypeChooser = new JComboBox();
   private JEditorPane mHelpPane = new JEditorPane();
   private JScrollPane mHelpScrollPane = new JScrollPane(mHelpPane);
   private JPanel mButtonPanel = new JPanel();
   private JButton mCreateButton = new JButton();
   private JButton mCancelButton = new JButton();

   void mTypeChooser_actionPerformed(ActionEvent event)
   {
      ConfigDefinition def = (ConfigDefinition) mTypeChooser.getSelectedItem();
      if ( def == null )
      {
         mHelpPane.setText("");
      }
      else
      {
         mHelpPane.setText(def.getHelp());
      }
   }

   void mCreateButton_actionPerformed(ActionEvent event)
   {
      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigElementFactory factory =
         new ConfigElementFactory(broker.getRepository().getAllLatest());
      ConfigDefinition sim_def =
         (ConfigDefinition) mTypeChooser.getSelectedItem();
      String dev_name = mNameField.getText();
      ConfigElement dev_elt = factory.create(dev_name + " Sim Device",
                                             sim_def);
      dev_elt.setProperty(KEYBOARD_MOUSE_PROXY_PTR_PROPERTY, 0,
                          mKbdProxyElement.getName());

      ConfigDefinition proxy_def =
         (ConfigDefinition) mSimDevProxyDefsMap.get(sim_def);
      ConfigElement proxy_elt = factory.create(dev_name + " Proxy", proxy_def);
      proxy_elt.setProperty(DEVICE_PROPERTY, 0, dev_elt.getName());

      ConfigDefinition alias_def = broker.getRepository().get(ALIAS_TYPE);
      ConfigElement alias_elt = factory.create(dev_name, alias_def);
      alias_elt.setProperty(PROXY_PROPERTY, 0, proxy_elt.getName());

      broker.add(mContext, dev_elt, mDataSourceName);
      broker.add(mContext, proxy_elt, mDataSourceName);
      broker.add(mContext, alias_elt, mDataSourceName);

      mSimDevConfig = new DeviceConfig(mContext, dev_elt,
                                       new ConfigElement[]{proxy_elt},
                                       new ConfigElement[]{alias_elt});

      dispose();
   }

   void mCancelButton_actionPerformed(ActionEvent event)
   {
      dispose();
   }

   void mNameField_actionPerformed(ActionEvent event)
   {
      validateUserInput();
   }

   void mNameField_focusLost(FocusEvent event)
   {
      validateUserInput();
   }

   private class SimDeviceDefRenderer
      extends JLabel
      implements ListCellRenderer
   {
      public SimDeviceDefRenderer()
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

         ConfigDefinition def = (ConfigDefinition) value;
         setFont(list.getFont());
         setText(def.getName());

         return this;
      }
   }
}

class SimDeviceCreateDialog_mCreateButton_actionAdapter
   implements ActionListener
{
   private SimDeviceCreateDialog adaptee;
   SimDeviceCreateDialog_mCreateButton_actionAdapter(SimDeviceCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mCreateButton_actionPerformed(actionEvent);
   }
}

class SimDeviceCreateDialog_mCancelButton_actionAdapter
   implements ActionListener
{
   private SimDeviceCreateDialog adaptee;
   SimDeviceCreateDialog_mCancelButton_actionAdapter(SimDeviceCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mCancelButton_actionPerformed(actionEvent);
   }
}

class SimDeviceCreateDialog_mTypeChooser_actionAdapter
   implements ActionListener
{
   private SimDeviceCreateDialog adaptee;
   SimDeviceCreateDialog_mTypeChooser_actionAdapter(SimDeviceCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mTypeChooser_actionPerformed(actionEvent);
   }
}

class SimDeviceCreateDialog_mNameField_actionAdapter
   implements ActionListener
{
   private SimDeviceCreateDialog adaptee;
   SimDeviceCreateDialog_mNameField_actionAdapter(SimDeviceCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }

   public void actionPerformed(ActionEvent actionEvent)
   {
      adaptee.mNameField_actionPerformed(actionEvent);
   }
}

class SimDeviceCreateDialog_mNameField_focusAdapter
   extends FocusAdapter
{
   private SimDeviceCreateDialog adaptee;
   SimDeviceCreateDialog_mNameField_focusAdapter(SimDeviceCreateDialog adaptee)
   {
      this.adaptee = adaptee;
   }

   public void focusLost(FocusEvent focusEvent)
   {
      adaptee.mNameField_focusLost(focusEvent);
   }
}
