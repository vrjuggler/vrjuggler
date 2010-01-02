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
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
class VPR_CLASS_API BufferObjectReader : public ObjectReader
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

   /** Destructor.
    * @throws nothing
    */
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
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void beginTag(const std::string& tagName)
   {
      boost::ignore_unused_variable_warning(tagName);
   }

   /**
    * Ends the most recently named tag.
    *
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void endTag()
   {;}

   /**
    * Starts an attribute of the name \p attributeName.
    *
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void beginAttribute(const std::string& attributeName)
   {
      boost::ignore_unused_variable_warning(attributeName);
   }

   /**
    * Ends the most recently named attribute.
    *
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void endAttribute()
   {;}
   //@}

   /**
    * Reads out the single byte.
    * @post data = old(data)+val, \c mCurHeadPos advanced 1.
    *
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   inline virtual vpr::Uint8 readUint8();

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   inline virtual vpr::Uint16 readUint16();

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   inline virtual vpr::Uint32 readUint32();

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   inline virtual vpr::Uint64 readUint64();

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   inline virtual float readFloat();

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   inline virtual double readDouble();

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   inline virtual std::string readString();

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   inline virtual bool readBool();

   /** @name Helper methods */
   //@{

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readUint8(vpr::Uint8& val)
   {
      val = this->readUint8();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readUint16(vpr::Uint16& val)
   {
      val = this->readUint16();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readUint32(vpr::Uint32& val)
   {
      val = this->readUint32();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readUint64(vpr::Uint64& val)
   {
      val = this->readUint64();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readFloat(float& val)
   {
      val = this->readFloat();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readDouble(double& val)
   {
      val = this->readDouble();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readString(std::string& str)
   {
      str = this->readString();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
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
    * @note data points to data owned elsewhere.
    *       DO NOT MODIFY THE DATA and DO NOT RELY ON THE DATA STAYING THERE
    *       LONG.
    * @throws vpr::IOException if the operation failed.
    */
   inline vpr::Uint8* readRaw(const unsigned int len = 1);

public:
   std::vector<vpr::Uint8>*   mData;
   unsigned int               mCurHeadPos;
   std::vector<unsigned int>  mHeadPosStateStack;  /**< Store pushed and popped state information */
};

inline vpr::Uint8 BufferObjectReader::readUint8()
{
   vpr::Uint8 temp_data;
   std::memcpy(&temp_data, readRaw(1), 1);
   return temp_data;
}

inline vpr::Uint16 BufferObjectReader::readUint16()
{
   vpr::Uint16 nw_val;
   std::memcpy(&nw_val, readRaw(2), 2);

   return vpr::System::Ntohs(nw_val);
}

inline vpr::Uint32 BufferObjectReader::readUint32()
{
   vpr::Uint32 nw_val;
   std::memcpy(&nw_val, readRaw(4), 4);

   return vpr::System::Ntohl(nw_val);
}

inline vpr::Uint64 BufferObjectReader::readUint64()
{
   vpr::Uint64 nw_val;
   std::memcpy(&nw_val, readRaw(8), 8);
   vpr::Uint64 h_val = vpr::System::Ntohll(nw_val);

   return h_val;
}

inline float BufferObjectReader::readFloat()
{
   // We are reading the float as a 4 byte value
   BOOST_STATIC_ASSERT(sizeof(float) == 4);

   vpr::Uint32 nw_val;
   std::memcpy(&nw_val, readRaw(4), 4);
   vpr::Uint32 h_val = vpr::System::Ntohl(nw_val);

   return *((float*)&h_val);
}

inline double BufferObjectReader::readDouble()
{
   // We are reading the double as a 8 byte value
   BOOST_STATIC_ASSERT(sizeof(double) == 8);

   vpr::Uint64 nw_val;
   std::memcpy(&nw_val, readRaw(8), 8);
   vpr::Uint64 h_val = vpr::System::Ntohll(nw_val);
   double d_val = *((double*)&h_val);

   return d_val;
}

inline std::string BufferObjectReader::readString()
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

inline bool BufferObjectReader::readBool()
{
   return (bool)*(readRaw(1));
}

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
