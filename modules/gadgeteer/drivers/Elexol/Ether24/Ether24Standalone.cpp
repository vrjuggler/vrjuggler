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
 * File:          $RCSfile: Ether24Standalone.cpp,v $
 * Date modified: $Date: 2006/01/08 20:28:21 $
 * Version:       $Revision: 1.12 $
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <string>

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
   vpr::Interval read_timeout(500, vpr::Interval::Msec);


   Elexol::device_map_t mac_to_addr_map;

   vprDEBUG_BEGIN(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "=== Elexol Ether I/O 24 Devices Found ==="
      << std::endl << vprDEBUG_FLUSH;
   try
   {
      while (true)
      {
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
   catch (vpr::TimeoutException& ex)
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
   vpr::Interval read_timeout(500, vpr::Interval::Msec);
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

/*
// TODO: setCause(ex)
);
*/
