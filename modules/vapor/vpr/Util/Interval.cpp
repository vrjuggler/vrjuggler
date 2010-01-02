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
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#if defined(VPR_OS_IRIX)
// these includes are needed for accessing the SGI cycle counter.
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/syssgi.h>
#endif

#if defined(VPR_USE_NSPR)
#  include <prinrval.h>
#endif

#ifdef VPR_SIMULATOR
#  include <vpr/md/SIM/Controller.h>
#endif

#include <vpr/Util/Interval.h>
#include <vpr/System.h>


const vpr::Interval vpr::Interval::NoWait(0,vpr::Interval::Base);
const vpr::Interval vpr::Interval::NoTimeout(0xffffffffUL, vpr::Interval::Base);
const vpr::Interval vpr::Interval::HalfPeriod((0xffffffffUL/2), vpr::Interval::Base);

namespace vpr
{

   // Simulator-only version of vpr::Interval::setNow().
#ifdef VPR_SIMULATOR
void Interval::setNow()
{
   mMicroSeconds = vpr::sim::Controller::instance()->getClock().getCurrentTime().getBaseVal();
}
#else
void Interval::setNow()
{
   setNowReal();
}
#endif /* ifdef VPR_SIMULATOR */

//
// Real implementation of setNow that uses the real clock time from the system
//
void Interval::setNowReal()
{
#if defined(VPR_OS_Windows)
   LARGE_INTEGER count, counts_per_sec;

   QueryPerformanceFrequency(&counts_per_sec);

   // XXX: Optimize this!!
   vpr::Uint64 counts_per_sec64;
   vpr::Uint64 counts_per_sec_high64;
   counts_per_sec_high64 = counts_per_sec.HighPart;
   counts_per_sec64 = counts_per_sec.LowPart;
   counts_per_sec64 += (counts_per_sec_high64 << 32);
   double usecs_per_count =
      (1.0f / (double) ((vpr::Int64) counts_per_sec64)) * 1000000.0f;

   if (QueryPerformanceCounter(&count))
   {
      vpr::Uint64 low  = count.LowPart;
      vpr::Uint64 high = count.HighPart;
      mMicroSeconds = low + (high << 32);
      mMicroSeconds =
         (vpr::Uint64) (((double) (vpr::Int64) mMicroSeconds) * usecs_per_count);
   }
/*
   else
   {
#if defined(__MINGW32__)
      mMicroSeconds = time();
#elif defined(WIN16)
      mMicroSeconds = clock();        // milliseconds since application start
#else
      mMicroSeconds = GetTickCount();  // milliseconds since system start
#endif
   }
*/
#elif defined(VPR_OS_IRIX) // SGI Cycle counter version
   if (mMmem_fd != -1)
   {
      if (64 == mClockWidth)
      {
         Uint64 temp = *(Uint64*)mTimerAddr;
         mTicks = temp;
         mMicroSeconds = temp*mTicksToMicroseconds;
      }
      else
      {
         // clockwidth <= 32. clockmask only relevent for width=24...?
         Uint64 ticks = mTicks;
         unsigned int now = *(unsigned int*)mTimerAddr, temp;
         unsigned int residual = mResidual;
         unsigned int previous = mPrevious;

         //if (now < previous)
         //   std::cout << "wraparound occurred!!!" << std::endl;

         temp = now - previous + residual;
         residual = temp & mClockMask;
         ticks += temp;

         mPrevious = now;
         mResidual = residual;
         mTicks = ticks;
         mMicroSeconds = ticks * mTicksToMicroseconds;
      }
   }
   else
   {
      // couldn't access the cycle counter for whatever reason.
      // use gettimeofday as our fallback.
      timeval cur_time;
      gettimeofday(&cur_time);
      vpr::Uint64 storage = vpr::Uint64(1000000u) * vpr::Uint64(cur_time.tv_sec);
      mMicroSeconds = vpr::Uint64(cur_time.tv_usec) + storage;
   }
#else    // Default to POSIX time setting
   vpr::TimeVal cur_time;
   vpr::System::gettimeofday(&cur_time);
   vpr::Uint64 storage = vpr::Uint64(1000000u) * vpr::Uint64(cur_time.tv_sec);
   mMicroSeconds = vpr::Uint64(cur_time.tv_usec) + storage;
#endif
}


#if defined(VPR_OS_IRIX) // SGI Cycle counter version support code


bool Interval::initializeCycleCounter()
{
   /*
    * As much as I would like, the service available through this
    * interface on R3000's (aka, IP12) just isn't going to make it.
    * The register is only 24 bits wide, and rolls over at a voracious
    * rate.
    */
   mMmem_fd = -1;
   mClockWidth = 0;
   mTimerAddr = NULL;
   mClockMask = 0;
   mPrevious = 0;          // used for wraparound handling
   mResidual = 0;          // used for wraparound handling

   /* on R3000 (IP12) don't even try to use the 24 bit cycle counter. */
   struct utsname utsinfo;
   uname(&utsinfo);
   if ( (strncmp("IP12", utsinfo.machine, 4) != 0) &&
        ((mMmem_fd = open("/dev/mmem", O_RDONLY)) != -1) )
   {
      int poffmask = getpagesize() - 1;
      __psunsigned_t phys_addr, raddr;
      vpr::Uint32 cycleval;

      phys_addr = syssgi(SGI_QUERY_CYCLECNTR, &cycleval);
      raddr = phys_addr & ~poffmask;
      mTimerAddr = mmap(0, poffmask, PROT_READ, MAP_PRIVATE, mMmem_fd,
                        (__psint_t)raddr);

      mClockWidth = syssgi(SGI_CYCLECNTR_SIZE);
      if (mClockWidth < 0)
      {
         /*
          * We must be executing on a 6.0 or earlier system, since the
          * SGI_CYCLECNTR_SIZE call is not supported.
          *
          * The only pre-6.1 platforms with 64-bit counters are
          * IP19 and IP21 (Challenge, PowerChallenge, Onyx).
          */
         if (!strncmp(utsinfo.machine, "IP19", 4) ||
             !strncmp(utsinfo.machine, "IP21", 4))
         {
            mClockWidth = 64;
         }
         else
         {
            mClockWidth = 32;
         }
      }

      //std::cout << "cycleval is " << cycleval << " ps." << std::endl;

      mTicksToMicroseconds = cycleval / 1000000.0L;
      mClockMask = (1 << mClockWidth) -1;
      mTimerAddr = (void*)
         ((__psunsigned_t)mTimerAddr + (phys_addr & poffmask));
   }

   return true;
}

int Interval::mMmem_fd;
int Interval::mClockWidth;
void *Interval::mTimerAddr;
vpr::Uint32 Interval::mClockMask;         // used for short cycle counters
vpr::Uint32 Interval::mPrevious;          // used for wraparound handling
vpr::Uint32 Interval::mResidual;          // used for wraparound handling
double  Interval::mTicksToMicroseconds;

const bool Interval::mInitialized = Interval::initializeCycleCounter();

#endif // VPR_OS_IRIX cycle counter version

} // namespace vpr

