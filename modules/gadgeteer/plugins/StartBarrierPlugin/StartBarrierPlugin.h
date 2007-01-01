/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _CLUSTER_START_BARRIER_PLUGIN_H
#define _CLUSTER_START_BARRIER_PLUGIN_H

#include <cluster/PluginConfig.h>
#include <cluster/ClusterPlugin.h>
#include <cluster/Packets/Packet.h>

#include <vpr/Util/Singleton.h>
#include <jccl/Config/ConfigElementPtr.h>

#include <map>

namespace gadget
{
   class Node;
}

namespace cluster
{
   class ClusterBarrier;

class GADGET_CLUSTER_PLUGIN_CLASS_API StartBarrierPlugin
   : public cluster::ClusterPlugin
{
public:
   StartBarrierPlugin();
   virtual ~StartBarrierPlugin();
   
   /**
    * Get the GUID associated with this plugin.
    */
   vpr::GUID getHandlerGUID()
   {
      return mHandlerGUID;
   }

   /**
    * Handle a incoming packet.
    */
   void handlePacket(Packet* packet, gadget::Node* node);

   virtual void preDraw();
   virtual void postPostFrame();
   virtual bool isPluginReady();
   
   virtual std::string getPluginName()
   {
      return(std::string("StartBarrierPlugin"));
   }
   
   virtual std::string getHandlerName()
   {
      return(std::string("StartBarrierPlugin"));
   }
   
   virtual void recoverFromLostNode(gadget::Node* lost_node)
   {
      boost::ignore_unused_variable_warning(lost_node);
   }

   /** Add the pending element to the configuration.
    *  @pre configCanHandle (element) == true.
    *  @return true iff element was successfully added to configuration.
    */
   bool configAdd(jccl::ConfigElementPtr element);
   
   /** Remove the pending element from the current configuration.
    *  @pre configCanHandle (element) == true.
    *  @return true iff the element (and any objects it represented)
    *          were successfully removed.
    */
   bool configRemove(jccl::ConfigElementPtr element);

   /** Checks if this handler can process element.
    *  Typically, an implementation of handler will check the element's
    *  description name/token to decide if it knows how to deal with
    *  it.
    *  @return true iff this handler can process element.
    */
   bool configCanHandle(jccl::ConfigElementPtr element);

   bool recognizeStartBarrierPluginConfig(jccl::ConfigElementPtr element);

   bool isBarrierMaster()
   { return mBarrierMaster; }
   
   std::vector<std::string> getBarrierSlaves()
   { return mSlaves; }
   
   std::vector<std::string> getPendingBarrierSlaves()
   {
      vpr::Guard<vpr::Mutex> guard(mPendingSlavesLock);

      return mPendingSlaves;
   }
   
   void removePendingBarrierSlave(const std::string& hostname)
   {

      vpr::Guard<vpr::Mutex> guard(mPendingSlavesLock);

      vpr::InetAddr searching_for_node;
      searching_for_node.setAddress(hostname, 0);

      for (std::vector<std::string>::iterator i = mPendingSlaves.begin();
           i != mPendingSlaves.end() ; i++)
      {
         vpr::InetAddr testing_node;
         
         std::string debug_name = (*i);

         testing_node.setAddress(*i,0);
         
         if (searching_for_node.getAddressString() == testing_node.getAddressString())
         {
            mPendingSlaves.erase(i);
            return;
         }
      }
   }

private:      
   /**
    * Returns the string representation of the element type used for the
    * StartBarrierPlugin.
    */   
   static std::string getElementType()
   {
      return std::string("start_barrier_plugin");
   }

private:
   // Barrier Variables
   std::vector<std::string>      mSlaves;
   std::vector<std::string>      mPendingSlaves;
   vpr::Mutex                    mPendingSlavesLock;
   bool                          mBarrierMaster;
   std::string                   mBarrierMasterHostname;
   std::string                   mBarrierMachineElementName;
   bool                          mComplete;
   vpr::GUID                     mHandlerGUID;
};

} // end namespace

#endif
