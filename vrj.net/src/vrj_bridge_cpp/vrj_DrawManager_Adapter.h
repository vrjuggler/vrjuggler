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


#ifndef __VRJ_DRAWMANAGER_ADAPTER__
#define __VRJ_DRAWMANAGER_ADAPTER__

#include "sharppy.h"
#include <vrj/Draw/DrawManager.h>

class SHARPPY_API vrj_DrawManager_Adapter : public vrj::DrawManager
{
public:
   vrj_DrawManager_Adapter()
      : vrj::DrawManager()
   {;}

   virtual ~vrj_DrawManager_Adapter()
   {;}

   typedef bool (*configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>);
   configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function jccl::ConfigElementHandler::configCanHandle.
   virtual bool configCanHandle(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement(p0);
   }

   typedef int (*configProcessPending_callback_t)();
   configProcessPending_callback_t configProcessPending_callback;

   /// Override for virtual function jccl::ConfigElementHandler::configProcessPending.
   virtual int configProcessPending()
   {
      return configProcessPending_callback();
   }

   typedef bool (*configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>);
   configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t configAdd_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function jccl::ConfigElementHandler::configAdd.
   virtual bool configAdd(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return configAdd_callback_boost_shared_ptr_jccl__ConfigElement(p0);
   }

   typedef bool (*configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>);
   configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t configRemove_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function jccl::ConfigElementHandler::configRemove.
   virtual bool configRemove(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return configRemove_callback_boost_shared_ptr_jccl__ConfigElement(p0);
   }

   typedef void (*draw_callback_t)();
   draw_callback_t draw_callback;

   /// Override for virtual function vrj::DrawManager::draw.
   virtual void draw()
   {
      draw_callback();
   }

   typedef void (*sync_callback_t)();
   sync_callback_t sync_callback;

   /// Override for virtual function vrj::DrawManager::sync.
   virtual void sync()
   {
      sync_callback();
   }

   typedef void (*setApp_callback_vrj_App_t)(vrj::App*);
   setApp_callback_vrj_App_t setApp_callback_vrj_App;

   /// Override for virtual function vrj::DrawManager::setApp.
   virtual void setApp(vrj::App* p0)
   {
      setApp_callback_vrj_App(p0);
   }

   typedef void (*initAPI_callback_t)();
   initAPI_callback_t initAPI_callback;

   /// Override for virtual function vrj::DrawManager::initAPI.
   virtual void initAPI()
   {
      initAPI_callback();
   }

   typedef void (*closeAPI_callback_t)();
   closeAPI_callback_t closeAPI_callback;

   /// Override for virtual function vrj::DrawManager::closeAPI.
   virtual void closeAPI()
   {
      closeAPI_callback();
   }

   typedef void (*outStream_callback_std_basic_ostream_char_std__char_traits_char___t)(std::basic_ostream<char,std::char_traits<char> >&);
   outStream_callback_std_basic_ostream_char_std__char_traits_char___t outStream_callback_std_basic_ostream_char_std__char_traits_char__;

   /// Override for virtual function vrj::DrawManager::outStream.
   virtual void outStream(std::basic_ostream<char,std::char_traits<char> >& p0)
   {
      outStream_callback_std_basic_ostream_char_std__char_traits_char__(p0);
   }

}; // class vrj_DrawManager_Adapter




#endif
