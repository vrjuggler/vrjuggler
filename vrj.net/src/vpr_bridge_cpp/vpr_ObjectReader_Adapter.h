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

// Generated from Revision: 1.17 of RCSfile: class_cxx_adapter.tmpl,v


#ifndef __VPR_OBJECTREADER_ADAPTER__
#define __VPR_OBJECTREADER_ADAPTER__

#include "sharppy.h"
#include <vpr/IO/ObjectReader.h>

class SHARPPY_API vpr_ObjectReader_Adapter : public vpr::ObjectReader
{
public:
   virtual ~vpr_ObjectReader_Adapter()
   {;}

   typedef vpr::ReturnStatus* (*beginTag_callback_std_basic_string_char_std__char_traits_char__std__allocator_char___t)(char*);
   beginTag_callback_std_basic_string_char_std__char_traits_char__std__allocator_char___t beginTag_callback_std_basic_string_char_std__char_traits_char__std__allocator_char__;

   /// Override for virtual function vpr::ObjectReader::beginTag.
   virtual vpr::ReturnStatus beginTag(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0)
   {
      char* marshal_p0 = strdup(p0.c_str());
      vpr::ReturnStatus result = *(beginTag_callback_std_basic_string_char_std__char_traits_char__std__allocator_char__(marshal_p0));
      free(marshal_p0);
      return result;
   }

   typedef vpr::ReturnStatus* (*endTag_callback_t)();
   endTag_callback_t endTag_callback;

   /// Override for virtual function vpr::ObjectReader::endTag.
   virtual vpr::ReturnStatus endTag()
   {
      vpr::ReturnStatus result = *(endTag_callback());
      return result;
   }

   typedef vpr::ReturnStatus* (*beginAttribute_callback_std_basic_string_char_std__char_traits_char__std__allocator_char___t)(char*);
   beginAttribute_callback_std_basic_string_char_std__char_traits_char__std__allocator_char___t beginAttribute_callback_std_basic_string_char_std__char_traits_char__std__allocator_char__;

   /// Override for virtual function vpr::ObjectReader::beginAttribute.
   virtual vpr::ReturnStatus beginAttribute(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0)
   {
      char* marshal_p0 = strdup(p0.c_str());
      vpr::ReturnStatus result = *(beginAttribute_callback_std_basic_string_char_std__char_traits_char__std__allocator_char__(marshal_p0));
      free(marshal_p0);
      return result;
   }

   typedef vpr::ReturnStatus* (*endAttribute_callback_t)();
   endAttribute_callback_t endAttribute_callback;

   /// Override for virtual function vpr::ObjectReader::endAttribute.
   virtual vpr::ReturnStatus endAttribute()
   {
      vpr::ReturnStatus result = *(endAttribute_callback());
      return result;
   }

   typedef void (*resetReading_callback_t)();
   resetReading_callback_t resetReading_callback;

   /// Override for virtual function vpr::ObjectReader::resetReading.
   virtual void resetReading()
   {
      resetReading_callback();
   }

   typedef void (*pushState_callback_t)();
   pushState_callback_t pushState_callback;

   /// Override for virtual function vpr::ObjectReader::pushState.
   virtual void pushState()
   {
      pushState_callback();
   }

   typedef void (*popState_callback_t)();
   popState_callback_t popState_callback;

   /// Override for virtual function vpr::ObjectReader::popState.
   virtual void popState()
   {
      popState_callback();
   }

   typedef unsigned char (*readUint8_callback_t)();
   readUint8_callback_t readUint8_callback;

   /// Override for virtual function vpr::ObjectReader::readUint8.
   virtual unsigned char readUint8()
   {
      unsigned char result = readUint8_callback();
      return result;
   }

   typedef short unsigned int (*readUint16_callback_t)();
   readUint16_callback_t readUint16_callback;

   /// Override for virtual function vpr::ObjectReader::readUint16.
   virtual short unsigned int readUint16()
   {
      short unsigned int result = readUint16_callback();
      return result;
   }

   typedef unsigned int (*readUint32_callback_t)();
   readUint32_callback_t readUint32_callback;

   /// Override for virtual function vpr::ObjectReader::readUint32.
   virtual unsigned int readUint32()
   {
      unsigned int result = readUint32_callback();
      return result;
   }

   typedef vpr::Uint64 (*readUint64_callback_t)();
   readUint64_callback_t readUint64_callback;

   /// Override for virtual function vpr::ObjectReader::readUint64.
   virtual vpr::Uint64 readUint64()
   {
      vpr::Uint64 result = readUint64_callback();
      return result;
   }

   typedef float (*readFloat_callback_t)();
   readFloat_callback_t readFloat_callback;

   /// Override for virtual function vpr::ObjectReader::readFloat.
   virtual float readFloat()
   {
      float result = readFloat_callback();
      return result;
   }

   typedef double (*readDouble_callback_t)();
   readDouble_callback_t readDouble_callback;

   /// Override for virtual function vpr::ObjectReader::readDouble.
   virtual double readDouble()
   {
      double result = readDouble_callback();
      return result;
   }

   typedef char* (*readString_callback_t)();
   readString_callback_t readString_callback;

   /// Override for virtual function vpr::ObjectReader::readString.
   virtual std::basic_string<char,std::char_traits<char>,std::allocator<char> > readString()
   {
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > result = readString_callback();
      return result;
   }

   typedef bool (*readBool_callback_t)();
   readBool_callback_t readBool_callback;

   /// Override for virtual function vpr::ObjectReader::readBool.
   virtual bool readBool()
   {
      bool result = readBool_callback();
      return result;
   }

   typedef void (*readUint8_callback_unsigned_char_t)(unsigned char&);
   readUint8_callback_unsigned_char_t readUint8_callback_unsigned_char;

   /// Override for virtual function vpr::ObjectReader::readUint8.
   virtual void readUint8(unsigned char& p0)
   {
      readUint8_callback_unsigned_char(p0);
   }

   typedef void (*readUint16_callback_short_unsigned_int_t)(short unsigned int&);
   readUint16_callback_short_unsigned_int_t readUint16_callback_short_unsigned_int;

   /// Override for virtual function vpr::ObjectReader::readUint16.
   virtual void readUint16(short unsigned int& p0)
   {
      readUint16_callback_short_unsigned_int(p0);
   }

   typedef void (*readUint32_callback_unsigned_int_t)(unsigned int&);
   readUint32_callback_unsigned_int_t readUint32_callback_unsigned_int;

   /// Override for virtual function vpr::ObjectReader::readUint32.
   virtual void readUint32(unsigned int& p0)
   {
      readUint32_callback_unsigned_int(p0);
   }

   typedef void (*readUint64_callback_long_long_unsigned_int_t)(vpr::Uint64&);
   readUint64_callback_long_long_unsigned_int_t readUint64_callback_long_long_unsigned_int;

   /// Override for virtual function vpr::ObjectReader::readUint64.
   virtual void readUint64(vpr::Uint64& p0)
   {
      readUint64_callback_long_long_unsigned_int(p0);
   }

   typedef void (*readFloat_callback_float_t)(float&);
   readFloat_callback_float_t readFloat_callback_float;

   /// Override for virtual function vpr::ObjectReader::readFloat.
   virtual void readFloat(float& p0)
   {
      readFloat_callback_float(p0);
   }

   typedef void (*readDouble_callback_double_t)(double&);
   readDouble_callback_double_t readDouble_callback_double;

   /// Override for virtual function vpr::ObjectReader::readDouble.
   virtual void readDouble(double& p0)
   {
      readDouble_callback_double(p0);
   }

   typedef void (*readString_callback_std_basic_string_char_std__char_traits_char__std__allocator_char___t)(char**);
   readString_callback_std_basic_string_char_std__char_traits_char__std__allocator_char___t readString_callback_std_basic_string_char_std__char_traits_char__std__allocator_char__;

   /// Override for virtual function vpr::ObjectReader::readString.
   virtual void readString(std::basic_string<char,std::char_traits<char>,std::allocator<char> >& p0)
   {
      char* marshal_p0 = (char*) malloc(sizeof(char) * 256);
      readString_callback_std_basic_string_char_std__char_traits_char__std__allocator_char__(&marshal_p0);
      p0 = marshal_p0;
      free(marshal_p0);
   }

   typedef void (*readBool_callback_bool_t)(bool&);
   readBool_callback_bool_t readBool_callback_bool;

   /// Override for virtual function vpr::ObjectReader::readBool.
   virtual void readBool(bool& p0)
   {
      readBool_callback_bool(p0);
   }

}; // class vpr_ObjectReader_Adapter




#endif
