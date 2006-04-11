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
package org.vrjuggler.jccl.config.parser;

import java.io.*;
import java.util.*;
import org.jdom.*;
import org.jdom.filter.ContentFilter;
import org.jdom.input.SAXBuilder;

import org.vrjuggler.jccl.config.Configuration;
import org.vrjuggler.jccl.config.ConfigDefinitionRepository;

/**
 * This object knows how to read a configuration from a stream.
 */
public class ConfigurationReader
   extends BufferedReader
{
   public ConfigurationReader(File file, ConfigDefinitionRepository repos)
      throws IOException
   {
      super(new FileReader(file));
      mDefinitionRepos = repos;
   }

   /**
    * Reads a configuration from the XML document stream.
    */
   public Configuration readConfiguration()
      throws IOException
           , ParseException
   {
      try
      {
         // Build a DOM tree from the XML stream
         SAXBuilder builder = new SAXBuilder();
         Document doc       = builder.build(this);

         // Gets a parser for this version of the XML document
         ConfigurationParser parser = getParser(doc);
         if (parser == null)
         {
            throw new IOException("Unknown file format version.");
         }

         // Ask the parser to parse the document
         parser.setRepository(mDefinitionRepos);
         return parser.parse(doc);
      }
      catch (JDOMException e)
      {
         e.printStackTrace();
         throw new IOException(e.getMessage());
      }
   }

   /**
    * Gets the parser for the given XML document. The version proccesing
    * instruction is evaluated to determine which parser to use.
    */
   private ConfigurationParser getParser(Document doc)
   {
      String version = getFileFormatVersion(doc);
      if (version != null)
      {
         return getParser(version);
      }
      return null;
   }

   /**
    * Gets the parser for the given file format version.
    */
   private ConfigurationParser getParser(String version)
   {
      // Determine the fully-qualified name for parser
      String version_pkg = "v" + version.replace('.', '_');
      String classname = "org.vrjuggler.jccl.config.parser."+version_pkg+".ConfigurationParser";

      try
      {
         Class clazz = Class.forName(classname);
         Object parser = clazz.newInstance();
         return (ConfigurationParser)parser;
      }
      catch (Exception e)
      {
         return null;
      }
   }

   /**
    * Gets the file format version for this XML document.
    *
    * @param doc           the XML document to examine
    *
    * @return the file format version for the document, null if there is no
    *         version specified
    */
   private String getFileFormatVersion(Document doc)
   {
      // Get all processing instructions
      List pis = doc.getContent(new ContentFilter(ContentFilter.PI));

      // Look for the version PI and check the version value
      for (Iterator itr = pis.iterator(); itr.hasNext(); )
      {
         ProcessingInstruction pi = (ProcessingInstruction)itr.next();
         if (pi.getTarget().equals(SETTINGS_INSTRUCTION))
         {
            if (pi.getNames().contains(CONFIGURATION_VERSION_ATTR))
            {
               return pi.getValue(CONFIGURATION_VERSION_ATTR);
            }
         }
      }

      // No version was specified
      return null;
   }

   private static final String SETTINGS_INSTRUCTION         = "org-vrjuggler-jccl-settings";
   private static final String CONFIGURATION_VERSION_ATTR   = "configuration.version";

   private ConfigDefinitionRepository mDefinitionRepos;
}
