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

#ifndef _VPR_STREAM_WRITER_H
#define _VPR_STREAM_WRITER_H

#include <vpr/vprConfig.h>
#include <vpr/System.h>

#include <vector>

#include <boost/static_assert.hpp>
#include <vpr/IO/Socket/SocketStream.h>


namespace gadget
{

/** Interface used to write object data to a stream.
*
* @todo: Add smart buffering for type sizes
*/
class StreamWriter
{
public:
   StreamWriter()
   {
      mData = new std::vector<vpr::Uint8>;
      mCurHeadPos = 0;
   }

   StreamWriter(std::vector<vpr::Uint8>* data, unsigned curPos=0)
   {
      mData = data;
      mCurHeadPos = curPos;
   }

   void setCurPos(unsigned val)
   { mCurHeadPos = val; }
   unsigned getCurPos()
   { return mCurHeadPos; }

	std::vector<vpr::Uint8>* getDataPtr()
	{ return mData; }


   vpr::ReturnStatus writeUint8(vpr::Uint8 val);
   vpr::ReturnStatus writeUint16(vpr::Uint16 val);
   vpr::ReturnStatus writeUint32(vpr::Uint32 val);
   vpr::ReturnStatus writeUint64(vpr::Uint64 val);
   vpr::ReturnStatus writeFloat(float val);
   vpr::ReturnStatus writeDouble(double val);
   vpr::ReturnStatus writeString(std::string val);
   vpr::ReturnStatus writeBool(bool val);
   
   /* Write raw data of length len */
   inline vpr::ReturnStatus writeRaw(vpr::Uint8* data, unsigned len=1);

	inline vpr::ReturnStatus StreamWriter::sendAtOnce(vpr::SocketStream& sock_stream);
	inline vpr::ReturnStatus StreamWriter::clearBuffer();
	inline vpr::ReturnStatus StreamWriter::sendAllAndClear(vpr::SocketStream& sock_stream);
public:
   std::vector<vpr::Uint8>*   mData;
   unsigned                   mCurHeadPos;
};

/* Write out the single byte.
* @post: data = old(data)+val, mCurHeadPos advaced 1
*/
inline vpr::ReturnStatus StreamWriter::writeUint8(vpr::Uint8 val)
{
   return writeRaw(&val, 1);
}

inline vpr::ReturnStatus StreamWriter::writeUint16(vpr::Uint16 val)
{
   vpr::Uint16 nw_val = vpr::System::Htons(val);
   return writeRaw((vpr::Uint8*)&nw_val, 2);
}

inline vpr::ReturnStatus StreamWriter::writeUint32(vpr::Uint32 val)
{
   vpr::Uint32 nw_val = vpr::System::Htonl(val);
   return writeRaw((vpr::Uint8*)&nw_val, 4);
}

inline vpr::ReturnStatus StreamWriter::writeUint64(vpr::Uint64 val)
{
   vpr::Uint64 nw_val = vpr::System::Htonll(val);
   return writeRaw((vpr::Uint8*)&nw_val, 8);
}

inline vpr::ReturnStatus StreamWriter::writeFloat(float val)
{
   // We are writing the float as a 4 byte value
   BOOST_STATIC_ASSERT(sizeof(float) == 4);
   vpr::Uint32 nw_val = vpr::System::Htonl(*((vpr::Uint32*)&val));
   return writeRaw((vpr::Uint8*)&nw_val, 4);
}

inline vpr::ReturnStatus StreamWriter::writeDouble(double val)
{
   // We are writing the double as a 8 byte value
   BOOST_STATIC_ASSERT(sizeof(double) == 8);   
   vpr::Uint64 nw_val = vpr::System::Htonll(*((vpr::Uint64*)&val));
   return writeRaw((vpr::Uint8*)&nw_val, 8);
}


inline vpr::ReturnStatus StreamWriter::writeString(std::string val)
{
   for(unsigned i=0; i<val.length();++i)
      writeRaw((vpr::Uint8*)&(val[i]),1);
   return vpr::ReturnStatus::Succeed;
}

inline vpr::ReturnStatus StreamWriter::writeBool(bool val)
{
   return writeRaw((vpr::Uint8*)&val, 1);
}

inline vpr::ReturnStatus StreamWriter::writeRaw(vpr::Uint8* data, unsigned len)
{
   for(unsigned i=0;i<len;++i)
      mData->push_back(data[i]);
   
   mCurHeadPos += len;
   return vpr::ReturnStatus::Succeed;
}
inline vpr::ReturnStatus StreamWriter::sendAllAndClear(vpr::SocketStream& sock_stream)
{
   sendAtOnce(sock_stream);  // transmit data
   this->clearBuffer();
   return vpr::ReturnStatus::Succeed;
}

inline vpr::ReturnStatus StreamWriter::clearBuffer()
{
   mData->clear();
	return(vpr::ReturnStatus::Succeed);
}

inline vpr::ReturnStatus StreamWriter::sendAtOnce(vpr::SocketStream& sock_stream)
{
   //vpr::Uint32 total_bytes_sent = 0;
   vpr::Uint32 bytes_just_sent = 0;
   //vpr::Uint32 bytes_to_send = bytes_to_send_param;
	
	std::vector<vpr::Uint8>::iterator i;
   
   while ( !mData->empty() )
   {
      sock_stream.send(*mData, mData->size(), bytes_just_sent);
		i=mData->begin();
		
		for(unsigned j =0; j<bytes_just_sent;j++)
		{
			i++;
		}

		i = mData->erase(mData->begin(),i);

      //std::vector<std::string> test;
		//test.
   }		 
   return vpr::ReturnStatus::Succeed;
}


} // namespace vpr

#endif
