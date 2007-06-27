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

#include <cstring>
#include <sstream>

#include <vpr/Util/ResourceException.h>
#include <vpr/md/POSIX/Sync/MutexPosix.h>


namespace vpr
{

MutexPosix::MutexPosix()
{
   int result(0);

   // Initialize the mutex.
#ifndef VPR_DEBUG
   result = pthread_mutex_init(&mMutex, NULL);
#else
#ifdef VPR_OS_Linux
   // If Linux and debug, then use error checking mutex
   pthread_mutexattr_t mutex_attr;
   pthread_mutexattr_init(&mutex_attr);
   pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK_NP);
   result = pthread_mutex_init(&mMutex, &mutex_attr);
   pthread_mutexattr_destroy(&mutex_attr);
#else
   result = pthread_mutex_init(&mMutex, NULL);
#endif
#endif

   if ( result != 0 )
   {
      std::ostringstream msg_stream;
      msg_stream << "Mutex allocation failed: " << std::strerror(result);
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }
}

// Tests the current lock status.
bool MutexPosix::test() const
{
   bool locked(true);

   const int status =
      pthread_mutex_trylock(const_cast<pthread_mutex_t*>(&mMutex));

   // If the return value from pthread_mutex_trylock(3) is 0, then this thread
   // now has a lock on mMutex. Therefore, no other thread could have held the
   // mutex, so unlock the mutex and return false.
   if ( status == 0 )
   {
      pthread_mutex_unlock(const_cast<pthread_mutex_t*>(&mMutex));
      locked = false;
   }

   return locked;
}

} // End of vpr namespace
