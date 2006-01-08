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
package org.vrjuggler.vrjconfig.wizards.newdevice;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.event.*;
import java.awt.Frame;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TreeSet;
import javax.swing.*;
import javax.swing.tree.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;
import org.vrjuggler.tweek.wizard.*;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import org.vrjuggler.vrjconfig.commoneditors.SimpleEditorDialog;
import org.vrjuggler.vrjconfig.commoneditors.CustomEditorDialog;

public class SelectDefinitionPanel extends JPanel implements EditorConstants
{
   private JPanel mDirectionsPanel = new JPanel();
   private JLabel mTitleLbl = new JLabel();
   private JLabel mDirectionsLbl = new JLabel();
   private JPanel mTopPanel = new JPanel();
   private JLabel mDeviceIcon = new JLabel();
   private JList mDefinitionList = new JList();
   private JScrollPane mDefinitionListScrollPane = new JScrollPane();
   private Map mWhiteBoard;
   private ConfigContext mConfigContext = null;
   private ConfigElement mConfigElement = null;

   public SelectDefinitionPanel()
   {
      try
      {
         createListModel();
         jbInit();
         
         DefinitionListRenderer renderer2 = new DefinitionListRenderer();
         mDefinitionList.setCellRenderer(renderer2);
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      try
      {
         ClassLoader loader = getClass().getClassLoader();
         mDeviceIcon.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/devices64.png")));
      }
      catch(NullPointerException exp)
      {
         mDeviceIcon.setText("");
      }
   }

   public ConfigDefinition getSelectedDefinition()
   {
      return (ConfigDefinition)mDefinitionList.getSelectedValue();
   }

   public boolean onExiting()
   {
      if (null == mDefinitionList.getSelectedValue())
      {
         return false;
      }

      mConfigContext = (ConfigContext)mWhiteBoard.get("context");
      
      ConfigDefinition def = getSelectedDefinition();
      String token = getSelectedDefinition().getToken();

      if (null == mConfigElement)
      {
         // Create a temporary list of ConfigDefinitions to pass to factory.
         java.util.List def_list = new ArrayList();
         def_list.add(def);
        
         // Initialize a ConfigElementFactory with the needed 
         // ConfigDefinition. And create a new ConfigElement.
         ConfigElementFactory temp_factory = new ConfigElementFactory(def_list);
         mConfigElement = temp_factory.create("New " + token, def);
      }

      List list = CustomEditorRegistry.findEditors(token);
     
      Color start_color = new Color(160, 160, 180);

      Object color = UIManager.get( "window" );
      if(null != color && color instanceof Color)
      {
         start_color = (Color)color;
      }
      else
      {
         System.out.println("Could not get the desktop color from the  UIManager.");
      }

      if(null == list || list.size() == 0)
      {
         System.out.println("No CustomEditors registered for token: " + token);
         
         JScrollPane scroll_pane = new JScrollPane();
         PropertySheet element_prop_sheet =
            PropertySheetFactory.instance().makeSheet(mConfigContext, mConfigElement, start_color);
         
         scroll_pane.getViewport().removeAll();
         scroll_pane.getViewport().add(element_prop_sheet, null);
         
         Frame parent = 
            (Frame) SwingUtilities.getAncestorOfClass(Frame.class,
                                                      SelectDefinitionPanel.this);
         SimpleEditorDialog dlg = new SimpleEditorDialog(parent,
                                                               scroll_pane,
                                                               mConfigElement.getDefinition().getName() + " Editor");
         int status = dlg.showDialog();

         if ( status == SimpleEditorDialog.CANCEL_OPTION )
         {
            mConfigElement = null;
            return false;
         }
      }
      else
      {
         CustomEditor editor = (CustomEditor)list.get(0);
         editor.setConfig(mConfigContext, mConfigElement);
         
         Frame parent = 
            (Frame) SwingUtilities.getAncestorOfClass(Frame.class,
                                                      SelectDefinitionPanel.this);
         CustomEditorDialog dlg = new CustomEditorDialog(parent,
                                                         editor);
         int status = dlg.showDialog();

         if ( status == CustomEditorDialog.CANCEL_OPTION )
         {
            mConfigElement = null;
            return false;
         }
      }
      
      if (!mConfigContext.containsElement(mConfigElement))
      {
         ConfigBroker broker = new ConfigBrokerProxy();
         broker.add(mConfigContext, mConfigElement);
      }

      return true;
   }

   public void createListModel()
   {
      DefaultListModel list_model = new DefaultListModel();

      ConfigDefinitionRepository repos = getBroker().getRepository();

      TreeSet all_defs = new TreeSet();

      List pos_defs = repos.getSubDefinitions("positional_device");
      List dig_defs = repos.getSubDefinitions("digital_device");
      List ana_defs = repos.getSubDefinitions("analog_device");

      all_defs.addAll(pos_defs);
      all_defs.addAll(dig_defs);
      all_defs.addAll(ana_defs);

      for(Iterator itr = all_defs.iterator() ; itr.hasNext() ; )
      {
         ConfigDefinition def = repos.get((String)itr.next());
         list_model.addElement(def);
      }

      mDefinitionList.setModel(list_model);
   }

   private void jbInit() throws Exception
   {
      mTitleLbl.setFont(new java.awt.Font("Serif", 1, 20));
      mTitleLbl.setHorizontalAlignment(SwingConstants.LEFT);
      mTitleLbl.setText("Add New Input Device Wizard");
      mDirectionsLbl.setText("Select the type of input device you want to add.");

      mDirectionsPanel.add(mTitleLbl, null);
      mDirectionsPanel.add(mDirectionsLbl, null);

      mTopPanel.setLayout(new BorderLayout());
      mTopPanel.setBorder(BorderFactory.createEtchedBorder());
      mTopPanel.add(mDirectionsPanel, BorderLayout.CENTER);
      mTopPanel.add(mDeviceIcon, BorderLayout.EAST);

      mDefinitionListScrollPane.getViewport().add(mDefinitionList, null);
      mDefinitionListScrollPane.setBorder(BorderFactory.createLoweredBevelBorder());
      mDefinitionList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

      setLayout(new BorderLayout());
      add(mTopPanel, BorderLayout.NORTH);
      add(mDefinitionListScrollPane, BorderLayout.CENTER);
   }

   public void init(Map whiteboard)
   {
      mWhiteBoard = whiteboard;
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
    * Custom ListCellRenderer for proxies.
    */
   public class DefinitionListRenderer extends DefaultListCellRenderer
   {
      /** Icon used for position nodes. */
      private Icon mPositionalIcon = null;

      /** Icon used for position nodes. */
      private Icon mDigitalIcon = null;

      /** Icon used for position nodes. */
      private Icon mAnalogIcon = null;

      /**
       * Constructor.
       */
      public DefinitionListRenderer()
      {
         ClassLoader loader = getClass().getClassLoader();

         // Load the icons.
         mPositionalIcon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/wizard/newdevice/images/position16.png"));
         mDigitalIcon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/wizard/newdevice/images/digital16.png"));
         mAnalogIcon = new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/wizard/newdevice/images/analog16.png"));
      }

      /**
       * Creates new TreeNodeRenderer for given value.
       *
       * @param list     JList which is asking for renderer.
       * @param value    Value of node being renderer.
       * @param selected Is the value selected.
       * @param hasFocus Does the value have focus.
       */
      public Component getListCellRendererComponent(JList list, Object value,
                                                    int index, boolean selected,
                                                    boolean hasFocus)
      {
         //DefaultMutableTreeNode node = (DefaultMutableTreeNode)value;

         // NOTE: DefaultTreeCellRenderer extends JLabel and returns 
         // itself, if we call the following method to set everything up
         // we can then set the Icon to whatever we want and return this.
         // Get the default renderer for the passed value because we know that the UserObject is
         // either a ProxyType or a DeviceUnit which both implement the toString method.
         super.getListCellRendererComponent(list, value, index, selected, hasFocus);

         ConfigDefinition config_def = (ConfigDefinition)value;

         // Set the correct Icon for this node in the tree.
         if (config_def.isOfType(POSITIONAL_DEVICE_TYPE))
         {
            setIcon(mPositionalIcon);
         }
         else if (config_def.isOfType(ANALOG_DEVICE_TYPE))
         {
            setIcon(mAnalogIcon);
         }
         else if (config_def.isOfType(DIGITAL_DEVICE_TYPE))
         {
            setIcon(mDigitalIcon);
         }

         setText(config_def.getName());

         return this;
      }
   }
}
