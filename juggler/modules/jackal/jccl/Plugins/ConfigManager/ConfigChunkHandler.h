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


#ifndef _VJ_CONFIG_CHUNK_HANDLER_H_
#define _VJ_CONFIG_CHUNK_HANDLER_H_

#include <vjConfig.h>
#include <Config/vjConfigChunk.h>
#include <Kernel/vjConfigManager.h>
#include <Kernel/vjDependencyManager.h>

//-----------------------------------------
//: Base class for all classes that can handle config chunks
//
// This means that they can be "configured"
// and queried using the specified interface.
//
//---------------------------------------------
//! PUBLIC_API:
class vjConfigChunkHandler
{
public:
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   //! RETURNS: success
   virtual bool configAdd(vjConfigChunk* chunk)
   { return false; }

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   //!RETURNS: success
   virtual bool configRemove(vjConfigChunk* chunk)
   { return false; }

   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   virtual bool configCanHandle(vjConfigChunk* chunk)
   { return false; }

   //: Process any pending reconfiguration that we can deal with
   // This function processes each pending reconfiguration in configuration manager
   // The default implementation does the following. <br>
   // if(this->configCanHandle && passesDepCheck)
   //    configAdd/configRemove(chunk)
   // if(success)
   //   add/remove from pending
   //   remove/add from current
   //
   //! ARGS: lockIt - Should we lock the cfg_mgr pending list
   //+       defaults to true.  The only time this should be false is 
   //+       if you override this function and still want to make use of it's abilities
   //+         (ex.  The kernel needs to do this because if has to call processPending on other managers in addition to itself)
   virtual void processPending(bool lockIt = true)
   {
      vjConfigManager*     cfg_mgr = vjConfigManager::instance();
      vjDependencyManager* dep_mgr = vjDependencyManager::instance();
      cfg_mgr->lockPending();     // We need to lock the pending first
      {
         std::list<vjConfigManager::vjPendingChunk>::iterator current, end, remove_me;
         current = cfg_mgr->getPendingBegin();
         end = cfg_mgr->getPendingEnd();

         while(current != end)
         {
            vjConfigChunk* cur_chunk = (*current).mChunk;

            // CONFIG ADD
            if((*current).mType == vjConfigManager::vjPendingChunk::ADD)
            {           
               if(this->configCanHandle(cur_chunk) &&
                  dep_mgr->depSatisfied(cur_chunk))
               {
                  bool added = this->configAdd(cur_chunk);
                  if(added)      // Was there success adding
                  {
                     remove_me = current;
                     current++;                          // Goto next item
                     cfg_mgr->removePending(remove_me);  // Delete previous item
                     cfg_mgr->lockActive();
                        cfg_mgr->addActive(cur_chunk);     // Add it to the current config
                     cfg_mgr->unlockActive();
                  }
                  else // Failed to add
                  {
                     current++;
                  }
               }
            }
            // CONFIG REMOVE
            else if((*current).mType == vjConfigManager::vjPendingChunk::REMOVE)
            {
               current++;
            }
         }        // END: while(current != end)

      }
      cfg_mgr->unlockPending();   // Unlock it
   }
};

#endif
