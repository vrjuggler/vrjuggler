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

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <vrj/Display/Display.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API vrj::Display* vrj_Display_Display__vrj_Display1(const vrj::Display* p0)
   {
      const vrj::Display& marshal_p0 = *p0;
      vrj::Display* obj = new vrj::Display(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vrj::Display* vrj_Display_Display__0()
   {
      vrj::Display* obj = new vrj::Display();
      return obj;
   }

   SHARPPY_API void delete_vrj_Display(vrj::Display* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method vrj::Display::configDisplayWindow()
   SHARPPY_API void vrj_Display_configDisplayWindow__boost_shared_ptr_jccl__ConfigElement1(vrj::Display* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      self_->configDisplayWindow(p0);
   }


   // Wrapper for non-virtual method vrj::Display::configViewports()
   SHARPPY_API void vrj_Display_configViewports__boost_shared_ptr_jccl__ConfigElement1(vrj::Display* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      self_->configViewports(p0);
   }


   // Wrapper for non-virtual method vrj::Display::updateProjections()
   SHARPPY_API void vrj_Display_updateProjections__float1(vrj::Display* self_, const float p0)
   {
      self_->updateProjections(p0);
   }


   // Wrapper for non-virtual method vrj::Display::isActive()
   SHARPPY_API bool vrj_Display_isActive__0(vrj::Display* self_)
   {
      bool result;
      result = self_->isActive();
      return result;
   }


   // Wrapper for non-virtual method vrj::Display::setName()
   SHARPPY_API void vrj_Display_setName__std_basic_string_char_std__char_traits_char__std__allocator_char__1(vrj::Display* self_, char* p0)
   {
      self_->setName(p0);
   }


   // Wrapper for non-virtual method vrj::Display::getName()
   SHARPPY_API char* vrj_Display_getName__0(vrj::Display* self_)
   {
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      char* result;
      temp_result = self_->getName();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for non-virtual method vrj::Display::shouldDrawBorder()
   SHARPPY_API bool vrj_Display_shouldDrawBorder__0(vrj::Display* self_)
   {
      bool result;
      result = self_->shouldDrawBorder();
      return result;
   }


   // Wrapper for non-virtual method vrj::Display::setOriginAndSize()
   SHARPPY_API void vrj_Display_setOriginAndSize__int_int_int_int_bool4(vrj::Display* self_, int p0, int p1, int p2, int p3)
   {
      self_->setOriginAndSize(p0, p1, p2, p3);
   }

   // Wrapper for non-virtual method vrj::Display::setOriginAndSize()
   SHARPPY_API void vrj_Display_setOriginAndSize__int_int_int_int_bool5(vrj::Display* self_, int p0, int p1, int p2, int p3, bool p4)
   {
      self_->setOriginAndSize(p0, p1, p2, p3, p4);
   }


   // Wrapper for non-virtual method vrj::Display::getOriginAndSize()
   SHARPPY_API void vrj_Display_getOriginAndSize__int_int_int_int4(vrj::Display* self_, int& p0, int& p1, int& p2, int& p3)
   {
      self_->getOriginAndSize(p0, p1, p2, p3);
   }


   // Wrapper for non-virtual method vrj::Display::setPipe()
   SHARPPY_API void vrj_Display_setPipe__int1(vrj::Display* self_, int p0)
   {
      self_->setPipe(p0);
   }


   // Wrapper for non-virtual method vrj::Display::getPipe()
   SHARPPY_API int vrj_Display_getPipe__0(vrj::Display* self_)
   {
      int result;
      result = self_->getPipe();
      return result;
   }


   // Wrapper for non-virtual method vrj::Display::isStereoRequested()
   SHARPPY_API bool vrj_Display_isStereoRequested__0(vrj::Display* self_)
   {
      bool result;
      result = self_->isStereoRequested();
      return result;
   }


   // Wrapper for non-virtual method vrj::Display::getConfigElement()
   SHARPPY_API boost::shared_ptr<jccl::ConfigElement>* vrj_Display_getConfigElement__0(vrj::Display* self_)
   {
      boost::shared_ptr<jccl::ConfigElement>* result;
      result = new boost::shared_ptr<jccl::ConfigElement>(self_->getConfigElement());
      return result;
   }


   // Wrapper for non-virtual method vrj::Display::getGlFrameBufferConfig()
   SHARPPY_API boost::shared_ptr<jccl::ConfigElement>* vrj_Display_getGlFrameBufferConfig__0(vrj::Display* self_)
   {
      boost::shared_ptr<jccl::ConfigElement>* result;
      result = new boost::shared_ptr<jccl::ConfigElement>(self_->getGlFrameBufferConfig());
      return result;
   }


   // Wrapper for non-virtual method vrj::Display::getNumViewports()
   SHARPPY_API unsigned int vrj_Display_getNumViewports__0(vrj::Display* self_)
   {
      unsigned int result;
      result = self_->getNumViewports();
      return result;
   }


   // Wrapper for virtual method vrj::Display::config()
   SHARPPY_API void vrj_Display_config__boost_shared_ptr_jccl__ConfigElement1(vrj::Display* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      self_->config(p0);
   }


} // extern "C" for vrj::Display


