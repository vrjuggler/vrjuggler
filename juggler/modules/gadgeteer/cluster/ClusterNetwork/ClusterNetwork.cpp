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

#include <gadget/Util/Debug.h>
#include <vpr/Util/ReturnStatus.h>	

// WE NEED TO ELIMINATE THIS DEP. We need it to signal the RIM on a new connection
// this is used to decide if we need to reconfigure or not. Either ClusterNetwork
// or RemoteInputManager has to depend on ClusterManager. I choose for the RIM
// to depend on it since it already did.                                                         
#include <cluster/Plugins/RemoteInputManager/RemoteInputManager.h>

#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork/ClusterNode.h>

#include <cluster/Packets/PacketFactory.h>
#include <cluster/Packets/ConnectionRequest.h>
#include <cluster/Packets/ConnectionAck.h>
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
      mUpdateThread = NULL;
      vpr::InetAddr local_addr;
      vpr::InetAddr::getLocalHost(local_addr);
      mLocalHostnameLong = local_addr.getHostname();
      
         // now strip the tail of the address to just get the computer name
      mLocalHostnameShort = cluster::getShortHostnameFromLong(mLocalHostnameLong);
      startUpdating();
   }

   ClusterNetwork::~ClusterNetwork()
   {
      shutdown();
   }
   vpr::ReturnStatus ClusterNetwork::addClusterNode(const std::string& name, const std::string& host_name, 
                                                    const vpr::Uint16& port, const std::string& manager_id, 
                                                    vpr::SocketStream* socket_stream)
   {
      vpr::Guard<vpr::Mutex> guard(mClusterNodesLock);
   
      cluster::ClusterNode* temp_node = new cluster::ClusterNode(name, host_name, port, manager_id, socket_stream);
      mClusterNodes.push_back(temp_node);
   
      return(vpr::ReturnStatus::Succeed);
   }

   void ClusterNetwork::addClusterNode(ClusterNode* node)
   {
      vpr::Guard<vpr::Mutex> guard(mClusterNodesLock);
      
      mClusterNodes.push_back(node);
   }

   void ClusterNetwork::removeClusterNode(const std::string& node_hostname)
   {
      vprASSERT(1 == mClusterNodesLock.test());
      //lockClusterNodes();
		
      for (std::vector<cluster::ClusterNode*>::iterator i = mClusterNodes.begin();
           i != mClusterNodes.end() ; i++)
      {
         if ((*i)->getHostname() == node_hostname)
         {
            //unlockClusterNodes();
            mClusterNodes.erase(i);
            return;
         }
      }
      
	  vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Removing node " << node_hostname
	     << std::endl << vprDEBUG_FLUSH;
      
      //unlockClusterNodes();
	}

   ClusterNode* ClusterNetwork::getClusterNodeByHostname(const std::string& host_name)
   { 
      vpr::Guard<vpr::Mutex> guard(mClusterNodesLock);
      
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
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL)
            << "[ClusterNetwork] Adding Pending Host: " << node->getHostname() << "\n"<< vprDEBUG_FLUSH;

         vpr::Guard<vpr::Mutex> guard(mPendingNodesLock);
         mPendingNodes.push_back(node);
      }
   }
   
   ClusterNode* ClusterNetwork::getPendingNode(std::string host_name)
   {      
      vpr::Guard<vpr::Mutex> guard(mPendingNodesLock);

      std::vector<cluster::ClusterNode*>::iterator begin = mPendingNodes.begin();
      std::vector<cluster::ClusterNode*>::iterator end = mPendingNodes.end();
      std::vector<cluster::ClusterNode*>::iterator i;

      for (i = begin ; i != end ; i++)
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
      vprASSERT(1 == mPendingNodesLock.test() 
                && "The mPendingNodes list must be locked before we can remove a pending node.");

      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL)
         << "[ClusterNetwork] Removing Pending Host: " << host_name << "\n"<< vprDEBUG_FLUSH;
      
      std::vector<cluster::ClusterNode*>::iterator begin = mPendingNodes.begin();
      std::vector<cluster::ClusterNode*>::iterator end = mPendingNodes.end();
      std::vector<cluster::ClusterNode*>::iterator i;

      for (i = begin ; i != end ; i++)
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

      std::vector<cluster::ClusterNode*>::iterator begin = mPendingNodes.begin();
      std::vector<cluster::ClusterNode*>::iterator end = mPendingNodes.end();
      std::vector<cluster::ClusterNode*>::iterator i;
      for (i = begin ; i != end ; i++)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) 
            << "Attempting Node to : " << std::endl << vprDEBUG_FLUSH;
         (*i)->debugDump(vprDBG_VERB_LVL);
         if ( (*i)->attemptConnect().success() )
         {
            ret_val = true;
         }
      }      
      return(ret_val);
   }

   bool ClusterNetwork::startListening(const int& listen_port)
   {
      if ( mAcceptThread == NULL )  // If we haven't already started the listening thread
      {     
         if ( listen_port > 0 )     // If the listen port is valid
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
      //////////////////////////// Create an acceptor socket that listens on port. //////////////////////////////

      vpr::SocketStream sock(mListenAddr, vpr::InetAddr::AnyAddr);
        // Open in server mode.
      if ( sock.openServer().success() )
      {
         sock.setReuseAddr(true);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
            << clrSetBOLD(clrRED) << "[ClusterNetwork]"
            << " Unable to open listening socket on port: " << mListenAddr.getPort() << std::endl << clrRESET << vprDEBUG_FLUSH;
         exit(0);
      }
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
         << " Listening on Port: " << mListenAddr.getPort() << std::endl << vprDEBUG_FLUSH;      

      ///////////////////////////  Wait for a connection //////////////////////////////

      vpr::SocketStream* client_sock = new vpr::SocketStream;

      while ( 1 )
      {    // Wait for an incoming connection.   
         vpr::ReturnStatus status = sock.accept(*client_sock, vpr::Interval::NoTimeout);
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
            << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
            << " Received a connection attempt on Port: " << mListenAddr.getPort() << std::endl << vprDEBUG_FLUSH;
         if ( status.success() )
         {               
            client_sock->setNoDelay(true);  
               // Lock Cluster Nodes while working on them
              // Try to receive a Packet
            Packet* temp_packet;
            try
            {
               temp_packet = PacketFactory::instance()->recvPacket(client_sock);
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
            
            ConnectionRequest* connection_request;

              // Make sure it is a ConnectionRequest
            try
            {
               connection_request = static_cast<ConnectionRequest*>(temp_packet);
            }
            catch(...)
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
                  << clrOutBOLD(clrRED,"ERROR:")
                  << " ClusterNetwork::acceptLoop() The first packet read on the socket is NOT a Connection Request" << std::endl << vprDEBUG_FLUSH;
               exit(0);
            }

            ClusterNode* node = getClusterNodeByHostname(connection_request->getHostname());
            
            if (node != NULL)
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
                  << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
                  << " Cluster Node already exists." << std::endl << vprDEBUG_FLUSH;
               
               // XXX: Need to FIX vpr::SocketStream::close()
               // XXX: HACK we don't do this because for some odd reason we can not close or delete a SocketStream
               /*old_node->setSockStream(NULL);
               if (old_node->getSockStream() != NULL)
               {
                  old_node->getSockStream()->close();
                  delete old_node->getSockStream();
                  old_node->setSockStream(NULL);
               }*/
               node->setSockStream(client_sock);
            }
            else
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
                  << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
                  << " Adding a new Cluster Node." << std::endl << vprDEBUG_FLUSH;           
               ClusterNode* node = new ClusterNode(std::string("Unkown"), std::string("Unkown"), vpr::Uint16(0), std::string("Unkown"), client_sock);               
               addClusterNode(node);               
            }
            temp_packet->printData(vprDBG_CONFIG_LVL);
            temp_packet->action(node);   
            delete temp_packet;
            
            // XXX: HACK FOR NOW
//            ClusterManager::instance()->addManager(RemoteInputManager::instance());
            RemoteInputManager::instance()->setActive(true);

            // NOTE: We must also set the Updated flag here since we are done with this node during this frame
            //node->setState(ClusterNode::NEWCONNECTION);  //?????????????????????????????????????????/
            node->debugDump(vprDBG_CONFIG_LVL);
            
            // TODO: When adding a Machine Specific Chunk we add new ClusterNodes, we must also test for repeats there!
                        
            ConnectionAck* responce_packet = new ConnectionAck(ClusterNetwork::instance()->getLocalHostname(),
                                                                       mListenAddr.getPort(), std::string("We need to add this to RIM")/*mManagerId*/,
                                                                       true);
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
               << "Sending responce: " << std::endl << vprDEBUG_FLUSH;
            
            responce_packet->printData(vprDBG_CONFIG_LVL);
            try
            {
               // THIS COULD BREAK IN THE FUTURE
               // We may need to convert this over to
               // using the ClusterNode send method soon
               responce_packet->send(client_sock);
            }
            catch(cluster::ClusterException cluster_exception)
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
                  << clrOutBOLD(clrRED,"ERROR:")
                  << "ClusterNetwork::acceptLoop() Caught an exception: " 
                  << cluster_exception.getMessage() << std::endl << vprDEBUG_FLUSH;

               node->setConnected(ClusterNode::DISCONNECTED);   
               delete client_sock;
               client_sock = new vpr::SocketStream;
               continue;
            }
            delete responce_packet;
                        
            // We need a new SocketStream since the old one is now being used by a ClusterNode
            client_sock = new vpr::SocketStream;
            
            //
            /////////////////////////////////////////////
            
            ClusterDelta cluster_delta;
            cluster_delta.clientClusterDelta(node->getSockStream());

            node->setConnected(ClusterNode::NEWCONNECTION);
            vpr::Guard<vpr::Mutex> guard(mPendingNodesLock);
            debugDumpClusterNodes(1);
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


   bool ClusterNetwork::startUpdating()
   {
      if ( mUpdateThread == NULL )  // If we haven't already started the listening thread
      {     
            // Start a thread to update Network
         vpr::ThreadMemberFunctor<ClusterNetwork>* memberFunctor =
         new vpr::ThreadMemberFunctor<ClusterNetwork>
         (this, &ClusterNetwork::updateLoop, NULL);
         
         mUpdateThread = new vpr::Thread(memberFunctor);
         vprASSERT(mUpdateThread->valid());
         return true;
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
               << "startUpdating() Update thread already active.\n" << vprDEBUG_FLUSH;
         return false;
      }
   }

   void ClusterNetwork::updateLoop(void* nullParam)
   {
      // AttemptConnections

      while(true)
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
         vpr::Thread::yield();
      }
   }

   void ClusterNetwork::shutdown()
   {     // Kill the accepting thread
      if ( mAcceptThread )
      {
         mAcceptThread->kill();
         mAcceptThread = NULL;
      }
      if ( mUpdateThread )
      {
         mUpdateThread->kill();
         mUpdateThread = NULL;
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
   std::string getShortHostnameFromLong(const std::string& long_name)
   {
      int first_period_pos = long_name.find(".");
      std::string temp = long_name.substr(0, first_period_pos);
      // just in case it is trailed by a ":port";
      int colon_pos = temp.find(":");
      std::string temp2 = temp.substr(0,colon_pos);
      return temp2;
   }



   //////////////////////////////////
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
         //  -Start Listening thread
         // -Else
         //  -Add Node to ClusterNetwork

         if (chunk->getProperty<std::string>("host_name") == getLocalHostname())
         {
            const int listen_port = chunk->getProperty<int>("listen_port");
            startListening(listen_port);
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[ClusterNetwork] ")
               << "ConfigAdd() Adding the Node: " << chunk->getName() 
               << " to the Cluster Network\n" << vprDEBUG_FLUSH;

            std::string    name        = chunk->getName();
            std::string    host_name   = chunk->getProperty<std::string>("host_name");
            vpr::Uint16    listen_port = chunk->getProperty<int>("listen_port");
            std::string    manager_id  = "001100";  // TODO: get an ID from the Remote Input Manager
           
            addClusterNode(name, host_name, listen_port, manager_id);
         }
         return(true);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) 
            << clrOutBOLD(clrRED,"[ClusterNetwork::ConfigAdd] ERROR, Something is seriously wrong, we should never get here\n")
            << vprDEBUG_FLUSH;
         return(true);
      }
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
      // If we gained a new connection last frame, we need to use it this frame
      vpr::Guard<vpr::Mutex> guard(mClusterNodesLock);

      for (std::vector<cluster::ClusterNode*>::iterator i = getClusterNodesBegin();
            i != getClusterNodesEnd() ; i++)
      {
         if ((*i)->getConnected() == ClusterNode::NEWCONNECTION)
         {
            (*i)->setConnected(ClusterNode::CONNECTED);
            (*i)->debugDump(vprDBG_CONFIG_LVL);
         }
      }
   }
   bool ClusterNetwork::isClusterNetworkReady()
   {
      return (0 == mPendingNodes.size());
   }
}	// end namespace cluster
