/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <boost/concept_check.hpp>

//#include <vpr/vpr.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/Util/ReturnStatus.h>
//#include <jccl/Config/ConfigElement.h>
//#include <vpr/Util/Error.h>

#include <gadget/Node.h>
#include <gadget/AbstractNetworkManager.h>
#include <cluster/Packets/Packet.h>
#include <cluster/Packets/ConnectionAck.h>
#include <cluster/Packets/DataPacket.h>
#include <cluster/Packets/PacketFactory.h>
#include <cluster/ClusterManager.h>
#include <cluster/ClusterDelta.h>

#include <gadget/Util/Debug.h>
#include <gadget/InputManager.h>

#include <jccl/RTRC/ConfigManager.h>

namespace gadget
{

Node::Node(const std::string& name, const std::string& host_name, 
                         const vpr::Uint16& port, vpr::SocketStream* socket_stream,
                         AbstractNetworkManager* net_mgr)
   : mRunning(false), mStatus(DISCONNECTED), mUpdateTriggerSema(0),
     mNodeDoneSema(0), mControlThread(NULL), mNetworkManager(net_mgr)
{
   mThreadActive = false;
   mUpdated = false;

   mName = name;
   mHostname = host_name;
   mPort = port;
   mSockStream = socket_stream;
   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      << clrOutBOLD(clrBLUE,"[Node]")
      << " Created a Node: " << name << " - " << host_name
      << std::endl << vprDEBUG_FLUSH;
}

Node::~Node()
{
   shutdown();
}

void Node::shutdown()
{
   setStatus(DISCONNECTED);
   // This may break the accept code since we might not want to delete the Socket.
   // We may be able to just use a smart pointer to point to the SocketStream.
   mRunning = false;
  
   // Make sure that the conrtol loop exits naturally.
   mUpdateTriggerSema.release();
   
   //mNodeDoneSema.acquire();
   
   if (NULL != mSockStream)
   {
      /*
      if(mSockStream->isOpen())
      {
         mSockStream->close();
      }
      delete mSockStream;
      */
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
   vpr::BandwidthIOStatsStrategy* bw_interface = dynamic_cast<vpr::BandwidthIOStatsStrategy*>(stats );

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

void Node::update()
{
   // - If connected() && !updated()
   //   - try recvPacket()
   //     - Catch ClusterException
   //       - set not connected
   //       - add node to connection pending list
   //       - set reconfig needed on reconnect
   //     - If no Exception
   //       - Print Packet Data
   //       - Take the action of the packet

   vprASSERT(isConnected() && "Node is not connected, we can not update!\nWe must not be calling update from the correct location.");
   cluster::Packet* temp_packet = NULL;
   
   temp_packet = recvPacket();
   
   // Print Packet Information
   temp_packet->printData(vprDBG_CONFIG_LVL);

   // Handle the packet correctly
   mNetworkManager->handlePacket(temp_packet,this);

   // Clean up after ourselves
   delete temp_packet;
}

void Node::controlLoop(void* nullParam)
{
   // - Block on an update call
   // - Update Local Data
   // - Send
   // - Signal Sync

   boost::ignore_unused_variable_warning(nullParam);
   
   while( mRunning )
   {
      // Wait for trigger
      if( mRunning )
      {
         mUpdateTriggerSema.acquire();
      }
         
      mUpdated = false;
      while ( mRunning && !mUpdated )
      {
         try
         {
            update();
         }
         catch(cluster::ClusterException cluster_exception)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED)
               << cluster_exception.getMessage() << clrRESET
               << std::endl << vprDEBUG_FLUSH;
            
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) <<
               "Node::update() We have lost our connection to: " << getName() << ":" << getPort()
               << std::endl << vprDEBUG_FLUSH;

            debugDump(vprDBG_CONFIG_LVL);
            
            // Set the Node as disconnected since we have lost the connection
            setStatus(DISCONNECTED);
            
            // Shut down manually instead of calling shutdown since
            // we are in the control thread.
            mRunning = false;
            //if (NULL != mSockStream)
            //{
               //if(mSockStream->isOpen())
               //{
               //   mSockStream->close();
               //}
               //delete mSockStream;
               //mSockStream = NULL;
            //}
         }
      }
      
      // Signal done with Update
      mNodeDoneSema.release();
   }
   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Node: " << getName() << " is stopping."
                                             << std::endl << vprDEBUG_FLUSH;
}

/** Starts the control loop. */
void Node::start()
{
   // --- Setup Multi-Process stuff --- //
   // Create a new thread to handle the control

   if (NULL != mControlThread && mControlThread->valid())
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
            << "Node " << getName() << " already running..."
            << std::endl << vprDEBUG_FLUSH;
      return;
   }

   mRunning = true;

   vpr::ThreadMemberFunctor<Node>* memberFunctor =
      new vpr::ThreadMemberFunctor<Node>(this, &Node::controlLoop, NULL);

   mControlThread = new vpr::Thread(memberFunctor);

   if (mControlThread->valid())
   {
      mThreadActive = true;
   }
   vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
      << "Node " << getName() << " started. thread: "
      << mControlThread << std::endl << vprDEBUG_FLUSH;
}

void Node::signalUpdate()
{
   while(!mThreadActive)
   {
      vprDEBUG(gadgetDBG_RIM, vprDBG_HVERB_LVL) << "Waiting for thread to start ClusterNode::start().\n" << vprDEBUG_FLUSH;
      vpr::Thread::yield();
   }
   //vprDEBUG(gadgetDBG_RIM,/*vprDBG_HVERB_LVL*/1) << getName() << "Signaling ClusterNode\n" << vprDEBUG_FLUSH;
   mUpdateTriggerSema.release();
}

/**
 * Blocks until the end of the frame.
 * @post The frame has been drawn.
 */
void Node::sync()
{
   vprASSERT(mThreadActive == true);
   mNodeDoneSema.acquire();
}

vpr::ReturnStatus Node::send(cluster::Packet* out_packet)
{
   vprASSERT(NULL != out_packet && "Can not send a NULL packet.");

   vpr::Guard<vpr::Mutex> guard(mSockWriteLock);

   // -Send header data
   // -Send packet data

   if (mSockStream == NULL)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << clrOutBOLD(clrRED, "ERROR:")
         << " SocketSteam is NULL" << std::endl << vprDEBUG_FLUSH;
      throw cluster::ClusterException("Node::send() - SocketStream is NULL!");
   }

   cluster::Header* mHeader = out_packet->getHeader();

   if (mHeader == NULL)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << clrOutBOLD(clrRED, "ERROR:")
         << " Packet Header is NULL" << std::endl << vprDEBUG_FLUSH;
      throw cluster::ClusterException("Node::send() - Packet Header is NULL!");
   }

   try
   {
      mHeader->send(mSockStream);
   }
   catch (vpr::IOException& ex)
   {
      // TODO: setCause(ex)
      throw cluster::ClusterException("Packet::recv() - Sending Header Data failed!");
   }

   vpr::Uint32 bytes_written;      

   if(mHeader->getPacketLength() == cluster::Header::RIM_PACKET_HEAD_SIZE)
   {
      return(vpr::ReturnStatus::Succeed);
   }
   
   // If we have a data packet we need to also send the raw data
   if (out_packet->getPacketType() != cluster::Header::RIM_DATA_PACKET)
   {
      std::vector<vpr::Uint8>* packet_data = out_packet->getData();

      try
      {
         mSockStream->send(*packet_data, 
            mHeader->getPacketLength() - cluster::Header::RIM_PACKET_HEAD_SIZE,
            bytes_written);
      }
      catch (vpr::IOException& ex)
      {
         // TODO: setCause(ex)
         throw cluster::ClusterException("Packet::recv() - Sending data packet failed!!");
      }
   }
   else
   {
      std::vector<vpr::Uint8>* packet_data = out_packet->getData();

      // Since we are sending a DataPacket we are not actually sending all data here. We are only sending 2 GUIDs here
      int size = 32;
      
      try
      {
         mSockStream->send(*packet_data, size, bytes_written);
      }
      catch (vpr::IOException& ex)
      {
         // TODO: setCause(ex)
         throw cluster::ClusterException("Packet::recv() - Sending packet failed!!");
      }


      cluster::DataPacket* temp_data_packet = dynamic_cast<cluster::DataPacket*>(out_packet);
      vprASSERT(NULL != temp_data_packet && "Dynamic cast failed!");

      // Testing GUIDs
      /*vpr::BufferObjectReader* testing = new vpr::BufferObjectReader(packet_data);
      vpr::GUID test;
      test.readObject(testing);
      vpr::GUID test2;
      test2.readObject(testing);

      std::cout << "1: " << test.toString() << " 2: " << test2.toString() << std::endl;

      delete testing;
      
      // Testing ID
      testing = new vpr::BufferObjectReader(temp_data_packet->getDeviceData());
      std::cout << "ID: " << (int)testing->readUint16() << std::endl;

      delete testing;
      */
      
      try
      {
         mSockStream->send(*(temp_data_packet->getDeviceData()),temp_data_packet->getDeviceData()->size(),bytes_written);
      }
      catch (vpr::IOException& ex)
      {
         // TODO: setCause(ex)
         throw cluster::ClusterException("Packet::recv() - Sending Packet Data failed!!");
      }
   }
   return vpr::ReturnStatus::Succeed;
}

cluster::Packet* Node::recvPacket()
{
   // - Read in header
   // - Get Constructor for correct PacketType
   // - Call constructor
   // - Read in Packet data
   // - Parse data into new packet
   // - Return finished packet
   
   vpr::Guard<vpr::Mutex> guard(mSockReadLock);
   
   cluster::Header* packet_head = new cluster::Header(mSockStream);
   
   vprDEBUG( gadgetDBG_RIM, vprDBG_HVERB_LVL )
      << "Node::recvPacket() PacketFactory is trying to make a packet type: " 
      << packet_head->getPacketType()
      << std::endl << vprDEBUG_FLUSH;

   // Get Packet from factory
   cluster::Packet* new_packet =
      cluster::PacketFactory::instance()->createObject( packet_head->getPacketType() );

   // Verify that the packet has been made
   if ( NULL == new_packet )
   {
      throw cluster::ClusterException( "Node::recvPacket() - Packet was not found in Factory." );
   }
   
   // - Recv the packet data
   //   - Copy over pointer to header
   //   - Continue reading packet from socket
   
   // Set the header for the new packet.
   new_packet->setHeader( packet_head );
   // Allocate memory for incoming packet.
   std::vector<vpr::Uint8> incoming_data;

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
      vpr::Uint32 bytes_read;	  
   
      try
      {
         // Get packet data.
         mSockStream->recvn( incoming_data,
                             packet_head->getPacketLength() -
                             cluster::Header::RIM_PACKET_HEAD_SIZE,
                             bytes_read );
      }
      catch (vpr::IOException& ex)
      {
         vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
            << clrOutBOLD( clrRED, "ERROR:" )
            << " Reading packet data failed. Expecting: "
            << packet_head->getPacketLength() - cluster::Header::RIM_PACKET_HEAD_SIZE
            << " But got: " << bytes_read << std::endl << vprDEBUG_FLUSH;
         
         // TODO: setCause(ex)
         throw cluster::ClusterException( "Node::recvPacket() - Reading packet data failed!" );
      }
   }
   
   vpr::BufferObjectReader* reader = new vpr::BufferObjectReader( &incoming_data );
   
   // Parse Packet with new data
   new_packet->parse( reader );
   
   //NOTE: incoming_data goes out of scope here which means that we are left with only the data that we parsed.
   //TODO: We could save memory by not parsing the raw DataPacket but just passing the location of the memory that we want to use.

   //parse_data_length = DataPacket::ParsedDataLength
   //recvn(incoming_parse_data, ...)
   //reader = new reader(incoming_parse_data);
   //new_packet->parse(reader);
   //recvn(incoming_raw_data, ...)
   //new_packet->setRawData(incoming_raw_data);

   return new_packet;
}

}  // end namespace gadget
