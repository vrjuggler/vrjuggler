// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



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
   SHARPPY_API bool vpr_ObjectReader_isBinary__0(vpr_ObjectReader_Adapter* self_)
   {
      bool result;
      result = self_->isBinary();
      return result;
   }


   // Wrapper for virtual method vpr::ObjectReader::beginTag()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectReader_beginTag__std_string1(vpr_ObjectReader_Adapter* self_, char* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->beginTag(p0));
      return result;
   }


   // Wrapper for virtual method vpr::ObjectReader::endTag()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectReader_endTag__0(vpr_ObjectReader_Adapter* self_)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->endTag());
      return result;
   }


   // Wrapper for virtual method vpr::ObjectReader::beginAttribute()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectReader_beginAttribute__std_string1(vpr_ObjectReader_Adapter* self_, char* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->beginAttribute(p0));
      return result;
   }


   // Wrapper for virtual method vpr::ObjectReader::endAttribute()
   SHARPPY_API vpr::ReturnStatus* vpr_ObjectReader_endAttribute__0(vpr_ObjectReader_Adapter* self_)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->endAttribute());
      return result;
   }


   // Wrapper for virtual method vpr::ObjectReader::resetReading()
   SHARPPY_API void vpr_ObjectReader_resetReading__0(vpr_ObjectReader_Adapter* self_)
   {
      self_->resetReading();
   }


   // Wrapper for virtual method vpr::ObjectReader::pushState()
   SHARPPY_API void vpr_ObjectReader_pushState__0(vpr_ObjectReader_Adapter* self_)
   {
      self_->pushState();
   }


   // Wrapper for virtual method vpr::ObjectReader::popState()
   SHARPPY_API void vpr_ObjectReader_popState__0(vpr_ObjectReader_Adapter* self_)
   {
      self_->popState();
   }


   // Wrapper for virtual method vpr::ObjectReader::readUint8()
   SHARPPY_API vpr::Uint8 vpr_ObjectReader_readUint8__0(vpr_ObjectReader_Adapter* self_)
   {
      vpr::Uint8 result;
      result = self_->readUint8();
      return result;
   }


   // Wrapper for virtual method vpr::ObjectReader::readUint16()
   SHARPPY_API vpr::Uint16 vpr_ObjectReader_readUint16__0(vpr_ObjectReader_Adapter* self_)
   {
      vpr::Uint16 result;
      result = self_->readUint16();
      return result;
   }


   // Wrapper for virtual method vpr::ObjectReader::readUint32()
   SHARPPY_API vpr::Uint32 vpr_ObjectReader_readUint32__0(vpr_ObjectReader_Adapter* self_)
   {
      vpr::Uint32 result;
      result = self_->readUint32();
      return result;
   }


   // Wrapper for virtual method vpr::ObjectReader::readUint64()
   SHARPPY_API vpr::Uint64 vpr_ObjectReader_readUint64__0(vpr_ObjectReader_Adapter* self_)
   {
      vpr::Uint64 result;
      result = self_->readUint64();
      return result;
   }


   // Wrapper for virtual method vpr::ObjectReader::readFloat()
   SHARPPY_API float vpr_ObjectReader_readFloat__0(vpr_ObjectReader_Adapter* self_)
   {
      float result;
      result = self_->readFloat();
      return result;
   }


   // Wrapper for virtual method vpr::ObjectReader::readDouble()
   SHARPPY_API double vpr_ObjectReader_readDouble__0(vpr_ObjectReader_Adapter* self_)
   {
      double result;
      result = self_->readDouble();
      return result;
   }


   // Wrapper for virtual method vpr::ObjectReader::readString()
   SHARPPY_API char* vpr_ObjectReader_readString__0(vpr_ObjectReader_Adapter* self_)
   {
      std::string temp_result;
      char* result;
      temp_result = self_->readString();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for virtual method vpr::ObjectReader::readBool()
   SHARPPY_API bool vpr_ObjectReader_readBool__0(vpr_ObjectReader_Adapter* self_)
   {
      bool result;
      result = self_->readBool();
      return result;
   }


   // Wrapper for virtual method vpr::ObjectReader::readUint8()
   SHARPPY_API void vpr_ObjectReader_readUint8__vpr_Uint81(vpr_ObjectReader_Adapter* self_, vpr::Uint8& p0)
   {
      self_->vpr::ObjectReader::readUint8(p0);
   }


   // Wrapper for virtual method vpr::ObjectReader::readUint16()
   SHARPPY_API void vpr_ObjectReader_readUint16__vpr_Uint161(vpr_ObjectReader_Adapter* self_, vpr::Uint16& p0)
   {
      self_->vpr::ObjectReader::readUint16(p0);
   }


   // Wrapper for virtual method vpr::ObjectReader::readUint32()
   SHARPPY_API void vpr_ObjectReader_readUint32__vpr_Uint321(vpr_ObjectReader_Adapter* self_, vpr::Uint32& p0)
   {
      self_->vpr::ObjectReader::readUint32(p0);
   }


   // Wrapper for virtual method vpr::ObjectReader::readUint64()
   SHARPPY_API void vpr_ObjectReader_readUint64__vpr_Uint641(vpr_ObjectReader_Adapter* self_, vpr::Uint64& p0)
   {
      self_->vpr::ObjectReader::readUint64(p0);
   }


   // Wrapper for virtual method vpr::ObjectReader::readFloat()
   SHARPPY_API void vpr_ObjectReader_readFloat__float1(vpr_ObjectReader_Adapter* self_, float& p0)
   {
      self_->vpr::ObjectReader::readFloat(p0);
   }


   // Wrapper for virtual method vpr::ObjectReader::readDouble()
   SHARPPY_API void vpr_ObjectReader_readDouble__double1(vpr_ObjectReader_Adapter* self_, double& p0)
   {
      self_->vpr::ObjectReader::readDouble(p0);
   }


   // Wrapper for virtual method vpr::ObjectReader::readString()
   SHARPPY_API void vpr_ObjectReader_readString__std_string1(vpr_ObjectReader_Adapter* self_, char** p0)
   {
      std::string marshal_p0 = *p0;
      self_->vpr::ObjectReader::readString(marshal_p0);
      *p0 = strdup(marshal_p0.c_str());
   }


   // Wrapper for virtual method vpr::ObjectReader::readBool()
   SHARPPY_API void vpr_ObjectReader_readBool__bool1(vpr_ObjectReader_Adapter* self_, bool& p0)
   {
      self_->vpr::ObjectReader::readBool(p0);
   }


} // extern "C" for vpr::ObjectReader


