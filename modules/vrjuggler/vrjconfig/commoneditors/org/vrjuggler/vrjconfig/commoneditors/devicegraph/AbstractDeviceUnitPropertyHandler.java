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

import java.util.Collection;
import java.util.Map;
import javax.swing.event.EventListenerList;

import org.vrjuggler.jccl.config.ConfigContext;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;

import org.vrjuggler.vrjconfig.commoneditors.event.DeviceUnitEvent;
import org.vrjuggler.vrjconfig.commoneditors.event.DeviceUnitListener;


/**
 * Partial implementation of <code>DeviceUnitPropertyHandler</code> that
 * deals with the aspects that would be common to all implementations of that
 * interface.
 */
public abstract class AbstractDeviceUnitPropertyHandler
   implements DeviceUnitPropertyHandler
{
   /**
    * Sets the configuration information for the configured device.
    */
   public void setConfig(ConfigContext ctx, ConfigElement elt)
   {
      this.configContext = ctx;
      this.configElement = elt;
   }

   /**
    * Sets the unit type map for the configured device.  The map must map
    * from <code>Integer</code> to some type, as defined in
    * <code>UnitConstants</code>.  The values of the map are dependent upon
    * the specific implementation of this interface.
    *
    * @see UnitConstants
    */
   public void setUnitTypeMap(Map unitTypeMap)
   {
      this.unitTypeMap = unitTypeMap;
      this.unitTypes   = unitTypeMap.keySet();
   }

   /**
    * Returns the unit types for this handler.  This is a collection of
    * <code>Integer</code> objects as defined in <code>UnitConstants</code>.
    *
    * @see UnitConstants
    */
   public Collection getUnitTypes()
   {
      return this.unitTypes;
   }

   /**
    * Invoked when a property value is added to the config element being
    * held by this handler.  Whether this results in a
    * <code>DeviceUnitEvent</code> event being generated is dependent upon the
    * specific implementation of this interface.
    */
   public abstract void propertyValueAdded(ConfigElementEvent evt);

   /**
    * Invoked when a proeprty value is changed in the config element being
    * held by this handler.  Whether this results in a
    * <code>DeviceUnitEvent</code> event being generated is dependent upon the
    * specific implementation of this interface.
    */
   public abstract void propertyValueChanged(ConfigElementEvent evt);

   /**
    * Invoked when a property value is removed from the config element being
    * held by this handler.  Whether this results in a
    * <code>DeviceUnitEvent</code> event being generated is dependent upon the
    * specific implementation of this interface.
    */
   public abstract void propertyValueRemoved(ConfigElementEvent evt);

   /**
    * Adds a new unit of the given type to the device managed by this handler.
    *
    * @param unitType   the type (as defined in <code>UnitConstants</code> of
    *                   the device unit to be added)
    *
    * @see UnitConstants
    */
   public abstract void addUnit(Integer unitType);

   /**
    * Removes the identified unit of the given type from the device managed by
    * this handler.
    *
    * @param unitType   the type (as defined in <code>UnitConstants</code> of
    *                   the device unit to be added)
    * @param untiNumber the index of the unit to remove
    *
    * @see UnitConstants
    */
   public abstract void removeUnit(Integer unitType, int unitNumber);

   /**
    * Adds the given <code>DeviceUnitListener</code> to the list of listeners.
    */
   public final void addDeviceUnitListener(DeviceUnitListener l)
   {
      listenerList.add(DeviceUnitListener.class, l);
   }

   /**
    * Removes the given <code>DeviceUnitListener</code> to from list of
    * listeners.
    */
   public final void removeDeviceUnitListener(DeviceUnitListener l)
   {
      listenerList.remove(DeviceUnitListener.class, l);
   }

   /**
    * Fires a <code>DeviceUnitEvent</code> for the addition of a unit of the
    * given type.
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.event.DeviceUnitEvent
    */
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

   /**
    * Fires a <code>DeviceUnitEvent</code> for the removal of the identified
    * unit of the given unit type.
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.event.DeviceUnitEvent
    */
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

   protected ConfigContext configContext = null;
   protected ConfigElement configElement = null;
   protected Map           unitTypeMap   = null;
   protected Collection    unitTypes     = null;

   private EventListenerList listenerList = new EventListenerList();
}
