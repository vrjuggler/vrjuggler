/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _GADGET_CONFIG_PACKET_ACK_H
#define _GADGET_CONFIG_PACKET_ACK_H

#include <gadget/gadgetConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <cluster/Packets/Header.h>
#include <cluster/Packets/Packet.h>
#include <cluster/Packets/ConfigPacketPtr.h>

namespace cluster
{

/** \class ConfigPacket ConfigPacket.h cluster/Packets/ConfigPacket.h
 *
 * Connection acknowledgement packet.
 */
class GADGET_CLASS_API ConfigPacket
   : public Packet
{
protected:
   ConfigPacket();

   /**
    * Create a ConfigPacket packet that contains a confuration change.
    *
    * @param config  Configuration stored as a string.
    * @param type    Type of configuration change, either ADD or REMOVE;
    */
   ConfigPacket(const std::string config, const vpr::Uint16 type);

public:
   /**
    * Creates a ConfigPacket instance and returns it wrapped in a
    * ConfigPacketPtr object.
    *
    * @since 1.3.7
    */
   static ConfigPacketPtr create();

   /**
    * Creates a ConfigPacket instance and returns it wrapped in a
    * ConfigPacketPtr object.
    *
    * @since 1.3.7
    */
   static ConfigPacketPtr create(const std::string config, const vpr::Uint16 type);
   
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
      return(Header::CONFIG_PACKET);
   }

   /**
    * Return the configuration in string form.
    */
   const std::string getConfig() const
   {
      return mConfig;
   }

   /**
    * Return the type of configuration change. Either ADD or REMOVE.
    */
   const vpr::Uint16 getType() const
   {
      return mType;
   }

private:
   std::string mConfig;      /**< Configuration data in string form. */
   vpr::Uint16 mType;        /**< Type of configuration change. ADD/REMOVE */
};

}

#endif
