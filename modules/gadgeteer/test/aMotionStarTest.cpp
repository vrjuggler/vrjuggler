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
#include <iomanip>
#include <sys/types.h>
#include <gadget/Devices/Ascension/MotionStarStandalone.h>


int
main () {
  std::string addr;
  unsigned short port;
  unsigned int birdsRequired;
  double measurement_rate;

  // Get configuration information
  std::cerr << "-------- MotionStar Test program -------\n"
            << "First we need some config info:\n"
            << "NOTE: Sample values are in ()'s\n" << std::flush;

  std::cout << "\nEnter address: " << std::flush;
  std::cin >> addr;
  std::cout << "Enter port number (5000, 6000): " << std::flush;
  std::cin >> port;
  std::cout << "How many birds: " << std::flush;
  std::cin >> birdsRequired;
  std::cout << "Measurement rate: " << std::flush;
  std::cin >> measurement_rate;
  std::cout << "\nUsing FRONT_HEMISPHERE" << std::flush;
  std::cout << "\nRequesting POSITION_ANGLES data" << std::flush;
  std::cout << "\nReading continuous data" << std::flush;

  MotionStarStandalone* motionstar =
     new MotionStarStandalone(addr.c_str(), port, BIRDNET::TCP, true,
                              FLOCK::FRONT_HEMISPHERE, FLOCK::POSITION_ANGLES,
                              BIRDNET::CONTINUOUS, 1, measurement_rate,
                              birdsRequired);

   char achar;
   bool birds_running(false);      // Are the birds currently running

   do
   {
      std::cerr << "\n----- MotionStar Test Program ----\n"
                << "----- Main Menu ----\n"
                << "Cur State:\n"
                << "   running:" << birds_running
                << "\n\nCommands:\n"
                << "   U - Update\n"
                << "   S - Start\n"
                << "   X - Stop\n"
                << "   Q - Quit\n"
                << "   O - Output\n"
                << "\nCommand: " << std::flush;
      std::cin >> achar;        // Get next command

      std::cerr << "\nGot command: " << achar << std::endl << std::flush;
      switch (achar)
      {
      case 'u':
      case 'U':
         std::cerr << "---- Updating (not implemented) ---\n" << std::flush;
         break;
      case 's':
      case 'S':
         std::cerr << "---- Starting MotionStar ---\n" << std::flush;
         motionstar->start();
         birds_running = true;
         break;
      case 'x':
      case 'X':
         std::cerr << "---- Stoping motionstar ---\n" << std::flush;
         motionstar->stop();
         birds_running = false;
         break;
      case 'o':
      case 'O':
         long num_samples;
         std::cerr << "Number of samples? ";
         std::cin >> num_samples;

         for (long z = 0; z < num_samples; z++)
         {
            motionstar->sample();
            //std::cout << "-------- MotionStarStandalone: Sampling (" << z << " of " << num_samples << ") -------" << std::endl;
            std::cout << z << ":\n";
            std::cout << "\t      X\t\t  Y\t     Z\t\tA\t    E\t\tR\n";
            for ( int bird=0; bird < motionstar->getNumBirds(); bird++ ) {
            //   Vec3 pos0 = Coord( *motionstar->getPosData(bird) ).pos;
            //   Vec3 ori0 = Coord( *motionstar->getPosData(bird) ).orient;
               /*
               std::cout << "Bird " << bird << ":\n"
                 << setiosflags(ios::right | ios::fixed | ios::showpoint)
                 << "Data: x:" << setprecision(2) << setw(10) << motionstar->getXPos(bird) << std::endl
                 << "      y:" << setprecision(2) << setw(10) << motionstar->getYPos(bird) << std::endl
                 << "      z:" << setprecision(2) << setw(10) << motionstar->getZPos(bird) << std::endl
                 << "    azi:" << setprecision(2) << setw(10) << motionstar->getXRot(bird) << std::endl
                 << "   elev:" << setprecision(2) << setw(10) << motionstar->getYRot(bird) << std::endl
                 << "   roll:" << setprecision(2) << setw(10) << motionstar->getZRot(bird) << std::endl << std::endl;
                 */
                  std::cout << " Bird " << bird << ":";
                  std::cout << "  " << std::setprecision(8) << motionstar->getXPos(bird)
                            << "  " << std::setprecision(8) << motionstar->getYPos(bird)
                            << "  " << std::setprecision(8) << motionstar->getZPos(bird)
                            << "  " << std::setprecision(8) << motionstar->getZRot(bird)
                            << "  " << std::setprecision(8) << motionstar->getYRot(bird)
                            << "  " << std::setprecision(8) << motionstar->getXRot(bird)
                            << std::endl;
            }
            std::cout << std::endl;
            //sleep(2);
         }
         break;
      }
   } while ((achar != 'q') && (achar != 'Q'));

   std::cerr << "---- Quiting -----\n";

   if ( motionstar->isActive() ) {
      std::cerr << "Stoping MotionStar ...\n" << std::flush;

      motionstar->stop();
   }

   delete motionstar;
   return 0;
}
