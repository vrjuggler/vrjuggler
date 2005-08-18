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

//#include <gadget/gadgetConfig.h>
#include <cluster/Packets/Header.h>
#include <gadget/Util/Debug.h>

#include <vpr/IO/Socket/SocketStream.h>

namespace cluster
{  
   Header::Header( vpr::Uint16 RIM_code, vpr::Uint16 packet_type,
                   vpr::Uint32 packet_length, vpr::Uint32 frame ) : mPacketReader(NULL), mPacketWriter(NULL)
   {
      mRIMCode = RIM_code;
      mPacketType = packet_type;
      mPacketLength = packet_length;
      mFrame = frame;
   }
   
   Header::Header( vpr::SocketStream* stream ) throw ( cluster::ClusterException ) : mPacketReader(NULL), mPacketWriter(NULL)
   {
      vprASSERT( NULL != stream && "Can not create a Header using a NULL SocketStream" );

      // - Is stream is a valid SocketStream?
      //   - Read in the packet from the socket
      //   - Set the BufferObjectReader and BufferObjectWriter to use mData  <====We only need BufferObjectReader
      if ( NULL != stream )
      {
         vpr::Uint32 bytes_read;   
         
         try
         {
            stream->readn( mData, Header::RIM_PACKET_HEAD_SIZE, bytes_read );
         }
         catch (vpr::IOException& ex)
         {
            stream->close();
            delete stream;
            stream = NULL;
            throw cluster::ClusterException( "Header::Header() - Could not read the header!" );
         }

         if (bytes_read != RIM_PACKET_HEAD_SIZE )
         {
            stream->close();
            delete stream;
            stream = NULL;
            
            throw cluster::ClusterException( "Header::Header() - Bytes Read was lower than RIM_PACKET_HEAD_SIZE" );
         }
         
         if ( bytes_read != RIM_PACKET_HEAD_SIZE )
         {
            vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
               << clrOutBOLD( clrRED, "ERROR:" )
               << "Header::Header Something is seriously wrong here!\n" 
               << "We only read: " << bytes_read << " bytes for the header\n"
               << "When we need " << RIM_PACKET_HEAD_SIZE << " bytes!" << std::endl
               << vprDEBUG_FLUSH;
               exit(0);
         }
         
         mPacketReader = new vpr::BufferObjectReader( &mData );
         parseHeader();
      }
      else
      {
         vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
            << clrOutBOLD( clrRED, "ERROR:" )
            << " SocketSteam is NULL"
            << std::endl << vprDEBUG_FLUSH;

         throw std::exception();
      }
   }

   void Header::serializeHeader()
   {  
      mPacketWriter = new vpr::BufferObjectWriter(&mData);
      mPacketWriter->getData()->clear();
      mPacketWriter->setCurPos( 0 );

      // -Write all packet header information to the base Header class

         
//         std::cout << "Head Starting at Position: " << mPacketWriter->getCurPos() << std::endl;
      mPacketWriter->writeUint16( mRIMCode );
//         std::cout << "Write RIMCode: " << mRIMCode << std::endl;
//         std::cout << "Current Position: " << mPacketWriter->getCurPos() << std::endl;

      mPacketWriter->writeUint16( mPacketType );
//         std::cout << "Write PacketType: " << mPacketType << std::endl;
//         std::cout << "Current Position: " << mPacketWriter->getCurPos() << std::endl;

      mPacketWriter->writeUint32( mFrame );
//         std::cout << "Write Frame: " << mFrame << std::endl;
//         std::cout << "Current Position: " << mPacketWriter->getCurPos() << std::endl;

      mPacketWriter->writeUint32( mPacketLength );
//         std::cout << "Write Packet Length: " << mPacketLength << std::endl;
//         std::cout << "Current Position: " << mPacketWriter->getCurPos() << std::endl;
   }
   
   void Header::parseHeader()
   {
      // *Now that we have recieved the correct number 
      //  of bytes from the socket(RIM_PACKET_HEAD_SIZE)
      
      // -Parse the new data using a BufferObjectReader
      // -Is this a valid RIM packet?
      //  -If not exit immediately
         
         //std::cout << "Head Starting at Position: " << mPacketReader->getCurPos() << std::endl;
      
      mRIMCode = mPacketReader->readUint16();
//         std::cout << "Read RIMCode: " << mRIMCode << std::endl;
//         std::cout << "Current Position: " << mPacketReader->getCurPos() << std::endl;
      mPacketType = mPacketReader->readUint16();
//         std::cout << "Read PacketType: " << mPacketType << std::endl;
//         std::cout << "Current Position: " << mPacketReader->getCurPos() << std::endl;
      mFrame = mPacketReader->readUint32();
//         std::cout << "Read Frame#: " << mFrame << std::endl;
//         std::cout << "Current Position: " << mPacketReader->getCurPos() << std::endl;
      mPacketLength = mPacketReader->readUint32();
//         std::cout << "Read Packet Length: " << mPacketLength << std::endl;
//         std::cout << "Current Position: " << mPacketReader->getCurPos() << std::endl;

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
   
   void Header::send( vpr::SocketStream* socket ) throw (cluster::ClusterException)
   {
      vprASSERT( NULL != socket && "Socket is NULL" );

      // -Send the data in this packet
      vpr::Uint32 bytes_written;
      
      try
      {
         socket->send( mData, RIM_PACKET_HEAD_SIZE, bytes_written );
      }
      catch (vpr::IOException& ex)
      {
         // TODO: setCause(ex)
         throw cluster::ClusterException( "Header::send() - failed to send header." );
      }
   }
   
   void Header::dump()
   {
      std::cout << "Dumping Header(" << mData.size() << " bytes): ";
      for ( std::vector<vpr::Uint8>::iterator i = mData.begin();
           i!= mData.end(); i++ )
      {
         std::cout << (int)*i << " ";
      }
      std::cout << std::endl;
   }
   
   void Header::printData( const int debug_level )
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

}// end namespace cluster
