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
#include <stdlib.h>

#include <vpr/vpr.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/Util/Interval.h>


int main (int argc, char* argv[])
{
   try
   {
      vpr::InetAddr remote_addr;

      // If we got one argument, it is of the form <address>:<port>.
      if ( argc == 2 )
      {
         remote_addr.setAddress(argv[1]);
      }
      // If we got two arguments, they are <address> and <port>.
      else if ( argc == 3 )
      {
         remote_addr.setAddress(argv[1], atoi(argv[2]));
      }
      else
      {
         std::cerr << "Usage: " << argv[0] << " <address>:<port>\n"
                   << "       " << argv[0] << " <address> <port>\n";
         return EXIT_FAILURE;
      }

      try
      {
         // The socket is a stack variable, so it will be deallocated if an
         // exception gets thrown. The vpr::SocketStream destructor closes
         // the socket if it is still open.
         vpr::SocketStream sock(vpr::InetAddr::AnyAddr, remote_addr);

         sock.open();

         // Connect to the server.
         sock.connect();

         // Read from the server.
         try
         {
            char buffer[40]; 
            const vpr::Uint32 bytes =
               sock.read(buffer, 40, vpr::Interval(5, vpr::Interval::Sec));

            // If we read anything, print it.
            if ( bytes > 0 )
            {
               std::cout << "Read " << bytes << " bytes from server\n"
                         << "     Got '" << buffer << "'" << std::endl;
            }
            else
            {
               std::cout << "What the ???" << std::endl;
            }
         }
         catch (vpr::TimeoutException&)
         {
            std::cerr << "No resposne from server within timeout period!\n";
         }

         sock.close();
      }
      catch (vpr::SocketException& ex)
      {
         std::cerr << "Caught a socket exception:\n" << ex.what()
                   << std::endl;
      }
      catch (vpr::IOException& ex)
      {
         std::cerr << "Caught an I/O exception:\n" << ex.what() << std::endl;
      }
   }
   catch (vpr::UnknownHostException& ex)
   {
      std::cerr << "Failed to set server address:\n" << ex.what()
                << std::endl;
      return EXIT_FAILURE;
   }
   catch (vpr::Exception& ex)
   {
      std::cerr << "Caught an exception:\n" << ex.what() << std::endl;
   }

   return EXIT_SUCCESS;
}
