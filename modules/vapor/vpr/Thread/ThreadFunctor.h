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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
#include <boost/concept_check.hpp>

#include <vpr/Util/Assert.h>


namespace vpr
{

/** \class BaseThreadFunctor ThreadFunctor.h vpr/Thread/ThreadFunctor.h
 *
 * Converts a function into a functor that can be passed to a extern C type
 * function to be called by a thread creation routine.
 */
class BaseThreadFunctor
{
public:

   virtual ~BaseThreadFunctor()
   {
      ;
   }

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

   /**
    * Function to see if we have a valid functor.
    */
   virtual bool isValid() = 0;
};

/** \class ThreadMemberFunctor ThreadFunctor.h vpr/Thread/ThreadFunctor.h
 *
 * Member functor class.  This class allows non-static class member functions
 * to be used as functors.
 */
template<class OBJ_TYPE>
class ThreadMemberFunctor : public BaseThreadFunctor
{
public:
   typedef void (OBJ_TYPE::* FunPtr)(void*);

   ThreadMemberFunctor(OBJ_TYPE* theObject, FunPtr func, void* arg = NULL)
   {
      mObject = theObject;
      mFunction = func;
      mArgument = arg;
   }

   virtual ~ThreadMemberFunctor()
   {
      mObject = (OBJ_TYPE*)0xDEADBEEF;
      //mFunction = 0xDEADBEEF;
      mArgument = (void*)0xDEADBEEF;
   }

   void operator() (void* arg)
   {
      (mObject->*mFunction)(arg);
   }

   void operator() ()
   {
      (mObject->*mFunction)(mArgument);
   }

   void setArg(void* arg)
   {
      mArgument = arg;
   }

   bool isValid()
   {
      if ( (NULL == mObject) || ((OBJ_TYPE*)0xDEADBEEF == mObject) )
      {
         vprASSERT( NULL != mObject);
         vprASSERT( (OBJ_TYPE*)0xDEADBEEF != mObject);
         return false;
      }
      /*
      else if (NULL == mFunction)
      {
         return false;
      }
      */
      else
      {
         return true;
      }
   }

private:
   OBJ_TYPE*  mObject;
   FunPtr     mFunction;
   void*      mArgument;
};

/** \class ThreadRunFunctor ThreadFunctor.h vpr/Thread/ThreadFunctor.h
 *
 * A variation on vpr::ThreadMemberFunctor that requires a class that
 * implements the "runnable" concept.  That is, the class must have a run()
 * method with the following signature: void run().  No argument will be
 * passed to the runnable object when its run() method is invoked.  Instead,
 * it is up to the class author to ensure that the object has all information
 * it needs to do its job before the thread is spawned.
 */
template<typename OBJ_TYPE>
class ThreadRunFunctor : public BaseThreadFunctor
{
public:
   typedef void (OBJ_TYPE::* FunPtr)();

   ThreadRunFunctor(OBJ_TYPE* theObject)
      : mObject(theObject)
      , mFunction(NULL)
   {
      mFunction = &OBJ_TYPE::run;
   }

   virtual ~ThreadRunFunctor()
   {
      mObject   = (OBJ_TYPE*) 0xDEADBEEF;
//      mFunction = (FunPtr) 0xDEADBEEF;
   }

   void operator() (void* arg)
   {
      boost::ignore_unused_variable_warning(arg);
      (mObject->*mFunction)();
   }

   void operator() ()
   {
      (mObject->*mFunction)();
   }

   virtual void setArg(void* arg)
   {
      boost::ignore_unused_variable_warning(arg);
   }

   virtual bool isValid()
   {
      if ( (NULL == mObject) || ((OBJ_TYPE*) 0xDEADBEEF == mObject) )
      {
         vprASSERT(NULL != mObject);
         vprASSERT((OBJ_TYPE*)0xDEADBEEF != mObject);
         return false;
      }
      else if ( (NULL == mFunction) /* || ((FunPtr) 0xDEADBEEF == mFunction) */ )
      {
         return false;
      }
      else
      {
         return true;
      }
   }

private:
   OBJ_TYPE*  mObject;
   FunPtr     mFunction;
};


/** \class ThreadNonMemberFunctor ThreadFunctor.h vpr/Thread/ThreadFunctor.h
 *
 * Nonmember functor class.  Converts a non-member function or a static class
 * member function into a functor.
 */
class ThreadNonMemberFunctor : public BaseThreadFunctor
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
      : mFunc(f)
      , mArgument(a)
   {
      ;
   }

   virtual ~ThreadNonMemberFunctor()
   {
      mFunc = (NonMemFunPtr)0xDEADBEEF;
      mArgument = (void*)0xDEADBEEF;
   }

   virtual void operator() (void* arg)
   {
      (*mFunc)(arg);
   }

   virtual void operator() ()
   {
      (*mFunc)(mArgument);
   }

   void setArg(void* arg)
   {
      mArgument = arg;
   }

   bool isValid()
   {
      vprASSERT(mFunc != NULL);
      vprASSERT( mArgument != (void*)0xDEADBEEF);
      return( (mFunc != NULL) && (mArgument != (void*)0xDEADBEEF) );
   }

   // private:
   // = Arguments to thread startup.
   NonMemFunPtr mFunc;  /**< Thread startup function (C++ linkage). */
   void* mArgument;     /**< Argument to thread startup function. */
};

// This is the actual function that is called.
// It must be extern "C".
#if defined(VPR_USE_IRIX_SPROC) /* ---- SGI IPC Barrier ------ */
extern "C" void vprThreadFunctorFunction(void* args);
#elif defined(VPR_USE_PTHREADS)
extern "C" void* vprThreadFunctorFunction(void* args);
#else
extern "C" void vprThreadFunctorFunction(void* args);
#endif  /* VPR_USE_IRIX_SPROC */

} // End of vpr namespace


#endif  /* _VPR_THREAD_FUNCTOR_H_ */
