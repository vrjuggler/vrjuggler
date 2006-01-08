/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
package org.vrjuggler.vrjconfig.wizards.vrsystem;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.border.*;

import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.vrjconfig.commoneditors.ConfigPtrListModel;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;

public class NodeSettingsPanel extends JPanel implements EditorConstants
{
   BorderLayout baseLayout = new BorderLayout();
   JPanel mLowerPanel = new JPanel();
   private TableLayout mLowerPanelLayout = null;

   private Icon mAddIcon = null;
   private Icon mRemoveIcon = null;
   private Icon mDisabledRemoveIcon = null;
   
   private JScrollPane mListScrollPane1 = new JScrollPane();
   private JButton mAddNodeBtn = new JButton();
   private JButton mRemoveNodeBtn = new JButton();
   private JLabel mHostnameLabel = new JLabel();
   private JTextField mHostnameField = new JTextField();
   
   private ConfigPtrListModel mNodesListModel = null;
   private JList lstNodes = new JList();

   private ConfigContext mContext = null;
   private ConfigBroker mBroker = null;
   private TitledBorder titledBorder1;
   private JPanel mDirectionsPanel = new JPanel();
   private TableLayout mDirectionsPanelLayout= null;
   private JLabel mTitleLabel = new JLabel();
   private JTextArea mDirectionsLabel = new JTextArea();
   private JLabel mIconLabel = new JLabel();
   private Icon mClusterIcon = null;

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
      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         mClusterIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/ClusterIcon.png"));
         
         mIconLabel.setIcon(mClusterIcon);
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         mIconLabel.setText("");
      }
   }

   private void jbInit() throws Exception
   {
      titledBorder1 = new TitledBorder("");
      this.setLayout(baseLayout);

      double[][] lower_size = {{TableLayout.PREFERRED, TableLayout.FILL, 25},
                              {25, 25, TableLayout.FILL}};
      mLowerPanelLayout = new TableLayout(lower_size);
      
      mLowerPanel.setLayout(mLowerPanelLayout);
      
      
      
      double[][] dir_size = {{TableLayout.FILL, TableLayout.PREFERRED},
                              {TableLayout.PREFERRED, TableLayout.FILL}};
      mDirectionsPanelLayout = new TableLayout(dir_size);
      mDirectionsPanel.setLayout(mDirectionsPanelLayout);
      
      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         mAddIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/add.gif"));
         mRemoveIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/remove.gif"));
         mDisabledRemoveIcon = new ImageIcon(loader.getResource(COMMON_IMG_ROOT +
                                                          "/remove_disabled.gif"));
         
         mAddNodeBtn.setIcon(mAddIcon);
         mRemoveNodeBtn.setIcon(mRemoveIcon);
         mRemoveNodeBtn.setDisabledIcon(mDisabledRemoveIcon);
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         mAddNodeBtn.setText("Add");
         mRemoveNodeBtn.setText("Remove");
      }


      
      /*
      mAddNodeBtn.setMaximumSize(new Dimension(130, 33));
      mAddNodeBtn.setMinimumSize(new Dimension(130, 33));
      mAddNodeBtn.setPreferredSize(new Dimension(130, 33));
      mAddNodeBtn.setText("Add Node");
      */
      mAddNodeBtn.addActionListener(new java.awt.event.ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               mAddNodeBtn_actionPerformed(e);
            }
         });
      /*
      mRemoveNodeBtn.setMaximumSize(new Dimension(130, 33));
      mRemoveNodeBtn.setMinimumSize(new Dimension(130, 33));
      mRemoveNodeBtn.setPreferredSize(new Dimension(130, 33));
      mRemoveNodeBtn.setText("Remove Node");
      */
      mRemoveNodeBtn.addActionListener(new java.awt.event.ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               mRemoveNode();
            }
         });
      mHostnameLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mHostnameLabel.setLabelFor(mHostnameField);
      mHostnameLabel.setText("Hostname:");

      mHostnameField.addActionListener(new java.awt.event.ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               mAddNodeBtn_actionPerformed(e);
            }
         });
      mDirectionsPanel.setBorder(BorderFactory.createEtchedBorder());
      mTitleLabel.setFont(new java.awt.Font("Serif", 1, 20));
      mTitleLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mTitleLabel.setText("Add Cluster Nodes");
      mDirectionsLabel.setText("Click on the add button to add nodes to your cluster configuration.");
      mDirectionsLabel.setLineWrap(true);
      mDirectionsLabel.setEditable(false);
      mDirectionsLabel.setBackground(mTitleLabel.getBackground());
      
      baseLayout.setHgap(5);
      baseLayout.setVgap(5);
      mLowerPanel.add(mHostnameLabel,
                             new TableLayoutConstraints(0, 0, 0, 0,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mLowerPanel.add(mHostnameField,
                             new TableLayoutConstraints(1, 0, 1, 0,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mLowerPanel.add(mListScrollPane1,
                             new TableLayoutConstraints(0, 1, 1, 2,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mLowerPanel.add(mAddNodeBtn,
                             new TableLayoutConstraints(2, 0, 2, 0,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mLowerPanel.add(mRemoveNodeBtn,
                             new TableLayoutConstraints(2, 1, 2, 1,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      this.add(mLowerPanel, BorderLayout.CENTER);
      mListScrollPane1.getViewport().add(lstNodes, null);
      mDirectionsPanel.add(mTitleLabel,
                             new TableLayoutConstraints(0, 0, 0, 0,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mDirectionsPanel.add(mDirectionsLabel,
                             new TableLayoutConstraints(0, 1, 0, 1,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mDirectionsPanel.add(mIconLabel,
                             new TableLayoutConstraints(1, 0, 1, 1,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      this.add(mDirectionsPanel, BorderLayout.NORTH);
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

      mHostnameField.setText("");
   }

   private void mAddNodeBtn_actionPerformed(ActionEvent e)
   {
      String host_name = mHostnameField.getText().trim();
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
      mHostnameField.setText("");
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

   private void mRemoveNode()
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
