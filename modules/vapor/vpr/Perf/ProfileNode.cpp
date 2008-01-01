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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

   ProfileNode::ProfileNode(const char* name, const unsigned int queue_size)
      : mName(name)
      , mTotalCalls(0)
      , mRecursionCounter(0)
      , mParent(NULL)
      , mChild(NULL)
      , mSibling(NULL)
   {
      mTotalTime.secf(0.0f);
      mMaxHistorySize = queue_size;
      mStartTime.secf(0);
      reset();
   }

   ProfileNode::~ProfileNode()
   {
      delete mChild;
      delete mSibling;
   }

   // - Set parent to us
   // - If we don't have a child yet, set that
   // - If we do, then add to end of sibling list
   void ProfileNode::addChild(ProfileNode* newNode)
   {
      vprASSERT(NULL != newNode);
      newNode->mParent = this;

      if(NULL == mChild)
      {
         mChild = newNode;
         newNode->mSibling = NULL;
      }
      else
      {
         ProfileNode* last_child = mChild;
         while ( last_child->mSibling )   // Iterate down the list
         {
            last_child = last_child->mSibling;
         }
         last_child->mSibling = newNode;
         newNode->mSibling = NULL;
      }
   }

   ProfileNode* ProfileNode::getChild(const char* nodeName)
   {
      // Try to find this sub node by iterating through children
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

   ProfileNode* ProfileNode::getNamedChild(const char* nodeName)
   {
      std::string needed_name(nodeName);

      // Try to find this sub node by iterating through children
      ProfileNode* child = mChild;
      while ( child )
      {
         std::string cur_name(child->mName);
         if(needed_name == cur_name)
         {
            return child;
         }
         child = child->mSibling;
      }

      return NULL;
   }

   ProfileNode* ProfileNode::getSubNode(const char* profileName,
                                        const unsigned int queueSize)
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

      // We didn't find it, so add it at end
      ProfileNode* node = new ProfileNode( profileName, queueSize);
      addChild(node);
      return node;
   }

   // Print us, then our children, then our siblings
   void ProfileNode::printTree(unsigned int depth)
   {
      std::string indent_str(depth * 3, ' ');
      if ( 0 == depth )
      {
         vprDEBUG(vprDBG_ALL, 0) << clrSetBOLD(clrGREEN)
            << "--- [PROFILE STATS] ---" << clrRESET << std::endl
            << vprDEBUG_FLUSH;
      }

      vprDEBUG(vprDBG_ALL, 0) << indent_str
         << clrSetBOLD(clrRED) << getName() << clrRESET
         << clrSetBOLD(clrYELLOW) << " total calls: " << clrRESET << getTotalCalls()
         << clrSetBOLD(clrYELLOW) << " total time: " << clrRESET << getTotalTime().msecf()
         << clrSetBOLD(clrYELLOW) << " avg: " << clrRESET
         << getAverage().msecf() << std::endl << vprDEBUG_FLUSH;

      std::stringstream s;
      NodeHistoryRange p = getNodeHistoryRange();
      for ( ; p.first != p.second; p.first++ )
      {
         s << p.first->msecf() << " ";
      }

      vprDEBUG(vprDBG_ALL, 0)  << indent_str << "  "
         << clrOutBOLD(clrYELLOW, " history: ")
         << s.str() << std::endl << vprDEBUG_FLUSH;

      if ( getChild() != NULL )
      {
         getChild()->printTree(depth+1);
      }

      if ( getSibling() != NULL )
      {
         getSibling()->printTree(depth);
      }
   }

   // Just make use of the helper method
   std::string ProfileNode::getXMLRep()
   {
      std::stringstream s;
      getXMLRep(s,0);
      return s.str();
   }

   // Recursively build up simple xml rep of this stuff.
   void ProfileNode::getXMLRep(std::stringstream& s, const unsigned int depth)
   {
      std::string indent_str(depth*3,' ');

      s << indent_str
        << "<profile_node name=\"" << getName()
        << "\" total_calls=\"" << getTotalCalls()
        << "\" total_time=\"" << getTotalTime().msecf()
        << "\" average=\"" << getAverage().msecf()
        << "\" sta=\"" << getSTA().msecf()
        << "\">\n";

      NodeHistoryRange p = getNodeHistoryRange();
      if ( p.first != p.second )
      {
         s << indent_str << "  ";
         for ( ; p.first != p.second; p.first++ )
         {
            s << p.first->msecf() << " ";
         }
         s << std::endl;
      }

      std::string child_xml_rep, sibling_xml_rep;

      if ( getChild() != NULL )
      {
         getChild()->getXMLRep(s, depth + 1);
      }
      s << indent_str << "</profile_node>\n";

      if ( getSibling() != NULL )
      {
         getSibling()->getXMLRep(s, depth);
      }
   }

   void ProfileNode::reset()
   {
      mTotalCalls = 0;
      mTotalTime.set(0, vpr::Interval::Base);
      mLastSample.set(0, vpr::Interval::Base);
      mHistory.clear();

      if ( mChild )
      {
         mChild->reset();
      }

      if ( mSibling )
      {
         mSibling->reset();
      }
   }

   void ProfileNode::startSample()
   {
      mTotalCalls++;
      if ( mRecursionCounter++ == 0 )
      {
         mStartTime.setNow();
      }
   }

   bool ProfileNode::stopSample()
   {
      if ( --mRecursionCounter == 0 && mTotalCalls != 0 )
      {
         mLastSample.setNow();
         mLastSample -= mStartTime;
         if ( 0 != mMaxHistorySize )
         {
            mHistory.push_front(mLastSample);
            if ( mHistory.size() > mMaxHistorySize )
            {
               mHistory.resize(mMaxHistorySize);
            }
         }
         mTotalTime += mLastSample;
      }

      return( mRecursionCounter == 0 );
   }

   vpr::Interval ProfileNode::getAverage()
   {
      if ( getTotalCalls() == 0 )
      {
         return vpr::Interval();
      }
      else
      {
         return vpr::Interval(getTotalTime().getBaseVal() / mTotalCalls,
                              vpr::Interval::Base);
      }
   }

   vpr::Interval ProfileNode::getSTA()
   {
      // If we are making no history or have no history yet
      if ( 0 == mMaxHistorySize || mHistory.empty() )
      {
         return mLastSample;
      }
      else
      {
         vpr::Interval sta_interval;
         std::deque<vpr::Interval>::iterator i;
         for ( i = mHistory.begin(); i != mHistory.end(); ++i )
         {
            sta_interval += *i;
         }
         sta_interval.set(sta_interval.getBaseVal() / mHistory.size(),
                          vpr::Interval::Base);
         return sta_interval;
      }
   }

} // end vpr namespace
