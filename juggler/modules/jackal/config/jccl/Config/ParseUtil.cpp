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

#include <jccl/jcclConfig.h>
#include <fstream>
#include <ctype.h>
#include <vpr/System.h>
#include <vpr/Util/FileUtils.h>
#include <jccl/Util/Debug.h>
#include <jccl/Config/ConfigTokens.h>
#include <jccl/Config/ParseUtil.h>

namespace tokens = jccl::types_tokens;

namespace jccl
{

ParseUtil::SearchPathData ParseUtil::mSearchInfo;

#ifdef VPR_OS_Win32
static const std::string sPathSep("\\");
#else
static const std::string sPathSep("/");
#endif

/** Is n an absolute path name? */
static bool isAbsolutePathName(const std::string& n)
{
#ifdef VPR_OS_Win32
   return((n.length() > 0) && (n[0] == '\\'))
      || ((n.length() > 2) && (n[1] == ':') && (n[2] == '\\'));
#else
   return(n.length() > 0) && (n[0] == '/');
#endif
}

std::string ParseUtil::expandFileName(const std::string& n,
                                      const std::string& parentfile)
{
   std::string fname = vpr::replaceEnvVars(n);

   if ( ! isAbsolutePathName(fname) )
   {
      // If the parentfile string is empty, we'll use the search path to find
      // the file.
      if ( parentfile.length() == 0 )
      {
         vprDEBUG(jcclDBG_CONFIG, vprDBG_STATE_LVL)
            << "jccl::ParseUtil::expandFileName(): Looking up '" << n
            << "' using search path ...\n" << vprDEBUG_FLUSH;
         findFileUsingPath(n, fname);
      }
      else
      {
         // It's a relative pathname, so we have to add in the path part
         // of parentfile.
         int lastslash(0);
         for ( unsigned int i = 0; i < parentfile.length(); ++i )
         {
            if ( parentfile[i] == sPathSep[0] )
            {
               lastslash = i;
            }
         }

         if ( lastslash )
         {
            std::string s(parentfile, 0, lastslash + 1);
            fname = s + n;
         }
      }
   }

   return fname;
}

void ParseUtil::setCfgSearchPath(const std::string& path)
{
   mSearchInfo.setSearchPath(path);
}

vpr::ReturnStatus ParseUtil::findFileUsingPath(const std::string& fileName,
                                               std::string& absoluteFile)
{
   vpr::ReturnStatus status(vpr::ReturnStatus::Fail);
   std::ifstream in;

   for ( std::vector<std::string>::iterator i = mSearchInfo.mPath.begin();
         i != mSearchInfo.mPath.end();
         ++i )
   {
      // Clear the flags on in so that we can try opening a new file.
      in.clear();

      std::string full_path(*i + sPathSep + fileName);

      vprDEBUG(jcclDBG_CONFIG, vprDBG_HVERB_LVL)
         << "jccl::ParseUtil::findFileUsingPath(): Attempting to open file '"
         << full_path << "'\n" << vprDEBUG_FLUSH;

      // Try to open the file name constructed above.
      in.open(full_path.c_str());

      if ( in )
      {
         absoluteFile = full_path;
         status.setCode(vpr::ReturnStatus::Succeed);
         in.close();
         break;
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

   if ( vpr::System::getenv(jccl_search_env, search_path).success() )
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
// Define the separator character for the elements of the path.  On Win32, we
// use ";", and on everything else, we use ":".
#ifdef VPR_OS_Win32
      char elem_sep(';');
      std::string ostype;

      // If we are in a Cygwin environment, use ":" as the element separator.
      if ( vpr::System::getenv("OSTYPE", ostype).success() )
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
