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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
//#include <strings.h>
#include <sys/types.h>
//#include <sys/socket.h>
#include <errno.h>

#include <vpr/Util/Debug.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/WouldBlockException.h>
#include <vpr/IO/Socket/SimpleAllocationStrategies.h>
#include <vpr/md/BOOST/IO/Socket/SocketStreamImplBOOST.h>


namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

SocketStreamImplBOOST::SocketStreamImplBOOST()
   : SocketImplBOOST(vpr::SocketTypes::STREAM)
   , mCorked(false)
   , mCorkedWriter(doublingAllocationStrategy)
{
   vpr::Uint16 port = getLocalAddr().getPort();
   mAcceptor = new boost::asio::ip::tcp::acceptor(mIOService,
      boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
}

SocketStreamImplBOOST::SocketStreamImplBOOST(const InetAddr& localAddr,
                                         const InetAddr& remoteAddr)
   : SocketImplBOOST(localAddr, remoteAddr, SocketTypes::STREAM)
   , mCorked(false)
   , mCorkedWriter(doublingAllocationStrategy)
{
   vpr::Uint16 port = getLocalAddr().getPort();
   mAcceptor = new boost::asio::ip::tcp::acceptor(mIOService,
      boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
}

SocketStreamImplBOOST::SocketStreamImplBOOST(const SocketStreamImplBOOST& sock)
   : SocketImplBOOST(sock.mLocalAddr, sock.mRemoteAddr, SocketTypes::STREAM)
   , mCorked(sock.mCorked)
   , mCorkedWriter(sock.mCorkedWriter)
{
   mUdpSocket = sock.mUdpSocket;
   mTcpSocket = sock.mTcpSocket;
   mLocalAddr = sock.mLocalAddr;
   mRemoteAddr = sock.mRemoteAddr;
   //mIOService = sock.mIOService;
   vpr::Uint16 port = getLocalAddr().getPort();
   mAcceptor = new boost::asio::ip::tcp::acceptor(mIOService,
      boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
}

SocketStreamImplBOOST::~SocketStreamImplBOOST()
{
   delete mAcceptor;
   /* Do nothing. */ ;
}

// Listen on the socket for incoming connection requests.
void SocketStreamImplBOOST::listen(const int backlog)
{
   boost::system::error_code ec;
   
   mAcceptor->listen(backlog, ec);

   if (ec)
   {
      std::ostringstream msg_stream;
      msg_stream << "[vpr::SocketStreamImplBOOST::listen()] Cannot listen on "
         << "socket: " << strerror(errno) << ec.value() <<ec.message();
      throw SocketException(msg_stream.str(), VPR_LOCATION);
   }
}

// Accept an incoming connection request.
void SocketStreamImplBOOST::accept(SocketStreamImplBOOST& sock,
                                 const vpr::Interval& timeout)
{
   boost::system::error_code ec;
   InetAddr addr;
   mBlockingFixed = true;
   mAcceptor->accept(*(sock.mTcpSocket), ec);
   
   if (ec)
   {
      if ( errno == EWOULDBLOCK && ! isBlocking() )
      {
         throw WouldBlockException("Would block while accepting.",
                                   VPR_LOCATION);
      }
      else
      {
         std::ostringstream ss;
         ss << "[vpr::SocketStreamImplBOOST::accept()] Error while accepting "
            << "incoming connection: " << strerror(errno);
         throw SocketException(ss.str(), VPR_LOCATION);
      }
   }
   // Otherwise, put the new socket in the passed socket object.
   else
   {
      sock.setBlocking(this->isBlocking());
      sock.mBound         = true;
      sock.mConnectCalled = true;
      sock.mBlockingFixed = true;
      sock.mRemoteAddr.mTcpAddr    = sock.mTcpSocket->remote_endpoint();
      sock.mLocalAddr.mTcpAddr     = sock.mTcpSocket->local_endpoint();
   }
}

vpr::Uint32 SocketStreamImplBOOST::write_i(const void* buffer,
                                         const vpr::Uint32 length,
                                         const vpr::Interval& timeout)
{
   mInBlockingCall = true;
#if ! defined(HAVE_CORKABLE_TCP)
   vpr::Uint32 written(0);

   if ( mCorked )
   {
      written = mCorkedWriter.write(buffer, length);
   }
   else
   {
      written = SocketImplBOOST::write_i(buffer, length, timeout);
   }
   mInBlockingCall = false;
   return written;
#else
   mInBlockingCall = false;
   return SocketImplBOOST::write_i(buffer, length, timeout);
#endif
}

void SocketStreamImplBOOST::cork()
{
#if ! defined(HAVE_CORKABLE_TCP)
   mCorked = true;
#endif
}

void SocketStreamImplBOOST::uncork()
{
#if ! defined(HAVE_CORKABLE_TCP)
   if ( mCorked )
   {
      mCorked = false;

      if ( mCorkedWriter.getBufferUse() > 0 )
      {
         SocketImplBOOST::write_i(mCorkedWriter.getBuffer(),
                                mCorkedWriter.getBufferUse(),
                                vpr::Interval::NoTimeout);
      }

      mCorkedWriter.flush();
   }
#endif
}

} // End of vpr namespace
