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
package org.vrjuggler.jccl.config.io;

import java.io.File;
import org.xml.sax.EntityResolver;
import org.xml.sax.InputSource;

import org.vrjuggler.tweek.services.EnvironmentServiceProxy;


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

      try
      {
         String full_schema_file;
         EnvironmentServiceProxy service = new EnvironmentServiceProxy();

         // This is a remote schema that would have to be downloaded using
         // HTTP.
         if ( systemId.startsWith("http://") )
         {
            String base_dir =
               service.getenv("JCCL_BASE_DIR");

            if ( null == base_dir )
            {
               System.err.println("WARNING: JCCL_BASE_DIR not available for " +
                                  "local schema loading!");
               throw new Exception("JCCL_BASE_DIR not available");
            }

            // Chop the "http://" off the front of the system ID to get the
            // schema location.
            String schema_file = systemId.substring(6);

            // Replace the '/' characters in the schema location with the
            // platform-specific file separator character.
            if ( '/' != File.separatorChar )
            {
               schema_file = schema_file.replace('/', File.separatorChar);
            }

            full_schema_file = base_dir + File.separator + 
                               service.getenv("JCCL_DATA_DIR") +
                               File.separator + "data" +
                               File.separator + "schema" +
                               File.separator + schema_file;
         }
         // This is a locally available schema.
         else if ( systemId.startsWith("file://") )
         {
            // Chop the "file://" off the front of the system ID to get the
            // schema location.
            full_schema_file = systemId.substring(6);
         }
         else
         {
            System.err.println("WARNING: Unexpected URI protocol in '" +
                              systemId + "'");
            throw new Exception("Unexpected protocol type in schema URI");
         }

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
            throw new Exception("Could not read " + f.getAbsolutePath());
         }
      }
      catch(Exception ex)
      {
         System.out.println("Falling back on " + systemId);
         source = new InputSource(systemId);
      }

      return source;
   }
}
