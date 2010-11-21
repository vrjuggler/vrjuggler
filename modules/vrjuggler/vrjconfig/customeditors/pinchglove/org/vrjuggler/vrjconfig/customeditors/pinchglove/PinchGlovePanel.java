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

package org.vrjuggler.vrjconfig.customeditors.pinchglove;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.GridLayout;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.util.ArrayList;
import java.util.List;
import javax.swing.BorderFactory;
import javax.swing.border.EtchedBorder;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingConstants;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.CustomEditor;
import org.vrjuggler.jccl.editors.CustomEditorRegistry;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;

import org.vrjuggler.vrjconfig.commoneditors.ProxyTreeEditor;
import org.vrjuggler.vrjconfig.commoneditors.proxytree.*;


public class PinchGlovePanel extends JPanel implements CustomEditor
{
   private JPanel               mCenterPanel = new JPanel();
   private JLabel               mPinchGloveImageLbl = new JLabel();

   private Box                  mBox = Box.createVerticalBox();

   private JPanel               mSettingsPanel = new JPanel();
   private JLabel               mNameLbl = new JLabel();
   private JTextField           mName = new JTextField();
   private JLabel               mPortLbl = new JLabel();
   private PropertyEditorPanel  mSerialPort = null;
   private JLabel               mBaudLbl = new JLabel();
   private PropertyEditorPanel  mBaud = null;
   private JLabel               mGlovePositionLbl = new JLabel();
   private PropertyEditorPanel  mGlovePosition = null;
   private JLabel               mDeviceHostLbl = new JLabel();
   private PropertyEditorPanel  mDeviceHost = null;

   private JPanel               mDirectionsPanel = new JPanel();

   private JPanel               mIconPanel = new JPanel();
   private JLabel               mGloveIconLbl = new JLabel();

   private JPanel               mTextPanel = new JPanel();
   private JLabel               mTitleLbl = new JLabel();
   private JLabel               mDirectionsLbl = new JLabel();

   private ProxyTree            mProxyTree = null;
   
   private ConfigBroker         mBroker = null;

   /** ConfigElement for this Intersense device. */
   private ConfigElement mConfigElement = null;

   /** ConfigContext associated with this Intersense device. */
   private ConfigContext mConfigContext = null;

   public PinchGlovePanel()
   {
      // Register ourselves with the CustomEditorRegistry.
      CustomEditorRegistry.registerEditor("pinch_glove",  PinchGlovePanel.class);
      try
      {
         List pos_icons = new ArrayList();
         List dig_icons = new ArrayList();
         List ana_icons = new ArrayList();
         ImageIcon temp_icon;

         ClassLoader loader = getClass().getClassLoader();     

         // Loading the positional icons.
         temp_icon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/position16.png"));
         pos_icons.add(temp_icon);

         // Load the digital icons.
         temp_icon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/digital16.png"));
         dig_icons.add(temp_icon);

         // Load the analog icons.
         temp_icon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/analog16.png"));
         ana_icons.add(temp_icon);

         // Must create the ProxyTree before calling jbInit since it will add it to the panel.
         mProxyTree = new ProxyTree(pos_icons, dig_icons, ana_icons);
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      this.loadLabelIcon(mGloveIconLbl, "org/vrjuggler/vrjconfig/customeditors/pinchglove/images/GloveIcon.png", "ICON");
      this.loadLabelIcon(mPinchGloveImageLbl, "org/vrjuggler/vrjconfig/customeditors/pinchglove/images/PinchGlove.png", "PinchGlove");
   }

   public void setConfig(ConfigContext ctx, ConfigElement elm)
   {
      mConfigContext = ctx;
      mConfigElement = elm;
      init();
   }

   public Container getPanel()
   {
      return this;
   }

   public String getTitle()
   {
      return "PinchGlove Editor";
   }

   public ActionListener getHelpActionListener()
   {
      return null;
   }

   public void editorClosing()
   {
   }

   private void loadLabelIcon(JLabel button, String path, String alt)
   {
      try
      {
         button.setIcon(new ImageIcon(this.getClass().getClassLoader().getResource(path)));
      }
      catch(NullPointerException exp)
      {
         button.setIcon(null);
         button.setText(alt);
      }
   }

   private void jbInit() throws Exception
   {
      // Used to load image while editing in JBuilder
      mGloveIconLbl.setIcon(new ImageIcon("/home/users/aronb/Source/ClusterWizard/images/GloveIcon.png"));

      BorderLayout base_layout = new BorderLayout();
      base_layout.setHgap(5);
      base_layout.setVgap(5);
      this.setLayout(base_layout);

      mTitleLbl.setFont(new java.awt.Font("Serif", 1, 20));
      mTitleLbl.setHorizontalAlignment(SwingConstants.LEFT);
      mTitleLbl.setText("FakeSpace PinchGlove");
      mDirectionsLbl.setText("Fill in the following fields for the your PinchGlove device.");
      mIconPanel.add(mGloveIconLbl, null);

      mTextPanel.setLayout(new BoxLayout(mTextPanel, BoxLayout.Y_AXIS));
      mTextPanel.add(mTitleLbl);
      mTextPanel.add(mDirectionsLbl);
      mTextPanel.add(Box.createVerticalGlue());

      mDirectionsPanel.setBorder(BorderFactory.createEtchedBorder());
      mDirectionsPanel.setLayout(new BorderLayout());
      mDirectionsPanel.add(mIconPanel, BorderLayout.EAST);
      mDirectionsPanel.add(mTextPanel, BorderLayout.CENTER);
      this.add(mDirectionsPanel, BorderLayout.NORTH);

      mNameLbl.setText("Name");
      mPortLbl.setText("Port");
      mBaudLbl.setText("Baud");
      mGlovePositionLbl.setText("Glove Position");
      mDeviceHostLbl.setText("Device Host");
      mSettingsPanel.setLayout(new GridLayout(5,2));

      mBox.add(Box.createVerticalGlue());
      mBox.add(mSettingsPanel);
      mBox.add(Box.createVerticalGlue());

      mPinchGloveImageLbl.setIcon(new ImageIcon("/home/users/aronb/Source/ClusterWizard/images/PinchGlove.png"));
      mCenterPanel.setLayout(new BorderLayout());
      mCenterPanel.add(mBox,  BorderLayout.CENTER);
      mCenterPanel.add(mPinchGloveImageLbl,  BorderLayout.WEST);

      ProxyTreeEditor proxy_panel = new ProxyTreeEditor(mProxyTree);

      this.add(mCenterPanel, BorderLayout.CENTER);
      this.add(proxy_panel, BorderLayout.EAST);
   }

   public void init()
   {
      // Get handle to broker
      mBroker = new ConfigBrokerProxy();

      ConfigDefinition cfg_def = mConfigElement.getDefinition();

      mSerialPort = new PropertyEditorPanel(mConfigContext, mConfigElement.getProperty("port", 0),
                                            cfg_def.getPropertyDefinition("port"),
                                            mConfigElement, 0, getBackground());

      mName.setText(mConfigElement.getName());
      mName.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));

      // The following two listeners allow the user to change the name of the
      // embedded element.
      mName.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            mName.setBackground(getParent().getBackground());
            // Force the focus to be lost.
            mName.transferFocusUpCycle();
            // Force the focus to be transfered to the next component.
            //name.transferFocus();

            // This is no longer needed since the above line will force a
            // focusLostEvent. But I have choosen to leave this line here in
            // case it might become useful later.
            //stopCellEditing();
         }
      });
      mName.addFocusListener(new FocusAdapter()
      {
         public void focusGained(FocusEvent evt)
         {
            mName.setBorder(BorderFactory.createLineBorder(Color.black, 2));
         }
         public void focusLost(FocusEvent evt)
         {
            System.out.println("New value: " + mName.getText());
            mName.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));
            //stopCellEditing();

            //Change name.
            mConfigElement.setName(mName.getText());
         }
      });

      mBaud = new PropertyEditorPanel(mConfigContext, mConfigElement.getProperty("baud", 0),
                                      cfg_def.getPropertyDefinition("baud"),
                                      mConfigElement, 0, getBackground());
      mGlovePosition = new PropertyEditorPanel(mConfigContext, mConfigElement.getProperty("glove_position", 0),
                                               cfg_def.getPropertyDefinition("glove_position"),
                                               mConfigElement, 0, getBackground());
      mDeviceHost = new PropertyEditorPanel(mConfigContext, mConfigElement.getProperty("device_host", 0),
                                            cfg_def.getPropertyDefinition("device_host"),
                                            mConfigElement, 0, getBackground());

      mSettingsPanel.add(mNameLbl, null);
      mSettingsPanel.add(mName, null);
      mSettingsPanel.add(mPortLbl, null);
      mSettingsPanel.add(mSerialPort, null);
      mSettingsPanel.add(mBaudLbl, null);
      mSettingsPanel.add(mBaud, null);
      mSettingsPanel.add(mGlovePositionLbl, null);
      mSettingsPanel.add(mGlovePosition, null);
      mSettingsPanel.add(mDeviceHostLbl, null);
      mSettingsPanel.add(mDeviceHost, null);

      mProxyTree.setModel(new ProxyTreeModel(mConfigElement, mConfigContext, 0, 5, 0));
   }
}
