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
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
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
   SHARPPY_API bool vrj_User_config__boost_shared_ptr_jccl__ConfigElement1(vrj::User* self_, boost::shared_ptr<jccl::ConfigElement>* p0)
   {
      bool result;
      result = self_->config(*p0);
      return result;
   }


} // extern "C" for vrj::User


