/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <iostream>
#include <iomanip>
#include <vrj/Input/Devices/Ascension/Flock.h>
#include <vrj/Math/Coord.h>
#include <vpr/System.h>

int main()
{
  // Get configuration information
  char port[100] = "/dev/ttyd3";
  int baud = 38400;
  int numBirds = 2;
  char* calfile = "";
  BIRD_HEMI hemi = LOWER_HEM;
  BIRD_FILT filt = AC_NARROW;
  int sync = 1;
  int blocking = 0;
  char report = 'R';
  int transmitter = 3;

  std::cout << "-------- Bird Test program -------\n"
            << "First we need some config info:\n"
            << "NOTE: Sample values are in ()'s\n" << std::flush;

  std::cout << "\nEnter port (/dev/ttyd3): " << std::flush;
  std::cin >> port;
  std::cout << "\nEnter baud rate (38400): " << std::flush;
  std::cin >> baud;
  std::cout << "\nHow many birds (2): " << std::flush;
  std::cin >> numBirds;
  std::cout << "\nNot using calibration file." << std::flush;
  std::cout << "\nUsing LOWER_HEM" << std::flush;
  std::cout << "\nUsing AC_NARROW filter" << std::flush;
  std::cout << "\nEnter Sync (0 or 1): " << std::flush;
  std::cin >> sync;
  std::cout << "\nBlocking port? (0 or 1): " << std::flush;
  std::cin >> blocking;
  std::cout << "\nWhat report rate (Q-every cycle, R-every other, S-every 8, T-every 32): " << std::flush;
  std::cin >> report;
  std::cout << "\nWhat is the transmitter id (1-based numbering): "
            << std::flush;
  std::cin >> transmitter;


  vrj::Flock* flock = new vrj::Flock (port, baud, sync, blocking, numBirds,
		                      transmitter, hemi, filt, report,
                                      calfile);

  char achar;
  bool birds_running(false);      // Are the birds currently running

   do
   {
      std::cout  << "\n----- Bird Test Program ----\n"
                 << "----- Main Menu ----\n"
                 << "Cur State:\n"
                 << "   running:" << birds_running
                 << "\n\nCommands:\n"
                 << "   U - Update\n"
                 << "   S - Start\n"
                 << "   X - Stop\n"
                 << "   Q - Quit\n"
                 << "   O - Output\n"
                 << "\nCommand:" << std::flush;
      std::cin >> achar;        // Get next command

      std::cout << "\nGot command: " << achar << std::endl << std::flush;
      switch (achar)
      {
      case 'u':
      case 'U':
         std::cout << "---- Updating (not implemented) ---\n" << std::flush;
         break;
      case 's':
      case 'S':
         std::cout << "---- Starting Flock ---\n" << std::flush;
         flock->startSampling();
         birds_running = true;
         break;
      case 'x':
      case 'X':
         std::cout << "---- Stoping flock ---\n" << std::flush;
         flock->stopSampling();
         birds_running = false;
         break;
      case 'o':
      case 'O':
         int num_samples = 10;
         for (int z = 0; z < num_samples; z++)
         {
            flock->updateData();
            system("clear");
            std::cout << "-------- Flock: Sampling (" << z << " of "
                      << num_samples << ") -------" << std::endl;
            for(int bird=0;bird<numBirds;bird++)
            {
               vrj::Vec3 pos0 = vrj::Coord( *flock->getPosData(bird) ).pos;
               vrj::Vec3 ori0 = vrj::Coord( *flock->getPosData(bird) ).orient;
               std::cout << "Bird " << bird << ":\n"
                 << std::setiosflags(std::ios::right | std::ios::fixed | std::ios::showpoint)
                 << "Data: x:" << std::setprecision(2) << std::setw(10)
                 << pos0[0] << std::endl
                 << "      y:" << std::setprecision(2) << std::setw(10)
                 << pos0[1] << std::endl
                 << "      z:" << std::setprecision(2) << std::setw(10)
                 << pos0[2] << std::endl
                 << "    azi:" << std::setprecision(2) << std::setw(10)
                 << ori0[0] << std::endl
                 << "   elev:" << std::setprecision(2) << std::setw(10)
                 << ori0[1] << std::endl
                 << "   roll:" << std::setprecision(2) << std::setw(10)
                 << ori0[2] << std::endl << std::endl;
            }
            vpr::System::sleep(2);
         }
         break;
      }
   } while ((achar != 'q') && (achar != 'Q'));

   std::cout << "---- Quiting -----\n"
             << "Stoping birds.....\n" << std::flush;

  flock->stopSampling();

  delete flock;
  return 0;
}
