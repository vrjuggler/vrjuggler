#ifndef _VJDebug_h_
#define _VJDebug_h_
#pragma once

// #define VJ_DEBUG  Defined in vjConfig.h
#include <vjConfig.h>
#include <stdlib.h>
#include <assert.h>

// Debug output categories
#define vjDBG_BASE 0
#define vjDBG_ALL (vjDBG_BASE+0)         // Use if you always want it ouput
#define vjDBG_ALLstr std::string("DBG_ALL")
#define vjDBG_ERROR (vjDBG_BASE+1)       // Error output
#define vjDBG_ERRORstr std::string("DBG_ERROR")
#define vjDBG_KERNEL (vjDBG_BASE+2)      // Kernel output
#define vjDBG_KERNELstr std::string("DBG_KERNEL")
#define vjDBG_INPUT_MGR (vjDBG_BASE+3)       // Input output
#define vjDBG_INPUT_MGRstr std::string("DBG_INPUT_MGR")
#define vjDBG_DRAW_MGR (vjDBG_BASE+4)
#define vjDBG_DRAW_MGRstr std::string("DBG_DRAW_MGR")
#define vjDBG_DISP_MGR (vjDBG_BASE+5)
#define vjDBG_DISP_MGRstr std::string("DBG_DISP_MGR")
#define vjDBG_ENV_MGR (vjDBG_BASE+6)
#define vjDBG_ENV_MGRstr std::string("DBG_ENV_MGR")
#define vjDBG_PERFORMANCE (vjDBG_BASE+7)
#define vjDBG_PERFORMANCEstr std::string("DBG_PERFORMANCE")

#define vjDBG_USER 100

// Suggested use of val/debugLevel
//
// 1 - critical messages / Config data
// 2 -
// 3 - Object construction
// 4 -
// 5 - Highly verbose debug output
// 6 - Function entry and exit
// 7 - In house only type debug output


#ifdef VJ_DEBUG
//#   define vjDEBUG(cat,val) if (0) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val)
//#   define vjDEBUG_BEGIN(cat,val) if (0) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val, 1)
//#   define vjDEBUG_END(cat,val) if (0) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val, -1)
#  define LOCK_DEBUG_STREAM
#  define MAX_DBG_LEVEL 100
#else
#  define LOCK_DEBUG_STREAM
#  define MAX_DBG_LEVEL 1
//#   define vjDEBUG(cat,val) if (1) ; else cout
//#   define vjDEBUG_BEGIN(cat,val) if (1) ; else cout
//#   define vjDEBUG_END(cat,val) if (1) ; else cout
#endif


#define vjDEBUG(cat,val) if (val>MAX_DBG_LEVEL) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val)
#define vjDEBUG_BEGIN(cat,val) if (val>MAX_DBG_LEVEL) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val, 1)
#define vjDEBUG_END(cat,val) if (val>MAX_DBG_LEVEL) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val, -1)


#ifdef LOCK_DEBUG_STREAM
#   define vjDEBUG_STREAM_LOCK vjStreamLock(vjDebug::instance()->debugLock())
#   define vjDEBUG_STREAM_UNLOCK vjStreamUnLock(vjDebug::instance()->debugLock())
#   define vjDEBUG_FLUSH vjDEBUG_STREAM_UNLOCK << flush
#else
#   define vjDEBUG_STREAM_LOCK flush
#   define vjDEBUG_STREAM_UNLOCK flush
#   define vjDEBUG_FLUSH flush
#endif

// -- ASSERT -- //
#ifdef VJ_DEBUG
#   define vjASSERT(val) assert((val))
#else
#   define vjASSERT(val) ((void)0)
#endif

#include <Sync/vjMutex.h>
#include <Threads/vjThread.h>
#include <Kernel/vjStreamLock.h>

//#include <Sync/vjNullMutex.h>



// NOTE: Quick Hack for now.  Need to really design something nice. :)
extern vjMutex DebugLock;




//------------------------------------------
//: Class to support debug output
//
//!PUBLIC_API:
//-----------------------------------------
class vjDebug
{
private:
   vjDebug()
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

public:
   ostream& getStream(int cat, int level, int indentChange = 0)
   {
      if(indentChange < 0)                // If decreasing indent
         indentLevel += indentChange;

      //cout << "VG " << level << ": ";
      cout << vjDEBUG_STREAM_LOCK << setw(6) << vjThread::self() << "  VG: ";

         // Insert the correct number of tabs into the stream for indenting
      for(int i=0;i<indentLevel;i++)
         cout << "\t";

      if(indentChange > 0)             // If increasing indent
         indentLevel += indentChange;

      return cout;
   }

   int getLevel()
   {
      return debugLevel;
   }

   vjMutex& debugLock()
   { return mDebugLock; }

   void addCategoryName(std::string name, int cat)
   {
      mCategoryNames[name] = cat;
   }

   void addAllowedCategory(int cat)
   {
      if(mAllowedCategories.size() < (cat+1))
         growAllowedCategoryVector(cat+1);

      mAllowedCategories[cat] = true;
   }

   // Are we allowed to print this category??
   bool isCategoryAllowed(int cat)
   {
      // If now entry for cat, grow the vector
      if(mAllowedCategories.size() < (cat+1))
         growAllowedCategoryVector(cat+1);

      // If I specified to listen to all OR
      // If it has category of ALL
      if((mAllowedCategories[vjDBG_ALL]) || (cat == vjDBG_ALL))
         return true;
      else
         return mAllowedCategories[cat];
   }

   void setDefaultCategoryNames()
   {
      addCategoryName(vjDBG_ALLstr,vjDBG_ALL);
      addCategoryName(vjDBG_ERRORstr,vjDBG_ERROR);
      addCategoryName(vjDBG_KERNELstr,vjDBG_KERNEL);
      addCategoryName(vjDBG_INPUT_MGRstr,vjDBG_INPUT_MGR);
      addCategoryName(vjDBG_DRAW_MGRstr,vjDBG_DRAW_MGR);
      addCategoryName(vjDBG_DISP_MGRstr,vjDBG_DISP_MGR);
      addCategoryName(vjDBG_ENV_MGRstr, vjDBG_ENV_MGR);
      addCategoryName(vjDBG_PERFORMANCEstr, vjDBG_PERFORMANCE);
   }

   void getAllowedCatsFromEnv()
   {
      char* dbg_cats_env = getenv("VJ_DEBUG_CATEGORIES");

      if(dbg_cats_env != NULL)
      {
         std::string dbg_cats(dbg_cats_env);

         std::map< std::string, int >::iterator i;
         for(i=mCategoryNames.begin();i != mCategoryNames.end();i++)
         {
            std::string cat_name = (*i).first;
            if (dbg_cats.find(cat_name) != std::string::npos )    // Found one
            {
               cout << "vjDEBUG::getAllowedCatsFromEnv: Allowing: " << (*i).first << " val:" << (*i).second << endl << flush;
               addAllowedCategory((*i).second);                   // Add the category
            }
         }
      }
   }

   void growAllowedCategoryVector(int newSize)
   {
      while(mAllowedCategories.size() < newSize)
         mAllowedCategories.push_back(false);
   }

private:
   int debugLevel;      // Debug level to use
   int indentLevel;     // Amount to indent

   vjMutex  mDebugLock;

   std::vector<bool> mAllowedCategories;      //: The categories we allow
   std::map<std::string,int> mCategoryNames; //: The names and id of allowed catagories

public:
   static vjDebug* instance()
   {
      if(_instance == NULL)
         _instance = new vjDebug;
      return _instance;
   }
private:
   static vjDebug* _instance;
};

#endif
