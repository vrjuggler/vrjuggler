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
import java.util.Iterator;
import java.util.Map;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigElementEvent;

import org.vrjuggler.vrjconfig.commoneditors.event.DeviceUnitEvent;
import org.vrjuggler.vrjconfig.commoneditors.event.DeviceUnitListener;


/**
 * The baseline implementation of <code>DeviceUnitPropertyHandler</code>
 * that can deal with the "standard" cases of config elements for input
 * devices in Gadgeteer.  There are three such standard cases:
 *
 * <ol>
 *    <li>
 *       A fixed number of inherent input sources (units) of any combination
 *       of input types (such as a Trackd sensor, an Immersion Tech IBox, or
 *       a simulated positional device)
 *    </lI>
 *    <li>
 *       A device that has a variable number of units of any combination of
 *       input types that is specified by the config element for the device
 *       (such as VRPN or a simulated digital device)
 *    </lI>
 *    <li>
 *      A device that has a variable number of units of any combination of
 *      input types that are not specified by the config element but instead
 *      are auto-detected when the device driver communciates with the
 *      hardware (such as the Ascension Flock of Birds or the Direct Input
 *      game controller)
 *    </li>
 * </ol>
 *
 * For special cases, a different implementation of
 * <code>DeviceUnitPropertyHandler</code> would be needed.
 */
public class DefaultUnitPropertyHandler
   extends AbstractDeviceUnitPropertyHandler
{
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

   /**
    * Handles the addition of a new property value to our config element.
    * The changed property is comopared with internally stored information to
    * determine how to handle the unit addition.  If the device units are
    * managed using a variable valued property, then a
    * <code>DeviceUnitEvent</code> is fired to inform listeners of the
    * additional unit.
    */
   public void propertyValueAdded(ConfigElementEvent evt)
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

   /**
    * Handles the change in a property value in our config element.  The
    * changed property is comopared with internally stored information to
    * determine how to interpret the chagne.  If the device units are managed
    * using the value of a property, then a <code>DeviceUnitEvent</code> is
    * fired to inform listeners of the addition or removal of a unit,
    * depending on how the change is interpreted.
    */
   public void propertyValueChanged(ConfigElementEvent evt)
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

   public void propertyValueOrderChanged(ConfigElementEvent evt)
   {
   }

   /**
    * Handles the removal of a new property value from our config element.
    * The changed property is comopared with internally stored information to
    * determine how to handle the unit removal.  If the device units are
    * managed using a variable valued property, then a
    * <code>DeviceUnitEvent</code> is fired to inform listeners of the
    * removed unit.
    */
   public void propertyValueRemoved(ConfigElementEvent evt)
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

   /**
    * Adds a new unit of the given type to the device managed by this handler.
    * Depending on how the units are managed, a <code>DeviceUnitEvent</code>
    * is fired by this method or later by <code>propertyValueAdded()</code> or
    * by <code>propertyValueChanged()</code>.
    *
    * @param unitType   the type (as defined in <code>UnitConstants</code> of
    *                   the device unit to be added)
    *
    * @see UnitConstants
    */
   public void addUnit(Integer unitType)
   {
      Object unit_prop_obj = this.unitTypeMap.get(unitType);

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
                  int count = this.configElement.getPropertyValueCount(token);
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
            this.configElement.addProperty(token, default_value,
                                           this.configContext);
         }
         else
         {
            if ( prop_def.getType() == Integer.class )
            {
               Integer old_value =
                  (Integer) this.configElement.getProperty(prop_def.getToken(),
                                                           0);
               Integer new_value = new Integer(old_value.intValue() + 1);
               this.configElement.setProperty(prop_def.getToken(), 0,
                                              new_value, this.configContext);
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
         this.unitTypeMap.put(unitType, new Integer(cur_value + 1));
         fireDeviceUnitAdded(unitType, cur_value);
      }
   }

   /**
    * Removes the identified unit of the given type from the device managed by
    * this handler.  Depending on how the units are managed, a
    * <code>DeviceUnitEvent</code> is fired by this method or later by
    * <code>propertyValueRemoved()</code> or by
    * <code>propertyValueChanged()</code>.
    *
    * @param unitType   the type (as defined in <code>UnitConstants</code> of
    *                   the device unit to be added)
    * @param unitNumber the index of the unit to remove
    *
    * @see UnitConstants
    */
   public void removeUnit(Integer unitType, int unitNumber)
   {
      Object unit_prop_obj = this.unitTypeMap.get(unitType);

      if ( unit_prop_obj instanceof PropertyDefinition )
      {
         PropertyDefinition prop_def = (PropertyDefinition) unit_prop_obj;

         if ( prop_def.isVariable() )
         {
            this.configElement.removeProperty(prop_def.getToken(), unitNumber,
                                              this.configContext);
         }
         else
         {
            if ( prop_def.getType() == Integer.class )
            {
               Integer value =
                  (Integer) this.configElement.getProperty(prop_def.getToken(),
                                                           0);
               Integer new_value = new Integer(value.intValue() - 1);
               this.configElement.setProperty(prop_def.getToken(), 0,
                                              new_value, this.configContext);
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
         this.unitTypeMap.put(unitType, new Integer(unit_number));
         fireDeviceUnitRemoved(unitType, unit_number);
      }
   }

   private int getUnitCount(Integer unitType)
   {
      int count = 0;
      Object unit_prop_obj = this.unitTypeMap.get(unitType);

      if ( unit_prop_obj instanceof PropertyDefinition )
      {
         PropertyDefinition prop_def = (PropertyDefinition) unit_prop_obj;

         if ( prop_def.isVariable() )
         {
            count =
               this.configElement.getPropertyValueCount(prop_def.getToken());
         }
         else
         {
            Number val =
               (Number) this.configElement.getProperty(prop_def.getToken(), 0);
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

      Object unit_prop_obj = this.unitTypeMap.get(unitType);

      if ( unit_prop_obj instanceof PropertyDefinition )
      {
         result = ((PropertyDefinition) unit_prop_obj).getToken();
      }

      return result;
   }
}
