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

#ifndef GADGET_MSG_PACKAGE_H
#define GADGET_MSG_PACKAGE_H

#include <gadget/gadgetConfig.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>
#include <gadget/RemoteInputManager/NetDevice.h>

//namespace vpr
//{
//   class SocketStream;
//   class ObjectWriter;
//   class ObjectReader;
//}
  
namespace gadget
{

  /** 
   * MsgPackage
   * 
   * Handles the packaging and unpackaging of network packets for the Remote Input Manager.
   */
   class GADGET_CLASS_API MsgPackage
   {
      vpr::ObjectWriter* mObjectWriter;
      vpr::ObjectWriter* mTempWriter;

      // Mostly used when receiving data
      vpr::Uint16 mOpcode;
      vpr::Uint16 mSenderId;
      vpr::Uint16 mReceiverId;
      std::string mDeviceName;
      std::string mBaseType;

   public:
      MsgPackage();
     /**
      * Send contents of current ObjectWriter to the specified socket and 
      * clear the ObjectWriter.
      * 
      * @pre   The ObjectWriter contains the packet(s) you want to send.
      * @post  An empty ObjectWriter and the packets sent over the network.
      * 
      * @param sock_stream The network socket that you want to send the 
      *                    data over.
      */
      void sendAndClear(vpr::SocketStream* sock_stream);
     

      bool isEmpty();

     /**
      * Send contents of current ObjectWriter to the specified socket and 
      * clear the ObjectWriter.
      * 
      * @pre   The ObjectWriter contains the packet(s) you want to send.
      * @post  An empty ObjectWriter and the packets sent over the network.
      * 
      * @param sock_stream The network socket that you want to send the 
      *                    data over.
      */
      void sendAndClearDeviceData(vpr::SocketStream* sock_stream, NetDevice* net_device);

      /**
       * Returns the sender's unique ID
       */
      vpr::Uint16 getSenderId()
      {
         return mSenderId;
      }
      
     /**
      * Returns the receiver's unique ID
      */
      vpr::Uint16 getReceiverId()
      {
         return mReceiverId;
      }

      /**
       * Returns a string mDataString which is used to store the device name.
       */
      const std::string& getDeviceName()
      {
         return mDeviceName;
      }
      
     /**
      * Returns the BaseType of the remote device.
      */
      const std::string& getBaseType()
      {
         return mBaseType;
      }

     /**
      * Creates a handshake packet to be sent to the remote host.
      *
      * @param send_reject Sends either a accept handshake or a rejection handshake.
      * @param host        local hostname
      * @param port        local port 
      * @param manager_id  local manager ID
      *
      */
      bool  createHandshake(bool send_reject, const std::string& host, const vpr::Uint16& port, std::string manager_id, bool sync);

     /**
      * Receives that data from a hanshake packet.
      *
      * @param receivedHostname     remote hostname
      * @param receivedPort         remote port
      * @param received_manager_id  remote manager ID 
      * @param newStream            socket stream to listen on
      * 
      */
      bool  receiveHandshake(std::string& receivedHostname, vpr::Uint16& receivedPort,std::string& received_manager_id, vpr::SocketStream* newStream,bool& sync);

     /**
      * Creates a device request packet
      *
      * @param device_id   local unique ID for the remote device
      * @param sevice_name name of the device that we want to connect to
      */
      void  createDeviceRequest(vpr::Uint16 device_id, const std::string device_name);

     /**
      * Processes a device request packet.
      *
      * @param object_reader The object that contains the incoming packet.
      *
      */ 
      bool  receiveDeviceRequest(vpr::ObjectReader* object_reader);
      
      /**
       * Creates a device acknowledgement packet.
       *
       * @param remote_device_id  unique device ID on the requesting machine
       * @param local_device_id   unique device ID on the responding machine
       * @param device_name       name of the device
       * @param base_type         base type of the device
       */
      void  createDeviceAck(const vpr::Uint16 remote_device_id,
                            const vpr::Uint16 local_device_id,
                            const std::string device_name,
                            const std::string base_type);

      /**
       * Creates a device negative acknowledgement packet.
       *
       * @param remote_device_id  unique device ID on the requesting machine
       * @param local_device_id   unique device ID on the responding machine
       * @param device_name       name of the device
       */
      void  createDeviceNack(const vpr::Uint16 remote_device_id,
                             const vpr::Uint16 local_device_id,
                             const std::string device_name);


     /**
      * Receives a device acknowledgement packet.
      *
      * @param object_reader  The object that contains the incoming packet.
      */ 
      bool  receiveDeviceAck(vpr::ObjectReader* object_reader);
      
     /**
      * Creates a packet that requests the remote computer to act as a client
      * in a synchronization process.
      */
      void  createClockSyncClientRequest();
      
     /**
      * Create a packet containing all of the device data that needs to be sent
      * over the network.
      *
      * @param net_device  device that we want to send the data for
      */
      bool  createDeviceDataPacket(NetDevice* net_device);
      
     /**
      * Receives a packet containing all of the device data for a remote device.
      *
      * @param object_reader  The object that contains the incoming packet.
      * @param virtual_device pointer to the local "virtual" device
      */
      bool  recieveDeviceDataPacket(vpr::ObjectReader* object_reader, Input* virtual_device, vpr::Uint64* delta);
      
     /**
      * Creates a marker for the end of a given update cycle.
      *
      */
      void  createEndBlock();

   };

}  // end namespace gadget

#endif


