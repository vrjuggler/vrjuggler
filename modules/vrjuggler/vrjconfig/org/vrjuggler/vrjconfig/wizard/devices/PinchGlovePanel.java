/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.vrjconfig.wizard.devices;

import java.util.Iterator;
import java.io.IOException;
import javax.swing.*;
import java.awt.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.wizard.*;
import java.awt.event.*;
import javax.swing.border.*;

import org.vrjuggler.vrjconfig.wizard.*;
import org.vrjuggler.vrjconfig.wizard.WizardComboBoxModel;


public class PinchGlovePanel extends JPanel
{
  BorderLayout baseLayout = new BorderLayout();
  JPanel centerPnl = new JPanel();
  BorderLayout centerLayout = new BorderLayout();


  ////////
  private ConfigContext mContext = null;
  private ConfigBroker mBroker = null;
  private TitledBorder titledBorder1;
  private JPanel jPanel3 = new JPanel();
  private JPanel jPanel2 = new JPanel();
  private JPanel directionsPanel = new JPanel();
  private JLabel jLabel2 = new JLabel();
  private BorderLayout borderLayout1 = new BorderLayout();
  private JLabel lblTitle = new JLabel();
  private JLabel lblDirections = new JLabel();
  private JLabel fakespaceImage = new JLabel();
  ///////
  private String mFileSourceName = null;
  private WizardComboBoxModel cbGlovePositionModel = null;
  private WizardComboBoxModel cbDeviceHostModel = null;
  private JPanel jPanel1 = new JPanel();
  private JComboBox cbGlovePosition = new JComboBox();
  private JTextField textPort = new JTextField();
  private JTextField textBaud = new JTextField();
  private JComboBox cbDeviceHost = new JComboBox();
  private GridLayout gridLayout1 = new GridLayout();
  private JLabel lblPort = new JLabel();
  private JLabel lblGlovePosition = new JLabel();
  private JLabel lblBaud = new JLabel();
  private JLabel lblDeviceHost = new JLabel();
  private JTextField textName = new JTextField();
  private JLabel lblName = new JLabel();

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

     this.loadLabelIcon(jLabel2, "org/vrjuggler/vrjconfig/wizard/devices/images/GloveIcon.png", "ICON");
     this.loadLabelIcon(fakespaceImage, "org/vrjuggler/vrjconfig/wizard/devices/images/PinchGlove.jpg", "PinchGlove");
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
    try{
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
    titledBorder1 = new TitledBorder("");
    this.setLayout(baseLayout);
    jLabel2.setIcon(new ImageIcon("/home/users/aronb/Source/ClusterWizard/images/GloveIcon.png"));

    directionsPanel.setBorder(BorderFactory.createEtchedBorder());
    directionsPanel.setLayout(borderLayout1);
    fakespaceImage.setIcon(new ImageIcon("/home/users/aronb/Source/ClusterWizard/images/PinchGlove.jpg"));
    lblTitle.setFont(new java.awt.Font("Serif", 1, 20));
    lblTitle.setHorizontalAlignment(SwingConstants.LEFT);
    lblTitle.setText("FakeSpace PinchGlove");
    lblDirections.setText("Fill in the following fields for the your PinchGlove device.");
    baseLayout.setHgap(5);
    baseLayout.setVgap(5);
    centerPnl.setLayout(centerLayout);
    textBaud.setText("9600");
    jPanel1.setLayout(gridLayout1);
    gridLayout1.setColumns(2);
    gridLayout1.setRows(5);
    lblPort.setText("Port");
    lblGlovePosition.setText("Glove Position");
    lblBaud.setText("Baud");
    lblDeviceHost.setText("Device Host");
    textPort.setText("/dev/ttyd4");
    lblName.setText("Name");
    textName.setText("PinchGlove");
    this.add(centerPnl, BorderLayout.CENTER);
    centerPnl.add(fakespaceImage,  BorderLayout.WEST);
    centerPnl.add(jPanel1,  BorderLayout.CENTER);
    jPanel1.add(lblName, null);
    jPanel1.add(textName, null);
    jPanel1.add(lblPort, null);
    jPanel1.add(textPort, null);
    jPanel1.add(lblBaud, null);
    jPanel1.add(textBaud, null);
    jPanel1.add(lblGlovePosition, null);
    jPanel1.add(cbGlovePosition, null);
    jPanel1.add(lblDeviceHost, null);
    jPanel1.add(cbDeviceHost, null);
    directionsPanel.add(jPanel3, BorderLayout.EAST);
    jPanel3.add(jLabel2, null);
    directionsPanel.add(jPanel2, BorderLayout.CENTER);
    jPanel2.add(lblTitle, null);
    jPanel2.add(lblDirections, null);
    this.add(directionsPanel, BorderLayout.NORTH);
  }
  public void init()
  {
    // Get handle to broker
    mBroker = new ConfigBrokerProxy();

    // Create a context
    mContext = new ConfigContext();
    mContext.add(mFileSourceName);

    cbGlovePositionModel = new WizardComboBoxModel();
    cbGlovePositionModel.setFileSource(mFileSourceName);
    cbGlovePositionModel.addChunkType("proxyAlias");
    cbGlovePositionModel.addChunkType("PosProxy");
    cbGlovePosition.setModel(cbGlovePositionModel);

    cbDeviceHostModel = new WizardComboBoxModel();
    cbDeviceHostModel.setFileSource(mFileSourceName);
    cbDeviceHostModel.addChunkType("MachineSpecific");
    cbDeviceHost.setModel(cbDeviceHostModel);
    textBaud.setText("9600");
    textPort.setText("/dev/ttyd4");
    textName.setText("PinchGlove");
  }

  public boolean saveFile()
  {
    DataSource mFileSource = mBroker.get(mFileSourceName);
    try{
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
    if (ConfigUtilities.getChunksWithName(mBroker.getChunks(mContext), textName.getText()).size() != 0)
    {
      int result = JOptionPane.showConfirmDialog(this,
          "ConfigChunk by this name already exists. "
         +"Do you want to replace it?.","ClusterWizard",JOptionPane.YES_NO_OPTION);
      if(result == JOptionPane.NO_OPTION)
      {
        return false;
      }

      //Remove all old chunks by this name
      java.util.List old_list =
          ConfigUtilities.getChunksWithName(mBroker.getChunks(mContext), textName.getText());

      for(int i=0;i<old_list.size();i++)
      {
        mBroker.remove(mContext,(ConfigChunk)old_list.get(i));
      }
    }

    ConfigChunk chunk = ChunkFactory.createConfigChunk("PinchGlove");
    chunk.setName(textName.getText());
    chunk.setProperty("port",0,textPort.getText());
    chunk.setProperty("baud",0,textBaud.getText());
    chunk.setProperty("deviceHost",0,(String)cbDeviceHost.getSelectedItem());
    chunk.setProperty("glovePos",0,(String)cbGlovePosition.getSelectedItem());
    mBroker.add(mContext, chunk);
    return true;
  }
}

/*
<ChunkDesc token="PinchGlove" name="PinchGlove" version="1.0">
  <category>Devices</category>
  <help>Configuration for the PinchGlove</help>
  <PropertyDesc token="port" name="Port" type="string" variable="0">
    <help>Serial port that the PinchGlove is connected to</help>
    <item label="Port" defaultvalue="/dev/ttyd1" />
  </PropertyDesc>
  <PropertyDesc token="baud" name="Baud Rate" type="int" variable="0">
    <help>Serial port speed</help>
    <item label="Baud Rate" defaultvalue="9600" />
  </PropertyDesc>
  <PropertyDesc token="glovePos" name="Glove Position" type="chunk" variable="0">
    <help>The position proxy for the glove position. i.e. the tracker on the glove.</help>
    <item label="Glove Position" defaultvalue="" />
    <allowedtype>proxyAlias</allowedtype>
    <allowedtype>PosProxy</allowedtype>
  </PropertyDesc>
  <PropertyDesc name="Host Node" token="deviceHost" type="chunk" variable="0">
    <help>Points to the machine that the device is physically connected to.</help>
    <item label="Host Node" defaultvalue="" />
    <allowedtype>MachineSpecific</allowedtype>
  </PropertyDesc>
</ChunkDesc>
*/
