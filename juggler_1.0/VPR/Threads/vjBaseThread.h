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

#ifndef _VJ_BASE_THREAD_H_
#define _VJ_BASE_THREAD_H_

#include <vjConfig.h>
#include <map>
#include <VPR/Threads/vjThreadFunctor.h>
#include <VPR/Threads/vjTSTable.h>            // Needed to cache a copy here

// --------------------------------------------------------------------------
// NOTES:
//    - This is used as the base class for all thread classes.
// -------------------------------------------------------------------------- 
class VJ_CLASS_API vjBaseThread
{
public:
   enum VJThreadPriority {
       VJ_PRIORITY_LOW,      //: The lowest possible priority
       VJ_PRIORITY_NORMAL,   //: The most common priority
       VJ_PRIORITY_HIGH,     //: Slightly higher priority
       VJ_PRIORITY_URGENT    //: The highest priority
   };

   enum VJThreadScope {
       VJ_LOCAL_THREAD,
       VJ_GLOBAL_THREAD
   };

   enum VJThreadState {
       VJ_JOINABLE_THREAD,
       VJ_UNJOINABLE_THREAD
   };

   vjBaseThread()
      :  mThreadId(0)
   {;}

   virtual ~vjBaseThread()
   {
      ;
   }

public:     // Thread specific data caching
   //: Get the Thread specific data table
   // NOTE: Users should NOT access the table directly
   // instead, use vjTSObjectProxies
   vjTSTable* getTSTable()
   { return &mTSTable; }

private:
   vjTSTable  mTSTable;      // Thread specific data for the thread

protected:
   //: After the object has been created, call this routine to complete
   //+ initialization.
   // Done this way, because I need to call this based on stuff that happens
   // in derived class's constructor.
   //! POST: Thread is setup correctly to run.
   //+       The thread has been registered with the system.
   //+       Creates the thread's id (mThreadId)
   //! ARGS: successfulCreation - Did the thread get created correctly
   void registerThread(bool succesfulCreation);

public:
   // -----------------------------------------------------------------------
   //: Create a new thread that will execute functorPtr.
   //
   //! PRE: None.
   //! POST: A thread (with any specified attributes) is created that begins
   //+       executing func().
   //
   //! ARGS: functorPtr - Function to be executed by the thread.
   //! ARGS: priority - Priority of created thread (optional).
   //! ARGS: stack_size - Size for thread's stack (optional).
   //
   //! RETURNS:  non-zero - Successful thread creation
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   virtual int spawn (vjBaseThreadFunctor* functorPtr,
                      VJThreadPriority priority = VJ_PRIORITY_NORMAL,
                      VJThreadScope scope = VJ_LOCAL_THREAD,
                      VJThreadState state = VJ_JOINABLE_THREAD,
                      size_t stack_size = 0)
   {
      std::cerr << "Base spawn.  Should NOT be called." << std::endl;
      return -1;
   }

   // -----------------------------------------------------------------------
   //: Make the calling thread wait for the termination of this thread.
   //
   //! PRE: None.
   //! POST: The caller blocks until this thread finishes its execution
   //+       (i.e., calls the exit() method).  This routine may return
   //+       immediately if this thread has already exited.
   //
   //! ARGS: status - Status value of the terminating thread when that
   //+                thread calls the exit routine (optional).
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   virtual int join (void** status = 0)
   {return -1;}

   // -----------------------------------------------------------------------
   //: Wait for one or more threads to exit.
   //
   //! PRE: None.
   //! POST: The caller blocks until the specified threads finish their
   //+       execution (i.e., call the exit() method).  This routine may
   //+       return immediately if the specified threads have already
   //+       exited.
   //
   //! ARGS: thread_array - Array of one or more thread IDs to be joined.
   //! ARGS: n - The number of threads to join.
   //! ARGS: ret_val - Storage for return value of an exiting thread
   //+                 (optional).
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   int join (vjBaseThread* thread_array[], int n, void** ret_val = 0)
   { return -1;}

   // -----------------------------------------------------------------------
   //: Resume the execution of a thread that was previously suspended using
   //+ suspend().
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   virtual int resume()
   {return -1;}

   // -----------------------------------------------------------------------
   //: Suspend the execution of this thread.
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   virtual int suspend()
   {return -1;}

   // -----------------------------------------------------------------------
   //: Get this thread's priority.
   //
   //! PRE: None.
   //! POST: The priority of this thread is returned in the integer pointer
   //+       variable.
   //
   //! ARGS: prio - Pointer to an int variable that will have the thread's
   //+              priority stored in it.
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   virtual int getPrio(VJThreadPriority* prio)
   { return -1;}

   // -----------------------------------------------------------------------
   //: Set this thread's priority.
   //
   //! ARGS: prio - The new priority for this thread.
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   virtual int setPrio(VJThreadPriority prio)
   { return -1;}

   // -----------------------------------------------------------------------
   //: Get the "thread ID" of this vjBaseThread object.  This is a unique
   //+ integer value assigned when the thread was created.
   //
   //! RETURNS: -1 - Bad thread. Creation error.
   //! RETURNS: other - id of thread in Juggler
   // -----------------------------------------------------------------------
   int32_t getTID()
   { return mThreadId; }

   // -----------------------------------------------------------------------
   //: Is this a valid thread?
   //
   //! RETURNS: true - object represents a thread that has been created
   //+                 correctly.
   // -----------------------------------------------------------------------
   bool valid()
   {
      return (mThreadId != -1);
   }

   // -----------------------------------------------------------------------
   //: Send the specified signal to this thread (not necessarily SIGKILL).
   //
   //! POST: This thread receives the specified signal.
   //
   //! ARGS: signum - The signal to send to the specified thread.
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   virtual int kill (int signum)
   { return -1;}

   // -----------------------------------------------------------------------
   //: Kill (cancel) this thread.
   //
   //! PRE: None.
   //! POST: This thread is cancelled. Immediate
   //+       cancellation is not guaranteed.
   // -----------------------------------------------------------------------
   virtual void kill()
   {;}

   // -----------------------------------------------------------------------
   //: Ouput the state of the object.
   // -----------------------------------------------------------------------
   virtual std::ostream& outStream(std::ostream& out)
   {
      out.setf(std::ios::right);
      out << std::setw(3) << mThreadId;
      out.unsetf(std::ios::right);
      return out;
   }

protected:
   // -----------------------------------------------------------------------
   //: Initialize the state of the object.
   // -----------------------------------------------------------------------
   void create_thread_id()
   {
      mThreadId = getNextThreadId();
   }

private:
   // Don't allow copy
   vjBaseThread(vjBaseThread& other)
   {;}

private:
   int32_t  mThreadId;    // The local id for the thread, -1 ==> invalid thread

   // --- STATICS ---- //

private:
   int32_t getNextThreadId();

   static int32_t mNextThreadId;    // Initialized to 0
};

// Ouput operator
VJ_API(std::ostream&) operator<<(std::ostream& out, vjBaseThread* threadPtr);


//: Helper class fot vjThread that maintains a list of threads and ides
//
// Basically maps from system specific index ==> vjThread*
//
// Used internally because we can have many types of indexes for the thread
// list depending upon the type of threads being used.
template <class IdxType>
class VJ_CLASS_API vjThreadTable
{
public:
   // -----------------------------------------------------------------------
   //: Add a thread to the list.
   // -----------------------------------------------------------------------
   void addThread(vjBaseThread* threadPtr, IdxType index)
   {
      mThreadMap[index] = threadPtr;
   }

   // -----------------------------------------------------------------------
   //: Get a thread from the list.
   // -----------------------------------------------------------------------
   vjBaseThread* getThread(IdxType index)
   {
      //std::hash_map<IdxType, vjBaseThread*>::iterator i;
      typename std::map<IdxType, vjBaseThread*>::iterator i;
      i = mThreadMap.find(index);
      if(i == mThreadMap.end())
         return NULL;
      else
         return (*i).second;
   }

   // -----------------------------------------------------------------------
   //: Remove a thread from the list.
   // -----------------------------------------------------------------------
   void removeThread(IdxType index)
   {
      mThreadMap.erase(index);
   }

private:
   std::map<IdxType, vjBaseThread*> mThreadMap;
   //std::hash_map<IdxType, vjBaseThread*> mThreadMap;
};

#endif
