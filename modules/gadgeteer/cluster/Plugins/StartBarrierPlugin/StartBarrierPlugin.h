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

#ifndef _CLUSTER_START_BARRIER_PLUGIN_H
#define _CLUSTER_START_BARRIER_PLUGIN_H

#include <cluster/Plugins/PluginConfig.h>

#include <vpr/Util/Singleton.h>

// Must implement the Abstract Base Class in order to be a manager used on the ClusterNetwork
#include <cluster/ClusterPlugin.h>

// Remove these includes when we move the handlePacket method to the cpp file.
#include <cluster/Packets/Packet.h>
#include <cluster/ClusterNetwork/ClusterNode.h>

#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <jccl/RTRC/ConfigChunkHandler.h>

#include <map>

extern "C" GADGET_CLUSTER_PLUGIN_API(cluster::ClusterPlugin*) initPlugin();

namespace cluster
{
   class ClusterBarrier;

class GADGET_CLUSTER_PLUGIN_CLASS_API StartBarrierPlugin
   : public cluster::ClusterPlugin
{
   vprSingletonHeader( StartBarrierPlugin );

public:
   StartBarrierPlugin();
   virtual ~StartBarrierPlugin();
   
   /**
    * Get the GUID associated with this plugin.
    */
   vpr::GUID getPluginGUID()
   {
      return mPluginGUID;
   }

   /**
    * Handle a incoming packet.
    */
   void handlePacket(Packet* packet, ClusterNode* node);

   virtual void preDraw();
   virtual void postPostFrame();
   virtual bool isPluginReady();
   
   virtual std::string getPluginName()
   {
      return(std::string("StartBarrierPlugin"));
   }

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

   bool recognizeStartBarrierPluginConfig(jccl::ConfigChunkPtr chunk);

   bool isBarrierMaster()
   { return mBarrierMaster; }
   
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

private:      
   /**
    * Returns the string representation of the chunk type used for the StartBarrierPlugin
    */   
   static std::string getChunkType() { return std::string( "StartBarrierPlugin" ); }

private:
   // Barrier Variables
   std::vector<std::string>      mSlaves;
   std::vector<std::string>      mPendingSlaves;
   bool                          mBarrierMaster;
   std::string                   mBarrierMasterHostname;
   bool                          mComplete;
   bool                          mSlaveWaitingOnPlugins;
   int                           mSlowDownMaster;
   vpr::GUID                     mPluginGUID;
};

} // end namespace

#endif
