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

#ifndef _VPR_Debug_h_
#define _VPR_Debug_h_

#include <vpr/vprConfig.h>

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>

#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>
#include <vpr/Util/StreamLock.h>
#include <vpr/Util/Singleton.h>
#include <vpr/Util/GUID.h>


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
#define vprDBG_WARNING_LVL 1
#define vprDBG_CONFIG_STATUS_LVL 2
#define vprDBG_CONFIG_LVL 3
#define vprDBG_STATE_LVL 4
#define vprDBG_VERB_LVL 5
#define vprDBG_HVERB_LVL 6
#define vprDBG_DETAILED_LVL 7
#define vprDBG_HEX_LVL 8

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

//#  define vprDEBUG(cat,val) if (0) ; else if((val <= vprDebug::instance()->getLevel()) && (vprDebug::instance()->isCategoryAllowed(cat))) vprDebug::instance()->getStream(cat, val)
//#  define vprDEBUG_BEGIN(cat,val) if (0) ; else if((val <= vprDebug::instance()->getLevel()) && (vprDebug::instance()->isCategoryAllowed(cat))) vprDebug::instance()->getStream(cat, val, true, 1)
//#  define vprDEBUG_END(cat,val) if (0) ; else if((val <= vprDebug::instance()->getLevel()) && (vprDebug::instance()->isCategoryAllowed(cat))) vprDebug::instance()->getStream(cat, val, true, -1)
#  define LOCK_DEBUG_STREAM
#  define VPR_MAX_DBG_LEVEL 100
#else
#  define LOCK_DEBUG_STREAM
#  define VPR_MAX_DBG_LEVEL vprDBG_CONFIG_LVL

//#  define vprDEBUG(cat,val) if (1) ; else std::cout
//#  define vprDEBUG_BEGIN(cat,val) if (1) ; else std::cout
//#  define vprDEBUG_END(cat,val) if (1) ; else std::cout
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
#define vprDEBUG(cat,val) if (val>VPR_MAX_DBG_LEVEL) ; else if((vpr::Debug::instance()->isDebugEnabled()) && (val <= vpr::Debug::instance()->getLevel()) && (vpr::Debug::instance()->isCategoryAllowed(cat))) vpr::Debug::instance()->getStream(cat, val, true)
#define vprDEBUGlg(cat,val,show_thread,use_indent,lockIt) if (val>VPR_MAX_DBG_LEVEL) ; else if((vpr::Debug::instance()->isDebugEnabled()) && (val <= vpr::Debug::instance()->getLevel()) && (vpr::Debug::instance()->isCategoryAllowed(cat))) vpr::Debug::instance()->getStream(cat, val, show_thread, use_indent, 0, lockIt)
#define vprDEBUG_BEGIN(cat,val) if (val>VPR_MAX_DBG_LEVEL) ; else if((vpr::Debug::instance()->isDebugEnabled()) && (val <= vpr::Debug::instance()->getLevel()) && (vpr::Debug::instance()->isCategoryAllowed(cat))) vpr::Debug::instance()->getStream(cat, val, true, true, 1)
#define vprDEBUG_BEGINlg(cat,val,show_thread,use_indent,lockIt) if (val>VPR_MAX_DBG_LEVEL) ; else if((vpr::Debug::instance()->isDebugEnabled()) && (val <= vpr::Debug::instance()->getLevel()) && (vpr::Debug::instance()->isCategoryAllowed(cat))) vpr::Debug::instance()->getStream(cat, val, show_thread, use_indent, 1, lockIt)
#define vprDEBUG_END(cat,val) if (val>VPR_MAX_DBG_LEVEL) ; else if((vpr::Debug::instance()->isDebugEnabled()) && (val <= vpr::Debug::instance()->getLevel()) && (vpr::Debug::instance()->isCategoryAllowed(cat))) vpr::Debug::instance()->getStream(cat, val, true, true, -1)
#define vprDEBUG_ENDlg(cat,val,show_thread,use_indent,lockIt) if (val>VPR_MAX_DBG_LEVEL) ; else if((vpr::Debug::instance()->isDebugEnabled()) && (val <= vpr::Debug::instance()->getLevel()) && (vpr::Debug::instance()->isCategoryAllowed(cat))) vpr::Debug::instance()->getStream(cat, val, show_thread, use_indent, -1, lockIt)
#define vprDEBUG_DECREMENT_INDENT(cat, val) if (val>VPR_MAX_DBG_LEVEL) ; else if((vpr::Debug::instance()->isDebugEnabled()) && (val <= vpr::Debug::instance()->getLevel()) && (vpr::Debug::instance()->isCategoryAllowed(cat))) vpr::Debug::instance()->decrementIndentLevel()


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

#define vprDEBUG_PushTSColor(color) vpr::Debug::instance()->pushThreadLocalColor(color)
#define vprDEBUG_PopTSColor() vpr::Debug::instance()->popThreadLocalColor()
#define vprDEBUG_TSColorGuard(color) vpr::DebugColorGuard debug_color_guard(color)

#define vprDEBUG_ThreadLocalEnable() vpr::Debug::instance()->enableThreadLocalSettings()
#define vprDEBUG_ThreadLocalDisable() vpr::Debug::instance()->disableThreadLocalSettings()

#define vprDEBUG_OutputGuard(cat, level, entryText, exitText) vpr::DebugOutputGuard debug_output_guard(cat, level, entryText, exitText)
#define vprDEBUG_OutputGuard_i(cat, level, indent, entryText, exitText) vpr::DebugOutputGuard debug_output_guard(cat, level, entryText, exitText, indent)


#ifdef LOCK_DEBUG_STREAM
#  define vprDEBUG_STREAM_LOCK vpr::StreamLock(vpr::Debug::instance()->debugLock())
#  define vprDEBUG_STREAM_UNLOCK vpr::StreamUnLock(vpr::Debug::instance()->debugLock())
#  define vprDEBUG_FLUSH vprDEBUG_STREAM_UNLOCK << std::flush
#else
#  define vprDEBUG_STREAM_LOCK std::flush
#  define vprDEBUG_STREAM_UNLOCK std::flush
#  define vprDEBUG_FLUSH std::flush
#endif


namespace vpr
{

   struct DebugCategory
   {
      DebugCategory(vpr::GUID guid, std::string name, std::string prefix)
      {
         mGuid = guid;
         mName = name;
         mPrefix = prefix;
      }

      vpr::GUID   mGuid;
      std::string mName;
      std::string mPrefix;
   };


/**
 * Class to support debug output
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

      /** Initialize the system */
      void init();

   public:
      // Get the debug stream to use
      std::ostream& getStream(const vpr::DebugCategory& cat, const int level, const bool show_thread_info = true,
                              const bool use_indent = true, const int indentChange = 0,
                              const bool lockStream = true);

      int getLevel()
      {
         return debugLevel;
      }

      Mutex& debugLock()
      {
         return mDebugLock;
      }

      /// Adds a category name.
      void addCategory(const vpr::DebugCategory& catId);

      /// Are we allowed to print this category??
      bool isCategoryAllowed(const vpr::DebugCategory& catId);

      /// Sets up the default categories.
      void setDefaultCategoryNames();

      /// Configures the allowed categories from the users environment.
      void updateAllowedCategories();

      void enableThreadLocalSettings()
      {
         mUseThreadLocal = true;
      }

      void disableThreadLocalSettings()
      {
         mUseThreadLocal = false;
         std::cout << clrRESET;     // Reset colors
      }

      // Thread local settings: Columns and color
      void pushThreadLocalColumn(int column);
      void popThreadLocalColumn();
      void pushThreadLocalColor(std::string color);
      void popThreadLocalColor();

      //@{
      /** Is debuging enabled */
      bool isDebugEnabled()
      { return mDebugEnabled;}
      void enableOutput()
      { mDebugEnabled = true;}
      void disableOutput()
      { mDebugEnabled = false;}
      //@}

      /** Dump the current status to screen. */
      void debugDump();

      /// Decrement the level of indention.
      void decrementIndentLevel();

      /// Increment the level of indention.
      void incrementIndentLevel();

   private:
      bool mDebugEnabled;  // Is debug output enabled
      int debugLevel;      //! Debug level to use
      int indentLevel;     //! Amount to indent

      bool  mUseThreadLocal;  //! Whether to use thread local info or not

      Mutex          mDebugLock;

      //std::vector<bool> mAllowedCategories;      //! The categories we allow
   
      
      struct CategoryInfo
      {
         /*CategoryInfo()
          : mName("un-named"), mPrefix("unset"), mAllowed(false)
         {;}*/

         CategoryInfo(std::string name, std::string prefix, bool allowed, bool disallowed)
          : mName(name), mPrefix(prefix), mAllowed(allowed), mDisallowed(disallowed)
         {;}

         std::string mName;         /**< What is the name of the category */
         std::string mPrefix;       /**< What is the prefix to output with the category */
         bool        mAllowed;      /**< Is the category output allowed */
         bool        mDisallowed;   /**< Is the category output dis-allowed */
      };

      // GUID, pair( name, prefix )
      typedef std::map<vpr::GUID, CategoryInfo > category_map_t;
      std::map<vpr::GUID, CategoryInfo > mCategories;    /**< The names and id of allowed catagories */

      vprSingletonHeaderWithInitFunc(Debug, init);
   };

// Helper class
   struct DebugColumnGuard
   {
      DebugColumnGuard(int col_val)
      {
         vprDEBUG_PushColumn(col_val);
      }

      ~DebugColumnGuard()
      {
         vprDEBUG_PopColumn();
      }
   };

   struct DebugColorGuard
   {
      DebugColorGuard(std::string color_val)
      {
         vprDEBUG_PushTSColor(color_val);
      }

      ~DebugColorGuard()
      {
         vprDEBUG_PopTSColor();
      }
   };

   /**
    * Helper class that outputs debug information at creation and destruction
    * of the object.
    */
   struct VPR_CLASS_API DebugOutputGuard
   {
      DebugOutputGuard(const vpr::DebugCategory& cat, const int level,
                       std::string entryText,
                       std::string exitText = std::string(""),
                       bool indent = true);

      ~DebugOutputGuard();

      const vpr::DebugCategory& mCat;
      int                       mLevel;
      std::string               mEntryText;
      std::string               mExitText;
      bool                      mIndent;
   };

} // End of vpr namespace


namespace vpr
{
   
/*
   struct DebugCatRegistrator
   {
      DebugCatRegistrator(vpr::GUID catGuid, std::string catName, std::string catPrefix)
      {
         vpr::Debug::instance()->addCategory(catGuid, catName, catPrefix);
      }
   };
   */
} // namespace

/** Helper macro for registering category
* Defines a (file) unique variable that create a registrator in the file prive namespace
* @param NAME  String name of the category (as used in the environment variable).  Note: This is not in string ("str") form
* @param CAT   GUID id of the category
* Use this in the .cpp files to register the actually token with vpr::Debug.
* @see Debug.cpp
*/
//#define vprREGISTER_DBG_CATEGORY(CAT, NAME, PREFIX ) vpr::DebugCatRegistrator NAME ## _registrator (CAT, #NAME, PREFIX);

// Debug output categories
// GUID, Name, Prefix
const vpr::DebugCategory vprDBG_ALL("660b4b06-1f5b-4e4b-abb8-d44229ce1319", "DBG_ALL", "DBG:");
const vpr::DebugCategory vprDBG_ERROR("b081eb68-0a61-4a65-a0a1-dd3ccc90a82b", "DBG_ERROR", "ERR:");   /* Error output */
const vpr::DebugCategory vprDBG_SIM("64872313-a5b7-4d1d-b7a3-5f269b4adde4", "DBG_SIM", "SIM:");   /* Sim output */
const vpr::DebugCategory vprDBG_VPR("28648014-ec63-4707-90e3-76a3ea450036", "DBG_VPR", "VPR:");
                                     
#endif
