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

#ifndef _VPR_Debug_h_
#define _VPR_Debug_h_

#include <vpr/vprConfig.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>

#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>
#include <vpr/Util/StreamLock.h>
#include <vpr/Util/Singleton.h>

// Debug output categories
#define vprDBG_BASE 0
#define vprDBG_ALL (vprDBG_BASE+0)         /* Use if you always want it ouput */
const std::string vprDBG_ALLstr("DBG_ALL");
#define vprDBG_ERROR (vprDBG_BASE+1)       /* Error output */
const std::string vprDBG_ERRORstr("DBG_ERROR");
#define vprDBG_KERNEL (vprDBG_BASE+2)      /* Kernel output */
const std::string vprDBG_KERNELstr("DBG_KERNEL");
#define vprDBG_INPUT_MGR (vprDBG_BASE+3)       /* Input output */
const std::string vprDBG_INPUT_MGRstr("DBG_INPUT_MGR");
#define vprDBG_DRAW_MGR (vprDBG_BASE+4)
const std::string vprDBG_DRAW_MGRstr("DBG_DRAW_MGR");
#define vprDBG_DISP_MGR (vprDBG_BASE+5)
const std::string vprDBG_DISP_MGRstr("DBG_DISP_MGR");
#define vprDBG_ENV_MGR (vprDBG_BASE+6)
const std::string vprDBG_ENV_MGRstr("DBG_ENV_MGR");
#define vprDBG_PERFORMANCE (vprDBG_BASE+7)
const std::string vprDBG_PERFORMANCEstr("DBG_PERFORMANCE");
#define vprDBG_CONFIG (vprDBG_BASE+8)
const std::string vprDBG_CONFIGstr("DBG_CONFIGDB");

#define vprDBG_USER 100

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
#define vprDBG_CRITICAL_LVL 0
#define vprDBG_CONFIG_LVL 1
#define vprDBG_WARNING_LVL 2
#define vprDBG_STATE_LVL 3
#define vprDBG_VERB_LVL 4
#define vprDBG_HVERB_LVL 5
#define vprDBG_DETAILED_LVL 6
#define vprDBG_HEX_LVL 7


// ANSI COLOR CONTROL CODES
// TODO: Make the work for windows
// 00=none 01=bold 04=underscore 05=blink 07=reverse 08=concealed
// 30=black 31=red 32=green 33=yellow 34=blue 35=magenta 36=cyan 37=white
// Add 10 for background colors.
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

#ifdef VPR_OS_Win32
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


#ifdef VPR_DEBUG
//#   define vprDEBUG(cat,val) if (0) ; else if((val <= vprDebug::instance()->getLevel()) && (vprDebug::instance()->isCategoryAllowed(cat))) vprDebug::instance()->getStream(cat, val)
//#   define vprDEBUG_BEGIN(cat,val) if (0) ; else if((val <= vprDebug::instance()->getLevel()) && (vprDebug::instance()->isCategoryAllowed(cat))) vprDebug::instance()->getStream(cat, val, true, 1)
//#   define vprDEBUG_END(cat,val) if (0) ; else if((val <= vprDebug::instance()->getLevel()) && (vprDebug::instance()->isCategoryAllowed(cat))) vprDebug::instance()->getStream(cat, val, true, -1)
#  define LOCK_DEBUG_STREAM
#  define MAX_DBG_LEVEL 100
#else
#  define LOCK_DEBUG_STREAM
#  define MAX_DBG_LEVEL vprDBG_WARNING_LVL
//#   define vprDEBUG(cat,val) if (1) ; else std::cout
//#   define vprDEBUG_BEGIN(cat,val) if (1) ; else std::cout
//#   define vprDEBUG_END(cat,val) if (1) ; else std::cout
#endif

// #undef LOCK_DEBUG_STREAM

// Define the actual macros to use
// vprDEBUG - Outputs debug info
// vprDEBUG_BEGIN - Starts some indenting of the thread information
// vprDEBUG_END - Ends the indenting level of the information
// vprDEBUG_CONT - Continue on the same line (no thread info, no indent)
// vprDEBUG_CONT_END - Continue on the same line AND decrease indent one level (no thread info, no indent)
// vprDEBUG_NEXT - Outputing more info on next line (no thread info)
// vprDEBUG_NEXT_BEGIN - Output more infor on next line AND indent one level more
// vprDEBUG_NEXT_END - Ouput more info on the next line AND decrease indent one level
#define vprDEBUG(cat,val) if (val>MAX_DBG_LEVEL) ; else if((val <= vpr::Debug::instance()->getLevel()) && (vpr::Debug::instance()->isCategoryAllowed(cat))) vpr::Debug::instance()->getStream(cat, val, true)
#define vprDEBUGlg(cat,val,show_thread,use_indent,lockIt) if (val>MAX_DBG_LEVEL) ; else if((val <= vpr::Debug::instance()->getLevel()) && (vpr::Debug::instance()->isCategoryAllowed(cat))) vpr::Debug::instance()->getStream(cat, val, show_thread, use_indent, 0, lockIt)
#define vprDEBUG_BEGIN(cat,val) if (val>MAX_DBG_LEVEL) ; else if((val <= vpr::Debug::instance()->getLevel()) && (vpr::Debug::instance()->isCategoryAllowed(cat))) vpr::Debug::instance()->getStream(cat, val, true, true, 1)
#define vprDEBUG_BEGINlg(cat,val,show_thread,use_indent,lockIt) if (val>MAX_DBG_LEVEL) ; else if((val <= vpr::Debug::instance()->getLevel()) && (vpr::Debug::instance()->isCategoryAllowed(cat))) vpr::Debug::instance()->getStream(cat, val, show_thread, use_indent, 1, lockIt)
#define vprDEBUG_END(cat,val) if (val>MAX_DBG_LEVEL) ; else if((val <= vpr::Debug::instance()->getLevel()) && (vpr::Debug::instance()->isCategoryAllowed(cat))) vpr::Debug::instance()->getStream(cat, val, true, true, -1)
#define vprDEBUG_ENDlg(cat,val,show_thread,use_indent,lockIt) if (val>MAX_DBG_LEVEL) ; else if((val <= vpr::Debug::instance()->getLevel()) && (vpr::Debug::instance()->isCategoryAllowed(cat))) vpr::Debug::instance()->getStream(cat, val, show_thread, use_indent, -1, lockIt)

#define vprDEBUG_CONT(cat,val) vprDEBUGlg(cat,val,false,false,true)
#define vprDEBUG_CONT_END(cat,val) vprDEBUG_ENDlg(cat,val,false,false,true)
#define vprDEBUG_NEXT(cat,val) vprDEBUGlg(cat,val,false,true,true)
#define vprDEBUG_NEXT_BEGIN(cat,val) vprDEBUG_BEGINlg(cat,val,false,true,true)
#define vprDEBUG_NEXT_END(cat,val) vprDEBUG_ENDlg(cat,val,false,true,true)

// Versions that don't lock the stream
// NOTE: USE WITH EXTREME RISK
#define vprDEBUGnl(cat,val) vprDEBUGlg(cat,val,true,true,false)
#define vprDEBUG_ENDnl(cat,val) vprDEBUG_ENDlg(cat,val,true,true,false)
#define vprDEBUG_BEGINnl(cat,val) vprDEBUG_BEGINlg(cat,val,true,true,false)
#define vprDEBUG_CONTnl(cat,val) vprDEBUGlg(cat,val,false,false,false)
#define vprDEBUG_CONT_ENDnl(cat,val) vprDEBUG_ENDlg(cat,val,false,false,false)
#define vprDEBUG_NEXTnl(cat,val) vprDEBUGlg(cat,val,false,true,false)
#define vprDEBUG_NEXT_BEGINnl(cat,val) vprDEBUG_BEGINlg(cat,val,false,true,false)
#define vprDEBUG_NEXT_ENDnl(cat,val) vprDEBUG_ENDlg(cat,val,false,true,false)

#define vprDEBUG_PushColumn(val) vpr::Debug::instance()->pushThreadLocalColumn(val)
#define vprDEBUG_PopColumn() vpr::Debug::instance()->popThreadLocalColumn()
#define vprDEBUG_ColumnGuard(val) vpr::DebugColumnGuard debug_col_guard(val)
#define vprDEBUG_TSColor(color) vpr::Debug::instance()->setThreadLocalColor(color)
#define vprDEBUG_ThreadLocalEnable() vpr::Debug::instance()->enableThreadLocalSettings()
#define vprDEBUG_ThreadLocalDisable() vpr::Debug::instance()->disableThreadLocalSettings()


#ifdef LOCK_DEBUG_STREAM
#   define vprDEBUG_STREAM_LOCK vpr::StreamLock(vpr::Debug::instance()->debugLock())
#   define vprDEBUG_STREAM_UNLOCK vpr::StreamUnLock(vpr::Debug::instance()->debugLock())
#   define vprDEBUG_FLUSH vprDEBUG_STREAM_UNLOCK << std::flush
#else
#   define vprDEBUG_STREAM_LOCK std::flush
#   define vprDEBUG_STREAM_UNLOCK std::flush
#   define vprDEBUG_FLUSH std::flush
#endif


namespace vpr {

/**
 * Class to support debug output
 *
 * @author Allen Bierbaum
 */
class VPR_CLASS_API Debug
{
protected:
   // Set default values
   // Set up default categories
   // Get debug config from environment
   Debug();

   // These two have to be here because Visual C++ will try to make them
   // exported public symbols.  This causes problems because copying vpr::Mutex
   // objects is not allowed.
   Debug(const Debug& d) {;}
   void operator= (const Debug& d) {;}

public:
   // Get the debug stream to use
   std::ostream& getStream(int cat, int level, bool show_thread_info = true,
                           bool use_indent = true, int indentChange = 0,
                           bool lockStream = true);

   int getLevel()
   { return debugLevel; }

   Mutex& debugLock()
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

   void enableThreadLocalSettings()
   { mUseThreadLocal = true; }
   void disableThreadLocalSettings()
   { mUseThreadLocal = false; }

   void pushThreadLocalColumn(int column);
   void popThreadLocalColumn();
   void setThreadLocalColor(std::string color);

private:
   int debugLevel;      // Debug level to use
   int indentLevel;     // Amount to indent

   bool  mUseThreadLocal;  // Wether to use thread local info or not

   Mutex          mDebugLock;

   std::vector<bool> mAllowedCategories;      //: The categories we allow
   std::map<std::string,int> mCategoryNames; //: The names and id of allowed catagories

vprSingletonHeader(Debug);
};

// Helper class
struct DebugColumnGuard
{
   DebugColumnGuard(int col_val)
   { vprDEBUG_PushColumn(col_val); }

   ~DebugColumnGuard()
   { vprDEBUG_PopColumn(); }
};

}; // End of vpr namespace


#endif
