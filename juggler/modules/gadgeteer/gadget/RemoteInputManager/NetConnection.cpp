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
#include <vpr/Util/Error.h>

#include <gadget/Util/Debug.h>
#include <gadget/RemoteInputManager/NetDevice.h>
#include <gadget/RemoteInputManager/NetConnection.h>
#include <gadget/Type/DeviceFactory.h>
#include <gadget/InputManager.h>
#include <gadget/RemoteInputManager/RemoteInputManager.h>
#include <gadget/Type/BaseTypeFactory.h>

namespace gadget
{

   NetConnection::NetConnection()
   {
      mSockStream = NULL;
      
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
      mSockStream->setNoDelay(true);
      
      //mObjectReader = new vpr::ObjectReader(new std::vector<vpr::Uint8>);
      mSendIterations = 0;
      mRecvIterations = 0;
   }

   NetConnection::~NetConnection()
   {
      mSockStream->close();
      delete mSockStream;
         // Remove all Transmitting NetDevices
      while ( mTransmittingDevices.size() > 0 )
      {
         delete *(mTransmittingDevices.begin()) ;
         mTransmittingDevices.pop_front();
      }
         // Remove all Receiving NetDevices
      while ( mReceivingDevices.size() > 0 )
      {
         delete *(mReceivingDevices.begin()) ;
         mReceivingDevices.pop_front();
      }

   }

   void NetConnection::sendNetworkData()
   {
      mSendIterations++;
      
      for ( std::list<NetDevice*>::iterator i = mTransmittingDevices.begin();i != mTransmittingDevices.end();i++ )
      {
         if ( (*i)->getWasInitialized() )
         {
             vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "[RIM] Sending data for device: " << (*i)->getSourceName() << "\n" << vprDEBUG_FLUSH;
             // (*i)->updateFromLocalSource();
             
             mMsgPackage.createDeviceDataPacket(*i);
             mMsgPackage.sendAndClearDeviceData(mSockStream,(*i));
         }
      }
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

   NetConnection* NetConnection::findNetDeviceConnection(const std::string& device_name)
    {
       for ( std::list<NetDevice*>::iterator i = mReceivingDevices.begin();
           i!= mReceivingDevices.end(); i++ )
       {
          if ( (*i)->getSourceName() == device_name )
          {
             return this;
          }
       }
       for ( std::list<NetDevice*>::iterator i = mTransmittingDevices.begin();
           i!= mTransmittingDevices.end(); i++ )
       {
          if ( (*i)->getSourceName() == device_name )
          {
             return this;
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
   NetDevice* NetConnection::findNetDeviceByLocalId(VJ_NETID_TYPE local_id)
   {
      std::list<NetDevice*>::iterator i;
   
      // check Receiving Devices
      for ( i = mReceivingDevices.begin(); i != mReceivingDevices.end(); i++ )
      {
         if ( (*i)->getLocalId() == local_id )
         {
            return(*i);
         }
      }
   
            // check Transmitting Devices
      for ( i = mTransmittingDevices.begin(); i != mTransmittingDevices.end(); i++ )
      {
         if ( (*i)->getLocalId() == local_id )
         {
            return(*i);
         }
      }
   
      return NULL;
   }

   
   Input* NetConnection::getDevice(const std::string device_name)
   {  
      for ( std::list<NetDevice*>::iterator i = mReceivingDevices.begin();
          i!= mReceivingDevices.end(); i++ )
      {
         if ( (*i)->getSourceName() == device_name )
         {
            return((*i)->getRealDevice());
         }
      }
      return NULL;
   }




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

   bool NetConnection::configureReceivingNetDevice(jccl::ConfigChunkPtr chunk)
   {
         // Get the name of the device that we are trying to configure
      std::string device_name = chunk->getName();
      vprDEBUG_TSColorGuard(clrGREEN);
      vprDEBUG_OutputGuard(gadgetDBG_RIM, vprDBG_CONFIG_LVL,
         std::string("[NetConnection::ConfigureReceivingNetDevice] " + device_name + "\n"),
         std::string("[END NetConnection::ConfigureReceivingNetDevice] " + device_name + "\n"));
   
         //Check if we have this NetDevice Already
      NetDevice* net_device = findReceivingNetDevice(device_name);
   
   
      if ( net_device != NULL )  // We are already receiving data for this device
      {  
         vprDEBUG(gadgetDBG_RIM,vprDBG_STATE_LVL) << clrSetBOLD(clrRED) << "We already have an instance of: " 
               << device_name << "\n" << clrRESET << vprDEBUG_FLUSH;
         return false;
      }
      else  // setup new structures to receive and manage data from the remote device
      {  
         vprDEBUG(gadgetDBG_RIM,vprDBG_STATE_LVL) << "Create a new NetDevice: " << device_name << "\n"<< vprDEBUG_FLUSH;
   
         NetDevice* recv_net_device = new NetDevice(device_name, this->generateLocalId());  // Create a new receiving NetDevice
   
         if ( recv_net_device != NULL ) // NetDevice was created successfully
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_STATE_LVL) << "Successful, now adding to receiveing NetDevices\n"<< vprDEBUG_FLUSH;
            //addReceivingNetDevice(recv_net_device);                     // Add NetDevice to mReceivingNetDevices
            addReceivingNetDevice(recv_net_device);     // Add NetDevice to NetConnection
            sendDeviceRequest(recv_net_device);         // Send a request to the device server
            return true;
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_STATE_LVL) << clrOutNORM(clrRED,"Failed trying to create a NetDevice: ") 
               << device_name << "\n"<< vprDEBUG_FLUSH;
            return false;
         }
      }
   }

   bool NetConnection::configureTransmittingNetDevice(std::string device_name, VJ_NETID_TYPE requester_device_id)
   {
      vprDEBUG_TSColorGuard(clrGREEN);
      vprDEBUG_OutputGuard(gadgetDBG_RIM, vprDBG_CONFIG_LVL,
         std::string("[RIM::ConfigureTransmittingNetDevice] " + device_name + "\n"),
         std::string("[END RIM::ConfigureTransmittingNetDevice] " + device_name + "\n"));

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Looking in our local Input Manager for: " << device_name << "\n" << vprDEBUG_FLUSH;
      
      Input* input_ptr = gadget::InputManager::instance()->getDevice(device_name);
      NetDevice* net_device;
      if ( input_ptr != NULL )   // Device exists locally
      {
         vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL)
            << "InputManager found: " << device_name << "\n" << vprDEBUG_FLUSH;

         net_device = new NetDevice(device_name, input_ptr, this->generateLocalId(), requester_device_id);
         net_device->setObjectWriter(gadget::InputManager::instance()->getRemoteInputManager()->getObjectWriter(device_name,input_ptr));
         
         if (net_device != NULL)    // Successfully created transmitting NetDevice
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Successful, now adding to receiveing NetDevices\n"<< vprDEBUG_FLUSH;
            //addTransmittingNetDevice(net_device, net_connection);
            addTransmittingNetDevice(net_device);
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED) << "ERROR, could not create NetDevice: " << device_name << "\n" << clrRESET << vprDEBUG_FLUSH;
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
         mMsgPackage.sendAndClear(mSockStream);
         
            // Send an END_BLOCK since we are done sending data on this connection
         //mMsgPackage.createEndBlock();
         //mMsgPackage.sendAndClear(net_connection->getSockStream());
            
            // Set a flag saying that we have in dead set up this NetDevice correctly
         net_device->setWasInitializedTrue();
         vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL) << "Transmitting NetDevice, " << device_name 
               << " has been created successfully.\n" << vprDEBUG_FLUSH;
         return true;
      }
      else
      {
            // Send NAcknowledgement that we encountered an error
         mMsgPackage.createDeviceNack(net_device->getRemoteId(), 0, device_name);
         mMsgPackage.sendAndClear(mSockStream);

            // Send an END_BLOCK since we are done sending data on this connection
         //mMsgPackage.createEndBlock();
         //mMsgPackage.sendAndClear(net_connection->getSockStream());

         delete net_device;
         vprDEBUG(gadgetDBG_RIM, vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED) << "ERROR: Transmitting NetDevice, " << device_name 
               << " could not be created!\n" << clrRESET << vprDEBUG_FLUSH;
         return false;
      }
   }


   void NetConnection::receiveNetworkPacket()
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
   
      vprDEBUG_TSColorGuard(clrGREEN);
      vprDEBUG_OutputGuard(gadgetDBG_RIM, vprDBG_VERB_LVL,
         std::string("[RIM::receiveNetworkPacket] Read Network Packet\n"),
         std::string("[RIM::receiveNetworkPacket] END Read Network Packet\n"));
   
      vpr::Uint32 bytes_read;
      NetDevice* tmp_net_device;
   
         // Get the socketStream and MsgPackage for this NetConnection
      
      // DELETE
      //vpr::SocketStream* socket_stream = this->net_connection->getSockStream();
      //socket_stream == mSockStream
      // DELETE

      // DELETE
      // gadget::MsgPackage* msg_package = net_connection->getMsgPackage();
      // msg_package == mMsgPackage
      // DELETE
   
      ///////////////////////////////
      // READ IN THE PACKET HEADER //
      ///////////////////////////////

         // Create a new vector can read the data into it
      std::vector<vpr::Uint8> packet_head(100);
         
         //Read in the known length of the packet header
      vpr::ReturnStatus status = mSockStream->recvn(packet_head,RIM_HEAD_LENGTH,bytes_read);
         
         // If the read was not successful 
      if (!status.success())
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrSetBOLD(clrRED) << "[RIM::receiveNetworkPacket] ERROR: Connection lost" << vprDEBUG_FLUSH;
            // Print the low level error
         vpr::Error::outputCurrentError(std::cout, "BlockIO readn ERROR: ");
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
         this->setAllPacketsReceived();
         vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"============ERROR, NOT A RIM PACKET=============\n") << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "BYTES:    " << (int)bytes_read << "\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "RIM CODE: " << (int)rimcode << "\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "OPCODE:   " << (int)opcode << "\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "LENGTH:   " << (int)length << "\n" << vprDEBUG_FLUSH;
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"================================================\n") << vprDEBUG_FLUSH;
   
         std::vector<vpr::Uint8> packet_data(100);
         status = mSockStream->recvn(packet_data,            // Empty the buffer and try to get valid data next time around
                                      mSockStream->availableBytes(),
                                      bytes_read);
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "PACKET: ";  // Try to display to contents of the buffer
         for (unsigned i=0;i<packet_data.size();i++)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << (int)packet_data[i] << " ";
         }
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
         status = mSockStream->recvn(&device_id,2,bytes_read);
         device_id = vpr::System::Ntohs(device_id);
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrSetNORM(clrCYAN) 
            << "==============DEVICE ID: " 
            << device_id << "==================\n" << clrRESET << vprDEBUG_FLUSH;
         
            // Must also read 2 fewer bytes since we just read two
         status = mSockStream->recvn(packet_data,length-RIM_HEAD_LENGTH-2,bytes_read);
      }
      else
      {
         status = mSockStream->recvn(packet_data,length-RIM_HEAD_LENGTH,bytes_read);
      }
   
   
         // Create a new ObjectReader to parse our incoming data
      vpr::ObjectReader* data_reader = new vpr::ObjectReader(&packet_data);
   
      switch ( opcode )
      {
      case MSG_DEVICE_REQ:    // Remote device is requesting a device from the local machine
         {
            vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] MSG_DEVICE_REQ\n") << vprDEBUG_FLUSH;
   
               // Have mMsgPackage receive the device request
            mMsgPackage.receiveDeviceRequest(data_reader);
   
               // Try to configure the NetDevice, it will send a responce
            configureTransmittingNetDevice(mMsgPackage.getDeviceName(), mMsgPackage.getSenderId());
   
            vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] END MSG_DEVICE_REQ\n") << vprDEBUG_FLUSH;
            break;
         }
      case MSG_DEVICE_ACK:
         {      
            vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] MSG_DEVICE_ACK\n") << vprDEBUG_FLUSH;
   
               // Have mMsgPackage receive the device acknoledgement
            mMsgPackage.receiveDeviceAck(data_reader);
   
               // Try to find the NetDevice for the device we requested
            tmp_net_device = findNetDeviceByLocalId(mMsgPackage.getReceiverId());
   
            if ( tmp_net_device != NULL )
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Found the local device that made the request\n" << vprDEBUG_FLUSH;
   
                  // Set the device servers device ID for this device
               tmp_net_device->setRemoteId(mMsgPackage.getSenderId());
   
                  // Try to create a "virtual device"
               Input* new_device = BaseTypeFactory::instance()->loadNetDevice(mMsgPackage.getBaseType());
   
                  // ASSERT if we can not create a "virtual device" because we have failed
               vprASSERT(new_device != NULL && "BaseTypeFactor failed to find base type");
   
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Made a new Virtual device of BaseType: " 
                        << mMsgPackage.getBaseType() <<"\n" << vprDEBUG_FLUSH;
   
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
   
            mMsgPackage.receiveDeviceAck(data_reader);
            tmp_net_device = findNetDeviceByLocalId(mMsgPackage.getReceiverId());
            if ( tmp_net_device != NULL )
            {
               tmp_net_device->setRemoteId(mMsgPackage.getSenderId());
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
            NetDevice* net_device_recvr = this->findReceivingNetDeviceByLocalId(device_id);    
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
   
               //std::cout << "READ: " << net_device_recvr->getSourceName() << std::endl;
               mMsgPackage.recieveDeviceDataPacket(data_reader,
                                                    net_device_recvr->getRealDevice(),
                                                    this->getDelta());
            }
         }
         vprDEBUG_END(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] END DEVICE_DATA\n") << vprDEBUG_FLUSH;         
         break;
      case MSG_CLUSTER_SYNC:  // Other machine is asking for a sync communication 
         //mClusterSync.clientClusterSync(mSockStream);
         break;
      case MSG_END_BLOCK:
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutNORM(clrGREEN,"[RIM Packet] END_BLOCK\n") << vprDEBUG_FLUSH;
         this->setAllPacketsReceived();
         break;
      default:
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR: UNKNOWN OPCODE\n") << vprDEBUG_FLUSH;
         break;
      }
         // Clear the data vector since we will not need this data again
      data_reader->mData->clear();
   }


}  // end namespace gadget
