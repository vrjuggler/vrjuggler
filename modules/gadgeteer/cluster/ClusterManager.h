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

#ifndef _CLUSTER_CLUSTER_MANAGER_H
#define _CLUSTER_CLUSTER_MANAGER_H

#include <vpr/Util/Singleton.h>

#include <gadget/gadgetConfig.h>
#include <gadget/Type/DeviceFactory.h>

#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <jccl/RTRC/ConfigChunkHandler.h>

#include <list>

namespace cluster
{
   class ClusterPlugin;

class GADGET_CLASS_API ClusterManager : public jccl::ConfigChunkHandler
{      
   vprSingletonHeader( ClusterManager );
public:
   ClusterManager();
   virtual ~ClusterManager();

   // ClusterPlugin Interface
public:
   void addPlugin(ClusterPlugin* new_manager);
   void removePlugin(ClusterPlugin* old_manager);
   bool doesPluginExist(ClusterPlugin* old_manager);
   
   void preDraw();
   void postPostFrame();
   void createBarrier();
   
   //Helper Functions for Plugins
private:   
   void sendEndBlocksAndSignalUpdate();
   
   //Configuration
public:
   bool recognizeRemoteDeviceConfig(jccl::ConfigChunkPtr chunk);
   bool recognizeClusterManagerConfig(jccl::ConfigChunkPtr chunk);
   

   /** Add the pending chunk to the configuration.
    *  PRE: configCanHandle (chunk) == true.
    *  @return true iff chunk was successfully added to configuration.
    */
   bool configAdd(jccl::ConfigChunkPtr chunk);
   
   /** Remove the pending chunk from the current configuration.
    *  PRE: configCanHandle (chunk) == true.
    *  @return true iff the chunk (and any objects it represented)
    *          were successfully removed.
    */
   bool configRemove(jccl::ConfigChunkPtr chunk);

   /** Checks if this handler can process chunk.
    *  Typically, an implementation of handler will check the chunk's
    *  description name/token to decide if it knows how to deal with
    *  it.
    *  @return true iff this handler can process chunk.
    */
   bool configCanHandle(jccl::ConfigChunkPtr chunk);
   
   //Config Helper Functions
private:      
   /**
    * Returns the string representation of the chunk type used for the ClusterManager
    */   
   static std::string getChunkType() { return std::string( "ClusterManager" ); }
   
   //Start Barrier Stuff
public:
   bool isClusterReady();
   
   bool isBarrierMaster()
   { return mBarrierMaster; }
   
   bool isRunning()
   { return mRunning; }
   
   void setRunning(bool run)
   { mRunning = run;}
   
   std::vector<std::string> getBarrierSlaves()
   { return mSlaves; }
   
   std::vector<std::string> getPendingBarrierSlaves()
   { return mPendingSlaves; }
   
   void removePendingBarrierSlave(const std::string& hostname)
   {
      for (std::vector<std::string>::iterator i = mPendingSlaves.begin();
           i != mPendingSlaves.end() ; i++)
      {
         if (hostname == (*i))
         {
            mPendingSlaves.erase(i);
            return;
         }
      }
   }

   //General helper functions
public:
   jccl::ConfigChunkPtr getConfigChunkPointer(std::string& name);

private:
   std::list<ClusterPlugin*>     mPlugins;            /**< List of Plugins.*/
   vpr::Mutex                    mPluginsLock;        /**< Lock on plugins list.*/
   std::string                   mBarrierMachineName; /**< Name of the barrier machine.*/
   
   // Barrier Variables //
   std::vector<std::string>      mSlaves;
   std::vector<std::string>      mPendingSlaves;
   bool                          mRunning;
   bool                          mBarrierMaster;
   std::string                   mBarrierMasterHostname;

};

} // end namespace

#endif
