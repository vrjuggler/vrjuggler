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

/*
 * --------------------------------------------------------------------------
 * NOTES:
 *    - This file (vprSystemPosix.h) MUST be included by vprSystemBase.h,
 *      not the other way around.
 * --------------------------------------------------------------------------
 */

#ifndef _VPR_SYSTEM_POSIX_H_
#define _VPR_SYSTEM_POSIX_H_

#include <vpr/vprConfig.h>

#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/time.h>

// I don't know why this is necessary, but I think something is being defined
// incorrectly somewhere.
#ifdef VPR_OS_IRIX
#  include <sys/endian.h>
#endif

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/utsname.h>

#include <vpr/Util/ReturnStatus.h>
#include <vpr/SystemBase.h>


namespace vpr
{

class SystemPosix : public SystemBase
{
public:
   static int gettimeofday (struct timeval* tp, struct timezone* tzp = NULL)
   {
      return ::gettimeofday(tp, tzp);
   }

   // ----- Host to network byte order conversions ---- //
   static vpr::Uint16 Ntohs (vpr::Uint32 conversion)
   {
      return ntohs(conversion);
   }

   static vpr::Uint32 Ntohl (vpr::Uint32 conversion)
   {
      return ntohl(conversion);
   }

   static vpr::Uint64 Ntohll(vpr::Uint64 conversion)
   {
      vpr::Uint64 ret_val;
      *((vpr::Uint32*)(&ret_val)) = SystemPosix::Ntohl(*((vpr::Uint32*)(&conversion)));
      *( ((vpr::Uint32*)(&ret_val)) + 1) = SystemPosix::Ntohl( *( ((vpr::Uint32*)(&conversion))+1) );
      return ret_val;
   }

   static vpr::Uint16 Htons (vpr::Uint16 conversion)
   {
      return htons(conversion);
   }

   static vpr::Uint32 Htonl (vpr::Uint32 conversion)
   {
      return htonl(conversion);
   }

   static vpr::Uint64 Htonll(vpr::Uint64 conversion)
   {
      vpr::Uint64 ret_val;
      *((vpr::Uint32*)(&ret_val)) = SystemPosix::Htonl(*((vpr::Uint32*)(&conversion)));
      *( ((vpr::Uint32*)(&ret_val)) + 1) = SystemPosix::Htonl( *( ((vpr::Uint32*)(&conversion))+1) );
      return ret_val;
   }

   static ReturnStatus getenv (const std::string& name, std::string& result)
   {
      char* val;
      ReturnStatus status;

      val = ::getenv(name.c_str());

      if ( val != NULL )
      {
         result = val;
      }
      else
      {
         status.setCode(ReturnStatus::Fail);
      }

      return status;
   }

   /**
    * Returns the name of the host.
    * For example the hostname of: vapor.vrjuggler.org is "vapor"
    */
   static std::string getHostname (void)
   {
      struct utsname buffer;

      if ( uname(&buffer) == 0 )
      {
         char* temp;
         temp = strchr(buffer.nodename, '.');

         // If the node name contains the full host, dots and all, truncate it
         // at the first dot.
         if ( temp != NULL )
         {
            *temp = '\0';
         }

         return std::string(buffer.nodename);
      }
      else
      {
         return std::string("<hostname-lookup failed>");
      }
   }
};

} // End of vpr namespace


#endif   /* _VPR_SYSTEM_POSIX_H_ */
