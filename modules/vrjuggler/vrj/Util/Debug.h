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


#ifndef _VJDebug_h_
#define _VJDebug_h_
//#pragma once

// #define VJ_DEBUG  Defined in vjConfig.h
#include <vjConfig.h>
#include <stdlib.h>

#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>
#include <Utils/vjStreamLock.h>
#include <Utils/vjSingleton.h>

// Debug output categories
#define vjDBG_BASE 0
#define vjDBG_ALL (vjDBG_BASE+0)         /* Use if you always want it ouput */
const std::string vjDBG_ALLstr("DBG_ALL");
#define vjDBG_ERROR (vjDBG_BASE+1)       /* Error output */
const std::string vjDBG_ERRORstr("DBG_ERROR");
#define vjDBG_KERNEL (vjDBG_BASE+2)      /* Kernel output */
const std::string vjDBG_KERNELstr("DBG_KERNEL");
#define vjDBG_INPUT_MGR (vjDBG_BASE+3)       /* Input output */
const std::string vjDBG_INPUT_MGRstr("DBG_INPUT_MGR");
#define vjDBG_DRAW_MGR (vjDBG_BASE+4)
const std::string vjDBG_DRAW_MGRstr("DBG_DRAW_MGR");
#define vjDBG_DISP_MGR (vjDBG_BASE+5)
const std::string vjDBG_DISP_MGRstr("DBG_DISP_MGR");
#define vjDBG_ENV_MGR (vjDBG_BASE+6)
const std::string vjDBG_ENV_MGRstr("DBG_ENV_MGR");
#define vjDBG_PERFORMANCE (vjDBG_BASE+7)
const std::string vjDBG_PERFORMANCEstr("DBG_PERFORMANCE");
#define vjDBG_CONFIG (vjDBG_BASE+8)
const std::string vjDBG_CONFIGstr("DBG_CONFIGDB");

#define vjDBG_USER 100

// Suggested use of val/debugLevel
//
// 0 - Critical messages (always need to be seen)
// 1 - Configuration of system
// 2 - Warnings and potential problems
// 3 - State changes & who is doing what
// 4 - Verbose (don't need source) ex
// 5 - Highly verbose (may need source)
// 6 - Very detailed: i.e. Function entry and exit
// 7 - You will be reading hexidecimal
#define vjDBG_CRITICAL_LVL 0
#define vjDBG_CONFIG_LVL 1
#define vjDBG_WARNING_LVL 2
#define vjDBG_STATE_LVL 3
#define vjDBG_VERB_LVL 4
#define vjDBG_HVERB_LVL 5
#define vjDBG_DETAILED_LVL 6
#define vjDBG_HEX_LVL 7


// COLOR CONTROL CODES
// TODO: Make the work for windows
// 00=none 01=bold 04=underscore 05=blink 07=reverse 08=concealed
// 30=black 31=red 32=green 33=yellow 34=blue 35=magenta 36=cyan 37=white
#define clrNONE "00"
#define clrBOLD "01"
#define clrBLACK "30"
#define clrRED "31"
#define clrGREEN "32"
#define clrYELLOW "33"
#define clrBLUE "34"
#define clrMAGENTA "35"
#define clrCYAN "36"
#define clrWHITE "37"

#ifdef VJ_OS_Win32
#  define clrESC ""
#  define clrCONTROL_CHARS(font, color) ""
#  define clrSetNORM(color) ""
#  define clrSetBOLD(color) ""
#  define clrRESET ""
#  define clrOutBOLD(color,text) text
#  define clrOutNORM(color,text) text
#else
#  define clrESC char(27)
#  define clrCONTROL_CHARS(font, color) clrESC << "[" << font << ";" << color << "m"
#  define clrSetNORM(color) clrESC << "[" << color << "m"
#  define clrSetBOLD(color) clrCONTROL_CHARS(clrBOLD, color)
#  define clrRESET clrESC << "[" << clrNONE << "m"
#  define clrOutBOLD(color,text) clrSetBOLD(color) << text << clrRESET
#  define clrOutNORM(color,text) clrSetNORM(color) << text << clrRESET
#endif


#ifdef VJ_DEBUG
//#   define vjDEBUG(cat,val) if (0) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val)
//#   define vjDEBUG_BEGIN(cat,val) if (0) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val, true, 1)
//#   define vjDEBUG_END(cat,val) if (0) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val, true, -1)
#  define LOCK_DEBUG_STREAM
#  define MAX_DBG_LEVEL 100
#else
#  define LOCK_DEBUG_STREAM
#  define MAX_DBG_LEVEL vjDBG_WARNING_LVL
//#   define vjDEBUG(cat,val) if (1) ; else std::cout
//#   define vjDEBUG_BEGIN(cat,val) if (1) ; else std::cout
//#   define vjDEBUG_END(cat,val) if (1) ; else std::cout
#endif

// #undef LOCK_DEBUG_STREAM

// Define the actual macros to use
// vjDEBUG - Outputs debug info
// vjDEBUG_BEGIN - Starts some indenting of the thread information
// vjDEBUG_END - Ends the indenting level of the information
// vjDEBUG_CONT - Continue on the same line (no thread info, no indent)
// vjDEBUG_CONT_END - Continue on the same line AND decrease indent one level (no thread info, no indent)
// vjDEBUG_NEXT - Outputing more info on next line (no thread info)
// vjDEBUG_NEXT_BEGIN - Output more infor on next line AND indent one level more
// vjDEBUG_NEXT_END - Ouput more info on the next line AND decrease indent one level
#define vjDEBUG(cat,val) if (val>MAX_DBG_LEVEL) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val, true)
#define vjDEBUGlg(cat,val,show_thread,use_indent,lockIt) if (val>MAX_DBG_LEVEL) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val, show_thread, use_indent, 0, lockIt)
#define vjDEBUG_BEGIN(cat,val) if (val>MAX_DBG_LEVEL) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val, true, true, 1)
#define vjDEBUG_BEGINlg(cat,val,show_thread,use_indent,lockIt) if (val>MAX_DBG_LEVEL) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val, show_thread, use_indent, 1, lockIt)
#define vjDEBUG_END(cat,val) if (val>MAX_DBG_LEVEL) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val, true, true, -1)
#define vjDEBUG_ENDlg(cat,val,show_thread,use_indent,lockIt) if (val>MAX_DBG_LEVEL) ; else if((val <= vjDebug::instance()->getLevel()) && (vjDebug::instance()->isCategoryAllowed(cat))) vjDebug::instance()->getStream(cat, val, show_thread, use_indent, -1, lockIt)

#define vjDEBUG_CONT(cat,val) vjDEBUGlg(cat,val,false,false,true)
#define vjDEBUG_CONT_END(cat,val) vjDEBUG_ENDlg(cat,val,false,false,true)
#define vjDEBUG_NEXT(cat,val) vjDEBUGlg(cat,val,false,true,true)
#define vjDEBUG_NEXT_BEGIN(cat,val) vjDEBUG_BEGINlg(cat,val,false,true,true)
#define vjDEBUG_NEXT_END(cat,val) vjDEBUG_ENDlg(cat,val,false,true,true)

// Versions that don't lock the stream
// NOTE: USE WITH EXTREME RISK
#define vjDEBUGnl(cat,val) vjDEBUGlg(cat,val,true,true,false)
#define vjDEBUG_ENDnl(cat,val) vjDEBUG_ENDlg(cat,val,true,true,false)
#define vjDEBUG_BEGINnl(cat,val) vjDEBUG_BEGINlg(cat,val,true,true,false)
#define vjDEBUG_CONTnl(cat,val) vjDEBUGlg(cat,val,false,false,false)
#define vjDEBUG_CONT_ENDnl(cat,val) vjDEBUG_ENDlg(cat,val,false,false,false)
#define vjDEBUG_NEXTnl(cat,val) vjDEBUGlg(cat,val,false,true,false)
#define vjDEBUG_NEXT_BEGINnl(cat,val) vjDEBUG_BEGINlg(cat,val,false,true,false)
#define vjDEBUG_NEXT_ENDnl(cat,val) vjDEBUG_ENDlg(cat,val,false,true,false)


#ifdef LOCK_DEBUG_STREAM
#   define vjDEBUG_STREAM_LOCK vjStreamLock(vjDebug::instance()->debugLock())
#   define vjDEBUG_STREAM_UNLOCK vjStreamUnLock(vjDebug::instance()->debugLock())
#   define vjDEBUG_FLUSH vjDEBUG_STREAM_UNLOCK << std::flush
#else
#   define vjDEBUG_STREAM_LOCK std::flush
#   define vjDEBUG_STREAM_UNLOCK std::flush
#   define vjDEBUG_FLUSH std::flush
#endif


#include <Kernel/vjAssert.h>


//------------------------------------------
//: Class to support debug output
//
//!PUBLIC_API:
//-----------------------------------------
class VJ_CLASS_API vjDebug
{
protected:
   // Set default values
   // Set up default categories
   // Get debug config from environment
   vjDebug();

   // This must be here so that Visual C++ does not try to export them.
   vjDebug(const vjDebug& o) {;}
   void operator=(const vjDebug& o) {;}

public:
   // Get the debug stream to use
   std::ostream& getStream(int cat, int level, bool show_thread_info = true,
                           bool use_indent = true, int indentChange = 0, bool lockStream = true);

   int getLevel()
   { return debugLevel; }

   vpr::Mutex& debugLock()
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

   vpr::Mutex          mDebugLock;

   std::vector<bool> mAllowedCategories;      //: The categories we allow
   std::map<std::string,int> mCategoryNames; //: The names and id of allowed catagories

/*
public:
   static vjDebug* instance()
   {
      if(_instance == NULL)                     // First check
      {
         vpr::Guard<vpr::Mutex> guard(_inst_lock);    // Serial critical section
         if (_instance == NULL)                 // Second check
            _instance = new vjDebug;
      }
      vjASSERT(_instance != NULL && "vjDEBUG has NULL _instance");
      return _instance;
   }
private:
   static vjDebug* _instance;
   static vpr::Mutex _inst_lock;
   */
vjSingletonHeader(vjDebug);
};

#endif
