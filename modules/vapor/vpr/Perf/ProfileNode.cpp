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
#include <vpr/Sync/Guard.h>
#include <sstream>

namespace vpr
{

   ProfileNode::ProfileNode( const char * name, ProfileNode * parent ) :
   mName( name ),
      mTotalCalls( 0 ),
      mRecursionCounter( 0 ),
      mParent( parent ),
      mChild( NULL ),
      mSibling( NULL )
   {
      mTotalTime.secf(0.0f);
      mHistorySize = 1;
      mHistory.resize(mHistorySize);
      mStartTime.secf(0);
      reset();
   }

   ProfileNode::ProfileNode( const char * name, ProfileNode * parent, const unsigned int queue_size) :
   mName( name ),
      mTotalCalls( 0 ),
      mRecursionCounter( 0 ),
      mParent( parent ),
      mChild( NULL ),
      mSibling( NULL )
   {
      mTotalTime.secf(0.0f);
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

   ProfileNode* ProfileNode::getNamedNode( const char* nodeName )
   {
      // Try to find this sub node
      ProfileNode* child = mChild;
      while ( child )
      {
         if ( child->mName == nodeName )
         {
            return child;
         }
         child = child->mSibling;
      }

      return NULL;
   }



   void ProfileNode::printTree(ProfileNode* node)
   {
      if ( node == NULL )
      {  return; }

      ProfileNode::printTree(node->getSibling());

      mNodeLock.acquire();
      vprDEBUG(vprDBG_ALL, 0) << clrSetBOLD(clrGREEN) << "[PROFILE STATS] " << clrRESET
         << clrSetBOLD(clrRED) << node->getName() << clrRESET
         << clrSetBOLD(clrYELLOW) << " total calls: " << clrRESET << node->getTotalCalls()
         << clrSetBOLD(clrYELLOW) << " total time: " << clrRESET << node->getTotalTime().msecf()
         << clrSetBOLD(clrYELLOW) << " ave: " << clrRESET
         << node->getTotalTime().msecf() / node->getTotalCalls() << std::endl << vprDEBUG_FLUSH;

      std::stringstream s;
      NodeHistoryRange p = node->getNodeHistoryRange();
      for ( ; p.first != p.second; p.first++ )
      {
         s << p.first->msecf() << " ";
      }

      vprDEBUG(vprDBG_ALL, 0)  << clrOutBOLD(clrYELLOW, " history: ")
      << s.str() << std::endl << vprDEBUG_FLUSH;
      mNodeLock.release();

      ProfileNode::printTree(node->getChild());

   }


   void  ProfileNode::reset( void )
   {
   Guard<Mutex> guard(mNodeLock);
      mTotalCalls = 0;
      mTotalTime.secf(0.0f);

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
   Guard<Mutex> guard(mNodeLock);
      mTotalCalls++;
      if ( mRecursionCounter++ == 0 )
      {
         profileGetTicks(&mStartTime);
      }
   }

   bool  ProfileNode::Return( void )
   {
   Guard<Mutex> guard(mNodeLock);
      if ( --mRecursionCounter == 0 && mTotalCalls != 0 )
      {
         vpr::Interval time;
         profileGetTicks(&time);
         time -= mStartTime;
         mHistory.push_front(time);
         mHistory.resize(mHistorySize);
         mTotalTime += time;
      }

      return( mRecursionCounter == 0 );
   }

   vpr::Interval ProfileNode::getSTA()
   {
      vpr::Interval sta_interval;
      for(std::deque<vpr::Interval>::iterator i=mHistory.begin(); i!=mHistory.end(); ++i)
      { sta_interval += *i; }
      sta_interval.set(sta_interval.getBaseVal()/mHistory.size(), vpr::Interval::Base);
      return sta_interval;
   }


} // end vpr namespace


