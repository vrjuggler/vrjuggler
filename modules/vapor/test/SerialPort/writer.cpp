/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
#include <string.h>

#include <vpr/vpr.h>
#include <vpr/IO/Port/SerialPort.h>
#include <vpr/IO/Port/SerialTypes.h>


int main (int argc, char* argv[])
{
   vpr::SerialPort* write_port;

   write_port = new vpr::SerialPort(argv[1]);

   write_port->setOpenWriteOnly();
   write_port->setOpenBlocking();

   for ( int i = 1; i <= 4; i++ )
   {
      if ( write_port->open().success() )
      {
         char buffer[] = "This is a test...";
         vpr::Uint32 bytes;
         vpr::Uint16 size;

         std::cout << "Port opened\n";

         if ( write_port->getBufferSize(size).success() )
         {
            std::cout << "Min buffer size: " << size << std::endl;
         }

         write_port->disableCanonicalInput();
         write_port->setUpdateAction(vpr::SerialTypes::NOW);
         write_port->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
         write_port->write(buffer, sizeof(char) * (strlen(buffer) + 1), bytes);
         write_port->flushQueue(vpr::SerialTypes::IO_QUEUES);
         std::cout << "Wrote " << bytes << " bytes to " << argv[1] << std::endl;
      }

      write_port->close();
   }

   return 0;
}
