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


#include <vjConfig.h>

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#ifdef HAVE_SYS_CAPABILITY_H
#   include <sys/capability.h>
#endif

#include <Threads/vjThread.h>
#include <Threads/vjThreadManager.h>
#include <Threads/vjThreadFunctor.h>


vjThreadPosix::staticWrapper vjThreadPosix::statics;

typedef struct sched_param sched_param_t;


// ---------------------------------------------------------------------------
//: Spawning constructor
//
// This will actually start a new thread that will execute the specified
// function.
// ---------------------------------------------------------------------------
vjThreadPosix::vjThreadPosix (vj_thread_func_t func, void* arg, long flags,
                              u_int priority, void* stack_addr,
                              size_t stack_size)
{
   mUserThreadFunctor = NULL;
   vjThreadManager* vj_tm_inst;

   mScope = VJ_THREAD_SCOPE;
   vj_tm_inst = vjThreadManager::instance();

   // Create the thread functor to start
   mUserThreadFunctor = new vjThreadNonMemberFunctor(func, arg);
   vjThreadMemberFunctor<vjThreadPosix>* start_functor
               = new vjThreadMemberFunctor<vjThreadPosix>(this,&vjThreadPosix::startThread,NULL);

   // START THREAD
   // NOTE: Automagically registers thread UNLESS failure
   int ret_val = spawn(start_functor, flags, priority, stack_addr,
                        stack_size);

   if(ret_val)
   {
      vj_tm_inst->lock();  // Need to lock thread manager before I register the thread with them
      {
         registerThread(false);
      }
      vj_tm_inst->unlock();
   }
}

// ---------------------------------------------------------------------------
// Spawning constructor with arguments (functor version).
//
// This will start a new thread that will execute the specified function.
// ---------------------------------------------------------------------------
vjThreadPosix::vjThreadPosix (vjBaseThreadFunctor* functorPtr, long flags,
                              u_int priority, void* stack_addr,
                              size_t stack_size)  : mUserThreadFunctor(NULL)
{
    vjThreadManager* vj_tm_inst;

    mScope = VJ_THREAD_SCOPE;
    vj_tm_inst = vjThreadManager::instance();

    // Create the thread functor to start
    mUserThreadFunctor = functorPtr;
    vjThreadMemberFunctor<vjThreadPosix>* start_functor
                  = new vjThreadMemberFunctor<vjThreadPosix>(this,&vjThreadPosix::startThread,NULL);

    // Start thread
    // NOTE: Automagically registers thread UNLESS failure
    int ret_val = spawn(start_functor, flags, priority, stack_addr, stack_size);

    if(ret_val)
    {
       vj_tm_inst->lock();  // Need to lock thread manager before I register the thread with them
       {
         registerThread(false);
       }
       vj_tm_inst->unlock();
    }
}

// ---------------------------------------------------------------------------
// Destructor.
//
// PRE: None.
// POST: This thread is removed from the thread table and from the local
//       thread hash.
// ---------------------------------------------------------------------------
vjThreadPosix::~vjThreadPosix (void) {
    int status;

    delete mUserThreadFunctor;

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
vjThreadPosix::spawn (vjBaseThreadFunctor* functorPtr, long flags,
                      u_int priority, void* stack_addr, size_t stack_size)
{
    int ret_val;
    pthread_attr_t thread_attrs;

    // Initialize thread_attrs and set the priority of the thread if it is
    // supported.  HP-UX requires a slightly different syntax than other
    // operating systems.
#ifdef _PTHREADS_DRAFT_4
    pthread_attr_create(&thread_attrs);

#   ifdef _POSIX_THREAD_REALTIME_SCHEDULING
        if ( priority > 0 ) {
            pthread_attr_setprio(&thread_attrs, priority);
        }
#   endif   /* _POSIX_THREAD_REALTIME_SCHEDULING */

#else /* ! _PTHREADS_DRAFT_4 */
    sched_param_t prio_param;

    pthread_attr_init(&thread_attrs);

    // If this pthreads implementation supports it and an alternate address
    // is specified, set the stack address for the thread.
#   ifdef _POSIX_THREAD_ATTR_STACKADDR
        if ( stack_addr != NULL && stack_addr > 0 ) {
#ifdef HAVE_PTHREAD_ATTR_SETSTACK
            pthread_attr_setstack(&thread_attrs, stack_addr, stack_size);
#else
            pthread_attr_setstackaddr(&thread_attrs, stack_addr);
#endif
        }
#   endif   /* _POSIX_THREAD_ATTR_STACKADDR */

    // If thread priority scheduling is available, set the thread's priority
    // if it is set to be higher than 0.
#   ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
#   if defined(HAVE_SYS_CAPABILITY_H) && ! defined(VJ_OS_FreeBSD)
        cap_t capabilities = cap_get_proc();

        // If we have the capability to do so, set the scope of the threads
        // to system scope.
        if ( capabilities->cap_effective & CAP_SCHED_MGT ) {
            mScope = PTHREAD_SCOPE_SYSTEM;
        }
#   endif   /* HAVE_SYS_CAPABILITY_H */

        pthread_attr_setscope(&thread_attrs, mScope);

        if ( priority > 0 ) {
            prio_param.sched_priority = priority;
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
                             (pthread_startroutine_t) ThreadFunctorFunction,
                             (pthread_addr_t) functorPtr);
#else
    ret_val = pthread_create(&(mThread), &thread_attrs,
                             ThreadFunctorFunction, (void *) functorPtr);
#endif

    // Inform the caller if the thread was not created successfully.
    if ( ret_val != 0 ) {
        std::cerr << "vjThreadPosix::spawn() - Cannot create thread:"
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
void vjThreadPosix::startThread(void* null_param)
{
   // WE are a new thread... yeah!!!!
   // TELL EVERYONE THAT WE LIVE!!!!
   vjThreadManager::instance()->lock();      // Lock manager
   {
      threadIdKey().setspecific((void*)this);     // Store the pointer to me
      registerThread(true);
   }
   vjThreadManager::instance()->unlock();

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
vjThreadPosix::getPrio (int* prio) {
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
    int policy, ret_val;
    sched_param_t fifo_sched_param;

    ret_val = pthread_getschedparam(mThread, &policy, &fifo_sched_param);
    *prio = fifo_sched_param.sched_priority;

    return ret_val;
#else
    std::cerr << "vjThreadPosix::getPrio(): Not supported\n";

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
vjThreadPosix::setPrio (int prio) {
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
    sched_param_t fifo_sched_param;

    fifo_sched_param.sched_priority = prio;

    return pthread_setschedparam(mThread, SCHED_FIFO, &fifo_sched_param);
#else
    std::cerr << "vjThreadPosix::setPrio(): Not supported\n";

    return -1;
#endif
}

vjBaseThread*
vjThreadPosix::self (void)
{
   vjASSERT((statics.mStaticsInitialized==1221) && "Trying to call vjThreadPosix::self before statics are initialized. Don't do that");

   vjBaseThread* my_thread;
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
vjThreadPosix::checkRegister (int status) {
    if ( status == 0 ) {
       mThreadTable.addThread(this, hash());      // Store way to look me up
       threadIdKey().setspecific((void*)this);     // Store the pointer to me
       registerThread(true);
    } else {
        registerThread(false);   // Failed to create
    }
}
*/
