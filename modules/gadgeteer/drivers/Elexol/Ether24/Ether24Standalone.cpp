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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <string>

#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/Util/Debug.h>
#include <drivers/Elexol/Ether24/Ether24Standalone.h>

void Ether24Standalone::open(vpr::InetAddr& addr)
{
   if (!mActive)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
         << "Ether24Standalone::open() Opening Ether24 UDP socket: "
         << addr.getAddressString() << ":" << addr.getPort()
         << " =====" << std::endl << vprDEBUG_FLUSH;
      // Open UDP connection.
      mSocket = new vpr::SocketDatagram();
      try
      {
         mSocket->open();
      }
      catch (vpr::IOException& ex)
      {
         delete mSocket;
         mSocket = NULL;
         mActive = false;
         throw ex;
      }
      mAddress = addr;
      mActive = true;
   }
}

void Ether24Standalone::close()
{
   if (mActive)
   {
      if (NULL != mSocket)
      {
         mSocket->close();
         delete mSocket;
         mSocket = NULL;
      }
      mActive = false;
   }
}

Elexol::device_map_t Ether24Standalone::getDevicesByMacAddress()
{
   vpr::SocketDatagram* socket = new vpr::SocketDatagram();
   socket->open();
   socket->setBroadcast(true);
   vpr::InetAddr broadcast_addr;
   broadcast_addr.setAddress("255.255.255.255", 2424);

   // Broadcast command.
   std::string msg("IO24");
   vpr::Uint32 bytes_sent;
   socket->sendto(msg, 4, broadcast_addr, bytes_sent);

   std::vector<vpr::Uint8> response(12);

   vpr::InetAddr from;
   vpr::Uint32 bytes_read;

   Elexol::device_map_t mac_to_addr_map;

   vprDEBUG_BEGIN(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "=== Elexol Ether I/O 24 Devices Found ==="
      << std::endl << vprDEBUG_FLUSH;
   try
   {
      while (true)
      {
         vpr::Interval read_timeout(1, vpr::Interval::Sec);
         socket->recvfrom(response, 12, from, bytes_read, read_timeout);
         vpr::BufferObjectReader reader(&response);
         std::string cmd_string((char*)reader.readRaw(4));

         std::stringstream mac_ss;
         std::stringstream fw_ss;
   
         vpr::Uint8* mac_address = reader.readRaw(6);
         vpr::Uint8* fw_version = reader.readRaw(2);
         mac_ss << std::hex << std::setfill('0')
            << std::setw(2) << (int)mac_address[0] << ":"
            << std::setw(2) << (int)mac_address[1] << ":"
            << std::setw(2) << (int)mac_address[2] << ":"
            << std::setw(2) << (int)mac_address[3] << ":"
            << std::setw(2) << (int)mac_address[4] << ":"
            << std::setw(2) << (int)mac_address[5] << std::dec;
         fw_ss << (int)fw_version[0] << "." << (int)fw_version[1];
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "[" << mac_ss.str() << "] [" << fw_ss.str() << "] [" << from.getAddressString()
            << ":" << from.getPort() << "]" << std::endl << vprDEBUG_FLUSH;

         if (mac_to_addr_map.find(mac_ss.str()) != mac_to_addr_map.end())
         {
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
               << "WARNING: Already have a response for a device with MAC: "
               << mac_ss.str() << std::endl << vprDEBUG_FLUSH;
         }
         else
         {
            mac_to_addr_map[mac_ss.str()] = from;
         }
         
      }
   }
   catch (vpr::SocketException&)
   {
      /* Do nothing. */
   }

   vprDEBUG_END(vprDBG_ALL, vprDBG_WARNING_LVL)
      << std::endl << vprDEBUG_FLUSH;

   socket->close();
   delete socket;

   return mac_to_addr_map;
}

vpr::Uint8 Ether24Standalone::getState(const Elexol::Port port, const Elexol::CommandType command) const
{
   vprASSERT(command == Elexol::Command::Value ||
             command == Elexol::Command::Direction ||
             command == Elexol::Command::PullUp ||
             command == Elexol::Command::Threshold ||
             command == Elexol::Command::SchmittTrigger);

   //std::vector<vpr::Uint8> cmd(0);
   std::string cmd;
   if (Elexol::Command::Value != command)
   {
      cmd.push_back(command);
   }

   vpr::Uint8 port_address = Elexol::Command::PortReadOffset + port;
   cmd.push_back(port_address);

   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
      << "Ether24StandalonegetState(): Sending command [" << cmd << "]"
      << std::endl << vprDEBUG_FLUSH;

   vpr::Uint32 bytes_sent;
   mSocket->sendto(cmd, cmd.size(), mAddress, bytes_sent);

   vpr::InetAddr from;
   vpr::Uint32 bytes_read;
   std::vector<vpr::Uint8> response(0);
   vpr::Interval read_timeout(100, vpr::Interval::Msec);
   mSocket->recvfrom(response, cmd.size() + 1, from, bytes_read, read_timeout);

   if (Elexol::Command::Value == command)
   {
      if (Elexol::Command::PortWriteOffset + port != response[0])
      {
         throw Elexol::ElexolException("Response does not match command sent.",
            VPR_LOCATION);
      }
   }
   else
   {
      if(cmd[0] != response[0] ||
         Elexol::Command::PortWriteOffset + port != response[1])
      {
         throw Elexol::ElexolException("Response does not match command sent.",
            VPR_LOCATION);
      }
   }
   return response[response.size()-1];
}

void Ether24Standalone::setState(const Elexol::Port port, const Elexol::CommandType command,
                                 const vpr::Uint8 value) const
{
   vprASSERT(command == Elexol::Command::Value ||
             command == Elexol::Command::Direction ||
             command == Elexol::Command::PullUp ||
             command == Elexol::Command::Threshold ||
             command == Elexol::Command::SchmittTrigger);

   //std::vector<vpr::Uint8> cmd(0);
   std::string cmd;
   if (Elexol::Command::Value != command)
   {
      cmd.push_back(command);
   }

   vpr::Uint8 port_address = Elexol::Command::PortWriteOffset + port;
   cmd.push_back(port_address);

   // Attach the actual data.
   cmd.push_back(value);

   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
      << "Ether24Standalone::setState() Sending command [" << cmd << "]"
      << std::endl << vprDEBUG_FLUSH;

   vpr::Uint32 bytes_sent;
   mSocket->sendto(cmd, cmd.size(), mAddress, bytes_sent);
}

vpr::InetAddr Ether24Standalone::getFixedIpAddress()
{
   std::pair<vpr::Uint8, vpr::Uint8> bytes_a =
      getWordValue(Elexol::Address::IpAddressA);
   std::pair<vpr::Uint8, vpr::Uint8> bytes_b =
      getWordValue(Elexol::Address::IpAddressB);

   /**
    * The Elexol driver seems to be using a middle-endian memory layout.
    * The address: 192.168.0.7
    *
    * Memory Layout:
    * 
    *  Word  MSB  LSB
    *   6    168  192
    *   7     7    0
    */

   vpr::Uint32 address_value(0);
   address_value = (bytes_a.second << 24) | (bytes_a.first << 16) |
                   (bytes_b.second << 8) | (bytes_b.first);

   vpr::InetAddr address;
   address.setAddress(address_value, 2424);
   return address;
}

void Ether24Standalone::setFixedIpAddress(const vpr::InetAddr& ipAddr)
{
   vpr::Uint32 network_address(ipAddr.getAddressValue());
   //vpr::Uint32 network_address(ipAddr.getAddressValue());

   vpr::Uint8 b3 = (network_address >> 24) & 0xFF;
   vpr::Uint8 b2 = (network_address >> 16) & 0xFF;
   vpr::Uint8 b1 = (network_address >> 8) & 0xFF;
   vpr::Uint8 b0 = (network_address >> 0) & 0xFF;

   // IP address in network order is: 6.0.168.192
   std::cout << "New Address: ["
      << (int)b3 << "]["
      << (int)b2 << "]["
      << (int)b1 << "]["
      << (int)b0 << "]" << std::endl;

   /**
    * The Elexol driver seems to be using a middle-endian memory layout.
    * The address: 192.168.0.7
    *
    * Memory Layout:
    * 
    *  Word  MSB  LSB
    *   6    168  192
    *   7     7    0
    */

   enableWriting();
   setWordValue(Elexol::Address::IpAddressA,
      std::pair<vpr::Uint8, vpr::Uint8>(b2, b3));
   vpr::System::msleep(50);

   setWordValue(Elexol::Address::IpAddressB,
      std::pair<vpr::Uint8, vpr::Uint8>(b0, b1));
   vpr::System::msleep(50);
   disableWriting();
}

void Ether24Standalone::setEnableFixedIpAddress(bool val)
{
   vpr::Uint8 control_bits = getByteValue(Elexol::Address::ControlBits1);

   // If we are enabling fixed IP address, clear bit 0
   // otherwise set it high.
   if (val)
   { control_bits &= 0xFE; }
   else
   { control_bits |= 0x01; }

   enableWriting();
   setByteValue(Elexol::Address::ControlBits1, control_bits);
   vpr::System::msleep(50);
   disableWriting();
}

vpr::Uint8 Ether24Standalone::getByteValue(const Elexol::AddressType addr)
{
   vpr::Uint8 memory_address = addr/2;
   bool is_msb = addr%2;
   //std::cout << "Mem: " << (int)memory_address << " " << (is_msb ? "MSB" : "LSB") << std::endl;

   std::pair<vpr::Uint8, vpr::Uint8> word_value =
      getWordValue(memory_address);

   if (is_msb)
   { return word_value.first; }
   else
   { return word_value.second; }
}

void Ether24Standalone::setByteValue(const Elexol::AddressType addr,
                                     const vpr::Uint8 value)
{
   vpr::Uint8 memory_address = addr/2;
   bool is_msb = addr%2;
   //std::cout << "Mem: " << (int)memory_address << " " << (is_msb ? "MSB" : "LSB") << std::endl;

   std::pair<vpr::Uint8, vpr::Uint8> word_value =
      getWordValue(memory_address);

   if (is_msb)
   { word_value.first = value; }
   else
   { word_value.second = value; }

   setWordValue(memory_address, word_value);
}


std::pair<vpr::Uint8, vpr::Uint8>
   Ether24Standalone::getWordValue(const Elexol::AddressType address)
{
   //std::vector<vpr::Uint8> read_cmd(0);
   std::string read_cmd;
   read_cmd.push_back(Elexol::Command::EEPROM);
   read_cmd.push_back(Elexol::Command::ReadWord);
   read_cmd.push_back(address);
   read_cmd.push_back(0x0);
   read_cmd.push_back(0x0);

   vprASSERT(5 == read_cmd.size());

   //vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
   //vprDEBUG(vprDBG_ALL, 0)
   //   << "Ether24Standalone::getWordValue(): Sending command [" << read_cmd << "]"
   //   << std::endl << vprDEBUG_FLUSH;

   vpr::Uint32 bytes_sent;
   mSocket->sendto(read_cmd, 5, mAddress, bytes_sent);

   vpr::InetAddr from;
   vpr::Uint32 bytes_read;
   std::vector<vpr::Uint8> response(0);

   vpr::Interval read_timeout(50, vpr::Interval::Msec);
   mSocket->recvfrom(response, 4, from, bytes_read, read_timeout);

   if (Elexol::Command::ReadWord != response[0])
   {
      throw Elexol::ElexolException("Response does not match command sent.",
         VPR_LOCATION);
   }
   if (address != response[1])
   {
      throw Elexol::ElexolException("Response address does not match requested address.",
         VPR_LOCATION);
   }
   //std::cout << "Response [" << (int)response[2] << "][" << (int)response[3] << "]" << std::endl;
   return std::pair<vpr::Uint8, vpr::Uint8>(response[2], response[3]);
}

void Ether24Standalone::enableWriting()
{
   std::string write_enable_cmd;
   write_enable_cmd.push_back(Elexol::Command::EEPROM);
   write_enable_cmd.push_back(Elexol::Command::WriteEnable);
   write_enable_cmd.push_back(0x0);
   write_enable_cmd.push_back((char) 0xAA);
   write_enable_cmd.push_back((char) 0x55);
   vprASSERT(5 == write_enable_cmd.size());
   
   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
      << "Ether24StandalonegetState(): Sending enable writing command ["
      << write_enable_cmd << "]" << std::endl << vprDEBUG_FLUSH;

   vpr::Uint32 bytes_sent;
   mSocket->sendto(write_enable_cmd, 5, mAddress, bytes_sent);
}

void Ether24Standalone::disableWriting()
{
   std::string write_disable_cmd;
   write_disable_cmd.push_back(Elexol::Command::EEPROM);
   write_disable_cmd.push_back(Elexol::Command::WriteDisable);
   write_disable_cmd.push_back(0x0);
   write_disable_cmd.push_back(0x0);
   write_disable_cmd.push_back(0x0);
   vprASSERT(5 == write_disable_cmd.size());

   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
      << "Ether24Standalone::disableWriting(): Sending disable writing command ["
      << write_disable_cmd << "]" << std::endl << vprDEBUG_FLUSH;

   vpr::Uint32 bytes_sent;
   mSocket->sendto(write_disable_cmd, 5, mAddress, bytes_sent);
}

void Ether24Standalone::setWordValue(const Elexol::AddressType address,
   std::pair<vpr::Uint8, vpr::Uint8> value)
{
   //std::vector<vpr::Uint8> write_cmd(0);
   std::string write_cmd;
   write_cmd.push_back(Elexol::Command::EEPROM);
   write_cmd.push_back(Elexol::Command::WriteWord);
   write_cmd.push_back(address);
   write_cmd.push_back(value.first);
   write_cmd.push_back(value.second);

   vprASSERT(5 == write_cmd.size());

   vpr::Uint32 bytes_sent;
   
   //vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
   //vprDEBUG(vprDBG_ALL, 0)
   //   << "Ether24Standalone::setWordValue(): Sending command [" << write_cmd << "]"
   //   << std::endl << vprDEBUG_FLUSH;
   mSocket->sendto(write_cmd, 5, mAddress, bytes_sent);
}

/*
// TODO: setCause(ex)
);
*/
