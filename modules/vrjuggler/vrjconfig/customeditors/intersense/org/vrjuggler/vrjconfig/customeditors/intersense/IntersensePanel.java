/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
package org.vrjuggler.vrjconfig.customeditors.intersense;

import javax.swing.*;
import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.GridLayout;
import java.awt.Dimension;
import java.awt.Container;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.util.*;
import javax.swing.border.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.tree.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;
import org.jdom.*;

public class IntersensePanel extends JPanel implements CustomEditor
{
   /** Panel which contains all general Intersense information. */
   private JPanel mDeviceInfoPanel = new JPanel();
   private JPanel mTopSectionPanel = new JPanel();
   private GridLayout mInfoPanelLayout = new GridLayout();
   
   private JLabel               mPortLbl        = new JLabel();
   private PropertyEditorPanel  mPortEditor     = null;
   private JLabel               mDriverLbl      = new JLabel();
   private PropertyEditorPanel  mDriverEditor   = null;
   
   private JLabel               mISenseIconLbl  = new JLabel();
   
   /** ConfigElement for this Intersense device. */
   private ConfigElement mConfigElement = null;

   /** ConfigContext associated with this Intersense device. */
   private ConfigContext mConfigContext = null;

   /** Table that displays all stations for this Intersense device. */
   private JTable mStationsTable = new StationTable();
   private JPanel mStationsPanel = null;
   
   private JPanel mProxyPanel = new JPanel();

   /** Toolbar for controling ProxyTree. */
   private JToolBar mToolbar = new JToolBar();
   private JScrollPane mProxyTreeScrollPane = new JScrollPane();
   
   /** Representation of proxies pointing at this device. */
   private ProxyTree mProxyTree = new ProxyTree(); 
   
   /** The model that contains all information about this Intersense device. */
   private IntersenseModel mModel = null;
   
   private JButton mCreateBtn = new JButton();
   private JButton mRemoveBtn = new JButton();
   private JButton mAttachBtn = new JButton();
   private JButton mDisconnectBtn = new JButton();

   private JButton mVisualizeBtn = new JButton("3D Visualization");

   /**
    * Constructor.
    */
   public IntersensePanel()
   {
      // Register ourselves with the CustomEditorRegistry.
      CustomEditorRegistry.registerEditor("intersense_api",  IntersensePanel.class);

      mStationsTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      
      // Change the ProxyTree when we select a station.
      mStationsTable.getSelectionModel().addListSelectionListener(new ListSelectionListener()
            {
               public void valueChanged(ListSelectionEvent e)
               {
                  //Ignore extra messages.
                  if (e.getValueIsAdjusting()) return;
                  
                  ListSelectionModel lsm = (ListSelectionModel)e.getSource();
                  
                  if (!lsm.isSelectionEmpty())
                  {
                     int selectedRow = lsm.getMinSelectionIndex();
                     Object value = mStationsTable.getModel().getValueAt(selectedRow, 0);
                     StationModel station_model = (StationModel)value;
                     mProxyTree.setModel(station_model.getProxyModel());
                     mProxyTree.expandAll(true);
                  }
               }
            });
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
         mCreateBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/Add16.gif")));
         mRemoveBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/Delete16.gif")));
         mAttachBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/Import16.gif")));
         mDisconnectBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/Export16.gif")));
         mISenseIconLbl.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/intersense/images/IS_logo.gif")));
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         mCreateBtn.setText("New");
         mRemoveBtn.setText("Delete");
         mAttachBtn.setText("Attach");
         mDisconnectBtn.setText("Disconnect");
         System.out.println(e);
         e.printStackTrace();
      }
   }

   /**
    * Specify the ConfigContext and ConfigElement for this Intersense device.
    */
   public void setConfig(ConfigContext ctx, ConfigElement elm)
   {
      mConfigContext = ctx;
      mConfigElement = elm;
      if(null == mConfigContext)
      {
         System.out.println("ERROR: You must set the ConfigContext before you set the ConfigElement.");
      }
      IntersenseModel isense_model = new IntersenseModel(mConfigElement, mConfigContext);
      setModel(isense_model);
      init();
   }

   /**
    * Specify the IntersenseModel that this panel should display.
    */
   public void setModel(IntersenseModel model)
   {
      mModel = model;

      // Create a model to use in the station table.
      StationTableModelAdaptor stations_model = new StationTableModelAdaptor(mModel);
      mStationsTable.setModel(stations_model);
     
      // Force the ProxyTree to display information about the first station by default.
      // TODO: Make it display nothing by default.
      List station_models = mModel.getStationModels();
      if (station_models.size() > 0)
      {
         StationModel station_model = (StationModel)station_models.get(0);
         mProxyTree.setModel((DefaultTreeModel)station_model.getProxyModel());
         mProxyTree.expandAll(true);
      }
   }

   /**
    * Get the panel for this CustomEditor.
    */
   public Container getPanel()
   {
      return this;
   }

   /**
    * Return the title of this CustomEditor.
    */
   public String getTitle()
   {
      return("Intersense Editor");
   }

   public ActionListener getHelpActionListener()
   {
      return null;
   }

   public void editorClosing()
   {
   }

   /**
    * Initialize the GUI created with JBuilder.
    */
   private void jbInit() throws Exception
   {
      // Make sure that the table's panel does not grow 
      // any larger than the preferred size.
      mStationsPanel = new JPanel()
      {
         public Dimension getMaximumSize()
         {
            return getPreferredSize();
         }
      };
      mCreateBtn.setToolTipText("Create New Proxy");
      mCreateBtn.setActionCommand("create");
      mCreateBtn.setFocusPainted(false);
      mRemoveBtn.setToolTipText("Remove Proxy");
      mRemoveBtn.setActionCommand("remove");
      mRemoveBtn.setFocusPainted(false);
      mAttachBtn.setToolTipText("Attach Existing Proxy");
      mAttachBtn.setActionCommand("attach");
      mAttachBtn.setFocusPainted(false);
      mDisconnectBtn.setToolTipText("Disconnect Proxy");
      mDisconnectBtn.setActionCommand("disconnect");
      mDisconnectBtn.setFocusPainted(false);

      mCreateBtn.addActionListener(mProxyTree);
      mRemoveBtn.addActionListener(mProxyTree);
      mAttachBtn.addActionListener(mProxyTree);
      mDisconnectBtn.addActionListener(mProxyTree);
      
      mToolbar.add(mCreateBtn, null);
      mToolbar.add(mRemoveBtn, null);
      mToolbar.add(mAttachBtn, null);
      mToolbar.add(mDisconnectBtn, null);
       
      ImageIcon wand = new ImageIcon("/home/users/aronb/JavaTesting/Intersense/wand1.jpg");
      this.setLayout(new BorderLayout());
      mPortLbl.setText("Port:");
      mDriverLbl.setText("Driver:");
      mStationsTable.setBackground(UIManager.getColor("Menu"));
      mInfoPanelLayout.setColumns(2);
      mInfoPanelLayout.setRows(3);


      mTopSectionPanel.setLayout(new BoxLayout(mTopSectionPanel, BoxLayout.X_AXIS));
      mTopSectionPanel.add(mISenseIconLbl);
      mTopSectionPanel.add(Box.createHorizontalGlue());
      mTopSectionPanel.add(mDeviceInfoPanel);

      try
      {
         ClassLoader loader = getClass().getClassLoader();

         String class_name = "org.vrjuggler.vrjconfig.commoneditors.positionaldeviceeditor.PositionalDeviceEditor";
         final Class jogl_editor_class = loader.loadClass(class_name);

         mTopSectionPanel.add(mVisualizeBtn);

         mVisualizeBtn.addActionListener(new ActionListener()
            {
               public void actionPerformed(ActionEvent evt)
               {
                  try
                  {
                     CustomEditor pos_editor = (CustomEditor)jogl_editor_class.newInstance();
                     JDialog dlg = new JDialog(
                        (Frame)SwingUtilities.getAncestorOfClass(Frame.class, IntersensePanel.this),
                        "3D Visualization", true);

                     pos_editor.setConfig(mConfigContext, mConfigElement);

                     dlg.getContentPane().add((JPanel)pos_editor);
                     dlg.pack();
                     dlg.setVisible(true);
                     //frame.setSize(750, 750);
                     //frame.show();
                  }
                  catch(InstantiationException e)
                  {
                     System.out.println(e);
                     e.printStackTrace();
                  }
                  catch(IllegalAccessException e)
                  {
                     System.out.println(e);
                     e.printStackTrace();
                  }
               }
            });
      }
      catch(ClassNotFoundException e)
      {
         System.out.println("*** Could not find the PositionalDeviceEditor, JOGL must not be availible. ***");
      }
      
      this.add(mTopSectionPanel, BorderLayout.NORTH);

      Border raisedbevel = BorderFactory.createEtchedBorder(EtchedBorder.LOWERED);
      TitledBorder title = BorderFactory.createTitledBorder(raisedbevel, "Intersense Stations");
      title.setTitlePosition(TitledBorder.TOP);
      mStationsPanel.setBorder(title);

      mStationsPanel.setLayout(new BorderLayout());
      mStationsPanel.add(mStationsTable, BorderLayout.NORTH);
      Box dumb = Box.createHorizontalBox();
      Box ver = Box.createVerticalBox();
      //dumb.setLayout(new FlowLayout());
      this.add(dumb, BorderLayout.CENTER);
      ver.add(mStationsPanel);
      ver.add(Box.createVerticalGlue());
      dumb.add(ver, BorderLayout.WEST);
      mProxyPanel.setLayout(new BorderLayout());
      mProxyPanel.add(mProxyTreeScrollPane, BorderLayout.CENTER);
      mProxyPanel.add(mToolbar, BorderLayout.NORTH);

      Border lowered = BorderFactory.createEtchedBorder(EtchedBorder.LOWERED);
      TitledBorder proxy_title = BorderFactory.createTitledBorder(lowered, "Station Proxies");
      proxy_title.setTitlePosition(TitledBorder.TOP);
      mProxyPanel.setBorder(proxy_title);
      
      dumb.add(mProxyPanel, BorderLayout.EAST);
      mProxyTreeScrollPane.getViewport().add(mProxyTree, null);
      mProxyTreeScrollPane.setBorder(BorderFactory.createLoweredBevelBorder());
   }

   public void init()
   {
      // Get handle to broker
      mBroker = new ConfigBrokerProxy();

      ConfigDefinition cfg_def = mConfigElement.getDefinition();
      
      mPortEditor = new PropertyEditorPanel(mConfigContext, mConfigElement.getProperty("port", 0),
                                            cfg_def.getPropertyDefinition("port"),
                                            mConfigElement, 0, getBackground());
      mDriverEditor = new PropertyEditorPanel(mConfigContext, mConfigElement.getProperty("driver", 0),
                                            cfg_def.getPropertyDefinition("driver"),
                                            mConfigElement, 0, getBackground());
      
      mDeviceInfoPanel.setLayout(mInfoPanelLayout);
      mDeviceInfoPanel.add(mDriverLbl, null);
      mDeviceInfoPanel.add(mDriverEditor, null);
      mDeviceInfoPanel.add(mPortLbl, null);
      mDeviceInfoPanel.add(mPortEditor, null);
   }

   /** Reference to the ConfigBroker used in this object. */
   private ConfigBroker mBroker = null;

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

   /**
    * Return a list of all ConfigElements that have the given ConfigDefinition and derived definitions.
    */
   protected java.util.List getElementsWithDefinitionAndDerived(String token)
   {
      java.util.List result = new ArrayList();
      java.util.List elts = getBroker().getElements(mConfigContext);

      // First get all for this type.
      result.addAll(ConfigUtilities.getElementsWithDefinition(elts, token));
      // Then get for all derived types.
      ConfigDefinitionRepository repos = getBroker().getRepository();
      java.util.List sub_defs = repos.getSubDefinitions(token);
      for(Iterator itr = sub_defs.iterator() ; itr.hasNext() ; )
      {
         java.util.List elements = ConfigUtilities.getElementsWithDefinition(elts, (String)itr.next());
         result.addAll(elements);
      }
      return result;
   }
}
