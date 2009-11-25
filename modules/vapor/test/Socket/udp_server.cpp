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
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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
#include <vpr/IO/Socket/InetAddr.h>


int main (int argc, char* argv[])
{
   int status;
   vpr::Uint16 port(15432);     // Default listening port

   // If a command-line argument was given, use it as the port value instead
   // of the default.
   if ( argc == 2 )
   {
      port = (unsigned short) atoi(argv[1]);
   }

   try
   {
      // Create a datagram socket that will be bound to port.
      vpr::InetAddr local;
      local.setPort(port);

      vpr::SocketDatagram sock(local, vpr::InetAddr::AnyAddr);

      // Bind the socket to the port.
      sock.open();
      sock.bind();

      char recv_buf[40];
      char send_buf[] = "Hello there!";

      // Loop forever reading messages from clients.
      while ( true )
      {
         vpr::InetAddr addr;

         try
         {
            // Read a message from a client.
            const vpr::Uint32 bytes = sock.recvfrom(recv_buf, sizeof(recv_buf),
                                                    addr);

            // If we read anything, print it and send a response.
            std::cout << "Read '" << recv_buf << "' (" << bytes
                      << " bytes) from " << addr.getAddressString()
                      << std::endl;

            sock.sendto(send_buf, sizeof(send_buf), addr);
         }
         catch (vpr::IOException& ex)
         {
            std::cerr << "Caught an I/O exception while communicating "
                      << "with client at " << addr.getAddressString()
                      << ":\n" << ex.what() << std::endl;
         }
      }

      sock.close();

      status = EXIT_SUCCESS;
   }
   catch (vpr::SocketException& ex)
   {
      std::cerr << "Caught a socket exception:\n" << ex.what() << std::endl;
      status = EXIT_FAILURE;
   }
   catch (vpr::IOException& ex)
   {
      std::cerr << "Caught an I/O exception:\n" << ex.what() << std::endl;
      status = EXIT_FAILURE;
   }

   return status;
}
