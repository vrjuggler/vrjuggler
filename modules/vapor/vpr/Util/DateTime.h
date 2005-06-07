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

#ifndef _VPR_DATE_TIME_H_
#define _VPR_DATE_TIME_H_

#include <vpr/vprConfig.h>
#include <vpr/vprTypes.h>
#include <vpr/System.h>


namespace vpr
{

/** \class DateTime DateTime.h vpr/Util/DateTime.h
 *
 * Simple wrapper around time since the UNIX Epoch (00:00 UTC January 1, 1970).
 */
class DateTime
{
public:
   /**
    * Constructs an empty time object.  To assign this object the current
    * time, invoke setNow on it.
    *
    * @see setNow
    */
   DateTime()
      : mSeconds(0)
      , mMicroSeconds(0)
   {
      /* Do nothing. */ ;
   }

   /**
    * Updates this object to reflect the current time.
    *
    * @post The member variables are updated the time returned by
    *       vpr::System::gettimeofday.
    */
   void setNow()
   {
      vpr::TimeVal tv;

      vpr::System::gettimeofday(&tv);
      mSeconds      = tv.tv_sec;
      mMicroSeconds = tv.tv_usec;
   }

   /**
    * Get the seconds since the Epoch for this object.
    *
    * @return An unsigned integer representing the seconds since the Epoch.
    *         If 0 (zero) is returned, this object has not been initialized
    *         using setNow.
    */
   vpr::Uint32 getSeconds()
   {
      return mSeconds;
   }

   /**
    * Get the seconds (and microseconds) since the Epoch for this object.
    *
    * @return A floating-point value representing the seconds since the Epoch.
    *         If 0.0 (zero) is returned, this object has not been initialized
    *         using setNow.
    */
   double getSecondsf()
   {
      double sec_f, usec_f, usec_f_div;

      sec_f      = (double) mSeconds;
      usec_f     = (double) mMicroSeconds;
      usec_f_div = usec_f / 1000000.0f;

      return sec_f + usec_f_div;
   }

   /**
    * Get the minutes since the Epoch for this object.
    *
    * @return An unsigned integer representing the minutes since the Epoch.
    *         If 0 (zero) is returned, this object has not been initialized
    *         using setNow.
    */
   vpr::Uint32 getMinutes()
   {
      return mSeconds / 60;
   }

   /**
    * Get the minutes since the Epoch for this object with floating-point
    * precision.
    *
    * @return A floating-point value representing the minutes since the Epoch.
    *         If 0.0 (zero) is returned, this object has not been initialized
    *         using setNow.
    */
   double getMinutesf()
   {
      return getSecondsf() / 60.0f;
   }

   /**
    * Get the hours since the Epoch for this object.
    *
    * @return An unsigned integer representing the hours since the Epoch.
    *         If 0 (zero) is returned, this object has not been initialized
    *         using setNow.
    */
   vpr::Uint32 getHours()
   {
      return mSeconds / 3600;
   }

   /**
    * Get the hours since the Epoch for this object with floating-point
    * precision.
    *
    * @return A floating-point value representing the hours since the Epoch.
    *         If 0.0 (zero) is returned, this object has not been initialized
    *         using setNow.
    */
   double getHoursf()
   {
      return getSecondsf() / 3600.0f;
   }

   /**
    * Get the days since the Epoch for this object.
    *
    * @return An unsigned integer representing the days since the Epoch.
    *         If 0 (zero) is returned, this object has not been initialized
    *         using setNow.
    */
   vpr::Uint32 getDays()
   {
      return mSeconds / 86400;
   }

   /**
    * Get the days since the Epoch for this object with floating-point
    * precision.
    *
    * @return A floating-point value representing the days since the Epoch.
    *         If 0.0 (zero) is returned, this object has not been initialized
    *         using setNow.
    */
   double getDaysf()
   {
      return getSecondsf() / 86400.0f;
   }

   /**
    * Get the weeks since the Epoch for this object.
    *
    * @return An unsigned integer representing the weeks since the Epoch.
    *         If 0 (zero) is returned, this object has not been initialized
    *         using setNow.
    */
   vpr::Uint32 getWeeks()
   {
      return mSeconds / 608400;
   }

   /**
    * Get the weeks since the Epoch for this object with floating-point
    * precision.
    *
    * @return A floating-point value representing the weeks since the Epoch.
    *         If 0.0 (zero) is returned, this object has not been initialized
    *         using setNow.
    */
   double getWeeksf()
   {
      return getSecondsf() / 608400.0f;
   }

   /**
    * Overloaded addition operator for adding this and another DateTime
    * object.
    */
   DateTime operator+ (const DateTime& o) const
   {
      return DateTime(mSeconds + o.mSeconds, mMicroSeconds + o.mMicroSeconds);
   }

   /**
    * Overloaded addition operator for subtracting another DateTime object
    * from this one.
    */
   DateTime operator- (const DateTime& o) const
   {
      return DateTime(mSeconds - o.mSeconds, mMicroSeconds - o.mMicroSeconds);
   }

private:
   /**
    * Private constructor for initializing a new object.  This is hidden
    * because the arguments reveal too much (i.e., all) of the implmentation
    * details.
    *
    * @post mSeconds and mMicroSeconds are initialized using the given
    *       parameters.
    *
    * @param seconds      Seconds since the Epoch.
    * @param microseconds Microseconds value for the given seconds value.
    */
   DateTime(const vpr::Uint32 seconds, const vpr::Uint32 microseconds)
      : mSeconds(seconds)
      , mMicroSeconds(microseconds)
   {
      /* Do nothing. */ ;
   }

   vpr::Uint32 mSeconds;       /**< Seconds since the Epoch */
   vpr::Uint32 mMicroSeconds;  /**< Microseconds value */
};

} // End of vpr namespace


#endif  /* _VPR_DATE_TIME_H_ */
