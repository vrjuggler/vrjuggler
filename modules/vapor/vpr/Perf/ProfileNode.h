/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

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

#ifndef VPR_PROFILE_NODE_H
#define VPR_PROFILE_NODE_H

/**
 * \file
 *
 * Primarily based on
 * Real-Time Hierarchical Profiling for Game Programming Gems 3
 * by Greg Hjelstrom & Byon Garrabrant
 */

#include <vpr/vprConfig.h>
#include <deque>
#include <vpr/Util/Interval.h>
#include <vpr/vprTypes.h>
#include <vpr/Sync/Mutex.h>

namespace vpr
{

   /** \class ProfileNode ProfileNode.h vpr/Perf/ProfileNode.h
    *
    * A node in the Profile Hierarchy Tree.
    *
    * This is the main class for performance profiling.
    * The performance profile is built out of a tree of these nodes.  They are
    * connected in a standard parent child relationship.  Each node can have
    * one parent and 0 or more children.
    *
    * @note This class is not multi-thread safe.  It relies upon only one
    *       thread using it at a time.  This really only makes sense since
    *       two threads accessing it at the same time would lead to invalid
    *       data.
    */
   class VPR_CLASS_API ProfileNode
   {
   public:
      /**
       * Constructor for a profile node.
       * Takes a static string pointer for the name and a reference to a
       * parent.  Parent and children default to NULL.
       *
       * @param name      Static string pointer and name for this node.
       * @param queueSize Queue size.
       */
      ProfileNode(const char* name, const unsigned int queueSize = 0);

      /**
       * Destructor.
       */
      ~ProfileNode();

      /**
       * Adds a new node as a child.
       *
       * @param newChild New child to add.  It will have its parent set to us.
       */
      void addChild(ProfileNode* newChild);

      /**
       * Returns pointer to sub-node (child) with the given name.
       *
       * @note Currently requires the char* to be same as used to create.
       */
      ProfileNode* getChild(const char* nodeName);

      /**
       * Finds a child using string name instead of pointer comparison.
       */
      ProfileNode* getNamedChild(const char* nodeName);

      /**
       * Returns named node or creates new child.
       * Returns a pointer to a subnode of this node given the name of the
       * subnode.  If the name doesn't exist it creates the new node and adds
       * it as a child to this node and returns this new node.
       */
      ProfileNode* getSubNode(const char* name,
                              const unsigned int queueSize = 0);

      /** Gets the static string name associated with this node. */
      const char* getName()
      {
         return mName;
      }

      /**
       * Returns this node's parent.
       */
      ProfileNode* getParent()
      {
         return mParent;
      }

      /**
       * Returns a pointer to next sibling node in line.
       */
      ProfileNode* getSibling()
      {
         return mSibling;
      }

      /**
       * Returns a pointer to its child.
       */
      ProfileNode* getChild()
      {
         return mChild;
      }

      /**
       * Prints tree rooted at this node.
       *
       * @param depth Depth in the traversal.  Used for indentation and the
       *              like.
       */
      void printTree(const unsigned int depth = 0);

      /** Gets an XML representation of the profile hierarchy. */
      std::string getXMLRep();

      /**
       * Recursively resets the metric values for all nodes rooted here.
       * Resets total calls and total times.  Also resets the history.
       */
      void reset();

      /**
       * Starts a sampling period for this profile node.
       * Starts the time running for us.
       */
      void startSample();

      /** Stops the sampling period for this profile node. */
      bool stopSample();

      /** @name Metric getters */
      //@{
      /** Returns the last sample taken. */
      vpr::Interval getLastSample()
      {
         return mLastSample;
      }

      /** Returns the total number of samples made on this node. */
      unsigned int getTotalCalls()
      {
         return mTotalCalls;
      }

      /** Returns the total sampled time for this node. */
      vpr::Interval getTotalTime()
      {
         return mTotalTime;
      }

      typedef std::pair<std::deque<vpr::Interval>::const_iterator,
                        std::deque<vpr::Interval>::const_iterator> NodeHistoryRange;

      const NodeHistoryRange getNodeHistoryRange()
      {
         return std::make_pair(mHistory.begin(), mHistory.end());
      }

      /**
       * Gets the average time sample.
       * Returns total time sampled/total calls.
       */
      vpr::Interval getAverage();

      /**
       * Gets the short term average.
       * Computed as the average of the history.
       */
      vpr::Interval getSTA();
      //@}

   protected:
      /** Helper for building up XML representation recursively. */
      void getXMLRep(std::stringstream& s, unsigned int depth = 0);

   protected:
      const char*    mName;         /**< Pointer to the name for this node.  Must be a static string. */
      unsigned int   mTotalCalls;   /**< Total number of times called since last reset. */
      vpr::Interval  mTotalTime;    /**< Total summed time over mTotalCalls. */
      vpr::Interval  mLastSample;   /**< The last sample taken. */

      std::deque<vpr::Interval>  mHistory;         /**< History of samples. */
      unsigned int               mMaxHistorySize;  /**< Max size allowed for history. If 0, then ignore history. */

      vpr::Interval  mStartTime;       /**< The time that this sample started. */
      int            mRecursionCounter;/**< The number of calls without a return. tracks recursion. */

      ProfileNode*   mParent;       /**< Parent of this node. */
      ProfileNode*   mChild;        /**< Direct child of this node. (first node in child list) */
      ProfileNode*   mSibling;      /**< Next node in linked list of children. */

      // XXX: Do we really need to use linked list.  Why not just child vector?
   };

} // end namespace vpr

#endif
