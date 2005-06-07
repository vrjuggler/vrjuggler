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

#ifndef VPR_INTERVAL_
#define VPR_INTERVAL_

#include <vpr/vprConfig.h>
#include <vpr/vprTypes.h>
#include <vpr/Util/Assert.h>


namespace vpr
{

/** \class Interval Interval.h vpr/Util/Interval.h
 *
 * This class captures a high-resolution interval.
 *
 * This interval is based off an unsigned always increasing
 * counter.  This means that the interval is only accurate for about 12 hours
 * and that it is able to be useful for a range of roughly 6 hours.
 * (This is because, internally, everything is stored as tens of microseconds.)
 *
 * The interval overflows whenever the counter maxes out.
 * The overloaded operator- will take care of the overflow automatically
 * to make it possible to compare to interval values.
 */
class VPR_CLASS_API Interval
{
public:
   /** Units of time. */
   enum Unit
   {
      Sec,     /**< Seconds */
      Msec,    /**< Milliseconds */
      Usec,    /**< Microseconds */
      Base     /**< The base units */
   };

public:
   static const Interval NoWait;     /**< Do not wait at all */
   static const Interval NoTimeout;  /**< Wait indefinitely */
   static const Interval HalfPeriod; /**< Half of the roll-over period */

public:
   /**
    * Default constructor sets the internal timer to 0.
    */
   Interval() : mMicroSeconds(0)
   {;}

   /**
    * Constructor that takes a vpr::Uint64 number and a unit and stores it in
    * the internal timer as the interval.
    */
   Interval(const vpr::Uint64 num, const Unit timeUnit) 
   { 
      set(num, timeUnit); 
   }

   /** 
    * Sets the interval using a vpr::Uint64 (\p num) and a unit type
    * (t\p imeUnit).  It sets mMicroSeconds to be the number of microseconds
    * in num timeUnit.
    * Example: set(1, vpr::Interval::Sec) then \c mMicroSeconds is set to
    * 1000000.
    */
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

   /** 
    * Sets the interval using a single-precision floating-point number (\p num)
    * and a unit type (\p timeUnit).  It sets mMicroSeconds to be the number of
    * microseconds in \p num \p timeUnit.
    * Example: setf(1.0f, vpr::Interval::Sec) then \c mMicroSeconds is set to
    * 1000000.
    */
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
    * Sets this interval using a double-precision floating-point number
    * (\p num) and a unit type (\p timeUnit).  It sets mMicroSeconds to be the
    * number of microseconds in \p num \p timeUnit.
    * Example: setd(1.0, vpr::Interval::Sec) then \c mMicroSeconds is set to
    * 1000000.
    */
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
    * Sets this interval to the current time.  This can then be used to compute 
    * a time interval by subtracting two intervals from each other.
    */
   void setNow();

   /** 
    * Set now that is gauranteed to be the time based on the system clock which
    * can then be used to compute a time interval by subracting two intervals
    * from each other.
    */
   void setNowReal();

   /**
    * Given an amount of time in seconds as a vpr::Uint64, store it as the
    * value of this interval.
    */
   void sec(const vpr::Uint64 num)
   { 
      set(num, Interval::Sec); 
   }
   
   /**
    * Returns the value of this interval in seconds as an unsigned integer.
    */
   vpr::Uint64 sec() const
   { 
      return (mMicroSeconds/1000000); 
   }
   
   /**
    * Given an amount of time in seconds as a single-precision floating-point
    * value, store it as the value of this interval.
    */
   void secf(const float num)
   { 
      setf(num, Interval::Sec); 
   }
   
   /**
    * Returns the value of this interval in seconds as a single-precision
    * floating-point value.
    */
   float secf() const
   {
#ifdef VPR_OS_Win32
      return (float((vpr::Int64) mMicroSeconds)/1000000.0f);
#else
      return (float(mMicroSeconds)/1000000.0f);
#endif
   }
   
   /**
    * Given an amount of time in seconds as a double-precision floating-point
    * value, store it as the value of this interval.
    */
   void secd(const double num)
   { 
      setd(num, Interval::Sec); 
   }
   
   /**
    * Returns the value of this interval in seconds as a double-precision
    * floating-point value.
    */
   double secd() const
   {
#ifdef VPR_OS_Win32
      return (double((vpr::Int64) mMicroSeconds)/1000000.0f);
#else
      return (double(mMicroSeconds)/1000000.0f);
#endif
   }

   /**
    * Given an amount of time in milliseconds as a vpr::Uint64, store it as the
    * value of this interval.
    */
   void msec(const vpr::Uint64 num)
   { 
      set(num, Interval::Msec); 
   }
   
   /**
    * Returns the value of this interval in milliseconds as an unsigned
    * integer value.
    */
   vpr::Uint64 msec() const
   { 
      return (mMicroSeconds/1000); 
   }
   
   /**
    * Given an amount of time in milliseconds as a single-precision
    * floating-point value, store it as the value of this interval.
    */
   void msecf(const float num)
   { 
      setf(num, Interval::Msec); 
   }
   
   /**
    * Returns the value of this interval in milliseconds as a single-precision
    * floating-point value.
    */
   float msecf() const
   {
#ifdef VPR_OS_Win32
      return (float((vpr::Int64) mMicroSeconds)/1000.0f);
#else
      return (float(mMicroSeconds)/1000.0f);
#endif
   }

   /**
    * Given an amount of time in milliseconds as a double-precision
    * floating-point value, store it as the value of this interval.
    */
   void msecd(const double num)
   { 
      setd(num, Interval::Msec); 
   }
   
   /** 
    * Returns the value of this interval in milliseconds as a double-precision
    * floating-point value.
    */
   double msecd() const
   {
#ifdef VPR_OS_Win32
      return (double((vpr::Int64) mMicroSeconds)/1000.0f);
#else
      return (double(mMicroSeconds)/1000.0f);
#endif
   }

   /**
    * Given an amount of time in microseconds as a vpr::Uint64, store it as the
    * value of this interval.
    */
   void usec(const vpr::Uint64 num)
   { 
      set(num, Interval::Usec); 
   }
   
   /**
    * Returns the value of this interval in microseconds as an unsigned
    * integer value.
    */
   vpr::Uint64 usec() const
   { 
      return mMicroSeconds; 
   }
   
   /**
    * Given an amount of time in microseconds as a single-precision
    * floating-point value, store it as the value of this interval.
    */
   void usecf(const float num)
   { 
      setf(num, Interval::Usec); 
   }
   
   /**
    * Returns the value of this interval in microseconds as a single-precision
    * floating-point value.
    */
   float usecf() const
   {
#ifdef VPR_OS_Win32
      return ((vpr::Int64) mMicroSeconds);
#else
      return (mMicroSeconds);
#endif
   }
   
   /**
    * Given an amount of time in microseconds as a double-precision
    * floating-point value, store it as the value of this interval.
    */
   void usecd(const double num)
   { 
      setd(num, Interval::Usec); 
   }
   
   /**
    * Returns the value of this interval in microseconds as a double-precision
    * floating-point value.
    */
   double usecd() const
   {
#ifdef VPR_OS_Win32
      return ((vpr::Int64) mMicroSeconds);
#else
      return (mMicroSeconds);
#endif
   }

   /** 
    * Returns the value of this interval in microseconds as a vpr:Uint64.
    */
   vpr::Uint64 getBaseVal() const
   { 
      return mMicroSeconds; 
   }

   /**
    * Tests two intervals for equality by checking the values of their
    * \c mMicroSeconds variables.
    *
    * @return true if both have an equal value for mMicroSeconds; false
    *         otherwise.
    */
   bool operator ==(const Interval& r) const
   { 
      return (mMicroSeconds == r.mMicroSeconds); 
   }

   /**
    * Tests two intervals for inequality by checking the values of their
    * \c mMicroSeconds variables.
    *
    * @return true if the intervals have unequal values for mMicroSeconds;
    *         false otherwise.
    */
   bool operator !=(const Interval& r) const
   { 
      return ! (*this == r); 
   }

   /**
    * Less-than test for two intervals.  This must handle the roll-over
    * condition:
    *
    * <pre>
    * |--------------|--------------|
    *  v1          v2 v3
    * </pre>
    *
    * v1 < v2, v2 < v3, v3 < v1
    */
   bool operator <(const Interval& r) const
   {
      return (mMicroSeconds < r.mMicroSeconds);
   }

   /**
    * Less-than-or-equal test for two intervals.  This must handle the
    * roll-over condition:
    *
    * <pre>
    * |--------------|--------------|
    *  v1          v2 v3
    * </pre>
    *
    * v1<v2, v2<v3, v3<v1
    */
   bool operator<= (const Interval& r) const
   {
      return (mMicroSeconds <= r.mMicroSeconds);
   }

   /**
    * Greater-than test for two intervals.  This must handle the roll-over
    * condition:
    *
    * <pre>
    * |--------------|--------------|
    *  v1          v2 v3
    * </pre>
    *
    * v2 > v1, v3 > v2, v3 > v1
    */
   bool operator> (const Interval& r) const
   {
      return (mMicroSeconds > r.mMicroSeconds);
   }

   /**
    * Adds the values of the two vpr::Interval objects and stores the result
    * in this vpr::Interval.
    */
   Interval& operator+=(const Interval& r)
   { 
      mMicroSeconds += r.mMicroSeconds;
      return *this; 
   }

   /**
    * Returns a vpr::Interval that is the addition of the value of this
    * vpr::Interval and the given vpr::Interval.
    */
   Interval operator+ (const Interval& r) const
   { 
      return (Interval(*this) += r);  
   }

   /**
    * Subtracts the values of the two vpr::Interval objects and stores the
    * result in this vpr::Interval.
    */
   Interval& operator-=(const Interval& r)
   { 
      mMicroSeconds -= r.mMicroSeconds; return *this; 
   }

   /**
    * Returns the difference of two vpr::Interval objects.
    *
    * @note The interval may overflow.  If so, then it will still
    *       evaluate correctly.
    *
    * @param r The Interval to subtract.
    */
   Interval operator- (const Interval& r) const
   { 
      return (Interval(*this) -= r); 
   }

   /**
    * Helper function that returns a vpr::Interval representing the current
    * time.
    */
   static vpr::Interval now()
   {
      vpr::Interval ret_val;
      ret_val.setNow();
      return ret_val;
   }


private:
   vpr::Uint64 mMicroSeconds; /** The amount of time in this interval in microseconds. */

#if defined(VPR_OS_IRIX)
   /**
    * @name IRIX cycle counter data members.
    *
    * All of these variables are required to make the cycle counter
    * timing implementation work.
    *
    * Closely copied from NSPR (irix.c), modified to translate ticks
    * directly to usecs (instead of NSPR 100us 'ticks'), with additional
    * error handling and initialization.
    *
    * For more information, see the discussion of SGI_QUERY_CYCLECNTR
    * in the syssgi man page.
    */
   //@{
   static int mMmem_fd;                  /**< Fd for mem mapping           */
   static int mClockWidth;               /**< #bits in cycle counter       */
   static void *mTimerAddr;              /**< Address of mmapped counter   */
   static vpr::Uint32 mClockMask;        /**< Used for wraparound handling */
   static vpr::Uint32 mPrevious;         /**< Used for wraparound handling */
   static vpr::Uint32 mResidual;         /**< Used for wraparound handling */
   static double mTicksToMicroseconds;   /**< Conversion multiplier        */
   vpr::Uint64 mTicks;                   /**< Actual cycle counter ticks   */
   //@}

public:
   /** Initializer method for SGI cycle counter access. */
   static bool initializeCycleCounter();

private:
   static const bool mInitialized;       /**< Forces counter initialization. */

#endif // VPR_OS_IRIX cycle counter stuff
}; // class Interval

} // namespace vpr


#endif
