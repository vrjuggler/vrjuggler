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
#include <jccl/RTRC/ConfigChunkHandler.h>

#include <vpr/Thread/Thread.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <gadget/RemoteInputManager/NetConnection.h>
#include <gadget/RemoteInputManager/MsgPackage.h>
//#include <gadget/Type/NetInput.h>
#include <gadget/RemoteInputManager/NetUtils.h>
//#include <gadget/Type/NetDigital.h>
//#include <gadget/Type/NetPosition.h>
#include <gadget/RemoteInputManager/NetDevice.h>
#include <gadget/Type/BaseTypeFactory.h>
#include <gadget/RemoteInputManager/ClusterSync.h>

namespace gadget{

   class Proxy;
   class Input;
   class InputManager;

   // Note the RemoteInputManager does not inherit vjConfigChunkHandler like the Input Manager does.
   // That's because it is closely connected to the Input Manager and the chunks it processes must also
   // be processed by the Input Manager.

   class GADGET_CLASS_API RemoteInputManager
   {
   protected:
      unsigned                      mIncomingConnections;
      int                           mNumMachines;
      ClusterSync                   mClusterSync;
      bool                          mListenWasInitialized;
      bool                          mConfigureDone;
      bool                          mActive;
      IdGenerator<VJ_NETID_TYPE>    mLocalIdGen;            /**< keeps track of used/free network ids */

      gadget::MsgPackage            mMsgPackage;
      gadget::MsgPackage            mAcceptMsgPackage;
      gadget::InputManager*         mInputManager;          /**< a ptr to the Input Manager so we can access devices */

      std::string                   mInstanceName;
      std::string                   mShortHostname;
      std::string                   mLongHostname;
      std::string                   mLocalMachineChunkName;
      std::list<NetConnection*>     mConnections;           /**< network connections to other juggler instances */
      std::list<NetDevice*>         mTransmittingDevices;   /**< devices/proxies that we transmit from */
      std::list<NetDevice*>         mReceivingDevices;      /**< devices/proxies that we receive from */

      std::vector<jccl::ConfigChunkPtr>                  mDeviceChunks;
      std::map<std::string, jccl::ConfigChunkPtr>        mMachineTable;    /**< list of proxies in the system */

      vpr::Uint16          mListenPort;
      vpr::GUID            mManagerId;
      vpr::Thread*         mAcceptThread;
      vpr::InetAddr        mListenAddr;
      vpr::Mutex           mConfigMutex;  // prevents us from try to read devices while they are being modified (added or removed)

      //enum
      //{
      //   RIM_FALSE, RIM_TRUE, RIM_UNKNOWN
      //} mRmtMgrChunkExists;  // needed to deteremine if we get our listening port
      // from an optional Manager chunk or from the Device Host Chunks

      // bool mWaitingForOtherHosts;  // flag checked by InputManager to see if we need to stall configuration to wait for connections to hosts
      //vpr::Mutex mNetInitMutex; // on windows helps us call WSAStartup only once
      //bool mNetworkInitted; // need for windows WSAStartup

      // bool mMetStartingRequirements;
      // int mMinConnections;   // allows us to wait until network devices are connected and synchronize our start

   public:
      RemoteInputManager(InputManager* input_manager);

      virtual ~RemoteInputManager();

      void debugDump();
      bool startCluster(std::vector<jccl::ConfigChunkPtr>);
      bool configAdd(jccl::ConfigChunkPtr chunk);
      bool configRemove(jccl::ConfigChunkPtr chunk);
      bool configCanHandle(jccl::ConfigChunkPtr chunk);
      bool configureRIM();
      bool recognizeClusterMachineConfig(jccl::ConfigChunkPtr chunk);
      bool recognizeRemoteDeviceConfig(jccl::ConfigChunkPtr chunk);
      bool recognizeClusterSystemConfig(jccl::ConfigChunkPtr chunk);
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

      /**
       * Kills the local listening thread and removes all connections.
       */
      void shutdown();
      
      bool isActive()
      {return mActive;}


      /**
       * Sends and receives data for all NetDevices
       * @post All cluster systems have the same device data.
       */
      void updateAll();

      /**
      * Adds machines to a table of cluster machines, and configures the RIM 
      * if the chunk is for the local machine
      * @param	chunk The chunk that contains information about a cluster machine.
      * @post	The cluster machine added to the table. And if the chunk is for
      * 			the local machine, it is now configured and listening.
      */
      bool configureClusterMachine(jccl::ConfigChunkPtr chunk);
      bool configureClusterSystem(jccl::ConfigChunkPtr chunk);

      //SPELLING
      /**			 
       * Determaines if the device's host_chunk field is pointing to a cluster machine
       * that is in the curent configuration. And create a connection to that machine 
       * and a NetDevice to manage the communication to the new "virtual device"
       * @param	chunk The chunk for the device that we are trying to find.
       * @post		A new NetDevice, BaseType(Virtual Device), and a NetConnection,
       * 			if one did not exist before.
       */
      bool configureDevice(jccl::ConfigChunkPtr chunk);

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

      /**
       * Returns the state of a flag that indicates if listening on the local
       * machine has been started
       */
      bool listenWasInitialized()
      {
         return mListenWasInitialized;
      }


      //------------------- INTER-JUGGLER CONNECTION MANAGEMENT ---------------

      /*	Add connection to the list
       * @param	sync_delta 	calculated difference between the local and remote clock
       * @param	hostname		remote machine's hostname
       * @param	port			port that the machine is connected on
       * @param	manager_id	remote machines manager id
       * @param sock_stream	stream that is used to transfer data to/from remote machine 
       *
       * @post	A new NetConnection to the machine that we specified.
       * @return	NetConnection* to the new NetConnection.
       *				NULL if it could not be created
       */
      NetConnection* addConnection(vpr::Interval sync_delta, const std::string& hostname, const int port, const std::string& manager_id, vpr::SocketStream* sock_stream);

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
       * Configures a new NetDevice for a device on a remote machine.
       *
       * @param   chunk    configuration chunk for the device that 
       *                   we are trying to access
       * @param   connection  NetConnection that the NetDevice is using to 
       *                      connect to the remote device
       * @return  TRUE  - if the configuration was successful
       *          FALSE - if the configuration was un-successful
       */
      bool configureReceivingNetDevice(jccl::ConfigChunkPtr chunk, NetConnection* connection);  // requests a connection to the device
      
      /**
       * Adds a NetDevice to the list of NetDevices in the RIM and the 
       * NetConnection that it uses.
       *
       * @param   net_device     NetDevice we are adding
       * @param   net_connection NetConnection that it is using
       */
      void addReceivingNetDevice(NetDevice* net_device, NetConnection* net_connection);

      /**
       * Configures a new NetDevice for sending data from a local device.
       *
       * @param   device_name       name of the local device
       * @param   requester_device_id  the unique ID for this device on the
       *                               remote machine
       * @param   net_connection    NetConnection that the NetDevice is using
       */
      bool configureTransmittingNetDevice(std::string device_name, VJ_NETID_TYPE requester_device_id, NetConnection* net_connection);

      NetDevice* createTransmittingNetDevice(const std::string& device_name, Input* input_ptr, VJ_NETID_TYPE requester_device_id);
      void addTransmittingNetDevice(NetDevice* net_device, NetConnection* net_connection);

      /**
       * Send local data to all connected remote machines.
       */
      void sendDeviceNetData();
      
      /**
       * Read the network until all connections have received the data 
       * they need
       */
      void receiveDeviceNetData();
      
      /**
       * Parse incoming packets.
       *
       * @param net_connection   NetConnection that it is getting the packets on.
       */
      void receiveNetworkPacket(NetConnection* net_connection);

      /* Searches through all of the NetConnection to determine if they have
       * received all of their data
       *
       * @return	TRUE if all connections have received their data.
       *				FALSE if the haven't.
       */
      bool allDataReceived();
      void markDataUnreceived();

      NetDevice* findTransmittingNetDevice(const std::string& device_name);
      NetConnection* getConnectionByHostAndPort(const std::string& hostname, const int port); // NetConnection* getConnectionByHostAndPort(const std::string& location_name);
      NetConnection* getConnectionByAliasName(const std::string& alias_name);
      NetConnection* getConnectionByManagerId(const vpr::GUID& manager_id);


      // returns unsigned short by default
      VJ_NETID_TYPE generateLocalId()
      {
         return mLocalIdGen.generateNewId();
      }

      NetDevice* findDeviceByLocalId(VJ_NETID_TYPE local_id);

      void resendRequestsForNackedDevices();
      
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
            << lowercase_host << " is not in " << mLongHostname << std::endl
            << vprDEBUG_FLUSH;
         }

         return false;
      }

   };

}  // end namespace gadget

#endif
