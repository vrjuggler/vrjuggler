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
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */

#include <vjConfig.h>
#include <Threads/vjTSTable.h>
#include <Sync/vjGuard.h>
#include <Kernel/vjDebug.h>


//-----------------------------------------------------------------
//: Return a new table of "fresh" objects.
//! NOTE: This is NOT a copy. All objects are created by their
//+       default constructors
//-----------------------------------------------------------------
vjTSTable* vjTSTable::createNew()
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
vjTSTable::~vjTSTable()
{
   // For all elements in the table
   for(int i=0;i<mTSObjects.size();i++)
      if(mTSObjects[i] != NULL)        // If valid object
         delete mTSObjects[i];         // Delete them
}

//: Get the object with the spcified key
vjTSBaseObject* vjTSTable::getObject(int objectKey)
{
vjGuard<vjMutex>  guard(mListGuard);
   vjASSERT((objectKey >= 0) && (objectKey < mTSObjects.size()));
   return mTSObjects[objectKey];
}


//-----------------------------------------------------------------
//: Set an object entry in the table.
//-----------------------------------------------------------------
void vjTSTable::setObject(vjTSBaseObject* object, long key)
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
void vjTSTable::releaseObject(long key)
{
vjGuard<vjMutex>  guard(mListGuard);
   vjASSERT( (key>=0) && (key<mTSObjects.size()) );
   if (mTSObjects[key] != NULL)
      delete mTSObjects[key];
   mTSObjects[key] = NULL;
}
