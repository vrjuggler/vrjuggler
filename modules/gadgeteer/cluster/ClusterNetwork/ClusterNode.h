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

#ifndef _CLUSTER_NODE_CONNECTION_H
#define _CLUSTER_NODE_CONNECTION_H

#include <gadget/gadgetConfig.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/Sync/Semaphore.h>
#include <vpr/Thread/Thread.h>

namespace cluster
{

   class DeviceRequest;
   class Packet;

   class GADGET_CLASS_API ClusterNode
   {
   public:
      enum Status
      {
         DISCONNECTED   = 0,
         CONNECTED      = 1,
         INPROGRESS     = 2,
         NEWCONNECTION  = 3
      };
      
      /**
       * Create a new ClusterNode for a Node that is connecting to us 
       * that we did not previously know about
       */
      ClusterNode();

      /**
       * Create a ClusterNode with the given attributes
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
      ~ClusterNode();

      /**
       * Display all relevant information about ClusterNode
       */
      void debugDump(int debug_level);

      /**
       * Display Bandwidth statistics for the given SocketStream
       */
      void printStats(int debug_level = 1);

      /**
       * Return the name of the node.
       */
      std::string getName() { return mName; }

      /**
       * Return the hostname of the node.
       */
      std::string getHostname() { return mHostname; }

      /**
       * Return the port number that we are connected to.
       */
      vpr::Uint16 getPort() { return mPort;}

      /**
       * Set the name of the ClusterNode.
       */
      void setName(std::string& name) { mName = name; }
      
      /**
       * Set the hostname of the ClusterNode.
       */
      void setHostname(std::string& host_name) { mHostname = host_name; }
      
      /**
       * Set the port of the ClusterNode.
       */
      void setPort(vpr::Uint16& port) { mPort = port;}
            
      /**
       * Get a pointer to the SocketStream used to communicate with this node.
       */
      vpr::SocketStream* getSockStream() { return mSockStream; }

      /**
       * Set the SocketStream used to communicate with this node.
       */
      void setSockStream(vpr::SocketStream* socket) { mSockStream = socket; }

      /**
       * Return if we are connected to this node.
       */
      bool isConnected() { return (mConnected == CONNECTED); }

      /**
       * Set the current connection state to this machine.
       */
      void setConnected(int connect);

      /**
       * Get the current connection state for this node.
       */
      int getConnected() { return mConnected; }
      
      /**
       * Return if this node is updated or not.
       */
      bool isUpdated() { return mUpdated; }

      /**
       * Set the update status for this node.
       */
      void setUpdated(bool update)
      { mUpdated = update; }
      
      /**
       * Return if this node is running or not.
       */
      bool isRunning() { return mRunning; }

      /**
       * Set the running status for this node.
       */
      void setRunning(bool started)
      { mRunning = started; }

      /**
       * Attempt to connect to this node.
       */
      vpr::ReturnStatus attemptConnect();
      
      /**
       * Update this cluster node
       */
      void update();

      /**
       * Lock a mutex for Reading from the SocketStream
       */
      void lockSockRead()
      { mSockReadLock.acquire(); }
      
      /**
       * Unlock a mutex for Reading from the SocketStream
       */
      void unlockSockRead()
      { mSockReadLock.release(); }
      
      /**
       * Lock a mutex for Writing from the SocketStream
       */
      void lockSockWrite()
      { mSockWriteLock.acquire(); }
      
      /**
       * Unlock a mutex for Writing from the SocketStream
       */
      void unlockSockWrite()
      { mSockWriteLock.release(); }

      /**
       * Start the update thread for this node
       */
      void start();

      /**
       * Control loop for updating this thread
       */
      void controlLoop(void* nullParam);

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
      
      /**
       * Kill the update thread
       */
      void shutdown();

      vpr::Uint64* getDelta()
      {
         return &mDelta;
      }
      
      vpr::ReturnStatus send(Packet* out_packet);
      Packet* recvPacket();
private:
      //vpr::Interval        mDelta;
      //ClusterSync          mClusterSync;
      std::string          mName;               /**< ClusterNode name */
      std::string          mHostname;           /**< Host that it is connected to */
      vpr::Uint16          mPort;               /**< Port that it is connected to */

      vpr::SocketStream*   mSockStream;         /**< Socket used for communication to this node */      
      vpr::Mutex           mSockReadLock;       /**< Lock reading from the SocketStream */
      vpr::Mutex           mSockWriteLock;      /**< Lock writing to the SocketStream */

      vpr::Mutex           mConnectedLock;      /**< Lock the isConnected value */
      int                  mConnected;          /**< States if this node is connected */
      
      vpr::Mutex           mUpdatedLock;        /**< Lock the isUpdated value */
      bool                 mUpdated;            /**< States if this node is updated */
      
      bool                 mPendingConnectionRequest;    /**< States if we have sent a Connetion Request */
      
      std::vector<cluster::Packet*>   mPendingDeviceRequests;       /**< Vector of Pending Device Requests */
      vpr::Mutex                      mPendingDeviceRequestsLock;   /**< Lock the mPendingDeviceRequests list */
      
      vpr::Semaphore    mUpdateTriggerSema;   /**< Semaphore trigger for UserData update  */
      
      vpr::Semaphore    mClusterNodeDoneSema;   /**< Semaphore trigger for completion */
      
      // Used for Update Thread
      vpr::Thread*      mControlThread;         /**< Update thread for this node */
      bool              mThreadActive;          /**< Has the update thread started? */
      
      vpr::Uint64       mDelta;
      bool              mRunning;
   };

} // end namespace gadget

#endif
