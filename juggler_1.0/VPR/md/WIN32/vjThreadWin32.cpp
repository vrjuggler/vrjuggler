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

#include <VPR/Threads/vjThread.h>
#include <VPR/Threads/vjThreadManager.h>
#include <VPR/md/WIN32/vjThreadWin32.h>


VJ_IMPLEMENT_DATA(vjThreadTable<DWORD>) vjThreadWin32::mThreadTable;


//: Spawning constructor
//  This will actually start a new thread
//  that will execute the specified function.
vjThreadWin32::vjThreadWin32 (vj_thread_func_t func, void* arg,
                              VJThreadPriority priority, VJThreadScope scope,
                              VJThreadState state, size_t stack_size)
{
   vjThreadManager::instance()->lock();
   {
      vjThreadNonMemberFunctor* NonMemFunctor = new vjThreadNonMemberFunctor(func, arg);
      int retVal = spawn(NonMemFunctor, priority, scope, state, stack_size);
      if (retVal > 0)
      {
         registerThread(true);
         mThreadTable.addThread(this, mThreadTID);
      }
      else
         registerThread(false);     // Failed to create
   }
   vjThreadManager::instance()->unlock();
}


//: Spawning constructor with arguments (functor version).
//   This will start a new
//   thread that will execute the specified function.
vjThreadWin32::vjThreadWin32 (vjBaseThreadFunctor* functorPtr,
                              VJThreadPriority priority, VJThreadScope scope,
                              VJThreadState state, size_t stack_size)
{
   vjThreadManager::instance()->lock();
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
   vjThreadManager::instance()->unlock();
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
vjThreadWin32::spawn (vjBaseThreadFunctor* functorPtr,
                      VJThreadPriority priority, VJThreadScope scope,
                      VJThreadState state, size_t stack_size)
{
    unsigned int thread_id;
    mThreadHandle = (HANDLE) _beginthreadex(NULL, 0, &ThreadFunctorFunction,
                                            functorPtr, 0, &thread_id);
    if (0 == mThreadHandle)
    {
        std::cerr << clrOutNORM(clrRED, "ERROR:") << " Could not create thread." << std::endl;
        return -1;
    }

    mThreadTID = thread_id;

    return 1;
}

vjBaseThread* vjThreadWin32::self ()
{
   DWORD thread_id = GetCurrentThreadId();   // Get our pid or handle

   // Get the entry in the thread table.
   vjBaseThread* cur_thread = mThreadTable.getThread(thread_id);

   // Return us.
   return cur_thread;
}
