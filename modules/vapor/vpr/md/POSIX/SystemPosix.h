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

/**
 * \file
 *
 * @note This file MUST be included by vpr/System.h, not the other way around.
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

#include <vpr/vprTypes.h>
#include <vpr/SystemBase.h>


namespace vpr
{

/** \class SystemPosix SystemPosix.h vpr/System.h
 *
 * Low-level operating system feature abstractions using POSIX functionality.
 * This is typedef'd to vpr::System.
 */
class VPR_CLASS_API SystemPosix : public SystemBase
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
    * @param milli The number of milliseconds to sleep.
    */
   static int msleep(vpr::Uint32 milli);

   /**
    * Sleeps for the given number of seconds.
    *
    * @param seconds The number of seconds to sleep.
    */
   static int sleep(vpr::Uint32 seconds)
   {
      return ::sleep(seconds);
   }

   /**
    * Gets the current time of day in seconds since January 1, 1970.
    *
    * @param tp  Storage for the timeval data structure.
    * @param tzp Storage for the time zone.  This parameter is optional and
    *            defaults to NULL.
    */
   static int gettimeofday(vpr::TimeVal* tp, vpr::TimeZone* tzp = NULL)
   {
      return ::gettimeofday(tp, tzp);
   }

   /** @name Host-to-network byte order conversions */
   //@{
   /**
    * Converts the given 16-bit value (a short) from native byte ordering to
    * network byte ordering.  This is safe to use with signed and unsigned
    * values.
    */
   static vpr::Uint16 Ntohs(vpr::Uint32 conversion)
   {
      return ntohs(conversion);
   }

   /**
    * Converts the given 32-bit value (a long) from native byte ordering to
    * network byte ordering.  This is safe to use with signed and unsigned
    * values.
    */
   static vpr::Uint32 Ntohl(vpr::Uint32 conversion)
   {
      return ntohl(conversion);
   }

   /**
    * Converts the given 64-bit value (a long long) from native byte ordering
    * to network byte ordering.  This is safe to use with signed and unsigned
    * values.
    */
   static vpr::Uint64 Ntohll(vpr::Uint64 conversion);
   //@}

   /** @name Host-to-network byte order conversions */
   //@{
   /**
    * Converts the given 16-bit value (a short) from network byte ordering to
    * native byte ordering.  This is safe to use with signed and unsigned
    * values.
    */
   static vpr::Uint16 Htons(vpr::Uint16 conversion)
   {
      return htons(conversion);
   }

   /**
    * Converts the given 32-bit value (a long) from network byte ordering to
    * native byte ordering.  This is safe to use with signed and unsigned
    * values.
    */
   static vpr::Uint32 Htonl(vpr::Uint32 conversion)
   {
      return htonl(conversion);
   }

   /**
    * Converts the given 64-bit value (a long long) from network byte ordering
    * to native byte ordering.  This is safe to use with signed and unsigned
    * values.
    */
   static vpr::Uint64 Htonll(vpr::Uint64 conversion);
   //@}

   /**
    * Queries the run-time environment for the value of the named environment
    * variable.
    *
    * @param name   The name of the environment variable to query.
    * @param result Storage for the value of the named environment variable.
    *
    * @return \c true is returned if the named environment variable is defined
    *         in the run-time environment and lookup of the variable succeeded.
    *         \c false is returned if the named environment variable could not
    *         be found in the run-time environment.
    */
   static bool getenv(const std::string& name, std::string& result);

   /**
    * Sets the value of the named environment variable in the run-time
    * environment.  If \p value is "", then it either unsets the variable or
    * clears it.
    *
    * @post On successful completion, the named environment variable has the
    *       given value in the run-time environment.  If the named environment
    *       variable does not exist in the run-time environment, it will be
    *       created.
    *
    * @param name  The name of the environment variable to set.
    * @param value The value to assign to the named environment variable.
    *
    * @return \c true is returned if the named environment was set to the new
    *         value successfully. \c false is returned if the environment
    *         variable set operation failed.
    */
   static bool setenv(const std::string& name, const std::string& value);

   /**
    * Returns the name of the local host.
    * For example the hostname of vapor.vrjuggler.org is "vapor".
    */
   static std::string getHostname();
};

} // End of vpr namespace


#endif   /* _VPR_SYSTEM_POSIX_H_ */
