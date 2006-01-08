/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <stdio.h>
#include <boost/concept_check.hpp>
#include <vpr/System.h>
#include <vpr/IO/TimeoutException.h>

#include <drivers/Immersion/IBox/IBoxStandalone.h>

#define printInfo(command, data, desc)                          \
{                                                               \
   if ( sendStringCommand(command))                             \
   {                                                            \
      std::string data = readString();                          \
      vprDEBUG(vprDBG_VPR, vprDBG_CONFIG_STATUS_LVL)            \
         << "  [IBox] " << desc << ": "                         \
         << data << std::endl << vprDEBUG_FLUSH;                \
   }                                                            \
   else                                                         \
   {                                                            \
      vprDEBUG(vprDBG_VPR, vprDBG_CONFIG_STATUS_LVL)            \
         << "  [IBox] Could not get "                           \
         << desc << "!" << std::endl << vprDEBUG_FLUSH;         \
   }                                                            \
}

IBoxStandalone::IBoxStandalone()
{
   mSerialPort = NULL;
   mPacket.clear();
}

IBoxStandalone::~IBoxStandalone()
{
   if ( mSerialPort != NULL )
   {
      try
      {
         if (mSerialPort->isOpen())
         {
            disconnect();
         }
      }
      catch (...)
      {;}
      delete mSerialPort;
      mSerialPort = NULL;
   }
}

void IBoxStandalone::connect(const std::string& port_name, long int baud)
   throw (vpr::IOException, IBoxException)
{
   mPortName = port_name;
   mBaudRate = baud;

   mSerialPort = new vpr::SerialPort(mPortName);
   mSerialPort->setOpenReadWrite();

   try
   {
      mSerialPort->open();
      vprDEBUG(vprDBG_VPR, vprDBG_CONFIG_STATUS_LVL)
         << "[IBox] Success, Port: " << mPortName
         << " (" << mBaudRate << ") opened." << std::endl
         << vprDEBUG_FLUSH;
   }
   catch (vpr::IOException&)
   {
      vprDEBUG(vprDBG_VPR, vprDBG_CONFIG_STATUS_LVL)
         << "[IBox] Port: " << mPortName << " could not be opened!"
         << std::endl << vprDEBUG_FLUSH;
      throw IBoxException("[IBox] Port: " + mPortName
         + " could not be opened!", VPR_LOCATION);
   }

   mSerialPort->clearAll();
   mSerialPort->setLocalAttach(true);
   mSerialPort->setMinInputSize(0);
   mSerialPort->setTimeout(0);
   mSerialPort->setOutputBaudRate(mBaudRate); // Put me before input to be safe
   mSerialPort->setInputBaudRate(mBaudRate);
   mSerialPort->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
   mSerialPort->setRead(true);

   vprDEBUG(vprDBG_VPR, vprDBG_CONFIG_STATUS_LVL)
      << "[IBox] Changed all of the serial port settings."
      << std::endl << vprDEBUG_FLUSH;
   if (!autoSync())
   {
      throw IBoxException(
         "Could not sync with IBox device, try power cycling",
         VPR_LOCATION);
   }
   
   vprDEBUG(vprDBG_VPR, vprDBG_CONFIG_STATUS_LVL)
      << "[IBox] Done with autosync." << std::endl << vprDEBUG_FLUSH;
   beginCommunication();
   getInfo();
}

vpr::Uint8 IBoxStandalone::getCommandByte(const int timerFlag,
                                          const int numAnalogs,
                                          const int numEncoders) const
{
   vpr::Uint8 timer_byte;
   vpr::Uint8 analog_byte;
   vpr::Uint8 encoder_byte;

   if ( timerFlag )
   {
      timer_byte = TIMER_BIT;
   }
   else
   {
      timer_byte = 0;
   }

   if ( numAnalogs > 4 )
   {
      analog_byte = ANALOG_BITS;
   }
   else if ( numAnalogs > 2 )
   {
      analog_byte = ANALOG_HI_BIT;
   }
   else if ( numAnalogs > 0 )
   {
      analog_byte = ANALOG_LO_BIT;
   }
   else
   {
      analog_byte = 0;
   }

   if ( numEncoders > 4 )
   {
      encoder_byte = ENCODER_BITS;
   }
   else if ( numEncoders > 2 )
   {
      encoder_byte = ENCODER_HI_BIT;
   }
   else if ( numEncoders > 0 )
   {
      encoder_byte = ENCODER_LO_BIT;
   }
   else
   {
      encoder_byte = 0;
   }
   return (timer_byte | analog_byte | encoder_byte);
}

void IBoxStandalone::getInfo() throw (vpr::IOException)
{
   printInfo(GET_PROD_NAME, mProductId, "Product Name");
   printInfo(GET_PROD_ID, mProductId, "Product ID");
   printInfo(GET_MODEL_NAME, mModelName, "Model Name");
   printInfo(GET_SERNUM, mSerialNumber, "Serial Number");
   printInfo(GET_COMMENT, mComment, "Comment");
   printInfo(GET_PRM_FORMAT, mParamFormat, "Parameter Format");
   printInfo(GET_VERSION, mVersion, "Version:");
}

void IBoxStandalone::clearUpdateFieldss()
{
   mTimerUpdated = 0;
   mAnalogUpdated[0] = 0;
   mAnalogUpdated[1] = 0;
   mAnalogUpdated[2] = 0;
   mAnalogUpdated[3] = 0;
   mAnalogUpdated[4] = 0;
   mAnalogUpdated[5] = 0;
   mAnalogUpdated[6] = 0;
   mAnalogUpdated[7] = 0;
   mEncoderUpdated[0] = 0;
   mEncoderUpdated[1] = 0;
   mEncoderUpdated[2] = 0;
   mEncoderUpdated[3] = 0;
   mEncoderUpdated[4] = 0;
   mEncoderUpdated[5] = 0;
   mMarkerUpdated = 0;
}

bool IBoxStandalone::autoSync()
{
   vpr::Uint32 written;
   char temp[4];
   vpr::Interval timeout(100, vpr::Interval::Msec);
   int trys = 0;
   
   // Try to reset the device.
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   sendEndCommand();
   vpr::System::msleep(100);
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

   // Try to sync for a specified number of times
   while ( trys < 10 )
   {
      try
      {
         mSerialPort->write(SIGNON_STR, sizeof(SIGNON_STR) - 1, written);
         mSerialPort->read(&temp, 1, written, timeout);

         if ( temp[0] == 'I' )
         {
            mSerialPort->read(&temp, 3, written, timeout);
            
            if ( temp[0] == 'M' && temp[1] == 'M' && temp[2] == 'C' )
            {
               return true;
            }
         }
      }
      catch (vpr::TimeoutException&)
      {
         // Do nothing on timeouts.
      }

      trys++;
   }
   return false;
}

bool IBoxStandalone::beginCommunication()
{
   vpr::Uint32 written;
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   mSerialPort->write(BEGIN_STR, sizeof(BEGIN_STR) - 1, written);
   
   try
   {
      mProductId = readString();
      return true;
   }
   catch (...)
   {}
   return false;
}

void IBoxStandalone::sendEndCommand() throw (vpr::IOException)
{
   vpr::Uint32 written;
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);

   mSerialPort->write(END_STR, sizeof(END_STR) - 1, written);
}

void IBoxStandalone::disconnect() throw (vpr::IOException)
{
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   sendEndCommand();
   mSerialPort->close();
}

bool IBoxStandalone::sendStringCommand(const vpr::Uint8 command) const
{
   vpr::Uint32 written;
   vpr::Uint8 response;
   vpr::Interval timeout(100, vpr::Interval::Msec);

   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   mSerialPort->write(&command, 1, written);
   mSerialPort->read(&response, 1, written, timeout);
   
   // If the returned command does not match the sent command
   if ( response != command )
   {
      mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
      mSerialPort->write(&command, 1, written);
      try
      {
         mSerialPort->read(&response, 1, written, timeout);
      }
      catch (vpr::IOException&)
      {
         return false;
      }
      
      if (response != command)
      {
         return false;
      }
   }
   return true;
}

std::string IBoxStandalone::readString() throw (vpr::IOException)
{
   char buffer[MAX_STRING_SIZE];
   char* buffer_ptr = buffer;
   vpr::Uint32 written;
   char ch;
   vpr::Interval timeout(100, vpr::Interval::Msec);

   mSerialPort->read(&ch,1,written, timeout);

   while ( 0 != ch )
   {
      *(buffer_ptr++) = (char) ch;
      mSerialPort->read(&ch,1,written, timeout);
   }
   return (std::string(buffer));
}

void IBoxStandalone::waitForUpdate(int timerFlag, int numAnalogs, int numEncoders)
   throw (vpr::IOException)
{
   sendCommand(timerFlag, numAnalogs, numEncoders);
   waitForPacket();
}

void IBoxStandalone::waitForPacket()
{
   bool running(true);
   while (running)
   {
      try
      {
         checkForPacket();
      }
      catch (vpr::TimeoutException&)
      {
         continue;
      }
      catch (IBoxException&)
      {
         continue;
      }

      // If there were no errors, then we know we have the packet.
      running = false;
   }   
}

void IBoxStandalone::sendCommand(const int timerFlag, const int numAnalogs,
                                 const int numEncoders) const
   throw (vpr::IOException)
{
   vpr::Uint32 written;
   vpr::Uint8 command = getCommandByte(timerFlag, numAnalogs, numEncoders);
   mSerialPort->write(&command, 1, written);
}

void IBoxStandalone::sendSimpleConfigCommand(const vpr::Uint8 command) const
{
   vpr::Uint32 written;
   mSerialPort->write(&command, 1, written);
}

void IBoxStandalone::sendPasswordCommand(const vpr::Uint8 command,
   const std::vector<vpr::Uint8> args) throw (vpr::IOException, IBoxException)
{
   vpr::Uint8 response;
   vpr::Uint32 bytes_written;
   vpr::Uint32 bytes_read;
   vpr::Interval timeout(100, vpr::Interval::Msec);

   // Write command
   mSerialPort->write(&command, 1, bytes_written);
   mSerialPort->read(&response, 1, bytes_read, timeout);

   while ( response != command )
   {
      mSerialPort->read(&response, 1, bytes_read, timeout);
   }

   mSerialPort->write(mSerialNumber.c_str(), strlen(mSerialNumber.c_str()), bytes_written);
   // Send NULL to terminate.
   mSerialPort->write(NULL, 1, bytes_written);
   mSerialPort->read(&response, 1, bytes_read, timeout);
   if ( response != PASSWD_OK )
   {
      throw IBoxException("Password not accepted.", VPR_LOCATION);
   }
   mSerialPort->write(args, args.size(), bytes_written);
}

void IBoxStandalone::insertMarker(const vpr::Uint8 marker) const
{
   sendSimpleConfigCommand(INSERT_MARKER);
   vpr::Uint32 bytes_written;
   mSerialPort->write( &marker, 1, bytes_written);
}

void IBoxStandalone::getHomeRef()
   throw (vpr::IOException, IBoxException)
{
   sendSimpleConfigCommand(GET_HOME_REF);
   checkForPacket();
}

void IBoxStandalone::setHomeRef(int *homeref)
   throw (vpr::IOException, IBoxException)
{
   std::vector<vpr::Uint8> args;
   args.resize(2 * NUM_ENCODERS);
   args[0] = homeref[0] >> 8;
   args[1] = homeref[0] & 0x00FF;
   args[2] = homeref[1] >> 8;
   args[3] = homeref[1] & 0x00FF;
   args[4] = homeref[2] >> 8;
   args[5] = homeref[2] & 0x00FF;
   args[6] = homeref[3] >> 8;
   args[7] = homeref[3] & 0x00FF;
   args[8] = homeref[4] >> 8;
   args[9] = homeref[4] & 0x00FF;
   args[10] = homeref[5] >> 8;
   args[11] = homeref[5] & 0x00FF;
   sendPasswordCommand(SET_HOME_REF, args);
}

void IBoxStandalone::goHomePosition() throw (vpr::IOException, IBoxException)
{
   sendSimpleConfigCommand(HOME_POS);
   checkForPacket();
}

void IBoxStandalone::setHomePosition(int *homepos)
   throw (vpr::IOException, IBoxException)
{
   std::vector<vpr::Uint8> args;
   args.resize(2 * NUM_ENCODERS);
   args[0] = homepos[0] >> 8;
   args[1] = homepos[0] & 0x00FF;
   args[2] = homepos[1] >> 8;
   args[3] = homepos[1] & 0x00FF;
   args[4] = homepos[2] >> 8;
   args[5] = homepos[2] & 0x00FF;
   args[6] = homepos[3] >> 8;
   args[7] = homepos[3] & 0x00FF;
   args[8] = homepos[4] >> 8;
   args[9] = homepos[4] & 0x00FF;
   args[10] = homepos[5] >> 8;
   args[11] = homepos[5] & 0x00FF;
   sendPasswordCommand(SET_HOME, args);
}

void IBoxStandalone::getMaxValues() throw (vpr::IOException, IBoxException)
{
   sendSimpleConfigCommand(GET_MAXES);
   checkForPacket();
}

void IBoxStandalone::setFactoryDefaults() throw (vpr::IOException, IBoxException)
{
   std::vector<vpr::Uint8> args;
   args.resize(0);
   sendPasswordCommand(RESTORE_FACTORY, args);
}

void IBoxStandalone::startMotionMode(int timer_flag, int analog_reports,
   int encoder_reports, int delay, vpr::Uint8 active_btns,
   std::vector<vpr::Uint8>& analog_deltas,
   std::vector<vpr::Uint8>& encoder_deltas) throw (vpr::IOException)
{
   boost::ignore_unused_variable_warning(active_btns);
   vpr::Uint32 written;
   vpr::Uint8 temp;
   vpr::Uint8 command = getCommandByte(timer_flag, analog_reports, encoder_reports);

   // Command Byte 4F or CF
   temp = REPORT_MOTION;
   mSerialPort->write(&temp, 1, written);

   // Minimum delay between packets, 16 bit integer, ticks are ~1 ms
   temp = (vpr::Uint16)delay;
   mSerialPort->write(&temp, 1, written);

   // Send the command that we are to act like we are responding to	(Byte 3)
   mSerialPort->write(&command, 1, written);

   // Does each button click generate a packet
   temp = 0xFF;
   mSerialPort->write(&temp, 1, written);

   // Send the minimum analog changes to generate a packet.
   analog_deltas.resize(NUM_ENCODERS);
   mSerialPort->write(analog_deltas, analog_deltas.size(), written);

   // Send the minimum encoder change to generate a packet.
   encoder_deltas.resize(NUM_ENCODERS);
   mSerialPort->write(encoder_deltas, encoder_deltas.size(), written);
}

void IBoxStandalone::stopMotionMode()
{
   vpr::Uint32 written;
   mSerialPort->write(0, 1, written);
   mSerialPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
}

void IBoxStandalone::checkForPacket() throw (vpr::IOException, IBoxException)
{
   getDataPacket();
   parsePacket();
}

void IBoxStandalone::checkForMotion() throw (vpr::IOException, IBoxException)
{
   getDataPacket();
   parsePacket();
}

void IBoxStandalone::getDataPacket() throw (vpr::IOException, IBoxException)
{
   vpr::Uint8 packet_type;
   vpr::Uint32 written;
   vpr::Interval timeout(100, vpr::Interval::Msec);

   try
   {
      mSerialPort->read(&packet_type, 1, written, timeout);
   }
   catch (vpr::IOException&)
   {
      vprDEBUG(vprDBG_VPR, vprDBG_CONFIG_STATUS_LVL)
         << "[IBox] Failure to get first byte" << std::endl << vprDEBUG_FLUSH;
      throw;
   }

   // Store the command byte as the packet type.
   mPacket.cmd_byte = packet_type;
   // This is because the 7th bit is always set high when echoed back
   if ( packet_type < 0x80 )
   {
      vprDEBUG(vprDBG_VPR, vprDBG_CONFIG_STATUS_LVL)
         << "[IBox] Not a valid command!" << std::endl << vprDEBUG_FLUSH;
      mPacket.error = 1;
      throw IBoxException("Not a valid command!", VPR_LOCATION);
   }
   else
   {
      mPacket.error = 0;
      mPacket.data_ptr = mPacket.data;
      mPacket.num_bytes_needed = getPacketSize(packet_type);

      mSerialPort->read(mPacket.data_ptr, mPacket.num_bytes_needed, written, timeout);
   }
}

void IBoxStandalone::parsePacket() throw (IBoxException)
{
   vpr::Uint8 command = mPacket.cmd_byte;
   unsigned char bits;
   vpr::Uint8* dp;

   if ( mPacket.num_bytes_needed == 0 )
   {
      throw IBoxException("Empty packet can not be parsed.", VPR_LOCATION);
   }
   if ( mPacket.error )
   {
      throw IBoxException("Packet error.", VPR_LOCATION);
   }

   clearUpdateFieldss();
   dp = mPacket.data;
   if ( command < CONFIG_MIN )
   {
      bits = *dp++; // Bits = the second byte of the data
      mButtonBits = bits; // mbuttons simply = this first byte
      mButtonValues[0] = (bits & 0x01);
      mButtonValues[1] = (bits & 0x02) >> 1;
      mButtonValues[2] = (bits & 0x04) >> 2;
      mButtonValues[3] = (bits & 0x08) >> 3;
      mButtonValues[4] = (bits & 0x10) >> 4;
      mButtonValues[5] = (bits & 0x20) >> 5;
      mButtonValues[6] = (bits & 0x40) >> 6;
      if ( command & TIMER_BIT )
      {
         mTimer = *dp++ << 7;
         mTimer += *dp++;
         mTimerUpdated = 1;
      }
      bits = (command & ANALOG_BITS) >> 2;
      if ( bits-- )
      {
         mAnalog[0] = *dp++ << 1;
         mAnalog[1] = *dp++ << 1;
         mAnalogUpdated[0] = 1;
         mAnalogUpdated[1] = 1;
         if ( bits-- )
         {
            mAnalog[2] = *dp++ << 1;
            mAnalog[3] = *dp++ << 1;
            mAnalogUpdated[2] = 1;
            mAnalogUpdated[3] = 1;
            if ( bits-- )
            {
               mAnalog[4] = *dp++ << 1;
               mAnalog[5] = *dp++ << 1;
               mAnalog[6] = *dp++ << 1;
               mAnalog[7] = *dp++ << 1;
               mAnalogUpdated[4] = 1;
               mAnalogUpdated[5] = 1;
               mAnalogUpdated[6] = 1;
               mAnalogUpdated[7] = 1;
            }
         }
         mAnalog[0] |= (*dp & 0x40 ? 1 : 0);
         mAnalog[1] |= (*dp & 0x20 ? 1 : 0);
         mAnalog[2] |= (*dp & 0x10 ? 1 : 0);
         mAnalog[3] |= (*dp & 0x08 ? 1 : 0);
         mAnalog[4] |= (*dp & 0x04 ? 1 : 0);
         mAnalog[5] |= (*dp & 0x02 ? 1 : 0);
         mAnalog[6] |= (*dp++ & 0x01 ? 1 : 0);
      }
      bits = command & ENCODER_BITS;
      if ( bits-- )
      {
         mEncoder[0] = *dp++ << 7;
         mEncoder[0] += *dp++;
         mEncoder[1] = *dp++ << 7;
         mEncoder[1] += *dp++;
         mEncoderUpdated[0] = 1;
         mEncoderUpdated[1] = 1;
         if ( bits-- )
         {
            mEncoder[2] = *dp++ << 7;
            mEncoder[2] += *dp++;
            mEncoderUpdated[2] = 1;
            if ( bits-- )
            {
               mEncoder[3] = *dp++ << 7;
               mEncoder[3] += *dp++;
               mEncoder[4] = *dp++ << 7;
               mEncoder[4] += *dp++;
               mEncoder[5] = *dp++ << 7;
               mEncoder[5] += *dp++;
               mEncoderUpdated[3] = 1;
               mEncoderUpdated[4] = 1;
               mEncoderUpdated[5] = 1;
            }
         }
      }
   }
   else
   {
      parseConfigPacket();
   }
}

void IBoxStandalone::parseConfigPacket() throw (IBoxException)
{
   vpr::Uint8 *dp = mPacket.data;

   switch ( mPacket.cmd_byte )
   {
   case GET_HOME_REF:
      mHomeRef[0] = *dp++ << 8;
      mHomeRef[0] += *dp++;
      mHomeRef[1] = *dp++ << 8;
      mHomeRef[1] += *dp++;
      mHomeRef[2] = *dp++ << 8;
      mHomeRef[2] += *dp++;
      mHomeRef[3] = *dp++ << 8;
      mHomeRef[3] += *dp++;
      mHomeRef[4] = *dp++ << 8;
      mHomeRef[4] += *dp++;
      mHomeRef[5] = *dp++ << 8;
      mHomeRef[5] += *dp++;
      break;
   case GET_MAXES:
      mButtonSupported[0] = *dp & 0x01;
      mButtonSupported[1] = (*dp & 0x02) >> 1;
      mButtonSupported[2] = (*dp & 0x04) >> 2;
      mButtonSupported[3] = (*dp & 0x08) >> 3;
      mButtonSupported[4] = (*dp & 0x10) >> 4;
      mButtonSupported[5] = (*dp & 0x20) >> 5;
      mButtonSupported[6] = (*dp++ & 0x40) >> 6;

      mMaxTimer = *dp++ << 8;
      mMaxTimer |= *dp++;

      mMaxAnalog[0] = *dp++;
      mMaxAnalog[1] = *dp++;
      mMaxAnalog[2] = *dp++;
      mMaxAnalog[3] = *dp++;
      mMaxAnalog[4] = *dp++;
      mMaxAnalog[5] = *dp++;
      mMaxAnalog[6] = *dp++;
      mMaxAnalog[7] = *dp++;

      mMaxAnalog[0] |= (0x40 & *dp ? 0x01 : 0);
      mMaxAnalog[1] |= (0x20 & *dp ? 0x01 : 0);
      mMaxAnalog[2] |= (0x10 & *dp ? 0x01 : 0);
      mMaxAnalog[3] |= (0x08 & *dp ? 0x01 : 0);
      mMaxAnalog[4] |= (0x04 & *dp ? 0x01 : 0);
      mMaxAnalog[5] |= (0x02 & *dp ? 0x01 : 0);
      mMaxAnalog[6] |= (0x01 & *dp++ ? 0x01 : 0);

      mMaxEncoder[0] = *dp++ << 8;
      mMaxEncoder[0] += *dp++;
      mMaxEncoder[1] = *dp++ << 8;
      mMaxEncoder[1] += *dp++;
      mMaxEncoder[2] = *dp++ << 8;
      mMaxEncoder[2] += *dp++;
      mMaxEncoder[3] = *dp++ << 8;
      mMaxEncoder[3] += *dp++;
      mMaxEncoder[4] = *dp++ << 8;
      mMaxEncoder[4] += *dp++;
      mMaxEncoder[5] = *dp++ << 8;
      mMaxEncoder[5] += *dp++;
      break;
   case INSERT_MARKER:
      mMarker = *dp;
      mMarkerUpdated = 1;
      break;
   case HOME_POS:	/* No action needed */
   case REPORT_MOTION:
      break;
   default:
      throw IBoxException("Unsupported configuration packet", VPR_LOCATION);
      break;
   }
}

vpr::Uint32 IBoxStandalone::getPacketSize(const vpr::Uint8 packetType)
{
   // Regular cmds always include buttons byte
   vpr::Uint32 size = 1;
   int bits;

   if ( packetType < CONFIG_MIN )
   {
      if ( packetType & TIMER_BIT )
      {
         size += 2;
      }

      // Extract ANALOG bits and TEST
      bits = packetType & ANALOG_BITS;
      if ( bits == ANALOG_LO_BIT )
      {
         size += 2 + 1;
      }
      else if ( bits == ANALOG_HI_BIT )
      {
         size += 4 + 1;
      }
      else if ( bits == ANALOG_BITS )
      {
         size += 8 + 1;
      }

      // Extract Encoder bits and TEST
      bits = packetType & ENCODER_BITS;
      if ( bits == ENCODER_LO_BIT )
      {
         size += 2*2;
      }
      else if ( bits == ENCODER_HI_BIT )
      {
         size += 2*3;
      }
      else if ( bits == ENCODER_BITS )
      {
         size += 2*6;
      }
   }
   else
   {
      switch ( packetType )
      {
      case GET_HOME_REF:
         size = 12;
         break;
      case SET_BAUD:
      case INSERT_MARKER:
         size = 1;
         break;
      case GET_MAXES:
         size = 24;
         break;
      case HOME_POS:
      case END_SESSION:
      case REPORT_MOTION:
         size = 0;
         break;
      case GET_PARAMS:
      case GET_PROD_NAME:
      case GET_PROD_ID:
      case GET_MODEL_NAME:
      case GET_SERNUM:
      case GET_COMMENT:
      case GET_PRM_FORMAT:
      case GET_VERSION:
      //case SET_PARAMS:
      case SET_HOME:
      case SET_HOME_REF:
      case RESTORE_FACTORY:
         size = 0;
         break;
      }
   }
   return size;
}

int IBoxStandalone::getButtonData(int pos) const
{
   return mButtonValues[pos];
}

int IBoxStandalone::getAnalogData(int pos) const
{
   return mAnalog[pos];
}
