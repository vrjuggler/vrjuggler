// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <vrj/Kernel/User.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API vrj::User* vrj_User_User__0()
   {
      vrj::User* obj = new vrj::User();
      return obj;
   }

   SHARPPY_API void delete_vrj_User(vrj::User* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method vrj::User::getId()
   SHARPPY_API int vrj_User_getId__0(vrj::User* self_)
   {
      int result;
      result = self_->getId();
      return result;
   }


   // Wrapper for non-virtual method vrj::User::getName()
   SHARPPY_API char* vrj_User_getName__0(vrj::User* self_)
   {
      std::string temp_result;
      char* result;
      temp_result = self_->getName();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for non-virtual method vrj::User::getHeadPosProxy()
   SHARPPY_API gadget::PositionProxy* vrj_User_getHeadPosProxy__0(vrj::User* self_)
   {
      gadget::PositionProxy* result;
      result = self_->getHeadPosProxy();
      return result;
   }


   // Wrapper for non-virtual method vrj::User::getHeadUpdateTime()
   SHARPPY_API vpr::Interval* vrj_User_getHeadUpdateTime__0(vrj::User* self_)
   {
      vpr::Interval* result;
      result = new vpr::Interval(self_->getHeadUpdateTime());
      return result;
   }


   // Wrapper for non-virtual method vrj::User::getInterocularDistance()
   SHARPPY_API float vrj_User_getInterocularDistance__0(vrj::User* self_)
   {
      float result;
      result = self_->getInterocularDistance();
      return result;
   }


   // Wrapper for virtual method vrj::User::config()
   SHARPPY_API bool vrj_User_config__jccl_ConfigElementPtr1(vrj::User* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->config(*p0);
      return result;
   }


} // extern "C" for vrj::User


