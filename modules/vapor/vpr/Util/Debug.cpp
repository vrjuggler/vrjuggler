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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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


namespace vpr {

/*
Debug* Debug::_instance = NULL;
Mutex  Debug::_inst_lock;
*/

// Some thread specific global variables
// They are globals because I can't include their type in the vjDEBUG header file
// If I did, then we could not output debug info in the Thread manager itself
TSObjectProxy<std::vector<int> > gVprDebugCurColumn;       // What column to indent to
TSObjectProxy<std::string> gVprDebugCurColor;        // What color to display "everything" in


vprSingletonImp(Debug);


Debug::Debug()
{
   indentLevel = 0;     // Initialy don't indent
   debugLevel = 1;      // Should actually try to read env variable

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
   
   getAllowedCatsFromEnv();
}

std::ostream& Debug::getStream(const vpr::GUID& cat, const int level, const bool show_thread_info,
                               const bool use_indent, const int indentChange,
                               const bool lockStream)
{
   if(indentChange < 0)                // If decreasing indent
      indentLevel += indentChange;

   //cout << "VPR " << level << ": ";

   // Lock the stream
#ifdef LOCK_DEBUG_STREAM
   if(lockStream)
   {
      debugLock().acquire();     // Get the lock
   }
#endif

   // --- Create stream header --- //
   /*
   if(show_thread_info)
      std::cout << vprDEBUG_STREAM_LOCK << Thread::self() << " VPR:";
   else
      std::cout << vprDEBUG_STREAM_LOCK << "              ";
   */

   // If we have thread local stuff to do
   if(mUseThreadLocal)
   {
      std::cout << clrSetBOLD(*gVprDebugCurColor);
   }

   // Ouput thread info
   // If not, then output space if we are also using indent (assume this means
   // new line used)
   if(show_thread_info)
      std::cout << "[" << vpr::Thread::self() << "] VPR: ";
   else if(use_indent)
      std::cout << "                  ";


      // Insert the correct number of tabs into the stream for indenting
   if(use_indent)
   {
      for(int i=0;i<indentLevel;i++)
         std::cout << "\t";
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

void Debug::addCategoryName(std::string name, const vpr::GUID& catId)
{
   mCategoryNames[name] = catId;
}

void Debug::addAllowedCategory(const vpr::GUID& catId)
{
   mAllowedCategories[catId] = true;
}

// Are we allowed to print this category??
bool Debug::isCategoryAllowed(const vpr::GUID& catId)
{
   // If no entry for cat, grow the vector
   if(mAllowedCategories.find(catId) == mAllowedCategories.end())
      mAllowedCategories[catId] = false;
      
   // If I specified to listen to all OR
   // If it has category of ALL
   bool cat_is_all = (catId == vprDBG_ALL);
   bool allow_all = (mAllowedCategories[vprDBG_ALL] == true);

   if(cat_is_all || allow_all)
      return true;
   else
      return mAllowedCategories[catId];
   
   //return true;
}


void Debug::getAllowedCatsFromEnv()
{
   ///* XXX: For insure
   char* dbg_cats_env = getenv("VPR_DEBUG_CATEGORIES");

   if(dbg_cats_env != NULL)
   {
      std::cout << "vprDEBUG::Found VPR_DEBUG_CATEGORIES: Listing allowed categories. (If blank, then none allowed.\n" << std::flush;
      std::string dbg_cats(dbg_cats_env);

      std::map< std::string, vpr::GUID >::iterator i;
      for(i=mCategoryNames.begin();i != mCategoryNames.end();i++)
      {
         std::string cat_name = (*i).first;
         if (dbg_cats.find(cat_name) != std::string::npos )    // Found one
         {
            std::cout << "vprDEBUG::getAllowedCatsFromEnv: Allowing: "
                      << (*i).first.c_str() << " val:" << (*i).second.toString()
                      << std::endl << std::flush;
            addAllowedCategory((*i).second);                   // Add the category
         }
      }
   }
   else
   {
      std::cout << "vprDEBUG::VPR_DEBUG_CATEGORIES not found:\n"
                << " Setting to: vprDBG_ALL!" << std::endl << std::flush;
      addAllowedCategory(vprDBG_ALL);
   }
   //*/
   //addAllowedCategory(vprDBG_ALL);
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

void Debug::setThreadLocalColor(std::string color)
{
    (*gVprDebugCurColor) = color;
}



}; // End of vpr namespace
