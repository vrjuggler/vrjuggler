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

#ifndef _VPR_BASE_THREAD_H_
#define _VPR_BASE_THREAD_H_

#include <vpr/vprConfig.h>

#include <iomanip>
#include <map>
#include <boost/concept_check.hpp>
#include <vpr/vprTypes.h>
#include <vpr/Thread/TSTable.h>            /* Needed to cache a copy here */


namespace vpr
{

/**
 * Typedef to help with cross-platform abilities.  This type is the basic
 * function pointer type for all functions initially passed as code to
 * execute within a separate thread of control.
 */
typedef void (*thread_func_t)(void *);


class BaseThreadFunctor;

/**
 * This is used as the base class for all thread classes.
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
   virtual void setFunctor(BaseThreadFunctor* functorPtr) = 0;

   /** Starts the thread's execution. */
   virtual vpr::ReturnStatus start() = 0;

public:     // Thread specific data caching
   /**
    * Gets the Thread specific data table.
    * NOTE: Users should NOT access the table directly
    * instead, use vpr::TSObjectProxies.
    */
   TSTable* getTSTable()
   {
      return &mTSTable;
   }

   /**
    * Get the Thread the global thread specific data table.
    * This table is shared by all threads that were not created by vpr.
    */
   static TSTable* getGlobalTSTable()
   {
      return &gTSTable;
   }

private:
   TSTable        mTSTable;  /**< Thread specific data for the thread */
   static TSTable gTSTable;  /**< Global thread specific data.  Used in all
                                  threads NOT created by vpr.  (ie. the
                                  primordial thread) */

protected:
   /**
    * After the object has been created, call this routine to complete
    * initialization.
    * Done this way, because I need to call this based on stuff that happens
    * in derived class's constructor.
    *
    * @pre The thread manager should be lock()'ed before calling this function
    *       so that the addThread function can execute correctly
    * @post Thread is setup correctly to run.
    *       The thread has been registered with the system.
    *       Creates the thread's id (mThreadId)
    *
    * @param successfulCreation Did the thread get created correctly
    */
   void registerThread(bool succesfulCreation);

   /**
    * Creates a new thread that will execute functorPtr.
    *
    * @post A thread (with any specified attributes) is created that begins
    *       executing func().
    *
    * @param functorPtr Function to be executed by the thread.
    * @param priority   Priority of created thread (optional).
    * @param scope      The scope of the new thread.  This argument is
    *                   optional, and it defaults to
    *                   vpr::Thread::VPR_GLOBAL_THREAD.
    * @param state      The state of the new thread (joinable or unjoinable).
    *                   This argument is optional, and it defaults to
    *                   vpr::Thread::VPR_JOINABLE_THREAD.
    * @param stack_size The stack size for the new thread.  This argument is
    *                   optional, and it defaults to 0 (use the default stack
    *                   size offered by the OS).
    *
    * @return A non-zero value is returned to indicate that the thread was
    *         created successfully.  -1 is returned otherwise.
    */
/*
   virtual vpr::ReturnStatus spawn(BaseThreadFunctor* functorPtr,
                                   VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
                                   VPRThreadScope scope = VPR_GLOBAL_THREAD,
                                   VPRThreadState state = VPR_JOINABLE_THREAD,
                                   size_t stack_size = 0)
   {
      std::cerr << "Base spawn.  Should NOT be called." << std::endl;
      return vpr::ReturnStatus::Fail;
   }
*/

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
    * @return 0 is returned if this thread is "joined" successfully.<br>
    *         -1 is returned on an error condition.
    */
   virtual int join (void** status = 0)
   {
      return -1;
   }

   /**
    * Resumes the execution of this thread (if it was previously suspended
    * using suspend()).
    *
    * @return 0 is returned if this thread resumes execuation successfully.<br>
    *         -1 is returned otherwise.
    */
   virtual int resume()
   {
      return -1;
   }

   /**
    * Suspends the execution of this thread.
    *
    * @return 0 is returned if this thread is suspended successfully.<br>
    *         -1 is returned otherwise.
    */
   virtual int suspend()
   {
      return -1;
   }

   /**
    * Gets this thread's current priority.
    *
    * @pre None.
    * @post The priority of this thread is returned in the integer pointer
    *       variable.
    *
    * @param prio Pointer to an int variable that will have the thread's
    *             priority stored in it.
    *
    * @return 0 is returned if the priority was retrieved successfully.<br>
    *         -1 is returned if the priority could not be read.
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
    * @return 0 is returned if this thread's priority was set successfully.<br>
    *         -1 is returned otherwise.
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
    *         there was a reation error.<br>
    *         Otherwise, the value returned is this thread's ID.
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
    * Sends the specified signal to this thread (not necessarily SIGKILL).
    *
    * @post This thread receives the specified signal.
    *
    * @param signum The signal to send to the specified thread.
    *
    * @return 0 is returned if the thread was sent the given signal.<br>
    *         -1 is returned if an error occurred.
    */
   virtual int kill(int signum)
   {
      boost::ignore_unused_variable_warning(signum);
      return -1;
   }

   /**
    * Kill (cancel) this thread.
    *
    * @pre None.
    * @post This thread is cancelled. Immediate cancellation is not guaranteed.
    */
   virtual void kill()
   {;}

   /**
    * Ouputs the state of the object.
    */
   virtual std::ostream& outStream(std::ostream& out)
   {
      out.setf(std::ios::right);
      out << std::setw(3) << mThreadId;
      out.unsetf(std::ios::right);
      return out;
   }

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
   BaseThread(BaseThread& other)
   {;}

private:
   vpr::Int32  mThreadId;    //! The local id for the thread, -1 ==> invalid thread

   // --- STATICS ---- //

private:
   // XXX: What happens when it rolls over after we have been running for a LONG time.
   vpr::Int32 getNextThreadId()
   {
      return mNextThreadId++;
   }

   static vpr::Int32 mNextThreadId;    // Initialized to 0
};


/**
 * Helper class for vpr::Thread that maintains a list of threads and IDs.
 * It maps from system specific index ==> vpr::BaseThread*
 *
 * Used internally because we can have many types of indexes for the thread
 * list depending upon the type of threads being used.
 */
template <class IdxType>
class VPR_CLASS_API ThreadTable
{
public:
   /**
    * Adds a thread to the list.
    *
    * @post The given thread object is registred using the given index.
    *
    * @param threadPtr A pointer to the thread to register.
    * @param index     The index of the thread to be added.
    */
   void addThread(BaseThread* threadPtr, IdxType index)
   {
      mThreadMap[index] = threadPtr;
   }

   /**
    * Gets a thread from the list.
    *
    * @post The thread indexed by the given value is returned to the caller.
    *
    * @param index The index of the thread to be retrieved.
    *
    * @return A pointer to a vpr::BaseThread is returned if the index was
    *         valid.<br>
    *         NULL is returned if the indexed thread could not be found.
    */
   BaseThread* getThread(IdxType index)
   {
      //std::hash_map<IdxType, BaseThread*>::iterator i;
      typename std::map<IdxType, BaseThread*>::iterator i;
      i = mThreadMap.find(index);
      if (i == mThreadMap.end())
      {
         return NULL;
      }
      else
      {
         return (*i).second;
      }
   }

   /**
    * Removes a thread from the list.
    *
    * @pre The given value is the index of a registered thread.
    * @post The thread indexed by the given value is removed from the list.
    *
    * @param index The index of the thread to be removed.
    */
   void removeThread(IdxType index)
   {
      mThreadMap.erase(index);
   }

private:
   std::map<IdxType, BaseThread*> mThreadMap;
   //std::hash_map<IdxType, BaseThread*> mThreadMap;
};

/// Ouput operator.
VPR_API(std::ostream&) operator<<(std::ostream& out, BaseThread* threadPtr);

} // End of vpr namespace


#endif
