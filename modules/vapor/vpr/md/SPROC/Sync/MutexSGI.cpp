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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <string.h>
#include <sstream>

#include <vpr/Util/ResourceException.h>
#include <vpr/md/SPROC/Sync/MutexSGI.h>


vpr::MemPoolSGI* vpr::MutexSGI::mMutexPool = NULL;
int* vpr::MutexSGI::mAttachedCounter       = NULL;

namespace vpr
{

MutexSGI::MutexSGI()
   : mMutex(NULL)
{
   // BUG: Possible race condition here
   if ( mMutexPool == NULL )
   {
      mMutexPool = new MemPoolSGI(65536, 32,
                                  "/var/tmp/memMutexPoolSGIXXXXXX");
      mAttachedCounter = static_cast<int*>(mMutexPool->allocate(sizeof(int)));
      *mAttachedCounter = 0;
   }

   // Track how many mutexes are allocated
   *mAttachedCounter = *mAttachedCounter + 1;
//      vprDEBUG << " vpr::MutexSGI::MutexSGI: mAttachedCounter: "
//               << *mAttachedCounter << endl << vprDEBUG_FLUSH;

   // ----- Allocate the mutex ----- //
   mMutex = usnewlock(mMutexPool->getArena());

   if ( NULL == mMutex )
   {
      std::ostringstream msg_stream;
      msg_stream << "Failed to allocate new mutex: " << strerror(errno);
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }
}

MutexSGI::~MutexSGI()
{
   // ---- Delete the mutex --- //
   usfreelock(mMutex, mMutexPool->getArena());

   // ---- Deal with the pool --- //

   // Track how many mutexes are allocated
   *mAttachedCounter = *mAttachedCounter - 1;

//      vprDEBUG << "vpr::MutexSGI::~MutexSGI: mAttachedCounter: "
//               << *mAttachedCounter << endl << vprDEBUG_FLUSH;

   if ( *mAttachedCounter == 0 )
   {
      mMutexPool->deallocate(mAttachedCounter);
      mAttachedCounter = NULL;
      delete mMutexPool;
      mMutexPool = NULL;
   }
}

}
