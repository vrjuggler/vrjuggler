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

#ifndef _CLUSTER_SWAP_LOCK_WIRED_PLUGIN_H
#define _CLUSTER_SWAP_LOCK_WIRED_PLUGIN_H

#include <cluster/PluginConfig.h>
#include <cluster/ClusterPlugin.h>
#include <cluster/Packets/Packet.h>

#include <vpr/Util/Singleton.h>
#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <jccl/RTRC/ConfigElementHandler.h>

#include <map>

namespace gadget
{
   class Node;
}

namespace cluster
{

class GADGET_CLUSTER_PLUGIN_CLASS_API SwapLockWiredPlugin
   : public cluster::ClusterPlugin
{
public:
   SwapLockWiredPlugin()
      : mWire(-1)
      , mHandlerGUID("f4f31d1c-eb4f-41fa-94d4-bde783bf32d6")
      , mIsMaster(false)
      , mActive(false)
      , mMasterWaitByte(0)
   {;}

   virtual ~SwapLockWiredPlugin()
   {;}

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
   virtual void handlePacket(Packet* packet, gadget::Node* node);

   /**
    * Virtual function that is unused by this plug since we are only
    * concerned with creating a SwapLock barrier.
    */
   virtual void preDraw();

   /**
    * Virtual function that is unused by this plug since we are only
    * concerned with creating a SwapLock barrier.
    */
   virtual void postPostFrame();

   /** Returns the status of SwapLockWiredPlugin(Not Used)
    *
    *  @return true If plugin is completly configured
    *               and has no pending tasks.
    *
    */
   virtual bool isPluginReady();

   /**
    * Create a barrier that will be used to have SwapLock syncronization.
    */
   virtual bool createBarrier();

   /**
    * Returns the name of this plugin.
    */
   virtual std::string getPluginName()
   {
      return(std::string("SwapLockWiredPlugin"));
   }

   virtual std::string getHandlerName()
   {
      return(std::string("SwapLockWiredPlugin"));
   }

   virtual void recoverFromLostNode(gadget::Node* lost_node)
   {
      boost::ignore_unused_variable_warning(lost_node);
   }

   /** Add the pending element to the configuration.
    *  PRE: configCanHandle (element) == true.
    *  @return true iff element was successfully added to configuration.
    */
   bool configAdd(jccl::ConfigElementPtr element);

   /** Remove the pending element from the current configuration.
    *  PRE: configCanHandle (element) == true.
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
private:
   /**
    * Helper function that determaines if the given ConfigElement is a SwapLockWiredPlugin.
    */
   bool recognizeSwapLockWiredPluginConfig(jccl::ConfigElementPtr element);

   /**
    * Returns the string representation of the element type used for the SwapLockWiredPlugin
    */
   static std::string getElementType() { return std::string( "swap_lock_wired_plugin" ); }

   bool ConnectToWiredParallel();
   bool Init();
   /**
    * Start listening for incoming connection requests.
    *
    * Note: This will only be used by the barrier master.
    */
   bool startListening();

   /**
    * Control loop that is run by mAcceptThread in order to accept incoming connection requests.
    *
    * Note: This will only be used by the barrier master.
    */
   void acceptLoop(void* nullParam);

   /**
    * Sends the "complete" signal to all sync slaves.
    */
   // void masterSend();

   /**
    * Waits for "ready" signals from all sync slaves.
    */
   // void masterReceive();

   /**
    * Sends "ready" signal to sync master.
    */
   // void slaveSend();

   /**
    * Waits for "complete" signal from sync master.
    */
   // void slaveReceive();

   void sendSignal(const unsigned char& val);
   void waitSignalUp(const unsigned char& val);
   void waitSignalDown(const unsigned char& val);
private:
   std::string                      mBarrierMasterHostname; /**< Hostname of the sync master. */

   int                              mWire;
   vpr::GUID                        mHandlerGUID;         /**< GUID for this ClusterPlugin */

   bool                             mIsMaster;           /**< Are we the sync master? */
   bool                             mActive;             /**< Is the plugin ready to be used? */
   unsigned char                    mMasterWaitByte;
};

} // end namespace

#endif
