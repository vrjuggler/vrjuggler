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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <cstring>
#include <iomanip>
#include <sstream>
#include <typeinfo>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

#ifdef HAVE_SYS_CAPABILITY_H
#  include <sys/capability.h>
#endif

#include <boost/concept_check.hpp>
#include <boost/bind.hpp>

#include <vpr/Thread/ThreadManager.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/Assert.h>
// It is safe to include Debug.h in this file because Debug.h does not
// include vpr/Thread/Thread.h or ThreadPosix.h.
#include <vpr/Util/Debug.h>
#include <vpr/Util/Error.h>
#include <vpr/Util/IllegalArgumentException.h>
#include <vpr/Util/ResourceException.h>
#include <vpr/Sync/DeadlockException.h>
#include <vpr/md/POSIX/Thread/ThreadPosix.h>


typedef struct sched_param sched_param_t;

namespace vpr
{

ThreadPosix::staticWrapper ThreadPosix::statics;


// Non-spawning constructor.  This will not start a new thread.
ThreadPosix::ThreadPosix(VPRThreadPriority priority, VPRThreadScope scope,
                         VPRThreadState state, size_t stackSize)
   : mRunning(false)
   , mPriority(priority)
   , mScope(scope)
   , mState(state)
   , mStackSize(stackSize)
   , mException("No exception caught")
   , mCaughtException(false)
   , mThreadStartCompleted(false)
{
   /* Do nothing. */ ;
}

// Spawning constructor with arguments.  This will start a new thread that will
// execute the specified function.
ThreadPosix::ThreadPosix(const vpr::thread_func_t& func,
                         VPRThreadPriority priority,
                         VPRThreadScope scope, VPRThreadState state,
                         size_t stackSize)
   : mRunning(false)
   , mPriority(priority)
   , mScope(scope)
   , mState(state)
   , mStackSize(stackSize)
   , mException("No exception caught")
   , mCaughtException(false)
   , mThreadStartCompleted(false)
{
   setFunctor(func);
   start();
}

// Destructor.
ThreadPosix::~ThreadPosix()
{
   // TELL EVERYONE THAT WE'RE DEAD!!!!
   ThreadManager::instance()->lock();      // Lock manager
   {
      unregisterThread();                  // Finish thread deallocation
   }
   ThreadManager::instance()->unlock();
}

void ThreadPosix::setFunctor(const vpr::thread_func_t& functor)
{
   vprASSERT(! mRunning && "Thread already running.");
   vprASSERT(! functor.empty() && "Invalid functor.");

   mUserThreadFunctor = functor;
}

void ThreadPosix::start()
{
   if ( mRunning )
   {
      throw vpr::Exception("Thread already started", VPR_LOCATION);
   }
   else if ( mUserThreadFunctor.empty() )
   {
      throw vpr::IllegalArgumentException("No functor set", VPR_LOCATION);
   }
   else
   {
      // Spawn the thread.  If the thread is spawned successfully, the method
      // startThread() will register the actual thread info.
      mThreadStartCompleted = false;  // Make sure this is set correctly
      mStartFunctor = boost::bind(&ThreadPosix::startThread, this);

      try
      {
         spawn();

         // The thread spawned successfully.
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
      catch (vpr::Exception& ex)
      {
         ThreadManager::instance()->lock();
         {
            registerThread(false);
         }
         ThreadManager::instance()->unlock();

         // Rethrow the exception.
         throw;
      }
   }
}

// Creates a new thread that will execute functorPtr.
void ThreadPosix::spawn()
{
   pthread_attr_t thread_attrs;

   const int pthread_prio = vprThreadPriorityToPOSIX(mPriority);

   // Initialize thread_attrs and set the priority of the thread if it is
   // supported.
   sched_param_t prio_param;

   pthread_attr_init(&thread_attrs);
   pthread_attr_setdetachstate(&thread_attrs, vprThreadStateToPOSIX(mState));

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
   const int ret_val = pthread_create(&(mThread), &thread_attrs,
                                      vprThreadFunctorFunction,
                                      (void *) &mStartFunctor);

   // Inform the caller if the thread was not created successfully.
   if ( ret_val != 0 )
   {
      std::ostringstream msg_stream;
      msg_stream << "Cannot create thread: " << std::strerror(ret_val);

      if ( EINVAL == ret_val )
      {
         throw vpr::IllegalArgumentException(msg_stream.str(), VPR_LOCATION);
      }
      else if ( EAGAIN == ret_val )
      {
         throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
      }
      else
      {
         throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
      }
   }
}

// Called by the spawn routine to start the user thread function.
void ThreadPosix::startThread()
{
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

   try
   {
      emitThreadStart(this);

      // --- CALL USER FUNCTOR --- //
      mUserThreadFunctor();

      emitThreadExit(this);
   }
   catch (std::exception& ex)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutNORM(clrYELLOW, "WARNING:")
         << " Caught an unhandled exception of type " << typeid(ex).name()
         << " in thread:" << std::endl
         << ex.what() << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "Thread exiting due to uncaught exception\n" << vprDEBUG_FLUSH;

      mCaughtException = true;
      mException.setException(ex);
   }
}

void ThreadPosix::join(void** status)
{
   const int result = pthread_join(mThread, status);

   if ( EINVAL == result )
   {
      throw vpr::IllegalArgumentException("Cannot join an unjoinable thread",
                                          VPR_LOCATION);
   }
   else if ( ESRCH == result )
   {
      throw vpr::IllegalArgumentException("Cannot join an invalid thread",
                                          VPR_LOCATION);
   }
   else if ( EDEADLK == result )
   {
      throw vpr::DeadlockException("Deadlock detected when joining thread",
                                   VPR_LOCATION);
   }

   vprASSERT(result == 0);

   if ( mCaughtException )
   {
      throw mException;
   }
}

// Get this thread's priority.
BaseThread::VPRThreadPriority ThreadPosix::getPrio() const
{
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
   int policy;
   sched_param_t fifo_sched_param;

   const int result = pthread_getschedparam(mThread, &policy,
                                            &fifo_sched_param);

   if ( ESRCH == result )
   {
      throw vpr::IllegalArgumentException(
         "Cannot query priority for invalid thread", VPR_LOCATION
      );
   }

   vprASSERT(result == 0);

   return posixThreadPriorityToVPR(fifo_sched_param.sched_priority);
#else
   std::cerr << "vpr::ThreadPosix::getPrio(): Not supported\n";

   return VPR_PRIORITY_NORMAL;
#endif
}

// Set this thread's priority.
void ThreadPosix::setPrio(VPRThreadPriority prio)
{
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
   sched_param_t sched_param;
   sched_param.sched_priority = vprThreadPriorityToPOSIX(prio);

   const int result = pthread_setschedparam(mThread, SCHED_RR, &sched_param);

   if ( EINVAL == result || ENOTSUP == result )
   {
      std::ostringstream msg_stream;
      msg_stream << "Invalid priority value " << sched_param.sched_priority;
      throw vpr::IllegalArgumentException(msg_stream.str(), VPR_LOCATION);
   }
   else if ( ESRCH == result )
   {
      throw vpr::IllegalArgumentException(
         "Cannot set priority for invalid thread", VPR_LOCATION
      );
   }

   vprASSERT(result == 0);
#else
   boost::ignore_unused_variable_warning(prio);
   std::cerr << "vpr::ThreadPosix::setPrio(): Not supported\n";
#endif
}

void ThreadPosix::setRunOn(const int cpu)
{
   if ( ThreadPosix::self() == this )
   {
      if ( cpu < 0 )
      {
         std::ostringstream msg_stream;
         msg_stream << "Illegal CPU identifier " << cpu << " is less than 0";
         throw vpr::IllegalArgumentException(msg_stream.str(), VPR_LOCATION);
      }

#if defined(VPR_OS_IRIX)
      if ( mScope == VPR_GLOBAL_THREAD )
      {
         pthread_setrunon_np(cpu);
      }
      else
      {
         throw vpr::IllegalArgumentException(
            "This thread is not a system-scope thread", VPR_LOCATION
         );
      }
#elif defined(VPR_OS_Linux)
      if ( sysconf(_SC_NPROCESSORS_CONF) > cpu )
      {
         const pid_t thread_id(syscall(__NR_gettid));

         if ( thread_id )
         {
            cpu_set_t cpu_mask;
            CPU_ZERO(&cpu_mask);
            CPU_SET(cpu, &cpu_mask);
            const int result = sched_setaffinity(thread_id, sizeof(cpu_mask),
                                                 &cpu_mask);

            if ( result != 0 )
            {
               std::ostringstream msg_stream;
               msg_stream << "Failed to set CPU affinity: "
                          << vpr::Error::getCurrentErrorMsg();
               throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
            }
         }
      }
#else
      boost::ignore_unused_variable_warning(cpu);
      std::cerr << "vpr::ThreadPosix::setRunOn(): Not available on this "
                << "operating system!\n";
#endif
   }
   else
   {
      throw vpr::IllegalArgumentException(
         "CPU affinity can only be set for a thread object from its thread",
         VPR_LOCATION
      );
   }
}

std::vector<unsigned int> ThreadPosix::getRunOn() const
{
   std::vector<unsigned int> cpus;

   if ( ThreadPosix::self() == this )
   {
#if defined(VPR_OS_IRIX)
      if ( mScope == VPR_GLOBAL_THREAD )
      {
         int cpu;
         if ( pthread_getrunon_np(&cpu) == 0 )
         {
            cpus.push_back(cpu);
         }
         else
         {
            std::ostringstream msg_stream;
            msg_stream << "Failed to query CPU affinity: "
                       << Error::getCurrentErrorMsg();
            throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
         }
      }
      else
      {
         throw vpr::IllegalArgumentException(
            "This thread is not a system-scope thread", VPR_LOCATION
         );
      }
#elif defined(VPR_OS_Linux)
      const pid_t thread_id(syscall(__NR_gettid));

      if ( thread_id )
      {
         cpu_set_t cpu_mask;
         CPU_ZERO(&cpu_mask);
         const int result = sched_getaffinity(thread_id, sizeof(cpu_mask),
                                              &cpu_mask);

         if ( result == 0 )
         {
            const long cpu_count(sysconf(_SC_NPROCESSORS_CONF));
            cpus.reserve(cpu_count);

            for ( int i = 0; i < cpu_count; ++i )
            {
               if ( CPU_ISSET(i, &cpu_mask) )
               {
                  cpus.push_back(i);
               }
            }
         }
         else
         {
            std::ostringstream msg_stream;
            msg_stream << "Failed to query CPU affinity: "
                       << Error::getCurrentErrorMsg();
            throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
         }
      }
#else
      std::cerr << "vpr::ThreadPosix::getRunOn(): Not available on this "
                << "operating system.\n";
#endif
   }
   else
   {
      throw vpr::IllegalArgumentException(
         "CPU affinity can only be queired for a thread object from its thread",
         VPR_LOCATION
      );
   }

   return cpus;
}

void ThreadPosix::kill(const int signum)
{
   const int result = pthread_kill(mThread, signum);

   if ( ESRCH == result )
   {
      throw vpr::IllegalArgumentException("Cannot kill an invalid thread",
                                          VPR_LOCATION);
   }
   else if ( EINVAL == result )
   {
      std::ostringstream msg_stream;
      msg_stream << "Invalid signal number " << signum;
      throw vpr::IllegalArgumentException(msg_stream.str(), VPR_LOCATION);
   }

   vprASSERT(result == 0);
}

Thread* ThreadPosix::self()
{
   vprASSERT((statics.mStaticsInitialized==1221) && "Trying to call vpr::ThreadPosix::self before statics are initialized. Don't do that");

   Thread* my_thread;
   threadIdKey().getspecific((void**)&my_thread);

   return my_thread;
}

// Provides a way of printing the process ID neatly.
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

int ThreadPosix::vprThreadPriorityToPOSIX(const VPRThreadPriority priority)
   const
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

int ThreadPosix::vprThreadScopeToPOSIX(const VPRThreadScope scope) const
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

int ThreadPosix::vprThreadStateToPOSIX(const VPRThreadState state) const
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

BaseThread::VPRThreadPriority ThreadPosix::
posixThreadPriorityToVPR(const int priority) const
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

BaseThread::VPRThreadScope ThreadPosix::posixThreadScopeToVPR(const int scope)
   const
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
      default:
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << clrOutNORM(clrYELLOW, "WARNING:")
            << " Unexpected value " << scope
            << " in vpr::ThreadPosix::posixThreadScopeToVPR()" << std::endl
            << vprDEBUG_FLUSH;
#if VPR_THREAD_SCOPE == PTHREAD_SCOPE_PROCESS
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Defaulting to VPR_LOCAL_THREAD for return value." << std::endl
            << vprDEBUG_FLUSH;
         vpr_scope = VPR_LOCAL_THREAD;
#else
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Defaulting to VPR_GLOBAL_THREAD for return value." << std::endl
            << vprDEBUG_FLUSH;
         vpr_scope = VPR_GLOBAL_THREAD;
#endif
         break;
   };

   return vpr_scope;
}

BaseThread::VPRThreadState ThreadPosix::posixThreadStateToVPR(const int state)
   const
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
      default:
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << clrOutNORM(clrYELLOW, "WARNING:")
            << " Unexpected value " << state
            << " in vpr::ThreadPosix::posixThreadStateToVPR()" << std::endl
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Defaulting to VPR_JOINABLE_THREAD for return value."
            << std::endl << vprDEBUG_FLUSH;
         vpr_state = VPR_JOINABLE_THREAD;
         break;
   };

   return vpr_state;
}


} // End of vpr namespace
