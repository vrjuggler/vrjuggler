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

#ifndef _VPR_SIM_CLOCK_H_
#define _VPR_SIM_CLOCK_H_

#include <vpr/vprConfig.h>
#include <vpr/vprTypes.h>
#include <vpr/Util/Interval.h>


namespace vpr
{

class Interval;

namespace sim
{

/**
 * Simulated clock for use with the socket simulation code.  The basic unit
 * of time is tens of microseconds since that is what vpr::Interval uses.
 */
class Clock
{
public:
   Clock (void)
      : mCurrentTime(0, vpr::Interval::Base)
   {
      /* Do nothing. */ ;
   }

   const vpr::Interval& getCurrentTime (void) const
   {
      return mCurrentTime;
   }

   void setCurrentTime (const vpr::Interval& time)
   {
      if ( time > mCurrentTime )
      {
         mCurrentTime = time;
      }
   }

private:
   vpr::Interval mCurrentTime;
};

} // End of sim namespace

} // End of vpr namespace


#endif /* _VPR_SIM_CLOCK_H_ */
