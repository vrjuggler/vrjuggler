/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef VPR_INTERVAL_
#define VPR_INTERVAL_

#include <vpr/vprConfig.h>
#include <vpr/Util/Assert.h>

#if defined(VPR_USE_NSPR)
#  include <prinrval.h>
#else
#  include <vpr/System.h>
#endif

namespace vpr
{

/**
 * This class captures an high resolution interval
 * 
 * This interval is based off an unsigned always increasing
 * counter.  This means the interval is only accurate for about 3 hours
 * 
 * 
 * The interval overflows whenever the counter maxes out.
 * The operator- will take care of the overflow automatically
 * to make it possible to compare to interval values.
 */
class VPR_CLASS_API Interval
{
public:
   /// Time times
   enum Unit {
      Sec,     /**< Seconds */
      Msec,    /**< Millieconds */
      Usec     /**< Microeconds */
   };

public:
   static const Interval NoWait;     /**< Do not wait at all */
   static const Interval NoTimeout;  /**< Wait indefinitely */

public:
   Interval()
   { mUsecs = 0; }

   Interval(const vpr::Uint32 num, const Unit timeUnit)
   { set(num, timeUnit); }

   void set(const vpr::Uint32 num, const Unit timeUnit)
   {
      switch(timeUnit)
      {
      case Interval::Sec:
         mUsecs = (1000000) * num;
         break;
      case Interval::Msec:
         mUsecs = 1000 * num;
         break;
      case Interval::Usec:
         mUsecs = num;
         break;
      default:
         vprASSERT(false && "vpr::Interval::set: Invalid Units used");
         break;
      }
   }

   void setf(const float num, const Unit timeUnit)
   {
      switch(timeUnit)
      {
      case Interval::Sec:
         mUsecs = vpr::Uint32(1000000.0f * num);
         break;
      case Interval::Msec:
         mUsecs = vpr::Uint32(1000.0f * num);
         break;
      case Interval::Usec:
         mUsecs = vpr::Uint32(num);
         break;
      default:
         vprASSERT(false && "vpr::Interval::setf: Invalid Units used");
         break;
      }
   }

   /**
    * Set the interval to the current time.  This can them be used to compute a time 
    * interval by subtracting two intervals from each other.
    */
   inline void setNow();

   void sec(const vpr::Uint32 num)
   { set(num, Interval::Sec); }
   vpr::Uint32 sec() const
   { return (mUsecs/1000000); }
   void secf(const float num)
   { setf(num, Interval::Sec); }
   float secf() const
   { return (float(mUsecs)/1000000.0f); }

   void msec(const vpr::Uint32 num)
   { set(num, Interval::Msec); }
   vpr::Uint32 msec() const
   { return (mUsecs/1000); }
   void msecf(const float num)
   { setf(num, Interval::Msec); }
   float msecf() const
   { return (float(mUsecs)/1000.0f); }

   void usec(const vpr::Uint32 num)
   { mUsecs = num; }
   vpr::Uint32 usec() const
   { return mUsecs; }
   void usecf(const float num)
   { setf(num, Interval::Usec); }
   float usecf() const
   { return mUsecs; }

   bool operator ==(const Interval& r) const
   { return (mUsecs == r.mUsecs); }

   bool operator !=(const Interval& r) const
   { return ! (*this == r); }
   
   Interval operator +(const Interval& r) const
   { return Interval(vpr::Uint32(mUsecs+r.mUsecs), Interval::Usec); }

   /**
    * Return the difference of two interval values
    * 
    * @note The interval may overflow.  If so, then it will still
    * evaluate correctly.
    * @param r      The parameter to subtract
    * @return
    */
   Interval operator -(const Interval& r) const
   { return Interval(vpr::Uint32(mUsecs-r.mUsecs), Interval::Usec); }


private:
   vpr::Uint32 mUsecs;
}; // class Interval

inline void Interval::setNow()
{
#if defined(VPR_USE_NSPR)
   /*  Todo our own rounding
   static vpr::Uint32 ticks_per_sec(0);
   if(ticks_per_sec == 0)
      ticks_per_sec = PR_TicksPerSecond();
   */

   mUsecs = PR_IntervalToMicroseconds( PR_IntervalNow() );
#else

   timeval cur_time;
   vpr::System::gettimeofday(&cur_time);
   mUsecs = cur_time.tv_usec + (1000000 * cur_time.tv_sec);
#endif
}

}; // namespace vpr


#endif

