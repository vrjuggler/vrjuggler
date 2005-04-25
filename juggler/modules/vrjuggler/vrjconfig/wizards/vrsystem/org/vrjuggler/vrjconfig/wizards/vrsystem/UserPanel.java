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

import java.awt.BorderLayout;
import java.awt.Color;
import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.PropertyEditorPanel;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import info.clearthought.layout.*;

public class UserPanel extends JPanel implements EditorConstants
{
   private JPanel mBottomPanel = new JPanel();
   private TableLayout mBottomPanelLayout = null;
   private JLabel mDeviceIcon = new JLabel();
   private JPanel mDirectionsPanel = new JPanel();
   private JLabel mTitleLbl = new JLabel();
   private JLabel mDirectionsLbl = new JLabel();
   private JPanel mTopPanel = new JPanel();
   private ConfigContext mConfigContext = null;
   private Map mWhiteBoard = null;

   private ConfigElement mUserElement = null;
   private ConfigElement mWandElement = null;
   private ConfigElement mHeadElement = null;

   private PropertyEditorPanel mDistanceEditor = null;
   private PropertyEditorPanel mHeadEditor = null;
   private PropertyEditorPanel mWandEditor = null;
   
   private JLabel mDistanceLabel = new JLabel();
   private JLabel mHeadLabel = new JLabel();
   private JLabel mWandLabel = new JLabel();

   public UserPanel()
   {
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
   
   private void jbInit() throws Exception
   {
      mTitleLbl.setFont(new java.awt.Font("Serif", 1, 20));
      mTitleLbl.setHorizontalAlignment(SwingConstants.LEFT);
      mTitleLbl.setText("User Settings");
      mDirectionsLbl.setText("In order for VR Juggler to know how to "
            + "display graphics from the users point of view, you must "
            + "set the following values.");
      
      mDirectionsPanel.add(mTitleLbl, null);
      mDirectionsPanel.add(mDirectionsLbl, null);

      mTopPanel.setLayout(new BorderLayout());
      mTopPanel.setBorder(BorderFactory.createEtchedBorder());
      mTopPanel.add(mDirectionsPanel, BorderLayout.CENTER);
      mTopPanel.add(mDeviceIcon, BorderLayout.EAST);
      
      mHeadLabel.setText("Head Proxy");
      mWandLabel.setText("Wand Proxy");

      double[][] bottom_size = {{0.2, 0.3, 0.3, 0.2},
                                {TableLayout.FILL, TableLayout.PREFERRED,
                                 TableLayout.PREFERRED, TableLayout.PREFERRED,
                                 TableLayout.FILL}};
      mBottomPanelLayout = new TableLayout(bottom_size);
      mBottomPanel.setLayout(mBottomPanelLayout);
      
      setLayout(new BorderLayout());
      add(mTopPanel, BorderLayout.NORTH);
      add(mBottomPanel, BorderLayout.CENTER);
   }

   private ConfigElement getElementNamed(String name)
   {
      List elms = getBroker().getElements(mConfigContext);
      List matches = ConfigUtilities.getElementsWithName(elms, name);
      if (matches.size() > 0)
      {
         return (ConfigElement)matches.get(0);
      }
      else
      {
         return null;
      }
   }
   
   public void init(Map whiteboard)
   {
      mWhiteBoard = whiteboard;
      
      if (null == mWhiteBoard)
      {
         System.out.println("WhiteBoard null.");
      }
      mConfigContext = (ConfigContext)mWhiteBoard.get("context");

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }
      
      ConfigDefinition alias_def = getBroker().getRepository().get(ALIAS_TYPE);
      ConfigDefinition user_def = getBroker().getRepository().get(USER_TYPE);
      
      // Create a temporary list of ConfigDefinitions to pass to factory.
      java.util.List def_list = new ArrayList();
      def_list.add(alias_def);
      def_list.add(user_def);
     
      // Initialize a ConfigElementFactory with the needed 
      // ConfigDefinition. And create a new ConfigElement.
      ConfigElementFactory temp_factory = new ConfigElementFactory(def_list);
      if (!mConfigContext.containsElementNamed("VJUser"))
      {
         mUserElement = temp_factory.create("VJUser", user_def);
         getBroker().add(mConfigContext, mUserElement);
      }
      else if(null == mUserElement)
      {
         mUserElement = getElementNamed("VJUser");
      }
      
      if (mConfigContext.containsElementNamed("VJHead"))
      {
         mHeadElement = getElementNamed("VJHead");
      }
      else
      {
         mHeadElement = temp_factory.create("VJHead", alias_def);
         getBroker().add(mConfigContext, mHeadElement);
      }
      
      if (mConfigContext.containsElementNamed("VJWand"))
      {
         mWandElement = getElementNamed("VJWand");
      }
      else
      {
         mWandElement = temp_factory.create("VJWand", alias_def);
         getBroker().add(mConfigContext, mWandElement);
      }

      // Setting up the interoccular distance editor.
      Object distance_value = mUserElement.getProperty(INTEROCULAR_DISTANCE_PROPERTY, 0);
      PropertyDefinition distance_prop_def =
         user_def.getPropertyDefinition(INTEROCULAR_DISTANCE_PROPERTY);
      mDistanceEditor = new PropertyEditorPanel(mConfigContext, distance_value, distance_prop_def, mUserElement,
                                              0, Color.white);
      String label = distance_prop_def.getPropertyValueDefinition(0).getLabel();
      mDistanceLabel.setText(label);
      
      // Set up the VJHead alias editor.
      Object head_value = mHeadElement.getProperty(PROXY_PROPERTY, 0);
      PropertyDefinition head_prop_def = alias_def.getPropertyDefinition(PROXY_PROPERTY);
      mHeadEditor = new PropertyEditorPanel(mConfigContext, head_value, head_prop_def, mHeadElement,
                                              0, Color.white);
      
      // Set up the VJWand alias editor.
      Object wand_value = mWandElement.getProperty(PROXY_PROPERTY, 0);
      PropertyDefinition wand_prop_def = alias_def.getPropertyDefinition(PROXY_PROPERTY);
      mWandEditor = new PropertyEditorPanel(mConfigContext, wand_value, wand_prop_def, mWandElement,
                                              0, Color.white);
      
      mBottomPanel.add(mDistanceLabel,
                             new TableLayoutConstraints(1, 1, 1, 1,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mBottomPanel.add(mDistanceEditor,
                             new TableLayoutConstraints(2, 1, 2, 1,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mBottomPanel.add(mHeadLabel,
                             new TableLayoutConstraints(1, 2, 1, 2,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mBottomPanel.add(mHeadEditor,
                             new TableLayoutConstraints(2, 2, 2, 2,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mBottomPanel.add(mWandLabel,
                             new TableLayoutConstraints(1, 3, 1, 3,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
      mBottomPanel.add(mWandEditor,
                             new TableLayoutConstraints(2, 3, 2, 3,
                                                        TableLayout.FULL,
                                                        TableLayout.FULL));
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
}
