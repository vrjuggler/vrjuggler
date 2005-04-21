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
package org.vrjuggler.vrjconfig.wizard.vrsystem;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.border.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.vrjconfig.commoneditors.ConfigPtrListModel;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;

public class NodeSettingsPanel extends JPanel implements EditorConstants
{
   BorderLayout baseLayout = new BorderLayout();
   JPanel mCenterPanel = new JPanel();
   BorderLayout mCenterLayout = new BorderLayout();
   private JPanel mLeftCenterPanel = new JPanel();
   private BorderLayout mLeftCenterBorderLayout = new BorderLayout();

   private Box menuBox;

   private JPanel mMenuPanel = new JPanel();
   private JScrollPane mListScrollPane1 = new JScrollPane();
   private JButton mAddNodeBtn = new JButton();
   private JButton removeNode = new JButton();
   private JPanel entryPanel = new JPanel();
   private JLabel lblHostname = new JLabel();
   private GridLayout entryPanelGridLayout = new GridLayout();
   private JTextField txtHostname = new JTextField();
   private ConfigPtrListModel mNodesListModel = null;
   private JList lstNodes = new JList();

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

   private Map mWhiteBoard;

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

      mAddNodeBtn.setMaximumSize(new Dimension(130, 33));
      mAddNodeBtn.setMinimumSize(new Dimension(130, 33));
      mAddNodeBtn.setPreferredSize(new Dimension(130, 33));
      mAddNodeBtn.setText("Add Node");
      mAddNodeBtn.addActionListener(new java.awt.event.ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               mAddNodeBtn_actionPerformed(e);
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
      mLeftCenterPanel.setLayout(mLeftCenterBorderLayout);
      entryPanel.setLayout(entryPanelGridLayout);
      entryPanelGridLayout.setColumns(2);
      txtHostname.addActionListener(new java.awt.event.ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               mAddNodeBtn_actionPerformed(e);
            }
         });
      directionsPanel.setBorder(BorderFactory.createEtchedBorder());
      directionsPanel.setLayout(borderLayout1);
      jLabel2.setIcon(new ImageIcon("/home/users/aronb/Source/ClusterWizard/images/ClusterIcon.png"));
      lblTitle.setFont(new java.awt.Font("Serif", 1, 20));
      lblTitle.setHorizontalAlignment(SwingConstants.LEFT);
      lblTitle.setText("Add Cluster Nodes");
      lblDirections.setText("Click on the add button to add nodes to your cluster configuration.");
      this.mMenuPanel.add(menuBox, BorderLayout.CENTER);
      menuBox.add(mAddNodeBtn, null);
      menuBox.add(topSpacer, null);
      menuBox.add(removeNode, null);
      baseLayout.setHgap(5);
      baseLayout.setVgap(5);
      mCenterPanel.setLayout(mCenterLayout);
      this.add(mCenterPanel, BorderLayout.CENTER);
      mCenterPanel.add(mLeftCenterPanel,  BorderLayout.CENTER);
      mLeftCenterPanel.add(entryPanel,  BorderLayout.NORTH);
      entryPanel.add(lblHostname, null);
      entryPanel.add(txtHostname, null);
      mLeftCenterPanel.add(mListScrollPane1,  BorderLayout.CENTER);
      mListScrollPane1.getViewport().add(lstNodes, null);
      mCenterPanel.add(mMenuPanel,  BorderLayout.EAST);
      directionsPanel.add(jPanel3, BorderLayout.EAST);
      jPanel3.add(jLabel2, null);
      directionsPanel.add(jPanel2, BorderLayout.CENTER);
      jPanel2.add(lblTitle, null);
      jPanel2.add(lblDirections, null);
      this.add(directionsPanel, BorderLayout.NORTH);
   }

   public void init(Map whiteboard)
   {
      mWhiteBoard = whiteboard;

      // Get handle to broker
      mBroker = new ConfigBrokerProxy();

      // Create a context
      mContext = (ConfigContext)mWhiteBoard.get("context");

      mNodesListModel = new ConfigPtrListModel(mContext);
      mNodesListModel.addElementType(CLUSTER_NODE_TYPE);
      mBroker.addConfigListener(mNodesListModel);
      lstNodes.setModel(mNodesListModel);

      txtHostname.setText("");
   }

   private void mAddNodeBtn_actionPerformed(ActionEvent e)
   {
      String host_name = txtHostname.getText().trim();
      String element_name = "Node(" + host_name + ")";
      java.util.List elts = mBroker.getElements(mContext);
      java.util.List matches = ConfigUtilities.getElementsWithDefinition(elts, element_name);

      if(!host_name.equals("") && matches.size() == 0)
      {
         // Create a cluster_node element for the node
         ConfigElementFactory factory = new ConfigElementFactory(mBroker.getRepository().getAllLatest());
         ConfigElement element = factory.create(element_name, CLUSTER_NODE_TYPE);
         mBroker.add(mContext, element);
         element.setProperty("host_name",0,host_name);
         element.setProperty("listen_port",0,"7000");
      }
      txtHostname.setText("");
   }

   private void createClusterManagerElement()
   {
      ConfigElementFactory factory = new ConfigElementFactory(mBroker.getRepository().getAllLatest());
      ConfigElement cluster_manager = factory.create("Sample Cluster", "cluster_manager");
      for(Enumeration e = mNodesListModel.elements() ; e.hasMoreElements() ; )
      {
         cluster_manager.addProperty("cluster_node", (String)e.nextElement());
      }

      cluster_manager.addProperty("plugin_path", "${VJ_BASE_DIR}/lib/gadgeteer/plugins/");
      cluster_manager.addProperty("plugin", "RemoteInputManager");
      cluster_manager.addProperty("plugin", "ApplicationDataManager");
      mBroker.add(mContext, cluster_manager);
   }

   private void removeNode()
   {
      String old_node = (String)lstNodes.getSelectedValue();

      java.util.List remove_list =
      ConfigUtilities.getElementsWithDefinition(mBroker.getElements(mContext), old_node);
      for(int i = 0 ; i < remove_list.size() ; i++)
      {
         mBroker.remove(mContext, (ConfigElement)remove_list.get(i));
      }
   }
}
