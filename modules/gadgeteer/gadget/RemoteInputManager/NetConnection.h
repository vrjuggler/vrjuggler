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
#include <gadget/RemoteInputManager/NetDevice.h>
//#include <vpr/IO/ObjectWriter.h>
#include <vpr/IO/ObjectReader.h>
#include <jccl/RTRC/ConfigChunkHandler.h>
#include <gadget/RemoteInputManager/ClusterSync.h>

namespace gadget
{
   class GADGET_CLASS_API NetConnection
   {
   protected:
      vpr::Interval mDelta;
      std::vector<std::string> mAliasList;   /**< aliases for the NetConnection */
      std::string mName;                     /**< NetConnection name */
      std::string mHostname;                 /**< Host that it is connected to */
      int mPort;                             /**< host:port */

      ClusterSync                   mClusterSync;

      vpr::SocketStream* mSockStream;
      //vpr::ObjectReader* mObjectReader;


      std::list<NetDevice*> mReceivingDevices;     /**< devices on remote machines*/
      std::list<NetDevice*> mTransmittingDevices;  /**< devices on local machine sending data*/
      MsgPackage mMsgPackage;                      /**< packet building/parsing class*/
      bool mAllPacketsReceived;                    /**< flag for when all packets are recieved*/
      vpr::GUID mManagerId;                        /**< remote RIM manager ID */

   public:
      int mSendIterations;  // for debugging
      int mRecvIterations;  // for debugging

   public:
      NetConnection();
      NetConnection(vpr::Interval sync_delta, const std::string& hostname, const int port, const std::string& manager_id, vpr::SocketStream* sock_stream);
      ~NetConnection();

      void debugDump();

      bool validateConnection();

      std::string getName()
      { return mName; }

      std::string getHostname()
      { return mHostname; }

      int getPort()
      { return mPort;}

      const vpr::GUID& getManagerId()
      { return mManagerId; }

      std::string getAliasName(int i)
      { return mAliasList[i]; }

      unsigned int getNumAliases()
      { return mAliasList.size(); }

      gadget::MsgPackage* getMsgPackage()
      {
         return(&mMsgPackage);
      }
      
      void getClusterSync()
      {
         mDelta = mClusterSync.getClusterSync(mSockStream);
         std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << mDelta.getBaseVal() << std::endl;
      }
      
      vpr::Uint64* getDelta()
      {
         vpr::Uint64 temp = mDelta.getBaseVal();
         vpr::Uint64* tempPtr = &temp;
         return(tempPtr);
      }


      void clientClusterSync()
      {
         mClusterSync.clientClusterSync(mSockStream);
      }

      //-----------Alias Stuff---------------------
      // for debugging output
      std::string printAliasNamesToString();

      // adds this name to or list of names for this connection
      void addAliasName(const std::string& alias_name);

      // returns true if test_name is one of this connection's aliases
      bool hasAliasName(const std::string& test_name);
      //-----------Alias Stuff---------------------
      
      vpr::SocketStream* getSockStream()
      {
         return mSockStream;
      }

      void sendNetworkData();

      void addTransmittingNetDevice(NetDevice* net_device);

      void addReceivingNetDevice(NetDevice* net_device);
      NetDevice* findReceivingNetDevice(const std::string& device_name);
      NetDevice* findReceivingNetDeviceByLocalId(VJ_NETID_TYPE local_id);

      void sendDeviceRequest(NetDevice* net_device );
      void resendRequestsForNackedDevices();
      
      bool getAllPacketsReceived()
      { return(mAllPacketsReceived); }
      void setAllPacketsUnreceived()
      { mAllPacketsReceived = false; }
      void setAllPacketsReceived()
      { mAllPacketsReceived = true; }
      
   };

} // end namespace gadget

#endif
