/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

#include <iostream.h>
#include <iomanip.h>
#include <SharedMem/vjMemPool.h>
//#include <SharedMem/vjSharedType.h>
#include <Input/vjPosition/vjFlock.h>
#include <Math/vjCoord.h>

int main()
{
  vjMemPool* aMemPool = new vjSharedPool(1024*1024);

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

  cout << "-------- Bird Test program -------\n"
       << "First we need some config info:\n"
       << "NOTE: Sample values are in ()'s\n" << flush;

  cout << "\nEnter port (/dev/ttyd3): " << flush;
  cin >> port;
  cout << "\nEnter baud rate (38400): " << flush;
  cin >> baud;
  cout << "\nHow many birds (2): " << flush;
  cin >> numBirds;
  cout << "\nNot using calibration file." << flush;
  cout << "\nUsing LOWER_HEM" << flush;
  cout << "\nUsing AC_NARROW filter" << flush;
  cout << "\nEnter Sync (0 or 1): " << flush;
  cin >> sync;
  cout << "\nBlocking port? (0 or 1): " << flush;
  cin >> blocking;
  cout << "\nWhat report rate (Q-every cycle, R-every other, S-every 8, T-every 32): " << flush;
  cin >> report;
  cout << "\nWhat is the transmitter id (1-based numbering): " << flush;
  cin >> transmitter;


    vjFlock* flock = new(aMemPool) vjFlock
    		(   port,
		    baud,
		    sync,
		    blocking,
		    numBirds,
		    transmitter,
		    hemi,
		    filt,
		    report,
		    calfile );

  char achar;
  bool birds_running(false);      // Are the birds currently running

   do
   {
      cout  << "\n----- Bird Test Program ----\n"
            << "----- Main Menu ----\n"
            << "Cur State:\n"
            << "   running:" << birds_running
            << "\n\nCommands:\n"
            << "   U - Update\n"
            << "   S - Start\n"
            << "   X - Stop\n"
            << "   Q - Quit\n"
            << "   O - Output\n"
            << "\nCommand:" << flush;
      cin >> achar;        // Get next command

      cout << "\nGot command: " << achar << endl << flush;
      switch (achar)
      {
      case 'u':
      case 'U':
         cout << "---- Updating (not implemented) ---\n" << flush;
         break;
      case 's':
      case 'S':
         cout << "---- Starting Flock ---\n" << flush;
         flock->startSampling();
         birds_running = true;
         break;
      case 'x':
      case 'X':
         cout << "---- Stoping flock ---\n" << flush;
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
            cout << "-------- vjFlock: Sampling (" << z << " of " << num_samples << ") -------" << endl;
            for(int bird=0;bird<numBirds;bird++)
            {
               vjVec3 pos0 = vjCoord( *flock->getPosData(bird) ).pos;
               vjVec3 ori0 = vjCoord( *flock->getPosData(bird) ).orient;
               cout << "Bird " << bird << ":\n"
                 << setiosflags(ios::right | ios::fixed | ios::showpoint)
                 << "Data: x:" << setprecision(2) << setw(10) << pos0[0] << endl
                 << "      y:" << setprecision(2) << setw(10) << pos0[1] << endl
                 << "      z:" << setprecision(2) << setw(10) << pos0[2] << endl
                 << "    azi:" << setprecision(2) << setw(10) << ori0[0] << endl
                 << "   elev:" << setprecision(2) << setw(10) << ori0[1] << endl
                 << "   roll:" << setprecision(2) << setw(10) << ori0[2] << endl << endl;
            }
            sleep(2);
         }
         break;
      }
   } while ((achar != 'q') && (achar != 'Q'));

   cout << "---- Quiting -----\n"
        << "Stoping birds.....\n" << flush;

  flock->stopSampling();

  delete flock;
  delete aMemPool;
  return 0;
}
