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
#include <boost/lexical_cast.hpp>
#include <iterator>

#include <drivers/Ascension/Flock/FlockStandalone.h>

#include <gmtl/Generate.h>
#include <gmtl/Vec.h>
#include <gmtl/Quat.h>
#include <gmtl/MatrixOps.h>

namespace Flock
{
   std::string getErrorDescription( vpr::Uint8 errCode, vpr::Uint8 expandedErr )
   {
   #define F_ERR(x,y)  case x: ret_val = y; break;
   
      std::string ret_val;
      switch(errCode)
      {
         F_ERR(0x0, "No Errors Have Occurred");    
         F_ERR(0x1, "System RAM Failure");    
         F_ERR(0x2, "Non-Volatile Storage Write Failure");    
         F_ERR(0x3, "PCB EEPROM Configuration Corrupt");    
         F_ERR(0x4, "Transmitter EEPROM Configuration Data Corrupt");    
         F_ERR(0x5, "Receiver EEPROM Configuration Corrupt");    
         F_ERR(0x6, "Invalid RS232 Command");    
         F_ERR(0x7, "Not an FBB Master");    
         F_ERR(0x8, "No 6DFOBs are Active");    
         F_ERR(0x9, "6DFOB has not been Initialized");    
         F_ERR(0x10, "FBB Receive Error - Intra Bird Bus");    
         F_ERR(0x11, "RS232 Overrun and/or Framing Error");    
         F_ERR(0x12, "FBB Receive Error - FBB Host Bus");    
         case 0x13:
            ret_val = "No FBB Command Response from device at address" + boost::lexical_cast<std::string>(expandedErr);
            break;
         F_ERR(0x14, "Invalid FBB Host Command");    
         F_ERR(0x15, "FBB Run Time Error");    
         F_ERR(0x16, "Invalid CPU Speed");    
         F_ERR(0x17, "Slave No Data Error");    
         F_ERR(0x18, "Illegal Baud Rate");    
         F_ERR(0x19, "Slave Acknowledge Error");    
         F_ERR(0x20, "CPU Overflow Error - call factory");    
         F_ERR(0x21, "Array Bounds Error - call factory");    
         F_ERR(0x22, "Unused Opcode Error - call factory");    
         F_ERR(0x23, "Escape Opcode Error - call factory");    
         F_ERR(0x24, "Reserved Int 9 - call factory");    
         F_ERR(0x25, "Reserved Int 10 - call factory");    
         F_ERR(0x26, "Reserved Int 11 - call factory");    
         F_ERR(0x27, "Numeric CPU Error - call factory");    
         F_ERR(0x28, "CRT Syncronization Error");    
         F_ERR(0x29, "Transmitter Not Active Error");    
         F_ERR(0x30, "ERC Extended Range Transmitter Not Attached Error");    
         F_ERR(0x31, "CPU Time Overflow Error");    
         F_ERR(0x32, "Receiver Saturated Error");    
         F_ERR(0x33, "Slave Configuration Error");    
         F_ERR(0x34, "ERC Watchdog Error");    
         F_ERR(0x35, "ERC Overtemp Error");    
         default:
            ret_val = "Uknown error.";
            break;
      }
   #undef F_ERR
   
      return ret_val;
   }
}  // end namespace flock


/** Configure constructor. */
FlockStandalone::FlockStandalone(std::string port, const unsigned& numBrds,
                                 const int& transmit, const int& baud,
                                 const int& sync, const BIRD_HEMI& hemi,
                                 const BIRD_FILT& filt, Flock::ReportRate report)
   : mStatus(FlockStandalone::CLOSED),
    mPort(port),
    mSerialPort(NULL),
    mBaud(baud),
    mMode(Flock::UnknownMode),
    mAddrMode(Flock::UnknownAddressing),
    mSwRevision(0.0f),
    mNumSensors(numBrds),
    mXmitterUnitNumber(transmit),
    mReportRate(report),
    mHemisphere(hemi),
    mFilter(filt),
    mSyncStyle(sync)
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
//  give - a serial port
//  give - a baud rate
//  returns portId twice (created by the open function)
//  NOTE: portId is returned from both ends of this function.
//  if portId == -1 then function failed to open the port.
vpr::ReturnStatus FlockStandalone::open ()
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
   mSerialPort->setBlocking(true);              // Open in blocking mode

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


/** Configure the flock and get it ready to go
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

   // Find address of transmitter to use
   vpr::Uint8 transmitter_addr(0);
   if(!mXmitterIndices.empty())
   {  transmitter_addr = mFlockUnits[mXmitterIndices[0]].mAddr; }

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting transmitter to addr: "
                                          << transmitter_addr << "\n" << vprDEBUG_FLUSH;
   sendNextTransmitterCmd(transmitter_addr, 0);
   //sendFilter();

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Setting output format to: " << Flock::Output::getFormatString(mOutputFormat) << "\n" << vprDEBUG_FLUSH;
   sendOutputFormatCmd(mOutputFormat,true);

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Flock configured and ready to run." << vprDEBUG_FLUSH;

   // flock is active.
   mStatus = FlockStandalone::RUNNING;

   // return success
   return vpr::ReturnStatus::Succeed;
}

/** Call this repeatedly to update the data from the birds. */
void FlockStandalone::sample()
{
   // can't sample when not streaming
   vprASSERT( (STREAMING == mStatus) || (RUNNING == mStatus) );

   std::vector<vpr::Uint8> data_record;
   vpr::Uint32 bytes_read;
   const vpr::Uint8 phase_mask(1<<7);     // Mask for finding phasing bit

   unsigned single_bird_data_size = Flock::Output::getDataSize(mOutputFormat);
   if(Flock::Standalone != mMode)      // If we are in group mode, then it is one byte longer
   { 
      single_bird_data_size += 1; 
   }
   const unsigned data_record_size(mNumSensors*single_bird_data_size);    // Size of the data record to read

   // - Get a data record in whatever way we should for this mode
   // - Take into account phasing bits
   // - Process the data record
   if(FlockStandalone::RUNNING == mStatus)     // Must use point mode
   {
      mSerialPort->drainOutput();
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);       // Clear the buffers
      sendCommand(Flock::Command::Point);       // Triggers a data record update      

      mSerialPort->read(data_record, data_record_size, bytes_read, mReadTimeout);
      if(data_record_size != bytes_read)
      { throw Flock::CommandFailureException("Did not read full data record in point mode."); }      
   }
   else if(FlockStandalone::STREAMING == mStatus)
   {
      // Look for phasing bit
      vpr::Uint8 buffer;
            
      // Read one byte at a time looking for phasing bit
      // Do while we don't have phasing bit
      do
      {
         mSerialPort->read(&buffer, 1, bytes_read, mReadTimeout);
         if(1 != bytes_read)
         { throw Flock::CommandFailureException("No response looking for first byte of streaming data"); }
      }
      while(!(phase_mask & buffer));
      
      vprASSERT(phase_mask & buffer);

      // Now read the rest of the record
      mSerialPort->read(data_record, data_record_size-1, bytes_read, mReadTimeout);
      if(data_record_size-1 != bytes_read)
      { throw Flock::CommandFailureException("Could not find entire streaming data record"); }

      // Check to make sure there are no other phase bits in the data record
      for(unsigned b=0;b<data_record.size();++b)
      {  vprASSERT(!(phase_mask & data_record[b])); }

      data_record.insert(data_record.begin(), buffer);      
   }

   // Process the data record
   vprASSERT(data_record[0] & phase_mask);
   processDataRecord(data_record);
}

/** Stops the Flock. */
int FlockStandalone::close()
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

/** Set the flock into streaming mode
*/
vpr::ReturnStatus FlockStandalone::startStreaming()
{
   // If port closed right now
   vprASSERT(mStatus == RUNNING && "Tried to start streaming before configuring flock");

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Changing to streaming mode. \n"
                                          << "                   Setting report rate to: " << Flock::getReportRateString(mReportRate) << "\n" << vprDEBUG_FLUSH;

   sendReportRateCmd(mReportRate);

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Starting stream." << vprDEBUG_FLUSH;
   sendStreamStartCommand();

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Streaming..." << vprDEBUG_FLUSH;

   // flock is streaming now
   mStatus = FlockStandalone::STREAMING;

   return vpr::ReturnStatus::Succeed;
}

/** Stop the streaming */
vpr::ReturnStatus FlockStandalone::stopStreaming()
{
   vprASSERT(mStatus == STREAMING && "Tried to stop streaming we are not currently doing it");

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << " [FlockStandalone] Switching out of streaming mode. \n"
                                          << vprDEBUG_FLUSH;
   sendStreamStopCommand();
   mStatus = FlockStandalone::RUNNING;
   
   return vpr::ReturnStatus::Succeed;
}


/** Sets the port to use. */
void FlockStandalone::setPort(const std::string& serialPort)
{
   if ( FlockStandalone::CLOSED != mStatus )
   {
      throw Flock::CommandFailureException("Cannot set port after flock has been opened.");
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
      throw Flock::CommandFailureException("Cannot set baud after flock has been opened.");
   }

   mBaud = baud;
}


/** Sets the hemisphere that the transmitter transmits from. */
void FlockStandalone::setHemisphere( const BIRD_HEMI& h )
{
   if ( (FlockStandalone::CLOSED != mStatus) && (FlockStandalone::OPEN != mStatus) )
   {
      throw Flock::CommandFailureException("Setting hemisphere not allowed after flock configured");
   }

   mHemisphere = h;
}

/** Sets the type of filtering that the Flock uses. */
void FlockStandalone::setFilterType( const BIRD_FILT& f )
{
   if ( (FlockStandalone::CLOSED != mStatus) && (FlockStandalone::OPEN != mStatus) )
   {
      throw Flock::CommandFailureException("Setting filter type not allowed after flock configured");
   }

   mFilter = f;
}

/** Set the report rate that the Flock uses. */
void FlockStandalone::setReportRate(Flock::ReportRate rRate )
{
   if ( (FlockStandalone::CLOSED != mStatus) && (FlockStandalone::OPEN != mStatus) )
   {
      throw Flock::CommandFailureException("Setting report rate not allowed after flock configured");
   }

   mReportRate = rRate;
}

void FlockStandalone::setOutputFormat(Flock::Output::Format format)
{
   if ( (FlockStandalone::CLOSED != mStatus) && (FlockStandalone::OPEN != mStatus) )
   {
      throw Flock::CommandFailureException("Setting output format not allowed after flock configured");
   }

   mOutputFormat = format;
}


/** Sets the unit number of the transmitter. */
void FlockStandalone::setTransmitter( const int& Transmit )
{
   if ( (FlockStandalone::CLOSED != mStatus) && (FlockStandalone::OPEN != mStatus) )
   {
      throw Flock::CommandFailureException("Setting transmitter not allowed after flock configured");
   }

   mXmitterUnitNumber = Transmit;
}

/** Sets the number of birds to use in the Flock. */
void FlockStandalone::setNumSensors( const unsigned& n )
{
   if ( (FlockStandalone::CLOSED != mStatus) && (FlockStandalone::OPEN != mStatus) )
   {
      throw Flock::CommandFailureException("Setting number birds not allowed after flock configured");
   }

   mNumSensors = n;
}

/** Sets the video sync type. */
void FlockStandalone::setSync(const vpr::Uint8& sync)
{
   if ( (FlockStandalone::CLOSED != mStatus) && (FlockStandalone::OPEN != mStatus) )
   {
      throw Flock::CommandFailureException("Sync command not allowed after flock configured");
   }

   mSyncStyle = sync;
}


void FlockStandalone::processDataRecord(std::vector<vpr::Uint8> dataRecord)
{
   const vpr::Uint8 phase_mask(1<<7);     // Mask for finding phasing bit

   unsigned single_bird_data_size = Flock::Output::getDataSize(mOutputFormat);
   if(Flock::Standalone != mMode)      // If we are in group mode, then it is one byte longer
   { single_bird_data_size += 1; }
   const unsigned data_record_size(mNumSensors*single_bird_data_size);    // Size of the data record to read
   vprASSERT(dataRecord.size() == data_record_size);
   vprASSERT(dataRecord[0] & phase_mask);

   // For each sensor
   // - Get matrix from data format
   // - If not standalone find sensor address
   // - Store the sensor data
   for(vpr::Uint8 sensor=0; sensor<mNumSensors; ++sensor)
   {
      unsigned data_offset = (single_bird_data_size*sensor);
      gmtl::Matrix44f sensor_mat = processSensorRecord(&(dataRecord[data_offset]));
      unsigned sensor_number(0);
      if(Flock::Standalone != mMode)
      {
         // Get address from last byte in record for this unit
         vpr::Uint8 unit_addr = dataRecord[data_offset+(single_bird_data_size-1)];
         vprASSERT(unit_addr <= mActiveUnitEndIndex);
         sensor_number = mAddrToSensorIdMap[unit_addr];
      }
      mSensorData[sensor_number] = sensor_mat;
   }

}

gmtl::Matrix44f FlockStandalone::processSensorRecord(vpr::Uint8* buff)
{
   gmtl::Matrix44f ret_mat;
   float x,y,z,xr,yr,zr, q0,q1,q2,q3;

   const float max_angle(gmtl::Math::PI);    // 180 degree max
   float max_pos(3.0f);      // Max of 36 inches in default standard mode
   if(Flock::ExtendedRange == mMode)
   {
      max_pos = 12.0f;       // Max of 144 inches in extended range mode
   }
      
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
      ret_mat.set( m11, m12, m13, 0.0f,
                   m21, m22, m23, 0.0f,
                   m31, m32, m33, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
      }
      break;
   case Flock::Output::Position:
      x = rawToFloat(buff[1], buff[0]) * max_pos;
      y = rawToFloat(buff[3], buff[2]) * max_pos;
      z = rawToFloat(buff[5], buff[4]) * max_pos;
      gmtl::setTrans(ret_mat, gmtl::Vec3f(x,y,z));
      break;
   case Flock::Output::PositionAngle:
      x = rawToFloat(buff[1], buff[0]) * max_pos;
      y = rawToFloat(buff[3], buff[2]) * max_pos;
      z = rawToFloat(buff[5], buff[4]) * max_pos;
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
      x = rawToFloat(buff[1], buff[0]) * max_pos;
      y = rawToFloat(buff[3], buff[2]) * max_pos;
      z = rawToFloat(buff[5], buff[4]) * max_pos;
      m11 = rawToFloat(buff[7], buff[6]);
      m21 = rawToFloat(buff[9], buff[8]);
      m31 = rawToFloat(buff[11], buff[10]);
      m12 = rawToFloat(buff[13], buff[12]);
      m22 = rawToFloat(buff[15], buff[14]);
      m32 = rawToFloat(buff[17], buff[16]);
      m13 = rawToFloat(buff[19], buff[18]);
      m23 = rawToFloat(buff[21], buff[20]);
      m33 = rawToFloat(buff[23], buff[22]);
      ret_mat.set( m11, m12, m13, x,
                   m21, m22, m23, y,
                   m31, m32, m33, z,
                   0.0f, 0.0f, 0.0f, 1.0f);
      }
      break;
   case Flock::Output::PositionQuaternion:
      x = rawToFloat(buff[1], buff[0]) * max_pos;
      y = rawToFloat(buff[3], buff[2]) * max_pos;
      z = rawToFloat(buff[5], buff[4]) * max_pos;
      q0 = rawToFloat(buff[7], buff[6]);
      q1 = rawToFloat(buff[9], buff[8]);
      q2 = rawToFloat(buff[11], buff[10]);
      q3 = rawToFloat(buff[13], buff[12]);
      gmtl::setRot(ret_mat, gmtl::Quatf(q0,q1,q2,q3));
      gmtl::setTrans(ret_mat, gmtl::Vec3f(x,y,z));
      break;
   case Flock::Output::Quaternion:
      q0 = rawToFloat(buff[1], buff[0]);
      q1 = rawToFloat(buff[3], buff[2]);
      q2 = rawToFloat(buff[5], buff[4]);
      q3 = rawToFloat(buff[7], buff[6]);
      gmtl::setRot(ret_mat, gmtl::Quatf(q0,q1,q2,q3));
      break;
   }

   return ret_mat;
}

float FlockStandalone::rawToFloat (const vpr::Uint8& MSchar, const vpr::Uint8& LSchar)
{
   // short int ival1,ival2,val;
   // ival1 = r1 & 0x7f;                  // Set 8th bit to 0
   // ival2 = r2 & 0x7f;                  // Set 8th bit to 0
   // val = (ival1 << 9) | ival2<<2;
   //return((float)val) / 0x7fff;

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

//   std::cout << std::endl << "------------------------------------------" << std::endl;
//   std::cout << "[vpr::Int16] Before DIV): " << returnVal  << std::endl;
//   std::cout << "[float]      Before DIV): " << (float)returnVal  << std::endl;
//   std::cout << "[float]      After DIV): " << ( (float)returnVal ) / 0x7fff  << std::endl;
//   std::cout << "------------------------------------------" << std::endl << std::endl;

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
Flock::AddressingMode FlockStandalone::queryAddressingMode()
{
   std::vector<vpr::Uint8> resp(1);
   getAttribute(Flock::Parameter::AddressingMode, 1, resp);
   Flock::AddressingMode amode = Flock::AddressingMode(resp[0]);
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
   vprASSERT(Flock::UnknownAddressing != mAddrMode && "No addressing mode set.  Has bird been opened?");

   std::vector<vpr::Uint8> sysStatus;

   // Find number of addresses that will be returned
   unsigned num_addrs(14);
   if(Flock::ExpandedAddressing == mAddrMode)
      num_addrs = 30;
   if(Flock::SuperExpandedAddressing == mAddrMode)
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

   std::ostringstream x_indices_oss;
   std::copy(mXmitterIndices.begin(), mXmitterIndices.end(), std::ostream_iterator<int>(x_indices_oss, ","));
   std::cout << "Transmitters at indices: " << x_indices_oss.str() << std::endl;
}

unsigned FlockStandalone::getMaxBirdAddr()
{
   if(Flock::NormalAddressing == mAddrMode) return 14;
   else if(Flock::ExpandedAddressing == mAddrMode) return 30;
   else if(Flock::SuperExpandedAddressing == mAddrMode) return 127;
   else return 0;
}

/** Send the group command
* @param newVal - New state of the group status
*/
void FlockStandalone::sendGroupCmd(bool newVal)
{
   std::vector<vpr::Uint8> param;
   param.push_back(vpr::Uint8(newVal));
   setAttribute(Flock::Parameter::GroupMode, param);
}

/** Send the sync command.
* @param syncMethod - The sync method to use
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
      sendCommandAll(Flock::Command::Hemisphere, params, true);      // Send to all but ercs
   }
   else
   {
      sendCommand(Flock::Command::Hemisphere, params);               // Send to all units
   }

}

void FlockStandalone::sendOutputFormatCmd(Flock::Output::Format format, bool sendToAll)
{
   std::vector<vpr::Uint8> param(0);
   vpr::Uint8 format_cmd = (Flock::Command::Position-1) + vpr::Uint8(format);  // Exploit a nice property of the format enum

   // Assert two standard cases to make sure above statement works
   vprASSERT((Flock::Output::PositionAngle == format) && (format_cmd == Flock::Command::PositionAngles));
   vprASSERT((Flock::Output::PositionQuaternion == format) && (format_cmd == Flock::Command::PositionQuaternion));

   if(sendToAll)
   {
      sendCommandAll(format_cmd, param, true);     // Send to all but erc
   }
   else
   {
      sendCommand(format_cmd, param);              // Send to all units
   }
}

/** Send the sync command.
* @param syncMethod - The sync method to use
*/
void FlockStandalone::sendReportRateCmd(Flock::ReportRate rate)
{
   vpr::Uint8 report_rate_cmd = vpr::Uint8(rate);

   // Assert above assignment works
   vprASSERT((Flock::MaxRate == rate) && (report_rate_cmd == Flock::Command::ReportRate1));

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
void FlockStandalone::sendFilter ()
{
   std::vector<vpr::Uint8> param(2);
   param[0] = 0x00;  // New filter state; This would turn on all filters
   param[1] = 0;     // Ignored

   setAttribute(Flock::Parameter::FilterStatus, param);
}
*/

/**
* @param addr - FBB address of the transmitter.
* @param transmitterNumber - The transmitter number
*/
void FlockStandalone::sendNextTransmitterCmd (vpr::Uint8 addr, vpr::Uint8 transmitterNumber)
{
   std::vector<vpr::Uint8> param(1);
   vprASSERT(transmitterNumber <= 3);
   vprASSERT(addr <= 14);

   param[0] = (addr << 4) & transmitterNumber;

   sendCommand(Flock::Command::NextTransmitter, param);

   vpr::System::msleep(300);
}

/** Autoconfig the system.
* @param numUnits - The number of units in the flock
*/
void FlockStandalone::sendAutoconfigCmd (vpr::Uint8 numUnits)
{
   // Must wait 600 milliseconds before and after this command
   std::vector<vpr::Uint8> params;
   params.push_back(numUnits);

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
   vpr::Uint8 cmd;
   std::vector<vpr::Uint8> cmd_data(0);      // Default to no data

   if(Flock::NormalAddressing == mAddrMode)
   {
      vprASSERT(birdID < 16);
      cmd = Flock::Command::ToFbbNormal + birdID;
   }
   if(Flock::ExpandedAddressing == mAddrMode)
   {
      vprASSERT(birdID < 31);
      if(birdID < 16)
         cmd = Flock::Command::ToFbbNormal + birdID;
      else
         cmd = Flock::Command::ToFbbExpanded + birdID - 0x10;
   }
   else if(Flock::SuperExpandedAddressing == mAddrMode)
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
   change_cmd[0] = Flock::Command::ChangeValue;
   change_cmd[1] = attrib;
   vpr::Uint32 bytes_written;

   if ( NULL == mSerialPort )
   { throw Flock::ConnectionException("NULL port"); }

   //vpr::System::msleep(200);                                   // Wait for any random input
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);       // Clear the buffers
   //vpr::System::msleep(300);                                   // Let the buffers clear

   // Send command
   mSerialPort->write(change_cmd, 2, bytes_written);
   if(bytes_written != sizeof(change_cmd))
   {  throw Flock::CommandFailureException("Full command not written"); }

   // Send args
   if(!attribData.empty())
   {
      mSerialPort->write(attribData, attribData.size(), bytes_written);
      if(bytes_written != attribData.size())
      {  throw Flock::CommandFailureException("Change command args not fully written"); }
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

/* Check for error, if there is, print out error message and throw exception */
void FlockStandalone::checkError()
{
   std::pair<vpr::Uint8,vpr::Uint8> errs = queryExpandedErrorCode();

   if(errs.first != 0)
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "ERROR: [FlockStandalone] Bird error: " 
                  << Flock::getErrorDescription(errs.first, errs.second) << std::endl << vprDEBUG_FLUSH;
      throw Flock::BirdErrorException(errs.first, errs.second);
   }         
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
      { mMode = Flock::Standalone; }
      else
      { mMode = Flock::Standard; }

      // Setup the flock unit data structure
      setupFlockUnitsDataStructure();

      mNumSensors = 0;
      mAddrToSensorIdMap.clear();
      mAddrToSensorIdMap.resize(getMaxBirdAddr());

      // Search for transmitters
      // - If we have erts set to extended range mode
      // - Store indicies of the transmitters
      // - Find number of sensors in system
      for(unsigned u=0;u<mActiveUnitEndIndex;++u)
      {
         const FlockUnit& unit = mFlockUnits[u];
         if(unit.mTransmitterType == Transmitter::Ert0)
         {  mMode = Flock::ExtendedRange; }
         if(unit.hasTransmitter())
         {  mXmitterIndices.push_back(u); }
         if(unit.mHasSensor)
         {
            mAddrToSensorIdMap[unit.mAddr] = mNumSensors;     // Map from unit address to the sensor index
            mNumSensors += 1;
         }
      }

   }
   catch(Flock::FlockException& e)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Flock error: " << e.getMessage() << std::endl << vprDEBUG_FLUSH;
   }
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
   if(Flock::Standalone == mMode)
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

