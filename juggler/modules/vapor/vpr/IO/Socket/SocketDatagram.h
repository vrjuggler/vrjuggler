/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SOCKET_DATAGRAM_H_
#define _VPR_SOCKET_DATAGRAM_H_

#include <vprConfig.h>

#include <IO/Socket/Socket.h>
#include <IO/Socket/SocketDatagramImp.h>
#include <IO/Socket/InetAddr.h>


namespace vpr {

// ----------------------------------------------------------------------------
//: Datagram socket interface.
// ----------------------------------------------------------------------------
//! PUBLIC_API:
template<class RealSocketDatagramImp, class RealSocketDatagramImpParent>
class SocketDatagram_t : public Socket_t<RealSocketDatagramImpParent> {
public:
    // ------------------------------------------------------------------------
    //: Default constructor.
    // ------------------------------------------------------------------------
    SocketDatagram_t (void)
        : m_socket_dgram_imp()
    {
        m_socket_imp = &m_socket_dgram_imp;
    }

    // ------------------------------------------------------------------------
    //: Constructor.  This takes a port number and stores the value for later
    //+ use in the member variables of the object.  The address defaults to
    //+ "INADDR_ANY" which is interpreted properly when the socket is created.
    //
    //! PRE: None.
    //! POST: The member variables are initialized with the m_type variable in
    //+       particular set to vpr::SocketTypes::DATAGRAM.
    //
    //! ARGS: port - The port on the remote site to which we will connect.
    // ------------------------------------------------------------------------
    SocketDatagram_t (const Uint16 port)
        : m_socket_dgram_imp("INADDR_ANY", port)
    {
        m_socket_imp = &m_socket_dgram_imp;
    }

    // ------------------------------------------------------------------------
    //: Constructor.  This takes the address (either hostname or IP address)
    //+ of a remote site and a port and stores the values for later use in the
    //+ member variables of the object.
    //
    //! PRE: None.
    //! POST: The member variables are initialized with the m_type variable in
    //+       particular set to vpr::SocketTypes::DATAGRAM.
    //
    //! ARGS: address - The hostname or IP address of the site to which we
    //+                 will connect.
    //! ARGS: port    - The port on the remote site to which we will connect.
    //! ARGS: domain  - The protocol family (domain) for this socket.
    // ------------------------------------------------------------------------
    SocketDatagram_t (const std::string& address, const Uint16 port,
                      const SocketTypes::Domain domain = SocketTypes::INET)
        : Socket_t<RealSocketDatagramImpParent>(address),
          m_socket_dgram_imp(address, port, domain)
    {
        m_socket_imp = &m_socket_dgram_imp;
    }

    // ------------------------------------------------------------------------
    //: Constructor.  This takes an address of the form
    //+ <host addr>:<port number> and creates a socket.
    //
    //! PRE: The given address is of the form <host addr>:<port number> where
    //+      <host addr> is either a hostname or a dotted-decimal IPv4
    //+      address and <port number> is a valid port number.
    //! POST: The host address and port number are extracted, and a socket is
    //+       created using those values.
    //
    //! ARGS: An address of the form <host addr>:<port number>.
    // ------------------------------------------------------------------------
    SocketDatagram_t (const std::string& address)
        : m_socket_dgram_imp(address)
    {
        std::string::size_type pos;
        std::string host_addr, host_port;
        Uint16 port;
        SocketDatagramFactory fac;

        pos       = address.find(":");
        host_addr = address.substr(0, pos);
        host_port = address.substr(pos + 1);
        port      = atoi(host_port.c_str());

//        m_socket_dgram_imp.getRemoteAddr().setAddress(host_addr);
//        m_socket_dgram_imp.getRemoteAddr().setPort(port);
        m_socket_imp = &m_socket_dgram_imp;
    }

    // ------------------------------------------------------------------------
    //: Constructor.  This takes a reference to a vpr::InetAddr object and
    //+ creates a socket using its address value and port number.
    //
    //! PRE: addr is a reference to a valid vpr::InetAddr object.
    //! POST: A socket is created using the contents of addr.
    //
    //! ARGS: addr - A reference to a vpr::InetAddr object.
    // ------------------------------------------------------------------------
    SocketDatagram_t (const InetAddr& addr)
        : m_socket_dgram_imp(addr)
    {
        m_socket_imp = &m_socket_dgram_imp;
    }

    // ------------------------------------------------------------------------
    //: Destructor.  This currently does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    virtual ~SocketDatagram_t (void) {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    recvfrom (void* msg, const size_t len, const int flags, InetAddr& from) {
        return m_socket_dgram_imp.recvfrom(msg, len, flags, from);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    recvfrom (unsigned char* msg, const size_t len, const int flags,
              InetAddr& from)
    {
        return m_socket_dgram_imp.recvfrom(msg, len, flags, from);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    recvfrom (char* msg, const size_t len, const int flags, InetAddr& from) {
        return m_socket_dgram_imp.recvfrom(msg, len, flags, from);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    recvfrom (std::vector<char>& msg, const int flags, InetAddr& from) {
        return m_socket_dgram_imp.recvfrom(msg, flags, from);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    recvfrom (std::vector<char>& msg, const size_t len, const int flags,
              InetAddr& from)
    {
        return m_socket_dgram_imp.recvfrom(msg, len, flags, from);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    sendto (const void* msg, const size_t len, const int flags,
            const InetAddr& to)
    {
        return m_socket_dgram_imp.sendto(msg, len, flags, to);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    sendto (const unsigned char* msg, const size_t len, const int flags,
            const InetAddr& to)
    {
        return m_socket_dgram_imp.sendto(msg, len, flags, to);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    sendto (const char* msg, const size_t len, const int flags,
            const InetAddr& to)
    {
        return m_socket_dgram_imp.sendto(msg, len, flags, to);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    sendto (const std::vector<char>& msg, const int flags,
            const InetAddr& to)
    {
        return m_socket_dgram_imp.sendto(msg, flags, to);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    sendto (const std::vector<char>& msg, const size_t len, const int flags,
            const InetAddr& to)
    {
        return m_socket_dgram_imp.sendto(msg, len, flags, to);
    }

protected:
    RealSocketDatagramImp m_socket_dgram_imp;  //: Platform-specific datagram
                                               //+ socket implementation object
};

}; // End of vpr namespace

#if defined(VPR_USE_NSPR)
#   include <md/NSPR/SocketDatagramImpNSPR.h>

namespace vpr {
  typedef SocketDatagram_t<SocketDatagramImpNSPR, SocketImpNSPR> SocketDatagram;
};
#elif defined(VPR_OS_Win32)
#   include <md/WIN32/SocketDatagramImpWinSock.h>

namespace vpr {
  typedef SocketDatagram_t<SocketDatagramImpWinSock, SocketImpWinSock> SocketDatagram;
};
#else
#   include <md/POSIX/SocketDatagramImpBSD.h>

namespace vpr {
  typedef SocketDatagram_t<SocketDatagramImpBSD, SocketImpBSD> SocketDatagram;
};
#endif


#endif	/* _VJ_SOCKET_DATAGRAM_H_ */
