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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <stdlib.h>

#include <vpr/Util/Assert.h>
#include <vpr/md/NSPR/NSPRHelpers.h>
#include <vpr/md/NSPR/DynLoad/LibraryNSPR.h>


namespace vpr
{

vpr::ReturnStatus LibraryNSPR::load()
{
   vpr::ReturnStatus status;

   // Make sure a library object has not already been loaded.
   if ( NULL != mLibrary )
   {
      vpr::ReturnStatus unload_status;
      unload_status = unload();
      vprASSERT(unload_status.success() && "Library unload failed");
   }

   mLibrary = PR_LoadLibrary(mName.c_str());

   if ( NULL == mLibrary )
   {
      vpr::NSPR_PrintError("WARNING: Failed to load library -- ");
      status.setCode(vpr::ReturnStatus::Fail);
      // XXX: Print error message.
   }

   return status;
}

vpr::ReturnStatus LibraryNSPR::unload()
{
   vprASSERT(mLibrary != NULL && "Library not loaded");
   vpr::ReturnStatus status;

   if ( PR_UnloadLibrary(mLibrary) == PR_FAILURE )
   {
      vpr::NSPR_PrintError("WARNING: Failed to unload library -- ");
      status.setCode(vpr::ReturnStatus::Fail);
   }
   else
   {
      mLibrary = NULL;
   }

   return status;
}

} // End of vpr namespace
