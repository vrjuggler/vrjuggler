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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_RIM_HEADER_H
#define _GADGET_RIM_HEADER_H

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/Util/ReturnStatus.h>

#include <gadget/gadgetConfig.h>
#include <cluster/ClusterException.h>

namespace cluster
{

class Header
{
public:
   const static unsigned short RIM_PACKET 		   = 400;
   const static unsigned short RIM_DATA_PACKET 	   = 401;
   const static unsigned short RIM_DEVICE_REQ 	   = 402;
   const static unsigned short RIM_DEVICE_ACK 	   = 403;
   const static unsigned short RIM_CONNECTION_REQ  = 404;
   const static unsigned short RIM_CONNECTION_ACK  = 405;
   const static unsigned short RIM_SYNC_REQ        = 406;
   const static unsigned short RIM_SYNC_ACK        = 407;
   const static unsigned short RIM_APPDATA_REQ     = 408;
   const static unsigned short RIM_APPDATA_ACK     = 409;
   const static unsigned short RIM_END_BLOCK 	   = 410;
   const static unsigned short RIM_START_BLOCK 	   = 411;

   //const unsigned short MSG_DEVICE_NACK 	= 404;
   //const unsigned short MSG_CLOCK_SRC 	= 405;
   //const unsigned short MSG_CLOCK_SYNC 	= 406;
   //const unsigned short MSG_HANDSHAKE 	= 407;
   //const unsigned short MSG_CLUSTER_SYNC	= 408;
   //const unsigned short MSG_BARRIER	= 5;
   const static unsigned short RIM_PACKET_HEAD_SIZE = 12;

public:      
   /**
    * Directly read the needed header data from socket(blocking), and parse the header 
    */
   Header(vpr::SocketStream* stream) throw(cluster::ClusterException);

   Header(vpr::Uint16 RIM_code, vpr::Uint16 packet_type, 
             vpr::Uint32 packet_length, vpr::Uint32 frame);


   virtual ~Header()
   {
      delete mPacketReader;
	  delete mPacketWriter;
	  //delete mData;
   }
   void serializeHeader();
   void parseHeader();
   vpr::ReturnStatus send(vpr::SocketStream* socket);
   void dump();
    //virtual bool action(ClusterNode* node);

   vpr::Uint16 getRIMCode() { return mRIMCode; }
   vpr::Uint16 getPacketType() { return mPacketType; }
   vpr::Uint32 getPacketLength() { return mPacketLength; }
   void setPacketLength(vpr::Uint32 length) { mPacketLength = length; }
   vpr::Uint32 getFrame() { return mFrame; }
   virtual void printData(int debug_level);
protected:
   // Needed to construct and parse the header data
   vpr::BufferObjectReader* mPacketReader;	
   vpr::BufferObjectWriter* mPacketWriter;
   std::vector<vpr::Uint8> mData;
   
   vpr::Uint16 mRIMCode;
   vpr::Uint16 mPacketType;
   vpr::Uint32 mPacketLength;
   vpr::Uint32 mFrame;
};
}

#endif
