/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

package org.vrjuggler.vrjconfig.commoneditors.devicegraph.extras;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;

import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;
import org.vrjuggler.vrjconfig.commoneditors.devicegraph.AbstractDeviceUnitPropertyHandler;
import org.vrjuggler.vrjconfig.commoneditors.devicegraph.UnitConstants;


/**
 * A custom unit property handler specifically for config elements for
 * simulated analog devices.
 */
public class SimAnalogUnitPropertyHandler
   extends AbstractDeviceUnitPropertyHandler
   implements EditorConstants
{
   /**
    * Handles the addition of a new property value to our config element.
    * A <code>DeviceUnitEvent</code> event is fired if and only if the
    * increment and decrement keypress counts match.
    */
   public void propertyValueAdded(ConfigElementEvent evt)
   {
      if ( evt.getProperty().equals(DECREMENT_KEYPRESS_PROPERTY) ||
           evt.getProperty().equals(INCREMENT_KEYPRESS_PROPERTY) )
      {
         int dec_count =
            this.configElement.getPropertyValueCount(DECREMENT_KEYPRESS_PROPERTY);
         int inc_count =
            this.configElement.getPropertyValueCount(INCREMENT_KEYPRESS_PROPERTY);

         // This check is necessary to avoid firing two events for each unit
         // addition.
         if ( dec_count == inc_count )
         {
            fireDeviceUnitAdded(UnitConstants.ANALOG, inc_count - 1);
         }
      }
   }

   /**
    * There is nothing to do for property value changes.  We only care about
    * additions and removals.
    */
   public void propertyValueChanged(ConfigElementEvent evt)
   {
      /* Do nothing. */ ;
   }

   public void propertyValueOrderChanged(ConfigElementEvent evt)
   {
      System.err.println(
         "WARNING: SimAnalogUnitPropertyHandler.propertyValueOrderChanged() " +
         "is not implemented!  Complain about this."
      );
   }

   /**
    * Handles the removal of an existing property value from our config
    * element.  A <code>DeviceUnitEvent</code> event is fired if and only if
    * the increment and decrement keypress counts match.
    */
   public void propertyValueRemoved(ConfigElementEvent evt)
   {
      if ( evt.getProperty().equals(DECREMENT_KEYPRESS_PROPERTY) ||
           evt.getProperty().equals(INCREMENT_KEYPRESS_PROPERTY) )
      {
         int dec_count =
            this.configElement.getPropertyValueCount(DECREMENT_KEYPRESS_PROPERTY);
         int inc_count =
            this.configElement.getPropertyValueCount(INCREMENT_KEYPRESS_PROPERTY);

         // This check is necessary to avoid firing two events for each unit
         // removal.
         if ( dec_count == inc_count )
         {
            fireDeviceUnitRemoved(UnitConstants.ANALOG, evt.getIndex());
         }
      }
   }

   /**
    * Adds a new unit to the device by adding the necessary property values
    * to the config element.  No <code>DeviceUnitEvent</code> is fired here,
    * though.  We delay that until <code>propertyValueAdded</code> is invoked.
    */
   public void addUnit(Integer unitType)
   {
      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigDefinitionRepository repos = broker.getRepository();
      ConfigElementFactory factory =
         new ConfigElementFactory(repos.getAllLatest());

      int count = getUnitCount() + 1;
      ConfigDefinition key_pair_def = repos.get(KEY_MODIFIER_PAIR_TYPE);
      ConfigElement new_inc = factory.create("Increment keypress " + count,
                                             key_pair_def);
      ConfigElement new_dec = factory.create("Decrement keypress " + count,
                                             key_pair_def);

      this.configElement.addProperty(INCREMENT_KEYPRESS_PROPERTY, new_inc,
                                     this.configContext);
      this.configElement.addProperty(DECREMENT_KEYPRESS_PROPERTY, new_dec,
                                     this.configContext);
   }

   /**
    * Removes a new unit from the device by removing the appropriate property
    * values from the config element.  No <code>DeviceUnitEvent</code> is fired
    * here, though.  We delay that until <code>propertyValueRemoved</code> is
    * invoked.
    */
   public void removeUnit(Integer unitType, int unitNumber)
   {
      this.configElement.removeProperty(INCREMENT_KEYPRESS_PROPERTY,
                                        unitNumber);
      this.configElement.removeProperty(DECREMENT_KEYPRESS_PROPERTY,
                                        unitNumber);
   }

   private int getUnitCount()
   {
      return
         this.configElement.getPropertyValueCount(INCREMENT_KEYPRESS_PROPERTY);
   }
}
