/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#include <vjConfig.h>

#include <Sync/vjMutex.h>
#include <Kernel/vjDebug.h>

vjMutex DebugLock;
vjDebug* vjDebug::_instance = NULL;

#include <vjConfig.h>
#include <stdlib.h>
#include <assert.h>

#include <Sync/vjMutex.h>
#include <Threads/vjThread.h>
#include <Kernel/vjStreamLock.h>



vjDebug::vjDebug()
{
   indentLevel = 0;     // Initialy don't indent
   debugLevel = 0;      // Should actually try to read env variable

   char* debug_lev = getenv("VJ_DEBUG_NFY_LEVEL");
   if(debug_lev != NULL)
   {
      debugLevel = atoi(debug_lev);
      cout << "VJ_DEBUG_NFY_LEVEL: Set to " << debugLevel << endl << flush;
   } else {
      cout << "VJ_DEBUG_NFY_LEVEL: Not found. " << endl << flush;
      cout << "VJ_DEBUG_NFY_LEVEL: Defaults to " << debugLevel << endl << flush;
   }

   setDefaultCategoryNames();
   getAllowedCatsFromEnv();

}

ostream& vjDebug::getStream(int cat, int level, bool show_thread_info, bool use_indent, int indentChange)
{
   if(indentChange < 0)                // If decreasing indent
      indentLevel += indentChange;

   //cout << "VG " << level << ": ";
   if(show_thread_info)
      cout << vjDEBUG_STREAM_LOCK << setw(6) << vjThread::self() << "  VG: ";
   else
      cout << vjDEBUG_STREAM_LOCK;

      // Insert the correct number of tabs into the stream for indenting
   if(use_indent)
   {
      for(int i=0;i<indentLevel;i++)
         cout << "\t";
   }

   if(indentChange > 0)             // If increasing indent
      indentLevel += indentChange;

   return cout;
}

void vjDebug::addCategoryName(std::string name, int cat)
{
   mCategoryNames[name] = cat;
}

void vjDebug::addAllowedCategory(int cat)
{
   if((int)mAllowedCategories.size() < (cat+1))
      growAllowedCategoryVector(cat+1);

   mAllowedCategories[cat] = true;
}

// Are we allowed to print this category??
bool vjDebug::isCategoryAllowed(int cat)
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

void vjDebug::setDefaultCategoryNames()
{
   addCategoryName(vjDBG_ALLstr,vjDBG_ALL);
   addCategoryName(vjDBG_ERRORstr,vjDBG_ERROR);
   addCategoryName(vjDBG_KERNELstr,vjDBG_KERNEL);
   addCategoryName(vjDBG_INPUT_MGRstr,vjDBG_INPUT_MGR);
   addCategoryName(vjDBG_DRAW_MGRstr,vjDBG_DRAW_MGR);
   addCategoryName(vjDBG_DISP_MGRstr,vjDBG_DISP_MGR);
   addCategoryName(vjDBG_ENV_MGRstr, vjDBG_ENV_MGR);
   addCategoryName(vjDBG_PERFORMANCEstr, vjDBG_PERFORMANCE);
   addCategoryName(vjDBG_CONFIGstr, vjDBG_CONFIG);
}

void vjDebug::getAllowedCatsFromEnv()
{
   char* dbg_cats_env = getenv("VJ_DEBUG_CATEGORIES");

   if(dbg_cats_env != NULL)
   {
      cout << "vjDEBUG::Found VJ_DEBUG_CATEGORIES: Listing allowed categories. (If blank, then none allowed.\n" << flush;
      std::string dbg_cats(dbg_cats_env);

      std::map< std::string, int >::iterator i;
      for(i=mCategoryNames.begin();i != mCategoryNames.end();i++)
      {
         std::string cat_name = (*i).first;
         if (dbg_cats.find(cat_name) != std::string::npos )    // Found one
         {
            cout << "vjDEBUG::getAllowedCatsFromEnv: Allowing: " << (*i).first.c_str() << " val:" << (*i).second << endl << flush;
            addAllowedCategory((*i).second);                   // Add the category
         }
      }
   }
   else
   {
      cout << "vjDEBUG::VJ_DEBUG_CATEGORIES not found:\n"
           << " Setting to: vjDBG_ALL!" << endl << flush;
      addAllowedCategory(vjDBG_ALL);
   }
}

void vjDebug::growAllowedCategoryVector(int newSize)
{
   while((int)mAllowedCategories.size() < newSize)
      mAllowedCategories.push_back(false);
}

