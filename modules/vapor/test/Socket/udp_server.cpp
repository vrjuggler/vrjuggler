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

#include <iostream>

#include <vpr/vpr.h>
#include <vpr/IO/Socket/SocketDatagram.h>
#include <vpr/IO/Socket/InetAddr.h>


int
main (int argc, char* argv[]) {
    int status;
    vpr::InetAddr local;
    vpr::Uint16 port = 15432;   // Default listening port

    // If a command-line argument was given, use it as the port value instead
    // of the default.
    if ( argc == 2 ) {
        port = (unsigned short) atoi(argv[1]);
    }

    // Create a datagram socket that will be bound to port.
    local.setPort(port);
    vpr::SocketDatagram sock(local, vpr::InetAddr::AnyAddr);

    // Bind the socket to the port.
    if ( sock.open().success() && sock.bind().success() ) {
        char recv_buf[32];
        char send_buf[] = "Hello there!";
        vpr::Uint32 bytes;
        vpr::ReturnStatus sock_stat;

        // Loop forever reading messages from clients.
        while ( 1 ) {
            vpr::InetAddr addr;

            // Read a message from a client.
            sock_stat = sock.recvfrom(recv_buf, sizeof(recv_buf), 0, addr,
                                      bytes);

            // If we read anything, print it and send a response.
            if ( sock_stat.success() ) {
                std::cout << "Read '" << recv_buf << "' (" << bytes
                          << " bytes) from " << addr.getAddressString()
                          << std::endl;

                sock.sendto(send_buf, sizeof(send_buf), 0, addr, bytes);
            }
        }

        status = 0;
    }
    else {
        status = 1;
    }

    return status;
}
