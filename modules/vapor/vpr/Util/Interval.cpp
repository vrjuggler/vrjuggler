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

#include <vpr/vprConfig.h>

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
#if defined(VPR_OS_Win32)
   LARGE_INTEGER count;

   // XXX: Implement this
   /* Sadly; nspr requires the interval to range from 1000 ticks per second
    * to only 100000 ticks per second; QueryPerformanceCounter is too high
    * resolution...
    */
   if (QueryPerformanceCounter(&count))
   {
      vpr::Int32 top = count.HighPart & _nt_highMask;
      top = top << (32 - _nt_bitShift);
      count.LowPart = count.LowPart >> _nt_bitShift;   
      count.LowPart = count.LowPart + top; 
      return(PRUint32)count.LowPart;
   } //else
   
    //#if defined(__MINGW32__)
    //        return time();
    //#elif defined(WIN16)
    //        return clock();        /* milliseconds since application start */
    //#else
    // return GetTickCount();  /* milliseconds since system start */
    //#endif
#else    // Default to POSIX time setting
   
   timeval cur_time;
   vpr::System::gettimeofday(&cur_time);
   mMicroSeconds = (cur_time.tv_usec + (1000000 * cur_time.tv_sec));

#endif
}

}; // namespace vpr




