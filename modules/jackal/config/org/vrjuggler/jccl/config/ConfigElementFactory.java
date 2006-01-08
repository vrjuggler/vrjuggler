/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
 *a
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
package org.vrjuggler.jccl.config;

import java.util.*;

/**
 * This class is responsible for creating new configuration elements within a
 * given configuration context.
 */
public class ConfigElementFactory
{
   /**
    * Creates a new configuration element factory that can create any element
    * defined by one of the configuration definitions in the given list.
    *
    * @param configDefs       the list of configuration definition from which
    *                         configuration element can be created
    */
   public ConfigElementFactory(List configDefs)
   {
      mConfigDefs = configDefs;
   }

   /**
    * Creates a new configuration element defined by the configuration
    * definition with the given token. The element is created with the default
    * values.
    *
    * @param name          the name for the new configuration element
    * @param token         the token for the configuration definition
    *
    * @throws IllegalArgumentException
    *             if the configuration definition is malformed or does not exist
    */
   public ConfigElement create(String name, String token)
      throws IllegalArgumentException
   {
      return create(name, getConfigDefinition(token));
   }

   /**
    * Creates a new configuration element defined by the given configuration
    * definition with the default values.
    *
    * @param name          the name for the new configuration element
    * @param def           the definition defining the contents of the element
    *
    * @throws IllegalArgumentException
    *             if the configuration definition is malformed
    */
   public ConfigElement create(String name, ConfigDefinition def)
      throws IllegalArgumentException
   {
      // Build up a map of the properties
      Map props = getDefaultProperties(def);

      // Create the configuration element
      return new ConfigElement(def, name, props);
   }

   public ConfigElement createUnique(ConfigDefinition def, ConfigContext ctx)
   {
      String base_name = def.getName().replaceAll(" ", "");
      int num = 0;
      while (ctx.containsElementNamed(base_name + Integer.toString(num)))
      {
         ++num;
      }
      
      ConfigElement elm = create(base_name + Integer.toString(num), def);
      return elm;
   }

   /**
    * Gets a map containing the default properties for the given configuration
    * definition. This can be used to create a new configuration element
    * containing default values.
    *
    * @param def           the definition containing the property definitions
    *
    * @throws IllegalArgumentException
    *             if the configuration definition is malformed
    */
   private Map getDefaultProperties(ConfigDefinition def)
      throws IllegalArgumentException
   {
      Map props = new TreeMap();

      // Go through each property definition in the config definition
      List prop_defs = def.getPropertyDefinitions();
      for (Iterator itr = prop_defs.iterator(); itr.hasNext(); )
      {
         // Get the default property values
         PropertyDefinition prop_def = (PropertyDefinition)itr.next();
         List prop_values = getDefaultPropertyValues(prop_def);

         // Add the property values into the property map
         props.put(prop_def.getToken(), prop_values);
      }

      return props;
   }

   /**
    * Gets a list containing the default values for the given property
    * definition.
    *
    * @param propDef       the property definition to examine
    *
    * @throws IllegalArgumentException
    *             if the property definition is malformed
    */
   private List getDefaultPropertyValues(PropertyDefinition propDef)
      throws IllegalArgumentException
   {
      Class prop_type = propDef.getType();
      List prop_values = new ArrayList();

      // Only add in values if the list is not variable
      if (! propDef.isVariable())
      {
         // Add in the default value for each value definition
         List prop_value_defs = propDef.getPropertyValueDefinitions();
         for (Iterator itr = prop_value_defs.iterator(); itr.hasNext(); )
         {
            PropertyValueDefinition prop_value_def = (PropertyValueDefinition)itr.next();


            // This property value should be a configuration element. Get the
            // appropriate definition and create the element from it.
            if (prop_type.equals(ConfigElement.class))
            {
               // Create a new configuration element from the first allowed type
               String allowed_type = propDef.getAllowedType(0);
               ConfigDefinition def = getConfigDefinition(allowed_type);

               // Build up a default name
               String name = propDef.getName() + " " + prop_values.size();
               prop_values.add(create(name, def));
            }
            // If the property is a ConfigElementPointer then we can not
            // use the default value since it will be the same reference
            // for all ConfigElement properties.
            else if (prop_type.equals(ConfigElementPointer.class))
            {
               prop_values.add(new ConfigElementPointer(""));
            }
            // If the property does not contain ConfigElements, create this
            // value normally.
            else
            {
               prop_values.add(prop_value_def.getDefaultValue());
            }
         }
      }

      return prop_values;
   }

   /**
    * Gets the configuration definition with the given token.
    *
    * @throws IllegalArgumentException
    *             if the configuration definition with the given token does not
    *             exist to the knowledge of this factory
    */
   private ConfigDefinition getConfigDefinition(String token)
      throws IllegalArgumentException
   {
      for (Iterator itr = mConfigDefs.iterator(); itr.hasNext(); )
      {
         ConfigDefinition def = (ConfigDefinition)itr.next();
         if (def.getToken().equals(token))
         {
            return def;
         }
      }

      throw new IllegalArgumentException("Invalid config definition: " + token);
   }

   /**
    * The list of configuration definitions from which this factory can create
    * new configuration elements.
    */
   private List mConfigDefs;
}
