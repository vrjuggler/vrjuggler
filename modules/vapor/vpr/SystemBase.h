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
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SYSTEM_BASE_H_
#define _VPR_SYSTEM_BASE_H_

#include <vpr/vprConfig.h>
#include <string>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif


namespace vpr
{

#if defined(HAVE_TIMEVAL) && ! defined(VPR_OS_Windows)
/**
 * @since 0.92.4
 */
typedef struct timeval TimeVal;
#else
/** \struct TimeVal SystemBase.h vpr/System.h
 *
 * A data structure for storing the time value returned by
 * vpr::System::gettimeofday().
 *
 * @since 0.92.4
 */
struct TimeVal
{
   long tv_sec;           /**< Seconds since Jan. 1, 1970 */
   long tv_usec;          /**< and microseconds */
};
#endif

#ifdef HAVE_TIMEZONE
typedef struct timezone TimeZone;
#else
/** \struct TimeZone SystemBase.h vpr/System.h
 *
 * A data structure for storing the time zone returned by
 * vpr::System::gettimeofday().  This is only defined on platforms that
 * do not defined the \c gettimeofday(3) function.
 */
struct TimeZone
{
   int tv_minuteswest;    /**< Minutes west of Greenwich */
   int tv_dsttime;        /**< Type of dst correction */
};
#endif

/** \class SystemBase SystemBase.h vpr/SystemBase.h
 *
 * vpr::SystemBase is a base class for vpr::System.  For cross-platform system
 * functions, use vpr::System.  Never use this class directly.
 *
 * @see vpr::SystemPosix, vpr::SystemNSPR
 */
class VPR_CLASS_API SystemBase
{
public:
   /**
    * Determines the endianness of the host platform.  A return nvalue of 0
    * means that the host uses little-endian byte order.  A return value of
    * 1 means that the host uses big-endian byte order.
    *
    * @return 0 is returned for little-endian hosts.
    * @return 1 is returned for big-endian hosts.
    */
   static int getEndian()
   {
      union
      {
         char   c[sizeof(short)];
         short  value;
      } endian;

      // The way this works is that we access the first byte of endian.value
      // directly.  If it is 1, the host treats that as the high-order byte.
      // Otherwise, it is the low-order byte.
      endian.value = 256;

      return endian.c[0];
   }

   /**
    * Tells if the host uses little-endian byte order or not.
    *
    * @return \c true is returned on a little-endian host.
    * @return \c false is returned on a big-endian host.
    */
   static bool isLittleEndian()
   {
      return(getEndian() == 0);
   }

   /**
    * Tells if the host uses big-endian byte order or not.
    *
    * @return \c true is returned on a big-endian host.
    * @return \c false is returned on a little-endian host.
    */
   static bool isBigEndian()
   {
      return(getEndian() == 1);
   }

   /**
    * Returns a stack trace.
    *
    * @post If supported, returns a string describing the current call stack.
    */
   static std::string getCallStack();
};

} // End of vpr namespace


#endif   /* _VPR_SYSTEM_BASE_H_ */
