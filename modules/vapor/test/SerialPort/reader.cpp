/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <iostream>

#include <vpr/vpr.h>
#include <vpr/IO/Port/SerialPort.h>


int main (int argc, char* argv[])
{
   vpr::SerialPort* read_port;

   read_port = new vpr::SerialPort(argv[1]);

   read_port->setOpenReadOnly();
   read_port->setBlocking(true);
   try
   {
      read_port->open();
      char buffer[80];

      std::cout << "Port opened\n";
      memset((void*) &buffer, '\0', sizeof(buffer));

      //read_port->setUpdateAction(vpr::SerialIO::NOW);
      read_port->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
      read_port->setRead(true);
      read_port->setCanonicalInput(false);
      //read_port->flushQueue(vpr::SerialTypes::INPUT_QUEUE);
      read_port->read(buffer, sizeof(buffer));
      std::cout << "Read '" << buffer << "'" << std::endl;
   }
   catch(...)
   {
      std::cout << "Serial Port Failed" << std::endl;
   }

   return 0;
}
