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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _VPR_SOCKET_DATAGRAM_IMPL_BSD_H_
#define _VPR_SOCKET_DATAGRAM_IMPL_BSD_H_

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/IO/Socket/SocketException.h>
#include <vpr/md/POSIX/IO/Socket/SocketImplBSD.h>


namespace vpr
{

/** \class SocketDatagramImplBSD SocketDatagramImplBSD.h vpr/IO/Socket/SocketDatagram.h
 *
 * Implementation class for datagram sockets using the BSD sockets interface.
 * This is used in conjunction with vpr::SocketConfiguration to create the
 * BSD implementation of the typedef vpr::SocketDatagram.
 */
class VPR_CLASS_API SocketDatagramImplBSD : public vpr::SocketImplBSD
{
public:
   typedef SocketImplBSD Parent;

   // ========================================================================
   // vpr::SocketDatagram interface.
   // ========================================================================

   /**
    * Default constructor.  This sets the socket type to
    * vpr::SocketTypes::DATAGRAM.
    */
   SocketDatagramImplBSD();

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
   SocketDatagramImplBSD(const InetAddr& localAddr, const InetAddr& remoteAddr);

   /**
    * Copy constructor.
    *
    * @post This socket is a copy of the given socket.
    */
   SocketDatagramImplBSD(const SocketDatagramImplBSD& sock);

   /**
    * Receives a message from the specified address.
    * 
    * @throws vpr::SocketException if the socket is not connected.
    * @throws vpr::WouldBlockException if the file is in non-blocking mode,
    *         and there is no data to read.
    * @throws vpr::TimeoutException if the read could not begin within the
    *         timeout interval.
    * @throws vpr::IOException if the read operation failed.
    */
   void recvfrom(void* msg, const vpr::Uint32 length,
                 vpr::InetAddr& from, vpr::Uint32& bytesRead,
                 const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException);

   /**
    * Sends a message to the specified address.
    */
   void sendto(const void* msg, const vpr::Uint32 length,
               const vpr::InetAddr& to, vpr::Uint32& bytesSent,
               const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException);
};

} // End of namespace


#endif  /* _VPR_SOCKET_DATAGRAM_IMPL_BSD_H_ */
