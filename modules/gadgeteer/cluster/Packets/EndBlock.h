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

#ifndef _GADGET_RIM_END_BLOCK_H
#define _GADGET_RIM_END_BLOCK_H

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <gadget/gadgetConfig.h>
#include <cluster/Packets/Header.h>                                                       
#include <cluster/Packets/Packet.h>
#include <cluster/ClusterNetwork/ClusterNode.h>
                                                                                                          
//#define RIM_PACKET_HEAD_SIZE 8

namespace cluster
{

class EndBlock : public Packet
{
public:
   /**
    * packet_head: Given a packet that has been parsed, and found to be a device request
    * stream: A socket that the connection is on
    * 
    * Create a deviceRequest packet
    */
   EndBlock(Header* packet_head, vpr::SocketStream* stream)
   {
      // -Copy over pointer to header
      // -Continue reading packet from socket

      mHeader = packet_head;
      
      //recv(packet_head,stream);
      //parse();
   }


   /**
    * Given a sender ID(self) and a requested device name
    *
    * Create a device request to be sent
    */
   EndBlock(vpr::Uint32 frame_number)
   {
      // Given the input, create the packet and then serialize the packet(which includes the header)
      // - Set member variables
      // - Create the correct packet header
      // - Serialize the packet
   
      // Header vars (Create Header)
      mHeader = new Header(Header::RIM_PACKET,
                                       Header::RIM_END_BLOCK,
                                       Header::RIM_PACKET_HEAD_SIZE
                                       /*+2 tempVar*/,frame_number);
      serialize();
   }

   
   /**
    * Helper for the above creation of a device request to be sent
    */
   void serialize()
   {
      mPacketWriter->getData()->clear();
      mPacketWriter->setCurPos(0);

      // Temp Var
      //mPacketWriter->writeUint16(mTempVar);

      // Create the header information
      mHeader->serializeHeader();
   }

   /**
    * After reading in the remaining bytes from the socket, create a new parse the data
    */
   void parse()
   {
      // Temp Var
      //mTempVar = mPacketReader->readUint16();
   }
   
   virtual void printData(int debug_level)
   {
/*      vprDEBUG_BEGIN(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
         <<  clrOutBOLD(clrYELLOW,"====== END BLOCK ======\n") << vprDEBUG_FLUSH;
      
      Packet::printData(debug_level);
      
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "New State:    ") << mNewState
         << std::endl << vprDEBUG_FLUSH;

      vprDEBUG_END(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
         <<  clrOutBOLD(clrYELLOW,"=======================\n") << vprDEBUG_FLUSH;
*/         
   }
   static vpr::Uint16 getBaseType()
   {
       return(Header::RIM_END_BLOCK);
   }
      
   virtual bool action(ClusterNode* node)
   {
      // -Set New State
      if (node == NULL)
      {
         return false;
      }

      //node->setState(mNewState);
      node->setUpdated(true);
      
      return true;
   }
private:
   //vpr::Uint16    mTempVar;
};
}

#endif




