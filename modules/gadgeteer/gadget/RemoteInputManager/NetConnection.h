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

#ifndef GADGET_NET_CONNECTION_H
#define GADGET_NET_CONNECTION_H

#include <gadget/gadgetConfig.h>
#include <gadget/RemoteInputManager/MsgPackage.h>
#include <gadget/RemoteInputManager/NetUtils.h>
#include <jccl/RTRC/ConfigChunkHandler.h>

//namespace vpr{
//   class SocketStream;
//}


namespace gadget
{

class NetInput;
class RecvBuffer;

class GADGET_CLASS_API NetConnection
{
protected:
   std::vector<std::string> mAliasList;  /**< readable names */
   std::string mName;   /**< host:port */
   std::string mHostname;
   int mPort;

   vpr::SocketStream* mSockStream;
   RecvBuffer* mRecvBuffer;
   // The following represent devices that are being updated over this connection
   // They each contain ptr to an Input, and the number of remote objects that are using it
   std::list<NetInput*> mReceivingInputs;     // these are allocated and destroyed here
   std::list<NetInput*> mTransmittingInputs;  // these are not allocated here, but are just copies
   // of the pointers in the Remote Input Manager
   MsgPackage mMsgPackage;                      // used to package and send messages.
   IdGenerator<VJ_NETID_TYPE> mRemoteIdGen;       // keeps track of used/free network ids
   bool mAllDataReceived;
   int mSendIterations;  // for debugging
   int mRecvIterations;  // for debugging
   SendBuffer mSendBuffer;
   vpr::GUID mManagerId;  // id of the remote input manager connecting on this connection.

   // Test of proxies instead of  inputs
   // std::list<vjNetProxy*> mReceivingProxies;
   // std::list<vjNetProxy*> mTransmittingProxies;

public:
   NetConnection();
   NetConnection(const std::string& alias_name, const std::string& hostname, const int port, const std::string& manager_id, vpr::SocketStream* sock_stream);
   ~NetConnection();

   // usually contains the host:port.  Mostly for helpful identification.
   std::string getName()
   {
      return mName;
   }

   std::string getHostname()
   {
      return mHostname;
   }
   int getPort()
   {
      return mPort;
   }
   const vpr::GUID& getManagerId()
   {
      return mManagerId;
   }

   // Other names for this connection
   std::string getAliasName(int i)
   {
      return mAliasList[i];
   }
   unsigned int getNumAliases()
   {
      return mAliasList.size();
   }

   // for debugging output
   std::string printAliasNamesToString();

   // adds this name to or list of names for this connection
   void addAliasName(const std::string& alias_name);

   // returns true if test_name is one of this connection's aliases
   bool hasAliasName(const std::string& test_name);

   vpr::SocketStream* getSockStream()
   {
      return mSockStream;
   }
   RecvBuffer* getRecvBuffer()
   {
      return mRecvBuffer;
   }

   void receiveNetworkData();

   void sendNetworkData();

   void addTransmittingNetInput(NetInput* net_input);

   RecvBuffer* getRecvBuff()
   {
      return mRecvBuffer;
   }

   //void removeTransmittingNetInput(NetInput* net_input){
   //    mTransmittingDevices.remove(net_input);
   //}

   void addReceivingNetInput(NetInput* net_input);

   NetInput* createReceivingNetInput(jccl::ConfigChunkPtr chunk);

/*
    NetProxy* createReceivingNetProxy(jccl::ConfigChunk* chunk)
    {
      NetProxy* net_proxy;
      std::string chunk_type = (std::string)chunk->getType();
      if(chunk_type == "DigProxy"){
         NetDigitalProxy* net_digital = new NetDigitalProxy(chunk, this->generateLocalId());
         net_proxy = dynamic_cast<NetProxy*> (net_digital);
      }
      // else if(chunk_type == "PosProxy"){
      //   NetPosition* net_pos = new NetPosition(chunk, this->generateLocalId());
      //   net_input = dynamic_cast<NetInput*> (net_pos);
      //}
      else{
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "createReceivingNetInput: chunk_type: " << chunk_type
            << " unrecognized." << std::endl << vprDEBUG_FLUSH;
         net_proxy = NULL;
      }

      if(net_proxy != NULL){
         mReceivingProxies.push_back(net_proxy);
         net_proxy->addDependency();
      }

      return net_proxy;
   }
*/

   NetInput* findReceivingNetInput(const std::string& device_name);
   NetInput* findReceivingNetInputByLocalId(VJ_NETID_TYPE local_id);

   void sendDeviceRequest(NetInput* net_input );
   void resendRequestsForNackedDevices();
   void sendEndBlock();
   bool allDataReceived();
   void setDataUnreceived();
   void setDataReceived();
   void sendMsg(const MsgPackage& msg_package);

   // returns unsigned short by default
   VJ_NETID_TYPE generateLocalId()
   {
      return mRemoteIdGen.generateNewId();
   }

};

} // end namespace gadget

#endif
