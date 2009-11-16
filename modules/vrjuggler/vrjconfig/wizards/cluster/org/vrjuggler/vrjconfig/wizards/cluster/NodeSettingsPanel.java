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
package org.vrjuggler.vrjconfig.wizards.cluster;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.border.*;

import org.vrjuggler.jccl.config.*;

public class NodeSettingsPanel
   extends JPanel
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
         jLabel2.setIcon(new ImageIcon(this.getClass().getClassLoader().getResource("org/vrjuggler/vrjconfig/wizards/cluster/images/ClusterIcon.png")));
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
      addNode.addActionListener(new java.awt.event.ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
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
      txtHostname.addActionListener(new java.awt.event.ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
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

   public void init(Map whiteboard)
   {
      mWhiteBoard = whiteboard;

      // Get handle to broker
      mBroker = new ConfigBrokerProxy();

      // Create a context
      mContext = new ConfigContext();
      mContext.add((String)mWhiteBoard.get("datasource.name"));

      lstModelNodes = new WizardListModel();
      lstModelNodes.setFileSource((String)mWhiteBoard.get("datasource.name"));
      lstModelNodes.addElementType("machine_specific");
      lstNodes.setModel(lstModelNodes);

      txtHostname.setText("");
   }

   public boolean saveFile()
   {
      createClusterManagerElement();
      DataSource mFileSource = mBroker.get((String)mWhiteBoard.get("datasource.name"));
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
   private void addNode()
   {
      String host_name = txtHostname.getText().trim();
      String element_name = "Node(" + host_name + ")";
      java.util.List elts = mBroker.getElements(mContext);
      java.util.List matches = ConfigUtilities.getElementsWithDefinition(elts, element_name);

      if(!host_name.equals("") && matches.size() == 0)
      {
         // Create a machine_specific element for the node
         ConfigElementFactory factory = new ConfigElementFactory(mBroker.getRepository().getAllLatest());
         ConfigElement element = factory.create(element_name, "machine_specific");
         element.setProperty("host_name",0,host_name);
         element.setProperty("listen_port",0,"7000");
         mBroker.add(mContext, element);
         update();
      }
      txtHostname.setText("");
   }

   private void update()
   {
      /*
      lstModelNodes.clear();
      java.util.List elt_list = mBroker.getElements(mContext);
      java.util.List matches = ConfigUtilities.getElementsWithToken(elt_list, "machine_specific");
      for(int i = 0; i < matches.size() ; i++)
      {
         lstModelNodes.addElement(((ConfigElement)matches.get(i)).getName());
      }
      */
   }
   private void createClusterManagerElement()
   {
      ConfigElementFactory factory = new ConfigElementFactory(mBroker.getRepository().getAllLatest());
      ConfigElement cluster_manager = factory.create("Sample Cluster", "cluster_manager");
      for(Enumeration e = lstModelNodes.elements() ; e.hasMoreElements() ; )
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
      update();
   }
}
