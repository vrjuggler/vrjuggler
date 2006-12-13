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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <iomanip>
#include <sstream>
#include <typeinfo>
#include <boost/bind.hpp>

#include <vpr/Util/Assert.h>
#include <vpr/Util/IllegalArgumentException.h>
#include <vpr/Util/ResourceException.h>
// It is safe to include Debug.h in this file because Debug.h does not
// include vpr/Thread/Thread.h or ThreadNSPR.h.
#include <vpr/Util/Debug.h>
#include <vpr/md/NSPR/NSPRHelpers.h>
#include <vpr/md/NSPR/Thread/ThreadNSPR.h>


namespace vpr
{

ThreadNSPR::staticWrapper ThreadNSPR::statics;    // Initialize the static data
PRUint32 ThreadNSPR::mTicksPerSec = PR_TicksPerSecond();

// Non-spawning constructor.  This will not start a thread.
ThreadNSPR::ThreadNSPR(VPRThreadPriority priority, VPRThreadScope scope,
                       VPRThreadState state, PRUint32 stackSize)
   : mThread(NULL)
   , mPriority(priority)
   , mScope(scope)
   , mState(state)
   , mStackSize(stackSize)
   , mException("No exception caught")
   , mCaughtException(false)
{
}

// Spawning constructor with arguments.  This will start a new thread that
// will execute the specified function.
ThreadNSPR::ThreadNSPR(const vpr::thread_func_t& func,
                       VPRThreadPriority priority, VPRThreadScope scope,
                       VPRThreadState state, PRUint32 stackSize)
   : mThread(NULL)
   , mPriority(priority)
   , mScope(scope)
   , mState(state)
   , mStackSize(stackSize)
   , mException("No exception caught")
   , mCaughtException(false)
{
   setFunctor(func);
   start();
}

// Destructor.
ThreadNSPR::~ThreadNSPR()
{
   ThreadManager::instance()->lock();
   {
      unregisterThread();
   }
   ThreadManager::instance()->unlock();
}

void ThreadNSPR::setFunctor(const vpr::thread_func_t& functor)
{
   vprASSERT(mThread == NULL && "Thread already running");
   vprASSERT(! functor.empty());

   mUserThreadFunctor = functor;
}

// Creates a new thread that will execute mUserFunctorPtr.
void ThreadNSPR::start()
{
   if ( NULL != mThread )
   {
      throw vpr::Exception("Thread already started", VPR_LOCATION);
   }
   else if ( mUserThreadFunctor.empty() )
   {
      throw vpr::IllegalArgumentException("No functor set", VPR_LOCATION);
   }
   else
   {
      PRThreadPriority nspr_prio;
      PRThreadScope nspr_scope;
      PRThreadState nspr_state;

      nspr_prio  = vprThreadPriorityToNSPR(mPriority);
      nspr_scope = vprThreadScopeToNSPR(mScope);
      nspr_state = vprThreadStateToNSPR(mState);

      vprASSERT(! mUserThreadFunctor.empty());

      // Store the member functor and create the functor for spawning to our
      // start routine.
      mStartFunctor = boost::bind(&ThreadNSPR::startThread, this);

      // Finally create the thread.
      // - On success --> The start method registers the actual thread info
      mThreadStartCompleted = false;      // Initialize registration flag (uses cond var for this)
      PRThread* ret_thread =
         PR_CreateThread(PR_USER_THREAD, vprThreadFunctorFunction,
                         (void*) &mStartFunctor, nspr_prio, nspr_scope,
                         nspr_state, (PRUint32) mStackSize);

      // Inform the caller if the thread was not created successfully.
      if ( NULL == ret_thread )
      {
         ThreadManager::instance()->lock();
         {
            registerThread(false);
         }
         ThreadManager::instance()->unlock();

         std::ostringstream msg_stream;
         NSPR_PrintError("Cannot create thread", msg_stream);
         throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
      }
      else
      {
         // start thread will register the thread, so let's wait for it
         // -- Wait for registration to complete
         mThreadStartCondVar.acquire();
         {
            // While not desired state (ie. register completed)
            while ( !mThreadStartCompleted )
            {
               mThreadStartCondVar.wait();
            }
         }
         mThreadStartCondVar.release();
         // ASSERT: Thread has completed registration
         vprASSERT(NULL != mThread && "Thread registration failed");
         vprASSERT(-1 != getTID() && "Thread id is invalid for successful thread");
      }
   }
}

void ThreadNSPR::join(void**)
{
   const PRStatus result = PR_JoinThread(mThread);

   if ( PR_FAILURE == result )
   {
      throw vpr::IllegalArgumentException("Failed to join invalid thread",
                                          VPR_LOCATION);
   }

   if ( mCaughtException )
   {
      throw mException;
   }
}

Thread* ThreadNSPR::self()
{
   vprASSERT((statics.mStaticsInitialized==1221) && "Trying to call vpr::ThreadNSPR::self before statics are initialized. Don't do that");

   Thread* my_thread;
   threadIdKey().getspecific((void**)&my_thread);

   return my_thread;
}

std::ostream& Thread::outStream(std::ostream& out)
{
   out.setf(std::ios::right);
   out << std::setw(7) << std::setfill('0')
#ifdef VPR_OS_Windows
       << _getpid()
#else
       << getpid()
#endif
       << "/";
   out.unsetf(std::ios::right);
   BaseThread::outStream(out);
   out << std::setfill(' ');
   return out;
}

/**
 * Helper method Called by the spawn routine to start the user thread function.
 *
 * @pre Called ONLY by a new thread
 * @post Do any thread registration necessary
 *       Call the user thread functor
 *
 * @param null_param
 */
void ThreadNSPR::startThread()
{
   // WE are a new thread... yeah!!!!
   // TELL EVERYONE THAT WE LIVE!!!!
   mThread = PR_GetCurrentThread();                   // Set the identity of the thread
   vprASSERT(NULL != mThread && "Invalid thread");    // We should not be able to have a NULL thread
   ThreadManager::instance()->lock();                 // Lock manager
   {
      threadIdKey().setspecific((void*)this);         // Store self in thread local data
      registerThread(true);                           // Finish thread initialization
   }
   ThreadManager::instance()->unlock();

   // Signal that registration is completed
   mThreadStartCondVar.acquire();
   {
      mThreadStartCompleted = true;
      mThreadStartCondVar.signal();
   }
   mThreadStartCondVar.release();

   try
   {
      // --- CALL USER FUNCTOR --- //
      mUserThreadFunctor();
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

BaseThread::VPRThreadPriority ThreadNSPR::getPrio()
{
   return nsprThreadPriorityToVPR(PR_GetThreadPriority(mThread));
}

// Set this thread's priority.
void ThreadNSPR::setPrio(const VPRThreadPriority prio)
{
   PR_SetThreadPriority(mThread, vprThreadPriorityToNSPR(prio));
}

// ===========================================================================
// Private methods follow.
// ===========================================================================


PRThreadPriority ThreadNSPR::vprThreadPriorityToNSPR(const VPRThreadPriority priority)
{
   PRThreadPriority nspr_prio;

   switch ( priority )
   {
      case VPR_PRIORITY_LOW:
         nspr_prio = PR_PRIORITY_LOW;
         break;
      case VPR_PRIORITY_NORMAL:
         nspr_prio = PR_PRIORITY_NORMAL;
         break;
      case VPR_PRIORITY_HIGH:
         nspr_prio = PR_PRIORITY_HIGH;
         break;
      case VPR_PRIORITY_URGENT:
         nspr_prio = PR_PRIORITY_URGENT;
         break;
   };

   return nspr_prio;
}

PRThreadScope ThreadNSPR::vprThreadScopeToNSPR(const VPRThreadScope scope)
{
   PRThreadScope nspr_scope;

   switch ( scope )
   {
      case VPR_LOCAL_THREAD:
         nspr_scope = PR_LOCAL_THREAD;
         break;
      case VPR_GLOBAL_THREAD:
         nspr_scope = PR_GLOBAL_THREAD;
         break;
   };

   return nspr_scope;
}

PRThreadState ThreadNSPR::vprThreadStateToNSPR(const VPRThreadState state)
{
   PRThreadState nspr_state;

   switch ( state )
   {
      case VPR_JOINABLE_THREAD:
         nspr_state = PR_JOINABLE_THREAD;
         break;
      case VPR_UNJOINABLE_THREAD:
         nspr_state = PR_UNJOINABLE_THREAD;
         break;
   };

   return nspr_state;
}

BaseThread::VPRThreadPriority ThreadNSPR::nsprThreadPriorityToVPR(const PRThreadPriority priority)
{
   VPRThreadPriority vpr_prio;

   switch ( priority )
   {
      case PR_PRIORITY_LOW:
         vpr_prio = VPR_PRIORITY_LOW;
         break;
      case PR_PRIORITY_NORMAL:
         vpr_prio = VPR_PRIORITY_NORMAL;
         break;
      case PR_PRIORITY_HIGH:
         vpr_prio = VPR_PRIORITY_HIGH;
         break;
      case PR_PRIORITY_URGENT:
         vpr_prio = VPR_PRIORITY_URGENT;
         break;
   };

   return vpr_prio;
}

BaseThread::VPRThreadScope ThreadNSPR::nsprThreadScopeToVPR(const PRThreadScope scope)
{
   VPRThreadScope vpr_scope;

   switch ( scope )
   {
      case PR_LOCAL_THREAD:
         vpr_scope = VPR_LOCAL_THREAD;
         break;
      case PR_GLOBAL_THREAD:
      case PR_GLOBAL_BOUND_THREAD:
         vpr_scope = VPR_GLOBAL_THREAD;
         break;
   };

   return vpr_scope;
}

BaseThread::VPRThreadState ThreadNSPR::nsprThreadStateToVPR(const PRThreadState state)
{
   VPRThreadState vpr_state;

   switch ( state )
   {
      case PR_JOINABLE_THREAD:
         vpr_state = VPR_JOINABLE_THREAD;
         break;
      case PR_UNJOINABLE_THREAD:
         vpr_state = VPR_UNJOINABLE_THREAD;
         break;
   };

   return vpr_state;
}

} // End of vpr namespace
