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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

#ifndef _VPR_THREAD_POOL_H_
#define _VPR_THREAD_POOL_H_

#include <vpr/vprConfig.h>

#include <vpr/Thread/Thread.h>
#include <vpr/Sync/Semaphore.h>


namespace vpr {

/**
 * Helper class for vpr::ThreadPool
 *
 * @author Allen Bierbaum
 */
class OneThread
{
public:
    OneThread() : thread(NULL), next(NULL), functor(NULL), threadWait(0) {
        next = NULL;
    }

public:
    Thread* thread;               //! Handle of the thread process
    OneThread* next;              //! -> next vprOneThread ready to run
    BaseThreadFunctor* functor;   //! -> function thread is to call
    Semaphore threadWait;         //! thread waits for work here
};


std::ostream& operator<< (std::ostream&, vpr::OneThread&);


/**
 * A pool of threads to process user jobs
 *
 * Outline of thread pool <br>
 *   |-Need to initialize all thread given a parameter to the constructor of the # to use <br>
 *   |-Need a function to block until all work is done that has started <br>
 *   |-Just repetiviely add work to the process <br>
 *
 * @author Allen Bierbaum
 *
 * Date: 2-5-97
 */
class ThreadPool
{
public:
    /// Constructor.
    ThreadPool(int numToStartWith = 1);

    /**
     * Gives a function to the processes.  Start a function going
     * asynchronously.  Called by master process.
     */
    void startFunc (thread_func_t func, void* arg = NULL)
    {
        ThreadNonMemberFunctor* NonMemFunctor = new ThreadNonMemberFunctor(func, arg);
        this->startFunc(NonMemFunctor);
    }

    void startFunc(BaseThreadFunctor* theFunctor, void* argument)
    {
        theFunctor->setArg(argument);
        this->startFunc(theFunctor);
    }

    void startFunc(BaseThreadFunctor* theFunctor)
    {
        OneThread* theThread = getThread();

        theThread->functor = theFunctor;     /* set address of func to exec */
        theThread->threadWait.release();     /* wake up sleeping process */
        //delete theFunctor;
    }

    OneThread* addThread(void);

    /**
     * Body of a general-purpose child process. The argument, which must be
     * declared void* to match the func prototype, is the vpr::OneThread
     * structure that represents this process.   The contents of that
     * struct, in particular threadWait, MUST be initialized by the parent.
     */
    void threadLoop(void* theThreadAsVoid);

    /**
     * Waits for work to do.  Put a vpr::OneThread structure on the ready list
     * and sleep on it.  Called by a child process when its work is done.
     */
    void threadSleep(OneThread* theThread);

    /**
     * Gets a process to run.  Acquire a vpr::OneThread structure from the
     * ready list, waiting if necessary.  Called by the master process as
     * part of dispatching a thread.
     */
    OneThread* getThread(void);

    /// Waits until all threads are done doing their work.
    void
    barrier (void) {
        finishedLock.acquire();      // Get the lock that means threads done
        finishedLock.release();      // Reset it to done
    }

    void printList(void);

private:
    Semaphore readyThreads;    //! count represents threads ready to work
    Mutex listLock;            //! mutex control of threadList head
    Mutex workingCountLock;    //! mutex on thread count
    Mutex finishedLock;        //! Lock for wether thread are finished, lock -> doing work
    OneThread* listHead;       //! -> first ready vpr::OneThread
    volatile int workingCount; //! Number of threads currently doing work
};

}; // End of vpr namespace


#endif
