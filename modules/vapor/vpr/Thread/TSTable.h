/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _VPR_TS_TABLE_H_
#define _VPR_TS_TABLE_H_
//#pragma once

#include <vpr/vprConfig.h>
#include <vector>
#include <vpr/Thread/TSObject.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Assert.h>


namespace vpr
{

/** \class TSTable TSTable.h vpr/Thread/TSTable.h
 *
 * This class is the actual thread-specific table.
 *
 * This class maintains a table that has ptrs to all the thread-specific data
 * in the system for a specific thread.  Only the owning thread may actually
 * access the table.  Because of this, we do not have to lock the table at all
 * when adding and removing.
 *
 * Uses vpr::TSBaseObject pointers so that there is some type safety.  (This
 * is better than using void*.)
 */
class TSTable
{
public:
   TSTable()
   {;}

   /**
    * Deletes the table. It deletes all objects in the table.
    */
   ~TSTable()
   {
      // For all elements in the table
      for ( unsigned int i = 0; i < mTSObjects.size(); ++i )
      {
         if(mTSObjects[i] != NULL)        // If valid object
         {
            delete mTSObjects[i];         // Delete them
         }
      }
   }

public:
   /**
    * Returns \c true if the table contains the given key.  If \c false is
    * returned, then the user should call setObject(...,key) before attempting
    * to access the object.
    */
   bool containsKey(unsigned long key)
   {
      //vprASSERT((key >= 0) && "Called contains key with invalid key");
      return (unsigned int) key < mTSObjects.size();
   }

   /** Gets the object with the spcified key. */
   TSBaseObject* getObject(unsigned long objectKey)
   {
      vprASSERT(containsKey(objectKey));    // Did you check to make sure the table contained it
      return mTSObjects[objectKey];
   }

   /** Sets an object entry in the table. */
   inline void setObject(TSBaseObject* object, unsigned long key);

   /**
    * Releases the object given by \p key.
    *
    * @post Obj(key) is deleted, and the pointer is set to NULL.
    */
   inline void releaseObject(unsigned long key);

private:
   std::vector<TSBaseObject*> mTSObjects;    /**< Map object key to TS Object ptr */
};

// Sets an object entry in the table.
void TSTable::setObject(TSBaseObject* object, unsigned long key)
{
   //vprASSERT(key >= 0);
#ifdef _DEBUG
   std::vector<TSBaseObject*>::size_type size_before = mTSObjects.size();
#endif
   if(mTSObjects.size() <= key)
   {
      mTSObjects.resize((key+1), NULL);       // We will find these later and actually allocate them
   }
   mTSObjects[key] = object;

#ifdef _DEBUG
   vprASSERT( size_before <= mTSObjects.size());   // ASSERT: vector should never get smaller
#endif
}

// Releases the object given by key.
void TSTable::releaseObject(unsigned long key)
{
   vprASSERT(containsKey(key));
   if (mTSObjects[key] != NULL)
   {
      delete mTSObjects[key];
   }
   mTSObjects[key] = NULL;
}

} // End of vpr namespace


#endif
