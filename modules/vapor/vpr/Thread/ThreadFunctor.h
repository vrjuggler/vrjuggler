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

#ifndef _VPR_THREAD_FUNCTOR_H_
#define _VPR_THREAD_FUNCTOR_H_

#include <vpr/vprConfig.h>
#include <stdlib.h>


namespace vpr {

/**
 * Converts a function into a functor that can be passed to a
 * extern C type function to be called by a thread creation
 * routine.
 *
 * @author Allen Bierbaum
 */
class VPR_CLASS_API BaseThreadFunctor
{
public:
    /**
     * Overloaded operator() used for invoking the function encapsulated by
     * this object.  This version takes no argument and instead passes the
     * argument given when this object was constructed.
     */
    virtual void operator()() = 0;    // Pure virtual

    /**
     * Overloaded operator() used for invoking the function encapsulated by
     * this object.  This version takes an argument that is passed on to the
     * function.
     *
     * @param arg The argument to be passed to the encapsulated function.
     */
    virtual void operator()(void* arg) = 0;

    /**
     * Sets the argument passed to the encapsulated function.  This will be
     * used when the function is invoked through the overloaded operator().
     */
    virtual void setArg(void*) = 0;
};

/**
 * Member functor class.  This class allows non-static class member functions
 * to be used as functors.
 *
 * @author Allen Bierbaum
 */
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


/**
 * Nonmember functor class.  Converts a non-member function or a static class
 * member function into a functor.
 *
 * @author Allen Bierbaum
 */
class VPR_CLASS_API ThreadNonMemberFunctor : public BaseThreadFunctor
{
public:
    typedef void(* NonMemFunPtr)(void*);

    /**
     * Constructor.
     *
     * @param f A pointer to a function that is not a class member.
     * @param a A pointer to an argument to be passed to the function.  This
     *          argument is optional and defaults to NULL.
     */
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
    NonMemFunPtr func;  /**< Thread startup function (C++ linkage). */
    void* argument;     /**< Argument to thread startup function. */
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
#endif	/* VPR_USE_IRIX_SPROC */

}; // End of vpr namespace

#endif	/* _VPR_THREAD_FUNCTOR_H_ */
