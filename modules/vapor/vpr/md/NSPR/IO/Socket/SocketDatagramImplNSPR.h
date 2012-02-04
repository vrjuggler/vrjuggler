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

#ifndef _VPR_SOCKET_DATAGRAM_IMPL_NSPR_H_
#define _VPR_SOCKET_DATAGRAM_IMPL_NSPR_H_

#include <vpr/vprConfig.h>

#include <vpr/md/NSPR/IO/Socket/SocketImplNSPR.h>


namespace vpr
{

/** \class SocketDatagramImplNSPR SocketDatagramImplNSPR.h vpr/IO/Socket/SocketDatagram.h
 *
 * NSPR implementation of the datagram socket interface.  This is used in
 * conjunction with vpr::SocketConfiguration to create the typedef
 * vpr::SocketDatagram.
 */
class VPR_API SocketDatagramImplNSPR : public SocketImplNSPR
{
public:
   typedef SocketImplNSPR Parent;

   // ========================================================================
   // vpr::SocketDatagram implementation.
   // ========================================================================

   /**
    * Default constructor.
    *
    * @post The member variables are initialized to default values.  The
    *       socket type is set to vpr::SocketTypes::DATAGRAM.
    */
   SocketDatagramImplNSPR();

   /**
    * Constructor.
    *
    * @post The member variables are initialized to default values.  The
    *       socket type is set to vpr::SocketTypes::DATAGRAM.
    *
    * @param localAddr  The local address to which this socket will be bound.
    * @param remoteAddr The remote address whith which this socket will
    *                   communicate.
    */
   SocketDatagramImplNSPR(const InetAddr& localAddr,
                          const InetAddr& remoteAddr);

   /** Copy constructor. */
   SocketDatagramImplNSPR(const SocketDatagramImplNSPR& sock);

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
};

} // End of namespace


#endif  /* _VPR_SOCKET_DATAGRAM_IMPL_NSPR_H_ */
