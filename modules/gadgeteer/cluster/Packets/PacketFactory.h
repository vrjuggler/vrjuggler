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

#ifndef _CLUSTER_PACKET_FACTORY_H_
#define _CLUSTER_PACKET_FACTORY_H_

#include <gadget/gadgetConfig.h>
#include <string>
#include <vpr/Util/Factory.h>
#include <vpr/Util/Singleton.h>
#include <cluster/Packets/Packet.h>

/**
 * Registers a creator for the Packet base class.
 *
 * @pre Requires that the method std::string getPacketFactoryType() be defined for
 *      class PacketType.
 *
 * Ex: CLUSTER_REGISTER_CLUSTER_PACKET_CREATOR(ConnectionAck)
 */
#define CLUSTER_REGISTER_CLUSTER_PACKET_CREATOR(PacketType) \
class PacketType; \
const bool reg_ctr_ ## PacketType = \
   cluster::PacketFactory::instance()-> \
      registerCreator(PacketType::getPacketFactoryType(), \
                      vpr::CreateProduct<cluster::Packet, PacketType>);

namespace cluster
{

/** \class PacketFactory PacketFactory.h cluster/Packets/PacketFactory.h
 *
 * Cluster packet factory.
 */
class GADGET_CLASS_API PacketFactory :
   public vpr::Factory<Packet, vpr::Uint16>
{
public:
   vprSingletonHeader(PacketFactory);
};

} // End of cluster namespace

#endif
