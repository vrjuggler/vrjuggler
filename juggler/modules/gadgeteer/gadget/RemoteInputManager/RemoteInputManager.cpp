/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>
#include <vpr/Util/Error.h>

#include <jccl/RTRC/ConfigChunkHandler.h>
#include <jccl/RTRC/ConfigManager.h>


#include <gadget/InputManager.h>
#include <gadget/Util/Debug.h>
#include <gadget/RemoteInputManager/NetUtils.h>
#include <gadget/RemoteInputManager/RecvBuffer.h>
#include <gadget/Type/InputMixer.h>
#include <gadget/RemoteInputManager/NetDevice.h>
#include <gadget/RemoteInputManager/RemoteInputManager.h>
#include <gadget/Type/DeviceFactory.h>


namespace gadget
{

   bool recognizeClusterMachineConfig(jccl::ConfigChunkPtr chunk);
   bool recognizeRemoteDeviceConfig(jccl::ConfigChunkPtr chunk);

   bool recognizeClusterMachineConfig(jccl::ConfigChunkPtr chunk)
   {
      return(chunk->getDescToken() == std::string("cluster_machine"));
   }
   
   bool recognizeRemoteDeviceConfig(jccl::ConfigChunkPtr chunk)
   {  //Can not check with BaseTypeFactory since we do not know thw BaseType yet
   
      if ( DeviceFactory::instance()->recognizeDevice(chunk) &&  chunk->getNum("host_chunk") > 0 )
      {
         std::string host_chunk = chunk->getProperty<std::string>("host_chunk");
         if ( !host_chunk.empty() )
         {
            return true;
         }// Else it is an empty chunk_ptr
      }// Else it is not a device, or does not have a host_chunk property
      return false;
   }


   RemoteInputManager::RemoteInputManager(InputManager* input_manager)
   {
      mLocalMachineChunkName = "";
      mConfigureDone = false;
      mActive = true;
      mIncomingConnections = 0;
      mNumMachines = 0;
      mAcceptThread = NULL;
      mListenPort = 7394; // will always be changed
      mInputManager = input_manager;

      vpr::InetAddr local_addr;
      vpr::InetAddr::getLocalHost(local_addr);
      mLongHostname = local_addr.getHostname();

         // now strip the tail of the address to just get the computer name
      mShortHostname = getShortHostnameFromLong(mLongHostname);

      mListenWasInitialized = 0;
      mManagerId.generate();
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Remote Input Manager Id: "
      << mManagerId << std::endl
      << vprDEBUG_FLUSH;
   }

   RemoteInputManager::~RemoteInputManager()
   {
      shutdown();  // kills thread, removes devices
   }




   bool RemoteInputManager::configAdd(jccl::ConfigChunkPtr chunk)
   {
      vprASSERT(configCanHandle(chunk));
      // check and store if a Remote Input Manager chunk exists in configuration
      
      mActive = true;
      
      if ( recognizeClusterMachineConfig(chunk) )
      {
         std::string machine = chunk->getName();
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]") << " Adding the Cluster Machine named: " << machine << "\n"<< vprDEBUG_FLUSH;
         mMachineTable[machine] = chunk;
      }
      else if ( recognizeRemoteDeviceConfig(chunk) )
      {
         std::string device = chunk->getName();
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]") << " Adding a device chunk: " << device << "\n" << vprDEBUG_FLUSH;
         mDeviceChunks.push_back(chunk);
      }
      return true;         // Return the success flag if we added at all
   }
   bool RemoteInputManager::configRemove(jccl::ConfigChunkPtr chunk)
   {
      vprDEBUG_BEGIN(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "\nInputManager: Removing config... " << std::endl << vprDEBUG_FLUSH;
      vprASSERT(configCanHandle(chunk));

      bool ret_val = false;      // Flag to return success
      // CAN NOT REMOVE YET
      //if (recognizeClusterMachineConfig(chunk)) 
      //	{;}
      //else if(recognizeRemoteDeviceConfig(chunk))
      //   ret_val = removeDevice(chunk);
      return ret_val;         // Return the success flag if we added at all
   }

   bool RemoteInputManager::configCanHandle(jccl::ConfigChunkPtr chunk)
   {
      return( recognizeClusterMachineConfig(chunk) ||
              recognizeClusterSystemConfig(chunk) ||
              recognizeRemoteDeviceConfig(chunk) );
   }

   bool RemoteInputManager::startListening()
   {
      if ( mAcceptThread == NULL )  // If we haven't started already
      {     
         if ( mListenPort > 0 )     // If the listen port is valid
         {
            mListenAddr.setPort(mListenPort);

            vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
                  << "Starting the listening thread.\n" << vprDEBUG_FLUSH;

               // Start a thread to monitor port
            vpr::ThreadMemberFunctor<RemoteInputManager>* memberFunctor =
            new vpr::ThreadMemberFunctor<RemoteInputManager>
            (this, &RemoteInputManager::acceptLoop, NULL);
            
            mAcceptThread = new vpr::Thread(memberFunctor);
            vprASSERT(mAcceptThread->valid());
            return true;
         }
         else
         {
            vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
            << "RemoteInputManager:startListening: cannot listen on port: "
            << mListenPort << "\n" << vprDEBUG_FLUSH;
            return false;
         }
      }
      else
      {
         vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
         << "RemoteInputManager:startListening: listening thread already active.\n" << vprDEBUG_FLUSH;
         return false;
      }
   }

   void RemoteInputManager::acceptLoop(void* nullParam)
   {
      
      // this->initNetwork(); // needed for windows
      vpr::ReturnStatus status;
      vpr::SocketStream* client_sock = new vpr::SocketStream;
      
         // Create an acceptor socket that listens on port.
      vpr::SocketStream sock(mListenAddr, vpr::InetAddr::AnyAddr);
         // Open in server mode.
      if ( sock.openServer().success() )
      {
         status = sock.setReuseAddr(true);
      }
      else
      {
         vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:") << "RemoteInputManager: unable to open listening socket on port: " << mListenAddr.getPort() << std::endl << vprDEBUG_FLUSH;
         exit(0);
      }
   
         
      while ( 1 )
      {     // Wait for an incoming connection.
         status = sock.accept(*client_sock, vpr::Interval::NoTimeout);// vpr::Interval(60, vpr::Interval::Sec));
         if ( status.success() )
         {
            mConfigMutex.acquire();   // Aquire a Mutex so we don't add/remove devices/connections while they are in use
   
            std::string streamHostname,streamManagerId;
            vpr::Uint16 streamPort;
   
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetNORM(clrYELLOW) << "[makeConnection]"
               << "Waiting for a handshake responce on port: "<< mListenPort << "\n"<< clrRESET << vprDEBUG_FLUSH;
               
               // If we receive a handshake successfully
            if ( mAcceptMsgPackage.receiveHandshake(streamHostname,streamPort, streamManagerId, client_sock) )
            {  
                  // Create a new NetConnection
               NetConnection* new_connection = addConnection(vpr::Interval(0,vpr::Interval::Base),streamHostname, streamPort, streamManagerId, client_sock);  // pass the host:port as the alias_name also.
                  // If created successfully return a positive responce
               if ( new_connection != NULL )
               {
                  mAcceptMsgPackage.createHandshake(true,mShortHostname,mListenPort, mManagerId.toString());   // send my name: send my hostname & port
                  mAcceptMsgPackage.sendAndClear(client_sock);
                  client_sock = new vpr::SocketStream;
                  mIncomingConnections++;
               }
               else  // Failed to create NetConnection, return a negative responce
               {
                  mAcceptMsgPackage.createHandshake(false,mShortHostname,mListenPort,mManagerId.toString());
                  mAcceptMsgPackage.sendAndClear(client_sock);
                     // Close and delete old socket
                  client_sock->close();
                  delete client_sock;
                  client_sock = new vpr::SocketStream;
               }
            }
            else  // Failed to receive a handshake, return a negative responce
            {
               mAcceptMsgPackage.createHandshake(false,mShortHostname, mListenPort, mManagerId.toString());   // send my manager's name
               mAcceptMsgPackage.sendAndClear(client_sock);
               client_sock->close();
               delete client_sock;
               client_sock = new vpr::SocketStream;
            }
   
            mConfigMutex.release();   // Aquire a Mutex so we don't add/remove devices/connections while they are in use
   
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

   void RemoteInputManager::shutdown()
   {     // Kill the accepting thread
      if ( mAcceptThread )
      {
         mAcceptThread->kill();
         mAcceptThread = NULL;
      }
         // Remove all connections
      while ( mConnections.size() > 0 )
      {
         delete *(mConnections.begin()) ;
         mConnections.pop_front();
      }
   }

   bool RemoteInputManager::configureRIM()
   {
      unsigned tempNumLocalDevices = 0;
      
      std::vector<jccl::ConfigChunkPtr> local_device_chunks;
      std::vector<jccl::ConfigChunkPtr> remote_device_chunks;

         // Figure out how many connectionwe need to wait for.
         // Find the name of the local machine chunk. Since all machine chunks are
         // in a std::map we can simple reference it by name
      for(std::map<std::string, jccl::ConfigChunkPtr>::iterator i=mMachineTable.begin();
          i!=mMachineTable.end();i++)
      {
         if ((*i).second->getProperty<std::string>("host_name") == getLocalHostName())
         {
            mLocalMachineChunkName = (*i).first;
            vprDEBUG(gadgetDBG_RIM,vprDBG_STATE_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
                  << " Found the local Cluster Machine Chunk: " << mLocalMachineChunkName << "\n"<< vprDEBUG_FLUSH;
         }
      }

         // Seperate devices from local and remote
      for (std::vector<jccl::ConfigChunkPtr>::iterator j = mDeviceChunks.begin();
           j != mDeviceChunks.end();j++)
      {
         if ((*j)->getProperty<std::string>("host_chunk") == mLocalMachineChunkName)
         {
            local_device_chunks.push_back(*j);
            tempNumLocalDevices++;
         }
         else
         {
            remote_device_chunks.push_back(*j);
         }
      }
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrMAGENTA) << "[Remote Input Manager]"
            << " Number of local devices: " << local_device_chunks.size() << "\n"<< clrRESET << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrMAGENTA) << "[Remote Input Manager]"
            << " Number of machines in cluster: " << mMachineTable.size() << "\n"<< clrRESET << vprDEBUG_FLUSH;
      
         // Configure local devices
      for (std::vector<jccl::ConfigChunkPtr>::iterator j = local_device_chunks.begin();
           j != local_device_chunks.end();j++)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetNORM(clrMAGENTA) << "[Remote Input Manager]"
            << " Configuring local device: " << (*j)->getName() << "\n"<< clrRESET << vprDEBUG_FLUSH;
            // Call the Input Manager Device Configuration
         gadget::InputManager::instance()->configureDevice(*j);
      }

         // Start listening
      if (mLocalMachineChunkName != "")
      {
         configureClusterMachine(mMachineTable[mLocalMachineChunkName]);
      }
      
         // Configure remote devices
      for (std::vector<jccl::ConfigChunkPtr>::iterator j = remote_device_chunks.begin();
           j != remote_device_chunks.end();)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetNORM(clrMAGENTA) << "[Remote Input Manager]"
            << " Configuring remote device: " << (*j)->getName() << "\n"<< clrRESET << vprDEBUG_FLUSH;
         if (configureDevice(*j))
         {
            j++;
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_STATE_LVL) << clrSetNORM(clrMAGENTA) << "[Remote Input Manager]"
               << " Waiting for Remote Device Server: " << (*j)->getProperty<std::string>("host_chunk") 
               << "\n"<< clrRESET << vprDEBUG_FLUSH;
         }
      }
      
         // Wait until #connections is correct
      if (tempNumLocalDevices > 0)
      {
         while (mIncomingConnections < (mMachineTable.size()-1))
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
               << mIncomingConnections << " - " << (mMachineTable.size()-1) << "\n"<< vprDEBUG_FLUSH;
         }
      }
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
         << " Done with device configuration!" << "\n"<< vprDEBUG_FLUSH;
      debugDump();
      return true;
   }

   void RemoteInputManager::debugDump()
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "\n"<< vprDEBUG_FLUSH;
      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"==========Remote Input Manager Configuration==========")
            << "\n"<< vprDEBUG_FLUSH;
      for (std::list<NetConnection*>::iterator i = mConnections.begin();
           i != mConnections.end();i++)
      {
            (*i)->debugDump(); 
      }
      vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"===========Remote Input Manager Configuration==========")
      << "\n"<< vprDEBUG_FLUSH;
   }

   bool RemoteInputManager::configureClusterMachine(jccl::ConfigChunkPtr chunk)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
            << " Configure Local Device Server: " << chunk->getFullName() 
            << " " << chunk->getProperty<std::string>("host_name") << ":" 
            << chunk->getProperty<std::string>("listen_port") << "\n"<< vprDEBUG_FLUSH;
            
      
            acquireConfigMutex();

            // Make sure that we are not already listening on this machine
            vprASSERT(mListenWasInitialized == false);

            mListenPort = chunk->getProperty<int>("listen_port");
            // int num_connections = chunk->getProperty("num_connections");
            mInstanceName = chunk->getFullName();

            // forcedHostname is used when a machine has two network interfaces, 
            // and you want to use the non-default one
            std::string forcedHostname = chunk->getProperty<std::string>("forced_hostname");

            if ( forcedHostname.size() > 0 )
            {
               mLongHostname = forcedHostname;
               mShortHostname = getShortHostnameFromLong(mLongHostname);
            }

            mListenWasInitialized = true;
            startListening();

            releaseConfigMutex();
         return true;
   
   }
   bool RemoteInputManager::configureClusterSystem(jccl::ConfigChunkPtr chunk)
   {
      this->mNumMachines = chunk->getNum("cluster_machine");
      return true;
   }
   bool RemoteInputManager::configureDevice(jccl::ConfigChunkPtr chunk)
   {
      vprASSERT(chunk->getNum("host_chunk") > 0 && "This chunk does not have a host_chunk");

         // Get the name of the device
         std::string dev_name = chunk->getFullName();

         vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[Configure Net Device] ") << dev_name << "\n"<< vprDEBUG_FLUSH;

         std::string host_chunk = chunk->getProperty<std::string>("host_chunk");
         jccl::ConfigChunkPtr host_machine = mMachineTable[host_chunk];

         //Get the Host Name & Port that we want to conenct to
         std::string host_name = host_machine->getProperty<std::string>("host_name");
         int port_num = host_machine->getProperty<int>("listen_port");

         // We are configuring a NetDevice, so make sure connections don't change while we're here
//         acquireConfigMutex();

         NetConnection* connection = this->getConnectionByHostAndPort(host_name,port_num);
         if ( connection != NULL )  //If connection exists, go for it.
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Connection Already Exists.\n"<< vprDEBUG_FLUSH;
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Configuring:" << host_name << "-" << chunk->getName() << "\n"<< vprDEBUG_FLUSH;
            
            configureReceivingNetDevice(chunk, connection);  // requests a connection to the device
         }
         else  //Else we have to create a new connection first	
         {
            
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Connection does not exist, creating a new one.\n"<< vprDEBUG_FLUSH;
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Configuring:" << host_name << "-" << chunk->getName() << "\n"<< vprDEBUG_FLUSH;
            
            connection = makeConnection(host_name, port_num);
            
            if (connection != NULL)
            {
               configureReceivingNetDevice(chunk, connection);  // requests a connection to the device
            }
            else
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrRED,"ERROR: Could not create connection") << dev_name << "\n"<< vprDEBUG_FLUSH;
               vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[END Configure Net Device] ") << dev_name << "\n"<< vprDEBUG_FLUSH;
               //releaseConfigMutex();
               return false;//ERROR, could not connect, waiting for remote server
            }
         }
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[END Configure Net Device] ") << dev_name << "\n"<< vprDEBUG_FLUSH;
//         releaseConfigMutex();
         return(true);
   }

   // send and receive data from network devices
   void RemoteInputManager::updateAll()
   {
      if (jccl::ConfigManager::instance()->pendingNeedsChecked() ||
          !jccl::ConfigManager::instance()->isChunkTypeInActiveList("cluster_machine") )
      {
         //Do Nothing
      }
      else if (!mListenWasInitialized)
      {
         configureRIM();
      }
      else
      {
         mConfigMutex.acquire();  // Don't allow devices to be added or removed when we are accessing them
         
         // if any devices are still requesting to be connected, do that here
         resendRequestsForNackedDevices();
      
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=====================\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=    SENDING DATA   =\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=====================\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "Number Connections: " << this->mConnections.size() << "\n" <<  vprDEBUG_FLUSH;
         
         sendDeviceNetData();

         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=====================\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=  RECEIVEING DATA  =\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=====================\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "Number Connections: " << this->mConnections.size() << "\n" << vprDEBUG_FLUSH;
         
         receiveDeviceNetData();  // NEED ROAD BLOCK
         
         mConfigMutex.release();  // Don't allow devices to be added or removed when we are accessing them
         
         //BaseDeviceInterface::refreshAllDevices();      // Refresh all the device interface handles
         
      }
   }
   
   void RemoteInputManager::sendDeviceNetData()
   {
      std::list<NetConnection*>::iterator i;
      for ( i = mConnections.begin(); i != mConnections.end();i++ )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "[RIM] Sending data for: " << (*i)->getHostname() << "\n" << vprDEBUG_FLUSH;
         (*i)->sendNetworkData();
      }
   }

   void RemoteInputManager::receiveDeviceNetData()
   {
      std::list<NetConnection*>::iterator i;
      for ( i = mConnections.begin(); i != mConnections.end();i++ )
      {
         (*i)->setAllPacketsUnreceived();          // Set a flag saying all connections have not been updated
         
         if ((*i)->getSockStream() == NULL ||      // If we have lost the connection
             !(*i)->getSockStream()->isOpen() )
         {
               // Print debug information, including system error
            vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrSetBOLD(clrRED) << "ERROR: Lost connection to: " 
                  << (*i)->getHostname() << " BLOCKING: " << (int)(*i)->getSockStream()->getBlocking() 
                  << " Accept Thread NULL?: " << (int)(mAcceptThread == NULL) << vprDEBUG_FLUSH;
            vpr::Error::outputCurrentError(std::cout, " BlockIO readn ERROR: ");
            vprDEBUG_CONT(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrRESET << "\n" << vprDEBUG_FLUSH;
               // Remove NetConnection so that we can continue without it
            mConnections.erase(i);
            return;
         }
         else
         {
               // While this connection has not received an END_BLOCK
            while ( !(*i)->getAllPacketsReceived() )
            {
               receiveNetworkPacket(*i);
            }
            vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "ALL PACKETS RECEIVED for: " << (*i)->getHostname() << "\n" << vprDEBUG_FLUSH;
         }
      }
   }
   void RemoteInputManager::receiveNetworkPacket(NetConnection* net_connection)
   {
      ///////////////////////////////////////////////////////////////////////
      //		  
      // PARSE PACKET	  
      //				  
      //	FORM:		 vpr::Uint16		vpr::Uint16		 vpr::Uint32
      //			--------------------------------------------------------
      //			| RIM_PACKET_MSG | MSG_DEVICE_DATA |   length   | data |
      //			-------------------------------------------------------	
      //																	
      ///////////////////////////////////////////////////////////////////////
      
      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrGREEN,"[RIM::receiveNetworkPacket] Read Network Packet\n") << vprDEBUG_FLUSH;
      
      vpr::Uint32 bytes_read;
      NetDevice* tmp_net_device;

         // Get the socketStream and MsgPackage for this NetConnection
      vpr::SocketStream* socket_stream = net_connection->getSockStream();
      gadget::MsgPackage* msg_package = net_connection->getMsgPackage();

      ///////////////////////////////
      // READ IN THE PACKET HEADER //
      ///////////////////////////////

         // Create a new vector can read the data into it
      std::vector<vpr::Uint8> packet_head(100);
         //Read in the known length of the packet header
      vpr::ReturnStatus status = socket_stream->recvn(packet_head,RIM_HEAD_LENGTH,bytes_read);
         // If the read was not successful 
      if (!status.success() || bytes_read < 0)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrSetBOLD(clrRED) << "[RIM::receiveNetworkPacket] ERROR: Connection lost" << vprDEBUG_FLUSH;
            // Print the low level error
         vpr::Error::outputCurrentError(std::cout, "BlockIO readn ERROR: ");
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "\n" << clrRESET << vprDEBUG_FLUSH;
         return;
      }
      
         // Create a new ObjectReader to retreive our data
      vpr::ObjectReader* head_reader = new vpr::ObjectReader(&packet_head);
      
         // Parse the data to get the Packet Header info
      vpr::Uint16 rimcode =  head_reader->readUint16();
      vpr::Uint16 opcode  =  head_reader->readUint16();
      vpr::Uint32 length  =  head_reader->readUint32();
      
         // We can clear the data vector since we are done with it
      head_reader->mData->clear();

      if (rimcode != RIM_PACKET_MSG)   // Packet not valid
      {
         net_connection->setAllPacketsReceived();
         vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"============ERROR, NOT A RIM PACKET=============\n") << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "BYTES:    " << (int)bytes_read << "\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "RIM CODE: " << (int)rimcode << "\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "OPCODE:   " << (int)opcode << "\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "LENGTH:   " << (int)length << "\n" << vprDEBUG_FLUSH;
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"================================================\n") << vprDEBUG_FLUSH;
         
         std::vector<vpr::Uint8> packet_data(100);
         status = socket_stream->recvn(packet_data,            // Empty the buffer and try to get valid data next time around
                                      socket_stream->availableBytes(),
                                      bytes_read);
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "PACKET: ";  // Try to display to contents of the buffer
         for (unsigned i=0;i<packet_data.size();i++)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << (int)packet_data[i] << " ";
         }
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "\n" << vprDEBUG_FLUSH;
         packet_data.clear();
         return;
      }

      /////////////////////////////
      // READ IN THE PACKET DATA //
      /////////////////////////////

         // Create a new vector can read the data into it
      std::vector<vpr::Uint8> packet_data(100);
      
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "PACKET SIZE: " << length << "\n" << vprDEBUG_FLUSH;
      
         // For alignment reasons we MUST grab the device_id now if a data packet
      vpr::Uint16 device_id;
      if (opcode == MSG_DEVICE_DATA)
      {
         status = socket_stream->recvn(&device_id,2,bytes_read);
         device_id = vpr::System::Ntohs(device_id);
         vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrSetNORM(clrCYAN) 
            << "==============DEVICE ID: " 
            << device_id << "==================\n" << clrRESET << vprDEBUG_FLUSH;
            
            // Must also read 2 fewer byts since we just read two
         status = socket_stream->recvn(packet_data,length-RIM_HEAD_LENGTH-2,bytes_read);
      }
      else
      {
         status = socket_stream->recvn(packet_data,length-RIM_HEAD_LENGTH,bytes_read);
      }
      
      
         // Create a new ObjectReader to parse our incoming data
      vpr::ObjectReader* data_reader = new vpr::ObjectReader(&packet_data);

      switch ( opcode )
      {
      case MSG_DEVICE_REQ:    // Remote device is requesting a device from the local machine
         {
            vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] MSG_DEVICE_REQ\n") << vprDEBUG_FLUSH;
               
               // Have msg_package receive the device request
            msg_package->receiveDeviceRequest(data_reader);
               
               // Try to configure the NetDevice, it will send a responce
            configureTransmittingNetDevice(msg_package->getDeviceName(), msg_package->getSenderId(),net_connection);
            
            vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] END MSG_DEVICE_REQ\n") << vprDEBUG_FLUSH;
            break;
         }
      case MSG_DEVICE_ACK:
         {
            vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] MSG_DEVICE_ACK\n") << vprDEBUG_FLUSH;
            
               // Have msg_package receive the device acknoledgement
            msg_package->receiveDeviceAck(data_reader);
               
               // Try to find the NetDevice for the device we requested
            tmp_net_device = findNetDeviceByLocalId(msg_package->getReceiverId());
            
            if ( tmp_net_device != NULL )
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Found the local device that made the request" << vprDEBUG_FLUSH;
                  
                  // Set the device servers device ID for this device
               tmp_net_device->setRemoteId(msg_package->getSenderId());
                  
                  // Try to create a "virtual device"
               Input* new_device = BaseTypeFactory::instance()->loadNetDevice(msg_package->getBaseType());

               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Made a new Virtual device of BaseType: " 
                        << msg_package->getBaseType() <<"\n" << vprDEBUG_FLUSH;
                  
                  // ASSERT if we can not create a "virtual device" because we have failed
               vprASSERT(new_device != NULL && "BaseTypeFactor failed to find base type");
                  
                  //Points the to virtual device type               
               tmp_net_device->setRealDevice(new_device);
               
                  // Set a flag that we have configured this NetDevice correctly
               tmp_net_device->setWasInitializedTrue();

                  // Tell the input Mangager that we may now have the device it was trying to point to
               InputManager::instance()->refreshAllProxies();
            }
            else
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED) 
                     << "ERROR: Did not find a local device that made a call to the device server!" 
                     << clrRESET << vprDEBUG_FLUSH;
            }
         }
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] END MSG_DEVICE_ACK\n") << vprDEBUG_FLUSH;         
         break;
      case MSG_DEVICE_NACK:
         {
            vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] MSG_DEVICE_NACK\n") << vprDEBUG_FLUSH;
            
            msg_package->receiveDeviceAck(data_reader);
            tmp_net_device = findNetDeviceByLocalId(msg_package->getReceiverId());
            if ( tmp_net_device != NULL )
            {
               tmp_net_device->setRemoteId(msg_package->getSenderId());
               tmp_net_device->setNeedToResendRequestFlag();
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Marking for Re-Try" << vprDEBUG_FLUSH;
            }
            else
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrRED,"ERROR: Did not find a local device that made a call to the device server!") << vprDEBUG_FLUSH;
            }
         }
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] END MSG_DEVICE_NACK\n") << vprDEBUG_FLUSH;
         break;
      case MSG_DEVICE_DATA:   // Data for a given device has been received
         {
            vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] DEVICE_DATA\n") << vprDEBUG_FLUSH;
            
               // Try to find the NetDevice for this remote device
            NetDevice* net_device_recvr = net_connection->findReceivingNetDeviceByLocalId(device_id);    
            if ( net_device_recvr == NULL )
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutBOLD(clrRED,"ERROR NetConnection does not have a device numbered: ") 
                     << clrOutBOLD(clrRED,device_id) << "\n" << vprDEBUG_FLUSH;
               debugDump();
               exit(0);
            }
            else
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "Receiving data for: " << net_device_recvr->getSourceName() << "\n" << vprDEBUG_FLUSH;
               msg_package->recieveDeviceDataPacket(data_reader,net_device_recvr->getRealDevice());
            }
         }
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] END DEVICE_DATA\n") << vprDEBUG_FLUSH;         
         break;
      case MSG_CLUSTER_SYNC:  // Other machine is asking for a sync communication 
         mClusterSync.clientClusterSync(socket_stream);
         break;
      case MSG_END_BLOCK:
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] END_BLOCK\n") << vprDEBUG_FLUSH;
         net_connection->setAllPacketsReceived();
         break;
      default:
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR: UNKNOWN OPCODE\n") << vprDEBUG_FLUSH;
         break;
      }
      vprDEBUG_END(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] END Receive Network Packet\n") << vprDEBUG_FLUSH;
      
         // Clear the data vector since we will not need this data again
      data_reader->mData->clear();
   }

      //NOTE: BLOCKS WHEN WAITING FOR RETURN HANDSHAKE
   NetConnection* RemoteInputManager::makeConnection(const std::string& connection_hostname, const int connection_port)
   {
      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[RIM::makeConnection] " 
         << connection_hostname <<":"<< connection_port << "\n"<< vprDEBUG_FLUSH;

      vpr::SocketStream* sock_stream;
      vpr::InetAddr inet_addr;

         // Set the address that we want to connect to
      if ( !inet_addr.setAddress(connection_hostname, connection_port).success() )
      {
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrRED," ERROR: Failed to set address\n") << vprDEBUG_FLUSH;
         return NULL;
      }
         // Create a new socket stream to this address
      sock_stream = new vpr::SocketStream(vpr::InetAddr::AnyAddr, inet_addr);

         // If we can successfully open the socket and connect to the server
      if ( sock_stream->open().success() && sock_stream->connect().success() )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << " Successfully connected to: " 
            << connection_hostname <<":"<< connection_port << "\n"<< vprDEBUG_FLUSH;

            // Send a handshake to initalize communication with remote computer
         mMsgPackage.createHandshake(true,mShortHostname,mListenPort,mManagerId.toString());
         mMsgPackage.sendAndClear(sock_stream);


         std::string received_hostname;
         std::string received_manager_id;
         vpr::Uint16 received_port;
         
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetNORM(clrYELLOW) 
            << "Waiting for a responce handshake responce on: " << connection_hostname <<":"
            << connection_port << "\n"<< clrRESET << vprDEBUG_FLUSH;
         
            // Wait for the remote machine to respond with a handshake
         if ( mMsgPackage.receiveHandshake(received_hostname, received_port, received_manager_id,sock_stream) )
         { 
            vpr::Interval delta_time = mClusterSync.getClusterSync(sock_stream);       // Find Sync time for connection
            
               // Does the conenction already exist
            if ( getConnectionByManagerId(received_manager_id) != NULL )
            {
               vprDEBUG_END(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
                  << "SUCCESS - RemoteInputManager: Connection to " << connection_hostname
                  <<" : "<< connection_port <<" : "<< received_manager_id << "\n" << vprDEBUG_FLUSH;
               return getConnectionByManagerId(received_manager_id);
            }
            else
            { 
                  // Create a new NetConnection
               NetConnection* connection = new NetConnection(delta_time ,connection_hostname, connection_port, received_manager_id, sock_stream);
               
                  // Add NetConnection
               mConnections.push_back(connection);
               
               vprDEBUG_END(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
               << "SUCCESS - RemoteInputManager: Added connection to " << connection_hostname
               <<" : "<< connection_port <<" : "<< received_manager_id << "\n" << vprDEBUG_FLUSH;
               return connection;
            }
         }
      }
         // Delete un-need socket
      delete sock_stream;
      vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetNORM(clrRED) << "ERROR: Failed to open connection" 
         << connection_hostname <<" : "<< connection_port << "\n" << clrRESET << vprDEBUG_FLUSH;
      return NULL;
   }

   Input* RemoteInputManager::getDevice(const std::string device_name)
   {
      for ( std::list<NetDevice*>::iterator i = mReceivingDevices.begin();
          i!= mReceivingDevices.end(); i++ )
      {
         if ( (*i)->getSourceName() == device_name )
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Proxy is now attached to remote device: " << device_name << "\n" << vprDEBUG_FLUSH;
            return((*i)->getRealDevice());
         }
      }
      return NULL;
   }

   bool RemoteInputManager::configureTransmittingNetDevice(std::string device_name, VJ_NETID_TYPE requester_device_id, NetConnection* net_connection)
   {
      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[RIM::ConfigureTransmittingNetDevice] " 
         << device_name << "\n"<< vprDEBUG_FLUSH;

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Looking in our local Input Manager for: " << device_name << "\n" << vprDEBUG_FLUSH;
      
      Input* input_ptr = mInputManager->getDevice(device_name);
      NetDevice* net_device;
      if ( input_ptr != NULL )   // Device exists locally
      {
         vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
            << "InputManager found: " << device_name << vprDEBUG_FLUSH;

         net_device = new NetDevice(device_name, input_ptr, this->generateLocalId(), requester_device_id);
         
         if (net_device != NULL)    // Successfully created transmitting NetDevice
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Successful, now adding to receiveing NetDevices\n"<< vprDEBUG_FLUSH;
            addTransmittingNetDevice(net_device, net_connection);
            net_connection->addTransmittingNetDevice(net_device);
         }
         else
         {
            vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED) << "ERROR, could not create NetDevice: " << device_name << "\n" << clrRESET << vprDEBUG_FLUSH;
         }
      }
      else
      {  // No devices found, search the proxies
         vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED) << "ERROR: Device " << device_name
         << " not found\n" << clrRESET << vprDEBUG_FLUSH;
      }
      
      //
      // SEND A RESPONCE BACK TO REQUESTING MACHINE
      //
      if ( net_device != NULL )
      {
            // Send Acknowledgement and Id back to requester
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Sending acknoladgement that we ere successful in sharing: " << device_name << "\n" << vprDEBUG_FLUSH;

            // Send an acknolegement
         mMsgPackage.createDeviceAck(net_device->getRemoteId(), net_device->getLocalId(), device_name, net_device->getRealDevice()->getBaseType());
         mMsgPackage.sendAndClear(net_connection->getSockStream());
         
            // Send an END_BLOCK since we are done sending data on this connection
         mMsgPackage.createEndBlock();
         mMsgPackage.sendAndClear(net_connection->getSockStream());
            
            // Set a flag saying that we have in dead set up this NetDevice correctly
         net_device->setWasInitializedTrue();
         vprDEBUG_END(gadgetDBG_RIM, vprDBG_CONFIG_LVL) << "Transmitting NetDevice, " << device_name 
               << " has been created successfully.\n" << vprDEBUG_FLUSH;
         return true;
      }
      else
      {
            // Send NAcknowledgement that we encountered an error
         mMsgPackage.createDeviceNack(net_device->getRemoteId(), 0, device_name);
         mMsgPackage.sendAndClear(net_connection->getSockStream());

            // Send an END_BLOCK since we are done sending data on this connection
         mMsgPackage.createEndBlock();
         mMsgPackage.sendAndClear(net_connection->getSockStream());

         delete net_device;
         vprDEBUG_END(gadgetDBG_RIM, vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED) << "ERROR: Transmitting NetDevice, " << device_name 
               << " could not be created!\n" << clrRESET << vprDEBUG_FLUSH;
         return false;
      }
   }

   bool RemoteInputManager::configureReceivingNetDevice(jccl::ConfigChunkPtr chunk, NetConnection* net_connection)
   {
         // Get the name of the device that we are trying to configure
      std::string device_name = chunk->getFullName();
      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[RIM::ConfigureReceivingNetDevice] " 
         << device_name << "\n"<< vprDEBUG_FLUSH;

         //Check if we have this NetDevice Already
      NetDevice* net_device = net_connection->findReceivingNetDevice(device_name);
      
         
      if ( net_device != NULL )  // We are already receiving data for this device
      {  
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED) << "We already have an instance of: " 
               << device_name << "\n" << clrRESET << vprDEBUG_FLUSH;
         return false;
      }
      else  // setup new structures to receive and manage data from the remote device
      {  
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Create a new NetDevice: " << device_name << "\n"<< vprDEBUG_FLUSH;
                  
         NetDevice* recv_net_device = new NetDevice(device_name, this->generateLocalId());  // Create a new receiving NetDevice
         
         if ( recv_net_device != NULL ) // NetDevice was created successfully
         {
            vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Successful, now adding to receiveing NetDevices\n"<< vprDEBUG_FLUSH;
            addReceivingNetDevice(recv_net_device);                     // Add NetDevice to mReceivingNetDevices
            net_connection->addReceivingNetDevice(recv_net_device);     // Add NetDevice to NetConnection
            net_connection->sendDeviceRequest(recv_net_device);         // Send a request to the device server
            return true;
         }
         else
         {
            vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrRED,"Failed trying to create a NetDevice: ") 
               << device_name << "\n"<< vprDEBUG_FLUSH;
            return false;
         }
      }
   }

   // addConnection if it doesn't exist already
   NetConnection* RemoteInputManager::addConnection(vpr::Interval sync_delta, const std::string& connection_hostname, const int connection_port, const std::string& manager_id, vpr::SocketStream* sock_stream)
   {
      // make sure connection doesn't exist already:
      if ( getConnectionByManagerId(manager_id) != NULL )
      {
         vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
         << "RemoteInputManager: Connection to " << connection_hostname
         <<" : "<< connection_port <<" : "<< manager_id << " already exists."
         << std::endl << vprDEBUG_FLUSH;
         return getConnectionByManagerId(manager_id);
         //return NULL;
      }
      else
      { // add connection
         NetConnection* connection = new NetConnection(sync_delta ,connection_hostname, connection_port, manager_id, sock_stream);
         mConnections.push_back(connection);
         vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
         << "RemoteInputManager: Added connection to " << connection_hostname
         <<" : "<< connection_port <<" : "<< manager_id << std::endl
         << vprDEBUG_FLUSH;
         return connection;
      }
   }

   void RemoteInputManager::addTransmittingNetDevice(NetDevice* net_device, NetConnection* net_connection)
   {
      if ( std::find(mTransmittingDevices.begin(), mTransmittingDevices.end(), net_device) == mTransmittingDevices.end() )
      {
         mTransmittingDevices.push_back(net_device);
      }
   }

   //+++++++++++++++++++++++++++++++++++++



   //++++++++++++++++++++++++++++
   void RemoteInputManager::addReceivingNetDevice(NetDevice* net_device)
   {
      if ( std::find(mReceivingDevices.begin(), mReceivingDevices.end(), net_device) == mReceivingDevices.end() )
      {
         mReceivingDevices.push_back(net_device);
      }
   }
   //++++++++++++++++++++++++++++



   bool RemoteInputManager::allDataReceived()
   {
      // check if connections are waiting for EndBlocks()
      for ( std::list<NetConnection*>::iterator i = mConnections.begin();
          i != mConnections.end(); i++ )
      {
         // std::cout << "Checking for match: " << (*i)->getName() << ", " << location_name << std::endl;
         if ( ! (*i)->getAllPacketsReceived() )
         {
            return false;
         }
      }

      return true;
   }

   void RemoteInputManager::markDataUnreceived()
   {

      for ( std::list<NetConnection*>::iterator i = mConnections.begin();
          i != mConnections.end(); i++ )
      {
         (*i)->setAllPacketsUnreceived();
      }
   }


   NetDevice* RemoteInputManager::findTransmittingNetDevice(const std::string& device_name)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Trying to find a existing transmitting device: " << device_name << "\n" << vprDEBUG_FLUSH;
      for ( std::list<NetDevice*>::iterator i = mTransmittingDevices.begin();
          i!= mTransmittingDevices.end(); i++ )
      {
         if ( (*i)->getSourceName() == device_name )
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << " FOUND: " << device_name << "\n" << vprDEBUG_FLUSH;
            return *i;
         }
      }
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "DIDN'T FIND: " << device_name << "\n" << vprDEBUG_FLUSH;
      return NULL;
   }


   NetConnection* RemoteInputManager::getConnectionByHostAndPort(const std::string& hostname, const int port)
   {
      // Method to check strings
      for ( std::list<NetConnection*>::iterator i = mConnections.begin();
          i != mConnections.end(); i++ )
      {
         // std::cout << "Checking for match: " << (*i)->getName() << ", " << location_name << std::endl;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Checking for match: " << (*i)->getName() << ", " << hostname<<":"<<port << std::endl << vprDEBUG_FLUSH;
         if ( ((*i)->getHostname() == hostname) && ((*i)->getPort() == port) )
         {
            return *i;
         }
      }
      return NULL;
   }

   NetConnection* RemoteInputManager::getConnectionByAliasName(const std::string& alias_name)
   {
      // Method to check strings
      for ( std::list<NetConnection*>::iterator i = mConnections.begin();
          i != mConnections.end(); i++ )
      {
         //std::cout << "Checking for match: " << (*i)->printAliasNames() << ", " << alias_name << std::endl;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Checking for match: " << (*i)->printAliasNamesToString() << ", " << alias_name << std::endl << vprDEBUG_FLUSH;
         if ( (*i)->hasAliasName(alias_name) )
         {
            return *i;
         }
      }
      return NULL;
   }

   NetConnection* RemoteInputManager::getConnectionByManagerId(const vpr::GUID& manager_id)
   {
      for ( std::list<NetConnection*>::iterator i = mConnections.begin();
          i != mConnections.end(); i++ )
      {
         if ( (*i)->getManagerId() == manager_id )
         {
            return *i;
         }
      }
      return NULL;
   }

   NetDevice* RemoteInputManager::findNetDeviceByLocalId(VJ_NETID_TYPE local_id)
   {
      std::list<NetDevice*>::iterator i;

      // check Transmitting Devices
      for ( i = mTransmittingDevices.begin(); i != mTransmittingDevices.end(); i++ )
      {
         if ( (*i)->getLocalId() == local_id )
         {
            return(*i);
         }
      }

      // check Receiving Devices
      for ( i = mReceivingDevices.begin(); i != mReceivingDevices.end(); i++ )
      {
         if ( (*i)->getLocalId() == local_id )
         {
            return(*i);
         }
      }

      return NULL;
   }

   void RemoteInputManager::resendRequestsForNackedDevices()
   {
      for ( std::list<NetConnection*>::iterator i = mConnections.begin();
          i != mConnections.end(); i++ )
      {
         (*i)->resendRequestsForNackedDevices();
      }
   }
   
}  // end namespace gadget
