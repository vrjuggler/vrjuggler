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
#include <gadget/Type/NetInput.h>
#include <gadget/RemoteInputManager/NetUtils.h>
#include <gadget/Type/NetDigital.h>
#include <gadget/Type/NetPosition.h>
#include <gadget/RemoteInputManager/NetClockSync.h>


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
   bool mListenWasInitialized;
   std::string mInstanceName;
   std::string mShortHostname;
   std::string mLongHostname;
   //std::string mHostnameFromConfig;
   //std::string mHostname;
   //std::string mHostIp;
   int mListenPort;
   vpr::GUID mManagerId;

   gadget::InputManager* mInputManager;  /**< a ptr to the Input Manager so we can access devices */

   // std::list<NetInput*> mLocalInputs;
   // std::list<NetInput*> mRemoteInputs;
   // std::vector<std::string> mConnectionNames;
   // std::list<vpr::SocketStream*> mConnections;
   // std::list<RecvBuffer*> recv_buffers;

   std::list<NetConnection*> mConnections;    /**< network connections to other juggler instances */
   std::list<NetInput*> mTransmittingInputs;  /**< devices/proxies that we transmit from */
   std::list<NetInput*> mReceivingInputs;     /**< devices/proxies that we receive from */
   IdGenerator<VJ_NETID_TYPE> mLocalIdGen;    /**< keeps track of used/free network ids */
   MsgPackage mMsgPackage;                    /**< used to package and send messages */
   NetClockSync mNetClockSync;

   vpr::Thread* mAcceptThread;
   vpr::InetAddr mListenAddr;

   // bool mWaitingForOtherHosts;  // flag checked by InputManager to see if we need to stall configuration to wait for connections to hosts

   enum
   {
      RIM_FALSE, RIM_TRUE, RIM_UNKNOWN
   } mRmtMgrChunkExists;  // needed to deteremine if we get our listening port
   // from an optional Manager chunk or from the Device Host Chunks


   vpr::Mutex mConfigMutex;  // prevents us from try to read devices while they are being modified (added or removed)
   //vpr::Mutex mNetInitMutex; // on windows helps us call WSAStartup only once
   //bool mNetworkInitted; // need for windows WSAStartup

   // bool mMetStartingRequirements;
   // int mMinConnections;   // allows us to wait until network devices are connected and synchronize our start

/*
    bool addLocalNetDevice(NetInput* net_dev)
    {
        mLocalInputs.push_back(net_dev);
        return true;
    }

    bool addRemoteNetDevice(NetInput* net_dev)
    {
        mRemoteInputs.push_back(net_dev);
        connectToRemoteDevice(net_device ); // net_dev->init();
        return true;
    }
*/

public:
   RemoteInputManager(InputManager* input_manager);

   virtual ~RemoteInputManager();

   // static std::string getChunkType() { return std::string("RemoteInput");}
   void findIfMgrChunkExists();
   // returns if there is a chunk (processed yet or not) that will explicitly configure the Remote Input Manager
   bool mgrChunkExists();

   // if we are waiting to connect to other hosts, return true;
   // bool chunksWaitingForOtherHosts(){ return mWaitingForOtherHosts; }
   // void setWaitingForHostsFlagOff(){ mWaitingForOtherHosts = false; }
   // void setWaitingForHostsFlagOn(){ mWaitingForOtherHosts = true; }

   /*
   // returns true if the hostname contains a period
   bool hostnameContainsPeriod(const std::string& hname){
      int first_period_pos = mLongHostname.find(".");
      if (first_period_pos == std::string::npos)
         return true;
      else
         return false;
   } */

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

   // returns a string without the ending (i.e. returns viper from viper.vrac.iastate.edu)
   std::string getShortHostnameFromLong(std::string long_name)
   {
      int first_period_pos = long_name.find(".");
      std::string temp = long_name.substr(0, first_period_pos);
      // just in case it is trailed by a ":port";
      int colon_pos = temp.find(":");
      std::string temp2 = temp.substr(0,colon_pos);
      return temp2;
   }



// HOSTNAME STRING UTILITY FUNCTIONS

   /*
   bool nameContainsColon(const std::string& hname){
      unsigned int first_colon_pos = mLongHostname.find(":");
      if (first_colon_pos == std::string::npos)
         return true;
      else
         return false;
   }

 hostnameMatchesLocalHostname
   // bool locationContainsLocalHostname(const std::string& location){
   // strips the
   bool hostnameMatchesLocalHostname(const std::string& location){
      std::string lowercase_location = location;
      // make lowercase
      std::transform (lowercase_location.begin(),lowercase_location.end(), lowercase_location.begin(), tolower);
      // remove any trailing colon and port number
      std::string test_hostname = "";
      // remove trailing ":port"

      unsigned int i = 0;// for(int i = 0; i < lowercase_location.size(); i++){
      while( (i < lowercase_location.size()) &&  (lowercase_location[i] != ':') ){
         test_hostname += lowercase_location[i];
         i++;
      }

      // check if test hostname is contained in our long hostname
      if(mLongHostname.find(test_hostname) != std::string::npos){
         // now make sure short hostnames match
         if (getShortHostnameFromLong(test_hostname) == mShortHostname)
            return true;
         else
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HEX_LVL)
               << test_hostname << " is not equal to " << mShortHostname
               << std::endl << vprDEBUG_FLUSH;
      }
      else
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HEX_LVL)
            << test_hostname << " is not in " << mLongHostname << std::endl
            << vprDEBUG_FLUSH;

      return false;
   }

   // accepts strings in the form "host:port" and returns port as an integer
   int stripPortFromName(const std::string location){
      unsigned int colon_pos = location.find(":");
      if (colon_pos == std::string::npos)
         return 0; // they don't match, there isn't even a port in "location"
      else{
         int port_pos = colon_pos + 1;
         std::string port_str = location.substr(port_pos, location.size() - port_pos);
         int return_port = atoi(port_str.c_str());
         return return_port;
      }
   }

   bool hostPortMatchesListeningPort(const std::string& location){
      unsigned int colon_pos = location.find(":");
      if (colon_pos == std::string::npos)
         return false; // they don't match, there isn't even a port in "location"
      else{
         int port_pos = colon_pos + 1;
         std::string port_str = location.substr(port_pos, location.size() - port_pos);
         int test_port = atoi(port_str.c_str());
         if(test_port == mListenPort)
            return true;
         else
            return false;
      }
   }
   */

// --- END HOSTNAME STRING UTILITY FUNCTIONS


   // Used by InputManager to pass RemoteInputManager a new NetDevice
   // bool addNetDevice(NetInput* devicePtr);

   // create a new thread to manage network connections
   bool startListening();

   // stop thread that accepts new connections
   // remove all net devices
   void shutdown();

   // send and receive data from network devices
   void updateAll();

   // loops and accepts new connections if they are valid
   virtual void acceptLoop(void* nullParam);

   // bool checkStartingRequirements();

   // void sendDeviceAccept(vpr::SocketStream* client_sock);
   // void sendDeviceReject(vpr::SocketStream* client_sock);
   // bool connectRemoteDevice(Input* device, int connection_index);

/*
   bool addRemoteDevice(NetInput* device){
      mRemoteInputs.push_back(device);
      connectRemoteDevice(device, connection_index);
      return true;
   }
*/
   // void initNetwork();


   bool config(jccl::ConfigChunkPtr chunk);
   bool configFromLocalConnection(jccl::ConfigChunkPtr chunk);
   bool listenWasInitialized()
   {
      return mListenWasInitialized;
   }  // only true after config has been called

   bool configConnection(jccl::ConfigChunkPtr chunk);
   void findMgrChunk(); // used to see if a Manager chunk exists to specify a listening port
   /*
   bool isLocalHostAndPort(const std::string& location){

      std::string lowercase_location = location;
      // make all lower case
      std::transform (lowercase_location.begin(),lowercase_location.end(), lowercase_location.begin(), tolower);
      char port_str[16];
      sprintf("%s", port_str, mListenPort);
      std::string our_location = mLocalHostname + std::string(":") + port_str;
      std::string our_location2 = std::string("localhost") + std::string(":") + port_str;

      if ( (lowercase_location == our_location) ||
         (lowercase_location == our_location2))
         return true;
      else
         return false;
   } */

// INTER-JUGGLER CONNECTION MANAGEMENT
   //  if new stream is valid, returns true and sets deviceName
   // bool handshake(std::string& deviceName, vpr::SocketStream* newStream);
   bool sendHandshake(const std::string& host, const std::string& port, const std::string& manager_id, vpr::SocketStream* newStream);
   bool sendRejectionHandshake(const std::string& host, const std::string& port, const std::string& manager_id, vpr::SocketStream* newStream);
   bool receiveHandshake(std::string& recievedHostname, int& receivedPort, std::string& received_manager_id, vpr::SocketStream* newStream);

   bool makeConnection(const std::string &connection_alias, const std::string &hostname, const int port);
   NetConnection* addConnection(const std::string &connection_alias, const std::string& hostname, const int port, const std::string& manager_id, vpr::SocketStream* sock_stream);

/*
   NetConnection* findConnection(const std::string& connection_name){
      std::list<NetConnection*>::iterator i;
      for(i = mConnections.begin(); i != mConnections.end(); i++){
         if((*i)->getName() == connection_name)
            return *i;
      }
      return NULL;
   }
*/

   // read network for messages to update devices
   void receiveDeviceNetData();
   void sendDeviceNetData();
   // parse and process network messages
   bool processRecvBuff(NetConnection* net_connection);

// ADDING AND REMOVING NET DEVICES
   void acquireConfigMutex()
   {
      mConfigMutex.acquire();
   }
   void releaseConfigMutex()
   {
      mConfigMutex.release();
   }

   NetInput* createTransmittingNetInput(const std::string& device_name, Input* input_ptr, VJ_NETID_TYPE requester_device_id);

   NetInput* createTransmittingNetInput(const std::string& device_name, Proxy* proxy_ptr, VJ_NETID_TYPE requester_device_id);

   bool configureReceivingNetInput(jccl::ConfigChunkPtr chunk, NetConnection* net_connection);
   bool configureTransmittingNetInput(std::string device_name, VJ_NETID_TYPE requester_device_id, NetConnection* net_connection);

   // just add to Input Manager and Remote Input Manager lists
   void addTransmittingNetInput(NetInput* net_input, NetConnection* net_connection);
   void addReceivingNetInput(NetInput* net_input, NetConnection* net_connection);

   bool allDataReceived();

   void markDataUnreceived();

   NetInput* findTransmittingNetInput(const std::string& device_name);
   NetConnection* getConnectionByHostAndPort(const std::string& hostname, const int port); // NetConnection* getConnectionByHostAndPort(const std::string& location_name);
   NetConnection* getConnectionByAliasName(const std::string& alias_name);
   NetConnection* getConnectionByManagerId(const vpr::GUID& manager_id);
   void sendEndBlocks();

   // returns unsigned short by default
   VJ_NETID_TYPE generateLocalId()
   {
      return mLocalIdGen.generateNewId();
   }

   NetInput* findDeviceByLocalId(VJ_NETID_TYPE local_id);

   void resendRequestsForNackedDevices();
   void updateManagerStatus();

/*
   // XXX debug function
   void configureTestProxy(jccl::ConfigChunkPtr chunk){
      NetDigitalProxy* dig_proxy = new NetDigitalProxy(chunk, this->generateLocalId());
      std::string name = chunk->getName();
      mInputManager->addProxy(name, dynamic_cast<Proxy*>(dig_proxy));

   }
*/

};

}  // end namespace gadget

#endif
