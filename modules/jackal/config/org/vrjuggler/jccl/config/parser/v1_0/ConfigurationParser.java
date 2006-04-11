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

import java.io.*;
import java.util.*;
import org.jdom.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.parser.ConfigurationReader;
import org.vrjuggler.jccl.config.parser.ParseException;
import org.vrjuggler.jccl.config.parser.MissingAttributeException;
import org.vrjuggler.jccl.config.ConfigDefinitionRepository;

/**
 * This object knows how to create a configuration from an XML document and a
 * configuration definition repository.
 *
 * This parser works on files formatted to the version 1.0 specification.
 */
public class ConfigurationParser
   implements org.vrjuggler.jccl.config.parser.ConfigurationParser
{
   public ConfigurationParser()
   {
      mDefinitionRepos = null;
   }

   /**
    * Sets the repository from which configuration definitions shall be located.
    */
   public void setRepository(ConfigDefinitionRepository repos)
   {
      mDefinitionRepos = repos;
   }

   /**
    * Parses the configuration out of the given XML document.
    */
   public Configuration parse(Document doc)
      throws ParseException
   {
      return parseConfiguration(doc.getRootElement());
   }

   /**
    * Parses a configuration node in the DOM tree. This will return a
    * Configuration object.
    */
   public Configuration parseConfiguration(Element root)
      throws ParseException
   {
      // Verify that the root element is a configuration
      if (!root.getName().equals(CONFIGURATION))
      {
         throw new ParseException("Root element must be: "+CONFIGURATION);
      }

      // Verify that this configuration has a name
      String name = root.getAttributeValue(NAME);
      if (name == null)
      {
         throw new MissingAttributeException(NAME);
      }

      // Create the configuration that we will build up
      Configuration config = new Configuration(name);

      // Get the included configurations
      for (Iterator itr = root.getChildren(INCLUDE).iterator(); itr.hasNext(); )
      {
         // Parse the given include
         Element elt = (Element)itr.next();
         config.addInclude(parseInclude(elt));
      }

      // Get the configuration definition lookup path
      List path = parseDefinitionPath(root.getChild(DEFINITION_PATH));
      for (Iterator itr = path.iterator(); itr.hasNext(); )
      {
         // Add the configuration definition lookup path
         String dir = (String)itr.next();
         config.addDefinitionPath(dir);
      }

      // Get the list of elements in this configuration
      List config_elts = parseElements(root.getChild(ELEMENTS), path);
      for (Iterator itr = config_elts.iterator(); itr.hasNext(); )
      {
         // Add the given configuration element to the configuration
         ConfigElement config_elt = (ConfigElement)itr.next();
         config.addElement(config_elt);
      }

      return config;
   }

   /**
    * Parses an include node in the DOM tree. This will attempt to open and
    * parse the file specified within the element.
    */
   private Configuration parseInclude(Element root)
      throws ParseException
   {
      // Verify that the root element is an include
      if (!root.getName().equals(INCLUDE))
      {
         throw new ParseException("Root element must be: "+INCLUDE);
      }

      // Get the file attribute
      String file_str = root.getAttributeValue(FILE);
      if (file_str == null)
      {
         throw new MissingAttributeException(FILE);
      }

      // Create the file
      File file = new File(file_str);

      try
      {
         // Create a new configuration reader for the file
         ConfigurationReader reader = new ConfigurationReader(file,
                                                              mDefinitionRepos);
         return reader.readConfiguration();
      }
      catch (IOException ioe)
      {
         throw new ParseException(ioe.getMessage());
      }
   }

   /**
    * Parses a definition path node in the DOM tree.
    */
   private List parseDefinitionPath(Element root)
      throws ParseException
   {
      // Verify that the root element is a definition path node
      if (!root.getName().equals(DEFINITION_PATH))
      {
         throw new ParseException("Root element must be: "+DEFINITION_PATH);
      }

      // Parse each dir child of the definition path node
      List path = new ArrayList();
      for (Iterator itr = root.getChildren(DIR).iterator(); itr.hasNext(); )
      {
         // Parse the directory addition to the path
         Element elt = (Element)itr.next();
         String dir = elt.getTextTrim();
         path.add(dir);
      }

      return path;
   }

   /**
    * Parses an elements node in the DOM tree. The result is a list of the
    * configuration elements that are children of the node.
    */
   private List parseElements(Element root, List searchPath)
      throws ParseException
   {
      // Verify that the root element is an elements node
      if (!root.getName().equals(ELEMENTS))
      {
         throw new ParseException("Root element must be: "+ELEMENTS);
      }

      ConfigElementParser parser = new ConfigElementParser(mDefinitionRepos,
                                                           searchPath);
      List config_elts = new ArrayList();

      // Parse each child of the elements node for a configuration element
      for (Iterator itr = root.getChildren().iterator(); itr.hasNext(); )
      {
         Element elt = (Element)itr.next();
         config_elts.add(parser.parse(elt));
      }

      return config_elts;
   }

   private static final String CONFIGURATION             = "configuration";
   private static final String DEFINITION_PATH           = "definition_path";
   private static final String DIR                       = "dir";
   private static final String ELEMENTS                  = "elements";
   private static final String FILE                      = "file";
   private static final String INCLUDE                   = "include";
   private static final String NAME                      = "name";

   /** The repository from which configuration definitions are retrieved. */
   private ConfigDefinitionRepository mDefinitionRepos;
}
