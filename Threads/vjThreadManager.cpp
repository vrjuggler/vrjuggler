#include <vjConfig.h>
#include <Threads/vjThreadManager.h>
#include <Kernel/vjDebug.h>

vjThreadManager* vjThreadManager::_instance = NULL;

//: Dump the state of the manager to debug
void vjThreadManager::debugDump()
{
   vjDEBUG(vjDBG_ALL,0) << "------- Thread Manager DUMP -------\n" << vjDEBUG_FLUSH;
   vjDEBUG_BEGIN(vjDBG_ALL,0) << "--- Thread List ----\n" << vjDEBUG_FLUSH;
   for (int i=0;i<mThreads.size();i++)
   {
      if (mThreads[i] != NULL)
         vjDEBUG(vjDBG_ALL,0) << i << ": [" << (void*)mThreads[i] << "] "<< mThreads[i] << endl << vjDEBUG_FLUSH;
      else
         vjDEBUG(vjDBG_ALL,0) << i << ": [" << (void*)mThreads[i] << "] No thread\n" << vjDEBUG_FLUSH;
   }
   vjDEBUG_END(vjDBG_ALL,0) << "---------------------\n" << vjDEBUG_FLUSH;
}


//: Add an object to all the tables
// Creates a new key
// Adds object to all tables using that key
long vjThreadManager::addTSObject(vjTSBaseObject* object)
{
   vjGuard<vjMutex> guard(mTSMutex);      // MUTEX Protection
   long new_key = generateNewTSKey();

   mBaseTSTable.setObject(object, new_key);     // Set it in the base table
   for (int i=0;i<mTSTables.size();i++)         // For all thread tables
      mTSTables[i]->setObject(object, new_key); // Add the object

   return new_key;
}


//: Remove objects of given key from the tables
// Requests that each table release the object
void vjThreadManager::removeTSObject(long key)
{
   vjGuard<vjMutex> guard(mTSMutex);      // MUTEX Protection
   mBaseTSTable.releaseObject(key);
   for (int i=0;i<mTSTables.size();i++)   // For all thread tables
      mTSTables[i]->releaseObject(key);
}

//: Return a ptr to the thread table of the current process
// Queries the TID of self.
// Looks up the element in the local TS Table based on this id
vjTSTable* vjThreadManager::getCurrentTSTable()
{
   vjGuard<vjMutex> guard(mTSMutex);		// MUTEX Protection
   vjBaseThread* cur_thread;
   int32_t thread_id;

   cur_thread = vjThread::self();		   // Get current thread
   thread_id = cur_thread->getTID();		// Get thread id

   vjASSERT((thread_id >= 0) && (thread_id < mTSTables.size()));
   return mTSTables[thread_id];                    // Get the table for that id
}

//: Called when a thread has been added to the system
// Creates a new table for the thread
// Puts table in the vector
//! PRE: Manager must be locked
void vjThreadManager::tsThreadAdded(vjBaseThread* thread)
{
   vjASSERT((mThreadVectorMutex.test()) && (mTSMutex.test())); // Assert manager locked
   vjASSERT(thread->valid());

   // Create the new table
   vjTSTable* new_table = mBaseTSTable.createNew();

   // Make sure there is room for new table
   while (mTSTables.size() <= thread->getTID())
      mTSTables.push_back(NULL);

   mTSTables[thread->getTID()] = new_table;
}


//: Called when a thread has been removed from the system
// Deletes the table assigned to that thread
// Remove the reference to that table too
//! PRE: Manager must be locked
void vjThreadManager::tsThreadRemoved(vjBaseThread* thread)
{
   vjASSERT((mThreadVectorMutex.test()) && (mTSMutex.test())); // Assert manager locked
    // Find this threads table
   if (mTSTables[thread->getTID()] != NULL)
      delete mTSTables[thread->getTID()];   // Delete it

   mTSTables[thread->getTID()] = NULL;
}
