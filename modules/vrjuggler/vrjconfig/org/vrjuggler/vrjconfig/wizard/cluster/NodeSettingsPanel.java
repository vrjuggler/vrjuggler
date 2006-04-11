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
package org.vrjuggler.vrjconfig.wizard.cluster;

import java.util.Iterator;
import java.io.IOException;
import javax.swing.*;
import java.awt.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.wizard.*;
import java.awt.event.*;
import javax.swing.border.*;
import org.vrjuggler.vrjconfig.wizard.WizardListModel;

public class NodeSettingsPanel extends JPanel
{
  BorderLayout baseLayout = new BorderLayout();
  JPanel centerPnl = new JPanel();
  BorderLayout centerLayout = new BorderLayout();
  private JPanel leftCenterPanel = new JPanel();

  private Box menuBox;

  private JPanel menuPanel = new JPanel();
  private JScrollPane listScrollPane1 = new JScrollPane();
  private JButton addNode = new JButton();
  private JButton removeNode = new JButton();
  private JPanel entryPanel = new JPanel();
  private JLabel lblHostname = new JLabel();
  private BorderLayout leftCenterBorderLayout = new BorderLayout();
  private GridLayout entryPanelGridLayout = new GridLayout();
  private JTextField txtHostname = new JTextField();
//  private DefaultListModel lstModelNodes = new DefaultListModel();
  private WizardListModel lstModelNodes = null;
  private JList lstNodes = new JList();

  private String mFileSourceName = null;

  ////////
  private ConfigContext mContext = null;
  private ConfigBroker mBroker = null;
  private Component topSpacer;
  private TitledBorder titledBorder1;
  private JPanel jPanel3 = new JPanel();
  private JPanel jPanel2 = new JPanel();
  private JPanel directionsPanel = new JPanel();
  private JLabel jLabel2 = new JLabel();
  private BorderLayout borderLayout1 = new BorderLayout();
  private JLabel lblTitle = new JLabel();
  private JLabel lblDirections = new JLabel();
  ///////

  public NodeSettingsPanel()
  {
     try
     {
       jbInit();
     }
     catch(Exception e)
     {
        e.printStackTrace();
     }

     try
     {
       jLabel2.setIcon(new ImageIcon(this.getClass().getClassLoader().getResource("org/vrjuggler/vrjconfig/wizard/cluster/images/ClusterIcon.png")));
     }
     catch(NullPointerException exp)
     {
       jLabel2.setText("");
     }
  }

  private void jbInit() throws Exception
  {
    titledBorder1 = new TitledBorder("");
    this.setLayout(baseLayout);

    menuBox = Box.createVerticalBox();
    topSpacer = Box.createVerticalStrut(8);

    addNode.setMaximumSize(new Dimension(130, 33));
    addNode.setMinimumSize(new Dimension(130, 33));
    addNode.setPreferredSize(new Dimension(130, 33));
    addNode.setText("Add Node");
    addNode.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        //addNode_actionPerformed(e);
        addNode();
      }
    });
    removeNode.setMaximumSize(new Dimension(130, 33));
    removeNode.setMinimumSize(new Dimension(130, 33));
    removeNode.setPreferredSize(new Dimension(130, 33));
    removeNode.setText("Remove Node");
    removeNode.addActionListener(new java.awt.event.ActionListener()
    {
      public void actionPerformed(ActionEvent e)
      {
        removeNode();
      }
    });
    lblHostname.setText("Hostname");
    leftCenterPanel.setLayout(leftCenterBorderLayout);
    entryPanel.setLayout(entryPanelGridLayout);
    entryPanelGridLayout.setColumns(2);
    txtHostname.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        addNode();
      }
    });
    directionsPanel.setBorder(BorderFactory.createEtchedBorder());
    directionsPanel.setLayout(borderLayout1);
    jLabel2.setIcon(new ImageIcon("/home/users/aronb/Source/ClusterWizard/images/ClusterIcon.png"));
    lblTitle.setFont(new java.awt.Font("Serif", 1, 20));
    lblTitle.setHorizontalAlignment(SwingConstants.LEFT);
    lblTitle.setText("Add Cluster Nodes");
    lblDirections.setText("Click on the add button to add nodes to your cluster configuration.");
    this.menuPanel.add(menuBox, BorderLayout.CENTER);
    menuBox.add(addNode, null);
    menuBox.add(topSpacer, null);
    menuBox.add(removeNode, null);
    baseLayout.setHgap(5);
    baseLayout.setVgap(5);
    centerPnl.setLayout(centerLayout);
    this.add(centerPnl, BorderLayout.CENTER);
    centerPnl.add(leftCenterPanel,  BorderLayout.CENTER);
    leftCenterPanel.add(entryPanel,  BorderLayout.NORTH);
    entryPanel.add(lblHostname, null);
    entryPanel.add(txtHostname, null);
    leftCenterPanel.add(listScrollPane1,  BorderLayout.CENTER);
    listScrollPane1.getViewport().add(lstNodes, null);
    centerPnl.add(menuPanel,  BorderLayout.EAST);
    directionsPanel.add(jPanel3, BorderLayout.EAST);
    jPanel3.add(jLabel2, null);
    directionsPanel.add(jPanel2, BorderLayout.CENTER);
    jPanel2.add(lblTitle, null);
    jPanel2.add(lblDirections, null);
    this.add(directionsPanel, BorderLayout.NORTH);
  }

  public void setFileSourceName(String file_name)
  {
    mFileSourceName = file_name;
  }
  public String getFileSourceName()
  {
    return(mFileSourceName);
  }


  public void init()
  {
    // Get handle to broker
    mBroker = new ConfigBrokerProxy();

    // Create a context
    mContext = new ConfigContext();
    mContext.add(mFileSourceName);

    lstModelNodes = new WizardListModel();
    lstModelNodes.setFileSource(mFileSourceName);
    lstModelNodes.addChunkType("MachineSpecific");
    lstNodes.setModel(lstModelNodes);

    txtHostname.setText("");
  }

  public boolean saveFile()
  {
    createClusterManagerChunk();
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
  private void addNode()
  {
    String host_name = txtHostname.getText().trim();
    String chunk_name = "Node(" + host_name + ")";
    java.util.List chunks_list = mBroker.getChunks(mContext);
    java.util.List matches = ConfigUtilities.getChunksWithName(chunks_list, chunk_name);

    if (!host_name.equals("") && matches.size() == 0)
    {
      // Create a MachineSpecific Chunk for the node
      ConfigChunk chunk = ChunkFactory.createConfigChunk("MachineSpecific");
      // Generate a default name for the chunk using the hostname
      chunk.setName(chunk_name);
      chunk.setProperty("host_name",0,host_name);
      chunk.setProperty("listen_port",0,"7000");
      mBroker.add(mContext, chunk);
      update();
    }
    txtHostname.setText("");
  }

  private void update()
  {
/*    lstModelNodes.clear();
    java.util.List chunks_list = mBroker.getChunks(mContext);
    java.util.List matches = ConfigUtilities.getChunksWithDescToken(chunks_list, "MachineSpecific");
    for(int i = 0; i < matches.size() ; i++)
    {
      lstModelNodes.addElement(((ConfigChunk)matches.get(i)).getName());
    }
*/
  }
  private void createClusterManagerChunk()
  {

    ConfigChunk cluster_manager = ChunkFactory.createConfigChunk("ClusterManager");
    cluster_manager.setName("Sample Cluster");
    cluster_manager.setProperty("barrier_master",0, (String)lstModelNodes.get(0));
    for(int i = 0 ; i < lstModelNodes.size() ; i++)
    {
      cluster_manager.setProperty("cluster_nodes",i,(String)lstModelNodes.get(0));
    }
    cluster_manager.setProperty("clusterPlugin",0,"RemoteInputManager");
    cluster_manager.setProperty("clusterPlugin",1,"ApplicationDataManager");
    mBroker.add(mContext, cluster_manager);
    //  cluster_manager.setProperty("clusterPlugin",2,"SwapLockPlugin");
  }

  private void removeNode()
  {
    String old_node = (String)lstNodes.getSelectedValue();

    java.util.List remove_list =
        ConfigUtilities.getChunksWithName(mBroker.getChunks(mContext), old_node);
    for(int i = 0 ; i < remove_list.size() ; i++)
    {
      mBroker.remove(mContext, (ConfigChunk)remove_list.get(i));
    }
    update();
  }
}