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

#include <vpr/vprConfig.h>

#include <vpr/Thread/Thread.h>
#include <vpr/md/NSPR/NSPRHelpers.h>
#include <vpr/md/NSPR/Thread/ThreadNSPR.h>


namespace vpr {

ThreadTable<PRThread*> ThreadNSPR::mThreadTable;
PRUint32 ThreadNSPR::mTicksPerSec = PR_TicksPerSecond();

// ---------------------------------------------------------------------------
//: Spawning constructor
//
// This will actually start a new thread that will execute the specified
// function.
// ---------------------------------------------------------------------------
ThreadNSPR::ThreadNSPR (thread_func_t func, void* arg,
                        VPRThreadPriority priority, VPRThreadScope scope,
                        VPRThreadState state, PRUint32 stack_size)
{
    ThreadManager* vpr_tm_inst;

    vpr_tm_inst = ThreadManager::instance();

    vpr_tm_inst->lock();
    {
        int ret_val;
        ThreadNonMemberFunctor* NonMemFunctor;

        NonMemFunctor = new ThreadNonMemberFunctor(func, arg);

        ret_val = spawn(NonMemFunctor, priority, scope, state, stack_size);
        checkRegister(ret_val);
    }
    vpr_tm_inst->unlock();
}

// ---------------------------------------------------------------------------
// Spawning constructor with arguments (functor version).
//
// This will start a new thread that will execute the specified function.
// ---------------------------------------------------------------------------
ThreadNSPR::ThreadNSPR (BaseThreadFunctor* functorPtr,
                        VPRThreadPriority priority, VPRThreadScope scope,
                        VPRThreadState state, PRUint32 stack_size)
{
    ThreadManager* vpr_tm_inst;

    vpr_tm_inst = ThreadManager::instance();

    vpr_tm_inst->lock();
    {
        int ret_val;

        ret_val = spawn(functorPtr, priority, scope, state, stack_size);
        checkRegister(ret_val);
    }
    vpr_tm_inst->unlock();
}

// ---------------------------------------------------------------------------
// Destructor.
//
// PRE: None.
// POST: This thread is removed from the thread table and from the local
//       thread hash.
// ---------------------------------------------------------------------------
ThreadNSPR::~ThreadNSPR () {
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
ThreadNSPR::spawn (BaseThreadFunctor* functor_ptr,
                   VPRThreadPriority priority, VPRThreadScope scope,
                   VPRThreadState state, size_t stack_size)
{
    PRThreadPriority nspr_prio;
    PRThreadScope nspr_scope;
    PRThreadState nspr_state;
    int retval;

    nspr_prio  = vprThreadPriorityToNSPR(priority);
    nspr_scope = vprThreadScopeToNSPR(scope);
    nspr_state = vprThreadStateToNSPR(state);

    retval = 0;

    // Finally create the thread.
    mThread = PR_CreateThread(PR_USER_THREAD, vprThreadFunctorFunction,
                              (void*) functor_ptr, nspr_prio, nspr_scope,
                              nspr_state, (PRUint32) stack_size);

    // Inform the caller if the thread was not created successfully.
    if ( mThread == NULL ) {
        NSPR_PrintError("vpr::ThreadNSPR::spawn() - Cannot create thread");
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
ThreadNSPR::setPrio (VPRThreadPriority prio) {
    int retval;

    retval = 0;

    if ( prio > 3 ) {
        retval = -1;
    } else {
        PR_SetThreadPriority(mThread, vprThreadPriorityToNSPR(prio));
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
ThreadNSPR::checkRegister (const int status) {
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
ThreadNSPR::vprThreadPriorityToNSPR (const VPRThreadPriority priority) {
    PRThreadPriority nspr_prio;

    switch (priority) {
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

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
PRThreadScope
ThreadNSPR::vprThreadScopeToNSPR (const VPRThreadScope scope) {
    PRThreadScope nspr_scope;

    switch (scope) {
      case VPR_LOCAL_THREAD:
        nspr_scope = PR_LOCAL_THREAD;
        break;
      case VPR_GLOBAL_THREAD:
        nspr_scope = PR_GLOBAL_THREAD;
        break;
    };

    return nspr_scope;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
PRThreadState
ThreadNSPR::vprThreadStateToNSPR (const VPRThreadState state) {
    PRThreadState nspr_state;

    switch (state) {
      case VPR_JOINABLE_THREAD:
        nspr_state = PR_JOINABLE_THREAD;
        break;
      case VPR_UNJOINABLE_THREAD:
        nspr_state = PR_UNJOINABLE_THREAD;
        break;
    };

    return nspr_state;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
BaseThread::VPRThreadPriority
ThreadNSPR::nsprThreadPriorityToVPR (const PRThreadPriority priority) {
    VPRThreadPriority vpr_prio;

    switch (priority) {
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

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
BaseThread::VPRThreadScope
ThreadNSPR::nsprThreadScopeToVPR (const PRThreadScope scope) {
    VPRThreadScope vpr_scope;

    switch (scope) {
      case PR_LOCAL_THREAD:
        vpr_scope = VPR_LOCAL_THREAD;
        break;
      case PR_GLOBAL_THREAD:
        vpr_scope = VPR_GLOBAL_THREAD;
        break;
    };

    return vpr_scope;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
BaseThread::VPRThreadState
ThreadNSPR::nsprThreadStateToVPR (const PRThreadState state) {
    VPRThreadState vpr_state;

    switch (state) {
      case PR_JOINABLE_THREAD:
        vpr_state = VPR_JOINABLE_THREAD;
        break;
      case PR_UNJOINABLE_THREAD:
        vpr_state = VPR_UNJOINABLE_THREAD;
        break;
    };

    return vpr_state;
}

}; // End of vpr namespace
