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

#include <vrj/vjConfig.h>

#include <stdlib.h>

#include <vpr/Sync/Mutex.h>
#include <vpr/Thread/Thread.h>

#include <vpr/Util/StreamLock.h>
#include <vrj/Util/Debug.h> /* my header */

namespace vrj
{

   vprSingletonImp(vrj::Debug);

   Debug::Debug()
   {
      indentLevel = 0;     // Initialy don't indent
      debugLevel = 1;      // Should actually try to read env variable

      char* debug_lev = getenv("VJ_DEBUG_NFY_LEVEL");
      if(debug_lev != NULL)
      {
         debugLevel = atoi(debug_lev);
         std::cout << "VJ_DEBUG_NFY_LEVEL: Set to " << debugLevel << std::endl
                   << std::flush;
      } else {
         std::cout << "VJ_DEBUG_NFY_LEVEL: Not found. " << std::endl << std::flush;
         std::cout << "VJ_DEBUG_NFY_LEVEL: Defaults to " << debugLevel
                   << std::endl << std::flush;
      }

      setDefaultCategoryNames();
      getAllowedCatsFromEnv();
   }

   std::ostream& Debug::getStream(int cat, int level, bool show_thread_info,
                                    bool use_indent, int indentChange, bool lockStream)
   {
      if(indentChange < 0)                // If decreasing indent
         indentLevel += indentChange;

      //cout << "VJ " << level << ": ";

      // Lock the stream
#  ifdef LOCK_DEBUG_STREAM
      if(lockStream)
      {
         debugLock().acquire();     // Get the lock
      }
#  endif

      // --- Create stream header --- //
      /*
      if(show_thread_info)
         std::cout << vjDEBUG_STREAM_LOCK << vpr::Thread::self() << " VJ:";
      else
         std::cout << vjDEBUG_STREAM_LOCK << "              ";
      */

      // Ouput thread info
      // If not, then output space if we are also using indent (assume this means new line used)
      if(show_thread_info)
         std::cout << "[" << vpr::Thread::self() << "] VJ: ";
      else if(use_indent)
         std::cout << "                  ";


         // Insert the correct number of tabs into the stream for indenting
      if(use_indent)
      {
         for(int i=0;i<indentLevel;i++)
            std::cout << "\t";
      }

      if(indentChange > 0)             // If increasing indent
         indentLevel += indentChange;

      return std::cout;
   }

   void Debug::addCategoryName(std::string name, int cat)
   {
      mCategoryNames[name] = cat;
   }

   void Debug::addAllowedCategory(int cat)
   {
      if((int)mAllowedCategories.size() < (cat+1))
         growAllowedCategoryVector(cat+1);

      mAllowedCategories[cat] = true;
   }

   // Are we allowed to print this category??
   bool Debug::isCategoryAllowed(int cat)
   {
      // If no entry for cat, grow the vector
      if((int)mAllowedCategories.size() < (cat+1))
         growAllowedCategoryVector(cat+1);

      // If I specified to listen to all OR
      // If it has category of ALL
      if((mAllowedCategories[vjDBG_ALL]) || (cat == vjDBG_ALL))
         return true;
      else
         return mAllowedCategories[cat];
   }

   void Debug::setDefaultCategoryNames()
   {
      ///* XXX: Removed for insure checking
      addCategoryName(vjDBG_ALLstr,vjDBG_ALL);
      addCategoryName(vjDBG_ERRORstr,vjDBG_ERROR);
      addCategoryName(vjDBG_KERNELstr,vjDBG_KERNEL);
      addCategoryName(vjDBG_INPUT_MGRstr,vjDBG_INPUT_MGR);
      addCategoryName(vjDBG_DRAW_MGRstr,vjDBG_DRAW_MGR);
      addCategoryName(vjDBG_DISP_MGRstr,vjDBG_DISP_MGR);
      addCategoryName(vjDBG_ENV_MGRstr, vjDBG_ENV_MGR);
      addCategoryName(vjDBG_PERFORMANCEstr, vjDBG_PERFORMANCE);
      addCategoryName(vjDBG_CONFIGstr, vjDBG_CONFIG);
      //*/
   }

   void Debug::getAllowedCatsFromEnv()
   {
      ///* XXX: For insure
      char* dbg_cats_env = getenv("VJ_DEBUG_CATEGORIES");

      if(dbg_cats_env != NULL)
      {
         std::cout << "vjDEBUG::Found VJ_DEBUG_CATEGORIES: Listing allowed categories. (If blank, then none allowed.\n" << std::flush;
         std::string dbg_cats(dbg_cats_env);

         std::map< std::string, int >::iterator i;
         for(i=mCategoryNames.begin();i != mCategoryNames.end();i++)
         {
            std::string cat_name = (*i).first;
            if (dbg_cats.find(cat_name) != std::string::npos )    // Found one
            {
               std::cout << "vjDEBUG::getAllowedCatsFromEnv: Allowing: "
                         << (*i).first.c_str() << " val:" << (*i).second
                         << std::endl << std::flush;
               addAllowedCategory((*i).second);                   // Add the category
            }
         }
      }
      else
      {
         std::cout << "vjDEBUG::VJ_DEBUG_CATEGORIES not found:\n"
                   << " Setting to: vjDBG_ALL!" << std::endl << std::flush;
         addAllowedCategory(vjDBG_ALL);
      }
      //*/
      //addAllowedCategory(vjDBG_ALL);
   }

   void Debug::growAllowedCategoryVector(int newSize)
   {
      while((int)mAllowedCategories.size() < newSize)
         mAllowedCategories.push_back(false);
   }

} // end namespace vrj
