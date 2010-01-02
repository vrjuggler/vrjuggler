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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef VPR_PROFILE_MANAGER_H
#define VPR_PROFILE_MANAGER_H

/**
 * \file
 *
 * Originally based on
 * Real-Time Hierarchical Profiling for Game Programming Gems 3
 * by Greg Hjelstrom & Byon Garrabrant
 */

#include <vpr/vprConfig.h>

#include <string>
#include <vector>
#include <map>
#include <sstream>

#include <vpr/Sync/Guard.h>
#include <vpr/Sync/Mutex.h>

#include <vpr/Perf/ProfileIterator.h>
#include <vpr/Perf/ProfileNode.h>

#include <vpr/Thread/TSObjectProxy.h>
#include <vpr/Thread/Thread.h>

namespace vpr
{

   /** \class ProfileManager ProfileManager.h vpr/Perf/ProfileManager.h
    *
    * Global static facade for using the profiling code.
    */
   class VPR_CLASS_API ProfileManager
   {
   public:
      struct ThreadProfileData
      {
         ThreadProfileData()
            : mRoot("root")
         {
            mCurrentNode = &mRoot;
         }

         ProfileNode    mRoot;         /**< Root of profile information for the thread. */
         ProfileNode*   mCurrentNode;  /**< Current node for the thread. */
         vpr::Interval  mResetTime;    /**< Reset time for the thread. */
      };

      /** Convenience typedef for use by the Performance Monitor Plugin. */
      typedef std::map<std::string, vpr::Interval> ProfileSampleResult;

      /** @name Profiling methods */
      //@{
      /**
       * Steps one level deeper into the tree, if a child already exists with
       * the specified name then it accumulates the profiling.  Otherwis, a
       * new child node is added to the profile tree.
       *
       * @param profileName Name of this profiling record.
       * @param queueSize   Size of the queue to use.
       *
       * @note The string used is assumed to be a static string.  Pointer
       *       comparisons are used throughout the profiling code for
       *       efficiency.
       */
      static void startProfile(const char* profileName,
                               const unsigned int queueSize = 0);

      /**
       * Stops timing on most resent startProfile and record the results.
       */
      static void stopProfile();
      //@}

      /**
       * Returns the root node of the manager.
       *
       * @param thread The thread to get the root for.  A value of NULL
       *               indicates that the current thread should be queried.
       */
      static ProfileNode* getRootNode(vpr::Thread* thread = NULL);

      /** Prints the full profile tree. */
      static void printTree(bool forAllThreads = true);

      /** @name Iterator handling. */
      //@{
      /** Returns a new Iterator that is set to the root. */
      static ProfileIterator begin()
      {
         return ProfileIterator(getRootNode());
      }

      /** Returns a new Iterator that is set to end (NULL). */
      static ProfileIterator end()
      {
         return ProfileIterator(NULL);
      }

      /** Releases the given iterator. */
      static void releaseIterator(ProfileIterator* iterator)
      {
         delete iterator;
      }
      //@}

      /** @name Reset methods. */
      //@{
      /**
       * Resets the contents of the profiling system.
       *
       * @post Everything is reset except tree structure. Timing data is reset.
       */
      static void reset();

      /**
       * Returns the elapsed time since last reset.
       */
      static float getTimeSinceReset();
      //@}

   public:
      /** @name Data sampling and aggregation methods. */
      //@{
      /**
       * Gets the vector of names in the profile.
       *
       * @return vector of the names in the Profile.
       */
      static std::vector<std::string> getNames()
      {
         std::vector<std::string> names_list;
         getNamesRecursively(names_list, getRootNode());
         return names_list;
      }

      /**
       * Returns a ProfileSampleResult that has the names in the profile and
       * their last sample.
       */
      static ProfileSampleResult getSampleResult()
      {
         ProfileSampleResult sample_time_map;
         std::vector<Thread*>::size_type num_threads =
            vpr::ThreadManager::instance()->getNumThreads();
         for ( std::vector<Thread*>::size_type t = 0; t < num_threads; ++t )
         {
            vpr::Thread* thread = vpr::ThreadManager::instance()->getThread(t);
            getSampleResultRecursively(sample_time_map, getRootNode(thread));
         }
         return sample_time_map;
      }

      /**
       * Returns the last sample from the node with the given name (by string).
       */
       static float getNamedNodeSample( const char * nodeName )
       {
           ProfileNode* node = getRootNode()->getNamedChild( nodeName );

           if(node == NULL )
           {
              return 0.0;
           }
           else
           {
              return node->getLastSample().msecf();
           }
       }
       //@}

   private:
      static TSObjectProxy<ThreadProfileData>  mThreadData;    /**< The profile data for each thread being sampled. */

      // Private Member Functions
      static void getNamesRecursively( std::vector<std::string>& nameList,
                                       ProfileNode* node )
      {
         if ( NULL == node )
         {
            return;
         }

         getNamesRecursively(nameList, node->getSibling());
         nameList.push_back(node->getName());
         getNamesRecursively(nameList, node->getChild());
       }

      static void getSampleResultRecursively( ProfileSampleResult& sampleTimeMap,
                                          ProfileNode* node )
      {
         if ( NULL == node )
         {
            return;
         }

         getSampleResultRecursively(sampleTimeMap, node->getSibling());
         vpr::Interval last_sample;
         last_sample.msecf( node->getNodeHistoryRange().first->msecf());
         std::string name = node->getName();
         sampleTimeMap[name] = last_sample;
         getSampleResultRecursively(sampleTimeMap, node->getChild());
      }
   };


/**
 * Set of routines for allowing simplified access to profile monitoring API.
 */
namespace prof
{
#if defined(DISABLE_VPR_PROFILE)
   inline void start(const char* name) {;}
   inline void start(const char* name, unsigned int histSize) {;}
   inline void next(const char* name) {;}
   inline void next(const char* name, unsigned int histSize) {;}
   inline void stop() {;}
   inline void printTree() {;}
#else
   /** Start a sample. */
   inline void start(const char* name)
   {
      vpr::ProfileManager::startProfile(name);
   }
   /** Start a sample with history. */
   inline void start(const char* name, unsigned int histSize)
   {
      vpr::ProfileManager::startProfile(name,histSize);
   }
   /** Go to the next sample (stoping previous sample). */
   inline void next(const char* name)
   {
      vpr::ProfileManager::stopProfile();
      vpr::ProfileManager::startProfile(name);
   }
   /** Go to the next sample (stoping previous sample). */
   inline void next(const char* name, unsigned int histSize)
   {
      vpr::ProfileManager::stopProfile();
      vpr::ProfileManager::startProfile(name,histSize);
   }
   /** Stop the previous sample. */
   inline void stop()
   {
      vpr::ProfileManager::stopProfile();
   }
   /** Print the current profile tree. */
   inline void printTree()
   {
      vpr::ProfileManager::printTree();
   }
#endif
} // End prof namespace

   /** \class ProfileSample ProfileManager.h vpr/Perf/ProfileManager.h
    *
    * ProfileSample is a guard style class for handle a single sample.
    *
    * This class implements a guard pattern where by the creation of a
    * ProfileSample object starts a sample and the destruction stops the
    * sampling.
    *
    * In most cases this class should not be used directly but instead the
    * PROFILE_ macro's should be used.
    */
   class ProfileSample
   {
   public:
      ProfileSample(const char * name)
      {
         ProfileManager::startProfile(name);
      }

      ProfileSample(const char * name, const unsigned int queue_size)
      {
         ProfileManager::startProfile(name, queue_size);
      }

      ~ProfileSample()
      {
         ProfileManager::stopProfile();
      }
   };


#if defined(DISABLE_VPR_PROFILE)
#define  VPR_PROFILE_GUARD( name )
#else
#define  VPR_PROFILE_GUARD( name )        vpr::ProfileSample __profile( name )
#endif

#if defined(DISABLE_VPR_PROFILE)
#define  VPR_PROFILE_GUARD_HISTORY( name, queue_size )
#else
#define  VPR_PROFILE_GUARD_HISTORY( name, queue_size )         vpr::ProfileSample __profile( name, queue_size)
#endif

#if defined(DISABLE_VPR_PROFILE)
#define  VPR_PROFILE_RESULTS( )
#else
#define  VPR_PROFILE_RESULTS( )        vpr::ProfileManager::printTree( )
#endif

} // end vpr namespace

#endif
