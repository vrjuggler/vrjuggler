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

#ifndef _VPR_TS_OBJECT_PROXY_H_
#define _VPR_TS_OBJECT_PROXY_H_
//#pragma once

#include <vpr/vprConfig.h>
#include <typeinfo>
#include <vpr/Thread/TSObject.h>
#include <vpr/Thread/ThreadManager.h>
#include <vpr/Util/Assert.h>


namespace vpr {

/**
 * This is a smart pointer to a thread specific object.
 *
 * This allows users to have an object that has a seperate copy
 * for each thread.
 *
 * NOTE: The object used for type T must have a default constructor
 *       This class creates each instance of the real objects
 *       using this default constructor.
 */
template <class T>
class TSObjectProxy
{
public:
   /// Constructor for proxy.
   TSObjectProxy() : mObjectKey(-1)
   {
      // Get a TS key for the object(s) that this will proxy
      mObjectKey = ThreadManager::instance()->generateNewTSKey();
   }

   /// Destructor.  This does nothing.
   ~TSObjectProxy()
   {;}

   T* operator->()
   { return getSpecific(); }

   T& operator*()
   { return *getSpecific();}

private:
   /** Get the correct version for current thread.
    * - Find the correct table
    * - Make sure that object exists locally
    * - Get the obj pointer
    * - Attempts a dynamic cast
    */
   T* getSpecific()
   {
      TSTable* table = NULL;

      // Get the correct TS data table
      // - If have self, get mine.  Otherwise use global one
      vpr::BaseThread* thread_self = NULL;
      thread_self = Thread::self();
      if(NULL != thread_self)
      {  table = Thread::self()->getTSTable(); }
      else
      {  table = Thread::getGlobalTSTable(); }

      // ---- DOES OBJECT EXIST --- //
      // If not, Create the object and add it to the table
      if(!table->containsKey(mObjectKey))
      {
         table->setObject(NULL, mObjectKey);       // Extend table and set to NULL
      }

      // --- GET THE TS OBJECT --- //
      TSBaseObject* object = table->getObject(mObjectKey);                  // get the specific object

      // Check if we have not allocated it yet, if not, then allocate
      if(object == NULL)
      {
         TSBaseObject* new_object = new TSObject<T>;                            // Allocate new object
         vprASSERT((new_object != NULL) && "Failed to allocate TSObject<T>");      // NULL is bad
         table->setObject(new_object, mObjectKey);                                  // Set the value
         object = new_object;                                                       // Reference the new one
      }

      TSObject<T>* real_object = dynamic_cast< TSObject<T>* >(object);    // try dynamic casting it

      vprASSERT((object != NULL) && "TS Object is NULL!!!");    // We should not have NULL objects

#ifdef VPR_DEBUG
      if(real_object == NULL)    // Failed cast
      {
         std::cout << "Failed dynamic cast\n";
         std::cout << "Have pointer of type: " << typeid(*object).name() << std::endl;
         std::cout << "Want type: " << typeid(T).name() << std::endl;
      }
#endif

      vprASSERT((real_object != NULL) && "Dynamic_cast of TS object failed");  // If fails, it means that "real" object was different type than the proxy
      /*
      if(real_object == NULL)    // Should NEVER return NULL.  If we did, then we can't dereference it.
         return NULL;
      else
      */
         return real_object->getObject();                                   // return the ptr;
   }

   // Don't allow copy construction
   TSObjectProxy(TSObjectProxy& proxy)
   {;}

private:
   long  mObjectKey;    //! The key to find the object
};

}; // End of vpr namespace


#endif
