/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <vpr/vprConfig.h>

#include <boost/filesystem/operations.hpp>
#include <boost/version.hpp>

#include <vpr/Util/Debug.h>
#include <vpr/DynLoad/LibraryFinder.h>

namespace fs = boost::filesystem;


namespace vpr
{

void LibraryFinder::rescan()
{
   // Wipe out the current list of known libraries.  This will cause the
   // memory for each to be deleted, thereby causing the destructor for
   // vpr::Library to be invoked.
   mLibList.clear();

   if ( fs::is_directory(mLibDir) )
   {
      fs::directory_iterator end_itr;

      for ( fs::directory_iterator file(mLibDir); file != end_itr; ++file )
      {
#if BOOST_VERSION < 103400
         // Ignore directories.  Normal files and symlinks are fine.
         if ( ! fs::is_directory(*file) )
         {
            // Construct a substring of file->leaf() that contains only the
            // file extension.  We require that the file we will match have
            // names that end with mLibExt.
            const std::string::size_type pos = file->leaf().size() - mLibExt.size();
            const std::string file_ext = file->leaf().substr(pos);

            if ( file_ext == mLibExt )
            {
               mLibList.push_back(vpr::LibraryPtr(new vpr::Library(file->native_file_string())));
            }
         }
#else
         // Ignore directories.  Normal files and symlinks are fine.
         if ( ! fs::is_directory(file->status()) )
         {
            // Construct a substring of file->path.leaf() that contains only
            // the file extension. We require that the file we will match have
            // names that end with mLibExt.
            const std::string::size_type pos =
               file->path().leaf().size() - mLibExt.size();
            const std::string file_ext = file->path().leaf().substr(pos);

            if ( file_ext == mLibExt )
            {
               mLibList.push_back(
                  vpr::LibraryPtr(
                     new vpr::Library(file->path().native_file_string())
                  )
               );
            }
         }
#endif
      }
   }
   else
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR") << ": " << mLibDir.native_file_string()
         << " is not a directory\n" << vprDEBUG_FLUSH;
   }
}

} // End of vpr namespace
