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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>
#include <vpr/System.h>

#include <gadget/RemoteInputManager/ClusterBarrier.h>
#include <vector>

#include <boost/static_assert.hpp>

#include <vpr/Util/ReturnStatus.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Port/SerialPort.h>

namespace gadget
{
/////////////////////////////////////////
//    Serial Protocol
/////////////////////////////////////////
   static vpr::Uint8 SYNC_SIGNAL = 'G';
   static const vpr::Interval read_timeout(5,vpr::Interval::Sec);
   
   void ClusterBarrierSerial::AddBarrierSlave(vpr::SocketStream* sock_stream)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ClusterBarrierTCP: Adding Barrier Slave\n"<< vprDEBUG_FLUSH;
      mSyncClients.push_back(sock_stream);
   }
   
   vpr::ReturnStatus ClusterBarrierSerial::ConnectToMasterSerial()
   {
      std::string port_name = "/dev/ttyS1";

      std::cout << "Before Create!\n" << std::endl;
      mSerialPort = new vpr::SerialPort(port_name);
      std::cout << "After Create!\n" << std::endl;
      
      std::cout << "Before set!\n" << std::endl;
      mSerialPort->setOpenReadWrite();
      std::cout << "After set!\n" << std::endl;
      std::cout << "Before Open!\n" << std::endl;
      if ( !mSerialPort->open().success() )
      {
         std::cout << "Before Create!\n" << std::endl;
          std::cerr << "SYNC-BARRIER, Port: " << port_name << " could not be opened!" << std::endl;
          return vpr::ReturnStatus::Fail;
      }
      else
      {
         std::cout << "Before Create!\n" << std::endl;
          std::cerr << "SYNC-BARRIER, Success, Port: " << port_name << " opened." << std::endl;
      }
      
      int baud_rate = 38400;
      
      mSerialPort->clearAll();
      mSerialPort->enableLocalAttach();
      mSerialPort->setBufferSize(1);
      mSerialPort->setTimeout(0);
      mSerialPort->setOutputBaudRate(baud_rate); // Put me before input to be safe
      mSerialPort->setInputBaudRate(baud_rate);
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
      if ( mSyncServer->open().success() && mSyncServer->connect().success() )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ClusterBarrierTCP: Successfully connected to sync server: " 
            << mHostname <<":"<< mTCPport << "\n"<< vprDEBUG_FLUSH;

         gadget::MsgPackage* msg_packer = new gadget::MsgPackage;
         msg_packer->createHandshake(true,mHostname,mTCPport,"Any Manager",true);
         msg_packer->sendAndClear(mSyncServer);
         delete msg_packer;
            // Send a handshake to initalize communication with remote computer
         //mMsgPackage->createHandshake(true,mHostname,mTCPport,"Any Manager",true);
         //mMsgPackage->sendAndClear(mSyncServer);
         return(vpr::ReturnStatus::Succeed);
      }
      else
      {
         delete mSyncServer;
         mSyncServer = NULL;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrSetNORM(clrRED) 
            << "ClusterBarrierTCP: Could not connect to sync server: " << mHostname 
            <<" : "<< mTCPport << "\n" << clrRESET << vprDEBUG_FLUSH;
         return vpr::ReturnStatus::Fail;
      }
   }

   void ClusterBarrierSerial::MasterSend()
   {
      vpr::Uint32 bytes_read;
      mSerialPort->write(&SYNC_SIGNAL,1,bytes_read);
   }
   void ClusterBarrierSerial::MasterReceive()
   {
      vpr::Uint8 temp;
      vpr::Uint32 bytes_read;
      
      mSerialPort->read(&temp,1,bytes_read,read_timeout);   // DONT USE, SERIAL ONLY
      /*for (std::vector<vpr::SocketStream*>::iterator i = this->mSyncClients.begin();
               i < this->mSyncClients.end();i++)
      {
         (*i)->recv(&temp , 1, bytes_read,read_timeout);
         vprASSERT(1==bytes_read && "SYNC-BARRIER, Master sync receive timeout");
      } */
   }
   void ClusterBarrierSerial::SlaveSend()
   {
      vpr::Uint32 bytes_read;
      mSerialPort->write(&SYNC_SIGNAL,1,bytes_read);     // DON'T USE, SERIAL ONLY
      // mSyncServer->send(&SYNC_SIGNAL , 1, bytes_read);
   }
   void ClusterBarrierSerial::SlaveReceive()
   {
      vpr::Uint32 bytes_read;
      vpr::Uint8 temp;
      mSerialPort->read(&temp,1,bytes_read,read_timeout);
   }
};
