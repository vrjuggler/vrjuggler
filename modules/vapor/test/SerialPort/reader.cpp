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
#include <vpr/IO/Port/SerialPort.h>


int
main (int argc, char* argv[]) {
    vpr::SerialPort* read_port;

    read_port = new vpr::SerialPort(argv[1]);

    read_port->setOpenReadOnly();
    read_port->setOpenBlocking();

    if ( read_port->open().success() ) {
        char buffer[80];
        vpr::Uint32 bytes;

//        read_port->setUpdateAction(vpr::SerialIO::NOW);
//        read_port->enableLocalAttach();
        read_port->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
//        read_port->flushQueue(vpr::SerialIO::INPUT_QUEUE);
        std::cout << "Port opened\n";
        read_port->read(buffer, sizeof(buffer), bytes);
        std::cout << "Read '" << buffer << "'\n";
    }

    return 0;
}
