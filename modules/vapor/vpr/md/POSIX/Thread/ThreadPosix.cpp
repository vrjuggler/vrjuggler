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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

#ifdef HAVE_SYS_CAPABILITY_H
#   include <sys/capability.h>
#endif

#include <vpr/Thread/ThreadManager.h>
#include <vpr/Thread/ThreadFunctor.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/Assert.h>
#include <vpr/md/POSIX/Thread/ThreadPosix.h>


typedef struct sched_param sched_param_t;

namespace vpr {

ThreadPosix::staticWrapper ThreadPosix::statics;


// ---------------------------------------------------------------------------
//: Spawning constructor
//
// This will actually start a new thread that will execute the specified
// function.
// ---------------------------------------------------------------------------
ThreadPosix::ThreadPosix (thread_func_t func, void* arg,
                          VPRThreadPriority priority, VPRThreadScope scope,
                          VPRThreadState state, size_t stack_size)
   : mUserThreadFunctor(NULL), mDeleteThreadFunctor(false)
{
   ThreadManager* vpr_tm_inst;

   mScope = VPR_THREAD_SCOPE;
   vpr_tm_inst = ThreadManager::instance();

   // Create the thread functor to start
   mUserThreadFunctor = new ThreadNonMemberFunctor(func, arg);
   mDeleteThreadFunctor = true;
   ThreadMemberFunctor<ThreadPosix>* start_functor
               = new ThreadMemberFunctor<ThreadPosix>(this,
                                                      &ThreadPosix::startThread,
                                                      NULL);

   // START THREAD
   // NOTE: Automagically registers thread UNLESS failure
   int ret_val = spawn(start_functor, priority, scope, state, stack_size);

   if(ret_val)
   {
      vpr_tm_inst->lock();  // Need to lock thread manager before I register the thread with them
      {
         registerThread(false);
      }
      vpr_tm_inst->unlock();
   }
}

// ---------------------------------------------------------------------------
// Spawning constructor with arguments (functor version).
//
// This will start a new thread that will execute the specified function.
// ---------------------------------------------------------------------------
ThreadPosix::ThreadPosix (BaseThreadFunctor* functorPtr,
                          VPRThreadPriority priority, VPRThreadScope scope,
                          VPRThreadState state, size_t stack_size)
    : mUserThreadFunctor(NULL), mDeleteThreadFunctor(false)
{
    ThreadManager* vpr_tm_inst;

    mScope = VPR_THREAD_SCOPE;
    vpr_tm_inst = ThreadManager::instance();

    // Create the thread functor to start
    mUserThreadFunctor = functorPtr;
    ThreadMemberFunctor<ThreadPosix>* start_functor
             = new ThreadMemberFunctor<ThreadPosix>(this,
                                                    &ThreadPosix::startThread,
                                                    NULL);

    // Start thread
    // NOTE: Automagically registers thread UNLESS failure
    int ret_val = spawn(start_functor, priority, scope, state, stack_size);

    if(ret_val)
    {
       vpr_tm_inst->lock();  // Need to lock thread manager before I register the thread with them
       {
         registerThread(false);
       }
       vpr_tm_inst->unlock();
    }
}

// ---------------------------------------------------------------------------
// Destructor.
//
// PRE: None.
// POST: This thread is removed from the thread table and from the local
//       thread hash.
// ---------------------------------------------------------------------------
ThreadPosix::~ThreadPosix (void) {
    int status;

    if ( mDeleteThreadFunctor ) {
        delete mUserThreadFunctor;
    }

    status = 0;
}

// ---------------------------------------------------------------------------
// Create a new thread that will execute functorPtr.
//
// PRE: None.
// POST: A thread (with any specified attributes) is created that begins
//       executing func().  Depending on the scheduler, it may begin
//       execution immediately, or it may block for a short time before
//       beginning execution.
// ---------------------------------------------------------------------------
int
ThreadPosix::spawn (BaseThreadFunctor* functorPtr,
                    VPRThreadPriority priority, VPRThreadScope scope,
                    VPRThreadState state, size_t stack_size)
{
    int ret_val;
    pthread_attr_t thread_attrs;
    int pthread_prio;

    pthread_prio = vprThreadPriorityToPOSIX(priority);

    // Initialize thread_attrs and set the priority of the thread if it is
    // supported.  HP-UX requires a slightly different syntax than other
    // operating systems.
#ifdef _PTHREADS_DRAFT_4
    pthread_attr_create(&thread_attrs);

#   ifdef _POSIX_THREAD_REALTIME_SCHEDULING
        if ( pthread_prio > 0 ) {
            pthread_attr_setprio(&thread_attrs, pthread_prio);
        }
#   endif   /* _POSIX_THREAD_REALTIME_SCHEDULING */

#else /* ! _PTHREADS_DRAFT_4 */
    sched_param_t prio_param;

    pthread_attr_init(&thread_attrs);

    // If thread priority scheduling is available, set the thread's priority
    // if it is set to be higher than 0.
#   ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
#   if defined(HAVE_SYS_CAPABILITY_H) && ! defined(VPR_OS_FreeBSD)
        cap_t capabilities = cap_get_proc();

        // If we have the capability to do so, set the scope of the threads
        // to system scope.
        if ( capabilities->cap_effective & CAP_SCHED_MGT ) {
            mScope = PTHREAD_SCOPE_SYSTEM;
        }
#   endif   /* HAVE_SYS_CAPABILITY_H */

        pthread_attr_setscope(&thread_attrs, mScope);

        if ( pthread_prio > 0 ) {
            prio_param.sched_priority = pthread_prio;
            pthread_attr_setschedparam(&thread_attrs, &prio_param);
        }
#   endif   /* _POSIX_THREAD_PRIORITY_SCHEDULING */

#endif   /* _PTHREADS_DRAFT_4 */

    // Set the stack size if a value greater than 0 is specified and this
    // pthreads implementation supports it.  Ensure that
    // _POSIX_THREAD_ATTR_STACKSIZE is defined before trying to test its
    // value.
#ifdef _POSIX_THREAD_ATTR_STACKSIZE
    if ( stack_size > 0 ) {
        // ** STACK SIZE CHECK NEEDED **

        pthread_attr_setstacksize(&thread_attrs, stack_size);
    }
#endif

    // Finally create the thread.
#ifdef _PTHREADS_DRAFT_4
    ret_val = pthread_create(&(mThread), thread_attrs,
                             (pthread_startroutine_t) vprThreadFunctorFunction,
                             (pthread_addr_t) functorPtr);
#else
    ret_val = pthread_create(&(mThread), &thread_attrs,
                             vprThreadFunctorFunction, (void *) functorPtr);
#endif

    // Inform the caller if the thread was not created successfully.
    if ( ret_val != 0 ) {
        std::cerr << "vpr::ThreadPosix::spawn() - Cannot create thread:"
                  << strerror(ret_val) << std::endl;
    }

    return ret_val;
}


/**
 * Called by the spawn routine to start the user thread function
 * PRE: Called ONLY by a new thread
 * POST: Do any thread registration necessary
 *       Call the user thread functor
 *
 * @param null_param
 */
void ThreadPosix::startThread(void* null_param)
{
   // WE are a new thread... yeah!!!!
   // TELL EVERYONE THAT WE LIVE!!!!
   ThreadManager::instance()->lock();      // Lock manager
   {
      threadIdKey().setspecific((void*)this);     // Store the pointer to me
      registerThread(true);
   }
   ThreadManager::instance()->unlock();

   // --- CALL USER FUNCTOR --- //
   (*mUserThreadFunctor)();
}


// ---------------------------------------------------------------------------
// Get this thread's priority.
//
// PRE: None.
// POST: The priority of this thread is returned in the integer pointer
//       variable.
// ---------------------------------------------------------------------------
int
ThreadPosix::getPrio (VPRThreadPriority* prio) {
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
    int policy, ret_val;
    sched_param_t fifo_sched_param;

    ret_val = pthread_getschedparam(mThread, &policy, &fifo_sched_param);
    *prio = posixThreadPriorityToVPR(fifo_sched_param.sched_priority);

    return ret_val;
#else
    std::cerr << "vpr::ThreadPosix::getPrio(): Not supported\n";

    return -1;
#endif
}

// ---------------------------------------------------------------------------
// Set this thread's priority.
//
// PRE: None.
// POST: This thread has its priority set to the specified value.
// ---------------------------------------------------------------------------
int
ThreadPosix::setPrio (VPRThreadPriority prio) {
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
    sched_param_t fifo_sched_param;

    fifo_sched_param.sched_priority = prio;

    return pthread_setschedparam(mThread, SCHED_FIFO, &fifo_sched_param);
#else
    std::cerr << "vpr::ThreadPosix::setPrio(): Not supported\n";

    return -1;
#endif
}

BaseThread*
ThreadPosix::self (void)
{
   vprASSERT((statics.mStaticsInitialized==1221) && "Trying to call vpr::ThreadPosix::self before statics are initialized. Don't do that");

   BaseThread* my_thread;
   threadIdKey().getspecific((void**)&my_thread);

   return my_thread;
}



// ===========================================================================
// Private methods follow.
// ===========================================================================

// ---------------------------------------------------------------------------
//: Check the status of the thread creation in order to determine if this
//+ thread should be registered in the thread table or not.
//
//! PRE: An attempt must have been made to create a thread using spawn().
//! POST: If status is 0, the thread gets registered in the thread table and
//+       in the local thread hash.  The count of created threads is
//+       incremented as well.
//
//! ARGS: status - The integer status returned by spawn().
// ---------------------------------------------------------------------------
/*
void
ThreadPosix::checkRegister (int status) {
    if ( status == 0 ) {
       mThreadTable.addThread(this, hash());      // Store way to look me up
       threadIdKey().setspecific((void*)this);     // Store the pointer to me
       registerThread(true);
    } else {
        registerThread(false);   // Failed to create
    }
}
*/

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
int
ThreadPosix::vprThreadPriorityToPOSIX (const VPRThreadPriority priority) {
    int posix_prio;
    int min_prio, max_prio;

    min_prio = sched_get_priority_min(SCHED_OTHER);
    max_prio = sched_get_priority_max(SCHED_OTHER);

    switch (priority) {
      case VPR_PRIORITY_LOW:
      case VPR_PRIORITY_NORMAL:
        posix_prio = min_prio;
        break;
      case VPR_PRIORITY_HIGH:
        posix_prio = min_prio + 1;
        break;
      case VPR_PRIORITY_URGENT:
        posix_prio = max_prio;
        break;
    };

    return posix_prio;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
int
ThreadPosix::vprThreadScopeToPOSIX (const VPRThreadScope scope) {
    int posix_scope;

    switch (scope) {
      case VPR_LOCAL_THREAD:
        posix_scope = PTHREAD_SCOPE_PROCESS;
        break;
      case VPR_GLOBAL_THREAD:
        posix_scope = PTHREAD_SCOPE_SYSTEM;
        break;
    };

    return posix_scope;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
int
ThreadPosix::vprThreadStateToPOSIX (const VPRThreadState state) {
    int posix_state;

    switch (state) {
      case VPR_JOINABLE_THREAD:
        posix_state = PTHREAD_CREATE_JOINABLE;
        break;
      case VPR_UNJOINABLE_THREAD:
        posix_state = PTHREAD_CREATE_DETACHED;
        break;
    };

    return posix_state;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
BaseThread::VPRThreadPriority
ThreadPosix::posixThreadPriorityToVPR (const int priority) {
    VPRThreadPriority vpr_prio;
    int min_prio, max_prio;

    min_prio = sched_get_priority_min(SCHED_OTHER);
    max_prio = sched_get_priority_max(SCHED_OTHER);

    if ( priority == min_prio ) {
        vpr_prio = VPR_PRIORITY_NORMAL;
    }
    else if ( priority == max_prio ) {
        vpr_prio = VPR_PRIORITY_URGENT;
    }
    else {
        vpr_prio = VPR_PRIORITY_HIGH;
    }

    return vpr_prio;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
BaseThread::VPRThreadScope
ThreadPosix::posixThreadScopeToVPR (const int scope) {
    VPRThreadScope vpr_scope;

    switch (scope) {
      case PTHREAD_SCOPE_PROCESS:
        vpr_scope = VPR_LOCAL_THREAD;
        break;
      case PTHREAD_SCOPE_SYSTEM:
        vpr_scope = VPR_GLOBAL_THREAD;
        break;
    };

    return vpr_scope;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
BaseThread::VPRThreadState
ThreadPosix::posixThreadStateToVPR (const int state) {
    VPRThreadState vpr_state;

    switch (state) {
      case PTHREAD_CREATE_JOINABLE:
        vpr_state = VPR_JOINABLE_THREAD;
        break;
      case PTHREAD_CREATE_DETACHED:
        vpr_state = VPR_UNJOINABLE_THREAD;
        break;
    };

    return vpr_state;
}

}; // End of vpr namespace
