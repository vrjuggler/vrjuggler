/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
package org.vrjuggler.vrjconfig.controlpanel.io;

import java.io.*;
import java.util.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.xml.transform.*;
import javax.xml.transform.stream.*;
import org.jdom.*;
import org.jdom.filter.*;
import org.jdom.input.SAXBuilder;
import org.jdom.transform.*;
import org.vrjuggler.jccl.config.*;

/**
 * This object knows how to read a control panel.
 */
public class ControlPanelReader
   extends BufferedReader
   implements ControlPanelIOConstants
{
   /**
    * Creates a new control panel reader that will read control panels
    * from the given file. This is a shortcut for:
    *
    * reader = new ControlPanelReader(new FileReader(file));
    */
   public ControlPanelReader(File file)
      throws IOException
   {
      this(new FileReader(file));
   }

   /**
    * Creates a new control panel reader that will read panels from the
    * given reader stream.
    */
   public ControlPanelReader(Reader reader)
   {
      super(reader);
   }

   /**
    * Reads a control panel from the XML document stream.
    *
    * @return  a complete control panel model.
    */
   public DefaultMutableTreeNode readControlPanel()
      throws IOException
           , ParseException
   {
      try
      {
         // Build a DOM tree from the XML stream
         // TODO: Re-enable Schema check..... :(
         SAXBuilder builder = new SAXBuilder(false);
         Document doc       = builder.build(this);

         // Create a parser for the file and parse the document
         return (new ControlPanelParser()).parse(doc);
         //ControlPanelParser parser = new ControlPanelParser();
         //return(parser.parse(doc));
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
    * @param  doc       the XML document to examine
    * @throws ParseException if the version cannot be determined
    * @return the file format version for the document
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

   private static final String SETTINGS_INSTRUCTION   = "org-vrjuggler-jccl-settings";
   private static final String DEF_VERSION_ATTR       = "definition.version";
   private static final String DESC_DB_VERSION_ATTR   = "desc.db.version";
}
