/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <vpr/vprConfig.h>
#include <vpr/System.h>

#include <cluster/Plugins/SwapLockPlugin/ClusterBarrier.h>
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/Packets/PacketFactory.h>
#include <cluster/Packets/SyncRequest.h>
#include <cluster/Packets/SyncAck.h>

#include <gadget/Util/Debug.h>

#include <vector>

#include <boost/static_assert.hpp>

#include <vpr/Util/ReturnStatus.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Port/SerialPort.h>

namespace cluster
{
/////////////////////////////////////////
//    Serial Protocol
/////////////////////////////////////////
   static vpr::Uint8 SYNC_SIGNAL = 'G';
   static const vpr::Interval read_timeout(5,vpr::Interval::Sec);
   
   ClusterBarrierSerial::ClusterBarrierSerial()
   {
   
   }
   ClusterBarrierSerial::~ClusterBarrierSerial()
   {
      if (mSerialPort != NULL)
      {
         mSerialPort->close();
         delete mSerialPort;
      }
   }
   vpr::ReturnStatus ClusterBarrierSerial::Init()
   {
      vpr::ReturnStatus status;
   
      if (mIsMaster)
      {
         status = ConnectToMasterSerial();
         if (status.success())
         {
            mActive = true;
         }
         return(status);
      }
      else
      {
         status = ConnectToMasterSocket();
         if (!status.success())
         {
            return status;   
         }
         else
         {  
            status = ConnectToMasterSerial();
            if (status.success())
            {
               mActive = true;
            }
            return(status);
         }
      }
   }

   
   void ClusterBarrierSerial::AddBarrierSlave(vpr::SocketStream* sock_stream)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ClusterBarrierTCP: Adding Barrier Slave\n"<< vprDEBUG_FLUSH;
      mSyncClients.push_back(sock_stream);
   }
   
   vpr::ReturnStatus ClusterBarrierSerial::ConnectToMasterSerial()
   {
      //std::string port_name = "/dev/ttyS1";
      //mSerialPort = new vpr::SerialPort(port_name);
      mSerialPort = new vpr::SerialPort(mSerialPortName);
      
      mSerialPort->setOpenReadWrite();
      if ( !mSerialPort->open().success() )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "ClusterBarrierSerial::Init() Failed, Port: " << mSerialPortName << " could not be opened!\n" << vprDEBUG_FLUSH;
         return vpr::ReturnStatus::Fail;
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ClusterBarrierSerial::Init() Success, Port: " << mSerialPortName << " opened.\n" << vprDEBUG_FLUSH;
      }
      
      //int baud_rate = 38400;
      
      mSerialPort->clearAll();
      mSerialPort->enableLocalAttach();
      mSerialPort->setBufferSize(1);
      mSerialPort->setTimeout(0);
      //mSerialPort->setOutputBaudRate(baud_rate); // Put me before input to be safe
      //mSerialPort->setInputBaudRate(baud_rate);
      mSerialPort->setOutputBaudRate(mBaudRate); // Put me before input to be safe
      mSerialPort->setInputBaudRate(mBaudRate);

      mSerialPort->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
      mSerialPort->enableRead();
      
      return(vpr::ReturnStatus::Succeed);
   }
   
   vpr::ReturnStatus ClusterBarrierSerial::ConnectToMasterSocket()
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ClusterBarrierTCP: Attempting to connect to SyncServer:  " 
         << mHostname <<":"<< mTCPport << "\n"<< vprDEBUG_FLUSH;

      vpr::InetAddr inet_addr;
      
         // Set the address that we want to connect to
      if ( !inet_addr.setAddress(mHostname, mTCPport).success() )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"ClusterBarrierTCP: Failed to set address\n") 
            << vprDEBUG_FLUSH;
         mActive = false;

         return vpr::ReturnStatus::Fail;
      }
         // Create a new socket stream to this address
      mSyncServer = new vpr::SocketStream(vpr::InetAddr::AnyAddr, inet_addr);
      
         // If we can successfully open the socket and connect to the server
      mSyncServer->open();
      mSyncServer->enableBlocking();

      while (!mSyncServer->connect().success())
      {
         std::cout << clrSetBOLD(clrMAGENTA) << "[Remote Input Manager]"
         << " Waiting for Sync Server: " << mHostname << ":" << mTCPport
         << "["/* << gadget::spiner() */<< "]" << "\r" << clrRESET;
      }
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ClusterBarrierTCP: Successfully connected to sync server: " 
         << mHostname <<":"<< mTCPport << "\n"<< vprDEBUG_FLUSH;
      
      // Send request
      // Wait for responce
      // return result

      std::string local_host_name = ClusterNetwork::instance()->getLocalHostname();
      std::string temp_manager_id = std::string("ManagerID");

      SyncRequest sync_request(local_host_name, mTCPport, temp_manager_id);
      sync_request.send(mSyncServer);

      Packet* packet = PacketFactory::instance()->recvPacket(mSyncServer);
      SyncAck* ack_packet = static_cast<SyncAck*>(packet);

      if (ack_packet->getAck())
      {
         // add to list
         // respond true
         return(vpr::ReturnStatus::Succeed);
      }

      return(vpr::ReturnStatus::Fail);
   }

   void ClusterBarrierSerial::MasterSend()
   {
      vpr::Uint32 bytes_read;
      
      vprASSERT(mActive==true && "Barrier is not active!");
      mSerialPort->write(&SYNC_SIGNAL,1,bytes_read);
   }
   void ClusterBarrierSerial::MasterReceive()
   {
      vpr::Uint8 temp;
      vpr::Uint32 bytes_read;
      
      vprASSERT(mActive==true && "Barrier is not active!");

      //mSerialPort->read(&temp,1,bytes_read,read_timeout);   // DONT USE, SERIAL ONLY
      for (std::vector<vpr::SocketStream*>::iterator i = this->mSyncClients.begin();
               i < this->mSyncClients.end();i++)
      {
         (*i)->recv(&temp , 1, bytes_read,read_timeout);
         vprASSERT(1==bytes_read && "SYNC-BARRIER, Master sync receive timeout");
      }
   }
   void ClusterBarrierSerial::SlaveSend()
   {
      vprASSERT(mSyncServer!=NULL && "mSyncServer is NULL!");
      vprASSERT(mActive==true && "Barrier is not active!");
      
      vpr::Uint32 bytes_read;
      //mSerialPort->write(&SYNC_SIGNAL,1,bytes_read);     // DON'T USE, SERIAL ONLY
      mSyncServer->send(&SYNC_SIGNAL , 1, bytes_read);
   }
   void ClusterBarrierSerial::SlaveReceive()
   {
      vprASSERT(mSyncServer!=NULL && "mSyncServer is NULL!");
      vprASSERT(mActive==true && "Barrier is not active!");

      vpr::Uint32 bytes_read;
      vpr::Uint8 temp;
      mSerialPort->read(&temp,1,bytes_read,read_timeout);
   }
};
