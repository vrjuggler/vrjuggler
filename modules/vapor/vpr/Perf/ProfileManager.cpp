#include <vpr/Perf/ProfileManager.h>
#include <iostream>

namespace vpr
{


   void  ProfileManager::startProfile( const char * name )
   {
      if ( name != mCurrentNode->getName() )
      {
         mCurrentNode = mCurrentNode->getSubNode( name );
      }

      mCurrentNode->call();
   }

   void  ProfileManager::startProfile( const char * name , const unsigned int queue_size)
   {
      if ( name != mCurrentNode->getName() )
      {
         mCurrentNode = mCurrentNode->getSubNode( name, queue_size);
      }

      mCurrentNode->call();
   }

   void  ProfileManager::stopProfile( void )
   {
      // Return will indicate whether we should back up to our parent (we may
      // be profiling a recursive function)
      if ( mCurrentNode->Return() )
      {
         mCurrentNode = mCurrentNode->getParent();
      }
   }

   void  ProfileManager::reset( void )
   {
      mRoot.reset(); 
      mFrameCounter = 0;
      profileGetTicks(mResetTime);
   }

   void ProfileManager::incrementFrameCounter( void )
   {
      mFrameCounter++;
   }

   float ProfileManager::getTimeSinceReset( void )
   {
      vpr::Interval time;
      profileGetTicks(&time);
      time = time - *mResetTime;
      return(float)(time.secf() / profileGetTickRate().secf());
   }

} // end namespace vpr
