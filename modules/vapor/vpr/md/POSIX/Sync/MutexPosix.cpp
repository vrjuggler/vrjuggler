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

#include <vpr/md/POSIX/Sync/MutexPosix.h>

namespace vpr
{

MutexPosix::MutexPosix()
{
   // Initialize the mutex.
#ifndef _DEBUG
   pthread_mutex_init(&mMutex, NULL);
#else
#ifdef VPR_OS_Linux
   // If Linux and debug, then use error checking mutex
   pthread_mutexattr_t mutex_attr;
   pthread_mutexattr_init(&mutex_attr);
   pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK_NP);
   pthread_mutex_init(&mMutex, &mutex_attr);
   pthread_mutexattr_destroy(&mutex_attr);
#else
   pthread_mutex_init(&mMutex, NULL);
#endif
#endif
}

// Tests the current lock status.
int MutexPosix::test() const
{
   int ret_val;

   ret_val = pthread_mutex_trylock(const_cast<pthread_mutex_t*>(&mMutex));

   // If the return value from pthread_mutex_trylock() is 0, then this
   // process now has a lock on mutex.  Therefore, no other process could
   // have held a lock on it, so unlock the mutex and return 0.
   if ( ret_val == 0 )
   {
      pthread_mutex_unlock(const_cast<pthread_mutex_t*>(&mMutex));
      ret_val = 0;
   }
   // The mutex is currently locked by some thread if ret_val is non-zero.
   else
   {
      ret_val = 1;
   }

   return ret_val;
}

} // End of vpr namespace
