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

#include <cluster/Plugins/SwapLockPlugin/SwapLockPlugin.h> // my header...
#include <cluster/Plugins/SwapLockPlugin/ClusterBarrier.h>

#include <gadget/Util/Debug.h>
#include <cluster/ClusterDepChecker.h>

#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>


namespace cluster
{
   vprSingletonImp( SwapLockPlugin );
   
   SwapLockPlugin::SwapLockPlugin()
   {
      // This is done by the ClusterManager
      //jccl::DependencyManager::instance()->registerChecker(new ClusterDepChecker());
      mFrameNumber = 0;
      mBarrier = NULL;
   }
   SwapLockPlugin::~SwapLockPlugin()
   {
      if (NULL != mBarrier)
      {
         delete mBarrier;
         mBarrier = NULL;
      }
   }
   
   /** Add the pending chunk to the configuration.
   *  PRE: configCanHandle (chunk) == true.
   *  @return true iff chunk was successfully added to configuration.
   */
   bool SwapLockPlugin::configAdd(jccl::ConfigChunkPtr chunk)
   {
      /*if (recognizeSwapLockPluginConfig(chunk))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[SwapLockPlugin] ")
         << "Configure the SwapLock: " << chunk->getName() 
         << "\n" << vprDEBUG_FLUSH;
         return(true);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) 
         << clrOutBOLD(clrRED,"[SwapLockPlugin::ConfigAdd] ERROR, Something is seriously wrong, we should never get here\n")
         << vprDEBUG_FLUSH;
         return(true);
      }       
      */
      


      // Find the name of the Sync Server
      ///////////////////////////////////////////////////////////////////////////////////
      std::string mSyncMasterChunkName = chunk->getProperty<std::string>("sync_server");
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "SYNC_MACHINE is: " << mSyncMasterChunkName << std::endl << vprDEBUG_FLUSH;
      ///////////////////////////////////////////////////////////////////////////////////
      
      // Configure the ClusterBarrier
      //////////////////////////////////////////////////////////////////////////////////
      
      // Temporary variables to hold SyncServer information
      //         jccl::ConfigChunkPtr sync_server_chunk = mClusterTable[mSyncMasterChunkName];
      //std::string          sync_server_hostname = sync_server_chunk->getProperty<std::string>("host_name");
      //int                  sync_server_listen_port = sync_server_chunk->getProperty<int>("listen_port");
      
      jccl::ConfigChunkPtr server_chunk = ClusterManager::instance()->getConfigChunkPointer(mSyncMasterChunkName);

      if (NULL == server_chunk.get())
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"This machine is not in the current Cluster Configuration!\n") << vprDEBUG_FLUSH;
         exit(1);
      }

      // Get Barrier method
      int sync_method = chunk->getProperty<int>("sync_method");
      std::cout << "Method: " << sync_method << std::endl;

      //////////// Get Local Machin Configuration /////////
      //         jccl::ConfigChunkPtr local_machine_chunk = mClusterTable[mLocalMachineChunkName];
      //         std::string          serial_port = local_machine_chunk->getProperty<std::string>("serialPort");
      //         int                  baud_rate = local_machine_chunk->getProperty<int>("serialBaud");
      /////////////////////////////////////////////////////
      
      switch (sync_method)
      {
      case 0:
         mBarrier = NULL;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << 
            clrOutBOLD(clrRED,"Barrier Method: NONE\n") << vprDEBUG_FLUSH;            
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << 
            clrOutBOLD(clrRED,"Significant Tear Might Appear Between Screens!!!\n") << vprDEBUG_FLUSH;            
         break;
      case 1:
         mBarrier = new ClusterBarrierTCP;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << 
            "ClusterBarrierTCP Barrier Method: TCP/IP Sockets \n" << vprDEBUG_FLUSH;
         break;
      case 2:
         mBarrier = new ClusterBarrierSerial;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << 
            "ClusterBarrierSerial Barrier Method: TCP/IP Sockets & Serial Port \n" << vprDEBUG_FLUSH;
         break;
      case 3:
#ifndef VPR_OS_Win32
         mBarrier = new ClusterBarrierWired;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << 
            "ClusterBarrierWire Barrier Method: Altered Serial Driver \n" << vprDEBUG_FLUSH;
#else
         vprASSERT(false && "gadget::ClusterBarrierWired is not available on Win32");
#endif
         break;
      default:
         mBarrier = new ClusterBarrierTCP;
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << 
            "ClusterBarrierTCP Barrier Method: TCP/IP Sockets \n" << vprDEBUG_FLUSH;
         break;
      }
         
         if (mBarrier != NULL)
         {
            int sync_port = chunk->getProperty<int>("sync_server_port");            

            // XXX: We are actually going to want to get a different
            // port from a config file so that we do not have to 
            // deal with the same slow port as the RIM or UserData
            
            //mBarrier->setHostname(sync_server->getHostname());  
            mBarrier->setHostname(server_chunk->getProperty<std::string>("host_name"));
            mBarrier->setTCPPort(sync_port);
            
            //? mBarrier->setSerialPort(serial_port);
            //? mBarrier->setBaudRate(baud_rate);
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrCYAN)                                
               << "=========== Barrier Settings ==============="
               << "\nBarrier-SyncServer Chunk:  " << mSyncMasterChunkName 
               << "\nBarrier-SyncServer Host    " << server_chunk->getProperty<std::string>("host_name")
               << "\nBarrier-SyncServer Port:   " << sync_port
               //<< "\nBarrier-Local Serial Port: " << serial_port
               //<< "\nBarrier-Local Baud Rate:   " << baud_rate 
               << clrRESET << std::endl << vprDEBUG_FLUSH;
      
            // Have all clients connect to sync server
            //   If master 
            //   - Set master true
            //   Else
            //   - connect to sync Master server  
      
            if (ClusterNetwork::instance()->getLocalHostname()== server_chunk->getProperty<std::string>("host_name"))
            {
               mBarrier->setMaster(true);
               mBarrier->Init();
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "SYNC This machine is sync server!" << std::endl << vprDEBUG_FLUSH;
            }
            else
            {
               mBarrier->Init();
            }
         }
         //////////////////////
         return true;
   }
   
   
   /** Remove the pending chunk from the current configuration.
   *  PRE: configCanHandle (chunk) == true.
   *  @return true iff the chunk (and any objects it represented)
   *          were successfully removed.
   */
   bool SwapLockPlugin::configRemove(jccl::ConfigChunkPtr chunk)
   {
      if (recognizeSwapLockPluginConfig(chunk))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SwapLockPlugin] Disabling SwapLock: " << chunk->getName() 
         << "\n" << vprDEBUG_FLUSH;
         return(true);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[SwapLockPlugin::configRemove] ERROR, Something is seriously wrong, we should never get here\n"
         << vprDEBUG_FLUSH;
         return(false);
      }
   }
   
   /** Checks if this handler can process chunk.
   *  Typically, an implementation of handler will check the chunk's
   *  description name/token to decide if it knows how to deal with
   *  it.
   *  @return true iff this handler can process chunk.
   */
   bool SwapLockPlugin::configCanHandle(jccl::ConfigChunkPtr chunk)
   {
      return( recognizeSwapLockPluginConfig(chunk) );   
   }
      
   bool SwapLockPlugin::recognizeSwapLockPluginConfig(jccl::ConfigChunkPtr chunk)
   {
      return(chunk->getDescToken() == getChunkType());
   }

   void SwapLockPlugin::preDraw()
   {;
   }
   void SwapLockPlugin::postPostFrame()
   {;
   }
   
   bool SwapLockPlugin::isPluginReady()
   {
      // We could do some sort of signal here I guess?
      return (true);
   }
   
   bool SwapLockPlugin::createBarrier()
   {
      // If Slave
      // - send ready to Master
      // - read ready from Server
      // Else if Master
      // - for each slave
      //   - read ready
      // - for each slave
      //   - send ready

      if (/*mConfigured &&*/ mBarrier != NULL && mBarrier->isActive())
      {
         //vpr::Interval first_time, second_time;
         //first_time.setNow();

         if (mBarrier->isMaster())
         {
            mBarrier->MasterReceive();
            mBarrier->MasterSend();
         }
         else
         {
            mBarrier->SlaveSend();
            mBarrier->SlaveReceive();
         }
         //second_time.setNow();
         //vpr::Interval diff_time(second_time-first_time);
         //vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrSetBOLD(clrCYAN) << "Latency: " 
         //   << diff_time.getBaseVal() << " usecs\n"<< clrRESET << vprDEBUG_FLUSH;
      }
      return(true);
   }

} // End of gadget namespace
