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

#ifndef _VPR_STREAM_READER_H
#define _VPR_STREAM_READER_H

#include <vpr/vprConfig.h>
#include <vpr/System.h>

#include <vector>
#include <algorithm>

#include <boost/static_assert.hpp>
#include <vpr/IO/Socket/SocketStream.h>

namespace gadget
{

/** Interface used to read object data to a stream.
*
* @todo: Add smart buffering for type sizes
*/
class StreamReader
{
public:
   StreamReader(std::vector<vpr::Uint8>* data, unsigned curPos=0)
   {
      mData = data;
      mCurHeadPos = curPos;
   }

   void setCurPos(unsigned val)
   { mCurHeadPos = val; }
   unsigned getCurPos()
   { return mCurHeadPos; }
   unsigned getSize()
   { return mData->size(); }
	std::vector<vpr::Uint8>* getDataPtr()
	{ return mData; }

   vpr::Uint8 readUint8();
   vpr::Uint16 readUint16();
   vpr::Uint32 readUint32();
   vpr::Uint64 readUint64();
   float readFloat();
   double readDouble();
   std::string readString(unsigned len);
	std::string readStringUntil(char paramChar);
   bool readBool();

   void readUint8(vpr::Uint8& val);
   void readUint16(vpr::Uint16& val);
   void readUint32(vpr::Uint32& val);
   void readUint64(vpr::Uint64& val);
   void readFloat(float& val);
   void readDouble(double& val);
   void readString(std::string& str, unsigned len);
   void readBool(bool& val);
	bool find(char paramChar);
	bool find(vpr::Uint16 param);

   /* Read raw data of length len
   * POST: Pointer to data returned
   * NOTE: data points to data owned elsewhere.
   * DO NOT MODIFY THE DATA and DO NOT RELY ON THE DATA STAYING THERE LONG.
   */
   inline vpr::Uint8* readRaw(unsigned len=1);
	inline void eraseRaw(unsigned len=1);
	inline int addIncomingData(vpr::SocketStream* socket_stream);

public:
   std::vector<vpr::Uint8>*   mData;
   unsigned                   mCurHeadPos;
};

/* Read out the single byte.
* @post: data = old(data)+val, mCurHeadPos advaced 1
*/
inline vpr::Uint8 StreamReader::readUint8()
{
   vpr::Uint8 nw_val = *(readRaw(1));
	eraseRaw(1);
	return (nw_val);
}

inline vpr::Uint16 StreamReader::readUint16()
{
   vpr::Uint16 nw_val = *((vpr::Uint16*)readRaw(2));
	eraseRaw(2);
   return vpr::System::Ntohs(nw_val);
}

inline vpr::Uint32 StreamReader::readUint32()
{
   vpr::Uint32 nw_val = *((vpr::Uint32*)readRaw(4));
	eraseRaw(4);
   return vpr::System::Ntohl(nw_val);
}

inline vpr::Uint64 StreamReader::readUint64()
{
   vpr::Uint64 nw_val = *((vpr::Uint64*)readRaw(8));
	eraseRaw(8);
   vpr::Uint64 h_val = vpr::System::Ntohll(nw_val);
   return h_val;
}

inline float StreamReader::readFloat()
{
   // We are reading the float as a 4 byte value
   BOOST_STATIC_ASSERT(sizeof(float) == 4);

   vpr::Uint32 nw_val = *((vpr::Uint32*)readRaw(4));
	eraseRaw(4);
   vpr::Uint32 h_val = vpr::System::Ntohl(nw_val);
   return *((float*)&h_val);
}

inline double StreamReader::readDouble()
{
   // We are reading the double as a 8 byte value
   BOOST_STATIC_ASSERT(sizeof(double) == 8);

   vpr::Uint64 nw_val = *((vpr::Uint64*)readRaw(8));
	eraseRaw(8);
   vpr::Uint64 h_val = vpr::System::Ntohll(nw_val);
   double d_val = *((double*)&h_val);
   return d_val;
}


inline std::string StreamReader::readString(unsigned len)
{
   std::string ret_val;

   for(unsigned i=0; i<len;++i)
   {
      ret_val += ((char)(*readRaw(1)));
		eraseRaw(1);
   }

   return ret_val;
}

inline std::string StreamReader::readStringUntil(char paramChar)
{
   char tempChar;
	std::string ret_val;
	
	std::vector<vpr::Uint8>::iterator i;

	for(i=mData->begin();(char)(*i) != paramChar;i++)
	{
		vprASSERT(i != mData->end());
	}

   
	tempChar = (char)(*readRaw(1));
	while( tempChar != paramChar)
   {
      ret_val += tempChar;
      eraseRaw(1);
		tempChar = (char)(*readRaw(1));
   }
	eraseRaw(1);
   return ret_val;
}


inline bool StreamReader::find(char paramChar)
{
	std::vector<vpr::Uint8>::iterator i;

	for(i=mData->begin();i != mData->end();i++)
	{
		
		if((char)(*i) == paramChar)
		{
			return true;
		}
	}
	return false;
}
inline bool StreamReader::find(vpr::Uint16 param)
{
	std::vector<vpr::Uint16>* test = (std::vector<vpr::Uint16>*)mData;

	std::vector<vpr::Uint16>::iterator i;

	for(i=test->begin();i != test->end();i++)
	{
		
		if((*i) == param)
		{
			return true;
		}
	}
	return false;
}

inline bool StreamReader::readBool()
{
   bool nw_val = (bool)*(readRaw(1));
	eraseRaw(1);
	return nw_val;
}

inline void StreamReader::readUint8(vpr::Uint8& val)
{
   val = readUint8();
}
inline void StreamReader::readUint16(vpr::Uint16& val)
{
   val = readUint16();
}
inline void StreamReader::readUint32(vpr::Uint32& val)
{
   val = readUint32();
}
inline void StreamReader::readUint64(vpr::Uint64& val)
{
   val = readUint64();
}
inline void StreamReader::readFloat(float& val)
{
   val = readFloat();
}
inline void StreamReader::readDouble(double& val)
{
   val = readDouble();
}
inline void StreamReader::readString(std::string& str, unsigned len)
{
   str = readString(len);
}
inline void StreamReader::readBool(bool& val)
{
   val = readBool();
}


inline vpr::Uint8* StreamReader::readRaw(unsigned len)
{
   //std::vector<vpr::Uint8>::iterator i;
	vpr::Uint8* tempPtr;
	//mCurHeadPos += len;
    vprASSERT(len <= mData->size());
	tempPtr=&((*mData)[0]);
   
	return tempPtr;
}
inline void StreamReader::eraseRaw(unsigned len)
{
   
	// if(len==1)
	//{
	//	vprASSERT(!mData->empty());
	//	mData->pop_front();
	//}
	//else if (len > 1)
	//{
		std::vector<vpr::Uint8>::iterator i;
		//vpr::Uint8* tempPtr;

		vprASSERT(len <= mData->size());
   
		i=mData->begin();
		for(unsigned j =0; j<len;j++)
		{
				i++;
		}
	
		i = mData->erase(mData->begin(),i);
   //}
   return;
}

inline int StreamReader::addIncomingData(vpr::SocketStream* socket_stream)
{
	//std::vector<vpr::Uint8>* tempVector = new std::vector<vpr::Uint8>;
	std::vector<vpr::Uint8> tempVector(1000);
	//std::vector<vpr::Uint8>::iterator i;

   vpr::Uint32 length = 100;
	vpr::Uint32 bytes_read;
	
   if (socket_stream->recv(tempVector, length, bytes_read , vpr::Interval(1,vpr::Interval::Msec))==vpr::ReturnStatus::Succeed)
	{
      //std::cout << "Yippy, we are loading something from the nedwork, actually: " << bytes_read << " bytes." << std::endl;
		//std::cout << "Yippy, we are loading something from the nedwork, actually: " << tempVector.size() << " bytes." << std::endl;
		//for(unsigned i=0;i<tempvector.size();i++)
		//{
		//	std::cout << (int)tempvector[i] << "|";
		//}
		//std::cout << std::endl;
		mData->insert(mData->end(),tempVector.begin(),tempVector.end());
		//tempVector.insert(tempVector.begin(),mData->begin(),mData->end());
		//delete mData;
		//mData = &tempVector;
      return(bytes_read);
	}
   return 0;
   
}

} // namespace vpr

#endif
