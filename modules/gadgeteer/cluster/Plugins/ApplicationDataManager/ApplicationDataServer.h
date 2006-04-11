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

#ifndef CLUSTER_ApplicationData_SERVER_H
#define CLUSTER_ApplicationData_SERVER_H

#include <gadget/gadgetConfig.h>
#include <cluster/Packets/DataPacket.h>

//#include <gadget/Type/Input.h>


// TODO: Remove this header by moving all needed data to a new location
//#include <gadget/RemoteInputManager/NetUtils.h>
//#include <gadget/RemoteInputManager/NetDevice.h>
//#include <vpr/IO/BufferObjectWriter.h>
//#include <vpr/IO/BufferObjectReader.h>
//#include <jccl/RTRC/ConfigChunkHandler.h>
//#include <gadget/RemoteInputManager/ClusterSync.h>

namespace cluster
{
   class ClusterNode;
   class ApplicationData;

   class GADGET_CLASS_API ApplicationDataServer
   {
   public:
      ApplicationDataServer(vpr::GUID guid, ApplicationData* user_data);
      ~ApplicationDataServer();

      void send();
      void updateLocalData();

      void addClient(ClusterNode* new_client_node);
      void removeClient(const std::string& host_name);

      void debugDump(int debug_level);

      vpr::GUID getId() { return mId; }
      /** Locks the active list.
       *
       *  This function blocks until it can lock the std::map of active
       *  ClusterNodes.
       *
       *  The caller of this method must call unlockActive() when it
       *  is finished viewing/modifying the active list.
       */
      void lockClients()
      { mClientsLock.acquire(); }

      /** Unlocks the active list.
       *
       *  The method releases the lock on the active connections std::map.
       *
       *  The caller of this method must have previously locked the active
       *  list with lockActive().
       */
      void unlockClients()
      { mClientsLock.release(); }

   private:
      vpr::GUID                                    mId;   /**< ApplicationDataServer name */
      std::vector<cluster::ClusterNode*>           mClients;
      vpr::Mutex                                   mClientsLock;   /**< Lock on active config list.*/   
      
      ApplicationData*                             mApplicationData;
      DataPacket*                                  mDataPacket;
      vpr::BufferObjectWriter*                     mBufferObjectWriter;
      std::vector<vpr::Uint8>*                     mDeviceData;
   };

} // end namespace gadget

#endif
