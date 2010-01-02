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
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _VPR_NSPR_HELPER_H_
#define _VPR_NSPR_HELPER_H_

#include <vpr/vprConfig.h>

#include <string>
#include <prinrval.h>
#include <prlong.h>
#include <vpr/Util/Interval.h>


namespace vpr
{

/** Prints the current NSPR error state to the given output stream. */
extern void NSPR_PrintError(const std::string& errorPrefixString,
                            std::ostream& = std::cout);

/** Converts the given vpr::Interval object to an NSPR interval. */
inline PRIntervalTime NSPR_getInterval(const vpr::Interval& interval)
{
   if(interval == vpr::Interval::NoTimeout)
   {
      return PR_INTERVAL_NO_TIMEOUT;
   }
   else if(interval == vpr::Interval::NoWait)
   {
      return PR_INTERVAL_NO_WAIT;
   }
   else
   {
      PRUint32 usec;
      LL_L2UI(usec, interval.usec());
      return PR_MicrosecondsToInterval(usec);
   }
}

} // namespace


#endif
