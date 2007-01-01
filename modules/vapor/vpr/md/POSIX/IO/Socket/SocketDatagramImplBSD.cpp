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
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <string.h>
#include <strings.h>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include <vpr/IO/Socket/ConnectionResetException.h>
#include <vpr/IO/Socket/UnknownHostException.h>
#include <vpr/IO/Socket/NoRouteToHostException.h>
#include <vpr/IO/IOException.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/WouldBlockException.h>
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

void SocketDatagramImplBSD::recvfrom(void* msg,
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

   // If not readable within timeout interval throw exception.
   if (!mHandle->isReadable(timeout))
   {
      bytesRead = 0;
      throw TimeoutException("Timeout occured while trying to read from: "
         + mHandle->getName(), VPR_LOCATION);
   }

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
         throw WouldBlockException("Would block while reading.", VPR_LOCATION);
      }
      else
      {
         std::ostringstream ss;
         ss << "[vpr::SocketDatagramImplBSD::recvfrom()] ERROR: Could not "
            << "read from socket (" << mRemoteAddr << "): "
            << strerror(errno);
         throw SocketException(ss.str(), VPR_LOCATION);
      }
   }
   else if ( bytes == 0 )
   {
      throw SocketException("Socket not connected.");
   }
   else
   {
      bytesRead = bytes;
   }
}

void SocketDatagramImplBSD::sendto(const void* msg,
                                   const vpr::Uint32 length,
                                   const vpr::InetAddr& to,
                                   vpr::Uint32& bytesSent,
                                   const vpr::Interval timeout)
{
   // If not writable within timeout interval throw exception.
   if (!mHandle->isWriteable(timeout))
   {
      bytesSent = 0;
      throw TimeoutException("Timeout occured while trying to write to: "
         + mHandle->getName(), VPR_LOCATION);
   }

   ssize_t bytes;

   mBlockingFixed = true;

   bytes = ::sendto(mHandle->mFdesc, msg, length, 0,
                    (struct sockaddr*) &to.mAddr, to.size());

   if ( bytes == -1 )
   {
      bytesSent = 0;

      if ( errno == EAGAIN && ! isBlocking() )
      {
         throw WouldBlockException("Would block while reading.", VPR_LOCATION);
      }
      else
      {
         std::stringstream ss;
         ss << "[vpr::SocketDatagramImplBSD::sendto()] ERROR: Could not send "
            << " to " << to << " on socket (" << mRemoteAddr << "): "
            << strerror(errno);

         throw SocketException(ss.str(), VPR_LOCATION);
      }
   }
   if ( ECONNRESET == errno)
   {
      throw ConnectionResetException("Connection reset.", VPR_LOCATION);
   }
   else if (EHOSTUNREACH == errno)
   {
      throw NoRouteToHostException("No route to host.", VPR_LOCATION);
   }
   else if (EHOSTDOWN == errno)
   {
      throw UnknownHostException("Unknown host.", VPR_LOCATION);
   }
   else if (ENETDOWN == errno)
   {
      throw IOException("Network is down.", VPR_LOCATION);
   }
   else
   {
      bytesSent = bytes;
   }
}

} // End of vpr namespace
