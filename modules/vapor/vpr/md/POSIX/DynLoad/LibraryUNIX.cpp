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
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <dlfcn.h>
#include <stdlib.h>
#include <sstream>
#include <boost/concept_check.hpp>

#include <vpr/DynLoad/LibraryException.h>
#include <vpr/IO/IOException.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vpr/md/POSIX/DynLoad/LibraryUNIX.h>


namespace vpr
{

LibraryUNIX::~LibraryUNIX()
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

void LibraryUNIX::load()
{
   if ( ! mName.empty() )
   {
      mLibrary = dlopen(mName.c_str(), RTLD_NOW | RTLD_GLOBAL);
   }
   else
   {
      mLibrary = dlopen(NULL, RTLD_NOW | RTLD_GLOBAL);
   }

   if ( NULL == mLibrary )
   {
      std::ostringstream msg_stream;
      msg_stream << "Failed to load '" << mName << "': " << dlerror();
      throw vpr::IOException(msg_stream.str(), VPR_LOCATION);
   }
}

void LibraryUNIX::unload()
{
   if ( NULL == mLibrary )
   {
      throw vpr::IOException("No library to unload", VPR_LOCATION);
   }

   if ( dlclose(mLibrary) != 0 )
   {
      std::ostringstream msg_stream;
      msg_stream << "Failed to unload '" << mName << "': " << dlerror();
      throw vpr::IOException(msg_stream.str(), VPR_LOCATION);
   }
   else
   {
      mLibrary = NULL;
   }
}

void* LibraryUNIX::findSymbol(const char* symbolName)
{
   // If the library has not been loaded yet, throw an exception.
   if ( NULL == mLibrary )
   {
      std::ostringstream msg_stream;
      msg_stream << "Library not loaded: '" << mName << "'";
      throw vpr::LibraryException(msg_stream.str(), VPR_LOCATION);
   }

   return dlsym(mLibrary, symbolName);
}

void* LibraryUNIX::findSymbolAndLibrary(const char* symbolName,
                                        LibraryUNIX& lib)
{
   boost::ignore_unused_variable_warning(symbolName);
   boost::ignore_unused_variable_warning(lib);
   vprASSERT(false && "Not implemented yet");
   return NULL;
}

} // End of vpr namespace
