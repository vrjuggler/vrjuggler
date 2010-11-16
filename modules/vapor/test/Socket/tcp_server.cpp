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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <stdlib.h>

#include <vpr/vpr.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/Util/Interval.h>


int main (int argc, char* argv[])
{
   int app_status;
   vpr::InetAddr local;
   vpr::Uint16 port(15432);     // Default listening port

   // If we got an argument, it names a different value for the port.
   if ( argc == 2 )
   {
      port = (unsigned short) atoi(argv[1]);
   }

   // Create an acceptor socket that listens on port.
   local.setPort(port);
   vpr::SocketStream sock(local, vpr::InetAddr::AnyAddr);

   try
   {
      // Open in server mode.
      sock.openServer();
      sock.setReuseAddr(true);

      char buffer[] = "Hello there!";
//      std::string buffer = "Hello there!";

      // Loop forever handling all clients serially.
      while ( true )
      {
         // Wait for an incoming connection.
         try
         {
            vpr::SocketStream client_sock;
            sock.accept(client_sock, vpr::Interval(60, vpr::Interval::Sec));

            // Using the new socket, send the buffer to the client and close
            // the socket.
            client_sock.write(buffer, sizeof(buffer));
//            client_sock.write(buffer, buffer.length());
            client_sock.close();
         }
         catch (vpr::TimeoutException&)
         {
            std::cerr << "No connections within timeout period!\n";
            break;
         }
         catch (vpr::SocketException& ex)
         {
            std::cerr << "Caught a socket exception:\n" << ex.what()
                      << std::endl;
         }
      }

      app_status = EXIT_SUCCESS;
   }
   catch (vpr::IOException& ex)
   {
      std::cerr << "Caught an I/O exception:\n" << ex.what() << std::endl;
      app_status = EXIT_FAILURE;
   }

   return app_status;
}
