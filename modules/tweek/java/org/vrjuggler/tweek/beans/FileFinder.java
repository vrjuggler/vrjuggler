/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

package org.vrjuggler.tweek.beans;

import org.vrjuggler.tweek.services.ExtensionFileFilter;

import java.io.File;
import java.io.FilenameFilter;


/**
 * Very simple class used to find files with the given extension in the given
 * path.  This is intended for use with files that contain JavaBeans or
 * "pointers" to JavaBeans&mdash;hence its inclusion in the
 * <code>org.vrjuggler.tweek.beans</code> package.
 */
public class FileFinder
{
   /**
    * Constructs a new FileFinder object looking for files with the given
    * extension in the given path.
    *
    * Pre-condition: The named path is a directory.
    *
    * @param ext     the extension to match against not including the '.'
    * @param path    the path in which to search for files
    *
    * @throws BeanPathException
    */
   public FileFinder (String ext, String path) throws BeanPathException
   {
      mDirectory = new File(path);

      if ( ! mDirectory.isDirectory() )
      {
         throw new BeanPathException(path);
      }

      // Search the directory for the files matching the given extension.
      ExtensionFileFilter filter = new ExtensionFileFilter("", false);
      filter.addExtension(ext);
      mFiles = mDirectory.listFiles(filter);
   }

   public int getFileCount ()
   {
      return mFiles.length;
   }

   public File[] getFiles ()
   {
      return mFiles;
   }

   public File getFile (int index)
   {
      return mFiles[index];
   }

   // =========================================================================
   // Private methods.
   // =========================================================================

   private File   mDirectory = null; /**< The directory searched */
   private File[] mFiles     = null; /**< The matching files found in mDirectory */
}
