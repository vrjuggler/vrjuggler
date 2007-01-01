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

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.vrjuggler.jccl.config.ConfigDefinition;
import org.vrjuggler.vrjconfig.commoneditors.EditorConstants;


public class UnitTypeHelpers
   implements UnitConstants
{
   private static Map mUnitTypeMap = new HashMap();

   static
   {
      mUnitTypeMap.put(EditorConstants.ANALOG_DEVICE_TYPE, ANALOG);
      mUnitTypeMap.put(EditorConstants.COMMAND_DEVICE_TYPE, COMMAND);
      mUnitTypeMap.put(EditorConstants.DIGITAL_DEVICE_TYPE, DIGITAL);
      mUnitTypeMap.put(EditorConstants.GLOVE_DEVICE_TYPE, GLOVE);
      mUnitTypeMap.put(EditorConstants.KEYBOARD_MOUSE_DEVICE_TYPE,
                       KEYBOARD_MOUSE);
      mUnitTypeMap.put(EditorConstants.POSITIONAL_DEVICE_TYPE, POSITION);
      mUnitTypeMap.put(EditorConstants.STRING_DEVICE_TYPE, STRING);
   }

   public static Integer getSingleUnitType(ConfigDefinition def)
   {
      for ( Iterator i = mUnitTypeMap.keySet().iterator(); i.hasNext(); )
      {
         String key = (String) i.next();
         if ( def.isOfType(key) )
         {
            return (Integer) mUnitTypeMap.get(key);
         }
      }

      throw new IllegalArgumentException("Definition " + def.getToken() +
                                         " is not a known device type");
   }

   public static List getMultiUnitType(ConfigDefinition def)
   {
      List types = new ArrayList();
      for ( Iterator i = mUnitTypeMap.keySet().iterator(); i.hasNext(); )
      {
         String key = (String) i.next();
         if ( def.isOfType(key) )
         {
            types.add(mUnitTypeMap.get(key));
         }
      }

      if ( types.isEmpty() )
      {
         throw new IllegalArgumentException("Definition " + def.getToken() +
                                            " is not a known device type");
      }

      return types;
   }

   /**
    * Returns the corresponding unit type constant for the given "type token"
    * string.  The type token must have been registered internally with this
    * class.  The collection of registered type tokens is the set of input
    * device types known to Gadgeteeer and defined as string constants in
    * <code>org.vrjuggler.vrjconfig.commoneditors.EditorConstants</code>.
    *
    * @param typeToken  the config definition token for the Gadgeteer input
    *                   device type of interest
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.EditorConstants
    */
   public static Integer getUnitType(String typeToken)
   {
      for ( Iterator i = mUnitTypeMap.keySet().iterator(); i.hasNext(); )
      {
         String key = (String) i.next();
         if ( typeToken.equals(key) )
         {
            return (Integer) mUnitTypeMap.get(key);
         }
      }

      throw new IllegalArgumentException("Type " + typeToken +
                                         " is not a known device type");
   }

   /**
    * Returns a human-readable name for the given unit type value.  The name
    * returned is capitalized using English title conventions.  Calling code
    * can easily convert this form to all uppper case or all lower case as
    * necessary.
    */
   public static String getUnitTypeName(Integer unitType)
   {
      String type_str = "Unknown";

      if ( unitType.equals(ANALOG) )
      {
         type_str = "Analog";
      }
      else if ( unitType.equals(COMMAND) )
      {
         type_str = "Command";
      }
      else if ( unitType.equals(DIGITAL) )
      {
         type_str = "Digital";
      }
      else if ( unitType.equals(GLOVE) )
      {
         type_str = "Glove";
      }
      else if ( unitType.equals(KEYBOARD_MOUSE) )
      {
         type_str = "Keyboard/Mouse";
      }
      else if ( unitType.equals(POSITION) )
      {
         type_str = "Position";
      }
      else if ( unitType.equals(STRING) )
      {
         type_str = "String";
      }

      return type_str;
   }
}
