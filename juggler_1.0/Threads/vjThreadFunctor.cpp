//:

/*
 * ---------------------------------------------------------------------------
 * ThreadFunctor.C
 * $Revision$
 * $Date$
 * ---------------------------------------------------------------------------
 */

#include <config.h>
#include <Threads/vjThreadFunctor.h>


//--------------------------------------------
// This is the actual function that is called.
// It must be extern "C"
//--------------------------------------------
#ifdef VJ_SGI_IPC   // ---- SGI IPC Barrier ------ //
    extern "C" void 
    ThreadFunctorFunction (void* args) {
         vjBaseThreadFunctor& func = *( vjBaseThreadFunctor*)args;
        func();	// Call the functor's operator ()
    }
#else
#ifdef WIN32
    unsigned int __stdcall 
    ThreadFunctorFunction (void* args) {
          vjBaseThreadFunctor& func = *( vjBaseThreadFunctor*)args;
         func();
         return 0;
    }
#else
#ifdef VJ_USE_PTHREADS
        extern "C" void
#   ifdef _PTHREADS_DRAFT_4
        ThreadFunctorFunction (void* args) {
#   else
        *ThreadFunctorFunction (void* args) {
#   endif
             vjBaseThreadFunctor& func = *( vjBaseThreadFunctor*)args;
            func();
        }

#else
    extern "C" void 
    ThreadFunctorFunction (void* args) {
         vjBaseThreadFunctor& func = *( vjBaseThreadFunctor*)args;
        func();
    }
#endif	/* CS_USE_PTHREADS */
#endif	/* WIN32 */
#endif	/* VJ_SGI_IPC */
