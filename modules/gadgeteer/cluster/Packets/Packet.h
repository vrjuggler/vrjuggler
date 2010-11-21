/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_RIM_PACKET_H
#define _GADGET_RIM_PACKET_H

#include <gadget/gadgetConfig.h>

#include <boost/noncopyable.hpp>
#include <vpr/vprTypes.h>
#include <vpr/Util/GUID.h>

#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <cluster/ClusterException.h>

#include <cluster/Packets/HeaderPtr.h>

namespace cluster
{

/** \class Packet Packet.h cluster/Packets/Packet.h
 *
 * Cluster packet base type.
 */
class GADGET_CLASS_API Packet
   : boost::noncopyable
{
protected:
   /**
    * Create a new empty packet.
    */
   Packet(const vpr::GUID& pluginId);

   /**
    * Copy the given data, and parse the header.
    */
   Packet(std::vector<vpr::Uint8>* data);

public:
   /**
    * Free all memory used by internal data.
    */
   virtual ~Packet();

   /**
    * Dump all internal data to the screen.
    */
   void dump() const;

   /**
    * Get the type of this packet.
    */
   vpr::Uint16 getPacketType() const;

   /**
    * Print the internal data in a readable format.
    */
   virtual void printData(int debugLevel) const;

   /**
    * Return the vpr::PacketReader object used to retrieve data from the packet.
    */
   vpr::BufferObjectReader* getPacketReader() const
   {
      return mPacketReader;
   }

   /**
    * Return the vpr::PacketReader object used to retrieve data from the packet.
    */
   vpr::BufferObjectWriter* getPacketWriter() const
   {
      return mPacketWriter;
   }

   /**
    * Get the GUID of the Plugin that should handle this packet.
    */
   const vpr::GUID& getPluginId() const
   {
      return mPluginId;
   }

   /**
    * Get the header for this packet.
    */
   HeaderPtr getHeader() const
   {
      return mHeader;
   }

   /**
    * Get a std::vector containing all internal data.
    */
   std::vector<vpr::Uint8>& getData()
   {
      return mData;
   }

   void setHeader(HeaderPtr head)
   {
      mHeader = head;
   }

   virtual void parse() = 0;
protected:
   HeaderPtr mHeader;                        /**< Header used to specify the type/size of this packet.*/
   vpr::BufferObjectReader* mPacketReader;   /**< ObjectReader that is used to parse all data. */
   vpr::BufferObjectWriter* mPacketWriter;   /**< ObjectWriter that is used to serialize all data. */
   std::vector<vpr::Uint8> mData;            /**< std::vector which contains all internal data */
   vpr::GUID mPluginId;                      /**< GUID that specifies which plugin is responsible for this packet */
};
}

#endif
