/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
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

#ifndef _VJ_QUEUED_CONFIG_CHUNK_HANDLER_H_
#define _VJ_QUEUED_CONFIG_CHUNK_HANDLER_H_


#include <vjConfig.h>
#include <Kernel/vjDebug.h>
#include <Config/vjConfigChunk.h>
class vjConfigChunkDB;
#include <Sync/vjGuardedQueue.h>
#include <Kernel/vjConfigChunkHandler.h>

//: Class that holds configuration queues for the system objects
class vjQueuedConfigChunkHandler : public vjConfigChunkHandler
{
public:
   // --- Chunk Databases --- //
   //: Add a database of config chunks
   // Just loops through the chunks and calls configAdd for each one
   virtual bool configAddDB(vjConfigChunkDB* chunkDB);

   //: Remove all config chunks in the given chunk db
   virtual bool configRemoveDB(vjConfigChunkDB* chunkDB);

   // ---- Inidividual chunks --- //
   //: Add a config chunk
   // Puts chunk into the add queue
   virtual bool configAdd(vjConfigChunk* chunk)
   {
      vjDEBUG(vjDBG_ALL,1) << "vjQueuedConfigChunkHandler::configAdd: Adding chunk to add queue\n" << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_ALL,4) << chunk << ":\n" << *chunk << endl << vjDEBUG_FLUSH;
      mConfigAddQueue.push(chunk);
      vjASSERT(chunk != NULL);
      return true;
   }

   //: Remove a config chunk
   // Puts chunk into the add queue
   virtual bool configRemove(vjConfigChunk* chunk)
   {
      vjDEBUG(vjDBG_ALL,1) << "vjQueuedConfigChunkHandler::configRemove: Adding chunk to remove queue\n" << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_ALL,4) << chunk << ":\n" << *chunk << endl << vjDEBUG_FLUSH;
      mConfigRemoveQueue.push(chunk);
      vjASSERT(chunk != NULL);
      return true;
   }


   //: Check the queues and process any chunks found therein
   //! NOTE: This should most likely only be called from the control thread of the object
   void checkConfigQueues()
   {
      // ---- RECONFIGURATION --- //
      if(!mConfigAddQueue.empty())
         processConfigAddQueue();

      if(!mConfigRemoveQueue.empty())
         processConfigRemoveQueue();

      // --- Ouput info --- //
      outputQueueStats();
   }

   void outputQueueStats()
   {
      int add_size = mConfigAddQueue.size();
      int remove_size = mConfigRemoveQueue.size();
      if((add_size != 0) && (remove_size != 0))
      {
         vjDEBUG(vjDBG_ALL,0) << "addQueue:    size: " << add_size << endl << vjDEBUG_FLUSH;
         vjDEBUG(vjDBG_ALL,0) << "removeQueue: size: " << remove_size << endl << vjDEBUG_FLUSH;
      }
   }

protected:
   //: Takes any chunks in add queue and adds them to running system
   void processConfigAddQueue();

   //: Takes any chunks in remove queue and reconfigures system by removing them
   void processConfigRemoveQueue();


   //: Take care of adding a single chunk
   //! RETVAL: true - Chunk has been added
   virtual bool processChunkAdd(vjConfigChunk* chunk) = 0;

   //: Take care of removing a single chunk
   //! RETVAL: true - Chunk has been added
   virtual bool processChunkRemove(vjConfigChunk* chunk) = 0;

protected:
   vjGuardedQueue<vjConfigChunk*> mConfigAddQueue;      //: A queue of chunDB's to reconfig from
   vjGuardedQueue<vjConfigChunk*> mConfigRemoveQueue;   //: A queue of chunkDB's to remove
};

#endif

