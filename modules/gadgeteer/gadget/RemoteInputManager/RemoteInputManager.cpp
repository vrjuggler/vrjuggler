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
//#include <gadget/Type/NetPosition.h>
//#include <gadget/Type/NetDigital.h>
#include <gadget/RemoteInputManager/NetDevice.h>
#include <gadget/RemoteInputManager/RemoteInputManager.h>
#include <gadget/RemoteInputManager/StreamReader.h>
#include <gadget/RemoteInputManager/StreamWriter.h>
#include <gadget/Type/DeviceFactory.h>
//#include <gadget/Type/DeviceInterface.h>

namespace gadget
{

   bool recognizeClusterMachineConfig(jccl::ConfigChunkPtr chunk);
   bool recognizeRemoteDeviceConfig(jccl::ConfigChunkPtr chunk);


   RemoteInputManager::RemoteInputManager(InputManager* input_manager)
   {
      vpr::ObjectReader* headReader = new vpr::ObjectReader(NULL);
      vpr::ObjectReader* dataReader = new vpr::ObjectReader(NULL);;

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
      vprDEBUG(gadgetDBG_INPUT_MGR,2) << "Remote Input Manager Id: "
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
         vprDEBUG(gadgetDBG_RIM,1) << clrOutNORM(clrGREEN,"[Remote Input Manager]") << " Adding the Cluster Machine named: " << machine << "\n"<< vprDEBUG_FLUSH;
         mMachineTable[machine] = chunk;
      }
      else if ( recognizeClusterSystemConfig(chunk) )
      {
         //ret_val = configureClusterSystem(chunk);
      }
      else if ( recognizeRemoteDeviceConfig(chunk) )
      {
         std::string device = chunk->getName();
         vprDEBUG(gadgetDBG_RIM,1) << clrOutNORM(clrGREEN,"[Remote Input Manager]") << " Adding a device chunk: " << device << "\n" << vprDEBUG_FLUSH;
         mDeviceChunks.push_back(chunk);
      }
      //   ret_val = configureDevice(chunk);
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
      //return ret_val;         // Return the success flag if we added at all
   }


   // Return true if:
   //  It is recognized device, proxy, or alias.
   bool RemoteInputManager::configCanHandle(jccl::ConfigChunkPtr chunk)
   {
      return( recognizeClusterMachineConfig(chunk) ||
              recognizeClusterSystemConfig(chunk) ||
              recognizeRemoteDeviceConfig(chunk) );
   }

   ////////////////////////////////////////////////////
   //     LOCAL HELPER FUNCTIONS
   ////////////////////////////////////////////////////
   bool RemoteInputManager::recognizeClusterMachineConfig(jccl::ConfigChunkPtr chunk)
   {
      //if (chunk->getDescToken() == std::string("cluster_machine"))
      //{
      //   std::cout << "recognizeClusterMachineConfig: TRUE" << std::endl;
      //}
      //else
      //{
      //   std::cout << "recognizeClusterMachineConfig: FALSE" << std::endl;
      //}
      return(chunk->getDescToken() == std::string("cluster_machine"));
   }
   
   bool RemoteInputManager::recognizeClusterSystemConfig(jccl::ConfigChunkPtr chunk)
   {
      return(chunk->getDescToken() == std::string("cluster_system"));
   }
   
   bool RemoteInputManager::recognizeRemoteDeviceConfig(jccl::ConfigChunkPtr chunk)
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

   // need to do this before we can accept any connections in accept loop
   bool RemoteInputManager::startListening()
   {
      if ( mAcceptThread == NULL )
      {     // if we haven't started already
         if ( mListenPort > 0 )
         {
            mListenAddr.setPort(mListenPort);

            vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
                  << "Starting the listening thread.\n" << vprDEBUG_FLUSH;

            // start a thread to monitor port
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
         //vpr::SocketStream client_sock;
         //vpr::Status status;
         //ssize_t bytes;
         // char buffer[] = "Hello there!";
         //        std::string buffer = "Hello there!";
         status = sock.setReuseAddr(true);
      }
      else
      {
         vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:") << "RemoteInputManager: unable to open listening socket on port: " << mListenAddr.getPort() << std::endl << vprDEBUG_FLUSH;
         exit(0);
      }
   
   
      while ( 1 )
      {
         // Wait for an incoming connection.
         status = sock.accept(*client_sock, vpr::Interval::NoTimeout);// vpr::Interval(60, vpr::Interval::Sec));
         if ( status.success() )
         {
            // Using the new socket, send the buffer to the client and close                // the socket.
            // status = client_sock.write(buffer, sizeof(buffer), bytes);
            //                status = client_sock.write(buffer, buffer.length(), bytes);
            // client_sock.close();
            // handshake, get the device name
   
            mConfigMutex.acquire();   // so we don't add/remove devices/connections while they are in use
   
            std::string streamHostname,streamManagerId; // name of device requested by the stream
            vpr::Uint16 streamPort;
            //char local_port_str[64];
            //sprintf(local_port_str, "%i", mListenPort);
   
            if ( mMsgPackage.receiveHandshake(streamHostname,streamPort, streamManagerId, client_sock) )
            {  // read the name from stream
   
               // create an alias_name to pass to addConnection
   
               NetConnection* new_connection = addConnection(vpr::Interval(0,vpr::Interval::Base),streamHostname, streamPort, streamManagerId, client_sock);  // pass the host:port as the alias_name also.
               if ( new_connection != NULL )
               {
                  // if connection is successfully added, send handshake
                  //    CLIENT_SOCK<<<<<<<<<<<<<<<<<<
                  mMsgPackage.createHandshake(true,mShortHostname,mListenPort, mManagerId.toString());   // send my name: send my hostname & port
                  mMsgPackage.sendAndClear(client_sock);
                  client_sock = new vpr::SocketStream;
                  mIncomingConnections++;
               }
               else
               {
                  // connection already exists or could not be made, so send rejection
                  mMsgPackage.createHandshake(false,mShortHostname,mListenPort,mManagerId.toString());
                  mMsgPackage.sendAndClear(client_sock);
                  // prepare new socket
                  client_sock->close();
                  delete client_sock;
                  client_sock = new vpr::SocketStream;
               }
            }
   
            else
            {
               mMsgPackage.createHandshake(false,mShortHostname, mListenPort, mManagerId.toString());   // send my manager's name
               mMsgPackage.sendAndClear(client_sock);
               // failed handshake so prepare new socket
               client_sock->close();
               delete client_sock;
               client_sock = new vpr::SocketStream;
            }
   
            mConfigMutex.release();   // so we don't add/remove devices while the list is in use
   
         }
         else if ( status == vpr::ReturnStatus::Timeout )
         {
            // will never happen since timeout is infinite
   
            client_sock->close();
            delete client_sock;
            client_sock = new vpr::SocketStream;
         }
      }   // end infinite while
   }


   void RemoteInputManager::shutdown()
   {
      if ( mAcceptThread )
      {
         mAcceptThread->kill();
         mAcceptThread = NULL;
      }

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

      //Figure out how many connectionwe need to wait for.
      for(std::map<std::string, jccl::ConfigChunkPtr>::iterator i=mMachineTable.begin();
          i!=mMachineTable.end();i++)
      {
         if ((*i).second->getProperty<std::string>("host_name") == getLocalHostName())
         {
            mLocalMachineChunkName = (*i).first;
            vprDEBUG(gadgetDBG_RIM,1) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
                  << " Found the local Cluster Machine Chunk: " << mLocalMachineChunkName << "\n"<< vprDEBUG_FLUSH;
         }
      }
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
      vprDEBUG(gadgetDBG_RIM,2) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
            << " Number of local devices: " << local_device_chunks.size() << "\n"<< vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,2) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
            << " Number of machines in cluster: " << mMachineTable.size() << "\n"<< vprDEBUG_FLUSH;
      
      //Configure local devices
      for (std::vector<jccl::ConfigChunkPtr>::iterator j = local_device_chunks.begin();
           j != local_device_chunks.end();j++)
      {
         vprDEBUG(gadgetDBG_RIM,1) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
            << " Configuring local device: " << (*j)->getName() << "\n"<< vprDEBUG_FLUSH;
         gadget::InputManager::instance()->configureDevice(*j);
      }

      //Start listening
      if (mLocalMachineChunkName != "")
      {
         configureClusterMachine(mMachineTable[mLocalMachineChunkName]);
      }
      
      //Configure remote devices
      for (std::vector<jccl::ConfigChunkPtr>::iterator j = remote_device_chunks.begin();
           j != remote_device_chunks.end();)
      {
         vprDEBUG(gadgetDBG_RIM,1) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
            << " Configuring remote device: " << (*j)->getName() << "\n"<< vprDEBUG_FLUSH;
         if (configureDevice(*j))
         {
            j++;
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,2) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
               << " Waiting for Remote Device Server: " << (*j)->getProperty<std::string>("host_chunk") << "\n"<< vprDEBUG_FLUSH;
         }
      }
      
      //Wait until #connections is correct
      if (tempNumLocalDevices > 0)
      {
         while (mIncomingConnections < (mMachineTable.size()-1))
         {//{;}
         vprDEBUG(gadgetDBG_RIM,2) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
            << mIncomingConnections << " - " << (mMachineTable.size()-1) << "\n"<< vprDEBUG_FLUSH;
         }
      }
      vprDEBUG(gadgetDBG_RIM,1) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
         << " Done with configuration!!!" << "\n"<< vprDEBUG_FLUSH;
      //MAKE A DEBUG SCREEN
      debugDump();
      //if ( mAcceptThread )
      //{
      //   mAcceptThread->kill();
      //   mAcceptThread = NULL;
      //}

   }

   void RemoteInputManager::debugDump()
   {
      vprDEBUG(gadgetDBG_RIM,1) << "\n"<< vprDEBUG_FLUSH;
      vprDEBUG_BEGIN(gadgetDBG_RIM,1) << clrOutNORM(clrGREEN,"Remote Input Manager Configuration")
            << "\n"<< vprDEBUG_FLUSH;
      for (std::list<NetConnection*>::iterator i = mConnections.begin();
           i != mConnections.end();i++)
      {
            (*i)->debugDump(); 
      }
      vprDEBUG_END(gadgetDBG_RIM,1) << clrOutNORM(clrGREEN,"Remote Input Manager Configuration")
      << "\n"<< vprDEBUG_FLUSH;

   }

   bool RemoteInputManager::configureClusterMachine(jccl::ConfigChunkPtr chunk)
   {
      vprDEBUG(gadgetDBG_RIM,1) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
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
   }
   bool RemoteInputManager::configureDevice(jccl::ConfigChunkPtr chunk)
   {
      vprASSERT(chunk->getNum("host_chunk") > 0 && "This chunk does not have a host_chunk");

         // Get the name of the device
         std::string dev_name = chunk->getFullName();

         vprDEBUG_BEGIN(gadgetDBG_RIM,1) << clrOutNORM(clrGREEN,"[Configure Net Device] ") << dev_name << "\n"<< vprDEBUG_FLUSH;

         std::string host_chunk = chunk->getProperty<std::string>("host_chunk");
         jccl::ConfigChunkPtr host_machine = mMachineTable[host_chunk];

         //Get the Host Name & Port that we want to conenct to
         std::string host_name = host_machine->getProperty<std::string>("host_name");
         int port_num = host_machine->getProperty<int>("listen_port");

         // We are configuring a NetDevice, so make sure connections don't change while we're here
         acquireConfigMutex();

         NetConnection* connection = this->getConnectionByHostAndPort(host_name,port_num);
         if ( connection != NULL )  //If connection exists, go for it.
         {
            vprDEBUG(gadgetDBG_RIM,1) << "Connection Already Exists.\n"<< vprDEBUG_FLUSH;
            vprDEBUG(gadgetDBG_RIM,1) << "Configuring:" << host_name << "-" << chunk->getName() << "\n"<< vprDEBUG_FLUSH;
            
            configureReceivingNetDevice(chunk, connection);  // requests a connection to the device
         }
         else  //Else we have to create a new connection first	
         {
            
            vprDEBUG(gadgetDBG_RIM,1) << "Connection does not exist, creating a new one.\n"<< vprDEBUG_FLUSH;
            vprDEBUG(gadgetDBG_RIM,1) << "Configuring:" << host_name << "-" << chunk->getName() << "\n"<< vprDEBUG_FLUSH;
            
            connection = makeConnection(host_name, port_num);
            
            if (connection != NULL)
            {
               configureReceivingNetDevice(chunk, connection);  // requests a connection to the device
            }
            else
            {
               vprDEBUG(gadgetDBG_RIM,1) << clrOutBOLD(clrRED,"ERROR: Could not create connection") << dev_name << "\n"<< vprDEBUG_FLUSH;
               vprDEBUG_END(gadgetDBG_RIM,1) << clrOutNORM(clrGREEN,"[END Configure Net Device] ") << dev_name << "\n"<< vprDEBUG_FLUSH;
               releaseConfigMutex();
               return false;//ERROR, could not connect, waiting for remote server
            }
         }
         vprDEBUG_END(gadgetDBG_RIM,1) << clrOutNORM(clrGREEN,"[END Configure Net Device] ") << dev_name << "\n"<< vprDEBUG_FLUSH;
         releaseConfigMutex();
         return(true);
   }

   // send and receive data from network devices
   void RemoteInputManager::updateAll()
   {
      if (!mConfigureDone)
      {
         //vprDEBUG(gadgetDBG_RIM,1) << clrOutNORM(clrGREEN,"[Remote Input Manager]") 
         //      << " Update All pending needs checked: " << (int)jccl::ConfigManager::instance()->pendingNeedsChecked() << "\n"<< vprDEBUG_FLUSH;
         mConfigureDone=!jccl::ConfigManager::instance()->pendingNeedsChecked();
      }
      else if (!mListenWasInitialized)
      {
         configureRIM();
      }
      else
      {
         
         //E while(! (this->checkStartingRequirements()))       // loop here until minimum connections have been accepted
         //E return; // System::sleep(1);  // let devices get loaded -- Should be a yield() instead of sleep
         mConfigMutex.acquire();  // Don't allow devices to be added or removed when we are accessing them
         // if any devices are still requesting to be connected, do that here
         resendRequestsForNackedDevices();
      
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=====================\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=    SENDING DATA   =\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=====================\n" << vprDEBUG_FLUSH;
      
         sendDeviceNetData();
      

         //vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         //   << "Receiving data from remote net devices.\n" << vprDEBUG_FLUSH;

         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=====================\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=  RECEIVEING DATA  =\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=====================\n" << vprDEBUG_FLUSH;
         
         receiveDeviceNetData();  // NEED ROAD BLOCK
         
         //updateManagerStatus();  // for any messages just received that require separate negotiation (i.e. Time Sync)
         //vpr::System::msleep(1);
         //InputManager::instance()->refreshAllProxies();
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
         (*i)->setAllPacketsUnreceived();
         if ((*i)->getSockStream() == NULL ||
             !(*i)->getSockStream()->isOpen() )
         {
            
            vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrSetBOLD(clrRED) << "ERROR: Lost connection to: " 
                  << (*i)->getHostname() << " BLOCKING: " << (int)(*i)->getSockStream()->getBlocking() 
                  << " Accept Thread NULL?: " << (int)(mAcceptThread == NULL) << vprDEBUG_FLUSH;
            vpr::Error::outputCurrentError(std::cout, " BlockIO readn ERROR: ");
            vprDEBUG_CONT(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) <<  clrRESET << "\n" << vprDEBUG_FLUSH;
            
            mConnections.erase(i);
            return;
         }
         else
         {
            
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
      
      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] Receive Network Packet\n") << vprDEBUG_FLUSH;
      
      vpr::Uint32 bytes_read;
      NetDevice* tmp_net_device;

      // Get the socket stream for this NetConnection
      vpr::SocketStream* socket_stream = net_connection->getSockStream();

      // Get the MsgPackage for this NetConnection
      gadget::MsgPackage* msg_package = net_connection->getMsgPackage();

      ///////////////////////////////
      // READ IN THE PACKET HEADER //
      ///////////////////////////////

      // Create a new vector can read the data into it
      std::vector<vpr::Uint8> packet_head(100);

      vpr::ReturnStatus status = socket_stream->recvn(packet_head,RIM_HEAD_LENGTH,bytes_read);
      if (bytes_read == -1)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrSetBOLD(clrRED) << "ERROR: Connection lost" << vprDEBUG_FLUSH;
         vpr::Error::outputCurrentError(std::cout, "BlockIO readn ERROR: ");
         vprDEBUG_CONT(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "\n" << clrRESET << vprDEBUG_FLUSH;
         return;
      }
      // Set the mData pointer in our headReader object to this new data,
      // and send the data cursor to the beginning
      //////////////////////////////////////////////////
      vpr::ObjectReader* head_reader = new vpr::ObjectReader(&packet_head);
      ////////////////////////////////////////////////

      // Read the info from the header
      vpr::Uint16 rimcode =  head_reader->readUint16();
      vpr::Uint16 opcode  =  head_reader->readUint16();
      vpr::Uint32 length  =  head_reader->readUint32();
      
      //vprASSERT(rimcode == RIM_PACKET_MSG && "Not a valid RIM packet!");
      head_reader->mData->clear();

      if (rimcode != RIM_PACKET_MSG)
      {
         net_connection->setAllPacketsReceived();
         vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"============ERROR, NOT A RIM PACKET=============\n") << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "BYTES:    " << (int)bytes_read << "\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "RIM CODE: " << (int)rimcode << "\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "OPCODE:   " << (int)opcode << "\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "LENGTH:   " << (int)length << "\n" << vprDEBUG_FLUSH;
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"================================================\n") << vprDEBUG_FLUSH;
         
         std::vector<vpr::Uint8> packet_data(100);
         bytes_read=0;
         status = socket_stream->recv(packet_data,1000,bytes_read,vpr::Interval(1,vpr::Interval::Sec));
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "PACKET: ";
         for (int i=0;i<packet_data.size();i++)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << (int)packet_data[i] << " ";
         }
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << vprDEBUG_FLUSH;
         packet_data.clear();
         return;
      }



      /////////////////////////////
      // READ IN THE PACKET DATA //
      /////////////////////////////

      // Create a new vector can read the data into it
      std::vector<vpr::Uint8> packet_data(100);
      bytes_read=0;

      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "PACKET SIZE: " << length << "\n" << vprDEBUG_FLUSH;
      
      status = socket_stream->recvn(packet_data,length-RIM_HEAD_LENGTH,bytes_read);
      vpr::ObjectReader* data_reader = new vpr::ObjectReader(&packet_data);

      switch ( opcode )
      {
      // Device Requested
      case MSG_DEVICE_REQ:  
         {
            vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] MSG_DEVICE_REQ\n") << vprDEBUG_FLUSH;
            msg_package->receiveDeviceRequest(data_reader);
            
            // Get the device's name from the MsgPackage
            std::string device_name = msg_package->getDataString();

            // find the device in the input manager
            Input* dev_ptr = InputManager::instance()->getDevice(device_name);  // see if we have the device requested

            // Check if we found the device in the Input Manager
            if ( dev_ptr != NULL )
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Found a local Device: " << device_name << vprDEBUG_FLUSH;
               // Configure will create a new Net Device if it can not find an existiing one, and send an DEVICE_ACK
               bool test = configureTransmittingNetDevice(device_name, msg_package->getSenderId(),net_connection);
            }
            else
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED) << "ERROR: " << device_name 
                     << " could not be found!" << clrRESET << vprDEBUG_FLUSH;
               // exit(0);  // will keep being retried
            }
         }
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] END MSG_DEVICE_REQ\n") << vprDEBUG_FLUSH;
         break;
      case MSG_DEVICE_ACK:
         {
            vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] MSG_DEVICE_ACK\n") << vprDEBUG_FLUSH;
            bool test = msg_package->receiveDeviceAck(data_reader);

            tmp_net_device = findDeviceByLocalId(msg_package->getReceiverId());
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Made a new Virtual device of BaseType: " 
                     << msg_package->getBaseType() <<"\n" << vprDEBUG_FLUSH;
            if ( tmp_net_device != NULL )
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Found the local device that made the request" << vprDEBUG_FLUSH;
               tmp_net_device->setRemoteId(msg_package->getSenderId());
               tmp_net_device->setWasInitializedTrue();
               Input* new_device;
               new_device = BaseTypeFactory::instance()->loadNetDevice(msg_package->getBaseType());
               tmp_net_device->setRealDevice(new_device);               //Points the to virtual device type
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
            
            bool test = msg_package->receiveDeviceAck(data_reader);
            tmp_net_device = findDeviceByLocalId(msg_package->getReceiverId());
            if ( tmp_net_device != NULL )
            {
               tmp_net_device->setRemoteId(msg_package->getSenderId());
               tmp_net_device->setNeedToResendRequestFlag();
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Marking for Re-Try" << vprDEBUG_FLUSH;
            }
            else
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrRED,"ERROR: Did not find a local device that made a call to the device server!" << vprDEBUG_FLUSH;
            }
         }
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[RIM Packet] END MSG_DEVICE_NACK\n") << vprDEBUG_FLUSH;
         break;
      case MSG_DEVICE_DATA:
         {
            vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] DEVICE_DATA\n") << vprDEBUG_FLUSH;
            vpr::Uint16 device_id = data_reader->readUint16();

            NetDevice* net_device_recvr = net_connection->findReceivingNetDeviceByLocalId(device_id);    
            if ( net_device_recvr == NULL )
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"ERROR NetConnection does not have a device numbered: ") 
                     << clrOutBOLD(clrRED,device_id) << "\n" << vprDEBUG_FLUSH;
               net_connection->debugDump();
               exit(0);
            }
            else
            {
               msg_package->recieveDeviceDataPacket(data_reader,net_device_recvr->getRealDevice());
            }
         }
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] END DEVICE_DATA\n") << vprDEBUG_FLUSH;         
         break;
      case MSG_CLUSTER_SYNC:
         mClusterSync.clientClusterSync(socket_stream);
         break;
      case MSG_END_BLOCK:
         //std::cout << "[RIM Packet]  END BLOCK" << std::endl;
         net_connection->setAllPacketsReceived();
         //std::cout << "[RIM Packet] (END) END BLOCK" << std::endl;
         break;
      default:
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR: UNKNOWN OPCODE\n") << vprDEBUG_FLUSH;
         break;
      }// END SWITCH
      vprDEBUG_END(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] END Receive Network Packet\n") << vprDEBUG_FLUSH;
      //std::cout << "[RIM Packet] END Receive Network Packet" << std::endl << std::endl;

      data_reader->mData->clear();
   }

   NetConnection* RemoteInputManager::makeConnection(const std::string& connection_hostname, const int connection_port)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Make connection: "<< connection_hostname <<":"<< connection_port << "\n"<< vprDEBUG_FLUSH;
      // this->initNetwork(); // needed for windows
      vpr::SocketStream* sock_stream;
      vpr::InetAddr inet_addr;

      // CONNECTING (code from VPR example)
      if ( ! inet_addr.setAddress(connection_hostname, connection_port).success() )
      {
         // if ( ! inet_addr.setAddress(connection_name).success() ) {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
               << clrOutNORM(clrRED," ERROR: Failed to set address\n")<< vprDEBUG_FLUSH;
         delete sock_stream;
         return NULL;
      }

      sock_stream = new vpr::SocketStream(vpr::InetAddr::AnyAddr, inet_addr);

      if ( sock_stream->open().success() && sock_stream->connect().success() )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
               << " Successfully connected to: " << connection_hostname << "\n"<< vprDEBUG_FLUSH;

         mMsgPackage.createHandshake(true,mShortHostname,mListenPort,mManagerId.toString());   // send my name: send my hostname & port
         mMsgPackage.sendAndClear(sock_stream);

         std::string received_hostname, received_manager_id;
         vpr::Uint16 received_port;
         if ( mMsgPackage.receiveHandshake(received_hostname, received_port, received_manager_id,sock_stream) )
         { // receive parameters from other manager, but to addConnection we'll actually use the variable names passed in from above
            
            vpr::Interval delta_time = mClusterSync.getClusterSync(sock_stream);
            NetConnection* new_connection = this->addConnection(delta_time, connection_hostname, connection_port, received_manager_id, sock_stream);    // hostname and port are saved to identify the connection.
            vprASSERT(new_connection != NULL); // addConnection should never fail since we requested connection
            return new_connection;
         }
      }
      delete sock_stream;
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
            << clrOutNORM(clrRED," ERROR: Failed to open connection\n")<< vprDEBUG_FLUSH;
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
      // first check if we have already set up this device to transmit on the network
      NetDevice* net_device = this->findTransmittingNetDevice(device_name);

      if ( net_device != NULL )
      {
         
         vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL) << "Transmitting NetDevice " << device_name << " already exists\n"  << vprDEBUG_FLUSH;
         net_device->addDependency(); // this->addTransmittingNetInput(net_input);  // actually just increments counter if already exists: net_input->addDependency();
         net_connection->addTransmittingNetDevice(net_device); // only does something if this net_connection isn't already requesting this Input's data
      }
      else
      {  // net_input == NULL.
         //  We didn't find it yet...so check our local Input Manager
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Looking in our local Input Manager for: " << device_name << "\n" << vprDEBUG_FLUSH;
         Input* input_ptr = mInputManager->getDevice(device_name);
         if ( input_ptr != NULL )
         {
            vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
            << "Input found: " << device_name << vprDEBUG_FLUSH;
            net_device = new NetDevice(device_name, input_ptr, this->generateLocalId(), requester_device_id);
            // this is done in the create function --> net_connection->addTransmittingNetInput(net_input);
            this->addTransmittingNetDevice(net_device, net_connection);
         }
         else
         {  // No devices found, search the proxies
            vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED) << "ERROR: Device " << device_name
            << " not found\n" << clrRESET << vprDEBUG_FLUSH;
         }
      }

      // now send acknowledgement if we have found the device
      if ( net_device != NULL )
      {
         // Send Acknowledgement and Id back to requester
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Sending acknoladgement that we have found the local device and are going to send its data: " << device_name << "\n" << vprDEBUG_FLUSH;

         mMsgPackage.createDeviceAck(net_device->getRemoteId(), net_device->getLocalId(), device_name, net_device->getRealDevice()->getBaseType());
         mMsgPackage.sendAndClear(net_connection->getSockStream());

         net_device->setWasInitializedTrue();
         vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
         << "setWasInitializedTrue() " << device_name << "\n" << vprDEBUG_FLUSH;
         return true;
      }
      else
      {
         return false;  // input device not found
      }

   }

   bool RemoteInputManager::configureReceivingNetDevice(jccl::ConfigChunkPtr chunk, NetConnection* net_connection)
   {
      std::string device_name = chunk->getFullName();

      // check if we are already receiving this device's data.
      NetDevice* net_device = NULL;//net_connection->findReceivingNetDevice(device_name);
      if ( net_device != NULL )
      {  // we already are receiving from this device, so mostly just increment it's dependency count
         // help us avoid receiving data from the same device more than once
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "We already have an instance of: " << device_name << "\n"<< vprDEBUG_FLUSH;

         net_device->addDependency(); // net_connection->addReceivingNetInput(net_input); //
         return true;
      }
      else
      {  // setup new structures to facilitate receiving and managing data from remote device
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Create a new NetDevice: " << device_name << "\n"<< vprDEBUG_FLUSH;
                  
         NetDevice* recv_net_device = net_connection->createReceivingNetDevice(chunk);
         if ( recv_net_device != NULL )
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Adding NetDevice to receiveing vector\n"<< vprDEBUG_FLUSH;
            this->addReceivingNetDevice(recv_net_device, net_connection);
            return true;
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrRED,"Failed trying to create: ") << device_name << "\n"<< vprDEBUG_FLUSH;
            
            return false;
         }
      }
      return true;  //TEMP
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
         return NULL;
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
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "addTransmittingNetDevice()" << vprDEBUG_FLUSH;
      net_connection->addTransmittingNetDevice(net_device);
      if ( std::find(mTransmittingDevices.begin(), mTransmittingDevices.end(), net_device) == mTransmittingDevices.end() )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
            << "[Remote Input Manager] Adding Transmitting Device to vector list" << vprDEBUG_FLUSH;
         mTransmittingDevices.push_back(net_device);
      }
      //vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      //   << "addReceivingNetInput() : " << net_input->getInstanceName()
      //   << std::endl << vprDEBUG_FLUSH;
      //mInputManager->addDevice( dynamic_cast<Input*> (net_input) );
      //vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      //   << "   Successfully added dev: " << net_input->getInstanceName().c_str()
      //   << std::endl << vprDEBUG_FLUSH;
   }

   //+++++++++++++++++++++++++++++++++++++



   //++++++++++++++++++++++++++++
   void RemoteInputManager::addReceivingNetDevice(NetDevice* net_device, NetConnection* net_connection)
   {
      net_connection->addReceivingNetDevice(net_device);
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
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Trying to find a existing transmitting device: " << device_name << vprDEBUG_FLUSH;
      for ( std::list<NetDevice*>::iterator i = mTransmittingDevices.begin();
          i!= mTransmittingDevices.end(); i++ )
      {
         if ( (*i)->getSourceName() == device_name )
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << " FOUND: " << device_name << vprDEBUG_FLUSH;
            return *i;
         }
      }
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "DIDN'T FIND: " << device_name << vprDEBUG_FLUSH;
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

   NetDevice* RemoteInputManager::findDeviceByLocalId(VJ_NETID_TYPE local_id)
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
