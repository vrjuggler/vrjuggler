/*************** <auto-copyright.pl BEGIN do not edit this line> ************** *
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

#include <stdio.h>

#include <vpr/vpr.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/Util/Interval.h>
#include <vpr/Util/Debug.h>


int
main (int argc, char* argv[]) {
    vpr::InetAddr remote_addr;
    vpr::SocketStream* sock;

    // If we got one argument, it is of the form <address>:<port>.
    if ( argc == 2 ) {
        if ( ! remote_addr.setAddress(argv[1]).success() ) {
            fprintf(stderr, "Failed to set address %s\n", argv[1]);
            return 1;
        }

        sock = new vpr::SocketStream(vpr::InetAddr::AnyAddr, remote_addr);
    }
    // If we got two arguments, they are <address> and <port>.
    else if ( argc == 3 ) {
        if ( ! remote_addr.setAddress(argv[1], atoi(argv[2])).success() ) {
            fprintf(stderr, "Failed to set address %s %s\n", argv[1], argv[2]);
            return 1;
        }

        sock = new vpr::SocketStream(vpr::InetAddr::AnyAddr, remote_addr);
    }
    else {
        fprintf(stderr, "Usage: %s <address>:<port>\n", argv[0]);
        fprintf(stderr, "       %s <address> <port>\n", argv[0]);
        return 1;
    }

    if ( sock->open().success() ) {
        char buffer[40];

        // Connect to the server.
        if ( sock->connect().success() ) {
            vpr::Status status;
            vpr::Uint32 bytes;

            // Read from teh server.
            status = sock->read(buffer, 40, bytes,
                                vpr::Interval(5, vpr::Interval::Sec));

            if ( status.success() ) {
                // If we read anything, print it.
                if ( bytes > 0 ) {
                    printf("Read %d bytes from server\n", bytes);
                    printf("    Got '%s'\n", buffer);
                }
                else if ( bytes == -1 ) {
                    printf("Error reading!\n");
                }
                else {
                    printf("What the ???\n");
                }
            }
            else if ( status == vpr::Status::Timeout ) {
                vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                    << "No resposne from server within timeout period!\n"
                    << vprDEBUG_FLUSH;
            }

            fflush(stdout);

            sock->close();
        }
    }
    else {
        printf("Socket failed to open!\n");
    }

    delete sock;

    return 0;
}
