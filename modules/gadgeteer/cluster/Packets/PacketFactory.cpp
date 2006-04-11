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

#include <gadget/gadgetConfig.h>
#include <typeinfo>

#include <cluster/Packets/PacketFactory.h>
#include <cluster/Packets/Packet.h>
#include <cluster/Packets/DeviceRequest.h>
#include <cluster/Packets/DeviceAck.h>
#include <cluster/Packets/ConnectionRequest.h>
#include <cluster/Packets/ConnectionAck.h>
#include <cluster/Packets/SyncRequest.h>
#include <cluster/Packets/SyncAck.h>
#include <cluster/Packets/ApplicationDataRequest.h>
#include <cluster/Packets/ApplicationDataAck.h>
#include <cluster/Packets/DataPacket.h>
#include <cluster/Packets/EndBlock.h>
#include <cluster/Packets/StartBlock.h>
#include <gadget/Util/Debug.h>


namespace cluster
{

// Initialize the singleton ptr
//vjDeviceFactory* DeviceFactory::mInstance = NULL;
//vjSingletonImp( DeviceFactory ); //kevin
vprSingletonImpWithInitFunc( PacketFactory, loadKnownPackets );

/**
 * Registers all the devices that I know about.
 * @note This should really be moved to dynamic library loading code.
 */
void PacketFactory::loadKnownPackets()
{
   // NOTE: These will all given unused variable errors in compiling.
   // That is okay, because the don't actually have to do anything.
   // They just register themselves in their constructor.

   
   PacketConstructor<DeviceRequest>* device_request
      = new PacketConstructor<DeviceRequest>;

   PacketConstructor<DeviceAck>* device_ack
      = new PacketConstructor<DeviceAck>;
   
   PacketConstructor<ConnectionRequest>* connection_request
      = new PacketConstructor<ConnectionRequest>;
   
   PacketConstructor<ConnectionAck>* connection_ack
   = new PacketConstructor<ConnectionAck>;
   
   PacketConstructor<SyncRequest>* sync_request
      = new PacketConstructor<SyncRequest>;
   
   PacketConstructor<SyncAck>* sync_ack
      = new PacketConstructor<SyncAck>;
   
   PacketConstructor<DataPacket>* data_packet
      = new PacketConstructor<DataPacket>;
   
   PacketConstructor<ApplicationDataRequest>* appdata_request
      = new PacketConstructor<ApplicationDataRequest>;

   PacketConstructor<ApplicationDataAck>* appdata_ack
      = new PacketConstructor<ApplicationDataAck>;
   
   PacketConstructor<EndBlock>* end_block
      = new PacketConstructor<EndBlock>;

   PacketConstructor<StartBlock>* start_block
   = new PacketConstructor<StartBlock>;


   if( (NULL == device_request)     ||
       (NULL == device_ack)         ||
       (NULL == connection_request) ||
       (NULL == connection_ack)     ||
       (NULL == sync_request)       ||
       (NULL == sync_ack)           ||
       (NULL == appdata_request)   ||
       (NULL == appdata_ack)       ||
       (NULL == data_packet)        ||
       (NULL == end_block)          ||
       (NULL == start_block) )
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n" << vprDEBUG_FLUSH;
   }
}

void PacketFactory::registerPacket(PacketConstructorBase* constructor)
{
   vprASSERT(constructor != NULL);
   mConstructors.push_back(constructor);     // Add the constructor to the list
}

// Simply query all device constructors registered looking
// for one that knows how to load the device
bool PacketFactory::recognizePacket(vpr::Uint16 base_type)
{
   if(findConstructor(base_type) == -1)
      return false;
   else
      return true;
}

/*Packet* PacketFactory::loadPacket(vpr::Uint16 base_type)
{
   vprASSERT(recognizePacket(base_type));

   int index = findConstructor(base_type);

   Packet* new_dev;
   PacketConstructorBase* constructor = mConstructors[index];

   //vprDEBUG(gadgetDBG_INPUT_MGR,3)
   //   << "gadget::BaseType::loadDevice: Loading device: "
   //   << chunk->getDescToken() << "  with: " << typeid(*constructor).name()
   //   << std::endl << vprDEBUG_FLUSH;

   new_dev = constructor->createPacket(base_type);
   if(new_dev!=NULL)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[Packet Factory] Found the Packet Type\n"<< vprDEBUG_FLUSH;
   }
   return new_dev;
}*/

/**
 * Loads the specified device.
 */
Packet* PacketFactory::recvPacket(vpr::SocketStream* stream)
{
   // - Read in header
   // - Get Constructor for correct PacketType
   // - Call constructor
   // - Return finished packet

   Header* testing = new Header(stream);
   
   vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "\n\nFACTORY is trying to make a packet type: " 
      << testing->getPacketType() << std::endl << vprDEBUG_FLUSH;

   vprASSERT("Packet Type is NOT Valid!" && recognizePacket(testing->getPacketType()));

   int index = findConstructor(testing->getPacketType());

   Packet* new_dev;
   PacketConstructorBase* constructor = mConstructors[index];

   //vprDEBUG(gadgetDBG_INPUT_MGR,3)
   //   << "gadget::BaseType::loadDevice: Loading device: "
   //   << chunk->getDescToken() << "  with: " << typeid(*constructor).name()
   //   << std::endl << vprDEBUG_FLUSH;

   new_dev = constructor->createPacket(testing, stream);
   if(new_dev!=NULL)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[Packet Factory] Found the Packet Type\n"<< vprDEBUG_FLUSH;
   }
   return new_dev;
}

int PacketFactory::findConstructor(vpr::Uint16 base_type)
{
   for(unsigned int i=0;i<mConstructors.size();i++)
   {
      // Get next constructor
      PacketConstructorBase* construct = mConstructors[i];
      vprASSERT(construct != NULL);
      if(construct->getBaseType() == base_type)
         return i;
   }

   return -1;
}


void PacketFactory::debugDump()
{
   vprDEBUG_OutputGuard(gadgetDBG_RIM, 0,
      std::string("gadget::PacketFactory::debugDump\n"),
      std::string("------ END DUMP ------\n"));

   vprDEBUG(gadgetDBG_RIM,0) << "num constructors:"
                             << mConstructors.size() << "\n"
                             << vprDEBUG_FLUSH;

   for(unsigned int cNum=0;cNum<mConstructors.size();cNum++)
   {
      PacketConstructorBase* dev_constr = mConstructors[cNum];
      vprDEBUG(gadgetDBG_RIM,0)
         << cNum << ": Constructor:" << (void*)dev_constr
         << "   type:" << typeid(*dev_constr).name() << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,0) << "   recog:"
                                << dev_constr->getBaseType() << "\n"
                                << vprDEBUG_FLUSH;
   }
}

} // End of gadget namespace
