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
{
   mReadTimeout = vpr::Interval(2,vpr::Interval::Sec);
}

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
         mSerialPort->setUpdateAction(vpr::SerialTypes::NOW);  // Changes apply immediately
         mSerialPort->clearAll();
         mSerialPort->setCanonicalInput(true);              // enable binary reading and timeouts
         mSerialPort->setTimeout(10);                       // Set to 1 inter-byte read second timeout
         mSerialPort->setRead(true);                        // Allow reading from port
         mSerialPort->setLocalAttach(true);                 // Say we are directly attached
         mSerialPort->setBreakByteIgnore(true);             // Ignore terminal breaks
                  
         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "  Setting baud rate: " << mBaud << std::endl << vprDEBUG_FLUSH;
         mSerialPort->setInputBaudRate(mBaud);
         mSerialPort->setOutputBaudRate(mBaud);

         mSerialPort->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
         mSerialPort->setHardwareFlowControl(false);     // No hardware flow control
         mSerialPort->setParityGeneration(false);        // No parity checking
         
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

         // Get the initial configuration from the flock
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


/** Configure the flock for execution.
* @pre Flock is open and all configurable settings have been passed.
* @post Flock is setup and configured to start getting samples.  It is ready to run.
*/
vpr::ReturnStatus FlockStandalone::configure()
{
   // Make sure we are called correctly
   vprASSERT((mStatus == FlockStandalone::OPEN) && "Tried to call configure with flock in wrong mode");

   vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << " [FlockStandalone] Configuring the flock.\n" << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] " << Flock::getModeString(mMode)
                                             << " mode.\n" << vprDEBUG_FLUSH;
   // If we are not standalone, then go into group mode first
   if(Flock::Standalone == mMode)
   {
         
   }
   else
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting group\n" << vprDEBUG_FLUSH;
      sendGroupCmd(true);        // Go into group mode   
   }

   // Set all parameters
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting sync\n" << vprDEBUG_FLUSH;
   sendSyncCmd(mSyncStyle);
      
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting hemisphere\n" << vprDEBUG_FLUSH;
   sendHemisphereCmd(mHemisphere,true);
      
   if(Flock::Standalone != mMode)
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting autoconfig\n" << vprDEBUG_FLUSH;
      sendAutoconfigCmd(mActiveUnitEndIndex-1);
   }
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
   mStatus = FlockStandalone::READY;

   // return success
   return vpr::ReturnStatus::Succeed;      
}

/** Call this to connect to the Flock device.
* - If closed, open port
* - If not streaming, start streaming
*
*/
vpr::ReturnStatus FlockStandalone::start()
{
   // If port closed right now
   vprASSERT(mStatus == OPEN && "Tried to call configure with flock in wrong mode");

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
void FlockStandalone::setSync(vpr::Uint8 sync)
{
   if ( (FlockStandalone::CLOSED != mStatus) && (FlockStandalone::OPEN != mStatus) )
   {
      throw Flock::CommandFailureException("Sync command not allowed after flock configured");
   }
   else
   {
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
            vpr::ReturnStatus status = mSerialPort->read(&buff,1,num_read, mReadTimeout);
            if (!status.success())
            {
               return(status);
            }
         } while( !(0x80 & buff[0]));

            // Read in the rest of the data, [ (6*2) + 1 for the group byte ] - 1 byte already read
         vpr::ReturnStatus status = mSerialPort->read(&buff[1],12,num_read, mReadTimeout);
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

      vpr::ReturnStatus status = mSerialPort->read(&buff,2,num_read, mReadTimeout);
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
      for(unsigned u=0;u<mActiveUnitEndIndex;++u)
      { 
         if(mFlockUnits[u].mTransmitterType == Transmitter::Ert0)
         {  mMode = ExtendedRange; }
      }
          
   }
   catch(Flock::FlockException& e)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Flock error: " << e.getMessage() << std::endl << vprDEBUG_FLUSH;      
   }
}

void FlockStandalone::sendGroupCmd(bool newVal)
{
   std::vector<vpr::Uint8> param;
   param.push_back(vpr::Uint8(newVal));
   setAttribute(Flock::Parameter::GroupMode, param);
}

void FlockStandalone::sendSyncCmd(vpr::Uint8 syncMethod)
{
   std::vector<vpr::Uint8> param;
   param.push_back(syncMethod);
   sendCommand(Flock::Command::Sync,param);
}

void FlockStandalone::sendHemisphereCmd(BIRD_HEMI hemi, bool sendToAll)
{
   std::vector<vpr::Uint8> params(2);   
    
   switch ( hemi )
   {
      case FRONT_HEM:
         params[1] = 0x00;
         params[2] = 0x00;
         break;
      case AFT_HEM:
         params[1] = 0x00;
         params[2] = 0x01;
         break;
      case UPPER_HEM:
         params[1] = 0x0C;
         params[2] = 0x01;
         break;
      case LOWER_HEM:
         params[1] = 0x0C;
         params[2] = 0x00;
         break;
      case LEFT_HEM:
         params[1] = 0x06;
         params[2] = 0x01;
         break;
      case RIGHT_HEM:
         params[1] = 0x06;
         params[2] = 0x00;
         break;
   }

   if(sendToAll)
   {
      sendCommandAll(Flock::Command::Hemisphere, params, true);
   }
   else
   {
      sendCommand(Flock::Command::Hemisphere, params);
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

void FlockStandalone::sendNextTransmitterCmd (vpr::Uint8 addr, vpr::Uint8 transmitterNumber)
{
   std::vector<vpr::Uint8> param(1);
   vprASSERT(transmitterNumber <= 3);
   vprASSERT(addr <= 14);

   param[0] = (addr << 4) & transmitterNumber;
   
   sendCommand(Flock::Command::NextTransmitter, param);
      
   vpr::System::msleep(300);   
}

void FlockStandalone::sendAutoconfigCmd (vpr::Uint8 numUnits)
{
   // Must wait 600 milliseconds before and after this command
   std::vector<vpr::Uint8> params;
   params.push_back(numUnits)

   vpr::System::msleep(600);
   setAttribute(Flock::Parameter::FbbAutoConfig,params);
   vpr::System::msleep(600);   
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

/** Send command to all units (except excluded types)
*/
void FlockStandalone::sendCommandAll(vpr::Uint8 cmd, std::vector<vpr::Uint8> data, bool excludeErc)
{
   for(unsigned u=0;u<mActiveUnitEndIndex;++u)
   {
      const FlockUnit& unit = mFlockUnits[u];
      if(excludeErc && Transmitter::isErt(unit.mTransmitterType))
      { continue; }

      // Send command after picking bird
      pickBird(unit.mAddr);
      sendCommand(cmd,data);
   }
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
   
   if ( NULL == mSerialPort )
   { throw Flock::ConnectionException("NULL port"); }

   //vpr::System::msleep(200);                                   // Wait for any random input
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);       // Clear the buffers
   //vpr::System::msleep(300);                                   // Let the buffers clear
   
   // Send command
   mSerialPort->write(exam_cmd, 2, bytes_written);
   vprASSERT(2 == bytes_written);
   mSerialPort->drainOutput();   

   if(bytes_written != sizeof(exam_cmd))
   {  throw Flock::CommandFailureException("Full command not written"); }

   // Read response and then flush the port to make sure we don't leave anything extra
   mSerialPort->read(respData, respSize, bytes_read, mReadTimeout);
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

   // Check response size
   if(bytes_read != respSize)
   {  throw Flock::CommandFailureException("Incomplete command response"); }
}

/**
 * Change an attribute.
 *
 * @param attrib - Attribute to set - see the Flock manual.
 * @param respData - Argument data
 */
void FlockStandalone::setAttribute(vpr::Uint8 attrib, std::vector<vpr::Uint8>& attribData)
{
   vpr::Uint8 change_cmd[2];                    // The command to send for the change
   exam_cmd[0] = Flock::Command::ChangeValue;
   exam_cmd[1] = attrib;
   vpr::Uint32 bytes_written;
   
   if ( NULL == mSerialPort )
   { throw Flock::ConnectionException("NULL port"); }

   //vpr::System::msleep(200);                                   // Wait for any random input
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);       // Clear the buffers
   //vpr::System::msleep(300);                                   // Let the buffers clear
   
   // Send command
   mSerialPort->write(change_cmd, 2, bytes_written);
   if(bytes_written != sizeof(exam_cmd))
   {  throw Flock::CommandFailureException("Full command not written"); }
   
   // Send args
   if(!attribData.empty())
   {
      mSerialPort->write(attribData, bytes_written);
      if(bytes_written != attribData.size())
      {  throw Flock::CommandFailureException("Change command args not fully written"); }
   }
   mSerialPort->drainOutput();   
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
   {  
      mFlockUnits[0].mAddr = 0;     // Manually set the correct bird address
      mActiveUnitEndIndex = 1;      // Manually position it past first unit
   }
   else
   {
      // Find the last active address (last one accessible)
      mActiveUnitEndIndex = 0;
      flock_units_t::iterator cur_unit = mFlockUnits.begin();
      // While(not at end && unit is accessible
      while( (cur_unit != mFlockUnits.end()) && ((*cur_unit).mAccessible))
      { mActiveUnitEndIndex += 1; cur_unit++; }
         
      vprASSERT(mActiveUnitEndIndex <= mFlockUnits.size());
      vprASSERT((mActiveUnitEndIndex == 0) || 
                (mFlockUnits[mActiveUnitEndIndex-1].mAccessible) );
   }

   // Query each bird individually for more information
   for(unsigned i=0;i<mActiveUnitEndIndex;++i)
   {
      FlockUnit& unit = mFlockUnits[i];
      vpr::Uint16 status = queryBirdStatus(unit.mAddr);
      unit.mIsMaster = Flock::BirdStatus::isMaster(status);
      unit.mHasBeenInitialized = Flock::BirdStatus::isInititalized(status);
      unit.mHasError = Flock::BirdStatus::hasError(status);
   }
}
      
