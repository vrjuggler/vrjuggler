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
 * Originally based on
 * Real-Time Hierarchical Profiling for Game Programming Gems 3
 * by Greg Hjelstrom & Byon Garrabrant
 */

#include <vpr/vprConfig.h>

#include <string>
#include <vector>
#include <map>

#include <vpr/Sync/Guard.h>
#include <vpr/Sync/Mutex.h>

#include <vpr/Perf/ProfileIterator.h>
#include <vpr/Perf/ProfileNode.h>

namespace vpr
{

   /***
   * Global static facade for using the Profiling code.
   *
   *
   */
   class VPR_CLASS_API ProfileManager
   {
   public:

      ///Convenience typedef; for use by the Performance Monitor Plugin
      typedef std::map<std::string, vpr::Interval> ProfileSampleResult;

      /** @name Profiling methods. */
      //@{
      /**
      * Steps one level deeper into the tree, if a child already exists with
      * the specified name then it accumulates the profiling;
      * otherwise a new child node is added to the profile tree.
      *
      * @param name      Name of this profiling record
      * @param queueSize Size of the queue to use
      * @note
      *   The string used is assumed to be a static string; pointer compares are used throughout
      *   the profiling code for efficiency.
      */
      static void startProfile( const char * profileName, const unsigned int queueSize = 0);

      /**
       * Stop timing on most resent startProfile and record the results.
       */
      static void stopProfile();
      //@}

      /** Return the root node of the manager. */
      static ProfileNode* getRootNode()
      {
         return &mRoot;
      }

      /** Print the profile tree rooted at the root node. */
      static void printTree()
      {
         mTreeLock.acquire();
         mRoot.printTree();
         mTreeLock.release();
      }

      // ------------------------
      /// @nameIterator handling.
      // ------------------------
      //@{
      /** Returns a new Iterator that is set to the root. */
      static ProfileIterator begin()
      {
         return ProfileIterator(&mRoot);
      }

      /** Returns a new Iterator that is set to end (NULL). */
      static ProfileIterator end()
      {
         return ProfileIterator(NULL);
      }

      /** Release the given iterator. */
      static void releaseIterator( ProfileIterator* iterator )
      {
         delete iterator;
      }
      //@}

      // ----------------------
      /// @name Global metrics.
      // ----------------------
      //@{
      /**
       * Reset the contents of the profiling system.
       *
       * @post Everything is reset except tree structure. Timing data is reset.
       */
      static void reset();

      /**
       * @return Returns the elapsed time since last reset
       */
      static float getTimeSinceReset();

      /**
       * Increment the frame counter.
       *
       * @post Frame counter incremented by one.
       */
      static void incrementFrameCounter();

      /** Get the number of frames since reset.
       * @return Returns the number of frames since reset.
       */
      static int getFrameCountSinceReset()
      {
         return mFrameCounter;
      }
      //@}

   public:
      // ---------------------------------------------
      /// @name Data sampling and aggregation methods.
      // ---------------------------------------------
      //@{
      /** Get vector of names in the profile.
       * @return vector of the names in the Profile.
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
       * @return Returns a ProfileSampleResult that has the names in the profile
       *         and their last sample.
       */
      static ProfileSampleResult getSampleResult( )
      {
         ProfileSampleResult sample_time_map;
         mTreeLock.acquire();
            getSampleResultRecursively(sample_time_map, &mRoot);
         mTreeLock.release();
         return sample_time_map;
      }

      /**
       * @return Returns the last sample from the node with the given name (by string).
       */
       static float getNamedNodeSample( const char * nodeName )
       {
           mTreeLock.acquire();
           ProfileNode* node = mRoot.getNamedChild( nodeName );

           if(node == NULL )
           {
              mTreeLock.release();
              return 0.0;
           }
           else
           {
              return node->getLastSample().msecf();
           }
       }
       //@}

   private:
      static   vpr::Mutex           mTreeLock;
      static   ProfileNode          mRoot;
      static   ProfileNode*         mCurrentNode;
      static   int                  mFrameCounter;
      static   vpr::Interval        mResetTime;

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

      static void getSampleResultRecursively( ProfileSampleResult& sampleTimeMap,
                                          ProfileNode* node )
      {
         if ( NULL == node )
         { return; }

         getSampleResultRecursively(sampleTimeMap, node->getSibling());
         vpr::Interval last_sample;
         last_sample.msecf( node->getNodeHistoryRange().first->msecf());
         std::string name = node->getName();
         sampleTimeMap[name] = last_sample;
         getSampleResultRecursively(sampleTimeMap, node->getChild());
      }
   };


/*
 * ProfileSample is a guard style class for handle a single sample.
 *
 * This class implements a guard pattern where by the creation of a
 * ProfileSample object starts a sample and the destruction stops the sampling.
 *
 * In most cases this class should not be used directly but instead the
 * PROFILE_ macro's should be used.
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


// XXX: We really need some macros that do not require scoping.
/// VPR_PROFILE_NEXT or something like that would be nice.
//
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
