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

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <vpr/vpr.h>
#include <vpr/IO/Port/SerialPort.h>
#include <vpr/IO/Port/SerialTypes.h>


int main (int argc, char* argv[])
{
   vpr::SerialPort* port;

   port = new vpr::SerialPort(argv[1]);

   port->setOpenReadWrite();
   port->setOpenBlocking();

   if ( port->open().success() )
   {
      char read_buffer[10], write_buffer[10];
      vpr::Uint32 bytes;
      int val;

      std::cerr << "Port opened\n";

      port->disableCanonicalInput();
      port->setUpdateAction(vpr::SerialTypes::NOW);
      port->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
      port->enableRead();

      for ( int i = 0; i < 10; i++ )
      {
         memset((void*) &read_buffer, '\0', sizeof(read_buffer));
         port->read(read_buffer, sizeof(read_buffer), bytes);
         std::cerr << "Read '" << read_buffer << "'\n";

         val = atoi(read_buffer);
         val++;

         memset((void*) &write_buffer, '\0', sizeof(write_buffer));
         sprintf(write_buffer, "%d", val);
         port->write(write_buffer, strlen(write_buffer) + 1, bytes);
         std::cerr << "Wrote '" << write_buffer << "' (" << bytes << " bytes)\n";
      }
   }

   return 0;
}
