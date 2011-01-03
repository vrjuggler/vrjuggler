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

#include <cluster/Packets/Header.h>
#include <gadget/Util/Debug.h>

#include <vpr/IO/Socket/SocketStream.h>

namespace cluster
{

Header::Header()
{;}

Header::Header( const vpr::Uint16 code, const vpr::Uint16 type,
                const vpr::Uint32 length, const vpr::Uint32 frame )
   : mRIMCode(code)
   , mPacketType(type)
   , mPacketLength(length)
   , mFrame(frame)
{;}

HeaderPtr Header::create()
{
   return HeaderPtr(new Header());
}

HeaderPtr Header::create(const vpr::Uint16 code, const vpr::Uint16 type,
                         const vpr::Uint32 length, const vpr::Uint32 frame)
{
   return HeaderPtr(new Header(code, type, length, frame));
}

void Header::readData(vpr::SocketStream* stream, bool dumpHeader)
{
   vprASSERT( NULL != stream && "Can not create a Header using a NULL SocketStream" );

   // - Is stream is a valid SocketStream?
   //   - Read in the packet from the socket
   //   - Set the BufferObjectReader and BufferObjectWriter to use mData  <====We only need BufferObjectReader
   if (NULL == stream)
   {
      vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
         << clrOutBOLD( clrRED, "ERROR:" )
         << " SocketSteam is NULL"
         << std::endl << vprDEBUG_FLUSH;

      throw cluster::ClusterException("Header::Header() - SocketStream is NULL");
   }

   vpr::Uint32 bytes_read;   
   std::vector<vpr::Uint8> header_data; 
   try
   {
      bytes_read = stream->readn(header_data, Header::RIM_PACKET_HEAD_SIZE);
   }
   catch (vpr::IOException& ex)
   {
      vprDEBUG( gadgetDBG_RIM, vprDBG_CRITICAL_LVL )
         << "Header::readData() - Could not read the header!\n"
         << ex.what()
         << std::endl << vprDEBUG_FLUSH;
      throw ex;
   }

   vprASSERT( RIM_PACKET_HEAD_SIZE == bytes_read && "Header::Header() - Bytes read != RIM_PACKET_HEAD_SIZE" );

   parseHeader(header_data);

   if(dumpHeader)
   {
      std::cout << "Dumping Header(" << header_data.size() << " bytes): ";
      for ( std::vector<vpr::Uint8>::const_iterator i = header_data.begin();
            i != header_data.end(); i++ )
      {
         std::cout << (int)*i << " ";
      }
      std::cout << std::endl;
   }
}

void Header::prependSerializedHeader(vpr::BufferObjectWriter* packetWriter)
{
   // Set Packet length
   setPacketLength( packetWriter->getData()->size() + RIM_PACKET_HEAD_SIZE );

   std::vector<vpr::Uint8> header_data;
   vpr::BufferObjectWriter writer( &header_data );

   // Serialize all header data.
   writer.writeUint16( mRIMCode );
   writer.writeUint16( mPacketType );
   writer.writeUint32( mFrame );
   writer.writeUint32( mPacketLength );

   // Prepend header onto packets data
   packetWriter->getData()->insert( packetWriter->getData()->begin(),
                                   header_data.begin(),
                                   header_data.end() );

   packetWriter->setCurPos( getPacketLength() );

}

void Header::parseHeader(std::vector<vpr::Uint8>& headerData)
{
   vpr::BufferObjectReader reader( &headerData );

   // Parse the incoming data.
   mRIMCode = reader.readUint16();
   mPacketType = reader.readUint16();
   mFrame = reader.readUint32();
   mPacketLength = reader.readUint32();

   if ( RIM_PACKET != mRIMCode )
   {
      vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
         << clrOutBOLD( clrRED, "ERROR:" )
         << " This Packet is not a valid RIM Packet!!"
         << " RIMCode: " << mRIMCode << " is not valid!"
         << std::endl << vprDEBUG_FLUSH;
         
      throw cluster::ClusterException( "Header::parseHeader() - Invalid packet header!" );
   }
}

void Header::printData( const int debug_level ) const
{
   vprDEBUG_BEGIN( gadgetDBG_RIM, debug_level )
      << clrOutBOLD( clrYELLOW, "====== Packet Header ======" )
      << std::endl << vprDEBUG_FLUSH;
   
   vprDEBUG( gadgetDBG_RIM, debug_level )
      << clrOutBOLD( clrYELLOW, "RIMCode:    " ) << mRIMCode
      << std::endl << vprDEBUG_FLUSH;
   vprDEBUG( gadgetDBG_RIM, debug_level )
      << clrOutBOLD( clrYELLOW, "PacketType: " ) << mPacketType
      << std::endl << vprDEBUG_FLUSH;
   vprDEBUG( gadgetDBG_RIM, debug_level )
      << clrOutBOLD( clrYELLOW, "Frame #:    " ) << mFrame
      << std::endl << vprDEBUG_FLUSH;
   vprDEBUG( gadgetDBG_RIM, debug_level )
      << clrOutBOLD( clrYELLOW, "Length:     " ) << mPacketLength
      << std::endl << vprDEBUG_FLUSH;

   vprDEBUG_END( gadgetDBG_RIM, debug_level )
      << clrOutBOLD( clrYELLOW, "===========================" )
      << std::endl << vprDEBUG_FLUSH;
}

} // end cluster namespace
