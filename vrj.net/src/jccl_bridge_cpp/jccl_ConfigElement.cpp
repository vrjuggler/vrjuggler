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
#include <jccl/Config/ConfigElement.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API boost::shared_ptr< jccl::ConfigElement >* jccl_ConfigElement_ConfigElement__0()
   {
      boost::shared_ptr< jccl::ConfigElement >* obj = new boost::shared_ptr< jccl::ConfigElement >(new jccl::ConfigElement());
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API boost::shared_ptr< jccl::ConfigElement >* jccl_ConfigElement_ConfigElement__jccl_ConfigDefinitionPtr1(jccl::ConfigDefinitionPtr* p0)
   {
      boost::shared_ptr< jccl::ConfigElement >* obj = new boost::shared_ptr< jccl::ConfigElement >(new jccl::ConfigElement(*p0));
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API boost::shared_ptr< jccl::ConfigElement >* jccl_ConfigElement_ConfigElement__jccl_ConfigElement1(const jccl::ConfigElement* p0)
   {
      const jccl::ConfigElement& marshal_p0 = *p0;
      boost::shared_ptr< jccl::ConfigElement >* obj = new boost::shared_ptr< jccl::ConfigElement >(new jccl::ConfigElement(marshal_p0));
      return obj;
   }

   SHARPPY_API void delete_jccl_ConfigElement(boost::shared_ptr< jccl::ConfigElement >* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method jccl::ConfigElement::isValid()
   SHARPPY_API bool jccl_ConfigElement_isValid__0(boost::shared_ptr< jccl::ConfigElement >* self_ptr)
   {
      bool result;
      result = (*self_ptr)->isValid();
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigElement::assertValid()
   SHARPPY_API void jccl_ConfigElement_assertValid__0(boost::shared_ptr< jccl::ConfigElement >* self_ptr)
   {
      (*self_ptr)->assertValid();
   }


   // Wrapper for non-virtual method jccl::ConfigElement::getChildElement()
   SHARPPY_API jccl::ConfigElementPtr* jccl_ConfigElement_getChildElement__std_string1(boost::shared_ptr< jccl::ConfigElement >* self_ptr, const char* p0)
   {
      jccl::ConfigElementPtr* result;
      result = new jccl::ConfigElementPtr((*self_ptr)->getChildElement(p0));
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigElement::getNum()
   SHARPPY_API int jccl_ConfigElement_getNum__std_string1(boost::shared_ptr< jccl::ConfigElement >* self_ptr, const char* p0)
   {
      int result;
      result = (*self_ptr)->getNum(p0);
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigElement::getName()
   SHARPPY_API char* jccl_ConfigElement_getName__0(boost::shared_ptr< jccl::ConfigElement >* self_ptr)
   {
      std::string temp_result;
      char* result;
      temp_result = (*self_ptr)->getName();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigElement::getFullName()
   SHARPPY_API char* jccl_ConfigElement_getFullName__0(boost::shared_ptr< jccl::ConfigElement >* self_ptr)
   {
      std::string temp_result;
      char* result;
      temp_result = (*self_ptr)->getFullName();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigElement::getVersion()
   SHARPPY_API unsigned int jccl_ConfigElement_getVersion__0(boost::shared_ptr< jccl::ConfigElement >* self_ptr)
   {
      unsigned int result;
      result = (*self_ptr)->getVersion();
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigElement::getID()
   SHARPPY_API char* jccl_ConfigElement_getID__0(boost::shared_ptr< jccl::ConfigElement >* self_ptr)
   {
      std::string temp_result;
      char* result;
      temp_result = (*self_ptr)->getID();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigElement::setProperty()
   SHARPPY_API bool jccl_ConfigElement_setProperty__std_string_int_bool3(boost::shared_ptr< jccl::ConfigElement >* self_ptr, const char* p0, const int p1, bool p2)
   {
      bool result;
      result = (*self_ptr)->setProperty(p0, p1, p2);
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigElement::setProperty()
   SHARPPY_API bool jccl_ConfigElement_setProperty__std_string_int_jccl_ConfigElementPtr3(boost::shared_ptr< jccl::ConfigElement >* self_ptr, const char* p0, const int p1, jccl::ConfigElementPtr* p2)
   {
      bool result;
      result = (*self_ptr)->setProperty(p0, p1, *p2);
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigElement::setDefinition()
   SHARPPY_API void jccl_ConfigElement_setDefinition__jccl_ConfigDefinitionPtr1(boost::shared_ptr< jccl::ConfigElement >* self_ptr, jccl::ConfigDefinitionPtr* p0)
   {
      (*self_ptr)->setDefinition(*p0);
   }


   // Wrapper for non-virtual method jccl::ConfigElement::operator ==()
   SHARPPY_API bool jccl_ConfigElement_equal__jccl_ConfigElement1(boost::shared_ptr< jccl::ConfigElement >* self_ptr, const jccl::ConfigElement* p0)
   {
      const jccl::ConfigElement& marshal_p0 = *p0;
      bool result;
      result = (*self_ptr)->operator==(marshal_p0);
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigElement::operator !=()
   SHARPPY_API bool jccl_ConfigElement_not_equal__jccl_ConfigElement1(boost::shared_ptr< jccl::ConfigElement >* self_ptr, const jccl::ConfigElement* p0)
   {
      const jccl::ConfigElement& marshal_p0 = *p0;
      bool result;
      result = (*self_ptr)->operator!=(marshal_p0);
      return result;
   }


   // Wrapper for non-virtual method jccl::ConfigElement::operator <()
   SHARPPY_API bool jccl_ConfigElement_less_than__jccl_ConfigElement1(boost::shared_ptr< jccl::ConfigElement >* self_ptr, const jccl::ConfigElement* p0)
   {
      const jccl::ConfigElement& marshal_p0 = *p0;
      bool result;
      result = (*self_ptr)->operator<(marshal_p0);
      return result;
   }


} // extern "C" for jccl::ConfigElement


