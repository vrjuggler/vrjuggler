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

#include <Utils/Socket/vjSocketDatagram.h>
#include <Utils/Socket/vjSocketDatagramFactory.h>


// ----------------------------------------------------------------------------
// Default constructor.
// ----------------------------------------------------------------------------
vjSocketDatagram::vjSocketDatagram (void) {
    vjSocketDatagramFactory fac;
    m_socket_imp = m_socket_dgram_imp = fac.create();
}

// ----------------------------------------------------------------------------
// Constructor.  This takes a port number and stores the value for later use
// in the member variables of the object.  The address defaults to
// "INADDR_ANY" which is interpreted properly when the socket is created.
// ----------------------------------------------------------------------------
vjSocketDatagram::vjSocketDatagram (const unsigned short port) {
    vjSocketDatagramFactory fac;

    m_socket_dgram_imp = fac.create(m_name, port, vjSocketTypes::INET);
    m_socket_imp       = m_socket_dgram_imp;
}

// ----------------------------------------------------------------------------
// Constructor.  This takes the address (either hostname or IP address) of a
// remote site and a port and stores the values for later use in the member
// variables of the object.
// ----------------------------------------------------------------------------
vjSocketDatagram::vjSocketDatagram (const std::string& address,
                                    const unsigned short port,
                                    const vjSocketTypes::Domain domain)
    : vjSocket(address)
{
    vjSocketDatagramFactory fac;

    m_socket_dgram_imp = fac.create(address, port, domain);
    m_socket_imp       = m_socket_dgram_imp;
}

// ----------------------------------------------------------------------------
// Constructor.  This takes an address of the form <host addr>:<port number>
// and creates a socket.
// ----------------------------------------------------------------------------
vjSocketDatagram::vjSocketDatagram (const std::string& address) {
    std::string::size_type pos;
    std::string host_addr, host_port;
    unsigned short port;
    vjSocketDatagramFactory fac;

    pos       = address.find(":");
    host_addr = address.substr(0, pos);
    host_port = address.substr(pos + 1);
    port      = atoi(host_port.c_str());

    m_socket_dgram_imp = fac.create(host_addr, port, vjSocketTypes::INET);
    m_socket_imp       = m_socket_dgram_imp;
}

// ----------------------------------------------------------------------------
// Constructor.  This takes a reference to a vjInetAddr object and creates a
// socket using its address value and port number.
// ----------------------------------------------------------------------------
vjSocketDatagram::vjSocketDatagram (const vjInetAddr& addr) {
    vjSocketDatagramFactory fac;

    m_socket_dgram_imp = fac.create(addr.getAddressString(), addr.getPort(),
                                    addr.getFamily());
    m_socket_imp       = m_socket_dgram_imp;
}
