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

// Generated from Revision: 1.15 of RCSfile: class_cxx_adapter.tmpl,v


#ifndef __VPR_OBJECTWRITER_ADAPTER__
#define __VPR_OBJECTWRITER_ADAPTER__

#include "sharppy.h"
#include <vpr/IO/ObjectWriter.h>

class SHARPPY_API vpr_ObjectWriter_Adapter : public vpr::ObjectWriter
{
public:
   virtual ~vpr_ObjectWriter_Adapter()
   {;}

   typedef vpr::ReturnStatus* (*beginTag_callback_std_basic_string_char_std__char_traits_char__std__allocator_char___t)(char*);
   beginTag_callback_std_basic_string_char_std__char_traits_char__std__allocator_char___t beginTag_callback_std_basic_string_char_std__char_traits_char__std__allocator_char__;

   /// Override for virtual function vpr::ObjectWriter::beginTag.
   virtual vpr::ReturnStatus beginTag(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0)
   {
      char* marshal_p0 = strdup(p0.c_str());
      vpr::ReturnStatus result = *(beginTag_callback_std_basic_string_char_std__char_traits_char__std__allocator_char__(marshal_p0));
      free(marshal_p0);
      return result;
   }

   typedef vpr::ReturnStatus* (*endTag_callback_t)();
   endTag_callback_t endTag_callback;

   /// Override for virtual function vpr::ObjectWriter::endTag.
   virtual vpr::ReturnStatus endTag()
   {
      vpr::ReturnStatus result = *(endTag_callback());
      return result;
   }

   typedef vpr::ReturnStatus* (*beginAttribute_callback_std_basic_string_char_std__char_traits_char__std__allocator_char___t)(char*);
   beginAttribute_callback_std_basic_string_char_std__char_traits_char__std__allocator_char___t beginAttribute_callback_std_basic_string_char_std__char_traits_char__std__allocator_char__;

   /// Override for virtual function vpr::ObjectWriter::beginAttribute.
   virtual vpr::ReturnStatus beginAttribute(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0)
   {
      char* marshal_p0 = strdup(p0.c_str());
      vpr::ReturnStatus result = *(beginAttribute_callback_std_basic_string_char_std__char_traits_char__std__allocator_char__(marshal_p0));
      free(marshal_p0);
      return result;
   }

   typedef vpr::ReturnStatus* (*endAttribute_callback_t)();
   endAttribute_callback_t endAttribute_callback;

   /// Override for virtual function vpr::ObjectWriter::endAttribute.
   virtual vpr::ReturnStatus endAttribute()
   {
      vpr::ReturnStatus result = *(endAttribute_callback());
      return result;
   }

   typedef vpr::ReturnStatus* (*writeUint8_callback_unsigned_char_t)(unsigned char);
   writeUint8_callback_unsigned_char_t writeUint8_callback_unsigned_char;

   /// Override for virtual function vpr::ObjectWriter::writeUint8.
   virtual vpr::ReturnStatus writeUint8(unsigned char p0)
   {
      vpr::ReturnStatus result = *(writeUint8_callback_unsigned_char(p0));
      return result;
   }

   typedef vpr::ReturnStatus* (*writeUint16_callback_short_unsigned_int_t)(short unsigned int);
   writeUint16_callback_short_unsigned_int_t writeUint16_callback_short_unsigned_int;

   /// Override for virtual function vpr::ObjectWriter::writeUint16.
   virtual vpr::ReturnStatus writeUint16(short unsigned int p0)
   {
      vpr::ReturnStatus result = *(writeUint16_callback_short_unsigned_int(p0));
      return result;
   }

   typedef vpr::ReturnStatus* (*writeUint32_callback_unsigned_int_t)(unsigned int);
   writeUint32_callback_unsigned_int_t writeUint32_callback_unsigned_int;

   /// Override for virtual function vpr::ObjectWriter::writeUint32.
   virtual vpr::ReturnStatus writeUint32(unsigned int p0)
   {
      vpr::ReturnStatus result = *(writeUint32_callback_unsigned_int(p0));
      return result;
   }

   typedef vpr::ReturnStatus* (*writeUint64_callback_long_long_unsigned_int_t)(vpr::Uint64);
   writeUint64_callback_long_long_unsigned_int_t writeUint64_callback_long_long_unsigned_int;

   /// Override for virtual function vpr::ObjectWriter::writeUint64.
   virtual vpr::ReturnStatus writeUint64(vpr::Uint64 p0)
   {
      vpr::ReturnStatus result = *(writeUint64_callback_long_long_unsigned_int(p0));
      return result;
   }

   typedef vpr::ReturnStatus* (*writeFloat_callback_float_t)(float);
   writeFloat_callback_float_t writeFloat_callback_float;

   /// Override for virtual function vpr::ObjectWriter::writeFloat.
   virtual vpr::ReturnStatus writeFloat(float p0)
   {
      vpr::ReturnStatus result = *(writeFloat_callback_float(p0));
      return result;
   }

   typedef vpr::ReturnStatus* (*writeDouble_callback_double_t)(double);
   writeDouble_callback_double_t writeDouble_callback_double;

   /// Override for virtual function vpr::ObjectWriter::writeDouble.
   virtual vpr::ReturnStatus writeDouble(double p0)
   {
      vpr::ReturnStatus result = *(writeDouble_callback_double(p0));
      return result;
   }

   typedef vpr::ReturnStatus* (*writeString_callback_std_basic_string_char_std__char_traits_char__std__allocator_char___t)(char*);
   writeString_callback_std_basic_string_char_std__char_traits_char__std__allocator_char___t writeString_callback_std_basic_string_char_std__char_traits_char__std__allocator_char__;

   /// Override for virtual function vpr::ObjectWriter::writeString.
   virtual vpr::ReturnStatus writeString(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0)
   {
      char* marshal_p0 = strdup(p0.c_str());
      vpr::ReturnStatus result = *(writeString_callback_std_basic_string_char_std__char_traits_char__std__allocator_char__(marshal_p0));
      free(marshal_p0);
      return result;
   }

   typedef vpr::ReturnStatus* (*writeBool_callback_bool_t)(bool);
   writeBool_callback_bool_t writeBool_callback_bool;

   /// Override for virtual function vpr::ObjectWriter::writeBool.
   virtual vpr::ReturnStatus writeBool(bool p0)
   {
      vpr::ReturnStatus result = *(writeBool_callback_bool(p0));
      return result;
   }

}; // class vpr_ObjectWriter_Adapter




#endif
