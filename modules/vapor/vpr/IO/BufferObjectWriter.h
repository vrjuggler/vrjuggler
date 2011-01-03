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
   /**
    * Number of bytes used to store the size of the string.
    *
    * @since 1.1.15
    */
   static const unsigned int STRING_LENGTH_SIZE;

   /** Constructor.
    * Build writer and point it at a fresh data buffer.
    * This data buffer will be owned exclusively by this object
    * and will be deleted on destruction.
    */
   BufferObjectWriter();

   /** Constructor.
    * Build writer and point it at an existing data buffer.
    * This buffer will not be owned or destroyed by the writer.
    */
   BufferObjectWriter(std::vector<vpr::Uint8>* data,
                      const unsigned int curPos = 0);

   virtual ~BufferObjectWriter();

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
   /**
    * Starts a new section/element of name \p tagName.
    *
    * @throw IOException If I/O errors occur while writing to the underlying
    *                    data source.
    */
   virtual void beginTag(const std::string& tagName)
   {
      boost::ignore_unused_variable_warning(tagName);
   }

   /**
    * Ends the most recently named tag.
    *
    * @throw IOException If I/O errors occur while writing to the underlying
    *                    data source.
    */
   virtual void endTag()
   {
      /* Do nothing. */ ;
   }

   /**
    * Starts an attribute of the name \p attributeName.
    *
    * @throw IOException If I/O errors occur while writing to the underlying
    *                    data source.
    */
   virtual void beginAttribute(const std::string& attributeName)
   {
      boost::ignore_unused_variable_warning(attributeName);
   }

   /**
    * Ends the most recently named attribute.
    *
    * @throw IOException If I/O errors occur while writing to the underlying
    *                    data source.
    */
   virtual void endAttribute()
   {
      /* Do nothing. */ ;
   }
   //@}

   /**
    * Writes out the single byte.
    * @post data = old(data)+val, \c mCurHeadPos advanced 1.
    *
    * @throw IOException If I/O errors occur while writing to the underlying
    *                    data source.
    */
   virtual void writeUint8(vpr::Uint8 val);

   /**
    * @throw IOException If I/O errors occur while writing to the underlying
    *                    data source.
    */
   virtual void writeUint16(vpr::Uint16 val);

   /**
    * @throw IOException If I/O errors occur while writing to the underlying
    *                    data source.
    */
   virtual void writeUint32(vpr::Uint32 val);

   /**
    * @throw IOException If I/O errors occur while writing to the underlying
    *                    data source.
    */
   virtual void writeUint64(vpr::Uint64 val);

   /**
    * @throw IOException If I/O errors occur while writing to the underlying
    *                    data source.
    */
   virtual void writeFloat(float val);

   /**
    * @throw IOException If I/O errors occur while writing to the underlying
    *                    data source.
    */
   virtual void writeDouble(double val);

   /**
    * @throw IOException If I/O errors occur while writing to the underlying
    *                    data source.
    */
   virtual void writeString(std::string val);

   /**
    * @throw IOException If I/O errors occur while writing to the underlying
    *                    data source.
    */
   virtual void writeBool(bool val);

   /**
    * Writes raw data of length \p len.
    * @throws vpr::IOException if the operation failed.
    */
   inline void writeRaw(vpr::Uint8* data,
                        const unsigned int len = 1);

private:
   /**
    * Do not allow copy.  To do so we would need to make the memory handling
    * much smarter.
    */
   BufferObjectWriter(const BufferObjectWriter& rhs)
      : vpr::ObjectWriter(rhs)
   {
      /* Do nothihng. */ ;
   }

public:
   bool                       mOwnDataBuffer;      /**< If true we allocated the data buffer and should delete it. */
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

   writeRaw((vpr::Uint8*) &nw_val, 2);
}

inline void BufferObjectWriter::writeUint32(vpr::Uint32 val)
{
   vpr::Uint32 nw_val = vpr::System::Htonl(val);

   writeRaw((vpr::Uint8*) &nw_val, 4);
}

inline void BufferObjectWriter::writeUint64(vpr::Uint64 val)
{
   vpr::Uint64 nw_val = vpr::System::Htonll(val);

   writeRaw((vpr::Uint8*) &nw_val, 8);
}

inline void BufferObjectWriter::writeFloat(float val)
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

inline void BufferObjectWriter::writeDouble(double val)
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

inline void BufferObjectWriter::writeString(std::string val)
{
   // Note: If you change this, you need to change STRING_LENGTH_SIZE
   writeUint32(val.size());

   for ( vpr::Uint32 i = 0; i < val.length(); ++i )
   {
      writeRaw((vpr::Uint8*) &val[i], 1);
   }
}

inline void BufferObjectWriter::writeBool(bool val)
{
   // Darwin uses four bytes (!) for bools.
#ifdef VPR_OS_Darwin
   vpr::Uint8 temp = (vpr::Uint8) val;
   writeRaw((vpr::Uint8*) &temp, 1);
#else
   writeRaw((vpr::Uint8*) &val, 1);
#endif
}

inline void BufferObjectWriter::writeRaw(vpr::Uint8* data,
                                         const unsigned int len)
{
   for ( unsigned int i = 0; i < len; ++i )
   {
      mData->push_back(data[i]);
   }

   mCurHeadPos += len;
}

} // namespace vpr

#endif
