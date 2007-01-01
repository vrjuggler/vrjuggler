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
package org.vrjuggler.jccl.config.io;

import java.io.*;
import java.util.*;
import org.jdom.*;

import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.jccl.config.*;

/**
 * This object knows how to create a configuration from an XML document and a
 * configuration definition repository.
 */
public class ConfigurationParser
   implements ConfigIOConstants
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
      for (Iterator itr = root.getChildren(INCLUDE, CFG_NS).iterator(); itr.hasNext(); )
      {
         // Parse the given include
         Element elt = (Element)itr.next();
         config.addInclude(elt.getText());
      }

      // Get the configuration definition lookup path
      Element def_path = root.getChild(DEFINITION_PATH, CFG_NS);
      if ( null != def_path )
      {
         List path = parseDefinitionPath(def_path);
         for (Iterator itr = path.iterator(); itr.hasNext(); )
         {
            // Add the configuration definition lookup path
            String dir = (String)itr.next();
            config.addDefinitionPath(dir);
            scanDefinitionDir(dir);
         }
      }

      // Get the list of elements in this configuration
      List config_elts = parseElements(root.getChild(ELEMENTS, CFG_NS)/*, path*/);
      for (Iterator itr = config_elts.iterator(); itr.hasNext(); )
      {
         // Add the given configuration element to the configuration
         ConfigElement config_elt = (ConfigElement)itr.next();
         config.addElement(config_elt);
      }

      return config;
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
      for (Iterator itr = root.getChildren(DIR, CFG_NS).iterator(); itr.hasNext(); )
      {
         // Parse the directory addition to the path
         Element elt = (Element)itr.next();
         String dir = elt.getTextTrim();
         path.add(dir);
      }

      return path;
   }

   private void scanDefinitionDir(String dirName)
   {
      String dir_name = dirName;

      try
      {
         dir_name = (new EnvironmentServiceProxy()).expandEnvVars(dirName);
      }
      catch(RuntimeException ex)
      {
         System.err.println("WARNING: Failed to load Tweek Environment Service: " +
                            ex.getMessage());
      }

      File dir = new File(dir_name);
      FilenameFilter filter = new DefinitionFilenameFilter();

      if ( dir.exists() && dir.isDirectory() && dir.canRead() )
      {
         File[] def_files = dir.listFiles(filter);

         for ( int i = 0; i < def_files.length; ++i )
         {
            try
            {
               ConfigDefinitionReader reader =
                  new ConfigDefinitionReader(def_files[i]);
               List defs = reader.readDefinition();
               for (Iterator def_itr = defs.iterator(); def_itr.hasNext(); )
               {
                  ConfigDefinition def = (ConfigDefinition)def_itr.next();
                  mDefinitionRepos.add(def);
               }
            }
            catch (ParseException pe)
            {
               System.err.println(
                  "Caught a parse exception while trying to load\n" +
                     def_files[i].getAbsolutePath() + "\n" +
                     pe.getMessage() + "\n" +
                     "The contents of this file will not be available!"
               );
            }
            catch (IOException ioe)
            {
               System.err.println(
                  "Caught an I/O exception while trying to load\n" +
                     def_files[i].getAbsolutePath() + "\n" +
                     ioe.getMessage() + "\n" +
                     "The contents of this file will not be available!"
               );
            }
         }
      }
   }

   /**
    * Parses an elements node in the DOM tree. The result is a list of the
    * configuration elements that are children of the node.
    */
   private List parseElements(Element root/*, List searchPath*/)
      throws ParseException
   {
      // Verify that the root element is an elements node
      if (!root.getName().equals(ELEMENTS))
      {
         throw new ParseException("Root element must be: "+ELEMENTS);
      }

      ConfigElementParser parser = new ConfigElementParser(mDefinitionRepos/*,
                                                           searchPath*/);
      List config_elts = new ArrayList();
      List created_elts = new ArrayList();

      // Parse each child of the element's node for a configuration element.
      parseElementList(parser, root.getChildren(), config_elts, created_elts);

      // Parse XML elements created as a result of parsing the initial input.
      List new_elts = created_elts;
      while ( ! new_elts.isEmpty() )
      {
         created_elts = new ArrayList();
         parseElementList(parser, new_elts, config_elts, created_elts);
         new_elts = created_elts;
      }

      return config_elts;
   }

   private void parseElementList(ConfigElementParser parser, List xmlElements,
                                 List configElements, List createdXmlElements)
      throws ParseException
   {
      for ( Iterator e = xmlElements.iterator(); e.hasNext(); )
      {
         configElements.add(parser.parse((Element) e.next(),
                                         createdXmlElements));
      }
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
