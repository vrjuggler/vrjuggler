/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <vpr/IO/BufferObjectReader.h>


namespace vpr
{
/** Number of bytes used to store the size of the string. */
const unsigned int BufferObjectReader::STRING_LENGTH_SIZE = 4;

BufferObjectReader::BufferObjectReader(std::vector<vpr::Uint8>* data,
                                       const unsigned int curPos)
   : mData(data)
   , mCurHeadPos(curPos)
{
   mIsBinary = true;
}

BufferObjectReader::~BufferObjectReader()
{
   /* Do nothing. */ ;
}

vpr::Uint8 BufferObjectReader::readUint8()
{
   vpr::Uint8 temp_data;
   std::memcpy(&temp_data, readRaw(1), 1);
   return temp_data;
}

vpr::Uint16 BufferObjectReader::readUint16()
{
   vpr::Uint16 nw_val;
   std::memcpy(&nw_val, readRaw(2), 2);

   return vpr::System::Ntohs(nw_val);
}

vpr::Uint32 BufferObjectReader::readUint32()
{
   vpr::Uint32 nw_val;
   std::memcpy(&nw_val, readRaw(4), 4);

   return vpr::System::Ntohl(nw_val);
}

vpr::Uint64 BufferObjectReader::readUint64()
{
   vpr::Uint64 nw_val;
   std::memcpy(&nw_val, readRaw(8), 8);
   vpr::Uint64 h_val = vpr::System::Ntohll(nw_val);

   return h_val;
}

vpr::Int8 BufferObjectReader::readInt8()
{
   vpr::Int8 temp_data;
   std::memcpy(&temp_data, readRaw(1), 1);
   return temp_data;
}

vpr::Int16 BufferObjectReader::readInt16()
{
   vpr::Int16 nw_val;
   std::memcpy(&nw_val, readRaw(2), 2);

   return vpr::System::Ntohs(nw_val);
}

vpr::Int32 BufferObjectReader::readInt32()
{
   vpr::Int32 nw_val;
   std::memcpy(&nw_val, readRaw(4), 4);

   return vpr::System::Ntohl(nw_val);
}

vpr::Int64 BufferObjectReader::readInt64()
{
   vpr::Int64 nw_val;
   std::memcpy(&nw_val, readRaw(8), 8);
   vpr::Int64 h_val = vpr::System::Ntohll(nw_val);

   return h_val;
}

float BufferObjectReader::readFloat()
{
   // We are reading the float as a 4 byte value
   BOOST_STATIC_ASSERT(sizeof(float) == 4);
   union
   {
     float       floatVal;
     vpr::Uint32 intVal;
   } data;

   std::memcpy(&data.intVal, readRaw(4), 4);
   data.intVal = vpr::System::Ntohl(data.intVal);

   return data.floatVal;
}

double BufferObjectReader::readDouble()
{
   // We are reading the double as a 8 byte value
   BOOST_STATIC_ASSERT(sizeof(double) == 8);
   union
   {
     double      doubleVal;
     vpr::Uint64 intVal;
   } data;

   std::memcpy(&data.intVal, readRaw(8), 8);
   data.intVal = vpr::System::Ntohl(data.intVal);

   return data.doubleVal;
}

std::string BufferObjectReader::readString()
{
   // Note: If you change this, you need to change STRING_LENGTH_SIZE
   vpr::Uint32 str_len = readUint32();
   std::string ret_val;
   char tempChar;
   for(vpr::Uint32 i=0; i<str_len;++i)
   {
      tempChar = (char)(*readRaw(1));
      ret_val += tempChar;
   }
   return ret_val;
}

bool BufferObjectReader::readBool()
{
   return (bool)*(readRaw(1));
}


}
