/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

package org.vrjuggler.vrjconfig.wizards.cluster;

import java.awt.*;
import java.awt.event.*;
import java.io.IOException;
import java.util.Iterator;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import javax.swing.*;
import javax.swing.border.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.wizard.*;

import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;

public class CreateClusteredSimDevicesPanel extends JPanel
{
   BorderLayout baseLayout = new BorderLayout();
   JPanel centerPnl = new JPanel();

   private DefaultComboBoxModel lstHeadNodes = new DefaultComboBoxModel();
   private DefaultComboBoxModel lstWandNodes = new DefaultComboBoxModel();
   private DefaultComboBoxModel lstCameraNodes = new DefaultComboBoxModel();
   // Contains all sim display windows and the display system after
   // copying the config files and before writing all machine specific details
   private List mDisplayElementList = new ArrayList();
   private List mDisplaySystemElementList = new ArrayList();

   private Map mWhiteBoard;

   private String mConfigFilePath =
       new String("${VJ_BASE_DIR}/${VJ_DATA_DIR}/data/configFiles/");
   private String mHeadEventWindowName = "Head Event Window";
   private String mSimViewEventWindowName = "Sim View Cameras Control";
   private String mWandEventWindowName = "Wand Event Window";

   private ConfigContext mContext = null;
   private TitledBorder titledBorder1;
   private JComboBox cbSimCameraKeyboard = new JComboBox();
   private JComboBox cbHeadKeyboard = new JComboBox();
   private JComboBox cbWandKeyboard = new JComboBox();
   private JLabel lblHeadKeyboard = new JLabel();
   private JLabel lblSimCameraKeyboard = new JLabel();
   private JLabel lblWandKeyboard = new JLabel();
   private Component component1;
   private Component component2;
   private Component component3;
   private Component component4;
   private Component component5;
   private Component component6;
   private Component component7;
   private Component component8;
   private GridLayout gridLayout1 = new GridLayout();

   public CreateClusteredSimDevicesPanel()
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
      setLayout(baseLayout);
      component1 = Box.createVerticalStrut(1);
      component2 = Box.createVerticalStrut(1);
      component3 = Box.createVerticalStrut(1);
      component4 = Box.createVerticalStrut(1);
      component5 = Box.createVerticalStrut(1);
      component6 = Box.createVerticalStrut(1);
      component7 = Box.createVerticalStrut(1);
      component8 = Box.createVerticalStrut(1);

      // Head Keyboard JComboBox
      cbHeadKeyboard.setActionCommand("headChanged");
      cbHeadKeyboard.setModel(lstHeadNodes);
      cbHeadKeyboard.addActionListener(new java.awt.event.ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               updateSimDevices(e);
            }
         });
      // Wand Keyboard JComboBox
      cbWandKeyboard.setActionCommand("wandChanged");
      cbWandKeyboard.setModel(lstWandNodes);
      cbWandKeyboard.addActionListener(new java.awt.event.ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               updateSimDevices(e);
            }
         });
      // Sim Camera Controls Keyboard JComboBox
      cbSimCameraKeyboard.setActionCommand("cameraChanged");
      cbSimCameraKeyboard.setModel(lstCameraNodes);
      cbSimCameraKeyboard.addActionListener(new java.awt.event.ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               updateSimDevices(e);
            }
         });

      lblHeadKeyboard.setToolTipText("");
      lblHeadKeyboard.setText("Head Keyboard");
      lblSimCameraKeyboard.setText("Sim Camera Keyboard");
      lblWandKeyboard.setText("Wand Keyboard");
      gridLayout1.setColumns(2);
      gridLayout1.setRows(7);
      directionsPanel.setBorder(BorderFactory.createEtchedBorder());
      directionsPanel.setLayout(borderLayout1);
      jLabel2.setIcon(new ImageIcon("/home/users/aronb/Source/ClusterWizard/images/ClusterIcon.png"));
      lblTitle.setFont(new java.awt.Font("Serif", 1, 20));
      lblTitle.setHorizontalAlignment(SwingConstants.LEFT);
      lblTitle.setText("Set Device Hosts");
      lblDirections.setText("Select the cluster node that you want to share" +
                            " each of the following keyboards from.");
      baseLayout.setHgap(5);
      baseLayout.setVgap(5);
      centerPnl.setLayout(gridLayout1);
      this.add(centerPnl, BorderLayout.CENTER);

      // Adding the comboboxes and making them spaced out
      centerPnl.add(component1, null);
      centerPnl.add(component2, null);
      centerPnl.add(lblHeadKeyboard, null);
      centerPnl.add(cbHeadKeyboard, null);
      centerPnl.add(component3, null);
      centerPnl.add(component4, null);
      centerPnl.add(lblWandKeyboard, null);
      centerPnl.add(cbWandKeyboard, null);
      centerPnl.add(component5, null);
      centerPnl.add(component6, null);
      centerPnl.add(lblSimCameraKeyboard, null);
      centerPnl.add(cbSimCameraKeyboard, null);
      centerPnl.add(component7, null);
      centerPnl.add(component8, null);
      jPanel2.add(lblTitle, null);
      jPanel2.add(lblDirections, null);
      directionsPanel.add(jPanel3, BorderLayout.EAST);
      jPanel3.add(jLabel2, null);
      directionsPanel.add(jPanel2, BorderLayout.CENTER);
      this.add(directionsPanel, BorderLayout.NORTH);
   }
   
   /**
    * Gets a handle to the configuration broker.
    */
   private ConfigBroker getBroker()
   {
      if (mBroker == null)
      {
         synchronized (this)
         {
            if (mBroker == null)
            {
               mBroker = new ConfigBrokerProxy();
            }
         }
      }
      return mBroker;
   }

   /** The handle to the configuration broker. */
   private ConfigBroker mBroker;

   public void init(Map whiteboard)
   {
      mWhiteBoard = whiteboard;
      
      // Create a context
      mContext = new ConfigContext();
      mContext.add((String)mWhiteBoard.get("datasource.name"));
   }

   private JPanel jPanel3 = new JPanel();
   private JPanel jPanel2 = new JPanel();
   private JPanel directionsPanel = new JPanel();
   private JLabel jLabel2 = new JLabel();
   private BorderLayout borderLayout1 = new BorderLayout();
   private JLabel lblTitle = new JLabel();
   private JLabel lblDirections = new JLabel();

   public void copySimFilesToContext()
   {
      try
      {
         // Set the paths to the known config files.
         String base_path = mConfigFilePath + "/sim.base.jconf";
         String wand_path = mConfigFilePath + "/sim.wand.mixin.jconf";

         // Expand in the cas of enviroment variables
         EnvironmentService env_service = new EnvironmentServiceProxy();
         base_path = env_service.expandEnvVars(base_path);
         wand_path = env_service.expandEnvVars(wand_path);

         // Load sim.base.jconf
         FileDataSource base_filesource = FileDataSource.open(base_path, getBroker().getRepository());
         getBroker().add("sim.base.jconf", base_filesource);

         // Load sim.wand.minxin.jconf
         FileDataSource wand_filesource = FileDataSource.open(wand_path, getBroker().getRepository());
         getBroker().add("sim.wand.mixin.jconf", wand_filesource);

         // Create a new context and add both file sources to it
         ConfigContext ctx = new ConfigContext();
         ctx.add("sim.base.jconf");
         ctx.add("sim.wand.mixin.jconf");

         // - Iterate over all elements in sim context.
         //   - if the element is a display_window.
         //     - Add to a list to be replicated on each node.
         //   - else if the element is a display_system.
         //     - Add to a list to be replicated on each node.
         //   - else
         //     - Move the element from the sim context into our new context.
         //     
         // NOTE: Since we are not saving the sim DataSources we will not
         //       lose any data.
         for(Iterator itr = getBroker().getElements(ctx).iterator() ; itr.hasNext() ; )
         {
            ConfigElement elt = (ConfigElement)itr.next();
            if(elt.getDefinition().getToken().equals("display_window"))
            {
               mDisplayElementList.add(elt);
            }
            else if(elt.getDefinition().getToken().equals("display_system"))
            {
               mDisplaySystemElementList.add(elt);
            }
            else
            {
               getBroker().remove(ctx, elt);
               getBroker().add(mContext, elt);
            }
         }
         getBroker().remove("sim.base.jconf");
         getBroker().remove("sim.wand.mixin.jconf");
      }
      catch(java.io.IOException exp)
      {
         exp.printStackTrace();
      }
   }

   /**
    * Add simulator display_system and display_windows to each node.
    */
   public void createDisplayElements()
   {
      // - Iterate over all cluster nodes.
      List matches = ConfigUtilities.getElementsWithDefinition(getBroker().getElements(mContext),
                                                     "machine_specific");
      for(Iterator itr = matches.iterator() ; itr.hasNext() ; )
      {
         ConfigElement node = (ConfigElement)itr.next();

         // Remove all old display_windows elements
         int num_displays = node.getPropertyValueCount("display_windows");
         for (int n = 0 ; n < num_displays ; n++)
         {
            node.removeProperty("display_windows", n);
         }
         // Add display_system element
         ConfigElement display_sys_elt = (ConfigElement)mDisplaySystemElementList.get(0);
         node.setProperty("display_system", 0, display_sys_elt);

         // Add every display_window element
         for(int j = 0 ; j < mDisplayElementList.size() ; j++)
         {
            ConfigElement display_elt = (ConfigElement)mDisplayElementList.get(j);
            node.addProperty("display_windows", display_elt);
         }
      }
   }

   public boolean saveFile()
   {
      try
      {
         getBroker().get((String)mWhiteBoard.get("datasource.name")).commit();
      }
      catch(java.io.IOException exp)
      {
         exp.printStackTrace();
      }
      return(true);
   }

   public void loadLists()
   {
      lstHeadNodes.removeAllElements();
      lstWandNodes.removeAllElements();
      lstCameraNodes.removeAllElements();

      List element_list = getBroker().getElements(mContext);
      List matches = ConfigUtilities.getElementsWithDefinition(element_list, "machine_specific");
      for(int i = 0; i < matches.size() ; i++)
      {
         lstHeadNodes.addElement(((ConfigElement)matches.get(i)).getName());
         lstWandNodes.addElement(((ConfigElement)matches.get(i)).getName());
         lstCameraNodes.addElement(((ConfigElement)matches.get(i)).getName());
      }
   }

   private void updateSimDevices(ActionEvent e)
   {
      JComboBox temp_cb = (JComboBox)e.getSource();
      String cb_name = temp_cb.getName();
      System.out.println(cb_name);

      if(e.getActionCommand().equals("headChanged"))
      {
         List element_list = ConfigUtilities.getElementsWithName(getBroker().getElements(mContext), mHeadEventWindowName);
         if (element_list.size() == 1)
         {
            String device_host = (String)cbHeadKeyboard.getSelectedItem();
            ((ConfigElement)element_list.get(0)).setProperty("device_host",0,device_host);
         }
         else
         {
            System.out.println("ERROR: You have more than two elements named, " + cb_name
                               + " in the current configureation");
         }
      }
      else if(e.getActionCommand().equals("cameraChanged"))
      {
         List element_list = ConfigUtilities.getElementsWithName(getBroker().getElements(mContext), mSimViewEventWindowName);
         if (element_list.size() == 1)
         {
            String device_host = (String)cbSimCameraKeyboard.getSelectedItem();
            ((ConfigElement)element_list.get(0)).setProperty("device_host",0,device_host);
         }
         else
         {
            System.out.println("ERROR: You have more than two elements named, " + cb_name
                               + " in the current configureation");
         }
      }
      else if(e.getActionCommand().equals("wandChanged"))
      {
         List element_list = ConfigUtilities.getElementsWithName(getBroker().getElements(mContext), mWandEventWindowName);
         if (element_list.size() == 1)
         {
            String device_host = (String)cbWandKeyboard.getSelectedItem();
            ((ConfigElement)element_list.get(0)).setProperty("device_host",0,device_host);
         }
         else
         {
            System.out.println("ERROR: You have more than two elements named, " + cb_name
                               + " in the current configureation");
         }
      }
   }
}
