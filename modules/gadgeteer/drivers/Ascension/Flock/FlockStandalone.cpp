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

#include <iostream>        // for std::cout
#include <fstream>         // for std::ifstream
#include <string.h>

#include <vpr/vpr.h>
#include <vpr/Util/Assert.h> /* for vprASSERT */
#include <gadget/Util/Debug.h>
#include <vpr/System.h>

#include <boost/format.hpp>

#include <drivers/Ascension/Flock/FlockStandalone.h>


/** Configure constructor. */
FlockStandalone::FlockStandalone(std::string port, const int& numBrds, 
                                 const int& transmit, const int& baud,
                                 const int& sync, const BIRD_HEMI& hemi, 
                                 const BIRD_FILT& filt, Flock::ReportRate report)
   : mStatus(FlockStandalone::CLOSED),
    mPort(port),
    mSerialPort(NULL),
    mBaud(baud),
    mMode(UnknownMode),
    mAddrMode(UnknownAddressing),
    mSwRevision(0.0f),
    mNumBirds(numBrds),
    mXmitterUnitNumber(transmit),
    mReportRate(report),
    mHemisphere(hemi),
    mFilter(filt),
    mSyncStyle(sync)   
{;}

FlockStandalone::~FlockStandalone()
{
   if ( FlockStandalone::CLOSED != mStatus )
   {
      this->stop();
      mStatus = FlockStandalone::CLOSED;
   }

   if ( mSerialPort != NULL )
   {
      delete mSerialPort;
      mSerialPort = NULL;
   }
}

/** Checks if the flock is active. */
bool FlockStandalone::isActive() const
{
   return (FlockStandalone::STREAMING == mStatus);
}

/** Sets the port to use. */
void FlockStandalone::setPort(const std::string& serialPort)
{
   if ( FlockStandalone::STREAMING == mStatus )
   {
      std::cout << "Flock: Cannot change the serial Port while active\n";
      return;
   }

   if ( ! serialPort.empty() )
   {
      mPort = serialPort;

      if ( mSerialPort != NULL )
      {
         delete mSerialPort;
      }

      mSerialPort = new vpr::SerialPort(mPort);
   }
}

/** Gets the port used. */
const std::string& FlockStandalone::getPort() const
{
   return mPort;
}

/** Sets the baud rate. */
void FlockStandalone::setBaudRate(const int& baud)
{
   if ( FlockStandalone::STREAMING == mStatus )
   {
      std::cerr << "Flock: Cannot change the baud rate while active\n";
      return;
   }
   else
   {
      mBaud = baud;
   }
}


// Open the port.
//  give - a serial port
//  give - a baud rate
//  returns portId twice (created by the open function)
//  NOTE: portId is returned from both ends of this function.
//  if portId == -1 then function failed to open the port.
vpr::ReturnStatus FlockStandalone::openPort ()
{
   // Allocate the port if we need to
   if ( NULL == mSerialPort )
   {
      mSerialPort = new vpr::SerialPort(mPort);
      if (!mSerialPort)
      {  return vpr::ReturnStatus::Fail; }
   }

   // - Open and close the port to reset the tracker, then
   // - Open the port
   vprDEBUG_BEGIN(vprDBG_ALL,vprDBG_CONFIG_LVL) << "====== Opening fob serial port: " << mPort << " =====\n" << vprDEBUG_FLUSH;

   mSerialPort->setOpenReadWrite();
   mSerialPort->setOpenBlocking();              // Open in blocking mode

   if (mSerialPort->open().success() )
   {
      // Reset the port by opening it and then closing it
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Resetting bird port:\n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) <<"   holding port open" << vprDEBUG_FLUSH;
      for(unsigned i=0;i<5;++i)
      {
         vpr::System::msleep(40);
         vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL) << "." << vprDEBUG_FLUSH;
      }
      vprDEBUG_CONT(vprDBG_ALL,vprDBG_CONFIG_LVL) << " ok\n" << vprDEBUG_FLUSH;

      mSerialPort->close();
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) <<"   holding port closed" << vprDEBUG_FLUSH;
      for(unsigned i=0;i<5;++i)
      {
         vpr::System::msleep(40);
         vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL) << "." << vprDEBUG_FLUSH;
      }
      vprDEBUG_CONT(vprDBG_ALL,vprDBG_CONFIG_LVL) << " ok\n" << vprDEBUG_FLUSH;
      
      // - Open the port for the last time
      //   - Set the port attributes to use
      if ( mSerialPort->open().success() )
      {
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Port opened successfully\n" << vprDEBUG_FLUSH ;
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "  Configuring port attributes\n" << vprDEBUG_FLUSH;

         // Set the basic port attributes to use
         mSerialPort->clearAll();
         mSerialPort->setTimeout(10);                          // Set to 1 second timeout
         mSerialPort->setRead(true);
         mSerialPort->setLocalAttach(true);
         mSerialPort->setBreakByteIgnore(true);
         mSerialPort->setUpdateAction(vpr::SerialTypes::NOW);

         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "  Setting baud rate: " << mBaud << std::endl << vprDEBUG_FLUSH;
         mSerialPort->setInputBaudRate(mBaud);
         mSerialPort->setOutputBaudRate(mBaud);

         mSerialPort->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
         mSerialPort->setHardwareFlowControl(false);
         mSerialPort->setParityGeneration(false);       // No parity checking
         
         // --- Reset the flock with RTS signal --- //
         // - When RTS signal is high, bird is put into reset mode
         // So, to reset it: put signal high, wait, put signal low, wait
         // 1 second of wait seems to work best
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Resetting flock (using RTS signal)." << vprDEBUG_FLUSH;
         mSerialPort->setRequestToSend(true);
         for(unsigned i=0;i<5;++i)
         {
            vpr::System::msleep(200);
            vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL) << "." << vprDEBUG_FLUSH;
         }
         
         mSerialPort->setRequestToSend(false);              // Set RTS low to allow the flock to start back up
         for(unsigned i=0;i<5;++i)
         {
            vpr::System::msleep(200);
            vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL) << "." << vprDEBUG_FLUSH;
         }
         vprDEBUG_CONT(vprDBG_ALL,vprDBG_CONFIG_LVL) << " done.\n" << vprDEBUG_FLUSH;

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Port setup completed.\n" << vprDEBUG_FLUSH;

         mStatus = FlockStandalone::OPEN;

         //vpr::System::msleep(600*3);         // Let connection finish
         readInitialFlockConfiguration();

         printFlockStatus();

         return vpr::ReturnStatus::Succeed;
      }
      else
      {
         vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "Port open failed\n" << vprDEBUG_FLUSH;
         return vpr::ReturnStatus::Fail;
      }
   }
   else
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "Failed to open port.\n" << vprDEBUG_FLUSH;
      return vpr::ReturnStatus::Fail;
   }    
}


/** Call this to connect to the Flock device.
* - If closed, open port
* - If not streaming, start streaming
*
*/
vpr::ReturnStatus FlockStandalone::start()
{
   // If port closed right now
   if ( FlockStandalone::CLOSED == mStatus)
   {
      if ( openPort() == vpr::ReturnStatus::Fail )
      {
         vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << " [FlockStandalone] **** ERROR, can't open serial port: " <<  mPort << " ****\n" << vprDEBUG_FLUSH;
         return vpr::ReturnStatus::Fail;
      }
   }

   // If open, but not streaming yet
   if (FlockStandalone::OPEN == mStatus)
   {
      vpr::Uint32 written;
      char stop_command;

      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "============================================================================\n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << " NOTE: Version number below might be wrong if we need to restart the flock!\n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "============================================================================\n" << vprDEBUG_FLUSH;

      //this->readSoftwareRevision();

      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << " [FlockStandalone] Restarting the flock.\n" << vprDEBUG_FLUSH;

      //vpr::System::msleep(500);
      stop_command = 'B';
      mSerialPort->write(&stop_command, 1, written);

      //vpr::System::msleep(500);
      stop_command = 'G';
      mSerialPort->write(&stop_command, 1, written);

      mSerialPort->close();
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)  << " [FlockStandalone] Flock has been Stopped." << std::endl << vprDEBUG_FLUSH;
      vpr::System::sleep(3);


      if ( openPort() == vpr::ReturnStatus::Fail )
      {
         vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << " [FlockStandalone] **** ERROR, can't open serial port: " <<  mPort << " ****\n" << vprDEBUG_FLUSH;
         return vpr::ReturnStatus::Fail;
      }
      else
      {         
         //std::cout << "Initial status:\n:";
                  
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting sync\n" << vprDEBUG_FLUSH;
         sendSync();
         //vpr::System::sleep(1);

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting group\n" << vprDEBUG_FLUSH;
         sendGroup();
         //vpr::System::sleep(1);

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting hemisphere\n" << vprDEBUG_FLUSH;
         sendHemisphere();
         //vpr::System::sleep(1);

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting autoconfig\n" << vprDEBUG_FLUSH;
         sendAutoconfig();
         //vpr::System::sleep(1);

         //std::cout << "After auto-config:\n:";
         
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting transmitter\n" << vprDEBUG_FLUSH;
         sendTransmitter();
         //vpr::System::sleep(1);

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting filter\n" << vprDEBUG_FLUSH;
         sendFilter();
         //vpr::System::sleep(1);


         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting pos_angles\n" << vprDEBUG_FLUSH;
         sendPosAngles();
         //vpr::System::sleep(1);

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting pickBird\n" << vprDEBUG_FLUSH;
         pickBird(mXmitterUnitNumber);
         //vpr::System::sleep(1);

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting rep_and_stream\n" << vprDEBUG_FLUSH;
         sendRepAndStream();
         //vpr::System::sleep(1);

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Ready to go!\n\n" << vprDEBUG_FLUSH;

         // flock is active.
         mStatus = FlockStandalone::STREAMING;

         // return success
         return vpr::ReturnStatus::Succeed;
      }
   }
   else
   {
      return vpr::ReturnStatus::Succeed;
   }
}

/** Call this repeatedly to update the data from the birds. */
bool FlockStandalone::sample()
{
   // can't sample when not streaming
   vprASSERT( STREAMING == mStatus );

   int loop_count = mNumBirds;
   if (ExtendedRange == mMode)
   {
      loop_count++;
   }

   int buffer_location=0;
   for (int bird_id=1;
        bird_id <= loop_count && bird_id < MAX_SENSORS;
        bird_id++)
   {
      if (! isTransmitter(bird_id))
      {
         vprASSERT( bird_id < MAX_SENSORS );
         vpr::ReturnStatus status = getReading(bird_id, xPos(buffer_location), yPos(buffer_location), zPos(buffer_location),
                                                        zRot(buffer_location), yRot(buffer_location), xRot(buffer_location));
         if (!status.success())
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "[Flock of Birds] Error reading packet for bird #: "
                  << bird_id << "\n" << vprDEBUG_FLUSH;
         }
                  
         vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL) << std::endl << "\n===================================================\n" << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL) << " Position of Bird #: " << bird_id
                     << " x: " << xPos(buffer_location)
                     << " y: " << yPos(buffer_location)
                     << " z: " << zPos(buffer_location) << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL)   << " \nRotation of Bird #: " << bird_id
                     << " x: " << xRot(buffer_location)
                     << " y: " << yRot(buffer_location)
                     << " z: " << zRot(buffer_location) << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL) << "\n===================================================\n\n" << vprDEBUG_FLUSH;

         buffer_location++;
      }


   }
   return 1;
}

/** Stops the Flock. */
int FlockStandalone::stop ()
{
   int retval;
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char bird_command[4];

      std::cout << " [FlockStandalone] Stopping the flock ...\n" << std::flush;

      vpr::System::msleep(500);
      bird_command[0] = 'B';
      mSerialPort->write(bird_command, 1, written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

      vpr::System::msleep(500);
      bird_command[0] = 'G';
      mSerialPort->write(bird_command, 1, written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      //vpr::System::usleep(200 * FlockStandalone::mSleepFactor);

      mSerialPort->close();

      // flock is not active now.
      mStatus = FlockStandalone::CLOSED;

      std::cout << " [FlockStandalone] Stopped." << std::endl << std::flush;

      retval = 1;
   }
   else
   {
      retval = 0;
   }

   return retval;
}

/** Sets the hemisphere that the transmitter transmits from. */
void FlockStandalone::setHemisphere( const BIRD_HEMI& h )
{
   if ( mStatus == FlockStandalone::STREAMING )
   {
      std::cout << "Flock: Cannot change the hemisphere\n" << std::flush;
      return;
   }
   else
   {
      // Set it.
      mHemisphere = h;
   }
}

/** Sets the type of filtering that the Flock uses. */
void FlockStandalone::setFilterType( const BIRD_FILT& f )
{
   if (  mStatus == FlockStandalone::STREAMING )
   {
      std::cout << "Flock: Cannot change filter type while active\n"
                << std::flush;
      return;
   }
   else
   {
      // Set it.
      mFilter = f;
   }
}

/** Set the report rate that the Flock uses. */
void FlockStandalone::setReportRate(Flock::ReportRate rRate )
{
   if ( FlockStandalone::STREAMING == mStatus )
   {
      std::cout << "Flock: Cannot change report rate while active\n"
                << std::flush;
      return;
   }
   else
   {
      // Set it.
      mReportRate = rRate;
   }
}

/** Sets the unit number of the transmitter. */
void FlockStandalone::setTransmitter( const int& Transmit )
{
   if ( FlockStandalone::STREAMING == mStatus )
   {
      std::cout << "Flock: Cannot change transmitter while active\n"
                << std::flush;
      return;
   }
   else
   {
      // Set it.
      mXmitterUnitNumber = Transmit;
   }
}

/** Sets the number of birds to use in the Flock. */
void FlockStandalone::setNumBirds( const int& n )
{
   if ( FlockStandalone::STREAMING == mStatus )
   {
      std::cout << "Flock: Cannot change num birds while active\n"
                << std::flush;
      return;
   }
   else
   {
      // Set it.
      mNumBirds = n;
   }
}

/** Sets the video sync type. */
void FlockStandalone::setSync(const int& sync)
{
   if ( FlockStandalone::STREAMING == mStatus )
   {
      std::cout << "Flock: Cannot change report rate while active\n"
                << std::flush;
      return;
   }
   else
   {
      // Set it.
      mSyncStyle = sync;
   }
}

float& FlockStandalone::xPos( const int& i )
{
   vprASSERT( i < MAX_SENSORS );
   return mPosition[i][0];
}

float& FlockStandalone::yPos( const int& i )
{
   vprASSERT( i < MAX_SENSORS );
   return mPosition[i][1];
}

float& FlockStandalone::zPos( const int& i )
{
   vprASSERT( i < MAX_SENSORS );
   return mPosition[i][2];
}

float& FlockStandalone::zRot( const int& i )
{
   vprASSERT( i < MAX_SENSORS );
   return mOrientation[i][0];
}

float& FlockStandalone::yRot( const int& i )
{
   vprASSERT( i < MAX_SENSORS );
   return mOrientation[i][1];
}

float& FlockStandalone::xRot( const int& i )
{
   vprASSERT( i < MAX_SENSORS );
   return mOrientation[i][2];
}

///////////////////////////////////////////////////////////////////
// Local functions to FlockStandalone.cpp
//////////////////////////////////////////////////////////////////

/**
 * Gets a reading.
 * @param n    The bird unit number.
 * @param port The flock port number.
 */
vpr::ReturnStatus FlockStandalone::getReading (const int& n, float& xPos, float& yPos,
                                 float& zPos, float& zRot, float& yRot,
                                 float& xRot)
{
   vpr::Uint64 timeout=10000;  // How long to wait for data to arrive
   vpr::Uint32 num_read;
   char buff[14];

   if ( mSerialPort != NULL )
   {
      // buff[0-5] are position
      // buff[6-11] are orientation
      // buff[12] is group

      // Do until we get the unit ID we want
      // Find the beginning of a data packet
      // Read in the data packet

      // -loop until we get the packet for the bird we want
      //   -loop until we get the start of a data packet
      //     -read in the packet
      //     -convert the data into floats
      //   -end loop
      // -end loop

         // Wait until we get the data for the correct bird
      do
      {
            // Wait until the first bit in a byte is a 1, which means start of a packet
         do
         {
            vpr::ReturnStatus status = mSerialPort->read(&buff,1,num_read,vpr::Interval(timeout, vpr::Interval::Msec));
            if (!status.success())
            {
               return(status);
            }
         } while( !(0x80 & buff[0]));

            // Read in the rest of the data, [ (6*2) + 1 for the group byte ] - 1 byte already read
         vpr::ReturnStatus status = mSerialPort->read(&buff[1],12,num_read,vpr::Interval(timeout, vpr::Interval::Msec));
         if (!status.success())
         {
            return(status);
         }
      } while((int)buff[12] != n);
      // buff[0] LSbyte #1
      // buff[1] MSbyte #1

      // buff[2] LSbyte #2
      // buff[3] MSbyte #2

      // etc.

      // Position

      xPos = rawToFloat(buff[1], buff[0]) * POSITION_RANGE;
      yPos = rawToFloat(buff[3], buff[2]) * POSITION_RANGE;
      zPos = rawToFloat(buff[5], buff[4]) * POSITION_RANGE;

//      std::cout   << "Float X: " << xPos
//                  << "Float Y: " << yPos
//                  << "Float Z: " << zPos << std::endl;

      // Orientation
      zRot = rawToFloat(buff[7], buff[6])   * ANGLE_RANGE;
      yRot = rawToFloat(buff[9], buff[8])   * ANGLE_RANGE;
      xRot = rawToFloat(buff[11], buff[10]) * ANGLE_RANGE;

      return vpr::ReturnStatus::Succeed;
   }
   else
   {
      return vpr::ReturnStatus::Fail;
   }
}

float FlockStandalone::rawToFloat (char& MSchar, char& LSchar)
{
   // return ((float) (((r1 & 0x7f) << 9) | (r2 & 0x7f) << 2) / 0x7fff);

   // short int ival1,ival2,val;
   // ival1 = r1 & 0x7f;                  // Set 8th bit to 0
   // ival2 = r2 & 0x7f;                  // Set 8th bit to 0
   // val = (ival1 << 9) | ival2<<2;
   //return((float)val) / 0x7fff;

   vpr::Int8 MSbyte;
   vpr::Int8 LSbyte;
   vpr::Int16 returnVal;

//   std::cout << "Bits before MS: ";
//   this->showbits(MSchar);
//   std::cout << " LS:";
//   this->showbits(LSchar);
//   std::cout << std::endl;

//   if (MSchar >= 120)
//   {
//      std::cout << "ERRORBIT";
//      std::cout << std::endl;
//   }

      // 1) Changes 8th bit from a "1" back to "0" after catching it
      //    0x7f = 01111111  so it masks out the 8th but
   MSbyte = MSchar & 0x7f;
   LSbyte = LSchar & 0x7f;

//   std::cout << "After 1): ";
//   this->showbits(MSbyte);
//   std::cout << " LS:";
//   this->showbits(LSbyte);
//   std::cout << std::endl;


      // 2) Shifts each LSByte left 1 bit
   LSbyte <<= 1;           // Shift the LS byte left 2 bits becasue flock data format

//   std::cout << "After 2): ";
//   this->showbits(MSbyte);
//   std::cout << " LS:";
//   this->showbits(LSbyte);
//   std::cout << std::endl;

      // 3) Combine each MSbytee/LSbyte pair
   returnVal = MSbyte;
//      std::cout << "After 3a): ";
//      this->showbits16(returnVal);
//      std::cout << std::endl;

   returnVal <<= 8;
//      std::cout << "After 3b): ";
//      this->showbits16(returnVal);
//      std::cout << std::endl;

   returnVal += LSbyte;
//      std::cout << "After 3c): ";
//      this->showbits16(returnVal);
//      std::cout << std::endl;




      // 4) Shift each word pair left 1 more bit
   returnVal <<= 1;

//   std::cout << std::endl << "------------------------------------------" << std::endl;
//   std::cout << "[vpr::Int16] Before DIV): " << returnVal  << std::endl;
//   std::cout << "[float]      Before DIV): " << (float)returnVal  << std::endl;
//   std::cout << "[float]      After DIV): " << ( (float)returnVal ) / 0x7fff  << std::endl;
//   std::cout << "------------------------------------------" << std::endl << std::endl;


/*   if ( vpr::System::isLittleEndian() )
   {
      returnVal = MSbyte;
//      std::cout << "ReturnVal-1: " << returnVal << std::endl;
      returnVal <<= 8;        // Shit the MS byte left 8 bits to make it the MS
                              // and 1 more because of flock data format
//      std::cout << "ReturnVal-2: " << returnVal << std::endl;
      returnVal |= LSbyte;
      std::cout << "RAW DATA: " << returnVal << std::endl;
   }
   else if ( vpr::System::isBigEndian() )
   {
      vprASSERT(1==2);
//      MSbyte <<= 8;           // Shit the MS byte left 8 bits to make it the MS
                              // and 1 more because of flock data format
//      returnVal = MSbyte & LSbyte;
   }
   else
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "Endianess not specified!\n" << vprDEBUG_FLUSH;
      vprASSERT(1==2);
   }

*/
   return( ( (float)returnVal ) / 0x7fff );
}

// ---------------------------------------------------------------------------------- //
//                                QUERY METHODS                                       //
// ---------------------------------------------------------------------------------- //

/** Get the software revision on the bird */
std::pair<unsigned,unsigned> FlockStandalone::querySoftwareRevision()
{
   std::vector<vpr::Uint8> resp(2);
   getAttribute(Flock::Parameter::SoftwareRevision, 2, resp);
   std::pair<unsigned,unsigned> sw_rev;
   sw_rev.first = resp[0];
   sw_rev.second = resp[1];
   return sw_rev;
}

/** Get the model id string */
std::string FlockStandalone::queryModelIdString()
{
   std::vector<vpr::Uint8> resp(10);
   getAttribute(Flock::Parameter::ModelIdentification, 10, resp);
   std::string model_id = "";
   for(unsigned i=0;i<10;++i)
   { model_id += char(resp[i]); }

   return model_id;
}

/** Get the model id string */
AddressingMode FlockStandalone::queryAddressingMode()
{
   std::vector<vpr::Uint8> resp(1);
   getAttribute(Flock::Parameter::AddressingMode, 1, resp);
   AddressingMode amode = AddressingMode(resp[0]);
   return amode;
}

vpr::Uint8 FlockStandalone::queryAddress()
{
   std::vector<vpr::Uint8> resp(1);
   getAttribute(Flock::Parameter::FbbAddress, 1, resp);
   vpr::Uint8 addr(resp[0]);
   return addr;
}

/** Get status of the bird */
vpr::Uint16 FlockStandalone::queryBirdStatus(unsigned addr)
{
   if(0 != addr)
   {  pickBird(addr); }

   std::vector<vpr::Uint8> resp(2);
   getAttribute(Flock::Parameter::BirdStatus, 2, resp);

   vpr::Uint16 msb(resp[1]), lsb(resp[0]), status;
   status = (msb<<8)|lsb;
   return status;
}

/** Get status of the bird */
std::vector<vpr::Uint8> FlockStandalone::querySystemStatus()
{
   vprASSERT(UnknownAddressing != mAddrMode && "No addressing mode set.  Has bird been opened?");

   std::vector<vpr::Uint8> sysStatus;

   // Find number of addresses that will be returned
   unsigned num_addrs(14);
   if(ExpandedAddressing == mAddrMode)
      num_addrs = 30;
   if(SuperExpandedAddressing == mAddrMode)
      num_addrs = 126;

   sysStatus.clear();
   sysStatus.resize(num_addrs);
   getAttribute(Flock::Parameter::FlockSystemStatus, num_addrs, sysStatus);

   return sysStatus;
}


void FlockStandalone::printFlockStatus()
{
   // Print the info out
   std::cout << "System status:\n";

   std::cout << "      model:" << mModelId << std::endl;
   std::cout << "      sw ver:" << mSwRevision << std::endl;
   std::cout << "   addr mode:" << getAddressingModeString(mAddrMode) << std::endl;
   std::cout << "   mstr addr:" << mMasterAddr << std::endl;

   for(unsigned i=0;i<mFlockUnits.size();++i)
   {
      const FlockUnit& unit = mFlockUnits[i];
      
      // build label
      std::string label("[");
      if(unit.hasTransmitter())
      {
         label += Transmitter::getTransmitterString(unit.mTransmitterType);
         if (unit.mHasSensor)
         { label += ", "; }
      }
      if(unit.mHasSensor)
      { label += "sensor"; }
      label += "]";

      std::cout << boost::format("  %s: %|-14|  master:%s accessible:%s  running:%s error:%s init:%s") % i 
                                 % label % unit.mIsMaster % unit.mAccessible % unit.mIsRunning % unit.mHasError % unit.mHasBeenInitialized << std::endl;
   }
   
}


// Does not work with older hadware
vpr::ReturnStatus FlockStandalone::readHemisphere()
{
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      vpr::Uint64 timeout=10000;  // How long to wait for data to arrive
      vpr::Uint32 num_read;
      char buff[2];


      pickBird(1);

      buff[0] = 0x4F;
      buff[1] = 0x16;

      vpr::System::msleep(600);
      mSerialPort->write(&buff, sizeof(buff), written);

      //mSerialPort->drainOutput();
      //vpr::System::msleep(500);

      //mSerialPort->flushQueue(vpr::SerialTypes::INPUT_QUEUE);

      vpr::ReturnStatus status = mSerialPort->read(&buff,2,num_read,vpr::Interval(timeout, vpr::Interval::Msec));
      if (!status.success())
      {
         return(status);
      }
      std::cout << "Hemisphere is: " << (int)buff[0] << "." << (int)buff[1] << std::endl;

      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      //vpr::System::msleep(500);

      return(vpr::ReturnStatus::Succeed);
   }
   else
   {
      return(vpr::ReturnStatus::Fail);
   }
}

unsigned FlockStandalone::getMaxBirdAddr()
{
   if(NormalAddressing == mAddrMode) return 14;
   else if(ExpandedAddressing == mAddrMode) return 30;
   else if(SuperExpandedAddressing == mAddrMode) return 127;
   else return 0;
}


void FlockStandalone::readInitialFlockConfiguration()
{
   std::pair<unsigned,unsigned> sw_rev;

   vprASSERT(( mSerialPort != NULL ) && (FlockStandalone::CLOSED != mStatus));
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "Asking the flock for it's configuration.\n" << vprDEBUG_FLUSH;
   
   try
   {
      // Software revision
      sw_rev = querySoftwareRevision();
      mSwRevision = float(sw_rev.first) + float(sw_rev.second)/100.0f;
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "      sw ver: " << mSwRevision << std::endl << vprDEBUG_FLUSH;
      vprASSERT(mSwRevision >= 2.67f && "This driver only works with fob sw version 3.67 or higher");
   
      // Get the model id
      mModelId = queryModelIdString();
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "    model id: " << mModelId << std::endl << vprDEBUG_FLUSH;
   
      // Get the addressing mode
      mAddrMode = queryAddressingMode();
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "   addr mode: " << getAddressingModeString(mAddrMode) << std::endl << vprDEBUG_FLUSH;  

      // Get the address of the master
      mMasterAddr = queryAddress();
      
      // Make initial guess at mode (finalize later after looking for erts)
      if(0 == mMasterAddr)
      { mMode = Standalone; }
      else
      { mMode = Standard; }

      // Setup the flock unit data structure
      setupFlockUnitsDataStructure();

      // Check for erts and set mode if we find one
      for(unsigned u=0;u<mLastActiveUnit;++u)
      { 
         if(mFlockUnits[u].mTransmitterType == Transmitter::Ert0)
            mMode = ExtendedRange;
      }
          
   }
   catch(Flock::FlockException& e)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Flock error: " << e.getMessage() << std::endl << vprDEBUG_FLUSH;      
   }
}


void FlockStandalone::sendSync ()
{
   /////////////////////////////////////////////////////////////////
   // Set CRT sync: (manual page 82)
   //   set crt sync
   //   nosync    -   0
   //   > 72Hz    -   1 (type 1)
   //                 2 (type 2)
   /////////////////////////////////////////////////////////////////
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      unsigned char buff[2];
      buff[0] = 'A';
      buff[1] = mSyncStyle;

      vpr::System::msleep(600);
      mSerialPort->write(buff, 2, written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   }
}

void FlockStandalone::sendHemisphere()
{
   /////////////////////////////////////////////////////////////////
   // Set Hemisphere for birds taking input
   //
   //  buff   [1]   [2]
   // Front: 0x00, 0x00
   // Aft  : 0x00, 0x01
   // Upper: 0x0C, 0x01
   // Lower: 0x0C, 0x00
   // Left : 0x06, 0x01
   // Right: 0x06, 0x00
   /////////////////////////////////////////////////////////////////

   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[3];

      buff[0] = 'L';

      int loop_count = mNumBirds;
      if (ExtendedRange == mMode)
      {
         loop_count++;
      }

      // This will loop over all the birds and the transmitter.  When it
      // encounters the transmitter, it skips it.
      for ( int i = 1; i < loop_count; i++ )
      {
         // Skip the transmitter.
         if (! isTransmitter(i))
         {
            pickBird(i);

            switch ( mHemisphere )
            {
               case FRONT_HEM:
                  buff[1] = 0x00;
                  buff[2] = 0x00;
                  break;
               case AFT_HEM:
                  buff[1] = 0x00;
                  buff[2] = 0x01;
                  break;
               case UPPER_HEM:
                  buff[1] = 0x0C;
                  buff[2] = 0x01;
                  break;
               case LOWER_HEM:
                  buff[1] = 0x0C;
                  buff[2] = 0x00;
                  break;
               case LEFT_HEM:
                  buff[1] = 0x06;
                  buff[2] = 0x01;
                  break;
               case RIGHT_HEM:
                  buff[1] = 0x06;
                  buff[2] = 0x00;
                  break;
            }

            vpr::System::msleep(600);
            mSerialPort->write(buff, sizeof(buff), written);
            // mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

            // vpr::System::usleep(500 * mSleepFactor);
         }
      }
   }
}

void FlockStandalone::sendRepAndStream ()
{
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[1];
      /////////////////////////////////////////////////////////////////
      // Set report rate
      //             Q  Every cycle
      //  buff[0] - 'R' Every other bird cycle
      //             S  every 8 cycles
      //             T  every 32 cycles
      /////////////////////////////////////////////////////////////////
      buff[0] = mReportRate;
      vpr::System::msleep(600);
      mSerialPort->write(buff, sizeof(buff), written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      //vpr::System::usleep(2000 * mSleepFactor);

      ////////////////////////////////////////////////////////////////
      // set stream mode
      ////////////////////////////////////////////////////////////////
      buff[0] = '@';
      vpr::System::msleep(600);
      mSerialPort->write(buff, sizeof(buff), written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      //vpr::System::usleep(500 * mSleepFactor);
   }
}

void FlockStandalone::sendPosAngles ()
{
   //////////////////////////////////////////////////////////////////
   // Set Position Angles
   /////////////////////////////////////////////////////////////////
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[1];

      int loop_count = mNumBirds;
      if (ExtendedRange == mMode)
      {
         loop_count++;
      }

      for ( int bird_id = 1; bird_id < loop_count; bird_id++ )
      {
         // Skip the transmitter unit.
         if (! isTransmitter(bird_id))
         {
            pickBird(bird_id);
            buff[0] = 'Y';

            vpr::System::msleep(600);
            mSerialPort->write(buff, sizeof(buff), written);
            //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

            //vpr::System::usleep(500 * mSleepFactor);
         }
      }
   }
}

void FlockStandalone::sendFilter ()
{
   ///////////////////////////////////////////////////////////////
   // Turn filters on (manual page 48)
   // 0s turn AC NARROW notch filter ON
   //         AC WIDE notch filter ON
   //         DC filter ON
   ///////////////////////////////////////////////////////////////
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[4];

      buff[0] = 'P';
      buff[1] = 0x04;
      buff[2] = 0x00;
      buff[3] = 0;

      vpr::System::msleep(600);
      mSerialPort->write(buff, sizeof(buff), written);
      // mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

      //TODO: Do I need to sleep here?
      // vpr::System::usleep(12000 * mSleepFactor);
   }
}

void FlockStandalone::sendTransmitter ()
{
   ///////////////////////////////////////////////////////////////
   // Sets up the device for Transmitting (manual page 67)
   // Command (0x30) for Next Transmitter
   ///////////////////////////////////////////////////////////////
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[2];

      buff[0] = (unsigned char) (0x30);
      buff[1] = (unsigned char) mXmitterUnitNumber << 4;

      vpr::System::msleep(600);
      mSerialPort->write(buff, sizeof(buff), written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

      //vpr::System::usleep(12000 * FlockStandalone::mSleepFactor);
   }
}

void FlockStandalone::sendAutoconfig ()
{
   ///////////////////////////////////////////////////////////////
   // FBB AUTO-CONFIGURATION (manual page 60)
   //
   // Must wait 300 milliseconds before and after this command
   ///////////////////////////////////////////////////////////////
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[3];

      buff[0] = 'P';
      buff[1] = 0x32;
      buff[2] = mNumBirds + ((ExtendedRange == mMode) ? 1 : 0); //number of input devices + 1 for transmitter

      vpr::System::msleep(600);
      mSerialPort->write(buff, sizeof(buff), written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      vpr::System::msleep(600);
   }
}

void FlockStandalone::sendGroup ()
{
   ////////////////////////////////////////////////////////////////
   // Setup group mode: (manual page 59)
   // 'P' Change Parameter
   // Number 35 (hex 23),
   // Set flag to 1 enabling group mode.
   ////////////////////////////////////////////////////////////////
   vpr::Uint32 written;
   if ( mSerialPort != NULL )
   {
      char buff[3];

      buff[0] = 'P';
      buff[1] = 0x23;
      buff[2] = 0x01;

      vpr::System::msleep(600);
      mSerialPort->write(buff, sizeof(buff), written);
      //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      //vpr::System::sleep(2);
   }
}

/****************************************************
 * Begin testing functions
 *
 ****************************************************/

void FlockStandalone::checkGroup()
{
   char exam1[1], exam2[1];
   exam1[0] = 'O';
   exam2[0] = 0x23;
   vpr::Uint32 buf=0, buf1=0;
   char in[2];
   in[0]='a';


   while ( true )
   {
      mSerialPort->write(exam1, sizeof(char), buf);
      mSerialPort->write(exam2, sizeof(char), buf);

      mSerialPort->readn(in, sizeof(char), buf1);
      std::cout << in[0] << " " << buf << " " << buf1 << std::endl;
   }
}

void FlockStandalone::checkConfig()
{
   if ( mSerialPort != NULL )
   {

      char exam1[1], exam2[1];
      exam1[0] = 'O';
      exam2[0] = 0x0;
      vpr::Uint32 buf=0, buf1=0;
      char in[2] = {'a','a'};
      int i = 0;

//   clearBuffer();

      while ( i < 400 )
      {
         i++;
         mSerialPort->write(exam1, sizeof(char), buf);
         mSerialPort->write(exam2, sizeof(char), buf);
         mSerialPort->readn(&in, sizeof(char)*2, buf1);
         showbits(in[1]); std::cout << "|"; showbits(in[0]);
         std::cout << std::endl;
      }
   }
}

void FlockStandalone::checkPosAngles()
{
   return;
}

void FlockStandalone::checkRepAndStream()
{
   return;
}

void FlockStandalone::showbits(char var)
{
   static unsigned mask[8]={1,2,4,8,16,32,64,128};

   //if (var < 0)
   //{
   //   std::cout << " (-) ";
   //}
   unsigned bitval;
   for ( int bit=7; bit>=0; bit-- )
   {
      bitval=mask[bit]&var;
      printf("%c",bitval?'1':'0');
   }
}

void FlockStandalone::showbits16(float var)
{
   static int mask[16]={  1,  2,   4,   8,  16,  32,   64,  128,
                             256,512,1024,2048,4096,8192,16384,32768};

   if (var < 0)
   {
      std::cout << " (-) ";
   }
   unsigned bitval;
   for ( int bit=15; bit>=0; bit-- )
   {
      bitval=mask[bit]&(int)var;
      printf("%c",bitval?'1':'0');
   }
}

int FlockStandalone::hexToInt(char var)
{
   static unsigned mask[8]={1,2,4,8,16,32,64,128};
   int bitval=0;
   for ( int bit=7; bit>=0; bit-- )
   {
      bitval+=mask[bit]&var;
   }
   return bitval;
}

void FlockStandalone::checkDataReadyChar()
{
   char exam1[1], exam2[1];
   exam1[0] = 'O';
   exam2[0] = 0x9;
   vpr::Uint32 buf=0, buf1=0;
   char in[2] = {'a','a'};
   int i = 0;

   // Initial exam
   do
   {
      i++;
      mSerialPort->write(exam1, sizeof(char), buf);
      mSerialPort->write(exam2, sizeof(char), buf);
      mSerialPort->readn(&in, sizeof(char)*2, buf1);
   } while ( in[0] != ',' );

   std::cout << "I=" << i << std::endl;
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   vpr::System::usleep(500);
}

/** Send command.
* @param cmd - cmd to send
*/
void FlockStandalone::sendCommand(vpr::Uint8 cmd, std::vector<vpr::Uint8> data )
{
   if ( NULL == mSerialPort )
   { throw Flock::ConnectionException("NULL port"); }

   unsigned bytes_written;
   
   vpr::ReturnStatus ret_val = mSerialPort->write(&cmd, 1, bytes_written);
   if(ret_val.failure())
   { throw Flock::CommandFailureException("Failed to write full command"); }

   if(!data.empty())
   {
      vpr::ReturnStatus ret_val = mSerialPort->write(&(data[0]), data.size(), bytes_written);
      if(ret_val.failure())
      { throw Flock::CommandFailureException("Failed to write full command args"); }
   }
}

void FlockStandalone::pickBird (const vpr::Uint8 birdID)
{
   vpr::Uint32 written;
   vpr::Uint8 cmd;
   std::vector<vpr::Uint8> cmd_data(0);      // Default to no data

   if(NormalAddressing == mAddrMode)
   {
      vprASSERT(birdID < 16);
      cmd = Flock::Command::ToFbbNormal + birdID;
   }
   if(ExpandedAddressing == mAddrMode)
   {  
      vprASSERT(birdID < 31);
      if(birdID < 16)
         cmd = Flock::Command::ToFbbNormal + birdID;
      else
         cmd = Flock::Command::ToFbbExpanded + birdID - 0x10;
   }
   else if(SuperExpandedAddressing == mAddrMode)
   {  
      cmd = Flock::Command::ToFbbSuperExpanded;
      cmd_data.push_back(birdID);
   }
   
   sendCommand(cmd, cmd_data);
}


/**
 * Examines an attribute.
 *
 * @param attrib - Attribute to query for - see the Flock manual.
 * @param respSize - Expected size of the response
 * @param respData   - Returned data
 */
void FlockStandalone::getAttribute(vpr::Uint8 attrib, unsigned respSize, std::vector<vpr::Uint8>& respData)
{
   vpr::Uint8 exam_cmd[2];       // The command to send for the examine
   exam_cmd[0] = Flock::Command::ExamineValue;
   exam_cmd[1] = attrib;
   vpr::Uint32 bytes_written;
   vpr::Uint32 bytes_read;
   //vpr::Interval read_timeout(2,vpr::Interval::Sec);

   if ( NULL == mSerialPort )
   { throw Flock::ConnectionException("NULL port"); }

   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);       // Clear the buffers

   vpr::System::msleep(500);
   
   // Send command
   mSerialPort->write(exam_cmd, 2, bytes_written);
   vprASSERT(2 == bytes_written);
   mSerialPort->drainOutput();
   //vpr::System::msleep(500);

   if(bytes_written != sizeof(exam_cmd))
   {  throw Flock::CommandFailureException("Full command not written"); }

   // Read response and then flush the port to make sure we don't leave anything extra
   mSerialPort->readn(respData, respSize, bytes_read);
   //mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

   // Check response size
   if(bytes_read != respSize)
   {  throw Flock::CommandFailureException("Incomplete command response"); }
}


/***********************************************************
 * Here are some additional methods for setting values of
 * the flock.
 **********************************************************/

void FlockStandalone::setRunMode()
{
   char exam1[1];
   exam1[0] = 'F';  // Put the flock into RUN mode
   vpr::Uint32 buf=0;

   for ( int i = 0; i < 150; i++ )
   {
      mSerialPort->write(exam1, sizeof(char), buf);
   }
}

void FlockStandalone::setSleepMode()
{
   char exam1[1];
   exam1[0] = 'G';  // Put the flock into SLEEP mode
   vpr::Uint32 buf=0;

   for ( int i = 0; i < 150; ++i )
   {
      mSerialPort->write(exam1, sizeof(char), buf);
   }
}

void FlockStandalone::setValue(char exam, char setdata, int reps)
{
   char exam1[1];
   char exam2[1];
   char data[1];
   exam1[0] = 'P';
   exam2[0] = exam;
   data[0] = setdata;
   vpr::Uint32 buf=0;
   int i = 1;

   while ( i < reps )
   {
      i++;
      mSerialPort->write(exam1, sizeof(char), buf);
      mSerialPort->write(exam2, sizeof(char), buf);
      mSerialPort->write(data, sizeof(char), buf);
   }

   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   vpr::System::usleep(500);
}

void FlockStandalone::setPosMode()
{
   char exam3[1];
   exam3[0] = 0x56;  // Position mode
   vpr::Uint32 buf=0;
   int i = 0;

   while ( i < 150 )
   {
      i++;
      mSerialPort->write(exam3, sizeof(char), buf);
   }

}

void FlockStandalone::setPosAngMode()
{
   char exam3[1];
   exam3[0] = 0x59;  // Position/angle mode
   vpr::Uint32 buf=0;
   int i = 0;

   while ( i < 150 )
   {
      i++;
      mSerialPort->write(exam3, sizeof(char), buf);
   }
}

void FlockStandalone::setStreamMode()
{
   char exam1[1];
   exam1[0] = '@';  // Put the flock into STREAM mode
   vpr::Uint32 buf=0;

   for ( int i = 0; i < 150; i++ )
   {
      mSerialPort->write(exam1, sizeof(char), buf);
   }
}

void FlockStandalone::setPointMode()
{
   char exam1[1];
   exam1[0] = 'B';  // Put the flock into STREAM mode
   vpr::Uint32 buf=0;

   for ( int i = 0; i < 150; ++i )
   {
      mSerialPort->write(exam1, sizeof(char), buf);
   }
}

void FlockStandalone::setDeviceReportRate(char rate)
{
   char exam1[1];
   vpr::Uint32 buf=0;

   switch ( rate )
   {
      case 'Q':
         std::cout << "setting report rate to every cycle" << std::endl;
         exam1[0] = 'Q';
         break;
      case 'R':
         std::cout << "setting report rate to every other cycle" << std::endl;
         exam1[0] = 'R';
         break;
      case 'S':
         std::cout << "setting report rate to every 8th cycle" << std::endl;
         exam1[0] = 'S';
         break;
      case 'T':
         std::cout << "setting report rate to every 32nd cycle" << std::endl;
         exam1[0] = 'T';
         break;
   }

   for ( int i = 0; i < 500; ++i )
   {
      mSerialPort->write(exam1, sizeof(char), buf);
   }
}


void FlockStandalone::printError( unsigned char ErrCode, unsigned char ExpandedErrCode )
{
  /*
    Display a message describing the Error
    */
  switch (ErrCode) {
  case 0:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** No Errors Have Occurred ****\n" << vprDEBUG_FLUSH;
    break;
  case 1:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** System RAM Test Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 2:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Non-Volatile Storage Write Failure ****\n" << vprDEBUG_FLUSH;
    break;
  case 3:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** System EEPROM Configuration Corrupt ****\n" << vprDEBUG_FLUSH;
    break;
  case 4:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Transmitter EEPROM Configuration Corrupt ****\n" << vprDEBUG_FLUSH;
    break;
  case 5:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Receiver EEPROM Configuration Corrupt ****\n" << vprDEBUG_FLUSH;
    break;
  case 6:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Invalid RS232 Command ****\n" << vprDEBUG_FLUSH;
    break;
  case 7:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Not an FBB Master ****\n" << vprDEBUG_FLUSH;
    break;
  case 8:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** No 6DFOBs are Active ****\n" << vprDEBUG_FLUSH;
    break;
  case 9:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** 6DFOB has not been Initialized ****\n" << vprDEBUG_FLUSH;
    break;
  case 10:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** FBB Receive Error - Intra Bird Bus ****\n" << vprDEBUG_FLUSH;
    break;
  case 11:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** RS232 Overrun and/or Framing Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 12:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** FBB Receive Error - FBB Host Bus ****\n" << vprDEBUG_FLUSH;
    break;
  case 13:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) <<
	    "**** No FBB Command Response from Device at address "
       << (ExpandedErrCode & 0x0f) << "(decimal) ****\n" << vprDEBUG_FLUSH;
    break;
  case 14:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Invalid FBB Host Command ****\n" << vprDEBUG_FLUSH;
    break;
  case 15:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** FBB Run Time Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 16:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Invalid CPU Speed ****\n" << vprDEBUG_FLUSH;
    break;
  case 17:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Slave No Data Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 18:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Illegal Baud Rate ****\n" << vprDEBUG_FLUSH;
    break;
  case 19:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Slave Acknowledge Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 20:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** CPU Overflow Error - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 21:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Array Bounds Error - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 22:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Unused Opcode Error - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 23:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Escape Opcode Error - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 24:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Reserved Int 9 - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 25:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Reserved Int 10 - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 26:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Reserved Int 11 - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 27:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Numeric CPU Error - call factory ****\n" << vprDEBUG_FLUSH;
    break;
  case 28:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** CRT Syncronization Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 29:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Transmitter Not Active Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 30:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** ERC Extended Range Transmitter Not Attached Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 31:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** CPU Time Overflow Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 32:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Receiver Saturated Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 33:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** Slave Configuration Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 34:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** ERC Watchdog Error ****\n" << vprDEBUG_FLUSH;
    break;
  case 35:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** ERC Overtemp Error ****\n" << vprDEBUG_FLUSH;
    break;
  default:
    vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "**** UNKNOWN ERROR... check user manual ****\n" << vprDEBUG_FLUSH;
    break;
  }
}

// check for flock error, return err number if there is an error
// zero if no error
int FlockStandalone::checkError()
{
   vpr::Uint32 bytes_read=0;
   unsigned char bird_command[2];

   // put the flock to sleep (B to get out of stream mode, G to sleep)
   bird_command[0] = 'B';
   bird_command[1] = 'G';
   if (mSerialPort->write(&bird_command,2,bytes_read) != vpr::ReturnStatus::Succeed)
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << " [FlockStandalone] Failed writing cmds to tracker.\n" << vprDEBUG_FLUSH;
      return -1;
   }

   mSerialPort->drainOutput();
   vpr::System::msleep(500);

   // now get error code and clear error status
   // we want error code 16, not 10 -- we want the expanded error code
   // prepare error status query (expanded error codes)

   mSerialPort->flushQueue(vpr::SerialTypes::INPUT_QUEUE);

   bird_command[0] = 'O';
   bird_command[1] = 16;

   if (mSerialPort->write(&bird_command,2,bytes_read) != vpr::ReturnStatus::Succeed)
   {
     vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << " [FlockStandalone] Failed writing cmds to tracker.\n" << vprDEBUG_FLUSH;
     return -1;
   }

   // make sure the command is sent out
   mSerialPort->drainOutput();
   vpr::System::msleep(500);

   // read response (2 char response to error query 16),
   // 1 char response to 10

   mSerialPort->read(&bird_command,2,bytes_read);
   if ( bytes_read !=2)
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << " [FlockStandalone] Received only "
         << bytes_read << " of 2 chars for error code\n" << vprDEBUG_FLUSH;
      return -1;
   }

   printError( bird_command[0], bird_command[1] );

   return bird_command[0];
}

bool FlockStandalone::isTransmitter(int birdID) const
{
   // This flock is configured for extended range and thus has a transmitter on
   // the FBB.
   if (ExtendedRange == mMode)
   {
      // Check if the given ID is the ID for the transmitter
      if (birdID == mXmitterUnitNumber)
      {
         // Yay ... this is a transmitter. God help us all.
         return true;
      }
   }

   // Must not be a transmitter
   return false;
}

void FlockStandalone::setupFlockUnitsDataStructure()
{
   unsigned max_units = getMaxBirdAddr();
   mFlockUnits.resize(max_units);   // Allocate enough room
   
   // --- Get data from system status command --- //
   std::vector<vpr::Uint8> sys_status = querySystemStatus();

   for(unsigned i=0;i<sys_status.size();++i)
   {
      vpr::Uint8 status = sys_status[i];
      FlockUnit& unit = mFlockUnits[i];

      unit.mAddr = i+1;                    // Address is one greater
      unit.mAccessible = Flock::SystemStatus::isAccessible(status);
      unit.mIsRunning = Flock::SystemStatus::isRunning(status);
      unit.mHasSensor = Flock::SystemStatus::hasSensor(status);
      unit.mTransmitterType = Flock::SystemStatus::getTransmitterType(status);      
   }

   // Correct for standalone case
   if(Standalone == mMode)
   {  mFlockUnits[0].mAddr = 0; }

   // Find the last active address (last one accessible)
   mLastActiveUnit = 0;
   flock_units_t::iterator cur_unit = mFlockUnits.begin();
   while( (cur_unit != mFlockUnits.end()) &&
          ((*cur_unit).mAccessible))
   { mLastActiveUnit += 1; }
      
   vprASSERT(mLastActiveUnit < mFlockUnits.size());
   vprASSERT(mFlockUnits[mLastActiveUnit].mAccessible);

   // Query each bird individually for more information
   for(unsigned i=0;i<mLastActiveUnit;++i)
   {
      FlockUnit& unit = mFlockUnits[i];
      vpr::Uint16 status = queryBirdStatus(unit.mAddr);
      unit.mIsMaster = Flock::BirdStatus::isMaster(status);
      unit.mHasBeenInitialized = Flock::BirdStatus::isInititalized(status);
      unit.mHasError = Flock::BirdStatus::hasError(status);
   }
}
      
