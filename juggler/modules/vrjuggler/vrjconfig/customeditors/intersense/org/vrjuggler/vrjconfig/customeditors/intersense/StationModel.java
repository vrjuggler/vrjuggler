/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
import org.vrjuggler.jccl.editors.*;

/**
 * Represents a single Intersense tracker station.
 */
public class StationModel
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
      mStationName = name;
      mStationElement.setName(mStationName);
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
      
      List elms = getBroker().getElements(mStationContext);
      List proxies = ConfigUtilities.getElementsWithDefinition(elms, "digital_proxy");

      for(Iterator itr = proxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         
         ConfigElementPointer device = (ConfigElementPointer)elm.getProperty("device", 0);

         int unit = ((Integer)elm.getProperty("unit", 0)).intValue();
         
         //System.out.println("Device: " + device + " Unit: " + unit);
         //System.out.println("Name: " + mISenseElement.getName() + " Index: " + mStationIndex);
         
         if(mUseDigital && device.getTarget().equals(mISenseElement.getName()))
         {
            if(unit >= mDigitalFirst && unit <= (mDigitalFirst + mDigitalCount))
            {
               System.out.println("Matched Proxy: " + elm.getName());
               digital_proxies.add(elm);
            }
         }
      }
      return digital_proxies;
   }
   
   public List findAnalogProxies()
   {
      List analog_proxies = new ArrayList();
      
      List elms = getBroker().getElements(mStationContext);
      List proxies = ConfigUtilities.getElementsWithDefinition(elms, "analog_proxy");

      for(Iterator itr = proxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         
         ConfigElementPointer device = (ConfigElementPointer)elm.getProperty("device", 0);

         int unit = ((Integer)elm.getProperty("unit", 0)).intValue();
         
         //System.out.println("Device: " + device + " Unit: " + unit);
         //System.out.println("Name: " + mISenseElement.getName() + " Index: " + mStationIndex);
         
         if(mUseAnalog && device.getTarget().equals(mISenseElement.getName()))
         {
            if(unit >= mAnalogFirst && unit <= (mAnalogFirst + mAnalogCount))
            {
               System.out.println("Matched Proxy: " + elm.getName());
               analog_proxies.add(elm);
            }
         }
      }
      return analog_proxies;
   }
   
   public List findPositionProxies()
   {
      List position_proxies = new ArrayList();
      
      List elms = getBroker().getElements(mStationContext);
      List proxies = ConfigUtilities.getElementsWithDefinition(elms, "position_proxy");

      for(Iterator itr = proxies.iterator() ; itr.hasNext() ; )
      {
         ConfigElement elm = (ConfigElement)itr.next();
         
         ConfigElementPointer device = (ConfigElementPointer)elm.getProperty("device", 0);

         int unit = ((Integer)elm.getProperty("unit", 0)).intValue();
         
         //System.out.println("Device: " + device + " Unit: " + unit);
         //System.out.println("Name: " + mISenseElement.getName() + " Index: " + mStationIndex);
         
         
         if(device.getTarget().equals(mISenseElement.getName()))
         {
            if(unit == mStationIndex)
            {
               System.out.println("Matched Proxy: " + elm.getName());
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
