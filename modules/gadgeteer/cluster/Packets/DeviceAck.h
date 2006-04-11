/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

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

#ifndef _GADGET_RIM_DEVICE_ACK_H
#define _GADGET_RIM_DEVICE_ACK_H

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <gadget/gadgetConfig.h>
#include <cluster/Packets/Header.h>                                                       
#include <cluster/Packets/Packet.h>

namespace cluster
{

class DeviceAck : public Packet
{
public:
   /**
    * packet_head: Given a packet that has been parsed, and found to be a device request
    * stream: A socket that the connection is on
    * 
    * Create a deviceRequest packet
    */
   DeviceAck(Header* packet_head, vpr::SocketStream* stream);

   // =============== Packet Specific =================
   //

   /**
    * Given a sender ID(self) and a requested device name
    *
    * Create a device request to be sent
    */
   DeviceAck(vpr::GUID& id, std::string& device_name, 
                std::string& device_base_type, bool ack);
   
   //
   // =============== Packet Specific =================

   /**
    * Helper for the above creation of a device request to be sent
    */
   void serialize();

   /**
    * After reading in the remaining bytes from the socket, create a new parse the data
    */
   void parse();
   
   virtual void printData(int debug_level);
   static vpr::Uint16 getBaseType()
   {
       return(Header::RIM_DEVICE_ACK);
   }
   
   // =============== Packet Specific =================
   //
   
   vpr::GUID getId() { return mId; }
   std::string getDeviceName() { return mDeviceName; }
   std::string getDeviceBaseType() { return mDeviceBaseType; }
   bool getAck() { return mAck; }
   
   //
   // =============== Packet Specific =================

   virtual bool action(ClusterNode* node);

private:
   // =============== Packet Specific =================
   //
   vpr::GUID   mId;
   std::string mDeviceName;
   std::string mDeviceBaseType;
   std::string mHostname;
   // true = ACK  & false = NACK
   bool        mAck;    
   //
   // =============== Packet Specific =================

};
}

#endif




