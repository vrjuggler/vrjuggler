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

#include <gadget/Util/Debug.h>
#include <vpr/Util/ReturnStatus.h>	
#include <vpr/IO/Socket/InetAddr.h>

#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork/ClusterNode.h>

#include <cluster/Packets/PacketFactory.h>
#include <cluster/Packets/ConnectionRequest.h>
#include <cluster/Packets/ConnectionAck.h>
#include <cluster/Packets/ConnectionRequest.h>
#include <cluster/Packets/ConnectionAck.h>
#include <cluster/Packets/Packet.h>
#include <cluster/ClusterDelta.h>

#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <jccl/RTRC/ConfigManager.h>

namespace cluster
{
   std::string getShortHostnameFromLong(const std::string& long_name);

   vprSingletonImp( ClusterNetwork );
	
   ClusterNetwork::ClusterNetwork()
   {
      mAcceptThread = NULL;
      vpr::InetAddr local_addr;
      vpr::InetAddr::getLocalHost(local_addr);
      mLocalHostnameLong = local_addr.getHostname();
      
      // Strip the tail of the address to get just the computer name
      // TODO: We should fix this to make the user fully specify the hostname
      mLocalHostnameShort = cluster::getShortHostnameFromLong(mLocalHostnameLong);
   }

   ClusterNetwork::~ClusterNetwork()
   {
      shutdown();
   }

   void ClusterNetwork::handlePacket(Packet* packet, ClusterNode* node)
   {
      // If the ClusterNetwork should handle this packet, then do so.
      if (packet->getPacketType() == Header::RIM_CONNECTION_REQ)
      {
         ConnectionRequest* temp_connection_request = dynamic_cast<ConnectionRequest*>(packet);
         vprASSERT(NULL != temp_connection_request && "Dynamic cast failed!");
         vprASSERT(NULL != node && "We can not handle a packet if we do not know which node it is coming from");
         
         std::string host_name   = temp_connection_request->getHostname();
         vpr::Uint16 port        = temp_connection_request->getPort();
         
         // If difference warn us
         node->setHostname(host_name);
         node->setPort(port);


         // Send back a responce
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
                        << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
                        << "Creating a new ConnectionAck Packet" << std::endl << vprDEBUG_FLUSH;

         ConnectionAck* responce_packet = new ConnectionAck(ClusterNetwork::instance()->getLocalHostname(),
                                                            mListenAddr.getPort(),true); 

         // Try to send a responce packet
         try
         {
            vprASSERT(NULL != node && "Node is null");
            vprASSERT(NULL != responce_packet && "Responce Packet is null");
            vprASSERT(NULL != node->getSockStream() && "SocketStream is null");

            // Send the responce
            responce_packet->printData(vprDBG_CONFIG_LVL);
            node->send(responce_packet);
         }
         catch(cluster::ClusterException cluster_exception)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrRED,"ERROR:")
               << "ClusterNetwork::acceptLoop() Could not send responce packet: " 
               << cluster_exception.getMessage() << std::endl << vprDEBUG_FLUSH;

            node->setConnected(ClusterNode::DISCONNECTED);
            node->setSockStream(NULL);
         }

         delete responce_packet;
         responce_packet = NULL;
         
         // Start updating the ClusterNode
         node->start();

         return;
      }
      else if (packet->getPacketType() == Header::RIM_CONNECTION_ACK)
      {
         ConnectionAck* temp_connection_ack = dynamic_cast<ConnectionAck*>(packet);
         vprASSERT(NULL != temp_connection_ack && "Dynamic cast failed!");
         
         if (node == NULL)
         {
            return;
         }

         if ( temp_connection_ack->getAck() )
         {
            std::string host_name   = temp_connection_ack->getHostname();
            
            //node->setConnected(ClusterNode::CONNECTED);
            node->setConnected(ClusterNode::NEWCONNECTION);
            node->setHostname(host_name);

            // Since we have gained a new connection we should start updating
            node->start();
         }
         else
         {
            //node->setConnected(ClusterNode::DISCONNECTED);
         }
      }
   }

   
   vpr::ReturnStatus ClusterNetwork::addClusterNode(const std::string& name, const std::string& host_name, 
                                                    const vpr::Uint16& port, vpr::SocketStream* socket_stream)
   {
      // -Create a new ClusterNode using the given information
      // -Add the new node to the ClusterNetwork
      vpr::Guard<vpr::Mutex> guard(mClusterNodesLock);
   
      cluster::ClusterNode* temp_node = new ClusterNode(name, host_name, port, socket_stream);
      
      mClusterNodes.push_back(temp_node);
   
      return(vpr::ReturnStatus::Succeed);
   }

   void ClusterNetwork::addClusterNode(ClusterNode* node)
   {
      // -Add the given node to the ClusterNetwork
      vpr::Guard<vpr::Mutex> guard(mClusterNodesLock);
      
      mClusterNodes.push_back(node);
   }

   void ClusterNetwork::removeClusterNode(const std::string& node_hostname)
   {
      vpr::Guard<vpr::Mutex> guard(mClusterNodesLock);
      
      // -Find the ClusterNode with the given hostname
      // -Remove it from the list of cluster nodes
      for (std::vector<cluster::ClusterNode*>::iterator i = mClusterNodes.begin();
           i != mClusterNodes.end() ; i++)
      {
         if ((*i)->getHostname() == node_hostname)
         {
            mClusterNodes.erase(i);
            return;
         }
      }
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Removing node: " << node_hostname
	     << std::endl << vprDEBUG_FLUSH;
	}

   ClusterNode* ClusterNetwork::getClusterNodeByHostname(const std::string& host_name)
   { 
      vpr::Guard<vpr::Mutex> guard(mClusterNodesLock);
      
      // -Find ClusterNode with given hostname and return a pointer to it.
      // -If we do not find one, return NULL
      for (std::vector<cluster::ClusterNode*>::iterator i = mClusterNodes.begin();
           i != mClusterNodes.end() ; i++)
      {
         if ((*i)->getHostname() == host_name)
         {
            return(*i);
         }
      }
      return(NULL);
   }
   
   ClusterNode* ClusterNetwork::getClusterNodeByName(const std::string& node_name)
   { 
      vpr::Guard<vpr::Mutex> guard(mClusterNodesLock);
      
      // -Find ClusterNode with given name and return a pointer to it.
      // -If we do not find one, return NULL
      for (std::vector<cluster::ClusterNode*>::iterator i = mClusterNodes.begin();
           i != mClusterNodes.end() ; i++)
      {
         if ((*i)->getName() == node_name)
         {
            return(*i);
         }
      }
      return(NULL);
   }

   void ClusterNetwork::addPendingNode(ClusterNode* node)
   {
      if (getPendingNode(node->getHostname()) == NULL)
      {
         vpr::Guard<vpr::Mutex> guard(mPendingNodesLock);
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL)
            << "[ClusterNetwork] Adding Pending ClusterNode: " << node->getHostname() 
            << "\n"<< vprDEBUG_FLUSH;
         mPendingNodes.push_back(node);
      }
   }
   
   ClusterNode* ClusterNetwork::getPendingNode(std::string host_name)
   {      
      // -If we can get a pending node with the given hostname
      //  return a pointer to the node.
      // -Else return NULL

      vpr::Guard<vpr::Mutex> guard(mPendingNodesLock);

      for (std::vector<cluster::ClusterNode*>::iterator i = mPendingNodes.begin();
           i != mPendingNodes.end() ; i++)
      {
         if ((*i)->getHostname() == host_name)
         {
            return(*i);
         }
      }
      return NULL;
   }

   
   void ClusterNetwork::removePendingNode(std::string host_name)
   {      
      vpr::Guard<vpr::Mutex> guard(mPendingNodesLock);
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL)
         << "[ClusterNetwork] Removing Pending Host: " << host_name << "\n"<< vprDEBUG_FLUSH;
      
      for (std::vector<cluster::ClusterNode*>::iterator i = mPendingNodes.begin();
           i != mPendingNodes.end() ; i++)
      {
         if ((*i)->getHostname() == host_name)
         {
            mPendingNodes.erase(i);
            return;
         }
      }
   }
   
   vpr::Uint16 ClusterNetwork::getNumPendingNodes()
   {
      vpr::Guard<vpr::Mutex> guard(mPendingNodesLock);
      vpr::Uint16 temp_pending_size = mPendingNodes.size();
      return(temp_pending_size);
   }

   bool ClusterNetwork::attemptPendingNodes()
   {
      vpr::Guard<vpr::Mutex> guard(mPendingNodesLock);
      
      bool ret_val = false;

      for (std::vector<cluster::ClusterNode*>::iterator i = mPendingNodes.begin();
           i != mPendingNodes.end() ; i++)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) 
            << "Attempting to connect to: " << (*i)->getHostname()
            << std::endl << vprDEBUG_FLUSH;
         if ( (*i)->attemptConnect().success() )
         {
            // If any of the nodes were successful connecting
            // then we should return true
            ret_val = true;
         }
      }      
      return(ret_val);
   }

   bool ClusterNetwork::startListening(const int& listen_port)
   {
      // If we haven't already started the listening thread
      if ( mAcceptThread == NULL )  
      {  
         // If the listen port is valid
         if ( listen_port > 0 )     
         {
            mListenAddr.setPort(listen_port);

            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
               << "Starting the listening thread...\n" << vprDEBUG_FLUSH;

               // Start a thread to monitor port
            vpr::ThreadMemberFunctor<ClusterNetwork>* memberFunctor =
            new vpr::ThreadMemberFunctor<ClusterNetwork>
            (this, &ClusterNetwork::acceptLoop, NULL);
            
            mAcceptThread = new vpr::Thread(memberFunctor);
            vprASSERT(mAcceptThread->valid());
            return true;
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
               << "startListening() Can NOT listen on port: "
               << listen_port << "\n" << vprDEBUG_FLUSH;
            return false;
         }
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
               << "startListening() Listening thread already active.\n" << vprDEBUG_FLUSH;
         return false;
      }
   }

   void ClusterNetwork::acceptLoop(void* nullParam)
   {
      // -Create a socket to listen for connection requests
      // -Wait for a connection
      //   -If successful
      //     -Get packet
      //     -Try to cast it to a connection request
      //     -If cluster node exists
      //       -Set the socketstream
      //     -Else
      //       -Create a new node using this socketstream
      //     -Send responce packet
      //     -Get Cluster Delta
      //   -Close and delete socket

      boost::ignore_unused_variable_warning(nullParam);

      // Create an socket to listen for connection requests on
      vpr::SocketStream sock(mListenAddr, vpr::InetAddr::AnyAddr);
        // Open in server mode.
      if ( sock.openServer().success() )
      {
         sock.setReuseAddr(true);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL)
            << clrSetBOLD(clrRED) << "[ClusterNetwork]"
            << " Unable to open listening socket on port: " << mListenAddr.getPort() << std::endl << clrRESET << vprDEBUG_FLUSH;
         exit(0);
      }
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
         << " Listening on Port: " << mListenAddr.getPort() << std::endl << vprDEBUG_FLUSH;      

      
      // Create a socketstream for new socket
      vpr::SocketStream* client_sock = new vpr::SocketStream;
      
      // Wait for an incoming connection.
      while ( 1 )
      {  
         vpr::ReturnStatus status = sock.accept(*client_sock, vpr::Interval::NoTimeout);
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
            << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
            << " Receiving a connection request on Port: " << mListenAddr.getPort() << std::endl << vprDEBUG_FLUSH;
         
         if ( status.success() )
         {               
            // Optimize new socket for low latency communication
            client_sock->setNoDelay(true);  

            std::string remote_host_name = client_sock->getRemoteAddr().getHostname();
            remote_host_name = cluster::getShortHostnameFromLong(remote_host_name);
            
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
               << " Receiving a connection request from: " << remote_host_name << std::endl << vprDEBUG_FLUSH;
            
            // See if we already have a ClusterNode with the same hostname
            ClusterNode* requesting_node = getClusterNodeByHostname(remote_host_name);

            // If we do not already have a ClusterNode with the given hostname
            if (NULL == requesting_node)
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
                  << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
                  << " ClusterNode does not exist in ClusterNetwork."
                  << " Creating new ClusterNode."
                  << " Starting ClusterNode."
                  << " Adding new node to ClusterNetwork..." << std::endl << vprDEBUG_FLUSH;           
               
               // Create a new ClusterNode and pass it the new SocketStream
               requesting_node = new ClusterNode(std::string("Unknown"), std::string("Unknown"), vpr::Uint16(0), client_sock);                              
               // Add the new node to the cluster
               ClusterNetwork::instance()->addClusterNode(requesting_node);
            }
            else
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
                  << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
                  << " ClusterNode already exists in ClusterNetwork."
                  << " Setting the SocketStream." << std::endl << vprDEBUG_FLUSH;                          
               
               // Set the SocketStream to use for this ClusterNode.
               requesting_node->setSockStream(client_sock);                              
            }
            
            // -Try to receive a Packet
            // -If we catch an exception then
            //  -Print error message
            //  -Delete new socket
            //  -Listen for another request
            Packet* temp_packet;
            try
            {
               temp_packet = requesting_node->recvPacket();
               if (NULL == temp_packet)
               {
                  throw(new cluster::ClusterException(std::string("The received packet is NULL...")));
               }
            }
            catch(cluster::ClusterException cluster_exception)
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
                  << clrOutBOLD(clrRED,"ERROR:")
                  << "ClusterNetwork::acceptLoop() Caught an exception: " 
                  << cluster_exception.getMessage() << std::endl << vprDEBUG_FLUSH;

               delete client_sock;
               client_sock = new vpr::SocketStream;
               continue;
            }
            
            //vprASSERT(remote_host_name == connection_request->getHostname() && 
            //          "We have the wrong node since we got a different hostname from the InetAddr...");
            
            //connection_request->printData(vprDBG_CONFIG_LVL);
            this->handlePacket(temp_packet,requesting_node);

            // We can still use client_sock here....
            ClusterDelta cluster_delta;
            cluster_delta.clientClusterDelta(requesting_node->getSockStream());

                        
            // We need to create a new SocketStream since the to
            // hold the value of the next recieved socketstream since
            // the old one is now being used by the new ClusterNode
            client_sock = new vpr::SocketStream;
            
            // Since we have just recieved a new connection,
            // set the connected status as so. We are not 
            // in a fully connected state until the begining
            // of the next frame in ClusterNetwork::updateNewConnetions()
            // this is becuase we only want to start using a
            // new connection at the start of a new frame.
            requesting_node->setConnected(ClusterNode::NEWCONNECTION);
            // Print the new state information about this node.
            requesting_node->debugDump(vprDBG_CONFIG_LVL);            
         }
         else if ( status == vpr::ReturnStatus::Timeout )
         {
               // Should never happen since timeout is infinite
            client_sock->close();
            delete client_sock;
            client_sock = new vpr::SocketStream;
         }
      }   // end infinite while      
   }


   void ClusterNetwork::shutdown()
   {  
      // Kill thread used to listen for incoming 
      // connection requests

      // TODO: Make this actually shutdown the Accepting thread, this will require
      //       non blocking accept calls.
      if ( mAcceptThread )
      {
         mAcceptThread->kill();
         mAcceptThread = NULL;
      }
      // Shutdown all cluster nodes
      vpr::Guard<vpr::Mutex> guard(mClusterNodesLock);
      for(std::vector<ClusterNode*>::iterator j = mClusterNodes.begin(); j != mClusterNodes.end(); j++)
      {
          (*j)->shutdown();
      }  
   }


   void ClusterNetwork::debugDumpClusterNodes(int debug_level)
   {
      vpr::Guard<vpr::Mutex> guard(mClusterNodesLock);
      vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
													std::string("-------------- Cluster Network --------------\n"),
													std::string("---------------------------------------------\n"));
		for(std::vector<ClusterNode*>::iterator j = mClusterNodes.begin(); j != mClusterNodes.end(); j++)
		{
			(*j)->debugDump(debug_level);
		}  
   }
   
   //-------- Helper Functions --------
   std::string getShortHostnameFromLong(const std::string& long_name)
   {
      int first_period_pos = long_name.find(".");
      std::string temp = long_name.substr(0, first_period_pos);
      // just in case it is trailed by a ":port";
      int colon_pos = temp.find(":");
      std::string temp2 = temp.substr(0,colon_pos);
      return temp2;
   }



   ////////////////// Config Functions ///////////////
   bool ClusterNetwork::recognizeClusterMachineConfig(jccl::ConfigChunkPtr chunk)
   {
      return(chunk->getDescToken() == ClusterNetwork::getMachineSpecificChunkType());
   }
   
   bool ClusterNetwork::configCanHandle(jccl::ConfigChunkPtr chunk)
   {
       return( recognizeClusterMachineConfig(chunk) );
   }

   bool ClusterNetwork::configAdd(jccl::ConfigChunkPtr chunk)
   {
      if (recognizeClusterMachineConfig(chunk))
      {
         // -If local machine chunk
         //   -Start Listening thread
         // -Else
         //   -Add Node to ClusterNetwork

         if (chunk->getProperty<std::string>("host_name") == getLocalHostname())
         {
            const int listen_port = chunk->getProperty<int>("listen_port");
            startListening(listen_port);
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterNetwork] ")
               << "Adding Node: " << chunk->getName() 
               << " to the Cluster Network\n" << vprDEBUG_FLUSH;

            std::string    name        = chunk->getName();
            std::string    host_name   = chunk->getProperty<std::string>("host_name");
            vpr::Uint16    listen_port = chunk->getProperty<int>("listen_port");
            
            addClusterNode(name, host_name, listen_port);
         }
         return true;
      }
      return false;
   }

   bool ClusterNetwork::configRemove(jccl::ConfigChunkPtr chunk)
   {
     if (recognizeClusterMachineConfig(chunk))
     {
        vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[ClusterNetwork] Removing the Node: " << chunk->getName() 
        << " from the Cluster Network\n" << vprDEBUG_FLUSH;
        return(true);
     }
     else
     {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[ClusterNetwork::configRemove] ERROR, Something is seriously wrong, we should never get here\n"
         << vprDEBUG_FLUSH;
         return(false);
     }
   }
   void ClusterNetwork::updateNewConnections()
   {
      if (getNumPendingNodes() > 0)
      {
         if(attemptPendingNodes())
         {
            // -If the pending list is stale
            //   - refresh the list               
            if(jccl::ConfigManager::instance()->isPendingStale())
            {
               jccl::ConfigManager::instance()->refreshPendingList();
            }
         }
      }
      
      // This fuction is needed in order to gurantee that all cluster
      // nodes become active at the same time, the beginning of the frame
      //
      // -Loop over all ClusterNodes checking for new connections
      // -If ClusterNode is a new connection
      //   - Set it to connected

      vpr::Guard<vpr::Mutex> guard(mClusterNodesLock);

      for (std::vector<cluster::ClusterNode*>::iterator i = getClusterNodesBegin();
            i != getClusterNodesEnd() ; i++)
      {
         if ((*i)->getConnected() == ClusterNode::NEWCONNECTION)
         {
            (*i)->setConnected(ClusterNode::CONNECTED);
         }
      }
   }
   bool ClusterNetwork::isClusterNetworkReady()
   {
      // ClusterNetwork is ready if it has
      // no pending connection requests
      return (0 == mPendingNodes.size());
   }
}	// end namespace cluster
