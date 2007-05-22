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

#include <sstream>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include <vpr/Util/Debug.h>
#include <vpr/md/POSIX/IO/Socket/SocketStreamImplBSD.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/WouldBlockException.h>


namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

SocketStreamImplBSD::SocketStreamImplBSD()
   : SocketImplBSD(vpr::SocketTypes::STREAM)
{
   /* Do nothing. */ ;
}

SocketStreamImplBSD::SocketStreamImplBSD(const InetAddr& localAddr,
                                         const InetAddr& remoteAddr)
   : SocketImplBSD(localAddr, remoteAddr, SocketTypes::STREAM)
{
   /* Do nothing. */ ;
}

SocketStreamImplBSD::SocketStreamImplBSD(const SocketStreamImplBSD& sock)
   : SocketImplBSD(sock.mLocalAddr, sock.mRemoteAddr, SocketTypes::STREAM)
{
   // mHandle is created in the base class constructor. Since we are creating
   // a copy we must clean up existing memory.
   if (NULL != mHandle)
   {
      delete mHandle;
      mHandle = NULL;
   }

   mHandle         = new FileHandleImplUNIX(sock.mHandle->getName());
   mHandle->mFdesc = sock.mHandle->mFdesc;
}

// Listen on the socket for incoming connection requests.
void SocketStreamImplBSD::listen(const int backlog)
{
   // Put the socket into listning mode.  If that fails, print an error and
   // return error status.
   if ( ::listen(mHandle->mFdesc, backlog) == -1 )
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketStreamImplBSD::listen()] Cannot listen on "
                 << "socket: " << strerror(errno);
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

// Accept an incoming connection request.
void SocketStreamImplBSD::accept(SocketStreamImplBSD& sock,
                                 vpr::Interval timeout)
{
   int accept_sock;
   InetAddr addr;
#if defined(VPR_OS_IRIX) || defined(VPR_OS_HPUX)
   int addrlen;
#else
   socklen_t addrlen;
#endif

   if (!mHandle->isReadable(timeout))
   {
      throw TimeoutException("Timeout occured when accepting connection.",
         VPR_LOCATION);
   }

   mBlockingFixed = true;

   // Accept an incoming connection request.
   addrlen = addr.size();
   accept_sock = ::accept(mHandle->mFdesc,
                          (struct sockaddr*) &addr.mAddr, &addrlen);

   // If accept(2) failed, print an error message and return error stauts.
   if ( accept_sock == -1 )
   {
      if ( errno == EWOULDBLOCK && ! isBlocking() )
      {
         throw WouldBlockException("Would block while accepting.",
                                   VPR_LOCATION);
      }
      else
      {
         std::ostringstream ss;
         ss << "[vpr::SocketStreamImplBSD::accept()] Error while accepting "
            << "incoming connection: " << strerror(errno);
         throw SocketException(ss.str(), VPR_LOCATION);
      }
   }
   // Otherwise, put the new socket in the passed socket object.
   else
   {
      sock.setRemoteAddr(addr);

      // Clean up the existing handle.
      if (NULL != sock.mHandle)
      {
         delete sock.mHandle;
         sock.mHandle = NULL;
      }
      sock.mHandle         = new FileHandleImplUNIX(addr.getAddressString());
      sock.mHandle->mFdesc = accept_sock;
      sock.mHandle->mOpen  = true;

      sock.setBlocking(this->isBlocking());

      sock.mBound         = true;
      sock.mConnectCalled = true;
      sock.mBlockingFixed = true;
   }
}

} // End of vpr namespace
