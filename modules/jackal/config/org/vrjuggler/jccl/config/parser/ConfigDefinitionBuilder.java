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
import org.jdom.filter.*;
import org.jdom.input.SAXBuilder;
import org.vrjuggler.jccl.config.*;

/**
 * This object knows how to create a configuration definition (all versions of
 * it) from a given input stream.
 */
public class ConfigDefinitionBuilder
{
   public ConfigDefinitionBuilder()
   {}

   /**
    * Reads the contents of the given file and returns a list of all versions of
    * configuration definitions contained therein.
    *
    * @param file          the file to parse
    */
   public List parse(File file)
      throws IOException
           , ParseException
   {
      return parse(new BufferedReader(new FileReader(file)));
   }

   /**
    * Reads the XML document from the given stream and returns a list of all
    * versions of configuration definitions contained therein.
    *
    * @param reader        the character stream to read from
    */
   public List parse(Reader reader)
      throws IOException
           , ParseException
   {
      try
      {
         // Build a DOM tree from the XML stream
         SAXBuilder builder = new SAXBuilder();
         Document doc       = builder.build(reader);

         // Gets a parser for the file format version of the XML document
         ConfigDefinitionParser parser = getParser(doc);
         if (parser == null)
         {
            throw new IOException("Unknown file format version.");
         }

         // Ask the parser to parse the document
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
   private ConfigDefinitionParser getParser(Document doc)
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
   private ConfigDefinitionParser getParser(String version)
   {
      // Determine the fully-qualified name for parser
      String version_pkg = "v" + version.replace('.', '_');
      String classname = "org.vrjuggler.jccl.config.parser."+version_pkg+".ConfigDefinitionParser";

      try
      {
         Class clazz = Class.forName(classname);
         Object parser = clazz.newInstance();
         return (ConfigDefinitionParser)parser;
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
            if (pi.getNames().contains(DEF_VERSION_ATTR))
            {
               return pi.getValue(DEF_VERSION_ATTR);
            }
         }
      }

      // No version was specified
      return null;
   }

   private static final String SETTINGS_INSTRUCTION   = "org-vrjuggler-jccl-settings";
   private static final String DEF_VERSION_ATTR       = "definition.version";
}
