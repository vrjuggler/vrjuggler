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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#include <vrj/vrjConfig.h>
#include <vrj/Draw/OGL/GlApp.h>

#include <iostream>     // for std::cout
#include <iomanip>      // for std::setw

#include <stdlib.h>     // for system
#include <unistd.h>     // for sleep
#include <signal.h>     // to trap Ctrl-C

#include <gadget/Devices/Ascension/FlockStandalone.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Display/Projection.h>
#include <vpr/System.h>

const char* port = "/dev/ttyS0";  //gfxn0 (linux)
//const char* port = "/dev/ttyd2";        // racer (irix)
const int baud = 38400;
const int numBirds = 2;
const char* calfile = "/home/users/bmila/juggler/modules/vrjuggler/build-linux/instlinks/data/calibration.table";
const BIRD_HEMI hemi = LOWER_HEM;
const BIRD_FILT filt = AC_NARROW;
//const int sync = 1;    // previously declared in <unistd.h>
const int blocking = 0;
const char report = 'R';
const int transmitter = 3;
FlockStandalone flock( port, baud, 1, blocking, numBirds, transmitter, hemi, filt, report, calfile );

// Signal handler
// Gets called when Ctrl-C is pressed 
void shutdown(int signo){

    if (flock.isActive())
        flock.stop();

    exit(0);
}

int main()
{

  int i = 1;
  char achar, rep_rate;
  int reps, format, parmnumber, databytes, parmnumb, databyte, rep;
  unsigned long timeout;

  timeout = 1000;

    // Trap Ctrl-C (SIGINT) and stop the flock gracefully
  signal(SIGINT, shutdown);

  do
    {
      std::cout << "S - Start (needs to be done first)\n"
		<< "Q - Quit\n"
		<< "O - Output (formatted, 2000 repetitions)\n"
		<< "P - Print raw location data (continuous loop)\n"
		<< "C - Check config (print status bits)\n"
		<< "E - Examine Value\n"
		<< "R - Repeat last examine\n"
		<< "X - Set Value\n"
		<< "Y - Repeat last Set\n"
		<< "W - Set Report Rate\n"
		<< "Z - Set Output delay\n"        
		<< "D - Put the flock into POINT mode\n"
		<< "F - Put the flock into STREAM mode\n"
		<< "G - Put the flock into RUN mode\n"
		<< "H - Put the flock into SLEEP mode\n"
		<< "A - Put the flock into Position mode\n"
		<< "B - Put the flock into Pos/Ang mode\n";
                
 
      std::cin >> achar;
	
      std::cout << std::flush;
      switch (achar)
        {
	  
	  // Start the flock. Needs to be done first
	case 's':case 'S': flock.start();
	  break;

	  // Examine a value.  
	  // Enter the hex value of the flock parameter number you want 
	  // to examine (in the manual), the number of data bytes the 
	  // parameter will return, the number of times to repeat the
	  // query, and the output format (binary or ascii).
	case 'e':case 'E':
	  std::cout << "Enter PARAMETERnumber (in hex): ";
	  std::cin >> hex >> parmnumber;
	  std::cout << "Enter number of data bytes to receive: ";
	  std::cin >> databytes;
	  std::cout << "Enter number of times to repeat: ";
	  std::cin >> reps;
	  std::cout << "Enter 1 for ascii output or 2 for binary: ";
	  std::cin >> format;
	  flock.examine_value((char)parmnumber, databytes, reps, format);
	  break;    

	  // Set a value.
	  // Enter the hex value of the flock parameter number you want
	  // to set, the parameter data, and the number of times to repeat
	  // the send.
	case 'x':case 'X':
	  std::cout << "Enter PARAMETERnumber (in hex): ";
	  std::cin >> hex >> parmnumb;
	  std::cout << "Enter the data to send: ";
	  std::cin >> hex >> databyte;
	  std::cout << "Enter number of times to repeat: ";
	  std::cin >> rep;
	  flock.set_value((char)parmnumb, (char)databyte, rep);
	  break;    

	  // Repeat the last Set Value
	case 'y':case 'Y':
	  flock.set_value((char)parmnumb, (char)databyte, rep);
	  break;

	  // Repeat the last Examine Value
	case 'r':case 'R':
	  flock.examine_value((char)parmnumber, databytes, reps, format);
	  break;

	  // Print raw (binary) data.
	  // This will print the data continuously, use Ctrl-C to stop
	case 'p':case 'P':
	  flock.read_data();
	  break;

	  // Put the flock into STREAM mode
	case 'f':case 'F':
	  flock.set_stream_mode();
	  break;

	  // Put the flock into POINT mode.
	  // All of the output functions are expecting the flock to be in
	  // STREAM mode.  This is really only useful for stopping
	  // a STREAMing flock.
	case 'd':case 'D':
	  flock.set_point_mode();
	  break;

	  // Print the flock config data.
	  // Look on page ? of the manual for the meanings of the bits.
	  // NOTE: sometimes the most significant byte and the least 
	  // significant byte get reversed
	case 'c':case 'C':
	  flock.check_config();
	  break;

	  // Put the flock into RUN mode
	case 'g':case 'G':
	  flock.set_run_mode();
	  break;

	  // Put the flock into SLEEP mode
	case 'h':case 'H':
	  flock.set_sleep_mode();
	  break;

	  // Set the output of the flock to be in position only mode.
	  // NOTE: the Output menu is expecting the flock to be in pos/ang mode.
	  // The print raw data function wont be affected, however.
	case 'a':case 'A':
	  flock.set_pos_mode();
	  break;

	  // Set the output of the flock to be in position and angles
	case 'b':case 'B':
	  flock.set_pos_ang_mode();
	  break;

	  // Set the delay
	  // Set the delay between successive Outputs.  Raw print data is not
	  // affected by this timeout.
	case 'z':case 'Z':
	  std::cout << "Enter timeout in microseconds (" << timeout << "): ";
	  std::cin >> timeout;
	  break;

	  // Set the report rate
	  // Q = every cycle
	  // R = every other cycle (default)
	  // S = every 8th cycle
	  // T = every 32nd cycle
	case 'w':case 'W':
	  std::cout << "Enter report rate (Q,R,S,T): ";
	  std::cin >> rep_rate;
	  flock.set_report_rate(rep_rate);
	  break;

	  // Have the flock read some output.
	  // This uses the regulation flock.sample() function.  
	case 'o':case 'O':
	  for(int z = 0; z < 2000; z++)
	    {
	      flock.sample();
	      //system("clear"); 
	      std::cout << "Number of Birds in Flock: " << flock.getNumBirds() << std::endl;
	      std::cout << "vjFlock------------------------------------------------------------" << std::endl;
	      std::cout<< "Data: x:" << std::setw(10);
	      for(i=1; i <= flock.getNumBirds(); ++i)
		std::cout<< std::setw(10) <<(int)flock.xPos(i);
	      std::cout<<std::endl;
	      std::cout<< "      y:" << std::setw(10);
	      for(  i=1; i <= flock.getNumBirds(); ++i)
		std::cout  << std::setw(10)  <<(int)flock.yPos(i);
	      std::cout<<std::endl;
	      std::cout<< "      z:" << std::setw(10);
	      for(  i=1; i <= flock.getNumBirds(); ++i)
		std::cout<< std::setw(10) <<(int)flock.zPos(i);
	      std::cout<<std::endl;
	      std::cout<< "    azi:" << std::setw(10);
	      for(  i=1; i <= flock.getNumBirds(); ++i)
		std::cout<< std::setw(10) <<(int)flock.zRot(i);
	      std::cout<<std::endl;
	      std::cout<< "   elev:" << std::setw(10);
	      for(  i=1; i <= flock.getNumBirds(); ++i)
		std::cout<< std::setw(10) <<(int)flock.yRot(i);
	      std::cout<<std::endl;
	      std::cout<< "   roll:"  ;
	      for(  i=1; i <= flock.getNumBirds(); ++i)
		std::cout<< std::setw(10) <<(int)flock.xRot(i);
	      std::cout<<std::endl<<std::endl;
	      usleep(timeout);
	      i++;
            }
	  break;
        }

    } while ((achar != 'q') && (achar != 'Q'));

  return 0;
}

