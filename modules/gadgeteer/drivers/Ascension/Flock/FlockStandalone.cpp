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

#include <iostream>        /* for std::cout */
#include <fstream>         /* for std::ifstream */
#include <string.h>
#include <iterator>
#include <cstddef>

#include <vpr/vpr.h>

#if ! defined(VPR_OS_IRIX)
#  include <boost/format.hpp>
#endif
#include <boost/lexical_cast.hpp>

#include <gmtl/Generate.h>
#include <gmtl/Vec.h>
#include <gmtl/Quat.h>
#include <gmtl/MatrixOps.h>

#include <vpr/System.h>
#include <vpr/IO/WouldBlockException.h>
#include <vpr/Util/Assert.h> /* for vprASSERT */
#include <gadget/Util/Debug.h>

#include <drivers/Ascension/Flock/FlockStandalone.h>


namespace Flock
{
   std::string getErrorDescription(vpr::Uint8 errCode, vpr::Uint8 expandedErr)
   {
#define F_ERR(x,y)  case x: ret_val = y; break;

      std::string ret_val;
      switch(errCode)
      {
         F_ERR(0, "No Errors Have Occurred");
         F_ERR(1, "System RAM Failure");
         F_ERR(2, "Non-Volatile Storage Write Failure");
         F_ERR(3, "PCB EEPROM Configuration Corrupt");
         F_ERR(4, "Transmitter EEPROM Configuration Data Corrupt");
         F_ERR(5, "Receiver EEPROM Configuration Corrupt");
         F_ERR(6, "Invalid RS232 Command");
         F_ERR(7, "Not an FBB Master");
         F_ERR(8, "No 6DFOBs are Active");
         F_ERR(9, "6DFOB has not been Initialized");
         F_ERR(10, "FBB Receive Error - Intra Bird Bus");
         F_ERR(11, "RS232 Overrun and/or Framing Error");
         F_ERR(12, "FBB Receive Error - FBB Host Bus");
         case 13:
            ret_val = "No FBB Command Response from device at address: " + boost::lexical_cast<std::string>(int(expandedErr));
            break;
         F_ERR(14, "Invalid FBB Host Command");
         F_ERR(15, "FBB Run Time Error");
         F_ERR(16, "Invalid CPU Speed");
         F_ERR(17, "Slave No Data Error");
         F_ERR(18, "Illegal Baud Rate");
         F_ERR(19, "Slave Acknowledge Error");
         F_ERR(20, "CPU Overflow Error - call factory");
         F_ERR(21, "Array Bounds Error - call factory");
         F_ERR(22, "Unused Opcode Error - call factory");
         F_ERR(23, "Escape Opcode Error - call factory");
         F_ERR(24, "Reserved Int 9 - call factory");
         F_ERR(25, "Reserved Int 10 - call factory");
         F_ERR(26, "Reserved Int 11 - call factory");
         F_ERR(27, "Numeric CPU Error - call factory");
         F_ERR(28, "CRT Syncronization Error");
         F_ERR(29, "Transmitter Not Active Error");
         F_ERR(30, "ERC Extended Range Transmitter Not Attached Error");
         F_ERR(31, "CPU Time Overflow Error");
         F_ERR(32, "Receiver Saturated Error");
         F_ERR(33, "Slave Configuration Error");
         F_ERR(34, "ERC Watchdog Error");
         F_ERR(35, "ERC Overtemp Error");
         default:
            ret_val = "Uknown error.";
            break;
      }
#undef F_ERR

      return ret_val;
   }
}  // end namespace flock


/** Configure constructor. */
FlockStandalone::FlockStandalone(std::string port, const int& baud,
                                 const int& sync, const BIRD_HEMI& hemi,
                                 const BIRD_FILT& filt,
                                 Flock::ReportRate report)
   : mStatus(FlockStandalone::CLOSED)
   , mPort(port)
   , mSerialPort(NULL)
   , mBaud(baud)
   , mMode(Flock::UnknownMode)
   , mAddrMode(Flock::UnknownAddressing)
   , mSwRevision(0.0f)
   , mNumSensors(0)
   , mMaxPos(3.0f)
   , mReportRate(report)
   , mHemisphere(hemi)
   , mFilter(filt)
   , mSyncStyle(sync)
{
   mReadTimeout = vpr::Interval(2,vpr::Interval::Sec);
   mOutputFormat = Flock::Output::PositionAngle;
}

FlockStandalone::~FlockStandalone()
{
   if ( FlockStandalone::CLOSED != mStatus )
   {
      this->close();
      mStatus = FlockStandalone::CLOSED;
   }

   if ( mSerialPort != NULL )
   {
      delete mSerialPort;
      mSerialPort = NULL;
   }
}

// Open the port.
// - Opens the flock port and configures it for communication
// - Acquires the initial configuration of the flock
bool FlockStandalone::open()
{
   bool status(true);

   // - Open and close the port to reset the tracker, then
   // - Open the port
   vprDEBUG_BEGIN(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "====== Opening fob serial port: " << mPort << " =====\n"
      << vprDEBUG_FLUSH;

   bool open_successfull;
   unsigned int attempt_num = 0;
   const unsigned max_open_attempts(7);

   // Allocate the port if we need to
   if ( NULL == mSerialPort )
   {
      mSerialPort = new vpr::SerialPort(mPort);
      if (!mSerialPort)
      {
         return false;
      }
   }

   // Initially open and close the port (this seems to help connections on
   // some operating systems.
   mSerialPort->open();
   mSerialPort->setRequestToSend(true);
   vpr::System::msleep(200);
   mSerialPort->close();

   // do: open port and try to read
   // while: not successful
   // --> Repetitively try to open port and setup parameters until connection
   //     works correctly
   do
   {
      open_successfull = true;

      mSerialPort->setOpenReadWrite();

      //   - Set the port attributes to use
      try
      {
         mSerialPort->open();
      }
      catch (vpr::IOException&)
      {
         vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
            << "Port open failed\n" << vprDEBUG_FLUSH;
         return false;
      }
      {
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
            << "Port opened successfully\n" << vprDEBUG_FLUSH ;
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
            << "  Configuring port attributes\n" << vprDEBUG_FLUSH;

         // Set the basic port attributes to use
         mSerialPort->setUpdateAction(vpr::SerialTypes::NOW);  // Changes apply immediately
         mSerialPort->clearAll();
         mSerialPort->setBlocking(true);              // Open in blocking mode
         mSerialPort->setCanonicalInput(false);              // enable binary reading and timeouts

         mSerialPort->setTimeout(10);                       // Set to 1 inter-byte read second timeout
         mSerialPort->setRead(true);                        // Allow reading from port
         mSerialPort->setLocalAttach(true);                 // Say we are directly attached
         mSerialPort->setBreakByteIgnore(true);             // Ignore terminal breaks

         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
            << "  Setting baud rate: " << mBaud << std::endl << vprDEBUG_FLUSH;
         mSerialPort->setInputBaudRate(mBaud);
         mSerialPort->setOutputBaudRate(mBaud);

         mSerialPort->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
         mSerialPort->setHardwareFlowControl(false);     // No hardware flow control
         mSerialPort->setParityGeneration(false);        // No parity checking

         // --- Reset the flock with RTS signal --- //
         // - When RTS signal is high, bird is put into reset mode
         // So, to reset it: put signal high, wait, put signal low, wait
         // 1 second of wait seems to work best
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
            << "Resetting flock (using RTS signal)." << vprDEBUG_FLUSH;
         mSerialPort->setRequestToSend(true);
         for(unsigned int i=0;i<5;++i)
         {
            vpr::System::msleep(200);
            vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL) << "." << vprDEBUG_FLUSH;
         }

         mSerialPort->setRequestToSend(false);              // Set RTS low to allow the flock to start back up
         for(unsigned int i=0;i<5;++i)
         {
            vpr::System::msleep(200);
            vprDEBUG_CONT(vprDBG_ALL, vprDBG_CONFIG_LVL)
               << "." << vprDEBUG_FLUSH;
         }
         vprDEBUG_CONT(vprDBG_ALL,vprDBG_CONFIG_LVL)
            << " done.\n" << vprDEBUG_FLUSH;

         try
         {
            mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
         }
         catch (vpr::IOException&)
         {
            open_successfull = false;
         }

         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
            << "Port setup completed.\n" << vprDEBUG_FLUSH;

         mStatus = FlockStandalone::OPEN;

         // If open was successfull so far, then try to read the initial
         // configuration information.
         if(open_successfull)
         {
            try
            {
               readInitialFlockConfiguration();    // Get the initial configuration from the flock
            }
            catch(Flock::CommandFailureException& cfe)
            {
               vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
                  << "Flockstandalone::open: command exception: "
                  << cfe.what() << std::endl << vprDEBUG_FLUSH;
               open_successfull = false;
            }
         }

         // If not successfull
         // - close port
         // - delete and allocate a new one
         if(!open_successfull)
         {
            if (mSerialPort->isOpen())
            {
               mSerialPort->close();
            }
            delete mSerialPort;

            mSerialPort = new vpr::SerialPort(mPort);
            if (!mSerialPort)
            {
               return false;
            }

            vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
               << "FlockStandalone::open: Failed to open successfully on attempt: "
               << attempt_num << ".  Trying again...\n" << vprDEBUG_FLUSH;
            attempt_num++;
         }
      }
   }
   while((!open_successfull) && (attempt_num <max_open_attempts));

   // Print the status information
   printFlockStatus();

   return status;
}

/** Configures the flock and gets it ready to go.
 */
void FlockStandalone::configure()
{
   //setErrorModeIgnore();      // Set to mode where fatal errors are ignored

   // Make sure we are called correctly
   vprASSERT((mStatus == FlockStandalone::OPEN) && "Tried to call configure with flock in wrong mode");

   vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
      << " [FlockStandalone] Configuring the flock.\n" << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << " [FlockStandalone] " << Flock::getModeString(mMode) << " mode.\n"
      << vprDEBUG_FLUSH;
   // If we are not standalone, then go into group mode first
   if(Flock::Standalone == mMode)
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << " [FlockStandalone] Standalone mode... skipping group mode.\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << " [FlockStandalone] Setting group\n" << vprDEBUG_FLUSH;
      sendGroupCmd(true);        // Go into group mode
      checkError();
   }

   // Set all parameters
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << " [FlockStandalone] Setting sync\n" << vprDEBUG_FLUSH;
   //**//sendSyncCmd(mSyncStyle);

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << " [FlockStandalone] Setting hemisphere\n" << vprDEBUG_FLUSH;
   sendHemisphereCmd(mHemisphere, true);

   //sendFilter();
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << " [FlockStandalone] Setting output format to: "
      << Flock::Output::getFormatString(mOutputFormat) << "\n"
      << vprDEBUG_FLUSH;
   sendOutputFormatCmd(mOutputFormat, true);

   // When not in standalone mode
   // - Autoconfigure flock
   // - Set to correct transmitter
   if(Flock::Standalone != mMode)
   {
      // Find address of transmitter to use
      // XXX: Do not try to automatically get transmitter address right now.
      /*
      vpr::Uint8 transmitter_addr(0);
      if(!mXmitterIndices.empty())
      {
         transmitter_addr = mFlockUnits[mXmitterIndices[0]].mAddr;
      }

      if(transmitter_addr != mMasterAddr)
      {
         vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
            << " [FlockStandalone] Setting transmitter to addr: "
            << int(transmitter_addr) << "\n" << vprDEBUG_FLUSH;
         sendNextTransmitterCmd(transmitter_addr, 0);
         checkError();
      }
      */

      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << " [FlockStandalone] Setting autoconfig\n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "    mActiveUnitEndIndex: " << mActiveUnitEndIndex << std::endl
         << vprDEBUG_FLUSH;
      sendAutoconfigCmd(mActiveUnitEndIndex);    // Add one for flock stupidity
      //checkError();
   }

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << " [FlockStandalone] Flock configured and ready to run.\n"
      << vprDEBUG_FLUSH;

   // Check for errors
   checkError();

   // ---- Final setup for all data structures --- //
   // Allocate space for sensor data structure
   mSensorData.resize(mNumSensors);             // Make sure we have enough room for all sensor ids

   // flock is active.
   mStatus = FlockStandalone::RUNNING;
}

/** Call this repeatedly to update the data from the birds. */
void FlockStandalone::sample()
{
   // can't sample when not streaming
   vprASSERT( (STREAMING == mStatus) || (RUNNING == mStatus) );

   std::vector<vpr::Uint8> data_record;
   std::vector<vpr::Uint8> temp_data_record;    // Temp buffer for reading data
   vpr::Uint8 buffer;                           // Temporary single byte buffer

   vpr::Uint32 bytes_read;
   vpr::Uint32 bytes_remaining;
   const vpr::Uint8 phase_mask(1<<7);     // Mask for finding phasing bit

   unsigned int single_bird_data_size = Flock::Output::getDataSize(mOutputFormat);
   if(Flock::Standalone != mMode)      // If we are in group mode, then it is one byte longer (the bird address for the sample)
   {
      single_bird_data_size += 1;
   }
   const unsigned int data_record_size(mNumSensors*single_bird_data_size);    // Size of the data record to read

   bool sample_succeeded;        // Flag for success. When false, there was an error so repeat.

   // counter to track the number of times a read doesn't complete in stream mode.  This counter is
   // used to intelligently back off the reporting rate if needed (baud rate not high enough is the normal cause)
   unsigned int num_stream_read_failures = 0;
   const unsigned int stream_read_failure_limit(5);

   // Try to send/read sample until it succeeds
   do
   {
      sample_succeeded = true;      // Default to success
      data_record.clear();

      // - Get a data record in whatever way we should for this mode
      // - Take into account phasing bits
      // - Process the data record
      try
      {
         // ------ POINT MODE ------ //
         if(FlockStandalone::RUNNING == mStatus)     // Must use point mode
         {
            sendCommand(Flock::Command::Point);       // Triggers a data record update

            // Read the reply data record
            bytes_remaining = data_record_size;                     // How many bytes do we have left to read

            while(bytes_remaining)        // While more left to read
            {
               try
               {
                  mSerialPort->read(temp_data_record, bytes_remaining,
                                    bytes_read, mReadTimeout);
                  // Append the temp data onto the end of the data record
                  data_record.insert(data_record.end(), temp_data_record.begin(),
                                     temp_data_record.end());
                  bytes_remaining -= bytes_read;
               }
               catch (vpr::WouldBlockException&)
               {
                  // Do nothing.
               }
               // If timeout or fail
               catch (vpr::IOException&)
               {
                  num_stream_read_failures++;
                  throw Flock::CommandFailureException(
                     "Did not read full data record in point mode.",
                     VPR_LOCATION
                  );
               }
            }
            vprASSERT(data_record.size() == data_record_size);            // Assert: We actually read the number of bytes we set out too
         }
         // ------- STREAMING MODE ------ //
         else if(FlockStandalone::STREAMING == mStatus)
         {
            // Check for read failure limit
            if (num_stream_read_failures > stream_read_failure_limit)
            {
               vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
                  << "[FlockStandalone::sample()] Stream reading failure "
                  << "limit hit.  Attempting to fix by decreasing report "
                  << "rate. (may need to increase baud setting.)\n"
                  << vprDEBUG_FLUSH;
               Flock::ReportRate report_rate = getReportRate();
               if(Flock::Every32 != report_rate)
               {
                  Flock::ReportRate new_rate =
                     Flock::ReportRate((unsigned int)report_rate+1);
                  setReportRate(new_rate);
               }
               else
               {
                  vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
                     << "[FlockStandalone::sample()] Report rate already at "
                     << "minimum.  Can't correct.\n" << vprDEBUG_FLUSH;
               }
               num_stream_read_failures = 0;       // Reset the count
            }

            // Read one byte at a time looking for phasing bit
            do
            {
               mSerialPort->read(&buffer, 1, bytes_read, mReadTimeout);
               if(1 != bytes_read)
               {
                  throw Flock::CommandFailureException(
                     "No response looking for first byte of streaming data",
                     VPR_LOCATION
                  );
               }
            }
            while(!(phase_mask & buffer));

            // Now read the rest of the record
            bytes_remaining = (data_record_size-1);                     // How many bytes do we have left to read

            while(bytes_remaining)        // While more left to read
            {
               try
               {
                  mSerialPort->read(temp_data_record, bytes_remaining,
                                    bytes_read, mReadTimeout);
                  // Append the temp data onto the end of the data record
                  data_record.insert(data_record.end(), temp_data_record.begin(),
                                     temp_data_record.end());
                  bytes_remaining -= bytes_read;
               }
               catch (vpr::WouldBlockException&)
               {
                  // Do nothing.
               }
               // If timeout or failed
               catch (vpr::IOException&)
               {
                  // TODO: setCause(ex)
                  num_stream_read_failures++;
                  throw Flock::CommandFailureException(
                     "Could not find entire streaming data record",
                     VPR_LOCATION
                  );
               }
            }
            data_record.insert(data_record.begin(), buffer);              // Add on the initial phase bit

            vprASSERT(data_record.size() == data_record_size);            // Assert: We actually read the number of bytes we set out too

            // Make sure the only phase bits the record are at the beginning
            // of a sensor record.
            for(unsigned int b=0;b<data_record.size();++b)
            {
               // If it is not the start of a sensor record, and it has a
               // phase bit.
               if((b%single_bird_data_size) && (phase_mask & data_record[b]))
               {
                  vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
                     << "[FlockStandalone::sample()] data out of phase, "
                     << "attempting to correct.\n"
                     << " byte: " << b << "  out of: " << data_record_size
                     << "\n" << vprDEBUG_FLUSH;
                  sample_succeeded = false;
                  num_stream_read_failures++;
                  break;                           // We have a failure, so quite checking the bits
               }
            }
         }
      }
      catch(Flock::CommandFailureException& cfe)
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "FlockStandalone::sample: Warning: exception:"
            << cfe.what() << std::endl << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "FlockStandalone::sample: Flushing queues to correct.\n"
            << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "FlockStandalone::sample: num_stream_read_failures:"
            << num_stream_read_failures << "\n" << vprDEBUG_FLUSH;
         vpr::System::msleep(750);     // Wait for data to arrive and clear
         mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
         sample_succeeded = false;
      }
   }
   while(!sample_succeeded);

   // Process the data record
   vprASSERT(data_record[0] & phase_mask);
   processDataRecord(data_record);
}

/** Stops the Flock. */
int FlockStandalone::close()
{
   int retval;
   if ( mSerialPort != NULL )
   {
      std::cout << " [FlockStandalone] Closing the flock ...\n" << std::flush;

      if (STREAMING == mStatus)
      {
         stopStreaming();
      }

      if(CLOSED != mStatus)
      {
         vpr::System::msleep(500);
         sendCommand(Flock::Command::Sleep);    // Put the flock to sleep
      }

      mSerialPort->close();

      // flock is not active now.
      mStatus = FlockStandalone::CLOSED;

      std::cout << " [FlockStandalone] Closed." << std::endl << std::flush;

      retval = 1;
   }
   else
   {
      retval = 0;
   }

   return retval;
}

/** Sets the flock into streaming mode.
 */
void FlockStandalone::startStreaming()
{
   // If port closed right now
   vprASSERT(mStatus == RUNNING && "Tried to start streaming before configuring flock");

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << " [FlockStandalone] Changing to streaming mode. \n"
      << "                   Setting report rate to: "
      << Flock::getReportRateString(mReportRate) << "\n" << vprDEBUG_FLUSH;

   sendReportRateCmd(mReportRate);

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << " [FlockStandalone] Starting stream.\n" << vprDEBUG_FLUSH;
   sendStreamStartCommand();

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << " [FlockStandalone] Streaming...\n" << vprDEBUG_FLUSH;

   // flock is streaming now
   mStatus = FlockStandalone::STREAMING;
}

/** Stops the streaming. */
void FlockStandalone::stopStreaming()
{
   vprASSERT(mStatus == STREAMING && "Tried to stop streaming we are not currently doing it");

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << " [FlockStandalone] Switching out of streaming mode.\n"
      << vprDEBUG_FLUSH;
   sendStreamStopCommand();
   mStatus = FlockStandalone::RUNNING;
}

/** Sets the port to use. */
void FlockStandalone::setPort(const std::string& serialPort)
{
   if ( FlockStandalone::CLOSED != mStatus )
   {
      throw Flock::CommandFailureException(
         "Cannot set port after flock has been opened.", VPR_LOCATION
      );
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

/** Sets the baud rate. */
void FlockStandalone::setBaudRate(const int& baud)
{
   if ( FlockStandalone::CLOSED != mStatus )
   {
      throw Flock::CommandFailureException(
         "Cannot set baud after flock has been opened.", VPR_LOCATION
      );
   }

   mBaud = baud;
}

/** Sets the hemisphere that the transmitter transmits from. */
void FlockStandalone::setHemisphere(const BIRD_HEMI& h)
{
   if ( (FlockStandalone::CLOSED != mStatus) &&
        (FlockStandalone::OPEN != mStatus) )
   {
      throw Flock::CommandFailureException(
         "Setting hemisphere not allowed after flock configured", VPR_LOCATION
      );
   }

   mHemisphere = h;
}

/** Sets the type of filtering that the Flock uses. */
void FlockStandalone::setFilterType(const BIRD_FILT& f)
{
   if ( (FlockStandalone::CLOSED != mStatus) &&
        (FlockStandalone::OPEN != mStatus) )
   {
      throw Flock::CommandFailureException(
         "Setting filter type not allowed after flock configured", VPR_LOCATION
      );
   }

   mFilter = f;
}

/** Sets the report rate that the Flock uses. */
// Note: Report rate only affects streaming mode and the start and stop
//       streaming actually set it so we don't need to send the command here.
//       We just rely upon start and stop streaming to do it.
void FlockStandalone::setReportRate(Flock::ReportRate rRate )
{
   bool was_streaming(false);
   if (FlockStandalone::STREAMING == mStatus)
   {
      was_streaming = true;
      stopStreaming();
   }

   mReportRate = rRate;
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << " [FlockStandalone] report rate to: "
      << Flock::getReportRateString(mReportRate) << "\n" << vprDEBUG_FLUSH;

   if(was_streaming)
   {
      startStreaming();
   }
}

void FlockStandalone::setOutputFormat(Flock::Output::Format format)
{
   bool was_streaming(false);
   if (FlockStandalone::STREAMING == mStatus)
   {
      was_streaming = true;
      stopStreaming();
   }

   mOutputFormat = format;

   if (FlockStandalone::RUNNING == mStatus)
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << " [FlockStandalone] Setting output format to: "
         << Flock::Output::getFormatString(mOutputFormat) << "\n"
         << vprDEBUG_FLUSH;
      vpr::System::msleep(50);
      sendOutputFormatCmd(mOutputFormat, true);
   }
   if(was_streaming)
   {
      startStreaming();
   }
}

/** Sets the number of birds to use in the Flock. */
void FlockStandalone::setNumSensors( const unsigned int& n )
{
   if ( (FlockStandalone::CLOSED != mStatus) && (FlockStandalone::OPEN != mStatus) )
   {
      throw Flock::CommandFailureException(
         "Setting number birds not allowed after flock configured",
         VPR_LOCATION
      );
   }

   mNumSensors = n;
}

/** Sets the video sync type. */
void FlockStandalone::setSync(const vpr::Uint8& sync)
{
   if ( (FlockStandalone::CLOSED != mStatus) && (FlockStandalone::OPEN != mStatus) )
   {
      throw Flock::CommandFailureException(
         "Sync command not allowed after flock configured", VPR_LOCATION
      );
   }

   mSyncStyle = sync;
}


/**
 * Processs a data record from the flock that contains one sensor record per
 * sensor.
 */
void FlockStandalone::processDataRecord(std::vector<vpr::Uint8> dataRecord)
{
   const vpr::Uint8 phase_mask(1<<7);     // Mask for finding phasing bit

   unsigned int single_bird_data_size = Flock::Output::getDataSize(mOutputFormat);
   if(Flock::Standalone != mMode)      // If we are in group mode, then it is one byte longer
   {
      single_bird_data_size += 1;
   }
   const unsigned int data_record_size(mNumSensors*single_bird_data_size);    // Size of the data record to read
   vprASSERT(dataRecord.size() == data_record_size);
   vprASSERT(dataRecord[0] & phase_mask);

   // For each sensor
   // - Get matrix from data format
   // - If not standalone find sensor address
   // - Store the sensor data
   vpr::Uint8 sensor = 0;
   for(unsigned int i=0; i<mNumSensors; ++i)
   {
      unsigned int data_offset = (single_bird_data_size*i);
      vprASSERT(dataRecord[data_offset] & phase_mask && "Unit record within data record does not have correct phase mask");
      gmtl::Matrix44f sensor_mat = processSensorRecord(&(dataRecord[data_offset]));
      int sensor_number(0);                                              // In standalone mode, only read into sensor 0
      if(Flock::Standalone != mMode)
      {
         // Get address from last byte in record for this unit
         // - Unit address *should* always just be sensor+1 since we are reading in order
         // XXX: There could be a bug here where we are getting the wrong group address
         vpr::Uint8 unit_addr = dataRecord[data_offset+(single_bird_data_size-1)];
         if(unit_addr != (sensor+1))
         {
            unit_addr = sensor+1;
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
               << "Got out of order unit data from group address byte.\n"
               << vprDEBUG_FLUSH;
         }
         vprASSERT(unit_addr <= mActiveUnitEndIndex);
         sensor_number = mAddrToSensorIdMap[unit_addr];
         vprASSERT(sensor_number != -1 && "Got addr of unit we don't think has sensor");
      }
      mSensorData[(unsigned int)sensor_number] = sensor_mat;
      ++sensor;
   }
}

gmtl::Matrix44f FlockStandalone::processSensorRecord(vpr::Uint8* buff)
{
   gmtl::Matrix44f ret_mat;
   float x,y,z,xr,yr,zr, q0,q1,q2,q3;

   const float max_angle(gmtl::Math::PI);    // 180 degree max

   switch(mOutputFormat)
   {
   case Flock::Output::Angle:
      zr = rawToFloat(buff[1], buff[0]) * max_angle;
      yr = rawToFloat(buff[3], buff[2]) * max_angle;
      xr = rawToFloat(buff[5], buff[4]) * max_angle;
      gmtl::setRot(ret_mat, gmtl::EulerAngleZYXf(zr,yr,xr));
      break;
   case Flock::Output::Matrix:
      {
      float m11, m12, m13,
            m21, m22, m23,
            m31, m32, m33;
      m11 = rawToFloat(buff[1], buff[0]);
      m21 = rawToFloat(buff[3], buff[2]);
      m31 = rawToFloat(buff[5], buff[4]);
      m12 = rawToFloat(buff[7], buff[6]);
      m22 = rawToFloat(buff[9], buff[8]);
      m32 = rawToFloat(buff[11], buff[10]);
      m13 = rawToFloat(buff[13], buff[12]);
      m23 = rawToFloat(buff[15], buff[14]);
      m33 = rawToFloat(buff[17], buff[16]);
      ret_mat.set( m11, m21, m31, 0.0f,
                   m12, m22, m32, 0.0f,
                   m13, m23, m33, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
      }
      break;
   case Flock::Output::Position:
      x = rawToFloat(buff[1], buff[0]) * mMaxPos;
      y = rawToFloat(buff[3], buff[2]) * mMaxPos;
      z = rawToFloat(buff[5], buff[4]) * mMaxPos;
      gmtl::setTrans(ret_mat, gmtl::Vec3f(x,y,z));
      break;
   case Flock::Output::PositionAngle:
      x = rawToFloat(buff[1], buff[0]) * mMaxPos;
      y = rawToFloat(buff[3], buff[2]) * mMaxPos;
      z = rawToFloat(buff[5], buff[4]) * mMaxPos;
      zr = rawToFloat(buff[7], buff[6])   * max_angle;
      yr = rawToFloat(buff[9], buff[8])   * max_angle;
      xr = rawToFloat(buff[11], buff[10]) * max_angle;
      gmtl::setRot(ret_mat, gmtl::EulerAngleZYXf(zr,yr,xr));
      gmtl::setTrans(ret_mat, gmtl::Vec3f(x,y,z));
      break;
   case Flock::Output::PositionMatrix:
      {
      float m11, m12, m13,
            m21, m22, m23,
            m31, m32, m33;
      x = rawToFloat(buff[1], buff[0]) * mMaxPos;
      y = rawToFloat(buff[3], buff[2]) * mMaxPos;
      z = rawToFloat(buff[5], buff[4]) * mMaxPos;
      m11 = rawToFloat(buff[7], buff[6]);
      m21 = rawToFloat(buff[9], buff[8]);
      m31 = rawToFloat(buff[11], buff[10]);
      m12 = rawToFloat(buff[13], buff[12]);
      m22 = rawToFloat(buff[15], buff[14]);
      m32 = rawToFloat(buff[17], buff[16]);
      m13 = rawToFloat(buff[19], buff[18]);
      m23 = rawToFloat(buff[21], buff[20]);
      m33 = rawToFloat(buff[23], buff[22]);
      ret_mat.set( m11, m21, m31, x,
                   m12, m22, m32, y,
                   m13, m23, m33, z,
                   0.0f, 0.0f, 0.0f, 1.0f);
      }
      break;
   case Flock::Output::PositionQuaternion:
      x = rawToFloat(buff[1], buff[0]) * mMaxPos;
      y = rawToFloat(buff[3], buff[2]) * mMaxPos;
      z = rawToFloat(buff[5], buff[4]) * mMaxPos;
      q0 = rawToFloat(buff[7], buff[6]);
      q1 = rawToFloat(buff[9], buff[8]);
      q2 = rawToFloat(buff[11], buff[10]);
      q3 = rawToFloat(buff[13], buff[12]);
      gmtl::setRot(ret_mat, gmtl::Quatf(-q1,-q2,-q3,q0));      // I don't fully understand why I need the sign flip
      gmtl::setTrans(ret_mat, gmtl::Vec3f(x,y,z));
      break;
   case Flock::Output::Quaternion:
      q0 = rawToFloat(buff[1], buff[0]);
      q1 = rawToFloat(buff[3], buff[2]);
      q2 = rawToFloat(buff[5], buff[4]);
      q3 = rawToFloat(buff[7], buff[6]);
      gmtl::setRot(ret_mat, gmtl::Quatf(-q1,-q2,-q3,q0));         // I don't fully understand why I need the sign flip
      break;
   }

   return ret_mat;
}

float FlockStandalone::rawToFloat(const vpr::Uint8& MSchar,
                                  const vpr::Uint8& LSchar)
{
   vpr::Int16 ival1,ival2,val;
   ival1 = MSchar & 0x7f;
   ival2 = LSchar & 0x7f;
   val = (ival1 << 9) | ival2<<2;
   return ((float)val) / 32768.0f;  // 32768=0x7fff

   /*  More slow and tedious way of doing the above.
   This is a more descriptive implementation as given pg.89 of Flock Manual

   vpr::Int8 MSbyte;
   vpr::Int8 LSbyte;
   vpr::Int16 returnVal;
   // 1) Changes 8th bit from a "1" back to "0" after catching it
   //    0x7f = 01111111  so it masks out the 8th but
   MSbyte = MSchar & 0x7f;
   LSbyte = LSchar & 0x7f;

   // 2) Shifts each LSByte left 1 bit
   LSbyte <<= 1;           // Shift the LS byte left 2 bits becasue flock data format

   // 3) Combine each MSbytee/LSbyte pair
   returnVal = MSbyte;
   returnVal <<= 8;
   returnVal += LSbyte;

   // 4) Shift each word pair left 1 more bit
   returnVal <<= 1;

   return( ( (float)returnVal ) / 0x7fff );
   */
}

// ------------------------------------------------------------------------- //
//                                QUERY METHODS                              //
// ------------------------------------------------------------------------- //

/** Gets the software revision on the bird. */
std::pair<unsigned int,unsigned int> FlockStandalone::querySoftwareRevision()
{
   std::vector<vpr::Uint8> resp(2);
   getAttribute(Flock::Parameter::SoftwareRevision, 2, resp);
   std::pair<unsigned int,unsigned int> sw_rev;
   sw_rev.first = resp[0];
   sw_rev.second = resp[1];
   return sw_rev;
}

/** Gets the model id string. */
std::string FlockStandalone::queryModelIdString()
{
   std::vector<vpr::Uint8> resp(10);
   getAttribute(Flock::Parameter::ModelIdentification, 10, resp);
   std::string model_id = "";
   for(unsigned int i=0;i<10;++i)
   {
      model_id += char(resp[i]);
   }

   return model_id;
}

/** Gets the model id string.
 *
 * NOTE: This requires firmware version 3.67
 */

Flock::AddressingMode FlockStandalone::queryAddressingMode()
{
   std::vector<vpr::Uint8> resp(1);
   getAttribute(Flock::Parameter::AddressingMode, 1, resp);
   Flock::AddressingMode amode = Flock::AddressingMode(resp[0]);
   return amode;
}

/**
 * NOTE: This requires firmware version 3.67
 */
vpr::Uint8 FlockStandalone::queryAddress()
{
   std::vector<vpr::Uint8> resp(1);
   getAttribute(Flock::Parameter::FbbAddress, 1, resp);
   vpr::Uint8 addr(resp[0]);
   return addr;
}

/** Gets status of the bird. */
vpr::Uint16 FlockStandalone::queryBirdStatus(unsigned int addr)
{
   if(0 != addr)
   {
      pickBird(addr);
   }

   std::vector<vpr::Uint8> resp(2);
   getAttribute(Flock::Parameter::BirdStatus, 2, resp);

   vpr::Uint16 msb(resp[1]), lsb(resp[0]), status;
   status = (msb<<8)|lsb;
   return status;
}

/** Gets status of the bird. */
std::vector<vpr::Uint8> FlockStandalone::querySystemStatus()
{
   vprASSERT(Flock::UnknownAddressing != mAddrMode && "No addressing mode set.  Has bird been opened?");

   std::vector<vpr::Uint8> sysStatus;

   // Find number of addresses that will be returned
   unsigned int num_addrs(14);
   if(Flock::ExpandedAddressing == mAddrMode)
   {
      num_addrs = 30;
   }
   if(Flock::SuperExpandedAddressing == mAddrMode)
   {
      num_addrs = 126;
   }

   sysStatus.clear();
   sysStatus.resize(num_addrs);
   getAttribute(Flock::Parameter::FlockSystemStatus, num_addrs, sysStatus);

   return sysStatus;
}

void FlockStandalone::printFlockStatus()
{
   // Print the info out
   std::cout << "Flock Status:\n";

   std::cout << "      status:" << getStatusString() << std::endl;
   std::cout << "        mode:" << Flock::getModeString(mMode) << std::endl;
   std::cout << "   addr mode:" << Flock::getAddressingModeString(mAddrMode) << std::endl;
   std::cout << "   mstr addr:" << mMasterAddr << std::endl;
   std::cout << "  output fmt:" << Flock::Output::getFormatString(mOutputFormat) << std::endl;
   std::cout << " report rate:" << Flock::getReportRateString(mReportRate) << std::endl;
   std::cout << "        hemi:" << getHemiString(mHemisphere) << std::endl;
   std::cout << "      filter:" << getFiltString(mFilter) << std::endl;
   std::cout << "        sync:" << int(mSyncStyle) << std::endl;
   std::cout << "       model:" << mModelId << std::endl;
   std::cout << "      sw ver:" << mSwRevision << std::endl;
   std::cout << "        port:" << mPort << std::endl;
   std::cout << "        baud:" << mBaud << std::endl;

   for(unsigned int i=0;i<mFlockUnits.size();++i)
   {
      const FlockUnit& unit = mFlockUnits[i];

      // build label
      std::string label("[");
      if(unit.hasTransmitter())
      {
         label += Transmitter::getTransmitterString(unit.mTransmitterType);
         if (unit.mHasSensor)
         {
            label += ", ";
         }
      }
      if(unit.mHasSensor)
      {
         label += "sensor";
      }
      label += "]";
#ifndef VPR_OS_IRIX
      std::cout << boost::format("  %s: %|-14|  master:%s accessible:%s  running:%s error:%s init:%s") % i
                                 % label % (bool) unit.mIsMaster
                                 % (bool) unit.mAccessible
                                 % (bool) unit.mIsRunning
                                 % (bool) unit.mHasError
                                 % (bool) unit.mHasBeenInitialized
                << std::endl;
#else
      std::cout << label << ": "
                << " master: " << (bool) unit.mIsMaster
                << " asssessible: " << (bool) unit.mAccessible
                << " running: " << (bool) unit.mIsRunning
                << " error: " <<  (bool) unit.mHasError
                << " init: " << (bool) unit.mHasBeenInitialized
                << std::endl;

#endif
   }

   std::ostringstream x_indices_oss;
   std::copy(mXmitterIndices.begin(), mXmitterIndices.end(),
             std::ostream_iterator<int>(x_indices_oss, ","));
   std::cout << "Transmitters at indices: " << x_indices_oss.str() << std::endl;

   std::cout << "--- Bird Internal ---:" << std::endl;
   std::cout << " mActiveUnitEndIndex:" << mActiveUnitEndIndex << std::endl;
   std::cout << "         mNumSensors:" << mNumSensors << std::endl;

   std::ostringstream s_indices_oss;
   std::copy(mAddrToSensorIdMap.begin(), mAddrToSensorIdMap.end(),
             std::ostream_iterator<int>(s_indices_oss, ","));
   std::cout << "Addr to sensor map: " << s_indices_oss.str() << std::endl;
}

unsigned int FlockStandalone::getMaxBirdAddr()
{
   if(Flock::NormalAddressing == mAddrMode)
   {
      return 14;
   }
   else if(Flock::ExpandedAddressing == mAddrMode)
   {
      return 30;
   }
   else if(Flock::SuperExpandedAddressing == mAddrMode)
   {
      return 127;
   }
   else
   {
      return 0;
   }
}

/** Send the group command.
 * @param newVal New state of the group status.
 */
void FlockStandalone::sendGroupCmd(bool newVal)
{
   std::vector<vpr::Uint8> param;
   param.push_back(vpr::Uint8(newVal));
   setAttribute(Flock::Parameter::GroupMode, param);
}

/** Send the sync command.
 * @param syncMethod The sync method to use.
 */
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
         params[0] = 0x00;
         params[1] = 0x00;
         break;
      case AFT_HEM:
         params[0] = 0x00;
         params[1] = 0x01;
         break;
      case UPPER_HEM:
         params[0] = 0x0C;
         params[1] = 0x01;
         break;
      case LOWER_HEM:
         params[0] = 0x0C;
         params[1] = 0x00;
         break;
      case LEFT_HEM:
         params[0] = 0x06;
         params[1] = 0x01;
         break;
      case RIGHT_HEM:
         params[0] = 0x06;
         params[1] = 0x00;
         break;
   }

   if(sendToAll)
   {
      sendCommandAll(Flock::Command::Hemisphere, params, true);      // Send to all but ercs
   }
   else
   {
      sendCommand(Flock::Command::Hemisphere, params);               // Send to all units
   }
}

void FlockStandalone::sendOutputFormatCmd(Flock::Output::Format format,
                                          bool sendToAll)
{
   std::vector<vpr::Uint8> param(0);
   vpr::Uint8 format_cmd = (Flock::Command::Position-1) + vpr::Uint8(format);  // Exploit a nice property of the format enum

   // Assert two standard cases to make sure above statement works
   //vprASSERT((Flock::Output::PositionAngle == format) && (format_cmd == Flock::Command::PositionAngles));
   //vprASSERT((Flock::Output::PositionQuaternion == format) && (format_cmd == Flock::Command::PositionQuaternion));

   if(sendToAll)
   {
      sendCommandAll(format_cmd, param, true);     // Send to all but erc
   }
   else
   {
      sendCommand(format_cmd, param);              // Send to all units
   }
}

/** Sends the sync command.
 * @param syncMethod The sync method to use.
 */
void FlockStandalone::sendReportRateCmd(Flock::ReportRate rate)
{
   vpr::Uint8 report_rate_cmd = vpr::Uint8(rate);

   // Assert above assignment works
   //vprASSERT((Flock::MaxRate == rate) && (report_rate_cmd == Flock::Command::ReportRate1));

   sendCommand(report_rate_cmd);
}

void FlockStandalone::sendStreamStartCommand()
{
   sendCommand(Flock::Command::Stream);
}

void FlockStandalone::sendStreamStopCommand()
{
   sendCommand(Flock::Command::StreamStop);
   vpr::System::msleep(500);                                // Wait for all streaming data to stop
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);    // Then clear the buffers
}

/*
void FlockStandalone::sendFilter()
{
   std::vector<vpr::Uint8> param(2);
   param[0] = 0x00;  // New filter state; This would turn on all filters
   param[1] = 0;     // Ignored
   setAttribute(Flock::Parameter::FilterStatus, param);
}
*/

/**
 * @param addr               FBB address of the transmitter.
 * @param transmitterNumber  The transmitter number.
 */
void FlockStandalone::sendNextTransmitterCmd(vpr::Uint8 addr,
                                             vpr::Uint8 transmitterNumber)
{
   std::vector<vpr::Uint8> param(1);
   vprASSERT(transmitterNumber <= 3);
   vprASSERT(addr <= 14);

   param[0] = (addr << 4) & transmitterNumber;

   sendCommand(Flock::Command::NextTransmitter, param);

   vpr::System::msleep(300);
}

/** Autoconfigures the system.
 * @param numUnits The number of units in the flock.
 */
void FlockStandalone::sendAutoconfigCmd(vpr::Uint8 numUnits)
{
   // Must wait 600 milliseconds before and after this command
   std::vector<vpr::Uint8> params;
   params.push_back(numUnits);

   vpr::System::msleep(1000);
   setAttribute(Flock::Parameter::FbbAutoConfig,params);
   vpr::System::msleep(1500);
}

/** Send command.
 * @param cmd The command to send.
 */
void FlockStandalone::sendCommand(vpr::Uint8 cmd, std::vector<vpr::Uint8> data )
{
   if ( NULL == mSerialPort )
   {
      throw Flock::ConnectionException("NULL port", VPR_LOCATION);
   }

   unsigned int bytes_written;
   
   try
   {
      mSerialPort->write(&cmd, 1, bytes_written);
   }
   catch (vpr::IOException&)
   {
      // TODO: setCause(ex)
      throw Flock::CommandFailureException("Failed to write full command",
                                           VPR_LOCATION);
   }

   if(!data.empty())
   {
      try
      {
         mSerialPort->write(&(data[0]), data.size(), bytes_written);
      }
      catch (vpr::IOException&)
      {
         // TODO: setCause(ex)
         throw Flock::CommandFailureException(
            "Failed to write full command args", VPR_LOCATION
         );
      }
   }
}

void FlockStandalone::pickBird (const vpr::Uint8 birdID)
{
   vpr::Uint8 cmd;
   std::vector<vpr::Uint8> cmd_data(0);      // Default to no data

   if(Flock::NormalAddressing == mAddrMode)
   {
      vprASSERT(birdID < 16);
      cmd = Flock::Command::ToFbbNormal + birdID;
   }
   else if(Flock::ExpandedAddressing == mAddrMode)
   {
      vprASSERT(birdID < 31);
      if(birdID < 16)
      {
         cmd = Flock::Command::ToFbbNormal + birdID;
      }
      else
      {
         cmd = Flock::Command::ToFbbExpanded + birdID - 0x10;
      }
   }
   else if(Flock::SuperExpandedAddressing == mAddrMode)
   {
      cmd = Flock::Command::ToFbbSuperExpanded;
      cmd_data.push_back(birdID);
   }

   //std::cout << "Picking bird: " << (int)birdID << std::endl;

   sendCommand(cmd, cmd_data);
}

/** Sends command to all units (except excluded types).
 */
void FlockStandalone::sendCommandAll(vpr::Uint8 cmd,
                                     std::vector<vpr::Uint8> data,
                                     bool excludeErc)
{
   for(unsigned int u=0;u<mActiveUnitEndIndex;++u)
   {
      const FlockUnit& unit = mFlockUnits[u];
      if(excludeErc && Transmitter::isErt(unit.mTransmitterType))
      {
         continue;
      }

      // Send command after picking bird
      pickBird(unit.mAddr);
      sendCommand(cmd,data);
   }
}

/**
 * Examines an attribute.
 *
 * @param attrib   Attribute to query for - see the Flock manual.
 * @param respSize Expected size of the response.
 * @param respData Returned data.
 */
void FlockStandalone::getAttribute(vpr::Uint8 attrib, unsigned int respSize,
                                   std::vector<vpr::Uint8>& respData)
{
   vpr::Uint8 exam_cmd[2];       // The command to send for the examine
   exam_cmd[0] = Flock::Command::ExamineValue;
   exam_cmd[1] = attrib;
   vpr::Uint32 bytes_written;
   vpr::Uint32 bytes_read;

   if ( NULL == mSerialPort )
   {
      throw Flock::ConnectionException("NULL port", VPR_LOCATION);
   }

   //vpr::System::msleep(200);                                   // Wait for any random input
   vpr::System::msleep(50);
   try
   {
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);       // Clear the buffers
   }
   catch (vpr::IOException&)
   {
      throw Flock::CommandFailureException(
         "Failed to flush queue before command", VPR_LOCATION
      );
   }
   //vpr::System::msleep(300);                                   // Let the buffers clear
   //vpr::System::msleep(50);         // Let the buffers clear.  Don't know if this is needed

   // Send command
   mSerialPort->write(exam_cmd, 2, bytes_written);
   vprASSERT(2 == bytes_written);
   mSerialPort->drainOutput();

   if(bytes_written != sizeof(exam_cmd))
   {
      throw Flock::CommandFailureException("Full command not written",
                                           VPR_LOCATION);
   }

   //vpr::System::msleep(500);

   // Read response and then flush the port to make sure we don't leave
   // anything extra.
   mSerialPort->readn(respData, respSize, bytes_read);
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

   // Check response size
   if(bytes_read != respSize)
   {
      throw Flock::CommandFailureException("Incomplete command response",
                                           VPR_LOCATION);
   }
}

/**
 * Change an attribute.
 *
 * @param attrib   Attribute to set - see the Flock manual.
 * @param respData Argument data.
 */
void FlockStandalone::setAttribute(vpr::Uint8 attrib,
                                   std::vector<vpr::Uint8>& attribData)
{
   vpr::Uint8 change_cmd[2];                    // The command to send for the change
   change_cmd[0] = Flock::Command::ChangeValue;
   change_cmd[1] = attrib;
   vpr::Uint32 bytes_written;

   if ( NULL == mSerialPort )
   {
      throw Flock::ConnectionException("NULL port", VPR_LOCATION);
   }

   //vpr::System::msleep(200);                                   // Wait for any random input
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);       // Clear the buffers
   //vpr::System::msleep(300);                                   // Let the buffers clear

   // Send command
   mSerialPort->write(change_cmd, 2, bytes_written);
   if(bytes_written != sizeof(change_cmd))
   {
      throw Flock::CommandFailureException("Full command not written",
                                           VPR_LOCATION);
   }

   // Send args
   if(!attribData.empty())
   {
      mSerialPort->write(attribData, attribData.size(), bytes_written);
      if(bytes_written != attribData.size())
      {
         throw Flock::CommandFailureException(
            "Change command args not fully written", VPR_LOCATION
         );
      }
   }
   mSerialPort->drainOutput();
}

vpr::Uint8 FlockStandalone::queryErrorCode()
{
   std::vector<vpr::Uint8> resp(1);
   getAttribute(Flock::Parameter::BirdErrorCode, 1, resp);
   return resp[0];
}

std::pair<vpr::Uint8,vpr::Uint8> FlockStandalone::queryExpandedErrorCode()
{
   std::vector<vpr::Uint8> resp(2);
   getAttribute(Flock::Parameter::BirdExpandedErrorCode, 2, resp);
   return std::make_pair(resp[0], resp[1]);
}

vpr::Uint8 FlockStandalone::queryPositionScaleFactor()
{
   std::vector<vpr::Uint8> resp(2);
   vpr::Uint8 scale_factor;

   if ( Flock::ExtendedRange == mMode )
   {
      scale_factor = 144;
   }
   else
   {
      // Find a non-ERT sensor to query the scaling factor.
      for(unsigned int u=0;u<mActiveUnitEndIndex;++u)
      {
         const FlockUnit& unit = mFlockUnits[u];
         if( Transmitter::isErt(unit.mTransmitterType) )
         {
            continue;
         }

         // Send command after picking bird
         pickBird(unit.mAddr);
         getAttribute(Flock::Parameter::PositionScaling, 2, resp);
         break;
      }

      scale_factor = (resp[1] == 0 ? 36 : 72);
   }

   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "Scaling factor from the Flock is " << (unsigned int) scale_factor
      << std::endl << vprDEBUG_FLUSH;

   return scale_factor;
}

void FlockStandalone::setErrorModeIgnore()
{
   std::vector<vpr::Uint8> param(1);
   param[0] = 0x1;
   setAttribute(Flock::Parameter::BirdErrorMask, param);
}

void FlockStandalone::setErrorModeStandard()
{
   std::vector<vpr::Uint8> param(1);
   param[0] = 0x0;
   setAttribute(Flock::Parameter::BirdErrorMask, param);
}

/* Check for error, if there are errors, print out error messages for all
 * errors and throw exception for last error.
 */
void FlockStandalone::checkError()
{
   std::pair<vpr::Uint8,vpr::Uint8> errs = queryExpandedErrorCode();
   std::pair<vpr::Uint8,vpr::Uint8> prev_err;
   bool have_error_to_throw(false);

   while(errs.first != 0)
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
         << "ERROR: [FlockStandalone] Bird error: [" << int(errs.first) << "] "
         << Flock::getErrorDescription(errs.first, errs.second) << std::endl
         << vprDEBUG_FLUSH;

      prev_err = errs;
      have_error_to_throw = true;
      errs = queryExpandedErrorCode();
   }

   if(have_error_to_throw)
   {
      throw Flock::BirdErrorException(prev_err.first, prev_err.second,
                                      VPR_LOCATION);
   }
}

void FlockStandalone::readInitialFlockConfiguration()
{
   std::pair<unsigned int,unsigned int> sw_rev;

   vprASSERT(( mSerialPort != NULL ) && (FlockStandalone::CLOSED != mStatus));
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "Asking the flock for its configuration.\n" << vprDEBUG_FLUSH;

   try
   {
      // Software revision
      sw_rev = querySoftwareRevision();
      mSwRevision = float(sw_rev.first) + (float(sw_rev.second)/100.0f);
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "      sw ver: " << mSwRevision << std::endl << vprDEBUG_FLUSH;
      //vprASSERT(mSwRevision >= 2.67f && "This driver only works with fob sw version 3.67 or higher");

      // Get the model id
      mModelId = queryModelIdString();
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "    model id: " << mModelId << std::endl << vprDEBUG_FLUSH;

      if(mSwRevision < 3.67f)
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
            << "Flock firmware older than 3.67: Using configured addressing mode "
            << Flock::getAddressingModeString(mAddrMode) << " and master address "
            << mMasterAddr << std::endl << vprDEBUG_FLUSH;
//         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
//            << "   NOTE: This means that standalone mode is NOT supported.\n"
//            << vprDEBUG_FLUSH;
      }
      else
      {
         // NOTE: Both queryAddressingMode() and queryAddress() require firmware version 3.67

         // Get the addressing mode
         mAddrMode = queryAddressingMode();
         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "   addr mode: "
            << Flock::getAddressingModeString(mAddrMode) << std::endl
            << vprDEBUG_FLUSH;
         // Get the address of the master
         mMasterAddr = queryAddress();
      }

      // Make initial guess at mode (finalize later after looking for erts)
      if(0 == mMasterAddr)
      {
         mMode = Flock::Standalone;
      }
      else
      {
         mMode = Flock::Standard;
      }

      // Setup the flock unit data structure
      // - Queries all birds in flock for data and then sets up extended data
      //   per bird
      setupFlockUnitsDataStructure();

      mNumSensors = 0;
      mAddrToSensorIdMap.clear();
      mAddrToSensorIdMap.resize(getMaxBirdAddr(), -1);      // Fill with -1

      // Update status variables: mMode, mXmitterIndices, mNumSensors
      // - Search for transmitters
      //    - If we have erts set to extended range mode
      //    - Store indicies of the transmitters
      // - Find number of sensors in system
      for(unsigned int u=0;u<mActiveUnitEndIndex;++u)
      {
         const FlockUnit& unit = mFlockUnits[u];
         if(unit.mTransmitterType == Transmitter::Ert0)
         {
            mMode = Flock::ExtendedRange;
         }
         if(unit.hasTransmitter())
         {
            mXmitterIndices.push_back(u);
         }
         if(unit.mHasSensor)
         {
            mAddrToSensorIdMap[unit.mAddr] = mNumSensors;     // Map from unit address to the sensor index
            mNumSensors += 1;
         }
      }

      // This driver will return all values in feet.
      mMaxPos = ((float) queryPositionScaleFactor()) / 12.0f;

      if(mMode == Flock::Standalone)
      {
         vprASSERT(1 == mNumSensors);
         vprASSERT(mAddrToSensorIdMap[0] == 0);
         vprASSERT((mXmitterIndices.size() == 1) && (mXmitterIndices[0] == 0));
      }

   }
   catch(Flock::FlockException& e)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Flock error: " << e.what() << std::endl << vprDEBUG_FLUSH;
   }
}

void FlockStandalone::setupFlockUnitsDataStructure()
{
   unsigned int max_units = getMaxBirdAddr();
   mFlockUnits.resize(max_units);   // Allocate enough room

   // --- Get data from system status command --- //
   std::vector<vpr::Uint8> sys_status = querySystemStatus();

   for(unsigned int i=0;i<sys_status.size();++i)
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
   if(Flock::Standalone == mMode)
   {
      mFlockUnits[0].mAddr = 0;     // Manually set the correct bird address
      mActiveUnitEndIndex = 1;      // Manually position it past first unit

      // XXX: For now manually setup sensor info to hack around flock system status issues
      mFlockUnits[0].mAccessible = true;
      mFlockUnits[0].mIsRunning = true;
      mFlockUnits[0].mHasSensor = true;
      mFlockUnits[0].mTransmitterType = Transmitter::Standard;
   }
   else
   {
      // Find the last active address (last one marked accessible)
      mActiveUnitEndIndex = 0;
      flock_units_t::iterator cur_unit = mFlockUnits.begin();
      // While(not at end && unit is accessible)
      // - Add onto the active unit max
      while( (cur_unit != mFlockUnits.end()) && ((*cur_unit).mAccessible))
      {
         mActiveUnitEndIndex += 1; cur_unit++;
      }

      vprASSERT(mActiveUnitEndIndex <= mFlockUnits.size());
      vprASSERT((mActiveUnitEndIndex == 0) ||
                (mFlockUnits[mActiveUnitEndIndex-1].mAccessible) );
   }

   // Query each bird individually for more information
   for(unsigned int i=0;i<mActiveUnitEndIndex;++i)
   {
      FlockUnit& unit = mFlockUnits[i];
      vpr::Uint16 status = queryBirdStatus(unit.mAddr);
      unit.mIsMaster = Flock::BirdStatus::isMaster(status);
      unit.mHasBeenInitialized = Flock::BirdStatus::isInititalized(status);
      unit.mHasError = Flock::BirdStatus::hasError(status);
   }
}
