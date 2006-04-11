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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

SocketDatagramImplBSD::SocketDatagramImplBSD()
   : SocketImplBSD(vpr::SocketTypes::DATAGRAM)
{
   /* Do nothing. */ ;
}

SocketDatagramImplBSD::SocketDatagramImplBSD(const InetAddr& localAddr,
                                             const InetAddr& remoteAddr)
   : SocketImplBSD(localAddr, remoteAddr, vpr::SocketTypes::DATAGRAM)
{
   /* Do nothing. */ ;
}

SocketDatagramImplBSD::SocketDatagramImplBSD(const SocketDatagramImplBSD& sock)
   : SocketImplBSD(sock.mLocalAddr, sock.mRemoteAddr, SocketTypes::DATAGRAM)
{
   mHandle            = new FileHandleImplUNIX(sock.mHandle->getName());
   mHandle->mFdesc    = sock.mHandle->mFdesc;
   mHandle->mOpen     = sock.mHandle->mOpen;
   mHandle->mBlocking = sock.mHandle->mBlocking;
}

vpr::ReturnStatus SocketDatagramImplBSD::recvfrom(void* msg,
                                                  const vpr::Uint32 length,
                                                  vpr::InetAddr& from,
                                                  vpr::Uint32& bytesRead,
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
      bytes   = ::recvfrom(mHandle->mFdesc, msg, length, 0,
                           (struct sockaddr*) &from.mAddr, &fromlen);

      if ( bytes == -1 )
      {
         bytesRead = 0;

         if ( errno == EAGAIN && ! isBlocking() )
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
         bytesRead = bytes;
      }
   }

   return retval;
}

vpr::ReturnStatus SocketDatagramImplBSD::sendto(const void* msg,
                                                const vpr::Uint32 length,
                                                const vpr::InetAddr& to,
                                                vpr::Uint32& bytesSent,
                                                const vpr::Interval timeout)
{
   vpr::ReturnStatus retval;

   retval = mHandle->isWriteable(timeout);

   if ( retval.success() )
   {
      ssize_t bytes;

      mBlockingFixed = true;

      bytes = ::sendto(mHandle->mFdesc, msg, length, 0,
                       (struct sockaddr*) &to.mAddr, to.size());

      if ( bytes == -1 )
      {
         bytesSent = 0;

         if ( errno == EAGAIN && ! isBlocking() )
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
         bytesSent = bytes;
      }
   }

   return retval;
}

} // End of vpr namespace
