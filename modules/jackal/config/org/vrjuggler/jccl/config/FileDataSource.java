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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.jccl.config;

import java.io.*;
import java.util.*;
import org.vrjuggler.jccl.config.io.*;

/**
 * Implementation of data source that uses a resource stored in a file.
 */
public class FileDataSource
   implements DataSource
{
   /**
    * Creates a data source for a new file. This constructor is private, use
    * create() or open() instead.
    */
   private FileDataSource(ConfigDefinitionRepository repos)
   {
      mDefinitionRepos = repos;
   }

   /**
    * Creates a data source for a new configuration file. The configuration will
    * be stored in the file with the given name.
    *
    * @param filename      the name of the file to create
    *
    * @throws IOException
    *             if the file already exists
    */
   public static FileDataSource create(String filename,
                                       ConfigDefinitionRepository repos)
      throws IOException
   {
      FileDataSource ds = new FileDataSource(repos);
      ds.createNew(filename);
      return ds;
   }

   /**
    * Creates a data source for an existing configuration file. The
    * configuration stored in the file will be loaded.
    *
    * @param filename      the name of the file to open
    *
    * @throws IOException
    *             if the file does not exist
    */
   public static FileDataSource open(String filename,
                                     ConfigDefinitionRepository repos)
      throws IOException
   {
      FileDataSource ds = new FileDataSource(repos);
      ds.openExisting(filename);
      return ds;
   }

   /**
    * Creates a new configuration for the given file.
    *
    * @throws IOException
    *             if the file already exists
    */
   private void createNew(String filename)
      throws IOException
   {
      mFile = new File(filename);
      if (mFile.exists())
      {
         throw new IOException("File already exists: "+filename);
      }
      
      //Date date = new Date();
      //mFile.setLastModified(date.getTime());
      resetLastModified();

      mConfig = new Configuration(filename);
   }

   /**
    * Loads a configuration from the given file.
    *
    * @throws IOException
    *             if the file does not exist
    */
   private void openExisting(String filename)
      throws IOException
   {
      mFile = new File(filename);
      reload();
   }

   /**
    * Loads the data from the current file.
    *
    * @throws IOException
    *             if there was a problem loading the file
    */
   private void reload()
      throws IOException
   {
      try
      {
         ConfigurationReader reader = new ConfigurationReader(mFile, mDefinitionRepos);
         mConfig = reader.readConfiguration();
         if (isReadOnly())
         {
            for (Iterator itr = mConfig.getElements().iterator() ; itr.hasNext() ; )
            {
               ((ConfigElement)itr.next()).setReadOnly(true);
            }
         }
      }
      catch (ParseException pe)
      {
         throw new IOException(pe.getMessage());
      }

      resetLastModified();
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
    * @param elt        the configuration element to add
    */
   public void add(ConfigElement elt)
   {
      if (!isReadOnly())
      {
         mConfig.addElement(elt);
      }
      else
      {
         throw new UnsupportedOperationException("Data source is read only.");
      }
   }

   /**
    * Removes the given configuration element from this resource.
    *
    * @param elt     the configuration element to remove
    */
   public void remove(ConfigElement elt)
   {
      if (!isReadOnly())
      {
         mConfig.removeElement(elt);
      }
      else
      {
         throw new UnsupportedOperationException("Data source is read only.");
      }
   }
   /**
    * Tests if the given configuration element is in this resource.
    *
    * @param elt     the configuration element to look for
    */
   public boolean contains(ConfigElement elt)
   {
      return mConfig.getElements().contains(elt);
   }

   /**
    * Gets a list of all the elements in this resource.
    *
    * @return  a list of the ConfigElements in this resource
    */
   public List getElements()
   {
      return mConfig.getElements();
   }

   /**
    * Commits all pending changes to this resource. This should save the file,
    * send across new data to the running application, etc.
    */
   public void commit()
      throws IOException
   {
      ConfigurationWriter writer = new ConfigurationWriter(mFile);
      writer.writeConfiguration(mConfig);
      
      resetLastModified();
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

   public boolean isOutOfDate()
   {
      return ( mLastKnownModify != mFile.lastModified());
   }

   public void resetLastModified()
   {
      mLastKnownModify = mFile.lastModified();
   }

   /**
    * Gets a list of the URLs for the resources included by this data source.
    *
    * @return  a list of URLs for the included resources
    */
   public List getIncludes()
   {
      return mConfig.getIncludes();
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

   /** The file associated with this data source. */
   private File mFile;

   /** The configuration contained in this data source. */
   private Configuration mConfig;

   /** The repository from which configuration definitions are retrieved. */
   private ConfigDefinitionRepository mDefinitionRepos;

   private long mLastKnownModify = 0;
}
