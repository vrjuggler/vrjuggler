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
 *    - This file (vprSystemNSPR.h) MUST be included by vprSystemBase.h,
 *      not the other way around.
 * --------------------------------------------------------------------------
 */

#ifndef _VPR_SYSTEM_NSPR_H_
#define _VPR_SYSTEM_NSPR_H_

#include <vpr/vprConfig.h>

#ifdef VPR_OS_Win32
#  include <winsock2.h>    /* For struct timeval */
#endif

#include <stdlib.h>
#include <string.h>
#include <string>
#include <prtime.h>
#include <prnetdb.h>
#include <prenv.h>
#include <prthread.h>      /* For PR_Sleep() */
#include <prsystem.h>

#include <vpr/Util/ReturnStatus.h>
#include <vpr/vprTypes.h>
#include <vpr/SystemBase.h>


#ifndef HAVE_GETTIMEOFDAY
#  ifndef VPR_OS_Win32
struct timeval
{
   long tv_sec;           /* seconds since Jan. 1, 1970 */
   long tv_usec;          /* and microseconds */
};
#  endif

struct timezone
{
   int tv_minuteswest;    /* minutes west of Greenwich */
   int tv_dsttime;        /* type of dst correction */
};
#else
#  include <sys/time.h>
#endif


namespace vpr
{

class SystemNSPR : public SystemBase
{
public:
   /**
    * Sleeps for the given number of microseconds.
    *
    * @param micro The number of microseconds to sleep.
    */
   static int usleep(vpr::Uint32 micro)
   {
      return PR_Sleep(PR_MicrosecondsToInterval(micro));
   }

   /**
    * Sleeps for the given number of milliseconds.
    *
    * @param micro The number of milliseconds to sleep.
    */
   static int msleep(vpr::Uint32 milli)
   {
      return PR_Sleep(PR_MillisecondsToInterval(milli));
   }

   /**
    * Sleeps for the given number of seconds.
    *
    * @param micro The number of seconds to sleep.
    */
   static int sleep(vpr::Uint32 seconds)
   {
      return PR_Sleep(PR_SecondsToInterval(seconds));
   }

   static int gettimeofday (struct timeval* tp, struct timezone* tzp = NULL)
   {
      PRTime now = PR_Now();

      tp->tv_sec  = now / 1000000;
      tp->tv_usec = now % 1000000;

      return 0;
   }

   static vpr::Uint16 Ntohs(vpr::Uint16 conversion)
   {
      return PR_ntohs(conversion);
   }
   static vpr::Uint32 Ntohl(vpr::Uint32 conversion)
   {
      return PR_ntohl(conversion);
   }
   static vpr::Uint64 Ntohll(vpr::Uint64 conversion)
   {
      vpr::Uint64 ret_val;
      if (isLittleEndian())
      {
         *((vpr::Uint32*)(&ret_val) + 1) = SystemNSPR::Ntohl(*((vpr::Uint32*)(&conversion)));
         *( ((vpr::Uint32*)(&ret_val))) = SystemNSPR::Ntohl( *( ((vpr::Uint32*)(&conversion))+1) );
      }
      else
      {
         *((vpr::Uint32*)(&ret_val)) = SystemNSPR::Ntohl(*((vpr::Uint32*)(&conversion)));
         *( ((vpr::Uint32*)(&ret_val)) + 1) = SystemNSPR::Ntohl( *( ((vpr::Uint32*)(&conversion))+1) );
      }
      return ret_val;
   }
   static vpr::Uint16 Htons(vpr::Uint16 conversion)
   {
      return PR_htons(conversion);
   }
   static vpr::Uint32 Htonl(vpr::Uint32 conversion)
   {
      return PR_htonl(conversion);
   }
   static vpr::Uint64 Htonll(vpr::Uint64 conversion)
   {
      vpr::Uint64 ret_val;
      if (isLittleEndian())
      {
         *((vpr::Uint32*)(&ret_val) + 1) = SystemNSPR::Htonl(*((vpr::Uint32*)(&conversion)));
         *( ((vpr::Uint32*)(&ret_val))) = SystemNSPR::Htonl( *( ((vpr::Uint32*)(&conversion))+1) );
      }
      else
      {
         *((vpr::Uint32*)(&ret_val)) = SystemNSPR::Htonl(*((vpr::Uint32*)(&conversion)));
         *( ((vpr::Uint32*)(&ret_val)) + 1) = SystemNSPR::Htonl( *( ((vpr::Uint32*)(&conversion))+1) );
      }
      return ret_val;
   }

   static ReturnStatus getenv (const std::string& name, std::string& result)
   {
      char* val;
      ReturnStatus status;

      val = PR_GetEnv(name.c_str());

      if ( val != NULL )
      {
         result = val;
         status.setCode(ReturnStatus::Succeed);
      }
      else
      {
         status.setCode(ReturnStatus::Fail);
      }

      return status;
   }

   /*
   * If value is "" then it either unsets the variable or clears it
   */
   static ReturnStatus setenv (const std::string& name,
                               const std::string& value)
   {
      // NSPR requires form of "name=value"
      // NSPR takes possesion of the string memory, so we just leak here
      std::string* set_value = new std::string(name);
      (*set_value) += "=";
      (*set_value) += value;

      ReturnStatus status;

      PRStatus ret_val = PR_SetEnv(set_value->c_str());

      if ( ret_val == PR_SUCCESS )
      {
         status.setCode(ReturnStatus::Succeed);
      }
      else
      {
         status.setCode(ReturnStatus::Fail);
      }

      return status;
   }

   /** Returns the name of the host
    * For example the hostname of: vapor.vrjuggler.org is "vapor"
    */
   static std::string getHostname()
   {
      char  hn_buf[SYS_INFO_BUFFER_LENGTH];

      //PR_SI_HOSTNAME
      //PR_SI_SYSNAME
      PRStatus ret_val = PR_GetSystemInfo( PR_SI_HOSTNAME, hn_buf, SYS_INFO_BUFFER_LENGTH );
      if ( ret_val == PR_SUCCESS )
         return std::string(hn_buf);
      else
         return std::string("<hostname-lookup failed>");
   }
};

} // End of vpr namespace


#endif   /* _VPR_SYSTEM_NSPR_H_ */
