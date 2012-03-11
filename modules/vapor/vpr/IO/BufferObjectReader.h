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

#ifndef _VPR_BUFFER_OBJECT_READER_H
#define _VPR_BUFFER_OBJECT_READER_H

#include <vpr/vprConfig.h>

#include <cstring>
#include <vector>
#include <boost/static_assert.hpp>
#include <boost/concept_check.hpp>

#include <vpr/IO/EOFException.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/Util/Assert.h>
#include <vpr/System.h>


namespace vpr
{

/** \class BufferObjectReader BufferObjectReader.h vpr/IO/BufferObjectReader.h
 *
 * Object reader that reads out of a data buffer.
 *
 * @todo Add smart buffering for type sizes.
 */
class VPR_API BufferObjectReader : public ObjectReader
{
public:
   /**
    * Number of bytes used to store the size of the string.
    *
    * @since 1.1.15
    */
   static const unsigned int STRING_LENGTH_SIZE;

   BufferObjectReader(std::vector<vpr::Uint8>* data,
                      const unsigned int curPos = 0);

   /** Destructor. */
   virtual ~BufferObjectReader();

   void setCurPos(const unsigned int val)
   {
      mCurHeadPos = val;
   }

   /**
    * @since 1.1.15
    */
   std::vector<vpr::Uint8*>::size_type getSize()
   {
      return mData->size();
   }

   unsigned int getCurPos()
   {
      return mCurHeadPos;
   }

   /** Reset the reading head to the start position. */
   virtual void resetReading()
   {
      setCurPos(0);
   }

   virtual void pushState()
   {
      mHeadPosStateStack.push_back(mCurHeadPos);
   }

   virtual void popState()
   {
      unsigned int new_head_pos = mHeadPosStateStack.back();
      mHeadPosStateStack.pop_back();
      setCurPos(new_head_pos);
   }

   /** @name Tag and attribute handling */
   //@{
   /**
    * Starts a new section/element of name \p tagName.
    *
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void beginTag(const std::string& tagName)
   {
      boost::ignore_unused_variable_warning(tagName);
   }

   /**
    * Ends the most recently named tag.
    *
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void endTag()
   {;}

   /**
    * Starts an attribute of the name \p attributeName.
    *
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void beginAttribute(const std::string& attributeName)
   {
      boost::ignore_unused_variable_warning(attributeName);
   }

   /**
    * Ends the most recently named attribute.
    *
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void endAttribute()
   {;}
   //@}

   /**
    * Reads out the single byte.
    *
    * @post data = old(data)+val, \c mCurHeadPos advanced 1.
    *
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual vpr::Uint8 readUint8();

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual vpr::Uint16 readUint16();

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual vpr::Uint32 readUint32();

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual vpr::Uint64 readUint64();

   /**
    * Reads out the single byte.
    *
    * @post data = old(data)+val, \c mCurHeadPos advanced 1.
    *
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual vpr::Int8 readInt8();

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual vpr::Int16 readInt16();

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual vpr::Int32 readInt32();

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual vpr::Int64 readInt64();

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual float readFloat();

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual double readDouble();

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual std::string readString();

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual bool readBool();

   /** @name Helper methods */
   //@{

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void readUint8(vpr::Uint8& val)
   {
      val = this->readUint8();
   }

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void readUint16(vpr::Uint16& val)
   {
      val = this->readUint16();
   }

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void readUint32(vpr::Uint32& val)
   {
      val = this->readUint32();
   }

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void readUint64(vpr::Uint64& val)
   {
      val = this->readUint64();
   }

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void readInt8(vpr::Int8& val)
   {
      val = this->readInt8();
   }

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void readInt16(vpr::Int16& val)
   {
      val = this->readInt16();
   }

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void readInt32(vpr::Int32& val)
   {
      val = this->readInt32();
   }

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void readInt64(vpr::Int64& val)
   {
      val = this->readInt64();
   }

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void readFloat(float& val)
   {
      val = this->readFloat();
   }

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void readDouble(double& val)
   {
      val = this->readDouble();
   }

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void readString(std::string& str)
   {
      str = this->readString();
   }

   /**
    * @throw EOFException Thrown if end of file is reached while reading.
    * @throw IOException  Thrown if some other I/O error occurs while reading
    *                     from the underlying data source.
    */
   virtual void readBool(bool& val)
   {
      val = this->readBool();
   }
   //@}

   /**
    * Reads raw data of length \p len.
    *
    * @post Pointer to data returned.
    *
    * @note data points to data owned elsewhere.
    *       DO NOT MODIFY THE DATA and DO NOT RELY ON THE DATA STAYING THERE
    *       LONG.
    *
    * @throw vpr::IOException THrown if the operation failed.
    */
   inline vpr::Uint8* readRaw(const unsigned int len = 1);

public:
   std::vector<vpr::Uint8>*   mData;
   unsigned int               mCurHeadPos;
   std::vector<unsigned int>  mHeadPosStateStack;  /**< Store pushed and popped state information */
};

inline vpr::Uint8* BufferObjectReader::readRaw(const unsigned int len)
{
   if ( mCurHeadPos >= mData->size() )
   {
      throw EOFException("Attempted to read beyond data block", VPR_LOCATION);
   }

   mCurHeadPos += len;

   return &((*mData)[mCurHeadPos-len]);
}

} // namespace vpr


#endif
