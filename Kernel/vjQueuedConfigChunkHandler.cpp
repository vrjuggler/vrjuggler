#include <vjConfig.h>
#include <Kernel/vjQueuedConfigChunkHandler.h>
#include <Kernel/vjKernel.h>

//: Add a database of config chunks
// Just loops through the chunks and calls configAdd for each one
bool vjQueuedConfigChunkHandler::configAddDB(vjConfigChunkDB* chunkDB)
{
   // ---- DEPENDENCY SORT  ---- //
   int dep_result = chunkDB->dependencySort(vjKernel::instance()->getChunkDB());

   // If dependency sort fails, exit with error
   if(dep_result == -1)
   {
      vjDEBUG(vjDBG_ERROR,0) << "vjQueuedConfiChunkHandler::processConfigAddQueue: ERROR: Dependency sort failed. Aborting add.\n" << vjDEBUG_FLUSH;
      return false;
   }

   std::vector<vjConfigChunk*> sorted_chunks = chunkDB->getChunks();  // Get sorted list of chunks to add

   //vjDEBUG_BEGIN(vjDBG_ALL,1) << "vjQueuedConfiChunkHandler: processConfigAddQueue: Processing chunks.\n" << vjDEBUG_FLUSH;
   //std::vector<vjConfigChunk*> chunks = chunkDB->getChunks();

   // For each element in chunk list
   for (int i=0;i<sorted_chunks.size();i++)
      configAdd(sorted_chunks[i]);

   return true;
}

bool vjQueuedConfigChunkHandler::configRemoveDB(vjConfigChunkDB* chunkDB)
{
   vjDEBUG_BEGIN(vjDBG_ALL,1) << "vjQueuedConfiChunkHandler: processConfigAddQueue: Processing chunks.\n" << vjDEBUG_FLUSH;

   std::vector<vjConfigChunk*> chunks = chunkDB->getChunks();

   // For each element in chunk list
   for (int i=0;i<chunks.size();i++)
      configRemove(chunks[i]);

   return true;
}


void vjQueuedConfigChunkHandler::processConfigAddQueue()
{
   vjDEBUG_BEGIN(vjDBG_ALL,1) << "vjQueuedConfiChunkHandler: processConfigAddQueue: Processing chunks.\n" << vjDEBUG_FLUSH;
   //vjASSERT(vjThread::self() == mControlThread);      // ASSERT: We are being called from kernel thread

   vjConfigChunk* chunk;     // Chunk db to add

   while (!mConfigAddQueue.empty())
   {
      // -- GET CHUNK to work with -- //
      chunk = mConfigAddQueue.front();
      mConfigAddQueue.pop();
      vjASSERT(chunk != NULL);

      vjDEBUG(vjDBG_ALL,3) << "Processing chunk: " << chunk << endl << *chunk << vjDEBUG_FLUSH;

      // --- ADD CHUNK -- //
      bool added_chunk = false;        // Flag: true - chunk was added
      added_chunk = processChunkAdd(chunk);      // PROCESS CHUNK

      // --- ADD to ACTIVE CHUNKDB --- //
      if (added_chunk)      // if added => add to config database
      {
         vjASSERT(chunk != NULL);
         vjKernel::instance()->getChunkDB()->addChunk(chunk);    // Add to global DB
         int num_chunks = vjKernel::instance()->getChunkDB()->getChunks().size();
         vjDEBUG(vjDBG_ALL,1) << "vjQueuedConfiChunkHandler::processConfigAddQueue: Added chunk: " << chunk->getProperty("name") << ", Now have " << num_chunks << " chunks.\n" << vjDEBUG_FLUSH;
      }
      else                 // Else: Give unrecognized error
      {
         vjDEBUG(vjDBG_ALL,0) << "vjQueuedConfiChunkHandler::processConfigAddQueue: Unrecognized chunk.\n"
         << "   type: " << chunk->getType() << endl << vjDEBUG_FLUSH;
      }
   }

   // Tell the environment manager to refresh
//**//   environmentManager->sendRefresh();
   vjDEBUG_END(vjDBG_ALL,1) << "vjQueuedConfiChunkHandler: configAdd: Done adding\n\n" << vjDEBUG_FLUSH;
}


void vjQueuedConfigChunkHandler::processConfigRemoveQueue()
{
   //vjASSERT(vjThread::self() == mControlThread);      // ASSERT: We are being called from kernel thread
   vjDEBUG_BEGIN(vjDBG_ALL,1) << "vjQueuedConfigChunkHandler: processConfigRemoveQueue: Removing chunk.\n" << vjDEBUG_FLUSH;

   vjConfigChunk* chunk;

   while(!mConfigRemoveQueue.empty())
   {
      chunk = mConfigRemoveQueue.front();
      mConfigRemoveQueue.pop();
      vjASSERT(chunk != NULL);

      // Remove the chunk
      bool removed_chunk = false;        // Flag: true - chunk was removed

      // Find manager to handle them
      removed_chunk = processChunkRemove(chunk);

      // --- Check for removal from active config --- //
      if(removed_chunk)      // if removed => remove from config database
      {
         vjASSERT(chunk != NULL);
         vjKernel::instance()->getChunkDB()->removeNamed(chunk->getProperty("name"));
         int num_chunks = vjKernel::instance()->getChunkDB()->getChunks().size();
         vjDEBUG(vjDBG_ALL,3) << "vjQueuedConfigChunkHandler::processConfigRemoveQueue: Removed chunk: Now have " << num_chunks << " chunks.\n" << vjDEBUG_FLUSH;
      }
      else                 // Else: Give unrecognized error
      {
         vjDEBUG(vjDBG_ALL,0) << "vjQueuedConfigChunkHandler::processConfigRemoveQueue: Unrecognized chunk.\n"
                    << "   type: " << chunk->getType() << endl << vjDEBUG_FLUSH;
      }
   }

   vjDEBUG_END(vjDBG_ALL,1) << "vjQueuedConfigChunkHandler: processConfigRemoveQueue: Exiting.\n" << vjDEBUG_FLUSH;

   // Tell the environment manager to refresh
//**//   environmentManager->sendRefresh();
}
