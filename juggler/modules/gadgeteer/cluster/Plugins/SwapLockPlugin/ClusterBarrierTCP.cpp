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

#include <gadget/Util/Debug.h>
                          
#include <cluster/Plugins/SwapLockPlugin/SwapLockPlugin.h>
#include <cluster/Plugins/SwapLockPlugin/ClusterBarrier.h>
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/Packets/PacketFactory.h>
#include <cluster/Packets/SyncRequest.h>
#include <cluster/Packets/SyncAck.h>
#include <vector>

#include <vpr/Thread/Thread.h>
#include <boost/static_assert.hpp>

#include <vpr/Util/ReturnStatus.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Port/SerialPort.h>


namespace cluster
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
         SwapLockPlugin::instance()->setActive(true);
         return(vpr::ReturnStatus::Succeed);
      }

      return(vpr::ReturnStatus::Fail);
   }

   void ClusterBarrierTCP::MasterSend()
   {
      //vprASSERT(mSyncClients!=NULL && "Sync Clients Vector is NULL!");
      vprASSERT(mActive==true && "Barrier is not active!");

      vpr::Uint32 bytes_read;

      for (std::vector<vpr::SocketStream*>::iterator i = this->mSyncClients.begin();
        i < this->mSyncClients.end();i++)
      {
         (*i)->send(&SYNC_SIGNAL , 1, bytes_read);
      }

   }
   void ClusterBarrierTCP::MasterReceive()
   {
      vprASSERT(mActive==true && "Barrier is not active!");

      vpr::Uint32 bytes_read;
      vpr::Uint8 temp;
      for (std::vector<vpr::SocketStream*>::iterator i = this->mSyncClients.begin();
           i < this->mSyncClients.end();i++)
      {
         if((*i)->recv(&temp , 1, bytes_read,read_timeout) == vpr::ReturnStatus::Timeout)
         {
            static int numTimeouts = 0;
            numTimeouts++;
            vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "ClusterBarrierTCP: Received a timeout from a cluster node, it was removed" 
               << std::endl << vprDEBUG_FLUSH;
            if (numTimeouts > 5)
            {
               mSyncClients.erase(i);
               vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << "ClusterBarrierTCP: Received too many timeouts from a cluster node,"
                  <<" so it was removed from the list of machines to syncronize with." 
                  << std::endl << vprDEBUG_FLUSH;
               vpr::System::sleep(1);
            }
         }
         //vprASSERT(1==bytes_read && "ClusterBarrierTCP: Master Barrier received timeout");
      } 
   }
   void ClusterBarrierTCP::SlaveSend()
   {
      vprASSERT(mSyncServer!=NULL && "mSyncServer is NULL!");
      vprASSERT(mActive==true && "Barrier is not active!");

      vpr::Uint32 bytes_read;
      mSyncServer->send(&SYNC_SIGNAL , 1, bytes_read);
   }
   void ClusterBarrierTCP::SlaveReceive()
   {
      vprASSERT(mSyncServer!=NULL && "mSyncServer is NULL!");
      vprASSERT(mActive==true && "Barrier is not active!");
      
      vpr::Uint32 bytes_read;
      vpr::Uint8 temp;
      if (!mSyncServer->recv(&temp , 1, bytes_read,read_timeout).success())
      {
         std::cout << "[ClusterBarrierTCP] SYNC ERROR" << std::endl;
         vpr::System::sleep(1);
      }
      //vprASSERT(1==bytes_read && "ClusterBarrierTCP: Slave Barrier received timeout");
   }


   //////////////////////////////////////////
   bool ClusterBarrierTCP::startListening()
   {
      if ( mAcceptThread == NULL )  // If we haven't already started the listening thread
      {     
         if ( mTCPport > 0 )     // If the listen port is valid
         {
            mListenAddr.setPort(mTCPport);

            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
               << "Starting the listening thread...\n" << vprDEBUG_FLUSH;

               // Start a thread to monitor port
            vpr::ThreadMemberFunctor<ClusterBarrierTCP>* memberFunctor =
            new vpr::ThreadMemberFunctor<ClusterBarrierTCP>
            (this, &ClusterBarrierTCP::acceptLoop, NULL);
            
            mAcceptThread = new vpr::Thread(memberFunctor);
            vprASSERT(mAcceptThread->valid());
            return true;
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrMAGENTA,"[ClusterBarrierTCP]")
               << "startListening() Can NOT listen on port: "
               << mTCPport << "\n" << vprDEBUG_FLUSH;
            return false;
         }
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrMAGENTA,"[ClusterBarrierTCP]")
               << "startListening() Listening thread already active.\n" << vprDEBUG_FLUSH;
         return false;
      }
   }
   
   void ClusterBarrierTCP::acceptLoop(void* nullParam)
   {
      //////////////////////////// Create an acceptor socket that listens on port. //////////////////////////////

      vpr::SocketStream sock(mListenAddr, vpr::InetAddr::AnyAddr);
        // Open in server mode.
      if ( sock.openServer().success() )
      {
         sock.setReuseAddr(true);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
            << clrSetBOLD(clrRED) << "[ClusterNetwork]"
            << " Unable to open listening socket on port: " << mListenAddr.getPort() << std::endl << clrRESET << vprDEBUG_FLUSH;
         exit(0);
      }
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
         << " Listening on Port: " << mListenAddr.getPort() << std::endl << vprDEBUG_FLUSH;      

      ///////////////////////////  Wait for a connection //////////////////////////////

      vpr::SocketStream* client_sock = new vpr::SocketStream;

      while ( 1 )
      {    // Wait for an incoming connection.   
         vpr::ReturnStatus status = sock.accept(*client_sock, vpr::Interval::NoTimeout);
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
            << clrOutBOLD(clrMAGENTA,"[ClusterNetwork]")
            << " Received a connection attempt on Port: " << mListenAddr.getPort() << std::endl << vprDEBUG_FLUSH;
         if ( status.success() )
         {               
            client_sock->setNoDelay(true);  
               // Lock Cluster Nodes while working on them
              // Try to receive a Packet
            Packet* temp_packet;
            try
            {
               temp_packet = PacketFactory::instance()->recvPacket(client_sock);
            }
            catch(cluster::ClusterException cluster_exception)
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
                  << clrOutBOLD(clrRED,"ERROR:")
                  << "ClusterNetwork::acceptLoop() Caught an exception: " 
                  << cluster_exception.getMessage() << std::endl << vprDEBUG_FLUSH;

               delete client_sock;
               client_sock = new vpr::SocketStream;
               continue;
            }
            
            SyncRequest* sync_request;

              // Make sure it is a ConnectionRequest
            try
            {
               sync_request = static_cast<SyncRequest*>(temp_packet);
               sync_request->printData(vprDBG_CONFIG_LVL);
            }
            catch(...)
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
                  << clrOutBOLD(clrRED,"ERROR:")
                  << " ClusterNetwork::acceptLoop() The first packet read on the socket is NOT a Sync Request" << std::endl << vprDEBUG_FLUSH;
               exit(0);
            }
            /////////////////
            this->AddBarrierSlave(client_sock);
            SwapLockPlugin::instance()->setActive(true);

            std::string host = ClusterNetwork::instance()->getLocalHostname();
            vpr::Uint16 temp_port = mTCPport;
            std::string temp_manager_id("ManagerID");

            SyncAck temp_ack(host, temp_port, temp_manager_id, true);


            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrMAGENTA,"[]")
               << "Sending responce: " << std::endl << vprDEBUG_FLUSH;
            
            temp_ack.printData(vprDBG_CONFIG_LVL);
            temp_ack.send(client_sock);
                        
            // We need a new SocketStream since the old one is now being used by a ClusterNode
            client_sock = new vpr::SocketStream;
            
            //
            /////////////////////////////////////////////
            
         }
         else if ( status == vpr::ReturnStatus::Timeout )
         {
               // Should never happen since timeout is infinite
            client_sock->close();
            delete client_sock;
            client_sock = new vpr::SocketStream;
         }
      }   // end infinite while      
   }


};
