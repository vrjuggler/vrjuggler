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

#ifndef _VPR_BASE_THREAD_H_
#define _VPR_BASE_THREAD_H_

#include <vpr/vprConfig.h>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/signal.hpp>

#include <vpr/vprTypes.h>
#include <vpr/Thread/TSTable.h>            /* Needed to cache a copy here */


namespace vpr
{

/**
 * Typedef to help with cross-platform abilities.  This type is the basic
 * function pointer type for all functions initially passed as code to
 * execute within a separate thread of control.
 */
typedef boost::function<void()> thread_func_t;

/** \class BaseThread BaseThread.h vpr/Thread/BaseThread.h
 *
 * Base class for all thread implementations.
 *
 * Provides functionality that is common to all threading implementations.
 *
 * @note This class is \em not designed to be used as a polymorphic base class
 *       to hold references to threads. The lack of virtual methods should
 *       make this fact self-evident. Just use the real thread type.
 */
class VPR_CLASS_API BaseThread : private boost::noncopyable
{
public:
   /** @name Thread State Callback Handling */
   //@{
   /**
    * The type for the thread start and exit signals.
    *
    * @since 1.1.44
    */
   typedef boost::signal<void (vpr::Thread*)> state_signal_t;

   /**
    * Connects the given slot to the signal emitted whenever a new thread
    * begins its execution. The slot is invoked immediately before the thread
    * functor is invoked.
    *
    * @post \p slot is connected to \c sStartSignal.
    *
    * @param slot The slot object to be connected to the thread start signal.
    *
    * @return The connection object that holds the association between the
    *         start signal and the given slot.
    *
    * @since 1.1.44
    */
   static boost::signals::connection
      addThreadStartCallback(state_signal_t::slot_function_type slot);

   /**
    * Connects the given slot to the signal emitted whenever a running thread
    * completes its execution. The slot is invoked immediately after the
    * thread functor returns.
    *
    * @post \p slot is connected to \c sExitSignal.
    *
    * @param slot The slot object to be connected to the thread exit signal.
    *
    * @return The connection object that holds the association between the
    *         exit signal and the given slot.
    *
    * @since 1.1.44
    */
   static boost::signals::connection
      addThreadExitCallback(state_signal_t::slot_function_type slot);
   //@}

protected:
   /**
    * Emits the thread start signal.
    *
    * @since 1.1.44
    */
   static void emitThreadStart(vpr::Thread* thread)
   {
      sStartSignal(thread);
   }

   /**
    * Emits the thread exit signal.
    *
    * @since 1.1.44
    */
   static void emitThreadExit(vpr::Thread* thread)
   {
      sExitSignal(thread);
   }

private:
   static state_signal_t sStartSignal;
   static state_signal_t sExitSignal;

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

protected:
   BaseThread();

   ~BaseThread();

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

   const TSTable* getTSTable() const
   {
      return &mTSTable;
   }

   /**
    * Get the Thread the global thread specific data table.
    * This table is shared by all threads that were not created by VPR.
    */
   static TSTable* getGlobalTSTable()
   {
      return &sTSTable;
   }

   /**
    * Gets the "thread ID" of this vpr::BaseThread object.  This is a unique
    * integer value assigned when the thread was created.
    *
    * @return -1 is returned if this is a bad thread.  This usually means that
    *         there was a creation error.
    * @return Otherwise, the value returned is this thread's ID.
    */
   vpr::Int32 getTID() const
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
   bool valid() const
   {
      return mThreadId != -1;
   }

private:
   TSTable        mTSTable;  /**< Thread specific data for the thread */
   static TSTable sTSTable;  /**< Global thread specific data.  Used in all
                                  threads NOT created by vpr.  (ie. the
                                  primordial thread). */

protected:
   /**
    * Ouputs the state of the object.
    */
   std::ostream& outStream(std::ostream& out);

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
   void registerThread(const bool successfulCreation);

   void unregisterThread();

   /**
    * Initializes the state of the object.
    */
   void createThreadId()
   {
      mThreadId = getNextThreadId();
   }

protected:
   /**
    * The local ID for the thread. A value of -1 implies that this is an
    * invalid thread.
    */
   vpr::Int32 mThreadId;

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
#elif defined(VPR_USE_WINTHREADS)
extern "C" unsigned __stdcall vprThreadFunctorFunction(void* args);
#else
extern "C" void vprThreadFunctorFunction(void* args);
#endif

} // End of vpr namespace


#endif
