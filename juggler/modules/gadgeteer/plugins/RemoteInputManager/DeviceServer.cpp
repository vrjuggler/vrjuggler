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

#include <gadget/gadgetConfig.h>
#include <gadget/Util/Debug.h>
#include <cluster/Plugins/RemoteInputManager/DeviceServer.h> // my header...
#include <cluster/ClusterNetwork/ClusterNode.h>



namespace cluster
{
   DeviceServer::DeviceServer(const std::string& name, gadget::Input* device) : deviceServerTriggerSema(0), deviceServerDoneSema(0)
   {
      mId.generate();   // Generate a unique ID for this device
      mThreadActive = false;
      mName = name;
      mDevice = device;
      mDataPacket = new DataPacket();
      mDeviceData = new std::vector<vpr::Uint8>;      
      mBufferObjectWriter = new vpr::BufferObjectWriter(mDeviceData);
      start();
   }
   DeviceServer::~DeviceServer()
   {
      shutdown();
   }
   
   void DeviceServer::shutdown()
   {     // Kill the accepting thread
      if ( mControlThread )
      {
         mThreadActive = false;
         mControlThread->kill();
         mControlThread = NULL;
      }
   }


   void DeviceServer::send()
   {
      lockClients();
//      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
//         << clrOutBOLD(clrMAGENTA,"DeviceServer::send()")
//         << "Sending Device Data for: " << getName() << std::endl << vprDEBUG_FLUSH;

      //--send to all nodes in the map
      //WE MUST NEVER USE THE BASE CLASS's SEND()
      for (std::vector<cluster::ClusterNode*>::iterator i = mClients.begin();
           i != mClients.end() ; i++)
      {
         //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Sending data to: " 
         //   << (*i)->getName() << std::endl << vprDEBUG_FLUSH;
         (*i)->lockSockWrite();
         //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "We have the lock for " << (*i)->getName()
         //   << std::endl << vprDEBUG_FLUSH;

         try
         {
            mDataPacket->send((*i)->getSockStream(), mId, mDeviceData);
         }
         catch(cluster::ClusterException cluster_exception)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "DeviceServer::send() Caught an exception!" 
               << std::endl << vprDEBUG_FLUSH;
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED)
               << cluster_exception.getMessage() << clrRESET
               << std::endl << vprDEBUG_FLUSH;

            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << 
               "DeviceServer::send() We have lost our connection to: " << (*i)->getName() << ":" << (*i)->getPort()
               << std::endl << vprDEBUG_FLUSH;
            
            (*i)->setConnected(ClusterNode::DISCONNECTED);
            (*i)->unlockSockWrite();
//            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Released the lock for " << (*i)->getName() 
//               << std::endl << vprDEBUG_FLUSH;

            debugDump(vprDBG_CONFIG_LVL);
         }
         (*i)->unlockSockWrite();
//         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "Released the lock for " << (*i)->getName() 
//            << std::endl << vprDEBUG_FLUSH;

      }
      unlockClients();
//      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
//         << clrOutBOLD(clrMAGENTA,"DeviceServer::send()")
//         << "Done Sending Device Data for: " << getName() << std::endl << vprDEBUG_FLUSH;
   }
   void DeviceServer::updateLocalData()
   {
      // -BufferObjectWriter
      mBufferObjectWriter->getData()->clear();
      mBufferObjectWriter->setCurPos(0);

      mDevice->writeObject(mBufferObjectWriter);
   }

   void DeviceServer::addClient(ClusterNode* new_client_node)
   {
      vprASSERT(0 == mClientsLock.test());
      vprASSERT(new_client_node != NULL && "You can not add a new client that is NULL");
      lockClients();

      mClients.push_back(new_client_node);
      
      unlockClients();
   }
   
   void DeviceServer::removeClient(const std::string& host_name)
   {
      vprASSERT(0 == mClientsLock.test());
      lockClients();
   
      for (std::vector<cluster::ClusterNode*>::iterator i = mClients.begin() ; 
            i!= mClients.end() ; i++)
      {
         if ((*i)->getHostname() == host_name)
         {
            mClients.erase(i);
            unlockClients();
            return;
         }
      }
      unlockClients();
   }
   
   void DeviceServer::debugDump(int debug_level)
   {
      vprASSERT(0 == mClientsLock.test());
      lockClients();

      vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                                 std::string("-------------- DeviceServer --------------\n"),
                                 std::string("------------------------------------------\n"));

      vprDEBUG(gadgetDBG_RIM,debug_level) << "Name:     " << mName << std::endl << vprDEBUG_FLUSH;
      
      { // Used simply to make the following DebugOutputGuard go out of scope 
         vpr::DebugOutputGuard dbg_output2(gadgetDBG_RIM,debug_level,
                           std::string("------------ Clients ------------\n"),
                           std::string("---------------------------------\n"));
         for (std::vector<cluster::ClusterNode*>::iterator i = mClients.begin() ; 
               i!= mClients.end() ; i++)
         {
            vprDEBUG(gadgetDBG_RIM,debug_level) << "-------- " << (*i)->getName() << " --------" << std::endl << vprDEBUG_FLUSH;
            vprDEBUG(gadgetDBG_RIM,debug_level) << "       Hostname: " << (*i)->getHostname() << std::endl << vprDEBUG_FLUSH;
            vprDEBUG(gadgetDBG_RIM,debug_level) << "----------------------------------" << std::endl << vprDEBUG_FLUSH;
         }
      }
      unlockClients();
   }

   void DeviceServer::controlLoop(void* nullParam)
   {
      // -Block on an update call
      // -Update Local Data
      // -Send
      // -Signal Sync

      while(true)
      {
         // Wait for trigger
         deviceServerTriggerSema.acquire();

         updateLocalData();
         send();
         
         // Signal Done Rendering
         deviceServerDoneSema.release();
      }
   }
   /** Starts the control loop. */
   void DeviceServer::start()
   {
      // --- Setup Multi-Process stuff --- //
      // Create a new thread to handle the control
      
      vpr::ThreadMemberFunctor<DeviceServer>* memberFunctor =
         new vpr::ThreadMemberFunctor<DeviceServer>(this, &DeviceServer::controlLoop, NULL);
   
      mControlThread = new vpr::Thread(memberFunctor);
   
      if (mControlThread->valid())
      {
         mThreadActive = true;
      }
      vprDEBUG(gadgetDBG_RIM,1) << "DeviceServer " << getName() << " started. thread: "
                                << mControlThread << std::endl << vprDEBUG_FLUSH;
   }

   void DeviceServer::go()
   {
      while(!mThreadActive)
      {
         vprDEBUG(gadgetDBG_RIM,/*vprDBG_HVERB_LVL*/1) << "Waiting in for thread to start DeviceServer::go().\n" << vprDEBUG_FLUSH;
         vpr::Thread::yield();
      }
      deviceServerTriggerSema.release();
   }
   
   
   /**
    * Blocks until the end of the frame.
    * @post The frame has been drawn.
    */
   void DeviceServer::sync()
   {
      vprASSERT(mThreadActive == true);
      deviceServerDoneSema.acquire();
   }



} // End of gadget namespace
