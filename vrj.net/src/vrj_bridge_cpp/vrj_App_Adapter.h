/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VRJ.NET is (C) Copyright 2004 by Patrick Hartling
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

// Generated from Revision: 1.17 of RCSfile: class_cxx_adapter.tmpl,v


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

   struct holder_p0_vrj_App_configCanHandle__boost_shared_ptr_jccl__ConfigElement { boost::shared_ptr<jccl::ConfigElement> mPtr; };
   typedef bool (*configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t)(holder_p0_vrj_App_configCanHandle__boost_shared_ptr_jccl__ConfigElement*);
   configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function vrj::App::configCanHandle.
   virtual bool configCanHandle(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      vrjnet::CliGuard guard;
      holder_p0_vrj_App_configCanHandle__boost_shared_ptr_jccl__ConfigElement* h_p0 = new holder_p0_vrj_App_configCanHandle__boost_shared_ptr_jccl__ConfigElement;
      h_p0->mPtr = p0;
      bool result = configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement(h_p0);
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

   struct holder_p0_vrj_App_configAdd__boost_shared_ptr_jccl__ConfigElement { boost::shared_ptr<jccl::ConfigElement> mPtr; };
   typedef bool (*configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t)(holder_p0_vrj_App_configAdd__boost_shared_ptr_jccl__ConfigElement*);
   configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t configAdd_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function vrj::App::configAdd.
   virtual bool configAdd(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      vrjnet::CliGuard guard;
      holder_p0_vrj_App_configAdd__boost_shared_ptr_jccl__ConfigElement* h_p0 = new holder_p0_vrj_App_configAdd__boost_shared_ptr_jccl__ConfigElement;
      h_p0->mPtr = p0;
      bool result = configAdd_callback_boost_shared_ptr_jccl__ConfigElement(h_p0);
      return result;
   }

   struct holder_p0_vrj_App_configRemove__boost_shared_ptr_jccl__ConfigElement { boost::shared_ptr<jccl::ConfigElement> mPtr; };
   typedef bool (*configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t)(holder_p0_vrj_App_configRemove__boost_shared_ptr_jccl__ConfigElement*);
   configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t configRemove_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function vrj::App::configRemove.
   virtual bool configRemove(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      vrjnet::CliGuard guard;
      holder_p0_vrj_App_configRemove__boost_shared_ptr_jccl__ConfigElement* h_p0 = new holder_p0_vrj_App_configRemove__boost_shared_ptr_jccl__ConfigElement;
      h_p0->mPtr = p0;
      bool result = configRemove_callback_boost_shared_ptr_jccl__ConfigElement(h_p0);
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
   bool configAdd_wrapper(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      vrjnet::CliGuard guard;
      return vrj::App::configAdd(p0);
   }

   /// Public wrapper for protected virtual function vrj::App::configRemove.
   bool configRemove_wrapper(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      vrjnet::CliGuard guard;
      return vrj::App::configRemove(p0);
   }

}; // class vrj_App_Adapter




#endif
