/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#include <jccl/jcclConfig.h>

#include <fstream>
#include <ctype.h>
#include <boost/version.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

#include <vpr/System.h>
#include <vpr/Util/FileUtils.h>
#include <jccl/Util/Debug.h>
#include <jccl/Config/ConfigTokens.h>
#include <jccl/Config/ParseUtil.h>

namespace fs     = boost::filesystem;
namespace tokens = jccl::types_tokens;

namespace jccl
{

ParseUtil::SearchPathData ParseUtil::mSearchInfo;

std::string ParseUtil::expandFileName(const std::string& name,
                                      const std::string& parentfile)
{
   const std::string fname = vpr::replaceEnvVars(name);

   fs::path fname_path(
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
      fname
#else
      fname, fs::native
#endif
   );

   // If the given file name does not exist, try searching for it.
   if ( ! fs::exists(fname_path) )
   {
      // If the given file name is not absolute, look it up relative to
      // parentfile or using the config file search path.
      if ( ! fname_path.is_complete() )
      {
         // If the parentfile string is empty, we'll use the search path to
         // find the file.
         if ( parentfile.empty() )
         {
            vprDEBUG(jcclDBG_CONFIG, vprDBG_STATE_LVL)
               << "[jccl::ParseUtil::expandFileName()] Looking up '" << fname
               << "' using search path ...\n" << vprDEBUG_FLUSH;

            std::string absolute_fname;
            if ( findFileUsingPath(fname, absolute_fname) )
            {
               fname_path =
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
                  fs::path(absolute_fname);
#else
                  fs::path(absolute_fname, fs::native);
#endif
            }
         }
         // parentfile is not empty, so we assume that fname is relative to
         // parentfile.
         else
         {
            const fs::path parentfile_path(
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
               parentfile
#else
               parentfile, fs::native
#endif
            );
            fs::path parentdir = parentfile_path.branch_path();
            fname_path = parentdir / fname_path;
         }
      }
   }

#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
   return fname_path.string();
#else
   return fname_path.native_file_string();
#endif
}

void ParseUtil::setCfgSearchPath(const std::string& path)
{
   mSearchInfo.setSearchPath(path);
}

bool ParseUtil::findFileUsingPath(const std::string& fileName,
                                  std::string& absoluteFile)
{
   bool status(false);
   const fs::path filename_path(
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
      fileName
#else
      fileName, fs::native
#endif
   );

   for ( std::vector<std::string>::iterator i = mSearchInfo.mPath.begin();
         i != mSearchInfo.mPath.end();
         ++i )
   {
      fs::path full_path =
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
         fs::path(*i)
#else
         fs::path(*i, fs::native)
#endif
            / filename_path;

      vprDEBUG(jcclDBG_CONFIG, vprDBG_HVERB_LVL)
         << "[jccl::ParseUtil::findFileUsingPath()] Attempting to open file '"
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
         << full_path.string()
#else
         << full_path.native_file_string()
#endif
         << "'\n" << vprDEBUG_FLUSH;

      try
      {
         if ( fs::exists(full_path) && ! fs::is_directory(full_path) )
         {
            status = true;
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
            absoluteFile = full_path.string();
#else
            absoluteFile = full_path.native_file_string();
#endif
            break;
         }
      }
      catch(fs::filesystem_error&)
      {
         /* Ignore the exception. */ ;
      }
   }

   return status;
}

ParseUtil::SearchPathData::SearchPathData()
{
   // By default, initialize the search path using the value (if any) in the
   // environment variable $JCCL_CFG_PATH.
   const std::string jccl_search_env("JCCL_CFG_PATH");
   std::string search_path;

   if ( vpr::System::getenv(jccl_search_env, search_path) )
   {
      this->setSearchPath(search_path);
   }
}

void ParseUtil::SearchPathData::setSearchPath(const std::string& path)
{
   mPath.clear();

   // Ensure that we have a path that is not empty before we try any parsing.
   if ( path.length() > 0 )
   {
// Define the separator character for the elements of the path.  On Windows,
// we use ";", and on everything else, we use ":".
#ifdef VPR_OS_Windows
      char elem_sep(';');
      std::string ostype;

      // If we are in a Cygwin environment, use ":" as the element separator.
      if ( vpr::System::getenv("OSTYPE", ostype) )
      {
         if ( strcmp(ostype.c_str(), "cygwin") == 0 )
         {
            elem_sep = ':';
         }
      }
#else
      char elem_sep(':');
#endif

      std::string::size_type cur_pos(0), old_pos(0);

      while ( true )
      {
         // Find the next occurrence of an element separator.
         cur_pos = path.find(elem_sep, old_pos);

         // If cur_pos is greater than the length of the path, there
         // are no more :'s in the path.
         if ( cur_pos > path.length() )
         {
            // If old_pos is still less than the length of the path,
            // there is one more directory to be read, so set cur_pos
            // to the length of the path string so we can read it.
            // Once it's read, we'll be done.
            if ( old_pos < path.length() )
            {
               cur_pos = path.length();
            }
            // At this point, both old_pos and cur_pos point beyond
            // the end of the path string.
            else
            {
               break;
            }
         }

         // Extract the current directory from the path and point old_sep to
         // be one character past the current position (which points at the
         // character elem_sep).
         std::string cur_entry(path.substr(old_pos, cur_pos - old_pos));

         // Update old_pos for the next pass through this loop.
         old_pos = cur_pos + 1;

         vprDEBUG(jcclDBG_CONFIG, vprDBG_STATE_LVL)
            << "jccl::ParseUtil::SearchPathData::setSearchPath(): Adding '"
            << cur_entry << "' to the search path.\n" << vprDEBUG_FLUSH;

         mPath.push_back(cur_entry);
      }
   }

#ifdef JCCL_DEBUG
   {
      vprDEBUG(jcclDBG_CONFIG, vprDBG_VERB_LVL)
         << "Config file search path is:\n" << vprDEBUG_FLUSH;

      for ( std::vector<std::string>::iterator i = mSearchInfo.mPath.begin();
            i != mSearchInfo.mPath.end();
            ++i )
      {
         vprDEBUG_NEXT(jcclDBG_CONFIG, vprDBG_VERB_LVL) << *i << std::endl
                                                            << vprDEBUG_FLUSH;
      }
   }
#endif
}

} // End of jccl namespace
