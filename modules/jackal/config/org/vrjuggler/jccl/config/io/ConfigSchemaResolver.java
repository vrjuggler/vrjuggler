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
package org.vrjuggler.jccl.config.io;

import java.io.File;
import org.xml.sax.EntityResolver;
import org.xml.sax.InputSource;

/**
 * If a SAX application needs to implement customized handling for external
 * entities, it must implement this interface and register an instance with
 * the SAX driver using the setEntityResolver method.
 *
 * The XML reader will then allow the application to intercept any external
 * entities (including the external DTD subset and external parameter
 * entities, if any) before including them.
 */
public class ConfigSchemaResolver implements EntityResolver
{
   public InputSource resolveEntity(String publicId, String systemId)
      throws org.xml.sax.SAXException
           , java.io.IOException
   {
      InputSource source;

      // If this systemId is one of the JCCL schemas, process it.
      // XXX: Is there a better way to filter this before it gets to us?
      if ( systemId.indexOf("jccl/xsd") != -1 )
      {
         // Get the schema file name from the string.
         int index = systemId.lastIndexOf('/');
         String schema_file = systemId.substring(index + 1);

         // Construct the full name of the schema file.  On UNIX, it will be
         // of the form $JCCL_BASE_DIR/share/jccl/data/<schema_file>
         String base_dir = System.getProperty("JCCL_BASE_DIR");

         if ( null == base_dir )
         {
            System.err.println("WARNING: JCCL_BASE_DIR not available for " +
                               "local schema loading!");
            source = new InputSource(systemId);
         }
         else
         {
            String full_schema_file = base_dir + File.separator + "share" +
                                      File.separator + "jccl" +
                                      File.separator + "data" +
                                      File.separator + schema_file;

            File f = new File(full_schema_file);

            // If the schema file exists locally, use it!
            if ( f.isFile() && f.canRead() )
            {
               // Let the File object give us the absolute file name just in
               // case there is anything wonky with $JCCL_BASE_DIR.
               source = new InputSource(f.getAbsolutePath());
            }
            // Otherwise, use what we were given.
            else
            {
               System.err.println("WARNING: Could not read " +
                                  f.getAbsolutePath());
               System.err.println("Falling back on " + systemId);
               source = new InputSource(systemId);
            }
         }
      }
      // This is not a JCCL schema, so pass it through untouched.
      else
      {
         source = new InputSource(systemId);
      }

      return source;
   }
}
