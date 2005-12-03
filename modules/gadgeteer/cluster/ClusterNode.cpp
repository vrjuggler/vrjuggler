/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <cluster/ClusterException.h>
#include <cluster/ClusterNode.h>


namespace cluster
{

void ClusterNode::controlLoop(void* nullParam)
{
   // - Block on an update call
   // - Update Local Data
   // - Send
   // - Signal Sync

   boost::ignore_unused_variable_warning(nullParam);
   
   while( mRunning )
   {
      // Wait for trigger
      if( mRunning )
      {
         mUpdateTriggerSema.acquire();
      }
         
      mUpdated = false;
      while ( mRunning && !mUpdated )
      {
         try
         {
            update();
         }
         catch(cluster::ClusterException& cluster_exception)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrRED, "ERROR") << ": "
               << cluster_exception.what() << std::endl << vprDEBUG_FLUSH;
            
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) <<
               "ClusterNode::update() We have lost our connection to: " << getName() << ":" << getPort()
               << std::endl << vprDEBUG_FLUSH;

            debugDump(vprDBG_CONFIG_LVL);
            
            // Set the ClusterNode as disconnected since we have lost the connection
            setStatus(DISCONNECTED);
            
            // Shut down manually instead of calling shutdown since
            // we are in the control thread.
            mRunning = false;
            //if (NULL != mSockStream)
            //{
               //if(mSockStream->isOpen())
               //{
               //   mSockStream->close();
               //}
               //delete mSockStream;
               //mSockStream = NULL;
            //}
         }
      }
      
      // Signal done with Update
      mNodeDoneSema.release();
   }
   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "ClusterNode: " << getName() << " is stopping."
                                             << std::endl << vprDEBUG_FLUSH;
}

void ClusterNode::signalUpdate()
{
   while(!mThreadActive)
   {
      vprDEBUG(gadgetDBG_RIM, vprDBG_HVERB_LVL) << "Waiting for thread to start ClusterNode::start().\n" << vprDEBUG_FLUSH;
      vpr::Thread::yield();
   }
   //vprDEBUG(gadgetDBG_RIM,/*vprDBG_HVERB_LVL*/1) << getName() << "Signaling ClusterNode\n" << vprDEBUG_FLUSH;
   mUpdateTriggerSema.release();
}

/**
 * Blocks until the end of the frame.
 * @post The frame has been drawn.
 */
void ClusterNode::sync()
{
   vprASSERT(mThreadActive == true);
   mNodeDoneSema.acquire();
}

}
