/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


//:

#include <vjConfig.h>
#include <Threads/vjThreadFunctor.h>
#include <Threads/vjThreadManager.h>


//--------------------------------------------
// This is the actual function that is called.
//--------------------------------------------
#ifdef VJ_SGI_IPC   // ---- SGI IPC Barrier ------ //
    void
    ThreadFunctorFunction (void* args) {
        vjBaseThreadFunctor& func = *(vjBaseThreadFunctor*)args;
        func();	// Call the functor's operator ()
    }
#else
#ifdef WIN32
    unsigned int __stdcall
    ThreadFunctorFunction (void* args) {
         vjBaseThreadFunctor& func = *(vjBaseThreadFunctor*)args;
         func();
         return 0;
    }
#else
#ifdef VJ_USE_PTHREADS
#   ifdef _PTHREADS_DRAFT_4
        void
#   else
        void*
#   endif
        ThreadFunctorFunction (void* args) {
            vjThreadManager* vj_tm_inst;
            vjBaseThreadFunctor& func = *((vjBaseThreadFunctor*) args);

            // Wait until this thread has been registered with the thread
            // manager before continuing execution.
            vj_tm_inst = vjThreadManager::instance();
            vj_tm_inst->lock();
            vj_tm_inst->unlock();

            func();

#   ifdef _PTHREADS_DRAFT_10
            return (void*) NULL;
#   endif
        }

#else
    void
    ThreadFunctorFunction (void* args) {
        vjBaseThreadFunctor& func = *(vjBaseThreadFunctor*)args;
        func();
    }
#endif	/* VJ_USE_PTHREADS */
#endif	/* WIN32 */
#endif	/* VJ_SGI_IPC */
