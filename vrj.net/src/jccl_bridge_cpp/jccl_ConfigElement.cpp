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
#include <jccl/Config/ConfigElement.h>

struct jccl_ConfigElement_Holder
{
   boost::shared_ptr< jccl::ConfigElement > mPtr;
};

extern "C"
{
   SHARPPY_API jccl_ConfigElement_Holder* new_jccl_ConfigElement_Holder(boost::shared_ptr< jccl::ConfigElement > ptr)
   {
      jccl_ConfigElement_Holder* h = new jccl_ConfigElement_Holder;
      h->mPtr = ptr;
      return h;
   }

   // Constructor wrapper.
   SHARPPY_API jccl_ConfigElement_Holder* jccl_ConfigElement_ConfigElement__()
   {
      jccl_ConfigElement_Holder* obj = new jccl_ConfigElement_Holder;
      obj->mPtr = boost::shared_ptr< jccl::ConfigElement >(new jccl::ConfigElement());
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API jccl_ConfigElement_Holder* jccl_ConfigElement_ConfigElement__boost_shared_ptr_jccl__ConfigDefinition(boost::shared_ptr<jccl::ConfigDefinition> p0)
   {
      jccl_ConfigElement_Holder* obj = new jccl_ConfigElement_Holder;
      obj->mPtr = boost::shared_ptr< jccl::ConfigElement >(new jccl::ConfigElement(p0));
      return obj;
   }

   SHARPPY_API void delete_jccl_ConfigElement(jccl_ConfigElement_Holder* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method jccl::ConfigElement::isValid()
   SHARPPY_API bool jccl_ConfigElement_isValid__(jccl_ConfigElement_Holder* self_)
   {
      bool result;
      result = self_->mPtr->isValid();

      return result;
   }

   // Wrapper for non-virtual method jccl::ConfigElement::assertValid()
   SHARPPY_API void jccl_ConfigElement_assertValid__(jccl_ConfigElement_Holder* self_)
   {
      self_->mPtr->assertValid();
   }

   // Wrapper for non-virtual method jccl::ConfigElement::getChildElement()
   SHARPPY_API boost::shared_ptr<jccl::ConfigElement>* jccl_ConfigElement_getChildElement__std_basic_string_char_std__char_traits_char__std__allocator_char__(jccl_ConfigElement_Holder* self_, const char* p0)
   {
      boost::shared_ptr<jccl::ConfigElement>* result;
      result = new boost::shared_ptr<jccl::ConfigElement>(self_->mPtr->getChildElement(p0));

      return result;
   }

   // Wrapper for non-virtual method jccl::ConfigElement::getNum()
   SHARPPY_API int jccl_ConfigElement_getNum__std_basic_string_char_std__char_traits_char__std__allocator_char__(jccl_ConfigElement_Holder* self_, const char* p0)
   {
      int result;
      result = self_->mPtr->getNum(p0);

      return result;
   }

   // Wrapper for non-virtual method jccl::ConfigElement::getName()
   SHARPPY_API char* jccl_ConfigElement_getName__(jccl_ConfigElement_Holder* self_)
   {
      char* result;
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      temp_result = self_->mPtr->getName();
      result = strdup(temp_result.c_str());

      return result;
   }

   // Wrapper for non-virtual method jccl::ConfigElement::getFullName()
   SHARPPY_API char* jccl_ConfigElement_getFullName__(jccl_ConfigElement_Holder* self_)
   {
      char* result;
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      temp_result = self_->mPtr->getFullName();
      result = strdup(temp_result.c_str());

      return result;
   }

   // Wrapper for non-virtual method jccl::ConfigElement::getVersion()
   SHARPPY_API unsigned int jccl_ConfigElement_getVersion__(jccl_ConfigElement_Holder* self_)
   {
      unsigned int result;
      result = self_->mPtr->getVersion();

      return result;
   }

   // Wrapper for non-virtual method jccl::ConfigElement::getID()
   SHARPPY_API char* jccl_ConfigElement_getID__(jccl_ConfigElement_Holder* self_)
   {
      char* result;
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      temp_result = self_->mPtr->getID();
      result = strdup(temp_result.c_str());

      return result;
   }

   // Wrapper for non-virtual method jccl::ConfigElement::setProperty()
   SHARPPY_API bool jccl_ConfigElement_setProperty__std_basic_string_char_std__char_traits_char__std__allocator_char___int_bool(jccl_ConfigElement_Holder* self_, const char* p0, const int p1, bool p2)
   {
      bool result;
      result = self_->mPtr->setProperty(p0, p1, p2);

      return result;
   }

   // Wrapper for non-virtual method jccl::ConfigElement::setProperty()
   SHARPPY_API bool jccl_ConfigElement_setProperty__std_basic_string_char_std__char_traits_char__std__allocator_char___int_boost_shared_ptr_jccl__ConfigElement(jccl_ConfigElement_Holder* self_, const char* p0, const int p1, boost::shared_ptr<jccl::ConfigElement> p2)
   {
      bool result;
      result = self_->mPtr->setProperty(p0, p1, p2);

      return result;
   }

   // Wrapper for non-virtual method jccl::ConfigElement::setDefinition()
   SHARPPY_API void jccl_ConfigElement_setDefinition__boost_shared_ptr_jccl__ConfigDefinition(jccl_ConfigElement_Holder* self_, boost::shared_ptr<jccl::ConfigDefinition> p0)
   {
      self_->mPtr->setDefinition(p0);
   }




   // Wrapper for non-virtual method jccl::ConfigElement::operator ==()
   SHARPPY_API bool jccl_ConfigElement_equal__jccl_ConfigElement(jccl_ConfigElement_Holder* self_, const jccl::ConfigElement* p0)
   {
      const jccl::ConfigElement& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->mPtr->operator==(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }

   // Wrapper for non-virtual method jccl::ConfigElement::operator !=()
   SHARPPY_API bool jccl_ConfigElement_not_equal__jccl_ConfigElement(jccl_ConfigElement_Holder* self_, const jccl::ConfigElement* p0)
   {
      const jccl::ConfigElement& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->mPtr->operator!=(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }

   // Wrapper for non-virtual method jccl::ConfigElement::operator <()
   SHARPPY_API bool jccl_ConfigElement_less_than__jccl_ConfigElement(jccl_ConfigElement_Holder* self_, const jccl::ConfigElement* p0)
   {
      const jccl::ConfigElement& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->mPtr->operator<(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }


} // extern "C" for jccl::ConfigElement


