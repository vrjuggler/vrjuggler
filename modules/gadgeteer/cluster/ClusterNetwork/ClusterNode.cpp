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

//#include <vpr/vpr.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/Util/ReturnStatus.h>
//#include <jccl/Config/ConfigChunk.h>
//#include <vpr/Util/Error.h>

#include <gadget/Util/Debug.h>
//#include <gadget/RemoteInputManager/NetDevice.h>
#include <cluster/ClusterNetwork/ClusterNode.h>
#include <cluster/Packets/Packet.h>
#include <cluster/Packets/ConnectionRequest.h>
#include <cluster/Packets/DeviceRequest.h>
#include <cluster/Packets/ApplicationDataRequest.h>
#include <cluster/Packets/EndBlock.h>
#include <cluster/Packets/PacketFactory.h>
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/Plugins/RemoteInputManager/RemoteInputManager.h>
#include <cluster/ClusterDelta.h>
#include <gadget/InputManager.h>
//#include <gadget/Type/DeviceFactory.h>
//#include <gadget/InputManager.h>
//#include <gadget/RemoteInputManager/RIM.h>
//#include <gadget/Type/BaseTypeFactory.h>

#include <jccl/RTRC/ConfigManager.h>

namespace cluster
{

   ClusterNode::ClusterNode() : mUpdateTriggerSema(0), mClusterNodeDoneSema(0)
   {
      mThreadActive = false;
      mPendingConnectionRequest = false;
      mUpdated = false;
      mRunning = false;
      
      mConnected = DISCONNECTED;

      mName = std::string("None Given");
      mHostname = std::string("None Given");
      mPort = 0;
      mRemoteManagerId = vpr::GUID("None Given"); // set the id of the other computer's remote manager
      mSockStream = NULL;
      
      start();
   }
   ClusterNode::ClusterNode(const std::string& name, const std::string& host_name, 
                            const vpr::Uint16& port, const std::string& manager_id, 
                            vpr::SocketStream* socket_stream) : mUpdateTriggerSema(0), mClusterNodeDoneSema(0)
   {
      mThreadActive = false;
      mUpdated = false;
      mPendingConnectionRequest = false;
      mRunning = false;
      
      mConnected = DISCONNECTED;
      
      mName = name;
      mHostname = host_name;
      mPort = port;
      mRemoteManagerId = vpr::GUID(manager_id); // set the id of the other computer's remote manager
      mSockStream = socket_stream;
      
      start();
   }

   ClusterNode::~ClusterNode()
   {
      shutdown();
      if (mSockStream != NULL)
      {
         mSockStream->close();
         delete mSockStream;
      }
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
//         sock_stream->setTypeOfService(vpr::SocketOptions::LowDelay);
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
      ConnectionRequest request(ClusterNetwork::instance()->getLocalHostname(),0/*Might be needed, look above*/, 
                                   std::string("We need to add this to the RemoteInputManager"));
      
      // THIS COULD BREAK IN THE FUTURE
      // We may need to convert this over to
      // using the ClusterNode send method soon
      request.send(mSockStream);
      
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << "Waiting for a responce! MIGHT WANT TO CHANGE THIS" << std::endl << vprDEBUG_FLUSH;
      
      Packet* temp_packet = PacketFactory::instance()->recvPacket(mSockStream);

      //vprASSERT(temp_packet->getBaseType() == cluster::Header::RIM_CONNECTION_ACK && "We must be receiving a ConnectionAck here");
      
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << "We have a responce" << std::endl << vprDEBUG_FLUSH;

      temp_packet->printData(vprDBG_CONFIG_LVL);
      temp_packet->action(this);
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
      vprDEBUG(gadgetDBG_RIM,debug_level) << "ManagerID: " << mRemoteManagerId << std::endl << vprDEBUG_FLUSH;
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
         RemoteInputManager::instance()->removeVirtualDevicesOnHost(mHostname);
         RemoteInputManager::instance()->removeDeviceClientsForHost(mHostname);
         gadget::InputManager::instance()->refreshAllProxies();
      }
      if (connect == CONNECTED)
      {
         ClusterNetwork::instance()->removePendingNode(mHostname);
      }
      mConnected = connect;
   }


   //void ClusterNode::update(int finish_state)
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
      try
      {
         temp_packet = PacketFactory::instance()->recvPacket(mSockStream);
      }
      catch(cluster::ClusterException cluster_exception)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ClusterNode::update() Caught an exception!" 
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED)
            << cluster_exception.getMessage() << clrRESET
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << 
            "ClusterNode::update() We have lost our connection to: " << getName() << ":" << getPort()
            << std::endl << vprDEBUG_FLUSH;

         setConnected(DISCONNECTED);
         setUpdated(true);
         debugDump(vprDBG_CONFIG_LVL);
         
         // We do NOT want to add Pending connection here since we do not know if it is
         // a device server or a device client. We can simply let reconfiguration take 
         // care of it in depManager
         RemoteInputManager::instance()->setReconfigurationNeededOnConnection(true);
           
         // Clean up after oursleves
         if (temp_packet != NULL)
         {
            delete temp_packet;
         }
         return;
      }  

      // Print Packet Information
      temp_packet->printData(vprDBG_CONFIG_LVL);
      temp_packet->action(this);
      delete temp_packet;
   }
   void ClusterNode::controlLoop(void* nullParam)
   {
      // -Block on an update call
      // -Update Local Data
      // -Send
      // -Signal Sync
      
      while(true)
      {
         // Wait for trigger
         mUpdateTriggerSema.acquire();
         {
            //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << getName() << "Dropping into UserData() " << std::endl << vprDEBUG_FLUSH;
            
            setUpdated(false);
            while (!isUpdated())
            {
               update();
            }
         }
         // Signal done with UserData
         mClusterNodeDoneSema.release();
      }
   }
   
   /** Starts the control loop. */
   void ClusterNode::start()
   {
      // --- Setup Multi-Process stuff --- //
      // Create a new thread to handle the control
      
      vpr::ThreadMemberFunctor<ClusterNode>* memberFunctor =
         new vpr::ThreadMemberFunctor<ClusterNode>(this, &ClusterNode::controlLoop, NULL);
   
      mControlThread = new vpr::Thread(memberFunctor);
   
      if (mControlThread->valid())
      {
         mThreadActive = true;
      }
      vprDEBUG(gadgetDBG_RIM,1) << "ClusterNode " << getName() << " started. thread: "
                                << mControlThread << std::endl << vprDEBUG_FLUSH;
   }

   void ClusterNode::signalUpdate()
   {
      while(!mThreadActive)
      {
         vprDEBUG(gadgetDBG_RIM, vprDBG_HVERB_LVL) << "Waiting in for thread to start ClusterNode::go().\n" << vprDEBUG_FLUSH;
         vpr::Thread::yield();
      }
      //vprDEBUG(gadgetDBG_RIM,/*vprDBG_HVERB_LVL*/1) << getName() << "Signaling UserData\n" << vprDEBUG_FLUSH;
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

   void ClusterNode::send(Packet* out_packet)
   {
      lockSockWrite();
      out_packet->send(getSockStream());
      unlockSockWrite();
   }

}  // end namespace cluster
