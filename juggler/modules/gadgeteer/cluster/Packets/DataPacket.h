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

#ifndef _GADGET_RIM_DATA_PACKET_H
#define _GADGET_RIM_DATA_PACKET_H

#include <gadget/gadgetConfig.h>
#include <vpr/vprTypes.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <cluster/Packets/Header.h>                                                       
#include <cluster/Packets/Packet.h>

//#define RIM_PACKET_HEAD_SIZE 8

namespace cluster
{

class GADGET_CLASS_API DataPacket : public Packet
{
public:
   DataPacket();

   DataPacket(const vpr::GUID& plugin_id, const vpr::GUID& device_id, std::vector<vpr::Uint8>* data);
   
   virtual ~DataPacket()
   {
      ;
   }

   /**
    * Helper for the above creation of a device request to be sent
    */
   void serialize();

   /**
    * After reading in the remaining bytes from the socket, create a new parse the data
    */
   virtual void parse(vpr::BufferObjectReader* reader);
   
   virtual void printData(int debug_level);
   
   static vpr::Uint16 getPacketFactoryType()
   {
       return(Header::RIM_DATA_PACKET);
   }
   vpr::GUID getId() { return mDeviceId; }
   
   std::vector<vpr::Uint8>* getDeviceData() { return mDeviceData; }
   
   void setDeviceData(std::vector<vpr::Uint8>* data){ mDeviceData = data; }
private:
   vpr::GUID mDeviceId;
   std::vector<vpr::Uint8>*   mDeviceData;
};
}

#endif




