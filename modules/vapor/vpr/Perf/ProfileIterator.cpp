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

#include <vpr/Perf/ProfileIterator.h>

namespace vpr
{

   ProfileIterator::ProfileIterator( ProfileNode* start )
   {
      mCurrentParent = start;
      mCurrentChild = mCurrentParent->getChild();
   }

   void  ProfileIterator::first(void)
   {
      mCurrentChild = mCurrentParent->getChild();
   }

   void  ProfileIterator::next(void)
   {
      ProfileNode* temp;
      temp = mCurrentParent;
      mCurrentParent = mCurrentChild; 
      mCurrentChild = mCurrentChild->getChild();
      if ( mCurrentChild == NULL )
      {
         mCurrentChild = mCurrentParent;
         mCurrentParent = temp;
         mCurrentChild->getSibling();
      }
   }

   bool  ProfileIterator::isDone(void)
   {
      return mCurrentChild == NULL;
   }

   void  ProfileIterator::enterChild( int index )
   {
      mCurrentChild = mCurrentParent->getChild();
      while ( (mCurrentChild != NULL) && (index != 0) )
      {
         index--;
         mCurrentChild = mCurrentChild->getSibling();
      }

      if ( mCurrentChild != NULL )
      {
         mCurrentParent = mCurrentChild;
         mCurrentChild = mCurrentParent->getChild();
      }
      if ( mCurrentChild == NULL )
      {
         enterParent();
         next();
      }
   }

   void  ProfileIterator::enterParent( void )
   {
      if ( mCurrentParent->getParent() != NULL )
      {
         mCurrentParent = mCurrentParent->getParent();
      }
      mCurrentChild = mCurrentParent->getChild();
   }

   VPR_API(std::ostream&) operator<< (std::ostream& out, ProfileIterator& iter)
   {
      while ( !iter.isDone() )
      {
         out << iter.getCurrentName() << " total calls: " << iter.getCurrentTotalCalls()
         << " total time: " << iter.getCurrentTotalTime() << " ave: " 
            << iter.getCurrentTotalCalls() /iter.getCurrentTotalTime() << " history: ";

         ProfileNode::NodeHistoryRange p = iter.getNodeHistoryRange();
         for ( ; p.first != p.second; p.first++ )
         {
            out << *(p.first) << " ";
         }

         out << std::endl;
         iter.next();
      }

      return out;
   }

} // end vpr namespace
