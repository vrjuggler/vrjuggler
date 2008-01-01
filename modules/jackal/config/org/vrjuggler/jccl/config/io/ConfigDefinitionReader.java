/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
import javax.xml.transform.*;
import javax.xml.transform.stream.*;
import org.jdom.*;
import org.jdom.filter.*;
import org.jdom.input.SAXBuilder;
import org.jdom.transform.*;
import org.vrjuggler.jccl.config.*;

/**
 * This object knows how to read a configuration definition (all versions of
 * it) from a stream.
 */
public class ConfigDefinitionReader
   extends BufferedReader
   implements ConfigIOConstants
{
   /**
    * Creates a new configuration definition reader that will read definitions
    * from the given file. This is a shortcut for:
    *
    * reader = new ConfigDefinitionReader(new FileReader(file));
    */
   public ConfigDefinitionReader(File file)
      throws IOException
   {
      this(new FileReader(file));
   }

   /**
    * Creates a new configuration definition reader that will read definitions
    * from the given reader stream.
    */
   public ConfigDefinitionReader(Reader reader)
   {
      super(reader);
   }

   /**
    * Reads a configuration definition from the XML document stream.
    *
    * @return  a list of all the versions of the configuration definitions
    *          contained in the stream
    */
   public List readDefinition()
      throws IOException
           , ParseException
   {
      try
      {
         // Build a DOM tree from the XML stream
         SAXBuilder builder =
            new SAXBuilder("org.apache.xerces.parsers.SAXParser", true);
         builder.setFeature("http://apache.org/xml/features/validation/schema",
                            true);
         builder.setEntityResolver(new ConfigSchemaResolver());
         Document doc = builder.build(this);

         // Bring the file up to date with the current file format version
         doc = upgrade(doc);

         // Create a parser for the file and parse the document
         return (new ConfigDefinitionParser()).parse(doc);
      }
      catch (org.jdom.input.JDOMParseException e)
      {
         System.err.println(e.getMessage());
         throw new IOException(e.getMessage());
      }
      catch (JDOMException e)
      {
         e.printStackTrace();
         throw new IOException(e.getMessage());
      }
   }

   /**
    * Gets the file format version for this XML document.
    *
    * @param doc           the XML document to examine
    *
    * @return the file format version for the document
    *
    * @throws ParseException
    *             if the version cannot be determined
    */
   private String getFileFormatVersion(Document doc)
      throws ParseException
   {
      // Get all processing instructions
      List pis = doc.getContent(new ContentFilter(ContentFilter.PI));

      // Look for the version PI and check the version value
      for (Iterator itr = pis.iterator(); itr.hasNext(); )
      {
         ProcessingInstruction pi = (ProcessingInstruction)itr.next();
         if (pi.getTarget().equals(SETTINGS_INSTRUCTION))
         {
            if (pi.getPseudoAttributeNames().contains(DEF_VERSION_ATTR))
            {
               return pi.getPseudoAttributeValue(DEF_VERSION_ATTR);
            }
            if (pi.getPseudoAttributeNames().contains(DESC_DB_VERSION_ATTR))
            {
               return pi.getPseudoAttributeValue(DESC_DB_VERSION_ATTR);
            }
         }
      }

      // No version was specified
      throw new ParseException("Unknown file format version.");
   }

   /**
    * Updates the version of the file to the latest version.
    *
    * @param doc        the document from which to upgrade
    *
    * @return  the document that is the result of the upgrade
    */
   private Document upgrade(Document doc)
      throws ParseException
   {
      // Apply transforms until we have converted the file to the latest version
      String version = getFileFormatVersion(doc);
      while (! version.equals(DEF_VERSION))
      {
         // Get a stream to the XSLT
         InputStream xslt_in = getXSLT(version);
         if (xslt_in == null)
         {
            throw new ParseException("No XSLT to upgrade from file format version: "+version);
         }

         // Upgrade the document to the next version
         try
         {
            doc = applyXSLT(xslt_in, doc);
         }
         catch (TransformerException te)
         {
            // Translate the exception into a parse exception
            throw new ParseException(te.getMessageAndLocation());
         }

         // Get the version of the document after application of the XSLT
         version = getFileFormatVersion(doc);
      }

      return doc;
   }

   /**
    * Applies the XSLT in the given stream to the given document.
    *
    * @param xslt       the stream from which the XSLT can be retrieved
    * @param doc        the XML document on which to apply the XSLT
    *
    * @return  the document resulting from the application of the XSLT
    *
    * @throws  TransformerException
    *             if there is a problem during the transformation
    */
   private Document applyXSLT(InputStream xslt, Document doc)
      throws TransformerException
   {
      // Create and setup the XSL transformer
      TransformerFactory factory = TransformerFactory.newInstance();
      Transformer transformer = factory.newTransformer(new StreamSource(xslt));
      JDOMSource source = new JDOMSource(doc);
      JDOMResult result = new JDOMResult();

      // Apply the XSLT
      transformer.transform(source, result);

      // Return the resulting document
      return result.getDocument();
   }

   /**
    * Gets a stream to the XSLT that will upgrade a configuration definition
    * file from the given version to the next version.
    *
    * @param version       the version of the definition from which to upgrade
    */
   private InputStream getXSLT(String version)
   {
      String xslt_file = "org/vrjuggler/jccl/config/xslt/definition_"+version+".xsl";
      ClassLoader loader = getClass().getClassLoader();
      return loader.getResourceAsStream(xslt_file);
   }

   private static final String SETTINGS_INSTRUCTION   = "org-vrjuggler-jccl-settings";
   private static final String DEF_VERSION_ATTR       = "definition.version";
   private static final String DESC_DB_VERSION_ATTR   = "desc.db.version";
}
