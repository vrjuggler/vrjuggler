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

#ifndef REMOTE_INPUT_MANAGER_H
#define REMOTE_INPUT_MANAGER_H

// The Remote Input Manager allows a proxy or device on one machine to point to a device on
// on another machine

#include <gadget/gadgetConfig.h>
//#include <jccl/RTRC/ConfigChunkHandler.h>

#include <vpr/Thread/Thread.h>
#include <vpr/Sync/Mutex.h>
//#include <vpr/IO/Socket/SocketStream.h>

//#include <gadget/RemoteInputManager/NetConnection.h>
#include <gadget/RemoteInputManager/MsgPackage.h>
//#include <gadget/RemoteInputManager/NetUtils.h>
//#include <gadget/RemoteInputManager/NetDevice.h>

//namespace vpr
//{
//   class Thread;
//}

namespace gadget
{
   class Proxy;
   class Input;
   class InputManager;
   class SocketStream;
   class NetConnection;
   class NetDevice;
   class ClusterBarrier;

   // Note the RemoteInputManager does not inherit ConfigChunkHandler like the Input Manager does.
   // That's because it is closely connected to the Input Manager and the chunks it processes must also
   // be processed by the Input Manager.   
   class GADGET_CLASS_API RemoteInputManager
   {
   protected:
      bool                          mListenWasInitialized;
      bool                          mActive;
      bool                          mConfigured;
      IdGenerator<VJ_NETID_TYPE>    mLocalIdGen;            /**< keeps track of used/free network ids */

      gadget::MsgPackage            mMsgPackage;
      gadget::MsgPackage            mAcceptMsgPackage;
      gadget::InputManager*         mInputManager;          /**< a ptr to the Input Manager so we can access devices */

      std::string                   mInstanceName;
      std::string                   mShortHostname;
      std::string                   mLongHostname;
      std::string                   mLocalMachineChunkName;
      std::list<NetConnection*>     mTransmittingConnections;           /**< network connections to other juggler instances */
      std::list<NetConnection*>     mReceivingConnections;           /**< network connections to other juggler instances */
      
      std::vector<jccl::ConfigChunkPtr>                  mPendingDeviceChunks;
      std::map<std::string, jccl::ConfigChunkPtr>        mMachineTable;
      std::map<std::string, jccl::ConfigChunkPtr>        mClusterTable;
      
      std::map<std::string, vpr::ObjectWriter*>          mCachedDeviceData;
      std::map<std::string, gadget::Input*>          mTransmittingDevicePointers;
      
      vpr::Uint16          mListenPort;
      vpr::GUID            mManagerId;
      vpr::Thread*         mAcceptThread;
      vpr::InetAddr        mListenAddr;
      vpr::Mutex           mConfigMutex;  // prevents us from try to read devices while they are being modified (added or removed)

      std::vector<vpr::SocketStream*>       mSyncClients;

      vpr::SocketStream*                  mSyncServer;
      std::string                         mSyncMasterChunkName;
      vpr::SerialPort                     *mSerialPort;
      gadget::ClusterBarrier*             mBarrier;

   public:
      RemoteInputManager(InputManager* input_manager);

      virtual ~RemoteInputManager();
            
      /**
       * Kills the local listening thread and removes all connections.
       */
      void shutdown();
      
      vpr::ObjectWriter* getObjectWriter(std::string device_name, gadget::Input* input_device);


      void debugDump();
      
      //-------------------- Remote Input Manager Configuration --------------------

      bool configAdd(jccl::ConfigChunkPtr chunk);
      bool configRemove(jccl::ConfigChunkPtr chunk);
      bool configCanHandle(jccl::ConfigChunkPtr chunk);
      bool configureRIM();
      bool recognizeClusterMachineConfig(jccl::ConfigChunkPtr chunk);
      bool recognizeRemoteInputManagerConfig(jccl::ConfigChunkPtr chunk);
      bool recognizeRemoteDeviceConfig(jccl::ConfigChunkPtr chunk);
     
     /**
      * Adds machines to a table of cluster machines, and configures the RIM 
      * if the chunk is for the local machine
      * @param	chunk The chunk that contains information about a cluster machine.
      * @post	The cluster machine added to the table. And if the chunk is for
      * 			the local machine, it is now configured and listening.
      */
      bool configureClusterMachine(jccl::ConfigChunkPtr chunk);
      
      /**			 
       * Determines if the device's host_chunk field is pointing to a cluster machine
       * that is in the curent configuration. And create a connection to that machine 
       * and a NetDevice to manage the communication to the new "virtual device"
       * @param	chunk The chunk for the device that we are trying to find.
       * @post		A new NetDevice, BaseType(Virtual Device), and a NetConnection,
       * 			if one did not exist before.
       */
      bool configureDevice(jccl::ConfigChunkPtr chunk);

      static std::string getChunkType() { return std::string( "RIMChunk" ); }
      static std::string getMachineSpecificChunkType() { return std::string( "MachineSpecific" ); }

      //END-------------------- Remote Input Manager Configuration --------------------

      
      //-------------------- Remote Input Manager Syncronization --------------------
      void createBarrier();
      //END-------------------- Remote Input Manager Syncronization --------------------
      
      //-------------------- Remote Input Manager Flags --------------------
      bool isActive()
      {return mActive;}
      
      bool isConfigured()
      {return mConfigured;}

      /**
       * Returns the state of a flag that indicates if listening on the local
       * machine has been started
       */
      bool listenWasInitialized()
      {
         return mListenWasInitialized;
      }
      /* Searches through all of the NetConnection to determine if they have
       * received all of their data
       *
       * @return	TRUE if all connections have received their data.
       *			FALSE if the haven't.
       */
      bool allDataReceived();
      void markDataUnreceived();

      //END-------------------- Remote Input Manager Flags --------------------
      
      
      
      //-------------------- Remote Input Manager Getters/Setters --------------------
      
      NetConnection* findReceivingDevice(const std::string device_name);
      NetConnection* findTransmittingDevice(const std::string device_name);

      /**
       * Get a pointer to a "virtual" device in the RIM
       *
       * @param   device_name Name of the device we are trying to find.
       * @return  A pointer to the base class.
       */
      Input* getDevice(std::string device_name);
      
      /**
       * Returns the local hostname as a string
       */
      std::string getLocalHostName()
      {
         return(mShortHostname);
      }


      //END-------------------- Remote Input Manager Getters/Setters --------------------
      

      //-------------------- Remote Input Manager Mutex --------------------
      
      /**
       * Locks the configureation of RIM.
       */
      void acquireConfigMutex()
      {
         mConfigMutex.acquire();
      }

      /**
       * Unlocks the configureation of RIM.
       */
      void releaseConfigMutex()
      {
         mConfigMutex.release();
      }
      
      //END-------------------- Remote Input Manager Mutex --------------------

      //-------------------- Remote Input Manager Connection Management --------------------
      
      /**
       * Sends and receives data for all NetDevices
       * @post All cluster systems have the same device data.
       */
      void updateAll();

      /**
       * If there is not already a thread for listening and the local listening
       * port is valid, then create a new thread and startListening on it.
       * 
       *	@post	A thread that is running the acceptLoop function, and receiving
       * 		connection requests.
       */
      bool startListening();

      /**
       * Open a socket and accept incoming connections. When a connection is 
       * initiated we try to recieve a handshake. If we do then try to create
       * a new NetConnection. If successful respond by sending a handshake back.
       *
       * @post	A NetConnection to a machine that has requested a connection.
       */
      virtual void acceptLoop(void* nullParam);


      /* Opens a connection to a remote machine's RIM
       * @param hostname	hostname of the remote machine
       * @param	port		the port on the remote machine to try to connect to
       *
       * @post	A NetConnection to the remote machine that we requested.
       * @return	NetConnection* to the new NetConnection.
       *				NULL if it could not be created
       */
      NetConnection* makeConnection(const std::string &hostname, const int port);

      /**
       * Send local data to all connected remote machines.
       */
      void sendNetworkData();
      
      /**
       * Read the network until all connections have received the data 
       * they need
       */
      void readReceivingConnectionData();
      void readTransmittingConnectionData();

      NetConnection* getTransmittingConnectionByHostAndPort(const std::string& hostname, const int port); // NetConnection* getConnectionByHostAndPort(const std::string& location_name);
      NetConnection* getTransmittingConnectionByManagerId(const vpr::GUID& manager_id);

      NetConnection* getReceivingConnectionByHostAndPort(const std::string& hostname, const int port); // NetConnection* getConnectionByHostAndPort(const std::string& location_name);
      NetConnection* getReceivingConnectionByManagerId(const vpr::GUID& manager_id);
      
      NetDevice* findNetDeviceByLocalId(VJ_NETID_TYPE local_id);

      void resendRequestsForNackedDevices();
      
      //END-------------------- Remote Input Manager Connection Management --------------------

      //-------------------- Remote Input Manager Helper Functions --------------------
      
         // returns unsigned short by default
      VJ_NETID_TYPE generateLocalId()
      { return mLocalIdGen.generateNewId(); }

      std::string getShortHostnameFromLong(std::string long_name)
      {
         int first_period_pos = long_name.find(".");
         std::string temp = long_name.substr(0, first_period_pos);
         // just in case it is trailed by a ":port";
         int colon_pos = temp.find(":");
         std::string temp2 = temp.substr(0,colon_pos);
         return temp2;
      }

      bool hostnameAndPortMatchLocal(const std::string& host, const int port)
      {
         if ( port == mListenPort )
         {
            return true;
         }
         else
         {

            std::string lowercase_host = host;
            // make lowercase
            std::transform (lowercase_host.begin(),lowercase_host.end(), lowercase_host.begin(), tolower);

            // check if test hostname is contained in our long hostname and vice versa
            if ( mLongHostname.find(lowercase_host) != std::string::npos )
            {
               // now make sure short hostnames match
               if ( getShortHostnameFromLong(lowercase_host) == mShortHostname )
               {
                  return true;
               }
               else
               {
                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HEX_LVL)
                  << lowercase_host << " is not equal to " << mShortHostname
                  << std::endl << vprDEBUG_FLUSH;
               }
            }
            else
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HEX_LVL)
               << lowercase_host << " is not in " << mLongHostname
               << std::endl << vprDEBUG_FLUSH;
            }

            return false;
         }
      }

      bool hostnameMatchesLocalHostname(const std::string& host)
      {
         std::string lowercase_host = host;
         // make lowercase
         std::transform (lowercase_host.begin(),lowercase_host.end(), lowercase_host.begin(), tolower);

         if (lowercase_host == "localhost")
         {
            return true;
         }// check if test hostname is contained in our long hostname and vice versa
         else if ( mLongHostname.find(lowercase_host) != std::string::npos )
         {
            // now make sure short hostnames match
            if ( lowercase_host == mShortHostname )
            {
               return true;
            }
            else
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HEX_LVL)
               << lowercase_host << " is not equal to " << mShortHostname
               << std::endl << vprDEBUG_FLUSH;
            }
         }
         else
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HEX_LVL)
            << lowercase_host << " is not in " << mLongHostname << std::endl
            << vprDEBUG_FLUSH;
         }

         return false;
      }
      //END-------------------- Remote Input Manager Helper Functions --------------------

   };

}  // end namespace gadget

#endif
