/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
package org.vrjuggler.jccl.config.parser.v1_0;

import java.util.*;
import org.jdom.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.parser.ParseException;
import org.vrjuggler.jccl.config.parser.MissingAttributeException;

/**
 * This object knows how to create a configuration element from the node in a
 * DOM tree that describes it.
 *
 * This parser works on files formatted to the version 1.0 specification.
 */
public class ConfigElementParser
{
   /**
    * Creates a new parser for configuration element XML nodes in which the
    * given configuration definition repository is used to lookup the required
    * configuration definitions.
    *
    * @param repos         the repository in which to lookup config definitions
    */
   public ConfigElementParser(ConfigDefinitionRepository repos)
   {
      mDefinitionRepos = repos;
   }

   /**
    * Parses the given XML DOM element for the configuration element defined
    * therein.
    *
    * @param root          the XML DOM element to parse
    */
   public ConfigElement parse(Element root)
      throws ParseException
   {
      // Get the name of the definition this element depends on
      String token = root.getName();

      // Get the definition version this element depends on
      String version_str = root.getAttributeValue(VERSION);
      if (version_str == null)
      {
         throw new MissingAttributeException(VERSION);
      }
      int version;
      try
      {
         version = Integer.parseInt(version_str);
      }
      catch (NumberFormatException nfe)
      {
         throw new ParseException("Version string is not a number: "+version_str);
      }

      // Get the name of this configuration element
      String name = root.getAttributeValue(NAME);
      if (name == null)
      {
         throw new MissingAttributeException(NAME);
      }

      // Get the definition for this configuration element
      ConfigDefinition def = mDefinitionRepos.get(token, version);
      if (def == null)
      {
         throw new ParseException("Could not find definition: "+token+" version "+version);
      }

      // Get all the properties for this element
      Map props = parseProperties(root, def);

      // Create the configuration element
      return new ConfigElement(def, name, props);
   }

   private Map parseProperties(Element root, ConfigDefinition def)
      throws ParseException
   {
      Map props = new TreeMap();

      // Process each property defined in the configuration definition
      List prop_defs = def.getPropertyDefinitions();
      for (Iterator itr = prop_defs.iterator(); itr.hasNext(); )
      {
         PropertyDefinition prop_def = (PropertyDefinition)itr.next();
         String prop_token = prop_def.getToken();

         // Process the property values for this property
         props.put(prop_token,
                   parsePropertyValues(root.getChildren(prop_token), prop_def));
      }

      return props;
   }

   private List parsePropertyValues(List elements, PropertyDefinition propDef)
      throws ParseException
   {
      List values = new ArrayList();

      // Make sure that a property that is not variable has a number of values
      // equals to those in the definition
      boolean variable = propDef.isVariable();
      if (!variable && (elements.size() != propDef.getPropertyValueDefinitionCount()))
      {
         throw new ParseException("Invalid number of property values for the property "+propDef.getToken());
      }

      // Go through each property value XML element and parse it
      for (Iterator itr = elements.iterator(); itr.hasNext(); )
      {
         Element elt = (Element)itr.next();
         values.add(parsePropertyValue(elt, propDef));
      }

      return values;
   }

   private Object parsePropertyValue(Element elt, PropertyDefinition propDef)
      throws ParseException
   {
      String valueStr = elt.getTextTrim();

      // If the value matches an enumeration, return the corresponding value
      if (propDef.getEnums().containsKey(valueStr))
      {
         return propDef.getEnums().get(valueStr);
      }

      // If the property type is not a configuration element, convert it to
      // that value type.
      if (propDef.getType() != ConfigElement.class)
      {
         return convertValue(valueStr, propDef.getType());
      }

      // The property type must be a configuration element, so recurse on it
      Element config_element_node = (Element)elt.getChildren().get(0);
      return parse(config_element_node);
   }

   private Object convertValue(String valueStr, Class type)
      throws ParseException
   {
      try
      {
         if (type == Boolean.class)
         {
            return new Boolean(valueStr);
         }
         else if (type == Integer.class)
         {
            return new Integer(valueStr);
         }
         else if (type == Float.class)
         {
            return new Float(valueStr);
         }
         else if (type == String.class)
         {
            return valueStr;
         }
         else
         {
            throw new ParseException("Unknown valuetype: "+type);
         }
      }
      catch (NumberFormatException nfe)
      {
         throw new ParseException("Invalid value: "+valueStr);
      }
   }

   private static final String CONFIGURATION_ELEMENTS = "configuration_elements";
   private static final String NAME                   = "name";
   private static final String VERSION                = "version";

   private ConfigDefinitionRepository mDefinitionRepos;
}
