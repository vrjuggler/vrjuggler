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
#include <jccl/ConfigManager/ConfigManager.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ChunkFactory.h>
#include <jccl/ConfigManager/DependencyManager.h>
#include <jccl/ConfigManager/ConfigChunkHandler.h>
#include <jccl/JackalServer/JackalServer.h>
#include <jccl/JackalServer/Connect.h>
#include <vpr/Util/Debug.h>



namespace jccl {

    vprSingletonImp(ConfigManager);


    ConfigManager::ConfigManager() {
        mPendingCheckCount = 0;
        mLastPendingSize = 0;
        jackal_server = 0;
        config_communicator = new XMLConfigCommunicator(this);
    }

    
    /*virtual*/ ConfigManager::~ConfigManager () {
        ;
    }



    //-------------------- Pending List Stuff -------------------------------


    // Add the given chunk db to the pending list as adds
    void ConfigManager::addPending (ConfigChunkDB* db) {
        vprASSERT(0 == mPendingLock.test());     // ASSERT: Make sure we don't already have it
        lockPending();

        PendingChunk pending;
        pending.mType = PendingChunk::ADD;
        
        for(std::vector<ConfigChunk*>::iterator i=db->begin();i!=db->end();i++) {
            pending.mChunk = (*i);
            mPendingConfig.push_back(pending);
        }
        
        unlockPending();
        
        // Reset pending count
        mPendingCountMutex.acquire();
        mPendingCheckCount = 0;
        mPendingCountMutex.release();
    }
    


    void ConfigManager::removePending (ConfigChunkDB* db) {
        vprASSERT(0 == mPendingLock.test());     // ASSERT: Make sure we don't already have it
        lockPending();

        PendingChunk pending;
        pending.mType = PendingChunk::REMOVE;
        
        for(std::vector<ConfigChunk*>::iterator i=db->begin();i!=db->end();i++) {
            pending.mChunk = (*i);
            mPendingConfig.push_back(pending);
        }
        
        unlockPending();

        // Reset pending count
        mPendingCountMutex.acquire();
        mPendingCheckCount = 0;
        mPendingCountMutex.release();
    }



    void ConfigManager::removePending(std::list<PendingChunk>::iterator item)
    {
        vprASSERT(1 == mPendingLock.test());
        mPendingConfig.erase(item);
    }



    //: Do we need to check the pending list
    //! CONCURRENCY: concurrent
    // The routine counts the number of pending chunks
    // each time it is called.
    // if it goes pending_repeat_limit calls without
    // changing size, then it returns false until mLastPendingSize changes
    bool ConfigManager::pendingNeedsChecked() {
        const int pending_repeat_limit = 1;    // Must be one or greater.  1 means only allow one time of no changes
        int cur_pending_size = 0;
        bool ret_val = false;
        
        mPendingCountMutex.acquire();
        
        cur_pending_size = mPendingConfig.size();
        if(cur_pending_size != mLastPendingSize) {
            ret_val = true;                           // Flag it for a check
            mPendingCheckCount=0;                     // Reset the counter
            mLastPendingSize = cur_pending_size;      // Keep track of size
        }
        else if(mPendingCheckCount < pending_repeat_limit) {
            // allowed in at least once [1...pending_repeat_limit]
            mPendingCheckCount++;   // Increment it
                    
            if(mPendingCheckCount < pending_repeat_limit) {
                ret_val = true;        // Repeats still allowed
            }
            else {
                vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                    << "ConfigManager::pendingNeedsChecked: Pending list is now\n"
                    << vprDEBUG_FLUSH;
                vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                    << clrOutNORM(clrGREEN,"STALE: ")
                    << cur_pending_size << " items still in pending\n"
                    << vprDEBUG_FLUSH;
                vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                    << "NOTE: These items have been specified in configuration,\n"
                    << vprDEBUG_FLUSH;
                vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                    << "      but have not been loaded.\n" << vprDEBUG_FLUSH;
                vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                    << "      This may be an error in the configuration OR\n"
                    << vprDEBUG_FLUSH;
                vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                    << "      it may be waiting for more configuration information.\n"
                    << vprDEBUG_FLUSH;
                //            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << vprDEBUG_FLUSH;
                
                lockPending();
                debugDumpPending(vprDBG_CRITICAL_LVL); // Output the stale pending list
                unlockPending();
                
                ret_val = false;
            }
        }
        else {
            ret_val = false;
        }

        mPendingCountMutex.release();
        
        return ret_val;
    }



    void ConfigManager::addPending(PendingChunk& pendingChunk) {
        vprASSERT(0 == mPendingLock.test());
        lockPending();
        mPendingConfig.push_back(pendingChunk);
        unlockPending();
        
        // Reset pending count
        mPendingCountMutex.acquire();
        mPendingCheckCount = 0;
        mPendingCountMutex.release();
    }



    // Look for items in the active list that don't have their dependencies filled anymore
    //
    //! POST: Any chunks in active with dependencies not filled are added to the
    //+       the pending list. (A remove and an add are added to the pending)
    //! RETURNS: The number of lost dependencies found
    int ConfigManager::scanForLostDependencies() {
        vprASSERT(0 == mActiveLock.test());        
        // We can't hold the lock upon entry

        vprDEBUG_BEGIN(vprDBG_ALL,vprDBG_CONFIG_LVL) << "ConfigManager::scanForLostDependencies: Entered: \n" << vprDEBUG_FLUSH;

        DependencyManager* dep_mgr = DependencyManager::instance();
        std::vector<ConfigChunk*> chunks;
        int num_lost_deps(0);

        // NOTE: can't hold this lock because the depSatisfied routines make use of the activeLock as well
        // NOTE: Make the copy of the chunks so that we can iterate without fear of active changing
        mActiveLock.acquire();
        chunks = mActiveConfig.getChunks();   // Get a copy of the chunks
        mActiveLock.release();

        // Now test them
        for(unsigned int i=0;i<chunks.size();i++) {
            if(!dep_mgr->depSatisfied(chunks[i])) {     // We are not satisfied
      
                vprDEBUG_NEXT(vprDBG_ALL,1) << chunks[i]->getProperty("name")
                                            << " type: " << ((std::string)chunks[i]->getType()).c_str()
                                            << " has lost dependencies.\n"
                                            << vprDEBUG_FLUSH;
                
                num_lost_deps++;              // Keep a count of the number lost deps found

                // Add the pending removal
                PendingChunk pending;
                pending.mType = PendingChunk::REMOVE;
                pending.mChunk = chunks[i];
                addPending(pending);
                
                // Add the pending re-addition
                ConfigChunk* copy_of_chunk;          // Need a copy so that the remove can delete the chunk
                copy_of_chunk = new ConfigChunk (*chunks[i]);
                pending.mType = PendingChunk::ADD;
                pending.mChunk = copy_of_chunk;
                addPending(pending);                   // Add the add item
            }
        }

        vprDEBUG_END(vprDBG_ALL,1) << "ConfigManager::scanForLostDependencies: Exiting: \n" << vprDEBUG_FLUSH;
        
        return num_lost_deps;
    }



    void ConfigManager::debugDumpPending(int debug_level) {
        vprASSERT(1 == mPendingLock.test());
        vprDEBUG(vprDBG_ALL,debug_level)
            << clrSetNORM(clrGREEN)
            << "---- Pending list: " << mPendingConfig.size() << " items ----\n"
            << clrRESET << vprDEBUG_FLUSH;
        std::list<ConfigManager::PendingChunk>::iterator current, end;
        current = getPendingBegin();
        end = getPendingEnd();
        
        while(current != end) {
            ConfigChunk* cur_chunk = (*current).mChunk;
            
            if((*current).mType == PendingChunk::ADD)
                vprDEBUG_NEXT(vprDBG_ALL,debug_level) << "   ADD -->" << vprDEBUG_FLUSH;
            else if((*current).mType == PendingChunk::REMOVE)
                vprDEBUG_NEXT(vprDBG_ALL,debug_level) << "REMOVE -->" << vprDEBUG_FLUSH;
            
            vprDEBUG_CONT(vprDBG_ALL,debug_level) << cur_chunk->getProperty("name")
                                                  << " type: "
                                                  << ((std::string)cur_chunk->getType()).c_str()
                                                  << std::endl << vprDEBUG_FLUSH;
            current++;
        }
        vprDEBUG_NEXT(vprDBG_ALL,debug_level)
            << "----------------------------------\n" << vprDEBUG_FLUSH;
    }


    //------------------ Active List Stuff -------------------------------

    //: Is the chunk in the active configuration??
    //! CONCURRENCY: concurrent
    //! NOTE: This locks the active list to do processing
    bool ConfigManager::isChunkInActiveList(std::string chunk_name)
    {
        vpr::Guard<vpr::Mutex> guard(mActiveLock);     // Lock the current list

        std::vector<ConfigChunk*>::iterator i;
        for(i=mActiveConfig.begin(); i != mActiveConfig.end();i++)
            {
                if(std::string((*i)->getProperty("name")) == chunk_name)
                    return true;
            }
        
        return false;     // Not found, so return false
    }



   //: Add an item to the active configuration
   //! NOTE: This DOES NOT process the chunk
   //+     it just places it into the active configuration list
   //! PRE: Current list must NOT be locked
   void ConfigManager::addActive (ConfigChunk* chunk)
   {
      vprASSERT(0 == mActiveLock.test());
      lockActive();
      mActiveConfig.addChunk(chunk);
      unlockActive();
   }



   //: Erase an item from the list
   //! PRE: Active list must be locked && item must be in list
   //! POST: list = old(list).erase(item) && item is invalid
   void ConfigManager::removeActive (const std::string& chunk_name)
   {
      vprASSERT(0 == mActiveLock.test());
      lockActive();
      mActiveConfig.removeNamed(chunk_name);
      unlockActive();
   }


    //------------------ DynamicReconfig Stuff ------------------------------

    void ConfigManager::addConfigChunkHandler (ConfigChunkHandler* h) {
        chunk_handlers.push_back (h);
    }


    void ConfigManager::removeConfigChunkHandler (ConfigChunkHandler* h) {
        std::vector<ConfigChunkHandler*>::iterator it;
        for (it = chunk_handlers.begin(); it != chunk_handlers.end(); it++) {
            if (*it == h) {
                chunk_handlers.erase(it);
                break;
            }
        }
    }


    int ConfigManager::attemptReconfiguration () {
        int chunks_processed(0);     // Needs to return this value
        if (pendingNeedsChecked()) {
            vprDEBUG_BEGIN(vprDBG_ALL,vprDBG_STATE_LVL) << "ConfigManager::attemptReconfiguration: Examining pending list.\n" << vprDEBUG_FLUSH;
            std::vector<ConfigChunkHandler*>::iterator it = chunk_handlers.begin();
            std::vector<ConfigChunkHandler*>::iterator end = chunk_handlers.end();
            for (; it != end; it++) {
                //chunks_processed += (*it)->configProcessPending (true);
                chunks_processed += attemptHandlerReconfiguration (*it, true);
            }
        }

        return chunks_processed;
    }



    enum PendItemResult { SUCCESS, FAILED, NEED_DEPS };

    void outputPendingItemState(int debugLevel, std::string chunkName, std::string chunkType, PendItemResult result);


    int ConfigManager::attemptHandlerReconfiguration (ConfigChunkHandler* h, bool lockIt) {
        DependencyManager* dep_mgr = DependencyManager::instance();

        // should we scan for lost dependencies?
        bool scan_for_lost_dependants(false);

        // store these to check if we've changed anything.
        int num_pending_before = getNumPending();
        int num_pending_after(0);

        bool success;

        vprDEBUG_BEGIN(vprDBG_ALL,vprDBG_STATE_LVL) << typeid(*this).name() << 
            "::configProcessPending: Entering: " << num_pending_before << 
            " items pending.\n" << vprDEBUG_FLUSH;

        if(lockIt)
            lockPending();     // We need to lock the pending first
   
        std::list<ConfigManager::PendingChunk>::iterator current, end, remove_me;
        current = getPendingBegin();
        end = getPendingEnd();

        // --- For each item in pending list --- //
        while(current != end) {
            // Get information about the current chunk
            ConfigChunk* cur_chunk = (*current).mChunk;
            vprASSERT(cur_chunk != NULL && "Trying to use an invalid chunk");
            std::string chunk_name = cur_chunk->getProperty("name");
            std::string chunk_type = cur_chunk->getType();

            vprDEBUG_BEGIN(vprDBG_ALL,vprDBG_VERB_LVL) << "Item: name:" << 
                chunk_name << " type:" << chunk_type << std::endl << 
                vprDEBUG_FLUSH;

            // If the current handler (this) knows about the chunk
            if (h->configCanHandle (cur_chunk)) {
                // ---- HANDLE THE CHUNK ---- //
                switch ((*current).mType) {

                    // Adding Chunk
                case ConfigManager::PendingChunk::ADD:   
                    if (dep_mgr->depSatisfied(cur_chunk)) {
                        success = h->configAdd(cur_chunk);
                        if (success) {
                            // advance iterator & remove old current.
                            remove_me = current;
                            current++;
                            removePending(remove_me);
                            // update active chunkdb
                            addActive(cur_chunk);
                            
                            outputPendingItemState(vprDBG_CONFIG_LVL,
                                                   cur_chunk->getProperty("name"),
                                                   ((std::string)cur_chunk->getType()).c_str(),
                                                   SUCCESS);
                        }
                        else { // FAILED adding
                            outputPendingItemState(vprDBG_CRITICAL_LVL,
                                                   cur_chunk->getProperty("name"),
                                                   ((std::string)cur_chunk->getType()).c_str(),
                                                   FAILED);
                            current++;
                        }
                    }
                    else {   // Dependencies not met
                        outputPendingItemState(vprDBG_WARNING_LVL,
                                               cur_chunk->getProperty("name"),
                                               ((std::string)cur_chunk->getType()).c_str(),
                                               NEED_DEPS);
                        vprDEBUG_CONT(vprDBG_ALL,vprDBG_WARNING_LVL) << 
                            std::endl << vprDEBUG_FLUSH;
                        dep_mgr->debugOutDependencies(cur_chunk,vprDBG_WARNING_LVL);
                        current++;
                    }
                    break;

                    // Removing Chunk
                case ConfigManager::PendingChunk::REMOVE:
                    success = h->configRemove(cur_chunk);
                    if (success) {
                        // advance iterator & delete old current
                        remove_me = current;
                        current++;
                        removePending(remove_me);
                        // update active chunkdb
                        removeActive(cur_chunk->getProperty("name"));
                        scan_for_lost_dependants = true;
                    }
                    else { // Failed to remove
                        current++;
                    }
                    break;

                    // Default would be from a really nasty bug...
                default:
                    current++;  // Goto next entry
                    break;
                }
            }

            else { 
                // Can't handle the chunk add or remove.
                vprDEBUG_NEXT(vprDBG_ALL,vprDBG_STATE_LVL) << "Pending item: " << 
                    cur_chunk->getProperty("name") << " type: " << 
                    ((std::string)cur_chunk->getType()).c_str() << 
                    " --> Not handled by this handler.\n" << vprDEBUG_FLUSH;
                current++;
            }

            vprDEBUG_END(vprDBG_ALL,vprDBG_VERB_LVL) << "==== End item =====\n" << vprDEBUG_FLUSH;

        }        // END: while(current != end)

        if(lockIt)
            unlockPending();   // Unlock it

        num_pending_after = getNumPending();
        
        vprDEBUG_END(vprDBG_ALL,vprDBG_STATE_LVL)
            << "              Exiting: "
            << num_pending_after << " items now pending ==> We processed "
            << (num_pending_before-num_pending_after) << " items.\n" << vprDEBUG_FLUSH;
        
        // Check for items that have lost their dependencies dues to a remove item being processed
        if(scan_for_lost_dependants) {
            scanForLostDependencies();
        }

        return (num_pending_before-num_pending_after);   
    }



//  void outputPendingItemState(int debugLevel, std::string chunkName, std::string chunkType, PendItemResult result)
//  {
//     const int item_width(25);
//     const int type_width(20);

//     const std::string name_prefix("Pending item: ");
//     const std::string type_prefix(" type: ");
//     vprDEBUG(vprDBG_ALL,debugLevel) << "Pending item: " << std::setiosflags(std::ios::right) << std::setfill(' ') << std::setw(item_width) << chunkName
//                                   <<    "     type: " << std::setiosflags(std::ios::right) << std::setfill(' ') << std::setw(type_width) << chunkType
//                                                       << std::resetiosflags(std::ios::right) << "  ";

//     /*
//     const int prefix_len = name_prefix.length() + type_prefix.length();
//     int item_and_type_len = chunkName.length() + chunkType.length() + prefix_len;
//     const int state_offset(60);

//     for(int c=0;c<(state_offset-item_and_type_len);c++)
//     {
//        vprDEBUG_CONTnl(vprDBG_ALL,debugLevel) << " ";
//     }
//     */

//     switch(result)
//     {
//     case SUCCESS:
//        vprDEBUG_CONTnl(vprDBG_ALL,debugLevel) << "[ " << clrSetNORM(clrGREEN) << "OK" << clrRESET << " ]";
//        break;
//     case FAILED:
//        vprDEBUG_CONTnl(vprDBG_ALL,debugLevel) << "[ " << clrSetNORM(clrRED) << "FAILED" << clrRESET << " ]";
//        break;
//     case NEED_DEPS:
//        vprDEBUG_CONTnl(vprDBG_ALL,debugLevel) << "[ " << clrSetNORM(clrYELLOW) << "NEED DEPS" << clrRESET << " ]";
//        break;
//     }

//     vprDEBUG_CONTnl(vprDBG_ALL,debugLevel) << std::endl << vprDEBUG_FLUSH;
//  }




    //------------------ JackalControl Stuff --------------------------------
    
    /*virtual*/ void ConfigManager::setJackalServer (JackalServer* js) {
        jackal_server = js;
        addConfigChunkHandler (js);
    }
    /*virtual*/ void ConfigManager::addConnect (Connect *c) {
        c->addCommunicator (config_communicator);
        addActive (new ConfigChunk(*(c->getConfiguration())));
    }
    /*virtual*/ void ConfigManager::removeConnect (Connect* c) {
        removeActive (c->getConfiguration()->getName());
    }


}; // namespace jccl
