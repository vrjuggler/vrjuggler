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



#include "sharppy.h"
#include <vrj/Kernel/User.h>
#include <vrj_User_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API vrj_User_Adapter* vrj_User_User__(vrj_User_Adapter::config_callback_boost_shared_ptr_jccl__ConfigElement_t cb0)
   {
      vrj_User_Adapter* obj = new vrj_User_Adapter();
      obj->config_callback_boost_shared_ptr_jccl__ConfigElement = cb0;
      return obj;
   }

   SHARPPY_API void delete_vrj_User(vrj_User_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method vrj::User::getId()
   SHARPPY_API int vrj_User_getId__(vrj_User_Adapter* self_)
   {
      int result;
      result = self_->getId();

      return result;
   }

   // Wrapper for non-virtual method vrj::User::getName()
   SHARPPY_API char* vrj_User_getName__(vrj_User_Adapter* self_)
   {
      char* result;
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      temp_result = self_->getName();
      result = strdup(temp_result.c_str());

      return result;
   }

   // Wrapper for non-virtual method vrj::User::getHeadPosProxy()
   SHARPPY_API gadget::PositionProxy* vrj_User_getHeadPosProxy__(vrj_User_Adapter* self_)
   {
      gadget::PositionProxy* result;
      result = self_->getHeadPosProxy();

      return result;
   }

   // Wrapper for non-virtual method vrj::User::getHeadUpdateTime()
   SHARPPY_API vpr::Interval* vrj_User_getHeadUpdateTime__(vrj_User_Adapter* self_)
   {
      vpr::Interval* result;
      result = new vpr::Interval(self_->getHeadUpdateTime());

      return result;
   }

   // Wrapper for non-virtual method vrj::User::getInterocularDistance()
   SHARPPY_API float vrj_User_getInterocularDistance__(vrj_User_Adapter* self_)
   {
      float result;
      result = self_->getInterocularDistance();

      return result;
   }


   // Wrapper for virtual method vrj::User::config()
   SHARPPY_API bool vrj_User_config__boost_shared_ptr_jccl__ConfigElement(vrj_User_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->vrj::User::config(p0);

      return result;
   }




} // extern "C" for vrj::User


