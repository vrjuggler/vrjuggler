// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.18 of RCSfile: class_cxx_adapter.tmpl,v


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
      int result = configProcessPending_callback();
      return result;
   }

   typedef bool (*configAdd_callback_jccl_ConfigElementPtr_t)(jccl::ConfigElementPtr*);
   configAdd_callback_jccl_ConfigElementPtr_t configAdd_callback_jccl_ConfigElementPtr;

   /// Override for virtual function vrj::SoundManager::configAdd.
   virtual bool configAdd(jccl::ConfigElementPtr p0)
   {
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = configAdd_callback_jccl_ConfigElementPtr(p_p0);
      return result;
   }

   typedef bool (*configRemove_callback_jccl_ConfigElementPtr_t)(jccl::ConfigElementPtr*);
   configRemove_callback_jccl_ConfigElementPtr_t configRemove_callback_jccl_ConfigElementPtr;

   /// Override for virtual function vrj::SoundManager::configRemove.
   virtual bool configRemove(jccl::ConfigElementPtr p0)
   {
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = configRemove_callback_jccl_ConfigElementPtr(p_p0);
      return result;
   }

   typedef bool (*configCanHandle_callback_jccl_ConfigElementPtr_t)(jccl::ConfigElementPtr*);
   configCanHandle_callback_jccl_ConfigElementPtr_t configCanHandle_callback_jccl_ConfigElementPtr;

   /// Override for virtual function vrj::SoundManager::configCanHandle.
   virtual bool configCanHandle(jccl::ConfigElementPtr p0)
   {
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = configCanHandle_callback_jccl_ConfigElementPtr(p_p0);
      return result;
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
