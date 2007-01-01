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

#ifndef _CLUSTER_CLUSTER_NODE_H
#define _CLUSTER_CLUSTER_NODE_H

#include <gadget/gadgetConfig.h>

#include <vpr/Sync/Semaphore.h>
#include <gadget/Node.h>

namespace cluster
{

/** \class ClusterNode ClusterNode.h cluster/ClusterNode.h
 *
 * Cluster node.
 */
class GADGET_CLASS_API ClusterNode : public gadget::Node
{
public:
   /**
    * Create a Node with the given attributes
    *
    * @param name          Name of the Cluster Node from the config file
    * @param host_name     Hostname of the remote machine
    * @param port          The scoket port that we should connect to
    * @param socket_stream SocketStream used to communicate with remote machine
    */
   ClusterNode(const std::string& name, const std::string& host_name, 
               const vpr::Uint16& port, vpr::SocketStream* socket_stream);

   /**
    * Shutdown the update thread and close the SocketStream
    */
   virtual ~ClusterNode();
   
   /**
    * Return if this node is updated or not.
    */
   bool isUpdated()
   {
      return mUpdated;
   }

   /**
    * Set the update status for this node.
    */
   void setUpdated(bool update)
   {
      mUpdated = update;
   }
   
public:
   /**
    * Control loop for updating this thread
    */
   virtual void controlLoop(void* nullParam);

   /**
    * Signal a semaphore to let the update thread fall into
    * the code to update the UserData structures
    */
   void signalUpdate();
         
   /**
    * Signal a semaphore to signal that we are done either
    * updating the UserData or DeviceData
    */
   void sync();
   
private:
   vpr::Mutex           mUpdatedLock;        /**< Lock the isUpdated value */
   bool                 mUpdated;            /**< States if this node is updated */
   
   vpr::Semaphore    mUpdateTriggerSema;     /**< Semaphore trigger for UserData update  */
   vpr::Semaphore    mNodeDoneSema;          /**< Semaphore trigger for completion */
};

} // end namespace gadget

#endif /* _CLUSTER_CLUSTER_NODE_H */
