#ifndef _VJ_THREAD_FUNCTOR_H_
#define _VJ_THREAD_FUNCTOR_H_

#include <vjConfig.h>
#include <stdlib.h>

class  vjBaseThreadFunctor
//---------------------------------------------------------------
// PURPOSE:
//     Converts a function into a functor that can be passed
//	to a extern C type function to be called by a thread creation
//	routine.
//!PUBLIC_API:
//---------------------------------------------------------------
{
public:
    virtual void operator()() = 0;    // Pure virtual
    virtual void operator()(void*) = 0;
    virtual void setArg(void*) = 0;
};

//: Member functor class.
//!PUBLIC_API:
template<class T>
class vjThreadMemberFunctor : public  vjBaseThreadFunctor
{
public:
    typedef void (T::* FunPtr)(void*);

    vjThreadMemberFunctor(T* theObject, FunPtr func, void* arg = NULL)
    {
    	object = theObject;
    	function = func;
    	argument = arg;
    }

    void
    operator() (void* arg) {
        (object->*function)(arg);
    }

    void
    operator() () {
        (object->*function)(argument);
    }

    void
    setArg (void* arg) {
        argument = arg;
    }

private:
    T*	    object;
    FunPtr  function;
    void*   argument;
};


//: Nonmember functor class.
//!PUBLIC_API:
class vjThreadNonMemberFunctor : public  vjBaseThreadFunctor
//---------------------------------------------------------------
// PURPOSE:
//     Converts a NonMember function into a functor
//---------------------------------------------------------------
{
public:
    typedef void(* NonMemFunPtr)(void*);

    // Constructor
    vjThreadNonMemberFunctor (NonMemFunPtr f, void* a = NULL) : func(f), argument(a)
    {;}

    virtual void operator() (void* arg) {
        (*func)(arg);
    }

    virtual void operator() () {
        (*func)(argument);
    }

    void setArg (void* arg) {
        argument = arg;
    }

    // private:
    // = Arguments to thread startup.
    NonMemFunPtr func;	// Thread startup function (C++ linkage).
    void* argument;		// Argument to thread startup function.
};

//--------------------------------------------
// This is the actual function that is called.
// It must be extern "C"
//--------------------------------------------
#ifdef VJ_SGI_IPC   // ---- SGI IPC Barrier ------ //
    extern "C" void ThreadFunctorFunction(void* args);
#else
#ifdef WIN32
    unsigned int __stdcall ThreadFunctorFunction(void* args);
#else
#ifdef VJ_USE_PTHREADS
#   ifdef _PTHREADS_DRAFT_4
        extern "C" void
#   else
        extern "C" void*
#   endif
        ThreadFunctorFunction (void* args);
#else
    extern "C" void ThreadFunctorFunction(void* args);
#endif	/* VJ_USE_PTHREADS */
#endif	/* WIN32 */
#endif	/* VJ_SGI_IPC */

#endif	/* _THREAD_FUNCTOR_H_ */
