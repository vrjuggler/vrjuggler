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

/*
 * ----------------------------------------------------------------------------
 * NOTES:
 *    - This file (CondVarNSPR.h) must be included by vpr/Sync/Cond.h, not the
 *      other way around.
 * ----------------------------------------------------------------------------
 */

#ifndef _VPR_COND_VAR_NSPR_H_
#define _VPR_COND_VAR_NSPR_H_

#include <vpr/vprConfig.h>

#include <prcvar.h>

#include <vpr/md/NSPR/Sync/MutexNSPR.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>
#include <vpr/md/NSPR/NSPRHelpers.h>


namespace vpr
{

/**
 * Condition variable wrapper for NSPR condition variables.
 */
class VPR_CLASS_API CondVarNSPR
{
public:
   /**
    * Constructor.
    *
    * @pre None.
    * @post The condition variable is intialized, and the mutex variable
    *       associated with it is defined.  These two steps must be done
    *       before any other member functions can use them.
    *
    * @param mutex Pointer to a vpr::MutexNSPR variable that is used in
    *              association with the condition variable in this class
    *              (optional).
    */
   CondVarNSPR (MutexNSPR* mutex = NULL)
   {
      // If the caller did not specify a mutex variable to use with
      // the condition variable, use mDefaultMutex.
      if ( mutex == NULL )
      {
         mCondMutex = &mDefaultMutex;
      }
      else
      {
         mCondMutex = mutex;
      }

      // Initialize the condition variable.
      mCondVar = PR_NewCondVar(mCondMutex->mMutex);

      vprASSERT(mCondVar != NULL);
   }

   /**
    * Destructor.
    *
    * @pre The condition variable is no longer in use.
    * @post The condition variable is destroyed.
    */
   ~CondVarNSPR (void)
   {
      PR_DestroyCondVar(mCondVar);
   }

   /**
    * Blocks on a condition.  The lock asociated with this condition variable
    * must be held prior to invoking this method.  When invoked, the lock on
    * the variable is released, and the calling thread is blocked until another
    * thread informs it that the condition has changed or until the timeout
    * expires.
    *
    * @pre The mutex variable associated with the condition variable must
    *      be locked.
    * @post The condition variable is locked.  If it was previously
    *       locked, the caller blocks until signaled.
    *
    * @return vpr::ReturnStatus::Succeed is returned when the calling thread is
    *         signaled.  vpr::ReturnStatus::Fail is returned if something
    *         went wrong in blocking on the condition.
    */
   vpr::ReturnStatus wait (vpr::Interval timeToWait = vpr::Interval::NoTimeout)
   {
      vpr::ReturnStatus status;

      // ASSERT:  We have been locked.
      if ( PR_WaitCondVar(mCondVar, NSPR_getInterval(timeToWait)) != PR_SUCCESS )
      {
         status.setCode(vpr::ReturnStatus::Fail);
      }

      // XXX: Use error status to print a message before the assertion.
      vprASSERT(status.success());

      return status;
   }

   /**
    * Signals another thread waiting on this condition variable.
    *
    * @pre The condition variable must be locked.
    * @post The condition variable is unlocked, and a signal is sent to a
    *       thread waiting on it.
    *
    * @return vpr::ReturnStatus::Succeed is returned when the signal is sent
    *         successfully.  vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus signal (void)
   {
      vpr::ReturnStatus status;

      // ASSERT:  We have been locked
      if ( PR_NotifyCondVar(mCondVar) != PR_SUCCESS )
      {
         status.setCode(vpr::ReturnStatus::Fail);
      }

      vprASSERT(status.success());

      return status;
   }

   /**
    * Signals all threads waiting on the condition variable.
    *
    * @pre The mutex variable associated with the condition variable
    *      should be locked.
    * @post The condition variable is unlocked, and all waiting threads
    *       are signaled of this event.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the broadcast message
    *         is sent successfully.  vpr::ReturnStatus::Fail is returned
    *         otherwise.
    */
   vpr::ReturnStatus broadcast (void)
   {
      vpr::ReturnStatus status;

      // ASSERT:  We have been locked
      if ( PR_NotifyAllCondVar(mCondVar) != PR_SUCCESS )
      {
         status.setCode(vpr::ReturnStatus::Fail);
      }

      vprASSERT(status.success());

      return status;
   }

   /**
    * Acquires a lock on the mutex variable associated with the condition
    * variable.
    *
    * @pre None.
    * @post A lock is acquired on the mutex variable associated with the
    *      condition variable.  If a lock is acquired, the caller controls
    *      the mutex variable.  If it was previously locked, the caller
    *      blocks until it is unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock on this
    *         condition variable is acquired successfully.
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus acquire (void)
   {
      return mCondMutex->acquire();
   }

   /**
    * Tries to acquire a lock on the mutex variable associated with the
    * condition variable.
    *
    * @pre None.
    * @post If the mutex variable is not already locked, the caller
    *       obtains a lock on it.  If it is already locked, the routine
    *       returns immediately to the caller.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock on this
    *         condition variable is acquired successfully.
    *         vpr::ReturnStatus::Fail is returned if the lock is already held
    *         by another thread.
    */
   vpr::ReturnStatus tryAcquire (void)
   {
      return mCondMutex->tryAcquire();
   }

   /**
    * Releases the lock on the mutex variable associated with the condition
    * variable.
    *
    * @pre None.
    * @post The lock held by the caller on the mutex variable is released.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock on this
    *         condition variable is released successfully.
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus release (void)
   {
      return mCondMutex->release();
   }

   /**
    * Changes the condition variable mutex to be the specifiec mutex
    * variable.
    *
    * @pre The specified mutex variable must be initialized.
    * @post The condition variable associated with the mutex variable is
    *       reset to the specified variable.
    *
    * @param mutex Pointer to a vpr::MutexNSPR variable that is used in
    *              association with the condition variable in this class.
    *
    * @note NEVER call except to initialize explicitly.
    */
   void setMutex (MutexNSPR* mutex)
   {
      // NOT exactly correct, but just make sure not to leave it locked
      mutex->release();
      mCondMutex = mutex;
   }

   /**
    * Prints out information about the condition variable to stderr.
    *
    * @pre None.
    * @post All important data and debugging information related to the
    *       condition variable and its mutex are dumped to stderr.
    */
   void dump (void) const
   {
      std::cerr << "------------- vpr::CondVarNSPR::Dump ---------\n"
                << "Not Implemented yet.\n";
   }

private:
   PRCondVar*      mCondVar;   /**< Condition variable */
   MutexNSPR*      mCondMutex; /**< Mutex for the condition variable */

   MutexNSPR       mDefaultMutex;  /**< A default mutex variable */

   // = Prevent assignment and initialization.
   void operator= (const CondVarNSPR&)
   {
      /* Do nothing.*/ ;
   }

   CondVarNSPR (const CondVarNSPR &c)
   {
      /* Do nothing.*/ ;
   }
};

} // End of vpr namespace


#endif  /* _VPR_COND_VAR_NSPR_H_ */
