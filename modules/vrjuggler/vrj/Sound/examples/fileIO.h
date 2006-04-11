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

#ifndef FILE___IO_STUFF
#define FILE___IO_STUFF

#include <stdlib.h>
#include <string>
#include <vector>

// use this class to store your filepath etc...
// if you don't use this then the pfFileIO funcs will set the path
// by default to "."
class fileIO
{
public:
   // == "." by default
   // this is in the form of: 
   //     /home/users/me
   // or: ../../hi-man
   // or: ./rgbfiles
   static void addFilePath( const std::string& filepath = "." )
   {
      mPaths.push_back( filepath );
   }

   static bool fileExists( const char* const name );
   static bool fileExists( const std::string& name );
   static bool fileExistsResolvePath( const char* const filename, std::string& realPath );
   static bool fileExistsResolvePath( const std::string& filename, std::string& realPath );
   static std::string resolvePathForName( const char* const filename );
   static std::vector<std::string> mPaths;  
};




#endif
