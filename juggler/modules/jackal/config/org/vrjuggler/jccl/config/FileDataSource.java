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

import java.io.*;
import java.util.*;
import org.jdom.Document;
import org.jdom.JDOMException;
import org.jdom.input.SAXBuilder;

/**
 * Implementation of data source that uses a resource stored in a file.
 */
public class FileDataSource
   implements DataSource
            , ConfigTokens
{
   public static final int UNKNOWN     = 0;
   public static final int DEFINITIONS = 1;
   public static final int ELEMENTS    = 2;

   /**
    * Creates a data source for a new file. The resulting data source is empty.
    */
   public FileDataSource(int type)
      throws IOException
   {
      setFilename(null);
      mType = type;
   }

   /**
    * Creates a data source for an existing file. The resulting data source will
    * contain the contents of the file.
    */
   public FileDataSource(String filename)
      throws IOException
   {
      setFilename(filename);
   }

   /**
    * Sets the filename this data source is operating on. The current data will
    * be cleared and the new data will be retrieved from the file.
    */
   public void setFilename(String filename)
      throws IOException
   {
      if (filename != null)
      {
         mFile = new File(filename);
         reload();
      }
      else
      {
         mFile = null;
      }
   }

   /**
    * Loads the data from the current file.
    */
   private void reload()
      throws IOException
   {
      InputStream in = new BufferedInputStream(new FileInputStream(mFile));

      // Try to load in the resource
      Document resource_doc = loadResource(in);
      if (resource_doc.getRootElement().getName().equals(chunk_desc_db_TOKEN))
      {
         // We just loaded a ChunkDescDB
         mDefinitions = new ChunkDescDB();
         mDefinitions.build(resource_doc);
         mType = DEFINITIONS;
      }
      else if (resource_doc.getRootElement().getName().equals(chunk_db_TOKEN))
      {
         // We just loaded a ConfigChunkDB
         mElements = new ConfigChunkDB();
         mElements.build(resource_doc);
         mType = ELEMENTS;
      }
      else
      {
         throw new IOException("Invalid file");
      }
   }

   /**
    * Gets the name of the file this datasource is operating on.
    */
   public String getFilename()
   {
      if (mFile != null)
      {
         return mFile.getAbsolutePath();
      }
      return null;
   }

   /**
    * Adds the given configuration element to this resource.
    *
    * @param chunk   the configuration element to add
    */
   public void add(ConfigChunk chunk)
   {
      if (acceptsElements() && !isReadOnly())
      {
         mElements.add(chunk);
      }
      else
      {
         throw new UnsupportedOperationException("Not a .config file");
      }
   }

   /**
    * Adds the given configuration definition to this resource.
    *
    * @param desc    the configuration definition to add
    */
   public void add(ChunkDesc desc)
   {
      if (acceptsDefinitions() && !isReadOnly())
      {
         mDefinitions.add(desc);
      }
      else
      {
         throw new UnsupportedOperationException("Not a .desc file");
      }
   }

   /**
    * Removes the given configuration element from this resource.
    *
    * @param chunk   the configuration element to remove
    */
   public void remove(ConfigChunk chunk)
   {
      if (acceptsElements() && !isReadOnly())
      {
         mElements.remove(chunk);
      }
      else
      {
         throw new UnsupportedOperationException("Not a .config file");
      }
   }

   /**
    * Removes the given configuration definition from this resource.
    *
    * @param desc    the configuration definition to remove
    */
   public void remove(ChunkDesc desc)
   {
      if (acceptsDefinitions() && !isReadOnly())
      {
         mDefinitions.remove(desc);
      }
      else
      {
         throw new UnsupportedOperationException("Not a .desc file");
      }
   }

   /**
    * Tests if the given configuration element is in this resource.
    */
   public boolean contains(ConfigChunk chunk)
   {
      if (acceptsElements())
      {
         return mElements.contains(chunk);
      }
      else
      {
         throw new UnsupportedOperationException("Not a .config file");
      }
   }

   /**
    * Tests if the given configuration definition is in this resource.
    */
   public boolean contains(ChunkDesc desc)
   {
      if (acceptsDefinitions())
      {
         return mDefinitions.contains(desc);
      }
      else
      {
         throw new UnsupportedOperationException("Not a .desc file");
      }
   }

   /**
    * Gets a list of all the elements in this resource.
    *
    * @return  a list of the ConfigChunks in this resource
    */
   public List getElements()
   {
      return mElements.getAll();
   }

   /**
    * Gets a list of all the definitions in this resource.
    *
    * @return  a list of the ChunkDescs in this resource
    */
   public List getDefinitions()
   {
      return mDefinitions.getAll();
   }

   /**
    * Commits all pending changes to this resource. This should save the file,
    * send across new data to the running application, etc.
    */
   public void commit()
      throws IOException
   {
      OutputStream out = new BufferedOutputStream(new FileOutputStream(mFile));
      switch (mType)
      {
      case DEFINITIONS:
         mDefinitions.write(out);
         break;
      case ELEMENTS:
         mElements.write(out);
         break;
      default:
         throw new IOException("Unknown file type");
      }
   }

   /**
    * Rolls back all pending changes to this resource. This should reload the
    * file, retrieve data from the application, etc.
    */
   public void rollback()
      throws IOException
   {
      reload();
   }

   /**
    * Checks if this data source is read only.
    */
   public boolean isReadOnly()
   {
      boolean read_only = false;
      if (mFile != null)
      {
         read_only = !mFile.canWrite();
      }
      return read_only;
   }

   /**
    * Checks if this data source can accept configuration elements.
    */
   public boolean acceptsElements()
   {
      return (mType == ELEMENTS);
   }

   /**
    * Checks if this data source can accept configuration definitions.
    */
   public boolean acceptsDefinitions()
   {
      return (mType == DEFINITIONS);
   }

   /**
    * Loads the DOM document from the given input stream.
    *
    * @param input      the stream to read from
    *
    * @return  the DOM document corresponding to the resource's data
    *
    * @throws IOException     if there was an error loading the resource
    */
   private Document loadResource(InputStream input)
      throws IOException
   {
      Document doc = null;

      try
      {
         SAXBuilder builder = new SAXBuilder();
         doc = builder.build(input);
      }
      catch (JDOMException e)
      {
         throw new IOException(e.getMessage());
      }

      return doc;
   }

   /**
    * The file associated with this data source.
    */
   private File mFile;

   /**
    * The database of configuration elements.
    */
   private ConfigChunkDB mElements;

   /**
    * The database of configuration descriptions.
    */
   private ChunkDescDB mDefinitions;

   /**
    * The type of configuration elements this file contains.
    */
   private int mType;
}
