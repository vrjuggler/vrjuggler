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
   static vpr::Uint8 SYNC_SIGNAL = 'G';
   static const vpr::Interval read_timeout(5,vpr::Interval::Sec);
   void ClusterBarrierTCP::AddBarrierSlave(vpr::SocketStream* sock_stream)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ClusterBarrierTCP: Adding Barrier Slave\n"<< vprDEBUG_FLUSH;
      mSyncClients.push_back(sock_stream);
   }
   
   vpr::ReturnStatus ClusterBarrierTCP::ConnectToMasterSocket()
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

   void ClusterBarrierTCP::MasterSend()
   {
      //vprASSERT(mSyncClients!=NULL && "Sync Clients Vector is NULL!");
      vprASSERT(mActive=true && "Barrier is not active!");

      vpr::Uint32 bytes_read;

      for (std::vector<vpr::SocketStream*>::iterator i = this->mSyncClients.begin();
        i < this->mSyncClients.end();i++)
      {
         (*i)->send(&SYNC_SIGNAL , 1, bytes_read);
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "ClusterBarrierTCP: Sent END_BARRIER signal from Master..." 
            << std::endl << vprDEBUG_FLUSH;
      }

   }
   void ClusterBarrierTCP::MasterReceive()
   {
      vprASSERT(mActive=true && "Barrier is not active!");

      vpr::Uint32 bytes_read;
      vpr::Uint8 temp;
      for (std::vector<vpr::SocketStream*>::iterator i = this->mSyncClients.begin();
           i < this->mSyncClients.end();i++)
      {
         (*i)->recv(&temp , 1, bytes_read,read_timeout);
         vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "ClusterBarrierTCP: Waiting at Master Barrier..." 
            << std::endl << vprDEBUG_FLUSH;
         vprASSERT(1==bytes_read && "ClusterBarrierTCP: Master Barrier received timeout");
      } 
   }
   void ClusterBarrierTCP::SlaveSend()
   {
      vprASSERT(mSyncServer!=NULL && "mSyncServer is NULL!");
      vprASSERT(mActive=true && "Barrier is not active!");

      vpr::Uint32 bytes_read;
      mSyncServer->send(&SYNC_SIGNAL , 1, bytes_read);
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "ClusterBarrierTCP: Sent START_BARRIER signal from Slave..." 
         << std::endl << vprDEBUG_FLUSH;
   }
   void ClusterBarrierTCP::SlaveReceive()
   {
      vprASSERT(mSyncServer!=NULL && "mSyncServer is NULL!");
      vprASSERT(mActive=true && "Barrier is not active!");
      
      vpr::Uint32 bytes_read;
      vpr::Uint8 temp;
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << "ClusterBarrierTCP: Waiting at Slave Barrier..." 
         << std::endl << vprDEBUG_FLUSH;
      mSyncServer->recv(&temp , 1, bytes_read,read_timeout);
      vprASSERT(1==bytes_read && "ClusterBarrierTCP: Slave Barrier received timeout");
   }
};
