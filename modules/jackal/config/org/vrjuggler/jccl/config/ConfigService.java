/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
package org.vrjuggler.jccl.config;

import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;

/**
 * Service bean for Tweek that will read and write config files.
 */
public class ConfigService
{
   /**
    * Reads in a ConfigChunkDB from the given input stream. They are validated
    * against the given ChunkDescDB.
    *
    * @param input      an input stream from which to look for a ConfigChunkDB.
    * @param descDB     the ChunkDescDB with which to validate the chunks
    *
    * @return  the ConfigChunkDB found in the stream.
    * @throw   IOException
    *             there was an error while reading from the stream
    */
   public ConfigChunkDB loadConfigChunks(InputStream input, ChunkDescDB descDB)
      throws IOException
   {
      // Read in the config chunks
//      ChunkFactory.setChunkDescDB(descDB);
      ConfigChunkDB chunk_db = new ConfigChunkDB();
      chunk_db.build(input);
//      ChunkFactory.setChunkDescDB(null);
      return chunk_db;
   }

   /**
    * Writes the given ConfigChunkDB to the given output stream.
    *
    * @param output     the output stream to write the ConfigChunkDB to
    * @param chunkDB    the ConfigChunkDB to write
    *
    * @throw   IOException
    *             there was an error while writing to the stream
    */
   public void saveConfigChunks(OutputStream output, ConfigChunkDB chunkDB)
      throws IOException
   {
      //Write out the config chunks
      DataOutputStream data_out = new DataOutputStream(output);
      chunkDB.write(data_out);
   }

   /**
    * Reads in a ChunkDescDB from the given input stream.
    *
    * @param input      an input stream from which to look for a ChunkDescDB.
    *
    * @return  the ChunkDescDB found in the stream.
    * @throw   IOException
    *             there was an error while reading from the stream
    */
   public ChunkDescDB loadChunkDescs(InputStream input)
      throws IOException
   {
      // Read in the config chunks
      ChunkDescDB desc_db = new ChunkDescDB();
      desc_db.build(input);
//      ChunkFactory.setChunkDescDB(desc_db);
      return desc_db;
   }

   /**
    * Writes the given ChunkDescDB to the given output stream.
    *
    * @param output     the output stream to write the ChunkDescDB to
    * @param descDB     the ChunkDescDB to write
    *
    * @throw   IOException
    *             there was an error while writing to the stream
    */
   public void saveChunkDescs(OutputStream output, ChunkDescDB descDB)
      throws IOException
   {
      //Write out the config chunks
      DataOutputStream data_out = new DataOutputStream(output);
      descDB.write(data_out);
   }
}
