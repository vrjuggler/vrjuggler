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
   Interval() : mMicroSeconds(0)
   { }

   Interval(const vpr::Uint64 num, const Unit timeUnit) : mMicroSeconds(0)
   { set(num, timeUnit); }

   void set(const vpr::Uint64 num, const Unit timeUnit)
   {
      switch(timeUnit)
      {
      case Interval::Sec:
         mMicroSeconds = (1000000) * num;
         break;
      case Interval::Msec:
         mMicroSeconds = 1000 * num;
         break;
      case Interval::Usec:
         mMicroSeconds = num;
         break;
      case Interval::Base:
         mMicroSeconds = num;
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
         mMicroSeconds = vpr::Uint64(1000000.0f * num);
         break;
      case Interval::Msec:
         mMicroSeconds = vpr::Uint64(1000.0f * num);
         break;
      case Interval::Usec:
         mMicroSeconds = vpr::Uint64(num);
         //mTensOfUsecs = num;
         break;
      case Interval::Base:
         mMicroSeconds = vpr::Uint64(num);
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
   void setNow();

   /** Set now that is gauranteed to be "real" time */
   void setNowReal();

   void sec(const vpr::Uint64 num)
   { set(num, Interval::Sec); }
   vpr::Uint64 sec() const
   { return (mMicroSeconds/1000000); }
   void secf(const float num)
   { setf(num, Interval::Sec); }
   float secf() const
   { return (float(mMicroSeconds)/1000000.0f); }

   void msec(const vpr::Uint64 num)
   { set(num, Interval::Msec); }
   vpr::Uint64 msec() const
   { return (mMicroSeconds/1000); }
   void msecf(const float num)
   { setf(num, Interval::Msec); }
   float msecf() const
   { return (float(mMicroSeconds)/1000.0f); }

   void usec(const vpr::Uint64 num)
   { set(num, Interval::Usec); }
   vpr::Uint64 usec() const
   { return mMicroSeconds; }
   void usecf(const float num)
   { setf(num, Interval::Usec); }
   float usecf() const
   { return (mMicroSeconds); }

   vpr::Uint64 getBaseVal() const
   { return mMicroSeconds; }

   bool operator ==(const Interval& r) const
   { return (mMicroSeconds == r.mMicroSeconds); }

   bool operator !=(const Interval& r) const
   { return ! (*this == r); }

   /** Operator <
   * Must handle the roll over condition
   * |--------------|--------------|
   *  v1          v2 v3
   *
   * v1<v2, v2<v3, v3<v1
   */
   bool operator <(const Interval& r) const
   {
      return (mMicroSeconds < r.mMicroSeconds);
   }

   bool operator<= (const Interval& r) const
   {
      return (mMicroSeconds <= r.mMicroSeconds);
   }

   Interval& operator+=(const Interval& r)
   { mMicroSeconds += r.mMicroSeconds; return *this; }

   Interval operator +(const Interval& r) const
   { return (Interval(*this) += r);  }

   Interval& operator-=(const Interval& r)
   { mMicroSeconds -= r.mMicroSeconds; return *this; }

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
   vpr::Uint64 mMicroSeconds;
}; // class Interval


}; // namespace vpr


#endif
