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


#ifndef VPR_PROFILE_NODE_H
#define VPR_PROFILE_NODE_H

/**
 * mostly taken from:
 * Real-Time Hierarchical Profiling for Game Programming Gems 3
 * by Greg Hjelstrom & Byon Garrabrant
 */

#include <vpr/vprConfig.h>
#include <deque>
#include <map>
#include <vpr/Util/Interval.h>
#include <vpr/vprTypes.h>

namespace vpr
{

   inline void profileGetTicks(vpr::Interval* ticks)
   {
      ticks->setNowReal();
   }

   inline vpr::Interval profileGetTickRate(void)
   {
      vpr::Interval i;
      i.secf(0.01f);
      return i;
   }


/**
 * A node in the Profile Hierarchy Tree
 */
   class VPR_CLASS_API ProfileNode
   {

   public:

      /** 
       * constructor for a profile node
       * Takes a static string pointer for the name and a reference to a parent
       */
      ProfileNode( const char * name, ProfileNode * parent );

      ProfileNode( const char * name, ProfileNode * parent, const unsigned int queueSize);

      /** 
       * destructor
       */
      ~ProfileNode( void );

      /**
       * Returns a pointer to a subnode of this node given the name of the subnode
       * If the name doesn't exist it creates the new node and adds it as a child
       * to this node and returns this new node.
       */
      ProfileNode*  getSubNode( const char * name );

      ProfileNode*  getSubNode( const char * name, const unsigned int queueSize);

      /**
       * return This nodes parent.
       */
      ProfileNode*  getParent( void )
      {
         return mParent;
      }

      /**
       * @return A pointer to a sibling node.
       */
      ProfileNode*  getSibling( void )
      {
         return mSibling;
      }

      /**
       * @returns A pointer to its child.
       */
      ProfileNode*  getChild( void )
      {
         return mChild;
      }

      void           printTree(ProfileNode* node);
      void           reset( void );
      void           call( void );
      bool           Return( void );

      const char*    getName( void )
      {
         return mName;
      }

      int            getTotalCalls( void )
      {
         return mTotalCalls;
      }

      float          getTotalTime( void )
      {
         return mTotalTime;
      }

      typedef std::pair< std::deque<float>::const_iterator, std::deque<float>::const_iterator> NodeHistoryRange;

      const NodeHistoryRange getNodeHistoryRange()
      {
         return std::make_pair(mHistory.begin(), mHistory.end());
      }

      void           clearHistory()
      {
         mHistory.clear();
      }


   protected:

      const char*    mName;
      int            mTotalCalls;
      float          mTotalTime;

      std::deque<float> mHistory;
      unsigned int      mHistorySize;

      vpr::Interval  mStartTime;
      int            mRecursionCounter;

      ProfileNode*   mParent;
      ProfileNode*   mChild;
      ProfileNode*   mSibling;
   };

} // end namespace vpr

#endif
