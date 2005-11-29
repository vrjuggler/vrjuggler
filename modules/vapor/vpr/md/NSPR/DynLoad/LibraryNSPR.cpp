/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <stdlib.h>
#include <sstream>

#include <vpr/IO/IOException.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vpr/md/NSPR/NSPRHelpers.h>
#include <vpr/md/NSPR/DynLoad/LibraryNSPR.h>


namespace vpr
{

LibraryNSPR::~LibraryNSPR() throw ()
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

void LibraryNSPR::load()
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

   mLibrary = PR_LoadLibrary(mName.c_str());

   if ( NULL == mLibrary )
   {
      std::ostringstream msg_stream;
      vpr::NSPR_PrintError("Failed to load library -- ", msg_stream);
      throw vpr::IOException(msg_stream.str(), VPR_LOCATION);
   }
}

void LibraryNSPR::unload()
{
   if ( NULL == mLibrary )
   {
      throw vpr::IOException("No library to unload", VPR_LOCATION);
   }

   if ( PR_UnloadLibrary(mLibrary) == PR_FAILURE )
   {
      std::ostringstream msg_stream;
      vpr::NSPR_PrintError("Failed to unload library -- ", msg_stream);
      throw vpr::IOException(msg_stream.str(), VPR_LOCATION);
   }
   else
   {
      mLibrary = NULL;
   }
}

} // End of vpr namespace
