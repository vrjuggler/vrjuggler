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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _VPR_BUFFER_OBJECT_WRITER_H
#define _VPR_BUFFER_OBJECT_WRITER_H

#include <vpr/vprConfig.h>
#include <vector>
#include <boost/static_assert.hpp>
#include <boost/concept_check.hpp>

#include <vpr/System.h>
#include <vpr/IO/ObjectWriter.h>

namespace vpr
{

/** \class BufferObjectWriter BufferObjectWriter.h vpr/IO/BufferObjectWriter.h
 *
 * Object writer for data buffers.  Writes directly to a data buffer.
 *
 * @todo Add smart buffering for type sizes.
 */
class VPR_CLASS_API BufferObjectWriter : public ObjectWriter
{
public:
   BufferObjectWriter();

   BufferObjectWriter(std::vector<vpr::Uint8>* data,
                      const unsigned int curPos = 0);

   void setCurPos(unsigned int val)
   {
      mCurHeadPos = val;
   }

   unsigned int getCurPos()
   {
      return mCurHeadPos;
   }

   std::vector<vpr::Uint8>* getData()
   {
      return mData;
   }

   /** @name Tag and attribute handling */
   //@{
   /** Starts a new section/element of name \p tagName. */
   virtual void beginTag(const std::string& tagName)
   {
      boost::ignore_unused_variable_warning(tagName);
   }
   
   /** Ends the most recently named tag. */
   virtual void endTag()
   {;}

   /** Starts an attribute of the name \p attributeName. */
   virtual void beginAttribute(const std::string& attributeName)
   {
      boost::ignore_unused_variable_warning(attributeName);
   }

   /** Ends the most recently named attribute. */
   virtual void endAttribute()
   {;}
   //@}

   /**
    * Writes out the single byte.
    * @post data = old(data)+val, \c mCurHeadPos advaced 1
    */
   virtual void writeUint8(vpr::Uint8 val);
   virtual void writeUint16(vpr::Uint16 val);
   virtual void writeUint32(vpr::Uint32 val);
   virtual void writeUint64(vpr::Uint64 val);
   virtual void writeFloat(float val);
   virtual void writeDouble(double val);
   virtual void writeString(std::string val);
   virtual void writeBool(bool val);

   /* Writes raw data of length \p len. */
   inline void writeRaw(vpr::Uint8* data,
                        const unsigned int len = 1);

public:
   std::vector<vpr::Uint8>*   mData;
   unsigned int               mCurHeadPos;
};

inline void BufferObjectWriter::writeUint8(vpr::Uint8 val)
{
   writeRaw(&val, 1);
}

inline void BufferObjectWriter::writeUint16(vpr::Uint16 val)
{
   vpr::Uint16 nw_val = vpr::System::Htons(val);

   writeRaw((vpr::Uint8*)&nw_val, 2);
}

inline void BufferObjectWriter::writeUint32(vpr::Uint32 val)
{
   vpr::Uint32 nw_val = vpr::System::Htonl(val);

   writeRaw((vpr::Uint8*)&nw_val, 4);
}

inline void BufferObjectWriter::writeUint64(vpr::Uint64 val)
{
   vpr::Uint64 nw_val = vpr::System::Htonll(val);

   writeRaw((vpr::Uint8*)&nw_val, 8);
}

inline void BufferObjectWriter::writeFloat(float val)
{
   // We are writing the float as a 4 byte value
   BOOST_STATIC_ASSERT(sizeof(float) == 4);
   vpr::Uint32 nw_val = vpr::System::Htonl(*((vpr::Uint32*)&val));

   writeRaw((vpr::Uint8*)&nw_val, 4);
}

inline void BufferObjectWriter::writeDouble(double val)
{
   // We are writing the double as a 8 byte value
   BOOST_STATIC_ASSERT(sizeof(double) == 8);
   vpr::Uint64 nw_val = vpr::System::Htonll(*((vpr::Uint64*)&val));

   writeRaw((vpr::Uint8*)&nw_val, 8);
}

inline void BufferObjectWriter::writeString(std::string val)
{
   writeUint16(val.size());
   for(unsigned i=0; i<val.length();++i)
   {
      writeRaw((vpr::Uint8*)&(val[i]),1);
   }

}

inline void BufferObjectWriter::writeBool(bool val)
{
   // Darwin uses four bytes (!) for bools.
#ifdef VPR_OS_Darwin
   vpr::Uint8 temp = (vpr::Uint8) val;
   writeRaw((vpr::Uint8*)&temp, 1);
#else
   writeRaw((vpr::Uint8*)&val, 1);
#endif
}

inline void BufferObjectWriter::writeRaw(vpr::Uint8* data,
                                                      const unsigned int len)
{
   for(unsigned i=0;i<len;++i)
      mData->push_back(data[i]);
   mCurHeadPos += len;
}

} // namespace vpr

#endif
