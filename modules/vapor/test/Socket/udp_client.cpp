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

#include <vpr/vpr.h>
#include <vpr/IO/Socket/SocketDatagram.h>


int main (int argc, char* argv[])
{
   vpr::InetAddr remote_addr;

   if ( argc != 3 )
   {
      fprintf(stderr, "Usage: %s <address> <port>\n", argv[0]);
   }

   // Create a socket that is sending to a remote host named in the first
   // argument listening on the port named in the second argument.
   remote_addr.setAddress(argv[1], atoi(argv[2]));
   vpr::SocketDatagram sock(vpr::InetAddr::AnyAddr, remote_addr);

   if ( sock.open().success() )
   {
      char buffer[40];

      // We only send to one host, so call connect().
      if ( sock.connect().success() )
      {
         vpr::Uint32 bytes;

         // Read from the server.
         strcpy(buffer, "Hi, I'm a client");
         sock.write(buffer, 40, bytes);

         // Read from the server.
         vpr::ReturnStatus status = sock.read(buffer, 40, bytes);

         // If the server reasponded, print the result.
         if ( status.success() )
         {
            printf("Read %d bytes from server\n", bytes);
            printf("    Got '%s'\n", buffer);
         }

         sock.close();
      }
   }
}
