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
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_TS_TABLE_H_
#define _VJ_TS_TABLE_H_
//#pragma once

#include <vjConfig.h>
#include <Threads/vjTSObject.h>
#include <Sync/vjMutex.h>

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
   //: Return a new table of "fresh" objects.
   //! NOTE: This is NOT a copy. All objects are created by their
   //+       default constructors
   //-----------------------------------------------------------------
   vjTSTable* createNew();

   //: Delete the table
   // Delete all objects in the table
   ~vjTSTable();

   //: Get the object with the spcified key
   vjTSBaseObject* getObject(int objectKey);

public:
   //-----------------------------------------------------------------
   //: Set an object entry in the table.
   //-----------------------------------------------------------------
   void setObject(vjTSBaseObject* object, long key);

   //-----------------------------------------------------------------
   //: Release the object given by key.
   //
   //! POST: Obj(key) is deleted, and the ptr is set to NULL.
   //-----------------------------------------------------------------
   void releaseObject(long key);

public:
//   friend vjThreadManager;

private:
   vjMutex                      mListGuard;  //: Guard access to the list
   std::vector<vjTSBaseObject*> mTSObjects; //: Map object key to TS Object ptr
};

#endif
