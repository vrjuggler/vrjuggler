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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
#include <stdlib.h>

#include <vpr/vpr.h>
#include <vpr/IO/Socket/SocketDatagram.h>


int main (int argc, char* argv[])
{
   if ( argc != 3 )
   {
      std::cerr << "Usage: " << argv[0] << " <address> <port>" << std::endl;
      return EXIT_FAILURE;
   }

   // Create a socket that is sending to a remote host named in the first
   // argument listening on the port named in the second argument.
   vpr::InetAddr remote_addr;
   remote_addr.setAddress(argv[1], atoi(argv[2]));
   vpr::SocketDatagram sock(vpr::InetAddr::AnyAddr, remote_addr);

   try
   {
      sock.open();

      try
      {
         // We only send to one host, so call connect().
         sock.connect();

         char buffer[40];
         vpr::Uint32 bytes;

         memset(buffer, '\0', sizeof(buffer));
         strcpy(buffer, "Hi, I'm a client");

         // Write to the server.
         sock.write(buffer, 40, bytes);

         // Read from the server.
         sock.read(buffer, 40, bytes);

         // If the server reasponded, print the result.
         std::cout << "Read " << bytes << " from server\n"
                   << "    Got '" << buffer << "'" << std::endl;
      }
      catch (vpr::SocketException& ex)
      {
         std::cerr << "Caught a socket exception:\n" << ex.what()
                   << std::endl;
      }

      sock.close();
   }
   catch (vpr::IOException& ex)
   {
      std::cerr << "Caught an I/O exception:\n" << ex.what() << std::endl;
   }

   return EXIT_SUCCESS;
}
