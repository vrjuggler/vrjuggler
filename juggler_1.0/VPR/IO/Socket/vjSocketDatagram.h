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

#ifndef _VJ_SOCKET_DATAGRAM_H_
#define _VJ_SOCKET_DATAGRAM_H_

#include <vjConfig.h>

#include <VPR/IO/Socket/vjSocket.h>
#include <VPR/IO/Socket/vjSocketDatagramImp.h>
#include <VPR/IO/Socket/vjInetAddr.h>


// ----------------------------------------------------------------------------
//: Datagram socket interface.
// ----------------------------------------------------------------------------
//! PUBLIC_API:
class VJ_CLASS_API vjSocketDatagram : public vjSocket {
public:
    // ------------------------------------------------------------------------
    //: Default constructor.
    // ------------------------------------------------------------------------
    vjSocketDatagram(void);

    // ------------------------------------------------------------------------
    //: Constructor.  This takes a port number and stores the value for later
    //+ use in the member variables of the object.  The address defaults to
    //+ "INADDR_ANY" which is interpreted properly when the socket is created.
    //
    //! PRE: None.
    //! POST: The member variables are initialized with the m_type variable in
    //+       particular set to vjSocketTypes::DATAGRAM.
    //
    //! ARGS: port - The port on the remote site to which we will connect.
    // ------------------------------------------------------------------------
    vjSocketDatagram(const unsigned short port);

    // ------------------------------------------------------------------------
    //: Constructor.  This takes the address (either hostname or IP address)
    //+ of a remote site and a port and stores the values for later use in the
    //+ member variables of the object.
    //
    //! PRE: None.
    //! POST: The member variables are initialized with the m_type variable in
    //+       particular set to vjSocketTypes::DATAGRAM.
    //
    //! ARGS: address - The hostname or IP address of the site to which we
    //+                 will connect.
    //! ARGS: port    - The port on the remote site to which we will connect.
    //! ARGS: domain  - The protocol family (domain) for this socket.
    // ------------------------------------------------------------------------
    vjSocketDatagram (const std::string& address, const unsigned short port,
                      const vjSocketTypes::Domain domain = vjSocketTypes::INET);

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
    vjSocketDatagram(const std::string& address);

    // ------------------------------------------------------------------------
    //: Constructor.  This takes a reference to a vjInetAddr object and
    //+ creates a socket using its address value and port number.
    //
    //! PRE: addr is a reference to a valid vjInetAddr object.
    //! POST: A socket is created using the contents of addr.
    //
    //! ARGS: addr - A reference to a vjInetAddr object.
    // ------------------------------------------------------------------------
    vjSocketDatagram(const vjInetAddr& addr);

    // ------------------------------------------------------------------------
    //: Destructor.  This currently does nothing.
    //
    //! PRE: None.
    //! POST: None.
    // ------------------------------------------------------------------------
    virtual ~vjSocketDatagram (void) {
        /* Do nothing. */ ;
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    recvfrom (void* msg, const size_t len, const int flags, vjInetAddr& from) {
        return m_socket_dgram_imp->recvfrom(msg, len, flags, from);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    recvfrom (unsigned char* msg, const size_t len, const int flags,
              vjInetAddr& from)
    {
        return m_socket_dgram_imp->recvfrom(msg, len, flags, from);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    recvfrom (char* msg, const size_t len, const int flags, vjInetAddr& from) {
        return m_socket_dgram_imp->recvfrom(msg, len, flags, from);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    recvfrom (std::vector<char>& msg, const int flags, vjInetAddr& from) {
        return m_socket_dgram_imp->recvfrom(msg, flags, from);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    recvfrom (std::vector<char>& msg, const size_t len, const int flags,
              vjInetAddr& from)
    {
        return m_socket_dgram_imp->recvfrom(msg, len, flags, from);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    sendto (const void* msg, const size_t len, const int flags,
            const vjInetAddr& to)
    {
        return m_socket_dgram_imp->sendto(msg, len, flags, to);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    sendto (const unsigned char* msg, const size_t len, const int flags,
            const vjInetAddr& to)
    {
        return m_socket_dgram_imp->sendto(msg, len, flags, to);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    sendto (const char* msg, const size_t len, const int flags,
            const vjInetAddr& to)
    {
        return m_socket_dgram_imp->sendto(msg, len, flags, to);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    sendto (const std::vector<char>& msg, const int flags,
            const vjInetAddr& to)
    {
        return m_socket_dgram_imp->sendto(msg, flags, to);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    inline virtual ssize_t
    sendto (const std::vector<char>& msg, const size_t len, const int flags,
            const vjInetAddr& to)
    {
        return m_socket_dgram_imp->sendto(msg, len, flags, to);
    }

protected:
    vjSocketDatagramImp* m_socket_dgram_imp;  //: Platform-specific datagram
                                              //+ socket implementation object
};


#endif	/* _VJ_SOCKET_DATAGRAM_H_ */
