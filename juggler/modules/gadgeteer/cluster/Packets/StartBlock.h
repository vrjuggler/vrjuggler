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
 ****************** <VPR heading Start do not edit this line> ******************/

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
 *************** <auto-copyright.pl Start do not edit this line> ***************/

#ifndef _GADGET_RIM_START_BLOCK_H
#define _GADGET_RIM_START_BLOCK_H

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <gadget/gadgetConfig.h>
#include <gadget/Util/Debug.h>
#include <cluster/Packets/Header.h>                                                       
#include <cluster/Packets/Packet.h>
#include <cluster/ClusterNetwork/ClusterNode.h>
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterManager.h>
                                                                                                          
//#define RIM_PACKET_HEAD_SIZE 8

namespace cluster
{

class StartBlock : public Packet
{
public:
   /**
    * packet_head: Given a packet that has been parsed, and found to be a device request
    * stream: A socket that the connection is on
    * 
    * Create a deviceRequest packet
    */
   StartBlock(Header* packet_head, vpr::SocketStream* stream)
   {
      // -Copy over pointer to header
      // -Continue reading packet from socket

      mHeader = packet_head;
      
      //recv(packet_head,stream);
      //parse();
   }


   /**
    * Given a sStarter ID(self) and a requested device name
    *
    * Create a device request to be sent
    */
   StartBlock(vpr::Uint32 frame_number)
   {
      // Given the input, create the packet and then serialize the packet(which includes the header)
      // - Set member variables
      // - Create the correct packet header
      // - Serialize the packet
   
      // Header vars (Create Header)
      mHeader = new Header(Header::RIM_PACKET,
                                       Header::RIM_START_BLOCK,
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
         <<  clrOutBOLD(clrYELLOW,"====== Start BLOCK ======\n") << vprDEBUG_FLUSH;
      
      Packet::printData(debug_level);
      
      vprDEBUG(gadgetDBG_RIM,debug_level) 
         << clrOutBOLD(clrYELLOW, "New State:    ") << mNewState
         << std::Startl << vprDEBUG_FLUSH;

      vprDEBUG_Start(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
         <<  clrOutBOLD(clrYELLOW,"=======================\n") << vprDEBUG_FLUSH;
*/         
   }
   static vpr::Uint16 getBaseType()
   {
       return(Header::RIM_START_BLOCK);
   }
      
   virtual bool action(ClusterNode* node)
   {
      // On Barrier Recv
      // -If Master
      //   -Remove Pending slave
      //   -If all recved
      //     -Send responce to all nodes
      //     -Set Running TRUE
      // -Else
      //   -Set Running TRUE

      if (node == NULL)
      {
         return false;
      }
      if (ClusterManager::instance()->isBarrierMaster())
      {
         ClusterManager::instance()->removePendingBarrierSlave(node->getHostname());
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[XXX] ")
            << "StartBlock::action() - Barrier Master!\n" << vprDEBUG_FLUSH;         
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[XXX] ")
            << "StartBlock::Action() - Slave\n" << vprDEBUG_FLUSH;         
         ClusterManager::instance()->setRunning(true);
      }
      return true;
   }
private:
   //vpr::Uint16    mTempVar;
};
}

#endif




