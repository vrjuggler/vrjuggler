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

/*
 * --------------------------------------------------------------------------
 * NOTES:
 *    - This file (SystemPosix.h) MUST be included by SystemBase.h, not the
 *      other way around.
 * --------------------------------------------------------------------------
 */

#ifndef _VPR_SYSTEM_POSIX_H_
#define _VPR_SYSTEM_POSIX_H_

#include <vpr/vprConfig.h>

#include <string>
#include <unistd.h>
#include <sys/time.h>

// I don't know why this is necessary, but I think something is being defined
// incorrectly somewhere.
#ifdef VPR_OS_IRIX
#  include <sys/endian.h>
#endif

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/param.h>

#include <vpr/Util/ReturnStatus.h>
#include <vpr/vprTypes.h>
#include <vpr/SystemBase.h>


namespace vpr
{

class SystemPosix : public SystemBase
{
public:
   /**
    * Sleeps for the given number of microseconds.
    *
    * @param micro The number of microseconds to sleep.
    */
   static int usleep(vpr::Uint32 micro);

   /**
    * Sleeps for the given number of milliseconds.
    *
    * @param micro The number of milliseconds to sleep.
    */
   static int msleep(vpr::Uint32 milli);

   /**
    * Sleeps for the given number of seconds.
    *
    * @param micro The number of seconds to sleep.
    */
   static int sleep(vpr::Uint32 seconds)
   {
      return ::sleep(seconds);
   }

   static int gettimeofday(struct timeval* tp, struct timezone* tzp = NULL)
   {
      return ::gettimeofday(tp, tzp);
   }

   // ----- Host to network byte order conversions ---- //
   static vpr::Uint16 Ntohs(vpr::Uint32 conversion)
   {
      return ntohs(conversion);
   }

   static vpr::Uint32 Ntohl(vpr::Uint32 conversion)
   {
      return ntohl(conversion);
   }

   static vpr::Uint64 Ntohll(vpr::Uint64 conversion);

   static vpr::Uint16 Htons(vpr::Uint16 conversion)
   {
      return htons(conversion);
   }

   static vpr::Uint32 Htonl(vpr::Uint32 conversion)
   {
      return htonl(conversion);
   }

   static vpr::Uint64 Htonll(vpr::Uint64 conversion);

   static vpr::ReturnStatus getenv(const std::string& name,
                                   std::string& result);

   static vpr::ReturnStatus setenv(const std::string& name,
                                   const std::string& value);

   /**
    * Returns the name of the host.
    * For example the hostname of: vapor.vrjuggler.org is "vapor"
    */
   static std::string getHostname();
};

} // End of vpr namespace


#endif   /* _VPR_SYSTEM_POSIX_H_ */
