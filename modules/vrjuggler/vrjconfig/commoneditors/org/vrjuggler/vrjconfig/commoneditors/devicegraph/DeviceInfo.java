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

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import javax.swing.event.EventListenerList;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;

import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import org.vrjuggler.vrjconfig.commoneditors.event.DeviceUnitEvent;
import org.vrjuggler.vrjconfig.commoneditors.event.DeviceUnitListener;


/**
 * A holder for <code>ConfigElement</code>s that is used to provide run-time
 * identification of a <code>ConfigElement</code> reference for a device
 * configuration.  Proper use of this type is critical for proper handling of
 * <code>ConfigElement</code> references held in a JGraph graph model.
 *
 * <p>
 * This class provides a means to determine whether the device configured by
 * the contained config element supports a variable number of input sources
 * (units) and to facilitate creation and deletion of those input sources if
 * a variable number is supported.  However, it does not provide an inherent
 * mechanism for determining the current number of input sources for that
 * device.  Since this class is designed specfically for use in conjunction
 * with <code>DeviceGraphModel</code>, it is up to the code constructing the
 * graph to provide a way for determining the current number of input sources
 * for a given config element.  In general, there is no way to look at an
 * arbitrary config element and determine how many input sources the device
 * has.
 * </p>
 *
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.DeviceGraphLayoutCache
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.DeviceGraphModel
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.ProxiedDeviceCellViewFactory
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.ProxyPointerEdge
 */
public class DeviceInfo
   extends ConfigElementHolder
   implements ConfigElementListener
{
   /**
    * Creates a new <code>DeviceInfo</code> object for a device that has
    * exactly one input source (unit) at all times.
    * 
    * @param devElt     the device config element
    * @param ctx        the config context that contains <code>devElt</code>
    */
   public DeviceInfo(ConfigElement devElt, ConfigContext ctx)
   {
      this(devElt, ctx, (String) null);
   }

   /**
    * Creates a new <code>DeviceInfo</code> object and looks up the
    * property definition for the named property if a name is provided.  The
    * value of <code>unitPropertyName</code> may be null.  If it is non-null,
    * then it must be the name of a property in <code>devElt</code>'s config
    * definition.  This version of the constructor should be used when the
    * config element has a proeprty with a variable number of values that
    * indicate how many input sources (units) are available in the device.
    *
    * @param devElt             the device config element
    * @param ctx                the config context that contains
    *                           <code>devElt</code>
    * @param unitPropertyName   the name of the property that represents
    *                           the number of available input sources (units)
    *                           the device has (may be null)
    */
   public DeviceInfo(ConfigElement devElt, ConfigContext ctx,
                     String unitPropertyName)
   {
      super(devElt, ctx);

      devElt.addConfigElementListener(this);

      ConfigDefinition def = devElt.getDefinition();
      Integer unit_type = UnitTypeHelpers.getSingleUnitType(def);

      // If unitPropertyName contains a non-empty string, then we have a device
      // that supports a variable number of units, each of the same input type.
      if ( unitPropertyName != null && ! unitPropertyName.equals("") )
      {
         mUnitTypeMap = new HashMap();
         mUnitTypeMap.put(unit_type,
                          def.getPropertyDefinition(unitPropertyName));

         this.unitTypes     = mUnitTypeMap.keySet();
         mVariableUnitCount = true;
      }
      // Otherwise, our device supports a fixed number of units (one or more),
      // each of the same input type.
      else
      {
         this.unitTypes = new ArrayList(1);
         this.unitTypes.add(unit_type);

         mVariableUnitCount = false;
      }
   }

   /**
    * This version of the constructor should be used when the config element
    * has a <i>variable</i> number of input sources (units) <i>and</i> supports
    * multiipe input source types.
    *
    * @param devElt             the device config element
    * @param ctx                the config context that contains
    *                           <code>devElt</code>
    * @param unitTypeMap        a mapping from unit types to property
    *                           definitions (<code>Integer</code> to
    *                           <code>PropertyDefinition</code>)
    *
    * @see UnitConstants
    * @see org.vrjuggler.jccl.config.PropertyDefinition
    */
   public DeviceInfo(ConfigElement devElt, ConfigContext ctx,
                     Map unitTypeMap)
   {
      super(devElt, ctx);

      devElt.addConfigElementListener(this);

      mVariableUnitCount = true;
      mUnitTypeMap       = unitTypeMap;
      this.unitTypes     = unitTypeMap.keySet();
   }

   /**
    * This version of the constructor should be used when the config element
    * has a <i>fixed</i> number of input sources (units) <i>and</i> supports
    * multiipe input source types.
    *
    * @param devElt     the device config element
    * @param ctx        the config context that contains <code>devElt</code>
    * @param unitTypes  the list of unit types supported by this device
    *
    * @see UnitConstants
    */
   public DeviceInfo(ConfigElement devElt, ConfigContext ctx,
                     Collection unitTypes)
   {
      super(devElt, ctx);

      devElt.addConfigElementListener(this);

      mVariableUnitCount = false;
      this.unitTypes     = unitTypes;

      mUnitTypeMap = new HashMap();
      for ( java.util.Iterator i = this.unitTypes.iterator(); i.hasNext(); )
      {
         mUnitTypeMap.put(i.next(), null);
      }
   }

   /**
    * Adds the given number of units to the artificial unit property and
    * stores the result in the given unit type map.  If there is already an
    * artificial unit property for the given unit type in the map, then
    * <code>numUnits</code> are added to the current set of units.
    *
    * @param unitTypeMap        the unit type map providing a mapping from
    *                           unit types (instances of
    *                           <code>java.lang.Integer</code>) to the
    *                           artificial unit property for that type
    * @param unitType           the type of the artificial unit property
    *                           (one of the <code>UnitConstants</code>
    *                           values)
    * @param numUnits           the number of units to add to the artificial
    *                           unit property
    *
    * @see UnitConstants
    */
   public static void addArtificialUnits(Map unitTypeMap, Integer unitType,
                                         int numUnits)
   {
      if ( unitTypeMap.get(unitType) == null )
      {
         unitTypeMap.put(unitType, new Integer(numUnits));
      }
      else
      {
         Integer cur_count = (Integer) unitTypeMap.get(unitType);
         unitTypeMap.put(unitType,
                         new Integer(cur_count.intValue() + numUnits));
      }
   }

   public void nameChanged(ConfigElementEvent evt)
   {
      /* Do nothing. */ ;
   }

   public void propertyValueAdded(ConfigElementEvent evt)
   {
      if ( hasVariableUnitCount() )
      {
         Collection unit_types = getUnitTypes();

         for ( Iterator t = unit_types.iterator(); t.hasNext(); )
         {
            Integer type = (Integer) t.next();
            String prop_token = getUnitPropertyToken(type);

            // If the device has a variable unit count and the value added
            // was for the device unit property, then we need to add another
            // row to the renderer to repersent the new unit.
            if ( evt.getProperty().equals(prop_token) )
            {
               fireDeviceUnitAdded(type, getUnitCount(type) - 1);
               break;
            }
         }
      }
   }

   public void propertyValueChanged(ConfigElementEvent evt)
   {
      if ( hasVariableUnitCount() )
      {
         Collection unit_types = getUnitTypes();

         for ( Iterator t = unit_types.iterator(); t.hasNext(); )
         {
            Integer type = (Integer) t.next();
            String prop_token = getUnitPropertyToken(type);

            // If the device has a variable unit count and the value added
            // was for the device unit property, then we need to add another
            // row to the renderer to repersent the new unit.
            if ( evt.getProperty().equals(prop_token) )
            {
               int cur_count = getUnitCount(type);
               int old_count = ((Number) evt.getValue()).intValue();

               // Unit addition.  This always appends the new unit.
               if ( cur_count > old_count )
               {
                  fireDeviceUnitAdded(type, cur_count - 1);
               }
               // Unit removal.
               else
               {
                  fireDeviceUnitRemoved(type, cur_count);
               }

               break;
            }
         }
      }
   }

   public void propertyValueRemoved(ConfigElementEvent evt)
   {
      if ( hasVariableUnitCount() )
      {
         Collection unit_types = getUnitTypes();

         for ( Iterator t = unit_types.iterator(); t.hasNext(); )
         {
            Integer type = (Integer) t.next();
            String prop_token = getUnitPropertyToken(type);

            if ( evt.getProperty().equals(prop_token) )
            {
               fireDeviceUnitRemoved(type, evt.getIndex());
               break;
            }
         }
      }
   }

   public void addDeviceUnitListener(DeviceUnitListener l)
   {
      listenerList.add(DeviceUnitListener.class, l);
   }

   public void removeDeviceUnitListener(DeviceUnitListener l)
   {
      listenerList.remove(DeviceUnitListener.class, l);
   }

   public void addUnit(Integer unitType)
      throws IllegalArgumentException
   {
      if ( ! hasVariableUnitCount() )
      {
         throw new IllegalArgumentException("Device element does not have " +
                                            "variable unit numbers");
      }

      Object unit_prop_obj = mUnitTypeMap.get(unitType);

      if ( unit_prop_obj instanceof PropertyDefinition )
      {
         PropertyDefinition prop_def = (PropertyDefinition) unit_prop_obj;

         if ( prop_def.isVariable() )
         {
            PropertyValueDefinition value_def =
               prop_def.getPropertyValueDefinition(0);
            String token = prop_def.getToken();

            Object default_value = value_def.getDefaultValue();

            if ( default_value == null )
            {
               if ( prop_def.getType() == ConfigElement.class )
               {
                  ConfigBroker broker = new ConfigBrokerProxy();
                  ConfigDefinitionRepository repos = broker.getRepository();
                  ConfigElementFactory factory =
                     new ConfigElementFactory(repos.getAllLatest());

                  // XXX: How do we deal with this?  The flexibility of allowed
                  // types makes this tricky.
                  int count = getElement().getPropertyValueCount(token);
                  default_value =
                     factory.create(value_def.getLabel() + " " + count,
                                    repos.get(prop_def.getAllowedType(0)));
               }
               else
               {
                  System.out.println("Don't know what to do for type " +
                                     prop_def.getType());
               }
            }

            System.out.println("[DeviceVertexRenderer.addUnit()] " +
                               "default_value == " + default_value + " (type: " +
                               prop_def.getType() + ")");
            getElement().addProperty(token, default_value, getContext());
         }
         else
         {
            if ( prop_def.getType() == Integer.class )
            {
               Integer old_value =
                  (Integer) getElement().getProperty(prop_def.getToken(), 0);
               Integer new_value = new Integer(old_value.intValue() + 1);
               getElement().setProperty(prop_def.getToken(), 0, new_value,
                                        getContext());
            }
            else
            {
               throw new IllegalArgumentException("Don't know how to add a " +
                                                  "new unit to property " +
                                                  prop_def.getToken());
            }
         }
      }
      else
      {
         int cur_value = ((Number) unit_prop_obj).intValue();
         mUnitTypeMap.put(unitType, new Integer(cur_value + 1));
         fireDeviceUnitAdded(unitType, cur_value);
      }
   }

   public void removeUnit(Integer unitType, int unitNumber)
      throws IllegalArgumentException
   {
      if ( ! hasVariableUnitCount() )
      {
         throw new IllegalArgumentException("Device element does not have " +
                                            "variable unit numbers");
      }

      Object unit_prop_obj = mUnitTypeMap.get(unitType);

      if ( unit_prop_obj instanceof PropertyDefinition )
      {
         PropertyDefinition prop_def = (PropertyDefinition) unit_prop_obj;

         if ( prop_def.isVariable() )
         {
            getElement().removeProperty(prop_def.getToken(), unitNumber,
                                        getContext());
         }
         else
         {
            if ( prop_def.getType() == Integer.class )
            {
               Integer value =
                  (Integer) getElement().getProperty(prop_def.getToken(), 0);
               Integer new_value = new Integer(value.intValue() - 1);
               getElement().setProperty(prop_def.getToken(), 0, new_value,
                                        getContext());
            }
            else
            {
               throw new IllegalArgumentException("Don't know how to remove " +
                                                  "unit for property " +
                                                  prop_def.getToken());
            }
         }
      }
      else
      {
         int unit_number = ((Number) unit_prop_obj).intValue() - 1;
         mUnitTypeMap.put(unitType, new Integer(unit_number));
         fireDeviceUnitRemoved(unitType, unit_number);
      }
   }

   private int getUnitCount(Integer unitType)
   {
      int count = 0;
      Object unit_prop_obj = mUnitTypeMap.get(unitType);

      if ( unit_prop_obj instanceof PropertyDefinition )
      {
         PropertyDefinition prop_def = (PropertyDefinition) unit_prop_obj;

         if ( prop_def.isVariable() )
         {
            count = getElement().getPropertyValueCount(prop_def.getToken());
         }
         else
         {
            Number val = (Number) getElement().getProperty(prop_def.getToken(),
                                                           0);
            count = val.intValue();
         }
      }
      else
      {
         count = ((Number) unit_prop_obj).intValue();
      }

      return count;
   }

   private int getUnitCount(String propToken)
   {
      int count = 0;
      Collection unit_types = getUnitTypes();

      for ( Iterator t = unit_types.iterator(); t.hasNext(); )
      {
         Integer type = (Integer) t.next();
         String cur_prop_token = getUnitPropertyToken(type);

         if ( propToken.equals(cur_prop_token) )
         {
            count = getUnitCount(type);
            break;
         }
      }

      return count;
   }

   private String getUnitPropertyToken(Integer unitType)
   {
      String result = null;

      Object unit_prop_obj = mUnitTypeMap.get(unitType);

      if ( unit_prop_obj instanceof PropertyDefinition )
      {
         result = ((PropertyDefinition) unit_prop_obj).getToken();
      }

      return result;
   }

   public Class getUnitPropertyType(Integer unitType)
   {
      Class result = null;

      Object unit_prop_obj = mUnitTypeMap.get(unitType);

      if ( unit_prop_obj instanceof PropertyDefinition )
      {
         result = ((PropertyDefinition) unit_prop_obj).getType();
      }

      return result;
   }

   /**
    * Determines whether the device config element contained within this
    * object is for a device that has a variable number of input sources
    * (units).
    *
    * @return <code>true</code> is returned if the device allows a variable
    *         number of input sources (units); <code>false</code> is returned
    *         otherwise.
    */
   public boolean hasVariableUnitCount()
   {
      return mVariableUnitCount;
   }

   /**
    * Returns the collection of input source (unit) types supported by the
    * device represented by the contained config element.  This method is safe
    * to call for all instances of this class regardless of the device type
    * behind the scenes.
    */
   public Collection getUnitTypes()
   {
      return unitTypes;
   }

   protected void fireDeviceUnitAdded(Integer type, int unitNum)
   {
      DeviceUnitEvent evt = null;
      Object[] listeners = listenerList.getListenerList();

      for ( int i = listeners.length - 2; i >= 0; i -= 2 )
      {
         if ( listeners[i] == DeviceUnitListener.class )
         {
            if (evt == null)
            {
               evt = new DeviceUnitEvent(this, type, unitNum);
            }

            ((DeviceUnitListener) listeners[i + 1]).deviceUnitAdded(evt);
         }
      }
   }

   protected void fireDeviceUnitRemoved(Integer type, int unitNum)
   {
      DeviceUnitEvent evt = null;
      Object[] listeners = listenerList.getListenerList();

      for ( int i = listeners.length - 2; i >= 0; i -= 2 )
      {
         if ( listeners[i] == DeviceUnitListener.class )
         {
            if (evt == null)
            {
               evt = new DeviceUnitEvent(this, type, unitNum);
            }

            ((DeviceUnitListener) listeners[i + 1]).deviceUnitRemoved(evt);
         }
      }
   }

   private boolean mVariableUnitCount = false;
   private Map     mUnitTypeMap       = null;

   private EventListenerList listenerList = new EventListenerList();

   private Collection unitTypes = null;
}
