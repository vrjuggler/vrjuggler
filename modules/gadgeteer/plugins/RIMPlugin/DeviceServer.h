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

#ifndef _CLUSTER_DEVICE_SERVER_H
#define _CLUSTER_DEVICE_SERVER_H

#include <cluster/PluginConfig.h>
#include <cluster/Packets/DataPacket.h>
#include <gadget/Type/Input.h>
#include <vpr/Sync/Semaphore.h>
#include <vpr/Thread/Thread.h>

namespace cluster
{
   class Node;
   
   /** \class DeviceServer DeviceServer.h gadget/DeviceServer.h
    *
    * Device server class.
    */
   class GADGET_CLUSTER_PLUGIN_CLASS_API DeviceServer
   {
   public:
      /**
       * Create a new DeviceServer.
       *
       * @param name      Name of the device that we are sharing.
       * @param device    Pointer to the device that we are sharing.
       * @param plugin_id GUID that should be placed at the beginning of 
       *                  each data packet so that the receiver knows which 
       *                  plugin the data is coming from.
       */
      DeviceServer(const std::string& name, gadget::Input* device,
                   const vpr::GUID& plugin_id);

      ~DeviceServer();

      void send();
      void updateLocalData();

      void addClient(gadget::Node* new_client_node);
      void removeClient(const std::string& host_name);

      void debugDump(int debug_level);

      std::string getName() { return mName; }

      /** Locks the active list.
       *
       *  This function blocks until it can lock the std::vector of active
       *  Nodes.
       *
       *  The caller of this method must call unlockActive() when it
       *  is finished viewing/modifying the active list.
       */
      void lockClients()
      { mClientsLock.acquire(); }

      /** Unlocks the active list.
       *
       *  The method releases the lock on the active connections std::vector.
       *
       *  The caller of this method must have previously locked the active
       *  list with lockActive().
       */
      void unlockClients()
      { mClientsLock.release(); }

      vpr::GUID getId()
      {
         return mId;
      }

   private:
      std::string                         mName;   /**< DeviceServer name */
      std::vector<gadget::Node*>  mClients;
      vpr::Mutex                          mClientsLock;   /**< Lock on active config list.*/   
      
      gadget::Input*                      mDevice;
      cluster::DataPacket*                mDataPacket;
      vpr::BufferObjectWriter*            mBufferObjectWriter;
      std::vector<vpr::Uint8>*            mDeviceData;

      vpr::Semaphore                      deviceServerTriggerSema;/**< Semaphore for draw trigger */
      vpr::Semaphore                      deviceServerDoneSema;   /**< Semaphore for drawing done */
      vpr::Thread*                        mControlThread;
      bool                                mThreadActive;
      vpr::GUID                           mId;                    /**< GUID for shared device */
      vpr::GUID                           mPluginGUID;
   };

} // end namespace cluster

#endif /*_CLUSTER_DEVICE_SERVER_H*/
