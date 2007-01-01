/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

import java.util.*;
import javax.swing.event.EventListenerList;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.jccl.editors.*;

/**
 * Keeps track of all configuration information for an Intersense tracker.
 *
 * @see ConfigListener
 * @see ConfigElementListener
 */
public class IntersenseModel implements ConfigListener, ConfigElementListener
{
   /** ConfigElement for Intersense tracker. */
   private ConfigElement   mISenseConfigElement = null;

   /** ConfigContext used by Intersense tracker. */
   private ConfigContext   mISenseConfigContext = null;
  
   /** List of ConfigElements, one for each Intersense station. */
   private List            mStationElements     = new ArrayList();

   /** List of Intersense station models. */
   private List            mStationModels       = new ArrayList();
   
   /** Location of the Intersense API library supplied by Intersense. */
   private String          mDriverLocation      = null;

   /** Name of serial port tracker is connected to. */
   private String          mPortName            = null;

   /** PositionFilter that translates and rotates position input into VR Juggler coordinate space. */
   private ConfigElement   mPositionFilter      = null;

   /** Wether verbose output is enabled for the Intersense API library. */
   private Boolean         mVerboseEnabled      = null;

   /** The minimum float value for analog input. */
   private Float           mRangeMin            = null;
   
   /** Listeners interested in this Intersense model. */
   private EventListenerList listenerList = new EventListenerList();

   /**
    * Returns the location of the Intersense API library supplied by Intersense.
    */
   public String getDriverLocation()
   {
      return mDriverLocation;
   }
   
   /**
    * Specify the location of the Intersense API library supplied by Intersense.
    *
    * @param location full path to Intersense API library.
    */
   public void setDriverLocation(String location)
   {
      mDriverLocation = location;
   }

   /**
    * Returns the name of the serial port the tracker is connected to.
    */
   public String getPortName()
   {
      return mPortName;
   }
   
   /**
    * Specifies the name of the serial port that the tracker is connected to.
    *
    * @param name serial port the tracker is connected to.
    */
   public void setPortName(String name)
   {
      mPortName = name;
   }

   /**
    * Returns a list of all station models associated with this Intersense device.
    */
   public List getStationModels()
   {
      return mStationModels;
   }

   /**
    * Constructor.
    *
    * @param elm        ConfigElement for this Intersense device.
    * @param context    ConfigContext associated with this Intersense device.
    */
   public IntersenseModel(ConfigElement elm, ConfigContext context)
   {
      // Save references for later use.
      mISenseConfigElement = elm;
      mISenseConfigContext = context;
      
      // Add a listener to update this model if more config elements are added to context.
      getBroker().addConfigListener(this);

      System.out.println("================================");
      System.out.println("Parsing Intersense ConfigElement");
      System.out.println("================================");

      // Get general information about tracker.
      mPortName = (String)mISenseConfigElement.getProperty("port",0);
      mDriverLocation = (String)mISenseConfigElement.getProperty("driver",0);
      
      // Get a list of all embedded station elements.
      java.util.List stations = elm.getPropertyValues("stations");
      for(Iterator itr = stations.iterator() ; itr.hasNext() ; )
      {
         Object value = itr.next();
         if(value instanceof ConfigElement)
         {
            ConfigElement station_elm = (ConfigElement)value;
            System.out.println("Embedded Element: " + station_elm.getName());
            station_elm.addConfigElementListener(this);
            mStationElements.add(station_elm);
            
            // Create a new StationModel for each embedded element.
            StationModel station = new StationModel(elm, station_elm, mISenseConfigContext);
            mStationModels.add(station);

            // Make this ProxyTreeModel listen to us for events.
            this.addIntersenseModelListener(station.getProxyModel());
         }
      }

      // Sort stations depending on station index.
      Collections.sort(mStationModels, new Comparator()
            {
               public int compare(Object o1, Object o2)
               {
                  if(((StationModel)o1).getStationIndex() < ((StationModel)o2).getStationIndex())
                  {
                     return -1;
                  }
                  else if(((StationModel)o1).getStationIndex() > ((StationModel)o2).getStationIndex())
                  {
                     return 1;
                  }
                  return 0;
               }
               public boolean equals(Object obj)
               { return(this == obj); }
            });

      // We must listen to all proxy elements to ensure that we are informed if
      // they start pointing at this device.
      List all_elms = getBroker().getElementsIncludingEmbedded(mISenseConfigContext);
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
      
      fixStationUnitNumbers();
   }

   public void fixStationUnitNumbers()
   {
      // Make sure that the buffer ranges are large enough.
      for(Iterator itr = mStationModels.iterator() ; itr.hasNext() ; )
      {
         StationModel station = (StationModel)itr.next();

         if(station.isDigitalEnabled() && station.getDigitalCount() < 6)
         {
            System.out.println("ERROR: This editor assumes that we are using 6 digital inputs."
                               + " Changing property digital_count to 6.");
            station.setDigitalCount(6);
         }
         if(station.isAnalogEnabled() && station.getAnalogCount() < 2)
         {
            System.out.println("ERROR: This editor assumes that we are using 2 analog inputs."
                               + " Changing property analog_count to 2.");
            station.setAnalogCount(2);
         }
      }
      
      int next_open_digital = 0;
      int next_open_analog = 0;
      
      // Make sure that the buffer ranges do not overlap.
      for(Iterator itr = mStationModels.iterator() ; itr.hasNext() ; )
      {
         StationModel station = (StationModel)itr.next();
         if(station.isDigitalEnabled() && station.getDigitalFirst() < next_open_digital)
         {
            System.out.println("ERROR: Digital Unit number out of order."
                               + " Station indices will be shifted to correct for this.");
            station.setDigitalFirst(next_open_digital);
         }
         next_open_digital = station.getDigitalFirst() + station.getDigitalCount();

         if(station.isAnalogEnabled() && station.getAnalogFirst() < next_open_analog)
         {
            System.out.println("ERROR: Analog Unit number out of order."
                               + " Station indices will be shifted to correct for this.");
            station.setAnalogFirst(next_open_analog);
         }
         next_open_analog = station.getAnalogFirst() + station.getAnalogCount();
      }
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
         String our_name = mISenseConfigElement.getName();
         // If the proxy pointed at us inform all listeners.
         if(device_name.equals(our_name))
         {
            int unit = ((Integer)elm.getProperty("unit", 0)).intValue();
            fireProxyAdded(elm, token, unit);
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
         String our_name = mISenseConfigElement.getName();
         // If the proxy pointed at us inform all listeners.
         if(device_name.equals(our_name))
         {
            int unit = ((Integer)elm.getProperty("unit", 0)).intValue();
            fireProxyRemoved(elm, token, unit);
         }
      }
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
         String our_name = mISenseConfigElement.getName();
         
         if(evt.getProperty() == null)
         {
            return;
         }
        
         String device_name = ((ConfigElementPointer)elm.getProperty("device", 0)).getTarget();
         int unit = ((Integer)elm.getProperty("unit", 0)).intValue();

         // Remove the node if it exists.
         fireProxyRemoved(elm, token, unit);

         // Add the node if it points at this device.
         if(device_name.equals(our_name))
         {
            fireProxyAdded(elm, token, unit);
         }
      }
   }

   /**
    * Adds the given listener to be notified when this Intersense model changes.
    */
   public void addIntersenseModelListener(IntersenseModelListener listener)
   {
      listenerList.add(IntersenseModelListener.class, listener);
   }

   /**
    * Removes the given listener that was registered to be notified when this
    * Intersense model changes.
    */
   public void removeIntersenseModelListener(IntersenseModelListener listener)
   {
      listenerList.remove(IntersenseModelListener.class, listener);
   }

   /**
    * Notifies listeners that a new element has been created that points at us.
    */
   protected void fireProxyAdded(ConfigElement elm, String token, int unit)
   {
      IntersenseModelEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == IntersenseModelListener.class)
         {
            if (evt == null)
            {
               evt = new IntersenseModelEvent(this, elm, token, unit);
            }
            ((IntersenseModelListener)listeners[i+1]).proxyAdded(evt);
         }
      }
   }
   
   /**
    * Notifies listeners that an element that points at us is being removed.
    */
   protected void fireProxyRemoved(ConfigElement elm, String token, int unit)
   {
      IntersenseModelEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == IntersenseModelListener.class)
         {
            if (evt == null)
            {
               evt = new IntersenseModelEvent(this, elm, token, unit);
            }
            ((IntersenseModelListener)listeners[i+1]).proxyRemoved(evt);
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
}
