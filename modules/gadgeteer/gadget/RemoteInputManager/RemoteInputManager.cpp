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

//#include <jccl/RTRC/ConfigChunkHandler.h>
#include <jccl/RTRC/ConfigManager.h>


#include <gadget/InputManager.h>
#include <gadget/Util/Debug.h>
#include <gadget/RemoteInputManager/NetUtils.h>

#include <gadget/Type/InputMixer.h>
#include <gadget/RemoteInputManager/NetDevice.h>
#include <gadget/RemoteInputManager/NetConnection.h>

#include <gadget/RemoteInputManager/RemoteInputManager.h>
#include <gadget/Type/BaseTypeFactory.h>
#include <gadget/Type/DeviceFactory.h>

#include <gadget/RemoteInputManager/ClusterBarrier.h>

#include <vpr/IO/Port/SerialPort.h>
#include <vpr/IO/Socket/SocketStream.h>


namespace gadget
{
   char spiner()
   {
      static float spinCounter = 0;
      static std::string spinner = "\\|/-";
      spinCounter += .0005;
      return spinner[ static_cast<int>(spinCounter) % spinner.size() ];	
   }

   RemoteInputManager::RemoteInputManager(InputManager* input_manager)
   {
      mBarrier = NULL;

      mLocalMachineChunkName = "";
      mSyncServer=NULL;
      mActive = false;
      mConfigured = false;
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

      mSerialPort = NULL;

   }

   RemoteInputManager::~RemoteInputManager()
   {
      shutdown();  // kills thread, removes devices
      if (mSerialPort != NULL)
      {
         mSerialPort->close();
         delete mSerialPort;
      }
   }

   vpr::ObjectWriter* RemoteInputManager::getObjectWriter(std::string device_name, gadget::Input* input_device)
   {
      if (mCachedDeviceData[device_name] != NULL)
      {
         return(mCachedDeviceData[device_name]);
      }
      else
      {
         mTransmittingDevicePointers[device_name] = input_device;
         mCachedDeviceData[device_name] = new vpr::ObjectWriter;
         return(mCachedDeviceData[device_name]);
      }
   }


   void RemoteInputManager::shutdown()
   {     // Kill the accepting thread
      if ( mAcceptThread )
      {
         mAcceptThread->kill();
         mAcceptThread = NULL;
      }
         // Remove all connections
      while ( mTransmittingConnections.size() > 0 )
      {
         delete *(mTransmittingConnections.begin()) ;
         mTransmittingConnections.pop_front();
      }
         // Remove all connections
      while ( mReceivingConnections.size() > 0 )
      {
         delete *(mReceivingConnections.begin()) ;
         mReceivingConnections.pop_front();
      }
   }
   
   void RemoteInputManager::debugDump()
   {
      vprDEBUG_TSColorGuard(clrGREEN);
      vprDEBUG_OutputGuard(gadgetDBG_RIM, vprDBG_CONFIG_LVL,
                                 std::string("==========Remote Input Manager Configuration==========\n"),
                                 std::string("==========Remote Input Manager Configuration==========\n"));
   
      for (std::list<NetConnection*>::iterator i = mTransmittingConnections.begin();
           i != mTransmittingConnections.end();i++)
      {
            (*i)->debugDump(); 
      }
   
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
            << clrOutNORM(clrGREEN,"================Receiving NetConnections==============\n") << vprDEBUG_FLUSH;
   
      for (std::list<NetConnection*>::iterator i = mReceivingConnections.begin();
           i != mReceivingConnections.end();i++)
      {
            (*i)->debugDump(); 
      }
   }


   // -------------------- Remote Input Manager Configuration --------------------

   bool RemoteInputManager::configAdd(jccl::ConfigChunkPtr chunk)
   {
      vprASSERT(configCanHandle(chunk));
      
      // If we have a RIMChunk configChunk
      if ( this->recognizeRemoteInputManagerConfig(chunk)) 
      {
         // Add all MachineSpecific chunks if they are in the current list of Cluster nodes
         ////////////////////////////////////////////////////////////////////////////////////
         
         int numNodes = chunk->getNum("cluster_nodes");
         for (int i=0;i<numNodes;i++)
         {
            std::string node = chunk->getProperty<std::string>("cluster_nodes",i);
            if (mMachineTable[node].get() != NULL)
            {
               mClusterTable[node] = mMachineTable[node];
            }
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrCYAN) << "Adding Cluster Node: " << node << clrRESET << std::endl << vprDEBUG_FLUSH;
         }
         ////////////////////////////////////////////////////////////////////////////////////
         
         
         // Find the name of the local machine chunk. Since all machine chunks are
         // in a std::map we can simple reference it by name
         ///////////////////////////////////////////////////////////////////////////////////

         for(std::map<std::string, jccl::ConfigChunkPtr>::iterator i=mClusterTable.begin();
             i!=mClusterTable.end();i++)
         {
            // if ((*i).second->getProperty<std::string>("host_name") == getLocalHostName())
            if (this->hostnameMatchesLocalHostname((*i).second->getProperty<std::string>("host_name")))
            {
               mLocalMachineChunkName = (*i).first;
               vprDEBUG(gadgetDBG_RIM,vprDBG_STATE_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
                     << " Found the local Cluster Machine Chunk: " << mLocalMachineChunkName << "\n"<< vprDEBUG_FLUSH;
            }
         }
         ////////////////////////////////////////////////////////////////////////////////////

         // Find the name of the Sync Server
         ///////////////////////////////////////////////////////////////////////////////////
         mSyncMasterChunkName = chunk->getProperty<std::string>("sync_machine");
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "SYNC_MACHINE is: " << mSyncMasterChunkName << std::endl << vprDEBUG_FLUSH;
         ///////////////////////////////////////////////////////////////////////////////////

         // Configure the ClusterBarrier
         //////////////////////////////////////////////////////////////////////////////////
         
            // Temporary variables to hold SyncServer information
         jccl::ConfigChunkPtr sync_server_chunk = mClusterTable[mSyncMasterChunkName];
         std::string          sync_server_hostname = sync_server_chunk->getProperty<std::string>("host_name");
         int                  sync_server_listen_port = sync_server_chunk->getProperty<int>("listen_port");
         
            // Get Barrier method
         int                  sync_method = chunk->getProperty<int>("sync_method");

         jccl::ConfigChunkPtr local_machine_chunk = mClusterTable[mLocalMachineChunkName];
         std::string          serial_port = local_machine_chunk->getProperty<std::string>("serialPort");
         int                  baud_rate = local_machine_chunk->getProperty<int>("serialBaud");
         
         switch (sync_method)
         {
         case 0:
            mBarrier = new gadget::ClusterBarrierTCP;
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << 
               "ClusterBarrierTCP Barrier Method: TCP/IP Sockets \n" << vprDEBUG_FLUSH;
            break;
         case 1:
            mBarrier = new gadget::ClusterBarrierSerial;
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << 
               "ClusterBarrierSerial Barrier Method: TCP/IP Sockets & Serial Port \n" << vprDEBUG_FLUSH;
            break;
/*         case 2:
            mBarrier = new gadget::ClusterBarrierUDP;


            for(std::map<std::string, jccl::ConfigChunkPtr>::iterator i=mClusterTable.begin();
                i!=mClusterTable.end();i++)
            {
               // if ((*i).second->getProperty<std::string>("host_name") == getLocalHostName())
               if (!this->hostnameMatchesLocalHostname((*i).second->getProperty<std::string>("host_name")))
               {
                  mBarrier->AddBarrierSlave((*i).second->getProperty<std::string>("host_name"));
                  vprDEBUG(gadgetDBG_RIM,vprDBG_STATE_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
                        << " Found the local Cluster Machine Chunk: " << mLocalMachineChunkName << "\n"<< vprDEBUG_FLUSH;
               }
            }



            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << 
               "ClusterBarrierSerial Barrier Method: UDP Sockets \n" << vprDEBUG_FLUSH;
            break;
*/
         default:
            mBarrier = new gadget::ClusterBarrierTCP;
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << 
               "ClusterBarrierTCP Barrier Method: TCP/IP Sockets \n" << vprDEBUG_FLUSH;
            break;
         }
         
         mBarrier->setHostname(sync_server_hostname);
         mBarrier->setTCPPort(sync_server_listen_port);
         mBarrier->setSerialPort(serial_port);
         mBarrier->setBaudRate(baud_rate);
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrCYAN)                                
             << "=========== Barrier Settings ==============="
             << "\nBarrier-SyncServer Chunk:  " << mSyncMasterChunkName 
             << "\nBarrier-SyncServer Host    " << sync_server_hostname 
             << "\nBarrier-SyncServer Port:   " << sync_server_listen_port
             << "\nBarrier-Local Serial Port: " << serial_port
             << "\nBarrier-Local Baud Rate:   " << baud_rate << clrRESET << std::endl << vprDEBUG_FLUSH;
   
         // Have all clients connect to sync server
         //   If master 
         //   - Set master true
         //   Else
         //   - connect to sync Master server  
   
         if (this->hostnameMatchesLocalHostname(sync_server_hostname))
         {
            mBarrier->setMaster(true);
            mBarrier->Init();
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "SYNC This machine is sync server!" << std::endl << vprDEBUG_FLUSH;
         }
         else
         {
            mBarrier->Init();
         }
         //////////////////////

            // RIM is now active but not completely configured
         mActive = true;
      }
      else if ( recognizeClusterMachineConfig(chunk) )
      {
         std::string machine = chunk->getName();
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]") << " Adding the Cluster Machine named: " << machine << "\n"<< vprDEBUG_FLUSH;
         mMachineTable[machine] = chunk;
      }
      else if ( recognizeRemoteDeviceConfig(chunk) )
      {
         std::string device = chunk->getName();
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]") << " Adding a device chunk: " << device << "\n" << vprDEBUG_FLUSH;
         mPendingDeviceChunks.push_back(chunk);
      }
      return true;         // Return the success flag if we added at all
   }
   bool RemoteInputManager::configRemove(jccl::ConfigChunkPtr chunk)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
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
              recognizeRemoteDeviceConfig(chunk) ||
              recognizeRemoteInputManagerConfig(chunk) );
   }

   bool RemoteInputManager::configureRIM()
   {
      std::vector<jccl::ConfigChunkPtr> local_device_chunks;
      std::vector<jccl::ConfigChunkPtr> remote_device_chunks;
   
         // Figure out how many connectionwe need to wait for.
   
   
      ///////////////////////////////////
   
      ///////////////////////////////////////////////////////////////////////
   
         // Seperate devices from local and remote
      for (std::vector<jccl::ConfigChunkPtr>::iterator j = mPendingDeviceChunks.begin();
           j != mPendingDeviceChunks.end();j++)
      {
         if ((*j)->getProperty<std::string>("host_chunk") == mLocalMachineChunkName)
         {
            local_device_chunks.push_back(*j);
         }
         else
         {
            remote_device_chunks.push_back(*j);
         }
      }
      mPendingDeviceChunks.clear();
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrMAGENTA) << "[Remote Input Manager]"
            << " Number of local devices: " << local_device_chunks.size() << "\n"<< clrRESET << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrMAGENTA) << "[Remote Input Manager]"
            << " Number of machines in cluster: " << mClusterTable.size() << "\n"<< clrRESET << vprDEBUG_FLUSH;
   
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
         configureClusterMachine(mClusterTable[mLocalMachineChunkName]);
      }
   
         // Configure remote devices
      for (std::vector<jccl::ConfigChunkPtr>::iterator j = remote_device_chunks.begin();
           j != remote_device_chunks.end();)
      {
   //         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetNORM(clrMAGENTA) << "[Remote Input Manager]"
   //            << " Configuring remote device: " << (*j)->getName() << "\n"<< clrRESET << vprDEBUG_FLUSH;
         if (configureDevice(*j))
         {
            j++;
         }
         else
         {
   //            vprDEBUG(gadgetDBG_RIM,vprDBG_STATE_LVL) << clrSetNORM(clrMAGENTA) << "[Remote Input Manager]"
   //               << " Waiting for Remote Device Server: " << (*j)->getProperty<std::string>("host_chunk") 
   //               << "\n"<< clrRESET << vprDEBUG_FLUSH;
         }
      }
   
         // Wait until #connections is correct
      if (local_device_chunks.size() > 0)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
            << clrOutNORM(clrGREEN,"Waiting for remaining cluster machines to connect...\n")<< vprDEBUG_FLUSH;
         while (mTransmittingConnections.size() /*+ mReceivingConnections.size()*/ < (mClusterTable.size()-1))
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
               << mTransmittingConnections.size() << " - " << (mClusterTable.size()-1) << "\n"<< vprDEBUG_FLUSH;
         }
      }
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[Remote Input Manager]")
         << " Done with device configuration!" << "\n"<< vprDEBUG_FLUSH;
      debugDump();
   
      mConfigured = true;
      return true;
   }
   
   bool RemoteInputManager::recognizeClusterMachineConfig(jccl::ConfigChunkPtr chunk)
   {
      return(chunk->getDescToken() == this->getMachineSpecificChunkType());
   }
   
   bool RemoteInputManager::recognizeRemoteInputManagerConfig(jccl::ConfigChunkPtr chunk)
   {
      return(chunk->getDescToken() == this->getChunkType());
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
   
            mListenWasInitialized = true;
            startListening();
   
            releaseConfigMutex();
         return true;
   
   }
   
   bool RemoteInputManager::configureDevice(jccl::ConfigChunkPtr chunk)
   {
      vprASSERT(chunk->getNum("host_chunk") > 0 && "This chunk does not have a host_chunk");
   
         // Get the name of the device
      std::string dev_name = chunk->getFullName();
      std::string host_chunk = chunk->getProperty<std::string>("host_chunk");
      jccl::ConfigChunkPtr host_machine = mClusterTable[host_chunk];
   
   
      vprDEBUG_TSColorGuard(clrGREEN);
      vprDEBUG_OutputGuard(gadgetDBG_RIM, vprDBG_CONFIG_LVL,
         std::string("[Configure Net Device] " + dev_name + "\n"),
         std::string("[END Configure Net Device] " + dev_name + "\n"));
   
   
      //Get the Host Name & Port that we want to conenct to
      std::string host_name = host_machine->getProperty<std::string>("host_name");
      int port_num = host_machine->getProperty<int>("listen_port");
   
      // We are configuring a NetDevice, so make sure connections don't change while we're here
   
      NetConnection* connection = this->getReceivingConnectionByHostAndPort(host_name,port_num);
      if ( connection != NULL )  //If connection exists, go for it.
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Connection Already Exists.\n"<< vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Configuring:" << host_name << "-" << chunk->getName() << "\n"<< vprDEBUG_FLUSH;
   
         connection->configureReceivingNetDevice(chunk);
         //configureReceivingNetDevice(chunk, connection);  // requests a connection to the device
      }
      else  //Else we have to create a new connection first	
      {
   
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Connection does not exist, creating a new one.\n"<< vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Configuring:" << host_name << "-" << chunk->getName() << "\n"<< vprDEBUG_FLUSH;
   
         connection = NULL;
         do
         {
            std::cout << clrSetBOLD(clrMAGENTA) << "[Remote Input Manager]"
            << " Waiting for Remote Device Server: " << host_name << ":" << port_num 
            << "[" << spiner() << "]" << "\r" << clrRESET;
   
            connection = makeConnection(host_name, port_num);
         }while(connection == NULL);
   
   
   
         //if (host_chunk == this->mSyncMachine)
         //{
         //    this->mSyncServer = this->makeSyncConnection(host_name,port_num);
         ///}
   
   
         if (connection != NULL)
         {
            //configureReceivingNetDevice(chunk, connection);  // requests a connection to the device
            connection->configureReceivingNetDevice(chunk);
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrRED,"ERROR: Could not create connection") << dev_name << "\n"<< vprDEBUG_FLUSH;
            return false;//ERROR, could not connect, waiting for remote server
         }
      }
      return(true);
   }

   // -------------------- END Remote Input Manager Configuration --------------------

   // -------------------- Remote Input Manager Syncronization --------------------

   void RemoteInputManager::createBarrier()
   {
      // If Slave
      // - send ready to Master
      // - read ready from Server
      // Else if Master
      // - for each slave
      //   - read ready
      // - for each slave
      //   - send ready      
   
      if (mConfigured && mBarrier != NULL && mBarrier->isActive())
      {
         if (mBarrier->isMaster())
         {
            mBarrier->MasterReceive();
            mBarrier->MasterSend();
         }
         else
         {
            //vpr::Interval first_time, second_time;
            //first_time.setNow();
   
            mBarrier->SlaveSend();
            mBarrier->SlaveReceive();
   
            //second_time.setNow();
            //vpr::Interval diff_time(second_time-first_time);
            //std::cout << "Latency:" << setw(8) << diff_time.getBaseVal() << " usecs" << std::endl;
         }
      }
   }

   // -------------------- END Remote Input Manager Syncronization --------------------

   //-------------------- Remote Input Manager Flags --------------------
   
   bool RemoteInputManager::allDataReceived()
   {
      // check if connections are waiting for EndBlocks()
      for ( std::list<NetConnection*>::iterator i = mReceivingConnections.begin();
          i != mReceivingConnections.end(); i++ )
      {
         if ( ! (*i)->getAllPacketsReceived() )
         {
            return false;
         }
      }
      for ( std::list<NetConnection*>::iterator i = mTransmittingConnections.begin();
          i != mTransmittingConnections.end(); i++ )
      {
         if ( ! (*i)->getAllPacketsReceived() )
         {
            return false;
         }
      }
   
      return true;
   }
   
   void RemoteInputManager::markDataUnreceived()
   {
   
      for ( std::list<NetConnection*>::iterator i = mReceivingConnections.begin();
          i != mReceivingConnections.end(); i++ )
      {
         (*i)->setAllPacketsUnreceived();
      }
      for ( std::list<NetConnection*>::iterator i = mTransmittingConnections.begin();
          i != mTransmittingConnections.end(); i++ )
      {
         (*i)->setAllPacketsUnreceived();
      }
   }
   
   //END-------------------- Remote Input Manager Flags --------------------
   
   //-------------------- Remote Input Manager Getters/Setters --------------------
   
   NetConnection* RemoteInputManager::findReceivingDevice(const std::string device_name)
   {
       for ( std::list<NetConnection*>::iterator i = mReceivingConnections.begin();
           i!= mReceivingConnections.end(); i++ )
       {
          if ( (*i)->findNetDeviceConnection(device_name) )
          {
             return((*i));
          }
       }
       return NULL;
   }
   
   NetConnection* RemoteInputManager::findTransmittingDevice(const std::string device_name)
   {
      for ( std::list<NetConnection*>::iterator i = mTransmittingConnections.begin();
          i!= mTransmittingConnections.end(); i++ )
      {
          if ( (*i)->findNetDeviceConnection(device_name) )
          {
             return((*i));
          }
      }
      return NULL;
   }

   Input* RemoteInputManager::getDevice(const std::string device_name)
   {
      for (std::list<NetConnection*>::iterator i = mReceivingConnections.begin();
           i != mReceivingConnections.end(); i++)
      {
         Input* temp = (*i)->getDevice(device_name);
         if (temp != NULL)
         {
            return(temp);
         }
      }
      return NULL;
   }

   //END-------------------- Remote Input Manager Getters/Setters --------------------

   //-------------------- Remote Input Manager Connection Management --------------------

      // send and receive data from network devices
   void RemoteInputManager::updateAll()
   {
      if (jccl::ConfigManager::instance()->pendingNeedsChecked())
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
         
         // Send Device Data
         // SendAndClear  including END_BLOCKS on all connections
         
         // Receive data on Receive Connections and, vprASSERT on anything left on the socket
         // Receive data on Transmitting Connections and, vprASSERT on anything left on the socket
         
         // if any devices are still requesting to be connected, do that here
         resendRequestsForNackedDevices();
      
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=====================\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=    SENDING DATA   =\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=====================\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "Number Transmitting Connections: " << this->mTransmittingConnections.size() << "\n" <<  vprDEBUG_FLUSH;
         
         for (std::map<std::string, gadget::Input*>::iterator i = mTransmittingDevicePointers.begin();
              i != mTransmittingDevicePointers.end();i++)
         {
            vpr::ObjectWriter* writer = mCachedDeviceData[(*i).first];
            writer->getData()->clear();
            writer->setCurPos(0);
            (*i).second->writeObject(writer);
         }
         
            // Send Device Data
         sendNetworkData();

            // SendAndClear all connections, this could pick up device_ack's
         for (std::list<NetConnection*>::iterator i = mTransmittingConnections.begin();
              i != mTransmittingConnections.end();i++)
         {
               (*i)->getMsgPackage()->createEndBlock();
               (*i)->getMsgPackage()->sendAndClear((*i)->getSockStream());
         }
         for (std::list<NetConnection*>::iterator i = mReceivingConnections.begin();
              i != mReceivingConnections.end();i++)
         {
               (*i)->getMsgPackage()->createEndBlock();
               (*i)->getMsgPackage()->sendAndClear((*i)->getSockStream());
         }
         

         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=====================\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=  RECEIVEING DATA  =\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "=====================\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "Number Receiving Connections: " << this->mReceivingConnections.size() << "\n" << vprDEBUG_FLUSH;
         
         readReceivingConnectionData();  // 
         
         readTransmittingConnectionData();           // Read in END_BLOCKS
         
         for (std::list<NetConnection*>::iterator i = mReceivingConnections.begin();
              i != mReceivingConnections.end();i++)
         {
             if ((*i)->getSockStream()->availableBytes() > 0)
             {
                 vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "ERROR: RECEIVING Bytes on socket: " << (*i)->getSockStream()->availableBytes() << std::endl << vprDEBUG_FLUSH;
                 std::cout << "ERROR: RECEIVING Bytes on socket: " << (*i)->getSockStream()->availableBytes() << std::endl;
             }
         }
         for (std::list<NetConnection*>::iterator i = mTransmittingConnections.begin();
              i != mTransmittingConnections.end();i++)
         {
            if ((*i)->getSockStream()->availableBytes() > 0)
            {
                vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "ERROR: TRANS Bytes on socket: " << (*i)->getSockStream()->availableBytes() << std::endl << vprDEBUG_FLUSH;
                std::cout << "ERROR: RECEIVING Bytes on socket: " << (*i)->getSockStream()->availableBytes() << std::endl;
            }
         }

         
         mConfigMutex.release();  // Don't allow devices to be added or removed when we are accessing them
         
         //BaseDeviceInterface::refreshAllDevices();      // Refresh all the device interface handles
         
      }
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
            bool sync = false;

            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetNORM(clrYELLOW)
               << "Waiting for a handshake responce on port: "<< mListenPort << "\n"<< clrRESET << vprDEBUG_FLUSH;
               
               // If we receive a handshake successfully
            if ( mAcceptMsgPackage.receiveHandshake(streamHostname,streamPort, streamManagerId, client_sock, sync) )
            {  
               if (sync)
               {
                  // We have a sync request.
                  if (mBarrier != NULL && mBarrier->isMaster())
                  {
                     mBarrier->AddBarrierSlave(client_sock);
                     client_sock->setNoDelay(true);
                     client_sock->setTypeOfService(vpr::SocketOptions::LowDelay);
                     client_sock = new vpr::SocketStream;
                  }
                  else
                  {
                     vprASSERT(false && "Tried to conneted to a client machine!");
                  }
               }
               else if (getTransmittingConnectionByManagerId(streamManagerId) == NULL)   // If the transmitting NetConnection does not exist yet
               {
                  NetConnection* connection = new NetConnection(vpr::Interval(0,vpr::Interval::Base),streamHostname, streamPort, streamManagerId, client_sock);
                  this->mTransmittingConnections.push_back(connection);
                  
                  vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
                  << "RemoteInputManager: Added Transmitting connection to " << streamHostname
                  <<" : "<< streamPort <<" : "<< streamManagerId << std::endl
                  << vprDEBUG_FLUSH;
                  
                     // Create handshake responce
                  mAcceptMsgPackage.createHandshake(true,mShortHostname,mListenPort, mManagerId.toString(),false);   // send my name: send my hostname & port
                  mAcceptMsgPackage.sendAndClear(client_sock);
                  
                  client_sock->setNoDelay(true);
                  client_sock->setTypeOfService(vpr::SocketOptions::LowDelay);
                  
                     //Cluster Sync crap
                  connection->clientClusterSync();

                  client_sock = new vpr::SocketStream;
               }
               else  // Transmitting NetConnection already exists
               {
                  vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
                     << "RemoteInputManager: Connection to " << streamHostname
                     <<" : "<< streamPort <<" : "<< streamManagerId << " already exists.\n"
                     << vprDEBUG_FLUSH;
                  mAcceptMsgPackage.createHandshake(false,mShortHostname, mListenPort, mManagerId.toString(),false);   // send my manager's name
                  mAcceptMsgPackage.sendAndClear(client_sock);
                  client_sock->close();
                  delete client_sock;
                  client_sock = new vpr::SocketStream;
               }
            }
            else
            {
               mAcceptMsgPackage.createHandshake(false,mShortHostname, mListenPort, mManagerId.toString(),false);   // send my manager's name
               mAcceptMsgPackage.sendAndClear(client_sock);
               client_sock->close();
               delete client_sock;
               client_sock = new vpr::SocketStream;
            }
            mConfigMutex.release();   // Release a Mutex so we don't add/remove devices/connections while they are in use
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
      //NOTE: BLOCKS WHEN WAITING FOR RETURN HANDSHAKE
   NetConnection* RemoteInputManager::makeConnection(const std::string& connection_hostname, const int connection_port)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_STATE_LVL) << "[RIM::makeConnection] " 
         << connection_hostname <<":"<< connection_port << "\n"<< vprDEBUG_FLUSH;
      
      vpr::SocketStream* sock_stream;
      vpr::InetAddr inet_addr;
      bool sync;

         // Set the address that we want to connect to
      if ( !inet_addr.setAddress(connection_hostname, connection_port).success() )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrRED,"[RIM::makeConnection] ERROR: Failed to set address\n") << vprDEBUG_FLUSH;
         return NULL;
      }
         // Create a new socket stream to this address
      sock_stream = new vpr::SocketStream(vpr::InetAddr::AnyAddr, inet_addr);
      

         // If we can successfully open the socket and connect to the server
      if ( sock_stream->open().success() && sock_stream->connect().success() )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << " Successfully connected to: " 
            << connection_hostname <<":"<< connection_port << "\n"<< vprDEBUG_FLUSH;
         sock_stream->setNoDelay(true);
         sock_stream->setTypeOfService(vpr::SocketOptions::LowDelay);

            // Send a handshake to initalize communication with remote computer
         mMsgPackage.createHandshake(true,mShortHostname,mListenPort,mManagerId.toString(),false);
         mMsgPackage.sendAndClear(sock_stream);


         std::string received_hostname;
         std::string received_manager_id;
         vpr::Uint16 received_port;
         
         vprDEBUG(gadgetDBG_RIM,vprDBG_STATE_LVL) << clrSetNORM(clrYELLOW) 
            << "Waiting for a responce handshake responce on: " << connection_hostname <<":"
            << connection_port << "\n"<< clrRESET << vprDEBUG_FLUSH;
         
            // Wait for the remote machine to respond with a handshake
         if ( mMsgPackage.receiveHandshake(received_hostname, received_port, received_manager_id,sock_stream, sync) )
         { 
               // Create a new NetConnection  (We use the hostname that we know, not that was sent to us)
            NetConnection* connection = new NetConnection(vpr::Interval(0,vpr::Interval::Base),connection_hostname, connection_port, received_manager_id, sock_stream);
            
            connection->getClusterSync();
               // Add NetConnection
            mReceivingConnections.push_back(connection);
            
            vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
            << "SUCCESS - RemoteInputManager: Added connection to " << connection_hostname
            <<" : "<< connection_port <<" : "<< received_manager_id << "\n" << vprDEBUG_FLUSH;
            return connection;
         }
         else
         {
            delete sock_stream;
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetNORM(clrRED) << "ERROR: Did not receive a handshake responce: " 
               << connection_hostname <<" : "<< connection_port << "\n" << clrRESET << vprDEBUG_FLUSH;
            return NULL;
         }
      }
      else
      {
         delete sock_stream;
//            vprDEBUG(gadgetDBG_RIM,vprDBG_STATE_LVL) << clrSetNORM(clrRED) << "ERROR: Could not connect to device server: " 
//               << connection_hostname <<" : "<< connection_port << "\n" << clrRESET << vprDEBUG_FLUSH;
         return NULL;
      }
   }

   void RemoteInputManager::sendNetworkData()
   {
      std::list<NetConnection*>::iterator i;
      for ( i = mTransmittingConnections.begin(); i != mTransmittingConnections.end();i++ )
      {
            vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) <<  "[RIM] Sending data for: " << (*i)->getHostname() << "\n" << vprDEBUG_FLUSH;
            (*i)->sendNetworkData();
      }
   }
   
   void RemoteInputManager::readReceivingConnectionData()
   {
      std::list<NetConnection*>::iterator i;
      for ( i = mReceivingConnections.begin(); i != mReceivingConnections.end();i++ )
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
            mReceivingConnections.erase(i);
            return;
         }
         else
         {
               // While this connection has not received an END_BLOCK
            while ( !(*i)->getAllPacketsReceived() ||
                     (*i)->getSockStream()->availableBytes() > 8)
   
            {  
               if ((*i)->getAllPacketsReceived())
               {
                  vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "Something is wrong!\n" << vprDEBUG_FLUSH;
               }
               if((*i)->receiveNetworkPacket() == vpr::ReturnStatus::NotConnected)
               {
                  mReceivingConnections.erase(i);
               }
            }
            vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "ALL PACKETS RECEIVED for: " << (*i)->getHostname() << "\n" << vprDEBUG_FLUSH;
         }
      }
   }
   
   void RemoteInputManager::readTransmittingConnectionData()
   {
      std::list<NetConnection*>::iterator i;
      for ( i = mTransmittingConnections.begin(); i != mTransmittingConnections.end();i++ )
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
            mReceivingConnections.erase(i);
            return;
         }
         else
         {
               // While this connection has not received an END_BLOCK
            while ( !(*i)->getAllPacketsReceived() || 
                     (*i)->getSockStream()->availableBytes() > 8)
            {
               if ((*i)->getAllPacketsReceived())
               {
                  vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "Something is wrong!\n" << vprDEBUG_FLUSH;
               }
               if((*i)->receiveNetworkPacket() == vpr::ReturnStatus::NotConnected)
               {
                  mTransmittingConnections.erase(i);
               }

            }
            vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "ALL PACKETS RECEIVED for: " << (*i)->getHostname() << "\n" << vprDEBUG_FLUSH;
         }
      }                                                                                                              
   }

   NetConnection* RemoteInputManager::getTransmittingConnectionByHostAndPort(const std::string& hostname, const int port)
   {
      // Method to check strings
      for ( std::list<NetConnection*>::iterator i = mTransmittingConnections.begin();
          i != mTransmittingConnections.end(); i++ )
      {
         
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Checking for match: " << (*i)->getName() << ", " << hostname<<":"<<port << std::endl << vprDEBUG_FLUSH;
         if ( ((*i)->getHostname() == hostname) && ((*i)->getPort() == port) )
         {
            return *i;
         }
      }
      return NULL;
   }
   
   NetConnection* RemoteInputManager::getTransmittingConnectionByManagerId(const vpr::GUID& manager_id)
   {
      for ( std::list<NetConnection*>::iterator i = mTransmittingConnections.begin();
          i != mTransmittingConnections.end(); i++ )
      {
         if ( (*i)->getManagerId() == manager_id )
         {
            return *i;
         }
      }
      return NULL;
   }
   
   NetConnection* RemoteInputManager::getReceivingConnectionByHostAndPort(const std::string& hostname, const int port)
   {
      // Method to check strings
      for ( std::list<NetConnection*>::iterator i = mReceivingConnections.begin();
          i != mReceivingConnections.end(); i++ )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Checking for match: " << (*i)->getName() << ", " << hostname<<":"<<port << std::endl << vprDEBUG_FLUSH;
         if ( ((*i)->getHostname() == hostname) && ((*i)->getPort() == port) )
         {
            return *i;
         }
      }
      return NULL;
   }

   NetConnection* RemoteInputManager::getReceivingConnectionByManagerId(const vpr::GUID& manager_id)
   {
      for ( std::list<NetConnection*>::iterator i = mReceivingConnections.begin();
          i != mReceivingConnections.end(); i++ )
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
      for (std::list<NetConnection*>::iterator i = mReceivingConnections.begin();
           i != mReceivingConnections.end(); i++)
      {
         NetDevice* temp = (*i)->findNetDeviceByLocalId(local_id);
         if (temp != NULL)
         {
            return(temp);
         }
      }
      return NULL;
   }

   void RemoteInputManager::resendRequestsForNackedDevices()
   {
      for ( std::list<NetConnection*>::iterator i = mReceivingConnections.begin();
          i != mReceivingConnections.end(); i++ )
      {
         (*i)->resendRequestsForNackedDevices();
      }
   }
   //END-------------------- Remote Input Manager Connection Management --------------------
}  // end namespace gadget
