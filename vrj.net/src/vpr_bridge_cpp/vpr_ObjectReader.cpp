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

// Generated from Revision: 1.58 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <vpr/IO/ObjectReader.h>
#include <vpr_ObjectReader_Adapter.h>

extern "C"
{
   SHARPPY_API void delete_vpr_ObjectReader(vpr_ObjectReader_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method vpr::ObjectReader::isBinary()
   SHARPPY_API bool vpr_ObjectReader_isBinary__(vpr_ObjectReader_Adapter* self_)
   {
      bool result;
      result = self_->isBinary();

      return result;
   }


   // Wrapper for virtual method vpr::ObjectReader::beginTag()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectReader_beginTag__std_basic_string_char_std__char_traits_char__std__allocator_char__(vpr_ObjectReader_Adapter* self_, char* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->beginTag(p0));

      return result;
   }

   // Wrapper for virtual method vpr::ObjectReader::endTag()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectReader_endTag__(vpr_ObjectReader_Adapter* self_)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->endTag());

      return result;
   }

   // Wrapper for virtual method vpr::ObjectReader::beginAttribute()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectReader_beginAttribute__std_basic_string_char_std__char_traits_char__std__allocator_char__(vpr_ObjectReader_Adapter* self_, char* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->beginAttribute(p0));

      return result;
   }

   // Wrapper for virtual method vpr::ObjectReader::endAttribute()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectReader_endAttribute__(vpr_ObjectReader_Adapter* self_)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->endAttribute());

      return result;
   }

   // Wrapper for virtual method vpr::ObjectReader::resetReading()
   SHARPPY_API void vpr_ObjectReader_resetReading__(vpr_ObjectReader_Adapter* self_)
   {
      self_->resetReading();
   }

   // Wrapper for virtual method vpr::ObjectReader::pushState()
   SHARPPY_API void vpr_ObjectReader_pushState__(vpr_ObjectReader_Adapter* self_)
   {
      self_->pushState();
   }

   // Wrapper for virtual method vpr::ObjectReader::popState()
   SHARPPY_API void vpr_ObjectReader_popState__(vpr_ObjectReader_Adapter* self_)
   {
      self_->popState();
   }

   // Wrapper for virtual method vpr::ObjectReader::readUint8()
   SHARPPY_API unsigned char vpr_ObjectReader_readUint8__(vpr_ObjectReader_Adapter* self_)
   {
      unsigned char result;
      result = self_->readUint8();

      return result;
   }

   // Wrapper for virtual method vpr::ObjectReader::readUint16()
   SHARPPY_API short unsigned int vpr_ObjectReader_readUint16__(vpr_ObjectReader_Adapter* self_)
   {
      short unsigned int result;
      result = self_->readUint16();

      return result;
   }

   // Wrapper for virtual method vpr::ObjectReader::readUint32()
   SHARPPY_API unsigned int vpr_ObjectReader_readUint32__(vpr_ObjectReader_Adapter* self_)
   {
      unsigned int result;
      result = self_->readUint32();

      return result;
   }

   // Wrapper for virtual method vpr::ObjectReader::readUint64()
   SHARPPY_API long long unsigned int vpr_ObjectReader_readUint64__(vpr_ObjectReader_Adapter* self_)
   {
      long long unsigned int result;
      result = self_->readUint64();

      return result;
   }

   // Wrapper for virtual method vpr::ObjectReader::readFloat()
   SHARPPY_API float vpr_ObjectReader_readFloat__(vpr_ObjectReader_Adapter* self_)
   {
      float result;
      result = self_->readFloat();

      return result;
   }

   // Wrapper for virtual method vpr::ObjectReader::readDouble()
   SHARPPY_API double vpr_ObjectReader_readDouble__(vpr_ObjectReader_Adapter* self_)
   {
      double result;
      result = self_->readDouble();

      return result;
   }

   // Wrapper for virtual method vpr::ObjectReader::readString()
   SHARPPY_API char* vpr_ObjectReader_readString__(vpr_ObjectReader_Adapter* self_)
   {
      char* result;
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      temp_result = self_->readString();
      result = strdup(temp_result.c_str());

      return result;
   }

   // Wrapper for virtual method vpr::ObjectReader::readBool()
   SHARPPY_API bool vpr_ObjectReader_readBool__(vpr_ObjectReader_Adapter* self_)
   {
      bool result;
      result = self_->readBool();

      return result;
   }

   // Wrapper for virtual method vpr::ObjectReader::readUint8()
   SHARPPY_API void vpr_ObjectReader_readUint8__unsigned_char(vpr_ObjectReader_Adapter* self_, unsigned char& p0)
   {
      self_->vpr::ObjectReader::readUint8(p0);
   }

   // Wrapper for virtual method vpr::ObjectReader::readUint16()
   SHARPPY_API void vpr_ObjectReader_readUint16__short_unsigned_int(vpr_ObjectReader_Adapter* self_, short unsigned int& p0)
   {
      self_->vpr::ObjectReader::readUint16(p0);
   }

   // Wrapper for virtual method vpr::ObjectReader::readUint32()
   SHARPPY_API void vpr_ObjectReader_readUint32__unsigned_int(vpr_ObjectReader_Adapter* self_, unsigned int& p0)
   {
      self_->vpr::ObjectReader::readUint32(p0);
   }

   // Wrapper for virtual method vpr::ObjectReader::readUint64()
   SHARPPY_API void vpr_ObjectReader_readUint64__long_long_unsigned_int(vpr_ObjectReader_Adapter* self_, long long unsigned int& p0)
   {
      self_->vpr::ObjectReader::readUint64(p0);
   }

   // Wrapper for virtual method vpr::ObjectReader::readFloat()
   SHARPPY_API void vpr_ObjectReader_readFloat__float(vpr_ObjectReader_Adapter* self_, float& p0)
   {
      self_->vpr::ObjectReader::readFloat(p0);
   }

   // Wrapper for virtual method vpr::ObjectReader::readDouble()
   SHARPPY_API void vpr_ObjectReader_readDouble__double(vpr_ObjectReader_Adapter* self_, double& p0)
   {
      self_->vpr::ObjectReader::readDouble(p0);
   }

   // Wrapper for virtual method vpr::ObjectReader::readString()
   SHARPPY_API void vpr_ObjectReader_readString__std_basic_string_char_std__char_traits_char__std__allocator_char__(vpr_ObjectReader_Adapter* self_, char** p0)
   {
      std::string marshal_p0 = *p0; // Pre-call marshaling for p0
      self_->vpr::ObjectReader::readString(marshal_p0);
      *p0 = strdup(marshal_p0.c_str()); // Post-call marshaling for p0
   }

   // Wrapper for virtual method vpr::ObjectReader::readBool()
   SHARPPY_API void vpr_ObjectReader_readBool__bool(vpr_ObjectReader_Adapter* self_, bool& p0)
   {
      self_->vpr::ObjectReader::readBool(p0);
   }





} // extern "C" for vpr::ObjectReader


