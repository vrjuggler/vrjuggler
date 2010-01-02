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

#include <vpr/vprConfig.h>

#include <sstream>
#include <prio.h>
#include <private/pprio.h>
#include <prinrval.h>

#include <vpr/IO/WouldBlockException.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/Socket/SimpleAllocationStrategies.h>
#include <vpr/Util/Error.h>
#include <vpr/Util/Debug.h>
#include <vpr/md/NSPR/NSPRHelpers.h>
#include <vpr/md/NSPR/IO/Socket/SocketStreamImplNSPR.h>


namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

// Constructor.  This takes the address (either hostname or IP address) of a
// remote site and a port and stores the values for later use in the member
// variables of the object.
SocketStreamImplNSPR::SocketStreamImplNSPR()
   : SocketImplNSPR(vpr::SocketTypes::STREAM)
   , mCorked(false)
   , mCorkedWriter(doublingAllocationStrategy)
{
   /* Do nothing. */ ;
}

// Constructor.  This takes the address (either hostname or IP address) of a
// remote site and a port and stores the values for later use in the member
// variables of the object.
SocketStreamImplNSPR::SocketStreamImplNSPR(const vpr::InetAddr& localAddr,
                                           const vpr::InetAddr& remoteAddr)
   : SocketImplNSPR(localAddr, remoteAddr, vpr::SocketTypes::STREAM)
   , mCorked(false)
   , mCorkedWriter(doublingAllocationStrategy)
{
   ;
}

// XXX: We need to have a reference count here
SocketStreamImplNSPR::SocketStreamImplNSPR(const SocketStreamImplNSPR& sock)
   : SocketImplNSPR(sock)
   , mCorked(sock.mCorked)
   , mCorkedWriter(sock.mCorkedWriter)
{
   /* Just call base class */ ;
}

SocketStreamImplNSPR::~SocketStreamImplNSPR()
{
   /* Do nothing. */ ;
}

// Listen on the socket for incoming connection requests.
void SocketStreamImplNSPR::listen(const int backlog)
{
   if ( !mBound )        // To listen, we must be bound
   {
      std::stringstream msg_stream;
      msg_stream << "[vpr::SocketStreamImplNSPR::listen()] "
              << "Trying to listen on an unbound socket.";
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << msg_stream.str() << std::endl << vprDEBUG_FLUSH;
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
   else
   {
      // Put the socket into listning mode.  If that fails, print an error
      // and return error status.
      PRStatus status = PR_Listen(mHandle, backlog);

      if ( PR_FAILURE == status )
      {
         std::stringstream msg_stream;
         msg_stream << "[vpr::SocketStreamImplNSPR::listen()] "
                    << "Cannot listen on socket: "
                    << vpr::Error::getCurrentErrorMsg();
         throw SocketException(msg_stream.str(), VPR_LOCATION);
      }
   }
}

// Accept an incoming connection request.
void SocketStreamImplNSPR::accept(SocketStreamImplNSPR& sock,
                                  const vpr::Interval& timeout)
{
   vpr::InetAddr addr;

   if ( ! mBound )        // To listen, we must be bound
   {
      std::stringstream msg_stream;
      msg_stream << "[vpr::SocketStreamImplNSPR::listen()] "
                 << "Trying to accept on an unbound socket.";
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << msg_stream.str() << std::endl << vprDEBUG_FLUSH;
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
   else
   {
      PRFileDesc* accept_sock = NULL;

      // PR_Accept() is a potentially blocking call.
      mBlockingFixed = true;

      // Accept an incoming connection request.
      vprASSERT(mHandle != NULL);
      accept_sock = PR_Accept(mHandle, addr.getPRNetAddr(),
                              NSPR_getInterval(timeout) );

      if ( NULL == accept_sock )
      {
         PRErrorCode err_code = PR_GetError();

         if ( err_code == PR_WOULD_BLOCK_ERROR )
         {
            throw WouldBlockException("Would block while accepting.",
                                      VPR_LOCATION);
         }
         else if ( err_code == PR_IO_TIMEOUT_ERROR )
         {
#if defined(WINNT)
            // Handle the case of a timeout error on an NT socket. We have to
            // tell NSPR to put the socket back into the right state. We do
            // not need to worry about whether the socket is blocking because
            // the timeout is ignored by non-blocking NSPR sockets.
            PR_NT_CancelIo(mHandle);
#endif

            throw TimeoutException("Timeout occured when accepting connection.",
                                   VPR_LOCATION);
         }
         else
         {
            std::stringstream msg_stream;
            msg_stream << "[vpr::SocketStreamImplNSPR::listen()] "
                       << "Cannot accept on socket: "
                       << vpr::Error::getCurrentErrorMsg();
            throw SocketException(msg_stream.str(), VPR_LOCATION);
         }
      }
      // Otherwise, put the new socket in the passed socket object.
      else
      {
         sock.mHandle = accept_sock;
         sock.setRemoteAddr(addr);
         sock.mOpen = true;
         sock.mBound = true;
         sock.mConnectCalled = true;
         sock.mBlocking = mBlocking;
         sock.mBlockingFixed = true;
      }
   }
}

vpr::Uint32 SocketStreamImplNSPR::write_i(const void* buffer,
                                          const vpr::Uint32 length,
                                          const vpr::Interval& timeout)
{
   vpr::Uint32 written(0);

   if ( mCorked )
   {
      written = mCorkedWriter.write(buffer, length);
   }
   else
   {
      written = SocketImplNSPR::write_i(buffer, length, timeout);
   }

   return written;
}

void SocketStreamImplNSPR::cork()
{
   mCorked = true;
}

void SocketStreamImplNSPR::uncork()
{
   if ( mCorked )
   {
      mCorked = false;

      if ( mCorkedWriter.getBufferUse() > 0 )
      {
         SocketImplNSPR::write_i(mCorkedWriter.getBuffer(),
                                 mCorkedWriter.getBufferUse(),
                                 vpr::Interval::NoTimeout);
      }

      mCorkedWriter.flush();
   }
}

} // End of vpr namespace
