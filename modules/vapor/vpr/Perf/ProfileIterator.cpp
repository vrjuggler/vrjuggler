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
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/Perf/ProfileIterator.h>

namespace vpr
{

   ProfileIterator::ProfileIterator( ProfileNode* start )
      : mCurrentParent(NULL)
      , mCurrentNode(NULL)
      , mDepth(0)
   {
      mCurrentNode = start;
      if(NULL == mCurrentNode)      // NULL iterator
      {
         mCurrentParent = NULL;
      }
      else
      {
         mCurrentParent = mCurrentNode->getParent();
      }
   }

   void ProfileIterator::operator++()
   {
      // If can decend, do so, else ascend until have a sibling.
      // If no sibling found, then set both to NULL (end iterator)
      if(mCurrentNode->getChild() != NULL)      // If can descend, do so
      {
         mCurrentParent = mCurrentNode;
         mCurrentNode = mCurrentParent->getChild();
         mDepth++;
      }
      else if(mCurrentNode->getSibling() != NULL)  // No children, try sibling
      {
         mCurrentNode = mCurrentNode->getSibling();
         vprASSERT(mCurrentNode->getParent() == mCurrentParent);
      }
      else     // No siblings, so search up for parental sibling
      {
         ProfileNode* cur_parent = mCurrentParent;
         --mDepth;
         while( (cur_parent != NULL) &&
                (cur_parent->getSibling() == NULL) )   // While no siblings and not at root
         {
            cur_parent = cur_parent->getParent();
            --mDepth;
         }

         if(mDepth > 10000)  // If overflow
         {
            mDepth = 0;
         }

         if(NULL == cur_parent)     // At end of iteration
         {
            mCurrentNode = NULL;
            mCurrentParent = NULL;
            mDepth = 0;
         }
         else
         {
            mCurrentNode = cur_parent->getSibling();
            mCurrentParent = mCurrentNode->getParent();
         }
      }
   }

   std::ostream& operator<< (std::ostream& out, ProfileIterator& iter)
   {
      while ( iter != ProfileIterator(NULL))
      {
         out << (*iter).getName() << " total calls: " << (*iter).getTotalCalls()
             << " total time: " << (*iter).getTotalTime().msecf()
             << " avg: " << (*iter).getAverage().msecf()
             << " history: ";

         ProfileNode::NodeHistoryRange p = (*iter).getNodeHistoryRange();
         for ( ; p.first != p.second; p.first++ )
         {
            out << p.first->msecf() << " ";
         }

         out << std::endl;
         ++iter;
      }

      return out;
   }

} // end vpr namespace
