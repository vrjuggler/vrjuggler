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

#include <boost/concept_check.hpp>

//#include <vpr/vpr.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/Util/ReturnStatus.h>
//#include <jccl/Config/ConfigChunk.h>
//#include <vpr/Util/Error.h>

#include <cluster/ClusterNetwork/ClusterNode.h>
#include <cluster/Packets/Packet.h>
#include <cluster/Packets/ConnectionRequest.h>
#include <cluster/Packets/DeviceRequest.h>
#include <cluster/Packets/ApplicationDataRequest.h>
#include <cluster/Packets/EndBlock.h>
#include <cluster/Packets/DataPacket.h>
#include <cluster/Packets/PacketFactory.h>
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterManager.h>
#include <cluster/ClusterDelta.h>

#include <gadget/Util/Debug.h>
#include <gadget/InputManager.h>

#include <jccl/RTRC/ConfigManager.h>

namespace cluster
{

   ClusterNode::ClusterNode() : mUpdateTriggerSema(0), mClusterNodeDoneSema(0), mControlThread(NULL)
   {
      mThreadActive = false;
      mPendingConnectionRequest = false;
      mUpdated = false;
      mRunning = false;

      mConnected = DISCONNECTED;

      mName = std::string("None Given");
      mHostname = std::string("None Given");
      mPort = 0;
      mSockStream = NULL;
   }

   ClusterNode::ClusterNode(const std::string& name, const std::string& host_name, 
                            const vpr::Uint16& port, vpr::SocketStream* socket_stream) 
      : mUpdateTriggerSema(0), mClusterNodeDoneSema(0), mControlThread(NULL)
   {
      mThreadActive = false;
      mUpdated = false;
      mPendingConnectionRequest = false;
      mRunning = false;

      mConnected = DISCONNECTED;

      mName = name;
      mHostname = host_name;
      mPort = port;
      mSockStream = socket_stream;
   }

   ClusterNode::~ClusterNode()
   {
      shutdown();
      // This may break the acept code since we migt not want to delete the Socket. We may be able to just 
      // use a smart pointer to point to the SocketStream.
      /*
      if (mSockStream != NULL)
      {
         mSockStream->close();
         delete mSockStream;
      }
      */
   }

   void ClusterNode::shutdown()
   {     // Kill the accepting thread
      if ( mControlThread )
      {
         mThreadActive = false;
         mControlThread->kill();
         mControlThread = NULL;
      }
   }

   vpr::ReturnStatus ClusterNode::attemptConnect()
   {
      // - Try to connect to given node
      // - If success
      //   - Create a ConnectionRequest Packet
      //   - Send request(a responce will come later in the normal controlLoop)

      /////////////////////////////////////////////

      /*if (mSockStream != NULL)
      {
         mSockStream->close();
         delete mSockStream;
         mSockStream == NULL;
      }*/

         // If ClusterNode is already connected
      // XXX: FIX THIS, it should not return SUCCESS or WOULDBLOCK
      if (isConnected())
      {
         return(vpr::ReturnStatus::WouldBlock);
      }
         // If we currently have a pending connection request
      if (getConnected() == NEWCONNECTION)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL)
            << clrOutBOLD(clrBLUE,"[ClusterNode::attemptConnect]")
            << " Pending Connection Request!" << std::endl << vprDEBUG_FLUSH;

         return(vpr::ReturnStatus::InProgress);
      }

      vpr::SocketStream* sock_stream;
      vpr::InetAddr inet_addr;

      //vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL)
      //   << clrOutBOLD(clrBLUE,"[ClusterNode::attemptConnect]")
      //   << " HostName: " << mHostname << ":" << mPort << std::endl << vprDEBUG_FLUSH;

         // Set the address that we want to connect to
      if ( !inet_addr.setAddress(mHostname, mPort).success() )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
            << clrOutBOLD(clrBLUE,"[ClusterNode::attemptConnect]")
            << clrOutBOLD(clrRED," ERROR: Failed to set address\n") << vprDEBUG_FLUSH;
         return vpr::ReturnStatus::Fail;
      }
         // Create a new socket stream to this address
      sock_stream = new vpr::SocketStream(vpr::InetAddr::AnyAddr, inet_addr);


         // If we can successfully open the socket and connect to the server
      if ( sock_stream->open().success() && sock_stream->connect().success() )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
            << clrOutBOLD(clrBLUE,"[ClusterNode::attemptConnect]")
            << " Successfully connected to: " << mHostname <<":"<< mPort << "\n"<< vprDEBUG_FLUSH;
         sock_stream->setNoDelay(true);
         mSockStream = sock_stream;
      }
      else
      {
         delete sock_stream;
//         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
//            << clrOutBOLD(clrBLUE,"[ClusterNode::attemptConnect]")
//            << clrSetNORM(clrRED) << "ERROR: Could not connect to device server: "
//            << mHostname <<" : "<< mPort << "\n" << clrRESET << vprDEBUG_FLUSH;
         return vpr::ReturnStatus::Fail;
      }

      ////////////////////////////////////////////////////

      // We might want to send the listen port on this machine to the remote machine just in
      // case they for some reason want to re-connect back to us. This should be taken care
      // of in configuration but in all reality the remote machine does not have to have the
      // machine specific chunk for this machine
      ConnectionRequest request(ClusterNetwork::instance()->getLocalHostname(),0/*Might be needed, look above*/);

      send(&request);
      
      Packet* temp_packet = recvPacket();

      //vprASSERT(temp_packet->getPacketType() == cluster::Header::RIM_CONNECTION_ACK && "We must be receiving a ConnectionAck here");
      
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << "We have a responce" << std::endl << vprDEBUG_FLUSH;

      temp_packet->printData(vprDBG_CONFIG_LVL);
      ClusterManager::instance()->handlePacket(temp_packet,this);

      delete temp_packet;
      if (getConnected() == NEWCONNECTION)
      {
         ClusterDelta cluster_delta;
         vpr::Interval temp;
         temp = cluster_delta.getClusterDelta(getSockStream());
         mDelta = temp.getBaseVal();
      }

      return(vpr::ReturnStatus::Succeed);
   }

   void ClusterNode::debugDump(int debug_level)
   {

      vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                                 std::string("-------------- ClusterNode --------------\n"),
                                 std::string("-----------------------------------------\n"));

      vprDEBUG(gadgetDBG_RIM,debug_level) << "Node Name: " << mName << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) << "Hostname:  " << mHostname << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) << "Port:      " << mPort << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,debug_level) << "SockStream " << (mSockStream == NULL ? "is NULL" : "is NOT NULL") << std::endl << vprDEBUG_FLUSH;
      if (getConnected() == CONNECTED)
      {
         vprDEBUG(gadgetDBG_RIM,debug_level) << clrOutBOLD(clrGREEN,"CONNECTED") << std::endl << vprDEBUG_FLUSH;
      }
      else if (getConnected() == NEWCONNECTION)
      {
         vprDEBUG(gadgetDBG_RIM,debug_level) << clrOutBOLD(clrRED,"PENDING CONNECTION REQUEST") << std::endl << vprDEBUG_FLUSH;
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,debug_level) << clrOutBOLD(clrRED,"DISCONNECTED") << std::endl << vprDEBUG_FLUSH;
      }
   }
   void ClusterNode::printStats(int debug_level)
   {
      vpr::BaseIOStatsStrategy* stats = mSockStream->getIOStatStrategy();
      vpr::BandwidthIOStatsStrategy* bw_interface = dynamic_cast<vpr::BandwidthIOStatsStrategy*>(stats );

      if(bw_interface != NULL)
      {
         // Dump out write stats
         vprDEBUG(gadgetDBG_RIM,debug_level) << "Socket Write bandwidth stats ---\n";
         vprDEBUG(gadgetDBG_RIM,debug_level) << "stats type: " << typeid(stats).name() << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,debug_level) << "      sent bytes: " << bw_interface->writeStats().getTotal() << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,debug_level) << "         av send: " << bw_interface->writeStats().getMean()/1024.0f << " k/s" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,debug_level) << "        STA send: " << bw_interface->writeStats().getSTA()/1024.0f << " k/s" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,debug_level) << "       Inst send: " << bw_interface->writeStats().getInstAverage()/1024.0f << " k/s" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,debug_level) << "    Max STA send: " << bw_interface->writeStats().getMaxSTA()/1024.0f << " k/s" << vprDEBUG_FLUSH;

         vprDEBUG(gadgetDBG_RIM,debug_level) << "      read bytes: " << bw_interface->readStats().getTotal() << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,debug_level) << "         av read: " << bw_interface->readStats().getMean()/1024.0f << " k/s" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,debug_level) << "        STA read: " << bw_interface->readStats().getSTA()/1024.0f << " k/s" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,debug_level) << "       Inst read: " << bw_interface->readStats().getInstAverage()/1024.0f << " k/s" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,debug_level) << "    Max STA read: " << bw_interface->readStats().getMaxSTA()/1024.0f << " k/s" << vprDEBUG_FLUSH;

      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,debug_level) << "SocketBWTest: Don't have BW Stats on stats. type is: " << typeid(stats).name() << std::endl << vprDEBUG_FLUSH;
      }

   }

   void ClusterNode::setConnected(int connect)
   {
      // - If we are disconnecting
      //   - Delete all VirtualDevices for this node
      //   - Delete all DeviceServers for this node
      //   - Refresh All Proxies
      vpr::Guard<vpr::Mutex> guard(mConnectedLock);
      if (mConnected == CONNECTED && connect == DISCONNECTED)
      {
         /*
         if (mSockStream != NULL && mSockStream->isOpen())
         {
            mSockStream->close();
            //delete mSockStream;
            //mSockStream = NULL;
         }
         */
         //TODO: ADD This back in SOON
         //ClusterManager::instance()->recoverFromLostNode(this);
      }
      if (connect == CONNECTED)
      {
         ClusterNetwork::instance()->removePendingNode(mHostname);
      }
      mConnected = connect;
   }

   /*void ClusterNode::getData()
   {
      while (mSockStream != NULL && mSockStream->availableBytes() > 0)
      {
         update();
      }
   }
   */

   void ClusterNode::update()
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

      //debugDump(vprDBG_CONFIG_LVL);
      if (mSockStream == NULL)
      {
         // Do Nothing
         setConnected(DISCONNECTED);
         setUpdated(false);
         //debugDump(vprDBG_CONFIG_LVL);
         return;
      }
      vprASSERT(isConnected() && "ClusterNode is not connected, we can not update!\nWe must not be calling update from the correct location.");
      Packet* temp_packet = NULL;
      
      temp_packet = recvPacket();
      
      // Print Packet Information
      temp_packet->printData(vprDBG_CONFIG_LVL);

      // Handle the packet correctly
      ClusterManager::instance()->handlePacket(temp_packet,this);

      // Clean up after ourselves
      delete temp_packet;
   }

   void ClusterNode::controlLoop(void* nullParam)
   {
      // -Block on an update call
      // -Update Local Data
      // -Send
      // -Signal Sync

      boost::ignore_unused_variable_warning(nullParam);
      
      bool running = true;

      while(running)
      {
         // Wait for trigger
         mUpdateTriggerSema.acquire();
         {
            //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << getName() << "Dropping into ClusterNode::controlLoop() "
            //                                          << std::endl << vprDEBUG_FLUSH;

            setUpdated(false);
            while (!isUpdated())
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
                     "ClusterNode::update() We have lost our connection to: " << getName() << ":" << getPort()
                     << std::endl << vprDEBUG_FLUSH;

                  debugDump(vprDBG_CONFIG_LVL);
                  // Set the ClusterNode as disconnected since we have lost the connection
                  setConnected(DISCONNECTED);
                  // Break out of the two loops we are in to stop the tread
                  setUpdated(true);             
                  running = false;
                  
                  // TODO: Deleting mSockStream causes a seg fault
                  mSockStream = NULL;
               }
            }
         }
         
         // Signal done with Update
         mClusterNodeDoneSema.release();
      }
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ClusterNode: " << getName() << " is stopping."
                                                << std::endl << vprDEBUG_FLUSH;
   }

   /** Starts the control loop. */
   void ClusterNode::start()
   {
      // --- Setup Multi-Process stuff --- //
      // Create a new thread to handle the control

      if (NULL == mControlThread || !mControlThread->valid())
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << "ClusterNode " << getName() << " already running..."
               << std::endl << vprDEBUG_FLUSH;         
      }

      vpr::ThreadMemberFunctor<ClusterNode>* memberFunctor =
         new vpr::ThreadMemberFunctor<ClusterNode>(this, &ClusterNode::controlLoop, NULL);

      mControlThread = new vpr::Thread(memberFunctor);

      if (mControlThread->valid())
      {
         mThreadActive = true;
      }
      vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
         << "ClusterNode " << getName() << " started. thread: "
         << mControlThread << std::endl << vprDEBUG_FLUSH;
   }

   void ClusterNode::signalUpdate()
   {
      while(!mThreadActive)
      {
         vprDEBUG(gadgetDBG_RIM, vprDBG_HVERB_LVL) << "Waiting in for thread to start ClusterNode::go().\n" << vprDEBUG_FLUSH;
         vpr::Thread::yield();
      }
      //vprDEBUG(gadgetDBG_RIM,/*vprDBG_HVERB_LVL*/1) << getName() << "Signaling ClusterNode\n" << vprDEBUG_FLUSH;
      mUpdateTriggerSema.release();
   }

   /**
    * Blocks until the end of the frame.
    * @post The frame has been drawn.
    */
   void ClusterNode::sync()
   {
      vprASSERT(mThreadActive == true);
      mClusterNodeDoneSema.acquire();
   }

   vpr::ReturnStatus ClusterNode::send(Packet* out_packet)
   {
      vprASSERT(NULL != out_packet && "Can not send a NULL packet.");

      vpr::Guard<vpr::Mutex> guard(mSockWriteLock);

      // -Send header data
      // -Send packet data

      if (mSockStream == NULL)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED)
            << "ERROR: SocketSteam is NULL\n" << clrRESET << vprDEBUG_FLUSH;
         throw cluster::ClusterException("ClusterNode::send() - SocketStream is NULL!");
      }

      Header* mHeader = out_packet->getHeader();

      if (mHeader == NULL)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED)
            << "ERROR: Packet Header is NULL\n" << clrRESET << vprDEBUG_FLUSH;
         throw cluster::ClusterException("ClusterNode::send() - Packet Header is NULL!");
      }

      if (!mHeader->send(mSockStream).success())
      {
         mSockStream->close();
         delete mSockStream;
         mSockStream = NULL;
         throw cluster::ClusterException("Packet::recv() - Sending Header Data failed!");
      }

      vpr::Uint32 bytes_written;      

      if(mHeader->getPacketLength() == Header::RIM_PACKET_HEAD_SIZE)
      {
         return(vpr::ReturnStatus::Succeed);
      }
      
      
      // If we have a data packet we need to also send the raw data
      //
      if (out_packet->getPacketType() != Header::RIM_DATA_PACKET)
      {
         std::vector<vpr::Uint8>* packet_data = out_packet->getData();

         vpr::ReturnStatus status = mSockStream->send(*packet_data,mHeader->getPacketLength()-Header::RIM_PACKET_HEAD_SIZE,bytes_written);
         if (!status.success())
         {
            mSockStream->close();
            delete mSockStream;
            mSockStream = NULL;
            throw cluster::ClusterException("Packet::recv() - Sending Packet Data failed!!");
         }
         
         return(status);
      }
      else
      {
         std::vector<vpr::Uint8>* packet_data = out_packet->getData();

         // Since we are sending a DataPacket we are not actually sending all data here. We are only sending 2 GUIDs here
         int size = 32;

         vpr::ReturnStatus status = mSockStream->send(*packet_data, size ,bytes_written);
         if (!status.success())
         {
            mSockStream->close();
            delete mSockStream;
            mSockStream = NULL;
            throw cluster::ClusterException("Packet::recv() - Sending Packet Data failed!!");
         }


         DataPacket* temp_data_packet = dynamic_cast<DataPacket*>(out_packet);
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
         
         status = mSockStream->send(*(temp_data_packet->getDeviceData()),temp_data_packet->getDeviceData()->size(),bytes_written);
         if (!status.success())
         {
            mSockStream->close();
            delete mSockStream;
            mSockStream = NULL;
            throw cluster::ClusterException("Packet::recv() - Sending Packet Data failed!!");
         }
         
         return(status);
      }     
   }

   Packet* ClusterNode::recvPacket()
   {
      // -Read in header
      // -Get Constructor for correct PacketType
      // -Call constructor
      // -Read in Packet data
      // -Parse data into new packet
      // -Return finished packet
      Header* packet_head = new Header(mSockStream);
      
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "ClusterNode::recvPacket() PacketFactory is trying to make a packet type: " 
         << packet_head->getPacketType() << std::endl << vprDEBUG_FLUSH;

      // Get Packet from factory
      Packet* new_packet = PacketFactory::instance()->createObject(packet_head->getPacketType());

      // Verify that the packet has been made
      if(NULL == new_packet)
      {
         throw cluster::ClusterException("ClusterNode::recvPacket() - Packet was not found in Factory.");
      }
      
      // Recv the packet data
      // -Copy over pointer to header
      // -Continue reading packet from socket
      
      new_packet->setHeader(packet_head);
      
      std::vector<vpr::Uint8> incoming_data;

      if (NULL == mSockStream)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED) 
            << "ERROR: SocketSteam is NULL\n" << clrRESET << vprDEBUG_FLUSH;
         throw cluster::ClusterException("ClusterNode::recvPacket::recv() - mSocketStream is NULL!");
      }
      //TODO: else if (!mSockStream->isConnected())
      //{
      //   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED) 
      //      << "ERROR: SocketSteam is not connected\n" << clrRESET << vprDEBUG_FLUSH;
      //   throw cluster::ClusterException("ClusterNode::recvPacket::recv() - mSocketStream is not connected!");
      //}
      else
      {
         vpr::Uint32 bytes_read;	  

         //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Blocking while reading " 
         //   << packet_head->getPacketLength()-Header::RIM_PACKET_HEAD_SIZE << " bytes.\n" << vprDEBUG_FLUSH;
         vpr::ReturnStatus status = mSockStream->recvn(incoming_data, packet_head->getPacketLength()-Header::RIM_PACKET_HEAD_SIZE, bytes_read);	  
         //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "We got " 
         //   << bytes_read << " bytes.\n" << vprDEBUG_FLUSH;

         if (!status.success())
         {
            mSockStream->close();
            delete mSockStream;
            mSockStream = NULL;
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED) << "Reading packet data failed. Expecting: "
                  << packet_head->getPacketLength()-Header::RIM_PACKET_HEAD_SIZE << " But got: " << bytes_read
                  << " ReturnStatus Code: " << (int)status.code() << std::endl << clrRESET << vprDEBUG_FLUSH;
            throw cluster::ClusterException("ClusterNode::recvPacket() - Reading packet data failed!");
         }
      }
      
      vpr::BufferObjectReader* reader = new vpr::BufferObjectReader(&incoming_data);
      
      // Parse Packet with new data
      new_packet->parse(reader);
      
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


}  // end namespace cluster
