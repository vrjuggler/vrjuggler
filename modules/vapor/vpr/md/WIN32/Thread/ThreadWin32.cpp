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
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <assert.h>
#include <errno.h>
#include <boost/bind.hpp>

#include <vpr/Thread/ThreadManager.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/Assert.h>
// It is safe to include Debug.h in this file because Debug.h does not
// include vpr/Thread/Thread.h or ThreadWin32.h.
#include <vpr/Util/Debug.h>
#include <vpr/Util/Error.h>
#include <vpr/Util/IllegalArgumentException.h>
#include <vpr/Util/ResourceException.h>
#include <vpr/md/WIN32/Thread/ThreadWin32.h>


namespace vpr
{

ThreadWin32::staticWrapper ThreadWin32::statics;

// Non-spawning constructor.  This will not start a new thread.
ThreadWin32::ThreadWin32(VPRThreadPriority priority, VPRThreadScope,
                         VPRThreadState, size_t stackSize)
   : mRunning(false)
   , mThreadHandle(NULL)
   , mPriority(priority)
   , mStackSize(stackSize)
   , mException("No exception caught")
   , mCaughtException(false)
   , mThreadStartCompleted(false)
{
   /* Do nothing. */ ;
}

// Spawning constructor with arguments.  This will start a new thread that will
// execute the specified function.
ThreadWin32::ThreadWin32(const vpr::thread_func_t& func,
                         VPRThreadPriority priority, VPRThreadScope,
                         VPRThreadState, size_t stackSize)
   : mRunning(false)
   , mThreadHandle(NULL)
   , mPriority(priority)
   , mStackSize(stackSize)
   , mException("No exception caught")
   , mCaughtException(false)
   , mThreadStartCompleted(false)
{
   setFunctor(func);
   start();
}

ThreadWin32::~ThreadWin32()
{
   // TELL EVERYONE THAT WE'RE DEAD!!!!
   ThreadManager::instance()->lock();      // Lock manager
   {
      unregisterThread();                  // Finish thread deallocation
   }
   ThreadManager::instance()->unlock();
}

void ThreadWin32::setFunctor(const vpr::thread_func_t& functor)
{
   vprASSERT(! mRunning && "Thread already running.");
   vprASSERT(! functor.empty() && "Invalid functor.");

   mUserThreadFunctor = functor;
}

void ThreadWin32::start()
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
      mStartFunctor = boost::bind(&ThreadWin32::startThread, this);

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
void ThreadWin32::spawn()
{
   unsigned int thread_id;
   mThreadHandle =
      reinterpret_cast<HANDLE>(_beginthreadex(NULL, mStackSize,
                                              &vprThreadFunctorFunction,
                                              (void*) &mStartFunctor, 0,
                                              &thread_id));

   // Inform the caller if the thread was not created successfully.
   if ( NULL == mThreadHandle )
   {
      std::ostringstream msg_stream;
      msg_stream << "Cannot create thread: "
                 << vpr::Error::getCurrentErrorMsg();

      if ( EINVAL == errno )
      {
         throw vpr::IllegalArgumentException(msg_stream.str(), VPR_LOCATION);
      }
      else if ( EAGAIN == errno )
      {
         throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
      }
      else
      {
         throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
      }
   }
   else if ( VPR_PRIORITY_NORMAL != mPriority )
   {
      setPrio(mPriority);
   }

   mThreadTID = thread_id;
}

// Called by the spawn routine to start the user thread function.
void ThreadWin32::startThread()
{
   // WE are a new thread... yeah!!!!
   // TELL EVERYONE THAT WE LIVE!!!!
   ThreadManager::instance()->lock();      // Lock manager
   {
      threadIdKey().setspecific((void*) this); // Store the pointer to me
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

void ThreadWin32::join(void**)
{
   int result(0);
   result = WaitForSingleObject(mThreadHandle, INFINITE);
   assert(result == WAIT_OBJECT_0);
   result = CloseHandle(mThreadHandle);
   assert(result);
   boost::ignore_unused_variable_warning(result);

   if ( mCaughtException )
   {
      throw mException;
   }
}

void ThreadWin32::resume()
{
   if ( 1 != ResumeThread(mThreadHandle) )
   {
      // Throw an exception
   }
}

void ThreadWin32::suspend()
{
   if ( -1 == SuspendThread(mThreadHandle) )
   {
      // Throw an exception
   }
}

BaseThread::VPRThreadPriority ThreadWin32::getPrio() const
{
   const int priority = GetThreadPriority(mThreadHandle);

   if ( THREAD_PRIORITY_ERROR_RETURN == priority )
   {
      std::ostringstream msg_stream;
      msg_stream << "Failed to query thread priority: "
                 << vpr::Error::getCurrentErrorMsg();
      // XXX: Throwing this exception type is done only to be consistent with
      // the other implementations.
      throw vpr::IllegalArgumentException(msg_stream.str(), VPR_LOCATION);
   }

   return win32ThreadPriorityToVPR(priority);
}

void ThreadWin32::setPrio(const VPRThreadPriority prio)
{
   const BOOL result = SetThreadPriority(mThreadHandle,
                                         vprThreadPriorityToWin32(prio));

   if ( result == FALSE )
   {
      std::ostringstream msg_stream;
      msg_stream << "Failed to set thread priority to "
                 << vprThreadPriorityToWin32(prio) << ": "
                 << vpr::Error::getCurrentErrorMsg();
      // XXX: Throwing this exception type is done only to be consistent with
      // the other implementations.
      throw vpr::IllegalArgumentException(msg_stream.str(), VPR_LOCATION);
   }
}

void ThreadWin32::setRunOn(const unsigned int cpu)
{
   // Even though we have easy access to mThreadHandle, we perform this test
   // to ensure semantic compatibility with vpr::ThreadPosix.
   if ( ThreadWin32::self() == this )
   {
      // The value of cpu is zero-based, but the CPU identifier values from
      // Windows are one-based.
      DWORD_PTR result = SetThreadAffinityMask(mThreadHandle, 0x1 << cpu);

      if ( result == 0 )
      {
         std::ostringstream msg_stream;
         msg_stream << "Failed to set CPU affinity: "
                    << vpr::Error::getCurrentErrorMsg();
         throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
      }
   }
   else
   {
      throw vpr::IllegalArgumentException(
         "CPU affinity can only be set for a thread object from its thread",
         VPR_LOCATION
      );
   }
}

std::vector<unsigned int> ThreadWin32::getRunOn() const
{
   std::vector<unsigned int> cpus;

   if ( ThreadWin32::self() == this )
   {
      DWORD_PTR proc_affinity_mask;
      DWORD_PTR sys_affinity_mask;

      const BOOL result = GetProcessAffinityMask(mThreadHandle,
                                                 &proc_affinity_mask,
                                                 &sys_affinity_mask);

      if ( result )
      {
         const size_t cpu_count(sizeof(DWORD_PTR) * 8);
         cpus.reserve(cpu_count);

         // Find the first CPU in proc_affinity_mask on which this thread is
         // allowed to run.
         for ( int i = 0; i < cpu_count; ++i )
         {
            if ( (0x1 << i) & proc_affinity_mask )
            {
               cpus.push_back(i);
            }
         }
      }
      else
      {
      }
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

Thread* ThreadWin32::self()
{
   vprASSERT(statics.mStaticsInitialized == 1221 &&
             "Trying to call vpr::ThreadWin32::self before statics are initialized. Don't do that");

   Thread* my_thread;
   threadIdKey().getspecific((void**) &my_thread);

   return my_thread;
}

// Provides a way of printing the process ID neatly.
std::ostream& ThreadWin32::outStream(std::ostream& out)
{
   out.setf(std::ios::right);
   out << std::setw(7) << std::setfill('0') << getpid() << "/";
   out.unsetf(std::ios::right);
   BaseThread::outStream(out);
   out << std::setfill(' ');
   return out;
}

int ThreadWin32::vprThreadPriorityToWin32(const VPRThreadPriority priority)
   const
{
   int win32_prio(THREAD_PRIORITY_NORMAL);

   switch ( priority )
   {
      case VPR_PRIORITY_LOW:
         win32_prio = THREAD_PRIORITY_LOWEST;
         break;
      case VPR_PRIORITY_NORMAL:
         win32_prio = THREAD_PRIORITY_NORMAL;
         break;
      case VPR_PRIORITY_HIGH:
         win32_prio = THREAD_PRIORITY_HIGHEST;
         break;
      case VPR_PRIORITY_URGENT:
         win32_prio = THREAD_PRIORITY_TIME_CRITICAL;
         break;
   };

   return win32_prio;
}

BaseThread::VPRThreadPriority
ThreadWin32::win32ThreadPriorityToVPR(const int priority) const
{
   VPRThreadPriority vpr_prio;

   if ( THREAD_PRIORITY_NORMAL == priority )
   {
      vpr_prio = VPR_PRIORITY_NORMAL;
   }
   else if ( THREAD_PRIORITY_LOWEST == priority )
   {
      vpr_prio = VPR_PRIORITY_LOW;
   }
   else if ( THREAD_PRIORITY_TIME_CRITICAL == priority )
   {
      vpr_prio = VPR_PRIORITY_URGENT;
   }
   else
   {
      vpr_prio = VPR_PRIORITY_HIGH;
   }

   return vpr_prio;
}

} // End of vpr namespace
