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
    * Creates a data source for a file. If the file exists, the resulting data
    * source will contain the contents of the file. If the file does not exist,
    * a new data source of the given type will be created
    */
   public FileDataSource(String filename, int type)
      throws IOException
   {
      setFilename(filename);
      if (mFile.exists())
      {
         reload();
      }
      else
      {
         createNewResource(type);
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
    * Creates a new file of the given type in memory. It is not saved to the
    * disk.
    */
   private void createNewResource(int type)
   {
      switch (type)
      {
      case DEFINITIONS:
         mDefinitions = new ChunkDescDB();
         break;
      case ELEMENTS:
         mElements = new ConfigChunkDB();
         break;
      default:
         throw new IllegalArgumentException("Invalid type: "+type);
      }

      mType = type;
   }

   /**
    * Sets the filename this data source is operating on. The current data will
    * be cleared and the new data will be retrieved from the file.
    */
   public void setFilename(String filename)
      throws IOException
   {
      if (filename == null)
      {
         throw new IllegalArgumentException("filename cannot be null");
      }
      mFile = new File(filename);
   }

   /**
    * Gets the name of the file this datasource is operating on.
    */
   public String getFilename()
   {
      return mFile.getAbsolutePath();
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
         throw new UnsupportedOperationException(mFile.getAbsolutePath()+" is not a .config file");
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
         throw new UnsupportedOperationException(mFile.getAbsolutePath()+" is not a .desc file");
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
         throw new UnsupportedOperationException(mFile.getAbsolutePath()+" is not a .config file");
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
         throw new UnsupportedOperationException(mFile.getAbsolutePath()+" is not a .desc file");
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
         throw new UnsupportedOperationException(mFile.getAbsolutePath()+" is not a .config file");
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
         throw new UnsupportedOperationException(mFile.getAbsolutePath()+" is not a .desc file");
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
      boolean read_only = true;
      if ((!mFile.exists()) || mFile.canWrite())
      {
         read_only = false;
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
    * Gets a list of the URLs for the resources included by this data source.
    *
    * @return  a list of URLs for the included resources
    */
   public List getIncludes()
   {
      List includes = new ArrayList();
      if (acceptsElements())
      {
         includes.addAll(mElements.getIncludes());
      }
      if (acceptsDefinitions())
      {
//XXX         includes.addAll(mDefinitions.getIncludes());
      }
      return includes;
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
    * File data sources are equal if they refer to the same object.
    */
   public boolean equals(Object obj)
   {
      if (obj instanceof FileDataSource)
      {
         FileDataSource ds = (FileDataSource)obj;
         if (mFile.equals(ds.mFile))
         {
            return true;
         }
      }
      return false;
   }

   //XXX: Debug when this data source gets finalized
   protected void finalize()
      throws Throwable
   {
      System.out.println("FileDataSource for "+mFile.getPath()+" being finalized.");
      super.finalize();
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
