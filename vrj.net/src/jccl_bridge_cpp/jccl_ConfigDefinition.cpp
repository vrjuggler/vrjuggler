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

// Generated from Revision: 1.60 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <jccl/Config/ConfigDefinition.h>

struct jccl_ConfigDefinition_Holder
{
   boost::shared_ptr< jccl::ConfigDefinition > mPtr;
};

extern "C"
{
   SHARPPY_API jccl_ConfigDefinition_Holder* new_jccl_ConfigDefinition_Holder(boost::shared_ptr< jccl::ConfigDefinition > ptr)
   {
      jccl_ConfigDefinition_Holder* h = new jccl_ConfigDefinition_Holder;
      h->mPtr = ptr;
      return h;
   }

   // Constructor wrapper.
   SHARPPY_API jccl_ConfigDefinition_Holder* jccl_ConfigDefinition_ConfigDefinition__()
   {
      jccl_ConfigDefinition_Holder* obj = new jccl_ConfigDefinition_Holder;
      obj->mPtr = boost::shared_ptr< jccl::ConfigDefinition >(new jccl::ConfigDefinition());
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API jccl_ConfigDefinition_Holder* jccl_ConfigDefinition_ConfigDefinition__std_basic_string_char_std__char_traits_char__std__allocator_char___cppdom_boost_shared_ptr_cppdom__Node(const char* p0, cppdom_boost::shared_ptr<cppdom::Node> p1)
   {
      jccl_ConfigDefinition_Holder* obj = new jccl_ConfigDefinition_Holder;
      obj->mPtr = boost::shared_ptr< jccl::ConfigDefinition >(new jccl::ConfigDefinition(p0, p1));
      return obj;
   }

   SHARPPY_API void delete_jccl_ConfigDefinition(jccl_ConfigDefinition_Holder* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method jccl::ConfigDefinition::assertValid()
   SHARPPY_API void jccl_ConfigDefinition_assertValid__0(jccl_ConfigDefinition_Holder* self_)
   {
      self_->mPtr->assertValid();
   }


   // Wrapper for non-virtual method jccl::ConfigDefinition::getName()
   SHARPPY_API char* jccl_ConfigDefinition_getName__0(jccl_ConfigDefinition_Holder* self_)
   {
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      char* result;
      temp_result = self_->mPtr->getName();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigDefinition::getToken()
   SHARPPY_API char* jccl_ConfigDefinition_getToken__0(jccl_ConfigDefinition_Holder* self_)
   {
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      char* result;
      temp_result = self_->mPtr->getToken();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigDefinition::getVersion()
   SHARPPY_API unsigned int jccl_ConfigDefinition_getVersion__0(jccl_ConfigDefinition_Holder* self_)
   {
      unsigned int result;
      result = self_->mPtr->getVersion();
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigDefinition::getHelp()
   SHARPPY_API char* jccl_ConfigDefinition_getHelp__0(jccl_ConfigDefinition_Holder* self_)
   {
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      char* result;
      temp_result = self_->mPtr->getHelp();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigDefinition::operator ==()
   SHARPPY_API bool jccl_ConfigDefinition_equal__jccl_ConfigDefinition1(jccl_ConfigDefinition_Holder* self_, const jccl::ConfigDefinition* p0)
   {
      const jccl::ConfigDefinition& marshal_p0 = *p0;
      bool result;
      result = self_->mPtr->operator==(marshal_p0);
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigDefinition::operator !=()
   SHARPPY_API bool jccl_ConfigDefinition_not_equal__jccl_ConfigDefinition1(jccl_ConfigDefinition_Holder* self_, const jccl::ConfigDefinition* p0)
   {
      const jccl::ConfigDefinition& marshal_p0 = *p0;
      bool result;
      result = self_->mPtr->operator!=(marshal_p0);
      return result;
   }


} // extern "C" for jccl::ConfigDefinition


