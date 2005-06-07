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
package org.vrjuggler.vrjconfig.customeditors.intersense;

import javax.swing.tree.*;
import java.util.*;
import org.vrjuggler.jccl.config.*;

/**
 * Model showing how proxies point at this Intersense station.
 */
public class ProxyTreeModel extends DefaultTreeModel 
   implements IntersenseModelListener
{
   /** Category node for position proxies. */
   private DefaultMutableTreeNode mPositionNode         = null;
   
   /** Category node for digital proxies. */
   private DefaultMutableTreeNode mDigitalNode          = null;
   
   /** Category node for analog proxies. */
   private DefaultMutableTreeNode mAnalogNode           = null;

   /** List of all position proxies. */
   private List                 mPositionProxies        = new ArrayList();
   
   /** List of all digital proxies. */
   private List                 mDigitalProxies         = new ArrayList();
   
   /** List of all analog proxies. */
   private List                 mAnalogProxies          = new ArrayList();

   /** Intersense Station model that is used for the same station as us.*/
   private StationModel         mStationModel           = null;

   /** ConfigElement for the Intersense device. */
   private ConfigElement        mISenseElement          = null;

   /** ConfigContext that all active ConfigElements reside in. */
   private ConfigContext        mContext                = null;
      
   /** Nodes that represent the digital units. */
   private List                 mDigitalButtonNodes     = null;
   
   /** Nodes that represent the analog units. */
   private List                 mAnalogButtonNodes      = null;
      

   /**
    * Constructor used for testing.
    */
   public ProxyTreeModel(TreeNode root)
   {
      super(root);
   }

   /**
    * Constructor that creates a tree representation of an Intersense station 
    * and all proxies pointing at it.
    */
   public ProxyTreeModel(StationModel model, ConfigElement isense_elm, ConfigContext context)
   {
      super(null);

      // Store references for later use.
      mContext = context;
      mStationModel = model;
      mISenseElement = isense_elm;
      rebuildTree();
   }

   public void rebuildTree()
   {  
      mAnalogButtonNodes      = new ArrayList();
      mDigitalButtonNodes     = new ArrayList();

      System.out.println("Building Tree");
      
      // Find all proxies pointing at this station.
      mPositionProxies = mStationModel.findPositionProxies();
      mDigitalProxies = mStationModel.findDigitalProxies();
      mAnalogProxies = mStationModel.findAnalogProxies();
      
      // Create root node with the name of the station.
      DefaultMutableTreeNode root_node = new DefaultMutableTreeNode(mStationModel.getName());
      
      // Create category nodes for position, digital, and analog.
      mPositionNode = new DefaultMutableTreeNode(new DeviceUnit("Position Proxies",
                                                                mISenseElement.getName(),
                                                                "position_proxy",
                                                                mStationModel.getStationIndex()));
      mDigitalNode = new DefaultMutableTreeNode(new ProxyType("Digital Proxies", "digital_proxy"));
      mAnalogNode = new DefaultMutableTreeNode(new ProxyType("Analog Proxies", "analog_proxy"));

      // XXX: Hard coding for 4 buttons right now.
      // For each button add a node.
      int digital_first = mStationModel.getDigitalFirst();
      for(int i = 0 ; i < 4 ; ++i)
      {
         DefaultMutableTreeNode button_node = new DefaultMutableTreeNode(
               new DeviceUnit("Button " + Integer.toString(i), 
                              mISenseElement.getName(),
                              "digital_proxy",
                              digital_first + i));
         mDigitalNode.add(button_node);
         mDigitalButtonNodes.add(button_node);
      }

      // Add in the joysitck and trigger buttons with special names.
      DefaultMutableTreeNode joystick_node = new DefaultMutableTreeNode(
            new DeviceUnit("Joystick", mISenseElement.getName(), "digital_proxy",
                           digital_first + 4));
      mDigitalNode.add(joystick_node);
      mDigitalButtonNodes.add(joystick_node);
         
      DefaultMutableTreeNode trigger_node = new DefaultMutableTreeNode(
            new DeviceUnit("Trigger", mISenseElement.getName(), "digital_proxy",
                           digital_first + 5));
      mDigitalNode.add(trigger_node);
      mDigitalButtonNodes.add(trigger_node);


      // XXX: Hard coding for 2 analogs right now.
      // For each button add a node.
      int analog_first = mStationModel.getAnalogFirst();
      for(int i = 0 ; i < 2 ; ++i)
      {
         DefaultMutableTreeNode ana_node = new DefaultMutableTreeNode(
               new DeviceUnit("Analog " + Integer.toString(i),
                              mISenseElement.getName(),
                              "analog_proxy",
                              analog_first + i));
         mAnalogNode.add(ana_node);
         mAnalogButtonNodes.add(ana_node);
      }
     
      // Add the position, digital, and analog category nodes to root node.
      root_node.add(mPositionNode);
      root_node.add(mDigitalNode);
      root_node.add(mAnalogNode);
      
      // Add all position proxies to the position parent.
      for(Iterator itr = mPositionProxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement pos_elm = (ConfigElement)itr.next();
         DefaultMutableTreeNode pos_node = new DefaultMutableTreeNode(pos_elm);
         mPositionNode.add(pos_node);
      }
     
      // For each digital proxy add it to the proper Button's parent in the tree.
      for(Iterator itr = mDigitalProxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement dig_elm = (ConfigElement)itr.next();
         DefaultMutableTreeNode dig_node = new DefaultMutableTreeNode(dig_elm);
         
         int unit = ((Integer)dig_elm.getProperty("unit", 0)).intValue();
         unit = unit - mStationModel.getDigitalFirst();
         
         if(unit >= 0 && unit < mDigitalButtonNodes.size())
         {
            ((DefaultMutableTreeNode)mDigitalButtonNodes.get(unit)).add(dig_node);
         }
      }
      
      // For each analog proxy add it to the proper Button's parent in the tree.
      for(Iterator itr = mAnalogProxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement ana_elm = (ConfigElement)itr.next();
         DefaultMutableTreeNode ana_node = new DefaultMutableTreeNode(ana_elm);
         
         int unit = ((Integer)ana_elm.getProperty("unit", 0)).intValue();
         unit = unit - mStationModel.getAnalogFirst();
         if(unit >= 0 && unit < mAnalogButtonNodes.size())
         {
            ((DefaultMutableTreeNode)mAnalogButtonNodes.get(unit)).add(ana_node);
         }
      }
     
      // Set the root of the tree.
      setRoot(root_node);
      setAsksAllowsChildren(false);
   }
   
   /**
    * Return the ConfigElement for this Intersense device.
    */
   public ConfigElement getISenseElement()
   {
      return mISenseElement;
   }

   /**
    * Return the ConfigContext associated with this station.
    */
   public ConfigContext getConfigContext()
   {
      return mContext;
   }

   /**
    * Gets a list of all the tree nodes that contain the given Object.
    */
   protected List getNodesFor(Object obj)
   {
      return getNodesFor(obj, (DefaultMutableTreeNode)getRoot());
   }

   /**
    * Recursive helper for getNodesFor(Object).
    *
    * @see #getNodesFor(Object)
    */
   private List getNodesFor(Object obj, DefaultMutableTreeNode node)
   {
      List results = new ArrayList();

      // Check if we found a match
      if (node.getUserObject().equals(obj))
      {
         results.add(node);
      }

      // Check all children of the current node
      for (Enumeration e = node.children(); e.hasMoreElements(); )
      {
         DefaultMutableTreeNode child = (DefaultMutableTreeNode)e.nextElement();
         results.addAll(getNodesFor(obj, child));
      }

      return results;
   }

   /**
    * IntersenseModelListener method that gets called whenever a new proxy 
    * for this Intersense is created.
    */
   public void proxyAdded(IntersenseModelEvent evt)
   {
      ConfigElement elm = evt.getElement();
      
      int unit = ((Integer)elm.getProperty("unit", 0)).intValue();
      
      DefaultMutableTreeNode parent = null;
        
      
      // Get the appropriate parent.
      if(evt.getToken().equals("digital_proxy"))
      {
         unit = unit - mStationModel.getDigitalFirst();
         if(unit >= 0 && unit < mDigitalButtonNodes.size())
         {
            parent = (DefaultMutableTreeNode)mDigitalNode.getChildAt(unit);
         }
      }
      else if(evt.getToken().equals("analog_proxy"))
      {
         unit = unit - mStationModel.getAnalogFirst();
         if(unit >= 0 && unit < mAnalogButtonNodes.size())
         {
            parent = (DefaultMutableTreeNode)mAnalogNode.getChildAt(unit);
         }
      }
      else if(evt.getToken().equals("position_proxy"))
      {
         if(unit == mStationModel.getStationIndex())
         {
            parent = (DefaultMutableTreeNode)mPositionNode;
         }
      }

      // If we don't have the parent of this new proxy return.
      if(null == parent)
      {
         return;
      }
        
      // If we already have this child skip it.
      for(Enumeration itr = parent.children() ; itr.hasMoreElements() ; )
      {
         DefaultMutableTreeNode child = (DefaultMutableTreeNode)itr.nextElement();
         if(child.getUserObject().equals(elm))
         {
            System.out.println("WARNING: We already have this element");
            return;
         }
      }
      
      // Create a new child for this proxy and add it to the correct parent.
      DefaultMutableTreeNode new_child = new DefaultMutableTreeNode(elm);
      parent.add(new_child);

      // Inform the JTree that the structure has changed.
      reload(parent);
   }

   /**
    * IntersenseModelListener method that gets called whenever a new proxy 
    * for this Intersense is removed.
    */
   public void proxyRemoved(IntersenseModelEvent evt)
   {
      ConfigElement elm = evt.getElement();
      DefaultMutableTreeNode parent = null;
         
      // Get the appropriate parent.
      if(evt.getToken().equals("digital_proxy"))
      {
         parent = (DefaultMutableTreeNode)mDigitalNode;
      }
      else if(evt.getToken().equals("analog_proxy"))
      {
         parent = (DefaultMutableTreeNode)mAnalogNode;
      }
      else if(evt.getToken().equals("position_proxy"))
      {
         parent = (DefaultMutableTreeNode)mPositionNode;
      }
      
      // Find the node for the given element and remove it.
      for(Enumeration itr = parent.depthFirstEnumeration() ; itr.hasMoreElements() ; )
      {
         DefaultMutableTreeNode child = (DefaultMutableTreeNode)itr.nextElement();
         if(child.getUserObject().equals(elm))
         {
            child.removeFromParent();
            
            // Inform the JTree that the structure has changed.
            reload(parent);
            return;
         }
      }
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
    * Used to represent different categories of units on each Intersense station.
    */
   protected class ProxyType
   {
      /** ConfigDefinition token for this proxy type. */
      private String mToken = null;

      /** Name of this proxy type. */
      private String mName = null;
      
      /**
       * Constructor.
       */
      public ProxyType(String name, String token)
      {
         mName = name;
         mToken = token;
      }

      /**
       * Return a String representation of this ProxyType.
       */
      public String toString()
      {
         return mName;
      }

      /**
       * Return the name of this proxy type.
       */
      public String getName()
      {
         return mName;
      }
      
      /**
       * Return the ConfigDefinition token used for this proxy type.
       */
      public String getToken()
      {
         return mToken;
      }
   }
  
   /**
    * Used to represent a position/digital/analog unit on an Intersense station.
    */
   protected class DeviceUnit
   {
      /** Name of the device unit. */
      private String mName = null;

      /** Name of the Intersense device. */
      private String mDeviceName = null;

      /** ConfigDefinition token of proxies that can point at this unit. */
      private String mToken = null;

      /** Unit number for this device. */
      private int mUnitNum = 0;
     
      /**
       * Constructor.
       */
      public DeviceUnit(String name, String device_name, String token, int unit_num)
      {
         mName = name;
         mDeviceName = device_name;
         mToken = token;
         mUnitNum = unit_num;
      }

      /**
       * Return the name of this DeviceUnit.
       */
      public String getName()
      {
         return mName;
      }

      /**
       * Return a String representation of this ProxyType.
       */
      public String toString()
      {
         return mName;
      }

      /**
       * Get the name of the Intersense device that this DeviceUnit is on.
       */
      public String getDeviceName()
      {
         return mDeviceName;
      }

      /**
       * Return the ConfigDefinition token of proxies that can point at this unit. 
       */
      public String getToken()
      {
         return mToken;
      }
     
      /**
       * Return the unit number for this device.
       */
      public int getDeviceNum()
      {
         return mUnitNum;
      }
   }
}
