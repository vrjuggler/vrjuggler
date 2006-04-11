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

// library of file utility functions, useful to IRIS Performer apps

#ifndef PERFORMER_FILEIO_UTILS
#define PERFORMER_FILEIO_UTILS

#include <string>
#include <assert.h>
#include <vpr/Util/FileUtils.h>
#include <vrj/Util/FileIO.h> /* bool fileExists() */
#include <Performer/pf/pfDCS.h>

// use this class to store your filepath etc...
// if you don't use this then the pfFileIO funcs will set the path
// by default to "."
class pfFileIO
{
public:
   enum units
   {
      FEET, METERS, NOCONVERT
   };

   // == "." by default
   static std::string filePath;
   static void setFilePath( const std::string& filepath = ".:" )
   {
      std::string demangled_path = vpr::replaceEnvVars( filepath );
      filePath = demangled_path;
      //std::cout << "pfFileIO::setFilePath: path now: " << filePath << std::endl;
   }

   static void addFilePath( const std::string& filepath)
   {
      std::string demangled_path = vpr::replaceEnvVars( filepath );
      filePath += demangled_path;
      filePath += std::string(":");    // Add on trailing seperator
      //std::cout << "pfFileIO::addFilePath: path now: " << filePath << std::endl;
   }

   //: Load geometry from File
   // trys to make performer load whatever filename is given
   // uses pfFileIO:filePath as the current path
   // uses pfdLoadFile to load the given filename
   // exits application on failure
   // outputs useful stuff, to let you know current status.
   static pfNode* loadFile( const std::string& filename );

   //: Load geometry from File, uses cached version if available
   // use this function just like the pfdLoadFile performer function.
   // this function automatically keeps track of optimised versions of the filename.
   // if there is no .pfb file, or the .pfb is older than filename, then
   // function generates a new pfb file.
   // if there is a current pfb file, then function uses it instead.
   // TODO: add time stamp check - not implemented yet.
   //
   // function will auto scale your model based on the units you provide,
   // default is no conversion.
   static pfNode* autoloadFile(const std::string& fileName, const pfFileIO::units& un );

   //: Write node to File
   // stores the geometry under node, in file originalFltName.pbf
   // NOTE: don't specify the .pfb extension.
   static void writeOptimizedFile( pfNode* node, std::string originalFltName );

   //: get the name of the optimized file given the original name
   static std::string optimizedName( std::string originalFltName );


   //: check the filename.  Does it look like it is already optimized?
   //  returns true if so.
   static bool isOptimized( const std::string& fileName );


   // returns a DCS that will scale an flt model to the suitable size for
   // viewing in a cave environment.
   // setting the units in multigen doesn't seem to do anything, but if anything
   // set them to METERS (in multigen), in VRAC's cave, they use FEET as their main
   // units, so call this function with FEET
   //
   // using the returned DCS addChild() your geometry returned from autoloadFile()
   // then add the DCS to your scene (but not under another one of these!!!)
   static pfDCS* newConversionDCS( const units& un = FEET )
   {
      pfDCS* dcs = new pfDCS();
      switch (un)
      {
         case FEET:
            dcs->setScale( 3.8f );
            break;
         case METERS:
         case NOCONVERT:
         default:
            // do nothing.
            break;
      }

      return dcs;
   }
};



#endif
