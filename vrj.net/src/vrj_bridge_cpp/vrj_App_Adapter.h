// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.18 of RCSfile: class_cxx_adapter.tmpl,v


#ifndef __VRJ_APP_ADAPTER__
#define __VRJ_APP_ADAPTER__

#include "sharppy.h"
#include <vrj/Kernel/App.h>
#include <vrjnet_util/CliGuard.h>

class SHARPPY_API vrj_App_Adapter : public vrj::App
{
public:
   vrj_App_Adapter(vrj::Kernel* p0)
      : vrj::App(p0)
   {;}

   vrj_App_Adapter()
      : vrj::App()
   {;}

   virtual ~vrj_App_Adapter()
   {;}

   typedef int (*configProcessPending_callback_t)();
   configProcessPending_callback_t configProcessPending_callback;

   /// Override for virtual function jccl::ConfigElementHandler::configProcessPending.
   virtual int configProcessPending()
   {
      vrjnet::CliGuard guard;
      int result = configProcessPending_callback();
      return result;
   }

   typedef void (*init_callback_t)();
   init_callback_t init_callback;

   /// Override for virtual function vrj::App::init.
   virtual void init()
   {
      vrjnet::CliGuard guard;
      init_callback();
   }

   typedef void (*apiInit_callback_t)();
   apiInit_callback_t apiInit_callback;

   /// Override for virtual function vrj::App::apiInit.
   virtual void apiInit()
   {
      vrjnet::CliGuard guard;
      apiInit_callback();
   }

   typedef void (*exit_callback_t)();
   exit_callback_t exit_callback;

   /// Override for virtual function vrj::App::exit.
   virtual void exit()
   {
      vrjnet::CliGuard guard;
      exit_callback();
   }

   typedef void (*preFrame_callback_t)();
   preFrame_callback_t preFrame_callback;

   /// Override for virtual function vrj::App::preFrame.
   virtual void preFrame()
   {
      vrjnet::CliGuard guard;
      preFrame_callback();
   }

   typedef void (*latePreFrame_callback_t)();
   latePreFrame_callback_t latePreFrame_callback;

   /// Override for virtual function vrj::App::latePreFrame.
   virtual void latePreFrame()
   {
      vrjnet::CliGuard guard;
      latePreFrame_callback();
   }

   typedef void (*intraFrame_callback_t)();
   intraFrame_callback_t intraFrame_callback;

   /// Override for virtual function vrj::App::intraFrame.
   virtual void intraFrame()
   {
      vrjnet::CliGuard guard;
      intraFrame_callback();
   }

   typedef void (*postFrame_callback_t)();
   postFrame_callback_t postFrame_callback;

   /// Override for virtual function vrj::App::postFrame.
   virtual void postFrame()
   {
      vrjnet::CliGuard guard;
      postFrame_callback();
   }

   typedef void (*reset_callback_t)();
   reset_callback_t reset_callback;

   /// Override for virtual function vrj::App::reset.
   virtual void reset()
   {
      vrjnet::CliGuard guard;
      reset_callback();
   }

   typedef void (*focusChanged_callback_t)();
   focusChanged_callback_t focusChanged_callback;

   /// Override for virtual function vrj::App::focusChanged.
   virtual void focusChanged()
   {
      vrjnet::CliGuard guard;
      focusChanged_callback();
   }

   typedef float (*getDrawScaleFactor_callback_t)();
   getDrawScaleFactor_callback_t getDrawScaleFactor_callback;

   /// Override for virtual function vrj::App::getDrawScaleFactor.
   virtual float getDrawScaleFactor()
   {
      vrjnet::CliGuard guard;
      float result = getDrawScaleFactor_callback();
      return result;
   }

   typedef bool (*configCanHandle_callback_jccl_ConfigElementPtr_t)(jccl::ConfigElementPtr*);
   configCanHandle_callback_jccl_ConfigElementPtr_t configCanHandle_callback_jccl_ConfigElementPtr;

   /// Override for virtual function vrj::App::configCanHandle.
   virtual bool configCanHandle(jccl::ConfigElementPtr p0)
   {
      vrjnet::CliGuard guard;
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = configCanHandle_callback_jccl_ConfigElementPtr(p_p0);
      return result;
   }

   typedef bool (*depSatisfied_callback_t)();
   depSatisfied_callback_t depSatisfied_callback;

   /// Override for virtual function vrj::App::depSatisfied.
   virtual bool depSatisfied()
   {
      vrjnet::CliGuard guard;
      bool result = depSatisfied_callback();
      return result;
   }

   typedef bool (*configAdd_callback_jccl_ConfigElementPtr_t)(jccl::ConfigElementPtr*);
   configAdd_callback_jccl_ConfigElementPtr_t configAdd_callback_jccl_ConfigElementPtr;

   /// Override for virtual function vrj::App::configAdd.
   virtual bool configAdd(jccl::ConfigElementPtr p0)
   {
      vrjnet::CliGuard guard;
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = configAdd_callback_jccl_ConfigElementPtr(p_p0);
      return result;
   }

   typedef bool (*configRemove_callback_jccl_ConfigElementPtr_t)(jccl::ConfigElementPtr*);
   configRemove_callback_jccl_ConfigElementPtr_t configRemove_callback_jccl_ConfigElementPtr;

   /// Override for virtual function vrj::App::configRemove.
   virtual bool configRemove(jccl::ConfigElementPtr p0)
   {
      vrjnet::CliGuard guard;
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = configRemove_callback_jccl_ConfigElementPtr(p_p0);
      return result;
   }

   typedef vrj::DrawManager* (*getDrawManager_callback_t)();
   getDrawManager_callback_t getDrawManager_callback;

   /// Override for virtual function vrj::App::getDrawManager.
   virtual vrj::DrawManager* getDrawManager()
   {
      vrjnet::CliGuard guard;
      vrj::DrawManager* result = getDrawManager_callback();
      return result;
   }

   typedef vrj::SoundManager* (*getSoundManager_callback_t)();
   getSoundManager_callback_t getSoundManager_callback;

   /// Override for virtual function vrj::App::getSoundManager.
   virtual vrj::SoundManager* getSoundManager()
   {
      vrjnet::CliGuard guard;
      vrj::SoundManager* result = getSoundManager_callback();
      return result;
   }

   /// Public wrapper for protected virtual function vrj::App::configAdd.
   bool configAdd_wrapper(jccl::ConfigElementPtr p0)
   {
      vrjnet::CliGuard guard;
      return vrj::App::configAdd(p0);
   }

   /// Public wrapper for protected virtual function vrj::App::configRemove.
   bool configRemove_wrapper(jccl::ConfigElementPtr p0)
   {
      vrjnet::CliGuard guard;
      return vrj::App::configRemove(p0);
   }

}; // class vrj_App_Adapter




#endif
