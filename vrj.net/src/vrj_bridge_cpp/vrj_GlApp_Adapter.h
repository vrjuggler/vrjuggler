// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.18 of RCSfile: class_cxx_adapter.tmpl,v


#ifndef __VRJ_GLAPP_ADAPTER__
#define __VRJ_GLAPP_ADAPTER__

#include "sharppy.h"
#include <vrj/Draw/OGL/GlApp.h>
#include <vrjnet_util/CliGuard.h>

class SHARPPY_API vrj_GlApp_Adapter : public vrj::GlApp
{
public:
   vrj_GlApp_Adapter(vrj::Kernel* p0)
      : vrj::GlApp(p0)
   {;}

   vrj_GlApp_Adapter()
      : vrj::GlApp()
   {;}

   virtual ~vrj_GlApp_Adapter()
   {;}

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
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = configAdd_callback_jccl_ConfigElementPtr(p_p0);
      return result;
   }

   typedef bool (*configRemove_callback_jccl_ConfigElementPtr_t)(jccl::ConfigElementPtr*);
   configRemove_callback_jccl_ConfigElementPtr_t configRemove_callback_jccl_ConfigElementPtr;

   /// Override for virtual function vrj::App::configRemove.
   virtual bool configRemove(jccl::ConfigElementPtr p0)
   {
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = configRemove_callback_jccl_ConfigElementPtr(p_p0);
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

   typedef void (*draw_callback_t)();
   draw_callback_t draw_callback;

   /// Override for virtual function vrj::GlApp::draw.
   virtual void draw()
   {
      vrjnet::CliGuard guard;
      draw_callback();
   }

   typedef void (*contextInit_callback_t)();
   contextInit_callback_t contextInit_callback;

   /// Override for virtual function vrj::GlApp::contextInit.
   virtual void contextInit()
   {
      vrjnet::CliGuard guard;
      contextInit_callback();
   }

   typedef void (*contextClose_callback_t)();
   contextClose_callback_t contextClose_callback;

   /// Override for virtual function vrj::GlApp::contextClose.
   virtual void contextClose()
   {
      vrjnet::CliGuard guard;
      contextClose_callback();
   }

   typedef void (*contextPreDraw_callback_t)();
   contextPreDraw_callback_t contextPreDraw_callback;

   /// Override for virtual function vrj::GlApp::contextPreDraw.
   virtual void contextPreDraw()
   {
      vrjnet::CliGuard guard;
      contextPreDraw_callback();
   }

   typedef void (*contextPostDraw_callback_t)();
   contextPostDraw_callback_t contextPostDraw_callback;

   /// Override for virtual function vrj::GlApp::contextPostDraw.
   virtual void contextPostDraw()
   {
      vrjnet::CliGuard guard;
      contextPostDraw_callback();
   }

   typedef void (*bufferPreDraw_callback_t)();
   bufferPreDraw_callback_t bufferPreDraw_callback;

   /// Override for virtual function vrj::GlApp::bufferPreDraw.
   virtual void bufferPreDraw()
   {
      vrjnet::CliGuard guard;
      bufferPreDraw_callback();
   }

   typedef void (*pipePreDraw_callback_t)();
   pipePreDraw_callback_t pipePreDraw_callback;

   /// Override for virtual function vrj::GlApp::pipePreDraw.
   virtual void pipePreDraw()
   {
      vrjnet::CliGuard guard;
      pipePreDraw_callback();
   }

   typedef vrj::DrawManager* (*getDrawManager_callback_t)();
   getDrawManager_callback_t getDrawManager_callback;

   /// Override for virtual function vrj::GlApp::getDrawManager.
   virtual vrj::DrawManager* getDrawManager()
   {
      vrjnet::CliGuard guard;
      vrj::DrawManager* result = getDrawManager_callback();
      return result;
   }

}; // class vrj_GlApp_Adapter




#endif
