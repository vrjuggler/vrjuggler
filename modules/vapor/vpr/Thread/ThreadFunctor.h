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

#ifndef _VPR_THREAD_FUNCTOR_H_
#define _VPR_THREAD_FUNCTOR_H_

#include <vpr/vprConfig.h>
#include <stdlib.h>


namespace vpr {

//---------------------------------------------------------------
//: Converts a function into a functor that can be passed to a
//  extern C type function to be called by a thread creation
//  routine.
//!PUBLIC_API:
//---------------------------------------------------------------
class VPR_CLASS_API BaseThreadFunctor
{
public:
    virtual void operator()() = 0;    // Pure virtual
    virtual void operator()(void*) = 0;
    virtual void setArg(void*) = 0;
};

//---------------------------------------------------------------
//: Member functor class.
//!PUBLIC_API:
//---------------------------------------------------------------
template<class T>
class VPR_CLASS_API ThreadMemberFunctor : public BaseThreadFunctor
{
public:
    typedef void (T::* FunPtr)(void*);

    ThreadMemberFunctor(T* theObject, FunPtr func, void* arg = NULL)
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


//---------------------------------------------------------------
//: Nonmember functor class.  Converts a NonMember function into
//+ a functor.
//
//!PUBLIC_API:
//---------------------------------------------------------------
class VPR_CLASS_API ThreadNonMemberFunctor : public BaseThreadFunctor
{
public:
    typedef void(* NonMemFunPtr)(void*);

    //---------------------------------------------------------------
    //: Constructor.
    //---------------------------------------------------------------
    ThreadNonMemberFunctor (NonMemFunPtr f, void* a = NULL)
       : func(f), argument(a)
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

//---------------------------------------------
// This is the actual function that is called.
// It must be extern "C"
//---------------------------------------------
#if defined(VPR_USE_IRIX_SPROC)	/* ---- SGI IPC Barrier ------ */
    extern "C" void vprThreadFunctorFunction(void* args);
#elif defined(VPR_USE_PTHREADS)
#   ifdef _PTHREADS_DRAFT_4
        extern "C" void
#   else
        extern "C" void*
#   endif
        vprThreadFunctorFunction(void* args);
#else
    extern "C" void vprThreadFunctorFunction(void* args);
#endif	/* VPR_IRIX_SPROC */

}; // End of vpr namespace

#endif	/* _VPR_THREAD_FUNCTOR_H_ */
