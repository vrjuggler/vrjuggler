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
#include <IO/Socket/SocketDatagram.h>
#include <IO/Socket/InetAddr.h>


int
main (int argc, char* argv[]) {
    unsigned short port = 5432;

    if ( argc == 2 ) {
        port = (unsigned short) atoi(argv[1]);
    }

    vpr::SocketDatagram sock(port);
    int status;

    if ( sock.open() && sock.bind() ) {
        char recv_buf[32];
        char send_buf[] = "Hello there!";
        ssize_t bytes;

        while ( 1 ) {
            vpr::InetAddr addr;

            bytes = sock.recvfrom(recv_buf, sizeof(recv_buf), 0, addr);

            if ( bytes != -1 ) {
                std::cout << "Read '" << recv_buf << "' (" << bytes
                          << " bytes) from " << addr.getAddressString()
                          << std::endl;

                sock.sendto(send_buf, sizeof(send_buf), 0, addr);
            }
        }

        status = 0;
    }
    else {
        status = 1;
    }

    return status;
}
