#ifndef _VJDebug_h_
#define _VJDebug_h_
#pragma once

// #define VJ_DEBUG  Defined in vjConfig.h
#include <vjConfig.h>
#define LOCK_DEBUG_STREAM

// --------------- //
// --- Headers --- //
// --------------- //
#include <stdlib.h>
#include <assert.h>

#ifdef VJ_DEBUG
#   define vjDEBUG(val) if (0) ; else if(val <= vjDebug::instance()->getLevel()) vjDebug::instance()->getStream(val)
#   define vjDEBUG_BEGIN(val) if (0) ; else if(val <= vjDebug::instance()->getLevel()) vjDebug::instance()->getStream(val, 1)
#   define vjDEBUG_END(val) if (0) ; else if(val <= vjDebug::instance()->getLevel()) vjDebug::instance()->getStream(val, -1)
#else
#   define vjDEBUG(val) if (1) ; else cout
#   define vjDEBUG_BEGIN(val) if (1) ; else cout
#   define vjDEBUG_END(val) if (1) ; else cout
#endif

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
// Suggested use of val/debugLevel
//
// 1 - critical messages / Config data
// 2 -
// 3 - Object construction
// 4 -
// 5 - Highly verbose debug output
// 6 - Function entry and exit
// 7 - In house only type debug output
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
   }

public:
   ostream& getStream(int level, int indentChange = 0)
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

private:
   int debugLevel;      // Debug level to use
   int indentLevel;     // Amount to indent

   vjMutex  mDebugLock;

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
