/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_OBJECT_WRITER_H
#define _VPR_OBJECT_WRITER_H

#include <vpr/vprConfig.h>
#include <vpr/System.h>
#include <vpr/Util/Debug.h>

#include <vector>

#include <boost/static_assert.hpp>


namespace vpr
{

/** Interface used to write object data to a stream.
*
* @todo: Add smart buffering for type sizes
*/
class ObjectWriter
{
public:
   ObjectWriter()
   {
      mData = new std::vector<vpr::Uint8>;
      mCurHeadPos = 0;
   }

   ObjectWriter(std::vector<vpr::Uint8>* data, unsigned curPos=0)
   {
      mData = data;
      mCurHeadPos = curPos;
   }

   void setCurPos(unsigned val)
   { mCurHeadPos = val; }
   unsigned getCurPos()
   { return mCurHeadPos; }

   std::vector<vpr::Uint8>* getData()
   { return mData; }

   vpr::ReturnStatus writeUint8(vpr::Uint8 val);
   vpr::ReturnStatus writeUint16(vpr::Uint16 val);
   vpr::ReturnStatus writeUint32(vpr::Uint32 val);
   vpr::ReturnStatus writeUint64(vpr::Uint64 val);
   vpr::ReturnStatus writeFloat(float val);
   vpr::ReturnStatus writeDouble(double val);
   vpr::ReturnStatus writeString(std::string val);
   vpr::ReturnStatus writeBool(bool val);
   inline void adjust(unsigned d);

   /* Write raw data of length len */
   inline vpr::ReturnStatus writeRaw(vpr::Uint8* data, unsigned len=1);

public:
   std::vector<vpr::Uint8>*   mData;
   unsigned                   mCurHeadPos;
};

/* Write out the single byte.
* @post: data = old(data)+val, mCurHeadPos advaced 1
*/
inline vpr::ReturnStatus ObjectWriter::writeUint8(vpr::Uint8 val)
{
   return writeRaw(&val, 1);
}

inline vpr::ReturnStatus ObjectWriter::writeUint16(vpr::Uint16 val)
{
   vpr::Uint16 nw_val = vpr::System::Htons(val);

   adjust(2);
   return writeRaw((vpr::Uint8*)&nw_val, 2);
}

inline vpr::ReturnStatus ObjectWriter::writeUint32(vpr::Uint32 val)
{
   vpr::Uint32 nw_val = vpr::System::Htonl(val);
   
   adjust(4);
   return writeRaw((vpr::Uint8*)&nw_val, 4);
}

inline vpr::ReturnStatus ObjectWriter::writeUint64(vpr::Uint64 val)
{
   vpr::Uint64 nw_val = vpr::System::Htonll(val);
   
   adjust(8);
   return writeRaw((vpr::Uint8*)&nw_val, 8);
}

inline vpr::ReturnStatus ObjectWriter::writeFloat(float val)
{
   // We are writing the float as a 4 byte value
   BOOST_STATIC_ASSERT(sizeof(float) == 4);
   vpr::Uint32 nw_val = vpr::System::Htonl(*((vpr::Uint32*)&val));
   
   adjust(4);
   return writeRaw((vpr::Uint8*)&nw_val, 4);
}

inline vpr::ReturnStatus ObjectWriter::writeDouble(double val)
{
   // We are writing the double as a 8 byte value
   BOOST_STATIC_ASSERT(sizeof(double) == 8);   
   vpr::Uint64 nw_val = vpr::System::Htonll(*((vpr::Uint64*)&val));
   
   adjust(8);
   return writeRaw((vpr::Uint8*)&nw_val, 8);
}


inline vpr::ReturnStatus ObjectWriter::writeString(std::string val)
{
   for(unsigned i=0; i<val.length();++i)
   {
      writeRaw((vpr::Uint8*)&(val[i]),1);
   }
      
   return vpr::ReturnStatus::Succeed;
}

inline vpr::ReturnStatus ObjectWriter::writeBool(bool val)
{
   return writeRaw((vpr::Uint8*)&val, 1);
}

inline void ObjectWriter::adjust(unsigned d)
{
   if (mCurHeadPos % d != 0)
   {
      for (unsigned i=0;i<( d-(mCurHeadPos%d) );i++)
      {
         mData->push_back(0);
      }
      mCurHeadPos += ( d-(mCurHeadPos%d) );
   }
}

inline vpr::ReturnStatus ObjectWriter::writeRaw(vpr::Uint8* data, unsigned len)
{
   for(unsigned i=0;i<len;++i)
      mData->push_back(data[i]);
   mCurHeadPos += len;
   return vpr::ReturnStatus::Succeed;
}

} // namespace vpr

#endif
