/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <jccl/jcclConfig.h>
#include <jccl/Plugins/ConfigManager/ConfigChunkHandler.h>
#include <jccl/Plugins/ConfigManager/ConfigManager.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/Plugins/ConfigManager/DependencyManager.h>
#include <jccl/Util/Debug.h>
#include <typeinfo>

namespace jccl {

   enum PendItemResult
   { SUCCESS, FAILED, NEED_DEPS };

void outputPendingItemState(int debugLevel, std::string chunkName, std::string chunkType, PendItemResult result);




int ConfigChunkHandler::configProcessPending()
{
    ConfigManager*     cfg_mgr = ConfigManager::instance();
    DependencyManager* dep_mgr = DependencyManager::instance();

   bool     scan_for_lost_dependants(false);       // Do we need to scan for un-filled dependencies

   // We need to track the number vefore and after to know if we changed anything
   int num_pending_before = cfg_mgr->getNumPending();
   int num_pending_after(0);

   vprDEBUG_BEGIN(vprDBG_ALL,vprDBG_STATE_LVL) << typeid(*this).name() << "::configProcessPending: Entering: "
                              << num_pending_before << " items pending.\n" << vprDEBUG_FLUSH;

//    if(lockIt)
//       cfg_mgr->lockPending();     // We need to lock the pending first
//    {
      std::list<ConfigManager::PendingChunk>::iterator current, end, remove_me;
      current = cfg_mgr->getPendingBegin();
      end = cfg_mgr->getPendingEnd();

      // --- For each item in pending list --- //
      while(current != end)
      {
         // Get information about the current chunk
         ConfigChunkPtr cur_chunk = (*current).mChunk;
         vprASSERT(cur_chunk.get() != NULL && "Trying to use an invalid chunk");
         std::string chunk_name = cur_chunk->getProperty("name");
         std::string chunk_type = cur_chunk->getType();

         vprDEBUG_BEGIN(vprDBG_ALL,vprDBG_VERB_LVL) << "Item: name:" << chunk_name << " type:" << chunk_type << std::endl << vprDEBUG_FLUSH;

         // If the current handler (this) knows about the chunk
         if(this->configCanHandle(cur_chunk))
         {
            // ---- HANDLE THE CHUNK ---- //
            switch ((*current).mType)
            {
            case ConfigManager::PendingChunk::ADD:         // -- CONFIG ADD -- //
               if(dep_mgr->depSatisfied(cur_chunk))            // Are all the dependencies satisfied
               {
                  bool added = this->configAdd(cur_chunk);
                  if(added)                                 // SUCCESS adding
                  {
                     remove_me = current;
                     current++;                          // Goto next item
                     cfg_mgr->removePending(remove_me);  // Delete previous item
                     cfg_mgr->addActive(cur_chunk);      // Add it to the current config

                     outputPendingItemState(vprDBG_CONFIG_LVL,
                                            cur_chunk->getProperty("name"),
                                            ((std::string)cur_chunk->getType()).c_str(),
                                            SUCCESS);
                  }
                  else  // FAILED adding
                  {
                     outputPendingItemState(vprDBG_CRITICAL_LVL,
                                            cur_chunk->getProperty("name"),
                                            ((std::string)cur_chunk->getType()).c_str(),
                                            FAILED);
                     current++;
                  }
               }
               else     // Dependency failed
               {

                  outputPendingItemState(vprDBG_WARNING_LVL,
                                            cur_chunk->getProperty("name"),
                                            ((std::string)cur_chunk->getType()).c_str(),
                                            NEED_DEPS);
                  vprDEBUG_CONT(vprDBG_ALL,vprDBG_WARNING_LVL) << std::endl << vprDEBUG_FLUSH;
                  dep_mgr->debugOutDependencies(cur_chunk,vprDBG_WARNING_LVL);
                  current++;
               }
               break;

            case ConfigManager::PendingChunk::REMOVE:      // Config remove
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
            vprDEBUG_NEXT(vprDBG_ALL,vprDBG_STATE_LVL) << "Pending item: " << cur_chunk->getProperty("name")
                                                 << " type: " << ((std::string)cur_chunk->getType()).c_str()
                                                 << " --> Not handled by this handler.\n" << vprDEBUG_FLUSH;
            current++;
         }
         vprDEBUG_END(vprDBG_ALL,vprDBG_VERB_LVL) << "==== End item =====\n" << vprDEBUG_FLUSH;

      }        // END: while(current != end)

//    }
//    if(lockIt)
//       cfg_mgr->unlockPending();   // Unlock it

   num_pending_after = cfg_mgr->getNumPending();

   vprDEBUG_END(vprDBG_ALL,vprDBG_STATE_LVL)
                                        << "              Exiting: "
                                        << num_pending_after << " items now pending ==> We processed "
                                        << (num_pending_before-num_pending_after) << " items.\n" << vprDEBUG_FLUSH;

   // Check for items that have lost their dependencies dues to a remove item being processed
   if(scan_for_lost_dependants)
   {
      cfg_mgr->scanForLostDependencies();
   }

   return (num_pending_before-num_pending_after);
}



void outputPendingItemState(int debugLevel, std::string chunkName, std::string chunkType, PendItemResult result)
{
   const int item_width(25);
   const int type_width(20);

   const std::string name_prefix("Pending item: ");
   const std::string type_prefix(" type: ");
   vprDEBUG(vprDBG_ALL,debugLevel) << "Pending item: " << std::setiosflags(std::ios::right) << std::setfill(' ') << std::setw(item_width) << chunkName
                                 <<    "     type: " << std::setiosflags(std::ios::right) << std::setfill(' ') << std::setw(type_width) << chunkType
                                                     << std::resetiosflags(std::ios::right) << "  ";

   /*
   const int prefix_len = name_prefix.length() + type_prefix.length();
   int item_and_type_len = chunkName.length() + chunkType.length() + prefix_len;
   const int state_offset(60);

   for(int c=0;c<(state_offset-item_and_type_len);c++)
   {
      vprDEBUG_CONTnl(vprDBG_ALL,debugLevel) << " ";
   }
   */

   switch(result)
   {
   case SUCCESS:
      vprDEBUG_CONTnl(vprDBG_ALL,debugLevel) << "[ " << clrSetNORM(clrGREEN) << "OK" << clrRESET << " ]";
      break;
   case FAILED:
      vprDEBUG_CONTnl(vprDBG_ALL,debugLevel) << "[ " << clrSetNORM(clrRED) << "FAILED" << clrRESET << " ]";
      break;
   case NEED_DEPS:
      vprDEBUG_CONTnl(vprDBG_ALL,debugLevel) << "[ " << clrSetNORM(clrYELLOW) << "NEED DEPS" << clrRESET << " ]";
      break;
   }

   vprDEBUG_CONTnl(vprDBG_ALL,debugLevel) << std::endl << vprDEBUG_FLUSH;
}




}; // namespace jccl

