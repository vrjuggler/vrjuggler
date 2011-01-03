/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

//#include <vrj/vrjConfig.h>
//#include <vrj/Draw/OGL/GlApp.h>

#include <iostream>     // for std::cout
#include <iomanip>      // for std::setw
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sgtty.h>

#include <stdlib.h>     // for system
#include <unistd.h>     // for sleep
#include <signal.h>     // to trap Ctrl-C

#include <gmtl/Matrix.h>
#include <gmtl/Coord.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>

#include <drivers/Ascension/Flock/FlockStandalone.h>
#include <vpr/System.h>

// Declarations
void printSettings();
void changeSettings();
void printBirdDetails();
void setRtsValue();
void printSamples();
void printContinuousSamples();

void readTest();

int kbhit();





FlockStandalone flock("/dev/ttyS0");

std::string getStatusString()
{
   std::string status_string;
   if (FlockStandalone::CLOSED == flock.getStatus()) status_string = "Closed";
   else if (FlockStandalone::OPEN == flock.getStatus()) status_string = "Open";
   else if (FlockStandalone::RUNNING == flock.getStatus()) status_string = "Running";
   else if (FlockStandalone::STREAMING == flock.getStatus()) status_string=  "Streaming";
   else status_string = "Unknown";

   std::ostringstream desc_string;
   desc_string << "Status: " << status_string << std::endl
               << "Format: " << Flock::Output::getFormatString(flock.getOutputFormat()) << std::endl
               << std::endl;
   return desc_string.str();
}

// Signal handler
// Gets called when Ctrl-C is pressed
void shutdown(int signo){

    if (flock.getStatus() != FlockStandalone::CLOSED)
    { flock.close(); }

    exit(0);
}


int main()
{
   // Flock parameters
   flock.setPort("/dev/ttyS0");  // linux
   flock.setBaudRate(38400);
   //flock.setBaudRate(115200);
   //fconfig.numBirds = 1;
   flock.setHemisphere(UPPER_HEM);
   flock.setFilterType(AC_NARROW);
   //fconfig.report_rate = Flock::EveryOther;
   //fconfig.transmitter = 3;

   char achar;

   // Trap Ctrl-C (SIGINT) and stop the flock gracefully
   signal(SIGINT, shutdown);

   do
   {
      std::cout
         << "\n--------------------------------------\n"
         << getStatusString() << std::endl
         << "Menu:\n"
         << "1 - Print current flock settings\n"
         << "2 - Change settings\n"
         << "3 - Open bird using current settings.\n"
         << "4 - Print bird details.\n"
         << "5 - Set RTS line value.\n"
         << "6 - Print system status.\n"
         << "7 - Cycle output formats.\n"
         << "8 - Configure the flock.\n"
         << "9 - Toggle streaming mode.\n"
         << "a - Print samples.\n"
         << "c - Print samples continuously.\n"
         << "x - Close the bird.\n"
         << "   -- Test modes --.\n"
         << "r - Read in 10 OK's from bird (test mode 1)\n"
         << "Q - Quit\n";
         
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
      {
         vpr::ReturnStatus ret_val = flock.open();

         if(ret_val.failure())
         {  std::cout << "FAILED!!!\n"; }
         else
         {  std::cout << "success.\n";}   
      }
      break;
      case '4':
         printBirdDetails();
         break;

      case '5':
         setRtsValue();
         break;

      case '6':
         std::cout << "\n\n";
         flock.printFlockStatus();
         break;

      case '7':
         {
            Flock::Output::Format cur_format = flock.getOutputFormat();
            if(cur_format == Flock::Output::PositionQuaternion)
            {  flock.setOutputFormat(Flock::Output::Position); }
            else
            {  flock.setOutputFormat(Flock::Output::Format(unsigned(cur_format)+1)); }
         }
         break;
      
      case '8':
      {
         vpr::ReturnStatus ret_val = flock.configure();

         if(ret_val.failure())
         {  std::cout << "FAILED!!!\n"; }
         else
         {  std::cout << "success.\n";}   
      }
      break;
      case '9':
         {
            FlockStandalone::Status stat = flock.getStatus();
            if(FlockStandalone::RUNNING == stat)
            {
               flock.startStreaming();
            }
            else if(FlockStandalone::STREAMING == stat)
            {
               flock.stopStreaming();
            }
         }
         break;
      case 'a':
         printSamples();
         break;
      case 'c':
         printContinuousSamples();
         break;
      case 'x':
         std::cout << "Closing the flock...\n";
         flock.close();
         break;
      case 'r':
         readTest();
         break;
      default:
         std::cout << "Unrecognized option: " << achar << std::endl;
         break;
      }
         
   } while ((achar != 'q') && (achar != 'Q'));

   return 0;
}

void printSettings()
{
   std::string port = flock.getPort();

   std::cout << "Flock settings: \n"
             << "         port:" << flock.getPort() << std::endl
             << "         baud:" << flock.getBaudRate() << std::endl
             << "         hemi:" << getHemiString(flock.getHemisphere()) << std::endl
             << "         filt:" << getFiltString(flock.getFilterType()) << std::endl;
             /*
             << "         rate:" << fconfig.report_rate << std::endl
             << "     numbirds:" << fconfig.numBirds << std::endl
             << "  transmitter:" << fconfig.transmitter 
             */     
}

void changeSettings()
{
   std::string in_str;
   int in_int;

   std::cout << "Change flock settings: ";

   std::cout << "port: [" << flock.getPort() << "] :";
   std::cin >> in_str;
   flock.setPort(in_str);

   std::cout << "baud: [" << flock.getBaudRate() << "] :";
   std::cin >> in_int;
   flock.setBaudRate(in_int);

   /*
   std::cout << "num birds: [" << fconfig.numBirds << "] :";
   std::cin >> fconfig.numBirds;

   std::cout << "transmitter: [" << fconfig.transmitter << "] :";
   std::cin >> fconfig.transmitter;
   */

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


void printBirdDetails()
{
   // Query all the parameters
   float sw_ver;
   std::string model_id;
   Flock::AddressingMode addr_mode;
   vpr::Uint16 bird_status;

   sw_ver = flock.getSoftwareRevision();
   model_id = flock.getModelId();
   addr_mode = flock.getAddressingMode();
   bird_status = flock.queryBirdStatus();


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
   std::cout << "Setting rts on port: " << flock.getPort() << std::endl;
   std::cout << "  new value: >";
   std::cin >> new_val;

   vpr::SerialPort ser(flock.getPort());
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

void printSamples()
{
   unsigned num_samples(5);
   std::cout << "Printing " << num_samples << " samples:" << std::endl;
   
   for(unsigned s=0;s<num_samples;++s)
   {
      flock.sample();
      gmtl::Matrix44f s0 = flock.getSensorPosition(0);
      gmtl::Coord3fXYZ coord;
      gmtl::set(coord, s0);
      gmtl::Quatf quat = gmtl::make<gmtl::Quatf>(s0);
      std::cout << s << ":  p:" << coord.mPos << "  r:" 
                     << " {" << gmtl::Math::rad2Deg(coord.mRot[0]) << ", "
                     << gmtl::Math::rad2Deg(coord.mRot[1]) << ", "
                     << gmtl::Math::rad2Deg(coord.mRot[2]) <<  "  q:" << quat << std::endl;
   }  
}

void printContinuousSamples()
{
   std::cout << "Printing samples:" << std::endl;
   
   unsigned num_samples(1000);

   //for(unsigned s=0;s<num_samples;++s)
   while(!kbhit())
   {
      flock.sample();
      gmtl::Matrix44f s0 = flock.getSensorPosition(0);
      gmtl::Coord3fXYZ coord;
      gmtl::set(coord, s0);
      std::cout << "p:" << coord.mPos << "  r:" << coord.mRot << "\n";
   }  
}


void readTest()
{
   unsigned bytes_read;
   vpr::SerialPort ser(flock.getPort());

   ser.setOpenReadWrite();

   if (!(ser.open().success() ))
   {
      std::cerr << "ERROR: Could not open serial port.\n";
      return;
   }

   // Reset the port by opening it and then closing it
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Resetting bird port:\n" << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) <<"   holding port open" << vprDEBUG_FLUSH;
   for(unsigned i=0;i<5;++i)
   {
      vpr::System::msleep(40);
      vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL) << "." << vprDEBUG_FLUSH;
   }
   vprDEBUG_CONT(vprDBG_ALL,vprDBG_CONFIG_LVL) << " ok\n" << vprDEBUG_FLUSH;

   ser.close();
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) <<"   holding port closed" << vprDEBUG_FLUSH;
   for(unsigned i=0;i<5;++i)
   {
      vpr::System::msleep(40);
      vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL) << "." << vprDEBUG_FLUSH;
   }
   vprDEBUG_CONT(vprDBG_ALL,vprDBG_CONFIG_LVL) << " ok\n" << vprDEBUG_FLUSH;
   
   if (!(ser.open().success() ))
   {
      std::cerr << "ERROR: Could not open serial port.\n";
      return;
   }
   
   ser.setUpdateAction(vpr::SerialTypes::NOW);  // Changes apply immediately
   ser.clearAll();
   ser.setBlocking(true);              // Open in blocking mode
   ser.setCanonicalInput(false);              // enable binary reading and timeouts
   ser.setTimeout(10);                       // Set to 1 inter-byte read second timeout
   ser.setRead(true);                        // Allow reading from port
   ser.setLocalAttach(true);                 // Say we are directly attached
   ser.setBreakByteIgnore(true);             // Ignore terminal breaks

   ser.setInputBaudRate(flock.getBaudRate());
   ser.setOutputBaudRate(flock.getBaudRate());

   ser.setCharacterSize(vpr::SerialTypes::CS_BITS_8);
   ser.setHardwareFlowControl(false);     // No hardware flow control
   ser.setParityGeneration(false);        // No parity checking

   /*
   ser.setInputBaudRate(flock.getBaudRate());
   ser.setOutputBaudRate(flock.getBaudRate());
   ser.setCharacterSize(vpr::SerialTypes::CS_BITS_8);
   ser.setHardwareFlowControl(false);     // No hardware flow control
   ser.setParityGeneration(false);        // No parity checking
   */

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Resetting flock (using RTS signal)." << vprDEBUG_FLUSH;
   ser.setRequestToSend(true);
   for(unsigned i=0;i<5;++i)
   {
      vpr::System::msleep(200);
      vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL) << "." << vprDEBUG_FLUSH;
   }

   ser.setRequestToSend(false);              // Set RTS low to allow the flock to start back up
   for(unsigned i=0;i<5;++i)
   {
      vpr::System::msleep(200);
      vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL) << "." << vprDEBUG_FLUSH;
   }
   vprDEBUG_CONT(vprDBG_ALL,vprDBG_CONFIG_LVL) << " done.\n" << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Port setup completed.\n" << vprDEBUG_FLUSH;

/*
   std::cout << "Setting RTS off.";
   ser.setRequestToSend(false);
   vpr::System::msleep(800);
   std::cout << "done.\n";
   */

   ser.flushQueue(vpr::SerialTypes::IO_QUEUES);
      
   for (unsigned i=0;i<10;++i)
   {
      std::cout << "Sample " << i << ": ";
      std::string in_str;
      ser.read(in_str, 4, bytes_read, vpr::Interval(2,vpr::Interval::Sec));
      std::cout << in_str << std::endl;
   }

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Closing the port.\n" << vprDEBUG_FLUSH;
   ser.close();
}


#define TTY_DESCR 0
/*===============================================================
 ROUTINE: kbhit
 PURPOSE: Use select to see if a key has been pressed.
 RETURNS: 0 If no key has been hit.
          1 If a key has been pressed.
================================================================*/
/*
int kbhit()
{
    struct timeval wait;
    fd_set readfd;
    int ret;
        
    wait.tv_sec  = 0;
    wait.tv_usec = 0;
    //readfd = 1 << TTY_DESCR;
    FD_ZERO(&readfd);
    FD_SET(TTY_DESCR,&readfd);
    ret = select(TTY_DESCR + 1, &readfd, NULL,NULL, &wait);
    return(ret);
}
*/

int kbhit()
{
   fd_set readfs;
   struct timeval timeout;

   FD_ZERO(&readfs);
   FD_SET(0, &readfs);

   timeout.tv_sec = 0;
   timeout.tv_usec = 0;

   select(1, &readfs, NULL, NULL, &timeout);

   return FD_ISSET(0, &readfs);
}

/*

 int kbhit(void)
 {
   fd_set readset;
   int keyPressed = 0, rc;
   struct timeval waitDuration;

   waitDuration.tv_sec = 0;
   waitDuration.tv_usec = 0;

   FD_ZERO(&readset);
   FD_SET(STDIN_FILENO,&readset);
   
   rc = select(STDIN_FILENO + 1,
               &readset,
               NULL,
               NULL,
               &waitDuration);

   if (rc > 0)
     if (FD_ISSET(STDIN_FILENO,&readset))
       keyPressed = 1;

   return keyPressed;
 }
 */
