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

#include <cstdlib>
#include <sstream>
#include <boost/concept_check.hpp>

#include <vpr/DynLoad/LibraryException.h>
#include <vpr/IO/IOException.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Error.h>
#include <vpr/md/WIN32/DynLoad/LibraryWin32.h>


namespace vpr
{

LibraryWin32::LibraryWin32(const std::string& name)
   : mName(name)
   , mLibrary(NULL)
{
   ;
}

LibraryWin32::LibraryWin32()
   : mName("")
   , mLibrary(NULL)
{
   ;
}

LibraryWin32::LibraryWin32(const LibraryWin32& lib)
   : mName("")
   , mLibrary(NULL)
{
   copy(lib);
}

LibraryWin32::~LibraryWin32()
{
   if ( NULL != mLibrary )
   {
      try
      {
         unload();
      }
      catch (vpr::IOException& ex)
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Failed to unload library during destruction" << std::endl
            << ex.what() << std::endl << vprDEBUG_FLUSH;
      }
   }
}

void LibraryWin32::load()
{
   // Make sure a library object has not already been loaded.
   if ( NULL != mLibrary )
   {
      try
      {
         unload();
      }
      catch (vpr::IOException& ex)
      {
         std::ostringstream msg_stream;
         msg_stream << "Library unload failed during loading:\n"
                    << ex.getExtendedDescription();
         throw vpr::IOException(msg_stream.str(), VPR_LOCATION);
      }
   }

   mLibrary = LoadLibrary(mName.c_str());

   if ( NULL == mLibrary )
   {
      std::ostringstream msg_stream;
      msg_stream << "Failed to load '" << mName << "': "
                 << vpr::Error::getCurrentErrorMsg();
      throw vpr::IOException(msg_stream.str(), VPR_LOCATION);
   }
}

void LibraryWin32::unload()
{
   if ( NULL == mLibrary )
   {
      throw vpr::IOException("No library to unload", VPR_LOCATION);
   }

   if ( FreeLibrary(mLibrary) != 0 )
   {
      std::ostringstream msg_stream;
      msg_stream << "Failed to unload '" << mName << "': "
                 << vpr::Error::getCurrentErrorMsg();
      throw vpr::IOException(msg_stream.str(), VPR_LOCATION);
   }
   else
   {
      mLibrary = NULL;
   }
}

void* LibraryWin32::findSymbol(const std::string& symbolName)
{
   // If the library has not been loaded yet, throw an exception.
   if ( NULL == mLibrary )
   {
      std::ostringstream msg_stream;
      msg_stream << "Library not loaded: '" << mName << "'";
      throw vpr::LibraryException(msg_stream.str(), VPR_LOCATION);
   }

   return GetProcAddress(mLibrary, symbolName.c_str());
}

void* LibraryWin32::findSymbolAndLibrary(const std::string& symbolName,
                                         LibraryWin32& lib)
{
   boost::ignore_unused_variable_warning(symbolName);
   boost::ignore_unused_variable_warning(lib);
   vprASSERT(false && "Not implemented yet");
   return NULL;
}

} // End of vpr namespace
