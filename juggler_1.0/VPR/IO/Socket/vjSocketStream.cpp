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

#include <stdlib.h>

#include <VPR/IO/Socket/vjSocketStream.h>
#include <VPR/IO/Socket/vjSocketStreamFactory.h>


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
vjSocketStream::vjSocketStream (void) {
    vjSocketStreamFactory fac;
    m_socket_imp = m_socket_stream_imp = fac.create();
}

// ----------------------------------------------------------------------------
// Constructor.  This takes a port number and stores the value for later use
// in the member variables of the object.  The address defaults to
// "INADDR_ANY" which is interpreted properly when the socket is created.
// ----------------------------------------------------------------------------
vjSocketStream::vjSocketStream (const unsigned short port) {
    vjSocketStreamFactory fac;

    m_socket_stream_imp = fac.create(m_name, port, vjSocketTypes::INET);
    m_socket_imp        = m_socket_stream_imp;
}

// ----------------------------------------------------------------------------
// Constructor.  This takes the address (either hostname or IP address) of a
// remote site and a port and stores the values for later use in the member
// variables of the object.
// ----------------------------------------------------------------------------
vjSocketStream::vjSocketStream (const std::string& address,
                                const unsigned short port)
    : vjSocket(address)
{
    vjSocketStreamFactory fac;

    m_socket_stream_imp = fac.create(address, port, vjSocketTypes::INET);
    m_socket_imp        = m_socket_stream_imp;
}

// ----------------------------------------------------------------------------
// Constructor.  This takes the address (either hostname or IP address) of a
// remote site and a port and stores the values for later use in the member
// variables of the object.
// ----------------------------------------------------------------------------
vjSocketStream::vjSocketStream (const std::string& address,
                                const unsigned short port,
                                const vjSocketTypes::Domain domain)
    : vjSocket(address)
{
    vjSocketStreamFactory fac;

    m_socket_stream_imp = fac.create(address, port, domain);
    m_socket_imp        = m_socket_stream_imp;
}

// ----------------------------------------------------------------------------
// Constructor.  This takes an address of the form <host addr>:<port number>
// and creates a socket.
// ----------------------------------------------------------------------------
vjSocketStream::vjSocketStream (const std::string& address) {
    std::string::size_type pos;
    std::string host_addr, host_port;
    unsigned short port;
    vjSocketStreamFactory fac;

    pos       = address.find(":");
    host_addr = address.substr(0, pos);
    host_port = address.substr(pos + 1);
    port      = atoi(host_port.c_str());

    m_socket_stream_imp = fac.create(host_addr, port, vjSocketTypes::INET);
    m_socket_imp        = m_socket_stream_imp;
}

// ----------------------------------------------------------------------------
// Constructor.  This takes a reference to a vjInetAddr object and creates a
// socket using its address value and port number.
// ----------------------------------------------------------------------------
vjSocketStream::vjSocketStream (const vjInetAddr& addr) {
    vjSocketStreamFactory fac;

    m_socket_stream_imp = fac.create(addr.getAddressString(), addr.getPort(),
                                     addr.getFamily());
    m_socket_imp        = m_socket_stream_imp;
}

// ----------------------------------------------------------------------------
// Open a socket to be used for the server side communications.  This is
// provided to automate the repeated tasks executed when a server is set up.
// It handles opening the socket, binding the address and going into listening
// mode.
// ----------------------------------------------------------------------------
bool
vjSocketStream::openServer (const int backlog) {
    bool retval;

    // First, open the socket.
    if ( retval = open() ) {
        // If that succeeded, bind to the internal address.
        if ( retval = bind() ) {
            // Finally, if that succeeded, go into listening mode.
            retval = listen(backlog);
        }
    }

    return retval;
}
