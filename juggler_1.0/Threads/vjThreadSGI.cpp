/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 */


#include <vjConfig.h>
#include <sys/types.h>
#include <sys/prctl.h>

#include <Threads/vjThread.h>
#include <Threads/vjThreadSGI.h>
#include <Threads/vjThreadManager.h>

//: Spawning constructor
//  This will actually start a new thread
//  that will execute the specified function.
vjThreadSGI::vjThreadSGI (THREAD_FUNC func, void* arg, long flags,
             u_int priority, void* stack_addr, size_t stack_size)
{
   mUserThreadFunctor = NULL;

   // Create the thread functor to start
   mUserThreadFunctor = new vjThreadNonMemberFunctor(func, arg);
   vjThreadMemberFunctor<vjThreadSGI>* start_functor
               = new vjThreadMemberFunctor<vjThreadSGI>(this,&vjThreadSGI::startThread,NULL);

   // START THREAD
   // NOTE: Automagically registers UNLESS failure
   int ret_val = spawn(start_functor, flags, priority, stack_addr, stack_size);

   if(!ret_val)
   {
      vjThreadManager::instance()->lock();
      {
         registerThread(false);     // Failed to create
      }
      vjThreadManager::instance()->unlock();
   }
}


//: Spawning constructor with arguments (functor version).
//   This will start a new
//   thread that will execute the specified function.
vjThreadSGI::vjThreadSGI ( vjBaseThreadFunctor* functorPtr, long flags,
              u_int priority, void* stack_addr, size_t stack_size)
{
    mUserThreadFunctor = NULL;

   // Create the thread functor to start
   mUserThreadFunctor = functorPtr;;
   vjThreadMemberFunctor<vjThreadSGI>* start_functor
               = new vjThreadMemberFunctor<vjThreadSGI>(this,&vjThreadSGI::startThread,NULL);

   // START THREAD
   // NOTE: Automagically registers UNLESS failuer
   int ret_val = spawn(start_functor, flags, priority, stack_addr, stack_size);

   if(!ret_val)
   {
      vjThreadManager::instance()->lock();
      {
         registerThread(false);     // Failed to create
      }
      vjThreadManager::instance()->unlock();
   }
}

/**
 * Called by the spawn routine to start the user thread function
 * PRE: Called ONLY by a new thread
 * POST: Do any thread registration necessary
 *       Call the user thread functor
 *
 * @param null_param
 */
void vjThreadSGI::startThread(void* null_param)
{
   // WE are a new thread... yeah!!!!
   // TELL EVERYONE THAT WE LIVE!!!!
   vjThreadManager::instance()->lock();      // Lock manager
   {
      setLocalThreadPtr(this);               // Store the pointer to me
      registerThread(true);
   }
   vjThreadManager::instance()->unlock();

   // --- CALL USER FUNCTOR --- //
   (*mUserThreadFunctor)();
}


// -----------------------------------------------------------------------
//: Make the calling thread wait for the termination of the specified
//+ thread.
//! NOTE:  Not implemented.
//! RETURNS:  0 - Successful completion
//! RETURNS: -1 - Error
// -----------------------------------------------------------------------
 int
vjThreadSGI::join (void** arg)
{
   std::cerr << "vjThreadSGI::join() not implemented yet!\n";
   return -1;
}

