/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_TS_TABLE_H_
#define _VJ_TS_TABLE_H_
#pragma once

#include <vjConfig.h>
#include <Threads/vjTSObject.h>
#include <Kernel/vjDebug.h>
#include <Sync/vjMutex.h>
#include <Sync/vjGuard.h>

//-----------------------------------------------------------------
//: This class is the actual TS Table.
//
// This class maintains a table that has ptrs to all the TS data
// in the system for a specific thread.
// As new thread specific data is added to the system, a copy is also
// added to this table. (This is done by the thread manager)
// The list is protected by a mutex to make it thread safe.  Only one
// thread and remove, add, or access the list at a time
// XXX: This should be changed to a read/write mutex
//-----------------------------------------------------------------
class vjTSTable
{
public:
   vjTSTable()
   {;}

   //-----------------------------------------------------------------
   //: Return a newly created table.
   //! NOTE: This is not a copy. All objects are created by their
   //+       default constructors
   //-----------------------------------------------------------------
   vjTSTable* createNew()
   {
   vjGuard<vjMutex>  guard(mListGuard);
      vjTSTable* new_table = new vjTSTable;

      // For all elements in the table
      for(int i=0;i<mTSObjects.size();i++)
         if(mTSObjects[i] != NULL)
            new_table->setObject(mTSObjects[i]->createNew(), i);

      return new_table;    // Return the newly created table
   }

   //: Delete the table
   // Delete all objects in the table
   ~vjTSTable()
   {
      // For all elements in the table
      for(int i=0;i<mTSObjects.size();i++)
         if(mTSObjects[i] != NULL)        // If valid object
            delete mTSObjects[i];         // Delete them
   }

   //: Get the object with the spcified key
   vjTSBaseObject* getObject(int objectKey)
   {
   vjGuard<vjMutex>  guard(mListGuard);
      vjASSERT((objectKey >= 0) && (objectKey < mTSObjects.size()));
      return mTSObjects[objectKey];
   }

public:
   //-----------------------------------------------------------------
   //: Set an object entry in the table.
   //-----------------------------------------------------------------
   void setObject(vjTSBaseObject* object, long key)
   {
   vjGuard<vjMutex>  guard(mListGuard);
      vjASSERT(key >= 0);
      while(mTSObjects.size() <= key)
         mTSObjects.push_back(NULL);
      mTSObjects[key] = object;
   }

   //-----------------------------------------------------------------
   //: Release the object given by key.
   //
   //! POST: Obj(key) is deleted, and the ptr is set to NULL.
   //-----------------------------------------------------------------
   void releaseObject(long key)
   {
   vjGuard<vjMutex>  guard(mListGuard);
      vjASSERT( (key>=0) && (key<mTSObjects.size()) );
      if (mTSObjects[key] != NULL)
         delete mTSObjects[key];
      mTSObjects[key] = NULL;
   }

public:
//   friend vjThreadManager;

private:
   vjMutex                      mListGuard;  //: Guard access to the list
   std::vector<vjTSBaseObject*> mTSObjects; //: Map object key to TS Object ptr
};

#endif
