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

// Generated from $Revision$ of $RCSfile$


#ifndef __VRJ_GLAPP_ADAPTER__
#define __VRJ_GLAPP_ADAPTER__

#include "sharppy.h"
#include <vrj/Draw/OGL/GlApp.h>

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
      init_callback();
   }

   typedef void (*apiInit_callback_t)();
   apiInit_callback_t apiInit_callback;

   /// Override for virtual function vrj::App::apiInit.
   virtual void apiInit()
   {
      apiInit_callback();
   }

   typedef void (*exit_callback_t)();
   exit_callback_t exit_callback;

   /// Override for virtual function vrj::App::exit.
   virtual void exit()
   {
      exit_callback();
   }

   typedef void (*preFrame_callback_t)();
   preFrame_callback_t preFrame_callback;

   /// Override for virtual function vrj::App::preFrame.
   virtual void preFrame()
   {
      preFrame_callback();
   }

   typedef void (*latePreFrame_callback_t)();
   latePreFrame_callback_t latePreFrame_callback;

   /// Override for virtual function vrj::App::latePreFrame.
   virtual void latePreFrame()
   {
      latePreFrame_callback();
   }

   typedef void (*intraFrame_callback_t)();
   intraFrame_callback_t intraFrame_callback;

   /// Override for virtual function vrj::App::intraFrame.
   virtual void intraFrame()
   {
      intraFrame_callback();
   }

   typedef void (*postFrame_callback_t)();
   postFrame_callback_t postFrame_callback;

   /// Override for virtual function vrj::App::postFrame.
   virtual void postFrame()
   {
      postFrame_callback();
   }

   typedef void (*reset_callback_t)();
   reset_callback_t reset_callback;

   /// Override for virtual function vrj::App::reset.
   virtual void reset()
   {
      reset_callback();
   }

   typedef void (*focusChanged_callback_t)();
   focusChanged_callback_t focusChanged_callback;

   /// Override for virtual function vrj::App::focusChanged.
   virtual void focusChanged()
   {
      focusChanged_callback();
   }

   typedef float (*getDrawScaleFactor_callback_t)();
   getDrawScaleFactor_callback_t getDrawScaleFactor_callback;

   /// Override for virtual function vrj::App::getDrawScaleFactor.
   virtual float getDrawScaleFactor()
   {
      return getDrawScaleFactor_callback();
   }

   typedef bool (*configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>);
   configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function vrj::App::configCanHandle.
   virtual bool configCanHandle(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement(p0);
   }

   typedef bool (*depSatisfied_callback_t)();
   depSatisfied_callback_t depSatisfied_callback;

   /// Override for virtual function vrj::App::depSatisfied.
   virtual bool depSatisfied()
   {
      return depSatisfied_callback();
   }

   typedef bool (*configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>);
   configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t configAdd_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function vrj::App::configAdd.
   virtual bool configAdd(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return configAdd_callback_boost_shared_ptr_jccl__ConfigElement(p0);
   }

   typedef bool (*configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>);
   configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t configRemove_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function vrj::App::configRemove.
   virtual bool configRemove(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return configRemove_callback_boost_shared_ptr_jccl__ConfigElement(p0);
   }

   typedef vrj::SoundManager* (*getSoundManager_callback_t)();
   getSoundManager_callback_t getSoundManager_callback;

   /// Override for virtual function vrj::App::getSoundManager.
   virtual vrj::SoundManager* getSoundManager()
   {
      return getSoundManager_callback();
   }

   typedef void (*draw_callback_t)();
   draw_callback_t draw_callback;

   /// Override for virtual function vrj::GlApp::draw.
   virtual void draw()
   {
      draw_callback();
   }

   typedef void (*contextInit_callback_t)();
   contextInit_callback_t contextInit_callback;

   /// Override for virtual function vrj::GlApp::contextInit.
   virtual void contextInit()
   {
      contextInit_callback();
   }

   typedef void (*contextClose_callback_t)();
   contextClose_callback_t contextClose_callback;

   /// Override for virtual function vrj::GlApp::contextClose.
   virtual void contextClose()
   {
      contextClose_callback();
   }

   typedef void (*contextPreDraw_callback_t)();
   contextPreDraw_callback_t contextPreDraw_callback;

   /// Override for virtual function vrj::GlApp::contextPreDraw.
   virtual void contextPreDraw()
   {
      contextPreDraw_callback();
   }

   typedef void (*contextPostDraw_callback_t)();
   contextPostDraw_callback_t contextPostDraw_callback;

   /// Override for virtual function vrj::GlApp::contextPostDraw.
   virtual void contextPostDraw()
   {
      contextPostDraw_callback();
   }

   typedef void (*bufferPreDraw_callback_t)();
   bufferPreDraw_callback_t bufferPreDraw_callback;

   /// Override for virtual function vrj::GlApp::bufferPreDraw.
   virtual void bufferPreDraw()
   {
      bufferPreDraw_callback();
   }

   typedef void (*pipePreDraw_callback_t)();
   pipePreDraw_callback_t pipePreDraw_callback;

   /// Override for virtual function vrj::GlApp::pipePreDraw.
   virtual void pipePreDraw()
   {
      pipePreDraw_callback();
   }

   typedef vrj::DrawManager* (*getDrawManager_callback_t)();
   getDrawManager_callback_t getDrawManager_callback;

   /// Override for virtual function vrj::GlApp::getDrawManager.
   virtual vrj::DrawManager* getDrawManager()
   {
      return getDrawManager_callback();
   }

}; // class vrj_GlApp_Adapter




#endif
