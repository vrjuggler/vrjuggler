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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _VPR_TS_OBJECT_PROXY_H_
#define _VPR_TS_OBJECT_PROXY_H_
//#pragma once

#include <vpr/vprConfig.h>
#include <stdlib.h>
#include <typeinfo>
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/TSObject.h>
#include <vpr/Thread/ThreadManager.h>
#include <vpr/Util/Assert.h>


namespace vpr
{

/** \class TSObjectProxyBase TSObjectProxy.h vpr/Thread/TSObjectProxy.h
 *
 * Base class for all thread-specific object proxies.  This handles the
 * thread-specific key allocation.  It allows for friendship (if needed).
 */
class VPR_CLASS_API TSObjectProxyBase
{
public:
   TSObjectProxyBase()
   {;}

protected:
   /**
    * Generates a unique key for thread-specific data.
    * This value will be used locally by each thread in the system.
    */
   static long generateNewTSKey();
};

/** @example "Example of using vpr::TSObjectProxy"
 *
 * \code
 * vpr::TSObjectProxy<obj_type> var;
 * (*var).method();
 * \endcode
 */

/** \class TSObjectProxy TSObjectProxy.h vpr/Thread/TSObjectProxy.h
 *
 * This is a smart pointer to a thread-specific object.
 *
 * This allows users to have an object that has a seperate copy for each
 * thread.
 *
 * @note The object used for type \p T must have a default constructor.
 *       This class creates each instance of the real objects using the
 *       default constructor.
 *
 * Uses TSObject<T> internally to keep some type information.
 */
template<class T>
class TSObjectProxy : public TSObjectProxyBase
{
public:
   /** Constructor for proxy. */
   TSObjectProxy() : mObjectKey(-1)
   {
      // Get a TS key for the object(s) that this will proxy
      mObjectKey = TSObjectProxyBase::generateNewTSKey();
   }

   /** Destructor.  This does nothing. */
   ~TSObjectProxy()
   {;}

   T* operator->()
   {
      return getSpecific();
   }

   T& operator*()
   {
      return *getSpecific();
   }

   /**
    * Returns the thread specific object pointer for the given thread.
    *
    * @note This should only be used by expert users.  It can cause MAJOR
    *       synchronization issues and even data corruption.
    */
   T* getObjPtrForThread(vpr::Thread* thread)
   {
      return getSpecific(thread);
   }

private:
   /**
    * Gets the correct version for current thread.
    *
    * - Find the correct table<br>
    * - Make sure that object exists locally<br>
    * - Get the obj pointer<br>
    * - Attempts a dynamic cast<br>
    *
    * @param reqThread Request for this specific thread.
    */
   T* getSpecific(vpr::Thread* reqThread = NULL)
   {
      TSTable* table(NULL);

      // --- GET TS TABLE --- //
      // - If have self, get mine.  Otherwise use global one
      vpr::Thread* thread_self(reqThread);
      if(NULL == thread_self)       // If didn't request specific thread, then get for current thread
      {
         thread_self = Thread::self();
      }

      if(NULL != thread_self)
      {
         table = thread_self->getTSTable();
      }
      else
      {
         table = Thread::getGlobalTSTable();
      }

      // ---- DOES OBJECT EXIST --- //
      // If not, Create the object and add it to the table
      if(!table->containsKey(mObjectKey))
      {
         table->setObject(NULL, mObjectKey);   // Extend table and set to NULL
      }

      // --- GET THE TS OBJECT --- //

      TSBaseObject* object = table->getObject(mObjectKey); // get the specific object

      // Check if we have not allocated it yet, if not, then allocate
      if(NULL == object)
      {
         TSBaseObject* new_object = new TSObject<T>;      // Allocate new object
         vprASSERT((new_object != NULL) && "Failed to allocate TSObject<T>");
         table->setObject(new_object, mObjectKey);        // Set the value
         object = new_object;                             // Reference the new one
      }

      // We should not have NULL objects
      vprASSERT((object != NULL) && "Bad object ptr.  It is NULL.");

      // --- Dynamic cast to "real" type wrapper
      TSObject<T>* real_object = dynamic_cast< TSObject<T>* >(object);

      if(real_object == NULL)    // Failed cast
      {
         std::cout << "Failed dynamic cast\n";
         std::cout << "Have pointer of type: " << typeid(*object).name()
                   << std::endl;
         std::cout << "Want type: " << typeid(T).name() << std::endl;
      }

      // If fails, it means that "real" object was different type than the
      // proxy.
      // XXX: Throw an exception here instead of asserting.  With optimized
      // code, a segmentation fault occurs for less-than-obvious reasons due
      // to the smart pointer indirection.  -PH 7/13/2004
      vprASSERT((real_object != NULL) && "Dynamic_cast of TS object failed");

      // Return the pointer.
      return real_object->getObject();
   }

   // Don't allow copy construction.
   TSObjectProxy(TSObjectProxy& proxy)
   {;}

private:
   long  mObjectKey;    /**< The key to find the object */
};

} // End of vpr namespace


#endif
