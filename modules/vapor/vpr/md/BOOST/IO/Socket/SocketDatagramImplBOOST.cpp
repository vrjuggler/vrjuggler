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

#include <string.h>
//#include <strings.h>
#include <sstream>
#include <sys/types.h>
//#include <sys/socket.h>
#include <errno.h>

#include <vpr/IO/Socket/ConnectionResetException.h>
#include <vpr/IO/Socket/UnknownHostException.h>
#include <vpr/IO/Socket/NoRouteToHostException.h>
#include <vpr/IO/IOException.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/WouldBlockException.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vpr/md/BOOST/IO/Socket/SocketDatagramImplBOOST.h>


namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

SocketDatagramImplBOOST::SocketDatagramImplBOOST()
   : SocketImplBOOST(vpr::SocketTypes::DATAGRAM)
{
   /* Do nothing. */ ;
}

SocketDatagramImplBOOST::SocketDatagramImplBOOST(const InetAddr& localAddr,
                                             const InetAddr& remoteAddr)
   : SocketImplBOOST(localAddr, remoteAddr, vpr::SocketTypes::DATAGRAM)
{
   /* Do nothing. */ ;
}

SocketDatagramImplBOOST::SocketDatagramImplBOOST(const SocketDatagramImplBOOST& sock)
   : SocketImplBOOST(sock.mLocalAddr, sock.mRemoteAddr, SocketTypes::DATAGRAM)
{
   mUdpSocket = sock.mUdpSocket;
   mTcpSocket = sock.mTcpSocket;
   mLocalAddr = sock.mLocalAddr;
   mRemoteAddr = sock.mRemoteAddr;
   //mIOService = sock.mIOService;
}

vpr::Uint32 SocketDatagramImplBOOST::recvfrom(void* msg,
                                            const vpr::Uint32 length,
                                            vpr::InetAddr& from,
                                            const vpr::Interval& timeout)
{
   vpr::Uint32 bytes_read(0);
   boost::system::error_code ec;

   bytes_read = mUdpSocket->receive_from(boost::asio::buffer(msg, length), from.mUdpAddr, 0, ec);

   if (ec)
   {
      //TODO Handle error
   }
   if (bytes_read == 0)
   {
       throw SocketException("Socket not connected.");
   }

   return bytes_read;
}

vpr::Uint32 SocketDatagramImplBOOST::sendto(const void* msg,
                                          const vpr::Uint32 length,
                                          const vpr::InetAddr& to,
                                          const vpr::Interval& timeout)
{
   boost::system::error_code ec;
   vpr::Uint32 bytes_sent(0);

   bytes_sent = mUdpSocket->send_to(boost::asio::buffer(msg, length),to.mUdpAddr, 0, ec);

   if (ec)
   {
      //TODO handle errors
   }

   return bytes_sent;
}

} // End of vpr namespace
