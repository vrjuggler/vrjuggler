//:

/*
 * ---------------------------------------------------------------------------
 * vjThreadFunctor.C
 * $Revision$
 * $Date$
 * ---------------------------------------------------------------------------
 */

#include <config.h>
#include <Threads/vjThreadFunctor.h>


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
            vjBaseThreadFunctor& func = *(vjBaseThreadFunctor*)args;
            func();
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
