/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

#include <vjConfig.h>

#include <VPR/Threads/vjThread.h>
#include <VPR/md/NSPR/vjThreadNSPR.h>


vjThreadTable<PRThread*> vjThreadNSPR::mThreadTable;
PRUint32 vjThreadNSPR::mTicksPerSec = PR_TicksPerSecond();

// ---------------------------------------------------------------------------
//: Spawning constructor
//
// This will actually start a new thread that will execute the specified
// function.
// ---------------------------------------------------------------------------
vjThreadNSPR::vjThreadNSPR (vj_thread_func_t func, void* arg,
                            VJThreadPriority priority, VJThreadScope scope,
                            VJThreadState state, PRUint32 stack_size)
{
    vjThreadManager* vj_tm_inst;

    vj_tm_inst = vjThreadManager::instance();

    vj_tm_inst->lock();
    {
        int ret_val;
        vjThreadNonMemberFunctor* NonMemFunctor;

        NonMemFunctor = new vjThreadNonMemberFunctor(func, arg);

        ret_val = spawn(NonMemFunctor, priority, scope, state, stack_size);
        checkRegister(ret_val);
    }
    vj_tm_inst->unlock();
}

// ---------------------------------------------------------------------------
// Spawning constructor with arguments (functor version).
//
// This will start a new thread that will execute the specified function.
// ---------------------------------------------------------------------------
vjThreadNSPR::vjThreadNSPR (vjBaseThreadFunctor* functorPtr,
                            VJThreadPriority priority, VJThreadScope scope,
                            VJThreadState state, PRUint32 stack_size)
{
    vjThreadManager* vj_tm_inst;

    vj_tm_inst = vjThreadManager::instance();

    vj_tm_inst->lock();
    {
        int ret_val;

        ret_val = spawn(functorPtr, priority, scope, state, stack_size);
        checkRegister(ret_val);
    }
    vj_tm_inst->unlock();
}

// ---------------------------------------------------------------------------
// Destructor.
//
// PRE: None.
// POST: This thread is removed from the thread table and from the local
//       thread hash.
// ---------------------------------------------------------------------------
vjThreadNSPR::~vjThreadNSPR (void) {
    mThreadTable.removeThread(gettid());
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
vjThreadNSPR::spawn (vjBaseThreadFunctor* functor_ptr,
                     VJThreadPriority priority, VJThreadScope scope,
                     VJThreadState state, size_t stack_size)
{
    PRThreadPriority nspr_prio;
    PRThreadScope nspr_scope;
    PRThreadState nspr_state;
    int retval;

    nspr_prio  = vjThreadPriorityToNSPR(priority);
    nspr_scope = vjThreadScopeToNSPR(scope);
    nspr_state = vjThreadStateToNSPR(state);

    retval = 0;

    // Finally create the thread.
    mThread = PR_CreateThread(PR_USER_THREAD, ThreadFunctorFunction,
                              (void*) functor_ptr, nspr_prio, nspr_scope,
                              nspr_state, (PRUint32) stack_size);

    // Inform the caller if the thread was not created successfully.
    if ( mThread == NULL ) {
        perror("vjThreadNSPR::spawn() - Cannot create thread");
        retval = -1;
    }

    return retval;
}

// ---------------------------------------------------------------------------
// Set this thread's priority.
//
// PRE: None.
// POST: This thread has its priority set to the specified value.
// ---------------------------------------------------------------------------
int
vjThreadNSPR::setPrio (VJThreadPriority prio) {
    int retval;

    retval = 0;

    if ( prio > 3 ) {
        retval = -1;
    } else {
        PR_SetThreadPriority(mThread, vjThreadPriorityToNSPR(prio));
    }

    return retval;
}

// ===========================================================================
// Private methods follow.
// ===========================================================================

// ---------------------------------------------------------------------------
// Check the status of the thread creation in order to determine if this
// thread should be registered in the thread table or not.
// ---------------------------------------------------------------------------
void
vjThreadNSPR::checkRegister (const int status) {
    if ( status == 0 ) {
        registerThread(true);
        mThreadTable.addThread(this, mThread);
    } else {
        registerThread(false);	// Failed to create
    }
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
PRThreadPriority
vjThreadNSPR::vjThreadPriorityToNSPR (const VJThreadPriority priority) {
    PRThreadPriority nspr_prio;

    switch (priority) {
      case VJ_PRIORITY_LOW:
        nspr_prio = PR_PRIORITY_LOW;
        break;
      case VJ_PRIORITY_NORMAL:
        nspr_prio = PR_PRIORITY_NORMAL;
        break;
      case VJ_PRIORITY_HIGH:
        nspr_prio = PR_PRIORITY_HIGH;
        break;
      case VJ_PRIORITY_URGENT:
        nspr_prio = PR_PRIORITY_URGENT;
        break;
    };

    return nspr_prio;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
PRThreadScope
vjThreadNSPR::vjThreadScopeToNSPR (const VJThreadScope scope) {
    PRThreadScope nspr_scope;

    switch (scope) {
      case VJ_LOCAL_THREAD:
        nspr_scope = PR_LOCAL_THREAD;
        break;
      case VJ_GLOBAL_THREAD:
        nspr_scope = PR_GLOBAL_THREAD;
        break;
    };

    return nspr_scope;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
PRThreadState
vjThreadNSPR::vjThreadStateToNSPR (const VJThreadState state) {
    PRThreadState nspr_state;

    switch (state) {
      case VJ_JOINABLE_THREAD:
        nspr_state = PR_JOINABLE_THREAD;
        break;
      case VJ_UNJOINABLE_THREAD:
        nspr_state = PR_UNJOINABLE_THREAD;
        break;
    };

    return nspr_state;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
vjBaseThread::VJThreadPriority
vjThreadNSPR::nsprThreadPriorityToVJ (const PRThreadPriority priority) {
    VJThreadPriority vj_prio;

    switch (priority) {
      case PR_PRIORITY_LOW:
        vj_prio = VJ_PRIORITY_LOW;
        break;
      case PR_PRIORITY_NORMAL:
        vj_prio = VJ_PRIORITY_NORMAL;
        break;
      case PR_PRIORITY_HIGH:
        vj_prio = VJ_PRIORITY_HIGH;
        break;
      case PR_PRIORITY_URGENT:
        vj_prio = VJ_PRIORITY_URGENT;
        break;
    };

    return vj_prio;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
vjBaseThread::VJThreadScope
vjThreadNSPR::nsprThreadScopeToVJ (const PRThreadScope scope) {
    VJThreadScope vj_scope;

    switch (scope) {
      case PR_LOCAL_THREAD:
        vj_scope = VJ_LOCAL_THREAD;
        break;
      case PR_GLOBAL_THREAD:
        vj_scope = VJ_GLOBAL_THREAD;
        break;
    };

    return vj_scope;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
vjBaseThread::VJThreadState
vjThreadNSPR::nsprThreadStateToVJ (const PRThreadState state) {
    VJThreadState vj_state;

    switch (state) {
      case PR_JOINABLE_THREAD:
        vj_state = VJ_JOINABLE_THREAD;
        break;
      case PR_UNJOINABLE_THREAD:
        vj_state = VJ_UNJOINABLE_THREAD;
        break;
    };

    return vj_state;
}
