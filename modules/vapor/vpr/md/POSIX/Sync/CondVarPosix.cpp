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

#include <sstream>
#include <cstring>
#include <sys/time.h>

#include <vpr/Util/Assert.h>
#include <vpr/Util/ResourceException.h>
#include <vpr/Util/IllegalArgumentException.h>
#include <vpr/md/POSIX/Sync/CondVarPosix.h>


namespace vpr
{

CondVarPosix::CondVarPosix(MutexPosix* mutex)
   : mCondMutex(NULL)
{
   // Initialize the condition variable.
   const int result = pthread_cond_init(&mCondVar, NULL);

   if ( result != 0 )
   {
      std::ostringstream msg_stream;
      msg_stream << "Condition variable allocation failed: "
                 << std::strerror(result);
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }

   // If the caller did not specify a mutex variable to use with
   // the condition variable, use mDefaultMutex.
   if ( mutex == NULL )
   {
      mutex = &mDefaultMutex;
   }

   mCondMutex = mutex;
}

bool CondVarPosix::wait(const vpr::Interval& timeToWait)
{
   bool result(true);

   // If not locked ...
   if ( ! mCondMutex->test() )
   {
      std::ostringstream msg_stream;
      msg_stream << "Condition variable mutex must be locked before calling "
                 << "wait()";
      throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
   }
   // The mutex variable must be locked when passed to pthread_cond_wait().
   else
   {
      // Wait indefinitely on the condition variable.
      if ( vpr::Interval::NoTimeout == timeToWait )
      {
         const int retcode = pthread_cond_wait(&mCondVar, &mCondMutex->mMutex);

         // If pthread_cond_wait(3) returned non-zero status, then we throw
         // an exception. Otherwise, we return true.
         if ( retcode != 0 )
         {
#ifdef HAVE_STRERROR_R
            char error_str[255];
            strerror_r(retcode, error_str, 254);
#else
            char* error_str = std::strerror(retcode);
#endif
            std::ostringstream msg_stream;
            msg_stream << "Unexpected error in vpr::CondVarPosix::wait(): "
                       << error_str;
            throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
         }
      }
      // Wait for no longer than the given timeout period to acquire the lock
      // on the condition variable.
      else
      {
         struct timeval  now;                // The current time
         struct timespec abs_timeout;        // The absolute time of the timeout

         const vpr::Uint64 UsecsPerSec(1000000);

         // Calculate the absolute time for wait timeout
         gettimeofday(&now, NULL);

         // - Calculate the amount of microseconds left as fractional seconds
         //   from wait time.
         // - Calcualte the absolute time number of microseconds.
         // - If greater then a second, then add on on those seconds to abs
         //   seconds.
         // - Add on the waiting seconds and the current time in seconds.
         vpr::Uint64 abs_secs(0);
         const vpr::Uint64 left_over_usecs = timeToWait.usec() % UsecsPerSec;
         vpr::Uint64 abs_usecs = now.tv_usec + left_over_usecs;
         if ( abs_usecs > UsecsPerSec )    // Have extra seconds
         {
            abs_secs = abs_usecs / UsecsPerSec;    // Get the number of seconds
            abs_usecs = abs_usecs % UsecsPerSec;
         }
         abs_secs += (now.tv_sec + timeToWait.sec());

         abs_timeout.tv_sec = abs_secs;
         abs_timeout.tv_nsec = 1000 * (abs_usecs);
         vprASSERT(abs_timeout.tv_nsec < vpr::Int64(UsecsPerSec * 1000) &&
                   "Nano seconds out of range (greater then one second).");

         const int retcode = pthread_cond_timedwait(&mCondVar,
                                                    &mCondMutex->mMutex,
                                                    &abs_timeout);

         // Successful completion: return true.
         if ( 0 == retcode )
         {
            result = true;
         }
         // Timeout: return false.
         else if ( ETIMEDOUT == retcode )
         {
            result = false;
         }
         // Throw an exception.
         else
         {
            // Get the error description to include with the exception
            // message.
#ifdef HAVE_STRERROR_R
            char error_str[255];
            strerror_r(retcode, error_str, 254);
#else
            char* error_str = std::strerror(retcode);
#endif

            // Invalid argument passed to pthread_cond_timedwait(3).
            if ( EINVAL == retcode )
            {
               std::ostringstream msg_stream;
               msg_stream << "Invalid value error: " << error_str;
               throw vpr::IllegalArgumentException(msg_stream.str(),
                                                   VPR_LOCATION);
            }
            // Some other error.
            else
            {
               std::ostringstream msg_stream;
               msg_stream << "Unexpected error: " << error_str;
               throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
            }
         }
      }
   }

   return result;
}

} // End of vpr namespace
