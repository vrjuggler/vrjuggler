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
 * This object knows how to create a configuration definition (all versions of
 * it) from an XML document.
 *
 * This parser works on files formatted to the version 1.0 specification.
 */
public class ConfigDefinitionParser
   implements org.vrjuggler.jccl.config.parser.ConfigDefinitionParser
{
   public ConfigDefinitionParser()
   {}

   /**
    * Parses the given XML document for all versions of the configuration
    * definition defined therein.
    *
    * @param doc           the XML document to parse
    */
   public List parse(Document doc)
      throws ParseException
   {
      return parseDefinition(doc.getRootElement());
   }

   private List parseDefinition(Element root)
      throws ParseException
   {
      // Verify that the root element is a definition
      if (!root.getName().equals(DEFINITION))
      {
         throw new ParseException("Root element must be: "+DEFINITION);
      }

      // Verify that this definition has a name
      String token = root.getAttributeValue(NAME);
      if (token == null)
      {
         throw new MissingAttributeException(NAME);
      }

      List defs = new ArrayList();
      // Parse the various versions of this definition
      for (Iterator itr = root.getChildren(DEFINITION_VERSION).iterator(); itr.hasNext(); )
      {
         // Parse the given definition version
         Element elt = (Element)itr.next();
         defs.add(parseDefinitionVersion(elt, token));
      }

      return defs;
   }

   private ConfigDefinition parseDefinitionVersion(Element root, String name)
      throws ParseException
   {
      // Verify that the root element is a definition version
      if (!root.getName().equals(DEFINITION_VERSION))
      {
         throw new ParseException("Expected '"+DEFINITION_VERSION+"'. Got '"+root.getName()+"'");
      }

      // Get the version attribute
      String version_str = root.getAttributeValue(VERSION);
      if (version_str == null)
      {
         throw new MissingAttributeException(VERSION);
      }
      // Convert the version string to an integer
      int version;
      try
      {
         version = Integer.parseInt(version_str);
      }
      catch (NumberFormatException nfe)
      {
         throw new ParseException("Version is not an integer: "+version_str);
      }

      // Get the label attribute
      String label = root.getAttributeValue(LABEL);
      if (label == null)
      {
         throw new MissingAttributeException(LABEL);
      }

      // Grok the help
      String help = root.getChildTextTrim(HELP);

      // Grok the categories
      List categories = new ArrayList();
      for (Iterator itr = root.getChildren(CATEGORY).iterator(); itr.hasNext(); )
      {
         Element elt = (Element)itr.next();
         String category = elt.getTextTrim();
         // Ignore empty categories
         if (category.length() > 0)
         {
            categories.add(category);
         }
      }

      // Grok the property definitions
      List prop_defs = new ArrayList();
      for (Iterator itr = root.getChildren(PROPERTY).iterator(); itr.hasNext(); )
      {
         // Parse the given property definition
         Element elt = (Element)itr.next();
         prop_defs.add(parsePropertyDefinition(elt));
      }

      // Create the new configuration definition
      return new ConfigDefinition(label,
                                  name,
                                  version,
                                  help,
                                  categories,
                                  prop_defs);
   }

   private PropertyDefinition parsePropertyDefinition(Element root)
      throws ParseException
   {
      // Verify that the root element is a property definition
      if (!root.getName().equals(PROPERTY))
      {
         throw new ParseException("Expected '"+PROPERTY+"'. Got '"+root.getName()+"'");
      }

      // Get the valuetype
      String valuetype_str = root.getAttributeValue(VALUETYPE);
      if (valuetype_str == null)
      {
         throw new MissingAttributeException(VALUETYPE);
      }
      Class valuetype = getValueTypeClass(valuetype_str);

      // Get the name
      String name = root.getAttributeValue(NAME);
      if (name == null)
      {
         throw new MissingAttributeException(NAME);
      }

      // Get the variable flag
      String variable_str = root.getAttributeValue(VARIABLE);
      if (variable_str == null)
      {
         throw new MissingAttributeException(VARIABLE);
      }
      // Get the boolean value of the flag
      boolean variable;
      try
      {
         variable = new Boolean(variable_str).booleanValue();
      }
      catch (NumberFormatException nfe)
      {
         throw new ParseException("Variable is not a boolean: "+variable_str);
      }

      // Get the help text
      String help = root.getChildTextTrim(HELP);

      // Handle the value list
      List values = new ArrayList();
      for (Iterator itr = root.getChildren(VALUE).iterator(); itr.hasNext(); )
      {
         Element elt = (Element)itr.next();
         values.add(parsePropertyValue(elt, valuetype));
      }

      // Get the allowed types
      List allowed_types = new ArrayList();
      for (Iterator itr = root.getChildren(ALLOWED_TYPE).iterator(); itr.hasNext(); )
      {
         Element elt = (Element)itr.next();
         allowed_types.add(elt.getTextTrim());
      }

      // Create the new property definition
      return new PropertyDefinition(name,
                                    name,
                                    valuetype,
                                    help,
                                    values,
                                    new TreeMap(),
                                    allowed_types,
                                    variable);
   }

   private PropertyValueDefinition parsePropertyValue(Element root, Class valuetype)
      throws ParseException
   {
      // Verify that the root element is a property value definition
      if (!root.getName().equals(VALUE))
      {
         throw new ParseException("Expected '"+VALUE+"'. Got '"+root.getName()+"'");
      }

      // Get the label
      String label = root.getAttributeValue(LABEL);
      if (label == null)
      {
         throw new MissingAttributeException(LABEL);
      }

      // Get the default value
      String defaultvalue_str = root.getAttributeValue(DEFAULTVALUE);
      if (defaultvalue_str == null && valuetype != ConfigElement.class)
      {
         throw new MissingAttributeException(DEFAULTVALUE);
      }
      // Convert the object to the correct type
      Object defaultvalue = convertTo(defaultvalue_str, valuetype);

      if (defaultvalue == null)
      {
         return new PropertyValueDefinition(label);
      }
      else
      {
         return new PropertyValueDefinition(label, defaultvalue);
      }
   }

   /**
    * Given the string name for a valuetype, this method will return the
    * appropriate Java class that represents it.
    */
   private Class getValueTypeClass(String valuetype)
      throws ParseException
   {
      if (valuetype.equals("boolean"))
      {
         return Boolean.class;
      }
      else if (valuetype.equals("integer"))
      {
         return Integer.class;
      }
      else if (valuetype.equals("float"))
      {
         return Float.class;
      }
      else if (valuetype.equals("string"))
      {
         return String.class;
      }
      else if (valuetype.equals("configelement"))
      {
         return ConfigElement.class;
      }
      else if (valuetype.equals("configelementpointer"))
      {
//         return ConfigElementPointer.class;
         throw new RuntimeException("Not implemented yet");
      }
      else
      {
         throw new ParseException("Unknown valuetype: "+valuetype);
      }
   }

   /**
    * Converts the string representation of the value to the correct object
    * type.
    */
   private Object convertTo(String valueStr, Class type)
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
         else if (type == ConfigElement.class)
         {
            return null;
         }
         else
         {
            throw new ParseException("Unknown valuetype: "+type);
         }
      }
      catch (NumberFormatException nfe)
      {
         throw new ParseException("Invalid default value: "+valueStr);
      }
   }

   private static final String ALLOWED_TYPE           = "allowed_type";
   private static final String CATEGORY               = "category";
   private static final String DEFAULTVALUE           = "defaultvalue";
   private static final String DEFINITION             = "definition";
   private static final String DEFINITION_VERSION     = "definition_version";
   private static final String ENUM                   = "enum";
   private static final String HELP                   = "help";
   private static final String LABEL                  = "label";
   private static final String NAME                   = "name";
   private static final String PARENT                 = "parent";
   private static final String PROPERTY               = "property";
   private static final String VALUE                  = "value";
   private static final String VALUETYPE              = "valuetype";
   private static final String VARIABLE               = "variable";
   private static final String VERSION                = "version";
}
