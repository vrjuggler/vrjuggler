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

#ifndef _JCCL_PARSEUTIL_H_
#define _JCCL_PARSEUTIL_H_

#include <jccl/jcclConfig.h>
#include <vector>
#include <string>


namespace jccl
{

/** \class ParseUtil ParseUtil.h jccl/Config/ParseUtil.h
 *
 * Utility functions for handling the parsing of input files.
 */
class JCCL_CLASS_API ParseUtil
{
public:
   /**
    * Expands a filename to a string capable of being passed to an I/O system
    * call.  The result may be an absolute path depending on various factors.
    *
    * @post All environment variables used in name are expanded.  If name is
    *       not an absolute path and parentFile is not an empty string, the
    *       path of parentFile will be prepended to the result.  If parentFile
    *       is an empty string, the search path will be used to try to find
    *       the named file.
    *
    * @param name       The name to expand.
    * @param parentFile The name of the file that n is being loaded
    *                   relative to.
    *
    * @return An expanded version of the string name.  All environment
    *
    * @note parentFile is always assumed to refer to a file, so if name is
    *       "included_file" and parentFile is "mydir/including_file", the
    *       result will be "mydir/included_file".
    */
   static std::string expandFileName(const std::string& name,
                                     const std::string& parentFile);

   /**
    * Replaces the current configuration file search path with the given
    * string.
    *
    * @param path A string of directories separted by a platform-specific
    *             path separation character.  On Windows, this character is
    *             ';', and on UNIX and Mac OS X, this character is ':'.
    */
   static void setCfgSearchPath(const std::string& path);

private:
   /**
    * Looks for a file in a path and opens it if found.
    *
    * @param fileName     The name of a file to search for.
    * @param absoluteFile Storage for the absolute path to the file if it was
    *                     found in the path.
    *
    * @return \c true is returned if a file is found in the path; \c false
    *         otherwise.
    */
   static bool findFileUsingPath(const std::string& fileName,
                                 std::string& absoluteFile);

   /**
    * Simple helper type to ensure that the static data in this class is
    * initialized at the same time.
    */
   struct SearchPathData
   {
      SearchPathData();

      void setSearchPath(const std::string& path);

      std::vector<std::string> mPath;
   };

   static SearchPathData mSearchInfo;
};

} // End of jccl namespace

#endif
