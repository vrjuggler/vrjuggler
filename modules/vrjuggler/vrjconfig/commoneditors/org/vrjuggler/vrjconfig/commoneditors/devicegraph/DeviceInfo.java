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

import org.vrjuggler.jccl.config.*;

import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;


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

      mVariableUnitCount = false;
      this.unitTypes     = unitTypes;

      mUnitTypeMap = new HashMap();
      for ( java.util.Iterator i = this.unitTypes.iterator(); i.hasNext(); )
      {
         mUnitTypeMap.put(i.next(), null);
      }
   }

   public void addUnit(Integer unitType)
      throws IllegalArgumentException
   {
      if ( ! hasVariableUnitCount() )
      {
         throw new IllegalArgumentException("Device element does not have " +
                                            "variable unit numbers");
      }

      PropertyDefinition prop_def = getUnitPropertyDefinition(unitType);

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

   public void removeUnit(Integer unitType, int unitNumber)
      throws IllegalArgumentException
   {
      if ( ! hasVariableUnitCount() )
      {
         throw new IllegalArgumentException("Device element does not have " +
                                            "variable unit numbers");
      }

      PropertyDefinition prop_def = getUnitPropertyDefinition(unitType);
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

   public int getUnitCount(Integer unitType)
   {
      int count = 0;
      PropertyDefinition prop_def = getUnitPropertyDefinition(unitType);

      if ( prop_def != null )
      {
         if ( prop_def.isVariable() )
         {
            count = getElement().getPropertyValueCount(prop_def.getToken());
         }
         else
         {
            Number val =
               (Number) getElement().getProperty(prop_def.getToken(), 0);
            count = val.intValue();
         }
      }

      return count;
   }

   public int getUnitCount(String propToken)
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

   public String getUnitPropertyToken(Integer unitType)
   {
      String result = null;
      PropertyDefinition prop_def = getUnitPropertyDefinition(unitType);

      if ( prop_def != null )
      {
         result = prop_def.getToken();
      }

      return result;
   }

   public Class getUnitPropertyType(Integer unitType)
   {
      Class result = null;
      PropertyDefinition prop_def = getUnitPropertyDefinition(unitType);

      if ( prop_def != null )
      {
         result = prop_def.getType();
      }

      return result;
   }

   /**
    * Retrieves the definition for the property in this device's config
    * element that provides an indication of how many input sources (units)
    * this device has.  To make a reliable determination about whether the
    * device has a variable number of input sources, use
    * <a href="#hasVariableUnitCount()"><code>hasVariableUnitCount()</code></a>.
    * Invoking this method when the device does not support a variable number
    * of input sources will generally result in a
    * <code>NullPointerException</code> being thrown.
    *
    * @param inputType  the input type of the property definition to return
    *
    * @return a <code>PropertyDefinition</code> reference for this device's
    *         input source (unit) configuration or null if this device has
    *         a fixed number of input sources (greater than or equal to 1)
    *         at all times.
    *
    * @see UnitConstants
    * @see #hasVariableUnitCount()
    * @see #getUnitTypes()
    */
   private PropertyDefinition getUnitPropertyDefinition(Integer inputType)
   {
      return getUnitPropertyDefinition((Object) inputType);
   }

   /**
    * Retrieves the definition for the property in this device's config
    * element that provides an indication of how many input sources (units)
    * this device has.  To make a reliable determination about whether the
    * this device has.  To make a reliable determination about whether the
    * device has a variable number of input sources, use
    * <a href="#hasVariableUnitCount()"><code>hasVariableUnitCount()</code></a>.
    * Invoking this method when the device does not support a variable number
    * of input sources will generally result in a
    * <code>NullPointerException</code> being thrown.
    *
    * <p>
    * This method is a convenience overload of <code>getUnitPropertyDefinition(Integer)</code> that
    * can be useful when iterating over the collection of supported unit
    * types.  Using this version of the method relieves the caller from having
    * to cast the collection item from <code>java.lang.Object</code> to
    * <code>java.lang.Integer</code>.
    * </p>
    *
    * @param inputType  the input type of the property definition to return
    *
    * @return a <code>PropertyDefinition</code> reference for this device's
    *         input source (unit) configuration or null if this device has
    *         a fixed number of input sources (greater than or equal to 1)
    *         at all times.
    *
    * @see #hasVariableUnitCount()
    * @see #getUnitTypes()
    */
   private PropertyDefinition getUnitPropertyDefinition(Object inputType)
   {
      return (PropertyDefinition) mUnitTypeMap.get(inputType);
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

   private boolean mVariableUnitCount = false;
   private Map     mUnitTypeMap       = null;

   private Collection unitTypes = null;
}
