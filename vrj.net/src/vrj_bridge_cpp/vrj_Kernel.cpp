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

// Generated from Revision: 1.57 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <vrj/Kernel/Kernel.h>

extern "C"
{
   // Wrapper for non-virtual method vrj::Kernel::start()
   SHARPPY_API int vrj_Kernel_start__(vrj::Kernel* self_)
   {
      int result;
      result = self_->start();

      return result;
   }

   // Wrapper for non-virtual method vrj::Kernel::stop()
   SHARPPY_API void vrj_Kernel_stop__(vrj::Kernel* self_)
   {
      self_->stop();
   }

   // Wrapper for non-virtual method vrj::Kernel::isRunning()
   SHARPPY_API bool vrj_Kernel_isRunning__(vrj::Kernel* self_)
   {
      bool result;
      result = self_->isRunning();

      return result;
   }

   // Wrapper for non-virtual method vrj::Kernel::waitForKernelStop()
   SHARPPY_API void vrj_Kernel_waitForKernelStop__(vrj::Kernel* self_)
   {
      self_->waitForKernelStop();
   }

   // Wrapper for non-virtual method vrj::Kernel::initConfig()
   SHARPPY_API void vrj_Kernel_initConfig__(vrj::Kernel* self_)
   {
      self_->initConfig();
   }

   // Wrapper for non-virtual method vrj::Kernel::setApplication()
   SHARPPY_API void vrj_Kernel_setApplication__vrj_App(vrj::Kernel* self_, vrj::App* p0)
   {
      self_->setApplication(p0);
   }

   // Wrapper for non-virtual method vrj::Kernel::loadConfigFile()
   SHARPPY_API void vrj_Kernel_loadConfigFile__char(vrj::Kernel* self_, const char* p0)
   {
      self_->loadConfigFile(p0);
   }

   // Wrapper for non-virtual method vrj::Kernel::loadConfigFile()
   SHARPPY_API void vrj_Kernel_loadConfigFile__std_basic_string_char_std__char_traits_char__std__allocator_char__(vrj::Kernel* self_, char* p0)
   {
      self_->loadConfigFile(p0);
   }

   // Wrapper for non-virtual method vrj::Kernel::scanForConfigDefinitions()
   SHARPPY_API void vrj_Kernel_scanForConfigDefinitions__std_basic_string_char_std__char_traits_char__std__allocator_char__(vrj::Kernel* self_, const char* p0)
   {
      self_->scanForConfigDefinitions(p0);
   }

   // Wrapper for non-virtual method vrj::Kernel::getInputManager()
   SHARPPY_API gadget::InputManager* vrj_Kernel_getInputManager__(vrj::Kernel* self_)
   {
      gadget::InputManager* result;
      result = self_->getInputManager();

      return result;
   }

   // Wrapper for non-virtual method vrj::Kernel::getUser()
   SHARPPY_API vrj::User* vrj_Kernel_getUser__std_basic_string_char_std__char_traits_char__std__allocator_char__(vrj::Kernel* self_, const char* p0)
   {
      vrj::User* result;
      result = self_->getUser(p0);

      return result;
   }




   // Wrapper for static method vrj::Kernel::instance()
   SHARPPY_API vrj::Kernel* vrj_Kernel_instance__()
   {
      vrj::Kernel* result;
      result = vrj::Kernel::instance();

      return result;
   }



} // extern "C" for vrj::Kernel


