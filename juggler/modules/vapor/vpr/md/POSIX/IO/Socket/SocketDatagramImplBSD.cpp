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

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vpr/md/POSIX/IO/Socket/SocketDatagramImplBSD.h>


namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
vpr::ReturnStatus SocketDatagramImplBSD::recvfrom (void* msg,
                                                   const vpr::Uint32 length,
                                                   const int flags,
                                                   vpr::InetAddr& from,
                                                   vpr::Uint32& bytes_read,
                                                   const vpr::Interval timeout)
{
#if defined(VPR_OS_IRIX) || defined(VPR_OS_HPUX)
   int fromlen;
#else
   socklen_t fromlen;
#endif

   vpr::ReturnStatus retval;

   retval = mHandle->isReadable(timeout);

   if ( retval.success() )
   {
      ssize_t bytes;

      mBlockingFixed = true;

      fromlen = from.size();
      bytes   = ::recvfrom(mHandle->mFdesc, msg, length, flags,
                           (struct sockaddr*) &from.mAddr, &fromlen);

      if ( bytes == -1 )
      {
         bytes_read = 0;

         if ( errno == EAGAIN && getNonBlocking() )
         {
            retval.setCode(vpr::ReturnStatus::WouldBlock);
         }
         else
         {
            fprintf(stderr,
                    "[vpr::SocketDatagramImplBSD] ERROR: Could not read from socket (%s:%hu): %s\n",
                    mRemoteAddr.getAddressString().c_str(),
                    mRemoteAddr.getPort(), strerror(errno));
            retval.setCode(vpr::ReturnStatus::Fail);
         }
      }
      else if ( bytes == 0 )
      {
         retval.setCode(vpr::ReturnStatus::NotConnected);
      }
      else
      {
         bytes_read = bytes;
      }
   }

   return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
vpr::ReturnStatus SocketDatagramImplBSD::sendto (const void* msg,
                                                 const vpr::Uint32 length,
                                                 const int flags,
                                                 const vpr::InetAddr& to,
                                                 vpr::Uint32& bytes_sent,
                                                 const vpr::Interval timeout)
{
   vpr::ReturnStatus retval;

   retval = mHandle->isWriteable(timeout);

   if ( retval.success() )
   {
      ssize_t bytes;

      mBlockingFixed = true;

      bytes = ::sendto(mHandle->mFdesc, msg, length, flags,
                       (struct sockaddr*) &to.mAddr, to.size());

      if ( bytes == -1 )
      {
         bytes_sent = 0;

         if ( errno == EAGAIN && getNonBlocking() )
         {
            retval.setCode(vpr::ReturnStatus::WouldBlock);
         }
         else
         {
            fprintf(stderr,
                    "[vpr::SocketDatagramImplBSD] ERROR: Could not send to %s:%hu on socket (%s:%hu): %s\n",
                    to.getAddressString().c_str(), to.getPort(),
                    mRemoteAddr.getAddressString().c_str(),
                    mRemoteAddr.getPort(), strerror(errno));
            retval.setCode(vpr::ReturnStatus::Fail);
         }
      }
      else if ( errno == EHOSTUNREACH || errno == EHOSTDOWN ||
                errno == ENETDOWN )
      {
         retval.setCode(vpr::ReturnStatus::NotConnected);
      }
      else
      {
         bytes_sent = bytes;
      }
   }

   return retval;
}

} // End of vpr namespace
