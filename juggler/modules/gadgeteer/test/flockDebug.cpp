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


//#include <vrj/vrjConfig.h>
//#include <vrj/Draw/OGL/GlApp.h>

#include <iostream>     // for std::cout
#include <iomanip>      // for std::setw

#include <stdlib.h>     // for system
#include <unistd.h>     // for sleep
#include <signal.h>     // to trap Ctrl-C

#include <drivers/Ascension/Flock/FlockStandalone.h>

// Declarations
void printSettings();
void changeSettings();
void openBird();
void printBirdDetails();
void setRtsValue();

FlockStandalone* flock(NULL);

struct flock_config
{
   std::string       port;
   int               baud;
   int               numBirds;
   BIRD_HEMI         hemi;
   BIRD_FILT         filt;
   Flock::ReportRate report_rate;
   int               transmitter;
};

flock_config fconfig;

enum CurStatus
{
   CLOSED,
   OPEN,
   RUNNING,
   STREAMING
};
CurStatus cur_status(CLOSED);

std::string getStatusString()
{
   if (CLOSED == cur_status) return "Closed";
   else if (OPEN == cur_status) return "Open";
   else if (RUNNING == cur_status) return "Running";
   else if (STREAMING == cur_status) return "Streaming";
   else return "Unknown";
}

// Signal handler
// Gets called when Ctrl-C is pressed
void shutdown(int signo){

    if (flock->isActive())
        flock->stop();

    exit(0);
}


int main()
{
   // Flock parameters
   fconfig.port = "/dev/ttyS0";  // linux
   fconfig.baud = 38400;
   fconfig.numBirds = 1;
   //std::string calfile = "/var/tmp/nothing.txt";
   fconfig.hemi = UPPER_HEM;
   fconfig.filt = AC_NARROW;
   fconfig.report_rate = Flock::EveryOther;
   fconfig.transmitter = 3;


   int i = 1;
   char achar, rep_rate;
   int reps, format, parmnumber, databytes, parmnumb, databyte, rep;
   unsigned long timeout;

   timeout = 1000;

   // Trap Ctrl-C (SIGINT) and stop the flock gracefully
   signal(SIGINT, shutdown);

   do
   {
      std::cout
         << "\n--------------------------------------\n"
         << "Status: " << getStatusString() << std::endl
         << "\nMenu:\n"
         << "1 - Print current flock settings\n"
         << "2 - Change settings\n"
         << "3 - Open bird using current settings.\n"
         << "4 - Print bird details.\n"
         << "5 - Set RTS line value.\n"
         << "6 - Print system status.\n"
         << "S - Start (needs to be done first)\n"
         << "Q - Quit\n"
         << "O - Output (formatted, 2000 repetitions)\n"
         //<< "P - Print raw location data (continuous loop)\n"
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
      case '1':
         printSettings();
         break;

      case '2':
         changeSettings();
         break;

      case '3':
         openBird();
         break;

      case '4':
         printBirdDetails();
         break;

      case '5':
         setRtsValue();
         break;

      case '6':
         std::cout << "\n\n";
         flock->printFlockStatus();
         break;

         // Start the flock. Needs to be done first
      case 's':
      case 'S':
         flock->start();
         break;

         // Examine a value.
         // Enter the hex value of the flock parameter number you want
         // to examine (in the manual), the number of data bytes the
         // parameter will return, the number of times to repeat the
         // query, and the output format (binary or ascii).
      /*
      case 'e':
      case 'E':
         std::cout << "Enter PARAMETERnumber (in hex): ";
         std::cin >> std::hex >> parmnumber;
         std::cout << "Enter number of data bytes to receive: ";
         std::cin >> databytes;
         std::cout << "Enter number of times to repeat: ";
         std::cin >> reps;
         std::cout << "Enter 1 for ascii output or 2 for binary: ";
         std::cin >> format;
         flock->examineValue((char)parmnumber, databytes, reps, format);
         break;
         */

         // Set a value.
         // Enter the hex value of the flock parameter number you want
         // to set, the parameter data, and the number of times to repeat
         // the send.
      case 'x':
      case 'X':
         std::cout << "Enter PARAMETERnumber (in hex): ";
         std::cin >> std::hex >> parmnumb;
         std::cout << "Enter the data to send: ";
         std::cin >> std::hex >> databyte;
         std::cout << "Enter number of times to repeat: ";
         std::cin >> rep;
         flock->setValue((char)parmnumb, (char)databyte, rep);
         break;

         // Repeat the last Set Value
      case 'y':
      case 'Y':
         flock->setValue((char)parmnumb, (char)databyte, rep);
         break;

         // Repeat the last Examine Value
      /*
      case 'r':
      case 'R':
         flock->examineValue((char)parmnumber, databytes, reps, format);
         break;
      */
         // Print raw (binary) data.
         // This will print the data continuously, use Ctrl-C to stop
         /*
         case 'p':case 'P':
           flock->readData();
           break;
        */

         // Put the flock into STREAM mode
      case 'f':
      case 'F':
         flock->setStreamMode();
         break;

         // Put the flock into POINT mode.
         // All of the output functions are expecting the flock to be in
         // STREAM mode.  This is really only useful for stopping
         // a STREAMing flock.
      case 'd':
      case 'D':
         flock->setPointMode();
         break;

         // Print the flock config data.
         // Look on page ? of the manual for the meanings of the bits.
         // NOTE: sometimes the most significant byte and the least
         // significant byte get reversed
      case 'c':
      case 'C':
         flock->checkConfig();
         break;

         // Put the flock into RUN mode
      case 'g':
      case 'G':
         flock->setRunMode();
         break;

         // Put the flock into SLEEP mode
      case 'h':
      case 'H':
         flock->setSleepMode();
         break;

         // Set the output of the flock to be in position only mode.
         // NOTE: the Output menu is expecting the flock to be in pos/ang mode.
         // The print raw data function wont be affected, however.
      case 'a':
      case 'A':
         flock->setPosMode();
         break;

         // Set the output of the flock to be in position and angles
      case 'b':
      case 'B':
         flock->setPosAngMode();
         break;

         // Set the delay
         // Set the delay between successive Outputs.  Raw print data is not
         // affected by this timeout.
      case 'z':
      case 'Z':
         std::cout << "Enter timeout in microseconds (" << timeout << "): ";
         std::cin >> timeout;
         break;

         // Set the report rate
         // Q = every cycle
         // R = every other cycle (default)
         // S = every 8th cycle
         // T = every 32nd cycle
      /*case 'w':
      case 'W':
         std::cout << "Enter report rate (Q,R,S,T): ";
         std::cin >> rep_rate;
         flock->setReportRate(rep_rate);
         break;
         */

         // Have the flock read some output.
         // This uses the regulation flock.sample() function.
      case 'o':
      case 'O':
         for (int z = 0; z < 2000; z++)
         {
            flock->sample();
            //system("clear");
            std::cout << "Number of Birds in Flock: " << flock->getNumBirds() << std::endl;
            std::cout << "vjFlock------------------------------------------------------------" << std::endl;
            std::cout<< "Data: x:" << std::setw(10);
            for (i=1; i <= flock->getNumBirds(); ++i)
               std::cout<< std::setw(10) <<(int)flock->xPos(i);
            std::cout<<std::endl;
            std::cout<< "      y:" << std::setw(10);
            for (  i=1; i <= flock->getNumBirds(); ++i)
               std::cout  << std::setw(10)  <<(int)flock->yPos(i);
            std::cout<<std::endl;
            std::cout<< "      z:" << std::setw(10);
            for (  i=1; i <= flock->getNumBirds(); ++i)
               std::cout<< std::setw(10) <<(int)flock->zPos(i);
            std::cout<<std::endl;
            std::cout<< "    azi:" << std::setw(10);
            for (  i=1; i <= flock->getNumBirds(); ++i)
               std::cout<< std::setw(10) <<(int)flock->zRot(i);
            std::cout<<std::endl;
            std::cout<< "   elev:" << std::setw(10);
            for (  i=1; i <= flock->getNumBirds(); ++i)
               std::cout<< std::setw(10) <<(int)flock->yRot(i);
            std::cout<<std::endl;
            std::cout<< "   roll:"  ;
            for (  i=1; i <= flock->getNumBirds(); ++i)
               std::cout<< std::setw(10) <<(int)flock->xRot(i);
            std::cout<<std::endl<<std::endl;
            usleep(timeout);
            i++;
         }
         break;
      }

   } while ((achar != 'q') && (achar != 'Q'));

   return 0;
}

void printSettings()
{
   std::cout << "Flock settings: "
             << "         port:" << fconfig.port << std::endl
             << "         baud:" << fconfig.baud << std::endl
             << "         hemi:" << getHemiString(fconfig.hemi) << std::endl
             << "         filt:" << getFiltString(fconfig.filt) << std::endl
             << "         rate:" << fconfig.report_rate << std::endl
             << "     numbirds:" << fconfig.numBirds << std::endl
             << "  transmitter:" << fconfig.transmitter << std::endl;
}

void changeSettings()
{
   std::string in_string;
   int in_int;

   std::cout << "Change flock settings: ";

   std::cout << "port: [" << fconfig.port << "] :";
   std::cin >> fconfig.port;

   std::cout << "baud: [" << fconfig.baud << "] :";
   std::cin >> fconfig.baud;

   std::cout << "num birds: [" << fconfig.numBirds << "] :";
   std::cin >> fconfig.numBirds;

   std::cout << "transmitter: [" << fconfig.transmitter << "] :";
   std::cin >> fconfig.transmitter;

/*
             << "         baud:" << fconfig.baud << std::endl
             << "         hemi:" << getHemiString(fconfig.hemi) << std::endl
             << "         filt:" << getFiltString(fconfig.filt) << std::endl
             << "        block:" << fconfig.blocking << std::endl
             << "         rate:" << fconfig.report_rate << std::endl
             << "     numbirds:" << fconfig.numBirds << std::endl
             << "  transmitter:" << fconfig.transmitter << std::endl;
*/
}

void openBird()
{
   std::cout << "Instantiating flock...\n";

   flock = new FlockStandalone( fconfig.port,
                                fconfig.numBirds,
                                fconfig.transmitter,
                                fconfig.baud,
                                1,
                                fconfig.hemi,
                                fconfig.filt,
                                fconfig.report_rate);

   std::cout << "Opening flock...";

   vpr::ReturnStatus ret_val = flock->openPort();

   if(ret_val.failure())
   {  std::cout << "FAILED!!!\n"; }
   else
   {  std::cout << "success.\n";}

   cur_status = OPEN;
}

void printBirdDetails()
{
   // Query all the parameters
   float sw_ver;
   std::string model_id;
   AddressingMode addr_mode;
   vpr::Uint16 bird_status;

   sw_ver = flock->getSoftwareRevision();
   model_id = flock->getModelId();
   addr_mode = flock->getAddressingMode();
   bird_status = flock->queryBirdStatus();


   std::cout << "--- Flock details --- \n";
   std::cout << "      model:" << model_id << std::endl;
   std::cout << "      sw ver:" << sw_ver << std::endl;
   std::cout << "   addr mode:" << getAddressingModeString(addr_mode) << std::endl;
   std::cout << " bird status:" << bird_status << std::endl
             << "      master:" << Flock::BirdStatus::isMaster(bird_status) << std::endl;

}

void setRtsValue()
{
   unsigned new_val;
   std::cout << "Setting rts on port: " << fconfig.port << std::endl;
   std::cout << "  new value: >";
   std::cin >> new_val;

   vpr::SerialPort ser(fconfig.port);
   if (!(ser.open().success() ))
   {
      std::cerr << "ERROR: Could not open serial port.\n";
      return;
   }
   bool val(true);
   if (new_val == 0)
      val = false;

   std::cout << "Setting RTS to: " << val << " ...";
   ser.setRequestToSend(val);
   std::cout << "done.\n";

   ser.close();
}


