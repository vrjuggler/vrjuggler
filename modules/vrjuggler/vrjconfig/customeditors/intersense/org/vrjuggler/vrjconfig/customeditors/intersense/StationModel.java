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
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.jccl.editors.*;

/**
 * Represents a single Intersense tracker station.
 */
public class StationModel implements ConfigElementListener
{
   /** ConfigElement for this station. */
   private ConfigElement        mStationElement         = null;

   /** ConfigContext associated with this station. */
   private ConfigContext        mStationContext         = null;
   
   /** ConfigElement for the entire Intersense tracker. */
   private ConfigElement        mISenseElement          = null;

   /** Name of this Intersense station. */
   private String               mStationName            = null;
   
   /** Model of which proxies are pointing at this station. */
   private ProxyTreeModel       mProxyModel             = null;
      
   private boolean              mEnabled                = false;
   private int                  mStationIndex           = 0;
   private boolean              mUseDigital             = false;
   private boolean              mUseAnalog              = false;
   
   /** Station's first position in the digital buffer. */
   private int                  mDigitalFirst           = 0;

   /** Number of digital units this device contains. */
   private int                  mDigitalCount           = 0;
   
   /** Station's first position in the analog buffer. */
   private int                  mAnalogFirst            = 0;
   
   /** Number of analog units this device contains. */
   private int                  mAnalogCount            = 0;

   /**
    * Constructor.
    *
    * @param isense_elm ConfigElement for the entire Intersense tracker
    * @param elm        ConfigElement for this station
    * @param context    ConfigContext associated with this station.
    */
   public StationModel(ConfigElement isense_elm, ConfigElement elm, ConfigContext context)
   {
      // Save references for later use.
      mISenseElement = isense_elm;
      mStationName = elm.getName();
      mStationElement = elm;
      mStationContext = context;

      mStationElement.addConfigElementListener(this);

      System.out.println("=======================================");
      System.out.println("Parsing ISense Station Element: " + elm.getName());
      System.out.println("=======================================");

      // Get configuration information about this station.
      mEnabled = ((Boolean)mStationElement.getProperty("enabled", 0)).booleanValue();
      mStationIndex = ((Integer)mStationElement.getProperty("station_index", 0)).intValue();
      mUseDigital = ((Boolean)mStationElement.getProperty("use_digital", 0)).booleanValue();
      mUseAnalog = ((Boolean)mStationElement.getProperty("use_analog", 0)).booleanValue();
      mDigitalFirst = ((Integer)mStationElement.getProperty("digital_first", 0)).intValue();
      mDigitalCount = ((Integer)mStationElement.getProperty("digital_count", 0)).intValue();
      mAnalogFirst = ((Integer)mStationElement.getProperty("analog_first", 0)).intValue();
      mAnalogCount = ((Integer)mStationElement.getProperty("analog_count", 0)).intValue();

      // Create a new ProxyTreeModel for this station.
      mProxyModel = new ProxyTreeModel(this, mISenseElement, mStationContext);
   }

   /**
    * Returns the ProxyTreeModel that represents this station.
    *
    * @see ProxyTreeModel
    */
   public ProxyTreeModel getProxyModel()
   {
      return mProxyModel;
   }

   /**
    * Get the name of this Intersense station.
    */
   public String getName()
   {
      return mStationName;
   }

   /**
    * Set the name of this Intersense station.
    */
   public void setName(String name)
   {
      mStationElement.setName(mStationName);
   }

   /**
    * Return the station index of this Intersense station.
    */
   int getStationIndex()
   {
      return mStationIndex;
   }

   /**
    * ConfigElementListener interface to catch all changes in existing proxies by
    * calling a helper method, elementChanged.
    *
    * @see ConfigElementListener
    * @see #elementChanged
    */
   public void nameChanged(ConfigElementEvent evt)
   {
      ConfigElement elm = (ConfigElement)evt.getSource();
      if(elm == mStationElement)
      {
         mStationName = elm.getName();
      }
   }
   public void propertyValueChanged(ConfigElementEvent evt)
   {
      ConfigElement elm = (ConfigElement)evt.getSource();
      String token = elm.getDefinition().getToken();

      if(token.equals("intersense_station") && elm == mStationElement)
      {
         if(evt.getProperty().equals("digital_count"))
         {
            mDigitalCount = ((Integer)mStationElement.getProperty("digital_count", 0)).intValue();
         }
         else if(evt.getProperty().equals("analog_count"))
         {
            mAnalogCount = ((Integer)mStationElement.getProperty("analog_count", 0)).intValue();
         }
         else if(evt.getProperty().equals("digital_first"))
         {
            int new_first = ((Integer)evt.getValue()).intValue();
            int delta = new_first - mDigitalFirst;
         
            List proxies = findDigitalProxies();
            for(Iterator itr = proxies.iterator() ; itr.hasNext() ; )
            {
               ConfigElement temp_proxy = (ConfigElement)itr.next();
               int unit = ((Integer)temp_proxy.getProperty("unit", 0)).intValue();
               temp_proxy.setProperty("unit", 0, new Integer(unit + delta));
            }
         
            mDigitalFirst += delta;
            mProxyModel.rebuildTree();
         }
         else if(evt.getProperty().equals("analog_first"))
         {
            int new_first = ((Integer)evt.getValue()).intValue();
            int delta = new_first - mAnalogFirst;
         
            List proxies = findAnalogProxies();
            for(Iterator itr = proxies.iterator() ; itr.hasNext() ; )
            {
               ConfigElement temp_proxy = (ConfigElement)itr.next();
               int unit = ((Integer)temp_proxy.getProperty("unit", 0)).intValue();
               temp_proxy.setProperty("unit", 0, new Integer(unit + delta));
            }
         
            mAnalogFirst += delta;
            mProxyModel.rebuildTree();
         }

      }
   }

   public void propertyValueOrderChanged(ConfigElementEvent evt)
   {;}

   public void propertyValueAdded(ConfigElementEvent evt)
   {;}
   public void propertyValueRemoved(ConfigElementEvent evt)
   {;}
   
   /**
    * Return the index of the starting location in the digital buffer.
    */
   public int getDigitalFirst()
   {
      return mDigitalFirst;
   }

   public void setDigitalFirst(int first)
   {
      mStationElement.setProperty("digital_first", 0, new Integer(first));
   }

   /**
    * Return the number of digital inputs.
    */
   public int getDigitalCount()
   {
      return mDigitalCount;
   }
   
   /**
    * Set the number of digital inputs.
    */
   public void setDigitalCount(int num)
   {
      mStationElement.setProperty("digital_count", 0, new Integer(num));
   }
   
   /**
    * Return the index of the starting location in the analog buffer.
    */
   public int getAnalogFirst()
   {
      return mAnalogFirst;
   }
   
   public void setAnalogFirst(int first)
   {
      mStationElement.setProperty("analog_first", 0, new Integer(first));
   }
   
   /**
    * Return the number of analog inputs.
    */
   public int getAnalogCount()
   {
      return mAnalogCount;
   }
   
   /**
    * Set the number of analog inputs.
    */
   public void setAnalogCount(int num)
   {
      mStationElement.setProperty("analog_count", 0, new Integer(num));
   }

   /**
    * Return whether digital input is enabled.
    */
   public boolean isDigitalEnabled()
   {
      return mUseDigital;
   }

   /**
    * Set whether digital input is enabled.
    */
   public void setDigitalEnabled(boolean val)
   {
      mUseDigital = val;
      mStationElement.setProperty("use_digital", 0, new Boolean(mUseDigital));
   }
   
   /**
    * Return whether analog input is enabled.
    */
   public boolean isAnalogEnabled()
   {
      return mUseAnalog;
   }
   
   /**
    * Set whether analog input is enabled.
    */
   public void setAnalogEnabled(boolean val)
   {
      mUseAnalog = val;
      mStationElement.setProperty("use_analog", 0, new Boolean(mUseAnalog));
   }

   public List findDigitalProxies()
   {
      List digital_proxies = new ArrayList();
      
      List elms = getBroker().getElementsIncludingEmbedded(mStationContext);
      List proxies = ConfigUtilities.getElementsWithDefinition(elms, "digital_proxy");

      for(Iterator itr = proxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         
         String device_name = ((ConfigElementPointer)elm.getProperty("device", 0)).getTarget();
         int unit = ((Integer)elm.getProperty("unit", 0)).intValue();
         
         //System.out.println("Device: " + device + " Unit: " + unit);
         //System.out.println("Name: " + mISenseElement.getName() + " Index: " + mStationIndex);
         
         if(mUseDigital && device_name.equals(mISenseElement.getName()))
         {
            if(unit >= mDigitalFirst && unit < (mDigitalFirst + mDigitalCount))
            {
               //System.out.println("Matched Proxy: " + elm.getName());
               digital_proxies.add(elm);
            }
         }
      }
      return digital_proxies;
   }
   
   public List findAnalogProxies()
   {
      List analog_proxies = new ArrayList();
      
      List elms = getBroker().getElementsIncludingEmbedded(mStationContext);
      List proxies = ConfigUtilities.getElementsWithDefinition(elms, "analog_proxy");

      for(Iterator itr = proxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         
         String device_name = ((ConfigElementPointer)elm.getProperty("device", 0)).getTarget();
         int unit = ((Integer)elm.getProperty("unit", 0)).intValue();
         
         //System.out.println("Device: " + device + " Unit: " + unit);
         //System.out.println("Name: " + mISenseElement.getName() + " Index: " + mStationIndex);
         
         if(mUseAnalog && device_name.equals(mISenseElement.getName()))
         {
            if(unit >= mAnalogFirst && unit < (mAnalogFirst + mAnalogCount))
            {
               //System.out.println("Matched Proxy: " + elm.getName());
               analog_proxies.add(elm);
            }
         }
      }
      return analog_proxies;
   }
   
   public List findPositionProxies()
   {
      List position_proxies = new ArrayList();
      
      List elms = getBroker().getElementsIncludingEmbedded(mStationContext);
      List proxies = ConfigUtilities.getElementsWithDefinition(elms, "position_proxy");

      for(Iterator itr = proxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         
         String device_name = ((ConfigElementPointer)elm.getProperty("device", 0)).getTarget();
         int unit = ((Integer)elm.getProperty("unit", 0)).intValue();
         
         //System.out.println("Device: " + device + " Unit: " + unit);
         //System.out.println("Name: " + mISenseElement.getName() + " Index: " + mStationIndex);
         
         
         if(device_name.equals(mISenseElement.getName()))
         {
            if(unit == mStationIndex)
            {
               //System.out.println("Matched Proxy: " + elm.getName());
               position_proxies.add(elm);
            }
         }
      }
      return position_proxies;
   }
   
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
