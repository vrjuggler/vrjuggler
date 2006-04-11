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
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
#include <vpr/vprTypes.h>
#include <vpr/Util/Assert.h>


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
   {;}

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

   void setd(const double num, const Unit timeUnit)
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
         vprASSERT(false && "vpr::Interval::setd: Invalid Units used");
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
   {
#ifdef VPR_OS_Win32
      return (float((vpr::Int64) mMicroSeconds)/1000000.0f);
#else
      return (float(mMicroSeconds)/1000000.0f);
#endif
   }
   void secd(const double num)
   { setd(num, Interval::Sec); }
   double secd() const
   {
#ifdef VPR_OS_Win32
      return (double((vpr::Int64) mMicroSeconds)/1000000.0f);
#else
      return (double(mMicroSeconds)/1000000.0f);
#endif
   }

   void msec(const vpr::Uint64 num)
   { set(num, Interval::Msec); }
   vpr::Uint64 msec() const
   { return (mMicroSeconds/1000); }
   void msecf(const float num)
   { setf(num, Interval::Msec); }
   float msecf() const
   {
#ifdef VPR_OS_Win32
      return (float((vpr::Int64) mMicroSeconds)/1000.0f);
#else
      return (float(mMicroSeconds)/1000.0f);
#endif
   }
   void msecd(const double num)
   { setd(num, Interval::Msec); }
   double msecd() const
   {
#ifdef VPR_OS_Win32
      return (double((vpr::Int64) mMicroSeconds)/1000.0f);
#else
      return (double(mMicroSeconds)/1000.0f);
#endif
   }

   void usec(const vpr::Uint64 num)
   { set(num, Interval::Usec); }
   vpr::Uint64 usec() const
   { return mMicroSeconds; }
   void usecf(const float num)
   { setf(num, Interval::Usec); }
   float usecf() const
   {
#ifdef VPR_OS_Win32
      return ((vpr::Int64) mMicroSeconds);
#else
      return (mMicroSeconds);
#endif
   }
   void usecd(const double num)
   { setd(num, Interval::Usec); }
   double usecd() const
   {
#ifdef VPR_OS_Win32
      return ((vpr::Int64) mMicroSeconds);
#else
      return (mMicroSeconds);
#endif
   }

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

   bool operator> (const Interval& r) const
   {
      return (mMicroSeconds > r.mMicroSeconds);
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

#if defined(VPR_OS_IRIX)
   // All of these variables are required to make the cycle counter
   // timing implementation work.
   // Closely copied from NSPR (irix.c), modified to translate ticks
   // directly to usecs (instead of NSPR 100us 'ticks'),
   // with additional error handling & initialization adapted
   // from JCCL::TimeStampSGI
   // For more information, see the discussion of SGI_QUERY_CYCLECNTR
   // in the syssgi man page.
   static int mMmem_fd;                  /**< Fd for mem mapping           */
   static int mClockWidth;               /**< #bits in cycle counter       */
   static void *mTimerAddr;              /**< Address of mmapped counter   */
   static vpr::Uint32 mClockMask;        /**< Used for wraparound handling */
   static vpr::Uint32 mPrevious;         /**< Used for wraparound handling */
   static vpr::Uint32 mResidual;         /**< Used for wraparound handling */
   static double mTicksToMicroseconds;   /**< Conversion multiplier        */
   vpr::Uint64 mTicks;                   /**< Actual cycle counter ticks   */

public:
   /** Initializer method for SGI cycle counter access. */
   static bool initializeCycleCounter ();

private:
   static const bool mInitialized;       /**< Forces counter initialization. */

#endif // VPR_OS_IRIX cycle counter stuff


}; // class Interval


}; // namespace vpr


#endif
