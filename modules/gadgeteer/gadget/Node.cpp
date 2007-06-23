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

#include <gadget/gadgetConfig.h>

#include <boost/bind.hpp>

#include <vpr/IO/Socket/SocketStream.h>

#include <gadget/Node.h>
#include <gadget/NetworkManager.h>
#include <cluster/Packets/Packet.h>
#include <cluster/Packets/DataPacket.h>
#include <cluster/Packets/PacketFactory.h>
#include <cluster/ClusterManager.h>
#include <cluster/ClusterDelta.h>

#include <gadget/Util/Debug.h>
#include <gadget/InputManager.h>

#include <jccl/RTRC/ConfigManager.h>

namespace gadget
{

Node::Node(const std::string& name, const std::string& hostName, 
           const vpr::Uint16 port, vpr::SocketStream* socketStream)
   : mName(name)
   , mHostname(hostName)
   , mPort(port)
   , mSockStream(socketStream)
   , mStatus(DISCONNECTED)
   , mUpdated(false)
{
   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      << clrOutBOLD(clrBLUE,"[Node]")
      << " Created a Node: " << name << " - " << mHostname
      << std::endl << vprDEBUG_FLUSH;
}

NodePtr Node::create(const std::string& name, const std::string& hostName, 
                     const vpr::Uint16 port, vpr::SocketStream* socketStream)
{
   return NodePtr(new Node(name, hostName, port, socketStream));
}

Node::~Node()
{
   shutdown();
}

void Node::shutdown()
{
   setStatus(DISCONNECTED);

   if (NULL != mSockStream)
   {
      if(mSockStream->isOpen())
      {
         mSockStream->close();
      }
      delete mSockStream;
      mSockStream = NULL;
   }
}

void Node::debugDump(int debug_level)
{

   vpr::DebugOutputGuard dbg_output(gadgetDBG_NET_MGR, debug_level,
                              std::string("-------------- Node --------------\n"),
                              std::string("-----------------------------------------\n"));

   vprDEBUG(gadgetDBG_NET_MGR, debug_level) << "Node Name: "
      << mName << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_NET_MGR, debug_level) << "Hostname:  "
      << mHostname << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_NET_MGR, debug_level) << "Port:      "
      << mPort << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_NET_MGR, debug_level) << "SockStream "
      << (NULL == mSockStream ? "is NULL" : "is NOT NULL") << std::endl << vprDEBUG_FLUSH;
   if (CONNECTED == getStatus())
   {
      vprDEBUG(gadgetDBG_NET_MGR, debug_level) << clrOutBOLD(clrGREEN,"CONNECTED") << std::endl << vprDEBUG_FLUSH;
   }
   else if (NEWCONNECTION == getStatus())
   {
      vprDEBUG(gadgetDBG_NET_MGR, debug_level) << clrOutBOLD(clrRED,"NEW CONNECTION") << std::endl << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(gadgetDBG_NET_MGR, debug_level) << clrOutBOLD(clrRED,"DISCONNECTED") << std::endl << vprDEBUG_FLUSH;
   }
}

void Node::printStats(int debug_level)
{
   vpr::BaseIOStatsStrategy* stats = mSockStream->getIOStatStrategy();
   vpr::BandwidthIOStatsStrategy* bw_interface = dynamic_cast<vpr::BandwidthIOStatsStrategy*>(stats);

   if(bw_interface != NULL)
   {
      // Dump out write stats
      vprDEBUG(gadgetDBG_RIM,debug_level) << "Socket Write bandwidth stats ---" << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) << "stats type: " << typeid(stats).name() << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) << "      sent bytes: " << bw_interface->writeStats().getTotal() << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) << "         av send: " << bw_interface->writeStats().getMean()/1024.0f << " k/s" << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) << "        STA send: " << bw_interface->writeStats().getSTA()/1024.0f << " k/s" << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) << "       Inst send: " << bw_interface->writeStats().getInstAverage()/1024.0f << " k/s" << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) << "    Max STA send: " << bw_interface->writeStats().getMaxSTA()/1024.0f << " k/s" << std::endl << std::endl << vprDEBUG_FLUSH;

      vprDEBUG(gadgetDBG_RIM,debug_level) << "      read bytes: " << bw_interface->readStats().getTotal() << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) << "         av read: " << bw_interface->readStats().getMean()/1024.0f << " k/s" << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) << "        STA read: " << bw_interface->readStats().getSTA()/1024.0f << " k/s" << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) << "       Inst read: " << bw_interface->readStats().getInstAverage()/1024.0f << " k/s" << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) << "    Max STA read: " << bw_interface->readStats().getMaxSTA()/1024.0f << " k/s" << std::endl << vprDEBUG_FLUSH;

   }
   else
   {
      vprDEBUG(gadgetDBG_RIM,debug_level) << "SocketBWTest: Don't have BW Stats on stats. type is: " << typeid(stats).name() << std::endl << vprDEBUG_FLUSH;
   }

}

void Node::setStatus(int connect)
{
   vpr::Guard<vpr::Mutex> guard(mStatusLock);

   if (mStatus == CONNECTED && connect == DISCONNECTED)
   {
      //TODO: ADD This back in SOON
      //ClusterManager::instance()->recoverFromLostNode(this);
   }

   mStatus = connect;
}

bool Node::send(cluster::PacketPtr outPacket)
{
   vprASSERT(NULL != outPacket.get() && "Can not send a NULL packet.");

   vpr::Guard<vpr::Mutex> guard(mSockWriteLock);

   cluster::HeaderPtr header = outPacket->getHeader();

   vprASSERT(NULL != header && "Node::send() - Can't have a NULL header.");
   vprASSERT(NULL != mSockStream && "Node::send() - SocketStream can't be NULL");

   // -Send header data
   // -Send packet data
   try
   {
      header->send(mSockStream);
   }
   catch (vpr::IOException&)
   {
      // TODO: setCause(ex)
      throw cluster::ClusterException("Packet::recv() - Sending Header Data failed!");
   }

   // Early out if we only have a header.
   if(header->getPacketLength() == cluster::Header::RIM_PACKET_HEAD_SIZE)
   {
      return true;
   }

   try
   {
      mSockStream->send(outPacket->getData(),
         header->getPacketLength() - cluster::Header::RIM_PACKET_HEAD_SIZE);
   }
   catch (vpr::IOException&)
   {
      // TODO: setCause(ex)
      throw cluster::ClusterException("Packet::recv() - Sending data packet failed!!");
   }

   return true;
}

cluster::PacketPtr Node::recvPacket()
{
   // - Read in header
   // - Get Constructor for correct PacketType
   // - Call constructor
   // - Read in Packet data
   // - Parse data into new packet
   // - Return finished packet

   vpr::Guard<vpr::Mutex> guard(mSockReadLock);

   cluster::HeaderPtr packet_head = cluster::Header::create();

   try
   {
      packet_head->readData(mSockStream);
   }
   catch (vpr::IOException& ex)
   {
      vprDEBUG( gadgetDBG_RIM, vprDBG_HVERB_LVL )
         << clrOutNORM(clrRED, "ERROR: ")
         << "Node::recvPacket() Could not read the header from the socket." << std::endl
         << ex.what() << std::endl << vprDEBUG_FLUSH;
      throw ex;
   }

   vprDEBUG( gadgetDBG_RIM, vprDBG_HVERB_LVL )
      << "Node::recvPacket() PacketFactory is trying to make a packet type: "
      << packet_head->getPacketType()
      << std::endl << vprDEBUG_FLUSH;

   // Get Packet from factory
   cluster::PacketPtr new_packet =
      cluster::PacketFactory::instance()->createObject( packet_head->getPacketType() );

   // Verify that the packet has been made
   if ( NULL == new_packet.get() )
   {
      throw cluster::ClusterException( "Node::recvPacket() - Packet was not found in Factory." );
   }

   // - Recv the packet data
   //   - Copy over pointer to header
   //   - Continue reading packet from socket

   // Set the header for the new packet.
   new_packet->setHeader( packet_head );

   // Make sure that we are connected.
   if ( NULL == mSockStream )
   {
      vprDEBUG( gadgetDBG_RIM, vprDBG_CRITICAL_LVL )
         << clrOutBOLD( clrRED, "ERROR:" )
         << " mSockSteam is NULL" <<  std::endl << vprDEBUG_FLUSH;
      throw cluster::ClusterException( "Node::recvPacket::recv() - mSocketStream is NULL!" );
   }
   //else if (!mSockStream->isConnected())
   //{
   //   vprDEBUG( gadgetDBG_RIM, vprDBG_CRITICAL_LVL )
   //      << clrOutBOLD( clrRED, "ERROR:" )
   //      << " mSockSteam is not connected."
   //      << std::endl << vprDEBUG_FLUSH;
   //   throw cluster::ClusterException( "Node::recvPacket::recv() - ClusterNode is not connected!" );
   //}
   else
   {
      try
      {
         // Get packet data.
         mSockStream->recvn(
            new_packet->getData(),
            packet_head->getPacketLength() - cluster::Header::RIM_PACKET_HEAD_SIZE
         );
      }
      catch (vpr::IOException&)
      {
         vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
            << clrOutBOLD( clrRED, "ERROR:" )
            << " Reading packet data failed. Expecting: "
            << packet_head->getPacketLength() - cluster::Header::RIM_PACKET_HEAD_SIZE
            << " bytes" << std::endl << vprDEBUG_FLUSH;

         // TODO: setCause(ex)
         throw cluster::ClusterException( "Node::recvPacket() - Reading packet data failed!" );
      }
   }

   // Parse Packet with new data
   new_packet->parse();

   return new_packet;
}

}  // end namespace gadget
