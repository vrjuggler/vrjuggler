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

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

#ifdef HAVE_SYS_CAPABILITY_H
#  include <sys/capability.h>
#endif

#include <boost/concept_check.hpp>

#include <vpr/Thread/ThreadManager.h>
#include <vpr/Thread/ThreadFunctor.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/Assert.h>
#include <vpr/md/POSIX/Thread/ThreadPosix.h>


typedef struct sched_param sched_param_t;

namespace vpr
{

ThreadPosix::staticWrapper ThreadPosix::statics;


// Non-spawning constructor.  This will not start a new thread.
ThreadPosix::ThreadPosix(VPRThreadPriority priority, VPRThreadScope scope,
                         VPRThreadState state, size_t stackSize)
   : mUserThreadFunctor(NULL), mDeleteThreadFunctor(false), mRunning(false),
     mPriority(priority), mScope(scope), mState(state), mStackSize(stackSize),
     mThreadStartCompleted(false)
{
   /* Do nothing. */ ;
}

// Spawning constructor with arguments.  This will start a new thread that will
// execute the specified function.
ThreadPosix::ThreadPosix(thread_func_t func, void* arg,
                         VPRThreadPriority priority, VPRThreadScope scope,
                         VPRThreadState state, size_t stackSize)
   : mUserThreadFunctor(NULL), mDeleteThreadFunctor(false), mRunning(false),
     mPriority(priority), mScope(scope), mState(state), mStackSize(stackSize),
     mThreadStartCompleted(false)
{
   // Create the thread functor to start.  This will be deleted in the
   // destructor.
   setFunctor(new ThreadNonMemberFunctor(func, arg));
   mDeleteThreadFunctor = true;
   start();
}

// Spawning constructor.  This will start a new thread that will execute the
// specified function.
ThreadPosix::ThreadPosix(BaseThreadFunctor* functorPtr,
                         VPRThreadPriority priority, VPRThreadScope scope,
                         VPRThreadState state, size_t stackSize)
   : mUserThreadFunctor(NULL), mDeleteThreadFunctor(false), mRunning(false),
     mPriority(priority), mScope(scope), mState(state), mStackSize(stackSize),
     mThreadStartCompleted(false)
{
   setFunctor(functorPtr);
   start();
}

// Destructor.
ThreadPosix::~ThreadPosix()
{
   if ( mDeleteThreadFunctor )
   {
      delete mUserThreadFunctor;
   }
}

void ThreadPosix::setFunctor(BaseThreadFunctor* functorPtr)
{
   vprASSERT(! mRunning && "Thread already running.");
   vprASSERT(functorPtr->isValid() && "Invalid functor.");

   mUserThreadFunctor = functorPtr;
}

vpr::ReturnStatus ThreadPosix::start()
{
   vpr::ReturnStatus status;

   if ( mRunning )
   {
      vprASSERT(false && "Thread already started");
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
      ThreadMemberFunctor<ThreadPosix>* start_functor =
         new ThreadMemberFunctor<ThreadPosix>(this, &ThreadPosix::startThread,
                                              NULL);

      // Spawn the thread.  If the thread is spawned successfully, the method
      // startThread() will register the actual thread info.
      mThreadStartCompleted = false;  // Make sure this is set correctly
      status = spawn(start_functor);

      // Thread spawned successfully.
      if ( status.success() )
      {
         // startThread() will register the thread, so we wait for
         // registration to complete here.
         mThreadStartCondVar.acquire();
         {
            while ( ! mThreadStartCompleted )
            {
               mThreadStartCondVar.wait();
            }
         }
         mThreadStartCondVar.release();

         mRunning = true;
      }
      // Thread spawning failed.  Yikes!
      else
      {
         ThreadManager::instance()->lock();
         {
            registerThread(false);
         }
         ThreadManager::instance()->unlock();
      }
   }

   return status;
}

// Creates a new thread that will execute functorPtr.
vpr::ReturnStatus ThreadPosix::spawn(BaseThreadFunctor* functorPtr)
{
   vpr::ReturnStatus status;
   int ret_val;
   pthread_attr_t thread_attrs;

   int pthread_prio = vprThreadPriorityToPOSIX(mPriority);

   // Initialize thread_attrs and set the priority of the thread if it is
   // supported.  HP-UX requires a slightly different syntax than other
   // operating systems.
#ifdef _PTHREADS_DRAFT_4
   pthread_attr_create(&thread_attrs);

#  ifdef _POSIX_THREAD_REALTIME_SCHEDULING
   if ( pthread_prio > 0 )
   {
      pthread_attr_setprio(&thread_attrs, pthread_prio);
   }
#  endif   /* _POSIX_THREAD_REALTIME_SCHEDULING */

#else /* ! _PTHREADS_DRAFT_4 */
   sched_param_t prio_param;

   pthread_attr_init(&thread_attrs);

   // If thread priority scheduling is available, set the thread's priority
   // if it is set to be higher than 0.
#  ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
   int thread_scope = vprThreadScopeToPOSIX(mScope);

#  if defined(HAVE_SYS_CAPABILITY_H) && ! defined(VPR_OS_FreeBSD) && \
      ! defined(VPR_OS_Linux)
   cap_t capabilities = cap_get_proc();

   // If we have the capability to do so, set the scope of the threads
   // to system scope.
   if ( capabilities->cap_effective & CAP_SCHED_MGT )
   {
      thread_scope = PTHREAD_SCOPE_SYSTEM;
   }
#  endif   /* HAVE_SYS_CAPABILITY_H */

   pthread_attr_setscope(&thread_attrs, thread_scope);

   if ( pthread_prio > 0 )
   {
      prio_param.sched_priority = pthread_prio;
      pthread_attr_setschedparam(&thread_attrs, &prio_param);
   }
#  endif   /* _POSIX_THREAD_PRIORITY_SCHEDULING */

#endif   /* _PTHREADS_DRAFT_4 */

   // Set the stack size if a value greater than 0 is specified and this
   // pthreads implementation supports it.  Ensure that
   // _POSIX_THREAD_ATTR_STACKSIZE is defined before trying to test its
   // value.
#ifdef _POSIX_THREAD_ATTR_STACKSIZE
   if ( mStackSize > 0 )
   {
      // XXX: ** STACK SIZE CHECK NEEDED **

      pthread_attr_setstacksize(&thread_attrs, mStackSize);
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
   if ( ret_val != 0 )
   {
      status.setCode(vpr::ReturnStatus::Fail);
      std::cerr << "vpr::ThreadPosix::spawn() - Cannot create thread:"
                << strerror(ret_val) << std::endl;
   }

   return status;
}

/**
 * Called by the spawn routine to start the user thread function.
 *
 * @pre Called ONLY by a new thread
 * @post Do any thread registration necessary
 *       Call the user thread functor
 *
 * @param null_param
 */
void ThreadPosix::startThread(void* null_param)
{
   boost::ignore_unused_variable_warning(null_param);

   // WE are a new thread... yeah!!!!
   // TELL EVERYONE THAT WE LIVE!!!!
   ThreadManager::instance()->lock();      // Lock manager
   {
      threadIdKey().setspecific((void*)this);  // Store the pointer to me
      registerThread(true);                    // Finish thread initialization
   }
   ThreadManager::instance()->unlock();

   // Signal that thread registration is complete.
   mThreadStartCondVar.acquire();
   {
      mThreadStartCompleted = true;
      mThreadStartCondVar.signal();
   }
   mThreadStartCondVar.release();

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
int ThreadPosix::getPrio (VPRThreadPriority* prio)
{
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
int ThreadPosix::setPrio (VPRThreadPriority prio)
{
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
   sched_param_t fifo_sched_param;

   fifo_sched_param.sched_priority = prio;

   return pthread_setschedparam(mThread, SCHED_FIFO, &fifo_sched_param);
#else
   boost::ignore_unused_variable_warning(prio);
   std::cerr << "vpr::ThreadPosix::setPrio(): Not supported\n";

   return -1;
#endif
}

int ThreadPosix::setRunOn(int cpu)
{
#ifdef VPR_OS_IRIX
   int ret_val;

   if ( mScope == PTHREAD_SCOPE_SYSTEM )
   {
      ret_val = pthread_setrunon_np(cpu);
   }
   else
   {
      std::cerr << "This thread is not a system-scope thread!\n";
      ret_val = -1;
   }

   return ret_val;
#else
   boost::ignore_unused_variable_warning(cpu);
   std::cerr << "vpr::ThreadPosix::setRunOn(): Not available on this system.\n";

   return -1;
#endif
}

int ThreadPosix::getRunOn(int* cur_cpu)
{
#ifdef VPR_OS_IRIX
   int ret_val;

   if ( mScope == PTHREAD_SCOPE_SYSTEM )
   {
      ret_val = pthread_getrunon_np(cur_cpu);
   }
   else
   {
      std::cerr << "This thread is not a system-scope thread!\n";
      ret_val = -1;
   }

   return ret_val;
#else
   boost::ignore_unused_variable_warning(cur_cpu);
   std::cerr << "vpr::ThreadPosix::getRunOn(): Not available on this system.\n";

   return -1;
#endif
}

int ThreadPosix::kill(int signum)
{
#ifdef _PTHREADS_DRAFT_4
   std::cerr << "vpr::ThreadPosix::kill(): Signals cannot be sent to threads "
             << "with this POSIX threads implementation.\n";

   return -1;
#else
   return pthread_kill(mThread, signum);
#endif
}

BaseThread* ThreadPosix::self()
{
   vprASSERT((statics.mStaticsInitialized==1221) && "Trying to call vpr::ThreadPosix::self before statics are initialized. Don't do that");

   BaseThread* my_thread;
   threadIdKey().getspecific((void**)&my_thread);

   return my_thread;
}

/// Provides a way of printing the process ID neatly.
std::ostream& ThreadPosix::outStream(std::ostream& out)
{
   out.setf(std::ios::right);
   out << std::setw(7) << std::setfill('0') << getpid() << "/";
   out.unsetf(std::ios::right);
   BaseThread::outStream(out);
   out << std::setfill(' ');
   return out;
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
void ThreadPosix::checkRegister (int status)
{
   if ( status == 0 )
   {
      mThreadTable.addThread(this, hash());      // Store way to look me up
      threadIdKey().setspecific((void*)this);    // Store the pointer to me
      registerThread(true);
   }
   else
   {
      registerThread(false);   // Failed to create
   }
}
*/

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
int ThreadPosix::vprThreadPriorityToPOSIX (const VPRThreadPriority priority)
{
   int posix_prio;
   int min_prio, max_prio;

   min_prio = sched_get_priority_min(SCHED_OTHER);
   max_prio = sched_get_priority_max(SCHED_OTHER);

   switch ( priority )
   {
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
      default:
         posix_prio = min_prio;
         break;
   };

   return posix_prio;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
int ThreadPosix::vprThreadScopeToPOSIX (const VPRThreadScope scope)
{
   int posix_scope;

   switch ( scope )
   {
      case VPR_LOCAL_THREAD:
         posix_scope = PTHREAD_SCOPE_PROCESS;
         break;
      case VPR_GLOBAL_THREAD:
         posix_scope = PTHREAD_SCOPE_SYSTEM;
         break;
      default:
         posix_scope = VPR_THREAD_SCOPE;
         break;
   };

   return posix_scope;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
int ThreadPosix::vprThreadStateToPOSIX (const VPRThreadState state)
{
   int posix_state;

   switch ( state )
   {
      case VPR_JOINABLE_THREAD:
         posix_state = PTHREAD_CREATE_JOINABLE;
         break;
      case VPR_UNJOINABLE_THREAD:
         posix_state = PTHREAD_CREATE_DETACHED;
         break;
      default:
         posix_state = PTHREAD_CREATE_JOINABLE;
         break;
   };

   return posix_state;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
BaseThread::VPRThreadPriority ThreadPosix::posixThreadPriorityToVPR (const int priority)
{
   VPRThreadPriority vpr_prio;
   int min_prio, max_prio;

   min_prio = sched_get_priority_min(SCHED_OTHER);
   max_prio = sched_get_priority_max(SCHED_OTHER);

   if ( priority == min_prio )
   {
      vpr_prio = VPR_PRIORITY_NORMAL;
   }
   else if ( priority == max_prio )
   {
      vpr_prio = VPR_PRIORITY_URGENT;
   }
   else
   {
      vpr_prio = VPR_PRIORITY_HIGH;
   }

   return vpr_prio;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
BaseThread::VPRThreadScope ThreadPosix::posixThreadScopeToVPR (const int scope)
{
   VPRThreadScope vpr_scope;

   switch ( scope )
   {
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
BaseThread::VPRThreadState ThreadPosix::posixThreadStateToVPR (const int state)
{
   VPRThreadState vpr_state;

   switch ( state )
   {
      case PTHREAD_CREATE_JOINABLE:
         vpr_state = VPR_JOINABLE_THREAD;
         break;
      case PTHREAD_CREATE_DETACHED:
         vpr_state = VPR_UNJOINABLE_THREAD;
         break;
   };

   return vpr_state;
}

thread_id_t ThreadPosix::hash()
{
#if defined(VPR_OS_IRIX)
   return mThread;
#else

   // This works on Linux, Solaris and FreeBSD.
   return(thread_id_t) mThread;
#endif    /* VPR_OS_IRIX */
}

thread_id_t ThreadPosix::hash(pthread_t thread)
{
#ifdef VPR_OS_IRIX
   return thread;
#else

   // This works on Linux, Solaris and FreeBSD.
   return(thread_id_t) thread;
#endif
}

} // End of vpr namespace
