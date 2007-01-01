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
#ifndef VPR_PROFILE_ITERATOR_H
#define VPR_PROFILE_ITERATOR_H

#include <vpr/vprConfig.h>

#include <vpr/Perf/ProfileNode.h>

namespace vpr
{

   /** \class ProfileIterator ProfileIterator.h vpr/Perf/ProfileIterator.h
    *
    * An iterator to navigate through the Profile tree.
    */
   class VPR_CLASS_API ProfileIterator
   {
   public:
      /** Constructor.  Returns iterator starting at "start" node. */
      ProfileIterator(ProfileNode* start);

      /** Copy constructor. */
      ProfileIterator(const ProfileIterator& rhs)
      {
         mCurrentParent = rhs.mCurrentParent;
         mCurrentNode = rhs.mCurrentNode;
         mDepth = rhs.mDepth;
      }

      /** Goes to the next element. */
      void operator++();

      ProfileNode& operator*()
      {
         return *mCurrentNode;
      }

      ProfileNode* operator->()
      {
         return mCurrentNode;
      }

      /** Returns the current depth in the traversal. */
      unsigned int depth()
      {
         return mDepth;
      }

      bool operator==(const ProfileIterator& rhs)
      {
         return ( (rhs.mCurrentParent == mCurrentParent) &&
                  (rhs.mCurrentNode == mCurrentNode) &&
                  (mDepth == rhs.mDepth) );
      }

      bool operator!=(const ProfileIterator& rhs)
      {
         return !(operator==(rhs));
      }

   protected:
      ProfileNode*   mCurrentParent;   /**< The current parent of the node we are holding. */
      ProfileNode*   mCurrentNode;     /**< The current node we are holding on to. */

      unsigned int   mDepth;           /**< The current depth in the tree. */

      friend class ProfileManager;
      friend class ProfileNode;
   };

   /**
    * Overloaded output operator for outputting the current statistics.
    */
   VPR_API(std::ostream&) operator<< (std::ostream& out, ProfileIterator& iter);

} // end vpr namespace

#endif
