#include <cluster/ClusterNode.h>
#include <cluster/ClusterException.h>

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
         catch(cluster::ClusterException cluster_exception)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED)
               << cluster_exception.getMessage() << clrRESET
               << std::endl << vprDEBUG_FLUSH;
            
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
