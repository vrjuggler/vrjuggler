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
 *************** <auto-copyright.pl END do not edit this line> ***************/


#include <vpr/Perf/ProfileManager.h>
#include <vpr/Perf/ProfileNode.h>
#include <vpr/Util/Debug.h>
#include <sstream>

namespace vpr
{

   ProfileNode::ProfileNode( const char * name, ProfileNode * parent ) :
   mName( name ),
      mTotalCalls( 0 ),
      mTotalTime( 0 ),
      mRecursionCounter( 0 ),
      mParent( parent ),
      mChild( NULL ),
      mSibling( NULL )
   {
      mHistorySize = 1;
      mHistory.resize(mHistorySize);
      mStartTime.secf(0);
      reset();
   }

   ProfileNode::ProfileNode( const char * name, ProfileNode * parent, const unsigned int queue_size) :
   mName( name ),
      mTotalCalls( 0 ),
      mTotalTime( 0 ),
      mRecursionCounter( 0 ),
      mParent( parent ),
      mChild( NULL ),
      mSibling( NULL )
   {
      mHistorySize = queue_size;
      mHistory.resize(mHistorySize);
      mStartTime.secf(0);
      reset();
   }

   ProfileNode::~ProfileNode( void )
   {
      delete mChild;
      delete mSibling;
   }

   ProfileNode* ProfileNode::getSubNode( const char* profileName )
   {
      // Try to find this sub node
      ProfileNode* child = mChild;
      while ( child )
      {
         if ( child->mName == profileName )
         {
            return child;
         }
         child = child->mSibling;
      }

      // We didn't find it, so add it
      ProfileNode* node = new ProfileNode( profileName, this );
      node->mSibling = mChild;
      mChild = node;
      return node;
   }

   ProfileNode* ProfileNode::getSubNode( const char* profileName, const unsigned int queueSize )
   {
      // Try to find this sub node
      ProfileNode* child = mChild;
      while ( child )
      {
         if ( child->mName == profileName )
         {
            return child;
         }
         child = child->mSibling;
      }

      // We didn't find it, so add it
      ProfileNode* node = new ProfileNode( profileName, this, queueSize);
      node->mSibling = mChild;
      mChild = node;
      return node;
   }

   void ProfileNode::printTree(ProfileNode* node)
   {
      if ( node == NULL ) return;

      ProfileNode::printTree(node->getSibling());

      vprDEBUG(vprDBG_ALL, 0) << clrSetBOLD(clrGREEN) << "[PROFILE STATS] " << clrRESET 
         << clrSetBOLD(clrRED) << node->getName() << clrRESET << clrSetBOLD(clrYELLOW)  
      << " total calls: " << clrRESET << node->getTotalCalls()
      << clrSetBOLD(clrYELLOW) << " total time: " << clrRESET << node->getTotalTime()
      << clrSetBOLD(clrYELLOW) << " ave: " << clrRESET 
         << node->getTotalTime() / node->getTotalCalls() << std::endl << vprDEBUG_FLUSH;

      std::stringstream s;
      NodeHistoryRange p = node->getNodeHistoryRange();
      for ( ; p.first != p.second; p.first++ )
      {
         s << *(p.first) << " ";
      }

      vprDEBUG(vprDBG_ALL, 0)  << clrOutBOLD(clrYELLOW, " history: ")
      << s.str() << std::endl << vprDEBUG_FLUSH;

      ProfileNode::printTree(node->getChild());

   }


   void  ProfileNode::reset( void )
   {
      mTotalCalls = 0;
      mTotalTime = 0.0f;

      if ( mChild )
      {
         mChild->reset();
      }

      if ( mSibling )
      {
         mSibling->reset();
      }
   }

   void  ProfileNode::call( void )
   {
      mTotalCalls++;
      if ( mRecursionCounter++ == 0 )
      {
         profileGetTicks(&mStartTime);
      }
   }

   bool  ProfileNode::Return( void )
   {
      if ( --mRecursionCounter == 0 && mTotalCalls != 0 )
      {
         vpr::Interval time;
         profileGetTicks(&time);
         time-=mStartTime;
         mHistory.push_front((float)(time.secf() / profileGetTickRate().secf()));
         mHistory.resize(mHistorySize);
         mTotalTime += mHistory.front();
      }
      return( mRecursionCounter == 0 );
   }

   ProfileNode    ProfileManager::mRoot( "Root", NULL );
   ProfileNode*   ProfileManager::mCurrentNode = &ProfileManager::mRoot;
   int            ProfileManager::mFrameCounter = 0;
   vpr::Interval* ProfileManager::mResetTime = 0;
   vpr::Mutex     ProfileManager::mSampleLock;



} // end vpr namespace


