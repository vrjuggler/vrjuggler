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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_EVENT_PACKET_H
#define _GADGET_EVENT_PACKET_H

#include <cluster/PluginConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <cluster/Packets/Header.h>
#include <cluster/Packets/Packet.h>
#include <plugins/EventPlugin/EventPacketPtr.h>

namespace cluster
{

/** \class EventPacket EventPacket.h cluster/Packets/EventPacket.h
 *
 * Cluster data packet.
 */
class GADGET_CLUSTER_PLUGIN_CLASS_API EventPacket
   : public Packet
{
public:
   static const unsigned short EVENT_PACKET = 501;

protected:
   /**
    * Default constructor used by the PacketFactory.
    */
   EventPacket();

   /**
    * Create a EventPacket that will be used to provide synchronization.
    *
    * @param plugin_id GUID of the ClusterPlugin that should handle this
    *                  packet.
    * @param name      Name of the event of interest.
    */
   EventPacket(const vpr::GUID& pluginId, const std::string& name);

public:
   /**
    * Creates a EventPacket instance and returns it wrapped in a
    * EventPacketPtr object.
    *
    * @since 1.3.7
    */
   static EventPacketPtr create();

   /**
    * Creates a EventPacket instance and returns it wrapped in a
    * EventPacketPtr object.
    *
    * @since 1.3.7
    */
   static EventPacketPtr create(const vpr::GUID& pluginId, const std::string& name);

   /**
    * Clean up all unused memory.
    */
   virtual ~EventPacket();

   /**
    * Serializes member variables into a data stream.
    */
   void serialize();

   /**
    * Parses the data stream into the local member variables.
    */
   virtual void parse();

   /**
    * Print the data to the screen in a readable form.
    */
   virtual void printData(int debugLevel) const;

   /**
    * Return the type of this packet.
    */
   static vpr::Uint16 getPacketFactoryType()
   {
      return EVENT_PACKET;
   }

   /**
    * Return the GUID of the object that we are sending raw data for.
    */
   const std::string& getName() const
   {
      return mName;
   }

private:
   std::string mName; /**< Name of the event. */
};

}// end namespace cluster


#endif
