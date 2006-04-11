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
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <vector>
#include <boost/static_assert.hpp>
#include <boost/concept_check.hpp>

#include <vpr/IO/ObjectReader.h>
#include <vpr/Util/Assert.h>
#include <vpr/System.h>


namespace vpr
{

/** Object reader that reads out of a data buffer
*
* @todo: Add smart buffering for type sizes
*/
class BufferObjectReader : public ObjectReader
{
public:
   BufferObjectReader(std::vector<vpr::Uint8>* data, unsigned curPos=0)
   {
      mData = data;
      mCurHeadPos = curPos;
   }

   void setCurPos(unsigned val)
   { mCurHeadPos = val; }
   unsigned getCurPos()
   { return mCurHeadPos; }

   /** @name Tag and attribute handling */
   //@{
   /** Starts a new section/element of name tagName.
   */
   virtual vpr::ReturnStatus beginTag(std::string tagName)
   {
      boost::ignore_unused_variable_warning(tagName);
      return vpr::ReturnStatus::Succeed;
   }

   /** Ends the most recently named tag. */
   virtual vpr::ReturnStatus endTag()
   {
      return vpr::ReturnStatus::Succeed;
   }

   /** Starts an attribute of the name attributeName */
   virtual vpr::ReturnStatus beginAttribute(std::string attributeName)
   {
      boost::ignore_unused_variable_warning(attributeName);
      return vpr::ReturnStatus::Succeed;
   }

   /** Ends the most recently named attribute */
   virtual vpr::ReturnStatus endAttribute()
   {
      return vpr::ReturnStatus::Succeed;
   }
   //@}


   inline virtual vpr::Uint8 readUint8();
   inline virtual vpr::Uint16 readUint16();
   inline virtual vpr::Uint32 readUint32();
   inline virtual vpr::Uint64 readUint64();
   inline virtual float readFloat();
   inline virtual double readDouble();
   inline virtual std::string readString();
   inline virtual bool readBool();

   /* Helper methods */
   virtual void readUint8(vpr::Uint8& val)
   { val = this->readUint8(); }
   virtual void readUint16(vpr::Uint16& val)
   { val = this->readUint16(); }
   virtual void readUint32(vpr::Uint32& val)
   { val = this->readUint32(); }
   virtual void readUint64(vpr::Uint64& val)
   { val = this->readUint64(); }
   virtual void readFloat(float& val)
   { val = this->readFloat(); }
   virtual void readDouble(double& val)
   { val = this->readDouble(); }
   virtual void readString(std::string& str)
   { str = this->readString(); }
   virtual void readBool(bool& val)
   { val = this->readBool(); }

   /* Read raw data of length len
   * POST: Pointer to data returned
   * NOTE: data points to data owned elsewhere.
   * DO NOT MODIFY THE DATA and DO NOT RELY ON THE DATA STAYING THERE LONG.
   */
   inline vpr::Uint8* readRaw(unsigned len=1);

public:
   std::vector<vpr::Uint8>*   mData;
   unsigned                   mCurHeadPos;
};

/* Read out the single byte.
* @post: data = old(data)+val, mCurHeadPos advaced 1
*/
vpr::Uint8 BufferObjectReader::readUint8()
{
   vpr::Uint8 temp_data;
   memcpy(&temp_data, readRaw(1), 1);
   return temp_data;
}

vpr::Uint16 BufferObjectReader::readUint16()
{
   vpr::Uint16 nw_val;
   memcpy(&nw_val, readRaw(2), 2);

   return vpr::System::Ntohs(nw_val);
}

vpr::Uint32 BufferObjectReader::readUint32()
{
   vpr::Uint32 nw_val;
   memcpy(&nw_val, readRaw(4), 4);

   return vpr::System::Ntohl(nw_val);
}

vpr::Uint64 BufferObjectReader::readUint64()
{
   vpr::Uint64 nw_val;
   memcpy(&nw_val, readRaw(8), 8);
   vpr::Uint64 h_val = vpr::System::Ntohll(nw_val);

   return h_val;
}

float BufferObjectReader::readFloat()
{
   // We are reading the float as a 4 byte value
   BOOST_STATIC_ASSERT(sizeof(float) == 4);

   vpr::Uint32 nw_val;
   memcpy(&nw_val, readRaw(4), 4);
   vpr::Uint32 h_val = vpr::System::Ntohl(nw_val);

   return *((float*)&h_val);
}

double BufferObjectReader::readDouble()
{
   // We are reading the double as a 8 byte value
   BOOST_STATIC_ASSERT(sizeof(double) == 8);

   vpr::Uint64 nw_val;
   memcpy(&nw_val, readRaw(8), 8);
   vpr::Uint64 h_val = vpr::System::Ntohll(nw_val);
   double d_val = *((double*)&h_val);

   return d_val;
}


std::string BufferObjectReader::readString()
{
   vpr::Uint16 str_len = readUint16();
   std::string ret_val;
   char tempChar;
   for(unsigned i=0; i<str_len;++i)
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


vpr::Uint8* BufferObjectReader::readRaw(unsigned len)
{
   mCurHeadPos += len;
   vprASSERT((mCurHeadPos-len) < mData->size());

   return &((*mData)[mCurHeadPos-len]);
}

} // namespace vpr

#endif
