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

//#include <gadget/gadgetConfig.h>
#include <gadget/Util/Debug.h>

#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <cluster/Packets/Header.h>
#include <cluster/Packets/Packet.h>


namespace cluster
{
   Packet::Packet() : mPluginId()
   {
      //mData = new std::vector<vpr::Uint8>(RIM_PACKET_HEAD_SIZE);
      mPacketReader = new vpr::BufferObjectReader(&mData);
      mPacketWriter= new vpr::BufferObjectWriter(&mData);
   }
   Packet::Packet(std::vector<vpr::Uint8>* data) : mPluginId()
   {
      //We must Copy
      mData = *(data);
      mPacketReader = new vpr::BufferObjectReader(&mData);
      mPacketWriter= new vpr::BufferObjectWriter(&mData);

      //parseHeader();
   }
   
   Packet::~Packet()
   {
      delete mHeader;
      delete mPacketReader;
	  delete mPacketWriter;
	  //delete mData;
   }

   vpr::Uint16 Packet::getPacketType() 
   { 
      return mHeader->getPacketType(); 
   }

   void Packet::recv(Header* packet_head, vpr::SocketStream* stream) throw(cluster::ClusterException)
   {
      // -Copy over pointer to header
      // -Continue reading packet from socket
      
      mHeader = packet_head;
        // We need to test if packetLength is greater than HEAD_SIZE since we might not 
        // actually have to read anything. And if we try to read 0 bytes, we get an exception
      if (stream != NULL)
      {
         vpr::Uint32 bytes_read;	  
         vpr::ReturnStatus status = stream->recvn(mData,mHeader->getPacketLength()-Header::RIM_PACKET_HEAD_SIZE,bytes_read);	  
         if (!status.success())
         {
            stream->close();
            delete stream;
            stream = NULL;
            throw cluster::ClusterException("Packet::recv() - Reading packet data failed!");
         }
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrSetBOLD(clrRED) 
            << "ERROR: SocketSteam is NULL\n" << clrRESET << vprDEBUG_FLUSH;
         throw cluster::ClusterException("Packet::recv() - SocketStream is NULL!");
      }
   }
   
   void Packet::dump()
   {
      if (mHeader != NULL)
      {
         mHeader->dump();
      }
      else
      {
         std::cout << "Could not dump Header since it is NULL!" << std::endl;
      }
      std::cout << "Dumping Packet(" << mData.size() << " bytes): ";
      for ( std::vector<vpr::Uint8>::iterator i = mData.begin();
           i!= mData.end(); i++ )
      {
         std::cout << (int)*i << " ";
      }
      std::cout << std::endl;
   }
   
   void Packet::printData(int debug_level)
   {
      if (mHeader != NULL)
      {
         mHeader->printData(debug_level);
         /*
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
            << clrOutBOLD(clrCYAN,"\n====== Packet Header ======")
            << "\nRIMCode:    " << mHeader->getRIMCode()
            << "\nPacketType: " << mHeader->getPacketType()
            << "\nFrame #:    " << mHeader->getFrame()
            << "\nLength:     " << mHeader->getPacketLength() << std::endl
            << vprDEBUG_FLUSH;               
         */            
      }
      else
      {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
         << clrOutBOLD(clrRED,"[Packet::printData] Header is still NULL, can not print data!")
         << vprDEBUG_FLUSH;               
      }
   }

}   // end namespace gadget
