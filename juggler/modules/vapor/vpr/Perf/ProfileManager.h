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


#ifndef VPR_PROFILE_MANAGER_H
#define VPR_PROFILE_MANAGER_H

/**
 * Primarily based on
 * Real-Time Hierarchical Profiling for Game Programming Gems 3
 * by Greg Hjelstrom & Byon Garrabrant
 */

#include <vpr/vprConfig.h>

#include <string>
#include <vector>

#include <vpr/Sync/Guard.h>
#include <vpr/Sync/Mutex.h>

#include <vpr/Perf/ProfileIterator.h>
#include <vpr/Perf/ProfileNode.h>

namespace vpr
{

/*
 * The Manager for the Profile system
 */
   class VPR_CLASS_API ProfileManager
   {
   public:

      ///Convenience typedef; for use by the Performance Monitor Plugin
      typedef std::map<std::string, float> SampleTimeMap;
      
      /**
      * Steps one level deeper into the tree, if a child already exists with 
      * the specified name
      * then it accumulates the profiling; otherwise a new child node is added to the profile tree.
      * INPUT: name - name of this profiling record
      * WARNINGS:
      * The string used is assumed to be a static string; pointer compares are used throughout
      * the profiling code for efficiency.
      */
      static   void                 startProfile( const char * name );

      /** Starts profile for given name.
       * Same as above but passes a queue size to the child node.
       */
      static   void                 startProfile( const char * profileName, const unsigned int queueSize);

      /**
       * Stop timing on most resent startProfile and record the results.
       */
      static   void                 stopProfile();

      /**
       * Reset the contents of the profiling system
       *
       * @post Everything is reset except tree structure. Timing data is reset.
       */
      static   void                 reset();

      /**
       * Increment the frame counter
       *
       * @post Frame counter incremented by one.
       */
      static   void                 incrementFrameCounter();

      /**
       * @return Returns the number of frames since reset
       */
      static   int                  getFrameCountSinceReset()
      {
         return mFrameCounter;
      }

      /**
       * @return Returns the elapsed time since last reset
       */
      static   float                getTimeSinceReset();

      /**
       * @return Returns a new Iterator that is set to the root.
       */
      static   ProfileIterator     begin()
      {
         return ProfileIterator(&mRoot);
      }

      /**
       * @return Returns a new Iterator that is set to NULL.
       */
      static   ProfileIterator     end()
      {
         return ProfileIterator(NULL);
      }


      /**
       * @return Returns the Root Node.
       */
      static   ProfileNode*         getRootNode()
      {
         return &mRoot;
      }

      /**
       * @return Profile Tree has been printed out using vpr::DBG.
       */
      static   void                 printTree()
      {
         mTreeLock.acquire();
         if ( mRoot.getChild() != NULL )
         {
            mRoot.printTree(mRoot.getChild());
         }
         mTreeLock.release();
      }

      /**
       * @return Returns a new vector of the names in the Profile.
       */
      static std::vector<std::string> getNames()
      {
            mTreeLock.acquire();
              std::vector<std::string> names_list;
              getNamesRecursively(names_list, &mRoot);
            mTreeLock.release();
            return names_list;
      }

      
      /**
       * @return Returns a SampleTimeMap that has the names in the profile
       *         and their last sample.
       */
      static SampleTimeMap getValueMap( )
      {
         SampleTimeMap sample_time_map;
         mTreeLock.acquire();
            getValueMapRecursively(sample_time_map, &mRoot);
         mTreeLock.release();
         return sample_time_map;
      }

      /**
       * @return Returns the last sample from the specified nodeName.
       */
       static float getNamedNodeSample( const char * nodeName )
       {
           mTreeLock.acquire();
           ProfileNode* node = mRoot.getNamedNode( nodeName );

           if(node == NULL )
            {
               mTreeLock.release();
               return 0.0;
            }

           ProfileNode::NodeHistoryRange p = node->getNodeHistoryRange();
           mTreeLock.release();
           return p.first->msecf();
       }

      /**
       * @post Iterator has been deleted
       */
      static  void  releaseIterator( ProfileIterator* iterator )
      {
         delete iterator;
      }



   private:
      static   vpr::Mutex           mTreeLock;
      static   ProfileNode          mRoot;
      static   ProfileNode*         mCurrentNode;
      static   int                  mFrameCounter;
      static   vpr::Interval*       mResetTime;

      /** Private Member Functions */
      static void getNamesRecursively( std::vector<std::string>& nameList, 
                                       ProfileNode* node )
      {
         if ( NULL == node )
         { return; }

         getNamesRecursively(nameList, node->getSibling());
         nameList.push_back(node->getName());
         getNamesRecursively(nameList, node->getChild());
       }

      static void getValueMapRecursively( SampleTimeMap& sampleTimeMap,
                                          ProfileNode* node )
      {
         if ( NULL == node )
         { return; }

         getValueMapRecursively(sampleTimeMap, node->getSibling());
         float last_sample = node->getNodeHistoryRange().first->msecf();
         std::string name = node->getName();
         sampleTimeMap[name] = last_sample;
         getValueMapRecursively(sampleTimeMap, node->getChild());
      }
   };


/*
 * ProfileSampleClass is a simple way to profile a function's scope
 * Use the PROFILE macro at the start of scope to time
 */
   class ProfileSample
   {
   public:
      ProfileSample( const char * name )
      {
         ProfileManager::startProfile( name );
      }

      ProfileSample( const char * name, const unsigned int queue_size)
      {
         ProfileManager::startProfile( name, queue_size);
      }

      ~ProfileSample( void )
      {
         ProfileManager::stopProfile();
      }
   };

#if defined(DISABLE_VPR_PROFILE)
#define  VPR_PROFILE( name )
#else
#define  VPR_PROFILE( name )        vpr::ProfileSample __profile( name )
#endif

#if defined(DISABLE_VPR_PROFILE)
#define  VPR_PROFILE_HISTORY( name, queue_size )
#else
#define  VPR_PROFILE_HISTORY( name, queue_size )         vpr::ProfileSample __profile( name, queue_size)
#endif

#if defined(DISABLE_VPR_PROFILE)
#define  VPR_PROFILE_RESULTS( )
#else
#define  VPR_PROFILE_RESULTS( )        vpr::ProfileManager::printTree( )
#endif

} // end vpr namespace

#endif
