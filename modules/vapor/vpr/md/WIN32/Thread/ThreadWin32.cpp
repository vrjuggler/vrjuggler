/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <Threads/Thread.h>
#include <Threads/ThreadManager.h>
#include <md/WIN32/ThreadWin32.h>


namespace vpr {

ThreadTable<DWORD> ThreadWin32::mThreadTable;


//: Spawning constructor
//  This will actually start a new thread
//  that will execute the specified function.
ThreadWin32::ThreadWin32 (thread_func_t func, void* arg,
                          VPRThreadPriority priority, VPRThreadScope scope,
                          VPRThreadState state, size_t stack_size)
{
   ThreadManager::instance()->lock();
   {
      ThreadNonMemberFunctor* NonMemFunctor = new ThreadNonMemberFunctor(func, arg);
      int retVal = spawn(NonMemFunctor, priority, scope, state, stack_size);
      if (retVal > 0)
      {
         registerThread(true);
         mThreadTable.addThread(this, mThreadTID);
      }
      else
         registerThread(false);     // Failed to create
   }
   ThreadManager::instance()->unlock();
}


//: Spawning constructor with arguments (functor version).
//   This will start a new
//   thread that will execute the specified function.
ThreadWin32::ThreadWin32 (BaseThreadFunctor* functorPtr,
                          VPRThreadPriority priority, VPRThreadScope scope,
                          VPRThreadState state, size_t stack_size)
{
   ThreadManager::instance()->lock();
   {
      int retVal = spawn(functorPtr, priority, scope, state, stack_size);
      if (retVal > 0)
      {
         registerThread(true);
         mThreadTable.addThread(this, mThreadTID);
      }
      else
         registerThread(false);     // Failed to create));
   }
   ThreadManager::instance()->unlock();
}


// ---------------------------------------------------------------------------
// Create a new thread that will execute functorPtr.
//
// PRE: None.
// POST: A thread (with any specified attributes) is created that begins
//       executing func().  Depending on the scheduler, it may being
//       execution immediately, or it may block for a short time before
//       beginning execution.
// ---------------------------------------------------------------------------
int
ThreadWin32::spawn (BaseThreadFunctor* functorPtr,
                    VPRThreadPriority priority, VPRThreadScope scope,
                    VPRThreadState state, size_t stack_size)
{
    unsigned int thread_id;
    mThreadHandle = (HANDLE) _beginthreadex(NULL, 0, &ThreadFunctorFunction,
                                            functorPtr, 0, &thread_id);
    if (0 == mThreadHandle)
    {
        std::cerr << clrOutNORM(clrRED, "ERROR:")
                  << " Could not create thread." << std::endl;
        return -1;
    }

    mThreadTID = thread_id;

    return 1;
}

}; // End of vpr namespace
