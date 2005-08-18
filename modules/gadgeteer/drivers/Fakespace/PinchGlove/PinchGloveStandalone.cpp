/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <gadget/Devices/DriverConfig.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <vpr/vpr.h>
#include <vpr/System.h>
#include <gadget/Util/Debug.h>
#include <drivers/Fakespace/PinchGlove/PinchGloveStandalone.h>

namespace gadget
{
   PinchGloveStandalone::PinchGloveStandalone() : mPort(NULL),
                                                  mTimeStampsOn(true)
   {;}

   PinchGloveStandalone::~PinchGloveStandalone()
   {
      if ( mPort != NULL )
      {
         mPort->close();
         delete mPort;
         mPort = NULL;
      }
   }

   vpr::ReturnStatus PinchGloveStandalone::connect(const std::string& port_name, int baud)
   {
      // Create a new serial port
      mPort = new vpr::SerialPort(port_name);

      // Open the file handle read & write.
      mPort->setOpenReadWrite();

      try
      {
         mPort->open();
      }
      catch (vpr::IOException& ex)
      {
         return(vpr::ReturnStatus::Fail);
      }
      
      // Reset all port settings to false.
      mPort->clearAll();

      // Set the current character size.
      mPort->setCharacterSize(vpr::SerialTypes::CS_BITS_8);

      // The device is local.
      mPort->setLocalAttach(true);

      // Enable reading from the port.(Note: This does not effect
      // SerialPort::setOpenReadWrite() from above.
      mPort->setRead(true);

      // Enable ignoring BREAK bytes.
      mPort->setBreakByteIgnore(true);

      // Set the output/input baud rates. Put output first to be safe.
      mPort->setOutputBaudRate(baud);
      mPort->setInputBaudRate(baud);
      return(vpr::ReturnStatus::Succeed);
   }
   
   vpr::ReturnStatus PinchGloveStandalone::setTimestampsOn(const bool val)
   {
      vpr::ReturnStatus status;
      std::vector<vpr::Uint8> result;
      vpr::Uint8 data_type = PinchGlove::Control::START_BYTE_INFO;
    
      // Remember if timestamps are on or not.
      mTimeStampsOn = val;

      const vpr::Uint8 param = (val ? PinchGlove::Parameter::TimeStampsOn :
                                PinchGlove::Parameter::TimeStampsOff);
      try
      {
         status = sendCommand(PinchGlove::Command::TimeStamp, param);
         
         // If we could not read from the device, or the device responded with a
         // different value then we sent it.
         if(!readData(result, data_type).success() ||
            result[0] != param)
         {
            return vpr::ReturnStatus::Fail;
         }
      }
      catch(PinchGlove::TimeoutException& e)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << e.getMessage() << std::endl << vprDEBUG_FLUSH;
         return vpr::ReturnStatus::Timeout;
      }
      
      return vpr::ReturnStatus::Succeed;
   }
   
   vpr::ReturnStatus 
   PinchGloveStandalone::setDataFormatVersion(const vpr::Uint8& format_version)
   {
      vpr::ReturnStatus status;
      std::vector<vpr::Uint8> result;
      vpr::Uint8 data_type = PinchGlove::Control::START_BYTE_INFO;
      
      try
      {
         status = sendCommand(PinchGlove::Command::TimeStamp, format_version);
         
         // If we could not read from the device, or the device responded with a
         // different value then we sent it.
         if(!readData(result, data_type).success() ||
            result[0] != format_version)
         {
            return vpr::ReturnStatus::Fail;
         }
      }
      catch(PinchGlove::TimeoutException& e)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << e.getMessage() << std::endl << vprDEBUG_FLUSH;
         return vpr::ReturnStatus::Timeout;
      }
      
      return vpr::ReturnStatus::Succeed;
   }

   vpr::ReturnStatus
   PinchGloveStandalone::printHardwareInformation(std::vector<std::string>& info)
   {
      vpr::ReturnStatus status;
      std::vector<vpr::Uint8> result;
      std::string result_string;
      vpr::Uint8 data_type = PinchGlove::Control::START_BYTE_INFO;

      
      try
      {
         info.clear();
         info.push_back("================ FakeSpace PinchGlove ================");

         // Query the version of the left glove.
         status = sendCommand(PinchGlove::Command::QueryHardware,
                              PinchGlove::Parameter::LeftVersion);
         status = readData(result, data_type);
         result_string.resize(result.size());
         std::copy(result.begin(), result.end(), result_string.begin());
         result_string = "[PinchGlove] Version for Left Glove:   " + result_string;
         info.push_back(result_string);

         // Query the version of the right glove.
         status = sendCommand(PinchGlove::Command::QueryHardware,
                              PinchGlove::Parameter::RightVersion);
         status = readData(result, data_type);
         result_string.resize(result.size());
         std::copy(result.begin(), result.end(), result_string.begin());
         result_string = "[PinchGlove] Version for Right Glove:  " + result_string;
         info.push_back(result_string);
         
         // Query the processor code.
         status = sendCommand(PinchGlove::Command::QueryHardware,
                              PinchGlove::Parameter::ProcessorCode);
         status = readData(result, data_type);
         result_string.resize(result.size());
         std::copy(result.begin(), result.end(), result_string.begin());
         result_string = "[PinchGlove] Processor Code:           " + result_string;
         info.push_back(result_string);
         
         // Query the length of each timestamp tick.
         status = sendCommand(PinchGlove::Command::QueryHardware,
                              PinchGlove::Parameter::TickLength);
         status = readData(result, data_type);
         result_string.resize(result.size());
         std::copy(result.begin(), result.end(), result_string.begin());
         result_string = "[PinchGlove] Length of timestamp tick: " + result_string;
         info.push_back(result_string);
         
         info.push_back("======================================================");
      }
      catch(PinchGlove::TimeoutException& e)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << e.getMessage() << std::endl << vprDEBUG_FLUSH;
         return vpr::ReturnStatus::Timeout;
      }
      
      return vpr::ReturnStatus::Succeed;
   }

   vpr::ReturnStatus PinchGloveStandalone::sample(std::vector<int>& data, int& timestamp)
   {
      std::vector<vpr::Uint8> input_data(0);
      vpr::Uint8 data_type;
      
      // Determine which type of data to expect.
      if(mTimeStampsOn)
      {
         data_type = PinchGlove::Control::START_BYTE_DATA_TS;
      }
      else
      {
         data_type = PinchGlove::Control::START_BYTE_DATA;
      }
     
      // Try to get data from serial port.
      try
      {
         vpr::ReturnStatus status 
               = readData(input_data, data_type);
      }
      catch(PinchGlove::TimeoutException)
      {
         // We recieved a timeout, this is normal since the PinchGlove only
         // returns data when a pinch occurs.
         return vpr::ReturnStatus::Timeout;
      }
      
      int num_pinch_bytes;
      
      // Determine how to parse data depending on data type recieved.
      if(mTimeStampsOn)
      {
         num_pinch_bytes = input_data.size() - 2;
         timestamp = 
            (input_data[input_data.size()])<<7 | input_data[input_data.size()-1];
      }
      else
      {
         num_pinch_bytes = input_data.size();
         timestamp = -1;
      }
    
      // Determine if each finger is touching something else.
      for(int i = 0 ; i < num_pinch_bytes ; i += 2)
      {
         data[0] |= (input_data[i] & 0x1);
         data[1] |= ((input_data[i] & 0x2)>>1);
         data[2] |= ((input_data[i] & 0x4)>>2);
         data[3] |= ((input_data[i] & 0x8)>>3);
         data[4] |= ((input_data[i] & 0x10)>>4);
         
         data[5] |= (input_data[i+1] & 0x1);
         data[6] |= ((input_data[i+1] & 0x2)>>1);
         data[7] |= ((input_data[i+1] & 0x4)>>2);
         data[8] |= ((input_data[i+1] & 0x8)>>3);
         data[9] |= ((input_data[i+1] & 0x10)>>4);
      }

      return vpr::ReturnStatus::Succeed;
   }

   vpr::ReturnStatus PinchGloveStandalone::sendCommand(const vpr::Uint8& first, const vpr::Uint8& second)
   {
      vpr::Uint32 written;
      unsigned char buf[10];

      vpr::Interval read_timeout(100,vpr::Interval::Msec);

      mPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

      // If there was already a byte in the PinchGlove's input buffer
      // the hardware will get the last byte and return a '?' because
      // it is confused. If we can read this, we need to fix it. In a
      // way we are clearing the PinchGloves input buffer.

      mPort->write(&first, 1, written);
      try
      {
         mPort->read(&buf[0], 10, written, read_timeout);
         // If read without an error.
         mPort->write(&first, 1, written);
      }
      catch (vpr::IOException& ex)
      {
         // Do nothing.
      }
      vpr::System::msleep(100);
      // Wait between sending bytes.
      mPort->write(&second, 1, written);

      return vpr::ReturnStatus::Succeed;
   }

   vpr::ReturnStatus PinchGloveStandalone::readData(std::vector<vpr::Uint8>& result,
                                                    const vpr::Uint8& data_type)
   {
      vpr::Uint32 read;
      vpr::Uint8  temp_byte;

      vpr::Interval read_timeout(5000,vpr::Interval::Msec);

      try
      {
         mPort->read(&temp_byte, 1, read, read_timeout);
      }
      catch (vpr::IOException& ex)
      {
         // TODO: setCause(ex)
         throw PinchGlove::TimeoutException("[PinchGlove] Timeout occured while trying to read."); 
      }
      vprASSERT(1 == read);

      while(data_type != temp_byte)
      {
         try
         {
            mPort->read(&temp_byte, 1, read, read_timeout);
         }
         catch (vpr::IOException& ex)
         {
            // TODO: setCause(ex)
            throw PinchGlove::TimeoutException("[PinchGlove] Timeout occured while trying to read."); 
         }
         vprASSERT(1 == read);
      }

      // Getting all chars until END_BYTE
      result.clear();
      
      try
      {
         mPort->read(&temp_byte, 1, read, read_timeout);
      }
      catch (vpr::IOException& ex)
      {
         // TODO: setCause(ex)
         throw PinchGlove::TimeoutException("[PinchGlove] Timeout occured while trying to read."); 
      }
      vprASSERT(1 == read);
      
      // Getting all chars until END_BYTE
      while( PinchGlove::Control::END_BYTE != temp_byte)
      {
         // Add the byte to the result.
         result.push_back(temp_byte);
         
         try
         {
            mPort->read(&temp_byte, 1, read, read_timeout);
         }
         catch (vpr::IOException& ex)
         {
            // TODO: setCause(ex)
            throw PinchGlove::TimeoutException("[PinchGlove] Timeout occured while trying to read."); 
         }
         vprASSERT(1 == read);
      }
      return vpr::ReturnStatus::Succeed;
   }
} // End gadget namespace
