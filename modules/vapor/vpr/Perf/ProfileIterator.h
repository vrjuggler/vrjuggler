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


#ifndef VPR_PROFILE_ITERATOR_H
#define VPR_PROFILE_ITERATOR_H

/**
 * mostly taken from:
 * Real-Time Hierarchical Profiling for Game Programming Gems 3
 * by Greg Hjelstrom & Byon Garrabrant
 */

#include <vpr/Perf/ProfileNode.h>

namespace vpr
{

/**
 * An iterator to navigate through the Profile tree
 */
   class ProfileIterator
   {
   public:

      /**
       * Access the first child of the parent.
       */
      void           first(void);

      /**
       * Access the next child of the parent.
       */
      void           next(void);

      ProfileIterator*   getChild(void)
      {
         return new ProfileIterator(mCurrentChild->getChild());
      }

      ProfileIterator*   getSibling(void)
      {
         return new ProfileIterator(mCurrentChild->getSibling());
      }

      /**
       * Tells if the there are no more children to iterate through.
       *
       * @return true is returned if the next child is NULL.
       *         false is returned if there is a valid next child.
       */
      bool           isDone(void);

      /**
       * Make the given child the new parent.
       */
      void           enterChild( int index );

      /**
       * Make the largest child the new parent.
       */
      void           enterLargestChild( void );

      /**
       * Make the current parent's parent the new parent.
       */
      void           enterParent( void ); 

      /**
       * @return Current childs name is returned.
       */
      const char*    getCurrentName( void )
      {
         return mCurrentChild->getName();
      }

      /**
       * @return Current childs number of total calls is returned.
       */
      int            getCurrentTotalCalls( void )
      {
         return mCurrentChild->getTotalCalls();
      }

      /**
       * @return Current childs total exectuion time is returned.
       */
      float          getCurrentTotalTime( void )
      {
         return mCurrentChild->getTotalTime();
      }

      /**
       * @return Current child's parent name is returned.
       */
      const char* getCurrentParentName( void )
      {
         return mCurrentParent->getName();
      }

      /**
       * @return tree structure is printed out
       */
      void printTree(ProfileNode* node)
      {
         node->printTree(node);
      }

      /*
       * @return The current size of history it is keeping track of
       */
      const ProfileNode::NodeHistoryRange getNodeHistoryRange()
      {
         return mCurrentChild->getNodeHistoryRange();
      }

      /**
       * @return Current child's parent number of total calls is returned.
       */
      int            getCurrentParentTotalCalls( void )
      {
         return mCurrentParent->getTotalCalls();
      }

      /**
       * @return Current child's parent total execution time is returned.
       */
      float          getCurrentParentTotalTime( void )
      {
         return mCurrentParent->getTotalTime();
      }

   protected:

      ProfileNode*   mCurrentParent;
      ProfileNode*   mCurrentChild;

      ProfileIterator( ProfileNode* start );
      friend   class    ProfileManager;
   };

/**
 * Overloaded output operator for outputting the current statistics
 */
   VPR_API(std::ostream&) operator<< (std::ostream& out, ProfileIterator& iter);

} // end vpr namespace

#endif
