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
 * mostly taken from:
 * Real-Time Hierarchical Profiling for Game Programming Gems 3
 * by Greg Hjelstrom & Byon Garrabrant
 */

#include <vpr/vprConfig.h>

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

      /**
      * Steps one level deeper into the tree, if a child already exists with the specified name
      * then it accumulates the profiling; otherwise a new child node is added to the profile tree.
      * INPUT: name - name of this profiling record
      * WARNINGS:
      * The string used is assumed to be a static string; pointer compares are used throughout
      * the profiling code for efficiency.
      */
      static   void                 startProfile( const char * name );

      static   void                 startProfile( const char * profileName, const unsigned int queueSize);

      /**
       * Stop timing and record the results.
       */
      static   void                 stopProfile( void );

      /**
       * Reset the contents of the profiling system
       * 
       * @post Everything is reset except tree structure. Timing data is reset.
       */
      static   void                 reset( void );

      /**
       * Increment the frame counter
       *
       * @post Frame counter incremented by one.
       */
      static   void                 incrementFrameCounter( void );

      /**
       * @return Returns the number of frames since reset
       */
      static   int                  getFrameCountSinceReset( void )
      {
         return mFrameCounter;
      }

      /**
       * @return Returns the elapsed time since last reset
       */
      static   float                getTimeSinceReset(void);

      /**
       * @return Returns a new Iterator that is set to the root.
       */
      static   ProfileIterator*     getIterator( void )
      {
         return new ProfileIterator(&mRoot);
      }

      static   ProfileNode*         getRootNode( void )
      {
         return &mRoot;
      }

      static   void                 printTree(void )
      {
         mTreeLock.acquire();
         if ( mRoot.getChild() != NULL )
         {
            mRoot.printTree(mRoot.getChild());
         }
         mTreeLock.release();
      }

      /**
       * @post Iterator has been deleted
       */
      static   void                 releaseIterator( ProfileIterator* iterator )
      {
         delete iterator;
      }


      static   vpr::Mutex           mTreeLock;

   private:
      static   ProfileNode          mRoot;
      static   ProfileNode*         mCurrentNode;
      static   int                  mFrameCounter;
      static   vpr::Interval*       mResetTime;
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

#if defined(DEBUG) || defined(_DEBUG)
#define	VPR_PROFILE( name )			vpr::ProfileSample __profile( name )
#else
#define	VPR_PROFILE( name )
#endif

#if defined(DEBUG) || defined(_DEBUG)
#define	VPR_PROFILE_HISTORY( name, queue_size )			vpr::ProfileSample __profile( name, queue_size)
#else
#define	VPR_PROFILE_HISTORY( name, queue_size )
#endif

#if defined(DEBUG) || defined(_DEBUG)
#define	VPR_PROFILE_RESULTS( )			vpr::ProfileManager::printTree( )
#else
#define	VPR_PROFILE_RESULTS( )
#endif

} // end vpr namespace

#endif
