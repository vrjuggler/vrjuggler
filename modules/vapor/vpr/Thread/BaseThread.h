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

#ifndef _VPR_BASE_THREAD_H_
#define _VPR_BASE_THREAD_H_

#include <vpr/vprConfig.h>

#include <boost/concept_check.hpp>
#include <boost/function.hpp>
#include <vpr/vprTypes.h>
#include <vpr/Thread/TSTable.h>            /* Needed to cache a copy here */

namespace vpr
{

/**
 * Typedef to help with cross-platform abilities.  This type is the basic
 * function pointer type for all functions initially passed as code to
 * execute within a separate thread of control.
 */
#if defined(_MSC_VER) && _MSC_VER <= 1300
typedef boost::function0<void> thread_func_t;
#else
typedef boost::function<void()> thread_func_t;
#endif

/** \class BaseThread BaseThread.h vpr/Thread/BaseThread.h
 *
 * Base class for all thread implementations.
 *
 * Provides functionality that is common to all threading implementations.
 *
 * @note This class is not designed to be used as a polymorphic base class
 * to hold references to threads.  Just use the real thread type.
 */
class VPR_CLASS_API BaseThread
{
public:
   enum VPRThreadPriority
   {
      VPR_PRIORITY_LOW,      /**< The lowest possible priority */
      VPR_PRIORITY_NORMAL,   /**< The most common priority */
      VPR_PRIORITY_HIGH,     /**< Slightly higher priority */
      VPR_PRIORITY_URGENT    /**< The highest priority */
   };

   enum VPRThreadScope
   {
      VPR_LOCAL_THREAD,
      VPR_GLOBAL_THREAD
   };

   enum VPRThreadState
   {
      VPR_JOINABLE_THREAD,   /**< The thread can be reattached later */
      VPR_UNJOINABLE_THREAD  /**< The thread cannot be attached with join() */
   };

   BaseThread() : mThreadId(-1)
   {
      ;
   }

   virtual ~BaseThread()
   {
      ;
   }

   /** Sets the thread's functor--the code that will get executed. */
   virtual void setFunctor(const vpr::thread_func_t& functor) = 0;

   /** Starts the thread's execution. */
   virtual vpr::ReturnStatus start() = 0;

public:     // Thread specific data caching
   /**
    * Gets the Thread specific data table.
    *
    * @note Users should NOT access the table directly instead, use
    *       vpr::TSObjectProxy objects.
    */
   TSTable* getTSTable()
   {
      return &mTSTable;
   }

   /**
    * Get the Thread the global thread specific data table.
    * This table is shared by all threads that were not created by VPR.
    */
   static TSTable* getGlobalTSTable()
   {
      return &gTSTable;
   }

private:
   TSTable        mTSTable;  /**< Thread specific data for the thread */
   static TSTable gTSTable;  /**< Global thread specific data.  Used in all
                                  threads NOT created by vpr.  (ie. the
                                  primordial thread). */

protected:
   /**
    * After the object has been created, call this routine to complete
    * initialization.  It is done this way because I need to call this based
    * on stuff that happens in derived class's constructor.
    *
    * @pre The Thread Manager should be lock()'ed before calling this function
    *       so that the addThread() function can execute correctly
    * @post Thread is setup correctly to run.
    *       The thread has been registered with the system.
    *       Creates the thread's ID (mThreadId).
    *
    * @param successfulCreation Did the thread get created correctly?
    */
   void registerThread(bool successfulCreation);

   void unregisterThread();

public:
   /**
    * Causes the calling thread wait for the termination of this thread.
    *
    * @post The caller blocks until this thread finishes its execution
    *       (i.e., calls the exit() method).  This routine may return
    *       immediately if this thread has already exited.
    *
    * @param status Current state of the terminating thread when that thread
    *               calls the exit routine (optional).
    *
    * @return 0 is returned if this thread is "joined" successfully.
    * @return -1 is returned on an error condition.
    */
   virtual int join(void** status = 0)
   {
      boost::ignore_unused_variable_warning(status);
      return -1;
   }

   /**
    * Resumes the execution of this thread (if it was previously suspended
    * using suspend()).
    *
    * @return 0 is returned if this thread resumes execuation successfully.
    * @return -1 is returned otherwise.
    */
   virtual int resume()
   {
      return -1;
   }

   /**
    * Suspends the execution of this thread.
    *
    * @return 0 is returned if this thread is suspended successfully.
    * @return -1 is returned otherwise.
    */
   virtual int suspend()
   {
      return -1;
   }

   /**
    * Gets this thread's current priority.
    *
    * @post The priority of this thread is returned in the integer pointer
    *       variable.
    *
    * @param prio Pointer to an int variable that will have the thread's
    *             priority stored in it.
    *
    * @return 0 is returned if the priority was retrieved successfully.
    * @return -1 is returned if the priority could not be read.
    */
   virtual int getPrio(VPRThreadPriority* prio)
   {
      boost::ignore_unused_variable_warning(prio);
      return -1;
   }

   /**
    * Sets this thread's priority.
    *
    * @param prio The new priority for this thread.
    *
    * @return 0 is returned if this thread's priority was set successfully.
    * @return -1 is returned otherwise.
    */
   virtual int setPrio(VPRThreadPriority prio)
   {
      boost::ignore_unused_variable_warning(prio);
      return -1;
   }

   /**
    * Gets the "thread ID" of this vpr::BaseThread object.  This is a unique
    * integer value assigned when the thread was created.
    *
    * @return -1 is returned if this is a bad thread.  This usually means that
    *         there was a reation error.
    * @return Otherwise, the value returned is this thread's ID.
    */
   vpr::Int32 getTID()
   {
      return mThreadId;
   }

   /**
    * Is this a valid thread?
    *
    * @return true is returned if this object represents a thread that has
    *         been spawned correctly.
    *
    * @note A true value may be returned before the spawned thread begins
    *       executing.  This is due to the way that the operating system
    *       schedules a newly created thread.  However, true is returned if
    *       and only if the thread has been spawned successfully.
    */
   bool valid()
   {
      return (mThreadId != -1);
   }

   /**
    * Sends the specified signal to this thread (not necessarily \c SIGKILL).
    *
    * @post This thread receives the specified signal.
    *
    * @param signum The signal to send to the specified thread.
    *
    * @return 0 is returned if the thread was sent the given signal.
    * @return -1 is returned if an error occurred.
    */
   virtual int kill(int signum)
   {
      boost::ignore_unused_variable_warning(signum);
      return -1;
   }

   /**
    * Kills (cancels) this thread.
    *
    * @post This thread is cancelled. Immediate cancellation is not guaranteed.
    */
   virtual void kill()
   {;}

   /**
    * Ouputs the state of the object.
    */
   virtual std::ostream& outStream(std::ostream& out);

protected:
   /**
    * Initializes the state of the object.
    */
   void createThreadId()
   {
      mThreadId = getNextThreadId();
   }

private:
   /// Don't allow copy
   BaseThread(BaseThread&)
   {;}

protected:
   vpr::Int32  mThreadId;    //! The local id for the thread, -1 ==> invalid thread

   // --- STATICS ---- //

private:
   // XXX: What happens when it rolls over after we have been running for a
   // LONG time.
   vpr::Int32 getNextThreadId()
   {
      return mNextThreadId++;
   }

   static vpr::Int32 mNextThreadId;    // Initialized to 0
};

/// Ouput operator.
VPR_API(std::ostream&) operator<<(std::ostream& out, Thread* threadPtr);

// This is the actual function that is called.
// It must be extern "C".
#if defined(VPR_USE_PTHREADS)
extern "C" void* vprThreadFunctorFunction(void* args);
#else
extern "C" void vprThreadFunctorFunction(void* args);
#endif

} // End of vpr namespace


#endif
