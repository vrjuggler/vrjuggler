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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

#ifndef VPR_INTERVAL_
#define VPR_INTERVAL_

#include <vpr/vprConfig.h>
#include <vpr/Util/Assert.h>

#ifndef VPR_SIMULATOR
#  if defined(VPR_USE_NSPR)
#     include <prinrval.h>
#  else
#     include <vpr/System.h>
#  endif
#endif

namespace vpr
{

/**
 * This class captures an high resolution interval
 *
 * This interval is based off an unsigned always increasing
 * counter.  This means the interval is only accurate for about 12 hours which
 * means that it is able to be useful for about a range of 6 hours
 * (This is because internally everything is stored as 10's of usecs)
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
      Usec,    /**< Microeconds */
      Base     /**< The base units */
   };

public:
   static const Interval NoWait;     /**< Do not wait at all */
   static const Interval NoTimeout;  /**< Wait indefinitely */
   static const Interval HalfPeriod;   /**< Half of the roll over period */

public:
   Interval() : mTensOfUsecs(0)
   { }

   Interval(const vpr::Uint32 num, const Unit timeUnit) : mTensOfUsecs(0)
   { set(num, timeUnit); }

   void set(const vpr::Uint32 num, const Unit timeUnit)
   {
      switch(timeUnit)
      {
      case Interval::Sec:
         mTensOfUsecs = (100000) * num;
         break;
      case Interval::Msec:
         mTensOfUsecs = 100 * num;
         break;
      case Interval::Usec:
         mTensOfUsecs = num/10;
         break;
      case Interval::Base:
         mTensOfUsecs = num;
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
         mTensOfUsecs = vpr::Uint32(100000.0f * num);
         break;
      case Interval::Msec:
         mTensOfUsecs = vpr::Uint32(100.0f * num);
         break;
      case Interval::Usec:
         mTensOfUsecs = vpr::Uint32(num / 10.0f);
         //mTensOfUsecs = num;
         break;
      case Interval::Base:
         mTensOfUsecs = vpr::Uint32(num);
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
#ifndef VPR_SIMULATOR
   inline
#endif
   void setNow();

   void sec(const vpr::Uint32 num)
   { set(num, Interval::Sec); }
   vpr::Uint32 sec() const
   { return (mTensOfUsecs/100000); }
   void secf(const float num)
   { setf(num, Interval::Sec); }
   float secf() const
   { return (float(mTensOfUsecs)/100000.0f); }

   void msec(const vpr::Uint32 num)
   { set(num, Interval::Msec); }
   vpr::Uint32 msec() const
   { return (mTensOfUsecs/100); }
   void msecf(const float num)
   { setf(num, Interval::Msec); }
   float msecf() const
   { return (float(mTensOfUsecs)/100.0f); }

   void usec(const vpr::Uint32 num)
   { set(num, Interval::Usec); }
   vpr::Uint32 usec() const
   { return mTensOfUsecs*10; }
   void usecf(const float num)
   { setf(num, Interval::Usec); }
   float usecf() const
   { return (mTensOfUsecs*10.0f); }

   vpr::Uint32 getBaseVal() const
   { return mTensOfUsecs; }

   bool operator ==(const Interval& r) const
   { return (mTensOfUsecs == r.mTensOfUsecs); }

   bool operator !=(const Interval& r) const
   { return ! (*this == r); }

   /** Operator <
   * Must handle the roll over condition
   */
   bool operator <(const Interval& r) const
   {
      return ( (mTensOfUsecs+(0xffffffffUL/2)) < (r.mTensOfUsecs+(0xffffffffUL/2)) );
   }

   bool operator<= (const Interval& r) const
   {
      return ( (mTensOfUsecs+(0xffffffffUL/2)) <= (r.mTensOfUsecs+(0xffffffffUL/2)) );
   }

   Interval& operator+=(const Interval& r)
   { mTensOfUsecs += r.mTensOfUsecs; return *this; }

   Interval operator +(const Interval& r) const
   { return (Interval(*this) += r);  }

   Interval& operator-=(const Interval& r)
   { mTensOfUsecs -= r.mTensOfUsecs; return *this; }

   /**
    * Return the difference of two interval values
    *
    * @note The interval may overflow.  If so, then it will still
    * evaluate correctly.
    * @param r      The parameter to subtract
    * @return
    */
   Interval operator -(const Interval& r) const
   { return (Interval(*this) -= r); }

   /** Helper function that returns an interval representing the current time */
   static vpr::Interval now()
   {
      vpr::Interval ret_val;
      ret_val.setNow();
      return ret_val;
   }


private:
   vpr::Uint32 mTensOfUsecs;
}; // class Interval

// In the non-simulator case, we want setNow() to be fast, so it should be
// inlined.  In the simulator case, we cannot inline it due to circular
// include problems (vpr::sim::Controller needs vpr::Interval, and
// vpr::Interval needs vpr::sim::Controller's vpr::sim::Clock instance).  Thus,
// the simulator version of setNow() is in Interval.cpp.
#ifndef VPR_SIMULATOR
inline void Interval::setNow()
{
#if defined(VPR_USE_NSPR)
   /*  Todo our own rounding
   static vpr::Uint32 ticks_per_sec(0);
   if(ticks_per_sec == 0)
      ticks_per_sec = PR_TicksPerSecond();
   */

   mTensOfUsecs = PR_IntervalToMicroseconds( PR_IntervalNow() ) / 10;
#else

   timeval cur_time;
   vpr::System::gettimeofday(&cur_time);
   mTensOfUsecs = (cur_time.tv_usec + (1000000 * cur_time.tv_sec)) / 10;
#endif
}
#endif /* ifndef VPR_SIMULATOR */

}; // namespace vpr


#endif
