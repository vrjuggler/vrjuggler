/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

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

#include <vpr/vprConfig.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <boost/concept_check.hpp>

#include <vpr/System.h>
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


vprSingletonImpLifetimeWithInitFunc(Debug, init, 50);

Debug::Debug()
   : mDebugEnabled(true)
   , mDebugLevel(2)
   , mIndentLevel(0)     // Initialy don't indent
   , mFile(NULL)
   , mStreamPtr(&std::cout)
   , mUseThreadLocal(false)
{
   std::string debug_lev;
   vpr::System::getenv("VPR_DEBUG_NFY_LEVEL", debug_lev);

   if ( ! debug_lev.empty() )
   {
      mDebugLevel = std::atoi(debug_lev.c_str());
      std::cout << "VPR_DEBUG_NFY_LEVEL set to " << mDebugLevel << std::endl;
   }
   else
   {
      std::cout << "VPR_DEBUG_NFY_LEVEL not found; defaults to "
                << mDebugLevel << std::endl;
   }

   std::cout << "---------------------------------------------------------\n"
             << "For more or less debug output, change VPR_DEBUG_NFY_LEVEL\n"
             << "---------------------------------------------------------"
             << std::endl;

   std::string debug_enable;
   vpr::System::getenv("VPR_DEBUG_ENABLE", debug_enable);

   if ( ! debug_enable.empty() )
   {
      const unsigned int debug_enable_val(std::atoi(debug_enable.c_str()));
      mDebugEnabled = debug_enable_val != 0;
      std::cout << "VPR_DEBUG_ENABLE set to " << mDebugEnabled << std::endl;
   }
   else
   {
      mDebugEnabled = true;
      std::cout << "VPR_DEBUG_ENABLE not found; defaults to " << mDebugEnabled
                << std::endl;
   }

   // Check to see if there is a default Debug target
   std::string debug_file;
   vpr::System::getenv("VPR_DEBUG_FILE", debug_file);

   if ( ! debug_file.empty() )
   {
      if ( "stderr" == debug_file )
      {
         mStreamPtr = &std::cerr;
      }
      else if ( "stdout" != debug_file )
      {
         setOutputFile(debug_file);
      }
      else
      {
         // Use std::cout by default, which is set via the member
         // initializer.
      }
   }
}

Debug::~Debug()
{
   /* Do nothing. */ ;
}

bool Debug::setOutputFile(const std::string& filename)
{
   // Attempt to open the file first.
   mFile = new std::ofstream(filename.c_str());
   if ( !(*mFile) )
   {
      // We couldn't open the file
      delete mFile;
      mFile = NULL;
      return false;
   }

   // Successfully opened the file, so switch the output handler
   mStreamPtr = mFile;
   return true;
}

void Debug::setOutputStream(std::ostream& stream)
{
   mStreamPtr = &stream;
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

std::ostream& Debug::getStream(const vpr::DebugCategory& cat, const int level,
                               const bool showThreadInfo,
                               const bool useIndent, const int indentChange,
                               const bool lockStream)
{
   boost::ignore_unused_variable_warning(level);

   // Lock the stream
#ifdef LOCK_DEBUG_STREAM
   if ( lockStream )
   {
      debugLock().acquire();     // Get the lock
   }
#endif
   // the generic stream to "buffer" output to for the output handler
   // this allows us to avoid pointer dereferences until necessary
   std::ostream& os = *mStreamPtr;
   if ( indentChange < 0 )                // If decreasing indent
   {
      mIndentLevel += indentChange;
   }

   vprASSERT(mIndentLevel >= 0 && "Decreased indent below 0, look for bad code");

   // --- Create stream header --- //
   // If we have thread local stuff to do
   if ( mUseThreadLocal )
   {
      if( (*gVprDebugCurColor).size() == 0 )
      {
         os << clrRESET;
      }
      else
      {
         os << clrSetBOLD((*gVprDebugCurColor).back());
      }
   }

   // Autoregister
   if ( mCategories.find(cat.mGuid) == mCategories.end() )
   {
      addCategory(cat);
   }

   vprASSERT(mCategories.find(cat.mGuid) != mCategories.end() &&
             "Failed to auto-register");

   std::ostringstream sstream;
   sstream << "[" << vpr::Thread::self() << "] "
           << (*mCategories.find(cat.mGuid)).second.mPrefix;

   // Ouput thread info
   // If not, then output space if we are also using indent (assume this means
   // new line used)
   if ( showThreadInfo )
   {
      os << sstream.str();
   }
   else if ( useIndent )
   {
      os << std::string(sstream.str().length(), ' ');
   }

   // Insert the correct number of tabs into the stream for indenting
   if ( useIndent )
   {
      for ( int i = 0; i < mIndentLevel; ++i )
      {
         os << "\t";
      }
   }

   // If we have thread local stuff to do
   if ( mUseThreadLocal )
   {
      const int column_width(3);
      int column(0);
      if ( (*gVprDebugCurColumn).size() > 0 )
      {
         column = (*gVprDebugCurColumn).back();
      }

      for ( int i = 0; i < column * column_width; ++i )
      {
         os << "\t";
      }
   }

   if ( indentChange > 0 )             // If increasing indent
   {
      mIndentLevel += indentChange;
   }

   return *mStreamPtr;
}

void Debug::addCategory(const vpr::DebugCategory& catId)
{
   if ( getLevel() >= vprDBG_VERB_LVL )
   {
      std::cout << "\n[vpr::Debug] Adding category named '" << catId.mName
                << "' (prefix='" << catId.mPrefix <<  "', GUID="
                << catId.mGuid << ")\n"
                << "             to debug categories: " << &mCategories
                << " (size=" << mCategories.size() << ")" << std::endl;
   }

   mCategories.insert(std::make_pair(catId.mGuid,
                                     CategoryInfo(catId.mName, catId.mPrefix,
                                                  false, false)));

   if ( getLevel() >= vprDBG_HVERB_LVL )
   {
      std::cout << "             new size=" << mCategories.size()
                << std::endl;
      debugDump();
   }

   updateAllowedCategories();

   if ( getLevel() >= vprDBG_HVERB_LVL )
   {
      debugDump();
   }
}

// Are we allowed to print this category??
bool Debug::isCategoryAllowed(const vpr::DebugCategory& catId)
{
   bool allow_category(false);

   // Make sure category is in the vector
   category_map_t::iterator cat = mCategories.find(catId.mGuid);

   if ( cat == mCategories.end() )
   {
      // Autoregister
      addCategory(catId);
      cat = mCategories.find(catId.mGuid);
   }

   vprASSERT(cat != mCategories.end() && "Auto-register failed");    // ASSERT: We have a valid category

   category_map_t::iterator cat_all = mCategories.find(vprDBG_ALL.mGuid);
   vprASSERT(cat_all != mCategories.end());    // ASSERT: We have a valid category

   // If I specified to listen to all OR
   // If it has category of ALL
   const bool cat_is_all(catId.mGuid == vprDBG_ALL.mGuid);
   const bool allow_all((*cat_all).second.mAllowed == true);

   if ( cat_is_all || allow_all )
   {
      allow_category = true;
   }
   else
   {
      allow_category = (*cat).second.mAllowed;
   }

   // Check dis-allowing
   // - If a category is disallowed, then set it false
   if ( allow_category )   // Only worry about it if it is already enabled
   {
      if ( (*cat).second.mDisallowed )      // If disallowed
      {
         allow_category = false;          // Dis-allow it
      }
   }

   return allow_category;
}

void Debug::updateAllowedCategories()
{
   // Get the environment variables
   const std::string allow_var("VPR_DEBUG_ALLOW_CATEGORIES");
   std::string dbg_allow_cats;
   vpr::System::getenv(allow_var, dbg_allow_cats);

   const std::string disallow_var("VPR_DEBUG_DISALLOW_CATEGORIES");
   std::string dbg_disallow_cats;
   vpr::System::getenv(disallow_var, dbg_disallow_cats);

   if ( getLevel() >= vprDBG_VERB_LVL )
   {
      std::cout << "[vpr::Debug::updateAllowedCategories()] "
                << "Trying to find vprDBG_ALL (guid=" << vprDBG_ALL.mGuid
                << ")" << std::endl;
   }

   // Get cat info for vprDBG_ALL
   // Auto-register vprDBG_ALL if needed
   category_map_t::iterator cat_all = mCategories.find(vprDBG_ALL.mGuid);

   if ( cat_all == mCategories.end() )
   {
      addCategory(vprDBG_ALL);
      cat_all = mCategories.find(vprDBG_ALL.mGuid);
   }

   vprASSERT(! mCategories.empty() && "Empty category list");
   // ASSERT: We have a valid category
   vprASSERT(cat_all != mCategories.end() &&
             "Could not find vprDBG_ALL in category list");

   // --- Setup allowed categories --- //
   // - If we have categories env var
   //    - Disable auto-showing of all
   //    - For each current category
   //       - Check if it should be enabled
   if ( ! dbg_allow_cats.empty() )
   {
     (*cat_all).second.mAllowed = false;       // Disable the showing of all for now

      if ( getLevel() >= vprDBG_VERB_LVL )
      {
         std::cout << "[vpr::Debug] Found VPR_DEBUG_ALLOW_CATEGORIES; "
                   << "updating allowed categories.\n"
                   << "             (If the list is empty, then none are "
                   << "allowed.)" << std::endl;
      }

      // For each currently known category name
      typedef category_map_t::iterator iter_type;
      for ( iter_type i = mCategories.begin(); i != mCategories.end(); ++i )
      {
         const std::string& cat_name = (*i).second.mName;
         if ( dbg_allow_cats.find(cat_name) != std::string::npos )    // Found one
         {
            if ( getLevel() >= vprDBG_CONFIG_LVL )
            {
               std::cout << "[vpr::Debug::updateAllowedCategories()] "
                         << "Allowing " << (*i).second.mName
                         << " (GUID=" << (*i).first << ")" << std::endl;
            }
            (*i).second.mAllowed = true;
         }
         else
         {
            if ( getLevel() >= vprDBG_HVERB_LVL )
            {
               std::cout << "[vpr::Debug::updateAllowedCategories()] "
                         << "Not found (to allow) " << (*i).second.mName
                         << " (GUID=" << (*i).first << ")" << std::endl;
            }
         }
      }
   }
   else
   {
      if ( getLevel() >= vprDBG_CONFIG_LVL )
      {
         std::cout << "[vpr::Debug] VPR_DEBUG_ALLOW_CATEGORIES not found; "
                   << "setting to vprDBG_ALL!" << std::endl;
      }
      (*cat_all).second.mAllowed = true;       // Enable the showing of all for now
   }

   // --- Setup dis-allowed categories --- //
   if ( ! dbg_disallow_cats.empty() )
   {
      if ( getLevel() >= vprDBG_CONFIG_LVL )
      {
         std::cout << "[vpr::Debug] Found VPR_DEBUG_DISALLOW_CATEGORIES; "
                   << "updating disallowed categories.\n"
                   << "             (If the list is empty, then none are "
                   << "disallowed.)" << std::endl;
      }

      // For each currently known category name
      typedef category_map_t::iterator iter_type;
      for ( iter_type i = mCategories.begin(); i != mCategories.end(); ++i )
      {
         const std::string& cat_name = (*i).second.mName;
         if ( dbg_disallow_cats.find(cat_name) != std::string::npos )    // Found one
         {
            if ( getLevel() >= vprDBG_CONFIG_LVL )
            {
               std::cout << "[vpr::Debug::updateAllowedCategories()] "
                         << "Disallowing " << (*i).second.mName << " (GUID="
                         << (*i).first << ")" << std::endl;
            }
            (*i).second.mDisallowed = true;
         }
         else
         {
            if ( getLevel() >= vprDBG_VERB_LVL )
            {
               std::cout << "[vpr::Debug::updateAllowedCategories()] "
                         << "Not found (to disallow): " << (*i).second.mName
                         << " (GUID=" << (*i).first << ")" << std::endl;
            }
         }
      }
   }
   else
   {
      if ( getLevel() >= vprDBG_VERB_LVL )
      {
         std::cout << "[vpr::Debug] VPR_DEBUG_DISALLOW_CATEGORIES not found."
                   << std::endl;
      }
   }

}

void Debug::pushThreadLocalColumn(const int column)
{
   (*gVprDebugCurColumn).push_back(column);
}

void Debug::popThreadLocalColumn()
{
   if ( (*gVprDebugCurColumn).size() > 0 )
   {
      (*gVprDebugCurColumn).pop_back();
   }
}

void Debug::pushThreadLocalColor(const std::string& color)
{
   (*gVprDebugCurColor).push_back(color);
}

void Debug::popThreadLocalColor()
{
   if ( (*gVprDebugCurColor).size() > 0 )
   {
      (*gVprDebugCurColor).pop_back();
   }
}

void Debug::debugDump() const
{
   std::cout << "--- vpr::Debug Status ----" << std::endl;
   typedef category_map_t::const_iterator iter_type;

   for ( iter_type i = mCategories.begin(); i != mCategories.end(); ++i )
   {
      CategoryInfo cat_info = (*i).second;
      std::cout << "   cateogry=" << (*i).first << " "
                << " name='" << cat_info.mName << "' "
                << " prefix='" << cat_info.mPrefix << "' "
                << " allowed=" << cat_info.mAllowed << " "
                << " disallowed=" << cat_info.mDisallowed << " "
                << std::endl;
   }
}

void Debug::decrementIndentLevel()
{
   vprASSERT(mIndentLevel > 0 &&
             "Attempt to decrrement indent level below 0, check for bad code");
   --mIndentLevel;
}

void Debug::incrementIndentLevel()
{
   ++mIndentLevel;
}

Debug::CategoryInfo::CategoryInfo(const std::string& name,
                                  const std::string& prefix,
                                  const bool allowed, const bool disallowed)
   : mName(name)
   , mPrefix(prefix)
   , mAllowed(allowed)
   , mDisallowed(disallowed)
{
   /* Do nothing. */ ;
}

DebugOutputGuard::DebugOutputGuard(const vpr::DebugCategory& cat,
                                   const int level,
                                   const std::string& entryText,
                                   const std::string& exitText,
                                   const bool indent)
   : mCat(cat)
   , mLevel(level)
   , mEntryText(entryText)
   , mExitText(exitText)
   , mIndent(indent)
{
   if ( mIndent )
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
   if ( mIndent )
   {
      // Don't bother printing anything if mExitText is an empty string.
      if ( mExitText.empty() )
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
      if ( ! mExitText.empty() )
      {
         vprDEBUG(mCat, mLevel) << mExitText << vprDEBUG_FLUSH;
      }
   }
}

} // End of vpr namespace
