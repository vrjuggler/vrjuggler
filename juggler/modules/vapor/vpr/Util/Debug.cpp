/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <vpr/Sync/Mutex.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/TSObjectProxy.h>
#include <vpr/Util/StreamLock.h>
#include <vpr/Util/Debug.h>


namespace vpr
{

/*
Debug* Debug::_instance = NULL;
Mutex  Debug::_inst_lock;
*/

// Some thread specific global variables
// They are globals because I can't include their type in the vjDEBUG header file
// If I did, then we could not output debug info in the Thread manager itself
TSObjectProxy<std::vector<int> > gVprDebugCurColumn;       // What column to indent to
TSObjectProxy<std::vector<std::string> > gVprDebugCurColor;        // What color to display "everything" in

// Register DEBUG categories
/*

NOTE: These primitive types are instead registered by the constructor (Debug::Debug) directly
as to avoid any initialization order dependencies.

vprREGISTER_DBG_CATEGORY(vprDBG_ALL, DBG_ALL, "DBG:");
vprREGISTER_DBG_CATEGORY(vprDBG_ERROR, DBG_ERROR, "ERR:");
vprREGISTER_DBG_CATEGORY(vprDBG_SIM, DBG_SIM, "I'm a little simulator:");
vprREGISTER_DBG_CATEGORY(vprDBG_VPR, DBG_VPR, "VPR:");
*/


vprSingletonImpWithInitFunc(Debug, init);

Debug::Debug()
{
   indentLevel = 0;     // Initialy don't indent
   debugLevel = 1;      // Should actually try to read env variable
   mUseThreadLocal = false;   // Initially set to false

   char* debug_lev = getenv("VPR_DEBUG_NFY_LEVEL");
   if(debug_lev != NULL)
   {
      debugLevel = atoi(debug_lev);
      std::cout << "VPR_DEBUG_NFY_LEVEL: Set to " << debugLevel << std::endl
                << std::flush;
   } else {
      std::cout << "VPR_DEBUG_NFY_LEVEL: Not found. " << std::endl
                << std::flush;
      std::cout << "VPR_DEBUG_NFY_LEVEL: Defaults to " << debugLevel
                << std::endl << std::flush;
   }

   char* debug_enable = getenv("VPR_DEBUG_ENABLE");
   if(debug_enable != NULL)
   {
      unsigned debug_enable_val = atoi(debug_enable);
      if(debug_enable_val)
         mDebugEnabled = true;
      else
         mDebugEnabled = false;
      std::cout << "VPR_DEBUG_ENABLE: Set to " << mDebugEnabled << std::endl
                << std::flush;
   } else {
      mDebugEnabled = true;
      std::cout << "VPR_DEBUG_ENABLE: Not found. " << std::endl
                << std::flush;
      std::cout << "VPR_DEBUG_ENABLE: Defaults to " << mDebugEnabled
                << std::endl << std::flush;
   }
}

void Debug::init()
{
   // --- Register primitive categories --- //
   /*
   addCategory(vprDBG_ALL, "DBG_ALL", "DBG:");
   addCategory(vprDBG_ERROR, "DBG_ERROR", "ERR:");
   addCategory(vprDBG_SIM, "DBG_SIM", "I'm a little simulator:");
   addCategory(vprDBG_VPR, "DBG_VPR", "VPR:");
   */
}

std::ostream& Debug::getStream(const vpr::DebugCategory& cat, const int level, const bool show_thread_info,
                               const bool use_indent, const int indentChange,
                               const bool lockStream)
{
   // Lock the stream
#ifdef LOCK_DEBUG_STREAM
   if(lockStream)
   {
      debugLock().acquire();     // Get the lock
   }
#endif

   if(indentChange < 0)                // If decreasing indent
      indentLevel += indentChange;

   vprASSERT(indentLevel >= 0 && "Decreased indent below 0, look for bad code");

   // --- Create stream header --- //
   // If we have thread local stuff to do
   if(mUseThreadLocal)
   {
      if((*gVprDebugCurColor).size() == 0)
         std::cout << clrRESET;
      else
         std::cout << clrSetBOLD((*gVprDebugCurColor).back());
   }

   // Autoregister
   if(mCategories.find(cat.mGuid) == mCategories.end())
   {
      addCategory(cat);
   }

   vprASSERT(mCategories.find(cat.mGuid) != mCategories.end() && "Failed to auto-register");

   // Ouput thread info
   // If not, then output space if we are also using indent (assume this means
   // new line used)
   if(show_thread_info)
      std::cout << "[" << vpr::Thread::self() << "] " << (*mCategories.find(cat.mGuid)).second.mPrefix;
   else if(use_indent)
      std::cout << "                  ";


      // Insert the correct number of tabs into the stream for indenting
   if(use_indent)
   {
      for(int i=0;i<indentLevel;i++)
      {
         std::cout << "\t";
      }

      /* For debugging indentation
      // Output the indent level that we are going to of coming from
      if(getLevel() >= 7)
      {
         if(indentChange >0)  // Not incremented yet
         {
            std::cout << " ind-";
            for(int i=0;i<indentLevel+indentChange;++i)
            {
               std::cout << indentLevel+indentChange << "-";
            }
            std::cout << "> ";
         }
         else if(indentChange < 0) // Already decremented
         {
            std::cout << " ind-";
            for(int i=0;i<indentLevel-indentChange;++i)
            {
               std::cout << indentLevel-indentChange << "-";
            }
            std::cout << "< ";
         }
      }
      */
   }

   // If we have thread local stuff to do
   if(mUseThreadLocal)
   {
      const int column_width(3);
      int column(0);
      if( (*gVprDebugCurColumn).size() > 0)
         column = (*gVprDebugCurColumn).back();

      for(int i=0;i<(column*column_width);i++)
         std::cout << "\t";
   }

   if(indentChange > 0)             // If increasing indent
      indentLevel += indentChange;

   return std::cout;
}

void Debug::addCategory(const vpr::DebugCategory& catId)
{
   if(getLevel() > 2)
   {
      std::cout << "\nAdding category named [" << catId.mName << "]  -- prefix: " << catId.mPrefix <<  " -- guid: " << catId.mGuid
               << " to debug categories:" << &mCategories << " size: " << mCategories.size() << std::endl;
   }
   mCategories.insert( std::pair<vpr::GUID,CategoryInfo>(catId.mGuid,
                                                         CategoryInfo(catId.mName, catId.mPrefix, false, false)));
   if(getLevel() > 4)
   {
      std::cout << "new size: " << mCategories.size() << std::endl;
      debugDump();
   }
   updateAllowedCategories();
   if(getLevel() > 4)
      debugDump();
}


// Are we allowed to print this category??
bool Debug::isCategoryAllowed(const vpr::DebugCategory& catId)
{
   bool allow_category(false);

   // Make sure category is in the vector
   Debug::category_map_t::iterator cat = mCategories.find(catId.mGuid);

   if(cat == mCategories.end())
   {
      // Autoregister
      addCategory(catId);
      cat = mCategories.find(catId.mGuid);
   }

   vprASSERT(cat != mCategories.end() && "Auto-register failed");    // ASSERT: We have a valid category

   Debug::category_map_t::iterator cat_all = mCategories.find(vprDBG_ALL.mGuid);
   vprASSERT(cat_all != mCategories.end());    // ASSERT: We have a valid category

   // If I specified to listen to all OR
   // If it has category of ALL
   bool cat_is_all = (catId.mGuid == vprDBG_ALL.mGuid);
   bool allow_all = ((*cat_all).second.mAllowed == true);

   if(cat_is_all || allow_all)
   {
      allow_category = true;
   }
   else
   {
      allow_category = (*cat).second.mAllowed;
   }

   // Check dis-allowing
   // - If a category is disallowed, then set it false
   if(allow_category)   // Only worry about it if it is already enabled
   {
      if( (*cat).second.mDisallowed)      // If disallowed
      {
         allow_category = false;          // Dis-allow it
      }
   }

   return allow_category;
}


void Debug::updateAllowedCategories()
{
   // Get the environment variables
   char* dbg_allow_cats_env = getenv("VPR_DEBUG_ALLOW_CATEGORIES");
   char* dbg_disallow_cats_env = getenv("VPR_DEBUG_DISALLOW_CATEGORIES");

   if(getLevel() > 4)
   {
      std::cout << "updateAllowedCategories" << std::endl;
      std::cout << "   updateAllowedCat: Trying to find vprDBG_ALL. guid [" << vprDBG_ALL.mGuid << "] " << std::endl;
   }

   // Get cat info for vprDBG_ALL
   // Auto-register vprDBG_ALL if needed
   Debug::category_map_t::iterator cat_all = mCategories.find(vprDBG_ALL.mGuid);

   if(cat_all == mCategories.end())
   {
      addCategory(vprDBG_ALL);
      cat_all = mCategories.find(vprDBG_ALL.mGuid);
   }
   vprASSERT(!mCategories.empty() && "Empty category list");
   vprASSERT((cat_all != mCategories.end()) && "Could not find vprDBG_ALL in category list");    // ASSERT: We have a valid category

   // --- Setup allowed categories --- //
   // - If we have categories env var
   //    - Disable auto-showing of all
   //    - For each current category
   //       - Check if it should be enabled
   if(dbg_allow_cats_env != NULL)
   {
     (*cat_all).second.mAllowed = false;       // Disable the showing of all for now

      if(getLevel() > 4)
         std::cout << "   vprDEBUG::Found VPR_DEBUG_ALLOW_CATEGORIES: Updating allowed categories. (If blank, then none allowed.)\n" << std::flush;

      std::string dbg_cats(dbg_allow_cats_env);

      // For each currently known category name
      category_map_t::iterator i;
      for(i=mCategories.begin();i != mCategories.end();i++)
      {
         std::string cat_name = (*i).second.mName;
         if (dbg_cats.find(cat_name) != std::string::npos )    // Found one
         {
            if(getLevel() > 2)
            {
               std::cout << "   vprDEBUG::updateAllowedCategories: Allowing: "
                      << (*i).second.mName.c_str() << " val:" << (*i).first.toString()
                      << std::endl << std::flush;
            }
            (*i).second.mAllowed = true;
         }
         else
         {
            if(getLevel() > 4)
            {
              std::cout << "vprDEBUG::updateAllowedCategories: Not found (to allow): "
                        << (*i).second.mName.c_str() << " val:" << (*i).first.toString()
                        << std::endl << std::flush;
            }
         }
      }
   }
   else
   {
      if(getLevel() > 2)
      {
         std::cout << "   vprDEBUG::VPR_DEBUG_ALLOW_CATEGORIES not found:\n"
                  << " Setting to: vprDBG_ALL!" << std::endl << std::flush;
      }
      (*cat_all).second.mAllowed = true;       // Enable the showing of all for now
   }

   // --- Setup dis-allowed categories --- //
   if(dbg_disallow_cats_env != NULL)
   {
      if(getLevel() > 2)
      {
         std::cout << "   vprDEBUG::Found VPR_DEBUG_DISALLOW_CATEGORIES: Updating dis-allowed categories. (If blank, then none dis-allowed.)\n" << std::flush;
      }

      std::string dbg_disallow_cats(dbg_disallow_cats_env);

      // For each currently known category name
      category_map_t::iterator i;
      for(i=mCategories.begin();i != mCategories.end();i++)
      {
         std::string cat_name = (*i).second.mName;
         if (dbg_disallow_cats.find(cat_name) != std::string::npos )    // Found one
         {
            if(getLevel() > 2)
            {
               std::cout << "   vprDEBUG::updateAllowedCategories: Dis-allowing: "
                         << (*i).second.mName.c_str() << " val:" << (*i).first.toString()
                         << std::endl << std::flush;
            }
            (*i).second.mDisallowed = true;
         }
         else
         {
            if(getLevel() > 4)
            {
               std::cout << "vprDEBUG::updateAllowedCategories: Not found (to allow): "
                         << (*i).second.mName.c_str() << " val:" << (*i).first.toString()
                         << std::endl << std::flush;
            }
         }
      }
   }
   else
   {
      if(getLevel() > 2)
      {
         std::cout << "   vprDEBUG::VPR_DEBUG_DISALLOW_CATEGORIES not found.\n"
                   << std::flush;
      }
   }

}


void Debug::pushThreadLocalColumn(int column)
{
   (*gVprDebugCurColumn).push_back(column);
}

void Debug::popThreadLocalColumn()
{
   if( (*gVprDebugCurColumn).size() > 0)
      (*gVprDebugCurColumn).pop_back();
}

void Debug::pushThreadLocalColor(std::string color)
{
   (*gVprDebugCurColor).push_back(color);
}


void Debug::popThreadLocalColor()
{
   if( (*gVprDebugCurColor).size() > 0)
      (*gVprDebugCurColor).pop_back();
}


void Debug::debugDump()
{
   std::cout << "--- vpr::Debug Status ----" << std::endl;
   Debug::category_map_t::iterator i;

   for(i=mCategories.begin(); i != mCategories.end(); ++i)
   {
      CategoryInfo cat_info = (*i).second;
      std::cout << "    cat [" << (*i).first << "]  "
                << " name [" << cat_info.mName << "]  "
                << " prefix [" << cat_info.mPrefix << "]  "
                << " allowed [" << cat_info.mAllowed << "] "
                << " disallowed [" << cat_info.mDisallowed << "] " << std::endl;
   }

}

void Debug::decrementIndentLevel()
{
   vprASSERT(indentLevel > 0 && "Attempt to decrrement indent level below 0, check for bad code");
   indentLevel--;
}

void Debug::incrementIndentLevel()
{
   indentLevel++;
}

DebugOutputGuard::DebugOutputGuard(const vpr::DebugCategory& cat,
                                   const int level, std::string entryText,
                                   std::string exitText, bool indent)
   : mCat(cat), mLevel(level), mEntryText(entryText), mExitText(exitText),
     mIndent(indent)
{
   if(mIndent)
   {
      vprDEBUG_BEGIN(mCat, mLevel) << mEntryText << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(mCat, mLevel) << mEntryText << vprDEBUG_FLUSH;
   }
}
   
DebugOutputGuard::~DebugOutputGuard()
{
   if(mIndent)
   {
      // Don't bother printing anything if mExitText is an empty string.
      if(mExitText == std::string(""))
      {
         vprDEBUG_DECREMENT_INDENT(mCat, mLevel);
      }
      else
      {     
         vprDEBUG_END(mCat, mLevel) << mExitText << vprDEBUG_FLUSH;
      }
   }
   else
   {
      // Don't bother printing anything if mExitText is an empty string.
      if(mExitText != std::string(""))
      {
         vprDEBUG(mCat, mLevel) << mExitText << vprDEBUG_FLUSH;
      }
   }
}

} // End of vpr namespace
