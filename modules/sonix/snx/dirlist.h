/*******************************************************************************
 *
 * xdl - cross platform dynamic library loader
 * xdl is Copyright (c) 2002 Kevin Meinert.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later
 * version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA.
 *
 * -----------------------------------------------------------------------------
 * File:          $RCSfile$
 * Last modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------------------
 *
 ******************************************************************************/
#ifndef DIRLIST_H
#define DIRLIST_H 1

#include <string>    // std::string
#include <vector>    // std::vector

#include <iostream>

#ifndef WIN32
#include <dirent.h>
#else
#include "dirent-win32.h"  // POSIX opendir compatibility wrapper
#endif

namespace xdl
{

   inline bool
   isDir(const char* dir)
   {
      DIR* dirh = NULL;
      
      // try to open the directory to see if it's there
      dirh = ::opendir( dir );
      if ( dirh == NULL ) {
         return false;
      }

      
      ::closedir(dirh);
      return true;
   }
   
   /**
    * Gets a list of the names of the files in the directory located using
    * the given path. The files vector is filled with the filenames.
    *
    * @param dir     the pathname of the directory
    * @param files   cleared and filled with the list of found files if
    *                successful; unmodified on failure
    *
    * @return  true if successful, false otherwise
    */
   inline bool
   dirlist( const char* dir, std::vector<std::string>& files )
   {
      DIR* dirh = NULL;
      struct dirent* entry = NULL;
      
      // try to open the directory to see if it's there
      dirh = ::opendir( dir );
      if ( dirh == NULL ) {
         return false;
      }

      // go through each entry in the directory and add it to our vector
      files.clear();
      entry = ::readdir( dirh );
      while ( entry != NULL ) {
         files.push_back( std::string( entry->d_name ) );
         // get the next entry
         entry = ::readdir( dirh );
      }

      // close the directory. we don't really care about failure here since we
      // know we have a valid DIR handle.
      ::closedir( dirh );
      return true;
   }
}

#endif // ! DIRLIST_H
