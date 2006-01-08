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

#ifndef _GADGET_RIM_DATA_PACKET_H
#define _GADGET_RIM_DATA_PACKET_H

#include <gadget/gadgetConfig.h>
#include <vpr/vprTypes.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <cluster/Packets/Header.h>
#include <cluster/Packets/Packet.h>

namespace cluster
{

/** \class DataPacket DataPacket.h cluster/Packets/DataPacket.h
 *
 * Cluster data packet.
 */
class GADGET_CLASS_API DataPacket : public Packet
{
public:
   /**
    * Default constructor used by the PacketFactory.
    */
   DataPacket();

   /**
    * Create a DataPacket to send raw data accross the network.
    *
    * @param plugin_id GUID of the ClusterPlugin that should handle this
    *                  packet.
    * @param object_id GUID of the object that we are acknowledging.
    * @param data      Pointer to the raw data that we want to send across
    *                  the network
    */
   DataPacket(const vpr::GUID& plugin_id, const vpr::GUID& object_id,
              std::vector<vpr::Uint8>* data);

   /**
    * Clean up all unused memory.
    */
   virtual ~DataPacket()
   {
      delete mDeviceData;
   }

   /**
    * Serializes member variables into a data stream.
    */
   void serialize();

   /**
    * Parses the data stream into the local member variables.
    */
   virtual void parse(vpr::BufferObjectReader* reader);

   /**
    * Print the data to the screen in a readable form.
    */
   virtual void printData(int debug_level);

   /**
    * Return the type of this packet.
    */
   static vpr::Uint16 getPacketFactoryType()
   {
      return(Header::RIM_DATA_PACKET);
   }

   /**
    * Return the GUID of the object that we are sending raw data for.
    */
   vpr::GUID getObjectId()
   {
      return mObjectId;
   }

   /**
    * Return a pointer to the raw data that we are sending across the network
    */
   std::vector<vpr::Uint8>* getDeviceData()
   {
      return mDeviceData;
   }

private:
   vpr::GUID                  mObjectId;     /**< GUID of the object that we are sending raw data for. */
   std::vector<vpr::Uint8>*   mDeviceData;   /**< Raw data that we are sending across the network. */
};

}// end namespace cluster


#endif
