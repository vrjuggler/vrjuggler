/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _CLUSTER_APPLICATION_BARRIER_H
#define _CLUSTER_APPLICATION_BARRIER_H

#include <cluster/PluginConfig.h>
#include <vpr/IO/SerializableObject.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>
#include <vpr/Sync/CondVar.h>

#include <cluster/ClusterManager.h>
#include <cluster/ClusterPlugin.h>
#include <gadget/Util/Debug.h>
#include <plugins/ApplicationBarrierManager/ApplicationBarrierManager.h>

namespace cluster
{

class ApplicationBarrier : public vpr::SerializableObject
{
public:
   /**
    * Construct a new ApplicationBarrier object.
    */
   ApplicationBarrier()
   {;}

   /**
    * Initialize the application barrier for the given GUID.
    *
    * @param guid      The GUID used to reference this object.
    */
   void init(const vpr::GUID& guid)
   {
      mId = guid;

      ClusterPluginPtr app_barrier_mgr =
         ClusterManager::instance()->getPluginByGUID(vpr::GUID("5edddf06-cf73-43ad-928e-318c3e67815e"));

      if (NULL != app_barrier_mgr.get())
      {
         app_barrier_mgr->addSerializableObject(this);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Can not register ApplicationBarrier with non-existent "
            << "Application Barrier Manager." << std::endl << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_RIM, vprDBG_WARNING_LVL)
            << "In order to synchronize ApplicationBarrier across a cluster, "
            << "you must load the ApplicationBarrierManager cluster plug-in."
            << std::endl << vprDEBUG_FLUSH;
      }
   }

   virtual ~ApplicationBarrier()
   {
      ;
   }

   /**
    * Wait until all nodes are waiting.
    */
   void wait()
   {
      boost::shared_ptr<ApplicationBarrierManager> app_barrier_mgr =
         boost::static_pointer_cast<ApplicationBarrierManager>(ClusterManager::instance()->getPluginByGUID(vpr::GUID("5edddf06-cf73-43ad-928e-318c3e67815e")));

      // If plugin isn't loaded, don't wait - user was warned on init
      if (NULL == app_barrier_mgr.get())
      {
         return;
      }

      // Get total number of nodes in cluster
      mTotalNodes = ClusterManager::instance()->getNetwork()->getNumNodes();

      // Notify peers that this node is waiting
      app_barrier_mgr->sendWait(mId);

      // Wait until all nodes are ready
      mWaitCond.acquire();
      while(mNodesWaiting != mTotalNodes )
      {
         mWaitCond.wait();
      }
      mNodesWaiting = 0;
      mWaitCond.release();

      vprDEBUG(gadgetDBG_RIM, vprDBG_HVERB_LVL)
         << clrOutBOLD(clrMAGENTA,"[ApplicationBarrier::wait()]")
         << "Completed.\n" << vprDEBUG_FLUSH;
   }

   /**
    * Signal waiting thread to proceed.
    */
   void incWaitingNodes()
   {
      mNodesWaiting++;

      vprDEBUG(gadgetDBG_RIM, vprDBG_HVERB_LVL)
         << clrOutBOLD(clrMAGENTA,"[ApplicationBarrier::incWaitingNodes()]")
         << "Nodes Waiting: " << mNodesWaiting << ".\n" << vprDEBUG_FLUSH;


      if (mNodesWaiting == mTotalNodes)
      {
         mWaitCond.acquire();
         {
            vprDEBUG(gadgetDBG_RIM, vprDBG_HVERB_LVL)
               << clrOutBOLD(clrMAGENTA,"[ApplicationBarrier::incWaitingNodes()]")
               << "All nodes updated.\n" << vprDEBUG_FLUSH;
            mWaitCond.signal();
         }
         mWaitCond.release();
      }
   }

   /**
    * Return the GUID for this object.
    */
   const vpr::GUID& getId() const
   {
      return mId;
   }

public:

   virtual void readObject(vpr::ObjectReader* reader)
   {
      // Read GUID
      mId.readObject(reader);
   }

   virtual void writeObject(vpr::ObjectWriter* writer)
   {
      // Write GUID
      mId.writeObject(writer);
   }

private:
   vpr::GUID   mId;        /**< GUID for this object */
   vpr::CondVar mWaitCond; /**< Conditional wait */
   size_t mTotalNodes;     /**< Number of nodes in cluster */
   size_t mNodesWaiting;   /**< Number of nodes waiting. */
};

} // end namespace gadget

#endif
