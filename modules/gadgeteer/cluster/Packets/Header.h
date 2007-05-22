/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _CLUSTER_HEADER_H
#define _CLUSTER_HEADER_H

#include <gadget/gadgetConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

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
   Header();

   Header( const vpr::Uint16 code, const vpr::Uint16 type,
           const vpr::Uint32 length, const vpr::Uint32 frame );

   virtual ~Header()
   {;}


   /**
    * Reads the packet header from the given socket.
    *
    * @pre \p stream is not NULL.
    * @post The packet header will be read from \p stream. If reading from
    *       \p stream fails, then \p stream is closed.
    *
    * @param stream The socket from which the header will be read.
    * 
    * @throw cluster::ClusterException is thrown if \p stream is NULL or if
    *        the bytes read from \p stream is not equal to
    *        \c RIM_PACKET_HEADER_SIZE.
    * @throw vpr::IOException is thrown if an I/O error occurs when trying
    *        to read from \p stream.
    */
   void readData(vpr::SocketStream* stream);

   void serializeHeader();

   void parseHeader();

   /**
    * Writes the packet header data to the given socket.
    *
    * @pre \p socket is not NULL.
    * @post The contents of \c mData (which is \c RIM_PACKET_HEADER_SIZE bytes
    *       long) are written to \p socket.
    *
    * @throw cluster::ClusterException is thrown if the packet header cannot
    *        be written to \p socket.
    */
   void send(vpr::SocketStream* socket);

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
   std::vector<vpr::Uint8> mData;

   vpr::Uint16 mRIMCode;
   vpr::Uint16 mPacketType;
   vpr::Uint32 mPacketLength;
   vpr::Uint32 mFrame;
};

} // End namespace cluster.

#endif /* _CLUSTER_HEADER_H */
