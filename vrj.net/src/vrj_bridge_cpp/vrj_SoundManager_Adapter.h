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


#ifndef __VRJ_SOUNDMANAGER_ADAPTER__
#define __VRJ_SOUNDMANAGER_ADAPTER__

#include "sharppy.h"
#include <vrj/Sound/SoundManager.h>

class SHARPPY_API vrj_SoundManager_Adapter : public vrj::SoundManager
{
public:
   vrj_SoundManager_Adapter(const vrj::SoundManager& p0)
      : vrj::SoundManager(p0)
   {;}

   vrj_SoundManager_Adapter()
      : vrj::SoundManager()
   {;}

   virtual ~vrj_SoundManager_Adapter()
   {;}

   typedef int (*configProcessPending_callback_t)();
   configProcessPending_callback_t configProcessPending_callback;

   /// Override for virtual function jccl::ConfigElementHandler::configProcessPending.
   virtual int configProcessPending()
   {
      return configProcessPending_callback();
   }

   typedef bool (*configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>);
   configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t configAdd_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function vrj::SoundManager::configAdd.
   virtual bool configAdd(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return configAdd_callback_boost_shared_ptr_jccl__ConfigElement(p0);
   }

   typedef bool (*configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>);
   configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t configRemove_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function vrj::SoundManager::configRemove.
   virtual bool configRemove(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return configRemove_callback_boost_shared_ptr_jccl__ConfigElement(p0);
   }

   typedef bool (*configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>);
   configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function vrj::SoundManager::configCanHandle.
   virtual bool configCanHandle(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement(p0);
   }

   typedef void (*update_callback_t)();
   update_callback_t update_callback;

   /// Override for virtual function vrj::SoundManager::update.
   virtual void update()
   {
      update_callback();
   }

   typedef void (*sync_callback_t)();
   sync_callback_t sync_callback;

   /// Override for virtual function vrj::SoundManager::sync.
   virtual void sync()
   {
      sync_callback();
   }

}; // class vrj_SoundManager_Adapter




#endif
