/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
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
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


//:

#include <vjConfig.h>
#include <Threads/vjThreadFunctor.h>
#include <Threads/vjThreadManager.h>


//--------------------------------------------
// This is the actual function that is called.
//--------------------------------------------
#if defined(VJ_IRIX_SPROC)	// ---- Using IRIX Sproc ------ //
    void
    ThreadFunctorFunction (void* args) {
        vjBaseThreadFunctor& func = *(vjBaseThreadFunctor*)args;
        func();	// Call the functor's operator ()
    }
#elif defined(VJ_OS_Win32)	// ----- Using Win32 ---- //
    unsigned int __stdcall
    ThreadFunctorFunction (void* args) {
         vjBaseThreadFunctor& func = *(vjBaseThreadFunctor*)args;
         func();
         return 0;
    }
#elif defined(VJ_USE_PTHREADS)
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
#endif	/* VJ_IRIX_SPROC */
