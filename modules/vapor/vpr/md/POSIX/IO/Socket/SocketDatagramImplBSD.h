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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

#include <vpr/md/POSIX/IO/Socket/SocketImplBSD.h>


namespace vpr {

/**
 * Implementation class for datagram sockets using the BSD sockets interface.
 */
class SocketDatagramImplBSD : public vpr::SocketImplBSD {
public:
    typedef SocketImplBSD Parent;

    // ========================================================================
    // vpr::SocketDatagram interface.
    // ========================================================================

    /**
     * Default constructor.  This sets the socket type to
     * vpr::SocketTypes::DATAGRAM.
     */
    SocketDatagramImplBSD (void)
        : SocketImplBSD(vpr::SocketTypes::DATAGRAM)
    {
        /* Do nothing. */ ;
    }

    /**
     * Constructs a datagram socket using the given addresses as defaults for
     * communication channels.
     *
     * @post The member variables are initialized with the type in particular
     *       set to vpr::SocketTypes::DATAGRAM.
     *
     * @param local_addr  The local address for this socket.  This is used for
     *                    binding the socket.
     * @param remote_addr The remote address for this socket.  This is used to
     *                    specify a default destination for all packets.
     */
    SocketDatagramImplBSD (const InetAddr& local_addr,
                           const InetAddr& remote_addr)
        : SocketImplBSD(local_addr, remote_addr, vpr::SocketTypes::DATAGRAM)
    {
        /* Do nothing. */ ;
    }

    /**
     * Copy constructor.
     *
     * @post This socket is a copy of the given socket.
     */
    SocketDatagramImplBSD (const SocketDatagramImplBSD& sock)
        : SocketImplBSD(SocketTypes::DATAGRAM)
    {
        m_local_addr         = sock.m_local_addr;
        m_remote_addr        = sock.m_remote_addr;
        m_handle             = new FileHandleImplUNIX(sock.m_handle->getName());
        m_handle->m_fdesc    = sock.m_handle->m_fdesc;
        m_handle->m_open     = sock.m_handle->m_open;
        m_handle->m_blocking = sock.m_handle->m_blocking;
    }

    /**
     * Receives a message from the specified address.
     */
    vpr::ReturnStatus recvfrom(void* msg, const vpr::Uint32 length, const int flags,
                         vpr::InetAddr& from, vpr::Uint32& bytes_read,
                         const vpr::Interval timeout = vpr::Interval::NoTimeout);

    /**
     * Sends a message to the specified address.
     */
    vpr::ReturnStatus sendto(const void* msg, const vpr::Uint32 length,
                       const int flags, const vpr::InetAddr& to,
                       vpr::Uint32& bytes_sent,
                       const vpr::Interval timeout = vpr::Interval::NoTimeout);
};

}; // End of namespace


#endif	/* _VPR_SOCKET_DATAGRAM_IMPL_BSD_H_ */
