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
package org.vrjuggler.vrjconfig.wizard.devices;

import java.awt.*;
import javax.swing.*;
import javax.swing.border.*;

import org.vrjuggler.jccl.config.*;


public class PinchGlovePanel extends JPanel
{
   BorderLayout baseLayout = new BorderLayout();
   JPanel centerPnl = new JPanel();
   BorderLayout centerLayout = new BorderLayout();


   ////////
   private ConfigContext        mContext = null;
   private ConfigBroker         mBroker = null;
   private JPanel jPanel3 = new JPanel();
   private JPanel jPanel2 = new JPanel();
   private JPanel               mDirectionsPanel = new JPanel();
   private JLabel               mGloveIconLbl = new JLabel();
   private BorderLayout         mDirectionsBorderLayout = new BorderLayout();
   private JLabel               mTitleLbl = new JLabel();
   private JLabel               mDirectionsLbl = new JLabel();
   private JLabel               mPinchGloveImageLbl = new JLabel();
   ///////
   private String               mFileSourceName = null;
   private WizardComboBoxModel  mGlovePositionModel = null;
   private WizardComboBoxModel  mDeviceHostModel = null;
   private JPanel jPanel1 = new JPanel();
   private JComboBox            mGlovePosition = new JComboBox();
   private JTextField           mSerialPort = new JTextField();
   private JTextField           mBaud = new JTextField();
   private JComboBox            mDeviceHost = new JComboBox();
   private GridLayout gridLayout1 = new GridLayout();
   private JLabel               mPortLbl = new JLabel();
   private JLabel               mGlovePositionLbl = new JLabel();
   private JLabel               mBaudLbl = new JLabel();
   private JLabel               mDeviceHostLbl = new JLabel();
   private JTextField           mName = new JTextField();
   private JLabel               mNameLbl = new JLabel();
   private Box                  mInfoBox = Box.createVerticalBox();
   private Component            mInfoGlue = Box.createVerticalGlue();

   public PinchGlovePanel()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      this.loadLabelIcon(mGloveIconLbl, "org/vrjuggler/vrjconfig/wizard/devices/images/GloveIcon.png", "ICON");
      this.loadLabelIcon(mPinchGloveImageLbl, "org/vrjuggler/vrjconfig/wizard/devices/images/PinchGlove.jpg", "PinchGlove");
   }

   public void setFileSourceName(String file_name)
   {
      mFileSourceName = file_name;
   }
   
   public String getFileSourceName()
   {
      return(mFileSourceName);
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
      this.setLayout(baseLayout);
      mGloveIconLbl.setIcon(new ImageIcon("/home/users/aronb/Source/ClusterWizard/images/GloveIcon.png"));

      mDirectionsPanel.setBorder(BorderFactory.createEtchedBorder());
      mDirectionsPanel.setLayout(mDirectionsBorderLayout);
      mPinchGloveImageLbl.setIcon(new ImageIcon("/home/users/aronb/Source/ClusterWizard/images/PinchGlove.jpg"));
      mTitleLbl.setFont(new java.awt.Font("Serif", 1, 20));
      mTitleLbl.setHorizontalAlignment(SwingConstants.LEFT);
      mTitleLbl.setText("FakeSpace PinchGlove");
      mDirectionsLbl.setText("Fill in the following fields for the your PinchGlove device.");
      baseLayout.setHgap(5);
      baseLayout.setVgap(5);
      centerPnl.setLayout(centerLayout);
      mBaud.setText("9600");
      jPanel1.setLayout(gridLayout1);
      gridLayout1.setColumns(2);
      gridLayout1.setRows(5);
      mPortLbl.setText("Port");
      mGlovePositionLbl.setText("Glove Position");
      mBaudLbl.setText("Baud");
      mDeviceHostLbl.setText("Device Host");
      mSerialPort.setText("/dev/ttyd4");
      mNameLbl.setText("Name");
      mName.setText("PinchGlove");
      this.add(centerPnl, BorderLayout.CENTER);
      centerPnl.add(mPinchGloveImageLbl,  BorderLayout.WEST);
      centerPnl.add(mInfoBox,  BorderLayout.CENTER);
      mInfoBox.add(mNameLbl, null);
      mInfoBox.add(mName, null);
      mInfoBox.add(mPortLbl, null);
      mInfoBox.add(mSerialPort, null);
      mInfoBox.add(mBaudLbl, null);
      mInfoBox.add(mBaud, null);
      mInfoBox.add(mGlovePositionLbl, null);
      mInfoBox.add(mGlovePosition, null);
      mInfoBox.add(mDeviceHostLbl, null);
      mInfoBox.add(mDeviceHost, null);
      mInfoBox.add(mInfoGlue, null);
      mDirectionsPanel.add(jPanel3, BorderLayout.EAST);
      jPanel3.add(mGloveIconLbl, null);
      mDirectionsPanel.add(jPanel2, BorderLayout.CENTER);
      jPanel2.add(mTitleLbl, null);
      jPanel2.add(mDirectionsLbl, null);
      this.add(mDirectionsPanel, BorderLayout.NORTH);
   }
   
   public void init()
   {
      // Get handle to broker
      mBroker = new ConfigBrokerProxy();

      // Create a context
      mContext = new ConfigContext();
      mContext.add(mFileSourceName);

      mGlovePositionModel = new WizardComboBoxModel();
      mGlovePositionModel.setFileSource(mFileSourceName);
      mGlovePositionModel.addElementType("alias");
      mGlovePositionModel.addElementType("position_proxy");
      mGlovePosition.setModel(mGlovePositionModel);

      mDeviceHostModel = new WizardComboBoxModel();
      mDeviceHostModel.setFileSource(mFileSourceName);
      mDeviceHostModel.addElementType("machine_specific");
      mDeviceHost.setModel(mDeviceHostModel);
      mBaud.setText("9600");
      mSerialPort.setText("/dev/ttyS0");
      mName.setText("PinchGlove");
   }

   public boolean saveFile()
   {
      DataSource mFileSource = mBroker.get(mFileSourceName);
      try
      {
         mFileSource.commit();
      }
      catch(java.io.IOException exp)
      {
         exp.printStackTrace();
      }
      return(true);
   }

   public boolean createPinchGloveConfigChunk()
   {
      if (ConfigUtilities.getElementsWithName(mBroker.getElements(mContext), mName.getText()).size() != 0)
      {
         int result = JOptionPane.showConfirmDialog(this, "ConfigChunk by this name already exists. "
            +"Do you want to replace it?.", "ClusterWizard", JOptionPane.YES_NO_OPTION);
         
         if(result == JOptionPane.NO_OPTION)
         {
            return false;
         }

         //Remove all old chunks by this name
         java.util.List old_list =
            ConfigUtilities.getElementsWithName(mBroker.getElements(mContext), mName.getText());

         for(int i=0;i<old_list.size();i++)
         {
            mBroker.remove(mContext,(ConfigElement)old_list.get(i));
         }
      }

      ConfigElementFactory factory = new ConfigElementFactory(mBroker.getRepository().getAllLatest());
      ConfigElement glove = factory.create(mName.getText(), "pinch_glove");
      glove.setProperty("port",0,mSerialPort.getText());
      glove.setProperty("baud",0,mBaud.getText());
      glove.setProperty("device_host",0,(String)mDeviceHost.getSelectedItem());
      glove.setProperty("glove_position",0,(String)mGlovePosition.getSelectedItem());
      mBroker.add(mContext, glove);
      return true;
   }
}
