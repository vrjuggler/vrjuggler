/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.jccl.config.io;

import java.io.*;
import java.util.*;
import org.jdom.*;
import org.jdom.filter.*;
import org.jdom.output.Format;
import org.jdom.output.XMLOutputter;
import org.vrjuggler.jccl.config.*;

/**
 * This object knows how to write a configuration definition (all versions of
 * it) to a stream.
 */
public class ConfigDefinitionWriter
   extends BufferedWriter
   implements ConfigIOConstants
            , ConfigDefinitionConstants
{
   /**
    * Creates a new configuration definition writer that will write definitions
    * to the given file. This is a shortcut for:
    *
    * reader = new ConfigDefinitionWriter(new FileWriter(file));
    */
   public ConfigDefinitionWriter(File file)
      throws IOException
   {
      this(new FileWriter(file));
   }

   /**
    * Creates a new configuration definition writer that will write definitions
    * to the given writer stream.
    */
   public ConfigDefinitionWriter(Writer writer)
   {
      super(writer);
   }

   /**
    * Writes all versions of the same configuration definition in the given list
    * of definitions to the XML document stream.
    */
   public void writeDefinition(List versions)
      throws IOException
   {
      // Sort all the versions from newest to oldest so that they get written
      // out in that order.
      Collections.sort(versions, new Comparator()
      {
         public int compare(Object o1, Object o2)
         {
            ConfigDefinition cd1 = (ConfigDefinition)o1;
            ConfigDefinition cd2 = (ConfigDefinition)o2;

            if (cd1.getVersion() < cd2.getVersion())
            {
               return -1;
            }
            else if (cd1.getVersion() > cd2.getVersion())
            {
               return 1;
            }
            return 0;
         }
      });

      // Create the XML document
      String name = ((ConfigDefinition)versions.get(0)).getToken();
      String icon_path = ((ConfigDefinition)versions.get(0)).getIconLocation();

      Document doc = createDocument(name, icon_path);

      // Add each definition version
      for (Iterator itr = versions.iterator(); itr.hasNext(); )
      {
         ConfigDefinition def = (ConfigDefinition)itr.next();
         addDefinition(def, doc.getRootElement());
      }

      // Write the document out to the stream
      Format f = Format.getPrettyFormat();
      f.setIndent("   ");
      f.setLineSeparator(System.getProperty("line.separator"));
      f.setTextMode(Format.TextMode.NORMALIZE);
      XMLOutputter out = new XMLOutputter(f);
      out.output(doc, this);
   }

   /**
    * Creates a new XML document for definitions with the given name.
    */
   private Document createDocument(String name, String icon_path)
   {
      // Create the document
      Document doc = new Document();

      // Insert the PI with the file format version
      ProcessingInstruction pi = new ProcessingInstruction(SETTINGS_INSTRUCTION,
                                                           DEF_VERSION_ATTR + "=" + "\"" + DEF_VERSION + "\"");
      doc.addContent(pi);

      // Create the definitions root element
      Element root = new Element(DEFINITION, DEF_NS);
      root.setAttribute(NAME, name);
      root.setAttribute(ICON_PATH, icon_path);
      root.setAttribute(SCHEMA_LOCATION, DEF_NS_str + " " + DEF_SCHEMA, XSI_NS);
      doc.setRootElement(root);

      return doc;
   }

   /**
    * Adds the given configuration definition to the given definition root
    * element.
    */
   private void addDefinition(ConfigDefinition def, Element root)
   {
      // Create the definition version element
      Element def_elt = new Element(DEFINITION_VERSION, DEF_NS);
      def_elt.setAttribute(VERSION, Integer.toString(def.getVersion()));
      def_elt.setAttribute(LABEL, def.getName());

      // Add the is_abstract child
      Element is_abstract = new Element(ABSTRACT, DEF_NS);
      is_abstract.setText(Boolean.toString(def.isAbstract()));
      def_elt.addContent(is_abstract);

      // Add the help child
      Element help = new Element(HELP, DEF_NS);
      help.setText(def.getHelp());
      def_elt.addContent(help);

      // Add the parents
      for (Iterator itr = def.getParents().iterator(); itr.hasNext(); )
      {
         Element parent = new Element(PARENT, DEF_NS);
         parent.setText((String)itr.next());
         def_elt.addContent(parent);
      }

      // Add the categories
      for (Iterator itr = def.getCategories().iterator(); itr.hasNext(); )
      {
         Category cat = (Category)itr.next();
         Element category = new Element(CATEGORY, DEF_NS);
         category.setText(cat.toString());
         def_elt.addContent(category);
      }

      // Add the properties
      for (Iterator itr = def.getPropertyDefinitions().iterator(); itr.hasNext(); )
      {
         PropertyDefinition prop_def = (PropertyDefinition)itr.next();
         Element prop_def_elt = createPropertyElement(prop_def);
         def_elt.addContent(prop_def_elt);
      }

      // Add the upgrade transform
      Element upgrade_xform = new Element(UPGRADE_TRANSFORM, DEF_NS);
      
      if(null != def.getXsltElement())
      {
         upgrade_xform.addContent(def.getXsltElement());
      }
      
      def_elt.addContent(upgrade_xform);

      // Add the definition version to the root element
      root.addContent(def_elt);
   }

   /**
    * Creates an XML element for a property definition.
    */
   private Element createPropertyElement(PropertyDefinition propDef)
   {
      // Create the property element
      Element property_elt = new Element(PROPERTY, DEF_NS);
      property_elt.setAttribute(VALUETYPE, getValuetypeString(propDef.getType()));
      property_elt.setAttribute(VARIABLE, (new Boolean(propDef.isVariable())).toString());
      property_elt.setAttribute(NAME, propDef.getToken());

      // Create the help element
      Element help_elt = new Element(HELP, DEF_NS);
      help_elt.setText(propDef.getHelp());
      property_elt.addContent(help_elt);

      // Create the value elements
      for (Iterator itr = propDef.getPropertyValueDefinitions().iterator(); itr.hasNext(); )
      {
         PropertyValueDefinition value = (PropertyValueDefinition)itr.next();
         Element value_elt = new Element(VALUE, DEF_NS);
         value_elt.setAttribute(LABEL, value.getLabel());

         // Only add the default value attribute for simple types
         if (propDef.getType() != ConfigElement.class &&
             propDef.getType() != ConfigElementPointer.class)
         {
            value_elt.setAttribute(DEFAULTVALUE, value.getDefaultValue().toString());
         }

         property_elt.addContent(value_elt);
      }

      // Create the allowed type elements
      for (Iterator itr = propDef.getAllowedTypes().iterator(); itr.hasNext(); )
      {
         String allowed_type = (String)itr.next();
         Element allowed_type_elt = new Element(ALLOWED_TYPE, DEF_NS);
         allowed_type_elt.setText(allowed_type);
         property_elt.addContent(allowed_type_elt);
      }

      // Create the enumeration elements
      Map enums = propDef.getEnums();
      if ( enums.keySet().size() > 0 )
      {
         Element enum_parent = new Element(ENUMERATION, DEF_NS);
         enum_parent.setAttribute(EDITABLE,
                                  String.valueOf(propDef.enumIsEditable()));

         for (Iterator itr = enums.keySet().iterator(); itr.hasNext();)
         {
            String enum_name = (String)itr.next();
            Element enum_elt = new Element(ENUM_VALUE, DEF_NS);
            enum_elt.setAttribute(LABEL, enum_name);
            enum_elt.setAttribute(VALUE, enums.get(enum_name).toString());
            enum_parent.addContent(enum_elt);
         }

         property_elt.addContent(enum_parent);
      }

      return property_elt;
   }

   /**
    * Gets the string representation of the given valuetype.
    */
   private String getValuetypeString(Class type)
   {
      if (type == Boolean.class)
      {
         return BOOLEAN;
      }
      else if (type == Integer.class)
      {
         return INTEGER;
      }
      else if (type == Float.class)
      {
         return FLOAT;
      }
      else if (type == String.class)
      {
         return STRING;
      }
      else if (type == ConfigElement.class)
      {
         return CONFIG_ELEMENT;
      }
      else if (type == ConfigElementPointer.class)
      {
         return CONFIG_ELEMENT_POINTER;
      }
      return null;
   }
}
