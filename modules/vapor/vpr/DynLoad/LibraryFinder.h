/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_LIBRARY_FINDER_H_
#define _VPR_LIBRARY_FINDER_H_

#include <vpr/vprConfig.h>

#include <vector>
#include <boost/filesystem/path.hpp>

#include <vpr/DynLoad/Library.h>


namespace vpr
{

/**
 * Helper class that will perform dynamic library discovery based on a file
 * extension (e.g., .so or .dll) in a specific directory.  A vector of all
 * discovered dynamic libraries will be created.
 */
class VPR_CLASS_API LibraryFinder
{
public:
   /** Helper typedef to simplify user code a little. */
   typedef std::vector<vpr::LibraryPtr> LibraryList;

   /**
    * @post The named directory is scanned for shared libraries if scanNow is
    *       true.
    *
    * @param libDir  The name of the directory to search for shared libraries.
    *                This directory can be an absolute platform-specific path.
    * @param libExt  The platform-specific extension that is used to identify
    *                shared libraries.  This should be of the form "so" (for
    *                most UNIX-based systems) or "dll" (for Win32).  Note that
    *                the "." is not included in the enxtension string.
    * @param scanNow A boolean value stating where the directory scan should be
    *                performed immediately upon object construction.  If it is
    *                false, the burden is on the calling code to invoke
    *                rescan() before getLibraries().  This parameter is
    *                optional, and it defaults to true.
    *
    * @see rescan
    * @see getLibraries
    */
   LibraryFinder(const std::string& libDir, const std::string& libExt,
                 bool scanNow = true)
      : mLibDir(libDir, boost::filesystem::native), mLibExt(libExt)
   {
      if ( scanNow )
      {
         rescan();
      }
   }

   /**
    * Changes the name of the directory to be searched for shared libraries.
    * After changing the name, the new directory is scanned for shared
    * libraries.
    *
    * @post The current directory is changed to the given value, and the new
    *       directory is scanned for shared libraries.
    *
    * @param dir The name of the directory to search for shared libraries.
    *            This directory can be an absolute platform-specific path.
    */
   void setLibraryDirectory(const std::string& dir)
   {
      mLibDir = boost::filesystem::path(dir, boost::filesystem::native);
      rescan();
   }

   /**
    * Changes the shared library file extension.  After changing the extension,
    * the current directory is rescanned.
    *
    * @post The library file extension is changed to the given value, and the
    *       new directory is scanned for shared libraries.
    *
    * @param ext The platform-specific extension that is used to identify
    *            shared libraries.  This should be of the form "so" (for most
    *            UNIX-based systems) or "dll" (for Win32).  Note that the "."
    *            is not included in the enxtension string.
    */
   void setLibraryExtension(const std::string& ext)
   {
      mLibExt = ext;
      rescan();
   }

   /**
    * Changes the name of the directory to be searched for shared libraries
    * and the shared library file extension.  After changing these settings,
    * the new directory is scanned for shared libraries.  This method can be
    * used to reduce disk I/O.  Whereas the combination of
    * setLibraryDirectory() and setLibraryExtension() would result in two
    * scans, this will only perform one directory scan.
    *
    * @post The library file extension is changed to the given value, and the
    *       new directory is scanned for shared libraries.
    *
    * @param dir The name of the directory to search for shared libraries.
    *            This directory can be an absolute platform-specific path.
    * @param ext The platform-specific extension that is used to identify
    *            shared libraries.  This should be of the form "so" (for most
    *            UNIX-based systems) or "dll" (for Win32).  Note that the "."
    *            is not included in the enxtension string.
    */
   void setDirAndExt(const std::string& dir, const std::string& ext)
   {
      mLibDir = boost::filesystem::path(dir, boost::filesystem::native);
      mLibExt = ext;
      rescan();
   }

   /**
    * Returns the current list of shared libraries found in the library
    * directory.  The returned list may be empty.  All vpr::LibraryPtr objects
    * will be in a state where the library has not actually been loaded.  They
    * are simply objects that encapsulate the shared library that could be
    * loaded and used by the caller.
    *
    * @pre The library directory has already been scanned.
    */
   const LibraryList& getLibraries() const
   {
      return mLibList;
   }

   /**
    * Performs a fresh search of the current library directory for files ending
    * in a platform-specific shared library extension (.so, .dll, etc.).  Each
    * time this method is invoked, the directory will be rescanned to account
    * for asynchronous changes to the file system, changes to the name of the
    * searched directory, or changes to the shared library file extension
    * string.  As a result, calling this method over and over will cause a lot
    * of disk activity.
    */
   void rescan();

private:
   boost::filesystem::path mLibDir;  /**< The path object (directory) to be searched. */
   std::string             mLibExt;  /**< The file extension used on the libraries. */

   LibraryList mLibList; /**< The list of libraries found in mLibDir with the extension mLibExt. */
};

} // End of vpr namespace


#endif /* _VPR_LIBRARY_FINDER_H_ */
