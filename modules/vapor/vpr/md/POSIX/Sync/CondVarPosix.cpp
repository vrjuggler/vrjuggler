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
#include <sys/time.h>

#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>
#include <vpr/md/POSIX/Sync/CondVarPosix.h>


namespace vpr
{

vpr::ReturnStatus CondVarPosix::wait(vpr::Interval timeToWait)
{
   // ASSERT:  We have been locked
   vpr::ReturnStatus status;

   // If not locked ...
   if ( mCondMutex->test() == 0 )
   {
      std::cerr << "[vpr::CondVarPosix::wait()] INCORRECT USAGE: Mutex was not "
                << "locked when wait invoked!!!\n";

      status.setCode(vpr::ReturnStatus::Fail);
   }
   else
   {
      // The mutex variable must be locked when passed to
      // pthread_cond_wait().
      if ( vpr::Interval::NoTimeout == timeToWait )
      {
         int retcode = pthread_cond_wait(&mCondVar, &(mCondMutex->mMutex));
         if ( retcode != 0 )
         {
#ifdef HAVE_STRERROR_R
            char error_str[255];
            strerror_r(retcode, error_str, 254);
#else
            char* error_str = strerror(retcode);
#endif
            vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
               << "[vpr::CondVarPosix::wait()] Unexpected error: "
               << error_str << std::endl << vprDEBUG_FLUSH;
            status.setCode(vpr::ReturnStatus::Fail);
         }
      }
      else
      {
         struct timeval  now;                // The current time
         struct timespec abs_timeout;        // The absolute time of the timeout

         const vpr::Uint64 UsecsPerSec(1000000);

         // Calculate the absolute time for wait timeout
         gettimeofday(&now, NULL);

         // - Calculate the amount of usecs left as fractional seconds from
         //   wait time
         // - Calcualte the absolute time number of usecs
         // - If greater then a second then addon on those seconds to abs
         //   seconds
         // - Add on the waiting secons and the current time in seconds
         vpr::Uint64 abs_secs(0);
         const vpr::Uint64 left_over_usecs = timeToWait.usec() % UsecsPerSec;
         vpr::Uint64 abs_usecs = now.tv_usec + left_over_usecs;
         if(abs_usecs > UsecsPerSec)    // Have extra seconds
         {
            abs_secs = abs_usecs / UsecsPerSec;    // Get the number of seconds
            abs_usecs = abs_usecs % UsecsPerSec;
         }
         abs_secs += (now.tv_sec + timeToWait.sec());

         abs_timeout.tv_sec = abs_secs;
         abs_timeout.tv_nsec = 1000 * (abs_usecs);
         vprASSERT(abs_timeout.tv_nsec < vpr::Int64(UsecsPerSec * 1000) &&
                   "Nano seconds out of range (greater then one second).");

         int retcode = pthread_cond_timedwait(&mCondVar, &(mCondMutex->mMutex),
                                              &abs_timeout);

         if(0 == retcode)
         {
            status.setCode(vpr::ReturnStatus::Succeed);
         }
         else if(ETIMEDOUT == retcode)
         {
            status.setCode(vpr::ReturnStatus::Timeout);
         }
         else if(EINVAL == retcode)
         {
            status.setCode(vpr::ReturnStatus::Fail);
            vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
               << "[vpr::CondVarPosix::wait()] Invalid value.\n"
               << vprDEBUG_FLUSH;
         }
         else
         {
#ifdef HAVE_STRERROR_R
            char error_str[255];
            strerror_r(retcode, error_str, 254);
#else
            char* error_str = strerror(retcode);
#endif
            vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
               << "[vpr::CondVarPosix::wait()] Unexpected error: "
               << error_str << std::endl << vprDEBUG_FLUSH;
            status.setCode(vpr::ReturnStatus::Fail);
         }
      }
   }

   return status;
}

} // End of vpr namespace
