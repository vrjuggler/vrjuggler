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

#include <vpr/vprConfig.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadManager.h>
#include <vpr/Util/Assert.h>
#include <vpr/md/SPROC/Thread/ThreadSGI.h>


namespace vpr
{

// Non-spawning constructor.  This will not start a thread.
ThreadSGI::ThreadSGI(BaseThread::VPRThreadPriority priority,
                     BaseThread::VPRThreadScope scope,
                     BaseThread::VPRThreadState state, size_t stackSize)
   : mUserThreadFunctor(NULL), mRunning(false)
{
}

/**
 * Spawning constructor.  This will actually start a new thread that will
 * execute the specified function.
 */
ThreadSGI::ThreadSGI(thread_func_t func, void* arg,
                     BaseThread::VPRThreadPriority priority,
                     BaseThread::VPRThreadScope scope,
                     BaseThread::VPRThreadState state, size_t stackSize)
   : mUserThreadFunctor(NULL), mRunning(false)
{
   // Create the thread functor to start.
   // XXX: Memory leak.
   setFunctor(new ThreadNonMemberFunctor(func, arg));
   start();
}

/**
 * Spawning constructor with arguments (functor version).  This will start a
 * new thread that will execute the specified function.
 */
ThreadSGI::ThreadSGI(BaseThreadFunctor* functorPtr,
                     BaseThread::VPRThreadPriority priority,
                     BaseThread::VPRThreadScope scope,
                     BaseThread::VPRThreadState state, size_t stackSize)
   : mUserThreadFunctor(NULL), mRunning(false)
{
   setFunctor(functorPtr);
   start();
}

void ThreadSGI::setFunctor(BaseThreadFunctor* functorPtr)
{
   vprASSERT(! mRunning && "Thread already running");
   vprASSERT(functorPtr->isValid());

   mUserThreadFunctor = functorPtr;
}

vpr::ReturnStatus ThreadSGI::start()
{
   vpr::ReturnStatus status;

   if ( mRunning )
   {
      vprASSERT(false && "Thread already running");
      status.setCode(vpr::ReturnStatus::Fail);
   }
   else if ( NULL == mUserThreadFunctor )
   {
      vprASSERT(false && "No functor set");
      status.setCode(vpr::ReturnStatus::Fail);
   }
   else
   {
      // XXX: Memory leak.
      ThreadMemberFunctor<ThreadSGI>* start_functor
                  = new ThreadMemberFunctor<ThreadSGI>(this,
                                                       &ThreadSGI::startThread,
                                                       NULL);

      // Spawn the thread.
      status = spawn(start_functor);

      if ( status.success() )
      {
         mRunning = true;

         ThreadManager::instance()->lock();
         {
            registerThread(true);
         }
         ThreadManager::instance()->unlock();
      }
      else
      {
         ThreadManager::instance()->lock();
         {
            registerThread(false);     // Failed to create
         }
         ThreadManager::instance()->unlock();
      }
   }

   return status;
}

vpr::ReturnStatus ThreadSGI::spawn(BaseThreadFunctor* functorPtr)
{
   vpr::ReturnStatus status;
   mThreadPID = sproc(thread_func_t(&vprThreadFunctorFunction),
                      PR_SADDR | PR_SFDS, functorPtr);

   if ( mThreadPID == -1 )
   {
      status.setCode(vpr::ReturnStatus::Fail);
   }

   return status;
}

/**
 * Called by the spawn routine to start the user thread function.
 */
void ThreadSGI::startThread(void* null_param)
{
   // WE are a new thread... yeah!!!!
   // TELL EVERYONE THAT WE LIVE!!!!
   ThreadManager::instance()->lock();      // Lock manager
   {
      setLocalThreadPtr(this);               // Store the pointer to me
   }
   ThreadManager::instance()->unlock();

   // Tell this thread to die when its parent dies
   prctl(PR_SETEXITSIG, 0);
   prctl(PR_TERMCHILD);

   // --- CALL USER FUNCTOR --- //
   (*mUserThreadFunctor)();
}


/**
 * Makes the calling thread wait for the termination of the specified
 * thread.
 */
int
ThreadSGI::join (void** arg)
{
    int status, retval;
    pid_t pid;

    do {
        pid = ::waitpid(mThreadPID, &status, 0);
    } while ( WIFSTOPPED(status) != 0 );

    if ( pid > -1 ) {
        vprASSERT(pid == mThreadPID);

        if ( WIFEXITED(status) != 0 && arg != NULL ) {
            **((int**) arg) = WEXITSTATUS(status);
        }
        else if ( WIFSIGNALED(status) != 0 && arg != NULL ) {
            **((int**) arg) = WTERMSIG(status);
        }

        retval = 0;
    }
    else {
        retval = -1;
    }

    return retval;
}

}; // End of vpr namespace
