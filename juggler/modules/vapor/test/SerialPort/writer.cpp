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
#include <string.h>

#include <IO/Port/SerialPort.h>
#include <IO/Port/SerialTypes.h>


int
main (int argc, char* argv[]) {
    vpr::SerialPort* write_port;

    write_port = new vpr::SerialPort(argv[1]);

    write_port->setOpenWriteOnly();
    write_port->setOpenBlocking();

    if ( write_port->open() ) {
        char* buffer = "This is a test...\n";
        ssize_t bytes;

        std::cout << "Port opened\n";
        write_port->setUpdateAction(vpr::SerialTypes::NOW);
        write_port->setCharacterSize(8);
//        write_port->enableLocalAttach();
        bytes = write_port->write(buffer, strlen(buffer) + 1);
        write_port->flushQueue(vpr::SerialTypes::IO_QUEUES);
        std::cout << "Wrote " << bytes << " bytes to " << argv[1] << std::endl;
    }

    write_port->close();

    return 0;
}
