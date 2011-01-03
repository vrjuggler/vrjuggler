/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <jccl/jcclConfig.h>
#include <iomanip>
#include <typeinfo>
#include <string>
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/Config/ConfigElement.h>
#include <jccl/RTRC/DependencyManager.h>
#include <jccl/Util/Debug.h>
#include <jccl/RTRC/ConfigElementHandler.h>

namespace jccl
{

enum PendItemResult
{ SUCCESS, FAILED, NEED_DEPS };


static void outputPendingItemState(int debugLevel,
                                   const std::string& elementName,
                                   const std::string& elementType,
                                   PendItemResult result);


int ConfigElementHandler::configProcessPending()
{
   ConfigManager*     cfg_mgr = ConfigManager::instance();
   DependencyManager* dep_mgr = DependencyManager::instance();

   bool scan_for_lost_dependants(false);       // Do we need to scan for un-filled dependencies

   // We need to track the number vefore and after to know if we changed anything
   std::list<ConfigManager::PendingElement>::size_type num_pending_before =
      cfg_mgr->getNumPending();
   std::list<ConfigManager::PendingElement>::size_type num_pending_after(0);

   vprDEBUG_BEGIN(vprDBG_ALL,vprDBG_STATE_LVL)
      << typeid(*this).name() << "::configProcessPending: Entering: "
      << num_pending_before << " items pending.\n" << vprDEBUG_FLUSH;

   // Perform the work of processing pending config elements.
   {
      std::list<ConfigManager::PendingElement>::iterator current, end, remove_me;
      current = cfg_mgr->getPendingBegin();
      end = cfg_mgr->getPendingEnd();

      // --- For each item in pending list --- //
      while(current != end)
      {
         // Get information about the current element
         ConfigElementPtr cur_element = (*current).mElement;
         vprASSERT(cur_element.get() != NULL && "Trying to use an invalid element");
         std::string element_name = cur_element->getName();
         std::string element_type = cur_element->getID();

         vprDEBUG_BEGIN(vprDBG_ALL,vprDBG_VERB_LVL)
            << "Item: name:" << element_name << " type:" << element_type
            << std::endl << vprDEBUG_FLUSH;

         // If the current handler (this) knows about the element
         if(this->configCanHandle(cur_element))
         {
            // ---- HANDLE THE ELEMENT ---- //
            switch ((*current).mType)
            {
            case ConfigManager::PendingElement::ADD:         // -- CONFIG ADD -- //
               if(dep_mgr->isSatisfied(cur_element))         // Are all the dependencies satisfied
               {
                  bool added = this->configAdd(cur_element);
                  if(added)                                 // SUCCESS adding
                  {
                     remove_me = current;
                     current++;                          // Goto next item
                     cfg_mgr->removePending(remove_me);  // Delete previous item
                     cfg_mgr->addActive(cur_element);    // Add it to the current config

                     outputPendingItemState(vprDBG_CONFIG_STATUS_LVL,
                                            element_name, element_type,
                                            SUCCESS);
                  }
                  else  // FAILED adding
                  {
                     outputPendingItemState(vprDBG_CRITICAL_LVL, element_name,
                                            element_type, FAILED);
                     current++;
                  }
               }
               else     // Dependency failed
               {
                  outputPendingItemState(vprDBG_CONFIG_LVL, element_name,
                                         element_type, NEED_DEPS);
                  vprDEBUG_CONT(vprDBG_ALL,vprDBG_CONFIG_LVL)
                     << std::endl << vprDEBUG_FLUSH;
                  dep_mgr->debugOutDependencies(cur_element,vprDBG_CONFIG_LVL);
                  current++;
               }
               break;

            case ConfigManager::PendingElement::REMOVE:      // Config remove
               {
                  bool removed = this->configRemove(cur_element);
                  if(removed)      // Was there success adding
                  {
                     remove_me = current;
                     current++;                          // Goto next item
                     cfg_mgr->removePending(remove_me);  // Delete previous item
                     cfg_mgr->removeActive(cur_element->getName());     // Add it to the current config
                     scan_for_lost_dependants = true;    // We have to scan to see if somebody depended on that element
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
         // ---- CAN'T HANDLE THE ELEMENT --- //
         else           // if(can_handle)
         {
            vprDEBUG_NEXT(vprDBG_ALL,vprDBG_STATE_LVL)
               << "Pending item: " << cur_element->getName()
               << " type: " << cur_element->getID()
               << " --> Not handled by this handler.\n" << vprDEBUG_FLUSH;
            current++;
         }

         vprDEBUG_END(vprDBG_ALL,vprDBG_VERB_LVL) << "==== End item =====\n"
                                                  << vprDEBUG_FLUSH;
      }        // END: while(current != end)
   }

   num_pending_after = cfg_mgr->getNumPending();

   vprDEBUG_END(vprDBG_ALL,vprDBG_STATE_LVL)
      << "              Exiting: "
      << num_pending_after << " items now pending ==> We processed "
      << (num_pending_before-num_pending_after) << " items.\n"
      << vprDEBUG_FLUSH;

   // Check for items that have lost their dependencies dues to a remove item being processed
   if(scan_for_lost_dependants)
   {
      cfg_mgr->scanForLostDependencies();
   }

   return (num_pending_before-num_pending_after);
}

void outputPendingItemState(int debugLevel, const std::string& elementName,
                            const std::string& elementType,
                            PendItemResult result)
{
   const int item_width(39);
   const int type_width(20);

   vprDEBUG(vprDBG_ALL,debugLevel)
      << "Type: " << std::setiosflags(std::ios::right)
      << std::setfill(' ') << std::setw(type_width) << elementType
      << std::setiosflags(std::ios::right)
      << std::setfill(' ') << std::setw(item_width) << elementName
      << std::resetiosflags(std::ios::right) << "  ";

   /*
   const int prefix_len = name_prefix.length() + type_prefix.length();
   int item_and_type_len = elementName.length() + elementType.length() + prefix_len;
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

}
