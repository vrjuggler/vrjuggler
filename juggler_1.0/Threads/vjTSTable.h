#ifndef _VJ_TS_TABLE_H_
#define _VJ_TS_TABLE_H_
#pragma once

#include <vjConfig.h>
#include <Threads/vjTSObject.h>
#include <Kernel/vjDebug.h>

//-----------------------------------------------------------------
//: This class is the actual TS Table.
//
// This class maintains a table that has ptrs to all the TS data
// in the system.
//-----------------------------------------------------------------
class vjTSTable
{
public:
   vjTSTable()
   {;}

   //-----------------------------------------------------------------
   //: Return a newly created table.
   //! NOTE: This is not a copy.
   //-----------------------------------------------------------------
   vjTSTable* createNew()
   {
      vjTSTable* new_table = new vjTSTable;

      // For all elements in the table
      for(int i=0;i<mTSObjects.size();i++)
         if(mTSObjects[i] != NULL)
            new_table->setObject(mTSObjects[i]->createNew(), i);

      return new_table;    // Return the newly created table
   }

   ~vjTSTable()
   {
      // For all elements in the table
      for(int i=0;i<mTSObjects.size();i++)
         if(mTSObjects[i] != NULL)        // If valid object
            delete mTSObjects[i];         // Delete them
   }

   vjTSBaseObject* getObject(int objectKey)
   {
      vjASSERT((objectKey >= 0) && (objectKey < mTSObjects.size()));
      return mTSObjects[objectKey];
   }

public:
   //-----------------------------------------------------------------
   //: Set an object entry in the table.
   //-----------------------------------------------------------------
   void setObject(vjTSBaseObject* object, long key)
   {
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
      vjASSERT( (key>=0) && (key<mTSObjects.size()) );
      if (mTSObjects[key] != NULL)
         delete mTSObjects[key];
      mTSObjects[key] = NULL;
   }

public:
//   friend vjThreadManager;

private:
   std::vector<vjTSBaseObject*> mTSObjects; //: Map object key to TS Object ptr
};

#endif
