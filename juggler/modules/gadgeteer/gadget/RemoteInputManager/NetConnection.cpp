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
#include <vpr/IO/Socket/SocketStream.h>
#include <jccl/Config/ConfigChunk.h>

#include <gadget/Util/Debug.h>
#include <gadget/RemoteInputManager/NetDevice.h>
#include <gadget/RemoteInputManager/NetConnection.h>
#include <gadget/Type/DeviceFactory.h>
#include <gadget/InputManager.h>
#include <gadget/RemoteInputManager/RemoteInputManager.h>

namespace gadget
{

   NetConnection::NetConnection()
   {
      mSockStream = NULL;
      //mRecvBuffer = new RecvBuffer;
      //mObjectReader = new vpr::ObjectReader(new std::vector<vpr::Uint8>);
      mSendIterations = 0;
      mRecvIterations = 0;
   }

   NetConnection::NetConnection(vpr::Interval sync_delta,
                                const std::string& hostname, const int port,
                                const std::string& manager_id,
                                vpr::SocketStream* sock_stream)
   {
      char port_str[32];
      sprintf(port_str, "%d", port);
      mName = hostname + ":" + port_str; // name contains "host:port"

      mDelta = sync_delta;
      mHostname = hostname;
      mPort = port;
      mManagerId = vpr::GUID(manager_id); // set the id of the other computer's remote manager
      mSockStream = sock_stream;
      //mRecvBuffer = new RecvBuffer;
      //mObjectReader = new vpr::ObjectReader(new std::vector<vpr::Uint8>);
      mSendIterations = 0;
      mRecvIterations = 0;
   }

   NetConnection::~NetConnection()
   {
      mSockStream->close();
      delete mSockStream;
   }

   void NetConnection::sendNetworkData()
   {
      mSendIterations++;
      

       for ( std::list<NetDevice*>::iterator i = mTransmittingDevices.begin();i != mTransmittingDevices.end();i++ )
         {
            if ( (*i)->getWasInitialized() )
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[RIM] Sending data for device: " << (*i)->getSourceName() << "\n" << vprDEBUG_FLUSH;
               (*i)->updateFromLocalSource();
               std::cout << "WRITE: " << (*i)->getSourceName() << std::endl;
               mMsgPackage.createDeviceDataPacket(*i);
               mMsgPackage.sendAndClearDeviceData(mSockStream,(*i));
            }
         }
      
      mMsgPackage.createEndBlock();
      
      mMsgPackage.sendAndClear(mSockStream);
   }

   void NetConnection::addTransmittingNetDevice(NetDevice* net_device)
   {
      if ( std::find(mTransmittingDevices.begin(), mTransmittingDevices.end(), net_device) == mTransmittingDevices.end() )
      {
         mTransmittingDevices.push_back(net_device);
      }
   }

   //++++++++++++++++++++++++

   void NetConnection::addReceivingNetDevice(NetDevice* net_device)
   {
      if ( std::find(mReceivingDevices.begin(), mReceivingDevices.end(), net_device) == mReceivingDevices.end() )
      {
         mReceivingDevices.push_back(net_device);
      }
   }



   NetDevice* NetConnection::findReceivingNetDevice(const std::string& device_name)
   {
      for ( std::list<NetDevice*>::iterator i = mReceivingDevices.begin();
          i!= mReceivingDevices.end(); i++ )
      {
         if ( (*i)->getSourceName() == device_name )
         {
            return *i;
         }
      }
      return NULL;
   }

   NetDevice* NetConnection::findReceivingNetDeviceByLocalId(VJ_NETID_TYPE local_id)
   {
      for ( std::list<NetDevice*>::iterator i = mReceivingDevices.begin();
          i!= mReceivingDevices.end(); i++ )
      {
         if ( (*i)->getLocalId() == local_id )
         {
            return *i;
         }
      }
      return NULL;
   }



   //+++++++++++++++++++++++++

   void NetConnection::sendDeviceRequest(NetDevice* net_device )
   {
      if ( mSockStream == NULL )
      {
         return;
      }
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Creating a device request: " 
            << net_device->getLocalId()<< "-" << net_device->getSourceName() 
            << "\n"<< vprDEBUG_FLUSH;
      mMsgPackage.createDeviceRequest(net_device->getLocalId(), net_device->getSourceName());
      mMsgPackage.sendAndClear(mSockStream);
      mMsgPackage.createEndBlock();
      mMsgPackage.sendAndClear(mSockStream);
   }

   //+++++++++++++++++++++++++




   void NetConnection::resendRequestsForNackedDevices()
   {
      for ( std::list<NetDevice*>::iterator i = mReceivingDevices.begin();
          i!= mReceivingDevices.end(); i++ )
      {
         if ( (*i)->needsToResendRequest() )
         {
            sendDeviceRequest(*i);
            (*i)->clearNeedToResendRequestFlag();
         }
      }
   }

   // adds this name to or list of names for this connection
   void NetConnection::addAliasName(const std::string& alias_name)
   {
      if ( !hasAliasName(alias_name) )
      {
         mAliasList.push_back(alias_name);
      }
   }

   // returns true if test_name is one of this connection's aliases
   bool NetConnection::hasAliasName(const std::string& test_name)
   {
      std::vector<std::string>::iterator i;
      for ( i = mAliasList.begin(); i != mAliasList.end(); i++ )
      {
         if ( test_name == *i )
         {
            return true; // the test_name is in our list
         }
      }

      // there were no matches
      return false;
   }

   // for debugging output
   // put all the alias names into one string so they can be used with vprDEBUG
   std::string NetConnection::printAliasNamesToString()
   {
      std::string out;
      std::vector<std::string>::iterator i;
      for ( i = mAliasList.begin(); i != mAliasList.end(); i++ )
      {
         out += *i + (i != mAliasList.end() ? ", " : "");
      }
      return out;
   }
   void NetConnection::debugDump()
   {
      
      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << "------ " << "Host Name: " << clrSetNORM(clrGREEN)  << mHostname << ":" << mPort 
         << clrRESET << " -----\n" << vprDEBUG_FLUSH;

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Trans./Receive \tLocalID \tRemoteID\n" << vprDEBUG_FLUSH;
      
      for (std::list<NetDevice*>::iterator i = mTransmittingDevices.begin();
           i != mTransmittingDevices.end();i++)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Transmitting:   " << (*i)->getSourceName() 
         << " " << (*i)->getLocalId() << " " << (*i)->getRemoteId() 
         << "\n" << vprDEBUG_FLUSH;
      }
      for (std::list<NetDevice*>::iterator i = mReceivingDevices.begin();
           i != mReceivingDevices.end();i++)
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Receiving:   " << (*i)->getSourceName() 
         << " " << (*i)->getLocalId() << " " << (*i)->getRemoteId() 
         << "\n" << vprDEBUG_FLUSH;
      }
      vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << "----------------------------------\n" << vprDEBUG_FLUSH;
}

}  // end namespace gadget
