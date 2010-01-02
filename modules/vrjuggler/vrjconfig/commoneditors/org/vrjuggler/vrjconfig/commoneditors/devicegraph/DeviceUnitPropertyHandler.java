/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;

import org.vrjuggler.vrjconfig.commoneditors.event.DeviceUnitListener;


/**
 * The interface for delegates used by <code>DeviceInfo</code> for managing
 * the device units of a configured device.
 *
 * <p>
 * User code should not invoke the methods of this class directly.  Instead,
 * instances of this interface must be handed off to new instances of
 * <code>DeviceInfo</code>.  Such objects will handle the use of this
 * interface to manage the device units.
 * </p>
 *
 * @see DeviceInfo
 */
public interface DeviceUnitPropertyHandler
{
   /**
    * Sets the configuration information for the configured device.
    */
   public void setConfig(ConfigContext ctx, ConfigElement elt);

   /**
    * Sets the unit type map for the configured device.  The map must map
    * from <code>Integer</code> to some type, as defined in
    * <code>UnitConstants</code>.  The values of the map are dependent upon
    * the specific implementation of this interface.
    *
    * @see UnitConstants
    */
   public void setUnitTypeMap(java.util.Map unitTypeMap);

   /**
    * Returns the unit types for this handler.  This is a collection of
    * <code>Integer</code> objects as defined in <code>UnitConstants</code>.
    *
    * @see UnitConstants
    */
   public java.util.Collection getUnitTypes();

   /**
    * Invoked when a property value is added to the config element being
    * held by this handler.  Whether this results in a
    * <code>DeviceUnitEvent</code> event being generated is dependent upon the
    * specific implementation of this interface.
    */
   public void propertyValueAdded(ConfigElementEvent evt);

   /**
    * Invoked when a property value is changed in the config element being
    * held by this handler.  Whether this results in a
    * <code>DeviceUnitEvent</code> event being generated is dependent upon the
    * specific implementation of this interface.
    */
   public void propertyValueChanged(ConfigElementEvent evt);

   /**
    * Invoked when the order of property values is changed in the config
    * element being held by this handler.  Whether this results in a
    * <code>DeviceUnitEvent</code> event being generated is dependent upon the
    * specific implementation of this interface.
    */
   public void propertyValueOrderChanged(ConfigElementEvent evt);

   /**
    * Invoked when a property value is removed from the config element being
    * held by this handler.  Whether this results in a
    * <code>DeviceUnitEvent</code> event being generated is dependent upon the
    * specific implementation of this interface.
    */
   public void propertyValueRemoved(ConfigElementEvent evt);

   /**
    * Adds a new unit of the given type to the device managed by this handler.
    *
    * @param unitType   the type (as defined in <code>UnitConstants</code> of
    *                   the device unit to be added)
    *
    * @see UnitConstants
    */
   public void addUnit(Integer unitType);
 
   /**
    * Removes the identified unit of the given type from the device managed by
    * this handler.
    *
    * @param unitType   the type (as defined in <code>UnitConstants</code> of
    *                   the device unit to be added)
    * @param unitNumber the index of the unit to remove
    *
    * @see UnitConstants
    */
   public void removeUnit(Integer unitType, int unitNumber);

   /**
    * Adds the given <code>DeviceUnitListener</code> to the list of listeners.
    */
   public void addDeviceUnitListener(DeviceUnitListener l);

   /**
    * Removes the given <code>DeviceUnitListener</code> to from list of
    * listeners.
    */
   public void removeDeviceUnitListener(DeviceUnitListener l);
}
