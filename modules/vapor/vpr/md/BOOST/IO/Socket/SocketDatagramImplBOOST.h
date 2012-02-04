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

#ifndef _VPR_SOCKET_DATAGRAM_IMPL_BOOST_H_
#define _VPR_SOCKET_DATAGRAM_IMPL_BOOST_H_

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/IO/Socket/SocketException.h>
#include <vpr/md/BOOST/IO/Socket/SocketImplBOOST.h>

#include <boost/optional.hpp>

namespace vpr
{

/** \class SocketDatagramImplBOOST SocketDatagramImplBOOST.h vpr/IO/Socket/SocketDatagram.h
 *
 * Implementation class for datagram sockets using the BOOST sockets interface.
 * This is used in conjunction with vpr::SocketConfiguration to create the
 * BOOST implementation of the typedef vpr::SocketDatagram.
 *
 * @since 2.1.16
 */
class VPR_API SocketDatagramImplBOOST : public vpr::SocketImplBOOST
{
public:
   typedef SocketImplBOOST Parent;

   // ========================================================================
   // vpr::SocketDatagram interface.
   // ========================================================================

   /**
    * Default constructor.  This sets the socket type to
    * vpr::SocketTypes::DATAGRAM.
    */
   SocketDatagramImplBOOST();

   /**
    * Constructs a datagram socket using the given addresses as defaults for
    * communication channels.
    *
    * @post The member variables are initialized with the type in particular
    *       set to vpr::SocketTypes::DATAGRAM.
    *
    * @param localAddr  The local address for this socket.  This is used for
    *                   binding the socket.
    * @param remoteAddr The remote address for this socket.  This is used to
    *                   specify a default destination for all packets.
    */
   SocketDatagramImplBOOST(const InetAddr& localAddr,
                           const InetAddr& remoteAddr);

   /**
    * Copy constructor.
    *
    * @post This socket is a copy of the given socket.
    */
   SocketDatagramImplBOOST(const SocketDatagramImplBOOST& sock);

   /**
    * Receives a message from the specified address.
    *
    * @return The number of bytes read into the buffer is returned.
    *
    * @throw vpr::WouldBlockException
    *           Thrown if the file is in non-blocking mode, and there is no
    *           data to read.
    * @throw vpr::TimeoutException
    *           Thrown if the read could not begin within the timeout
    *           interval.
    * @throw vpr::SocketException
    *           Thrown if the read operation failed.
    */
   vpr::Uint32 recvfrom(void* msg, const vpr::Uint32 length,
                        vpr::InetAddr& from, const vpr::Interval& timeout);

   /**
    * Sends a message to the specified address.
    *
    * @return The number of bytes written to the socket is returned.
    *
    * @throw vpr::ConnectionResetException
    *           Thrown if connection is reset.
    * @throw vpr::NoRouteToHostException
    *           Thrown if a route to host does not exist.
    * @throw vpr::UnknownHostException
    *           Thrown if host does not exist.
    * @throw vpr::IOException
    *           Thrown if the network is down.
    * @throw vpr::WouldBlockException
    *           Thrown if the handle is in non-blocking mode, and the send
    *           operation could not be completed.
    * @throw vpr::TimeoutException
    *           Thrown if the send could not begin within the timeout
    *           interval.
    * @throw vpr::SocketException
    *           Thrown if the send operation failed.
    * @throw vpr::IOException
    *           Thrown if the file handle write operation failed.
    */
   vpr::Uint32 sendto(const void* msg, const vpr::Uint32 length,
                      const vpr::InetAddr& to, const vpr::Interval& timeout);

private:
   void setResult(boost::optional<boost::system::error_code>* a,
                  const boost::system::error_code b, const std::size_t bytes);
};

} // End of namespace


#endif  /* _VPR_SOCKET_DATAGRAM_IMPL_BOOST_H_ */
