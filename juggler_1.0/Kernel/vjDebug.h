/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
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


#ifndef _VJDebug_h_
#define _VJDebug_h_
//#pragma once

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
#define vjDBG_CONFIG (vjDBG_BASE+8)
#define vjDBG_CONFIGstr std::string("DBG_CONFIGDB")

#define vjDBG_USER 100

// Suggested use of val/debugLevel
//
// 0 - Critical messages (always need to be seen)
// 1 - Configuration of system
// 2 - Warnings and potential problems
// 3 - State changes & who is doing what
// 4 - Verbose (don't need source) ex
// 5 - Highly verbose (may need source)
// 6 - Function entry and exit
// 7 - You will be reading hexidecimal


#ifdef VJ_DEBUG
//#   define vjDEBUG(cat,val) if (0) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val)
//#   define vjDEBUG_BEGIN(cat,val) if (0) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val, 1)
//#   define vjDEBUG_END(cat,val) if (0) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val, -1)
#  define LOCK_DEBUG_STREAM
#  define MAX_DBG_LEVEL 100
#else
#  define LOCK_DEBUG_STREAM
#  define MAX_DBG_LEVEL 2
//#   define vjDEBUG(cat,val) if (1) ; else cout
//#   define vjDEBUG_BEGIN(cat,val) if (1) ; else cout
//#   define vjDEBUG_END(cat,val) if (1) ; else cout
#endif

#undef LOCK_DEBUG_STREAM
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
#include <Kernel/vjStreamLock.h>


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
   // Set default values
   // Set up default categories
   // Get debug config from environment
   vjDebug();

public:
   // Get the debug stream to use
   ostream& getStream(int cat, int level, int indentChange = 0);

   int getLevel()
   { return debugLevel; }

   vjMutex& debugLock()
   { return mDebugLock; }

   // Add a category name
   void addCategoryName(std::string name, int cat);

   // Allow the given category
   void addAllowedCategory(int cat);

   // Are we allowed to print this category??
   bool isCategoryAllowed(int cat);

   // Setup the default categories
   void setDefaultCategoryNames();

   // Configure the allowed categories from the users environment
   void getAllowedCatsFromEnv();

   void growAllowedCategoryVector(int newSize);

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
