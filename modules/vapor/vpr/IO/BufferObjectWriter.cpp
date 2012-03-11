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

#include <vpr/IO/BufferObjectWriter.h>


namespace vpr
{
/** Number of bytes used to store the size of the string. */
const unsigned int BufferObjectWriter::STRING_LENGTH_SIZE = 4;

BufferObjectWriter::BufferObjectWriter()
   : mOwnDataBuffer(true)
   , mData(new std::vector<vpr::Uint8>)
   , mCurHeadPos(0)
{
   mIsBinary = true;
}

BufferObjectWriter::BufferObjectWriter(std::vector<vpr::Uint8>* data,
                                       const unsigned int curPos)
   : mOwnDataBuffer(false)
   , mData(data)
   , mCurHeadPos(curPos)
{
   mIsBinary = true;
}

BufferObjectWriter::~BufferObjectWriter()
{
   if(mOwnDataBuffer)
   {
      delete mData;
      mData = NULL;
   }
}

void BufferObjectWriter::writeUint8(vpr::Uint8 val)
{
   writeRaw(&val, 1);
}

void BufferObjectWriter::writeUint16(vpr::Uint16 val)
{
   vpr::Uint16 nw_val = vpr::System::Htons(val);

   writeRaw((vpr::Uint8*) &nw_val, 2);
}

void BufferObjectWriter::writeUint32(vpr::Uint32 val)
{
   vpr::Uint32 nw_val = vpr::System::Htonl(val);

   writeRaw((vpr::Uint8*) &nw_val, 4);
}

void BufferObjectWriter::writeUint64(vpr::Uint64 val)
{
   vpr::Uint64 nw_val = vpr::System::Htonll(val);

   writeRaw((vpr::Uint8*) &nw_val, 8);
}

void BufferObjectWriter::writeInt8(vpr::Int8 val)
{
   writeRaw((vpr::Uint8*) &val, 1);
}

void BufferObjectWriter::writeInt16(vpr::Int16 val)
{
   vpr::Int16 nw_val = vpr::System::Htons(val);

   writeRaw((vpr::Uint8*) &nw_val, 2);
}

void BufferObjectWriter::writeInt32(vpr::Int32 val)
{
   vpr::Int32 nw_val = vpr::System::Htonl(val);

   writeRaw((vpr::Uint8*) &nw_val, 4);
}

void BufferObjectWriter::writeInt64(vpr::Int64 val)
{
   vpr::Int64 nw_val = vpr::System::Htonll(val);

   writeRaw((vpr::Uint8*) &nw_val, 8);
}

void BufferObjectWriter::writeFloat(float val)
{
   // We are writing the float as a 4 byte value
   BOOST_STATIC_ASSERT(sizeof(float) == 4);
   union
   {
     float       floatVal;
     vpr::Uint32 intVal;
   } data;

   data.floatVal = val;
   data.intVal   = vpr::System::Htonl(data.intVal);

   writeRaw((vpr::Uint8*) &data.intVal, 4);
}

void BufferObjectWriter::writeDouble(double val)
{
   // We are writing the double as a 8 byte value
   BOOST_STATIC_ASSERT(sizeof(double) == 8);
   union
   {
     double      doubleVal;
     vpr::Uint64 intVal;
   } data;

   data.doubleVal = val;
   data.intVal    = vpr::System::Htonll(data.intVal);

   writeRaw((vpr::Uint8*) &data.intVal, 8);
}

void BufferObjectWriter::writeString(std::string val)
{
   // Note: If you change this, you need to change STRING_LENGTH_SIZE
   writeUint32(val.size());

   for ( vpr::Uint32 i = 0; i < val.length(); ++i )
   {
      writeRaw((vpr::Uint8*) &val[i], 1);
   }
}

void BufferObjectWriter::writeBool(bool val)
{
   // Darwin uses four bytes (!) for bools.
#ifdef VPR_OS_Darwin
   vpr::Uint8 temp = (vpr::Uint8) val;
   writeRaw((vpr::Uint8*) &temp, 1);
#else
   writeRaw((vpr::Uint8*) &val, 1);
#endif
}

}
