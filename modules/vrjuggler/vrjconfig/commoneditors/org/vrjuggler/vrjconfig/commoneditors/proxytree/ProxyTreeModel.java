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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors.proxytree;

import javax.swing.tree.*;
import java.util.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;

/**
 * Model showing how proxies point at a device.
 *
 * @see ConfigListener
 * @see ConfigElementListener
 */
public class ProxyTreeModel
   extends DefaultTreeModel
   implements ConfigListener
            , ConfigElementListener
{
   /** Category node for position proxies. */
   private DefaultMutableTreeNode mPositionTypeNode         = null;
   
   /** Category node for digital proxies. */
   private DefaultMutableTreeNode mDigitalTypeNode          = null;
   
   /** Category node for analog proxies. */
   private DefaultMutableTreeNode mAnalogTypeNode           = null;

   /** List of all position proxies. */
   private List                 mPositionProxies        = new ArrayList();
   
   /** List of all digital proxies. */
   private List                 mDigitalProxies         = new ArrayList();
   
   /** List of all analog proxies. */
   private List                 mAnalogProxies          = new ArrayList();

   /** ConfigElement for the device. */
   private ConfigElement        mConfigElement          = null;

   /** ConfigContext that all active ConfigElements reside in. */
   private ConfigContext        mContext                = null;
      
   /** Nodes that represent the digital units. */
   private List                 mPositionalButtonNodes     = null;
   
   /** Nodes that represent the digital units. */
   private List                 mDigitalButtonNodes     = null;
   
   /** Nodes that represent the analog units. */
   private List                 mAnalogButtonNodes      = null;

   private int                  mNumPositionalUnits     = 0;
   private int                  mNumDigitalUnits        = 0;
   private int                  mNumAnalogUnits         = 0;
      
   /**
    * Constructor used for testing.
    */
   public ProxyTreeModel(TreeNode root)
   {
      super(root);
   }

   /**
    * Constructor that creates a tree representation of a device 
    * and all proxies pointing at it.
    */
   public ProxyTreeModel(ConfigElement elm, ConfigContext context,
         int num_positional, int num_digital, int num_analog)
   {
      super(null);
      
      // Store references for later use.
      mContext = context;
      mConfigElement = elm;
      mNumPositionalUnits = num_positional;
      mNumDigitalUnits = num_digital;
      mNumAnalogUnits = num_analog;
      
      // Add a listener to update this model if more config elements are added to context.
      getBroker().addConfigListener(this);
      elm.addConfigElementListener(this);

      // We must listen to all proxy elements to ensure that we are informed if
      // they start pointing at this device.
      List all_elms = getBroker().getElementsIncludingEmbedded(mContext);
      for (Iterator itr = all_elms.iterator(); itr.hasNext(); )
      {
         ConfigElement elt = (ConfigElement)itr.next();
         String token = elt.getDefinition().getToken();
         if(token.equals("digital_proxy") ||
            token.equals("analog_proxy") ||
            token.equals("position_proxy"))
         {
            elt.addConfigElementListener(this);
         }
      }
      
      rebuildTree();
   }

   public void rebuildTree()
   {
      mPositionalButtonNodes  = new ArrayList();
      mAnalogButtonNodes      = new ArrayList();
      mDigitalButtonNodes     = new ArrayList();

      System.out.println("Building Tree");
      
      // Find all proxies pointing at this station.
      if ( mNumPositionalUnits > 0)
      {
         mPositionProxies = findPositionProxies();
      }
      if ( mNumDigitalUnits > 0)
      {
         mDigitalProxies = findDigitalProxies();
      }
      if ( mNumAnalogUnits > 0)
      {
         mAnalogProxies = findAnalogProxies();
      }
      
      // Create root node with the name of the station.
      DefaultMutableTreeNode root_node = new DefaultMutableTreeNode(mConfigElement.getName());
      
      // Create category nodes for position, digital, and analog.
      mPositionTypeNode = new DefaultMutableTreeNode(new ProxyType("Position Proxies", "positional_proxy"));
      mDigitalTypeNode = new DefaultMutableTreeNode(new ProxyType("Digital Proxies", "digital_proxy"));
      mAnalogTypeNode = new DefaultMutableTreeNode(new ProxyType("Analog Proxies", "analog_proxy"));     
      
      // For each digital button add a node.
      for(int i = 0 ; i < mNumPositionalUnits ; ++i)
      {
         DefaultMutableTreeNode pos_node = new DefaultMutableTreeNode(
               new DeviceUnit("Bird " + Integer.toString(i), 
                              mConfigElement.getName(),
                              "positional_proxy", i));
         mPositionTypeNode.add(pos_node);
         mPositionalButtonNodes.add(pos_node);
      }
      
      // For each digital button add a node.
      for(int i = 0 ; i < mNumDigitalUnits ; ++i)
      {
         DefaultMutableTreeNode button_node = new DefaultMutableTreeNode(
               new DeviceUnit("Digital " + Integer.toString(i), 
                              mConfigElement.getName(),
                              "digital_proxy", i));
         mDigitalTypeNode.add(button_node);
         mDigitalButtonNodes.add(button_node);
      }
      
      // For each analog button add a node.
      for(int i = 0 ; i < mNumAnalogUnits ; ++i)
      {
         DefaultMutableTreeNode ana_node = new DefaultMutableTreeNode(
               new DeviceUnit("Analog " + Integer.toString(i),
                              mConfigElement.getName(),
                              "analog_proxy", i));
         mAnalogTypeNode.add(ana_node);
         mAnalogButtonNodes.add(ana_node);
      }
      
      // Add the position, digital, and analog category nodes to root node.
      if ( mNumPositionalUnits > 0)
      {
         root_node.add(mPositionTypeNode);
      }
      if ( mNumDigitalUnits > 0)
      {
         root_node.add(mDigitalTypeNode);
      }
      if ( mNumAnalogUnits > 0)
      {
         root_node.add(mAnalogTypeNode);
      }

      // Add all position proxies to the position parent.
      for(Iterator itr = mPositionProxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement pos_elm = (ConfigElement)itr.next();
         DefaultMutableTreeNode pos_node = new DefaultMutableTreeNode(pos_elm);
         //mPositionTypeNode.add(pos_node);
         
         // Get the unit number that this proxy is pointing at.
         int unit = ((Integer)pos_elm.getProperty("unit", 0)).intValue();
         
         // If the unit number is valid, add it to the tree.
         if(unit >= 0 && unit < mPositionalButtonNodes.size())
         {
            ((DefaultMutableTreeNode)mPositionalButtonNodes.get(unit)).add(pos_node);
         }
         else
         {
            // Should we throw an exception here?
            System.out.println("ERROR: Position proxy named \"" + pos_elm.getName() 
                  + "\" is assigned to an invalid unit number " + unit
                  + " valid values are in the rage [0," + mPositionalButtonNodes.size() + "]");
         }

      }
      
      // For each digital proxy add it to the proper Button's parent in the tree.
      for(Iterator itr = mDigitalProxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement dig_elm = (ConfigElement)itr.next();
         DefaultMutableTreeNode dig_node = new DefaultMutableTreeNode(dig_elm);
         
         // Get the unit number that this proxy is pointing at.
         int unit = ((Integer)dig_elm.getProperty("unit", 0)).intValue();
         
         // If the unit number is valid, add it to the tree.
         if(unit >= 0 && unit < mDigitalButtonNodes.size())
         {
            ((DefaultMutableTreeNode)mDigitalButtonNodes.get(unit)).add(dig_node);
         }
         else
         {
            // Should we throw an exception here?
            System.out.println("ERROR: Digital proxy named \"" + dig_elm.getName() 
                  + "\" is assigned to an invalid unit number " + unit
                  + " valid values are in the rage [0," + mDigitalButtonNodes.size() + "]");
         }
      }
      
      // For each analog proxy add it to the proper Button's parent in the tree.
      for(Iterator itr = mAnalogProxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement ana_elm = (ConfigElement)itr.next();
         DefaultMutableTreeNode ana_node = new DefaultMutableTreeNode(ana_elm);
         
         // Get the unit number that this proxy is pointing at.
         int unit = ((Integer)ana_elm.getProperty("unit", 0)).intValue();
         
         // If the unit number is valid, add it to the tree.
         if(unit >= 0 && unit < mAnalogButtonNodes.size())
         {
            ((DefaultMutableTreeNode)mAnalogButtonNodes.get(unit)).add(ana_node);
         }
         else
         {
            // Should we throw an exception here?
            System.out.println("ERROR: Analog proxy named \"" + ana_elm.getName() 
                  + "\" is assigned to an invalid unit number " + unit
                  + " valid values are in the rage [0," + mAnalogButtonNodes.size() + "]");
         }
      }
      
      // Set the root of the tree.
      setRoot(root_node);
      setAsksAllowsChildren(false);
   }
   
   /**
    * Return the ConfigElement for this device.
    */
   public ConfigElement getConfigElement()
   {
      return mConfigElement;
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
    * ConfigListener interface to catch all added ConfigElements.
    * 
    * @see ConfigListener
    */
   public void configElementAdded(ConfigEvent evt)
   {
      // - If the element is a proxy.
      //   - If new new element points at us we must inform all listeners.
      //   - We must start listening to see if it every starts pointing at us.
      ConfigElement elm = evt.getElement();
      String token = elm.getDefinition().getToken();
      if(token.equals("digital_proxy") ||
         token.equals("analog_proxy") ||
         token.equals("position_proxy"))
      {
         // We must start listening to this proxy now.
         elm.addConfigElementListener(this);
         
         String device_name = ((ConfigElementPointer)elm.getProperty("device", 0)).getTarget();
         String our_name = mConfigElement.getName();
         // If the proxy pointed at us inform all listeners.
         if(device_name.equals(our_name))
         {
            int unit = ((Integer)elm.getProperty("unit", 0)).intValue();
            proxyAdded(elm, token, unit);
         }
      }
   }
   
   /**
    * ConfigListener interface to catch removed added ConfigElements.
    * 
    * @see ConfigListener
    */
   public void configElementRemoved(ConfigEvent evt)
   {
      // - If element is a proxy
      //   - If it pointed at us inform all listeners.
      ConfigElement elm = evt.getElement();
      String token = elm.getDefinition().getToken();
      if(token.equals("digital_proxy") ||
         token.equals("analog_proxy") ||
         token.equals("position_proxy"))
      {
         // We should stop listening to this proxy now.
         elm.removeConfigElementListener(this);
         
         String device_name = ((ConfigElementPointer)elm.getProperty("device", 0)).getTarget();
         String our_name = mConfigElement.getName();
         // If the proxy pointed at us inform all listeners.
         if(device_name.equals(our_name))
         {
            int unit = ((Integer)elm.getProperty("unit", 0)).intValue();
            proxyRemoved(elm, token, unit);
         }
      }
   }

   /**
    * ConfigElementListener interface to catch all changes in existing proxies
    * by calling a helper method, elementChanged.
    *
    * @see ConfigElementListener
    */
   public void nameChanged(ConfigElementEvent evt)
   {
      elementChanged(evt);
   }
   public void propertyValueChanged(ConfigElementEvent evt)
   {
      elementChanged(evt);
   }
   public void propertyValueOrderChanged(ConfigElementEvent evt)
   {
      elementChanged(evt);
   }
   public void propertyValueAdded(ConfigElementEvent evt)
   {
      elementChanged(evt);
   }
   public void propertyValueRemoved(ConfigElementEvent evt)
   {
      elementChanged(evt);
   }
   
   /**
    * Helper function that is used to process all changes to existing proxies.
    */
   private void elementChanged(ConfigElementEvent evt)
   { 
      // - If element is a proxy
      //   - Remove it from the tree.
      //   - If device and unit match add it again.
      ConfigElement elm = (ConfigElement)evt.getSource();
      String token = elm.getDefinition().getToken();
      if(token.equals("position_proxy") ||
         token.equals("digital_proxy") ||
         token.equals("analog_proxy"))
      {
         String our_name = mConfigElement.getName();
         
         if(evt.getProperty() == null)
         {
            return;
         }
        
         String device_name = ((ConfigElementPointer)elm.getProperty("device", 0)).getTarget();
         int unit = ((Integer)elm.getProperty("unit", 0)).intValue();

         // Remove the node if it exists.
         proxyRemoved(elm, token, unit);

         // Add the node if it points at this device.
         if(device_name.equals(our_name))
         {
            proxyAdded(elm, token, unit);
         }
      }
   }


   /**
    * Called whenever a new proxy for this device is added.
    */
   public void proxyAdded(ConfigElement elm, String token, int unit)
   {
      DefaultMutableTreeNode parent = null;
      
      // Get the appropriate parent.
      if(token.equals("digital_proxy"))
      {
         if(unit >= 0 && unit < mDigitalButtonNodes.size())
         {
            parent = (DefaultMutableTreeNode)mDigitalTypeNode.getChildAt(unit);
         }
      }
      else if(token.equals("analog_proxy"))
      {
         if(unit >= 0 && unit < mAnalogButtonNodes.size())
         {
            parent = (DefaultMutableTreeNode)mAnalogTypeNode.getChildAt(unit);
         }
      }
      else if(token.equals("position_proxy"))
      {
         if(unit >= 0 && unit < mPositionalButtonNodes.size())
         {
            parent = (DefaultMutableTreeNode)mPositionTypeNode;
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
    * Called whenever a proxy for this device is removed.
    */
   public void proxyRemoved(ConfigElement elm, String token, int unit)
   {
      DefaultMutableTreeNode parent = null;
         
      // Get the appropriate parent.
      if(token.equals("digital_proxy"))
      {
         parent = (DefaultMutableTreeNode)mDigitalTypeNode;
      }
      else if(token.equals("analog_proxy"))
      {
         parent = (DefaultMutableTreeNode)mAnalogTypeNode;
      }
      else if(token.equals("position_proxy"))
      {
         parent = (DefaultMutableTreeNode)mPositionTypeNode;
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

   public List findDigitalProxies()
   {
      List digital_proxies = new ArrayList();
      
      List elms = getBroker().getElementsIncludingEmbedded(mContext);
      List proxies = ConfigUtilities.getElementsWithDefinition(elms, "digital_proxy");

      for(Iterator itr = proxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         
         String device_name = ((ConfigElementPointer)elm.getProperty("device", 0)).getTarget();
         int unit = ((Integer)elm.getProperty("unit", 0)).intValue();
         
         //System.out.println("Device: " + device + " Unit: " + unit);
         //System.out.println("Name: " + mConfigElement.getName());
         
         if(device_name.equals(mConfigElement.getName()))
         {
            //System.out.println("Matched Proxy: " + elm.getName());
            digital_proxies.add(elm);
         }
      }
      return digital_proxies;
   }

   public List findAnalogProxies()
   {
      List analog_proxies = new ArrayList();
      
      List elms = getBroker().getElementsIncludingEmbedded(mContext);
      List proxies = ConfigUtilities.getElementsWithDefinition(elms, "analog_proxy");

      for(Iterator itr = proxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         
         String device_name = ((ConfigElementPointer)elm.getProperty("device", 0)).getTarget();
         int unit = ((Integer)elm.getProperty("unit", 0)).intValue();
         
         //System.out.println("Device: " + device + " Unit: " + unit);
         //System.out.println("Name: " + mConfigElement.getName());
         
         if(device_name.equals(mConfigElement.getName()))
         {
            //System.out.println("Matched Proxy: " + elm.getName());
            analog_proxies.add(elm);
         }
      }
      return analog_proxies;
   }
   
   public List findPositionProxies()
   {
      List position_proxies = new ArrayList();
      
      List elms = getBroker().getElementsIncludingEmbedded(mContext);
      List proxies = ConfigUtilities.getElementsWithDefinition(elms, "position_proxy");

      for(Iterator itr = proxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         
         String device_name = ((ConfigElementPointer)elm.getProperty("device", 0)).getTarget();
         int unit = ((Integer)elm.getProperty("unit", 0)).intValue();
         
         //System.out.println("Device: " + device + " Unit: " + unit);
         //System.out.println("Name: " + mConfigElement.getName());
         
         if(device_name.equals(mConfigElement.getName()))
         {
            //System.out.println("Matched Proxy: " + elm.getName());
            position_proxies.add(elm);
         }
      }
      return position_proxies;
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
    * Used to represent different categories of units on each device.
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
    * Used to represent a position/digital/analog unit on an device.
    */
   protected class DeviceUnit
   {
      /** Name of the device unit. */
      private String mName = null;

      /** Name of the device. */
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
       * Get the name of the device that this DeviceUnit is on.
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
