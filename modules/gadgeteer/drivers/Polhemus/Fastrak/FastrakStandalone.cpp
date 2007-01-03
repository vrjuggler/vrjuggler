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

#include <string>
#include <string.h>
#include <boost/bind.hpp>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/WouldBlockException.h>
#include <vpr/Util/Assert.h>
#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/format.hpp>
#include <limits>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>
#include <gmtl/Quat.h>
#include <gmtl/Vec.h>

#include <drivers/Polhemus/Fastrak/FastrakStandalone.h>

namespace Fastrak
{
void printBinary(const vpr::Uint32 val)
{
   for (int i = 31; i >= 0; i--)
   {
      if (val & 1<<i)
      {
         std::cout << 1;
      }
      else
      {
         std::cout << 0;
      }
      if (0 == i % 8)
      {
         std::cout << " ";
      }
   }
   std::cout << std::endl;
}
}

bool FastrakStandalone::open()
{
   vprDEBUG_BEGIN(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "====== Opening Fastrak serial port: " << mPort << " =====\n"
      << vprDEBUG_FLUSH;

   mSerialPort = new vpr::SerialPort(mPort);
   mSerialPort->setBlocking(false);
   mSerialPort->setOpenReadWrite();

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

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "Port opened successfully\n" << vprDEBUG_FLUSH ;
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "Configuring port attributes\n" << vprDEBUG_FLUSH;

   mSerialPort->setUpdateAction(vpr::SerialTypes::NOW);      // Changed apply immediately

   mSerialPort->clearAll();
   mSerialPort->setCanonicalInput(false);              // enable binary reading and timeouts
   mSerialPort->setTimeout(10);                       // Set to 1 inter-byte read second timeout
   mSerialPort->setRead(true);
   mSerialPort->setLocalAttach(true);
   mSerialPort->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
   mSerialPort->setBreakByteIgnore(true);
   mSerialPort->setBadByteIgnore(true);

   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "Setting baud rate: " << mBaud << std::endl << vprDEBUG_FLUSH;
   mSerialPort->setInputBaudRate(mBaud);
   mSerialPort->setOutputBaudRate(mBaud);

   mSerialPort->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
   mSerialPort->setHardwareFlowControl(false);     // No hardware flow control
   mSerialPort->setParityGeneration(false);        // No parity checking

   try
   {
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   }
   catch (vpr::IOException&)
   {
      if (mSerialPort->isOpen())
      {
         mSerialPort->close();
      }
      delete mSerialPort;
      mSerialPort = NULL;

      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "FlockStandalone::open: Failed to open successfully."
         << std::endl << vprDEBUG_FLUSH;

      return false;
   }

   vprDEBUG_END(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "========== Port setup completed. ==========\n" << vprDEBUG_FLUSH;


   return true;
}

void FastrakStandalone::close()
{
   if (NULL != mSerialPort)
   {
      mSerialPort->close();
      delete mSerialPort;
      mSerialPort = NULL;
   }
}

void FastrakStandalone::init()
{
   sendCommand(Fastrak::Command::DisableContinuous);
   setBinaryMode(true);
   printStatus();
   setOutputDataList(1, "2,11,1");
   setOutputDataList(2, "2,11,1");
   // Getting the status of any station saves the status of all stations in mStationStatus.
   getStationStatus(1);
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "Num stations: " << mNumActiveStations << std::endl << vprDEBUG_FLUSH;
}

void FastrakStandalone::readData()
{
   std::vector<vpr::Uint8> data_record;
   std::vector<vpr::Uint8> temp_data_record;    // Temp buffer for reading data

   vpr::Uint32 bytes_read;
   vpr::Uint32 bytes_remaining;

   unsigned int single_bird_data_size = 28 + 5;
   const unsigned int data_record_size(mNumActiveStations*single_bird_data_size);    // Size of the data record to read

   sendCommand(Fastrak::Command::Point);       // Triggers a data record update

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
         //num_stream_read_failures++;
         throw vpr::Exception("Did not read full data record in point mode.", VPR_LOCATION);
         //throw Flock::CommandFailureException(
         //   "Did not read full data record in point mode.",
         //   VPR_LOCATION
         //);
      }
   }
   vprASSERT(data_record.size() == data_record_size);            // Assert: We actually read the number of bytes we set out too
   processDataRecord(data_record);
}

void FastrakStandalone::processDataRecord(std::vector<vpr::Uint8>& dataRecord)
{
   unsigned int single_bird_data_size = 28 + 5;

   // For each station
   // - Get matrix from data format
   for(unsigned int i=0; i < mNumActiveStations; ++i)
   {
      unsigned int data_offset = (single_bird_data_size*i);
      vprASSERT(dataRecord[data_offset] == '0');
      unsigned int station_index = boost::lexical_cast<unsigned int>(dataRecord[data_offset+1]);
      //std::cout << "Station: " << station_index << std::endl;
      vprASSERT(station_index >= 1 && station_index <= 4 && "Invalid station index.");
      vprASSERT(dataRecord[data_offset+2] == ' ');

      // Position and orientation offsets into data buffer.
      unsigned int pos_offset = data_offset + 3;
      unsigned int ori_offset = data_offset + 3 + 12;

      gmtl::Vec3f pos;
      gmtl::Quatf ori;

      // Get position values.
      pos[0] = getFloatValue(&dataRecord[pos_offset]);
      pos[1] = getFloatValue(&dataRecord[pos_offset+4]);
      pos[2] = getFloatValue(&dataRecord[pos_offset+8]);

      // Convert translation into meters.
      if (Fastrak::INCHES == mUnits)
      {
         pos *= 0.0254f;
      }
      else
      {
         pos *= 0.01f;
      }

      // Get quaternion for orientation.
      ori[0] = getFloatValue(&dataRecord[ori_offset]);
      ori[1] = getFloatValue(&dataRecord[ori_offset+4]);
      ori[2] = getFloatValue(&dataRecord[ori_offset+8]);
      ori[3] = getFloatValue(&dataRecord[ori_offset+12]);

      // Create a gmtl::Matrix for easy storage.
      gmtl::Matrix44f position;
      gmtl::setRot(position, ori);
      gmtl::setTrans(position, pos);

      vprASSERT(4 == mStationData.size());
      mStationData[station_index] = position;
   }
}

float FastrakStandalone::getFloatValue(vpr::Uint8* buff)
{
   vpr::Uint32 value = ((vpr::Uint32*)buff)[0];
   float fvalue = *((float*)&value);
   return fvalue;
}

void FastrakStandalone::sendCommand(vpr::Uint8 cmd, std::string data)
{
   try
   {
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);       // Clear the buffers
   }
   catch (vpr::IOException&)
   {
      throw vpr::Exception("Failed to flush queue before command.", VPR_LOCATION);
   }
   vpr::Uint32 bytes_written;
   mSerialPort->write(&cmd, 1, bytes_written);
   if(!data.empty())
   {
      mSerialPort->write(data, data.size(), bytes_written);
      vprASSERT(data.size() == bytes_written);
   }
   mSerialPort->drainOutput();
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);       // Clear the buffers
}

void FastrakStandalone::setOutputDataList(const vpr::Uint16 unit,
   const std::string& list)
{
   std::string data = boost::lexical_cast<std::string>(unit)
      + "," + list + "\r";

   sendCommand(Fastrak::Command::SetOutputList, data);
}

bool FastrakStandalone::getStationStatus(const vpr::Uint16 station)
{
   vprASSERT(station >= 1 && station <= 4 && "Station index must between 1-4");

   std::string data = boost::lexical_cast<std::string>(station) + "\r";
   sendCommand(Fastrak::Command::StationStatus, data);
   std::vector<vpr::Uint8> data_record;
   vpr::Uint32 bytes_read;

   try
   {
      mSerialPort->read(data_record, 9,
                        bytes_read, mReadTimeout);
   }
   catch (vpr::IOException&)
   {
      throw vpr::Exception("Failed to get station status.", VPR_LOCATION);
   }

   vprASSERT('2' == data_record[0]);
   vprASSERT('l' == data_record[2]);

   mStationStatus.resize(4, false);
   mStationStatus[0] = boost::lexical_cast<bool>(data_record[3]);
   mStationStatus[1] = boost::lexical_cast<bool>(data_record[4]);
   mStationStatus[2] = boost::lexical_cast<bool>(data_record[5]);
   mStationStatus[3] = boost::lexical_cast<bool>(data_record[6]);

   // Save the number of active stations.
   mNumActiveStations = std::count(mStationStatus.begin(), mStationStatus.end(), true);

   // Fastrak uses 1-4 indexes, but in memory we store as 0-3.
   return mStationStatus[station-1];
}

void FastrakStandalone::printStatus()
{
   vpr::Uint32 bytes_read;
   std::vector<vpr::Uint8> respData;
   unsigned int respSize = 55;

   if (NULL == mSerialPort)
   {
      throw vpr::Exception("NULL port.", VPR_LOCATION);
   }

   // Send status update command.
   sendCommand(Fastrak::Command::SystemStatus);

   // Read response and then flush the port to make sure we don't leave
   // anything extra.
   mSerialPort->readn(respData, respSize, bytes_read);
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

   // Check response size
   if(bytes_read != respSize)
   {
      throw vpr::Exception("Incomplete command response.", VPR_LOCATION);
   }

   std::string blank;
   std::string software_version_id;
   std::string system_id;
   std::string system_flags_string;
   vpr::Uint32 system_flags;

   // Ensure that we are getting the correct response.
   vprASSERT('2' == respData[0]);
   vprASSERT('S' == respData[2]);

   // Get error bits.
   vpr::Uint32 bit_error = (respData[8] << 16) + (respData[7] << 8) + respData[6];

   // Get a string that is a hex value representing tghe current system status.
   for (unsigned int i = 3; i < 6; i++)
   {
      system_flags_string += respData[i];
   }
   // Convert hex string to an integer.
   system_flags = strtol(system_flags_string.c_str(), NULL, 16);

   // Ensure that data is always blank like specified.
   for (unsigned int i = 9; i < 15; i++)
   {
      blank += respData[i];
   }
   vprASSERT(std::string::npos == blank.find_first_not_of(' '));

   // Get software version id.
   for (unsigned int i = 15; i < 21; i++)
   {
      software_version_id += respData[i];
   }

   // Get system id that can be set using the Configuration Control Data ('X') command.
   for (unsigned int i = 21; i < 53; i++)
   {
      system_id += respData[i];
   }

   vprDEBUG_BEGIN(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "==== Fastrak Status ====" << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "Software Version: " << software_version_id << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "System ID: " << system_id << std::endl << vprDEBUG_FLUSH;

   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "Error Bits: " << bit_error << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "Output Format: " << (system_flags & 0x01 ? "Binary" : "ASCII") << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "Units: " << (system_flags & 0x02 ? "Centimeters" : "Inches") << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "Compensation: " << (system_flags & 0x04 ? "On" : "Off") << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "Transmit Mode: " << (system_flags & 0x08 ? "Continuous" : "Non-Continuous") << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "Configuration: " << (system_flags & 0x10 ? "Tracker" : "Other") << std::endl << vprDEBUG_FLUSH;

   vprASSERT(system_flags & 0x11 && "Bit 5 should always by 1");

   // Check to see what units we are using.
   // NOTE: This can be used to check the value after calling setUnits().
   if (system_flags & 0x02)
   { mUnits = Fastrak::CENTIMETERS; }
   else
   { mUnits = Fastrak::INCHES; }

   vprDEBUG_END(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "========================" << std::endl << vprDEBUG_FLUSH;
}

void FastrakStandalone::setBinaryMode(const bool binary)
{
   if (binary)
   {
      sendCommand(Fastrak::Command::BinaryMode);
   }
   else
   {
      sendCommand(Fastrak::Command::AsciiMode);
   }
}

void FastrakStandalone::setStylusButtonEnabled(const vpr::Uint16 station, bool enabled)
{
   std::string data = boost::lexical_cast<std::string>(station) + "," + (enabled ? "1":"0") + "\r";
   sendCommand(Fastrak::Command::SetStylusButton, data);
}

void FastrakStandalone::setHemisphere(const vpr::Uint16 station, Fastrak::HEMISPHERE hemi)
{
   std::string data = getHemiData(station, hemi);

   sendCommand(Fastrak::Command::SetHemisphere, data);
}

void FastrakStandalone::setUnits(Fastrak::UNITS units)
{
   if (Fastrak::INCHES == units)
   {
      sendCommand(Fastrak::Command::SetInches);
   }
   else
   {
      sendCommand(Fastrak::Command::SetCentimeters);
   }
}
