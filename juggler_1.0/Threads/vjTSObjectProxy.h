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


#ifndef _VJ_TS_OBJECT_PROXY_H_
#define _VJ_TS_OBJECT_PROXY_H_
//#pragma once

#include <Threads/vjTSObject.h>
#include <Threads/vjThreadManager.h>

//-----------------------------------------------------------------
//: This is a smart pointer to a thread specific object.
//
//  This allows users to have an object that has a seperate copy
// for each thread.
//
//! NOTE: The object used for type T must have a default constructor
//+       This class creates each instance of the real objects
//+       using this default constructor.
//-----------------------------------------------------------------
//! PUBLIC_API:

template <class T>
class vjTSObjectProxy
{
public:
   //-----------------------------------------------------------------
   //: Constructor for proxy.
   //-----------------------------------------------------------------
   vjTSObjectProxy() : mObjectKey(-1)
   {
      // Get a TS key for the object(s) that this will proxy
      mObjectKey = vjThreadManager::instance()->generateNewTSKey();
   }

   //-----------------------------------------------------------------
   //: Destructor.
   //-----------------------------------------------------------------
   ~vjTSObjectProxy()
   {;}

   T* operator->()
   { return getSpecific(); }

   T& operator*()
   { return *getSpecific();}

private:
   //-----------------------------------------------------------------
   //: Get the correct version for current thread
   // - Find the correct table
   // - Make sure that object exists locally
   // - Get the obj pointer
   // - Attempts a dynamic cast
   //-----------------------------------------------------------------
   T* getSpecific()
   {
      vjTSTable* table = vjThread::self()->getTSTable();

      // ---- DOES OBJECT EXIST --- //
      // If not, Create the object and add it to the table
      if(!table->containsKey(mObjectKey))
      {
         vjTSBaseObject* new_object = new vjTSObject<T>;
         table->setObject(new_object,mObjectKey);
      }

      // --- GET THE TS OBJECT --- //
      vjTSBaseObject* object = table->getObject(mObjectKey);                  // get the specific object
      vjTSObject<T>* real_object = dynamic_cast< vjTSObject<T>* >(object);    // try dynamic casting it

      vjASSERT(real_object != NULL);      // If fails, it means that "real" object was different type than the proxy
      if(real_object == NULL)
         return NULL;
      else
         return real_object->getObject();                                   // return the ptr;
   }

   // Don't allow copy construction
   vjTSObjectProxy(vjTSObjectProxy& proxy)
   {;}

private:
   long  mObjectKey;    // The key to find the object
};

#endif
