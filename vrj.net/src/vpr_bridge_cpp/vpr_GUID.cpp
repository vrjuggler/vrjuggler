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
#include <vpr/Util/GUID.h>
#include <vpr_GUID_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API vpr_GUID_Adapter* vpr_GUID_GUID__vpr_GUID_GenerateTag(const vpr::GUID::GenerateTag* p0, vpr_GUID_Adapter::writeObject_callback_vpr_ObjectWriter_t cb0, vpr_GUID_Adapter::readObject_callback_vpr_ObjectReader_t cb1)
   {
      const vpr::GUID::GenerateTag& marshal_p0 = *p0; // Pre-call marshaling for p0
      vpr_GUID_Adapter* obj = new vpr_GUID_Adapter(marshal_p0);
      obj->writeObject_callback_vpr_ObjectWriter = cb0;
      obj->readObject_callback_vpr_ObjectReader = cb1;
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vpr_GUID_Adapter* vpr_GUID_GUID__(vpr_GUID_Adapter::writeObject_callback_vpr_ObjectWriter_t cb0, vpr_GUID_Adapter::readObject_callback_vpr_ObjectReader_t cb1)
   {
      vpr_GUID_Adapter* obj = new vpr_GUID_Adapter();
      obj->writeObject_callback_vpr_ObjectWriter = cb0;
      obj->readObject_callback_vpr_ObjectReader = cb1;
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vpr_GUID_Adapter* vpr_GUID_GUID__vpr_GUID_StdGUID(const vpr::GUID::StdGUID* p0, vpr_GUID_Adapter::writeObject_callback_vpr_ObjectWriter_t cb0, vpr_GUID_Adapter::readObject_callback_vpr_ObjectReader_t cb1)
   {
      const vpr::GUID::StdGUID& marshal_p0 = *p0; // Pre-call marshaling for p0
      vpr_GUID_Adapter* obj = new vpr_GUID_Adapter(marshal_p0);
      obj->writeObject_callback_vpr_ObjectWriter = cb0;
      obj->readObject_callback_vpr_ObjectReader = cb1;
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vpr_GUID_Adapter* vpr_GUID_GUID__std_basic_string_char_std__char_traits_char__std__allocator_char__(const char* p0, vpr_GUID_Adapter::writeObject_callback_vpr_ObjectWriter_t cb0, vpr_GUID_Adapter::readObject_callback_vpr_ObjectReader_t cb1)
   {
      vpr_GUID_Adapter* obj = new vpr_GUID_Adapter(p0);
      obj->writeObject_callback_vpr_ObjectWriter = cb0;
      obj->readObject_callback_vpr_ObjectReader = cb1;
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vpr_GUID_Adapter* vpr_GUID_GUID__vpr_GUID_std_basic_string_char_std__char_traits_char__std__allocator_char__(const vpr::GUID* p0, const char* p1, vpr_GUID_Adapter::writeObject_callback_vpr_ObjectWriter_t cb0, vpr_GUID_Adapter::readObject_callback_vpr_ObjectReader_t cb1)
   {
      const vpr::GUID& marshal_p0 = *p0; // Pre-call marshaling for p0
      vpr_GUID_Adapter* obj = new vpr_GUID_Adapter(marshal_p0, p1);
      obj->writeObject_callback_vpr_ObjectWriter = cb0;
      obj->readObject_callback_vpr_ObjectReader = cb1;
      ; // Post-call marshaling for p0
      return obj;
   }

   // Copy constructor wrapper.
   SHARPPY_API vpr::GUID* vpr_GUID_GUID__vpr_GUID(const vpr_GUID_Adapter* p)
   {
      vpr_GUID_Adapter* obj = new vpr_GUID_Adapter(*p);
      obj->writeObject_callback_vpr_ObjectWriter = p->writeObject_callback_vpr_ObjectWriter;
      obj->readObject_callback_vpr_ObjectReader = p->readObject_callback_vpr_ObjectReader;
      return obj;
   }

   SHARPPY_API void delete_vpr_GUID(vpr_GUID_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method vpr::GUID::toString()
   SHARPPY_API char* vpr_GUID_toString__(vpr_GUID_Adapter* self_)
   {
      char* result;
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      temp_result = self_->toString();
      result = strdup(temp_result.c_str());

      return result;
   }

   // Wrapper for non-virtual method vpr::GUID::generate()
   SHARPPY_API void vpr_GUID_generate__(vpr_GUID_Adapter* self_)
   {
      self_->generate();
   }

   // Wrapper for non-virtual method vpr::GUID::generate()
   SHARPPY_API void vpr_GUID_generate__vpr_GUID_std_basic_string_char_std__char_traits_char__std__allocator_char__(vpr_GUID_Adapter* self_, const vpr::GUID* p0, const char* p1)
   {
      const vpr::GUID& marshal_p0 = *p0; // Pre-call marshaling for p0
      self_->generate(marshal_p0, p1);
      ; // Post-call marshaling for p0
   }


   // Wrapper for virtual method vpr::GUID::writeObject()
   SHARPPY_API vpr::ReturnStatus* vpr_GUID_writeObject__vpr_ObjectWriter(vpr_GUID_Adapter* self_, vpr::ObjectWriter* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->vpr::GUID::writeObject(p0));

      return result;
   }

   // Wrapper for virtual method vpr::GUID::readObject()
   SHARPPY_API vpr::ReturnStatus* vpr_GUID_readObject__vpr_ObjectReader(vpr_GUID_Adapter* self_, vpr::ObjectReader* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->vpr::GUID::readObject(p0));

      return result;
   }



   // Wrapper for reading static data member vpr::GUID::generateTag
   SHARPPY_API vpr::GUID::GenerateTag* vpr_GUID_generateTag_get()
   {
      return &vpr::GUID::generateTag;
   }

   // Wrapper for modifying static data member vpr::GUID::generateTag
   SHARPPY_API void vpr_GUID_generateTag_set(vpr::GUID::GenerateTag* v)
   {
      vpr::GUID::generateTag = *v;
   }

   // Wrapper for reading static data member vpr::GUID::NullGUID
   SHARPPY_API const vpr::GUID* vpr_GUID_NullGUID_get()
   {
      return &vpr::GUID::NullGUID;
   }


   // Wrapper for non-virtual method vpr::GUID::operator ==()
   SHARPPY_API bool vpr_GUID_equal__vpr_GUID(vpr_GUID_Adapter* self_, const vpr::GUID* p0)
   {
      const vpr::GUID& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->operator==(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }

   // Wrapper for non-virtual method vpr::GUID::operator !=()
   SHARPPY_API bool vpr_GUID_not_equal__vpr_GUID(vpr_GUID_Adapter* self_, const vpr::GUID* p0)
   {
      const vpr::GUID& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->operator!=(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }

   // Wrapper for non-virtual method vpr::GUID::operator <()
   SHARPPY_API bool vpr_GUID_less_than__vpr_GUID(vpr_GUID_Adapter* self_, const vpr::GUID* p0)
   {
      const vpr::GUID& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->operator<(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }


} // extern "C" for vpr::GUID

// Nested class vpr::GUID::GenerateTag.
extern "C"
{
   // Copy constructor wrapper.
   SHARPPY_API vpr::GUID::GenerateTag* vpr_GUID_GenerateTag_GenerateTag__vpr_GUID_GenerateTag(const vpr::GUID::GenerateTag* p)
   {
      vpr::GUID::GenerateTag* obj = new vpr::GUID::GenerateTag(*p);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vpr::GUID::GenerateTag* vpr_GUID_GenerateTag_GenerateTag__()
   {
      vpr::GUID::GenerateTag* obj = new vpr::GUID::GenerateTag();
      return obj;
   }

   SHARPPY_API void delete_vpr_GUID_GenerateTag(vpr::GUID::GenerateTag* self_)
   {
      delete self_;
   }





} // extern "C" for vpr::GUID::GenerateTag


// Nested class vpr::GUID::StdGUID.
extern "C"
{
   // Copy constructor wrapper.
   SHARPPY_API vpr::GUID::StdGUID* vpr_GUID_StdGUID_StdGUID__vpr_GUID_StdGUID(const vpr::GUID::StdGUID* p)
   {
      vpr::GUID::StdGUID* obj = new vpr::GUID::StdGUID(*p);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vpr::GUID::StdGUID* vpr_GUID_StdGUID_StdGUID__()
   {
      vpr::GUID::StdGUID* obj = new vpr::GUID::StdGUID();
      return obj;
   }

   SHARPPY_API void delete_vpr_GUID_StdGUID(vpr::GUID::StdGUID* self_)
   {
      delete self_;
   }





} // extern "C" for vpr::GUID::StdGUID


// Nested class vpr::GUID::hash.
extern "C"
{
   // Copy constructor wrapper.
   SHARPPY_API vpr::GUID::hash* vpr_GUID_hash_hash__vpr_GUID_hash(const vpr::GUID::hash* p)
   {
      vpr::GUID::hash* obj = new vpr::GUID::hash(*p);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vpr::GUID::hash* vpr_GUID_hash_hash__()
   {
      vpr::GUID::hash* obj = new vpr::GUID::hash();
      return obj;
   }

   SHARPPY_API void delete_vpr_GUID_hash(vpr::GUID::hash* self_)
   {
      delete self_;
   }





} // extern "C" for vpr::GUID::hash



