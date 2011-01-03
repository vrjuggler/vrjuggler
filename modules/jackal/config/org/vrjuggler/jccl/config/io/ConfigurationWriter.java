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
import org.jdom.output.Format;
import org.jdom.output.XMLOutputter;
import org.vrjuggler.jccl.config.*;

/**
 * This object knows how to write a configuration to a stream.
 */
public class ConfigurationWriter
   extends BufferedWriter
   implements ConfigIOConstants
{
   public ConfigurationWriter(File file)
      throws IOException
   {
      this(new FileWriter(file));
   }

   public ConfigurationWriter(Writer writer)
   {
      super(writer);
   }

   /**
    * Writes a configuration to the XML document stream.
    */
   public void writeConfiguration(Configuration config)
      throws IOException
   {
      // Create the XML document
      String name = config.getName();
      Document doc = createDocument(name);

      // Add the includes
      for (Iterator itr = config.getIncludes().iterator(); itr.hasNext(); )
      {
         String file = (String)itr.next();
         Element include_elt = new Element(INCLUDE, CFG_NS);
         include_elt.setText(file);
         doc.getRootElement().addContent(include_elt);
      }

      // Add the elements
      Element elements_elt = new Element(ELEMENTS, CFG_NS);
      for (Iterator itr = config.getElements().iterator(); itr.hasNext(); )
      {
         ConfigElement elt = (ConfigElement)itr.next();
         addElement(elt, elements_elt);
      }
      doc.getRootElement().addContent(elements_elt);

      // Write the document out to the stream
      Format f = Format.getPrettyFormat();
      f.setIndent("   ");
      f.setLineSeparator(System.getProperty("line.separator"));
      f.setTextMode(Format.TextMode.NORMALIZE);
      XMLOutputter out = new XMLOutputter(f);
      out.output(doc, this);
   }

   /**
    * Creates a new XML document for the configuration with the given name.
    */
   private Document createDocument(String name)
   {
      // Create the document
      Document doc = new Document();

      // Insert the PI with the file format version
      ProcessingInstruction pi = new ProcessingInstruction(SETTINGS_INSTRUCTION,
                                                           CONFIGURATION_VERSION_ATTR + "=" + "\"" + CFG_VERSION + "\"");
      doc.addContent(pi);

      // Create the configuration root element
      Element root = new Element(CONFIGURATION, CFG_NS);
      root.setAttribute(NAME, name);
      root.setAttribute(SCHEMA_LOCATION, CFG_NS_str + " " + CFG_SCHEMA, XSI_NS);
      doc.setRootElement(root);

      return doc;
   }

   /**
    * Adds the given configuration element to the given elements root element.
    */
   private void addElement(ConfigElement elt, Element root)
   {
      // Create the element element
      Element element_elt = new Element(elt.getDefinition().getToken(), CFG_NS);
      element_elt.setAttribute(NAME, elt.getName());
      element_elt.setAttribute(VERSION, Integer.toString(elt.getDefinition().getVersion()));
      root.addContent(element_elt);

      // Add each property
      ConfigDefinition def = elt.getDefinition();
      for (Iterator itr = def.getPropertyDefinitions().iterator(); itr.hasNext(); )
      {
         PropertyDefinition prop_def = (PropertyDefinition)itr.next();
         String prop_name = prop_def.getToken();

         // Add each property value
         for (int i=0; i<elt.getPropertyValueCount(prop_name); ++i)
         {
            Object prop_value = elt.getProperty(prop_name, i);
            Element prop_elt = new Element(prop_name, CFG_NS);

            // If the property is a simple type, add it as text
            if (prop_def.getType() != ConfigElement.class)
            {
               prop_elt.setText(prop_def.getValueString(prop_value));
            }
            // Otherwise, recurse on the embedded element
            else
            {
               addElement((ConfigElement)prop_value, prop_elt);
            }

            element_elt.addContent(prop_elt);
         }
      }
   }

   private static final Namespace XSI_NS = Namespace.getNamespace("xsi", "http://www.w3.org/2001/XMLSchema-instance");

   private static final String CONFIGURATION                = "configuration";
   private static final String ELEMENTS                     = "elements";
   private static final String FILE                         = "file";
   private static final String INCLUDE                      = "include";
   private static final String NAME                         = "name";
   private static final String VERSION                      = "version";

   private static final String SETTINGS_INSTRUCTION         = "org-vrjuggler-jccl-settings";
   private static final String CONFIGURATION_VERSION_ATTR   = "configuration.version";
   private static final String SCHEMA_LOCATION              = "schemaLocation";
}
