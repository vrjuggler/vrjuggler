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
#include <jccl/Config/ParseUtil.h>
#include <jccl/Util/Debug.h>
#include <jccl/Config/ConfigTokens.h>
#include <vpr/System.h>
#include <vpr/Util/FileUtils.h>

namespace jccl
{

VarType stringToVarType(const std::string& str)
{
   if ( str == int_TOKEN)
   {
      return T_INT;
   }
   if ( str == float_TOKEN)
   {
      return T_FLOAT;
   }
   if ( str == bool_TOKEN)
   {
      return T_BOOL;
   }
   if ( str == string_TOKEN)
   {
      return T_STRING;
   }
   if ( str == distance_TOKEN )
   {
      return T_DISTANCE;
   }
   if ( str == chunk_TOKEN )
   {
      return T_CHUNK;
   }
   if ( str == embeddedchunk_TOKEN)
   {
      return T_EMBEDDEDCHUNK;
   }

   return VJ_T_INVALID;
}

std::string typeString(const VarType t)
{
   switch ( t )
   {
      case T_INT:
         return int_TOKEN;
      case T_BOOL:
         return bool_TOKEN;
      case T_FLOAT:
         return float_TOKEN;
      case T_STRING:
         return string_TOKEN;
      case T_CHUNK:
         return chunk_TOKEN;
      case T_EMBEDDEDCHUNK:
         return embeddedchunk_TOKEN;
      default:
         return "Unrecognized_Type";
   }
}

/** Is n an absolute path name? */
bool isAbsolutePathName(const std::string& n)
{
#ifdef WIN32
   return((n.length() > 0) && (n[0] == '\\'))
      || ((n.length() > 2) && (n[1] == ':') && (n[2] == '\\'));
#else
   return(n.length() > 0) && (n[0] == '/');
#endif
}

std::string demangleFileName(const std::string& n, std::string parentfile)
{
   std::string fname = vpr::replaceEnvVars(n);

   if ( ! isAbsolutePathName(fname) )
   {
      // it's a relative pathname... so we have to add in the path part
      // of parentfile...
      int lastslash(0);
      for ( unsigned int i = 0; i < parentfile.length(); ++i )
      {
#ifdef VPR_OS_Win32
         if ( parentfile[i] == '\\' )
         {
            lastslash = i;
         }
#else
         if ( parentfile[i] == '/' )
         {
            lastslash = i;
         }
#endif
      }

      if ( lastslash )
      {
         std::string s(parentfile, 0, lastslash + 1);
         fname = s + n;
      }
   }

   return fname;
}

JCCL_IMPLEMENT(vpr::ReturnStatus) findFileUsingPathVar (const std::string& file_name,
                                                        const std::string& env_name,
                                                        std::string& absolute_file)
{
   // based on patrick's code to support VJ_CFG_PATH
   // if we find file_name on any path in the variable env_var, we'll
   // open it.  someone calling this function should check in to see
   // if it's an open file afterwards.

   bool found = false;
   std::string path_string;
   vpr::ReturnStatus status(vpr::ReturnStatus::Fail);

   // Read the value in the config path environment variable.
   if ( vpr::System::getenv(env_name, path_string).success() )
   {
      vprDEBUG(jcclDBG_CONFIG, vprDBG_STATE_LVL)
         << "Falling back on " << env_name << ": '" << path_string << "'\n"
         << vprDEBUG_FLUSH;
   }

   // If the user set a value for $VJ_CFG_PATH, parse it, baby!
   if ( path_string.length() > 0 )
   {
      std::string::size_type cur_pos = 0, old_pos = 0;
      std::string full_path;

// Define the separator character for the elements of $VJ_CFG_PATH.  On Win32,
// we use ";", and on everything else, we use ":".
#ifdef VPR_OS_Win32
      char elem_sep = ';';
      std::string ostype;

      // If we are in a Cygwin environment, use ":" as the element
      // separator.
      if ( vpr::System::getenv("OSTYPE", ostype).success() )
      {
         if ( strcmp(ostype.c_str(), "cygwin") == 0 )
         {
            elem_sep = ':';
         }
      }
#else
      char elem_sep = ':';
#endif
      std::ifstream in;

      while ( ! found )
      {
         // Clear the flags on in so that we can try opening a new file.
         in.clear();

         // Find the next occurrence of an element separator.
         cur_pos = path_string.find(elem_sep, old_pos);

         // If cur_pos is greater than the length of the path, there
         // are no more :'s in the path.
         if ( cur_pos > path_string.length() )
         {
            // If old_pos is still less than the length of the path,
            // there is one more directory to be read, so set cur_pos
            // to the length of the path string so we can read it.
            // Once it's read, we'll be done.
            if ( old_pos < path_string.length() )
            {
               cur_pos = path_string.length();
            }
            // At this point, both old_pos and cur_pos point beyond
            // the end of the path string.
            else
            {
               break;
            }
         }

         // Extract the current directory from the path and point
         // old_pos to be one character past the current position
         // (which points at a ':').
         full_path = path_string.substr(old_pos, cur_pos - old_pos);
         old_pos   = cur_pos + 1;

         // Append "/" + file_name to the current directory.
         full_path += "/";
         full_path += file_name;

         vprDEBUG(jcclDBG_CONFIG, vprDBG_STATE_LVL)
            << "ConfigChunkDB::load(): opening file " << full_path
            << "\n" << vprDEBUG_FLUSH;

         // Try to open the file name constructed above.
         in.open(full_path.c_str());

         if ( in )
         {
            found         = true;
            absolute_file = full_path;
            status.setCode(vpr::ReturnStatus::Succeed);
            in.close();
         }
      }
   }

   return status;
}

bool hasSeparator (const std::string &path)
{
   return(path.find(char('/')) != path.npos);
}

std::string getRemainder (const std::string &path)
{
   std::string::size_type i = path.find (char('/'));
   if ( i == path.npos )
   {
      return path;
   }
   else
   {
      return path.substr(i + 1);    // Skip the "/"
   }
}

std::string getFirstNameComponent (const std::string& path)
{
   std::string::size_type i = path.find (char('/'));
   if ( i == path.npos )
   {
      return path;
   }
   else
   {
      return path.substr (0, i);
   }
}



} // End of jccl namespace
