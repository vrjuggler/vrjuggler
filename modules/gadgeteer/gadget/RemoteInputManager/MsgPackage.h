/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#ifndef GADGET_MSG_PACKAGE_H
#define GADGET_MSG_PACKAGE_H

// The Remote Input Manager allows a proxy or device on one machine to point to a device on
// on another machine

#include <gadget/gadgetConfig.h>
#include <vpr/IO/Socket/SocketStream.h>

namespace gadget
{

// CLASS MsgPackage
// Handles the packaging and unpackaging of network messages for the Remote Input Manager.
// It packages and processes configuration messages for the manager's network connections.

class GADGET_CLASS_API MsgPackage
{
   char mBuffer[256];
   int mDataLength;

   // Mostly used when receiving data
   vpr::Uint16 mOpcode;
   vpr::Uint16 mSenderId;
   vpr::Uint16 mReceiverId;
   std::string mDataString;  // can hold the name of the device being requested

public:
   MsgPackage();

   // send contents of current package to the socket specified
   void sendContents(vpr::SocketStream& sock_stream) const;

   // send contents of current package to the socket specified
   // and then clear the message
   void sendAndClear(vpr::SocketStream& sock_stream);

   // clear our current message buffer
   void clear()
   {
      mDataLength = 0;
   }

   // The sender and receiver ids found inside messages
   vpr::Uint16 getSenderId()
   {
      return mSenderId;
   }
   vpr::Uint16 getReceiverId()
   {
      return mReceiverId;
   }

   // GetDataString
   // When data is part of an incoming message, this class parses the message
   //   and stores the data. it can be retrieved with the following two functions
   //   Not used in all messages.
   const std::string& getDataString()
   {
      return mDataString;
   }
   int getDataLength() const
   {
      return mDataLength;
   }

   // creates a device request message with the specified id and name
   void createDeviceRequest(vpr::Uint16 device_id, const std::string device_name);

   // creates a device acknowledgement message with the specified id and name
   void createDeviceAck(const vpr::Uint16 remote_device_id,
                        const vpr::Uint16 local_device_id,
                        const std::string device_name);

   // creates a rejection (negative acknowledgement) message with the specified id and name
   // for simplicity, same as ACK, but with different opcode
   void createDeviceNack(const vpr::Uint16 remote_device_id,
                         const vpr::Uint16 local_device_id,
                         const std::string device_name);

   // processes a device request message
   // returns the number of bytes used from buffer (excluding the already read opcode)
   // A return value of zero means unable to process -- complete message not received yet.
   int receiveDeviceRequest(char* ptr, int len);

   // This works for both ACK and NACK
   // returns the number of bytes used from buffer (excluding the already read opcode)
   // A return value of zero means unable to process -- complete message not received yet.
   int receiveDeviceAck(char* ptr, int len);

   void createClockSync(const float& time_a, const float& time_b, const bool clock_is_synced);
   int receiveClockSync(char* ptr, int len, float& time_a, float& time_b, bool& clock_is_synced);
   void createClockSyncHaveSrc(const vpr::GUID& id, const bool clock_is_synced);
   int receiveClockSyncHaveSrc(char* ptr, int len, vpr::GUID& id, bool& clock_is_synced);



};

}  // end namespace gadget

#endif


