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

namespace vpr
{

class VPR_CLASS_API Interval
{
public:
   enum Unit { SEC, MSEC, USEC };     // Time times

public:
   static const Interval NoWait;
   static const Interval NoTimeout;

public:
   Interval()
   { mUsecs = 0; }

   Interval(const vpr::Uint32 num, const Unit timeUnit)
   { set(num, timeUnit); }

   void set(const vpr::Uint32 num, const Unit timeUnit)
   {
      switch(timeUnit)
      {
      case Interval::SEC:
         mUsecs = (1000000) * num;
         break;
      case Interval::MSEC:
         mUsecs = 1000 * num;
         break;
      case Interval::USEC:
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
      case Interval::SEC:
         mUsecs = vpr::Uint32(1000000.0f * num);
         break;
      case Interval::MSEC:
         mUsecs = vpr::Uint32(1000.0f * num);
         break;
      case Interval::USEC:
         mUsecs = vpr::Uint32(num);
         break;
      default:
         vprASSERT(false && "vpr::Interval::setf: Invalid Units used");
         break;
      }
   }

   void sec(const vpr::Uint32 num)
   { set(num, Interval::SEC); }
   vpr::Uint32 sec() const
   { return (mUsecs/1000000); }
   void secf(const float num)
   { setf(num, Interval::SEC); }
   float secf() const
   { return (float(mUsecs)/1000000.0f); }

   void msec(const vpr::Uint32 num)
   { set(num, Interval::MSEC); }
   vpr::Uint32 msec() const
   { return (mUsecs/1000); }
   void msecf(const float num)
   { setf(num, Interval::MSEC); }
   float msecf() const
   { return (float(mUsecs)/1000.0f); }

   void usec(const vpr::Uint32 num)
   { mUsecs = num; }
   vpr::Uint32 usec() const
   { return mUsecs; }
   void usecf(const float num)
   { setf(num, Interval::USEC); }
   float usecf() const
   { return mUsecs; }

private:
   vpr::Uint32 mUsecs;
}; // class Interval

}; // namespace vpr


#endif

