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

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigDefinition;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;
import org.vrjuggler.jccl.config.event.ConfigElementListener;

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
 */
public class DeviceInfo
   extends BaseDeviceInfo
   implements ConfigElementListener
{
   /**
    * Creates a new <code>DeviceInfo</code> object for a device that has
    * exactly one input source (unit) at all times.  The default device unit
    * handler delegate is created to manage the units.
    * 
    * @param devElt     the device config element
    * @param ctx        the config context that contains <code>devElt</code>
    *
    * @see DefaultUnitPropertyHandler
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
    * The default device unit handler delegate is created to manage the units.
    *
    * @param devElt             the device config element
    * @param ctx                the config context that contains
    *                           <code>devElt</code>
    * @param unitPropertyName   the name of the property that represents
    *                           the number of available input sources (units)
    *                           the device has (may be null)
    *
    * @see DefaultUnitPropertyHandler
    */
   public DeviceInfo(ConfigElement devElt, ConfigContext ctx,
                     String unitPropertyName)
   {
      super(devElt, ctx);

      devElt.addConfigElementListener(this);

      ConfigDefinition def = devElt.getDefinition();
      Integer unit_type = UnitTypeHelpers.getSingleUnitType(def);

      Map unit_type_map = new HashMap();

      // If unitPropertyName contains a non-empty string, then we have a device
      // that supports a variable number of units, each of the same input type.
      if ( unitPropertyName != null && ! unitPropertyName.equals("") )
      {
         unit_type_map.put(unit_type,
                           def.getPropertyDefinition(unitPropertyName));

         mVariableUnitCount = true;
      }
      // Otherwise, our device supports a fixed number of units (one or more),
      // each of the same input type.
      else
      {
         unit_type_map.put(unit_type, null);
         mVariableUnitCount = false;
      }

      initUnitPropHandler(ctx, devElt, unit_type_map);
   }

   /**
    * This version of the constructor should be used when the config element
    * has a <i>variable</i> number of input sources (units) <i>and</i> supports
    * multiipe input source types.  The default device unit handler delegate
    * is created to manage the units.
    *
    * @param devElt             the device config element
    * @param ctx                the config context that contains
    *                           <code>devElt</code>
    * @param unitTypeMap        a mapping from unit types (as defined in
    *                           <code>UnitConstants</code> to some value that
    *                           the default device unit property handler knows
    *                           how to manage
    *
    * @see UnitConstants
    * @see DefaultUnitPropertyHandler
    */
   public DeviceInfo(ConfigElement devElt, ConfigContext ctx,
                     Map unitTypeMap)
   {
      this(devElt, ctx, unitTypeMap, new DefaultUnitPropertyHandler());
   }

   /**
    * This version of the constructor should be used when the config element
    * has a <i>variable</i> number of input sources (units) <i>and</i> supports
    * multiipe input source types.  The given unit handler delegate is stored
    * to manage the device units.
    *
    * @param devElt             the device config element
    * @param ctx                the config context that contains
    *                           <code>devElt</code>
    * @param unitTypeMap        a mapping from unit types (as defined in
    *                           <code>UnitConstants</code> to some value that
    *                           the device unit property handler knows how to
    *                           manage
    * @param handler            the delegate for managing the device unit
    *                           properties
    *
    * @see UnitConstants
    */
   public DeviceInfo(ConfigElement devElt, ConfigContext ctx,
                     Map unitTypeMap, DeviceUnitPropertyHandler handler)
   {
      super(devElt, ctx);

      devElt.addConfigElementListener(this);

      mVariableUnitCount = true;

      initUnitPropHandler(ctx, devElt, unitTypeMap, handler);
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

      Map unit_type_map = new HashMap();
      for ( java.util.Iterator i = unitTypes.iterator(); i.hasNext(); )
      {
         unit_type_map.put(i.next(), null);
      }

      initUnitPropHandler(ctx, devElt, unit_type_map);
   }

   /**
    * Invoked when the name of our config element changes.
    */
   public void nameChanged(ConfigElementEvent evt)
   {
      /* Do nothing. */ ;
   }

   /**
    * Invoked when a property value is added to our config element.  The event
    * is handed off to the device unit delegate for this instance if the
    * device managed by this instance has a variable unit count.
    */
   public void propertyValueAdded(ConfigElementEvent evt)
   {
      if ( hasVariableUnitCount() )
      {
         this.unitPropHandler.propertyValueAdded(evt);
      }
   }

   /**
    * Invoked when a property value is chagned in our config element.  The
    * event is handed off to the device unit delegate for this instance if the
    * device managed by this instance has a variable unit count.
    */
   public void propertyValueChanged(ConfigElementEvent evt)
   {
      if ( hasVariableUnitCount() )
      {
         this.unitPropHandler.propertyValueChanged(evt);
      }
   }

   /**
    * Invoked when the property value order is chagned in our config element.
    * The event is handed off to the device unit delegate for this instance if
    * the device managed by this instance has a variable unit count.
    */
   public void propertyValueOrderChanged(ConfigElementEvent evt)
   {
      if ( hasVariableUnitCount() )
      {
         this.unitPropHandler.propertyValueOrderChanged(evt);
      }
   }

   /**
    * Invoked when a property value is removed from our config element.  The
    * event is handed off to the device unit delegate for this instance if the
    * device managed by this instance has a variable unit count.
    */
   public void propertyValueRemoved(ConfigElementEvent evt)
   {
      if ( hasVariableUnitCount() )
      {
         this.unitPropHandler.propertyValueRemoved(evt);
      }
   }

   /**
    * Adds the given <code>DeviceUnitListener</code> to the list of listeners.
    */
   public void addDeviceUnitListener(DeviceUnitListener l)
   {
      this.unitPropHandler.addDeviceUnitListener(l);
   }

   /**
    * Removes the given <code>DeviceUnitListener</code> to from list of
    * listeners.
    */
   public void removeDeviceUnitListener(DeviceUnitListener l)
   {
      this.unitPropHandler.removeDeviceUnitListener(l);
   }

   /**
    * Adds a new unit of the given type to our device.
    *
    * @param unitType   the type (as defined in <code>UnitConstants</code> of
    *                   the device unit to be added)
    *
    * @see UnitConstants
    */
   public void addUnit(Integer unitType)
      throws IllegalArgumentException
   {
      if ( ! hasVariableUnitCount() )
      {
         throw new IllegalArgumentException("Device element does not have " +
                                            "variable unit numbers");
      }

      this.unitPropHandler.addUnit(unitType);
   }

   /**
    * Removes the identified unit of the given type from our device.
    *
    * @param unitType   the type (as defined in <code>UnitConstants</code> of
    *                   the device unit to be added)
    * @param unitNumber the index of the unit to remove
    *
    * @see UnitConstants
    */
   public void removeUnit(Integer unitType, int unitNumber)
      throws IllegalArgumentException
   {
      if ( ! hasVariableUnitCount() )
      {
         throw new IllegalArgumentException("Device element does not have " +
                                            "variable unit numbers");
      }

      this.unitPropHandler.removeUnit(unitType, unitNumber);
   }

   /**
    * Removes the identified unit of the given type from our device.
    *
    * @param unitType   the type (as defined in <code>UnitConstants</code> of
    *                   the device unit to be added)
    * @param unitNumber the index of the unit to remove
    *
    * @see UnitConstants
    */
   public void removeUnit(Integer unitType, Integer unitNumber)
   {
      removeUnit(unitType, unitNumber.intValue());
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
      return this.unitPropHandler.getUnitTypes();
   }

   private void initUnitPropHandler(ConfigContext ctx, ConfigElement elt,
                                    Map unitTypeMap)
   {
      initUnitPropHandler(ctx, elt, unitTypeMap,
                          new DefaultUnitPropertyHandler());
   }

   private void initUnitPropHandler(ConfigContext ctx, ConfigElement elt,
                                    Map unitTypeMap,
                                    DeviceUnitPropertyHandler handler)
   {
      this.unitPropHandler = handler;
      this.unitPropHandler.setConfig(ctx, elt);
      this.unitPropHandler.setUnitTypeMap(unitTypeMap);
   }

   private boolean mVariableUnitCount = false;

   private DeviceUnitPropertyHandler unitPropHandler = null;
}
