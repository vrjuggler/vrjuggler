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

#ifndef _VPR_TIMER_H_
#define _VPR_TIMER_H_

#include <vpr/vprConfig.h>

#ifndef VPR_OS_Windows
#include <sys/time.h>
#endif

namespace vpr
{

/** \class Timer Timer.h vpr/Util/Timer.h
 *
 * Simple class to take timings and give averages.
 *
 * @note All times are in seconds.
 */
class Timer
{
public:
   Timer()
      : mStartTime(0)
      , mEndTime(0)
      , mTotalTime(0)
      , mTimeCount(0)
   {
      ;
   }

   /** Starts a timing. */
   void startTiming()
   {
      mStartTime = wallclock();
   }

   /**
    * Stops current timing.
    *
    * @pre Timing has been started.
    */
   void stopTiming()
   {
      mEndTime = wallclock();
      if ( mStartTime == 0 )   // If we haven't called start then ignore
      {
         mTotalTime = 0;
      }
      else
      {
         mLastTiming = (mEndTime - mStartTime);
         mTotalTime += mLastTiming;
         mTimeCount++;
      }
   }

   /** Gets the average timing. */
   double getTiming()
   {
      return (mTotalTime/(double)mTimeCount);
   }

   double getLastTiming()
   {
      return mLastTiming;
   }

   /** Returns the number of timings collected. */
   long getTimeCount()
   {
      return mTimeCount;
   }

   void reset()
   {
      mTotalTime = 0;
      mTimeCount = 0;
      mStartTime = 0;
      mEndTime = 0;
   }

#ifdef VPR_OS_Windows
   double wallclock()
   {
      return ((double)GetTickCount())/1000.0f;
   }
#else
   double wallclock()
   {
      double v;
      struct timeval tv;

      gettimeofday(&tv, 0);

      v = (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;
      return v;
   }
#endif

private:
   /** @name Timing stuff */
   //@{
   double  mStartTime;       /**< The start time */
   double  mEndTime;         /**< The end time */
   double  mLastTiming;      /**< The last timing recieved */
   double  mTotalTime;       /**< The total time */
   long    mTimeCount;       /**< The count on the number of times */
   //@}
};  // class Timer

}   // namespace vpr


#endif
