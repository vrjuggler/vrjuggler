/*
 *  File:       $RCSfile$
 *  Date modified:  $Date$
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

#include <vjConfig.h>
#include <Kernel/vjConfigChunkHandler.h>
#include <Kernel/vjConfigManager.h>
#include <Config/vjConfigChunk.h>
#include <Kernel/vjDependencyManager.h>
#include <Kernel/vjDebug.h>
#include <typeinfo>

void vjConfigChunkHandler::configProcessPending(bool lockIt)
{
   vjConfigManager*     cfg_mgr = vjConfigManager::instance();
   vjDependencyManager* dep_mgr = vjDependencyManager::instance();

   bool     scan_for_lost_dependants(false);       // Do we need to scan for un-filled dependencies

   int initial_num_pending = cfg_mgr->getNumPending();

   vjDEBUG_BEGIN(vjDBG_ALL,0) << typeid(*this).name() << "::configProcessPending: Entering: "
                              << initial_num_pending << " items pending.\n" << vjDEBUG_FLUSH;

   cfg_mgr->lockPending();     // We need to lock the pending first
   {
      std::list<vjConfigManager::vjPendingChunk>::iterator current, end, remove_me;
      current = cfg_mgr->getPendingBegin();
      end = cfg_mgr->getPendingEnd();

      while(current != end)
      {
         vjConfigChunk* cur_chunk = (*current).mChunk;

         vjDEBUG_BEGINlg(vjDBG_ALL,1,false,true) << "Processing: item: " << cur_chunk->getProperty("name")
                                                 << " type: " << (std::string)cur_chunk->getType() << vjDEBUG_FLUSH;
         
         // If we know about it
         if(this->configCanHandle(cur_chunk))
         {                        
            bool removed;

            // ---- HANDLE THE CHUNK ---- //
            switch ((*current).mType)
            {
            case vjConfigManager::vjPendingChunk::ADD:         // CONFIG ADD
               vjDEBUGlg(vjDBG_ALL,1,false,false) << " --> Config Add.\n" << vjDEBUG_FLUSH;
               if(dep_mgr->depSatisfied(cur_chunk))            // Are all the dependencies satisfied
               {
                  bool added = this->configAdd(cur_chunk);
                  if(added)                                 // Was there success adding
                  {
                     remove_me = current;
                     current++;                          // Goto next item
                     cfg_mgr->removePending(remove_me);  // Delete previous item
                     cfg_mgr->addActive(cur_chunk);      // Add it to the current config                     
                     vjDEBUGlg(vjDBG_ALL,1,false,true) << "Successfully added." << vjDEBUG_FLUSH;
                  }
                  else  // Failed to add
                  {
                     vjDEBUGlg(vjDBG_ALL,1,false,true) << "failed to add correctly." << vjDEBUG_FLUSH;
                     current++;
                  }
               }
               else     // Dependency failed
               {
                  vjDEBUGlg(vjDBG_ALL,1,false,true) << "Dependencies are not loaded yet." << vjDEBUG_FLUSH;
                  dep_mgr->debugOutDependencies(cur_chunk,1);
                  current++;
               }
               break;
            
            case vjConfigManager::vjPendingChunk::REMOVE:      // Config remove
               removed = this->configRemove(cur_chunk);
               if(removed)      // Was there success adding
               {
                  remove_me = current;
                  current++;                          // Goto next item
                  cfg_mgr->removePending(remove_me);  // Delete previous item
                  cfg_mgr->removeActive(cur_chunk->getProperty("name"));     // Add it to the current config                     
                  scan_for_lost_dependants = true;    // We have to scan to see if somebody depended on that chunk
               }
               else // Failed to remove
               {
                  current++;
               }              
               break;
            
            default:
               current++;  // Goto next entry
               break;
            }            
         }    
         // ---- CAN'T HANDLE THE CHUNK --- //
         else
         {
            vjDEBUGlg(vjDBG_ALL,1,false,false) << " --> Not handled by this handler." << vjDEBUG_FLUSH;
            current++;
         }
         
         // Close of the processing begin
         vjDEBUG_ENDlg(vjDBG_ALL,1,false,false) << endl << vjDEBUG_FLUSH;

      }        // END: while(current != end)

   }
   cfg_mgr->unlockPending();   // Unlock it

   vjDEBUG_END(vjDBG_ALL,0) << typeid(*this).name() << "::configProcessPending: Exiting: "
                            << cfg_mgr->getNumPending() << " items now pending ==> We processed "
                            << (initial_num_pending-cfg_mgr->getNumPending()) << " items.\n" << vjDEBUG_FLUSH;

   if(scan_for_lost_dependants)
   {

   }
}


