// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <vrj/Kernel/Kernel.h>

extern "C"
{
   // Wrapper for non-virtual method vrj::Kernel::start()
   SHARPPY_API int vrj_Kernel_start__0(vrj::Kernel* self_)
   {
      int result;
      result = self_->start();
      return result;
   }


   // Wrapper for non-virtual method vrj::Kernel::stop()
   SHARPPY_API void vrj_Kernel_stop__0(vrj::Kernel* self_)
   {
      self_->stop();
   }


   // Wrapper for non-virtual method vrj::Kernel::isRunning()
   SHARPPY_API bool vrj_Kernel_isRunning__0(vrj::Kernel* self_)
   {
      bool result;
      result = self_->isRunning();
      return result;
   }


   // Wrapper for non-virtual method vrj::Kernel::waitForKernelStop()
   SHARPPY_API void vrj_Kernel_waitForKernelStop__0(vrj::Kernel* self_)
   {
      self_->waitForKernelStop();
   }


   // Wrapper for non-virtual method vrj::Kernel::initConfig()
   SHARPPY_API void vrj_Kernel_initConfig__0(vrj::Kernel* self_)
   {
      self_->initConfig();
   }


   // Wrapper for non-virtual method vrj::Kernel::setApplication()
   SHARPPY_API void vrj_Kernel_setApplication__vrj_App1(vrj::Kernel* self_, vrj::App* p0)
   {
      self_->setApplication(p0);
   }


   // Wrapper for non-virtual method vrj::Kernel::loadConfigFile()
   SHARPPY_API void vrj_Kernel_loadConfigFile__char1(vrj::Kernel* self_, const char* p0)
   {
      self_->loadConfigFile(p0);
   }


   // Wrapper for non-virtual method vrj::Kernel::loadConfigFile()
   SHARPPY_API void vrj_Kernel_loadConfigFile__std_string1(vrj::Kernel* self_, char* p0)
   {
      self_->loadConfigFile(p0);
   }


   // Wrapper for non-virtual method vrj::Kernel::scanForConfigDefinitions()
   SHARPPY_API void vrj_Kernel_scanForConfigDefinitions__std_string1(vrj::Kernel* self_, const char* p0)
   {
      self_->scanForConfigDefinitions(p0);
   }


   // Wrapper for non-virtual method vrj::Kernel::getInputManager()
   SHARPPY_API gadget::InputManager* vrj_Kernel_getInputManager__0(vrj::Kernel* self_)
   {
      gadget::InputManager* result;
      result = self_->getInputManager();
      return result;
   }


   // Wrapper for non-virtual method vrj::Kernel::getUser()
   SHARPPY_API vrj::User* vrj_Kernel_getUser__std_string1(vrj::Kernel* self_, const char* p0)
   {
      vrj::User* result;
      result = self_->getUser(p0);
      return result;
   }


   // Wrapper for static method vrj::Kernel::instance()
   SHARPPY_API vrj::Kernel* vrj_Kernel_instance__0()
   {
      vrj::Kernel* result;
      result = vrj::Kernel::instance();
      return result;
   }


} // extern "C" for vrj::Kernel


