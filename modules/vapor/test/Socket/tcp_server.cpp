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

#include <vpr/vpr.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Interval.h>


int
main (int argc, char* argv[]) {
    int app_status;
    vpr::InetAddr local;
    vpr::Uint16 port = 15432;    // Default listening port

    // If we got an argument, it names a different value for the port.
    if ( argc == 2 ) {
        port = (unsigned short) atoi(argv[1]);
    }

    // Create an acceptor socket that listens on port.
    local.setPort(port);
    vpr::SocketStream sock(local, vpr::InetAddr::AnyAddr);

    // Open in server mode.
    if ( sock.openServer().success() ) {
        vpr::SocketStream client_sock;
        vpr::ReturnStatus status;
        vpr::Uint32 bytes;
        char buffer[] = "Hello there!";
//        std::string buffer = "Hello there!";

        status = sock.setReuseAddr(true);

        // Loop forever handling all clients serially.
        while ( 1 ) {
            // Wait for an incoming connection.
            status = sock.accept(client_sock,
                                 vpr::Interval(60, vpr::Interval::Sec));

            if ( status.success() ) {
                // Using the new socket, send the buffer to the client and close
                // the socket.
                status = client_sock.write(buffer, sizeof(buffer), bytes);
//                status = client_sock.write(buffer, buffer.length(), bytes);
                client_sock.close();
            }
            else if ( status == vpr::ReturnStatus::Timeout ) {
                vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                    << "No connections within timeout period!\n"
                    << vprDEBUG_FLUSH;
                break;
            }
        } 

        app_status = 0;
    }
    else {
        app_status = 1;
    }

    return app_status;
}
