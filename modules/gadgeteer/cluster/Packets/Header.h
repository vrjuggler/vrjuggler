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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _CLUSTER_HEADER_H
#define _CLUSTER_HEADER_H

#include <gadget/gadgetConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/Util/ReturnStatus.h>

#include <cluster/ClusterException.h>

namespace cluster
{

/** \class Header Header.h cluster/Packets/Header.h
 *
 * Cluster packet header block.
 */
class GADGET_CLASS_API Header
{
public:
   static const unsigned short RIM_PACKET          = 400;
   static const unsigned short RIM_DATA_PACKET     = 401;
   static const unsigned short RIM_DEVICE_REQ      = 402;
   static const unsigned short RIM_DEVICE_ACK      = 403;
   static const unsigned short RIM_CONNECTION_REQ  = 404;
   static const unsigned short RIM_CONNECTION_ACK  = 405;
   static const unsigned short RIM_SYNC_REQ        = 406;
   static const unsigned short RIM_SYNC_ACK        = 407;
   static const unsigned short RIM_APPDATA_REQ     = 408;
   static const unsigned short RIM_APPDATA_ACK     = 409;
   static const unsigned short RIM_END_BLOCK       = 410;
   static const unsigned short RIM_START_BLOCK     = 411;
   static const unsigned short RIM_PACKET_HEAD_SIZE = 12;

public:
   /**
    * Directly read the needed header data from socket(blocking), and parse the
    * header.
    */
   Header() : mPacketReader(NULL), mPacketWriter(NULL)
   {;}

   void readData( vpr::SocketStream* stream ) throw( cluster::ClusterException );

   Header( vpr::Uint16 RIM_code, vpr::Uint16 packet_type,
           vpr::Uint32 packet_length, vpr::Uint32 frame );

   virtual ~Header()
   {
      if ( NULL != mPacketReader )
      {
         delete mPacketReader;
         mPacketReader = NULL;
      }
      if ( NULL != mPacketWriter )
      {
         delete mPacketWriter;
         mPacketWriter = NULL;
      }
   }

   void serializeHeader();

   void parseHeader();

   vpr::ReturnStatus send( vpr::SocketStream* socket );

   void dump();

   vpr::Uint16 getRIMCode()
   {
      return mRIMCode;
   }

   vpr::Uint16 getPacketType()
   {
      return mPacketType;
   }

   vpr::Uint32 getPacketLength()
   {
      return mPacketLength;
   }

   void setPacketLength( const vpr::Uint32 length )
   {
      mPacketLength = length;
   }

   vpr::Uint32 getFrame()
   {
      return mFrame;
   }

   virtual void printData( const int debug_level );
protected:
   vpr::BufferObjectReader* mPacketReader;
   vpr::BufferObjectWriter* mPacketWriter;
   std::vector<vpr::Uint8> mData;

   vpr::Uint16 mRIMCode;
   vpr::Uint16 mPacketType;
   vpr::Uint32 mPacketLength;
   vpr::Uint32 mFrame;
};

} // End namespace cluster.

#endif /* _CLUSTER_HEADER_H */
