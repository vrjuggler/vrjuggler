/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <Threads/vjThread.h>
#include <Threads/vjThreadWin32.h>


int vjThreadWin32::thread_count = 1;

// ---------------------------------------------------------------------------
// Overload the << operator so that the PID and address of this vjThreadWin32
// structure can be output in a readable format.
// ---------------------------------------------------------------------------
ostream&
operator<< (ostream& outfile, vjThreadWin32& thread) {
    outfile << "[PID " << thread.getpid() << ":" << thread.gettid() << "]";

    return outfile;
}

// ---------------------------------------------------------------------------
// Create a new thread that will execute functorPtr.
//
// PRE: None.
// POST: A thread (with any specified attributes) is created that begins
//       executing func().  Depending on the scheduler, it may being
//       execution immediately, or it may block for a short time before
//       beginning execution.
// ---------------------------------------------------------------------------
int
vjThreadWin32::create ( vjBaseThreadFunctor* functorPtr, long flags, u_int priority,
                     void* stack_addr, size_t stack_size)
{
    unsigned tid;
    int result;
    
//    if (stack_size != 0)
//    {
//	size_t size = stack_size;
//	// -- Check size -- //
//    }
//
//    if (stack != NULL)
//      {
//	// Check for validity and set and stuff
//      }

    if (0 == (*threadID = (HANDLE) _beginthreadex(NULL,0, &ThreadFunctorFunction, functorPtr, 0, &tid)))
    {
        return 0;
    }

    return 0;
}

// ---------------------------------------------------------------------------
// Create n new threads that execute func() with the argument arg if given.
//
// PRE: The thread_ids[] array should not have anything stored in it.  Its
//      contents are destroyed in the routine.
// POST: The thread ID's created in this routine are stored and returned in
//       the array thread_ids[] so that the caller may access them.
// ---------------------------------------------------------------------------
int
vjThreadWin32::create_n (vjThreadWin32 thread_ids[], int n, THREAD_FUNC func,
                       void* arg, long flags, u_int priority,
                       void* stack_addr[], size_t stack_size[])
{
    for ( int i = 0; i < n; i++ ) {
        vjThreadWin32* newThread = vjThread::spawn(func, arg, flags, priority,
                                                 (stack_addr == 0) ? 0 : stack_addr[i],
                                                 (stack_size == 0) ? 0 : stack_size[i]);

        thread_ids[i] = *newThread;
    }

    return i;
} 
