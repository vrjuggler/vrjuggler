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

int vjConfigChunkHandler::configProcessPending(bool lockIt)
{
   vjConfigManager*     cfg_mgr = vjConfigManager::instance();
   vjDependencyManager* dep_mgr = vjDependencyManager::instance();

   bool     scan_for_lost_dependants(false);       // Do we need to scan for un-filled dependencies

   // We need to track the number vefore and after to know if we changed anything
   int num_pending_before = cfg_mgr->getNumPending();
   int num_pending_after(0);

   vjDEBUG_BEGIN(vjDBG_ALL,vjDBG_STATE_LVL) << typeid(*this).name() << "::configProcessPending: Entering: "
                              << num_pending_before << " items pending.\n" << vjDEBUG_FLUSH;

   if(lockIt)
      cfg_mgr->lockPending();     // We need to lock the pending first
   {
      std::list<vjConfigManager::vjPendingChunk>::iterator current, end, remove_me;
      current = cfg_mgr->getPendingBegin();
      end = cfg_mgr->getPendingEnd();

      // --- For each item in pending list --- //
      while(current != end)
      {
         vjConfigChunk* cur_chunk = (*current).mChunk;         
         
         // If the current handler (this) knows about the chunk
         if(this->configCanHandle(cur_chunk))
         {                        
            // ---- HANDLE THE CHUNK ---- //
            switch ((*current).mType)
            {
            case vjConfigManager::vjPendingChunk::ADD:         // CONFIG ADD              
               if(dep_mgr->depSatisfied(cur_chunk))            // Are all the dependencies satisfied
               {
                  bool added = this->configAdd(cur_chunk);
                  if(added)                                 // Was there success adding
                  {
                     remove_me = current;
                     current++;                          // Goto next item
                     cfg_mgr->removePending(remove_me);  // Delete previous item
                     cfg_mgr->addActive(cur_chunk);      // Add it to the current config                     
                     vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL) 
                                                 << "Pending item [ADD]: " << cur_chunk->getProperty("name")
                                                 << " type: " << (std::string)cur_chunk->getType()
                                                 << "  --> Successfully added.\n\n" << vjDEBUG_FLUSH;
                  }
                  else  // Failed to add
                  {
                     vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL) 
                                                 << "Pending item [ADD]: " << cur_chunk->getProperty("name")
                                                 << " type: " << (std::string)cur_chunk->getType()
                                                 << "  --> failed to add correctly.\n\n" << vjDEBUG_FLUSH;
                     current++;
                  }
               }
               else     // Dependency failed
               {
                  vjDEBUG(vjDBG_ALL,vjDBG_WARNING_LVL) 
                                                 << "Pending item [ADD]: " << cur_chunk->getProperty("name")
                                                 << " type: " << (std::string)cur_chunk->getType()
                                                 << "  -->  Config Add: Dependencies are not loaded yet.\n\n" << vjDEBUG_FLUSH;
                  dep_mgr->debugOutDependencies(cur_chunk,vjDBG_WARNING_LVL);
                  current++;
               }
               break;
            
            case vjConfigManager::vjPendingChunk::REMOVE:      // Config remove
               {
                  bool removed = this->configRemove(cur_chunk);
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
               }
               break;
            
            default:
               current++;  // Goto next entry
               break;
            }            
         }    
         // ---- CAN'T HANDLE THE CHUNK --- //
         else           // if(can_handle)
         {
            vjDEBUGlg(vjDBG_ALL,vjDBG_STATE_LVL,false,true) 
                                                 << "Pending item: " << cur_chunk->getProperty("name")
                                                 << " type: " << (std::string)cur_chunk->getType()
                                                 << " --> Not handled by this handler.\n" << vjDEBUG_FLUSH;
            current++;
         }
                  
      }        // END: while(current != end)

   }
   if(lockIt)
      cfg_mgr->unlockPending();   // Unlock it

   num_pending_after = cfg_mgr->getNumPending();

   vjDEBUG_END(vjDBG_ALL,vjDBG_STATE_LVL) 
                                        << "              Exiting: "
                                        << num_pending_after << " items now pending ==> We processed "
                                        << (num_pending_before-num_pending_after) << " items.\n" << vjDEBUG_FLUSH;

   // Check for items that have lost their dependencies dues to a remove item being processed
   if(scan_for_lost_dependants)
   {
      cfg_mgr->scanForLostDependencies();
   }

   return (num_pending_before-num_pending_after);
}


