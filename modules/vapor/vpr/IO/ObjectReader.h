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

#ifndef _VPR_OBJECT_READER_H
#define _VPR_OBJECT_READER_H

#include <vpr/vprConfig.h>
#include <vpr/Util/AttributeMapBase.h>

#include <vector>

#include <boost/static_assert.hpp>


namespace vpr
{

/** Interface used to read object data to a stream.
*
*/
class ObjectReader : public AttributeMapBase
{
public:
   ObjectReader()
   {;}

   virtual ~ObjectReader()
   {;}


   virtual vpr::Uint8 readUint8() = 0;
   virtual vpr::Uint16 readUint16() = 0;
   virtual vpr::Uint32 readUint32() = 0;
   virtual vpr::Uint64 readUint64() = 0;
   virtual float readFloat() = 0;
   virtual double readDouble() = 0;
   virtual std::string readString(unsigned len) = 0;
   virtual bool readBool() = 0;

   /* Helper methods */
   void readUint8(vpr::Uint8& val);
   void readUint16(vpr::Uint16& val);
   void readUint32(vpr::Uint32& val);
   void readUint64(vpr::Uint64& val);
   void readFloat(float& val);
   void readDouble(double& val);
   void readString(std::string& str, unsigned len);
   void readBool(bool& val);
};

inline void ObjectReader::readUint8(vpr::Uint8& val)
{
   val = readUint8();
}
inline void ObjectReader::readUint16(vpr::Uint16& val)
{
   val = readUint16();
}
inline void ObjectReader::readUint32(vpr::Uint32& val)
{
   val = readUint32();
}
inline void ObjectReader::readUint64(vpr::Uint64& val)
{
   val = readUint64();
}
inline void ObjectReader::readFloat(float& val)
{
   val = readFloat();
}
inline void ObjectReader::readDouble(double& val)
{
   val = readDouble();
}
inline void ObjectReader::readString(std::string& str, unsigned len)
{
   str = readString(len);
}
inline void ObjectReader::readBool(bool& val)
{
   val = readBool();
}

} // namespace vpr

#endif
