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
#include <IO/Socket/SocketStream.h>


int
main (int argc, char* argv[]) {
    int status;
    vpr::Uint16 port = 5432;    // Default listening port

    // If we got an argument, it names a different value for the port.
    if ( argc == 2 ) {
        port = (unsigned short) atoi(argv[1]);
    }

    vpr::InetAddr local(port);
    const vpr::InetAddr& remote = vpr::InetAddr::AnyAddr;

    // Create an acceptor socket that listens on port.
    vpr::SocketStream sock(local, remote);

    // Open in server mode.
    if ( sock.openServer() ) {
        vpr::SocketStream* client_sock;
        char buffer[] = "Hello there!";
//        std::string buffer = "Hello there!";

        // Loop forever handling all clients serially.
        while ( 1 ) {
            // Wait for an incoming connection.
            client_sock = sock.accept();

            // Using the new socket, send the buffer to the client.
            client_sock->write(buffer, sizeof(buffer));
//            client_sock->write(buffer);
            delete client_sock;
        }

        status = 0;
    }
    else {
        status = 1;
    }

    return status;
}
