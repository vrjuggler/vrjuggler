/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#ifndef _VRJ_FILE___IO_STUFF
#define _VRJ_FILE___IO_STUFF

#include <vrj/vrjConfig.h>

#include <stdlib.h>
#include <string>
#include <vector>


namespace vrj
{


class VJ_CLASS_API FileIO
{
public:
   // == "." by default
   // this is in the form of:
   //     /home/users/me
   // or: ../../hi-man
   // or: ./rgbfiles
   // or: ${HOME}/VR/juggler
   // NOTE: you can use ${VARIABLE} in the path, to use env variables...
   static void addFilePath( const std::string& filepath = "." );

   /** filename handling routines **/

   //: Returns a copy of s with all environment variable names replaced
   //+ with their values.
   // if ${...} not found... then replaces it with "", and emits error
   // message to console.
   static std::string replaceEnvVars( const std::string& s);

   //: is n an absolute path name?
   // examples of one are:
   // ${HOME}/filename
   // /home/vr/filename
   static bool isAbsolutePathName( const std::string& n );

   // n is your filename
   // returns n with all ${...} names replaced with their env vars.
   // if ${...} not found... then replaces it with "", and emits error
   // message to console.
   static std::string demangleFileName( const std::string& n, std::string parentfile );

   // check if file exists... does a demangle on the name before checking
   static bool fileExists( const char* const name );
   static bool fileExists( const std::string& name );

   // check if file exists in any of the registered paths
   // returns true if exists and the resolved filepath
   static bool fileExistsResolvePath( const char* const filename, std::string& realPath );
   static bool fileExistsResolvePath( const std::string& filename, std::string& realPath );

   // TODO: remove this function, and move it's code into fileExistsResolvePath
   static std::string resolvePathForName( const char* const filename );

   static std::vector<std::string> mPaths;
};


};


#endif
