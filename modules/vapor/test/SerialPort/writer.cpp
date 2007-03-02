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
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
   int memleak = 38400;

   write_port = new vpr::SerialPort(argv[1]);

   write_port->setOpenWriteOnly();
   write_port->setBlocking(true);

   for ( int i = 1; i <= 4; i++ )
   {
      memleak = 38400;
      try
      {
         write_port->open();
         char buffer[] = "This is a test...";
         vpr::Uint16 size;

         std::cout << "Port opened\n";

         write_port->getMinInputSize(size);
         std::cout << "Min buffer size: " << size << std::endl;
         

         write_port->setCanonicalInput(false);
         write_port->setUpdateAction(vpr::SerialTypes::NOW);
         write_port->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
         std::cout << "Pre Write memleak :" << memleak << std::endl;
         const vpr::Uint32 bytes =
            write_port->write(buffer, sizeof(char) * (strlen(buffer) + 1));
         std::cout << std::flush;
         std::cout << "Post Write memleak :" << memleak << std::endl;
         write_port->flushQueue(vpr::SerialTypes::IO_QUEUES);
         std::cout << "Wrote " << bytes << " bytes to " << argv[1] << std::endl;

         if(38400 != memleak)
         {
            std::cout << "THERE IS A MEMORY LEAK" << std::endl;
         }
         if(38400 == memleak)
         {
            std::cout << "Strange" << std::endl;
         }
      }
      catch(...)
      {
         std::cout << "Serial Port Failed" << std::endl;
      }
      

      write_port->close();
   }

   return 0;
}
