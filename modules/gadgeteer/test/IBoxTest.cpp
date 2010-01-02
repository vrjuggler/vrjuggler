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
#include <iomanip>
#include <sys/types.h>
#include <drivers/Immersion/IBox/IBoxStandalone.h>

int main ()
{
   std::string port("/dev/ttyS0");
   //vpr::Uint32 baud(9600);
   vpr::Uint32 baud(38400);

   // Get configuration information
   std::cerr << "-------- Immersion Interface Box Test program -------\n"
      << "First we need some config info:\n"
      << "NOTE: Sample values are in ()'s\n"
      << "Enter 0 for defaults." << std::endl;
   
   /*
   std::cout << "Enter serial port(/dev/ttyS0, /dev/ttyd0, com1): " << std::endl;
   std::cin >> port;
   std::cout << "Enter baud rate (9600): " << std::endl;
   std::cin >> baud;

   if ("0" == port)
   {
      port = "/dev/ttyS0";
   }
   if (0 == baud)
   {
      baud = 9600;
   }
   */

   IBoxStandalone* ibox = new IBoxStandalone();

   try
   {
      ibox->connect(port, baud);
   }
   catch (IBoxException& ex)
   {
      delete ibox;
      ibox = NULL;
      std::cout << ex.what() << std::endl;
      exit(-1);
   }
   
   vpr::ReturnStatus status(vpr::ReturnStatus::Succeed);
   while (status.success())
   {
      try
      {
         ibox->sendCommand(0,4,0);
         ibox->checkForPacket();

         printf("\nBtns: %2X%2X%2X%2X, ",
            ibox->getButtonData(3), ibox->getButtonData(2),
            ibox->getButtonData(1), ibox->getButtonData(0));
         /*
         if (timer_flag) printf("Time: %4X, ", ibox.timer);
         if (num_anlg) printf("A/D: ");
         if (num_anlg > 4) printf("%3X%3X%3X%3X",
            ibox->analog[7], ibox->analog[6],
            ibox->analog[5], ibox->analog[4]);
         if (num_anlg > 2) printf("%3X%3X",
            ibox.analog[3], ibox.analog[2]);
         if (num_anlg) printf("%3X%3X, ",
            ibox.analog[1], ibox.analog[0]);
         if (num_encd) printf("Enc: ");
         if (num_encd > 3) printf("%5X%5X%5X",
            ibox.encoder[5], ibox.encoder[4],
            ibox.encoder[3]);
         if (num_encd > 2) printf("%5X", ibox.encoder[2]);
         if (num_encd) printf("%5X%5X",
            ibox.encoder[1], ibox.encoder[0]);
         */
         if (ibox->getButtonData(1))
         {
            break;
         }
      }
      catch (vpr::TimeoutException& ex)
      {
         std::cout << ex.what() << std::endl;
         continue;
      }


   }
   ibox->disconnect();
   delete ibox;
   ibox = NULL;
}
