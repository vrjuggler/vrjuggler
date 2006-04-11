/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/


#include <vpr/Perf/ProfileManager.h>
#include <iostream>

namespace vpr
{

// Initialize statics
TSObjectProxy<ProfileManager::ThreadProfileData>  ProfileManager::mThreadData;

   void  ProfileManager::startProfile( const char * profileName , const unsigned int queueSize)
   {
      ThreadProfileData& prof_data(*mThreadData);

      if ( profileName != prof_data.mCurrentNode->getName() )
      {
         prof_data.mCurrentNode = prof_data.mCurrentNode->getSubNode( profileName, queueSize);
      }
      prof_data.mCurrentNode->startSample();
   }

   void  ProfileManager::stopProfile( void )
   {
      ThreadProfileData& prof_data(*mThreadData);

      // Return will indicate whether we should back up to our parent (we may
      // be profiling a recursive function)
      if ( prof_data.mCurrentNode->stopSample() )
      {
         prof_data.mCurrentNode = prof_data.mCurrentNode->getParent();
      }
   }

   ProfileNode* ProfileManager::getRootNode(vpr::Thread* thread)
   {
      if (NULL == thread)
      { thread = vpr::Thread::self();}
      ThreadProfileData* prof_data = mThreadData.getObjPtrForThread(thread);
      return &(prof_data->mRoot);
   }

   void ProfileManager::printTree(bool forAllThreads)
   {
      if(!forAllThreads)
      {
         getRootNode()->printTree();
      }
      else
      {
         std::vector<Thread*>::size_type num_threads =
            vpr::ThreadManager::instance()->getNumThreads();
         for(std::vector<Thread*>::size_type t = 0; t < num_threads; ++t)
         {
            vpr::Thread* thread = vpr::ThreadManager::instance()->getThread(t);
            std::cout << "Print thread: " << (void*)thread << std::endl;
            std::cout << "Thread: " << thread << std::endl;
            ProfileNode* root_node = getRootNode(thread);
            root_node->printTree();
         }

         /*
         std::vector<vpr::Thread*> cur_threads = vpr::ThreadManager::instance()->getThreads();

         for(unsigned t=0;t<cur_threads.size();t++)
         {
            std::cout << "Print thread: " << (void*)cur_threads[t] << std::endl;
            std::cout << "Thread: " << cur_threads[t] << std::endl;
            getRootNode(cur_threads[t])->printTree();
         }
         */
      }
   }


   void  ProfileManager::reset( void )
   {
      ThreadProfileData& prof_data(*mThreadData);

      prof_data.mRoot.reset();
      prof_data.mResetTime.setNow();
   }

   float ProfileManager::getTimeSinceReset( void )
   {
      ThreadProfileData& prof_data(*mThreadData);

      vpr::Interval time;
      time.setNow();
      time = time - prof_data.mResetTime;
      return time.secf();
   }

} // end namespace vpr
