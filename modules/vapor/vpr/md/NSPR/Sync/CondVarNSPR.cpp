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

#include <sstream>

#include <vpr/Util/ResourceException.h>
#include <vpr/md/NSPR/NSPRHelpers.h>
#include <vpr/md/NSPR/Sync/CondVarNSPR.h>


namespace vpr
{

CondVarNSPR::CondVarNSPR(MutexNSPR* mutex)
   : mCondVar(NULL)
   , mCondMutex(NULL)
{
   // If the caller did not specify a mutex variable to use with
   // the condition variable, use mDefaultMutex.
   if ( mutex == NULL )
   {
      mCondMutex = &mDefaultMutex;
   }
   else
   {
      mCondMutex = mutex;
   }

   // Initialize the condition variable.
   mCondVar = PR_NewCondVar(mCondMutex->mMutex);

   if ( NULL == mCondVar )
   {
      std::ostringstream msg_stream;
      NSPR_PrintError("Condition variable allocation failed: ", msg_stream);
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }
}

bool CondVarNSPR::wait(const vpr::Interval& timeToWait)
{
   const PRStatus status = PR_WaitCondVar(mCondVar,
                                          NSPR_getInterval(timeToWait));

   // XXX: How do we deal with a timeout?
   if ( PR_SUCCESS != status )
   {
      std::ostringstream msg_stream;
      NSPR_PrintError("Unexpected error: ", msg_stream);
      throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
   }

   return true;
}

}
