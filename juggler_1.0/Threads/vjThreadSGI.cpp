/*
 * ---------------------------------------------------------------------------
 * vjThreadSGI.C
 * $Revision$
 * $Date$
 * ---------------------------------------------------------------------------
 */

#include <iostream.h>
#include <sys/types.h>
#include <sys/prctl.h>

#include <Threads/vjThread.h>
#include <Threads/vjThreadSGI.h>


// ---------------------------------------------------------------------------
// Overload the << operator so that the PID and address of this vjThreadSGI
// structure can be output in a readable format.
// ---------------------------------------------------------------------------
ostream&
operator<< (ostream& outfile, vjThreadSGI& thread) {
    outfile << "[PID " << thread.getpid() << "]";

    return outfile;
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
vjThreadSGI::create_n (vjThreadSGI thread_ids[], int n, THREAD_FUNC func,
                     void* arg, long flags, u_int priority,
                     void* stack_addr[], size_t stack_size[])
{
    for ( int i = 0; i < n; i++ ) {
        vjThreadSGI* newThread = vjThread::spawn(func, arg, flags, priority,
                                               (stack_addr == 0) ? 0 : stack_addr[i],
                                               (stack_size == 0) ? 0 : stack_size[i]);

        thread_ids[i] = *newThread;
    }

    return i;
} 
